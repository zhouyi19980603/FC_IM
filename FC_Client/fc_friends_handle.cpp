#include "fc_friends_handle.h"
#include "fc_message.h"
#include "fc_client.h"
#include "fc_buddylist_ctrl.h"
#include "fc_buddyitem.h"
#include "fc_buddymodel.h"
#include "fc_buddyteam.h"
#include "fc_buddy.h"
#include <json/json.h>
#include <QDebug>
#include <filesystem>
namespace fs = std::filesystem;

FC_Friends_Handle::FC_Friends_Handle(FC_Client *client, QObject *parent)
    :QObject(parent),_client(client)
{
    _buddy_list = new FC_BuddyListCtrl(client);
}

FC_Friends_Handle::~FC_Friends_Handle()
{
    delete _buddy_list;
}

void FC_Friends_Handle::search_friends(const QString &friendsAccount)
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

void FC_Friends_Handle::add_friends(const QString &msg)
{
    qDebug()<<"add msg: "<<msg;
    char* account = (char*)malloc(msg.size()+1);
    memset(account,'\0',msg.size()+1);
    strcpy(account,msg.toLocal8Bit().data());//friends unique

    char* self = stringTochar(_client->getUniqueUserName());
    char* body = text_content(account,self);//好友在前，自己在后，合并在一起

    //消息
    FC_Message* message = new FC_Message;
    message->set_message_type(FC_FRIENDS_ADD);
    message->set_body_length(strlen(body));
    message->set_body(body,strlen(body));

    free(account);
    _client->add_msg_to_socket(message);
}

void FC_Friends_Handle::update_remark(const int &team, const int &item, const QString &user)
{
    //本地修改
    _buddy_list->SetBuddyItemMarkName(team,item,user.toLocal8Bit().data());
    _buddy_list->addBuddyModel();


//    string friends = _buddy_list->GetBuddyItemAccNum(team,item);
//    string content = _client->getUniqueUserName() +'.'+friends+'.'+user.toLocal8Bit().data();


//    FC_Message* msg = new FC_Message;
//    msg->set_message_type(FC_FRIENDS_REMARK);
//    msg->set_body_length(content.size());
//    msg->set_body(content.c_str(),msg->body_length());

//    _client->add_msg_to_socket(msg);
}

void FC_Friends_Handle::delete_friend(const int &team, const int &item)
{
    qDebug()<<"_buddy_list->GetBuddyTeamCount()"<<_buddy_list->GetBuddyTeamCount();
//    string friends = _buddy_list->GetBuddyItemAccNum(team,item);
//    string friends = _buddy_list->GetBuddyItemAccNum(team,item);
    //本地修改
//    string content = _client->getUniqueUserName() +'.'+friends;
//    string content = _client->getUniqueUserName() +'.'+friends;

//    FC_Message* msg = new FC_Message;
//    msg->set_message_type(FC_DELETE_FRIENDS);
//    msg->set_body_length(FC_ACC_LEN*2);
//    msg->set_friend_identify(friends.c_str());
//    msg->set_self_identify(content.c_str());

//    _buddy_list->DelBuddyItem(team,item);
//    _buddy_list->addBuddyModel();
//    _client->add_msg_to_socket(msg);
}

void FC_Friends_Handle::validation_request(const QString &result)
{
    //验证请求是否ok
    if(result.toStdString() == "ok")
    {
        Buddy *buddy = Buddy::getInstance();
        //更新本地消息
        int m_id = _buddy_list->GetBuddyItemCount(0);//默认分组
        _buddy_list->AddBuddyItem(0,m_id);
        _buddy_list->SetBuddyTeamMaxCnt(0,m_id+1);
        _buddy_list->SetBuddyItemAccNum(0,m_id,buddy->account().toStdString());
        _buddy_list->SetBuddyItemHeadPic(0,m_id,buddy->heading().toStdString());
        _buddy_list->SetBuddyItemNickName(0,m_id,buddy->nickname().toStdString());
    //    _client->get_buddy_list()->SetBuddyItemGender(0,m_id,buddy.)
        _buddy_list->addBuddyModel();
        FC_Message* message = new FC_Message;
        message->set_message_type(FC_FRIENDS_ADD_R);
        char* status = (char*) malloc(3);
        memset(status,'\0',3);
        strcpy(status,"ok"); //test data



        message->set_body_length(2*FC_ACC_LEN + strlen(status));  //?
        message->set_friend_identify(buddy->account().toStdString().c_str());//朋友标识
        message->set_self_identify(stringTochar(_client->getUniqueUserName()));//自己标识
        message->set_core_body(status,strlen(status));
        free(status);
        _client->add_msg_to_socket(message);
    }else
    {
        qDebug()<<"不同意添加为好友";
    }
}

