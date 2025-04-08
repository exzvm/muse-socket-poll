#include <iostream>

#include <socket/server_socket.h>
#include <socket/poller.h>
using namespace exzvm::socket;


int main() {

    auto logger = Singleton<Logger>::instance();
    logger->open("./../log/server.log");
    logger->set_max_size(1024);

    ServerSocket server("127.0.0.1", 8080);

    // 创建一个 Poller 实例
    Poller poller;
    poller.create(1024);

    // 把服务端套接字加入队列
    poller.add(server.fd());

    while (true) {
        int num = poller.poll(2000); // -1表示一直等待
        if (num < 0) {
            log_error("poll error: errno=%d, errmsg=%s", errno, strerror(errno));
            break;
        } else if (num == 0) {
            log_error("poll timeout");
            continue;
        }
        log_debug("poll ok: num=%d", num);

        for (int i = 0; i < poller.max_fd()+1; i++) {
            if (!poller.is_set(poller.get_fd(i))) { // 不可读，注意这里不能直接用 == 来判断，是位运算
                continue;
            }

            if (num-- == 0) {
                break;
            }

            if (i == 0) {
                // 服务端套接字可读
                int connfd = server.accept();
                if (connfd < 0) {
                    log_error("server accept error: errno=%d, errmsg=%s", errno, strerror(errno));
                } else {
                    // 把新的连接套接字加入到监听队列中
                    poller.add(connfd);
                }
            } else {
                // 连接套接字可读
                Socket client(poller.get_fd(i));

                // 接收客户端的数据
                char buf[1024] = {0};
                size_t len = client.recv(buf, sizeof(buf));
                if (len < 0) {
                    log_error("recv error: errno=%d, errmsg=%s", errno, strerror(errno));
                } else if (len == 0) {
                    log_debug("socket closed by peer: conn=%d", poller.get_fd(i));
                    poller.del(poller.get_fd(i));
                    client.close();
                } else {
                    log_debug("recv: conn=%d msg=%s", poller.get_fd(i), buf);

                    // 向客户端发送数据
                    client.send(buf, len);
                    client.clear(); // 避免服务端主动关闭连接
                }
            }
        }
    }


    return 0;
}