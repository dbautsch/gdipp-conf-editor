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

#include "demo_render.h"

#include <iostream>

#include <gdiplus.h>
#include <gdiplusimaging.h>
#include <gdiplusimagecodec.h>

#include <windows.h>

#include <sstream>

#include "resource.h"

#include "../gdipp-conf-editor/util.h"

static const UINT Trigger_WMPRINT = 0x9103;

DemoRender::DemoRender(const MetaString & outputFileName)
    : outputFileName(outputFileName),
      gdiplusToken(NULL),
      capturedBitmap(NULL),
      stopMessageLoop(false)
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

DemoRender::~DemoRender()
{
    using namespace Gdiplus;

    if (capturedBitmap)
    {
        DeleteObject(capturedBitmap);
        capturedBitmap = NULL;
    }

    if (gdiplusToken)
    {
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
    }
}

void DemoRender::RenderToFile(const MetaString & text)
{
    using namespace Gdiplus;

    textToRender = text;

    MSG msg;
    
    // Create hidden window and use WM_PRINT message
    HWND hwnd = CreateDialogParam(NULL,
        MAKEINTRESOURCE(IDD_MAIN_DLG),
        NULL, MainDlgProc,
        reinterpret_cast<LPARAM>(this));            

    SetDlgItemText(hwnd, IDC_STATIC1, text.c_str());

	SendDlgItemMessage(hwnd,
		IDC_TEST_LISTBOX,
		LB_ADDSTRING,
		NULL,
		reinterpret_cast<WPARAM>(TEXT("ABCDEFGHIJKLMNOPQRSTUWXYZ")));

	SendDlgItemMessage(hwnd,
		IDC_TEST_LISTBOX,
		LB_ADDSTRING,
		NULL,
		reinterpret_cast<WPARAM>(TEXT("0123456789")));

    ShowWindow(hwnd, SW_HIDE);

    // trigger window printing
    SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(Trigger_WMPRINT, 0), 0);
    
    while (true)
    {
        if (stopMessageLoop)
        {
            break;
        }

        if (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    if (capturedBitmap)
    {
        Bitmap * bitmap = new Bitmap(capturedBitmap, NULL);

        if (!bitmap)
        {
            throw std::runtime_error("Unable to allocate memory to GDI+ Bitmap.");
        }
       
        CLSID bmpClsid;
    
        try
        {
            if (GetEncoderClsid(TEXT("image/bmp"), &bmpClsid) == false)
            {
                throw std::runtime_error("Unable to obtain image/bmp GDI+ encoder.");
            }
        }
        catch (const std::exception & e)
        {
            delete bitmap;

            std::cerr << e.what() << std::endl;
            throw;
        }

        Status status = bitmap->Save(Util::MetaStringToUnicode(outputFileName).c_str(), &bmpClsid);

        delete bitmap;

        if (status != Gdiplus::Ok)
        {
            std::stringstream stream;
            stream << "Gdiplus::Bitmap::Save failed with code " << static_cast<int>(status);

            throw std::runtime_error(stream.str().c_str());
        }
    }
}

bool DemoRender::GetEncoderClsid(const MetaString & format, CLSID * clsid) const
{
    using namespace Gdiplus;

    UINT num = 0;
    UINT size = 0;

    ImageCodecInfo * imageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);

    if(size == 0)
    {
        throw std::runtime_error("GetImageEncodersSize has failed.");
    }

    imageCodecInfo = new ImageCodecInfo[size];

    if(imageCodecInfo == NULL)
    {
        throw std::runtime_error("GetEncoderClsid: Out of memory.");
    }

    GetImageEncoders(num, size, imageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if(Util::CreateMetaString(imageCodecInfo[j].MimeType) == format)
        {
            *clsid = imageCodecInfo[j].Clsid;
            delete [] imageCodecInfo;

            return true;
        }    
    }

    delete [] imageCodecInfo;

    return false;
}

INT_PTR CALLBACK DemoRender::MainDlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static DemoRender * _this = NULL;

    switch (msg)
    {
    case WM_INITDIALOG:
        {
            _this = reinterpret_cast<DemoRender*>(lparam);
            break;
        }
    case WM_COMMAND:
        {
            switch (LOWORD(wparam))
            {
            case Trigger_WMPRINT:
                {
                    if (_this)
                    {
                        _this->CaptureWindowContents(hwnd);
                    }
                    break;
                }
            }
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC dc;

            if (wparam == 0)
            {
                // window is visible
                dc = BeginPaint(hwnd, &ps);
            }
            else
            {
                // window is hidden - get HDC from wparam
                dc = reinterpret_cast<HDC>(wparam);
            }

            if (wparam == 0)
            {
                EndPaint(hwnd, &ps);
            }
            else
            {
                // destroy window, no more needed
                _this->stopMessageLoop = true;
                EndDialog(hwnd, 0);
            }

            break;
        }
    case WM_PRINTCLIENT:
        {
            // request window repaint
            SendMessage(hwnd, WM_PAINT, wparam, lparam);
            break;
        }
    }

    return FALSE;
}

bool DemoRender::CaptureWindowContents(HWND hwnd)
{
    // begin printing window
    HDC hDCMem = CreateCompatibleDC(NULL);

    RECT rect;

    GetClientRect(hwnd, &rect);

    if (capturedBitmap)
    {
        DeleteObject(capturedBitmap);
        capturedBitmap = NULL;
    }

    HDC hDC = GetDC(hwnd);
    capturedBitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
    ReleaseDC(hwnd, hDC);
   
    HGDIOBJ oldObject = SelectObject(hDCMem, capturedBitmap);
    SendMessage(hwnd, WM_PRINT, (WPARAM) hDCMem, PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND | PRF_OWNED);

    SelectObject(hDCMem, oldObject);
    DeleteObject(hDCMem);

    return true;
}
