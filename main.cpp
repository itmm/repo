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
        *got >> value; assert(value == "abc");
    }
    {
        auto got { retriever.get("a", 10) };
        assert(! got);
    }
    {
        auto got { retriever.get("a", "no-tag") };
        assert(! got);
    }
    {
        auto got { retriever.get("x") };
        assert(! got);
    }
    std::filesystem::remove_all(retriever.base);

    Test_Reader::run(
        { "a\r\nb", "\r\nc\r\n", "\r\n" },
        { "a\r\n", "b\r\n", "c\r\n", "\r\n" }
    );
}

[[noreturn]] void mainloop(unsigned short port, const std::filesystem::path &path) {
    Selector_Handler handler;
    Retriever retriever;
    retriever.base = path;
    auto server { std::make_unique<Server>(retriever) };
    server->port = port;
    server->open();
    handler += std::move(server);
    for (;;) {
        handler.handle_next_io();
    }
}

#define AFTER_PREFIX(S, P) (memcmp(S, P, sizeof(P) - 1) == 0 ? S + sizeof(P) - 1 : nullptr)

int main(int argc, const char *argv[]) {
    bool dont_run_tests { false };
    bool run_only_tests { false };
    unsigned short port { 3142 };
    auto path { std::filesystem::current_path() };
    const char *found;

    for (int i { 1 }; i < argc; ++i) {
        if (strcmp(argv[i], "--dont-run-tests") == 0) {
            dont_run_tests = true;
        } else if (strcmp(argv[i], "--run-only-tests") == 0) {
            run_only_tests = true;
        } else if ((found = AFTER_PREFIX(argv[i], "--port="))) {
            port = std::stoi(found);
        } else if ((found = AFTER_PREFIX(argv[i], "--base="))) {
            path = found;
        } else {
            std::cerr << "unknown option: " << argv[i] << "\n";
            return EXIT_FAILURE;
        }
    }
    if (! dont_run_tests) { run_unit_tests(); }
    if (run_only_tests) { return EXIT_SUCCESS; }
    try {
        mainloop(port, path);
    } catch (const Error &err) {
        std::cerr << err.what() << "\n";
        return EXIT_FAILURE;
    }
}
