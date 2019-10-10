#include <memory>
#include <map>
#include <fstream>
#include "request.h"
#include "response.h"


int main() {
    std::map<std::string, std::string> headers;
    headers.insert(std::make_pair("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36"));
    std::string url = "https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1571298874&di=5f65e634dfc0e134f841508f8156e21a&imgtype=jpg&er=1&src=http%3A%2F%2F5b0988e595225.cdn.sohucs.com%2Fimages%2F20190108%2F0ce87d199bec4970a014df24836df532.jpeg";
    std::shared_ptr<Request> request = std::make_shared<Request>(url, GET, headers);
    std::shared_ptr<Response> response = request->get_response();
    std::ofstream img("jth.jpg", std::ios_base::out);
    std::vector<uint8_t> content = response->get_content();
    for (uint8_t it : content) img << it;
    img.close();
    return 0;
}