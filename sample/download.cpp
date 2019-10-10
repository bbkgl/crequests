#include <map>
#include <cstdio>
#include <fstream>
#include "crequests.h"

int main() {
    std::map<std::string, std::string> headers;
    headers["Connection"] = "keep-alive";
    std::string url = "https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1570721042152&di=60e08fccf7e7164987a4ff62e43bcfd5&imgtype=0&src=http%3A%2F%2Fi0.hdslb.com%2Fbfs%2Farticle%2F3c2753503ae4955cd4fd3ae1824bac7d15320532.jpg";
    headers.insert(std::make_pair("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36"));
    auto r = crequests::get(url, headers);
    printf("%d\n", r.status_code_);
    std::ofstream img("jth.jpg");
    img << r.get_body();
    img.close();
    return 0;
}