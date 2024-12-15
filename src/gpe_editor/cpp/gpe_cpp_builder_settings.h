/*
gpe_cpp_builder_settings.h
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
#ifndef GPE_CPP_COMPILER_RESOURCES_H
#define GPE_CPP_COMPILER_RESOURCES_H

#include "../resources/basic/gpe_basic_resource_page.h"

class cppCompilerManager
{

};

class gpeCPPSubsytemHolder
{
    public:
    std::string subsystemName;
    pawgui::widget_label_text  * systemNameLabel;
    pawgui::widget_input_text * systemNameField;
    pawgui::widget_button_label * removeSystem_button;

    pawgui::widget_label_text  * systemProgramScriptLabel;
    pawgui::widget_input_text * mainProgramFileScript;
    pawgui::widget_button_label * mainProgramFileScript_button;

    pawgui::widget_label_text  * systemProgramTemplateFileLabel;
    pawgui::widget_input_text * systemProgramTemplateFile;
    pawgui::widget_button_label * systemProgramTemplateFile_button;

    gpeCPPSubsytemHolder(std::string gpeSubSystemName  = "");
    ~gpeCPPSubsytemHolder();
    bool add_to_list(pawgui::widget_panel_list * listIn);

};

class gpeCPPBuildHolder
{
    public:
    int subSystemToRemove;
    std::string builderName;
    pawgui::widget_label_text  * builderNameLabel;
    pawgui::widget_input_text * builderNameField;
    pawgui::widget_button_label * addSubSystem_button;
    pawgui::widget_dropdown_menu * buildSystemOS;
    std::vector <gpeCPPSubsytemHolder *> mySubsystems;
    gpeCPPBuildHolder(std::string buildSystemName = "NewBuildHolder");
    ~gpeCPPBuildHolder();
    bool add_to_list(pawgui::widget_panel_list * listIn);
    gpeCPPSubsytemHolder * add_subsystem( std::string newSysName = "");
    bool clear_subsystems();
    gpeCPPSubsytemHolder * find_sub_system( std::string sysName );
    void process_self(); //no need for  viewspace/cam as it is processed within a context
    bool remove_subsystem_indexed(int subsystemPos, bool resetIdToRemove = false);
};

class gameCPPBuilder_settingsResource: public standardEditableGameResource
{
    public:
        std::string fileOpenDefaultScriptDir;
        std::string fileOpenDefaultTemplateDir;
        std::vector <gpeCPPBuildHolder *> cppBuildSystems;
        pawgui::widget_dropdown_menu * cppBuildSystemsDropDown;
        pawgui::widget_selectbox * sideAreaPanel;
        gpe::shape_rect * sidePanelRect;
        //Compiler _settings
        pawgui::widget_label_title * cppbuildSystemsLabel;
        pawgui::widget_button_label * addCPPBuildSystem_button;
        pawgui::widget_dropdown_menu * compilerInUse;
        pawgui::widget_button_label * defaultCompiler_button;
        pawgui::widget_button_label * copyCompiler_button;
        pawgui::widget_button_label * renameCompiler_button;
        pawgui::widget_button_label * resetDefaultsCompiler_button;
        pawgui::widget_checkbox * stopCompileOnError;
        gpe::shape_rect gpeLogoSpace;
        gpe::shape_rect subViewedSpace;
        pawgui::widget_panel_list * editorPageList;
        std::string projectFolderListLocation;
        gameCPPBuilder_settingsResource(pawgui::widget_resource_container * pFolder = nullptr);
        ~gameCPPBuilder_settingsResource();
        gpeCPPBuildHolder * add_cpp_build_system( std::string newSysName = "");
        void clear_build_systems();
        gpeCPPBuildHolder * find_build_system( std::string sysName );
        bool include_local_files( std::string pBuildDir , int buildType );

        void prerender_self( );
        void load_resource(std::string file_path = "");
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void save_resource(std::string file_path = "", int backupId = -1);
        bool write_cpp_header_file(std::ofstream * fileTarget,std::string f_name);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gameCPPBuilder_settingsResource * GPE_CPP_BUILDER_SETTINGS;

#endif
