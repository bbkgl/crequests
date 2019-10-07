#include <errno.h>
#include "sslsocket.h"

SSLSocket::SSLSocket(std::string addr, int port, std::string out_path) : 
    Socket(addr, port, out_path) {
    //添加SSL的加密/HASH算法 
	SSLeay_add_ssl_algorithms();
    //客户端，服务端选择SSLv23_server_method() 
	ssl_method_ = SSLv23_client_method();
    //建立新的SSL上下文 
	ctx_ = SSL_CTX_new(ssl_method_);
    // TCP连接
    while (0 != ::connect(fd_, (struct sockaddr*)&serv_addr_, sizeof(serv_addr_))) {
        if (errno == EINPROGRESS) {
            std::cout << "TCPConnect nonblock wait!" << std::endl; 
            usleep(1000);
            int so_error;
            socklen_t len = sizeof(so_error);
            getsockopt(fd_, SOL_SOCKET, SO_ERROR, &so_error, &len);
            if (so_error != 0) {
                std::cerr << "TCPConnect error!" << std::endl;
		        exit(1);
            } else break;
        } else {
            std::cerr << "TCPConnect error!" << std::endl;
		    exit(1);
        }
    }
    std::cout << "TCPConnect success!" << std::endl; 
    // 创建ssl
    ssl_ = SSL_new(ctx_);
    //将SSL与TCP SOCKET 连接 
	SSL_set_fd(ssl_ , fd_);
    SSL_set_connect_state(ssl_);
	//SSL连接 
    int flag = -1;
	while (-1 == SSL_connect(ssl_)) {
        int ssl_conn_err = SSL_get_error(ssl_, flag);
        if (SSL_ERROR_WANT_READ == ssl_conn_err || SSL_ERROR_WANT_WRITE == ssl_conn_err) {
            std::cout << "SSLConnect nonblock wait!" << std::endl;
            usleep(1000);
        } else {
            std::cerr << "SSL ACCEPT error " << std::endl;
            exit(1);
        }
	}
    std::cout << "SSLConnect nonblock success!" << std::endl;
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
    int err = SSL_get_error(ssl_, tlen);
    if (err == SSL_ERROR_NONE) {
        return tlen;
    } else if (err == SSL_ERROR_WANT_READ) {
        return -1;
    }
}

int SSLSocket::write_buff(const char *left, int remaining) {
    ssize_t w_len = ::SSL_write(ssl_, left, static_cast<size_t>(remaining));
    int err = SSL_get_error(ssl_, w_len);
    if (err == SSL_ERROR_NONE) {
        return w_len;
    } else if (err == SSL_ERROR_WANT_READ) {
        return -1;
    }
    return w_len;
}
