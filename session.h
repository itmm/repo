#pragma once

#include "retriever.h"
#include "selector.h"
#include "reader.h"

class Session: public Selector, Reader {
        Retriever &retriever_;
        const char *status_ = "500 Internal Server Error";
        bool eat_headers_ { false };
        Retriever::Value_Type value_;
    public:
        Session(Retriever &retriever, int handle): Reader(handle), retriever_ { retriever } {
            Selector::handle = handle;
            can_read = true;
        }

        void process_line(const char *begin, const char *end) override;
        void do_read(Selector_Handler &handler) override;
};
