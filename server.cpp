#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>

#include "server.h"

Server::~Server() {
    if (listen_socket_ >= 0) {
        close(listen_socket_);
        listen_socket_ = -1;
    }
}
void Server::open() {
    assert(listen_socket_ < 0);
    listen_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket_ < 0) {
        std::cerr << "cannot open socket\n"; exit(EXIT_FAILURE);
    }
    sockaddr_in listen_addr { 0 };
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_addr.sin_port = htons(port);
    if (bind(listen_socket_, (sockaddr *) &listen_addr, sizeof(listen_addr)) < 0) {
        close(listen_socket_); listen_socket_ = -1;
        std::cerr << "cannot bind\n"; exit(EXIT_FAILURE);
    }
    if (listen(listen_socket_, 10) < 0) {
        close(listen_socket_); listen_socket_ = -1;
        std::cerr << "cannot listen\n"; exit(EXIT_FAILURE);
    }
    std::cout << "listening on port " << port << "\n";
}

void Server::handle_next_request() {

}
