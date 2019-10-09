#include <memory>
#include <map>
#include "request.h"


int main() {
    std::map<std::string, std::string> headers;
    headers.insert(std::make_pair("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36"));
    std::string url = "https://img-blog.csdn.net/20180819102037653?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MjA2MDkw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70";
    std::shared_ptr<Request> r = std::make_shared<Request>(url, GET, headers);
    return 0;
}