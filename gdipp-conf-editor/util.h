/*
    Copyright (c) 2019 Dawid Bautsch

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

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

    extern std::string UnicodeToAnsi(const std::wstring & input);

    extern std::wstring AnsiToUnicode(const std::string & input);
   
    extern MetaString CreateMetaString(const std::string & input);

    extern MetaString CreateMetaString(const std::wstring & input);

    extern std::string MetaStringToAnsi(const MetaString & input);

    extern std::wstring MetaStringToUnicode(const MetaString & input);
}
