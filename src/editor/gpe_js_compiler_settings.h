/*
gpe_js_compiler_settings.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef GPE_JS_COMPILER_RESOURCES_H
#define GPE_JS_COMPILER_RESOURCES_H


#include "gpe_basic_resource_page.h"

class gameJSCompilerSettingsResource: public standardEditableGameResource
{
public:
    GPE_SelectBoxBasic * sideAreaPanel;
    GPE_Rect * sidePanelRect;
    //Compiler Settings
    GPE_CheckBoxBasic * stopCompileOnError;
    //Added as of 1.15 [ BEGIN ]
    GPE_CheckBoxBasic * minifyCode;
    GPE_CheckBoxBasic * pluginConstantValues;
    GPE_CheckBoxBasic * obfuscateCode;
    GPE_TextInputBasic * obfuscatorDirectoryField;
    GPE_ToolIconButton * obfuscatorDirectoryLoadButton;
    GPE_TextInputBasic * googleClosureCompilerFile;
    GPE_ToolIconButton * googleClosureCompilerLoadButton;
    //Added as of 1.15 [ END ]
    GPE_Rect subViewedSpace;
    GPE_TabBar * editorPageTabBar;
    GPE_GuiElementList * editorPageList;
    std::string projectFolderListLocation;
    gameJSCompilerSettingsResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~gameJSCompilerSettingsResource();
    bool include_local_files( std::string pBuildDir , int buildType );
    void prerender_self( );
    void preprocess_self(std::string alternatePath = "");
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void save_resource(std::string alternatePath = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gameJSCompilerSettingsResource * GPE_JS_COMPILER_SETTINGS;

#endif
