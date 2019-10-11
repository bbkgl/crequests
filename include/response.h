#ifndef RESPONSE
#define RESPONSE

#include <string>
#include <map>
#include <vector>

class Response {
public:
    Response(std::string head, std::string body);
    Response(int sig);
    ~Response();

    std::vector<uint8_t> get_content();
    std::string get_body();
    
    std::map<std::string, std::string> head_info_;
    std::string scheme_;
    int status_code_;
    std::string status_;
private:
    std::string body_;
    std::string head_;
    std::vector<uint8_t> content_;
};

#endif