#include "fc_session.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "fc_buddylist_ctrl.h"
#include <json/json.h>

using namespace boost::property_tree;


FC_Session::FC_Session(FC_Server &server, tcp::socket socket)
    :_socket(std::move(socket)),_server(server)
{

}


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


void FC_Session::write(FC_Message *msg)
{
    do_write(msg);
}

//5-5 解决了搜索好友，但是还没有布局到QML中去
void FC_Session::friends_search_handle()
{
    FC_Message* message = new FC_Message;
    if(_server._onlineP.count(_fc_message.body()) !=0 && _server._onlineP[_fc_message.body()] != shared_from_this()) //同时要排除不是自己
    {//user  exist
        message->set_message_type(FC_FRIENDS_SEARCH_R);
        message->set_body_length(_fc_message.body_length());
        message->set_body(_fc_message.body(),_fc_message.body_length());
    }
    else
    { //not exist
        cout<<"not exist"<<endl;
        message->set_message_type(FC_FRIENDS_SEARCH_R);

        std::string msg = "error";
        char* buff = (char*)malloc(msg.size()+1);
        memset(buff,'\0',msg.size()+1);
        strcpy(buff,msg.c_str());
        message->set_body_length(msg.size());
        message->set_body(buff,msg.size());

        free(buff);
    }
    write(message); //return search message
}

void FC_Session::add_connection(const string &acc)
{
    //这是在线用户列表
    _server._onlineP[acc] = shared_from_this();
    //现在还需要发送用户信息
//    send_self_msg(acc);
    send_self_msg(acc); //通道传递数据不可以过大
    //发送好友列表过去
//    send_friends_lists(acc);
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

    FC_Message* msg = new FC_Message;
    msg->set_message_type(FC_FRIENDS_ADD);
    msg->set_body_length(strlen(sd));
    msg->set_body(sd,strlen(sd));



    //    //now return account message
    //发送给对方好友
    //    _socket = std::move(_server._onlineP[rv]->_socket);

    //错误在这里 当我从这里把数据发送给指定的客户端（std::move(_server._onlineP[rv]->_socket）时，客户端A会被关闭，他们之间就没有连接了
    //    do_write(msg,std::move(_server._onlineP[rv]->_socket));
    //    write(message);

    //这里可以优化
    auto self(shared_from_this());
    boost::asio::async_write(_server._onlineP[rv]->_socket,boost::asio::buffer(msg->header(),msg->header_length()+msg->body_length()),[this,self,msg](boost::system::error_code ec,size_t length){
        if(ec)
        {
            cout<<"失败"<<endl;
            stop();
        }else{
            cout<<"server do_write message_type： "<<msg->mess_type()<<endl;
            cout<<"server do_write： "<<msg->body()<<endl;
            cout<<"传送成功"<<endl;
            cout<<"length :: "<<length<<endl;
        }
    });
    //发送给对方好友

}

//5-29同意添加为好友后，更新服务端数据信息，并发送新的好友列表给客户端AB，同时发送反馈信息
void FC_Session::add_friends_lists()
{
    char* one = _fc_message.get_friends_identify(); //one是自己的
    char* two = _fc_message.get_self_identify(); //two是好友的

    //这里逻辑可以分出去]
    int n_id=0;
    int m_id= _server._friendsList[one]->GetBuddyItemCount(n_id);
    _server._friendsList[one]->AddBuddyItem(n_id,m_id);//添加到默认分组,默认分组id为0
    _server._friendsList[one]->SetBuddyTeamMaxCnt(n_id,m_id+1);
    _server._friendsList[one]->SetBuddyItemAccNum(n_id,m_id,two);
    _server._friendsList[one]->SetBuddyItemNickName(n_id,m_id,"bad");
    //其他不插入 默认为空

    int m2_id=  _server._friendsList[two]->GetBuddyItemCount(n_id);
    _server._friendsList[two]->AddBuddyItem(n_id,m2_id);//添加到默认分组,默认分组id为0
    _server._friendsList[two]->SetBuddyTeamMaxCnt(n_id,m2_id+1);
    _server._friendsList[two]->SetBuddyItemAccNum(n_id,m2_id,one);
    _server._friendsList[two]->SetBuddyItemNickName(n_id,m2_id,"sons");

    _server.make_data_json(); //更新数据库信息


    //send message to client
    FC_Message* msg = new FC_Message;
    msg->set_message_type(_fc_message.mess_type());
    msg->set_body_length(_fc_message.body_length());
    msg->set_body(_fc_message.body(),_fc_message.body_length());

    //这里可以优化
    auto self(shared_from_this());
    boost::asio::async_write(_server._onlineP[one]->_socket,boost::asio::buffer(msg->header(),msg->header_length()+msg->body_length()),[this,self,msg](boost::system::error_code ec,size_t length){
        if(ec)
        {
            cout<<"失败"<<endl;
            stop();
        }else{
            cout<<"server to Client do_write message_type： "<<msg->mess_type()<<endl;
            cout<<"server do_write to Client： "<<msg->body()<<endl;
            cout<<"length :: "<<length<<endl;
        }
        delete msg;
    });
}
//删除完后 需要更新好友列表

