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

#include "gdipp_configuration_reader.h"

#include "gdipp_configuration_values.h"
#include "pugixml/pugixml.hpp"
#include "util.h"

#include <exception>

namespace GDIPPConfiguration
{
    Reader::Reader(const MetaString & fileName)
        : fileName(fileName)
    {

    }

    GDIPPConfiguration::Values Reader::GetValues() const
    {
        Values values;

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(fileName.c_str());

        if (!result)
        {
            throw std::runtime_error("Unable to parse GDIPP configuration XML file.");
        }

        pugi::xml_node gdippNode = doc.root().child("gdipp");

        if (gdippNode.empty() == false)
        {
            pugi::xml_node freeTypeNode = gdippNode.child("gdimm").child("process").child("freetype");

            if (freeTypeNode.empty() == false)
            {
                pugi::xml_node lcdFilterNode = freeTypeNode.child("lcd_filter");

                if (lcdFilterNode.empty() == false)
                {
                    values.lcdFilter = std::string(lcdFilterNode.first_child().value());
                }
            }

            pugi::xml_node fontNode = gdippNode.child("gdimm").child("font");

            if (fontNode.empty() == false)
            {
                pugi::xml_node autoHintingNode = fontNode.child("auto_hinting");
                pugi::xml_node embeddedBitmapNode = fontNode.child("embedded_bitmap");
                pugi::xml_node emboldenNode = fontNode.child("embolden");
                pugi::xml_node gammaNode = fontNode.child("gamma");
                pugi::xml_node hintingNode = fontNode.child("hinting");
                pugi::xml_node kerningNode = fontNode.child("kerning");
                pugi::xml_node renderModeNode = fontNode.child("render_mode");
                pugi::xml_node rendererNode = fontNode.child("renderer");
                pugi::xml_node shadowNode = fontNode.child("shadow");

                if (autoHintingNode.empty() == false)
                {
                    values.autoHintingMode = std::string(autoHintingNode.first_child().value());
                }

                if (embeddedBitmapNode.empty() == false)
                {
                    values.embeddedBitmap = 
                        Util::TryIntFromStr(embeddedBitmapNode.first_child().value(), INT_MIN);
                }

                if (emboldenNode.empty() == false)
                {
                    values.embolden = Util::TryIntFromStr(emboldenNode.first_child().value(), INT_MIN);
                }

                if (gammaNode.empty() == false)
                {
                    pugi::xml_node gammaRNode = gammaNode.child("red");
                    pugi::xml_node gammaGNode = gammaNode.child("green");
                    pugi::xml_node gammaBNode = gammaNode.child("blue");
                    MetaString r, g, b;

                    if (!gammaRNode.empty())
                    {
                        r = gammaRNode.first_child().value();
                    }

                    if (!gammaGNode.empty())
                    {
                        g = gammaGNode.first_child().value();
                    }

                    if (!gammaBNode.empty())
                    {
                        b = gammaBNode.first_child().value();
                    }

                    values.gamma = GDIPPConfiguration::Values::Gamma(r, g, b);
                }

                if (hintingNode.empty() == false)
                {
                    values.hinting = Util::TryIntFromStr(hintingNode.first_child().value(), INT_MIN);
                }

                if (kerningNode.empty() == false)
                {
                    values.kerning = Util::TryIntFromStr(kerningNode.first_child().value(), INT_MIN);
                }

                if (renderModeNode.empty() == false)
                {
                    pugi::xml_node monoNode = renderModeNode.child("mono");
                    pugi::xml_node grayNode = renderModeNode.child("gray");
                    pugi::xml_node subpixelNode = renderModeNode.child("subpixel");
                    pugi::xml_node pixelGeometryNode = renderModeNode.child("pixel_geometry");
                    pugi::xml_node aliasedTextNode = renderModeNode.child("aliased_text");

                    MetaString mono, gray, subpixel;

                    if (!monoNode.empty())
                    {
                        mono = monoNode.first_child().value();
                    }

                    if (!grayNode.empty())
                    {
                        gray = grayNode.first_child().value();
                    }

                    if (!subpixelNode.empty())
                    {
                        subpixel = subpixelNode.first_child().value();
                    }

                    values.renderMode = GDIPPConfiguration::Values::RenderMode(mono, gray, subpixel);

                    if (!pixelGeometryNode.empty())
                    {
                        values.pixelGeometry 
                            = GDIPPConfiguration::Values::PixelGeometry(
                            pixelGeometryNode.first_child().value());
                    }

                    if (!aliasedTextNode.empty())
                    {
                        values.aliasedText = 
                            Util::TryIntFromStr(aliasedTextNode.first_child().value(), INT_MIN);
                    }
                }

                if (rendererNode.empty() == false)
                {
                    values.renderer = Util::TryIntFromStr(rendererNode.first_child().value(), INT_MIN);
                }

                if (shadowNode.empty() == false)
                {
                    pugi::xml_node offsetXNode = shadowNode.child("offset_x");
                    pugi::xml_node offsetYNode = shadowNode.child("offset_y");
                    pugi::xml_node alphaNode = shadowNode.child("alpha");

                    int offsetX, offsetY, alpha;

                    if (!offsetXNode.empty())
                    {
                        offsetX = Util::TryIntFromStr(offsetXNode.first_child().value(), INT_MIN);
                    }

                    if (!offsetYNode.empty())
                    {
                        offsetY = Util::TryIntFromStr(offsetYNode.first_child().value(), INT_MIN);
                    }

                    if (!alphaNode.empty())
                    {
                        alpha = Util::TryIntFromStr(alphaNode.first_child().value(), INT_MIN);
                    }

                    values.shadow = GDIPPConfiguration::Values::Shadow(offsetX, offsetY, alpha);
                }
            }
        }

        return values;
    }
} // namespace GDIPPConfiguration
