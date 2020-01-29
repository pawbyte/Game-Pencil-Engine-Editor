/*
paw_gui_tab_manager.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "paw_gui_tab_manager.h"
#include "paw_gui_search_controller.h"
#include "ambitious_gui_library.h"
#include "text_editor.h"


GPE_TabManager * GPE_Main_TabManager = NULL;

GPE_TabManager::GPE_TabManager()
{
    isFullWidth = true;
    needsNewLine = true;
    useDockButton = false;
    isExpanded = false;
    tabExpandButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/expand.png","Undock View",-1,18);
    selectedResource = NULL;
    guiListTypeName = "tabmanager";
    elementBox.x = 16;
    elementBox.y = 16;
    elementBox.w = SCREEN_WIDTH/2;
    elementBox.h = 18;

    tabHeaderBox.x = 16;
    tabHeaderBox.y = 0;
    tabHeaderBox.w = SCREEN_WIDTH/2;
    tabHeaderBox.h = 18;
    tabBox.x = 16;
    tabBox.y = 32;
    tabBox.w = SCREEN_WIDTH;
    tabBox.h = SCREEN_HEIGHT-elementBox.y;
    barXPadding = GENERAL_GPE_GUI_PADDING;
    barYPadding = GENERAL_GPE_GUI_PADDING;
    tabXPAdding = GENERAL_GPE_GUI_PADDING;

    tabToClose = -1;
    tabXHover = -1;
    tabInUse = -1;
    tabSize = -1; //defaults to equally divide the tab onto the screen

    miniTabSize = 32;
    extraSmallSize = 48;
    smallTabSize = 64;
    medTabSize = 96;
    largeTabSize = 128;
    extraLargeTabSize = 160;
    xxLargeTabSize = 192;
    tabsPerView = -1;
    tabPos = 0;
    openContextMenu = false;
}

GPE_TabManager::~GPE_TabManager()
{
    if( tabExpandButton!=NULL )
    {
        delete tabExpandButton;
        tabExpandButton = NULL;
    }
}

void GPE_TabManager::add_new_tab(generalGameResource * newTabResource)
{
    if( newTabResource!=NULL)
    {
        if( (int)subOptions.size() > 0 )
        {
            int i = 0;
            int foundTab = -1;
            generalGameResource * cGResource = NULL;
            //makes sure the tab is not already open
            for( i = 0; i < (int)subOptions.size(); i +=1)
            {
                cGResource = subOptions[i];
                if( cGResource!=NULL)
                {
                    if( cGResource->matches( newTabResource) )
                    {
                        foundTab = i;
                        tabInUse = i;
                        move_to_tab(i);
                    }
                }
            }
            //if tab is not already open, open it
            if(foundTab==-1)
            {
                subOptions.push_back(newTabResource);
                newTabResource->justOpenedThisFrame = true;
                tabInUse = (int)subOptions.size() - 1;
            }
        }
        else
        {
            //automatically add  resource to when the tab bar is empty
            subOptions.push_back(newTabResource);
            tabInUse = 0;
            newTabResource->justOpenedThisFrame = true;
        }
        if( tabInUse < (int)subOptions.size() )
        {
            subOptions[tabInUse]->justOpenedThisFrame = true;
        }
        GPE_MAIN_HIGHLIGHTER->clear_all();
    }
}

void GPE_TabManager::close_resource_tab( std::string projectParentName, int resIdIn)
{
    generalGameResource * tRes = NULL;
    GPE_Report("Removing tab... ["+projectParentName+"]["+int_to_string( resIdIn ) +"]");
    GPE_MAIN_HIGHLIGHTER->clear_all();
    GPE_Report("Highlighter cleared...");
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        tRes = subOptions[i];
        if( tRes!=NULL)
        {
            if( projectParentName==tRes->parentProjectName && tRes->get_global_rid()==resIdIn )
            {
                close_tab(i);
            }
        }
        else
        {
            GPE_Report("Removing tab value with unknown origin...");
            close_tab(i);
        }
    }
}

void GPE_TabManager::close_tab(int tabIdToClose)
{
    if( tabIdToClose>=0 && tabIdToClose < (int)subOptions.size() )
    {
        subOptions.erase(subOptions.begin()+tabIdToClose);
        if( tabInUse>=tabIdToClose )
        {
            move_to_tab( tabInUse - 1 );
        }
        tabToClose = -1;
        update_tabsizes();
        if( tabInUse >=0 && tabInUse < (int)subOptions.size() )
        {
            subOptions[tabInUse]->justOpenedThisFrame = true;
        }
        MAIN_SEARCH_CONTROLLER->close_finder();
        MAIN_OVERLAY->update_temporary_message("","","",0);
        GPE_MAIN_HIGHLIGHTER->clear_all();
    }
}

void GPE_TabManager::close_tabs_left(int tabIdToClose)
{
    if( tabIdToClose>0 && tabIdToClose < (int) subOptions.size() )
    {
        for( int i = tabIdToClose-1; i >=0; i--)
        {
            subOptions.erase(subOptions.begin()+i);
        }
        tabToClose = -1;
        tabInUse = 0;
        update_tabsizes();
        if( tabInUse < (int)subOptions.size() )
        {
            subOptions[tabInUse]->justOpenedThisFrame = true;
        }
        MAIN_SEARCH_CONTROLLER->close_finder();
        MAIN_OVERLAY->update_temporary_message("","","",0);
        GPE_MAIN_HIGHLIGHTER->clear_all();
    }
}

void GPE_TabManager::close_tabs_right(int tabIdToClose)
{
    if( tabIdToClose>=0 && tabIdToClose < (int) subOptions.size()-1 )
    {
        for( int i = (int) subOptions.size()-1; i >tabIdToClose; i--)
        {
            subOptions.erase(subOptions.begin()+i);
        }
        tabToClose = -1;
        update_tabsizes();
        if( tabInUse < (int)subOptions.size() )
        {
            subOptions[tabInUse]->justOpenedThisFrame = true;
        }
        MAIN_SEARCH_CONTROLLER->close_finder();
        MAIN_OVERLAY->update_temporary_message("","","",0);
        GPE_MAIN_HIGHLIGHTER->clear_all();
    }
}

void GPE_TabManager::close_tabs_from_project( std::string projectDirNameIn)
{
    generalGameResource * tRes = NULL;
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        tRes = subOptions[i];
        if( tRes!=NULL)
        {
            if( tRes->parentProjectName==projectDirNameIn)
            {
                close_tab(i);
            }
        }
        else
        {
            close_tab(i);
        }
    }
    MAIN_SEARCH_CONTROLLER->close_finder();
    MAIN_OVERLAY->update_temporary_message("","","",0);
    GPE_MAIN_HIGHLIGHTER->highlightedTerm = NULL;
}

void GPE_TabManager::close_all_tabs()
{
    tabPos = 0;
    tabToClose = -1;
    tabXHover = -1;
    tabInUse= 0;
    subOptions.clear();
    update_tabsizes();
    MAIN_SEARCH_CONTROLLER->close_finder();
    MAIN_OVERLAY->update_temporary_message("","","",0);
    GPE_MAIN_HIGHLIGHTER->highlightedTerm = NULL;

}

void GPE_TabManager::update_tabsizes()
{
    if( tabHeaderBox.w!=0)
    {
        //gets tab size for tabs, dependent on number of tiles
        int subOpCount = (int)subOptions.size();
        tabSize = xxLargeTabSize;
        if( subOpCount > 0)
        {
            tabSize = subOpCount*xxLargeTabSize;
            tabsPerView = subOpCount;
            if( tabSize < tabHeaderBox.w)
            {
                tabSize = xxLargeTabSize;
            }
            else
            {
                //lol no one wants to divide by 0 right. :-)
                tabSize = tabHeaderBox.w/(subOpCount+1);
                if( tabSize < miniTabSize )
                {
                    tabSize = miniTabSize;
                }
                /*
                tabsPerView = subOpCount;
                if( tabSize < tabHeaderBox.w)
                {
                    tabSize = extraLargeTabSize;
                }
                else
                {
                    tabSize = subOpCount*largeTabSize;
                    tabsPerView = subOpCount;
                    if( tabSize < tabHeaderBox.w)
                    {
                        tabSize = largeTabSize;
                    }
                    else
                    {
                        tabSize = subOpCount*medTabSize;
                        if( tabSize < tabHeaderBox.w)
                        {
                            tabSize = medTabSize;
                        }
                        else
                        {
                            tabSize = subOpCount*smallTabSize;
                            if( tabSize < tabHeaderBox.w)
                            {
                                tabSize = smallTabSize;
                            }
                            else
                            {
                                tabSize = subOpCount*extraSmallSize;
                                if( tabSize < tabHeaderBox.w)
                                {
                                    tabSize = extraSmallSize;
                                }
                                else
                                {
                                    tabsPerView = floor(tabHeaderBox.w/miniTabSize);
                                    tabSize = miniTabSize;
                                }
                            }
                        }
                    }
                }
                */
            }
        }
    }
    else
    {
        tabSize = -1;
    }
}

