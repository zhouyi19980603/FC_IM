#include "fc_server.h"
#include "fc_thread_pool.h"
#include "fc_database.h"
#include "fc_buddylist_ctrl.h"
#include "fc_db_proxy.h"
#include <QString>
#include <QVariant>

FC_Server::FC_Server()
{
    _thread_pool = new FC_Thread_Pool;
//    _fc_friends = new FC_Database(this);
    io_context& _io_context = _thread_pool->getIOCOntext();
    tcp::endpoint endpoint(tcp::v4(),56781);

    _fc_accept = new FC_Accept (this,_io_context,endpoint);
    json_data_parser();

}

//现在使用的是第二个构造函数

FC_Server::FC_Server(boost::asio::io_context &io_context, tcp::endpoint &endpoints)
{
    _fc_accept = new FC_Accept (this,io_context,endpoints); //在这里连接server
    _broker = new DbBroker(this);
     init_accounts();
     json_data_parser();//获得从数据库中得到信息，目前为json中获得

}

FC_Server::~FC_Server()
{
    delete _fc_accept;

}

void FC_Server::add_connect(std::string account, clients_ptr session)
{
    _onlineP[account] = session;
}


void FC_Server::json_data_parser()
{
    _friendsList.clear();
    const string file_path = "buddys.json";
    ptree root;
    ptree buddy;
    read_json<boost::property_tree::ptree>(file_path,root);
    buddy = root.get_child("buddys");//得到数组

//    时间复杂度为O(n^3)


    for(ptree::iterator it = begin(buddy);it != end(buddy);it++)
    {
        FC_BuddyListCtrl* _buddy_list = new  FC_BuddyListCtrl(this);
        string name =  it->second.get<string>("username"); //用户名

        ptree items;
        items = it->second.get_child("group");
        for(ptree::iterator lit = begin(items);lit != end(items);lit++)
        {

            //添加一个项,项的ID为之前数组中分组个数
            int n_id = _buddy_list->GetBuddyTeamCount();
            _buddy_list->AddBuddyTeam(n_id);
            //设置团队名字
            _buddy_list->SetBuddyTeamName(n_id,lit->second.get<string>("teamname"));
            ptree item= lit->second.get_child("members");
            for(ptree::iterator llit = begin(item);llit != end(item);llit++)
            {
                //计算该分组中member的值
                int m_id = _buddy_list->GetBuddyItemCount(n_id);
                _buddy_list->AddBuddyItem(n_id,m_id);  //这个

    //            //添加一个新成员后，会增加一个成员信息
                _buddy_list->SetBuddyTeamMaxCnt(n_id,m_id+1);
                _buddy_list->SetBuddyItemAccNum(n_id,m_id,llit->second.get<string>("account"));
                _buddy_list->SetBuddyItemNickName(n_id,m_id,llit->second.get<string>("nickname"));
                _buddy_list->SetBuddyItemMarkName(n_id,m_id,llit->second.get<string>("markname"));
                _buddy_list->SetBuddyItemSign(n_id,m_id,llit->second.get<string>("sign"));
                _buddy_list->SetBuddyItemHeadPic(n_id,m_id,llit->second.get<string>("heading"));
                _buddy_list->SetBuddyItemGender(n_id,m_id,llit->second.get<string>("gender"));
            }
        }
        _friendsList[name] = _buddy_list;
    }
}

//生成新json
void FC_Server::make_data_json()
{
    try {

    const string file_path = "budd.json";

    ptree wroot,wbuddy;
    ptree pt_allitem;
    for(auto &each : _friendsList)
    {
        ptree pt_item;
        ptree groups;
        //这里可以得到name
        for(int i=0;i<each.second->GetBuddyTeamCount();i++)
        {
            ptree members;
            for(int j=0;j<each.second->GetBuddyItemCount(i);j++)
            {
                ptree subitem;
                subitem.put("account",each.second->GetBuddyItemAccNum(i,j));
                subitem.put("nickname",each.second->GetBuddyItemNickName(i,j));
                subitem.put("markname",each.second->GetBuddyItemMarkName(i,j));
                subitem.put("sign",each.second->GetBuddyItemSignName(i,j));
                subitem.put("heading",each.second->GetBuddyItemHeading(i,j));
                subitem.put("gender",each.second->GetBuddyItemGender(i,j));

                members.push_back(make_pair("",subitem));
            }
            pt_item.put("teamname",each.second->GetBuddyTeamName(i));
            pt_item.put_child("members",members);
            groups.push_back(make_pair("",pt_item));
        }
        pt_allitem.put("username",each.first);
        pt_allitem.put_child("group",groups);
        wbuddy.push_back(make_pair("",pt_allitem));
    }
    wroot.put_child("buddys",wbuddy);

    boost::property_tree::write_json(file_path, wroot);
    } catch (const std::exception& e) {
        cout<<"make_data_json:"<<e.what()<<endl;
    }
}

void FC_Server::test_data()
{
    cout<<"friends size"<<_friendsList.size()<<endl;

}

void FC_Server::init_accounts()
{
        QSqlQuery qu = _broker->get_user_account();
        while (qu.next()) {
            QString account = qu.value(0).toString();
            QString password = qu.value(1).toString();
            set_accounts(account.toStdString(),password.toStdString());
        }
//    const string file_path = "account.json";
//    ptree root;
//    ptree accounts;
//    try {
//        read_json<boost::property_tree::ptree>(file_path,root);
//    } catch (ptree_error& e) {
//        clog << "FC_Session::init_accounts:" << e.what() << endl;
//    } catch(int e ){
//        clog <<"FC_Session::init_accounts:" <<e << endl;
//    }

//    accounts = root.get_child("accounts");
//    for(ptree::iterator it = begin(accounts);it != end(accounts);it++)
//    {
//        string acc = it->second.get<string>("account");
//        string pass = it->second.get<string>("password");
//        set_accounts(acc,pass);
//    }
//    root.clear();
}

bool FC_Server::login_verify(const string &acc, const string &pass)
{
   if(_accounts[acc] == pass)
       return true;
   return false;
}

void FC_Server::set_accounts(const string &acc, const string &pass)
{
    _accounts[acc] = pass;
}

std::unordered_map<string, string> FC_Server::get_accounts()
{
    return this->_accounts;
}



