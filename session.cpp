#include "session.h"

#include "selector-handler.h"

#include <iostream>

void Session::process_line(const char *begin, const char *end) {
    std::cout << "got " << std::string { begin, end };
    if (eat_headers_) {
        if (memcmp("\r\n", begin, end - begin) == 0) {
            eat_headers_ = false;
            std::cout << "sending reply\n";
            if (value_) {
                std::string reply = std::string  { "HTTP/1.1 " } + status_ + "\r\n";
                send(handle, reply.c_str(), reply.size(), 0);
                char buffer[1024];
                for (;;) {
                    value_->read(buffer, sizeof(buffer));
                    auto got { value_->gcount() };
                    if (got <= 0) { break; }
                    send(handle, buffer, got, 0);
                    if (got < sizeof(buffer)) { break; }
                }
            } else {
                std::string reply = std::string { "HTTP/1.1 " } + status_ + "\r\n\r\n";
                send(handle, reply.c_str(), reply.size(), 0);
            }
        }
        return;
    }
    if (memcmp(begin, "GET ", 4) == 0) {
        status_ = "404 Not Found";
        begin += 4;
        while (begin != end && *begin == ' ') { ++begin; }
        auto e { begin };
        while (e != end && *e != ' ') { ++e; }
        if (memcmp(begin, "/file/", 6) == 0) {
            begin += 6;
            auto q { begin };
            while (q < e && *q != '?') { ++q; }
            if (q == e || memcmp(q, "?v=", 3) != 0) {
                std::cerr << "no version in `" << std::string { begin, e } << "\n";
            } else {
                unsigned version = std::stoi(std::string { q + 3, e });
                std::filesystem::path path { begin, q };
                value_ = std::move(retriever_.get(path, version));
                if (value_) {
                    std::cout << "found " << path << ", " << version << "\n";
                    status_ = "200 OK";
                }
            }
        } else {
            std::cerr << "unknown path `" << std::string { begin, e } << "'\n";
        }
        eat_headers_ = true;
    } else {
        std::cerr << "unknown header: `" << std::string { begin, end } << "'\n";
    }
}

void Session::do_read(Selector_Handler &handler) {
    std::cout << "do read\n";
    if (! read_next_bytes()) {
        handler -= *this;
    }
}
