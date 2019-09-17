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

#include "gdipp_preview.h"

GDIPPPreview::GDIPPPreview(HWND targetWindow, const MetaString & configurationDirectory)
    : targetWindow(targetWindow),
      configurationDirectory(configurationDirectory),
      demoProcessCapture(NULL)
{

}

GDIPPPreview::~GDIPPPreview()
{
    if (demoProcessCapture)
    {
        DeleteObject(demoProcessCapture);
        demoProcessCapture = NULL;
    }
}

void GDIPPPreview::UpdateView()
{
    PROCESS_INFORMATION processInfo;

    try
    {
        processInfo = StartGDIPPDemoProcess();

        if (demoProcessCapture != NULL)
        {
            DeleteObject(demoProcessCapture);
            demoProcessCapture = NULL;
        }

        demoProcessCapture = GrabGDIPPDemoWindow(processInfo);
        EndGDIPPDemoProcess(processInfo);
    }
    catch (const std::exception & e)
    {
        EndGDIPPDemoProcess(processInfo);
    }
}

PROCESS_INFORMATION GDIPPPreview::StartGDIPPDemoProcess()
{
    PROCESS_INFORMATION processInfo;
    STARTUPINFO startupInfo;

    memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));
    memset(&startupInfo, 0, sizeof(STARTUPINFO));
    
    startupInfo.cb = sizeof(STARTUPINFO);

#ifdef _WIN64
    MetaString demoImageName = TEXT("gdipp_pre_64.exe");
#else
    MetaString demoImageName = TEXT("gdipp_pre_32.exe");
#endif

    MetaString processCompleteName = configurationDirectory + TEXT("\\") + demoImageName;

    if (CreateProcess(processCompleteName.c_str(),
                      NULL,
                      NULL,
                      NULL,
                      FALSE,
                      0,
                      NULL,
                      NULL,
                      &startupInfo,
                      &processInfo) != TRUE)
    {
        throw std::runtime_error("Unable to start gdipp demo process.");
    }

    return processInfo;
}

HBITMAP GDIPPPreview::GrabGDIPPDemoWindow(const PROCESS_INFORMATION & demoProcess)
{
    HBITMAP capture = NULL;

    return capture;
}

void GDIPPPreview::EndGDIPPDemoProcess(PROCESS_INFORMATION & demoProcess)
{
    if (demoProcess.hProcess != NULL)
    {
        TerminateProcess(demoProcess.hProcess, 0);
        WaitForSingleObject(demoProcess.hProcess, 5000);

        CloseHandle(demoProcess.hThread);
        CloseHandle(demoProcess.hProcess);
    }

    memset(&demoProcess, 0, sizeof(PROCESS_INFORMATION));
}
