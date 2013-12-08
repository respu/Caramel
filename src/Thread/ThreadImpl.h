// Caramel C++ Library - Thread Facility - Thread Private Header

#ifndef __CARAMEL_THREAD_THREAD_IMPL_H
#define __CARAMEL_THREAD_THREAD_IMPL_H
#pragma once

#include <Caramel/Caramel.h>
#include "Thread/ThreadIdImpl.h"
#include <Caramel/Thread/Thread.h>
#include <thread>


namespace Caramel
{

///////////////////////////////////////////////////////////////////////////////
//
// Thread
//

class ThreadImpl
{
public:

    ThreadImpl( const std::string& name, WorkFunction work );

    void Join();


private:

    void RunWork();

    std::string  m_name;
    WorkFunction m_workFunction;

    std::unique_ptr< std::thread > m_thread;

    ThreadIdPtr m_threadId;
};


///////////////////////////////////////////////////////////////////////////////

} // namespace Caramel

#endif // __CARAMEL_THREAD_THREAD_IMPL_H
