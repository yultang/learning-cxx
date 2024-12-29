#include "../exercise.h"

// READ: 有 cv 限定符的成员函数 <https://zh.cppreference.com/w/cpp/language/member_functions>

struct Fibonacci {
    int numbers[11];
    // TODO: 修改方法签名和实现，使测试通过

    /*
    constexpr Fibonacci(const int (&numbers)[11]) : numbers{numbers} {} {
        for(int i = 0; i < 11; i++) {
            numbers[i] = numbers[i];
        }
    }
    */

   /*
   编译器要求get方法再编译时可用, 即get方法必须是const的
   */

    constexpr int get(int i) const {
        return numbers[i];
    }
};

int main(int argc, char **argv) {
    Fibonacci constexpr FIB{{0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55}};
    ASSERT(FIB.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << FIB.get(10) << std::endl;
    return 0;
}
