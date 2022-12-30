#pragma once

#include "selector.h"

class Session: public Selector {
        enum { read_request } mode_ { read_request };
    public:
        explicit Session(int handler) { can_read = true; }
};
