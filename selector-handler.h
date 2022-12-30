#pragma once

#include <algorithm>
#include <memory>
#include <list>

#include "selector.h"

class Selector_Handler {
        using Value_Type = std::unique_ptr<Selector>;
        std::list<Value_Type> selectors_;

    public:
        Selector_Handler &operator+=(Value_Type &&selector) {
            selectors_.push_back(std::move(selector)); return *this;
        }

        Selector_Handler &operator-=(const Selector &sel);

        void handle_next_io();
};
