#include <iostream>
#include "openssl/ssl.h"
#include "socket.h"

class SSLSocket : public Socket {
public:
    SSLSocket(std::string addr, int port = 443, std::string out_path = "2333.html");
    ~SSLSocket();

    virtual int sendl(std::string content);
    virtual int recvl();
private:
    const SSL_METHOD *ssl_method_;
    SSL_CTX *ctx_;
    SSL *ssl_;
};