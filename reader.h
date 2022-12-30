#pragma once

#include <cstring>
#include <sys/socket.h>

class Reader {
        int handle_;
        char buffer_[1024];
        char *end_ { buffer_ };
    public:
        explicit Reader(int handle): handle_ { handle } { }

        void read_next_bytes();

        virtual ssize_t receive(void *buffer, size_t count) {
            return recv(handle_, buffer, count, 0);
        }

        virtual void process_line(const char *begin, const char *end) = 0;
};
