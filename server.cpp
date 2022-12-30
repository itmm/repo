#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>

#include "err.h"
#include "server.h"

void Server::open() {
    assert(handler < 0);
    handler = socket(AF_INET, SOCK_STREAM, 0);
    if (handler < 0) { close(); throw Error { "cannot open socket" }; }
    sockaddr_in listen_addr { 0 };
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_addr.sin_port = htons(port);
    if (bind(handler, (sockaddr *) &listen_addr, sizeof(listen_addr)) < 0) {
        close(); throw Error { "cannot bind" };
    }
    if (listen(handler, 10) < 0) { close(); throw Error { "cannot listen" }; }
    std::cout << "listening on port " << port << "\n";
    can_read = true;
}

void Server::handle_next_request() {

}

void Server::do_read() {
    sockaddr_in session_addr { 0 };
    socklen_t addr_len { sizeof(session_addr) };
    int session_socket { accept(handler, (sockaddr *) &session_addr, &addr_len) };
    if (session_socket < 0) {
        std::cerr << "cannot accept\n";
        return;
    }
}
