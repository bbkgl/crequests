#ifndef SOCKET
#define SOCKET

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <fstream>

#define BUFF_SIZE 4096
#define CRLF "\r\n"

class Socket {
public:
    Socket(std::string addr, int port, std::string out_path);
    virtual ~Socket();
    int sendl(std::string content);
    int recvl();

    // 以下两个函数的功能是封装系统的IO函数
    virtual int read_buff(char *buff, const int read_len) = 0;
    virtual int write_buff(const char *left, int remaining) = 0;

    
    int find_len(std::string text, int &header_len);
    // 根据chunk的十进制长度进行长度控制，即一次性读取完一次chunk的内容
    std::string recv_chunked(char *buff, int chunk_d);
    
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
    bool chunked_;
};

#endif