#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include "utils.h"
#include "norsocket.h"
#include "sslsocket.h"

int main() {
    std::string host = "www.163.com";
    std::string addr = bbkgl::get_host(host);
    std::stringstream stream;
    stream << "GET /" << " HTTP/1.1\r\n";
    stream << "Accept: */*\r\n";
    // stream << "Accept-Encoding: gzip, deflate,
    // br\r\n";//不要编码，否则还得多一个解码的步骤
    stream << "Accept-Language: zh-Hans-CN, zh-Hans; q=0.8, en-US; q=0.5, en; "
              "q=0.3\r\n";
    stream << "Connection: Keep-Alive\r\n";
    stream << "Host: " << host << "\r\n";
    stream << "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
              "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 "
              "Safari/537.36\r\n";
    stream << "\r\n";
    std::shared_ptr<Socket> sockhttps = std::make_shared<SSLSocket>(addr, 443, "2333.txt");
    sockhttps->sendl(stream.str());
    // std::cout << stream.str() << std::endl;
    sockhttps->recvl();
    return 0;
}