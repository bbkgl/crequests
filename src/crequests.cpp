#include "crequests.h"

namespace crequests {
    Response get(std::string url, std::map<std::string, std::string>  headers) {
        std::shared_ptr<Request> request = std::make_shared<Request>(url, GET, headers);
        std::shared_ptr<Response> response = request->get_response();
        return *response.get();
    }
    Response post(std::string url, std::map<std::string, std::string>  headers, std::string data) {
        std::shared_ptr<Request> request = std::make_shared<Request>(url, POST, headers, data);
        std::shared_ptr<Response> response = request->get_response();
        return *response.get();
    };
}

