#include <socket/poller.h>
using namespace exzvm::socket;

Poller::Poller() : m_fds(nullptr), m_max_conn(0), m_max_fd(0) {

}

Poller::~Poller() {

}

void Poller::create(int max_conn){
    // 定义一个 pollfd 结构体数组
    m_fds = new pollfd[max_conn];
    for (int i = 0; i < max_conn; i++) {
        m_fds[i].fd = -1;
    }
    m_max_conn = max_conn;
}

void Poller::add(int fd) {
    for (int i = 0; i < m_max_conn; i++) {
        if(m_fds[i].fd == -1) { // 找到第一个空闲的 pollfd
            m_fds[i].fd = fd;
            m_fds[i].events = POLLIN;
            if (m_max_fd < i) {
                m_max_fd = i;
            }
            m_fds_map[fd] = i;
            return;
        }
    }
}

void Poller::del(int fd) {
    // 通过fd找到下标
    auto it = m_fds_map.find(fd);
    if (it == m_fds_map.end()) {
        return;
    }
    m_fds[it->second].fd = -1; // 使fd从监听中被移除
}

bool Poller::is_set(int fd) {
    // 通过fd找到下标
    auto it = m_fds_map.find(fd);
    if (it == m_fds_map.end()) {
        return false;
    }
    return m_fds[it->second].revents & POLLIN;
}

int Poller::poll(int milliseconds) {
    return ::poll(m_fds, m_max_fd+1, milliseconds);
}

int Poller::max_fd() {
    return m_max_fd;
}

int Poller::get_fd(int idx) {
    // 根据下标获取fd
    return m_fds[idx].fd;
}