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

#define BBKGLOK 0
#define TIMEOUTERROR 999
#define DNSERROR 1
#define TCPERROR 2
#define SSLERROR 3

// 使用inline是避免生成库的时候被多次定义（本头文件被多次include）
namespace bbkgl {
    
    extern std::string tmp;
    extern int error_num;
    
    // 错误处理函数
    void alarmhandle(int sig);

    // 这里注意参数是不能包括”http://“的
    std::string get_host(std::string name);

    int set_nonblock(int fd);
    int set_block(int fd);
}  // namespace bbkgl


#endif