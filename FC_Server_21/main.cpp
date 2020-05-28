#include <iostream>
#include "fc_server.h"

using namespace std;

int main()
{
    boost::asio::io_context io_context;
    try {
//        FC_Server server;
        //若要添加线程，直接在这里添加，把FC_Server换为没有参数的
        tcp::endpoint endpoint(tcp::v4(),56781);
        FC_Server fc_server(io_context,endpoint);//暂时不要总的控制类
//        fc_server.init_friends_list();
//        cout <<"size:"<<fc_server.get_groupfriendsList().size()<<endl;
        io_context.run();
    } catch (std::exception& e) {
        cout<<e.what()<<endl;
    }

    return 0;
}