void FC_Session::delete_friend()
{

    string teamId,itemId;
    string account;

    stringstream input(_fc_message.body());
    getline(input,account,'.');
    getline(input,teamId,'.');
    getline(input,itemId,'.');


    int team = stoi(teamId);
    int item = stoi(itemId);

    cout<<"delete_friend: "<<account<<endl;

    _server._friendsList[account]->DelBuddyItem(team,item);
    _server.make_data_json(); //更新数据库
}

//帐号的验证
void FC_Session::login_verify()
{
    char* account = new char[FC_ACC_LEN+1];
    memset(account,'\0',FC_ACC_LEN+1);
    memcpy(account,_fc_message.body(),FC_ACC_LEN);

    char* password = new char[_fc_message.body_length()-FC_ACC_LEN+1];
    memset(password,'\0',_fc_message.body_length()-FC_ACC_LEN+1);
    strcpy(password,_fc_message.body()+FC_ACC_LEN);

    if(_server.login_verify(account,password))
    {
        add_connection(account);
    }
    else
    {
        cout<<"login failed"<<endl;
        exit(0);
    }

}

void FC_Session::send_friends_lists(string username)
{
    std::string str ;
    str = make_json(username);
    FC_Message* msg = new FC_Message;
    msg->set_message_type(FC_FRIENDS_MESSAGE);

    clog << "make_json::body_size():" << str.size() << endl;
    msg->set_body_length(str.size());
    msg->set_body(str.c_str(),str.size());
    write(msg);
}

void FC_Session::send_self_msg(const string &username)
{
    std::string str ;
    str = make_json_user(username);
    FC_Message* msg = new FC_Message;
    msg->set_message_type(FC_SELF_MES);

    clog << "make_json::body_size():" << str.size() << endl;
    msg->set_body_length(str.size());
    msg->set_body(str.c_str(),str.size());
    write(msg);
}


//处理备注
void FC_Session::handle_remark()
{
    string teamId,itemId;
    string account;
    string remark;

    stringstream input(_fc_message.body());
    getline(input,account,'.');
    getline(input,teamId,'.');
    getline(input,itemId,'.');
    getline(input,remark,'.');

    cout<<"teamId: "<<teamId<<"remark"<<remark<<endl;

    int team = stoi(teamId);
    int item = stoi(itemId);

    _server._friendsList[account]->SetBuddyItemNickName(team,item,remark);
    _server.make_data_json();

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
                cout<<"login"<<endl;
                //按理说登录也会返回相应的信息
                login_verify();
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
                }
                else{
                    cout<<"这是不同意添加好友"<<endl;
                }
                break;
            case FC_DELETE_FRIENDS:
                cout<<"删除好友"<<endl;
                delete_friend();
                break;
            case FC_FRIENDS_REMARK:
            {
                cout<<"好友信息（备注）"<<endl;
                handle_remark();
                break;
            }

            default:
                cout<<"没有这样的类型"<<endl;
                break;

            }
            do_read_header(); //循环的等待读消息
        }
    });
}


