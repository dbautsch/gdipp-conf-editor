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

#include <stdexcept>
#include <iostream>

#include "demo_render.h"
#include "commandline.h"

#include "../gdipp-conf-editor/local_types.h"

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
    DemoRender * demoRender = NULL;

    try
    {
        CommandLine commandLine;
        MetaString outputFileName = commandLine.Get(TEXT("output"));

        if (outputFileName.empty())
        {
            throw std::runtime_error("Unable to start. Missing `output` parameter.");
        }

        demoRender = new DemoRender(outputFileName);
        demoRender->RenderToFile(TEXT("Ala ma kota"));
        delete demoRender;
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;

        if (demoRender)
        {
            delete demoRender;
        }

        return -1;
    }

    return 0;
}
