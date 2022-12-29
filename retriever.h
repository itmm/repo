#pragma once

#include <filesystem>
#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <fstream>

class Retriever {
    public:
        constexpr static char LATEST_TAG[] = "latest";

        using Path = std::filesystem::path;
        using Value_Type = std::optional<std::unique_ptr<std::istream>>;

        Path base {std::filesystem::current_path()};

        [[nodiscard]] Value_Type get(const Path &path, unsigned version) const;

        [[nodiscard]] Value_Type get(const Path &path, const char *tag) const;

        [[nodiscard]] Value_Type get(const Path &path) const { return get(path, LATEST_TAG); }
};
