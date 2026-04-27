#ifndef PUBLIC_H
#define PUBLIC_H

// server和client的公共文件

enum EnMsgTypeenum
{
    //c
    LOGIN_MSG=1,       //登录
    LOGIN_MSG_ACK=2,
    REG_MSG=3,         //注册
    REG_MSG_ACK=4,
    ONE_CHAT_MSG=5,    //1V1聊天
    ADD_FRIEND_MSG=6,   //添加好友

    CREATE_GROUP_MSG=7,   // 创建群组
    ADD_GROUP_MSG=8,      // 加入群组
    GROUP_CHAT_MSG=9,     // 群聊天

    LOGINOUT_MSG=10 // 正常退出登录消息
};
#endif