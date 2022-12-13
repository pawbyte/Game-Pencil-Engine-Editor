/*
pawgui_settings.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 PAWGUI contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-PAWGUI <https://www.pawbyte.com/pawgui>
*/

#include "pawgui_settings.h"

namespace pawgui
{
    gui_settings * main_settings = nullptr;

    gui_settings::gui_settings()
    {
        defaultElementWidth = 192;
        defaultElementHeight = 24;
        useShortProjectNames = false;
        textAreaFindBox.x = 0;
        textAreaFindBox.y = 0;
        textAreaFindBox.w = 32;
        textAreaFindBox.h = 32;
        autoFindMouseTabs = true;

        textAreaDelayTime = 50;
        textInputDelayTime = 50;
        normalInputDelayTime = 100;
        cursorBlinkTime = 300;

        tabSpaceCount = 4;

        defaultLineCountWidth = 48;
        showTextEditorLineCount = true;
        showTextEditorSyntaxHightlight = true;


        fileOpenAudioDir = "";
        fileOpenanimationDir = "";
        fileOpenTextureDir = "";
        fileOpenTilesheetDir = "";
        fileOpenFontDir = "";
        fileOpenFunctionDir = "";
        fileOpenTextFileDir = "";
        fileOpenImportFileDir = "";
        fileSaveExportFileDir = "";
        fileSaveTextFileDir = "";
        fileOpenDefaultDir = "";
        fileSaveDefaultDir = "";
    }

    gui_settings::~gui_settings()
    {


    }
}
