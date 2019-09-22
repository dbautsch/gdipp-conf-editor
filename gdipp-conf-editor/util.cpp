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

        int requiredLength = WideCharToMultiByte(CP_ACP,
            0,
            input.c_str(),
            0,
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

            WideCharToMultiByte(CP_ACP,
                0,
                input.c_str(),
                0,
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
           0,
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
                0,
                buffer,
                0);

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
