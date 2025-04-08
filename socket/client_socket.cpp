#include "client_socket.h"

using namespace exzvm::socket;

ClientSocket::ClientSocket(const std::string &ip, int port) : Socket() {
    connect(ip, port);
}