#pragma once

#include <socket/server_socket.h>
#include <socket/poller.h>


namespace exzvm
{
    namespace socket
    {
        class SocketHandler
        {
            SINGLETON(SocketHandler);

        private:
            Socket * m_server = nullptr;
            Poller m_poller;
            std::map<int, Socket *> m_conns;
        };
    }
}