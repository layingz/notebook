
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread -laio")

aux_source_directory(. DIR_SRCS)

add_definitions(-DEPOLLEVENT)

#生产bin文件
add_executable(libaio ${DIR_SRCS})

#输出到目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
