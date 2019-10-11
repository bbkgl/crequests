#ifndef UTILS
#define UTILS

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstdio>
#include <string>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>

// 使用inline是避免生成库的时候被多次定义（本头文件被多次include）
namespace bbkgl {
    extern std::string tmp;
    
    // 错误处理函数
    void alarmhandle(int sig);

    // 这里注意参数是不能包括”http://“的
    std::string get_host(std::string name);

    int set_nonblock(int fd);
}  // namespace bbkgl


#endif