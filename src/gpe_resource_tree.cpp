/*
gpe_resource_tree.cpp
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

#include "gpe_resource_tree.h"


GPE_ResourceTree::GPE_ResourceTree()
{
    upDelayTime = 0;
    downDelayTime = 0;
    leftDelayTime = 0;
    rightDelayTime = 0;

    guiListTypeName = "resourcemangementbar";
    cameraBox.x = 0;
    cameraBox.y = 0;
    cameraBox.w = 128;
    cameraBox.h = 128;


    elementBox.x = 0;
    elementBox.y = 48;
    elementBox.w = 192;
    elementBox.h = 128;

    entireBox.x = 0;
    entireBox.y = 48;
    entireBox.w = 192;
    entireBox.h = 32;

    viewBox.x = 0;
    viewBox.y = 0;
    viewBox.w = 128;
    viewBox.h = 128;

    barXPadding = 8;
    barYPadding = 8;
    elementBox.h = SCREEN_HEIGHT-elementBox.y;
    subMenuIsOpen = true;
    selectedSubOption = -1;
    hasScrollControl = false;
    hasArrowkeyControl = false;
    xScroll = new GPE_ScrollBar_XAxis();
    yScroll = new GPE_ScrollBar_YAxis();
    barTitleWidth = 0;
    barTitleHeight = 24;
    GPE_DEFAULT_FONT->get_metrics("Project Resources",&barTitleWidth,&barTitleHeight);
    barTitleHeight= 24;
    //menuNameTexture->loadFromRenderedText(MAIN_RENDERER,"Project Resources",GPE_MAIN_THEME->Main_Box_Font_Color,FONT_CATEGORY_BAR);
    lastWidth = elementBox.w;
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

GPE_GeneralResourceContainer * GPE_ResourceTree::add_resource_folder(int resourceType, std::string projFolderName, std::string resourceTypeName)
{
    GPE_GeneralResourceContainer * newResourceFolder = new GPE_GeneralResourceContainer(projFolderName,resourceTypeName,resourceType,-1,true,0,restype_superfolder);
    newResourceFolder->optionBox.x = elementBox.x;
    subOptions.push_back(newResourceFolder);
    return newResourceFolder;
}

GPE_GeneralResourceContainer * GPE_ResourceTree::add_project_folder(int resourceType, std::string projFolderName, std::string resourceTypeName)
{
    GPE_GeneralResourceContainer * newResourceFolder = new GPE_GeneralResourceContainer(projFolderName,resourceTypeName,resourceType,-1,true,0,restype_projfolder);
    newResourceFolder->optionBox.x = elementBox.x;
    subOptions.push_back(newResourceFolder);
    return newResourceFolder;
}

void GPE_ResourceTree::delete_project_resources(std::string projectFileName)
{
    remove_project_resources( projectFileName);
}

void GPE_ResourceTree::prerender_self( )
{
    GPE_GeneralResourceContainer * tSubOption= NULL;
    for( int i = 0; i < (int)subOptions.size(); i++)
    {
        tSubOption = subOptions[i];
        if( tSubOption!=NULL)
        {
            tSubOption->prerender_self( );
        }
    }
}

void GPE_ResourceTree::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera(cam);

    /*elementBox.x = 0;
    elementBox.y = 0;*/
    GPE_GeneralGuiElement::process_self( viewedSpace, cam );

    int sOpNumber = -1;

    bool mouseInRange = false;
    cameraBox.w = elementBox.w - yScroll->get_box_width();
    cameraBox.h = elementBox.h - xScroll->get_box_height();

    int xPos = 0;
    int yPos = 0;
    int y2Pos = yPos;

    if( isHovered )
    {
        mouseInRange = true;
    }
    if( input->check_mouse_pressed(0) || input->check_mouse_pressed(1) || input->check_mouse_pressed(2) )
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
        entireBox.w = cameraBox.w - GENERAL_GPE_PADDING;
        entireBox.h = RESOURCE_AREA_HEIGHT*3;
        GPE_GeneralResourceContainer * cContainer = NULL;

        viewBox.x = elementBox.x + viewedSpace->x - cam->x;
        viewBox.y = elementBox.y + viewedSpace->y - cam->y;
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
                entireBox.h+=cContainer->optionBox.h;
                y2Pos+=cContainer->optionBox.h;
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
        xScroll->set_coords( elementBox.x, elementBox.y+elementBox.h - xScroll->get_height() );
        xScroll->set_width( elementBox.w - yScroll->get_width() );
        xScroll->fullRect.x = 0;
        xScroll->fullRect.y = 0;
        xScroll->fullRect.w = entireBox.w;
        xScroll->fullRect.h = entireBox.h;

        //if( hasScrollControl && input->check_keyboard_down(kb_ctrl) )
        if( mouseInRange && input->check_keyboard_down(kb_ctrl) )
        {
            if( input->mouseScrollingUp)
            {
                cameraBox.x-=cameraBox.w/8;
            }
            else if( input->mouseScrollingDown)
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
            if( input->check_keyboard_down(kb_left) && !input->check_keyboard_pressed(kb_left) )
            {
                leftDelayTime++;
            }
            else if( input->check_keyboard_down(kb_right)  && !input->check_keyboard_pressed(kb_right) )
            {
                rightDelayTime++;
            }

            if( leftDelayTime > MAIN_GUI_SETTINGS->normalInputDelayTime || input->check_keyboard_pressed(kb_left) )
            {
                xScroll->contextRect.x-=cameraBox.w/8;
                leftDelayTime = 0;
            }
            else if( rightDelayTime > MAIN_GUI_SETTINGS->normalInputDelayTime || input->check_keyboard_pressed(kb_right) )
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

        yScroll->set_coords( elementBox.x+elementBox.w - yScroll->get_width() -4, elementBox.y );
        yScroll->set_height( elementBox.h );

        yScroll->fullRect.x = 0;
        yScroll->fullRect.y = 0;
        yScroll->fullRect.w = entireBox.w,
        yScroll->fullRect.h = entireBox.h;

        yScroll->contextRect.x = cameraBox.x;
        yScroll->contextRect.y = cameraBox.y;
        yScroll->contextRect.w = cameraBox.w,
        yScroll->contextRect.h = cameraBox.h;

        //if( hasScrollControl && input->check_keyboard_down(kb_ctrl)==false )
        if( mouseInRange && input->check_keyboard_down(kb_ctrl)==false )
        {
            if( input->mouseScrollingUp)
            {
                yScroll->contextRect.y-=cameraBox.h/8;
            }
            else if( input->mouseScrollingDown)
            {
                yScroll->contextRect.y+=cameraBox.h/8;
            }
        }

        if( hasScrollControl)
        {
            if( input->check_keyboard_down(kb_up) && !input->check_keyboard_pressed(kb_up) )
            {
                upDelayTime++;
            }
            else if( input->check_keyboard_down(kb_down)  && !input->check_keyboard_pressed(kb_down) )
            {
                downDelayTime++;
            }

            if( upDelayTime > MAIN_GUI_SETTINGS->normalInputDelayTime || input->check_keyboard_pressed(kb_up) )
            {
                yScroll->contextRect.y-=cameraBox.h/8;
                upDelayTime = 0;
            }
            else if( downDelayTime > MAIN_GUI_SETTINGS->normalInputDelayTime || input->check_keyboard_pressed(kb_down) )
            {
                yScroll->contextRect.y+=cameraBox.h/8;
                downDelayTime = 0;
            }
        }
        yScroll->process_self( viewedSpace, cam );
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
            yScroll->process_self( viewedSpace, cam );
        }
    }

    //Processes if a previous right click was made and if so, make context menu
    if( RESOURCEMENU_WAS_RIGHTCLICKED && LAST_CLICKED_RESOURCE!=NULL)
    {
        RESOURCEMENU_WAS_RIGHTCLICKED = false;
        GPE_open_context_menu(input->mouse_x-256,input->mouse_y,256);
        if( !LAST_CLICKED_RESOURCE->is_folder() && !LAST_CLICKED_RESOURCE->is_super_project_folder() && !LAST_CLICKED_RESOURCE->is_super_project_folder() )
        {
            int tempResType= LAST_CLICKED_RESOURCE->get_resource_type() ;
            if( tempResType!=RESOURCE_TYPE_TEXTURE && tempResType!=RESOURCE_TYPE_TILESHEET && tempResType!=RESOURCE_TYPE_SPRITE && tempResType!=RESOURCE_TYPE_AUDIO && tempResType!=RESOURCE_TYPE_VIDEO && tempResType!=RESOURCE_TYPE_PROJECT_SETTINGS )
            {
                MAIN_CONTEXT_MENU->add_menu_option("Duplicate Resource",-1,NULL,-1,NULL,false,true);
            }
            MAIN_CONTEXT_MENU->add_menu_option("Rename Resource",-1,NULL,-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Remove Resource",-1,NULL,-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Resource Properties",-1,NULL,-1,NULL,true,true);
        }
        else if(LAST_CLICKED_RESOURCE->is_super_project_folder() )
        {
            MAIN_CONTEXT_MENU->add_menu_option("Save Project",-1,NULL,-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Close Project",-1,NULL,-1,NULL,true,true);

            MAIN_CONTEXT_MENU->add_menu_option("Build Project",-1,NULL,-1,NULL,false,true);
            //MAIN_CONTEXT_MENU->add_menu_option("Debug Project",-1,NULL,-1,NULL,false,true);
            GPE_PopUpMenu_Option *cleanFolderOptions =  MAIN_CONTEXT_MENU->add_menu_option("Clean Project",-1,NULL,-1,NULL,true,false);
            cleanFolderOptions->add_menu_option("Clean [HTML5] Build Folder",-1);
            //if( GPE_MAIN_GUI->includeNintendoWiiUExport )
            cleanFolderOptions->add_menu_option("Clean [WiiU] Build Folder",-1);
            cleanFolderOptions->add_menu_option("Clean [Windows] Build Folder",-1);
            cleanFolderOptions->add_menu_option("Clean [Linux] Build Folder",-1);
            cleanFolderOptions->add_menu_option("Clean [OSX] Build Folder",-1);
            cleanFolderOptions = MAIN_CONTEXT_MENU->add_menu_option("Open Project Directory");
            MAIN_CONTEXT_MENU->add_menu_option("Find Resource...");

            //MAIN_CONTEXT_MENU->add_menu_option("Browse Directory",-1,-1,NULL,false,true);
        }
        else if(LAST_CLICKED_RESOURCE->is_super_folder() || LAST_CLICKED_RESOURCE->is_folder() )
        {
            MAIN_CONTEXT_MENU->add_menu_option("Add Resource",-1,NULL,-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Add Folder",-1,NULL,-1,NULL,true,true);
            if( !LAST_CLICKED_RESOURCE->is_super_folder())
            {
                MAIN_CONTEXT_MENU->add_menu_option("Rename Folder",-1,NULL,-1,NULL,true,true);
                MAIN_CONTEXT_MENU->add_menu_option("Clear Resource Folder Contents",-1,NULL,-1,NULL,true,true);
            }
            MAIN_CONTEXT_MENU->add_menu_option("Sort by Name",-1,NULL,-1,NULL,true,true);
            if( !LAST_CLICKED_RESOURCE->is_super_folder())
            {
                MAIN_CONTEXT_MENU->add_menu_option("Remove Folder",-1,NULL,-1,NULL,false,true);
            }
            MAIN_CONTEXT_MENU->add_menu_option("Find Resource...");
        }
        else if( LAST_CLICKED_RESOURCE->is_folder()==false)
        {
            int tempResType= LAST_CLICKED_RESOURCE->get_resource_type() ;
            if( tempResType!=RESOURCE_TYPE_TEXTURE && tempResType!=RESOURCE_TYPE_TILESHEET && tempResType!=RESOURCE_TYPE_SPRITE && tempResType!=RESOURCE_TYPE_AUDIO && tempResType!=RESOURCE_TYPE_VIDEO && tempResType!=RESOURCE_TYPE_PROJECT_SETTINGS )
            {
                MAIN_CONTEXT_MENU->add_menu_option("Duplicate Resource",-1,NULL,-1,NULL,false,true);
            }
            MAIN_CONTEXT_MENU->add_menu_option("Rename Resource",-1,NULL,-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Delete Resource",-1,NULL,-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Properties",-1,NULL,-1,NULL,false,true);
        }
        GPE_Get_Context_Result();
    }
    else if( input->check_mouse_released( mb_left)&&  RESOURCE_TO_DRAG!=NULL )
    {
        if( LAST_CLICKED_RESOURCE!=NULL)
        {
            if( LAST_CLICKED_RESOURCE->is_super_folder() || LAST_CLICKED_RESOURCE->is_folder() )
            {
                if( RESOURCE_TO_DRAG->parentResource!=NULL && LAST_CLICKED_RESOURCE->can_obtain(RESOURCE_TO_DRAG) )
                {
                    RESOURCE_TO_DRAG->parentResource->remove_resource(RESOURCE_TO_DRAG,false);
                    LAST_CLICKED_RESOURCE->add_resource_container(RESOURCE_TO_DRAG);
                    input->reset_all_input();
                    selectedSubOption = RESOURCE_TO_DRAG->get_global_id();
                    RESOURCE_TO_DRAG = NULL;
                    LAST_CLICKED_RESOURCE = NULL;
                    process_self();
                }
            }
            else if( RESOURCE_TO_DRAG->parentResource!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL && LAST_CLICKED_RESOURCE->parentResource->can_obtain(RESOURCE_TO_DRAG) )
            {
                if( RESOURCE_TO_DRAG->matches(LAST_CLICKED_RESOURCE)!=1  &&  RESOURCE_TO_DRAG->matches(LAST_CLICKED_RESOURCE->parentResource)!=1  )
                {
                    GPE_GeneralResourceContainer * previousParentResource = RESOURCE_TO_DRAG->parentResource;
                    RESOURCE_TO_DRAG->parentResource->remove_resource(RESOURCE_TO_DRAG, false);
                    GPE_Report("Trying to copy ["+RESOURCE_TO_DRAG->get_name()+"] resource.");
                    GPE_Report("Trying to copy to ["+LAST_CLICKED_RESOURCE->get_name()+"]'s parent resource.");
                    GPE_Report("Trying to copy to ["+LAST_CLICKED_RESOURCE->parentResource->get_name()+"]'s resource.");
                    LAST_CLICKED_RESOURCE->parentResource->add_resource_container(RESOURCE_TO_DRAG,false,LAST_CLICKED_RESOURCE);
                    selectedSubOption = RESOURCE_TO_DRAG->get_global_id();
                    input->reset_all_input();
                    RESOURCE_TO_DRAG = NULL;
                    LAST_CLICKED_RESOURCE = NULL;
                    process_self();
                }
            }
            LAST_CLICKED_RESOURCE= NULL;
        }
    }
}

void GPE_ResourceTree::render_self( GPE_Rect *viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );
    MAIN_RENDERER->set_viewpoint( NULL );
    MAIN_RENDERER->set_viewpoint( viewedSpace);

    //if( forceRedraw)
    {
        if( GPE_MAIN_THEME->themeBgTexture == NULL)
        {
            gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Program_Color,false);
        }
        GPE_GeneralResourceContainer * cResource = NULL;
        int xDrawPos = 0;
        int yDrawPos = 0;
        for(int i=0; i<(int)subOptions.size(); i+=1)
        {
            cResource = subOptions[i];
            if(cResource!=NULL)
            {
                cResource->render_option(xDrawPos,yDrawPos,selectedSubOption, viewedSpace,&cameraBox, true, forceRedraw );
                yDrawPos+=cResource->optionBox.h;
            }
        }

        MAIN_RENDERER->set_viewpoint( NULL );
        MAIN_RENDERER->set_viewpoint( viewedSpace );
        if( xScroll!=NULL)
        {
            xScroll->render_self( viewedSpace, cam , forceRedraw);
        }
        if( yScroll!=NULL)
        {
            yScroll->render_self(viewedSpace, cam, forceRedraw  );
            //if( RENDER_RESOURCEBAR_LEFT)
            {
                //gcanvas->render_rectangle( yScroll->elementBox.x+yScroll->elementBox.w,yScroll->elementBox.y,elementBox.x+elementBox.w,elementBox.h,barColor,false);
            }
        }

        if( hasScrollControl)
        {
            gcanvas->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+elementBox.h,GPE_MAIN_THEME->Button_Box_Highlighted_Color,true);
        }
        else
        {
            gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Text_Box_Outline_Color,true);
        }
    }
    //gcanvas->render_rectangle(0,0,viewedSpace->w, viewedSpace->h, c_blue, false );
    MAIN_RENDERER->set_viewpoint( &elementBox);

}

void GPE_ResourceTree::remove_project_resources(std::string projectFileName)
{
    if( (int)projectFileName.size()>0 )
    {
        GPE_GeneralResourceContainer * tContainer = NULL;
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

