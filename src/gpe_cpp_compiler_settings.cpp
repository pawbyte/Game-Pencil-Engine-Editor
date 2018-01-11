/*
gpe_cpp_compiler_settings.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

gameCPPCompilerSettingsResource::gameCPPCompilerSettingsResource(GPE_ResourceContainer * pFolder)
{
    resourceFileName = "";
    resourceName = "C++ Compiler Settings";//C++ Settings
    parentProjectName = "";
    parentProjectDirectory = "";

    editorPageList = new GPE_GuiElementList();
    editorPageList->barXPadding = GENERAL_GPE_PADDING;
    editorPageList->barXMargin = 0;

    editorPageTabBar = new GPE_TabBar();
    editorPageTabBar->set_coords(16, 16);
    editorPageTabBar->add_new_tab("Compiler Settings");
    editorPageTabBar->add_new_tab("Linker Settings");
    editorPageTabBar->add_new_tab("Search Directories");
    editorPageTabBar->add_new_tab("Toolchain");
    //editorPageTabBar->add_new_tab("Themes");
    //editorPageTabBar->add_new_tab("Other Settings");
    editorPageTabBar->open_tab(0);

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

void gameCPPCompilerSettingsResource::process_self(GPE_Rect * viewedSpace ,GPE_Rect * cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && editorPageList!=NULL && editorPageTabBar!=NULL && viewedSpace!=NULL)
    {
        /*std::string nExportOptionName = exportSettingsBar->get_selected_name();
        std::string nExportOptionHtml5 = projectHTML5SettingsTabBar->get_selected_name();*/
        subViewedSpace.x = editorPageTabBar->get_xpos()+viewedSpace->x;
        subViewedSpace.y = editorPageTabBar->get_y2pos()+viewedSpace->y;
        subViewedSpace.w = -editorPageTabBar->get_xpos()+viewedSpace->w;
        subViewedSpace.h = -editorPageTabBar->get_y2pos()+viewedSpace->h;
        int prevTab = editorPageTabBar->tabInUse;
        editorPageTabBar->process_self(viewedSpace,cam);
        if( prevTab!=editorPageTabBar->tabInUse)
        {
            editorPageList->reset_self();
        }
        editorPageList->clear_list();
        editorPageList->set_coords(0,0);
        editorPageList->set_width(subViewedSpace.w);
        editorPageList->set_height(subViewedSpace.h );
        editorPageList->barXPadding = GENERAL_GPE_PADDING;
        editorPageList->barXMargin = GENERAL_GPE_PADDING;
        int i = 0;
        if( editorPageTabBar->get_selected_name() == "Compiler Settings")
        {

        }
        else if( editorPageTabBar->get_selected_name() == "Linker Settings")
        {

        }
        else if( editorPageTabBar->get_selected_name() == "Search Directories" )
        {

        }
        else if( editorPageTabBar->get_selected_name() == "Toolchain" )
        {

        }
        editorPageList->add_gui_element(confirmResourceButton,false);
        editorPageList->add_gui_element(cancelResourceButton,true);
        editorPageList->process_self(&subViewedSpace,cam);
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


void gameCPPCompilerSettingsResource::render_self( GPE_Rect * viewedSpace , GPE_Rect * cam , bool forceRedraw )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && editorPageList!=NULL && editorPageTabBar!=NULL && viewedSpace!=NULL)
    {
        if( forceRedraw)
        {
            gpe->render_rectangle( 0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_THEME->Program_Color,false);
            editorPageTabBar->render_self( viewedSpace,cam,forceRedraw);
            gpe->render_rectangle( editorPageTabBar->get_xpos(),editorPageTabBar->get_y2pos(),viewedSpace->w,viewedSpace->h,GPE_MAIN_THEME->Program_Header_Color,false);
        }
        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint(&subViewedSpace);
        editorPageList->render_self( &subViewedSpace,cam, forceRedraw);
        /*
        if(editorPageTabBar->get_selected_name()=="Platforms")
        {
            exportSettingsBar->render_self( &subViewedSpace,cam,forceRedraw);
        }*/
        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint( viewedSpace);
    }
}


void gameCPPCompilerSettingsResource::save_resource(std::string alternatePath , int backupId )
{

}

bool gameCPPCompilerSettingsResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{

}
