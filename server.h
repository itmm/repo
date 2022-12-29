#pragma once

#include "retriever.h"

class Server {
        Retriever &retriever_;
    public:
        unsigned short port = 3142;

        explicit Server(Retriever &retriever): retriever_ { retriever } { }

        void open();

        void handle_next_request();
};
