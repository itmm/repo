#include "reader.h"

#include <cstring>

#include "err.h"

void Reader::read_next_bytes() {
    auto bytes_to_read { buffer_ + sizeof(buffer_) - end_ };

    if (bytes_to_read <= 0) { throw Error { "line too long" }; }
    auto got = receive(end_, bytes_to_read);
    if (got < 0) { return; }
    if (got == 0) { throw Error { "socket half closed" }; }
    auto begin { buffer_ };
    auto cur { end_ };
    end_ += got;
    for (; cur < end_; ++cur) {
        if (*cur == '\n') {
            process_line(begin, cur + 1);
            begin = cur + 1;
        }
    }
    if (begin > buffer_) {
        end_ -= begin - buffer_;
        memmove(buffer_, begin, end_ - buffer_);
    }
}
