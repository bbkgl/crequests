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

# 添加生成对象
file(GLOB_RECURSE SAMPLELIST RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/sample/" "${CMAKE_CURRENT_SOURCE_DIR}/sample/*.cpp")

foreach(curr_file ${SAMPLELIST})
    string( REPLACE ".cpp" "" program ${curr_file})
    # 将动态库链接到目标
    message(${program})
    add_executable(${program} ${CMAKE_CURRENT_SOURCE_DIR}/sample/${curr_file})
    target_link_libraries(${program} 
        libcrequests 
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libssl.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcrypto.so
    )
endforeach(curr_file)
