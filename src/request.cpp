#include <sstream>
#include "request.h"
#include "utils.h"

Request::Request(std::string url, METD method, std::map<std::string, std::string>  headers, std::string data, int timeout) :
    method_(method),
    url_(url), 
    headers_(headers), 
    data_(data),
    timeout_(timeout) {
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
    // 首先获得ip
    std::string server_ip = bbkgl::get_host(host_);
    if (bbkgl::error_num == BBKGLOK) {
        // 生成socket对象
        if (scheme_ == HTTP)
            socket_ = std::make_shared<NORSocket>(server_ip, 80, timeout_);
        else
            socket_ = std::make_shared<SSLSocket>(server_ip, 443, timeout_);
        // 如果是HEAD方法，提醒socket只接收请求头
        if (method_ == HEAD)
            socket_->set_head_method(true);
        // 构造函数中执行
        if (bbkgl::error_num == BBKGLOK) run();
    }
}

Request::~Request() {}

void Request::run() {
    std::stringstream message;
    if (method_ == GET) message << "GET ";
    else if (method_ == POST) message << "POST ";
    else if (method_ == HEAD) message << "HEAD ";
    message << path_ << " " << "HTTP/1.1" << "\r\n";
    message << "Host: " << host_ << "\r\n";
    message << "Connection: keep-alive"  << "\r\n";
    if (method_ == POST) 
        message << "Content-Length: " << data_.length() << "\r\n";
    for (const auto &it : headers_)
        message << it.first << ": " << it.second << "\r\n";
    message << "\r\n";
    if (method_ == POST) message << data_;
    socket_->sendl(message.str());
    socket_->recvl();
    response_ = std::make_shared<Response>(socket_->get_head(), socket_->get_body());
}

std::shared_ptr<Response> Request::get_response() {
    if (bbkgl::error_num == BBKGLOK)
        return response_;
    else
        return std::make_shared<Response>(-1);
}