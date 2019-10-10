#include <sstream>
#include "request.h"
#include "utils.h"

Request::Request(std::string url, METD method, std::map<std::string, std::string>  headers) :
    method_(method),
    url_(url), 
    headers_(headers) {
    // 首先判断协议类型
    int host_left = url_.find("://");
    if (host_left != std::string::npos) {
        std::string scheme_str = url_.substr(0, host_left);
        if (scheme_str == "http")
            scheme_ = HTTP;
        else if (scheme_str == "https")
            scheme_ = HTTPS;
        else {
            std::cerr << "Please input the right scheme(http, https)!!!" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Please input the right scheme(http, https)!!!" << std::endl;
        exit(1);
    }
    // 从host的起始位置开始找
    host_left += 3;
    int path_left = url_.find("/", host_left);
    if (path_left != std::string::npos) {
        host_ = url_.substr(host_left, path_left - host_left);
        path_ = url_.substr(path_left);
    } else {
        host_ = url_.substr(host_left);
        path_ = "/";
    }
    // 生成socket对象
    if (scheme_ == HTTP)
        socket_ = std::make_shared<NORSocket>(bbkgl::get_host(host_), 80);
    else
        socket_ = std::make_shared<SSLSocket>(bbkgl::get_host(host_), 443);
    // 构造函数中执行
    run();
}

Request::~Request() {}

void Request::run() {
    std::stringstream message;
    if (method_ == GET) message << "GET ";
    else if (method_ == POST) message << "POST ";
    message << path_ << " " << "HTTP/1.1" << "\r\n";
    message << "Host: " << host_ << "\r\n";
    message << "Connection: keep-alive"  << "\r\n";
    for (const auto &it : headers_)
        message << it.first << ": " << it.second << "\r\n";
    message << "\r\n";
    socket_->sendl(message.str());
    socket_->recvl();
    response_ = std::make_shared<Response>(socket_->get_head(), socket_->get_body());
}