#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>

#define BUFF_SIZE 4096

class Socket {
public:
    Socket(std::string addr, int port = 80);
    ~Socket();
    int sendl(std::string content);
    std::string recvl();

private:
    int port_;
    int fd_;
    struct sockaddr_in serv_addr_;
};