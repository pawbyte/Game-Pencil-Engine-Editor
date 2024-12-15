/*
gpe_resource_tree.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_resource_tree.h"


GPE_ResourceTree::GPE_ResourceTree()
{
    selected_folder_option = nullptr;
    selected_option = nullptr;
    smallIconSize = 64;
    mediumIconSize = 96;
    largeIconSize = 128;
    xlIconSize = 256;

    treeGuiList = new pawgui::widget_panel_list();
    backbutton_option = new pawgui::widget_resource_container("","Back",gpe::resource_type_back_button,-1,false, -2, -1 );
    treeGuiList->lastColumnFloatsRight = false;
    options_position = 0;
    options_max_in_view = 0;
    treeMode = pawgui::tree_mode_list;
    upDelayTime = 0;
    downDelayTime = 0;
    leftDelayTime = 0;
    rightDelayTime = 0;

    widget_type = "resourcemangementbar";
    cameraBox.x = 0;
    cameraBox.y = 0;
    cameraBox.w = 128;
    cameraBox.h = 128;


    widget_box.x = 0;
    widget_box.y = 48;
    widget_box.w = 192;
    widget_box.h = 128;

    entireBox.x = 0;
    entireBox.y = 48;
    entireBox.w = 192;
    entireBox.h = 32;

    viewBox.x = 0;
    viewBox.y = 0;
    viewBox.w = 128;
    viewBox.h = 128;

    barXPadding = pawgui::padding_default;
    barYPadding = pawgui::padding_default;
    widget_box.h = gpe::screen_height-widget_box.y;
    subMenuIsOpen = true;
    selectedSubOption = -1;
    hasScrollControl = false;
    hasArrowkeyControl = false;
    barTitleWidth = 0;
    barTitleHeight = 24;
    gpe::font_default->get_metrics("Project Resources",&barTitleWidth,&barTitleHeight);
    barTitleHeight= 24;
    lastWidth = widget_box.w;
    searchBox = new pawgui::widget_input_text("","Search Resources...");
    treeModeBar = new pawgui::widget_button_iconbar( searchBox->get_height(), true );
    treeModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/list.png", "List", pawgui::tree_mode_list, false );
    treeModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/smile-o.png", "Icons - Small",pawgui::tree_mode_icons_small, true );
    treeModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/th.png", "Icons - Medium", pawgui::tree_mode_icons_medium, true );
    treeModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/th-large.png", "Icons - Large", pawgui::tree_mode_icons_large, true );
    treeModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/square.png", "Icons - Extra Large", pawgui::tree_mode_icons_xlarge, true );
}

GPE_ResourceTree::~GPE_ResourceTree()
{
    if( treeGuiList!=nullptr)
    {
        treeGuiList->clear_list();
        delete treeGuiList;
        treeGuiList = nullptr;
    }
    if( treeModeBar!=nullptr)
    {
        delete treeModeBar;
        treeModeBar = nullptr;
    }

    if( searchBox!=nullptr)
    {
        delete searchBox;
        searchBox = nullptr;
    }

    if( backbutton_option!=nullptr)
    {
        delete backbutton_option;
        backbutton_option = nullptr;
    }

}

void GPE_ResourceTree::add_resource_container( pawgui::widget_resource_container * new_resource )
{
    if( new_resource == nullptr)
    {
        return;
    }
    new_resource->element_box.x = widget_box.x;
    sub_options.push_back(new_resource);
}

pawgui::widget_resource_container * GPE_ResourceTree::add_resource_folder(int resourceType, std::string projFolderName, std::string resourceTypeName)
{
    pawgui::widget_resource_container * newResourceFolder = new pawgui::widget_resource_container(projFolderName,resourceTypeName,resourceType,-1,true,0,pawgui::restype_superfolder);
    newResourceFolder->element_box.x = widget_box.x;
    sub_options.push_back(newResourceFolder);
    return newResourceFolder;
}

pawgui::widget_resource_container * GPE_ResourceTree::add_project_folder(int resourceType, std::string projFolderName, std::string resourceTypeName)
{
    pawgui::widget_resource_container * newResourceFolder = new pawgui::widget_resource_container(projFolderName,resourceTypeName,resourceType,-1,true,0, pawgui::restype_projfolder);
    newResourceFolder->element_box.x = widget_box.x;
    sub_options.push_back(newResourceFolder);
    return newResourceFolder;
}

void GPE_ResourceTree::delete_project_resources(std::string projectFileName)
{
    remove_project_resources( projectFileName);
}

void GPE_ResourceTree::prerender_self( )
{
    pawgui::widget_resource_container * tSubOption= nullptr;
    for( int i = 0; i < (int)sub_options.size(); i++)
    {
        tSubOption = sub_options[i];
        if( tSubOption!=nullptr)
        {
            tSubOption->prerender_self( );
        }
    }
}

void GPE_ResourceTree::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find( view_space );
    cam = gpe::camera_find(cam);

    int current_res_id = -1;

    pawgui::widget_resource_container * past_resource_folder = selected_folder_option;
    if( pawgui::resource_last_clicked != nullptr )
    {
        current_res_id =  pawgui::resource_last_clicked->get_global_id();
    }

    /*widget_box.x = 0;
    widget_box.y = 0;*/
    pawgui::widget_basic::process_self( view_space, cam );

    if( view_space->w > treeModeBar->get_width() + 128 )
    {
        searchBox->set_coords( widget_box.x, widget_box.y );
        searchBox->set_width( view_space->w  - treeModeBar->get_width() );
        treeModeBar->set_coords( searchBox->get_x2(), widget_box.y );
    }
    else
    {
        searchBox->set_coords( widget_box.x, widget_box.y );
        searchBox->set_width( view_space->w );
        treeModeBar->set_coords( widget_box.x, widget_box.y + searchBox->get_y2() );
    }

    searchBox->process_self( view_space, cam );
    treeModeBar->process_self( view_space, cam );
    treeMode = treeModeBar->get_tab_id();

    if( widget_box.w == 0 || widget_box.h == 0 )
    {
        widget_box.w = view_space->w;
        widget_box.h = view_space->h;
    }

    if( treeMode == pawgui::tree_mode_list )
    {
        // treeMode == pawgui::tree_mode_list ( default)
        treeRowCount = ceil( widget_box.h / ( pawgui::resource_container_default_height + pawgui::padding_default ) );
        treeColumnCount = 1;
        past_resource_folder = nullptr;
        selected_folder_option = nullptr;
    }
    else
    {
        currentIconSize = smallIconSize;

        switch( treeMode )
        {
            case pawgui::tree_mode_icons_xlarge:
                currentIconSize = xlIconSize;
            break;

            case pawgui::tree_mode_icons_large:
                currentIconSize = largeIconSize;
            break;

            case pawgui::tree_mode_icons_medium:
                currentIconSize = mediumIconSize;
            break;

            default:
                currentIconSize = smallIconSize;
            break;
        }
        treeColumnCount = floor( widget_box.w / ( currentIconSize+pawgui::padding_default) );
        treeRowCount = floor( widget_box.h / ( currentIconSize+pawgui::padding_default) );

        if( currentIconSize > widget_box.w- pawgui::padding_default )
        {
            currentIconSize = widget_box.w - pawgui::padding_default;
            treeColumnCount = 1;
        }
    }


    options_max_in_view = treeColumnCount * treeRowCount;
    options_in_browser.clear();
    treeGuiList->clear_list();

    pawgui::widget_resource_container * cContainer = nullptr;

    int i_res = 0;

    if( searchBox->has_content() )
    {
        searchString = searchBox->get_string();
        searchString = stg_ex::string_lower( searchString );
    }
    else
    {
        searchString = "";
    }

    if( (int)searchString.size() > 0 )
    {
        if( selected_folder_option == nullptr )
        {
            for( i_res = 0; i_res < (int)sub_options.size(); i_res++ )
            {
                cContainer = sub_options[i_res];
                cContainer->add_if_contains_string( searchString, options_in_browser );
            }
        }
        else
        {
            selected_folder_option->add_if_contains_string( searchString, options_in_browser );
        }
    }
    else
    {
        if( treeMode == pawgui::tree_mode_list )
        {
            for( i_res = 0; i_res < (int)sub_options.size(); i_res++ )
            {
                cContainer = sub_options[i_res];
                cContainer->add_to_list( options_in_browser );
            }
        }
        else if( selected_folder_option != nullptr )
        {
            backbutton_option->add_to_list( options_in_browser );
            selected_folder_option->add_to_list(  options_in_browser, selected_folder_option );
        }
        else
        {
            for( i_res = 0; i_res < (int)sub_options.size(); i_res++ )
            {
                cContainer = sub_options[i_res];
                cContainer->add_to_list( options_in_browser );
            }
        }
    }

    treeGuiList->set_coords( widget_box.x, treeModeBar->get_y2()  );
    treeGuiList->set_width( widget_box.w );
    treeGuiList->set_height( widget_box.h - treeModeBar->get_y2()  );
    treeGuiList->barXPadding = pawgui::padding_default;
    treeGuiList->barYPadding = pawgui::padding_default;

    bool readyForNewRow = false;
    int current_column = 0;
    for( int i_options = 0; i_options < (int)options_in_browser.size(); i_options++ )
    {
        readyForNewRow = false;
        cContainer = options_in_browser[i_options];
        cContainer->treeMode = treeMode;
        if( treeMode == pawgui::tree_mode_list )
        {
            cContainer->set_width( widget_box.w );
            cContainer->set_height( pawgui::resource_container_default_height );
        }
        else
        {
            cContainer->set_width( currentIconSize );
            cContainer->set_height( currentIconSize );
        }
        current_column++;
        if( current_column >= treeColumnCount )
        {
            current_column = 0;
            readyForNewRow = true;
        }
        treeGuiList->add_gui_element( cContainer, readyForNewRow );
    }
    treeGuiList->process_self( view_space, cam );


    if( pawgui::resource_last_clicked != nullptr && treeMode != pawgui::tree_mode_list )
    {
        /*if( pawgui::resource_last_clicked->matches(past_resource_folder) != 1 )
        {
            gpe::input->reset_all_input();
            process_self(view_space, cam );
            return;
        }*/

        if( gpe::input->check_mouse_released( mb_left) && isHovered &&  pawgui::resource_last_clicked->is_clicked() )
        {
            pawgui::resource_dragged = nullptr;

            if( selected_folder_option != nullptr && pawgui::resource_last_clicked!= nullptr )
            {
                if( pawgui::resource_last_clicked->matches( backbutton_option) == 1 )
                {
                    if( past_resource_folder != nullptr )
                    {
                        selected_folder_option = past_resource_folder->parentResource;
                    }
                    else
                    {
                        selected_folder_option = selected_folder_option->parentResource;
                    }

                }
                else if( pawgui::resource_last_clicked->is_folder() || pawgui::resource_last_clicked->is_super_folder() || pawgui::resource_last_clicked->is_super_project_folder() )
                {
                    current_res_id = pawgui::resource_last_clicked->get_global_id();
                    selected_folder_option = pawgui::resource_last_clicked;
                }
            }
            else if( pawgui::resource_last_clicked->is_folder() || pawgui::resource_last_clicked->is_super_folder() || pawgui::resource_last_clicked->is_super_project_folder() )
            {
                current_res_id = pawgui::resource_last_clicked->get_global_id();
                selected_folder_option = pawgui::resource_last_clicked;
            }


            gpe::input->reset_all_input();
            backbutton_option->set_clicked( false );
            backbutton_option->set_hovered( false );
        }
    }
    else
    {
        selected_folder_option = nullptr;
    }

    int sOpNumber = -1;
    //Processes if a previous right click was made and if so, make context menu
    if( pawgui::resourcemenu_rightclicked && pawgui::resource_last_clicked!=nullptr )
    {
        pawgui::resourcemenu_rightclicked = false;
        pawgui::context_menu_open(gpe::input->mouse_position_x-256,gpe::input->mouse_position_y,256);
        if( !pawgui::resource_last_clicked->is_folder() && !pawgui::resource_last_clicked->is_super_project_folder() && !pawgui::resource_last_clicked->is_super_project_folder() )
        {
            int tempResType= pawgui::resource_last_clicked->get_resource_type() ;
            if( tempResType!=gpe::resource_type_texture && tempResType!=gpe::resource_type_tilesheet && tempResType!=gpe::resource_type_animation && tempResType!=gpe::resource_type_audio && tempResType!=gpe::resource_type_video && tempResType!=gpe::resource_type_project_settings )
            {
                pawgui::main_context_menu->add_menu_option("Duplicate Resource",-1,nullptr,-1,nullptr,false,true);
            }
            pawgui::main_context_menu->add_menu_option("Rename Resource",-1,nullptr,-1,nullptr,false,true);
            pawgui::main_context_menu->add_menu_option("Remove Resource",-1,nullptr,-1,nullptr,false,true);
            pawgui::main_context_menu->add_menu_option("Resource Properties",-1,nullptr,-1,nullptr,true,true);
        }
        else if(pawgui::resource_last_clicked->is_super_project_folder() )
        {
            pawgui::main_context_menu->add_menu_option("Save Project",-1,nullptr,-1,nullptr,false,true);
            pawgui::main_context_menu->add_menu_option("Close Project",-1,nullptr,-1,nullptr,true,true);

            pawgui::main_context_menu->add_menu_option("Build Project",-1,nullptr,-1,nullptr,false,true);
            //pawgui::main_context_menu->add_menu_option("Debug Project",-1,nullptr,-1,nullptr,false,true);
            pawgui::popup_menu_option *cleanFolderOptions =  pawgui::main_context_menu->add_menu_option("Clean Project",-1,nullptr,-1,nullptr,true,false);
            cleanFolderOptions->add_menu_option("Clean [HTML5] Build Folder",-1);
            cleanFolderOptions->add_menu_option("Clean [Windows] Build Folder",-1);
            cleanFolderOptions->add_menu_option("Clean [Linux] Build Folder",-1);
            cleanFolderOptions->add_menu_option("Clean [OSX] Build Folder",-1);
            cleanFolderOptions = pawgui::main_context_menu->add_menu_option("Open Project Directory");
            pawgui::main_context_menu->add_menu_option("Find Resource...");

            //pawgui::main_context_menu->add_menu_option("Browse Directory",-1,-1,nullptr,false,true);
        }
        else if(pawgui::resource_last_clicked->is_super_folder() || pawgui::resource_last_clicked->is_folder() )
        {
            pawgui::main_context_menu->add_menu_option("Add Resource",-1,nullptr,-1,nullptr,false,true);
            pawgui::main_context_menu->add_menu_option("Add Folder",-1,nullptr,-1,nullptr,true,true);
            if( !pawgui::resource_last_clicked->is_super_folder())
            {
                pawgui::main_context_menu->add_menu_option("Rename Folder",-1,nullptr,-1,nullptr,true,true);
                pawgui::main_context_menu->add_menu_option("Clear Resource Folder Contents",-1,nullptr,-1,nullptr,true,true);
            }
            pawgui::main_context_menu->add_menu_option("Sort by Name",-1,nullptr,-1,nullptr,true,true);
            if( !pawgui::resource_last_clicked->is_super_folder())
            {
                pawgui::main_context_menu->add_menu_option("Remove Folder",-1,nullptr,-1,nullptr,false,true);
            }
            pawgui::main_context_menu->add_menu_option("Find Resource...");
        }
        else if( pawgui::resource_last_clicked->is_folder()==false)
        {
            int tempResType= pawgui::resource_last_clicked->get_resource_type() ;
            if( tempResType!=gpe::resource_type_texture && tempResType!= gpe::resource_type_tilesheet && tempResType!= gpe::resource_type_animation && tempResType!= gpe::resource_type_audio && tempResType!= gpe::resource_type_video && tempResType!= gpe::resource_type_project_settings )
            {
                pawgui::main_context_menu->add_menu_option("Duplicate Resource",-1,nullptr,-1,nullptr,false,true);
            }
            pawgui::main_context_menu->add_menu_option("Rename Resource",-1,nullptr,-1,nullptr,false,true);
            pawgui::main_context_menu->add_menu_option("Delete Resource",-1,nullptr,-1,nullptr,false,true);
            pawgui::main_context_menu->add_menu_option("Properties",-1,nullptr,-1,nullptr,false,true);
        }
        pawgui::context_menu_process();
    }
    else if( gpe::input->check_mouse_released( mb_left)&&  pawgui::resource_dragged!=nullptr )
    {
        if( pawgui::resource_last_clicked!=nullptr)
        {
            if( pawgui::resource_last_clicked->is_super_folder() || pawgui::resource_last_clicked->is_folder() )
            {
                if( pawgui::resource_dragged->parentResource!=nullptr && pawgui::resource_last_clicked->can_obtain(pawgui::resource_dragged) )
                {
                    pawgui::resource_dragged->parentResource->remove_resource(pawgui::resource_dragged,false);
                    pawgui::resource_last_clicked->add_resource_container(pawgui::resource_dragged);
                    gpe::input->reset_all_input();
                    selectedSubOption = pawgui::resource_dragged->get_global_id();
                    pawgui::resource_dragged = nullptr;
                    pawgui::resource_last_clicked = nullptr;
                    process_self( view_space, cam);
                    return;
                }
            }
            else if( pawgui::resource_dragged->parentResource!=nullptr && pawgui::resource_last_clicked->parentResource!=nullptr && pawgui::resource_last_clicked->parentResource->can_obtain(pawgui::resource_dragged) )
            {
                if( pawgui::resource_dragged->matches(pawgui::resource_last_clicked)!=1  &&  pawgui::resource_dragged->matches(pawgui::resource_last_clicked->parentResource)!=1  )
                {
                    pawgui::widget_resource_container * previousParentResource = pawgui::resource_dragged->parentResource;
                    pawgui::resource_dragged->parentResource->remove_resource(pawgui::resource_dragged, false);
                    gpe::error_log->report("Trying to copy ["+pawgui::resource_dragged->get_name()+"] resource.");
                    gpe::error_log->report("Trying to copy to ["+pawgui::resource_last_clicked->get_name()+"]'s parent resource.");
                    gpe::error_log->report("Trying to copy to ["+pawgui::resource_last_clicked->parentResource->get_name()+"]'s resource.");
                    pawgui::resource_last_clicked->parentResource->add_resource_container(pawgui::resource_dragged,false,pawgui::resource_last_clicked);
                    selectedSubOption = pawgui::resource_dragged->get_global_id();
                    gpe::input->reset_all_input();
                    pawgui::resource_dragged = nullptr;
                    //pawgui::resource_last_clicked = nullptr;
                    process_self();
                    return;
                }
            }
        }
    }
}

