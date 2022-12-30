#pragma once

#include <exception>

class Error: public std::exception {
        const char *what_;
    public:
        explicit Error(const char *what) noexcept: what_ { what } { }
        [[nodiscard]] const char* what() const noexcept override { return what_; }
};
