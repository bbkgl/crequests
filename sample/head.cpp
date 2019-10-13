#include <map>
#include <cstdio>
#include "crequests.h"

int main() {
    std::string cookie = "_zap=759f187a-225f-4e58-9819-8552a21f956e; d_c0=\"AHDrMtZWGhCPTsYs8Nu1yI9JdgQ8W8nBFgA=|1569425501\"; capsion_ticket=\"2|1:0|10:1569493832|14:capsion_ticket|44:OTgyZWNkYzU3Y2FjNDY5Zjg0MmRiN2YwYmU4YzFkOGQ=|385ec9cf69f8329170a2ee5a463408d741abd16e207d4757a820719e5bd6bc4d\"; z_c0=\"2|1:0|10:1569493851|4:z_c0|92:Mi4xQ0FCLUFnQUFBQUFBY09zeTFsWWFFQ1lBQUFCZ0FsVk5XLUY1WGdEelppd1o3bU5WdVgzX0pxN01VS29URU9SUHl3|4c7232fac6dc101dbd2af15043863f258cf4e16b1ad59377a3d604c0753160d2\"; tst=r; tgw_l7_route=66cb16bc7f45da64562a077714739c11; _xsrf=e96bb677-ef24-43b0-a793-67140c7f52be; Hm_lvt_98beee57fd2ef70ccdd5ca52b9740c49=1570712031,1570783743,1570890384,1570941041; Hm_lpvt_98beee57fd2ef70ccdd5ca52b9740c49=1570941041";
    std::map<std::string, std::string> headers;
    headers["Connection"] = "keep-alive";
    std::string url = "https://www.zhihu.com";
    headers.insert(std::make_pair("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36"));
    headers["cookie"] = cookie;
    // 设置超时时间为10s
    auto r = crequests::get(url, headers, 3);
    printf("%d\n", r.status_code_);
    printf("%s\n", r.get_body().c_str());
    return 0;
}