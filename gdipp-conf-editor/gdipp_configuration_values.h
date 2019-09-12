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
				value = NotSet;
			}

			RenderMode(const std::string & textValue)
			{
				std::stringstream str(textValue);
				int number;
				str >> number;

				if (number == static_cast<int>(Mono) ||
					number == static_cast<int>(Grayscale) ||
					number == static_cast<int>(Subpixel))
				{
					value = static_cast<Modes>(number);
				}
			}

			enum Modes
			{
				NotSet = -1,
				Mono = 0,
				Grayscale = 1,
				Subpixel = 2
			};

			RenderMode & operator=(RenderMode::Modes mode)
			{
				value = mode;
				return * this;
			}

			bool operator==(RenderMode::Modes mode) const
			{
				return value == mode;
			}

		private:
			Modes value;
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
		int embolden;
		LCDFilter lcdFilter;
		std::string gamma;
		int hinting;
		int kerning;
		RenderMode renderMode;
		PixelGeometry pixelGeometry;
	};
};
