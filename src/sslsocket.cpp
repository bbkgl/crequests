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
    int flag = ::connect(fd_, (struct sockaddr*)&serv_addr_, sizeof(serv_addr_));
    if (flag == 0) 
        std::cout << "TCPConnect success!" << std::endl; 
    else
        std::cerr << "TCPConnect failed!(" << flag << ")" << std::endl;
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
}

SSLSocket::~SSLSocket() {
    //关闭SSL套接字 
	SSL_shutdown(ssl_);
	//释放SSL套接字 
	SSL_free(ssl_);
	//释放SSL会话环境 
	SSL_CTX_free(ctx_);
}

int SSLSocket::sendl(std::string content) {
    int left = 0;
    int remaining = content.size();
    while (remaining > 0) {
        ssize_t tlen = ::SSL_write(ssl_, content.data() + left, static_cast<size_t>(remaining));
        if (tlen >= 0)
            remaining -= tlen;
        else
            return -1;
        left += tlen;
    }
    return 0;
}

int SSLSocket::recvl() {
    std::string recv_str;
    char buf[BUFF_SIZE];
    int tlen = 1, body_len = -1, head_len = 0;
    bool first = true;
    while (tlen > 0) {
        tlen = ::SSL_read(ssl_, buf, BUFF_SIZE);
        recv_str += buf;
        out_html_ << buf << std::endl;
        if (first) {
            body_len = find_len(buf, head_len);
            first = false;
            printf("header_len:%d, body_len:%d.\n", head_len, body_len);
            head_ += recv_str.substr(0, head_len);
            recv_str.erase(0, head_len);
        }
        if (recv_str.size() >= body_len)
            break;
        printf("%d\n", tlen);
    }
    printf("%d\n", recv_str.size());
    body_ = recv_str;
    return 0;
}
