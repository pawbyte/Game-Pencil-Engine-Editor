/*
pawgui_tab_basic.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/

#include "pawgui_tab_basic.h"

namespace pawgui
{
    widget_tabbar::widget_tabbar( bool dynamicClosing )
    {
        canCloseTabs = dynamicClosing;
        needsNewLine = true;
        isFullWidth = true;
        tabIsRightClicked = false;
        isInUse = false;
        widget_type = "tabbar";
        widget_box.x = 16;
        widget_box.y = 16;
        widget_box.w = 32;
        widget_box.h = 18;

        barXPadding = padding_default;
        barYPadding = 4;
        fontTextWidth = 12;
        fontTextHeight = 12;
        if( font_tab!=nullptr)
        {
            font_tab->get_metrics("A",&fontTextWidth,&fontTextHeight);
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

    widget_tabbar::~widget_tabbar()
    {
        sub_options.clear();
    }

    void widget_tabbar::add_new_tab(std::string newOption, bool switchToNew )
    {
        if( (int)newOption.size() > 0)
        {
            //automatically add  resource to when the tab bar is empty
            if( switchToNew)
            {
                tabInUse = (int)sub_options.size();
            }
            sub_options.push_back(newOption);
            calculate_tabs();
        }
    }

    void widget_tabbar::calculate_tabs()
    {
        int currentSize = (int)sub_options.size();
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
            tabSize = widget_box.w;
        }
        else
        {
            tabSize = widget_box.w/ currentSize;
        }
    }

    bool widget_tabbar::contains_tab( std::string tabName )
    {
        if( (int)tabName.size() > 0)
        {
            for( int i = 0; i < (int)sub_options.size(); i++)
            {
                if( tabName == sub_options[i] )
                {
                    return true;
                }
            }
        }
        return false;
    }

    void widget_tabbar::open_tab(int tabId)
    {
        if( tabId >=0 && tabId < (int)sub_options.size() )
        {
            tabInUse = tabId;
        }
        else
        {
            tabInUse = 0;
        }
    }

    void widget_tabbar::open_tab(std::string tabName)
    {
        if( (int)tabName.size() > 0)
        {
            for( int i = 0; i < (int)sub_options.size(); i++)
            {
                if( tabName==sub_options[i] )
                {
                    tabInUse = i;
                    break;
                }
            }
        }
    }

    std::string widget_tabbar::get_selected_name()
    {
        if( (int)sub_options.size() > 0 )
        {
            if( tabInUse >=0 && tabInUse < (int)sub_options.size() )
            {
                return sub_options[tabInUse];
            }
        }
        return "";
    }

    int widget_tabbar::get_selected_tab()
    {
        return tabInUse;
    }

    std::string widget_tabbar::get_tab_at( int pos )
    {
        if( pos >= 0 && pos < (int)sub_options.size() )
        {
            return sub_options[ pos ];
        }
        return "";
    }

    int widget_tabbar::get_tab_count()
    {
        return (int)sub_options.size();
    }

    void widget_tabbar::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        tabIsRightClicked = false;
        //widget_box.w = view_space->w - widget_box.x;
        if( widget_box.w!=0 && view_space!=nullptr && cam!=nullptr)
        {
            widget_basic::process_self(view_space, cam);
            calculate_tabs();
            if( isClicked || isRightClicked)
            {
                isInUse = true;
            }
            else if( clickedOutside)
            {
                isInUse = false;
            }

            int cTabXPos = widget_box.x+view_space->x-cam->x;
            int cTabX2Pos = cTabXPos;
            int cTabYPos = widget_box.y+view_space->y-cam->y;
            int cTabY2Pos = widget_box.y+widget_box.h+view_space->y-cam->y;
            int optionsSize = (int)sub_options.size();
            cTabX2Pos += optionsSize * tabSize;
            cTabXPos = cTabX2Pos - tabSize;
            for(int i = optionsSize-1; i >=0; i--)
            {
                if(gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,cTabXPos,cTabYPos,cTabX2Pos,cTabY2Pos) )
                {
                    main_overlay_system->update_tooltip( sub_options[i] );
                    if( isClicked )
                    {
                        if( canCloseTabs )
                        {
                            if( gpe::point_between(gpe::input->mouse_position_x, gpe::input->mouse_position_y,cTabX2Pos-12,cTabYPos, cTabX2Pos,cTabY2Pos) )
                            {
                                sub_options.erase( sub_options.begin()+i );
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
                if( gpe::input->check_kb_released(kb_left) &&  tabInUse > 0)
                {
                    tabInUse--;
                }
                else if( gpe::input->check_kb_released(kb_right) && tabInUse < (int)sub_options.size()-1 )
                {
                    tabInUse++;
                }
            }
        }
    }

    void widget_tabbar::remove_all_tabs( )
    {
        sub_options.clear();
        isClicked = false;
        tabIsRightClicked = false;
        tabPos = tabInUse = 0;
        calculate_tabs();
    }

    bool widget_tabbar::remove_tab( int tabId )
    {
        if( tabId >=0 && tabId < (int)sub_options.size() )
        {
            sub_options.erase(sub_options.begin()+tabId );
            calculate_tabs();
            return true;
        }
        return false;
    }

    bool widget_tabbar::remove_tab( std::string tabName )
    {
        bool tabRemoved = false;
        //Reverse iterate for safety
        for( int i = (int)sub_options.size()-1; i >=0; i--)
        {
            if( sub_options.at(i) ==tabName )
            {
                sub_options.erase(sub_options.begin()+i );
                calculate_tabs();
                tabRemoved = true;
            }
        }
        return tabRemoved;
    }

    void widget_tabbar::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
    {
        //gpe::gcanvas->render_rect(&widget_box,barColor,false);
        //gpe::gcanvas->render_rect(&widget_box,barOutlineColor,true);
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if(view_space!=nullptr && cam!=nullptr)
        {
            if( (int)sub_options.size() >0 )
            {
                std::string tabOptionStr = "";
                int tabFontWidth = 0;
                int tabFontHeight = 0;
                int maxCharactersAllowed = 0;
                font_tab->get_metrics("A", &tabFontWidth, &tabFontHeight );
                maxCharactersAllowed = tabSize/tabFontWidth -1;

                int cTabXPos = widget_box.x-cam->x;
                int cTabX2Pos = widget_box.x-cam->x;
                int cTabY1Pos = widget_box.y-cam->y;
                int cTabY2Pos = widget_box.y-cam->y+widget_box.h;

                gpe::gcanvas->render_roundrect_filled_color( cTabXPos, cTabY1Pos, cTabX2Pos, cTabY2Pos, pawgui::theme_main->tab_background_color, 255 );
                gpe::gcanvas->render_rectangle( cTabXPos, (cTabY1Pos + cTabY2Pos) / 2, cTabX2Pos, cTabY2Pos, pawgui::theme_main->tab_background_color, false );
                for(int i=0; i< (int)sub_options.size(); i++)
                {
                    cTabX2Pos+=tabSize;
                    tabOptionStr = sub_options[i];
                    if( (int)tabOptionStr.size() > maxCharactersAllowed )
                    {
                        if( maxCharactersAllowed > 1 )
                        {
                            tabOptionStr = stg_ex::get_substring( tabOptionStr,0,maxCharactersAllowed-1 )+"..";
                        }
                        else
                        {
                            tabOptionStr = stg_ex::get_substring( tabOptionStr,0, 1 );
                        }
                    }
                    if( tabInUse==i)
                    {
                        gpe::gcanvas->render_roundrect_filled_color( cTabXPos,widget_box.y-cam->y,cTabX2Pos,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->tab_selected_color,255);
                        gpe::gcanvas->render_rectangle( cTabXPos,widget_box.y + widget_box.h / 2 - cam->y,cTabX2Pos,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->tab_selected_color, false, 255);
                    }
                    else
                    {
                        gpe::gcanvas->render_roundrect_filled_color( cTabXPos,widget_box.y-cam->y,cTabX2Pos,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->tab_color,255);
                        gpe::gcanvas->render_rectangle( cTabXPos,widget_box.y + widget_box.h / 2 - cam->y,cTabX2Pos,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->tab_color,false);

                    }
                    gpe::gfs->render_text( cTabXPos+tabSize/2,widget_box.y+widget_box.h/2-cam->y, tabOptionStr, pawgui::theme_main->tab_text_color,font_tab,gpe::fa_center,gpe::fa_middle);

                    if( canCloseTabs )
                    {
                        if( gpe::point_between(gpe::input->mouse_position_x, gpe::input->mouse_position_y,view_space->x+cTabX2Pos-32,view_space->y+cTabY1Pos, view_space->x+cTabX2Pos,view_space->y+cTabY2Pos) )
                        {
                            gpe::gfs->render_text( cTabXPos+tabSize-padding_default,widget_box.y+widget_box.h/2-cam->y, "×", pawgui::theme_main->main_box_font_highlight_color,font_tab,gpe::fa_right,gpe::fa_middle);
                        }
                        else
                        {
                            gpe::gfs->render_text( cTabXPos+tabSize-padding_default,widget_box.y+widget_box.h/2-cam->y, "×", pawgui::theme_main->main_box_font_color,font_tab,gpe::fa_right,gpe::fa_middle);
                        }
                    }
                    cTabXPos = cTabX2Pos;
                }
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->program_color,false);
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_border_color,true);
            }
            /*if( isHovered)
            {
                //Uncomment in the even a tab bar randomly behaves weirdly
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_border_highlight_color,true);
            }*/
        }
    }

    void widget_tabbar::select_tab( int tabToSelect )
    {
        if( tabToSelect >=0 && tabToSelect < (int)sub_options.size() )
        {
            tabInUse = tabToSelect;
        }
    }

    void widget_tabbar::toggle_tab( std::string tabName )
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
}
