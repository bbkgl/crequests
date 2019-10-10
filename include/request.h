#ifndef REQUEST
#define REQUEST

#include <string>
#include <map>
#include <memory>
#include "sslsocket.h"
#include "norsocket.h"
#include "response.h"

// 请求方法的枚举类型
enum METD {GET = 0, POST};
enum SCHEME {HTTP = 0, HTTPS};

class Request {
public:
    Request(std::string url, METD method = GET, std::map<std::string, std::string>  headers = {}, std::string data = "");
    ~Request();

    std::shared_ptr<Response> get_response();

private:
    // 暂时写成void类型的
    void run();

    std::string url_;
    std::string host_;
    std::string path_;

    SCHEME scheme_;
    METD method_;

    std::map<std::string, std::string> headers_;

    std::shared_ptr<Socket> socket_;

    std::shared_ptr<Response> response_;

    std::string data_;
};


#endif