void FC_Friends_Handle::update_remark(char *content)
{
    string teamId,itemId;
    string account;
    string remark;

    stringstream input(content);
    getline(input,account,'.');
    getline(input,teamId,'.');
    getline(input,itemId,'.');
    getline(input,remark,'.');

    cout<<"teamId: "<<teamId<<"remark"<<remark<<endl;

    int team = stoi(teamId);
    int item = stoi(itemId);

    _buddy_list->SetBuddyItemNickName(team,item,remark);
    _buddy_list->addBuddyModel();
}

void FC_Friends_Handle::displaytoQML(FC_Message *message)
{
    switch (message->mess_type()) {
    case FC_FRIENDS_SEARCH_R:   //搜索结果显示
        search_show(message->body());
        break;
    case FC_FRIENDS_ADD:      //添加好友结果
        qDebug()<<"我进来了 添加好友结果";
        add_friends_show(message->body());
//        add_friends_result(message->body(),"ok");
        //将添加信息发送给用户界面
        //
        break;
    }
}

void FC_Friends_Handle::refresh_friends_list(const string &msg)
{
    Json::Value root;
    Json::Reader reader;
    std::string acc;
    std::string nick;
    std::string heading;
    std::string sign;
    std::string sex;
    if(!reader.parse(msg, root)){
      std::cout <<"failed" <<std::endl;
    }
    else{
        acc = root["account"].asString();
        nick = root["nickname"].asString();
        sex = root["gender"].asString();
        sign = root["sign"].asString();
        heading = root["heading"].asString();
    }

    //保存在配置文件中
    _client->save_user_head(acc,heading);

    //得到整体当前文件所属的位置

    fs::path p = fs::current_path(); //目的是为了得到相对路径

    string path = "file://"+p.string()+"/assert/"+acc+".jpg";

    int m_id = _buddy_list->GetBuddyItemCount(0);//默认分组
    _buddy_list->AddBuddyItem(0,m_id);
    _buddy_list->SetBuddyTeamMaxCnt(0,m_id+1);
    _buddy_list->SetBuddyItemAccNum(0,m_id,acc);
    _buddy_list->SetBuddyItemNickName(0,m_id,nick);
    _buddy_list->SetBuddyItemGender(0,m_id,sex);
    _buddy_list->SetBuddyItemHeadPic(0,m_id,path);
    _buddy_list->SetBuddyItemSign(0,m_id,sign);
    _buddy_list->addBuddyModel();

}

