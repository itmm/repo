#pragma once

#include "retriever.h"
#include "selector.h"

class Server: public Selector {
        Retriever &retriever_;
    public:
        unsigned short port = 3142;

        explicit Server(Retriever &retriever): retriever_ { retriever } { }

        void open();

        void do_read(Selector_Handler &handler) override;
};
