#include "fc_connection.h"
#include "fc_client.h"
#include <fstream>
#include <ostream>
#include "fc_buddylist_ctrl.h"

using namespace std;

FC_Connection::FC_Connection(FC_Client *client,boost::asio::io_context &io_context, const tcp::resolver::results_type &endpoints)
    :_socket(io_context),_io_context(io_context),_client(client)
{
    _fc_message = new FC_Message;
    do_connect(endpoints);
}


void FC_Connection::write(FC_Message *message)
{
    do_write(message);
}

FC_Connection::~FC_Connection()
{
    delete _client;
    delete _fc_message;
}

void FC_Connection::do_connect(const tcp::resolver::results_type &endpoints)
{
    //_socket是与服务端的连接
    boost::asio::async_connect(_socket,endpoints,[this](boost::system::error_code ec,tcp::endpoint)
    {
        if(!ec)
        {
            do_read_header();
            std::cout<<"连接成功"<<std::endl;
            //处理函数
        }else{
            std::cerr<<"connect failed"<<std::endl;
            exit(-1);
        }
    });
    //tcp套接字可以重新使用
    _socket.set_option(tcp::socket::reuse_address(true));
    //重复使用
}

void FC_Connection::do_read_header()
{
    _fc_message->reset_message();
    boost::asio::async_read(_socket,boost::asio::buffer(_fc_message->header(),_fc_message->header_length()),[this](boost::system::error_code ec,size_t length)
    {
        //错误处理
        if(ec)
        {
            std::cout<<"fc_connection read_header error: "<<ec.message()<<std::endl;
            close();
        }
        else{
            std::cout<<"client do_read_header: "<<_fc_message->mess_type()<<std::endl;
            std::cout<<"do_read_header length: "<<length<<std::endl;
            do_read_body();
        }
    });
}

void FC_Connection::do_read_body()
{

    //这里的_fc_message的body_length()为之前的长度
    boost::asio::async_read(_socket,boost::asio::buffer(_fc_message->body(),_fc_message->body_length()),[this](boost::system::error_code ec,size_t length)
    {
       if(ec)
       {
           std::cout<<"fc_connection read_body error: "<<ec.message()<<std::endl;
            close();
       }else
       {
           switch (_fc_message->mess_type()) {
           case FC_FRIENDS_SEARCH_R:
               std::cout<<"come on result"<<std::endl;
               _client->add_msg_to_qml(_fc_message);  //去显示搜索结果
               //show search result
               break;
           case FC_FRIENDS_ADD:{
               //QML中显示
               std::cout<<"add friends:"<<_fc_message->body()<<std::endl;
               _client->add_msg_to_qml(_fc_message);
               break;
           }
           case FC_FRIENDS_ADD_R:
               //产生一个添加好友结果
               std::cout<<"add friends result: "<<_fc_message->body()<<std::endl;
               break;
           case FC_FRIENDS_MESSAGE:
           {
//               std::cout<<"消息"<<std::endl;
               //生成json文件
               //在这之前不能有任何输出函数
               std::ofstream in("friends_list.json");
               in.write(_fc_message->body(),_fc_message->body_length());
               in.close();
               _client->json_data_parser();
               _client->test_data();
               break;
           }
           case FC_FRIENDS_TEST:
               std::cout<<"测试数据"<<std::endl;
               break;
           case FC_FRIENDS_REMARK:
                update_remark();
               break;
           default:
               std::cout<<"没有这种类型"<<std::endl;
               break;
           }
           do_read_header();
       }
    });
}


void FC_Connection::close()
{
    std::cout<<"client close"<<std::endl;
    exit(0);
}


void FC_Connection::do_write(FC_Message *msg)
{
    boost::asio::async_write(_socket,boost::asio::buffer(msg->header(),msg->header_length()+msg->body_length()),[this,msg](boost::system::error_code ec,size_t length){
        if(ec)
        {
           std::cout<<"fc_connection write error: "<<ec.message()<<std::endl;
           close();
        }else
        {
            std::cout<<"fc_connection mess_type: "<< msg->mess_type()<<std::endl;
            std::cout<<"fc_connection mess_body: "<< msg->body()<<std::endl;
            std::cout<<"fc_connection length: "<< length<<std::endl;
        }
        delete msg;
    });
}

void FC_Connection::update_remark()
{
    string teamId,itemId;
    string account;
    string remark;

    stringstream input(_fc_message->body());
    getline(input,account,'.');
    getline(input,teamId,'.');
    getline(input,itemId,'.');
    getline(input,remark,'.');

    cout<<"teamId: "<<teamId<<"remark"<<remark<<endl;

    int team = stoi(teamId);
    int item = stoi(itemId);

    _client->get_buddy_list()->SetBuddyItemNickName(team,item,remark);
    _client->get_buddy_list()->addBuddyModel();
}
