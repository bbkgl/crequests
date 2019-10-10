#ifndef CREQUEST
#define CREQUEST

#include "request.h"
#include "response.h"

namespace crequests {
    Response get(std::string url, std::map<std::string, std::string>  headers);
    Response post(std::string url, std::map<std::string, std::string>  headers, std::string data);
}

#endif