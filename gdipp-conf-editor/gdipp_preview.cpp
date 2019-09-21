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

static const int CAPTURE_WIDTH = 530;
static const int CAPTURE_HEIGHT = 500;

GDIPPPreview::GDIPPPreview(HWND targetWindow)
    : targetWindow(targetWindow),
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
    // TODO
}

PROCESS_INFORMATION GDIPPPreview::StartGDIPPDemoProcess()
{
    PROCESS_INFORMATION processInfo;
    STARTUPINFO startupInfo;

    memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));
    memset(&startupInfo, 0, sizeof(STARTUPINFO));
    
    startupInfo.cb = sizeof(STARTUPINFO);
    
    MetaString demoImageName = TEXT("gdipp_demo_render.exe");

    if (CreateProcess(demoImageName.c_str(),
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
        throw std::runtime_error("Unable to start gdipp_demo_render.exe process.");
    }

    return processInfo;
}

void GDIPPPreview::DrawWidgetToDC(HDC dc)
{
    if (demoProcessCapture == NULL)
    {
        // nothing to draw
        return;
    }

    HDC memDC = CreateCompatibleDC(dc);
    HGDIOBJ oldObject = SelectObject(memDC, demoProcessCapture);
    BitBlt(dc, 860, 50, CAPTURE_WIDTH, CAPTURE_HEIGHT, memDC , 0, 0, SRCCOPY);
    SelectObject(memDC, oldObject);
    DeleteDC(memDC);
}
