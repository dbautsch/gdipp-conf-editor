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

#include "../gdipp-conf-editor/util.h"

DemoRender::DemoRender(const MetaString & outputFileName)
    : outputFileName(outputFileName),
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

DemoRender::~DemoRender()
{
    using namespace Gdiplus;

    GdiplusShutdown(gdiplusToken);
}

void DemoRender::RenderToFile(const MetaString & text)
{
    using namespace Gdiplus;
    
    Bitmap bmp(300, 300);
    Graphics graphics(&bmp);
    SolidBrush brush(Color(255, 255, 255));

    FontFamily fontFamily(TEXT("Verdana"));
    Font font(&fontFamily, 12);

    graphics.DrawString(
        text.c_str(),
        -1,
        &font,
        PointF(0, 0),
        &brush);

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
        std::cerr << e.what() << std::endl;
        throw;
    }

    bmp.Save(outputFileName.c_str(), &bmpClsid);
}

bool DemoRender::GetEncoderClsid(const MetaString & format, CLSID * clsid) const
{
    using namespace Gdiplus;

    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* imageCodecInfo = NULL;

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
        if(MetaString(imageCodecInfo[j].MimeType) == format)
        {
            *clsid = imageCodecInfo[j].Clsid;
            delete [] imageCodecInfo;

            return true;
        }    
    }

    delete [] imageCodecInfo;
    return false;
}
