#include <memory>
#include <map>
#include "request.h"


int main() {
    std::map<std::string, std::string> headers;
    headers.insert(std::make_pair("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36"));
    std::string url = "https://www.baidu.com";
    std::shared_ptr<Request> r = std::make_shared<Request>(url, GET, headers);
    return 0;
}