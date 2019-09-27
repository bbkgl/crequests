#include <iostream>
#include <errno.h>
#include "socket.h"
#include "utils.h"

int find_len(std::string text, int &header_len) {
    int pos = text.find("Content-Length:");
    // 如果是chunked
    if (pos == std::string::npos) return -1;
    int left = pos + 15;
    int offset = 1;   // 计算偏移量
    for (; text[left + offset] != '\r'; offset++) ;
    // 找到头部长度
    header_len = text.find("\r\n\r\n");
    header_len = header_len == std::string::npos ? -1 : header_len + 7;
    return std::stoi(text.substr(left, offset));
}

Socket::Socket(std::string addr, int port) :
    port_(port)
{
    // 申请socket
    fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bzero(&serv_addr_, sizeof serv_addr_);
    serv_addr_.sin_family = AF_INET;  //使用IPv4地址
    serv_addr_.sin_addr.s_addr = inet_addr(addr.c_str());  //具体的IP地址
    serv_addr_.sin_port = htons(port_);  //端口

    int flag = connect(fd_, (struct sockaddr*)&serv_addr_, sizeof(serv_addr_));
    if (flag == 0) 
        std::cout << "Connect success!" << std::endl; 
    else
        std::cerr << "Connect failed!(" << flag << ")" << std::endl;
}

Socket::~Socket() {
    close(fd_);
}

// 表示能发送很长的信息
int Socket::sendl(std::string content) {
    int left = 0;
    int remaining = content.size();
    while (remaining > 0) {
        ssize_t tlen = ::write(fd_, content.data() + left, static_cast<size_t>(remaining));
        if (tlen >= 0)
            remaining -= tlen;
        else
            return -1;
        left += tlen;
    }
    return 0;
}

// 表示能接收很长的信息
std::string Socket::recvl() {
    std::string recv_str;
    char buf[BUFF_SIZE];
    int tlen = 1, body_len = -1, header_len = 0;
    bool first = true;
    // 非阻塞，直到EAGIN
    while (tlen > 0) {
        tlen = ::read(fd_, buf, BUFF_SIZE);
        recv_str += buf;
        if (first) {
            body_len = find_len(buf, header_len);
            first = false;
            printf("header_len:%d, body_len:%d.\n", header_len, body_len);
        }
        if (recv_str.size() - header_len >= body_len) {
            while (recv_str.size() > body_len + header_len) recv_str.pop_back();
            break;
        }
    }
    printf("%d\n", recv_str.size());
    return recv_str;
}