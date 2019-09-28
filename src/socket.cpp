#include <iostream>
#include <errno.h>
#include "socket.h"
#include "utils.h"

int Socket::find_len(std::string text, int &header_len) {
    int pos = text.find("Content-Length:");
    // 如果是chunked
    if (pos == std::string::npos) return -1;
    int left = pos + 15;
    int offset = 1;   // 计算偏移量
    for (; text[left + offset] != '\r'; offset++) ;
    // 找到头部长度
    header_len = text.find("\r\n\r\n");
    header_len = header_len == std::string::npos ? -1 : header_len + 4;
    return std::stoi(text.substr(left, offset));
}

Socket::Socket(std::string addr, int port, std::string out_path) :
    port_(port) {
    // 申请socket
    fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bzero(&serv_addr_, sizeof serv_addr_);
    serv_addr_.sin_family = AF_INET;  //使用IPv4地址
    serv_addr_.sin_addr.s_addr = inet_addr(addr.c_str());  //具体的IP地址
    serv_addr_.sin_port = htons(port_);  //端口

    // 输出到文件、
    out_html_.open(out_path);
}

Socket::~Socket() {
    close(fd_);
    out_html_.close();
}

