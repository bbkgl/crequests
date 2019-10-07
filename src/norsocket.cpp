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


int NORSocket::read_buff(char *buff, const int read_len) {
    ssize_t tlen = ::read(fd_, buff, read_len);
    return tlen;
}

int NORSocket::write_buff(const char *left, int remaining) {
    ssize_t w_len = ::write(fd_, left, static_cast<size_t>(remaining));
    return w_len;
}
