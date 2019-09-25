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

#include "util.h"

#include <sstream>

#include <windows.h>

namespace Util
{
    int TryIntFromStr(const MetaString & str, int defaultValue)
    {
        MetaStream stream(str);
        int value = defaultValue;
        stream >> value;

        return value;
    }

    MetaString IntToStr(int value)
    {
        MetaStream stream;
        stream << value;
        return stream.str();
    }

    std::string UnicodeToAnsi(const std::wstring & input)
    {
        char * buffer = NULL;
        std::string result;

        int requiredLength = WideCharToMultiByte(CP_UTF8,
            0,
            input.c_str(),
            -1,
            NULL,
            0,
            NULL,
            NULL);

        if (requiredLength <= 0)
        {
            return std::string();
        }

        try
        {
            buffer = new char[requiredLength + 1];

            WideCharToMultiByte(CP_UTF8,
                0,
                input.c_str(),
                -1,
                buffer,
                requiredLength,
                NULL,
                NULL);

            result = buffer;
            delete [] buffer;
        }
        catch (const std::exception & e)
        {
            if (buffer)
            {
                delete [] buffer;
                return std::string();
            }
        }

        return result;
    }

    std::wstring AnsiToUnicode(const std::string & input)
    {
       wchar_t * buffer = NULL;
       std::wstring result;

       int requiredLength = MultiByteToWideChar(CP_ACP,
           0,
           input.c_str(),
           -1,
           NULL,
           0);

       if (requiredLength == 0)
       {
           return std::wstring();
       }

       try
       {
            buffer = new wchar_t[requiredLength + 1];

            MultiByteToWideChar(CP_ACP,
                0,
                input.c_str(),
                -1,
                buffer,
                requiredLength);

            result = buffer;

            delete [] buffer;
       }
       catch (const std::exception & e)
       {
           if (buffer)
           {
               delete [] buffer;
               return std::wstring();
           }
       }

       return result;
    }

    MetaString CreateMetaString(const std::string & input)
    {
#if defined(UNICODE) || defined(_UNICODE)
        // ANSI -> UNICODE
        return AnsiToUnicode(input.c_str());
#else
        // ANSI -> ANSI
        return input;
#endif
    }

    MetaString CreateMetaString(const std::wstring & input)
    {
#if defined(UNICODE) || defined(_UNICODE)
        // UNICODE -> UNICODE
        return input;
#else
        // UNICODE-> ANSI
        return UnicodeToAnsi(input);
#endif
    }

    std::string MetaStringToAnsi(const MetaString & input)
    {
#if defined(UNICODE) || defined(_UNICODE)
        return UnicodeToAnsi(input);
#else
        return input;
#endif
    }

    std::wstring MetaStringToUnicode(const MetaString & input)
    {
#if defined(UNICODE) || defined(_UNICODE)
        return input;
#else
        return AnsiToUnicode(input);
#endif
    }
} // namespace Util
