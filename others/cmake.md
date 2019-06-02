介绍

    cmake是跨平台编译工具,包含cpack，一般每层目录都有一个CMakeLists.txt文件

变量

    CMAKE_C_COMPILER：指定C编译器
    CMAKE_CXX_COMPILER：指定C++编译器
    MAKE_C_FLAGS：编译C文件时的选项，如-g；也可以通过add_definitions添加编译选项
    EXECUTABLE_OUTPUT_PATH：可执行文件的存放路径
    LIBRARY_OUTPUT_PATH：库文件路径
    CMAKE_BUILD_TYPE:：build 类型(Debug, Release, ...)

    CMAKE_CURRENT_LIST_FILE：输出调用该变量的CMakeLists.txt的完整路径
    CMAKE_CURRENT_LIST_LINE：输出该变量所在的行
    CMAKE_MODULE_PATH：定义自己的cmake模块所在路径
    CMAKE_CURRENT_BINARY_DIR：target编译目录
    PROJECT_SOURCE_DIR：工程根目录；
    PROJECT_BINARY_DIR：运行cmake命令的目录
    CMAKE_INCLUDE_PATH：环境变量，非cmake变量
    CMAKE_LIBRARY_PATH：环境变量
    CMAKE_CURRENT_SOURCE_DIR：当前处理的CMakeLists.txt文件所在路径

    BUILD_SHARED_LIBS 控制默认的库编译方式，如果未进行设置,使用ADD_LIBRARY时又没有指定库类型,默认编译生成的库都是静态库

    CMAKE_INSTALL_PREFIX    安装路径前缀 


基础语法

    #最低版本号
    cmake_minimum_required 
    #项目名称
    project()
    #设置字段
    set()
    #编译选项
    add_definitions()
    #头文件
    INCLUDE_DIRECTORIES()
    #编译源文件
    AUX_SOURCE_DIRECTORY()
    #添加子项目
    add_subdirectory()
    #输出库文件到
    ADD_LIBRARY()
        add_library(archive SHARED archive.cpp zip.cpp lzma.cpp)
        SHARED  动态链接库
        STATIC  静态链接库
        MODULE  插件
    #输出程序到文件
    ADD_EXECUTABLE()
    #添加链接动态库
    TARGET_LINK_LIBRARIES()
    #打印信息
    message([SEND_ERROR | STATUS | FATAL_ERROR] "message to display" ...)
    #链接外部库搜索路径
    LINK_DIRECTORIES(directory1 directory2 ...)
    #安装
    INSTALL(TARGETS targets... [[ARCHIVE|LIBRARY|RUNTIME] [DESTINATION < dir >] [PERMISSIONS permissions...] [CONFIGURATIONS [Debug|Release|...]] [COMPONENT < component >] [OPTIONAL] ] [...])

语句

    IF
        IF(DEBUG_mode)
            add_definitions(-DDEBUG)
        ENDIF()
        IF (var1 AND var2)
        IF (var1 OR var2)
        IF (COMMAND cmd) 如果cmd确实是命令并可调用，为真
        IF (EXISTS dir) 如果目录存在，为真
        IF (EXISTS file) 如果文件存在，为真
        IF (IS_DIRECTORY dir) 当dir是目录时，为真
        IF (DEFINED var) 如果变量被定义，为真
        IF (var MATCHES regex) 此处var可以用var名，也可以用${var}
        IF (string MATCHES regex) 当给定变量或字符串能匹配正则表达式regex时，为真         
        IF (var1 STRLESS var2)
        IF (var1 STRGREATER var2)
        IF (var1 STREQUAL var2)

    WHILE
        WHILE(condition)
            COMMAND1(ARGS ...)
            COMMAND2(ARGS ...)
            ...
        ENDWHILE(condition)
    
    FOREACH
        FOREACH(loop_var arg1 arg2 ...)
            COMMAND1(ARGS ...)
            COMMAND2(ARGS ...)
        ...
        ENDFOREACH(loop_var)
        example：
        FOREACH(F ${SRC_LIST})
            MESSAGE(${F})
        ENDFOREACH(F