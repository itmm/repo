#include "selector-handler.h"

Selector_Handler &Selector_Handler::operator-=(const Selector &sel) {
    auto got { std::find_if(
            selectors_.begin(), selectors_.end(),
            [&](const Value_Type &arg) { return &*arg == &sel; }
    ) };
    if (got != selectors_.end()) { selectors_.erase(got); }
    return *this;
}

void Selector_Handler::handle_next_io() {

}
