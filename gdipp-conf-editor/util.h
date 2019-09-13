#pragma once

#include <string>

namespace Util
{
    extern int TryIntFromStr(const std::string & str, int defaultValue);
    
    template <typename T, typename W, typename X>
    bool ValueInRange(const T & value, const W & min, const X & max)
    {
        return (value >= static_cast<T>(min) &&
                value <= static_cast<T>(max));
    }
}
