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

#include <tchar.h>

#include <iostream>

#include "util.h"

GDIPPPreview::GDIPPPreview(HWND targetWindow)
    : targetWindow(targetWindow),
      fontPreviewImage(NULL),
      gdiplusToken(NULL)
{
    using namespace Gdiplus;

    GdiplusStartupInput gdiplusStartupInput;
    

    Status gdiplusStatus =
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    if (gdiplusStatus != Ok)
    {
        MetaString message = TEXT("GdiplusStartup has failed. Status code: ");
        message += Util::IntToStr(static_cast<int>(gdiplusStatus));

        throw std::runtime_error(Util::MetaStringToAnsi(message));
    }
}

GDIPPPreview::~GDIPPPreview()
{
    using namespace Gdiplus;

    if (fontPreviewImage)
    {
        delete fontPreviewImage;
        fontPreviewImage = NULL;
    }

    if (gdiplusToken)
    {
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
    }
}

void GDIPPPreview::UpdateView()
{
    using namespace Gdiplus;

    PROCESS_INFORMATION demoProcess;
    MetaString bitmapFileName;

    try
    {
        bitmapFileName = GenerateTemporaryFileName();
        demoProcess = StartGDIPPDemoProcess(bitmapFileName);
        WaitForSingleObject(demoProcess.hProcess, INFINITE);

        CloseHandle(demoProcess.hThread);
        CloseHandle(demoProcess.hProcess);

        if (fontPreviewImage)
        {
            delete fontPreviewImage;
            fontPreviewImage = NULL;
        }

        fontPreviewImage = Image::FromFile(bitmapFileName.c_str());
        DeleteFile(bitmapFileName.c_str());
        RedrawWindow(targetWindow, NULL, NULL, RDW_ERASE | RDW_INVALIDATE );
    }
    catch (const std::exception & e)
    {
        if (demoProcess.hThread)
        {
            CloseHandle(demoProcess.hThread);
        }

        if (demoProcess.hProcess)
        {
            CloseHandle(demoProcess.hProcess);
        }

        DeleteFile(bitmapFileName.c_str());
        std::cerr << e.what() << std::endl;

        throw std::runtime_error("Unable to generate preview.");
    }
}

PROCESS_INFORMATION GDIPPPreview::StartGDIPPDemoProcess(const MetaString & bitmapFileName)
{
    PROCESS_INFORMATION processInfo;
    STARTUPINFO startupInfo;

    memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));
    memset(&startupInfo, 0, sizeof(STARTUPINFO));
    
    startupInfo.cb = sizeof(STARTUPINFO);
    
    MetaString demoImageName = TEXT("C:\\SOURCE\\gdipp-conf-editor\\debug\\gdipp_demo_render.exe");

    MetaString commandLineStr = TEXT("output=\"");
    commandLineStr += bitmapFileName;
    commandLineStr += TEXT("\"");

    TCHAR * commandLine = new TCHAR[commandLineStr.length() + 1];
    _tcscpy(commandLine, commandLineStr.c_str());

    if (CreateProcess(demoImageName.c_str(),
                      commandLine,
                      NULL,
                      NULL,
                      FALSE,
                      0,
                      NULL,
                      NULL,
                      &startupInfo,
                      &processInfo) != TRUE)
    {
        delete [] commandLine;
        throw std::runtime_error("Unable to start gdipp_demo_render.exe process.");
    }

    delete [] commandLine;

    return processInfo;
}

void GDIPPPreview::DrawWidgetToDC(HDC dc)
{
    using namespace Gdiplus;

    if (fontPreviewImage == NULL)
    {
        // nothing to draw
        return;
    }

    Graphics graphics(dc);
    graphics.DrawImage(fontPreviewImage, PointF(880, 50));
}

MetaString GDIPPPreview::GenerateTemporaryFileName() const
{
    TCHAR tempDirectory[MAX_PATH];

    if (GetTempPath(MAX_PATH, tempDirectory) == 0)
    {
        throw std::runtime_error(
            "GDIPPPreview::GenerateTemporaryFileName has failed. Unable to obtain temp directory.");
    }

    TCHAR tempFileName[MAX_PATH];

    if (GetTempFileName(tempDirectory,
        TEXT("demo_render"),
        0,
        tempFileName)
        == 0)
    {
        throw std::runtime_error(
            "GDIPPPreview::GenerateTemporaryFileName has failed. Unable to obtain temp filename.");
    }

    return MetaString(tempFileName);
}
