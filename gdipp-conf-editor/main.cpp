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

#include <windows.h>
#include <exception>

#include "application.h"


#if defined(UNICODE) || defined(_UNICODE)
INT APIENTRY wWinMain(HINSTANCE thisInstance,
                     HINSTANCE prevInstance,
                     wchar_t * cmdLine,
                     int showCmd)
#else
INT APIENTRY WinMain(HINSTANCE thisInstance,
                     HINSTANCE prevInstance,
                     char * cmdLine,
                     int showCmd)
#endif
{
    int retCode = -1;

    try
    {
        GDIPPConfigurationEditor::Application application(thisInstance,
                                                          cmdLine,
                                                          showCmd);
        retCode = application.Run();
    }
    catch (const std::exception & e)
    {
        MessageBox(0,
            Util::CreateMetaString(e.what()).c_str(),
            TEXT("GDIPP Configuration editor: An error occurred"),
            MB_ICONERROR);

        return -1;
    }
    catch (...)
    {
        MessageBox(0,
                   TEXT("Unknown error occurred. Program will terminate."),
                   TEXT("GDIPP Configuration editor: An error occurred"),
                   MB_ICONERROR);

        return -1;
    }

    return retCode;
}
