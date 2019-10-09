#include <string>
#include <map>
#include <memory>
#include "sslsocket.h"
#include "norsocket.h"

// 请求方法的枚举类型
enum METD {GET = 0, POST};
enum SCHEME {HTTP = 0, HTTPS};

class Request {
public:
    Request(std::string url, METD method = GET, std::map<std::string, std::string>  headers = {});
    ~Request();

private:
    // 暂时写成void类型的
    void get_response();

    std::string url_;
    std::string host_;
    std::string path_;

    SCHEME scheme_;
    METD method_;

    std::map<std::string, std::string> headers_;

    std::shared_ptr<Socket> socket_;
};