#include "test-reader.h"

void Test_Reader::run(const std::vector<std::string> &to_receive, const std::vector<std::string> &to_expect) {
    Test_Reader t { to_receive, to_expect };
    while (! t.all_received()) {
        t.read_next_bytes();
    }
    assert(t.all_expected());
}

ssize_t Test_Reader::receive(void *buffer, size_t count) {
    assert(cur_receive_ != to_receive_.end());
    auto got = (ssize_t) cur_receive_->size();
    assert(count >= got);
    memcpy(buffer, cur_receive_->c_str(), got);
    ++cur_receive_;
    return got;
}

void Test_Reader::process_line(const char *begin, const char *end) {
    std::string value { begin, end };
    assert(cur_expect_ != to_expect_.end());
    assert(*cur_expect_ == value);
    ++cur_expect_;
}
