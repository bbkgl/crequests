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

// 使用inline是避免生成库的时候被多次定义（本头文件被多次include）
namespace bbkgl {
    static std::string host = "";

    inline void alarmhandle(int sig) {
        if (host == "") {
            char tmp[40];
            sprintf(tmp, "Can't connect the DNS or server, please check your url and network!(%d)", sig);
            std::cerr << tmp << std::endl;
            exit(1);
        }
    }

    // 这里注意参数是不能包括”http://“的
    inline std::string get_host(std::string name) {
        struct hostent *addr = nullptr;
        signal(SIGALRM, alarmhandle);
        alarm(5);
        addr = gethostbyname(name.c_str());
        // 如果是网络问题，这里会返回空指针
        if (!addr) {
            std::cerr << "Can't connect the DNS or server, please check your url and network!" << std::endl;
            exit(1);
        }
        host = std::string(inet_ntoa(*(struct in_addr *)addr->h_addr_list[0]));
        alarm(0);
        return host;
    }

    inline int set_nonblock(int fd) {
        int flag = fcntl(fd, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(fd, F_SETFL, flag);
        return flag;
    }
}  // namespace bbkgl


#endif