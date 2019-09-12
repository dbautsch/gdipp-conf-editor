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

#include <exception>

namespace GDIPPConfiguration
{
	Reader::Reader(const std::string fileName)
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

		pugi::xml_node gdippNode = doc.child("gdipp");

		if (gdippNode.empty() == false)
		{
			pugi::xml_node freeTypeNode = gdippNode.child("gdimm").child("process").child("freetype");

			if (freeTypeNode.empty() == false)
			{
				pugi::xml_node lcdFilterNode = freeTypeNode.child("lcd_filter");

				if (lcdFilterNode.empty() == false)
				{
					
				}
			}
		}

		return values;
	}
} // namespace GDIPPConfiguration
