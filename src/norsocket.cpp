#include "norsocket.h"
#include "utils.h"

NORSocket::NORSocket(std::string addr, int port, int timeout) : 
    Socket(addr, port, timeout) {
    signal(SIGALRM, bbkgl::alarmhandle);
    alarm(timeout_);
    int flag = ::connect(fd_, (struct sockaddr*)&serv_addr_, sizeof(serv_addr_));
    alarm(0);
    if (flag == 0) 
        std::cout << "TCPConnect success!" << std::endl; 
    else
        std::cerr << "TCPConnect failed!(" << flag << ")" << std::endl;
}

NORSocket::~NORSocket() {
    
}


int NORSocket::read_buff(char *buff, const int read_len) {
    signal(SIGALRM, bbkgl::alarmhandle);
    alarm(timeout_);
    ssize_t tlen = ::read(fd_, buff, read_len);
    alarm(0);
    return tlen;
}

int NORSocket::write_buff(const char *left, int remaining) {
    ssize_t w_len = ::write(fd_, left, static_cast<size_t>(remaining));
    return w_len;
}
