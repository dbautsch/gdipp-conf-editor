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

#include <windows.h>
#include <string>

#include "gdipp_configuration_values.h"
#include "local_types.h"

namespace GDIPPConfiguration
{
    class Values;
}

class GDIPPPreview;

namespace GDIPPConfigurationEditor
{
    class Application
    {
    public:
        Application(HINSTANCE hinstance,
                    TCHAR * cmdLine,
                    int showCmd);

        int Run();

    private:
        HINSTANCE thisInstance;
        HWND hwnd;
        MetaString cmdLine;
        int showCmd;
        GDIPPConfiguration::Values values;
        GDIPPPreview * preview;

        static INT_PTR MainDlgProc(HWND hwnd,
                                   UINT msg,
                                   WPARAM wparam,
                                   LPARAM lparam);

        MetaString GetGDIPPDirectory() const;
        GDIPPConfiguration::Values GetValuesFromControls() const;
        void ApplyValuesToControls(const GDIPPConfiguration::Values & values);
        void InitializeGUI();
        void ComboBoxById_AddString(int controlId, const MetaString & text);
        void ComboBoxById_SetCurSel(int controlId, int position);
        int ComboBoxById_GetCurSel(int controlId) const;
        void EditById_SetText(int controlId, const MetaString & text);
        MetaString EditById_GetText(int controlId) const;

        // application events
        void OnInitApplication();
        void OnShowMainWindow();
        void OnClose();
        void OnPaint();

        // button click events
        void OnClickSaveConfiguration();
        void OnClickUpdatePreview();
        void OnClickAbout();
        void OnClickHelp();
    };
} // namespace GDIPPConfigurationEditor