void FC_Friends_Handle::parser_friends_json(const string &content)
{
    Json::Reader reader;
    Json::Value root;
    BuddyModel* model = BuddyModel::getInstance();
    // reader将Json字符串解析到root，root将包含Json里所有子元素
    if (reader.parse(content, root))
    {
        cout<<"root asString():"<<root["username"].asString()<<endl;
        Json::Value items  = root["group"];
        for(int i=0;i<items.size();i++)
        {
             BuddyTeam* team = new BuddyTeam ();
//            int n_id = _buddy_list->GetBuddyTeamCount();
//            _buddy_list->AddBuddyTeam(n_id);
//            //设置团队名字
//            _buddy_list->SetBuddyTeamName(n_id,items[i]["teamname"].asString());

            string teamname = items[i]["teamname"].asString();
            team->setTeamname(QString::fromLocal8Bit(teamname.c_str()));

            Json::Value item = items[i]["members"];
            for(int j=0;j<item.size();j++)
            {
                 BuddyItem* buddyitem = new BuddyItem();

                string acc = item[j]["account"].asString();
                string nickname = item[j]["nickname"].asString();
                _client->save_user_head(acc,item[j]["heading"].asString());
                fs::path p = fs::current_path(); //目的是为了得到相对路径
                string path = "file://"+p.string()+"/assert/"+acc+".jpg";
                string markname = item[j]["markname"].asString();
                string gender = item[j]["gender"].asString();
                string sign = item[j]["sign"].asString();

                buddyitem->setMarkname(QString::fromLocal8Bit(markname.c_str()));
                buddyitem->setAccount(QString::fromLocal8Bit(acc.c_str()));
                buddyitem->setNickname(QString::fromLocal8Bit(nickname.c_str()));
                buddyitem->setHeading(QString::fromLocal8Bit(path.c_str()));
                buddyitem->setGender(QString::fromLocal8Bit(gender.c_str()));
                buddyitem->setSign(QString::fromLocal8Bit(sign.c_str()));
                team->appendItem(buddyitem);


            }
            model->appendTeam(team);
        }
        emit model->teamsChanged();
    }
//    _buddy_list->addBuddyModel();
}

FC_BuddyListCtrl *FC_Friends_Handle::get_buddy_list()
{
    return this->_buddy_list;
}

void FC_Friends_Handle::search_show(const string &msg)
{
    if(strcmp(msg.c_str(),"error\0") == 0 )
    {
        qDebug()<<"没有这个好友信息"<<"\n";
    }
    Json::Value root;
    Json::Reader reader;
    std::string acc;
    std::string nick;
    std::string heading;
    if(!reader.parse(msg, root)){
      std::cout <<"failed" <<std::endl;
    }else{
        acc = root["account"].asString();
        nick = root["nickname"].asString();
        heading = root["heading"].asString();
    }
    _client->save_user_head(acc,heading); //保存图片

    fs::path p = fs::current_path(); //目的是为了得到相对路径
    string path = "file://"+p.string()+"/assert/"+acc+".jpg";

    Buddy *buddy = Buddy::getInstance();
    buddy->setAccount(QString::fromLocal8Bit(acc.c_str()));
    buddy->setNickname(QString::fromLocal8Bit(nick.c_str()));
    buddy->setHeading(QString::fromLocal8Bit(path.c_str())); //设置了相应的数据

    qDebug()<<buddy->account()<<" "<<buddy->heading()<<" "<<buddy->nickname();
}

void FC_Friends_Handle::add_friends_show(const string &msg)
{
    Json::Value root;
    Json::Reader reader;
    std::string acc;
    std::string nick;
    std::string heading;
    std::string sign;
    std::string sex;
    if(!reader.parse(msg, root)){
      std::cout <<"failed" <<std::endl;
    }
    else{
        acc = root["account"].asString();
        nick = root["nickname"].asString();
        sex = root["gender"].asString();
        sign = root["sign"].asString();
        heading = root["heading"].asString();
    }

    //保存在配置文件中
    _client->save_user_head(acc,heading);

    //得到整体当前文件所属的位置

    fs::path p = fs::current_path(); //目的是为了得到相对路径

    string path = "file://"+p.string()+"/assert/"+acc+".jpg";


    Buddy *buddy = Buddy::getInstance();
    buddy->setAccount(QString::fromLocal8Bit(acc.c_str()));
    buddy->setNickname(QString::fromLocal8Bit(nick.c_str()));
    buddy->setHeading(QString::fromLocal8Bit(path.c_str())); //设置了相应的数据
    buddy->setValue("1"); //直接传入1目前
}

char *FC_Friends_Handle::text_content(const char *account, const char *pass)
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

string FC_Friends_Handle::QStrTostr(QString &qstr)
{
    return (const char*)qstr.toLocal8Bit();
}

char *FC_Friends_Handle::stringTochar(const string &str)
{
    char* buff = (char*)malloc(str.size()+1);
    memset(buff,'\0',str.size()+1);
    strcpy(buff,str.c_str());
    return buff;
}
