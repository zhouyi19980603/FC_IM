#ifndef FC_SESSION_H
#define FC_SESSION_H
#include <boost/asio.hpp>
#include "fc_message.h"
#include "fc_server.h"
#include <queue>

using namespace boost::asio::ip;
using namespace std;

typedef std::deque<FC_Message> message_queue;

class FC_Session : public enable_shared_from_this<FC_Session>
{
public:
    FC_Session(FC_Server& server,tcp::socket socket);
//    FC_Session(tcp::socket socket);
    void start();
    void stop(); //exit login
//    void write(FC_Message& msg);
    void write(FC_Message* msg);


private:
//    typedef std::shared_ptr<FC_Session> clients_ptr;

    void friends_search_handle();//好友查找处理
    void add_connection();
    void add_friends();
    void add_friends_lists();
    void delete_friend();
    void send_friends_lists(string username);
//    void send_friends_lists(string username,int flag  );
    void handle_remark();

//    void on_add
    void do_read_header();
    void do_read_body();
    void do_write(FC_Message* msg);
    void return_message_ok();

    //about json operation
    string make_json(string username);

    void append_josn_node();



//    void do_write(FC_Message& msg);
//    void do_write(FC_Message& msg,tcp::socket socket);

//    unordered_map<string,clients_ptr> _onlineP;
    FC_Message _fc_message;

//    message_queue _write_msgs; //暂时先不要队列

    tcp::socket _socket;
    FC_Server& _server;
};




#endif // FC_SESSION_H
