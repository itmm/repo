#pragma once

#include <string>
#include <utility>
#include <vector>

#include "reader.h"

class Test_Reader: public Reader {
        using Vector = std::vector<std::string>;
        Vector to_receive_;
        Vector::iterator cur_receive_;
        Vector to_expect_;
        Vector::iterator cur_expect_;

    public:
        static void run(const std::vector<std::string> &to_receive, const std::vector<std::string> &to_expect);

        Test_Reader(Vector to_receive, Vector to_expect): Reader { -1 },
            to_receive_ { std::move(to_receive) }, cur_receive_ { to_receive_.begin() },
            to_expect_ { std::move(to_expect) }, cur_expect_ { to_expect_.begin() }
        { }

        [[nodiscard]] bool all_received() const { return cur_receive_ == to_receive_.end(); }
        [[nodiscard]] bool all_expected() const { return cur_expect_ == to_expect_.end(); }

        ssize_t receive(void *buffer, size_t count) override;
        void process_line(const char *begin, const char *end);
};
