#include "util.h"

#include <sstream>

namespace Util
{
    int TryIntFromStr(const std::string & str, int defaultValue)
    {
        std::stringstream stream(str);
        int value = defaultValue;
        stream >> value;

        return value;
    }

    MetaString IntToStr(int value)
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }
} // namespace Util
