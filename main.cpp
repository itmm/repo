#include "retriever.h"
#include "server.h"

#include <iostream>

void run_unit_tests() {
    Retriever retriever; retriever.base = retriever.base / "unit-tests";
    std::filesystem::create_directories(retriever.base / "a");
    { std::ofstream out { retriever.base / "a" / "0" }; out << "abc"; }
    { std::ofstream out { retriever.base / "a" / Retriever::LATEST_TAG }; out << 0; }
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
}

[[noreturn]] void mainloop() {
    Retriever retriever;
    Server server { retriever };
    for (;;) {
        server.handle_next_request();
    }
}

int main(int argc, const char *argv[]) {
    bool dont_run_tests { false };
    bool run_only_tests { false };
    for (int i { 1 }; i < argc; ++i) {
        if (argv[i] == std::string { "--dont-run-tests" }) {
            dont_run_tests = true;
        } else if (argv[i] == std::string { "--run-only-tests" }) {
            run_only_tests = true;
        } else {
            std::cerr << "unknown option: " << argv[i] << "\n";
            return EXIT_FAILURE;
        }
    }
    if (! dont_run_tests) { run_unit_tests(); }
    if (run_only_tests) { return EXIT_SUCCESS; }
    mainloop();
}
