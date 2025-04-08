#pragma once

#include "socket.h"

namespace exzvm
{
    namespace socket
    {
        class ServerSocket : public Socket
        {
        public:
            ServerSocket() = delete;
            ServerSocket(const string & ip, int port);
            ~ServerSocket() = default;

        };
    }
}
