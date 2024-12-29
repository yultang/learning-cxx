#include "../exercise.h"

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

struct A {
    // TODO: 正确初始化静态字段(要在类外定义)
    static int num_a;

    // 要想通过基类指针 delete 派生类对象，需要虚析构
    A() {
        ++num_a;
    }
    virtual ~A() { // virtual
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};

struct B final : public A {
    // TODO: 正确初始化静态字段(要在类外定义)
    static int num_b;

    B() {
        ++num_b;
    }
    ~B() override {
        --num_b;
    }

    // 覆盖父类的虚函数
    char name() const final {
        return 'B';
    }
};

// 在类外部定义静态成员
int A::num_a = 0;
int B::num_b = 0;

int main(int argc, char **argv) {
    auto a = new A;
    auto b = new B;

    // new A => A::num_a 从 0 → 1
    // new B => 因为 B 继承了 A，所以会先构造基类 A::num_a 从 1 → 2，之后 B::num_b 从 0 → 1
    // 所以这里 A::num_a = 2, B::num_b = 1
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");

    // a->name() 调用 A::name()
    // b->name() 调用 B::name()
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    // delete a => A::num_a 从 2 → 1
    // delete b => 调用 ~B(), B::num_b 从 1 → 0，再调用 ~A(), A::num_a 从 1 → 0
    delete a;
    delete b;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    // 再次创建：A *ab = new B
    // 首先构造 A: A::num_a 从 0 → 1
    // 然后构造 B: B::num_b 从 0 → 1
    // 所以现在 A::num_a = 1, B::num_b = 1
    A *ab = new B;
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");

    // ab->name() 根据动态类型（B）调用 B::name() => 'B'
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    // 正确做法：dynamic_cast (C++ RTTI)
    B &bb = dynamic_cast<B&>(*ab);
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // 通过基类指针释放派生类对象，会先调用 ~B 再调用 ~A
    // => B::num_b 从 1 → 0
    // => A::num_a 从 1 → 0
    delete ab;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    std::cout << "All tests passed!\n";
    return 0;
}