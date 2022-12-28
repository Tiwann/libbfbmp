#include "bfbmp/version.h"

BFBMP_BEGIN
version::version(uint16_t major, uint16_t minor) : major(major), minor(minor){}

version::version(uint32_t version)
{
    major = version >> 16 & 0xFF;
    minor = version & 0xFF;
}

bool version::operator>(const version& rhs) const
{
    return (major > rhs.major) || ((major == rhs.major) && minor > rhs.minor);
}

bool version::operator<(const version& rhs) const
{
    return (major < rhs.major) || ((major == rhs.major) && minor < rhs.minor);
}

bool version::operator==(const version& rhs) const
{
    return major == rhs.major && minor == rhs.minor;
}

bool version::operator!=(const version& rhs) const
{
    return !(*this == rhs);
}

bool version::operator<=(const version& rhs) const
{
    return !(rhs < *this);
}

bool version::operator>=(const version& rhs) const
{
    return !(*this < rhs);
}
BFBMP_END