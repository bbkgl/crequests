#include <iostream>
#include <errno.h>
#include <vector>
#include "socket.h"
#include "utils.h"

void change_chunk(std::string &str) {
    // 首先找出所有"\r\n****\r\n"的位置
    int _r1 = -1, _r2 = -1, _n1 = -1, _n2 = -1;
    std::vector<int> pairs;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '\r') {
            if (_r1 == -1) 
                _r1 = i;
            else if (_n1 != -1)
                _r2 = i;
        } else if (_r1 != -1 && str[i] == '\n') {
            if (_n1 == -1) 
                _n1 = i;
            else if (_r2 != -1) {
                _n2 = i;
                if (1 < _r2 - _n1 && _r2 - _n1 < 10) {
                    bool flag = true;
                    for (int j = _n1 + 1; j < _r2; j++) {
                        if (!isalnum(str[j])) {
                            flag = false;
                            break;
                        }
                    }
                    if (flag) {
                        pairs.push_back(_r1);
                        pairs.push_back(_n2);
                        _r1 = -1;
                        _r2 = -1;
                        _n1 = -1;
                        _n2 = -1;
                    } else {
                        _r1 = _r2;
                        _n1 = _n2;
                        _r2 = -1;
                        _n2 = -1;
                    }
                } else {
                    _r1 = _r2;
                    _n1 = _n2;
                    _r2 = -1;
                    _n2 = -1;
                }
            }
        }
    }
    // DEBUG
    // for (int i = 0; i < pairs.size(); i += 2) {
    //     int left = pairs[i], right = pairs[i + 1];
    //     printf("(%d-%d)", pairs[i], pairs[i + 1]);
    //     std::cout << str.substr(left, right - left + 1) << std::endl;
    // }
    
    // 将所有的"\r\n****\r\n"去除
    int count = 0;
    for (int i = 1; i < pairs.size(); i += 2) {
        int cur_left = pairs[i - 1];
        int cur_right = pairs[i];
        if (i < pairs.size() - 1) {
            int next_left = pairs[i + 1];
            int offset = count + (cur_right - cur_left) + 1;
            for (int j = cur_right + 1; j < next_left; j++)
                str[j - offset] = str[j];
            count += (cur_right - cur_left) + 1;
        } else {
            count += (str.length() - cur_left);
        }
    }
    str.erase(str.length() - count, count);
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

Socket::Socket(std::string addr, int port, int timeout, bool ishead) :
    debug_txt_("debug.txt"),
    port_(port),
    timeout_(timeout),
    ishead_(ishead) {
    // 申请socket
    fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bzero(&serv_addr_, sizeof serv_addr_);
    serv_addr_.sin_family = AF_INET;  //使用IPv4地址
    serv_addr_.sin_addr.s_addr = inet_addr(addr.c_str());  //具体的IP地址
    serv_addr_.sin_port = htons(port_);  //端口
}

Socket::~Socket() {
    close(fd_);
    debug_txt_.close();
}

int Socket::sendl(std::string content) {
    int left = 0;
    int remaining = content.size();
    while (remaining > 0) {
        ssize_t tlen = write_buff(content.data() + left, remaining);
        if (tlen >= 0)
            remaining -= tlen;
        else {
            usleep(1000);
        }
        left += tlen;
    }
    return 0;
}

int Socket::recvl() {
    char buff[BUFF_SIZE];
    int tlen = 1;
    int header_len = -1, body_len = -1;
    tlen = read_buff(buff, BUFF_SIZE);
    // 判断是否chunk编码并且找出响应头部长度
    body_len = find_len(std::string(buff, buff + tlen), header_len);
    head_ = std::string(buff, buff + header_len);
    debug_txt_ << head_ << std::endl;
    body_ = std::string(buff + header_len + 2, buff + tlen);
    // 如果是HEAD方法，直接返回
    if (ishead_) {
        body_ = "";
        return 0;
    }
    // 如果是chunk编码控制长度，则最后会出现“\r\n0\r\n\r\n”，以此判断结尾即可
    if (chunked_) {
        while (tlen) {
            tlen = read_buff(buff, BUFF_SIZE);
            // 如果出现了超时
            if (bbkgl::error_num != 0) return -1;
            // HTTP1.0短连接直接关闭，长连接分情况讨论
            if (tlen == 0) break;
            else {
                std::string tail;
                int left = 0, right = -1;
                if (body_.length() > 5) {
                    tail += body_.substr(body_.length() - 5, 5);
                    left = 5;
                }
                else {
                    tail += body_;
                    left = body_.length();
                }
                tail += std::string(buff, buff + tlen);
                if ((right = tail.find("0\r\n\r\n")) != std::string::npos) {
                    body_ += tail.substr(left, right + 5 - left + 1);
                    break;   
                } else body_ += tail.substr(left);
            }
        }
        // 将那些表示的chunk长度都删除掉
        debug_txt_ << body_;
        change_chunk(body_);
    } else if (body_len != -1) {
        body_len -= body_.length();
        while (body_len > 0) {
            tlen = read_buff(buff, BUFF_SIZE);
            // HTTP1.0短连接直接关闭，长连接分情况讨论
            if (tlen == 0) break;
            else {
                body_len -= tlen;
                body_ += std::string(buff, buff + tlen);
            }
        }
        // 这是因为处理chunk时需要chunk码前面有"\r\n"，而不使用chunk的时候我们需要去除
        if (*body_.begin() == '\r' || *body_.begin() == '\n') body_.erase(body_.begin());
        if (*body_.begin() == '\r' || *body_.begin() == '\n') body_.erase(body_.begin());
        debug_txt_ << body_;
    }
    return 0;
}