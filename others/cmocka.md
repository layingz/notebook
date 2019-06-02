# 介绍
cmocka是google开源的用于C单元测试的一个轻量级的框架, 相比其他c单元测试框架，优势在于 兼容性好 、耦合度低 、不依赖外部库，对测试代码进行mock，原理是在链接阶段将mock函数替换成自定义实现的函数，因此仅限于对自己写的函数进行mock，对于系统函数，无法实现自定义返回值

支持如下特性：

    提供跨平台支持
    文档完整详细
    无第三方库依赖
    非fork()执行
    提供对于信号的处理
    提供基本的抽象，如Test Fixture等，支持setup和teardown函数，集成mock
    支持多种格式输出(编译时指定)
    提供基本的内存检测，如内存泄露，buffer的上下溢检测

[官方文档地址](https://api.cmocka.org/)

测试代码头文件除了cmocka.h需要包含stdarg.h，stddef.h, setjmp.h，编译时候需要添加-lcmocka

cmocka除了提供mock功能还提供断言、检测内存泄漏、对函数返回期望值功能

# 使用

1、断言

    assert_int_equal(int a, int b)
    assert_int_not_equal(int a, int b)

    assert_ptr_equal(void* a, void* b)
    assert_ptr_not_equal(void* a, void* b)
    assert_null(void* p)
    assert_not_null(void* p)

    assert_memory_equal(const void* a, const void* b, size_t size)
    assert_memory_not_equal(const void* a, const void* b, size_t size)

    assert_string_equal(const char* a, const char* b)
    assert_string_not_equal(const char* a, const char* b)

    assert_return_code(int rc, int error)

2、设定返回值

    will_return(#func, LargestIntegralType val)
    will_return_always(#func, LargestIntegralType val)
    will_return_count(#func, LargestIntegralType val, int count)





