/*
paw_gui_tab_basic.cpp
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

#include "paw_gui_tab_basic.h"


GPE_TabBar::GPE_TabBar( bool dynamicClosing )
{
    canCloseTabs = dynamicClosing;
    needsNewLine = true;
    isFullWidth = true;
    tabIsRightClicked = false;
    isInUse = false;
    guiListTypeName = "tabbar";
    elementBox.x = 16;
    elementBox.y = 16;
    elementBox.w = 32;
    elementBox.h = 18;

    barXPadding = GENERAL_GPE_GUI_PADDING;
    barYPadding = 4;
    fontTextWidth = 12;
    fontTextHeight = 12;
    if( GUI_TAB_FONT!=NULL)
    {
        GUI_TAB_FONT->get_metrics("A",&fontTextWidth,&fontTextHeight);
    }
    else
    {
        fontTextWidth = 12;
        fontTextHeight = 12;
    }
    tabInUse = 0;
    tabSize = -1; //defaults to equally divide the tab onto the screen

    miniTabSize = 48;
    extraSmallSize = 64;
    smallTabSize = 96;
    medTabSize = 128;
    largeTabSize = 160;
    extraLargeTabSize = 192;
    xxLargeTabSize = 256;
    tabsPerView = -1;
    tabPos = 0;
}

GPE_TabBar::~GPE_TabBar()
{
    subOptions.clear();
}

void GPE_TabBar::add_new_tab(std::string newOption, bool switchToNew )
{
    if( (int)newOption.size() > 0)
    {
        //automatically add  resource to when the tab bar is empty
        if( switchToNew)
        {
            tabInUse = (int)subOptions.size();
        }
        subOptions.push_back(newOption);
        calculate_tabs();
    }
}

void GPE_TabBar::calculate_tabs()
{
    int currentSize = (int)subOptions.size();
    if( tabPos >= currentSize )
    {
        tabPos = currentSize -1;
    }
    if( tabPos < 0)
    {
        tabPos = 0;
    }
    if( tabInUse >= currentSize )
    {
        tabInUse = currentSize - 1;
    }
    if( tabPos < 0)
    {
        tabInUse = 0;

    }
    if( currentSize == 0 )
    {
        tabSize = elementBox.w;
    }
    else
    {
        tabSize = elementBox.w/ currentSize;
    }
}

bool GPE_TabBar::contains_tab( std::string tabName )
{
    if( (int)tabName.size() > 0)
    {
        for( int i = 0; i < (int)subOptions.size(); i++)
        {
            if( tabName==subOptions[i] )
            {
                return true;
            }
        }
    }
    return false;
}

void GPE_TabBar::open_tab(int tabId)
{
    if( tabId >=0 && tabId < (int)subOptions.size() )
    {
        tabInUse = tabId;
    }
    else
    {
        tabInUse = 0;
    }
}

void GPE_TabBar::open_tab(std::string tabName)
{
    if( (int)tabName.size() > 0)
    {
        for( int i = 0; i < (int)subOptions.size(); i++)
        {
            if( tabName==subOptions[i] )
            {
                tabInUse = i;
                break;
            }
        }
    }
}

std::string GPE_TabBar::get_selected_name()
{
    if( (int)subOptions.size() > 0 )
    {
        if( tabInUse >=0 && tabInUse < (int)subOptions.size() )
        {
            return subOptions[tabInUse];
        }
    }
    return "";
}

int GPE_TabBar::get_selected_tab()
{
    return tabInUse;
}

int GPE_TabBar::get_tab_count()
{
    return (int)subOptions.size();
}

void GPE_TabBar::process_self(GPE_Rect * viewedSpace,GPE_Rect *cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    tabIsRightClicked = false;
    //elementBox.w = viewedSpace->w - elementBox.x;
    if( elementBox.w!=0 && viewedSpace!=NULL && cam!=NULL)
    {
        GPE_GeneralGuiElement::process_self(viewedSpace, cam);
        calculate_tabs();
        if( isClicked || isRightClicked)
        {
            isInUse = true;
        }
        else if( clickedOutside)
        {
            isInUse = false;
        }

        int cTabXPos = elementBox.x+viewedSpace->x-cam->x;
        int cTabX2Pos = cTabXPos;
        int cTabYPos = elementBox.y+viewedSpace->y-cam->y;
        int cTabY2Pos = elementBox.y+elementBox.h+viewedSpace->y-cam->y;
        int optionsSize = (int)subOptions.size();
        cTabX2Pos += optionsSize * tabSize;
        cTabXPos = cTabX2Pos - tabSize;
        for(int i = optionsSize-1; i >=0; i--)
        {
            if(point_within(input->mouse_x,input->mouse_y,cTabXPos,cTabYPos,cTabX2Pos,cTabY2Pos) )
            {
                MAIN_OVERLAY->update_tooltip( subOptions[i] );
                if( isClicked )
                {
                    if( canCloseTabs )
                    {
                        if( point_between(input->mouse_x, input->mouse_y,cTabX2Pos-12,cTabYPos, cTabX2Pos,cTabY2Pos) )
                        {
                            subOptions.erase( subOptions.begin()+i );
                            tabInUse = i -1;
                            return;
                        }
                        else
                        {
                            tabInUse = i;
                        }
                    }
                    else
                    {
                        tabInUse = i;
                    }
                }
                if( isRightClicked )
                {
                    tabIsRightClicked = true;
                    tabInUse = i;
                }
            }
            cTabX2Pos = cTabXPos;
            cTabXPos-=tabSize;
        }

        if( isInUse)
        {
            if( input->check_keyboard_released(kb_left) &&  tabInUse > 0)
            {
                tabInUse--;
            }
            else if( input->check_keyboard_released(kb_right) && tabInUse < (int)subOptions.size()-1 )
            {
                tabInUse++;
            }
        }
    }
}

void GPE_TabBar::remove_all_tabs( )
{
    subOptions.clear();
    isClicked = false;
    tabIsRightClicked = false;
    tabPos = tabInUse = 0;
    calculate_tabs();
}

bool GPE_TabBar::remove_tab( int tabId )
{
    if( tabId >=0 && tabId < (int)subOptions.size() )
    {
        subOptions.erase(subOptions.begin()+tabId );
        calculate_tabs();
        return true;
    }
    return false;
}

bool GPE_TabBar::remove_tab( std::string tabName )
{
    bool tabRemoved = false;
    //Reverse iterate for safety
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        if( subOptions.at(i) ==tabName )
        {
            subOptions.erase(subOptions.begin()+i );
            calculate_tabs();
            tabRemoved = true;
        }
    }
    return tabRemoved;
}

void GPE_TabBar::render_self( GPE_Rect * viewedSpace,GPE_Rect * cam)
{
    //gcanvas->render_rect(&elementBox,barColor,false);
    //gcanvas->render_rect(&elementBox,barOutlineColor,true);
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(viewedSpace!=NULL && cam!=NULL)
    {
        if( (int)subOptions.size() >0 )
        {
            std::string tabOptionStr = "";
            int tabFontWidth = 0;
            int tabFontHeight = 0;
            int maxCharactersAllowed = 0;
            GUI_TAB_FONT->get_metrics("A", &tabFontWidth, &tabFontHeight );
            maxCharactersAllowed = tabSize/tabFontWidth -1;

            int cTabXPos = elementBox.x-cam->x;
            int cTabX2Pos = elementBox.x-cam->x;
            int cTabY1Pos = elementBox.y-cam->y;
            int cTabY2Pos = elementBox.y-cam->y+elementBox.h;
            for(int i=0; i< (int)subOptions.size(); i++)
            {
                cTabX2Pos+=tabSize;
                tabOptionStr = subOptions[i];
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
                    gcanvas->render_rectangle( cTabXPos,elementBox.y-cam->y,cTabX2Pos,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Program_Header_Color,false);
                    gcanvas->render_rectangle( cTabXPos,elementBox.y-cam->y,cTabX2Pos,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Color,true);
                    gfs->render_text( cTabXPos+tabSize/2,elementBox.y+elementBox.h/2-cam->y, tabOptionStr, GPE_MAIN_THEME->PopUp_Box_Font_Color,GUI_TAB_FONT,FA_CENTER,FA_MIDDLE);
                }
                else
                {
                    gcanvas->render_rectangle( cTabXPos,elementBox.y-cam->y,cTabX2Pos,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Program_Color,false);
                    gcanvas->render_rectangle( cTabXPos,elementBox.y-cam->y,cTabX2Pos,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Color,true);
                    gfs->render_text( cTabXPos+tabSize/2,elementBox.y+elementBox.h/2-cam->y, tabOptionStr, GPE_MAIN_THEME->Main_Box_Font_Color,GUI_TAB_FONT,FA_CENTER,FA_MIDDLE);
                }
                if( canCloseTabs )
                {
                    if( point_between(input->mouse_x, input->mouse_y,viewedSpace->x+cTabX2Pos-32,viewedSpace->y+cTabY1Pos, viewedSpace->x+cTabX2Pos,viewedSpace->y+cTabY2Pos) )
                    {
                        gfs->render_text( cTabXPos+tabSize-GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2-cam->y, "×", GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,GUI_TAB_FONT,FA_RIGHT,FA_MIDDLE);
                    }
                    else
                    {
                        gfs->render_text( cTabXPos+tabSize-GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2-cam->y, "×", GPE_MAIN_THEME->Main_Box_Font_Color,GUI_TAB_FONT,FA_RIGHT,FA_MIDDLE);
                    }
                }
                cTabXPos = cTabX2Pos;
            }
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Program_Color,false);
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Color,true);
        }
        /*if( isHovered)
        {
            //Uncomment in the even a tab bar randomly behaves weirdly
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Highlighted_Color,true);
        }*/
    }
}

void GPE_TabBar::select_tab( int tabToSelect )
{
    if( tabToSelect >=0 && tabToSelect < (int)subOptions.size() )
    {
        tabInUse = tabToSelect;
    }
}

void GPE_TabBar::toggle_tab( std::string tabName )
{
    if( contains_tab( tabName) )
    {
        remove_tab( tabName);
    }
    else
    {
        add_new_tab( tabName);
    }
}
