#include "fc_thread_pool.h"
#include <QDebug>

FC_Thread_Pool::FC_Thread_Pool(std::size_t size)
    :io_context_(size),
      works_(size),
      nextIOContext_(0)
{
    for(std::size_t i=0;i<size;i++)
        works_[i] = std::unique_ptr<Work>(new Work(io_context_[i]));

    for(std::size_t i=0;i<io_context_.size();i++)
        threads_.emplace_back([this,i](){
            io_context_[i].run();
        });
}

boost::asio::io_context &FC_Thread_Pool::getIOCOntext()
{
    auto &service = io_context_[nextIOContext_++];
    if(nextIOContext_ == io_context_.size())
        nextIOContext_ = 0;
    return service;
}

void FC_Thread_Pool::stop()
{
    for(auto& work: works_)
        work.reset();

    for(auto& t: threads_)
        t.join();
}

