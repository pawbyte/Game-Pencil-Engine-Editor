/*
gpe_js_compiler_settings.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#ifndef GPE_JS_COMPILER_RESOURCES_H
#define GPE_JS_COMPILER_RESOURCES_H


#include "../resources/basic/gpe_basic_resource_page.h"


class gameJSCompiler_settingsResource: public standardEditableGameResource
{
public:
    pawgui::widget_selectbox * sideAreaPanel;
    gpe::shape_rect * sidePanelRect;
    //Compiler _settings
    pawgui::widget_checkbox * stopCompileOnError;
    //Added as of 1.15 [ BEGIN ]
    pawgui::widget_checkbox * minifyCode;
    pawgui::widget_checkbox * pluginConstantValues;
    pawgui::widget_checkbox * obfuscateCode;
    pawgui::widget_input_text * obfuscatorDirectoryField;
    pawgui::widget_button_icon * obfuscatorDirectoryLoad_button;
    pawgui::widget_input_text * googleClosureCompilerFile;
    pawgui::widget_button_icon * googleClosureCompilerLoad_button;
    //Added as of 1.15 [ END ]
    gpe::shape_rect subViewedSpace;
    pawgui::widget_tabbar * editorPageTabBar;
    pawgui::widget_panel_list * editorPageList;
    std::string projectFolderListLocation;
    gameJSCompiler_settingsResource(pawgui::widget_resource_container * pFolder = nullptr);
    ~gameJSCompiler_settingsResource();
    bool include_local_files( std::string pBuildDir , int buildType );
    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gameJSCompiler_settingsResource * GPE_JS_COMPILER_SETTINGS;

#endif
