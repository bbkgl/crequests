#include <cstdio>
#include "response.h"

Response::Response(std::string head, std::string body) :
    head_(head),
    body_(body) {
    // 先解析请求头的第一行，找到行首和行尾
    char key[20], value[200];
    int line_left = 0, line_right = head.find("\r\n");
    sscanf(head.substr(line_left, line_right - line_left).c_str(), \
    "%s %d %s", key, &status_code_, value);
    scheme_ = std::string(key);
    status_ = std::string(value);
    // 解析请求头的其他行，基本是“key: value”的结构
    line_left = line_right + 2;
    while(line_left < head.length() && head[line_left] != '\r') {
        line_right = head.find("\r\n", line_left);
        // 因为之前在获得请求头字符串的时候把最后的“\r\n\r\n"给去掉了
        if (line_right == std::string::npos) line_right = head.length();
        // 这是百度上学到的神奇魔法？？？https://blog.csdn.net/crazy_huang/article/details/11747643
        sscanf(head.substr(line_left, line_right - line_left).c_str(), \
        "%[^:]: %[^:]", key, value);
        head_info_[std::string(key)] = value;
        line_left = line_right + 2;
    }

}

Response::~Response() {}

