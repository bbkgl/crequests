#ifndef SOCKET
#define SOCKET

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <fstream>

#define BUFF_SIZE 4096

class Socket {
public:
    Socket(std::string addr, int port, std::string out_path);
    virtual ~Socket();
    virtual int sendl(std::string content) = 0;
    virtual int recvl() = 0;

    
    int find_len(std::string text, int &header_len);

    inline std::string get_head() { return head_; }
    inline std::string get_body() { return body_; } 

// 属性
protected:
    int port_;
    int fd_;
    std::string head_;
    std::string body_;
    struct sockaddr_in serv_addr_;
    std::ofstream out_html_;
};

#endif