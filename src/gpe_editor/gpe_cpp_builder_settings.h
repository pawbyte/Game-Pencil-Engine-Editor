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

#include "gpe_basic_resource_page.h"

class cppCompilerManager
{

};

class gpeCPPSubsytemHolder
{
    public:
    std::string subsystemName;
    GPE_Label_Text * systemNameLabel;
    gpe_text_widget_string * systemNameField;
    GPE_ToolLabelButton * removeSystemButton;

    GPE_Label_Text * systemProgramScriptLabel;
    gpe_text_widget_string * mainProgramFileScript;
    GPE_ToolLabelButton * mainProgramFileScriptButton;

    GPE_Label_Text * systemProgramTemplateFileLabel;
    gpe_text_widget_string * systemProgramTemplateFile;
    GPE_ToolLabelButton * systemProgramTemplateFileButton;

    gpeCPPSubsytemHolder(std::string gpeSubSystemName  = "");
    ~gpeCPPSubsytemHolder();
    bool add_to_list(GPE_GuiElementList * listIn);

};

class gpeCPPBuildHolder
{
    public:
    int subSystemToRemove;
    std::string builderName;
    GPE_Label_Text * builderNameLabel;
    gpe_text_widget_string * builderNameField;
    GPE_ToolLabelButton * addSubSystemButton;
    std::vector <gpeCPPSubsytemHolder *> mySubsystems;
    gpeCPPBuildHolder(std::string buildSystemName = "NewBuildHolder");
    ~gpeCPPBuildHolder();
    bool add_to_list(GPE_GuiElementList * listIn);
    gpeCPPSubsytemHolder * add_subsystem( std::string newSysName = "");
    bool clear_subsystems();
    gpeCPPSubsytemHolder * find_sub_system( std::string sysName );
    void process_self(); //no need for  viewspace/cam as it is processed within a context
    bool remove_subsystem_indexed(int subsystemPos, bool resetIdToRemove = false);
};

class gameCPPBuilderSettingsResource: public standardEditableGameResource
{
    public:
        std::string fileOpenDefaultScriptDir;
        std::string fileOpenDefaultTemplateDir;
        std::vector <gpeCPPBuildHolder *> cppBuildSystems;
        GPE_DropDown_Menu * cppBuildSystemsDropDown;
        GPE_SelectBoxBasic * sideAreaPanel;
        gpe::shape_rect * sidePanelRect;
        //Compiler Settings
        GPE_ToolLabelButton * addCPPBuildSystemButton;
        GPE_DropDown_Menu * compilerInUse;
        GPE_ToolLabelButton * defaultCompilerButton;
        GPE_ToolLabelButton * copyCompilerButton;
        GPE_ToolLabelButton * renameCompilerButton;
        GPE_ToolLabelButton * resetDefaultsCompilerButton;
        GPE_CheckBoxBasic * stopCompileOnError;
        gpe::shape_rect gpeLogoSpace;
        gpe::shape_rect subViewedSpace;
        GPE_GuiElementList * editorPageList;
        std::string projectFolderListLocation;
        gameCPPBuilderSettingsResource(GPE_GeneralResourceContainer * pFolder = NULL);
        ~gameCPPBuilderSettingsResource();
        gpeCPPBuildHolder * add_cpp_build_system( std::string newSysName = "");
        void clear_build_systems();
        gpeCPPBuildHolder * find_build_system( std::string sysName );
        bool include_local_files( std::string pBuildDir , int buildType );

        void prerender_self( );
        void load_resource(std::string file_path = "");
        void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
        void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
        void save_resource(std::string file_path = "", int backupId = -1);
        bool write_cpp_header_file(std::ofstream * fileTarget,std::string fName);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gameCPPBuilderSettingsResource * GPE_CPP_BUILDER_SETTINGS;

#endif
