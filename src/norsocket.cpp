#include "norsocket.h"
#include "utils.h"

NORSocket::NORSocket(std::string addr, int port, int timeout) : 
    Socket(addr, port, timeout) {
    
}

NORSocket::~NORSocket() {
    
}


int NORSocket::read_buff(char *buff, const int read_len) {
    int flag = select(fd_ + 1, &rfds_, nullptr, nullptr, &time_out_);
    ssize_t tlen = -1;
    if (flag && FD_ISSET(fd_, &rfds_))
        ssize_t tlen = ::read(fd_, buff, read_len);
    return tlen;
}

int NORSocket::write_buff(const char *left, int remaining) {
    ssize_t w_len = ::write(fd_, left, static_cast<size_t>(remaining));
    return w_len;
}
