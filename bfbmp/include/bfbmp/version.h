#pragma once
#include <cstdint>
#include "config.h"

BFBMP_BEGIN
struct BFBMP_API version
{
    uint16_t major = 0;
    uint16_t minor = 0;

    version() = default;
    version(uint16_t major, uint16_t minor);
    version(uint32_t version);
    version(const version& version) = default;
    version(version&& version) noexcept = default;
    version& operator=(const version& version) = default;
    version& operator=(version&& version) noexcept = default;
    bool operator>(const version& rhs) const;
    bool operator<(const version& rhs) const;
    bool operator==(const version& rhs) const;
    bool operator!=(const version& rhs) const;
    bool operator<=(const version& rhs) const;
    bool operator>=(const version& rhs) const;
};
BFBMP_END