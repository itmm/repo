#pragma once

#include <filesystem>
#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <fstream>

class Retriever {
        constexpr static char LATEST_TAG[] = "latest";

    public:
        using Path = std::filesystem::path;
        using Value_Type = std::optional<std::unique_ptr<std::istream>>;

        Path base {std::filesystem::current_path()};

        [[nodiscard]] Value_Type get(const Path &path, unsigned version) const {
            auto v_path { base / path / std::to_string(version) };
            auto in {std::make_unique<std::ifstream>(v_path)};
            if (in->fail()) { return std::nullopt; }
            return Value_Type {std::move(in)};
        }

        [[nodiscard]] Value_Type get(const Path &path, const char *tag) const {
            if (! tag || ! *tag || isdigit(*tag)) { return std::nullopt; }
            auto tag_path { base / path / tag };
            std::ifstream tag_in(tag_path);
            unsigned v;
            tag_in >> v;
            if (tag_in.fail()) { return std::nullopt; }
            return get(path, v);
        }

        [[nodiscard]] Value_Type get(const Path &path) const { return get(path, LATEST_TAG); }
};
