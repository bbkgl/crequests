
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstdio>
#include <string>
#include <fcntl.h>

namespace bbkgl {
    // 这里注意参数是不能包括”http://“的
    std::string get_host(std::string name) {
        struct hostent *addr = nullptr;
        addr = gethostbyname(name.c_str());
        std::string host =
            std::string(inet_ntoa(*(struct in_addr *)addr->h_addr_list[0]));
        return host;
    }

    int set_nonblock(int fd) {
        int flag = fcntl(fd, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(fd, F_SETFL, flag);
        return flag;
    }
}  // namespace bbkgl


