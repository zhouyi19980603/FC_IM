#include "fc_session.h"
#include <iostream>
#include <fstream>
#include <ostream>


FC_Session::FC_Session(FC_Server &server, tcp::socket socket)
    :_socket(std::move(socket)),_server(server)
{

}

//FC_Session::FC_Session(boost::asio::ip::tcp::socket socket)
//    :_socket(std::move(socket))
//    //移动构造函数
//{

//}

//全局变量

void FC_Session::start()
{
    //一旦成功连接，则发送好友列表信息
    do_read_header(); //等待消息来读
}

void FC_Session::stop()
{
    _socket.close();
    for(auto it = _server._onlineP.begin();it !=_server._onlineP.end();)
    {
        if(it->second == shared_from_this())
            it = _server._onlineP.erase(it);
        else
            it++;
    }
    cout<<_server._onlineP.size()<<endl;
}

void FC_Session::write(FC_Message &msg)
{
    bool write_in_progress = !_write_msgs.empty();
    _write_msgs.push_back(msg);
    if(!write_in_progress)
        do_write();
}

//5-5 解决了搜索好友，但是还没有布局到QML中去
void FC_Session::friends_search_handle()
{
    FC_Message message;
    if(_server._onlineP.count(_fc_message.body()) !=0 && _server._onlineP[_fc_message.body()] != shared_from_this()) //同时要排除不是自己
    {//user  exist
        message.set_message_type(FC_FRIENDS_SEARCH_R);
        //now return account message
        message.set_body_length(_fc_message.body_length());
        message.set_body(_fc_message.body(),_fc_message.body_length());
    }
    else
    { //not exist
        cout<<"not exist"<<endl;
        message.set_message_type(FC_FRIENDS_SEARCH_R);

        std::string msg = "error";
        char* buff = (char*)malloc(msg.size()+1);
        memset(buff,'\0',msg.size()+1);
        strcpy(buff,msg.c_str());
        message.set_body_length(msg.size());
        message.set_body(buff,msg.size());

        free(buff);
    }
    write(message); //return search message
}

void FC_Session::add_connection()
{
    cout<<"_fc_message body"<<_fc_message.body()<<endl;
    _server._onlineP[_fc_message.body()] = shared_from_this();
//    _onlineP[_fc_message.body()]=shared_from_this();
}

//5.6 在添加好友这里，5.7还是这里
void FC_Session::add_friends()
{
    //目前不考虑好友列表
    //分离消息（好友的唯一标识为5位）,为char* 类型
    char* body = _fc_message.body();
    std::cout<<"add_friends message: "<<body<<std::endl;

    //这里可以优化
    char* sd = _fc_message.get_self_identify();
    char* rv = _fc_message.get_friends_identify();


    std::cout<<"sd: 发送方"<<sd<<":  rv 接收方:"<<rv<<std::endl;

    FC_Message msg;
    msg.set_message_type(FC_FRIENDS_ADD);
    msg.set_body_length(strlen(sd));
    msg.set_body(sd,strlen(sd));



//    //now return account message
    //发送给对方好友
//    _socket = std::move(_server._onlineP[rv]->_socket);

    //错误在这里 当我从这里把数据发送给指定的客户端（std::move(_server._onlineP[rv]->_socket）时，客户端A会被关闭，他们之间就没有连接了
//    do_write(msg,std::move(_server._onlineP[rv]->_socket));
//    write(message);

    //这里可以优化
    auto self(shared_from_this());
    boost::asio::async_write(_server._onlineP[rv]->_socket,boost::asio::buffer(msg.header(),msg.header_length()+msg.body_length()),[this,self,msg](boost::system::error_code ec,size_t length){
       if(ec)
       {
           cout<<"失败"<<endl;
           stop();
       }else{
           cout<<"server do_write message_type： "<<msg.mess_type()<<endl;
           cout<<"server do_write： "<<msg.body()<<endl;
           cout<<"传送成功"<<endl;
           cout<<"length :: "<<length<<endl;
       }
    });
    //发送给对方好友

}

void FC_Session::add_friends_lists()
{
    char* one = _fc_message.get_friends_identify();
    char* two = _fc_message.get_self_identify();
    _server._friendsList[one].push_back(two);
    _server._friendsList[two].push_back(one);


//    cout<<"我完了"<<endl;

    //send message to client
    FC_Message msg;
    msg = _fc_message;

//    write(_fc_message);

    //这里可以优化
    auto self(shared_from_this());
    boost::asio::async_write(_server._onlineP[one]->_socket,boost::asio::buffer(msg.header(),msg.header_length()+msg.body_length()),[this,self,msg](boost::system::error_code ec,size_t length){
       if(ec)
       {
           cout<<"失败"<<endl;
           stop();
       }else{
           cout<<"server to Client do_write message_type： "<<msg.mess_type()<<endl;
           cout<<"server do_write to Client： "<<msg.body()<<endl;
           cout<<"传送成功"<<endl;
           cout<<"length :: "<<length<<endl;
       }
    });

}
//删除完后 需要更新好友列表

void FC_Session::delete_friend()
{
    char* self = _fc_message.get_self_identify();
    char* friends = _fc_message.get_friends_identify();

    std::vector<std::string>& vec = _server._friendsList[self]; //对后面vector的引用
    const auto new_end(remove(begin(vec),end(vec),friends));
    vec.erase(new_end,end(vec));

    //会把该好友列表发送给用户，用于更新好友列表
}

