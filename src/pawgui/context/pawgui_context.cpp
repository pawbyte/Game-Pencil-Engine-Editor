/*
pawgui_context.cpp
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

#include "pawgui_context.h"

namespace pawgui
{
    std::string action_message_text = "";
    int action_message_id = -1;
    bool action_is_context = false;
    popup_menu_option * main_context_menu = nullptr;
    int popup_font_size_width = 12;
    int popup_font_size_height = 12;

    popup_menu_option::popup_menu_option(std::string name, int id, bool selectable, bool seeShortCut, bool makeContext, int kbS1, int kbS2, int kbS3 )
    {
        browsing_result = -1;
        menuDirection = gpe::fa_right;
        upDelay = 0;
        downDelay = 0;
        leftDelay = 0;
        rightDelay = 0;
        kbShortCut1 = kbS1;
        kbShortCut2 = kbS2;
        kbShortCut3 = kbS3;
        shortcutstring = "";
        if( kbShortCut1 >=0 && kbShortCut1 < gpe::input->kb_button_count )
        {
            shortcutstring = gpe::input->kb_binding_name[kbShortCut1];
            if( kbShortCut2 >=0 && kbShortCut2 < gpe::input->kb_button_count )
            {
                shortcutstring += "+"+gpe::input->kb_binding_name[kbShortCut2];
                if( kbShortCut3 >=0 && kbShortCut3 < gpe::input->kb_button_count )
                {
                    shortcutstring += "+"+gpe::input->kb_binding_name[kbShortCut3];
                }
            }
        }
        //barColor = pawgui::theme_main->popup_box_color;
        renderWhite = false;
        isResourceOption = false;
        sectionBottom = false;
        widget_name = name;
        opId = id;
        optionWidth = 0;

        optionWidthSpace = INT_MIN;
        subOptionWidthSpace = INT_MIN;

        int textW = 0;
        int textH = 0;
        if( (int)widget_name.size()>0 )
        {
            textW = 0;
            textH = 0;
            gpe::font_default->get_metrics(widget_name,&textW, &textH);
            optionWidth = textW + default_toolkey_padding;
            set_width( optionWidth );
            optionWidthSpace = textW + default_toolkey_padding;
            subOptionWidthSpace = optionWidthSpace;
        }
        if( (int)shortcutstring.size()>0 )
        {
            textW = 0;
            textH = 0;
            gpe::font_default->get_metrics(shortcutstring,&textW, &textH);
            optionWidth += textW;
            optionWidthSpace += textW;
            subOptionWidthSpace += optionWidthSpace;
        }
        widestOptionSpace = INT_MIN;
        subMenuIsOpen = false;
        isTopOfMenu = false;
        isContext = makeContext;
        hoverOption = -1;
        opanimation = nullptr;
        opTexture = nullptr;
        animationFrameNumber = 0;
        widget_box.w = optionWidth;
        widget_box.h = context_menu_height_default;
        isSelectable = selectable;
        showShortCuts = seeShortCut;
        subMenuStartPos= 0;
        maxOptionsToView = gpe::screen_height/context_menu_height_default;
        selfDistanceToBottom = 0;
        isFolderOption = false;
    }

    popup_menu_option::~popup_menu_option()
    {
        popup_menu_option * tOption = nullptr;
        for (int i = 0; i < (int)sub_options.size(); i++)
        {
            tOption = sub_options[i];
            if( tOption!=nullptr)
            {
                delete tOption;
                tOption = nullptr;
            }
        }
        sub_options.clear();
    }

    int popup_menu_option::activate_hovered()
    {
        popup_menu_option * gOption = nullptr;
        if( hoverOption >=0 && hoverOption < (int)sub_options.size() )
        {
            gOption = sub_options[hoverOption];
        }
        else if( selectedOption >=0 && selectedOption < (int)sub_options.size() )
        {
            gOption = sub_options[selectedOption];
        }
        //if its possible to open up the suboption do it/return mkay
        if( gOption!=nullptr )
        {
            return gOption->activate_hovered();
        }
        else
        {
            if( get_menu_size() == 0 )
            {
                if(isSelectable)
                {
                    //select, execute and exit
                    update_popup_info(widget_name,opId, isContext);
                    return opId;
                }
                else
                {
                    return -3; //unselectable
                }
            }
        }
        return -1;
    }

    void popup_menu_option::clear_menu()
    {
        popup_menu_option * tOption = nullptr;
        for (int i = 0; i < (int)sub_options.size(); i++)
        {
            tOption = sub_options[i];
            if( tOption!=nullptr)
            {
                delete tOption;
                tOption = nullptr;
            }
        }
        sub_options.clear();
        hoverOption = -1;
        selectedOption = -1;
        subMenuIsOpen = false;
        subMenuStartPos = 0;
        maxOptionsToView = 0;
        selfDistanceToBottom = 0;
    }

    void popup_menu_option::close_menu()
    {
        subMenuIsOpen = false;
    }

    std::string popup_menu_option::get_name()
    {
        return widget_name;
    }

    int popup_menu_option::get_menu_size()
    {
        return (int)sub_options.size();
    }

    popup_menu_option * popup_menu_option::get_menu_option(int atNumb)
    {
        if( atNumb>=0 && atNumb<(int)sub_options.size())
        {
            return sub_options[atNumb];
        }
        return nullptr;
    }

    int popup_menu_option::get_browsing_results()
    {
        return browsing_result;
    }

    int popup_menu_option::get_width()
    {
        return optionWidth;
    }

    bool popup_menu_option::is_open()
    {
        return subMenuIsOpen;
    }

    bool popup_menu_option::push_left()
    {
        popup_menu_option * gOption = nullptr;
        if( selectedOption >=0 && selectedOption < (int)sub_options.size() )
        {
            gOption = sub_options[selectedOption];
        }
        else if( hoverOption >=0 && hoverOption < (int)sub_options.size() )
        {
            gOption = sub_options[hoverOption];
        }
        else
        {
            //hoverOption = -1;
            selectedOption = -1;
            if( isContext)
            {
                hoverOption = 0;
            }
            else
            {
                subMenuIsOpen = false;
            }
        }
        //if its possible to open up the suboption do it/return mkay
        if( gOption!=nullptr )
        {
            if( gOption->is_open()==false || gOption->get_menu_size() == 0 )
            {
                gOption->subMenuIsOpen = false;
                hoverOption = -1;
                selectedOption = -1;
                if( isContext)
                {
                    hoverOption = 0;
                }
                else
                {
                    subMenuIsOpen = false;
                }
                return true;
            }
            else
            {
                return gOption->push_left();
            }
        }
        return false;
    }

    bool popup_menu_option::push_right()
    {
        if( subMenuIsOpen==false && (int)sub_options.size() > 0 )
        {
            reset_suboptions();
            subMenuIsOpen = true;
            selectedOption =  hoverOption = 0;
            return true;
        }
        else
        {
            popup_menu_option * gOption = nullptr;
            int tempCursorPos = 0;
            if( selectedOption >=0 && selectedOption < (int)sub_options.size() )
            {
                tempCursorPos = selectedOption;
                gOption = sub_options[selectedOption];
                hoverOption = -1;
            }
            else if( hoverOption >=0 && hoverOption < (int)sub_options.size() )
            {
                tempCursorPos = hoverOption;
                gOption = sub_options[hoverOption];
                selectedOption = -1;
            }
            else if( (int)sub_options.size() > 0)
            {
                selectedOption = hoverOption = 0;
                gOption = sub_options[0];
                selectedOption = -1;
            }
            if( gOption!=nullptr )
            {
                //if its possible to open up the suboption do it/return mkay
                if( gOption->push_right() )
                {
                    selectedOption = hoverOption =  tempCursorPos;
                    return true;
                }
            }
        }
        return false;
    }

    bool popup_menu_option::push_up()
    {
        if( subMenuIsOpen==false && isContext==false)
        {
            return false;
        }
        if( (int)sub_options.size() > 0 )
        {
            popup_menu_option * fOption = nullptr;
            int currentCursorPos = 0;
            if( selectedOption >=0 && selectedOption < (int)sub_options.size() )
            {
                currentCursorPos = hoverOption = selectedOption;
                fOption = sub_options[currentCursorPos];

            }
            else if( hoverOption >=0 && hoverOption < (int)sub_options.size() )
            {
                currentCursorPos = hoverOption;
                fOption = sub_options[currentCursorPos];
            }
            else if( (int)sub_options.size() > 0 )
            {
                if( subMenuIsOpen == false && isContext==false )
                {
                    reset_suboptions();
                    subMenuIsOpen = true;
                }
                else if( isContext)
                {
                    subMenuIsOpen = true;
                }
                currentCursorPos = hoverOption = selectedOption = 0;
                fOption = sub_options[currentCursorPos];
            }
            /*else
            {
                currentCursorPos = hoverOption = selectedOption = -1;
            }*/

            if( fOption!=nullptr)
            {
                bool moveUpward = false;
                if( fOption->get_menu_size() > 0 && fOption->subMenuIsOpen  )
                {
                    fOption->push_up();
                    /*if( fOption->push_up() == false )
                    {
                        moveUpward = true;
                    }*/
                }
                else
                {
                    moveUpward = true;
                }

                if( moveUpward)
                {
                    currentCursorPos--;
                    if( currentCursorPos < 0)
                    {
                        currentCursorPos = (int)sub_options.size()-1;
                    }
                    if( currentCursorPos < 0)
                    {
                        currentCursorPos = 0;
                    }
                    //reset_suboptions();
                    hoverOption = selectedOption = currentCursorPos;
                }
                return moveUpward;
            }
        }
        return false;
    }

    bool popup_menu_option::push_down()
    {
        if( subMenuIsOpen==false && isContext==false)
        {
            return false;
        }
        if( (int)sub_options.size() > 0 )
        {
            popup_menu_option * fOption = nullptr;
            int currentCursorPos = 0;
            if( selectedOption>=0 && selectedOption < (int)sub_options.size() )
            {
                currentCursorPos = hoverOption =  selectedOption;
                fOption = sub_options[currentCursorPos];
            }
            else if( hoverOption>=0 && hoverOption < (int)sub_options.size() )
            {
                currentCursorPos = selectedOption =  hoverOption;
                fOption = sub_options[currentCursorPos];
            }
            else if( (int)sub_options.size() > 0 )
            {
                if( subMenuIsOpen == false)
                {
                    //reset_suboptions();
                    subMenuIsOpen = true;
                }
                currentCursorPos = selectedOption =  hoverOption = 0;
                fOption = sub_options[currentCursorPos];
            }
            /*else
            {
                currentCursorPos = hoverOption = selectedOption = -1;
            }*/

            if( fOption!=nullptr)
            {
                bool moveDownward = false;
                if( fOption->get_menu_size() > 0 && fOption->subMenuIsOpen )
                {
                    fOption->push_down();
                    /*
                    if( fOption->push_down() == false )
                    {
                        moveDownward = true;
                    }
                    */
                }
                else
                {
                    moveDownward = true;
                }

                if( moveDownward)
                {
                    currentCursorPos++;
                    if( currentCursorPos >= (int)sub_options.size() )
                    {
                        currentCursorPos = 0;
                    }
                    selectedOption = hoverOption = currentCursorPos;
                }
                return moveDownward;
            }
        }
        return false;
    }


    void popup_menu_option::set_id(int new_id)
    {
        opId = new_id;
    }

    popup_menu_option * popup_menu_option::add_option( popup_menu_option * otherOption )
    {
        if( otherOption !=nullptr)
        {
            otherOption->set_height( context_menu_height_default );
            maxOptionsToView = gpe::screen_height/context_menu_height_default;
            selfDistanceToBottom = (gpe::screen_height-widget_box.y-widget_box.h)/context_menu_height_default;

            otherOption->menuDirection = menuDirection;
            int menuCountSize = (int)sub_options.size();
            if(isContext)
            {
                if( menuCountSize > selfDistanceToBottom)
                {
                    otherOption->set_position(widget_box.x,widget_box.y+(menuCountSize-selfDistanceToBottom)*context_menu_height_default);
                }
                else
                {
                    otherOption->set_position(widget_box.x,widget_box.y+menuCountSize*context_menu_height_default );
                }
                //otherOption->barColor = pawgui::theme_main->popup_box_color;
                //otherOption->set_width(subOptionWidthSpace);
            }
            else if( isTopOfMenu)
            {
                otherOption->set_position(widget_box.x,widget_box.y+widget_box.h+ menuCountSize*context_menu_height_default );
                //otherOption->barColor = pawgui::theme_main->popup_box_color;
                //otherOption->set_width(subOptionWidthSpace);
            }
            else
            {
                int menuXPosition;
                if( menuDirection == gpe::fa_right)
                {
                    menuXPosition = widget_box.x+widget_box.w;
                }
                else
                {
                    menuXPosition = widget_box.x - subOptionWidthSpace;
                }
                if( menuCountSize > selfDistanceToBottom)
                {
                    otherOption->set_position( menuXPosition,widget_box.y+(menuCountSize-selfDistanceToBottom)*context_menu_height_default );
                }
                else
                {
                    otherOption->set_position(menuXPosition,widget_box.y+menuCountSize*context_menu_height_default );
                }
                //otherOption->barColor = barColor;
                otherOption->set_width(subOptionWidthSpace);
            }
            sub_options.push_back(otherOption);

            //resizes submenu in the event that the new option is wider
            if( otherOption->get_width()>=widestOptionSpace )
            {
                widestOptionSpace = otherOption->get_width();
                if( showShortCuts)
                {
                    subOptionWidthSpace = widestOptionSpace+default_toolkey_shortcut_width+default_icon_width_padded*2;
                }
                else
                {
                    subOptionWidthSpace = widestOptionSpace+default_icon_width_padded*2;
                }
                //if( isContext || isTopOfMenu)
                {
                    optionWidthSpace = subOptionWidthSpace;
                }
                popup_menu_option * tempFOption = nullptr;
                for( int i=0; i< (int)sub_options.size(); i++ )
                {
                    tempFOption = sub_options[i];
                    //if( isTopOfMenu)
                    {
                        tempFOption->set_width(optionWidthSpace);
                    }
                    //else
                    {
                        tempFOption->set_width(subOptionWidthSpace);
                    }
                }
            }

            return otherOption;
        }

        return nullptr;
    }

    popup_menu_option * popup_menu_option::add_menu_option( std::string name, int id, gpe::texture_base * gTexture, int animationImgNumb, gpe::animation2d * ganimation, bool endsSection, bool selectable, bool isResource, int kbS1, int kbS2, int kbS3 )
    {
        popup_menu_option * newOp = new popup_menu_option(name,id,selectable,showShortCuts,false, kbS1, kbS2, kbS3);
        newOp->isTopOfMenu = false;
        newOp->sectionBottom = endsSection;
        newOp->subMenuIsOpen = false;
        newOp->isResourceOption = isResource;

        int i = 0;
        if( id<0 )
        {
            //newOp->set_id( (int)sub_options.size() *-1);
            newOp->set_id(  -1 );
        }
        newOp->change_texture_data(ganimation, animationImgNumb);
        newOp->set_texture(gTexture);
        add_option( newOp );
        return newOp;
    }

    void popup_menu_option::prerender_self(  )
    {
        /*popup_menu_option * tempOption = nullptr;
        for( int i = 0; i < (int)sub_options.size(); i++)
        {
            tempOption = sub_options[i];
            tempOption->prerender_self( );
        }*/
    }

    void popup_menu_option::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam  )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        int barAction = -1;
        browsing_result = -1; //Negative values are out of range
        //defaults to negative 1 if nothing is selected
        //Returns if window resized, other mouse buttons or esc is pressed.
        widget_basic::process_self(view_space,cam);

        if( gpe::input->check_mouse_released(1) || gpe::input->check_mouse_released(2) || gpe::window_controller_main->is_resized()  || gpe::input->kb_button_released[kb_esc]  )
        {
            browsing_result = -2;
        }
        else
        {
            if( subMenuIsOpen )
            {
                if( isContext )
                {
                    if( gpe::input->kb_button_down[kb_up] && gpe::input->kb_button_pressed[kb_up]==false && gpe::input->kb_button_released[kb_up]==false )
                    {
                        upDelay+= gpe::time_keeper->get_delta_ticks();
                    }
                    else
                    {
                        upDelay = -1;
                    }

                    if( gpe::input->kb_button_down[kb_down] && gpe::input->kb_button_pressed[kb_down]==false && gpe::input->kb_button_released[kb_down]==false )
                    {
                        downDelay+=gpe::time_keeper->get_delta_ticks();
                    }
                    else
                    {
                        downDelay = -1;
                    }

                    if( gpe::input->kb_button_down[kb_left] && gpe::input->kb_button_pressed[kb_left]==false && gpe::input->kb_button_released[kb_left]==false )
                    {
                        leftDelay+=gpe::time_keeper->get_delta_ticks();
                    }
                    else
                    {
                        leftDelay = -1;
                    }

                    if( gpe::input->kb_button_down[kb_right] && gpe::input->kb_button_pressed[kb_right]==false && gpe::input->kb_button_released[kb_right]==false )
                    {
                        rightDelay+=gpe::time_keeper->get_delta_ticks();
                    }
                    else
                    {
                        rightDelay = -1;
                    }
                }
                else
                {
                    upDelay = -1;
                    downDelay = -1;
                    leftDelay = -1;
                    rightDelay = -1;
                }
                int subPosX = widget_box.x;
                int subPosY = widget_box.y;
                maxOptionsToView = gpe::screen_height/context_menu_height_default;
                selfDistanceToBottom = (gpe::screen_height-widget_box.y-widget_box.h)/context_menu_height_default;
                popup_menu_option * fOption = nullptr;
                int menuCountSize = (int)sub_options.size() ;
                int extraRenderSpace = 0;
                int subOptionsCurrWidth = optionWidthSpace;
                if( isTopOfMenu==false && isContext==false)
                {
                    extraRenderSpace = 0;
                    subOptionsCurrWidth = subOptionWidthSpace;
                }

                if( menuCountSize > maxOptionsToView)
                {
                    if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x+extraRenderSpace,0,widget_box.x+extraRenderSpace+subOptionsCurrWidth, gpe::screen_height) )
                    {
                        if( gpe::input->mouse_scrolling_up )
                        {
                            subMenuStartPos-=maxOptionsToView/8;
                        }

                        else if( gpe::input->mouse_scrolling_down)
                        {
                            subMenuStartPos+=maxOptionsToView/8;
                        }
                    }
                    if( subMenuStartPos+maxOptionsToView > (int)sub_options.size() )
                    {
                        subMenuStartPos = (int)sub_options.size()-maxOptionsToView;
                    }
                    if( subMenuStartPos < 0)
                    {
                        subMenuStartPos = 0;
                    }
                }

                //Keyboard activity reserved for context menu only
                bool keyActivityHappend = false;
                if( isContext)
                {
                    int currentCursorPos = 0;
                    if( selectedOption>=0 && selectedOption < (int)sub_options.size() )
                    {
                        fOption = sub_options[selectedOption];
                        currentCursorPos = selectedOption;
                    }
                    else if( hoverOption>=0 && hoverOption < (int)sub_options.size() )
                    {
                        fOption = sub_options[hoverOption];
                        currentCursorPos = hoverOption;
                    }
                    /*else if( (int)sub_options.size() > 0 && sub_options[0]!=nullptr )
                    {
                        currentCursorPos = hoverOption = 0;
                        selectedOption = -1;
                        fOption = sub_options[hoverOption];
                    }*/

                    if( leftDelay >= (main_settings->normalInputDelayTime)  || ( !gpe::input->kb_button_pressed[kb_left] && gpe::input->kb_button_released[kb_left] ) )
                    {
                        leftDelay = -1;
                        if( menuDirection == gpe::fa_right )
                        {
                            push_left();
                        }
                        else
                        {
                            push_right();
                        }
                        keyActivityHappend = true;
                    }
                    else if( rightDelay >= (main_settings->normalInputDelayTime)  || ( !gpe::input->kb_button_pressed[kb_right] && gpe::input->kb_button_released[kb_right] ) )
                    {
                        if( menuDirection == gpe::fa_right )
                        {
                            push_right();
                        }
                        else
                        {
                            push_left();
                        }
                        rightDelay = -1;
                        keyActivityHappend = true;

                    }
                    else if( upDelay >= (main_settings->normalInputDelayTime)  || ( !gpe::input->kb_button_pressed[kb_up] && gpe::input->kb_button_released[kb_up] ) )
                    {
                        push_up();
                        upDelay = -1;
                        keyActivityHappend = true;
                    }
                    else if( downDelay >= (main_settings->normalInputDelayTime)  || ( !gpe::input->kb_button_pressed[kb_down] && gpe::input->kb_button_released[kb_down] ) )
                    {
                        push_down();
                        downDelay = -1;
                        keyActivityHappend = true;
                    }
                    else if( ( gpe::input->kb_button_released[kb_enter] || gpe::input->kb_button_released[kb_space] ) && subMenuIsOpen  )
                    {
                        browsing_result  = activate_hovered();
                        hoverOption = -1;
                        selectedOption = -1;
                        reset_suboptions();
                        subMenuIsOpen = false;
                        keyActivityHappend = true;
                    }
                    else
                    {
                        keyActivityHappend = false;
                    }
                }
                else
                {
                    keyActivityHappend = false;
                }

                //if nothing happened with keyboard we do the other fun stuff
                if( keyActivityHappend == false )
                {
                    for( int i=subMenuStartPos; i<menuCountSize && i < subMenuStartPos+maxOptionsToView; i++)
                    {
                        if( barAction<0)
                        {
                            fOption = sub_options[i];
                            fOption->menuDirection = menuDirection;
                            if( isTopOfMenu==false || isContext)
                            {
                                if( isContext)
                                {
                                    fOption->set_width(widget_box.w);
                                    subPosX = widget_box.x;
                                }
                                else
                                {
                                    if( menuDirection == gpe::fa_right)
                                    {
                                        subPosX = widget_box.x+widget_box.w;
                                        if( widget_box.x+widget_box.w+subOptionWidthSpace > gpe::screen_width )
                                        {
                                            /*
                                            BE RIGHT BACK
                                            LUNCH TIME
                                            */
                                            subPosY = 0;
                                        }
                                    }
                                    else
                                    {
                                        subPosX = widget_box.x - subOptionWidthSpace;
                                        if( widget_box.x - subOptionWidthSpace < 0 )
                                        {
                                            subPosY = 0;
                                        }
                                    }
                                }

                                if( menuCountSize > selfDistanceToBottom)
                                {
                                    if( menuCountSize > maxOptionsToView)
                                    {
                                        //scroller will be required here
                                        fOption->set_position(subPosX,(i-subMenuStartPos)*context_menu_height_default );
                                    }
                                    else
                                    {
                                        //offset box, no scroller needed
                                        fOption->set_position(subPosX,subPosY+(i-(menuCountSize-selfDistanceToBottom) )*context_menu_height_default );
                                    }
                                }
                                else
                                {
                                    fOption->set_position(subPosX,subPosY+i*context_menu_height_default);
                                }
                            }
                            else if( isTopOfMenu)
                            {
                                fOption->set_position(widget_box.x+extraRenderSpace,subPosY+widget_box.h+i*context_menu_height_default);
                            }

                            fOption->process_self( view_space, cam );
                            barAction = fOption->get_browsing_results();
                            if( gpe::input->mouse_movement_received )
                            {
                                if( fOption->is_hovered() )
                                {
                                    reset_suboptions();
                                    hoverOption = i;
                                    fOption->subMenuIsOpen = true;
                                }
                            }
                            if(barAction >= 0)
                            {
                                browsing_result  = barAction;
                            }
                        }
                    }
                }
            }
            else
            {
                //All delays reset when menu is closed.
                upDelay = -1;
                downDelay = -1;
                leftDelay = -1;
                rightDelay = -1;
            }

            if(  isTopOfMenu==false && isHovered )
            {
                if( (int)sub_options.size()>0)
                {
                    if( gpe::input->mouse_movement_received )
                    {
                        reset_suboptions();
                        subMenuIsOpen = true;
                        popup_menu_option * fOptionOpened = nullptr;
                        for( int iN = 0; iN < (int)sub_options.size(); iN++)
                        {
                            fOptionOpened = sub_options.at(iN);
                            if( fOptionOpened!=nullptr)
                            {
                                fOptionOpened->menuDirection = menuDirection;
                                if( menuDirection == gpe::fa_right)
                                {
                                    fOptionOpened->set_position( widget_box.x+widget_box.w, fOptionOpened->get_ypos());
                                }
                                else
                                {
                                    fOptionOpened->set_position( widget_box.x - subOptionWidthSpace, fOptionOpened->get_ypos() );
                                }
                            }
                        }
                    }
                }
                else if( isClicked || gpe::input->kb_button_released[kb_enter] )
                {
                    //actual action happened...
                    if(isSelectable)
                    {
                        browsing_result = opId; //select, execute and exit
                        update_popup_info(widget_name,opId, isContext);
                    }
                    else
                    {
                        browsing_result = -3; //unselectable
                    }
                }
            }
            //Closed menu and then returns -1
            if( browsing_result < 0 )
            {
                //hoverOption = -1;
                //selectedOption = -1;
                //subMenuIsOpen = false;
                //reset_suboptions();
            }
        }
    }

    void popup_menu_option::open_menu()
    {
        reset_suboptions();
        subMenuIsOpen = true;
        if( (int)sub_options.size() > 0 )
        {
            hoverOption = selectedOption = 0;
        }
        set_position( widget_box.x, widget_box.y );
    }

    void popup_menu_option::reset_suboptions()
    {
        popup_menu_option * gOption = nullptr;
        for(int j=0; j<(int)sub_options.size(); j++)
        {
            gOption = sub_options[j];
            gOption->selectedOption = -1;
            gOption->hoverOption = -1;
            gOption->subMenuIsOpen = false;
            gOption->subMenuStartPos = 0;
            gOption->reset_suboptions();
        }
        /*subMenuIsOpen = false;
        selectedOption = -1;
        hoverOption = -1;*/
    }

    void popup_menu_option::change_texture_data( gpe::animation2d * new_animation,int new_id)
    {
        opanimation = new_animation;
        animationFrameNumber = new_id;
    }
    //Slim Shady back in this mofo!!!

    void popup_menu_option::set_position(int x_pos, int y_pos)
    {
        widget_box.x = x_pos;
        widget_box.y = y_pos;

        if( menuDirection == gpe::fa_right && !isContext )
        {
            if( widget_box.x+widget_box.w > gpe::screen_width && widget_box.w < gpe::screen_width / 2)
            {
                //wrap to top/left side of screen thru recurssion
                //gpe::error_log->report("Moving ["+widget_name+"]["+ stg_ex::int_to_string( gpe::screen_width - widget_box.w - widget_box.x - widget_box.w )+"] to top left of screen.." );
                widget_box.x = 0;
                widget_box.y = 0;
            }

        }
        //BE RIGHT BACK
        else if( widget_box.x - 32 < 0 && !isContext )
        {
            //wrap to top/right side of screen thru recurssion
            widget_box.x =  gpe::screen_width-widget_box.w;
            widget_box.y = 0;
            //gpe::error_log->report("Moving ["+widget_name+"]["+ stg_ex::int_to_string( menuDirection )+"] to top right of screen.." );
        }

        popup_menu_option* cOption = nullptr;
        int subBoxTotal = (int)sub_options.size();
        for( int i=0; i< subBoxTotal; i++)
        {
            cOption = sub_options[i];
            if(cOption!=nullptr)
            {
                cOption->menuDirection = menuDirection;
                cOption->set_width( subOptionWidthSpace );
                if( menuDirection == gpe::fa_right)
                {
                    if( widget_box.x+ widget_box.w  > gpe::screen_width )
                    {
                        cOption->set_position( 0,i*context_menu_height_default);

                    }
                    else
                    {
                        cOption->set_position(widget_box.x+widget_box.w,context_menu_height_default);
                    }
                }
                else if( widget_box.x - subOptionWidthSpace <= 0 )
                {
                    //wrap to top/right side of screen
                    cOption->set_position( gpe::screen_width-subOptionWidthSpace,i*context_menu_height_default);
                }
                else
                {
                    cOption->set_position(widget_box.x+subOptionWidthSpace,i*context_menu_height_default);
                }
            }
        }
    }

    void popup_menu_option::set_texture( gpe::texture_base * newTexture)
    {
        opTexture = newTexture;
    }

    void popup_menu_option::set_width(int new_width)
    {
        widget_box.w = new_width;
        if(!isTopOfMenu)
        {
            popup_menu_option * cOption = nullptr;
            int subBoxTotal = (int)sub_options.size();
            for( int i=0; i< subBoxTotal; i++)
            {
                cOption = sub_options[i];
                if(cOption!=nullptr)
                {
                    //cOption->set_width(new_width);
                }
            }
        }
        set_position( widget_box.x, widget_box.y );
    }

    void popup_menu_option::render_self(  gpe::shape_rect * view_space, gpe::shape_rect *cam )
    {
        //if( forceRedraw )
        {
            int i = 0;
            //Renders the line if this is the end of a section
            if(sectionBottom )
            {
                gpe::gcanvas->render_line_color( widget_box.x,widget_box.y+widget_box.h,widget_box.x+widget_box.w,widget_box.y+widget_box.h, gpe::c_dkgray);
            }

            if( !isTopOfMenu )
            {
                gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_color, false);

                if( isHovered || subMenuIsOpen)
                {
                    gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_highlight_color, false);
                }
            }

            if( (int)sub_options.size()>0)
            {
                if( subMenuIsOpen)
                {
                    popup_menu_option * fOption = nullptr;
                    int menuCountSize = (int)sub_options.size();
                    int subRenderYPos = widget_box.y;
                    int subRenderHeight = (int)sub_options.size()*context_menu_height_default;
                    if( menuCountSize > selfDistanceToBottom)
                    {
                        if( menuCountSize > maxOptionsToView)
                        {
                            //scroller will be required here
                            subRenderYPos = 0;
                            subRenderHeight = gpe::screen_height;
                        }
                        else
                        {
                            //offset box, no scroller needed
                            if( isTopOfMenu && isContext==false)
                            {
                                subRenderYPos = widget_box.y+widget_box.h;
                            }
                            else
                            {
                                subRenderYPos = widget_box.y+widget_box.h;
                                //subRenderYPos = widget_box.y- ( abs(menuCountSize-selfDistanceToBottom) *context_menu_height_default);
                            }
                        }
                    }
                    else
                    {
                        if( isTopOfMenu && isContext==false )
                        {
                            subRenderYPos = widget_box.y+widget_box.h;
                        }
                        else if( isContext)
                        {
                            subRenderYPos = widget_box.y;
                        }
                    }

                    if( isTopOfMenu || isContext)
                    {
                        if( isContext)
                        {
                            gpe::gcanvas->render_rectangle( widget_box.x,subRenderYPos,widget_box.get_x2(),subRenderYPos+subRenderHeight,pawgui::theme_main->popup_box_color, false);
                        }
                        else
                        {
                            gpe::gcanvas->render_rectangle( widget_box.x,subRenderYPos,widget_box.get_x2(),subRenderYPos+subRenderHeight,pawgui::theme_main->popup_box_color, false);
                        }

                        for( i=subMenuStartPos; i<(int)sub_options.size() && i < subMenuStartPos+maxOptionsToView; i++)
                        {
                            fOption = sub_options[i];
                            if( fOption!=nullptr)
                            {
                                if( hoverOption == i  && hoverOption>=0)
                                {
                                    gpe::gcanvas->render_rect( &fOption->widget_box,pawgui::theme_main->popup_box_highlight_color, false);
                                }
                                else
                                {
                                    gpe::gcanvas->render_rect( &fOption->widget_box,pawgui::theme_main->popup_box_color, false);
                                }
                                fOption->render_self( cam );
                            }
                        }
                        if( isContext)
                        {
                            gpe::gcanvas->render_rectangle( widget_box.x,subRenderYPos,widget_box.x+widget_box.w,subRenderYPos+subRenderHeight,pawgui::theme_main->popup_box_border_color, true);
                        }
                        else
                        {
                            gpe::gcanvas->render_rectangle( widget_box.x,subRenderYPos,widget_box.x+subOptionWidthSpace,subRenderYPos+subRenderHeight,pawgui::theme_main->main_border_color, true);
                        }
                    }
                    else
                    {
                        for( i=subMenuStartPos; i<(int)sub_options.size() && i < subMenuStartPos+maxOptionsToView; i++)
                        {
                            fOption = sub_options[i];
                            if( hoverOption == i  && hoverOption>=0)
                            {
                                gpe::gcanvas->render_rect( &fOption->widget_box,pawgui::theme_main->popup_box_highlight_color, false);
                            }
                            else
                            {
                                gpe::gcanvas->render_rect( &fOption->widget_box,pawgui::theme_main->popup_box_color, false);
                            }
                            fOption->render_self( cam );
                        }
                    }
                }

                if( !isTopOfMenu)
                {
                    if( menuDirection == gpe::fa_right)
                    {
                        gpe::gfs->render_text( widget_box.x+widget_box.w-padding_default,widget_box.y+widget_box.h/2,">",pawgui::theme_main->popup_box_font_color,font_toolbar,gpe::fa_right,gpe::fa_middle);
                    }
                    else
                    {
                        gpe::gfs->render_text( widget_box.get_x2()-padding_default,widget_box.y+widget_box.h/2,"<",pawgui::theme_main->popup_box_font_color,font_toolbar,gpe::fa_right,gpe::fa_middle);
                    }
                }
            }

            if( isTopOfMenu )
            {
                gpe::gfs->render_text( widget_box.get_center(),widget_box.get_middle(),widget_name,pawgui::theme_main->main_box_font_color,font_toolbar,gpe::fa_left,gpe::fa_middle);
            }
            else
            {
                if( subMenuIsOpen  )
                {
                    gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+widget_box.h,pawgui::theme_main->popup_box_highlight_color, false);
                    gpe::gfs->render_text( widget_box.x+default_icon_width_padded,widget_box.y+context_menu_height_default/2,widget_name,pawgui::theme_main->popup_box_highlight_font_color,font_toolbar,gpe::fa_left,gpe::fa_middle);
                }
                /*else if( )
                {
                    render_text( widget_box.x+default_icon_width_padded,widget_box.y+context_menu_height_default/2,widget_name,pawgui::theme_main->popup_box_highlight_font_color,font_toolbar,gpe::fa_left,gpe::fa_middle);

                }*/
                else
                {
                    gpe::gfs->render_text( widget_box.x+default_icon_width_padded,widget_box.y+context_menu_height_default/2,widget_name,pawgui::theme_main->popup_box_font_color,font_toolbar,gpe::fa_left,gpe::fa_middle);
                    //opTexture->render_tex( widget_box.x+default_icon_width_padded, widget_box.y+16-(opTexture->get_height()/2),nullptr);
                }
                if ( opanimation!=nullptr && opanimation->has_texture() )
                {
                    gpe::gcanvas->render_animation_resized( opanimation,animationFrameNumber,widget_box.x+padding_default,widget_box.y+widget_box.h/4,widget_box.h/2,widget_box.h/2);
                }
                else if( opTexture!=nullptr)
                {
                    if( isFolderOption)
                    {
                        opTexture->render_tex_resized( widget_box.x+padding_default,widget_box.y+widget_box.h/4,widget_box.h/2,widget_box.h/2,nullptr,pawgui::theme_main->folder_color );
                    }
                    else if( isResourceOption || renderWhite )
                    {
                        opTexture->render_tex_resized(widget_box.x+padding_default,widget_box.y+widget_box.h/4,widget_box.h/2,widget_box.h/2,nullptr, gpe::c_white );
                    }
                    else
                    {
                        opTexture->render_tex_resized(widget_box.x+padding_default,widget_box.y+widget_box.h/4,widget_box.h/2,widget_box.h/2,nullptr,pawgui::theme_main->popup_box_font_color  );
                    }
                }
                if( (int)shortcutstring.size()>0 )
                {
                    gpe::gfs->render_text( widget_box.x+widget_box.w-padding_default,widget_box.y+widget_box.h/2,shortcutstring,pawgui::theme_main->popup_box_font_color,font_toolbar,gpe::fa_right,gpe::fa_middle);
                }
            }

            if( (int)widget_name.size() == 0 )
            {
                gpe::gfs->render_text( widget_box.get_center(),widget_box.y+widget_box.h/2,"nullptr",pawgui::theme_main->popup_box_font_color,font_toolbar,gpe::fa_center,gpe::fa_middle);
            }
        }
    }

    void popup_menu_option::resize_self()
    {
        set_width( subOptionWidthSpace );
    }

    void popup_menu_option::update_selectability( bool selectable )
    {
        if( selectable!=isSelectable)
        {
            isSelectable = selectable;
        }
    }


    widget_toolbar::widget_toolbar()
    {
        widget_type = "toolbar";
        widget_name = "";
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = gpe::screen_width;
        widget_box.h = 16;

        barWidthTotal = 0;
        hoverOption = -1;
        selectedOption = -1;
        justClosed = false;
        openRequested = false;
        toolBarIsOpen = false;
        upDelay = 0;
        downDelay = 0;
        leftDelay = 0;
        rightDelay = 0;
    }

    widget_toolbar::~widget_toolbar()
    {

    }

    void widget_toolbar::close_toolbar()
    {
        justClosed = true;
        reset_options();
        hoverOption = -1;
        selectedOption = -1;
        toolBarIsOpen = false;
        upDelay = 0;
        downDelay = 0;
        leftDelay = 0;
        rightDelay = 0;
    }

    bool widget_toolbar::is_open()
    {
        return toolBarIsOpen;
    }

    bool widget_toolbar::just_closed()
    {
        return justClosed;
    }

    popup_menu_option * widget_toolbar::add_menu_option( std::string name, int id)
    {
        popup_menu_option * newOp = new popup_menu_option(name,id);
        newOp->isTopOfMenu = true;
        if( id<0)
        {
            newOp->set_id( (int)barOptions.size() );
        }
        newOp->menuDirection = gpe::fa_right;
        newOp->set_position( widget_box.x + barWidthTotal,widget_box.y);
        newOp->set_width( widget_box.h );
        barOptions.push_back(newOp);
        barWidthTotal+=newOp->get_width();
        return newOp;
    }

    void widget_toolbar::set_width(int newWid)
    {
        int pastWidth = widget_box.w;
        widget_box.w = newWid;

        if( pastWidth !=newWid )
        {
            popup_menu_option * gOption = nullptr;
            barWidthTotal = 0;
            for(int j=0; j<(int)barOptions.size(); j++)
            {
                gOption = barOptions[j];
                gOption->menuDirection = gpe::fa_right;
                gOption->set_position(widget_box.x + barWidthTotal,widget_box.y);
                gOption->set_height( widget_box.h );
                barWidthTotal+=gOption->get_width();
            }
        }
    }

    bool widget_toolbar::open_requested()
    {
        return openRequested;
    }

    void widget_toolbar::reset_options()
    {
        popup_menu_option * gOption = nullptr;
        int x_current = widget_box.x;
        for(int j=0; j<(int)barOptions.size(); j++)
        {
            gOption = barOptions[j];
            gOption->subMenuIsOpen = false;
            gOption->hoverOption = -1;
            gOption->selectedOption = -1;
            gOption->reset_suboptions();
            gOption->set_position(x_current, widget_box.y );
            x_current += gOption->get_width();
        }
    }

    void widget_toolbar::open_toolbar()
    {
        justClosed = false;
        if( toolBarIsOpen==false)
        {
            toolBarIsOpen = true;
            main_overlay_system->take_frozen_screenshot();
        }
    }

    void widget_toolbar::prerender_self(  )
    {
        popup_menu_option * tempOption = nullptr;
        for( int i = 0; i < (int)barOptions.size(); i++)
        {
            tempOption = barOptions[i];
            tempOption->prerender_self();
        }
    }

    void widget_toolbar::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if(view_space==nullptr || cam==nullptr)
        {
            return;
        }

        widget_basic::process_self(view_space,cam);

        justClosed = false;
        openRequested = false;
        int i;
        int drawXPos;
        int actionHappened = false;
        int barAction = -1;

        update_popup_info(); //resets the popup option to process
        if( gpe::window_controller_main->is_resized() )
        {
            gpe::error_log->report("Unable to process toolbar, window is resized....");
            hoverOption = -1;
            selectedOption = -1;
            reset_options();
            toolBarIsOpen = false;
            return;
        }
        popup_menu_option * fOption = nullptr;
        if( gpe::input->kb_button_released[kb_alt] & !gpe::input->kb_button_down[kb_ctrl] && !gpe::input->kb_button_down[kb_shift] && !gpe::input->kb_button_down[kb_tab] )
        {
            if( toolBarIsOpen)
            {
                hoverOption = -1;
                selectedOption = -1;
                reset_options();
                toolBarIsOpen = false;
            }
            else
            {
                reset_options();
                hoverOption = 0;

                selectedOption = 0;
                //resets the bar options
                if( (int)barOptions.size()>0)
                {
                    fOption = barOptions[0];
                    fOption->open_menu();
                }
                actionHappened = 2;
                open_toolbar();
                openRequested = true;
            }
        }


        if( barOptions.size()>0)
        {
            /*
            if( gpe::point_between_rect(gpe::input->mouse_position_x, gpe::input->mouse_position_y,&widget_box) )
            {
                if( gpe::input->check_mouse_released( mb_left))
                {
                    close_toolbar();
                }
            }*/

            drawXPos = widget_box.x;
            if( toolBarIsOpen )
            {
                if( gpe::input->kb_button_down[kb_up] && gpe::input->kb_button_pressed[kb_up]==false && gpe::input->kb_button_released[kb_up]==false )
                {
                    upDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    upDelay = -1;
                }

                if( gpe::input->kb_button_down[kb_down] && gpe::input->kb_button_pressed[kb_down]==false && gpe::input->kb_button_released[kb_down]==false )
                {
                    downDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    downDelay = -1;
                }

                if( gpe::input->kb_button_down[kb_left] && gpe::input->kb_button_pressed[kb_left]==false && gpe::input->kb_button_released[kb_left]==false )
                {
                    leftDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    leftDelay = -1;
                }

                if( gpe::input->kb_button_down[kb_right] && gpe::input->kb_button_pressed[kb_right]==false && gpe::input->kb_button_released[kb_right]==false )
                {
                    rightDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    rightDelay = -1;
                }
            }
            else
            {
                upDelay = -1;
                downDelay = -1;
                leftDelay = -1;
                rightDelay = -1;
            }

            int currentCursorPos = 0;
            if( selectedOption>=0 && selectedOption < (int)barOptions.size() )
            {
                fOption = barOptions[selectedOption];
                currentCursorPos = selectedOption;
            }
            else if( hoverOption>=0 && hoverOption < (int)barOptions.size() )
            {
                fOption = barOptions[hoverOption];
                currentCursorPos = hoverOption;
            }

            bool toolBarActionHappened = false;
            if( toolBarIsOpen )
            {
                if( leftDelay >= main_settings->normalInputDelayTime )
                {
                    leftDelay = -1;
                    toolBarActionHappened = true;
                    if( fOption!=nullptr)
                    {
                        if( fOption->push_left() ==false)
                        {
                            currentCursorPos--;
                            if( currentCursorPos < 0)
                            {
                                currentCursorPos = (int)barOptions.size()-1;
                            }
                            if(currentCursorPos < 0 )
                            {
                                currentCursorPos = 0;
                            }
                            reset_options();
                            hoverOption = selectedOption = currentCursorPos;
                            if( hoverOption>=0 && hoverOption < (int)barOptions.size() )
                            {
                                fOption = barOptions[currentCursorPos];
                                if( fOption!=nullptr)
                                {
                                    fOption->subMenuIsOpen = true;
                                }
                            }
                        }
                    }
                }
                else if( rightDelay >= main_settings->normalInputDelayTime )
                {
                    toolBarActionHappened = true;
                    rightDelay = -1;
                    if( fOption!=nullptr)
                    {
                        if( fOption->push_right() ==false )
                        {
                            currentCursorPos++;
                            if( currentCursorPos >=(int)barOptions.size() )
                            {
                                currentCursorPos = 0;
                            }
                            reset_options();
                            hoverOption = selectedOption = currentCursorPos;
                            if( hoverOption>=0 && hoverOption < (int)barOptions.size() )
                            {
                                fOption = barOptions[currentCursorPos];
                                if( fOption!=nullptr)
                                {
                                    fOption->subMenuIsOpen = true;
                                }
                            }
                        }
                    }
                }
                else if( upDelay >= main_settings->normalInputDelayTime )
                {
                    toolBarActionHappened = true;
                    if( fOption!=nullptr)
                    {
                        if( fOption->push_up() ==false )
                        {
                        }
                    }
                    upDelay = -1;
                }
                else if( downDelay >= main_settings->normalInputDelayTime )
                {
                    toolBarActionHappened = true;
                    if( fOption!=nullptr)
                    {
                        if( fOption->push_down() ==false )
                        {

                        }
                    }
                    downDelay = -1;
                }
                else if( ( gpe::input->kb_button_released[kb_enter] || gpe::input->kb_button_released[kb_space] ) && toolBarIsOpen  )
                {
                    toolBarActionHappened = true;
                    if( fOption!=nullptr)
                    {
                        if( fOption->activate_hovered() )
                        {
                            hoverOption = -1;
                            selectedOption = -1;
                            reset_options();
                            toolBarIsOpen = false;
                            close_toolbar();
                        }
                    }
                }
            }

            if( !toolBarActionHappened )
            {
                for(i=0; i<(int)barOptions.size(); i++)
                {
                    fOption = barOptions[i];
                    if( gpe::point_between(gpe::input->mouse_position_x, gpe::input->mouse_position_y,drawXPos,widget_box.y,drawXPos+fOption->get_width(),widget_box.y+widget_box.h) )
                    {
                        if( gpe::input->mouse_movement_received || hoverOption < 0 )
                        {
                            hoverOption = i;
                        }
                        if( selectedOption>=0 && toolBarIsOpen && gpe::input->mouse_movement_received )
                        {
                            reset_options();
                            hoverOption = i;

                            selectedOption = i;
                            //resets the bar options
                            fOption->open_menu();
                            actionHappened = 2;
                            open_toolbar();
                        }
                        else if( gpe::input->check_mouse_released( mb_left))
                        {
                            //resets the bar options
                            reset_options();
                            fOption->open_menu();
                            open_toolbar();
                            openRequested = true;
                            selectedOption=i;
                            hoverOption = -1;
                            actionHappened = 2;
                        }
                    }

                    fOption->process_self( view_space, cam );
                    barAction = fOption->get_browsing_results();
                    if(barAction >= 0 )
                    {
                        actionHappened = barAction;
                    }
                    drawXPos+=fOption->get_width();
                }
                if( gpe::input->check_mouse_released( mb_left))
                {
                    //resets the toolbar if the user clicks on the far right side of it.
                    if(actionHappened==false || actionHappened==1)
                    {
                        close_toolbar();
                    }
                }
            }
        }

        if( gpe::input->check_mouse_released(mb_middle) ||gpe::input->check_mouse_released(mb_right) || gpe::input->check_kb_released(kb_esc) )
        {
            close_toolbar();
        }


        if((int)action_message_text.size() > 0 )
        {
            close_toolbar();
            gpe::input->reset_all_input();
        }
    }

    void widget_toolbar::render_self( gpe::shape_rect * view_space, gpe::shape_rect  * cam )
    {
        if( toolBarIsOpen)
        {
            main_overlay_system->render_frozen_screenshot();
        }

        gpe::gcanvas->render_rect(&widget_box,pawgui::theme_main->popup_box_color,false);
        if( barOptions.size()==0)
        {
            gpe::gfs->render_text( widget_box.get_center(),widget_box.get_middle(),"(Toolbar)",pawgui::theme_main->main_box_font_color,nullptr,gpe::fa_center,gpe::fa_middle);
        }
        else
        {
            popup_menu_option * fOption = nullptr;
            int drawXPos = 0;
            int y2 = widget_box.y+widget_box.h;
            for(int i=0; i<(int)barOptions.size(); i++)
            {
                fOption = barOptions[i];
                if( hoverOption == i  || fOption->is_hovered() )
                {
                    gpe::gcanvas->render_rectangle( drawXPos,widget_box.y,drawXPos+fOption->get_width(),y2,pawgui::theme_main->popup_box_highlight_color, false);
                }
                fOption->render_self(nullptr);
                drawXPos+=fOption->get_width();
            }
        }
    }

    void context_menu_open(int menuXPos,int menuYPos, int new_width )
    {
        if( menuXPos<0)
        {
            menuXPos = gpe::input->mouse_position_x;
        }
        if( menuYPos<0)
        {
            menuYPos = gpe::input->mouse_position_y;
        }
        if( main_context_menu!=nullptr)
        {
            main_context_menu->clear_menu();
            main_context_menu->subMenuIsOpen = true;
            if( new_width > 0)
            {
                main_context_menu->set_width(new_width);
            }
            else
            {
                if( main_context_menu->get_width() < 128 )
                {
                    main_context_menu->set_width(128);
                }
            }
            main_context_menu->set_position( menuXPos, menuYPos );
        }
    }

    void context_menu_close()
    {
        if( main_context_menu!=nullptr && main_context_menu->subMenuIsOpen)
        {
            main_context_menu->clear_menu();
            main_context_menu->subMenuIsOpen = false;
            gpe::input->reset_all_input();
            if( main_overlay_system!=nullptr)
            {
                //main_overlay_system->render_frozen_screenshot();
            }
        }
        main_context_menu->close_menu();

    }

    int context_menu_process( gpe::shape_rect * cam, bool redrawScreen, bool autoResize )
    {
        gpe::game_runtime->end_loop();
        //resource_dragged = nullptr;
        gpe::cursor_main_controller->cursor_change("arrow");
        main_overlay_system->take_frozen_screenshot( );
        gpe::input->reset_all_input();
        int returnValue = -1;
        bool exitOperation = false;
        if( main_context_menu!=nullptr)
        {
            main_context_menu->hoverOption = main_context_menu->selectedOption = 0;
            if( autoResize )
            {
                //Resize popup 1st layer
                main_context_menu->resize_self();
            }
        }
        else
        {
            return -1;
        }
        bool firstFrame = true;

        /*
        Checks if we should move the menu direction based on x position
        Also if the x is too far to the right it right aligns it to the edge of the screen.
        */

        if( main_context_menu->get_xpos() + main_context_menu->optionWidthSpace >= gpe::screen_width )
        {
            main_context_menu->menuDirection = gpe::fa_left;
            main_context_menu->set_position( gpe::screen_width - main_context_menu->get_width() - main_context_menu->optionWidthSpace , main_context_menu->get_ypos() );
        }


        while(exitOperation==false)
        {
            gpe::cursor_main_controller->cursor_change("arrow");
            //Start the frame timer
            gpe::game_runtime->start_loop();
            //gpe::error_log->report("Viewing context menu...");
            if( gpe::input->window_input_received || gpe::window_controller_main->is_resized()  )
            {
                exitOperation = true;
                returnValue = -1;
                gpe::renderer_main->clear_renderer( gpe::window_controller_main->is_minimized() );
            }
            else
            {
                if( main_context_menu!=nullptr)
                {
                    if( main_context_menu->is_open() )
                    {
                        main_context_menu->process_self( nullptr, cam );
                        returnValue = main_context_menu->get_browsing_results();
                        if( gpe::input->check_mouse_released(kb_anykey)  )
                        {
                            //gpe::error_log->report("Releasing due to mouse input...");
                            context_menu_close();
                            exitOperation = true;
                        }
                        else if( gpe::window_controller_main->is_resized()   )
                        {
                            //gpe::error_log->report("Releasing due to WINDOW_WAS_JUST_RESIZED...");
                            context_menu_close();
                            exitOperation = true;
                        }
                        else if( gpe::input->kb_button_released[kb_esc] || gpe::input->kb_button_released[kb_enter] )
                        {
                            //gpe::error_log->report("Releasing due esc/enter...");
                            context_menu_close();
                            exitOperation = true;
                        }
                    }
                    else
                    {
                        exitOperation = true;
                    }

                    gpe::renderer_main->set_viewpoint( nullptr );
                    gpe::renderer_main->clear_renderer( gpe::window_controller_main->is_minimized() );
                    main_overlay_system->render_frozen_screenshot( );
                    //Update screen
                    main_context_menu->render_self(cam);
                }
                else
                {
                    exitOperation = true;
                    returnValue = -1;
                }
            }

            firstFrame = false;
            gpe::game_runtime->end_loop();
        }
        context_menu_close();
        gpe::game_runtime->start_loop();
        return returnValue;
    }
}
