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

#include "gdipp_configuration_writer.h"

#include "util.h"
#include "gdipp_configuration_values.h"

#include "pugixml/pugixml.hpp"

namespace GDIPPConfiguration
{
    Writer::Writer(const std::string & fileName)
        : fileName(fileName)
    {

    }

    void Writer::Save(const GDIPPConfiguration::Values & values)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(fileName.c_str());

        if (!result)
        {
            throw std::runtime_error("Unable to parse GDIPP configuration XML file.");
        }

        pugi::xml_node gdippNode = doc.child("gdipp");

        if (gdippNode.empty() == false)
        {
            pugi::xml_node freeTypeNode = gdippNode.child("gdimm").child("process").child("freetype");

            if (freeTypeNode.empty() == false)
            {
                pugi::xml_node lcdFilterNode = freeTypeNode.child("lcd_filter");

                if (lcdFilterNode.empty() == false)
                {
                    lcdFilterNode.first_child().set_value(Util::IntToStr(values.lcdFilter()).c_str());
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
                    autoHintingNode.first_child().set_value(Util::IntToStr(values.autoHintingMode()).c_str());
                }

                if (embeddedBitmapNode.empty() == false)
                {
                    embeddedBitmapNode.first_child().set_value(Util::IntToStr(values.embeddedBitmap).c_str());
                }

                if (emboldenNode.empty() == false)
                {
                    emboldenNode.first_child().set_value(Util::IntToStr(values.embolden).c_str());
                }

                if (gammaNode.empty() == false)
                {
                    pugi::xml_node gammaRNode = gammaNode.child("red");
                    pugi::xml_node gammaGNode = gammaNode.child("green");
                    pugi::xml_node gammaBNode = gammaNode.child("blue");
                    MetaString r, g, b;

                    if (!gammaRNode.empty())
                    {
                        gammaRNode.first_child().set_value(values.gamma.GetR().c_str());
                    }

                    if (!gammaGNode.empty())
                    {
                        gammaGNode.first_child().set_value(values.gamma.GetG().c_str());
                    }

                    if (!gammaBNode.empty())
                    {
                        gammaBNode.first_child().set_value(values.gamma.GetB().c_str());
                    }
                }

                if (hintingNode.empty() == false)
                {
                    hintingNode.first_child().set_value(Util::IntToStr(values.hinting).c_str());
                }

                if (kerningNode.empty() == false)
                {
                    kerningNode.first_child().set_value(Util::IntToStr(values.kerning).c_str());
                }

                if (renderModeNode.empty() == false)
                {
                    pugi::xml_node monoNode = renderModeNode.child("mono");
                    pugi::xml_node grayNode = renderModeNode.child("gray");
                    pugi::xml_node subpixelNode = renderModeNode.child("subpixel");
                    pugi::xml_node pixelGeometryNode = renderModeNode.child("pixel_geometry");
                    pugi::xml_node aliasedTextNode = renderModeNode.child("aliased_text");

                    if (!monoNode.empty())
                    {
                        monoNode.first_child().set_value(Util::IntToStr(values.renderMode.GetMonoMode()).c_str());
                    }

                    if (!grayNode.empty())
                    {
                        grayNode.first_child().set_value(Util::IntToStr(values.renderMode.GetGrayMode()).c_str());
                    }

                    if (!subpixelNode.empty())
                    {
                        subpixelNode.first_child().set_value(Util::IntToStr(values.renderMode.GetSubpixelMode()).c_str());
                    }

                    if (!pixelGeometryNode.empty())
                    {
                        pixelGeometryNode.first_child().set_value(Util::IntToStr(values.pixelGeometry()).c_str());
                    }

                    if (!aliasedTextNode.empty())
                    {
                        aliasedTextNode.first_child().set_value(Util::IntToStr(values.aliasedText).c_str());
                    }
                }

                if (rendererNode.empty() == false)
                {
                    rendererNode.first_child().set_value(Util::IntToStr(values.renderer).c_str());
                }

                if (shadowNode.empty() == false)
                {
                    pugi::xml_node offsetXNode = shadowNode.child("offset_x");
                    pugi::xml_node offsetYNode = shadowNode.child("offset_y");
                    pugi::xml_node alphaNode = shadowNode.child("alpha");

                    if (!offsetXNode.empty())
                    {
                        offsetXNode.first_child().set_value(Util::IntToStr(values.shadow.GetOffsetX()).c_str());
                    }

                    if (!offsetYNode.empty())
                    {
                        offsetYNode.first_child().set_value(Util::IntToStr(values.shadow.GetOffsetY()).c_str());
                    }

                    if (!alphaNode.empty())
                    {
                        alphaNode.first_child().set_value(Util::IntToStr(values.shadow.GetAlpha()).c_str());
                    }
                }
            }
        }

        if (doc.save_file(fileName.c_str()) == false)
        {
            throw std::runtime_error("Unable to save configuration XML file.");
        }
    }
} // namespace GDIPPConfiguration
