#include "selector-handler.h"

#include <unistd.h>
#include <iostream>

Selector_Handler &Selector_Handler::operator-=(const Selector &sel) {
    auto got { std::find_if(
            selectors_.begin(), selectors_.end(),
            [&](const Value_Type &arg) { return &*arg == &sel; }
    ) };
    if (got != selectors_.end()) { selectors_.erase(got); }
    return *this;
}

void Selector_Handler::handle_next_io() {
    std::cout << "in handle next io\n";
    fd_set read_fds, write_fds, except_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    int max { -1 };
    for (const auto &i : selectors_) {
        if (i->can_read) { FD_SET(i->handle, &read_fds); std::cout << "can read " << i->handle << "\n"; }
        if (i->can_write) { FD_SET(i->handle, &write_fds); }
        FD_SET(i->handle, &except_fds);
        max = std::max(max, i->handle);
    }
    int activity { select(max + 1, &read_fds, &write_fds, &except_fds, nullptr) };
    if (activity <= 0) { return; }
    for (auto &i: selectors_) {
        if (FD_ISSET(i->handle, &except_fds)) {
            std::cerr << "exception on socket " << i->handle << "\n";
            *this -= *i;
            return;
        }
        if (i->can_write && FD_ISSET(i->handle, &write_fds)) {
            std::cout << "call do write " << i->handle << "\n";
            i->do_write(*this);
            return;
        }
        if (i->can_read && FD_ISSET(i->handle, &read_fds)) {
            std::cout << "call do read " << i->handle << "\n";
            i->do_read(*this);
            return;
        }
    }
}
