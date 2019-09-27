#include <iostream>
#include <memory>
#include <fstream>
#include "utils.h"
#include "socket.h"

int main() {
    std::string query;
    std::ifstream q_in("../2333.txt");
    while (!q_in.eof()) {
        std::string str;
        getline(q_in, str);
        query += str + "\r\n";
    }
    query += "\r\n";
    std::cout << "start" << std::endl;
    std::string addr = bbkgl::get_host("www.baidu.com");
    std::cout << addr << std::endl;
    std::shared_ptr<Socket> sock = std::make_shared<Socket>(addr, 80);
    for (char ch : query) std::cout << ch;
    sock->sendl(query);
    std::string response = sock->recvl();
    std::cout << response << std::endl;
    return 0;
}