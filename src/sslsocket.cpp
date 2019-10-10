#include <errno.h>
#include "sslsocket.h"

SSLSocket::SSLSocket(std::string addr, int port) : 
    Socket(addr, port) {
    //添加SSL的加密/HASH算法 
	SSLeay_add_ssl_algorithms();
    //客户端，服务端选择SSLv23_server_method() 
	ssl_method_ = SSLv23_client_method();
    //建立新的SSL上下文 
	ctx_ = SSL_CTX_new(ssl_method_);
    // TCP连接
    int flag = ::connect(fd_, (struct sockaddr*)&serv_addr_, sizeof(serv_addr_));
    if (flag == 0) 
        std::cout << "TCPConnect success!" << std::endl; 
    else {
        std::cerr << "TCPConnect failed!(" << flag << ")" << std::endl;
        exit(1);
    }
    // 创建ssl
    ssl_ = SSL_new(ctx_);
    //将SSL与TCP SOCKET 连接 
	SSL_set_fd(ssl_ , fd_);
	//SSL连接 
    flag = SSL_connect(ssl_);
    if(flag == -1) {
        std::cerr << "SSL ACCEPT error ";
        exit(1);
    }
    std::cout << "SSLConnect success!" << std::endl;
}

SSLSocket::~SSLSocket() {
    //关闭SSL套接字 
	SSL_shutdown(ssl_);
	//释放SSL套接字 
	SSL_free(ssl_);
	//释放SSL会话环境 
	SSL_CTX_free(ctx_);
}

int SSLSocket::read_buff(char *buff, const int read_len) {
    ssize_t tlen = ::SSL_read(ssl_, buff, read_len);
    return tlen;
}

int SSLSocket::write_buff(const char *left, int remaining) {
    ssize_t w_len = ::SSL_write(ssl_, left, static_cast<size_t>(remaining));
    return w_len;
}
