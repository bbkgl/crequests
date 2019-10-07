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
        else
            return -1;
        left += tlen;
    }
    return 0;
}

int Socket::recvl() {
    char buff[BUFF_SIZE];
    int tlen = 1, body_len = -1, head_len = 0, chunk_num = 0;
    
    // 读取响应头部，尽量多读取
    tlen = read_buff(buff, BUFF_SIZE);
    // 读取得到头部长度和头部内容
    body_len = find_len(buff, head_len);
    head_ = std::string(buff, buff + head_len);

    // 如果是chunked编码控制长度
    if (chunked_) {
        // 得到chunk编码数字
        int chunk_end = head_len + 4;
        for (; buff[chunk_end] != '\n'; chunk_end++) ;
        // chunk_end这时候应该是chunked编码数后面的第一个'\n'
        sscanf(std::string(buff).substr(head_len, chunk_end - head_len).c_str(), "%x", &chunk_num);
        // 修改chunk_num表示接下来要接收的长度
        out_html_ << chunk_num << std::endl;
        // 获得当前接收的所有内容
        body_ = std::string(buff).substr(chunk_end + 1, tlen);
        chunk_num -= body_.length();

        while (chunk_num > 0) {
            tlen = read_buff(buff, BUFF_SIZE);
            std::string temp = std::string(buff, buff + tlen);
            change(temp);
            out_html_ << temp;
            body_ += temp;
        }
    }

    return 0;
}