generalGameResource * GPE_TabManager::get_selected_resource()
{
    if(tabInUse>=0 && tabInUse < (int)subOptions.size() )
    {
        generalGameResource * cGenResource = subOptions.at(tabInUse);
        return cGenResource;
    }
    return NULL;
}

void GPE_TabManager::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam )
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );
    GPE_GeneralGuiElement::process_self( viewedSpace, cam );
    add_new_tab( selectedResource );
    set_selected_gresource( NULL );
    update_tabsizes();
    generalGameResource * fResource = NULL;
    if(useDockButton && tabExpandButton!=NULL)
    {
        tabExpandButton->set_coords(elementBox.x+elementBox.w - tabExpandButton->get_width(), elementBox.y );
        tabExpandButton->process_self();
        if( tabExpandButton->is_clicked() )
        {
            toggle_full_width();
            input->reset_all_input();
        }
    }

    if( openContextMenu )
    {
        openContextMenu = false;
        int prevTabInUse = tabInUse;
        if( prevTabInUse >=0 && prevTabInUse < (int)subOptions.size() )
        {
            fResource = subOptions.at(prevTabInUse);
            GPE_open_context_menu(input->mouse_x,input->mouse_y,256);
            MAIN_CONTEXT_MENU->add_menu_option("Close",0);
            MAIN_CONTEXT_MENU->add_menu_option("Close All Tabs",1);
            MAIN_CONTEXT_MENU->add_menu_option("Close Other Tabs",2);
            MAIN_CONTEXT_MENU->add_menu_option("Close Tabs to the Left",3);
            MAIN_CONTEXT_MENU->add_menu_option("Close Tabs to the Right",4);
            MAIN_CONTEXT_MENU->add_menu_option("Save",5);
            int foundResult = GPE_Get_Context_Result();
            switch(foundResult)
            {
            case 0:
                tabToClose = prevTabInUse;
                tabXHover = -1;
                input->reset_all_input();
                break;
            case 1:
                close_all_tabs();
                input->reset_all_input();
                if( GPE_MAIN_HIGHLIGHTER!=NULL)
                {
                    GPE_MAIN_HIGHLIGHTER->highlightedTerm = NULL;
                }
                break;
            case 2:
                close_tabs_right(prevTabInUse);
                close_tabs_left(prevTabInUse);
                tabInUse = 0;
                if( tabInUse < (int)subOptions.size() )
                {
                    subOptions[tabInUse]->justOpenedThisFrame = true;
                }
                break;

            case 3:
                close_tabs_left(prevTabInUse);
                tabInUse = 0;
                if( tabInUse < (int)subOptions.size() )
                {
                    subOptions[tabInUse]->justOpenedThisFrame = true;
                }
                break;

            case 4:
                close_tabs_right(prevTabInUse);
                tabInUse = prevTabInUse;
                if( tabInUse < (int)subOptions.size() )
                {
                    subOptions[tabInUse]->justOpenedThisFrame = true;
                }
                break;

            case 5:
                if( fResource!=NULL)
                {
                    fResource->save_resource();
                }
                break;

            default:
                break;
            }
            GPE_close_context_menu();
            update_tabsizes();
        }
    }
    if( tabHeaderBox.w!=0)
    {
        bool withinTabArea = false;
        if (point_between_rect(input->mouse_x,input->mouse_y,&tabHeaderBox) )
        {
            withinTabArea = true;
        }

        //Processes tab management
        tabXHover = -1;
        if(withinTabArea)
        {
            int subPos = 0;
            for(int i=tabPos; i< (int)subOptions.size() && i < tabsPerView+tabPos; i+=1)
            {
                fResource = subOptions[i];
                if( fResource!=NULL)
                {
                    if (point_between(input->mouse_x,input->mouse_y,tabHeaderBox.x+(subPos)*(tabSize+tabXPAdding)+tabSize-16,tabHeaderBox.y,tabHeaderBox.x+(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.y+tabHeaderBox.h) )
                    {
                        MAIN_OVERLAY->update_tooltip("Close "+fResource->get_name() );
                        tabXHover = i;
                        if(input->check_mouse_released( mb_left))
                        {
                            tabToClose = i;
                            tabXHover = -1;
                            input->reset_all_input();
                        }
                    }
                    else if(point_between(input->mouse_x,input->mouse_y,tabHeaderBox.x+subPos*(tabSize+tabXPAdding),tabHeaderBox.y,tabHeaderBox.x+(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.y+tabHeaderBox.h) )
                    {
                        MAIN_OVERLAY->update_tooltip( fResource->get_name() );
                        if(input->check_mouse_released( mb_left))
                        {
                            tabInUse = i;
                            if( tabInUse < (int)subOptions.size() )
                            {
                                subOptions[tabInUse]->justOpenedThisFrame = true;
                            }
                            input->reset_all_input();
                        }
                        else if(input->check_mouse_released(1) )
                        {
                            tabInUse = i;
                            openContextMenu = true;
                        }
                        else if(input->check_mouse_released(2) )
                        {
                            tabToClose = i;
                            tabXHover = -1;
                            input->reset_all_input();
                        }
                    }
                }
                subPos+=1;
            }
        }

        if( input->check_keyboard_down(kb_ctrl)  )
        {
            if( input->check_keyboard_released(kb_w))
            {
                tabToClose = tabInUse;
            }
            else if( input->check_keyboard_released(kb_tab) )
            {
                if( input->shiftKeyIsPressed )
                {
                    move_to_tab(tabInUse-1);
                }
                else
                {
                    move_to_tab(tabInUse+1);
                }
            }
        }
        else if( input->check_gamecontroller_released(0, gc_l2) )
        {
                    move_to_tab(tabInUse-1);
        }
        else if( input->check_gamecontroller_released(0, gc_r2) )
        {
            move_to_tab(tabInUse+1);
        }

        if( tabToClose>=0 && tabToClose < (int) subOptions.size() )
        {
            close_tab(tabToClose);
            tabToClose = -1;
        }

        //Proceses open tab...
        //if(!GPE_MAIN_GUI->mainResourceTree->beingResized )
        {
            generalGameResource * cGenResource  = NULL;
            if(tabInUse>=0 && tabInUse < (int)subOptions.size() )
            {
                cGenResource = subOptions.at(tabInUse);
            }
            if( cGenResource!=NULL)
            {
                cGenResource->process_resource(&tabBox );
            }
        }
    }
}

void GPE_TabManager::render_self( GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera( viewedSpace );
    viewedSpace = GPE_find_camera( cam);
    if( GPE_MAIN_THEME->themeBgTexture==NULL )
    {
        gcanvas->render_rect(&tabHeaderBox,GPE_MAIN_THEME->Program_Color,false);
        gcanvas->render_rect(&tabHeaderBox,GPE_MAIN_THEME->Main_Border_Color,true);
    }
    if( (int)subOptions.size() >0 )
    {
        int subPos = 0;
        generalGameResource * fResource = NULL;
        std::string tabOptionStr = "";
        int tabFontWidth = 0;
        int tabFontHeight = 0;
        int maxCharactersAllowed = 0;
        GUI_TAB_FONT->get_metrics("A", &tabFontWidth, &tabFontHeight );
        maxCharactersAllowed = tabSize/tabFontWidth -1;

        GPE_MAIN_RENDERER->set_viewpoint(&tabHeaderBox);
        for(int i=tabPos; i< (int)subOptions.size() && i < tabsPerView+tabPos; i+=1)
        {
            fResource = subOptions[i];
            if( tabInUse==i)
            {
                gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h,GPE_MAIN_THEME->Program_Header_Color,false);
            }
            else
            {
                gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h,GPE_MAIN_THEME->Program_Color,false);
            }
            if( fResource!=NULL)
            {
                if( fResource->is_modified() )
                {
                    tabOptionStr = "* "+fResource->get_name();
                }
                else
                {
                    tabOptionStr = fResource->get_name();
                }
                if( (int)tabOptionStr.size() > maxCharactersAllowed )
                {
                    if( maxCharactersAllowed > 1 )
                    {
                        tabOptionStr = get_substring( tabOptionStr,0,maxCharactersAllowed-1 )+"..";
                    }
                    else
                    {
                        tabOptionStr = get_substring( tabOptionStr,0, 1 );
                    }
                }

                if( tabInUse==i)
                {
                    gfs->render_text_resized(subPos*(tabSize+tabXPAdding)+GENERAL_GPE_GUI_PADDING,tabHeaderBox.h/2,tabOptionStr,GPE_MAIN_THEME->PopUp_Box_Font_Color, GUI_TAB_FONT,FA_LEFT,FA_MIDDLE,tabSize-tabXPAdding-16);
                }
                else
                {
                    gfs->render_text_resized(subPos*(tabSize+tabXPAdding)+GENERAL_GPE_GUI_PADDING,tabHeaderBox.h/2,tabOptionStr,GPE_MAIN_THEME->Main_Box_Font_Color,GUI_TAB_FONT,FA_LEFT,FA_MIDDLE,tabSize-tabXPAdding-16);
                }
            }

            if( tabXHover==i)
            {
                gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding)+tabSize-GENERAL_ICON_WIDTH,0,+(subPos)*(tabSize+tabXPAdding)+tabSize,+tabHeaderBox.h,c_red,false);
            }
            //gfs->render_text( (subPos)*(tabSize+tabXPAdding)+tabSize-GENERAL_GPE_GUI_PADDING,tabHeaderBox.h/2,"×",GPE_MAIN_THEME->Main_Box_Font_Color,GUI_TAB_FONT,FA_RIGHT,FA_MIDDLE);


            /*if( i!=tabInUse)
            {
                gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
                gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize-1,tabHeaderBox.h-1,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
            }*/
            subPos+=1;
        }

        GPE_MAIN_RENDERER->reset_viewpoint();
        //if( forceRedraw )
            gcanvas->render_horizontal_line_color(elementBox.y,elementBox.x, elementBox.x+elementBox.w, c_black );
        if( useDockButton )
        {
            tabExpandButton->render_self(NULL, NULL );
        }

        if(tabInUse>=0 && tabInUse < (int)subOptions.size() )
        {
            generalGameResource * cGenResource = subOptions.at(tabInUse);
            if( cGenResource!=NULL)
            {
                GPE_MAIN_RENDERER->set_viewpoint( &tabBox );
                cGenResource->render_resource(&tabBox, NULL);
                GPE_MAIN_RENDERER->reset_viewpoint();
            }
        }
    }
    else
    {
        GPE_MAIN_RENDERER->reset_viewpoint();
        GPE_MAIN_RENDERER->set_viewpoint( viewedSpace );

        if( GPE_MAIN_THEME->themeBgTexture==NULL)
        {
            gcanvas->render_rect(&tabBox,GPE_MAIN_THEME->Program_Color,false);
        }
        gfs->render_text(viewedSpace->w / 2, viewedSpace->h / 2,"No Tabs currently open",GPE_MAIN_THEME->PopUp_Box_Font_Color, GUI_TAB_FONT, FA_CENTER, FA_MIDDLE );
        gfs->render_text(viewedSpace->w / 2, viewedSpace->h / 2 + 32,"Press CTRL+O to open a project",GPE_MAIN_THEME->PopUp_Box_Font_Color, GUI_TAB_FONT, FA_CENTER, FA_MIDDLE );
    }
    //gcanvas->render_rect(&tabBox,GPE_MAIN_THEME->Text_Box_Outline_Color,true);
    GPE_MAIN_RENDERER->reset_viewpoint();
}

