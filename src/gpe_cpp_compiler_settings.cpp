/*
gpe_cpp_compiler_settings.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_cpp_compiler_settings.h"

gameCPPCompilerSettingsResource * GPE_CPP_COMPILER_SETTINGS = NULL;

gameCPPCompilerSettingsResource::gameCPPCompilerSettingsResource(GPE_GeneralResourceContainer * pFolder)
{
    resourceFileName = "";
    resourceName = "C++ Compiler Settings";//C++ Settings
    parentProjectName = "";
    parentProjectDirectory = "";

    sideAreaPanel = new GPE_SelectBoxBasic("Mode");
    sideAreaPanel->set_width(160);
    sideAreaPanel->set_option_height(64);
    sideAreaPanel->add_option("Compiler Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/wrench.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Linker Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/link.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Search Directories",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/binoculars.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Toolchain",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/chain.png"),NULL,2, false, false);

    sidePanelRect = new GPE_Rect();

    editorPageList = new GPE_GuiElementList();
    editorPageList->barXPadding = GENERAL_GPE_PADDING;
    editorPageList->barXMargin = 0;

    subViewedSpace.x = 0;
    subViewedSpace.y = 0;
    subViewedSpace.w = SCREEN_WIDTH;
    subViewedSpace.h = SCREEN_HEIGHT;

}

gameCPPCompilerSettingsResource::~gameCPPCompilerSettingsResource()
{

}


void gameCPPCompilerSettingsResource::prerender_self( )
{

}

void gameCPPCompilerSettingsResource::preprocess_self(std::string alternatePath)
{

}

void gameCPPCompilerSettingsResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && editorPageList!=NULL && sideAreaPanel!=NULL && viewedSpace!=NULL)
    {
        int prevTab = sideAreaPanel->get_selection();
        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = viewedSpace->w;
            subViewedSpace.h = viewedSpace->h;
            PANEL_GENERAL_EDITOR->add_gui_element_fullsize( sideAreaPanel );
            PANEL_GENERAL_EDITOR->process_self();
        }
        else
        {
            sideAreaPanel->set_coords(  0,0  );
            sideAreaPanel->set_height( viewedSpace->h );
            sideAreaPanel->process_self( viewedSpace, cam );
            subViewedSpace.x = sideAreaPanel->get_x2pos();
            subViewedSpace.y = 0;
            subViewedSpace.w = viewedSpace->w - sideAreaPanel->get_width();
            subViewedSpace.h = viewedSpace->h;
        }

        if( prevTab!=sideAreaPanel->get_selection() )
        {
            editorPageList->reset_self();
        }

        editorPageList->clear_list();
        editorPageList->set_coords( subViewedSpace.x, subViewedSpace.y );
        editorPageList->set_width(subViewedSpace.w);
        editorPageList->set_height(subViewedSpace.h );
        editorPageList->barXPadding = GENERAL_GPE_PADDING;
        editorPageList->barXMargin = GENERAL_GPE_PADDING;
        int i = 0;
        if( sideAreaPanel->get_selected_name() == "Compiler Settings")
        {

        }
        else if( sideAreaPanel->get_selected_name() == "Linker Settings")
        {

        }
        else if( sideAreaPanel->get_selected_name() == "Search Directories" )
        {

        }
        else if( sideAreaPanel->get_selected_name() == "Toolchain" )
        {

        }
        editorPageList->add_gui_element(confirmResourceButton,false);
        editorPageList->add_gui_element(cancelResourceButton,true);
        editorPageList->process_self( viewedSpace,cam);
        if( confirmResourceButton->is_clicked() )
        {
            save_resource();
        }
        if( cancelResourceButton->is_clicked() )
        {
            preprocess_self();
        }
    }
}


void gameCPPCompilerSettingsResource::render_self( GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        if( sideAreaPanel!=NULL && PANEL_GENERAL_EDITOR==NULL )
        {
            sideAreaPanel->render_self( viewedSpace,cam, forceRedraw);
        }
        if( editorPageList!=NULL )
        {
            editorPageList->render_self( viewedSpace,cam, forceRedraw);
        }
    }
}


void gameCPPCompilerSettingsResource::save_resource(std::string alternatePath, int backupId )
{

}

bool gameCPPCompilerSettingsResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{

}
