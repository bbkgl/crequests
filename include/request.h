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
    Request(std::string url, METD method = GET);
    ~Request();

private:
    // 暂时写成void类型的
    void run();

    std::string url_;
    std::string host_;
    std::string path_;

    SCHEME scheme_;
    METD method_;

    std::map<std::string, std::string> infos_;

    std::shared_ptr<Socket> socket_;
};