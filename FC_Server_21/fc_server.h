#ifndef FC_SERVER_H
#define FC_SERVER_H

//服务端的主要逻辑部分
#include "boost/asio.hpp"
#include <unordered_map>
#include <iostream>
#include "fc_accept.h"
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

using namespace boost::asio::ip;
using boost::asio::io_context;
using namespace std;
class FC_Thread_Pool;
class FC_Accept;
class FC_Session;
class FC_BuddyListCtrl;
class DbBroker;

typedef std::shared_ptr<FC_Session> clients_ptr;

class FC_Server
{
public:
    FC_Server();
    FC_Server(boost::asio::io_context& io_context,tcp::endpoint& endpoints);
    ~FC_Server();
    void add_connect(std::string,clients_ptr session);

    //这里应该细化以下，不应该暴露出来
    std::unordered_map<std::string,clients_ptr> _onlineP; //所有的在线好友信息


    //前面为网友A（string）的唯一标识，后面为每个网友A的所有好友的标识

    void json_data_parser();
    void make_data_json();
    void test_data();
    void init_accounts();
    bool login_verify(const string& acc,const string& pass);
    void set_accounts(const string& acc,const string& pass);
    std::unordered_map<std::string,std::string> get_accounts();

    std::unordered_map<std::string,FC_BuddyListCtrl*> _friendsList;

private:

    FC_Accept* _fc_accept=nullptr;
    FC_Thread_Pool* _thread_pool = nullptr;
    std::unordered_map<std::string,std::string> _accounts;
    DbBroker* _broker = nullptr;

    //在线列表
    //使用智能指针
//    std::unordered_map<std::string,clients_ptr> _onlineP; //所有的在线好友信息
//    std::unordered_map<std::string,std::vector<std::string>> _friendsList;


};

#endif // FC_SERVER_H
