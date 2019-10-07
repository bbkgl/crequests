#include <iostream>
#include <errno.h>
#include "socket.h"
#include "utils.h"

void change(std::string &str) {
    int len = str.length();
    for (int i = 1; i < len; i++) {
        if (str[i] == '\n' && str[i-1] == '\r') {
            str[i] = 'n';
            str[i-1] = '&';
        }
    }
}

std::string Socket::recv_chunked(char *buff, int chunk_d) {
    int tlen = -1;
    std::string content;
    while (chunk_d > 0) {
        tlen = std::min(BUFF_SIZE, chunk_d);
        tlen = read_buff(buff, tlen);
        content += std::string(buff, buff + tlen);
        chunk_d -= tlen;
    }
    return content;
}

int Socket::find_len(std::string text, int &header_len) {
    int pos = text.find("Content-Length:");
    // 找到头部长度
    header_len = text.find("\r\n\r\n");
    header_len = header_len == std::string::npos ? -1 : header_len;
    // 判断是不是chunked编码
    if (pos == std::string::npos || text.find("Transfer-Encoding: chunked") != std::string::npos) {
        chunked_ = true;
        return -1;
    } else chunked_ = false;
    int left = pos + 15;
    int offset = 1;   // 计算偏移量
    for (; text[left + offset] != '\r'; offset++) ;
    return std::stoi(text.substr(left, offset));
}

Socket::Socket(std::string addr, int port, std::string out_path) :
    port_(port) {
    // 申请socket
    fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bbkgl::set_nonblock(fd_);
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

int Socket::sendl(std::string content) {
    int left = 0;
    int remaining = content.size();
    while (remaining > 0) {
        ssize_t tlen = write_buff(content.data() + left, remaining);
        if (tlen >= 0)
            remaining -= tlen;
        else {
            usleep(10000);
        }
        left += tlen;
    }
    return 0;
}

int Socket::recvl() {
    std::string recv_str;
    char buff[BUFF_SIZE];
    int tlen = 1;
    bool flag = false;
    sleep(1);
    while (tlen) {
        tlen = read_buff(buff, BUFF_SIZE);
        if (tlen == 0) break;
        else if (tlen == -1) {
            usleep(100 * ONEMMSECOND);
            // 连续两次-1就退出
            if (flag) {
                std::cout << "2333" << std::endl;
                break;
            }
            flag = true;
        } else {
            flag = false;
            out_html_ << buff << std::endl;
            recv_str += std::string(buff, buff + tlen);
            if (std::string(buff, buff + tlen).find("0\r\n\r\n") != std::string::npos)
                break;
        }
    }
    return 0;
}