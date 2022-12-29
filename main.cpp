#include <iostream>
#include "retriever.h"

int main() {
    Retriever retriever; retriever.base = retriever.base / "unit-tests";
    std::filesystem::create_directories(retriever.base / "a");
    { std::ofstream out { retriever.base / "a" / "0" }; out << "abc"; }
    { std::ofstream out { retriever.base / "a" / "latest" }; out << 0; }
    {
        auto got { retriever.get("a") };
        std::string value;
        *got.value() >> value; assert(value == "abc");
    }
    {
        auto got { retriever.get("a", 10) };
        assert(! got.has_value());
    }
    {
        auto got { retriever.get("a", "no-tag") };
        assert(! got.has_value());
    }
    {
        auto got { retriever.get("x") };
        assert(! got.has_value());
    }
    std::filesystem::remove_all(retriever.base);
    return 0;
}
