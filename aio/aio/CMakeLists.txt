
include_directories(
    ${PROJECT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_BUILD_TYPE "Debug")

#add_definitions(-DCALLBACK_THREAD)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread -lrt")

SET(GCC_COVERAGE_COMPILE_FLAGS "-g")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

aux_source_directory(. DIR_SRCS)

#生产bin文件
add_executable(aio ${DIR_SRCS})

#输出到目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