void GPE_ResourceTree::render_self( gpe::shape_rect *view_space, gpe::shape_rect *cam)
{
    view_space = gpe::camera_find( view_space );
    cam = gpe::camera_find( cam );
    gpe::renderer_main->reset_viewpoint( );
    gpe::renderer_main->set_viewpoint( view_space);

    searchBox->render_self( view_space, cam );
    treeModeBar->render_self( view_space, cam );
    treeGuiList->render_self( view_space, cam );
    //return;

    gpe::renderer_main->reset_viewpoint( );
    gpe::renderer_main->set_viewpoint( nullptr);
    if( pawgui::theme_main->theme_texture_bg == nullptr)
    {
        //gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->panel_color,false);
    }

    if( hasScrollControl)
    {
        gpe::gcanvas->render_roundrect_filled_color( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+widget_box.h, pawgui::theme_main->button_box_highlight_color );
    }

    return;

    searchBox->render_self( view_space, cam );
    treeModeBar->render_self( view_space, cam );
    pawgui::widget_resource_container * cResource = nullptr;
    int xDrawPos = pawgui::padding_default;
    int yDrawPos = pawgui::padding_default;
    int optionSize = (int)options_in_browser.size();
    int i_max = std::min( optionSize, options_position + options_max_in_view );

    for( int i = options_position; i< i_max; i++ )
    {
        cResource = sub_options[i];
        if(cResource!=nullptr)
        {
            cResource->render_option(xDrawPos,yDrawPos,selectedSubOption, view_space,&cameraBox, true );
            yDrawPos+=cResource->element_box.h;
        }
    }

    gpe::renderer_main->set_viewpoint( nullptr );
    gpe::renderer_main->set_viewpoint( view_space );
    if( xScroll!=nullptr)
    {
        xScroll->render_self( view_space, cam );
    }
    if( yScroll!=nullptr)
    {
        yScroll->render_self(view_space, cam  );
        //if( RENDER_RESOURCEBAR_LEFT)
        {
            //gpe::gcanvas->render_rectangle( yScroll->widget_box.x+yScroll->widget_box.w,yScroll->widget_box.y,widget_box.x+widget_box.w,widget_box.h,barColor,false);
        }
    }

    gpe::renderer_main->reset_viewpoint( );
    gpe::renderer_main->set_viewpoint( nullptr);

}

void GPE_ResourceTree::remove_project_resources(std::string projectFileName)
{
    if( (int)projectFileName.size()>0 )
    {
        selected_option = nullptr; //To be safe we're going to just reset our selection option
        pawgui::widget_resource_container * tContainer = nullptr;
        for( int i = (int)sub_options.size()-1; i>=0; i--)
        {
            tContainer = sub_options[i];
            if( tContainer!=nullptr )
            {
                if( projectFileName.compare(tContainer->projectParentFileName )==0)
                {
                    sub_options.erase(sub_options.begin()+i);
                }
            }
        }
    }
}