bool GPE_TabManager::requests_fullscreen()
{
    return isExpanded;
    if( tabInUse>=0 && tabInUse < (int)subOptions.size())
    {
        if( subOptions[tabInUse]!=NULL )
        {
            return subOptions[tabInUse]->isFullScreenResource;
        }
    }
    return false;
}

int GPE_TabManager::search_for_string(std::string needle)
{
    int foundStrings = 0;
    generalGameResource * fOption  = NULL;
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        fOption = subOptions[i];
        if( fOption!=NULL)
        {
            foundStrings+=fOption->search_for_string(needle);
        }
    }

    return foundStrings;
}

int GPE_TabManager::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundStrings = 0;
    generalGameResource * fOption  = NULL;
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        fOption = subOptions[i];
        if( fOption!=NULL)
        {
            foundStrings+=fOption->search_and_replace_string(needle, newStr);
        }
    }
    return foundStrings;
}

void GPE_TabManager::set_coords(int newX, int newY)
{
    if( newX!=-1)
    {
        elementBox.x = newX;
        tabBox.x = newX;
        tabHeaderBox.x = newX;
        tabBox.x = tabHeaderBox.x = elementBox.x;
        tabHeaderBox.w = elementBox.w  - tabExpandButton->get_width();
        tabBox.w = elementBox.w;
    }
    if(newY!=-1)
    {
        elementBox.y = newY;
        tabHeaderBox.y = newY;
        tabBox.y = newY+tabHeaderBox.h;
        tabBox.h = elementBox.h-tabHeaderBox.h;
    }
}

