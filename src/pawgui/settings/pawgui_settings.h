/*
pawgui_settings.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PAWGUI contributors ( Contributors Page )

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

#ifndef PAWGUI_SETTINGS_H
#define PAWGUI_SETTINGS_H

#include "../gpe/gpe_shapes.h"
#include <string>

namespace pawgui
{
    class gui_settings
    {
        public:
            int defaultElementWidth;
            int defaultElementHeight;
            gpe::shape_rect textAreaFindBox;
            bool useShortProjectNames;
            //Text Editor Related _settings [ BEGIN ]
            float textInputDelayTime;
            float normalInputDelayTime;
            float textAreaDelayTime;
            float cursorBlinkTime;
            bool autoFindMouseTabs;
            int tabSpaceCount;

            bool showTextEditorLineCount;
            bool showTextEditorSyntaxHightlight;
            int defaultLineCountWidth;
            //Text Editor Related _settings [ END ]
            //Default Folder Related _settings [ BEGIN ]
            std::string fileOpenAudioDir;
            std::string fileOpenVideoDir;
            std::string fileOpenanimationDir;
            std::string fileOpenTextureDir;
            std::string fileOpenTilesheetDir;
            std::string fileOpenFontDir;
            std::string fileOpenFunctionDir;
            std::string fileOpenTextFileDir;
            std::string fileOpenImportFileDir;
            std::string fileSaveExportFileDir;

            std::string fileSaveTextFileDir;
            std::string fileOpenDefaultDir;
            std::string fileSaveDefaultDir;
            //Default Folder Related_settings [ END ]
            //std::string
            gui_settings();
            virtual ~gui_settings();
    };
    extern gui_settings * main_settings;
}

#endif //PAWGUI_SETTINGS_H
