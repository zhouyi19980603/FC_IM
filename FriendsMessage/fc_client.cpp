#include "fc_client.h"
#include "fc_message_handle.h"
#include "fc_thread_pool.h"
#include "fc_buddylist_ctrl.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>



using namespace boost::property_tree;

FC_Client::FC_Client()
{
    _thread_pool = new FC_Thread_Pool;
    this->_handle = new FC_Message_Handle(this);
    this->_buddy_list = new FC_BuddyListCtrl (this); //申请新空间
    io_context& _io_context = _thread_pool->getIOCOntext();

    tcp::resolver resolver(_io_context);
    auto endpoints = resolver.resolve("127.0.0.1","56781");

    _fc_connection = new FC_Connection (this,_io_context,endpoints);
    //在这里创建了connection连接

}


FC_Client::~FC_Client()
{
    delete _fc_connection;
    delete _thread_pool;
    delete _handle;
    delete _buddy_list;
    qDebug()<<"fc_client destructor"<<endl;
}


void FC_Client::add_msg_to_socket(FC_Message *msg)
{
    (*_fc_connection).write(msg);
}

void FC_Client::add_msg_to_qml(char *msg)
{
    _handle->displaytoQML(msg);
}

//void FC_Client::add_msg_to_qml(FC_Message &msg)
//{
//    _handle->displaytoQML(msg);
//}

void FC_Client::add_msg_to_qml(FC_Message *msg)
{
    _handle->displaytoQML(msg);
}

void FC_Client::setUniqueUserName(std::string name)
{
    uniqueUserName = name;
}

std::string FC_Client::getUniqueUserName()
{
    std::cout<<uniqueUserName<<std::endl;
    return this->uniqueUserName;
}

//得到服务端的解析信息

void FC_Client::json_data_parser()
{
    const string file_path = "friends_list.json";
    ptree root;
    ptree items;
    read_json<boost::property_tree::ptree>(file_path,root);
    cout<<root.get<string>("username")<<endl;
    items = root.get_child("group");
    cout<<"items: "<<items.size()<<endl;
    for(ptree::iterator it = begin(items);it != end(items);it++)
    {
        //添加一个项,项的ID为之前数组中分组个数
        int n_id = _buddy_list->GetBuddyTeamCount();
        _buddy_list->AddBuddyTeam(n_id);
        //设置团队名字
        _buddy_list->SetBuddyTeamName(n_id,it->second.get<string>("teamname"));
        ptree item;
        item = it->second.get_child("members");
        cout<<"item: "<<item.size()<<endl;
        for(ptree::iterator lit = begin(item);lit != end(item);lit++)
        {
            //计算该分组中member的值
            int m_id = _buddy_list->GetBuddyItemCount(n_id);
            _buddy_list->AddBuddyItem(n_id,m_id);  //这个

//            //添加一个新成员后，会增加一个成员信息
            _buddy_list->SetBuddyTeamMaxCnt(n_id,m_id+1);
            _buddy_list->SetBuddyItemAccNum(n_id,m_id,lit->second.get<string>("account"));
            _buddy_list->SetBuddyItemNickName(n_id,m_id,lit->second.get<string>("nickname"));
            _buddy_list->SetBuddyItemMarkName(n_id,m_id,lit->second.get<string>("markname"));
            _buddy_list->SetBuddyItemSign(n_id,m_id,lit->second.get<string>("sign"));
            _buddy_list->SetBuddyItemHeadPic(n_id,m_id,lit->second.get<string>("heading"));
            _buddy_list->SetBuddyItemGender(n_id,m_id,lit->second.get<string>("gender"));
        }
    }
    _buddy_list->addBuddyModel();
    //移除这个文件，暂时不移动
    remove(file_path.c_str());
}

void FC_Client::test_data()
{
    cout<<"friends team:"<<_buddy_list->GetBuddyTeamCount()<<endl;
    cout<<"one team is friends num:"<<_buddy_list->GetBuddyItemCount(0)<<endl;
}

FC_BuddyListCtrl *FC_Client::get_buddy_list()
{
    return this->_buddy_list;
}



