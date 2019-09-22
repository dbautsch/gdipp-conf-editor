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

#include "commandline.h"

#include <iostream>

#include "../gdipp-conf-editor/util.h"

CommandLine::CommandLine()
{
    ParseCommands();
}

void CommandLine::ParseCommands()
{
    LPWSTR * commandsList = NULL;
    int argc;

    commandsList = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (commandsList == NULL)
    {
        throw std::runtime_error("Unable to obtain program arguments list.");
    }

    for (int i = 0; i < argc; ++i)
    {
        try
        {
            LPWSTR argument = commandsList[i];

            std::pair<MetaString, MetaString> programArgument = 
                SplitArgument(argument);

            commands.insert(programArgument);
        }
        catch (const std::exception & e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    LocalFree(commandsList);
}

MetaString CommandLine::Get(const MetaString & name) const
{
    CommandEntities::const_iterator it = commands.find(name);

    if (it == commands.end())
    {
        return MetaString();
    }

    return it->second;
}

std::pair<MetaString, MetaString> CommandLine::SplitArgument(LPWSTR argument) const
{
    std::pair<MetaString, MetaString> result;
    MetaString argumentStr = Util::CreateMetaString(argument);

    size_t equalSignPos = argumentStr.find(TEXT("="));

    if (equalSignPos == MetaString::npos)
    {
        // no equal sign, key is the same as the value
        // this is type of `flag` argument
        result = std::make_pair(Unquote(argumentStr), Unquote(argumentStr));
    }
    else
    {
        MetaString key = Unquote(argumentStr.substr(0, equalSignPos));
        MetaString value = Unquote(argumentStr.substr(equalSignPos + 1, argumentStr.length() - equalSignPos - 1));

        result = std::make_pair(key, value);
    }

    return result;
}

MetaString CommandLine::Unquote(const MetaString & text) const
{
    if (text[0] == TEXT('\"') && text[text.length() - 1] == TEXT('\"'))
    {
        // trim quotes
        return text.substr(1, text.length() - 2);
    }

    return text;
}
