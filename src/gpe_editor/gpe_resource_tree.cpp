/*
gpe_resource_tree.cpp
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

#include "gpe_resource_tree.h"


GPE_ResourceTree::GPE_ResourceTree()
{
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
    xScroll = new pawgui::widget_scrollbar_xaxis();
    yScroll = new pawgui::widget_scrollbar_yaxis();
    barTitleWidth = 0;
    barTitleHeight = 24;
    gpe::font_default->get_metrics("Project Resources",&barTitleWidth,&barTitleHeight);
    barTitleHeight= 24;
    lastWidth = widget_box.w;
}

GPE_ResourceTree::~GPE_ResourceTree()
{
    if( xScroll!=NULL)
    {
        delete xScroll;
        xScroll = NULL;
    }

    if( yScroll!=NULL)
    {
        delete yScroll;
        yScroll = NULL;
    }

}

void GPE_ResourceTree::add_resource_container( pawgui::widget_resource_container * new_resource )
{
    if( new_resource == NULL)
    {
        return;
    }
    new_resource->element_box.x = widget_box.x;
    subOptions.push_back(new_resource);
}

pawgui::widget_resource_container * GPE_ResourceTree::add_resource_folder(int resourceType, std::string projFolderName, std::string resourceTypeName)
{
    pawgui::widget_resource_container * newResourceFolder = new pawgui::widget_resource_container(projFolderName,resourceTypeName,resourceType,-1,true,0,pawgui::restype_superfolder);
    newResourceFolder->element_box.x = widget_box.x;
    subOptions.push_back(newResourceFolder);
    return newResourceFolder;
}

pawgui::widget_resource_container * GPE_ResourceTree::add_project_folder(int resourceType, std::string projFolderName, std::string resourceTypeName)
{
    pawgui::widget_resource_container * newResourceFolder = new pawgui::widget_resource_container(projFolderName,resourceTypeName,resourceType,-1,true,0, pawgui::restype_projfolder);
    newResourceFolder->element_box.x = widget_box.x;
    subOptions.push_back(newResourceFolder);
    return newResourceFolder;
}

void GPE_ResourceTree::delete_project_resources(std::string projectFileName)
{
    remove_project_resources( projectFileName);
}

void GPE_ResourceTree::prerender_self( )
{
    pawgui::widget_resource_container * tSubOption= NULL;
    for( int i = 0; i < (int)subOptions.size(); i++)
    {
        tSubOption = subOptions[i];
        if( tSubOption!=NULL)
        {
            tSubOption->prerender_self( );
        }
    }
}

void GPE_ResourceTree::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find( view_space );
    cam = gpe::camera_find(cam);

    /*widget_box.x = 0;
    widget_box.y = 0;*/
    pawgui::widget_basic::process_self( view_space, cam );

    int sOpNumber = -1;

    bool mouseInRange = false;
    cameraBox.w = widget_box.w - yScroll->get_box_width();
    cameraBox.h = widget_box.h - xScroll->get_box_height();

    int xPos = 0;
    int yPos = 0;
    int y2Pos = yPos;

    if( isHovered )
    {
        mouseInRange = true;
    }
    if( gpe::input->check_mouse_pressed(0) || gpe::input->check_mouse_pressed(1) || gpe::input->check_mouse_pressed(2) )
    {
        if( mouseInRange)
        {
            hasScrollControl = true;
            hasArrowkeyControl = true;
        }
        else
        {
            hasScrollControl = false;
            hasArrowkeyControl = false;
        }
    }

    //if( mouseInRange || menuResized || resourcebarMoved)
    {
        entireBox.x = 0;
        entireBox.y = 0;
        entireBox.w = cameraBox.w - pawgui::padding_default;
        entireBox.h = pawgui::resource_container_default_height*3;
        pawgui::widget_resource_container * cContainer = NULL;

        viewBox.x = widget_box.x + view_space->x - cam->x;
        viewBox.y = widget_box.y + view_space->y - cam->y;
        viewBox.w = cameraBox.w;
        viewBox.h = cameraBox.h;

        for(int i=0; i<(int)subOptions.size(); i++)
        {
            cContainer = subOptions[i];
            if(cContainer!=NULL)
            {
                sOpNumber=cContainer->process_container(xPos,y2Pos,selectedSubOption, &viewBox,&cameraBox,mouseInRange);
                if( sOpNumber>=0)
                {
                    selectedSubOption = sOpNumber;
                }
                entireBox.h+=cContainer->element_box.h;
                y2Pos+=cContainer->element_box.h;
                if( cContainer->foundX2Pos > entireBox.w)
                {
                    entireBox.w = cContainer->foundX2Pos;
                }
            }
        }
    }
    showYScroll = true;
    //Xscroll code

    if( xScroll!=NULL && yScroll!=NULL)
    {
        xScroll->set_coords( widget_box.x, widget_box.y+widget_box.h - xScroll->get_height() );
        xScroll->set_width( widget_box.w - yScroll->get_width() );
        xScroll->fullRect.x = 0;
        xScroll->fullRect.y = 0;
        xScroll->fullRect.w = entireBox.w;
        xScroll->fullRect.h = entireBox.h;

        //if( hasScrollControl && gpe::input->check_kb_down(kb_ctrl) )
        if( mouseInRange && gpe::input->check_kb_down(kb_ctrl) )
        {
            if( gpe::input->mouse_scrolling_up)
            {
                cameraBox.x-=cameraBox.w/8;
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                cameraBox.x+=cameraBox.w/8;
            }
        }

        xScroll->contextRect.x = cameraBox.x;
        xScroll->contextRect.y = cameraBox.y;
        xScroll->contextRect.w = cameraBox.w;
        xScroll->contextRect.h = cameraBox.h;

        if( hasScrollControl)
        {
            if( gpe::input->check_kb_down(kb_left) && !gpe::input->check_kb_pressed(kb_left) )
            {
                leftDelayTime++;
            }
            else if( gpe::input->check_kb_down(kb_right)  && !gpe::input->check_kb_pressed(kb_right) )
            {
                rightDelayTime++;
            }

            if( leftDelayTime > pawgui::main_settings->normalInputDelayTime || gpe::input->check_kb_pressed(kb_left) )
            {
                xScroll->contextRect.x-=cameraBox.w/8;
                leftDelayTime = 0;
            }
            else if( rightDelayTime > pawgui::main_settings->normalInputDelayTime || gpe::input->check_kb_pressed(kb_right) )
            {
                xScroll->contextRect.x+=cameraBox.w/8;
                rightDelayTime = 0;
            }
        }
        xScroll->process_self();
        if( xScroll->has_moved() || xScroll->is_scrolling() )
        {
            cameraBox.x = xScroll->contextRect.x;
            if( cameraBox.x +cameraBox.w > entireBox.w)
            {
                cameraBox.x = entireBox.w - cameraBox.w;
            }
            if( cameraBox.x < 0)
            {
                cameraBox.x = 0;
            }
            xScroll->process_self();
        }

        yScroll->set_coords( widget_box.x+widget_box.w - yScroll->get_width(), widget_box.y );
        yScroll->set_height( widget_box.h - xScroll->get_height() );

        yScroll->fullRect.x = 0;
        yScroll->fullRect.y = 0;
        yScroll->fullRect.w = entireBox.w,
        yScroll->fullRect.h = entireBox.h;

        yScroll->contextRect.x = cameraBox.x;
        yScroll->contextRect.y = cameraBox.y;
        yScroll->contextRect.w = cameraBox.w,
        yScroll->contextRect.h = cameraBox.h;

        //if( hasScrollControl && gpe::input->check_kb_down(kb_ctrl)==false )
        if( mouseInRange && gpe::input->check_kb_down(kb_ctrl)==false )
        {
            if( gpe::input->mouse_scrolling_up)
            {
                yScroll->contextRect.y-=cameraBox.h/8;
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                yScroll->contextRect.y+=cameraBox.h/8;
            }
        }

        if( hasScrollControl)
        {
            if( gpe::input->check_kb_down(kb_up) && !gpe::input->check_kb_pressed(kb_up) )
            {
                upDelayTime++;
            }
            else if( gpe::input->check_kb_down(kb_down)  && !gpe::input->check_kb_pressed(kb_down) )
            {
                downDelayTime++;
            }

            if( upDelayTime > pawgui::main_settings->normalInputDelayTime || gpe::input->check_kb_pressed(kb_up) )
            {
                yScroll->contextRect.y-=cameraBox.h/8;
                upDelayTime = 0;
            }
            else if( downDelayTime > pawgui::main_settings->normalInputDelayTime || gpe::input->check_kb_pressed(kb_down) )
            {
                yScroll->contextRect.y+=cameraBox.h/8;
                downDelayTime = 0;
            }
        }
        yScroll->process_self( view_space, cam );
        //if( yScroll->has_moved() || yScroll->is_scrolling() || hasScrollControl)
        if( yScroll->has_moved() || yScroll->is_scrolling() || mouseInRange)
        {
            cameraBox.y = yScroll->contextRect.y;
            if( cameraBox.y +cameraBox.h> entireBox.h)
            {
                cameraBox.y = entireBox.h - cameraBox.h;
            }
            if( cameraBox.y < 0)
            {
                cameraBox.y = 0;
            }
            yScroll->process_self( view_space, cam );
        }
    }

    //Processes if a previous right click was made and if so, make context menu
    if( pawgui::resourcemenu_rightclicked && pawgui::resource_last_clicked!=NULL)
    {
        pawgui::resourcemenu_rightclicked = false;
        pawgui::context_menu_open(gpe::input->mouse_position_x-256,gpe::input->mouse_position_y,256);
        if( !pawgui::resource_last_clicked->is_folder() && !pawgui::resource_last_clicked->is_super_project_folder() && !pawgui::resource_last_clicked->is_super_project_folder() )
        {
            int tempResType= pawgui::resource_last_clicked->get_resource_type() ;
            if( tempResType!=gpe::resource_type_texture && tempResType!=gpe::resource_type_tilesheet && tempResType!=gpe::resource_type_animation && tempResType!=gpe::resource_type_audio && tempResType!=gpe::resource_type_video && tempResType!=gpe::resource_type_project_settings )
            {
                pawgui::main_context_menu->add_menu_option("Duplicate Resource",-1,NULL,-1,NULL,false,true);
            }
            pawgui::main_context_menu->add_menu_option("Rename Resource",-1,NULL,-1,NULL,false,true);
            pawgui::main_context_menu->add_menu_option("Remove Resource",-1,NULL,-1,NULL,false,true);
            pawgui::main_context_menu->add_menu_option("Resource Properties",-1,NULL,-1,NULL,true,true);
        }
        else if(pawgui::resource_last_clicked->is_super_project_folder() )
        {
            pawgui::main_context_menu->add_menu_option("Save Project",-1,NULL,-1,NULL,false,true);
            pawgui::main_context_menu->add_menu_option("Close Project",-1,NULL,-1,NULL,true,true);

            pawgui::main_context_menu->add_menu_option("Build Project",-1,NULL,-1,NULL,false,true);
            //pawgui::main_context_menu->add_menu_option("Debug Project",-1,NULL,-1,NULL,false,true);
            pawgui::popup_menu_option *cleanFolderOptions =  pawgui::main_context_menu->add_menu_option("Clean Project",-1,NULL,-1,NULL,true,false);
            cleanFolderOptions->add_menu_option("Clean [HTML5] Build Folder",-1);
            cleanFolderOptions->add_menu_option("Clean [Windows] Build Folder",-1);
            cleanFolderOptions->add_menu_option("Clean [Linux] Build Folder",-1);
            cleanFolderOptions->add_menu_option("Clean [OSX] Build Folder",-1);
            cleanFolderOptions = pawgui::main_context_menu->add_menu_option("Open Project Directory");
            pawgui::main_context_menu->add_menu_option("Find Resource...");

            //pawgui::main_context_menu->add_menu_option("Browse Directory",-1,-1,NULL,false,true);
        }
        else if(pawgui::resource_last_clicked->is_super_folder() || pawgui::resource_last_clicked->is_folder() )
        {
            pawgui::main_context_menu->add_menu_option("Add Resource",-1,NULL,-1,NULL,false,true);
            pawgui::main_context_menu->add_menu_option("Add Folder",-1,NULL,-1,NULL,true,true);
            if( !pawgui::resource_last_clicked->is_super_folder())
            {
                pawgui::main_context_menu->add_menu_option("Rename Folder",-1,NULL,-1,NULL,true,true);
                pawgui::main_context_menu->add_menu_option("Clear Resource Folder Contents",-1,NULL,-1,NULL,true,true);
            }
            pawgui::main_context_menu->add_menu_option("Sort by Name",-1,NULL,-1,NULL,true,true);
            if( !pawgui::resource_last_clicked->is_super_folder())
            {
                pawgui::main_context_menu->add_menu_option("Remove Folder",-1,NULL,-1,NULL,false,true);
            }
            pawgui::main_context_menu->add_menu_option("Find Resource...");
        }
        else if( pawgui::resource_last_clicked->is_folder()==false)
        {
            int tempResType= pawgui::resource_last_clicked->get_resource_type() ;
            if( tempResType!=gpe::resource_type_texture && tempResType!= gpe::resource_type_tilesheet && tempResType!= gpe::resource_type_animation && tempResType!= gpe::resource_type_audio && tempResType!= gpe::resource_type_video && tempResType!= gpe::resource_type_project_settings )
            {
                pawgui::main_context_menu->add_menu_option("Duplicate Resource",-1,NULL,-1,NULL,false,true);
            }
            pawgui::main_context_menu->add_menu_option("Rename Resource",-1,NULL,-1,NULL,false,true);
            pawgui::main_context_menu->add_menu_option("Delete Resource",-1,NULL,-1,NULL,false,true);
            pawgui::main_context_menu->add_menu_option("Properties",-1,NULL,-1,NULL,false,true);
        }
        pawgui::context_menu_process();
    }
    else if( gpe::input->check_mouse_released( mb_left)&&  pawgui::resource_dragged!=NULL )
    {
        if( pawgui::resource_last_clicked!=NULL)
        {
            if( pawgui::resource_last_clicked->is_super_folder() || pawgui::resource_last_clicked->is_folder() )
            {
                if( pawgui::resource_dragged->parentResource!=NULL && pawgui::resource_last_clicked->can_obtain(pawgui::resource_dragged) )
                {
                    pawgui::resource_dragged->parentResource->remove_resource(pawgui::resource_dragged,false);
                    pawgui::resource_last_clicked->add_resource_container(pawgui::resource_dragged);
                    gpe::input->reset_all_input();
                    selectedSubOption = pawgui::resource_dragged->get_global_id();
                    pawgui::resource_dragged = NULL;
                    pawgui::resource_last_clicked = NULL;
                    process_self();
                }
            }
            else if( pawgui::resource_dragged->parentResource!=NULL && pawgui::resource_last_clicked->parentResource!=NULL && pawgui::resource_last_clicked->parentResource->can_obtain(pawgui::resource_dragged) )
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
                    pawgui::resource_dragged = NULL;
                    pawgui::resource_last_clicked = NULL;
                    process_self();
                }
            }
            pawgui::resource_last_clicked= NULL;
        }
    }
}

