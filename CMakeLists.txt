cmake_minimum_required(VERSION 3.9)
project(crequests)
set(CMAKE_CXX_STANDARD 11)
add_definitions(-g)

# 加入所有头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/base)

# 添加所有源文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRCLIST)

# message(${SRCLIST})

# 将源文件打包成动态库
add_library(libcrequests SHARED ${SRCLIST})

add_executable(main sample/test.cpp)

LINK_DIRECTORIES()

# 将动态库链接到目标
target_link_libraries(main 
    libcrequests 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libssl.so
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcrypto.so)