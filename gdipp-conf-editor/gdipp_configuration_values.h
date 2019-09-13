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

#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "util.h"

namespace GDIPPConfiguration
{
    class Values
    {
    public:
        Values();

        class ValidationResult
        {
        public:
            bool status;
            std::vector<std::string> incorrectValues;
        };

        GDIPPConfiguration::Values::ValidationResult Validate() const;

        class AutoHintingMode
        {
        public:
            AutoHintingMode()
            {
                value = NotSet;
            }

            AutoHintingMode(const std::string & textValue)
            {
                std::stringstream str(textValue);
                int number;
                str >> number;

                if (number >= static_cast<int>(Disabled) &&
                    number <= static_cast<int>(ForceAutoHinting))
                {
                    value = static_cast<Modes>(number);
                }                
            }

            enum Modes
            {
                NotSet = -1,
                Disabled = 0,
                UseTTFByteCode = 1,
                ForceAutoHinting = 2
            };

            AutoHintingMode & operator=(AutoHintingMode::Modes mode)
            {
                value = mode;
                return * this;
            }

            bool operator==(const AutoHintingMode::Modes & mode) const
            {
                return value == mode;
            }

        private:
            Modes value;
        };

        class LCDFilter
        {
        public:
            LCDFilter()
            {
                value = NotSet;
            }

            LCDFilter(const std::string & textValue)
            {
                std::stringstream str(textValue);
                int number;
                str >> number;

                if (number == static_cast<int>(None) ||
                    number == static_cast<int>(Default) ||
                    number == static_cast<int>(Light) ||
                    number == static_cast<int>(Legacy))
                {
                    value = static_cast<Modes>(number);
                }    
            }

            enum Modes
            {
                NotSet = -1,
                None = 0,
                Default = 1,
                Light = 2,
                Legacy = 16
            };

            LCDFilter & operator=(LCDFilter::Modes mode)
            {
                value = mode;
                return * this;
            }

            bool operator==(LCDFilter::Modes mode) const
            {
                return value == mode;
            }

        private:
            Modes value;
        };

        class RenderMode
        {
        public:
            RenderMode()
            {
                monoMode = NotSet;
                grayMode = NotSet;
                subpixelMode = NotSet;
            }

            RenderMode(const std::string & monoMode,
                       const std::string & grayMode,
                       const std::string & subpixelMode)
            {
                int monoModeValue = Util::TryIntFromStr(monoMode, INT_MIN);
                int grayModeValue = Util::TryIntFromStr(grayMode, INT_MIN);
                int subpixelModeValue = Util::TryIntFromStr(subpixelMode, INT_MIN);

                if (Util::ValueInRange(monoModeValue, Disabled, Forced))
                {
                    this->monoMode = static_cast<Modes>(monoModeValue);
                }

                if (Util::ValueInRange(grayModeValue, Disabled, Forced))
                {
                    this->grayMode = static_cast<Modes>(grayModeValue);
                }

                if (Util::ValueInRange(subpixelModeValue, Disabled, Forced))
                {
                    this->subpixelMode = static_cast<Modes>(subpixelModeValue);
                }
            }

            enum Modes
            {
                NotSet = -1,
                Disabled = 0,
                Auto = 1,
                Forced = 2
            };

            Modes GetMonoMode() const
            {
                return monoMode;
            }

            Modes GetGrayMode() const
            {
                return grayMode;
            }

            Modes GetSubpixelMode() const
            {
                return subpixelMode;
            }

        private:
            Modes monoMode;
            Modes grayMode;
            Modes subpixelMode;
        };

        class Gamma
        {
        public:
            Gamma()
            {
                r = g = b = std::string("");
            }

            Gamma(const std::string & r,
                  const std::string & g,
                  const std::string & b)
                  : r(r), g(g), b(b)
            {

            }

            std::string GetR() const
            {
                return r;
            }

            std::string GetG() const
            {
                return g;
            }

            std::string GetB() const
            {
                return b;
            }

        private:
            std::string r, g, b;
        };


        class PixelGeometry
        {
        public:
            PixelGeometry()
            {
                value = NotSet;
            }

            PixelGeometry(const std::string & textValue)
            {
                std::stringstream str(textValue);
                int number;
                str >> number;

                if (number == static_cast<int>(RGB) ||
                    number == static_cast<int>(BGR))
                {
                    value = static_cast<Modes>(number);
                }
            }

            enum Modes
            {
                NotSet = -1,
                RGB = 0,
                BGR = 1
            };

            PixelGeometry & operator=(PixelGeometry::Modes mode)
            {
                value = mode;
                return * this;
            }

            bool operator==(PixelGeometry::Modes mode) const
            {
                return value == mode;
            }

        private:
            Modes value;
        };

        AutoHintingMode autoHintingMode;
        int embeddedBitmap;
        int embolden;
        LCDFilter lcdFilter;
        Gamma gamma;
        int hinting;
        int kerning;
        RenderMode renderMode;
        int renderer;
        PixelGeometry pixelGeometry;
    };
};
