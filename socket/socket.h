#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <sys/unistd.h>
#include <fcntl.h>

//#include <utility/logger.h>
#include "../utility/logger.h"
using namespace exzvm::utility;

using std::string;

namespace exzvm
{
    namespace socket
    {
        class Socket
        {
        public:
            Socket();
            Socket(int sockfd);
            virtual ~Socket(); // 虚的析构函数

            bool bind(const string & ip, int port);
            bool listen(int backlog);
            bool connect(const string & ip, int port);
            int accept();
            int send(const char * buf, int len);
            int recv(char * buf, int len);
            void close();
            void clear();
            int fd();

            // 套接字常用属性
            bool set_non_blocking();
            bool set_send_buffer(int size);
            bool set_recv_buffer(int size);
            bool set_linger(bool active, int seconds);
            bool set_keepalive();
            bool set_reuseaddr();

        protected:
            string m_ip;
            int m_port;
            int m_sockfd;
        };
    }
}
