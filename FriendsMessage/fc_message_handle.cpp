#include "fc_message_handle.h"
#include <QDebug>
#include <QQuickItem>
#include <QQuickView>
#include <memory>
#include <string>
#include "FC_BuddyItem.h"
#include "FC_BuddyModel.h"
#include "FC_BuddyTeam.h"
#include "fc_buddylist_ctrl.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <json/json.h>


//控制类 好友模块的

using namespace boost::property_tree;


FC_Message_Handle::FC_Message_Handle()
{

}

FC_Message_Handle::FC_Message_Handle(FC_Client *client, QObject *parent)
    :QObject(parent),_client(client)
{
}

FC_Message_Handle::~FC_Message_Handle()
{
}

void FC_Message_Handle::show()
{

}

void FC_Message_Handle::add_friends(const QString &msg)
{
    qDebug()<<"add msg: "<<msg;
    char* account = (char*)malloc(msg.size()+1);
    memset(account,'\0',msg.size()+1);
    strcpy(account,msg.toLocal8Bit().data());//friends unique

//    std::cout<<"_client name:"<<_client->getUniqueUserName();
    char* self = stringTochar(_client->getUniqueUserName());
    char* body = text_content(account,self);//好友在前，自己在后

    //消息
//    FC_Message message;
    FC_Message* message = new FC_Message;
    message->set_message_type(FC_FRIENDS_ADD);
    message->set_body_length(strlen(body));
    message->set_body(body,strlen(body));

    free(account);
    _client->add_msg_to_socket(message);

}

void FC_Message_Handle::add_friends_result(const char* accout,const QString &msg)
{
//    int m_id = _client->get_buddy_list()->GetBuddyItemCount(0);//默认分组
//    _client->get_buddy_list()->AddBuddyItem(0,m_id);
//    _client->get_buddy_list()->SetBuddyTeamMaxCnt(0,m_id+1);
//    _client->get_buddy_list()->SetBuddyItemAccNum(0,m_id,accout);
//    _client->get_buddy_list()->addBuddyModel(); //更新数据

    FC_Message* message = new FC_Message;
    message->set_message_type(FC_FRIENDS_ADD_R);
    char* status = (char*) malloc(3);
    memset(status,'\0',3);
    strcpy(status,"ok"); //test data



    message->set_body_length(2*FC_ACC_LEN + strlen(status));  //?
    message->set_friend_identify(accout);
    message->set_self_identify(stringTochar(_client->getUniqueUserName()));
    message->set_core_body(status,strlen(status));
    free(status);
    _client->add_msg_to_socket(message);
    //假设同意添加好友
}

void FC_Message_Handle::update_remark(const int &team, const int &item, const QString &user)
{
    //本地修改
    _client->get_buddy_list()->SetBuddyItemNickName(team,item,user.toLocal8Bit().data());
    _client->get_buddy_list()->addBuddyModel();


    string content = _client->getUniqueUserName() +'.'+std::to_string(team)+'.'+std::to_string(item)+'.'+user.toLocal8Bit().data();

//    cout<<"content:"<<content<<endl;

//    Json::Value root;
//    root["username"] = self;
//    root["teamId"] = team;
//    root["itemId"] = item;
//    root["remark"] = user.toLocal8Bit().data();

//    Json::FastWriter writer;
//    string strJson = writer.write(root);

    FC_Message* msg = new FC_Message;
    msg->set_message_type(FC_FRIENDS_REMARK);
    msg->set_body_length(content.size());
    msg->set_body(content.c_str(),msg->body_length());



    _client->add_msg_to_socket(msg);

}

void FC_Message_Handle::delete_friend(const int &team, const int &item)
{
    //本地修改
    _client->get_buddy_list()->DelBuddyItem(team,item);
    _client->get_buddy_list()->addBuddyModel();
    string content = _client->getUniqueUserName() +'.'+std::to_string(team)+'.'+std::to_string(item);

    FC_Message* msg = new FC_Message;
    msg->set_message_type(FC_DELETE_FRIENDS);
    msg->set_body_length(content.size());
    msg->set_body(content.c_str(),msg->body_length());

    _client->add_msg_to_socket(msg);
}




