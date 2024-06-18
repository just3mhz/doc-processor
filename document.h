#pragma once

#include <string>
#include <optional>
#include <cstdint>

struct Document {
    std::string url;

    uint64_t pubDate;
    uint64_t fetchTime;

    std::string text;

    std::optional<uint64_t> firstFetchTime;
};
