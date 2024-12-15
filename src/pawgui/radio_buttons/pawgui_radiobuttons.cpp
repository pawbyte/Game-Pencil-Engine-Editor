/*
pawgui_radiobuttons.cpp
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


#include "pawgui_radiobuttons.h"

namespace pawgui
{
    widget_radio_button_controller::widget_radio_button_controller(std::string cName, bool alphabetize, int colNum)
    {
        widget_type = "radio";
        widget_type_id = 5;
        hoveredOption = -1;
        selectedId = 0;
        widget_name = cName;
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 0;
        widget_box.h = 32;
        opWidth = widget_box.w;
        opHeight = widget_box.h;

        if( (int)widget_name.size()>0)
        {
            int textW = 0;
            int textH = 0;
            gpe::font_default->get_metrics(widget_name,&textW, &textH);
            opWidth = textW;
            if( widget_box.w < textW )
            {
                widget_box.w = textW+padding_default*2;
            }
        }
        allowDuplicates = false;
        keepAlphabetized = alphabetize;
        numCols = std::max(0,colNum);
        rowsPerCol = -1;
    }

    widget_radio_button_controller::~widget_radio_button_controller()
    {
        gpe::key_pair * kp = nullptr;
        for( int i = sub_options.size()-1; i >=0; i--)
        {
            kp = sub_options[i];
            if( kp!=nullptr )
            {
                delete kp;
                kp = nullptr;
            }
        }
        sub_options.clear();
    }


    std::string widget_radio_button_controller::get_data()
    {
        std::string datastring = widget_type+":"+widget_name+"==|||==[menu]";
        gpe::key_pair * tPair = nullptr;
        for( int i = 0; i < (int)sub_options.size(); i++ )
        {
            tPair = sub_options[i];
            if( tPair!=nullptr)
            {
                datastring+="[option]"+tPair->key_string+":"+tPair->key_substring+":"+ stg_ex::int_to_string(tPair->key_value)+"[/option]";
            }
        }
        datastring+="[/menu]"+ stg_ex::int_to_string(selectedId)+",";
        return datastring;
    }

    void widget_radio_button_controller::load_data(std::string datastring)
    {
        if( (int)datastring.size() > 0 )
        {
            std::string optionTag = "[option]";
            int optionTagSize = (int)optionTag.size();

            std::string allOptionsstring = "";
            std::string newOptionData = "";
            std::string newOptionstring = "";
            std::string newOptionName = "";
            std::string newOptionSubame = "";
            int newOptionId = -1;
            int beginOptionPos=datastring.find_first_of(optionTag);
            if(beginOptionPos!=(int)std::string::npos)
            {
                allOptionsstring = stg_ex::split_first_string(datastring,"[/menu]");

                newOptionData = stg_ex::split_first_string(allOptionsstring,"[menu]");
                while( (int)allOptionsstring.size() > 0)
                {
                    newOptionstring = stg_ex::split_first_string(allOptionsstring,"[/option]");

                    beginOptionPos=newOptionstring.find_first_of(optionTag);
                    if(beginOptionPos!=(int)std::string::npos)
                    {
                        //if the beginOptionPos is present, then parse on through and carryon
                        if( (int)newOptionstring.size() > beginOptionPos+optionTagSize )
                        {
                            newOptionData = stg_ex::get_substring(newOptionstring, beginOptionPos+optionTagSize );

                            newOptionName = stg_ex::split_first_string(newOptionData,":");
                            newOptionSubame = stg_ex::split_first_string(newOptionData,":");
                            newOptionId = stg_ex::string_to_int(newOptionData,-1);
                            if( (int)newOptionName.size() > 0)
                            {
                                add_menu_option(newOptionName,newOptionSubame,newOptionId,false);
                            }
                        }
                        else
                        {
                            return;
                        }
                    }
                }
                set_selection( stg_ex::split_first_int(datastring,',') );
                //showJustOptions = string_to_bool( stg_ex::split_first_string(datastring,",") );
            }
        }
    }

    void widget_radio_button_controller::remove_data(std::string datastring)
    {
        if( (int)datastring.size() > 0)
        {
            std::string optionTag = "[option]";
            int optionTagSize = (int)optionTag.size();

            std::string newOptionData = "";
            std::string newOptionstring = "";
            std::string foundOptionName = "";
            int beginOptionPos = -1;
            while( (int)datastring.size() > 0)
            {
                newOptionstring = stg_ex::split_first_string(datastring,"[/option]");

                beginOptionPos=newOptionstring.find_first_of(optionTag);
                if(beginOptionPos!=(int)std::string::npos)
                {
                    //if the beginOptionPos is present, then parse on through and carryon
                    foundOptionName = newOptionstring.substr(beginOptionPos+optionTagSize,newOptionstring.length());
                    if( (int)foundOptionName.size() > 0)
                    {
                        remove_option(foundOptionName);
                    }
                }
            }
        }
    }

    void widget_radio_button_controller::add_opton(std::string newOption)
    {
        if( (int)newOption.size()>0 )
        {
            int textW = 0;
            int textH = 0;
            gpe::font_default->get_metrics(newOption,&textW, &textH);
            if( opWidth < textW )
            {
                opWidth = textW;
                widget_box.w = textW+padding_default*2;
                //newTex->loadFromRenderedText(gpe::renderer_main,newOption,pawgui::theme_main->main_box_font_color,gpe::font_default);
            }
            gpe::key_pair * kp = nullptr;
            gpe::key_pair * newOptionPair = new gpe::key_pair(-1,newOption,newOption);
            if(keepAlphabetized)
            {
                std::string fstring = "";
                bool isInserted = false;
                if( rowsPerCol>0)
                    for(int i=0; i< (int)sub_options.size() && !isInserted; i++)
                    {
                        kp = sub_options[i];
                        if( kp!=nullptr)
                        {
                            fstring = kp->key_string;
                            if( fstring > newOption && !isInserted )
                            {
                                sub_options.insert( sub_options.begin()+i,newOptionPair);
                                isInserted = true;
                                organize_options();
                                return;
                            }
                        }
                    }
            }
            sub_options.push_back(newOptionPair);
            widget_box.h +=32;
            organize_options();
        }
    }

    void widget_radio_button_controller::add_menu_option(std::string optionName, std::string optionSubStr,int optionValue, bool selectOption)
    {
        if( (int)optionName.size()>0 )
        {
            int textW = 0;
            int textH = 0;
            gpe::font_default->get_metrics(optionName,&textW, &textH);
            if( opWidth < textW )
            {
                opWidth = textW;
                widget_box.w = textW+padding_default*2;
                //newTex->loadFromRenderedText(gpe::renderer_main,optionName,pawgui::theme_main->main_box_font_color,gpe::font_default);
            }
            bool optionExists = false;
            gpe::key_pair * tOption = nullptr;
            for( int i = 0; i < (int)sub_options.size(); i++)
            {
                tOption = sub_options[i];
                if( tOption!=nullptr )
                {
                    if( tOption->key_string==optionName)
                    {
                        optionExists = false;
                        return;
                    }
                }
            }
            if( !optionExists)
            {
                gpe::key_pair * kp = new gpe::key_pair(optionValue,optionName,optionSubStr, optionValue );
                sub_options.push_back(kp);
                if( selectOption )
                {
                    set_selection( (int)sub_options.size()-1 );
                }
                widget_box.h +=32;
                organize_options();
            }
        }
    }

    std::string widget_radio_button_controller::get_menu_option(int atNumb)
    {
        gpe::key_pair * kp = nullptr;
        if( atNumb >=0 && atNumb < (int)sub_options.size() )
        {
            kp = sub_options.at(atNumb);
            if( kp!=nullptr)
            {
                return kp->key_string;
            }
        }
        return "";
    }

    std::string widget_radio_button_controller::get_plain_string()
    {
        gpe::key_pair * kp = nullptr;
        if( selectedId >=0 && selectedId < (int)sub_options.size() )
        {
            kp = sub_options[selectedId];
            if( kp!=nullptr)
            {
                return "'"+kp->key_string+"'";
            }
        }
        return "''";
    }

    int widget_radio_button_controller::get_selected_id()
    {
        return selectedId;
    }

    std::string widget_radio_button_controller::get_selected_tag()
    {
        gpe::key_pair * kp = nullptr;
        if( selectedId >=0 && selectedId < (int)sub_options.size() )
        {
            kp = sub_options[selectedId];
            return kp->key_substring;
        }
        return "";
    }

    int widget_radio_button_controller::get_selected_value()
    {
        gpe::key_pair * kp = nullptr;
        if( selectedId >=0 && selectedId < (int)sub_options.size() )
        {
            kp = sub_options[selectedId];
            return kp->key_value;
        }
        return 0;
    }

    void widget_radio_button_controller::organize_options()
    {
        widget_box.w = padding_default*2;
        widget_box.h = 32;
        int maxWidthText = 0;
        int textW = 0;
        int textH = 0;
        if( (int)widget_name.size()>0)
        {
            textW = 0;
            textH = 0;
            gpe::font_default->get_metrics(widget_name,&textW, &textH);
            opWidth = textW;
            maxWidthText = textW;
            widget_box.w += maxWidthText;
        }

        std::string opStr = "";
        gpe::key_pair * tPair = nullptr;
        if( numCols<=1)
        {
            for(int i =0; i < (int)sub_options.size(); i++)
            {
                tPair = sub_options[i];
                if( tPair!=nullptr)
                {
                    opStr = tPair->key_string;
                    if( (int)opStr.size() > 0)
                    {
                        textW = 0;
                        textH = 0;
                        gpe::font_default->get_metrics(opStr,&textW, &textH);
                        if( maxWidthText < textW )
                        {
                            maxWidthText = textW;
                        }
                    }
                    widget_box.h+=32;
                }
            }
            widget_box.w+=maxWidthText;
        }
    }

    void widget_radio_button_controller::remove_option(std::string optionToCut)
    {
        if( (int)sub_options.size()>0 )
        {
            int removePos = -1;
            gpe::key_pair * kp = nullptr;
            for(int i=(int)sub_options.size()-1; i>=0 && removePos < 0; i--)
            {
                kp = sub_options[i];
                if( kp!=nullptr)
                {
                    if(  kp->key_string == optionToCut)
                    {
                        delete kp;
                        kp = nullptr;
                        sub_options.erase(sub_options.begin()+i);
                        if( i==selectedId)
                        {
                            i = -1;
                        }
                        else if( selectedId > i)
                        {
                            selectedId-=1;
                        }
                        removePos = i;
                        widget_box.h -=32;
                    }
                }
            }
        }
    }

    void widget_radio_button_controller::remove_option_id(int optionToCut)
    {
        if( (int)sub_options.size()>0 )
        {
            if(optionToCut >=0 && optionToCut < (int)sub_options.size() )
            {
                gpe::key_pair * kp = sub_options.at( optionToCut );
                if( kp!=nullptr)
                {
                    delete kp;
                    kp = nullptr;
                }
                sub_options.erase(sub_options.begin()+optionToCut);
                if( optionToCut==selectedId)
                {
                    optionToCut = -1;
                }
                else if( selectedId > optionToCut)
                {
                    selectedId-=1;
                }
                widget_box.h -=32;
            }
        }
    }

    void widget_radio_button_controller::set_selection(int newSelection)
    {
        if( newSelection>=0 && newSelection < (int)sub_options.size() )
        {
            selectedId = newSelection;
        }
    }

    bool widget_radio_button_controller::set_from_tag(std::string newselectedTag)
    {
        gpe::key_pair * tempPair = nullptr;
        for( int i = (int)(sub_options.size())-1; i >=0; i--)
        {
            tempPair = sub_options[i];
            if( tempPair!=nullptr)
            {
                if( tempPair->key_substring == newselectedTag)
                {
                    selectedId = i;
                    return true;
                }
            }
        }
        return false;
    }

    void widget_radio_button_controller::set_value(int valueToSelect)
    {
        gpe::key_pair * tempPair = nullptr;
        for( int i = (int)sub_options.size()-1; i>=0; i--)
        {
            tempPair = sub_options[i];
            if( tempPair!=nullptr)
            {
                if( tempPair->key_value==valueToSelect)
                {
                    selectedId = i;
                    break;
                }
            }
        }
    }

    void widget_radio_button_controller::prerender_self(  )
    {

    }

    void widget_radio_button_controller::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);
        hoveredOption = -1;
        if( cam!=nullptr && view_space!=nullptr)
        {
            int buttonXPos = widget_box.x+view_space->x+16-cam->x;
            int buttonYPos = widget_box.y+view_space->y+32-cam->y;
            widget_basic::process_self(view_space,cam);
            bool selectionClicked = false;
            if( isInUse)
            {
                hasArrowkeyControl = true;
            }
            else
            {
                hasArrowkeyControl = false;
            }
            //currently only handles single columns
            if( numCols<=1)
            {
                for(int i = 0; i  < (int)sub_options.size(); i++)
                {
                    if(gpe::point_between( gpe::input->mouse_position_x,gpe::input->mouse_position_y,buttonXPos,buttonYPos,buttonXPos+32,buttonYPos+32) )
                    {
                        hoveredOption = i;
                        if( gpe::input->check_mouse_released( mb_left))
                        {
                            selectedId = i;
                            selectionClicked = true;
                        }
                    }
                    buttonYPos+=32;
                }
                if( hasArrowkeyControl && isInUse )
                {
                    if( gpe::input->check_kb_down(kb_left) || gpe::input->check_kb_down(kb_up) )
                    {
                        selectedId--;
                    }
                    if( gpe::input->check_kb_down(kb_right) || gpe::input->check_kb_down(kb_down) )
                    {
                        selectedId++;
                    }
                    if( selectedId >=(int)sub_options.size() )
                    {
                        selectedId =(int)sub_options.size()-1;
                    }
                    if( selectedId < 0)
                    {
                        selectedId = 0;
                    }
                }
            }
            else
            {

            }
            if( isClicked && !selectionClicked)
            {
                hasArrowkeyControl = true;
            }
        }
    }

    void widget_radio_button_controller::render_self(  gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        //if( forceRedraw )
        {
            view_space = gpe::camera_find(view_space);
            cam = gpe::camera_find(cam);
            if( view_space!=nullptr && cam!=nullptr)
            {
                int buttonXPos = widget_box.x-cam->x;
                int buttonYPos = widget_box.y-cam->y;
                if(showBorderBox)
                {
                    if((int)widget_name.size()>0 )
                    {
                        gpe::gcanvas->render_line_color( buttonXPos,buttonYPos+default_padding_title,buttonXPos+default_padding_title,buttonYPos+default_padding_title,pawgui::theme_main->button_box_color,255);
                        gpe::gcanvas->render_line_color( buttonXPos+default_padding_title*2+opWidth,buttonYPos+default_padding_title,
                                          buttonXPos+widget_box.w,buttonYPos+default_padding_title,pawgui::theme_main->button_box_color,255);

                        gpe::gcanvas->render_line_color( buttonXPos,buttonYPos+default_padding_title,buttonXPos,buttonYPos+widget_box.h,pawgui::theme_main->button_box_color,255);
                        gpe::gcanvas->render_line_color( buttonXPos,buttonYPos+widget_box.h,buttonXPos+widget_box.w,buttonYPos+widget_box.h,pawgui::theme_main->button_box_color,255);
                        gpe::gcanvas->render_line_color( buttonXPos+widget_box.w,buttonYPos+default_padding_title,buttonXPos+widget_box.w,buttonYPos+widget_box.h,pawgui::theme_main->button_box_color,255);
                        gpe::gfs->render_text(  buttonXPos+default_padding_title*1.5,buttonYPos+default_padding_title/2,widget_name,pawgui::theme_main->main_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top,255);
                    }
                    else
                    {
                        gpe::gcanvas->render_rectangle( buttonXPos,buttonYPos,buttonXPos+widget_box.w,buttonYPos+widget_box.h,pawgui::theme_main->button_box_color,255);
                    }
                    buttonXPos += 16;
                    buttonYPos += 32;
                    int renderedInCol = 0;

                    gpe::key_pair * kp = nullptr;
                    for(int i = 0; i  < (int)sub_options.size(); i++)
                    {
                        if( i == hoveredOption )
                        {
                            gpe::gcanvas->render_circle_filled_color( buttonXPos+16,buttonYPos+8,10, pawgui::theme_main->button_border_highlight_color, 255 );
                        }
                        gpe::gcanvas->render_circle_filled_color( buttonXPos+16,buttonYPos+8,8, pawgui::theme_main->button_box_color, 255 );

                        if( i == selectedId )
                        {
                            gpe::gcanvas->render_circle_filled_color( buttonXPos+16,buttonYPos+8,4, pawgui::theme_main->button_box_selected_color, 255 );
                        }
                        kp = sub_options[i];
                        if( kp!=nullptr)
                        {
                            gpe::gfs->render_text(  buttonXPos+32,buttonYPos,kp->key_string,pawgui::theme_main->main_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top,255);
                        }
                        buttonYPos+=32;
                        renderedInCol+=1;
                        /*
                        if( rowsPerCol>0 && numCols>1 )
                        {
                            if(renderedInCol > rowsPerCol)
                            {
                                renderedInCol = 0;
                                buttonXPos+=160; //will be changed later from constant
                                buttonYPos = widget_box.y+32;
                            }
                        }
                        */
                    }
                }
                if( isInUse)
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_color,true);
                }
            }
        }
    }
}
