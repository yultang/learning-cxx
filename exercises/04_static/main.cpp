#include "../exercise.h"

// READ: `static` 关键字 <https://zh.cppreference.com/w/cpp/language/storage_duration>
// THINK: 这个函数的两个 `static` 各自的作用是什么？
static int func(int param) {
    static int static_ = param;
    // std::cout << "static_ = " << static_ << std::endl;
    return static_++;
}

int main(int argc, char **argv) {
    // TODO: 将下列 `?` 替换为正确的数字
    ASSERT(func(5) == 5, "static variable value incorrect");
    ASSERT(func(4) == 6, "static variable value incorrect");
    ASSERT(func(3) == 7, "static variable value incorrect");
    ASSERT(func(2) == 8, "static variable value incorrect");
    ASSERT(func(1) == 9, "static variable value incorrect");
    return 0;
}

/*
1. 函数被定义为 static:
链接属性为 “内部链接(internal linkage)”,
函数只能在定义他的源文件内被使用;

2. 变量被定义为 static:
生命周期从程序首次进入其声明开始直到程序结束;
变量只在第一次执行到声明语句时初始化(仅一次), 保留其值后续不再初始化.
*/
