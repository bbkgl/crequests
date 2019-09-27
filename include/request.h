#include <string>

class Request {
public:
    Request(std::string url, std::string method = "GET");

private:
    std::string url_;
    std::string method_;
};