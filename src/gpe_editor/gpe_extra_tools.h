
/*
gpe_extra_tools.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef GPE_EXTRA_TOOLS_H
#define GPE_EXTRA_TOOLS_H

#include "gpe_basic_resource_page.h"

class atlas_data_viewer
{
private:
        GPE_Label_Title * atlas_title;
        GPE_Label_Text * atlas_text_animation;
        GPE_Label_Text * atlas_text_music;
        GPE_Label_Text * atlas_text_sounds;
        GPE_Label_Text * atlas_text_textures;
        GPE_Label_Text * atlas_text_tilesheets;
    public:
        atlas_data_viewer( gpe::asset_manager * manager );
        ~atlas_data_viewer();
        void add_to_list( GPE_GuiElementList * element_list );
};

class gamePencilExtraTools: public generalGameResource
{
    private:
        std::vector< atlas_data_viewer * >atlas_list;
    public:
        GPE_SelectBoxBasic * sideAreaPanel;
        GPE_Label_Title * sectionTitleExtras;
        GPE_ToolLabelButton * refreshAtlasRefreshButton;
        GPE_ToolPushButton * transformImageDirectoryButton;//Editor Related variables
        gpe::shape_rect subViewedSpace;
        gpe::shape_rect * sidePanelRect;
        GPE_GuiElementList * editorPageList;
        std::string projectFolderListLocation;
        gamePencilExtraTools();
        ~gamePencilExtraTools();
        bool include_local_files( std::string pBuildDir , int buildType );
        int modify_folder_images(std::string folderLocation, int modifcationType);

        void prerender_self( );
        void load_resource(std::string file_path = "");
        void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
        void refresh_atlas_info();
        void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
        void save_resource(std::string file_path = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gamePencilExtraTools * main_EXTRA_TOOLS;
#endif // GPE_EXTRA_TOOLS_H
