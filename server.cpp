#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>

#include "err.h"
#include "selector-handler.h"
#include "server.h"
#include "session.h"

void Server::open() {
    assert(handle < 0);
    handle = socket(AF_INET, SOCK_STREAM, 0);
    if (handle < 0) { close(); throw Error {"cannot open socket" }; }
    sockaddr_in listen_addr { 0 };
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_addr.sin_port = htons(port);
    if (bind(handle, (sockaddr *) &listen_addr, sizeof(listen_addr)) < 0) {
        close(); throw Error { "cannot bind" };
    }
    if (listen(handle, 10) < 0) { close(); throw Error {"cannot listen" }; }
    std::cout << "listening on port " << port << "\n";
    can_read = true;
}

void Server::do_read(Selector_Handler &handler) {
    sockaddr_in session_addr { 0 };
    socklen_t addr_len { sizeof(session_addr) };
    int session_socket { accept(handle, (sockaddr *) &session_addr, &addr_len) };
    if (session_socket < 0) {
        std::cerr << "cannot accept\n";
        return;
    }
    auto session = std::make_unique<Session>(session_socket);
    handler += std::move(session);
}
