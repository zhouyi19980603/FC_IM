#ifndef FC_THREAD_POOL_H
#define FC_THREAD_POOL_H

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <vector>
#include <unordered_map>


class FC_Thread_Pool
{   
public:
    using IOContext = boost::asio::io_context;
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;
    FC_Thread_Pool(std::size_t size=std::thread::hardware_concurrency() );
    FC_Thread_Pool(const FC_Thread_Pool&)=delete ;
    FC_Thread_Pool &operator=(const FC_Thread_Pool &)=delete ;
    boost::asio::io_context &getIOCOntext();
    void stop();
private:
    std::vector<IOContext>      io_context_;
    std::vector<WorkPtr>        works_;
    std::vector<std::thread>    threads_;
    std::size_t                 nextIOContext_;
};

#endif // FC_THREAD_POOL_H