void FC_Session::send_friends_lists()
{
    ifstream infile;
    infile.open("friends_list.json");
    assert(infile.is_open());
    std::ostringstream tmp;
    tmp << infile.rdbuf();
    std::string str = tmp.str();

//    std::string str="0101000001";
//    char* buff = (char*) malloc(str.size()+1);
//    memset(buff,'\0',str.size()+1);
//    memcpy(buff,str.c_str(),str.size());

    FC_Message message;
    message.set_message_type(FC_FRIENDS_MESSAGE);
    message.set_body_length(str.size());
    message.set_body(str.c_str(),str.size());
//    infile.close();
    write(message);
}

//4-30这里的问题 5-3已经解决
void FC_Session::do_read_header()
{
    _fc_message.reset_message();
    auto self(shared_from_this());
    boost::asio::async_read(_socket,boost::asio::buffer(_fc_message.header(),_fc_message.header_length()),[this,self](boost::system::error_code ec,size_t length)
    {
        //错误处理
        if(ec)
        {
            std::cout<<"ec error :"<<ec.message()<<std::endl;
            std::cout<<"server do_read_header error"<<std::endl;
            stop();

        }else
        {
            std::cout<<"do_ read_header: "<<_fc_message.mess_type()<<std::endl;
            std::cout<<"do_ read_header length: "<<length<<std::endl;
            do_read_body();
        }
    });

}

void FC_Session::do_read_body()
{
    auto self = shared_from_this();
    boost::asio::async_read(_socket,boost::asio::buffer(_fc_message.body(),_fc_message.body_length()),[this,self](boost::system::error_code ec,size_t length)
    {
       if(ec)
       {
           std::cout<<"error"<<std::endl;
           std::cout<<"error do_ read_body: "<<_fc_message.body()<<std::endl;
           stop();
       }else
       {
           std::cout<<"do_ read_body: "<<_fc_message.body()<<std::endl;
           std::cout<<"do_ read_body length: "<<length<<std::endl;
           switch (_fc_message.mess_type()) {
           case FC_FRIENDS_SEARCH:
               std::cout<<"查询好友信息"<<std::endl;
               friends_search_handle();
               break;
           case FC_SIGN_IN:
               //按理说登录也会返回相应的信息
               add_connection();
               send_friends_lists();
               cout<<_server._onlineP.size()<<endl;
               break;
           case FC_FRIENDS_ADD:
               cout<<"add friends"<<endl;
               add_friends();
               //添加好友信息
               break;

           case FC_FRIENDS_ADD_R:
               cout<<"反馈消息"<<endl;
               if(strcmp(_fc_message.get_core_body(),"ok\0") == 0)
               {
                   cout<<"同意添加好友"<<endl;
                   add_friends_lists();
                   cout<<"_friendsList:"<<_server._friendsList[_fc_message.get_self_identify()].size()<<endl;
               }
               else{
                   cout<<"这是不同意添加好友"<<endl;
               }
               break;
           case FC_DELETE_FRIENDS:
               cout<<"删除好友"<<endl;
               delete_friend();
               break;
           default:
               cout<<"没有这样的类型"<<endl;
               break;

           }
           do_read_header(); //循环的等待读消息
       }
    });
}

void FC_Session::do_write()
{
    auto self(shared_from_this());
    boost::asio::async_write(_socket,boost::asio::buffer(_write_msgs.front().header(),_write_msgs.front().header_length()+_write_msgs.front().body_length()),[this,self](boost::system::error_code ec,size_t length){
        if(!ec)
        {
            cout<<"server do_write message_type： "<<_write_msgs.front().mess_type()<<endl;
            cout<<"server do_write： "<<_write_msgs.front().body()<<endl;
            cout<<"传送成功"<<endl;
            cout<<"length :: "<<length<<endl;
            _write_msgs.pop_front();
            //类似于消息队列的形式
            if(!_write_msgs.empty())
            {
                do_write();
            }
        }
        else
        {
            cout<<"do_write:  "<<ec.message()<<endl;
            stop();
        }
    });
}

//void FC_Session::do_write(FC_Message &msg)
//{
//    auto self(shared_from_this());
//    boost::asio::async_write(_socket,boost::asio::buffer(msg.header(),msg.header_length()+msg.body_length()),[this,self,msg](boost::system::error_code ec,size_t length){
//       if(ec)
//       {
//           cout<<"失败"<<endl;
//           stop();
//       }else{
//           cout<<"server do_write message_type： "<<msg.mess_type()<<endl;
//           cout<<"server do_write： "<<msg.body()<<endl;
//           cout<<"传送成功"<<endl;
//           cout<<"length :: "<<length<<endl;
//       }
//    });
//}

//void FC_Session::do_write(FC_Message &msg, tcp::socket socket)
//{
//    auto self(shared_from_this());
//    boost::asio::async_write(socket,boost::asio::buffer(msg.header(),msg.header_length()+msg.body_length()),[this,self,msg](boost::system::error_code ec,size_t length){
//       if(ec)
//       {
//           cout<<"失败"<<endl;
//           stop();
//       }else{
//           cout<<"server do_write message_type： "<<msg.mess_type()<<endl;
//           cout<<"server do_write： "<<msg.body()<<endl;
//           cout<<"传送成功"<<endl;
//           cout<<"length :: "<<length<<endl;
//       }
//    });
//}
