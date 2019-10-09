#include "request.h"
#include <memory>

int main() {
    std::string url = "http://www.cplusplus.com/reference/string/string/find/";
    std::shared_ptr<Request> r = std::make_shared<Request>(url);

    return 0;
}