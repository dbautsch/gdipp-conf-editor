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

#include "application.h"

#include "resource.h"

#include "gdipp_configuration_reader.h"
#include "gdipp_configuration_writer.h"
#include "gdipp_configuration_values.h"

#include <commctrl.h>
#include <windowsx.h>

#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' ""version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace GDIPPConfigurationEditor
{
    Application::Application(HINSTANCE hinstance,
                             LPSTR cmdLine,
                             int showCmd)
        :
            thisInstance(hinstance),
            cmdLine(cmdLine),
            showCmd(showCmd)
    {

    }

    int Application::Run()
    {
        INITCOMMONCONTROLSEX comctlInfo;

        memset(&comctlInfo, 0, sizeof(INITCOMMONCONTROLSEX));

        comctlInfo.dwSize = sizeof(INITCOMMONCONTROLSEX);
        comctlInfo.dwICC = ICC_WIN95_CLASSES | ICC_USEREX_CLASSES;

        if (InitCommonControlsEx(&comctlInfo) != TRUE)
        {
            DWORD e = GetLastError();
            return -1;
        }

        hwnd = CreateDialogParam(thisInstance,
                                 MAKEINTRESOURCE(IDD_MAIN),
                                 0,
                                 reinterpret_cast<DLGPROC>(MainDlgProc),
                                 reinterpret_cast<LPARAM>(this));

        ShowWindow(hwnd, showCmd);

        MSG msg;

        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return 0;
    }

    INT_PTR Application::MainDlgProc(HWND hwnd,
                                     UINT msg,
                                     WPARAM wparam,
                                     LPARAM lparam)
    {
        static Application * app = NULL;

        switch (msg)
        {
        case WM_SHOWWINDOW:
            {
                app->OnShowMainWindow();
                break;
            }
        case WM_INITDIALOG:
            {
                app = reinterpret_cast<Application*>(lparam);

                if (app == NULL)
                {
                    throw std::runtime_error("Unable to initialize application.");
                }

                app->OnInitApplication();

                break;
            }

        case WM_COMMAND:
            {
                switch (LOWORD(wparam))
                {
                case IDC_SAVE_CONFIGURATION:
                    {
                        app->OnClickSaveConfiguration();
                        break;
                    }
                case IDC_UPDATE_PREVIEW:
                    {
                        app->OnClickUpdatePreview();
                        break;
                    }

                case IDC_ABOUT:
                    {
                        app->OnClickAbout();
                        break;
                    }

                case IDC_HELP:
                    {
                        app->OnClickHelp();
                        break;
                    }
                }
                break;
            }

        case WM_CLOSE:
            {
                PostQuitMessage(0);
                break;
            }
        }

        return FALSE;
    }

    std::string Application::GetGDIPPDirectory() const
    {
        return std::string("C:\\Program Files (x86)\\gdipp");
    }

    GDIPPConfiguration::Values Application::GetValuesFromControls() const
    {
        GDIPPConfiguration::Values values;

        return values;
    }

    void Application::ApplyValuesToControls(const GDIPPConfiguration::Values & values)
    {

    }

    void Application::InitializeGUI()
    {
        /*
        *   Fill comboboxes & initialize other controls.
        */

        // autohinting
        ComboBoxById_AddString(IDC_AUTOHINTING, TEXT("FALSE"));
        ComboBoxById_AddString(IDC_AUTOHINTING, TEXT("TRUE"));

        // embedded bitmap
        ComboBoxById_AddString(IDC_EMBEDDED_BITMAP, TEXT("FALSE"));
        ComboBoxById_AddString(IDC_EMBEDDED_BITMAP, TEXT("TRUE"));

        // LCD filter
        ComboBoxById_AddString(IDC_LCDFILTER, TEXT("None"));
        ComboBoxById_AddString(IDC_LCDFILTER, TEXT("Default"));
        ComboBoxById_AddString(IDC_LCDFILTER, TEXT("Light"));
        ComboBoxById_AddString(IDC_LCDFILTER, TEXT("Legacy"));

        // Hinting
        ComboBoxById_AddString(IDC_HINTING, TEXT("0"));
        ComboBoxById_AddString(IDC_HINTING, TEXT("1"));
        ComboBoxById_AddString(IDC_HINTING, TEXT("2"));
        ComboBoxById_AddString(IDC_HINTING, TEXT("3"));

        // Kerning
        ComboBoxById_AddString(IDC_KERNING, TEXT("FALSE"));
        ComboBoxById_AddString(IDC_KERNING, TEXT("TRUE"));

        // Render mode (monochromatic)
        ComboBoxById_AddString(IDC_RENDERMODE_MONO, TEXT("Disabled"));
        ComboBoxById_AddString(IDC_RENDERMODE_MONO, TEXT("Auto"));
        ComboBoxById_AddString(IDC_RENDERMODE_MONO, TEXT("Forced"));

        // Render mode (gray)
        ComboBoxById_AddString(IDC_RENDERMODE_GRAYSCALE, TEXT("Disabled"));
        ComboBoxById_AddString(IDC_RENDERMODE_GRAYSCALE, TEXT("Auto"));
        ComboBoxById_AddString(IDC_RENDERMODE_GRAYSCALE, TEXT("Forced"));

        // Render mode (subpixel)
        ComboBoxById_AddString(IDC_RENDERMODE_SUBPIXEL, TEXT("Disabled"));
        ComboBoxById_AddString(IDC_RENDERMODE_SUBPIXEL, TEXT("Auto"));
        ComboBoxById_AddString(IDC_RENDERMODE_SUBPIXEL, TEXT("Forced"));

        // Pixel geometry
        ComboBoxById_AddString(IDC_PIXELGEOMETRY, TEXT("RGB"));
        ComboBoxById_AddString(IDC_PIXELGEOMETRY, TEXT("BGR"));

        // Aliased text
        ComboBoxById_AddString(IDC_ALIASEDTEXT, TEXT("FALSE"));
        ComboBoxById_AddString(IDC_ALIASEDTEXT, TEXT("TRUE"));
    }

    void Application::ComboBoxById_AddString(int controlId, const TCHAR * text)
    {
        HWND controlHwnd = GetDlgItem(hwnd, controlId);

        if (controlHwnd == NULL)
        {
            throw std::runtime_error("ListBoxById_AddString: control not found.");
        }

        ComboBox_AddString(controlHwnd, text);
    }

    // application events
    void Application::OnInitApplication()
    {
        // This method may terminate the whole application. Exceptions are catched in the main()
        // function by design.

        const std::string configurationDirectory = GetGDIPPDirectory();

        if (configurationDirectory.empty())
        {
            throw std::runtime_error("GDIPP is not installed. Please install GDIPP.");
        }

        GDIPPConfiguration::Reader reader(configurationDirectory + "\\gdipp_setting.xml");
        values = reader.GetValues();
    }

    void Application::OnShowMainWindow()
    {
        InitializeGUI();
        ApplyValuesToControls(values);
    }

    // button click events
    void Application::OnClickSaveConfiguration()
    {
        const std::string configurationDirectory = GetGDIPPDirectory();

        if (configurationDirectory.empty())
        {
            MessageBox(hwnd,
                       TEXT("GDIPP is not installed. Please install GDIPP."),
                       TEXT("GDIPP Configuration editor: Unable to save"),
                       MB_ICONERROR);
            return;
        }

        try
        {
            GDIPPConfiguration::Values values = GetValuesFromControls();
            GDIPPConfiguration::Writer writer(configurationDirectory + "\\gdipp_setting.xml");
            writer.Save(values);
        }
        catch (const std::exception & e)
        {
            MessageBox(hwnd,
                       e.what(),
                       TEXT("Unable to save configuration."),
                       MB_ICONERROR);
        }
    }

    void Application::OnClickUpdatePreview()
    {

    }

    void Application::OnClickAbout()
    {

    }

    void Application::OnClickHelp()
    {

    }
} // namespace GDIPPConfigurationEditor
