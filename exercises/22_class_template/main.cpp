#include "../exercise.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <cstdlib>


template<class T>
struct Tensor4D {
    unsigned int shape[4];  // 存储本张量的形状
    T *data;                // 存储本张量的数据

    // 构造函数（支持任意类型 U 的数组，通过 static_cast 转为 T）
    template <class U>
    Tensor4D(unsigned int const shape_[4], U const *data_) {
        unsigned int size = 1;
        for (int i = 0; i < 4; ++i) {
            shape[i] = shape_[i];
            size *= shape_[i];
        }
        data = new T[size];
        // 将每个元素从 U 转成 T
        for (unsigned int i = 0; i < size; ++i) {
            data[i] = static_cast<T>(data_[i]);
        }
    }

    ~Tensor4D() {
        delete[] data;
    }

    // 禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 单向广播加法: this += others
    Tensor4D &operator+=(Tensor4D const &others) {
        // 先检查维度是否满足广播规则
        for (int i = 0; i < 4; ++i) {
            // 如果 others.shape[i] != 1，且也 != this->shape[i]，则无法广播
            if (others.shape[i] != 1 && others.shape[i] != shape[i]) {
                throw std::runtime_error("Shape mismatch: cannot broadcast.");
            }
        }

        // 计算本张量的总大小
        unsigned int totalSize = shape[0] * shape[1] * shape[2] * shape[3];

        // 对每个元素进行广播加法
        for (unsigned int idx = 0; idx < totalSize; ++idx) {
            // 根据 idx 反推在 4D 空间的坐标 (i0, i1, i2, i3)
            unsigned int tmp = idx;
            unsigned int i0 = tmp / (shape[1] * shape[2] * shape[3]);
            tmp %= (shape[1] * shape[2] * shape[3]);

            unsigned int i1 = tmp / (shape[2] * shape[3]);
            tmp %= (shape[2] * shape[3]);

            unsigned int i2 = tmp / shape[3];
            unsigned int i3 = tmp % shape[3];

            // 对 others 同样推导 (j0, j1, j2, j3)，考虑维度=1的广播
            unsigned int j0 = (others.shape[0] == 1 ? 0 : i0);
            unsigned int j1 = (others.shape[1] == 1 ? 0 : i1);
            unsigned int j2 = (others.shape[2] == 1 ? 0 : i2);
            unsigned int j3 = (others.shape[3] == 1 ? 0 : i3);

            // 计算在 others.data 中的线性下标 offsetOther
            unsigned int offsetOther =
                j0 * (others.shape[1] * others.shape[2] * others.shape[3]) +
                j1 * (others.shape[2] * others.shape[3]) +
                j2 * others.shape[3] +
                j3;

            // 累加
            data[idx] += others.data[offsetOther];
        }

        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        // 这里 data 的类型是 int[]
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on

        // 显式指定 Tensor4D<float>，并将 int[] 转为 float
        auto t0 = Tensor4D<float>(shape, data);
        auto t1 = Tensor4D<float>(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        // 这里 d0 是 float[]
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on

        // 这里 d1 是 float[]
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D<float>(s0, d0);
        auto t1 = Tensor4D<float>(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        // 这里 d0 是 double[]
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on

        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        // 显式指定 Tensor4D<float>，并将 double[] 转为 float
        auto t0 = Tensor4D<float>(s0, d0);
        auto t1 = Tensor4D<float>(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }

    std::cout << "All tests passed!\n";
    return 0;
}