void GPE_ResourceTree::render_self( gpe::shape_rect *view_space, gpe::shape_rect *cam)
{
    view_space = gpe::camera_find( view_space );
    cam = gpe::camera_find( cam );
    gpe::renderer_main->reset_viewpoint( );
    gpe::renderer_main->set_viewpoint( view_space);

    if( pawgui::theme_main->theme_texture_bg == NULL)
    {
        gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->program_color,false);
    }

    pawgui::widget_resource_container * cResource = NULL;
    int xDrawPos = 0;
    int yDrawPos = 0;
    int optionSize = (int)subOptions.size();
    for(int i=0; i< optionSize; i++ )
    {
        cResource = subOptions[i];
        if(cResource!=NULL)
        {
            cResource->render_option(xDrawPos,yDrawPos,selectedSubOption, view_space,&cameraBox, true );
            yDrawPos+=cResource->element_box.h;
        }
    }

    gpe::renderer_main->set_viewpoint( NULL );
    gpe::renderer_main->set_viewpoint( view_space );
    if( xScroll!=NULL)
    {
        xScroll->render_self( view_space, cam );
    }
    if( yScroll!=NULL)
    {
        yScroll->render_self(view_space, cam  );
        //if( RENDER_RESOURCEBAR_LEFT)
        {
            //gpe::gcanvas->render_rectangle( yScroll->widget_box.x+yScroll->widget_box.w,yScroll->widget_box.y,widget_box.x+widget_box.w,widget_box.h,barColor,false);
        }
    }

    if( hasScrollControl)
    {
        gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+widget_box.h,pawgui::theme_main->button_box_highlight_color,true);
    }
    else
    {
        gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->text_box_outline_color,true);
    }

    gpe::renderer_main->reset_viewpoint( );
    gpe::renderer_main->set_viewpoint( NULL);

}

void GPE_ResourceTree::remove_project_resources(std::string projectFileName)
{
    if( (int)projectFileName.size()>0 )
    {
        pawgui::widget_resource_container * tContainer = NULL;
        for( int i = (int)subOptions.size()-1; i>=0; i--)
        {
            tContainer = subOptions[i];
            if( tContainer!=NULL )
            {
                if( projectFileName.compare(tContainer->projectParentFileName )==0)
                {
                    subOptions.erase(subOptions.begin()+i);
                }
            }
        }
    }
}

