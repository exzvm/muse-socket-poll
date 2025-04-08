#include <iostream>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <cstring>
//#include <string>
//using std::string;
//#include <sys/unistd.h>

#include <socket/client_socket.h>
using namespace exzvm::socket;

int main() {

//    // 1.创建socket
//    Socket client;
//
//    // 2.连接服务端
//    client.connect("127.0.0.1", 8080); // 会发生阻塞

    auto logger = Singleton<Logger>::instance();
    logger->open("./../log/server.log");
    logger->set_max_size(1024);

    ClientSocket client("127.0.0.1", 8080);

    // 3.向服务端发送数据
    string data = "hello world";
    client.send(data.c_str(), data.size()); // 可能会发生阻塞
//    exit(1); // 模拟发送数据异常

    // 4.接收服务端的数据
    char buf[1024] = {0};
    client.recv(buf, sizeof(buf)); // 可能会发生阻塞

    log_info("recv: %s\n", buf);

    // 5.关闭socket
//    client.close(); // 析构函数关闭即可

    return 0;
}
