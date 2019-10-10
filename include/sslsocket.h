#include <iostream>
#include "openssl/ssl.h"
#include "socket.h"

class SSLSocket : public Socket {
public:
    SSLSocket(std::string addr, int port = 443);
    ~SSLSocket();

    int read_buff(char *buff, const int read_len);
    int write_buff(const char *left, int remaining);

private:
    const SSL_METHOD *ssl_method_;
    SSL_CTX *ctx_;
    SSL *ssl_;
};