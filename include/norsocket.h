#include <iostream>
#include "socket.h"

class NORSocket : public Socket {
public:
    NORSocket(std::string addr, int port, std::string out_path = "2333.html");
    ~NORSocket();

    int read_buff(char *buff, const int read_len);
    int write_buff(const char *left, int remaining);

private:

};