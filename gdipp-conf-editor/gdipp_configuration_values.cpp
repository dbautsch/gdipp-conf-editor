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

#include "gdipp_configuration_values.h"

#include <limits>

namespace GDIPPConfiguration
{
    Values::Values()
    {
        embeddedBitmap = INT_MIN;
        embolden = INT_MIN;
        hinting = INT_MIN;
        kerning = INT_MIN;
        renderer = INT_MIN;
        aliasedText = INT_MIN;
    }

    Values::ValidationResult Values::Validate() const
    {
        ValidationResult validationResult;

        if (autoHintingMode == AutoHintingMode::NotSet)
        {
            validationResult.AppendIncorrectValue(TEXT("auto_hinting"));
        }

        if (embolden < -1000 || embolden > 1000)
        {
            validationResult.AppendIncorrectValue(TEXT("embolden"));
        }

        if (lcdFilter == LCDFilter::NotSet)
        {
            validationResult.AppendIncorrectValue(TEXT("lcd_filter"));
        }

        if (gamma.GetR().empty())
        {
            validationResult.AppendIncorrectValue(TEXT("gamma.red"));
        }

        if (gamma.GetG().empty())
        {
            validationResult.AppendIncorrectValue(TEXT("gamma.green"));
        }

        if (gamma.GetB().empty())
        {
            validationResult.AppendIncorrectValue(TEXT("gamma.blue"));
        }

        if (hinting < 0 || hinting > 3)
        {
            validationResult.AppendIncorrectValue(TEXT("hinting"));
        }

        if (kerning < 0 || kerning > 1)
        {
            validationResult.AppendIncorrectValue(TEXT("kerning"));
        }

        if (renderMode.GetGrayMode() == RenderMode::NotSet ||
            renderMode.GetMonoMode() == RenderMode::NotSet ||
            renderMode.GetSubpixelMode() == RenderMode::NotSet)
        {
            validationResult.AppendIncorrectValue(TEXT("render_mode"));
        }

        if (renderer == INT_MIN)
        {
            validationResult.AppendIncorrectValue(TEXT("renderer"));
        }

        if (pixelGeometry == PixelGeometry::NotSet)
        {
            validationResult.AppendIncorrectValue(TEXT("pixel_geometry"));
        }

        if (shadow.GetOffsetX() == INT_MIN)
        {
            validationResult.AppendIncorrectValue(TEXT("shadow.offset_x"));
        }

        if (shadow.GetOffsetY() == INT_MIN)
        {
            validationResult.AppendIncorrectValue(TEXT("shadow.offset_y"));
        }

        if (shadow.GetAlpha() == INT_MIN)
        {
            validationResult.AppendIncorrectValue(TEXT("shadow.alpha"));
        }

        if (aliasedText == INT_MIN)
        {
            validationResult.AppendIncorrectValue(TEXT("aliased_text"));
        }
        
        return validationResult;
    }
} // namespace GDIPPConfiguration
