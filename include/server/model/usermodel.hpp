#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.h"

// User表的数据操作类
class UserModel {
public:
    // User表的增加方法
    bool insert(User &user);
    //根据id查询用户
    User query(int id);
    //更新用户的状态信息
    bool updateState(User user); 
    // 服务器异常退出，业务重置把online状态的用户，设置成offline
    void resetState();

};
#endif