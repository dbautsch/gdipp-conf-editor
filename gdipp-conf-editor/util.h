#pragma once

#include "local_types.h"

namespace Util
{
    extern int TryIntFromStr(const MetaString & str, int defaultValue);

    extern MetaString IntToStr(int value);
    
    template <typename T, typename W, typename X>
    bool ValueInRange(const T & value, const W & min, const X & max)
    {
        return (value >= static_cast<T>(min) &&
                value <= static_cast<T>(max));
    }
}