void GPE_TabManager::set_height(int newHeight)
{
    elementBox.h = newHeight;
    tabBox.h = newHeight-tabHeaderBox.h;
}

void GPE_TabManager::set_selected_gresource(generalGameResource *newResource)
{
    selectedResource = newResource;
}

void GPE_TabManager::set_width(int newWidth)
{
    tabBox.w = newWidth;
    tabHeaderBox.w = newWidth - tabExpandButton->get_width()  - tabExpandButton->get_width();
    elementBox.w = newWidth;
}

void GPE_TabManager::move_to_tab(int newTabId)
{
    if( (int)subOptions.size() > 0)
    {
        tabInUse= newTabId;
        if( newTabId<0)
        {
            tabInUse = subOptions.size()-1;
        }
        if( tabInUse >=(int)subOptions.size() )
        {
            tabInUse = 0;
        }
    }
    else
    {
        tabInUse = 0;
        tabPos = 0;
    }
    MAIN_SEARCH_CONTROLLER->close_finder();
    MAIN_OVERLAY->update_temporary_message("","","",0);
    GPE_MAIN_HIGHLIGHTER->highlightedTerm = NULL;
}

void GPE_TabManager::toggle_full_width()
{
    isExpanded = !isExpanded;
    if( isExpanded )
    {
        tabExpandButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/compress.png");
        tabExpandButton->descriptionText = "Dock View";
    }
    else
    {
        tabExpandButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/expand.png");
        tabExpandButton->descriptionText = "Undock View";
    }
}
