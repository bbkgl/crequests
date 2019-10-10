#include <string>
#include <map>

class Response {
public:
    Response(std::string head, std::string body);
    ~Response();
    
    std::map<std::string, std::string> head_info_;
    std::string scheme_;
    int status_code_;
    std::string status_;
private:
    std::string body_;
    std::string head_;
};