void FC_Session::do_write(FC_Message *msg)
{
    auto self(shared_from_this());
    boost::asio::async_write(_socket,boost::asio::buffer(msg->header(),msg->header_length()+msg->body_length()),[this,self,msg](boost::system::error_code ec,size_t length){
        if(ec)
        {
            cout<<"失败"<<endl;
            stop();
        }else{
            cout<<"server do_write message_type： "<<msg->mess_type()<<endl;
            cout<<"server do_write： "<<msg->body()<<endl;
            cout<<"传送成功"<<endl;
            cout<<"length :: "<<length<<endl;
        }
        delete msg;
    });
}

//void FC_Session::return_message_ok()
//{
//    FC_Message* message = new FC_Message;
//    message->set_message_type(FC_FRIENDS_REMARK);
//    message->set_body_length(_fc_message.body_length());
//    message->set_body(_fc_message.body(),_fc_message.body_length());
////    char* status = (char*) malloc(3);
////    memset(status,'\0',3);
////    strcpy(status,"ok"); //test data
////    message->set_body_length(strlen(status));
////    message->set_body(status,strlen(status));
//    write(message);
//}

//生成json文件

string FC_Session::make_json(string username)
{
    const string file_path = "buddys.json";
    ptree root;
    ptree buddy;
    ptree writeroot,writeitem;
    try {
        read_json<boost::property_tree::ptree>(file_path,root);
    } catch (ptree_error& e) {
        clog << "FC_Session::make_json:" << e.what() << endl;
    } catch(int e ){
        clog <<"FC_Session::make_json:" <<e << endl;
    }

    buddy = root.get_child("buddys");//得到数组

    cout<<buddy.size()<<endl;

    //时间复杂度为O(n^3)

    ptree groups;
    ptree pt_allitem;
    for(ptree::iterator it = begin(buddy);it != end(buddy);it++)
    {
        if(it->second.get<string>("username") == username)
        {
            ptree items;
            items = it->second.get_child("group");
            ptree pt_item;
            for(ptree::iterator lit = begin(items);lit != end(items);lit++)
            {
                ptree item= lit->second.get_child("members");
                ptree members;
                for(ptree::iterator llit = begin(item);llit != end(item);llit++)
                {
                    ptree subitem;
                    subitem.put("account",llit->second.get<string>("account"));
                    subitem.put("nickname",llit->second.get<string>("nickname"));
                    subitem.put("markname",llit->second.get<string>("markname"));
                    subitem.put("sign",llit->second.get<string>("sign"));
                    subitem.put("heading",llit->second.get<string>("heading"));
                    subitem.put("gender",llit->second.get<string>("gender"));

                    members.push_back(make_pair("",subitem));
                }
                pt_item.put("teamname",lit->second.get<string>("teamname"));
                pt_item.put_child("members",members);
                groups.push_back(make_pair("",pt_item));
            }

            pt_allitem.put("username",it->second.get<string>("username"));
            pt_allitem.put_child("group",groups);
        }

    }
    std::stringstream ss;
    boost::property_tree::write_json(ss, pt_allitem);
    std::string strContent = ss.str();
    return strContent;
}

string FC_Session:: make_json_user(const string &username)
{
    const string file_path = "user.json";
    ptree root;
    ptree users;
    ptree writeroot,writeitem;
    try {
        read_json<boost::property_tree::ptree>(file_path,root);
    } catch (ptree_error& e) {
        clog << "FC_Session::make_json_user:" << e.what() << endl;
    } catch(int e ){
        clog <<"FC_Session::make_json_user:" <<e << endl;
    }

    users = root.get_child("users");//得到数组
    for(ptree::iterator it = begin(users);it != end(users);it++)
    {
        if(it->second.get<string>("account") == username)
        {
            writeroot.put("account",it->second.get<string>("account"));
            writeroot.put("nickname",it->second.get<string>("nickname"));
            writeroot.put("gender",it->second.get<string>("gender"));
        }
    }
    std::stringstream ss;
    boost::property_tree::write_json(ss, writeroot);
    std::string strContent = ss.str();
    return strContent;
}

