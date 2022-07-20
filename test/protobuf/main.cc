#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main()
{
    // //封装了login请求对象的数据
    // LoginRequest req;
    // req.set_name("jiang can");
    // req.set_pwd("123456");

    // //对象数据序列化
    // std::string send_str;
    // if (req.SerializePartialToString(&send_str))
    // {
    //     std::cout << send_str << std::endl;
    // }

    // //从send_str反序列化一个login请求对象
    // LoginRequest reqB;
    // if (reqB.ParseFromString(send_str))
    // {
    //     std::cout << reqB.name() << std::endl;
    //     std::cout << reqB.pwd() << std::endl;
    // }

    // LoginResponse rsp;
    // ResultCode *rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登录失败了");

    GetFriendListsResponse rsp;
    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(0);
    rc->set_errmsg("success!!!");

    User *user1 = rsp.add_friend_list();
    user1->set_age(20);
    user1->set_name("jiangcan");
    user1->set_sex(User::MAN);

    std::cout << rsp.friend_list_size() << std::endl;

    return 0;
}