#include "selector.h"

#include <cassert>
#include <unistd.h>

void Selector::close() {
    if (handle >= 0) {
        ::close(handle);
        handle = -1;
        can_read = can_write = false;
    }
}

void Selector::do_read(Selector_Handler &handler) {
    assert("not implemented");
}

void Selector::do_write(Selector_Handler &handler) {
    assert("not implemented");
}
