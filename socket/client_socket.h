#pragma once

#include "socket.h"

namespace exzvm
{
    namespace socket
    {
        class ClientSocket : public Socket
        {
        public:
            ClientSocket() = delete;
            ClientSocket(const string & ip, int port);
            ~ClientSocket() = default;

        };
    }
}