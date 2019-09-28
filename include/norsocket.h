#include <iostream>
#include "socket.h"

class NORSocket : public Socket {
public:
    NORSocket(std::string addr, int port, std::string out_path = "2333.html");
    ~NORSocket();

    virtual int sendl(std::string content);
    virtual int recvl();

private:

};