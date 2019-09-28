#include "norsocket.h"

NORSocket::NORSocket(std::string addr, int port, std::string out_path) : 
    Socket(addr, port, out_path) {
    int flag = ::connect(fd_, (struct sockaddr*)&serv_addr_, sizeof(serv_addr_));
    if (flag == 0) 
        std::cout << "TCPConnect success!" << std::endl; 
    else
        std::cerr << "TCPConnect failed!(" << flag << ")" << std::endl;
}

NORSocket::~NORSocket() {
    
}

// 表示能发送很长的信息
int NORSocket::sendl(std::string content) {
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
int NORSocket::recvl() {
    std::string recv_str;
    char buf[BUFF_SIZE];
    int tlen = 1, body_len = -1, head_len = 0;
    bool first = true;
    while (tlen > 0) {
        tlen = ::read(fd_, buf, BUFF_SIZE);
        recv_str += buf;
        if (first) {
            body_len = find_len(buf, head_len);
            first = false;
            printf("header_len:%d, body_len:%d.\n", head_len, body_len);
            head_ += recv_str.substr(0, head_len);
            recv_str.erase(0, head_len);
        }
        if (recv_str.size() >= body_len)
            break;
        printf("%d\n", tlen);
    }
    printf("%d\n", recv_str.size());
    body_ = recv_str;
    out_html_ << body_;
    return 0;
}