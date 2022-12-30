#include "err.h"
#include "retriever.h"
#include "selector-handler.h"
#include "test-reader.h"
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

    Test_Reader::run(
        { "a\r\nb", "\r\nc\r\n", "\r\n" },
        { "a\r\n", "b\r\n", "c\r\n", "\r\n" }
    );
}

[[noreturn]] void mainloop() {
    Selector_Handler handler;
    Retriever retriever;
    auto server { std::make_unique<Server>(retriever) };
    server->open();
    handler += std::move(server);
    for (;;) {
        handler.handle_next_io();
    }
}

int main(int argc, const char *argv[]) {
    bool dont_run_tests { false };
    bool run_only_tests { false };
    for (int i { 1 }; i < argc; ++i) {
        if (strcmp(argv[i], "--dont-run-tests") == 0) {
            dont_run_tests = true;
        } else if (strcmp(argv[i], "--run-only-tests") == 0) {
            run_only_tests = true;
        } else {
            std::cerr << "unknown option: " << argv[i] << "\n";
            return EXIT_FAILURE;
        }
    }
    if (! dont_run_tests) { run_unit_tests(); }
    if (run_only_tests) { return EXIT_SUCCESS; }
    try {
        mainloop();
    } catch (const Error &err) {
        std::cerr << err.what() << "\n";
        return EXIT_FAILURE;
    }
}
