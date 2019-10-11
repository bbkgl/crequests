#include <iostream>
#include "socket.h"

class NORSocket : public Socket {
public:
    NORSocket(std::string addr, int port, int timeout);
    ~NORSocket();

    int read_buff(char *buff, const int read_len);
    int write_buff(const char *left, int remaining);

private:

};