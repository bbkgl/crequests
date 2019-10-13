#include "crequests.h"
#include "utils.h"

namespace crequests {
    Response get(std::string url, std::map<std::string, std::string>  headers, int timeout) {
        std::shared_ptr<Request> request = std::make_shared<Request>(url, GET, headers, "", timeout);
        std::shared_ptr<Response> response = request->get_response();
        if (bbkgl::error_num == BBKGLOK)
            return *response.get();
        else
            return Response(-1);
    }

    Response post(std::string url, std::map<std::string, std::string>  headers, std::string data, int timeout) {
        std::shared_ptr<Request> request = std::make_shared<Request>(url, POST, headers, data, timeout);
        std::shared_ptr<Response> response = request->get_response();
        if (bbkgl::error_num == BBKGLOK)
            return *response.get();
        else
            return Response(-1);
    };

    Response head(std::string url, std::map<std::string, std::string>  headers, int timeout) {
        std::shared_ptr<Request> request = std::make_shared<Request>(url, HEAD, headers, "", timeout);
        std::shared_ptr<Response> response = request->get_response();
        if (bbkgl::error_num == BBKGLOK)
            return *response.get();
        else
            return Response(-1);
    }
}

