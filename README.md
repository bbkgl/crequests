# crequests: A simple C++ requests

## Introduction

crequests是根据Python requests库写的一个C++ HTTP请求库，使用C++11编写，目前支持GET/POST方法，支持http/https访问，但暂时只能在Linux上使用。

## Features

访问https站点需要openssl，所以本项目中已经引入了openssl的头文件和动态库，可直接使用，不需要额外安装，但目前只适配linux，windows后面会跟上。

crequests目前支持的点有：

- 使用了openssl支持https访问
- 可以使用GET、POST方法
- 支持自定义请求头
- POST可以添加请求正文，提交表单
- 可以查看响应头的各项参数
- 支持chunk编码控制长度和Content-length控制长度

## Example

### GET

```cpp
#include <memory>
#include <map>
#include <cstdio>
#include "crequests.h"

int main() {
    std::map<std::string, std::string> headers;
    headers["Connection"] = "keep-alive";
    std::string url = "https://www.baidu.com";
    headers.insert(std::make_pair("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36"));
    std::shared_ptr<Request> request = std::make_shared<Request>(url, GET, headers);
    std::shared_ptr<Response> response = request->get_response();
    printf("%s\n", response->get_body().c_str());
    return 0;
}
```

输出：

```http
<!DOCTYPE html>
<!--STATUS OK-->
...

<html>
<head>
    
    <meta http-equiv="content-type" content="text/html;charset=utf-8">
...
</body>
</html>
```

### POST

```cpp
#include <memory>
#include <map>
#include <cstdio>
#include "crequests.h"


int main() {
    std::map<std::string, std::string> headers;
    headers["Connection"] = "keep-alive";
    std::string data = "is_pad=1&username=XXXX&password=XXXXXX";
    std::string url = "http://192.0.0.6/cgi-bin/do_login";
    headers.insert(std::make_pair("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36"));
    std::shared_ptr<Request> request = std::make_shared<Request>(url, POST, headers, data);
    std::shared_ptr<Response> response = request->get_response();
    printf("%s\n", response->get_body().c_str());
    return 0;
}
```

输出：

```cpp
password_error
```

