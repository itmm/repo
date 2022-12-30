#include "selector.h"

#include <cassert>
#include <unistd.h>

void Selector::close() {
    if (handler >= 0) {
        ::close(handler);
        handler = -1;
        can_read = can_write = false;
    }
}

void Selector::do_read() {
    assert("not implemented");
}

void Selector::do_write() {
    assert("not implemented");
}
