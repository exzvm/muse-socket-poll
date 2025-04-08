#pragma once

#include <poll.h>
#include <map>

namespace exzvm
{
    namespace socket
    {
        class Poller
        {
        public:
            Poller();
            ~Poller();

            void create(int max_conn);
            void add(int fd);
            void del(int fd);
            bool is_set(int fd);

            int poll(int milliseconds);
            int max_fd();
            int get_fd(int idx);

        private:
            struct pollfd * m_fds;
            int m_max_conn;
            int m_max_fd;
            std::map<int, int> m_fds_map;
        };

    }
}