#pragma once

#include "retriever.h"

class Server {
        Retriever &retriever_;
        int listen_socket_ { - 1 };
    public:
        unsigned short port = 3142;

        explicit Server(Retriever &retriever): retriever_ { retriever } { }
        ~Server();

        void open();

        void handle_next_request();
};
