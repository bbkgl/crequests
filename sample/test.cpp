#include <memory>
#include <map>
#include <cstdio>
#include "crequests.h"


int main() {
    std::map<std::string, std::string> headers;
    headers["Connection"] = "keep-alive";
    std::string data = "is_pad=1&username=21951111&password=XXXXXX";
    std::string url = "http://192.0.0.6/cgi-bin/do_login";
    headers.insert(std::make_pair("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36"));
    std::shared_ptr<Request> request = std::make_shared<Request>(url, POST, headers, data);
    std::shared_ptr<Response> response = request->get_response();
    printf("%s\n", response->get_body().c_str());
    return 0;
}