void FC_Message_Handle::search_show(char *msg)
{
    qDebug()<<"displaytoQML msg: "<<msg<<"\n";
    if(strcmp(msg,"error\0") == 0 )
    {
        qDebug()<<"没有这个好友信息"<<"\n";
    }else
    {
//        QQuickView view(QUrl("qrc:/FriendsMessage.qml"));
//        QObject *item=view.rootObject();
//        QVariant msg1="hello c++";
//        QMetaObject::invokeMethod(item,"showF",Q_ARG(QVariant,msg));
//        delete item;
         qDebug()<<"输出好友信息"<<"\n";
    }
}


//void FC_Message_Handle::login(const QString &userAcount,const QString &password)
//{
//    qDebug()<<userAcount<<"  "<<password<<"\n";
//    char* account = (char*)malloc(userAcount.size()+1);
//    memset(account,'\0',userAcount.size()+1);

//    strcpy(account,userAcount.toLocal8Bit().data());
////    char account[userAcount.size()+1];
////    QByteArray sr=userAcount.toLocal8Bit();

////    //QString to char*
////    strcpy(account,sr.data());

////    char* pass= (char*)malloc(password.size());
////    strcpy(pass,password.toLocal8Bit().data());

//    //test data
////    std::cout<<"account"<<account<<std::endl;
////    std::cout<<"password"<<pass<<std::endl;
//    _client->setUniqueUserName(account);//复制用户名信息

//    std::cout<<"输出"<<_client->getUniqueUserName();

//    FC_Message* message = new FC_Message;
//    message->set_message_type(FC_SIGN_IN);
//    message->set_body_length(strlen(account));
//    message->set_body(account,strlen(account));

//    free(account);
//    _client->add_msg_to_socket(message);
//}

void FC_Message_Handle::search_friends(const QString &friendsAccount)
{
    //read qml to c++
    char* account = (char*)malloc(friendsAccount.size()+1);
    memset(account,'\0',friendsAccount.size()+1);
    //QString to char*
    strcpy(account,friendsAccount.toLocal8Bit().data());
    qDebug()<<"test data: in handle"<<account<<"\n";
    FC_Message* fc_message= new FC_Message;
    fc_message->set_message_type(FC_FRIENDS_SEARCH);
    fc_message->set_body_length(strlen(account));
    fc_message->set_body(account,strlen(account));

    free(account);//记得释放内存，否则会报错
    _client->add_msg_to_socket(fc_message);

}


void FC_Message_Handle::displaytoQML(FC_Message *message)
{
    switch (message->mess_type()) {
    case FC_FRIENDS_SEARCH_R:   //搜索结果显示
        search_show(message->body());
        break;
    case FC_FRIENDS_ADD:      //添加好友结果
        add_friends_result(message->body(),"ok");
        //将添加信息发送给用户界面
        //
        break;
    }
}

//5.6 显示在QML中去,显示在,这里判定不同的信息将有不同的显示方式，即消息类型不同，显示不同

char *FC_Message_Handle::text_content(const char *account, const char *pass)
{
    std::cout<<"account"<<account<<std::endl;
    std::cout<<"password"<<pass<<std::endl;
    int n = strlen(account)+strlen(pass)+1;
    char * ret = (char*) malloc(n);
    memset(ret,'\0',n);
    memcpy(ret,account,strlen(account));
    memcpy(ret+strlen(account),pass,strlen(pass));
    return ret;
}



std::string FC_Message_Handle::QStrTostr(QString &qstr)
{
    return (const char*)qstr.toLocal8Bit();
}

char *FC_Message_Handle::stringTochar(const std::string &str)
{
    char* buff = (char*)malloc(str.size()+1);
    memset(buff,'\0',str.size()+1);
    strcpy(buff,str.c_str());
    return buff;
}

