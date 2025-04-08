#include <iostream>

#include <socket/server_socket.h>
using namespace exzvm::socket;

#include <poll.h>

#define MAX_CONN 1024

int main() {

    auto logger = Singleton<Logger>::instance();
    logger->open("./../log/server.log");
    logger->set_max_size(1024);

    ServerSocket server("127.0.0.1", 8080);

    // 定义一个 pollfd 结构体数组
    struct pollfd fds[MAX_CONN];
    for (int i = 0; i < MAX_CONN; i++) {
        fds[i].fd = -1;
    }

    // 把服务端套接字加入队列
    fds[0].fd = server.fd();
    fds[0].events = POLLIN;

    int max_fd = 0; // 在结构体数组中目前最大下标是0

    while (true) {
        int num = poll(fds, max_fd+1, -1); // -1表示一直等待
        if (num < 0) {
            log_error("poll error: errno=%d, errmsg=%s", errno, strerror(errno));
            break;
        } else if (num == 0) {
            log_error("poll timeout");
            continue;
        }
        log_debug("poll ok: num=%d", num);

        for (int i = 0; i < max_fd+1; i++) {
            if (!(fds[i].revents & POLLIN)) { // 不可读，注意这里不能直接用 == 来判断，是位运算
                continue;
            }
            if (i == 0) {
                // 服务端套接字可读
                int connfd = server.accept();
                if (connfd < 0) {
                    log_error("server accept error: errno=%d, errmsg=%s", errno, strerror(errno));
                } else {
                    // 把新的连接套接字加入到监听队列中
                    for (int j = 0; j < MAX_CONN; j++) {
                        if(fds[j].fd == -1) { // 找到第一个空闲的 pollfd
                            fds[j].fd = connfd;
                            fds[j].events = POLLIN;
                            if (max_fd < connfd) {
                                max_fd = connfd;
                            }
                            break;
                        }
                    }
                }
            } else {
                // 连接套接字可读
                Socket client(fds[i].fd);

                // 接收客户端的数据
                char buf[1024] = {0};
                size_t len = client.recv(buf, sizeof(buf));
                if (len < 0) {
                    log_error("recv error: errno=%d, errmsg=%s", errno, strerror(errno));
                } else if (len == 0) {
                    log_debug("socket closed by peer: conn=%d", fds[i].fd);
                    fds[i].fd = -1;
                    client.close();
                } else {
                    log_debug("recv: conn=%d msg=%s", fds[i].fd, buf);

                    // 向客户端发送数据
                    client.send(buf, len);
                    client.clear(); // 避免服务端主动关闭连接
                }
            }
        }
    }


    return 0;
}