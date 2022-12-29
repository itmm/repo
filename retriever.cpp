#include "retriever.h"

[[nodiscard]] Retriever::Value_Type Retriever::get(
    const Retriever::Path &path, unsigned version
) const {
    auto v_path { base / path / std::to_string(version) };
    auto in {std::make_unique<std::ifstream>(v_path)};
    if (in->fail()) { return std::nullopt; }
    return Value_Type {std::move(in)};
}

[[nodiscard]] Retriever::Value_Type Retriever::get(
    const Retriever::Path &path, const char *tag
) const {
    if (! tag || ! *tag || isdigit(*tag)) { return std::nullopt; }
    auto tag_path { base / path / tag };
    std::ifstream tag_in(tag_path);
    unsigned v;
    tag_in >> v;
    if (tag_in.fail()) { return std::nullopt; }
    return get(path, v);
}
