#ifndef FC_CLIENT_H
#define FC_CLIENT_H
#include "fc_connection.h"
#include <QObject>
#include <QDebug>
#include <string>

class FC_Message_Handle;
class FC_Thread_Pool;
class FC_BuddyListCtrl;
using boost::asio::io_context;

//==============================================
//  Part II 好友管理模块
//==============================================

class FC_Client
{
public:
    FC_Client();
    ~FC_Client();

    void add_msg_to_socket(FC_Message& msg);
    void add_msg_to_qml(char* msg);
    void add_msg_to_qml(FC_Message& msg);
    //set funcation
    void setUniqueUserName(std::string name);
    //get funcation
    std::string getUniqueUserName();
    void json_data_parser();
    void test_data();

    FC_BuddyListCtrl* get_buddy_list();
private:
    FC_Connection* _fc_connection=nullptr;
    FC_Thread_Pool* _thread_pool = nullptr;
    FC_Message_Handle* _handle;
    FC_BuddyListCtrl* _buddy_list = nullptr; //存放好友列表的结果

    //这里需要有一个用户的标识信息
    std::string uniqueUserName;
};

#endif // FC_CLIENT_H
