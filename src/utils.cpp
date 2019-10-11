#include "utils.h"

namespace bbkgl {
    std::string tmp = "";

    void alarmhandle(int sig) {
        char error_info[100];
        sprintf(error_info, tmp.c_str(), sig);
        std::cerr << error_info << std::endl;
        exit(1);
    }

    std::string get_host(std::string name) {
        struct hostent *addr = nullptr;
        tmp = "Can't connect the DNS server, please check your url and network!(%d)";
        signal(SIGALRM, alarmhandle);
        alarm(5);
        addr = gethostbyname(name.c_str());
        // 如果是网络问题，这里会返回空指针
        if (!addr) {
            std::cerr << "Can't connect the DNS or server, please check your url and network!" << std::endl;
            exit(1);
        }
        std::string host = std::string(inet_ntoa(*(struct in_addr *)addr->h_addr_list[0]));
        alarm(0);
        return host;
    }

    int set_nonblock(int fd) {
        int flag = fcntl(fd, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(fd, F_SETFL, flag);
        return flag;
    }
}