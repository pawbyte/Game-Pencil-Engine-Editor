/*
pawgui_selectbox.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_selectbox.h"

namespace pawgui
{
    selectbox_option::selectbox_option()
    {
        optionValue = 0;
        optionName = "";
        optionTexture = NULL;
        optionanimation = NULL;
        subimageIndex = 0;
        isClicked = true;
        useGuiColor= false;
        sectionHasContent = true;
    }

    selectbox_option::~selectbox_option()
    {

    }


    void selectbox_option::prerender_self(  )
    {

    }

    widget_selectbox::widget_selectbox( std::string name)
    {
        upDelay = 0;
        downDelay = 0;
        widget_type = "selectbox";
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 192;
        widget_box.h = padding_default;
        maxHeight = -1;
        optionHeight = -1;
        widget_name = name;
        intedPos = 0;
        pos = 0;
        startPos = 0;
        maxOptionsInView = 10;
        optionHeight = default_padding_title+padding_default*2;
        optionIconWidth = default_line_height;
        showCheckboxes = false;
        showHideOthersCheckboxToggle = true;
        optionsScroller = new widget_scrollbar_yaxis();
    }

    widget_selectbox::~widget_selectbox()
    {
        selectbox_option * tOption = NULL;
        for( int i = subOptions.size()-1; i>=0; i--)
        {
            tOption = subOptions[i];
            if( tOption!=NULL)
            {
                delete tOption;
                tOption = NULL;
            }
        }
        subOptions.clear();

        if( optionsScroller!=NULL )
        {
            delete optionsScroller;
            optionsScroller = NULL;
        }
    }

    void widget_selectbox::correct_camera()
    {
        maxOptionsInView = floor( (float)widget_box.h / (float)optionHeight );
        //checks if pos is no longer in view
        if( pos < 0 )
        {
            pos = 0;
        }
        if( pos < startPos )
        {
            //startPos = pos-maxOptionsInView;
        }
        else if( pos >= startPos+maxOptionsInView )
        {
            //startPos = pos - maxOptionsInView-1;
        }

        if( startPos < 0)
        {
            startPos = 0;
        }

        //correctos the start pos if its out of view
        if( startPos+maxOptionsInView > (float)subOptions.size() )
        {
            startPos = (float)subOptions.size() - maxOptionsInView;
        }
        if( startPos < 0)
        {
            startPos = 0;
        }

        optionsScroller->update_box(  widget_box.x+widget_box.w-16, widget_box.y, 16,widget_box.h);
        gpe::update_rectangle(&optionsScroller->fullRect, 0, 0, 0,(float)subOptions.size() );
        gpe::update_rectangle(&optionsScroller->contextRect, 0, (float)startPos,0, (float)maxOptionsInView );
    }

    std::string widget_selectbox::get_data()
    {
        return "";
    }

    void widget_selectbox::load_data(std::string dataString)
    {

    }


    void widget_selectbox::add_option(std::string newOptionName, float newOpValue, gpe::texture_base * evRepIcon, gpe::animaton2d * evRepanimation, int subimageInIndex, bool selectNew, bool useGuiColor )
    {
        if( (int)newOptionName.size() > 0 || evRepIcon!=NULL || evRepanimation!=NULL )
        {
            selectbox_option * newOption = new selectbox_option();
            newOption->optionName = newOptionName;
            newOption->optionValue = newOpValue;
            newOption->optionanimation = evRepanimation;
            newOption->optionTexture = evRepIcon;
            newOption->useGuiColor = useGuiColor;
            //newOption->optionNameTexture->loadFromRenderedText(gpe::renderer_main,newOptionName,pawgui::theme_main->main_box_font_color,font_textinput);

            newOption->subimageIndex = subimageInIndex;

            subOptions.push_back(newOption);
            if( maxHeight < 0)
            {
                widget_box.h+=optionHeight;
            }
            if( selectNew)
            {
                set_selection( (int)subOptions.size() - 1 );
            }
            startPos++;
        }
    }

    void widget_selectbox::clear_list()
    {
        selectbox_option * tempOption = NULL;
        for( int i = (int)subOptions.size()-1; i >=0; i--)
        {
            tempOption = subOptions[i];
            if( tempOption!=NULL)
            {
                delete tempOption;
                tempOption = NULL;
            }
        }
        subOptions.clear();
        pos = 0;
        startPos = 0;
    }

    int widget_selectbox::get_option_id(std::string optionName )
    {
        selectbox_option * tempOption = NULL;
        for( int i = (int)subOptions.size()-1; i >=0; i--)
        {
            tempOption = subOptions[i];
            if( tempOption!=NULL)
            {
                if( tempOption->optionName == optionName )
                {
                    return i;
                }
            }
        }
        return -1;
    }

    selectbox_option * widget_selectbox::get_option(int optionId)
    {
        if( optionId >=0 && optionId < (int)subOptions.size() )
        {
            selectbox_option * tOption = subOptions.at(optionId);
            if( tOption!=NULL)
            {
                return tOption;
            }
        }
        return NULL;
    }

    std::string widget_selectbox::get_option_name(int optionId)
    {
        if( optionId >=0 && optionId < (int)subOptions.size() )
        {
            selectbox_option * tOption = subOptions.at(optionId);
            if( tOption!=NULL)
            {
                return tOption->optionName;
            }
        }
        return "";
    }

    std::string widget_selectbox::get_selected_name()
    {
        if( pos >=0 && pos < (int)subOptions.size() )
        {
            selectbox_option * nOption = subOptions[pos];
            if( nOption!=NULL)
            {
                return nOption->optionName;
            }
        }
        return "";
    }

    float widget_selectbox::get_selected_value()
    {
        if( pos >=0 && pos < (int)subOptions.size() )
        {
            selectbox_option * nOption = subOptions[pos];
            if( nOption!=NULL)
            {
                return nOption->optionValue;
            }
        }
        return 0;
    }

    int widget_selectbox::get_selection()
    {
        return pos;
    }

    int widget_selectbox::get_size()
    {
        return (int)subOptions.size();
    }

    void widget_selectbox::insert_option(int optionId, std::string newOptionName, gpe::texture_base * evRepIcon, gpe::animaton2d * evRepanimation, int subimageInIndex, bool selectNew )
    {
        if( (int)newOptionName.size() > 0 || evRepIcon!=NULL || evRepanimation!=NULL )
        {
            selectbox_option * newOption = new selectbox_option();
            newOption->optionName = newOptionName;
            newOption->optionanimation = evRepanimation;
            newOption->optionTexture = evRepIcon;
            newOption->subimageIndex = subimageInIndex;

            if( optionId >=0 && optionId < (int)subOptions.size() )
            {
                subOptions.insert( subOptions.begin()+optionId,newOption );
                startPos++;
            }
            else
            {
                subOptions.push_back( newOption );
            }
            if( maxHeight < 0)
            {
                widget_box.h+=optionHeight;
            }
            if( selectNew)
            {
                set_selection( optionId );
            }
            else if( pos <= optionId )
            {
                set_selection( pos -1 );
            }
        }
    }

    void widget_selectbox::limit_height(int newH)
    {
        if( newH >=0)
        {
            maxHeight = newH;
            widget_box.h = newH;
        }
        else
        {
            maxHeight = -1;
        }
        maxOptionsInView = floor( (float)widget_box.h / (float)optionHeight );
    }

    void widget_selectbox::alter_content( int optionId, bool sectionHasContent)
    {
        if( optionId >=0 && optionId < (int)subOptions.size() )
        {
            if( subOptions[optionId]!=NULL)
            {
                subOptions[optionId]->sectionHasContent = sectionHasContent;
            }
        }
    }

    void widget_selectbox::alter_content_from_value( float valueId, bool sectionHasContent)
    {
        for( int i = (int)subOptions.size()-1; i >=0; i--)
        {
            if( subOptions[i]!=NULL && subOptions[i]->optionValue==valueId )
            {
                subOptions[i]->sectionHasContent = sectionHasContent;
                break;
            }
        }
    }

    bool widget_selectbox::move_down_space()
    {
        if( pos +1 < (int)subOptions.size() )
        {
            selectbox_option * tempVar = subOptions[pos+1];
            subOptions[pos+1] = subOptions[pos];
            subOptions[pos] = tempVar;
            pos++;
            return true;
        }
        return false;
    }

    bool widget_selectbox::move_up_space()
    {
        if( pos > 0 && (int)subOptions.size() > pos-1)
        {
            selectbox_option * tempVar = subOptions[pos-1];
            subOptions[pos-1] = subOptions[pos];
            subOptions[pos] = tempVar;
            pos--;
            return true;
        }
        return false;
    }

    void widget_selectbox::prerender_self(  )
    {
        selectbox_option * tOption = NULL;
        for( int i = 0; i < (int)subOptions.size() && i < (int)subOptions.size(); i++)
        {
            tOption = subOptions[i];
            if( tOption!=NULL)
            {
                tOption->prerender_self( );
            }
        }

    }

    void widget_selectbox::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);

        correct_camera();

        //only works if scrollbar is actually needed
        if( maxOptionsInView <= (int)subOptions.size() )
        {
            optionsScroller->process_self( view_space, cam );
            startPos = (float)(optionsScroller->contextRect.y);
        }

        bool scrollHappened = false;
        if( gpe::input->check_kb_pressed(kb_esc) )
        {
            hasArrowkeyControl = false;
            hasScrollControl = false;
            isInUse = false;
        }

        if( isInUse)
        {
            hasArrowkeyControl = true;
            hasScrollControl = true;

            if( gpe::input->kb_button_released[kb_up] )
            {
                pos-=1;
                if( pos < 0 )
                {
                    pos= 0;
                }
                if( pos == startPos )
                {
                    startPos--;
                }
                scrollHappened = true;
            }
            else if( gpe::input->kb_button_released[kb_down] )
            {
                pos+=1;
                if( pos >= (int)subOptions.size() )
                {
                    pos=(int)subOptions.size()-1;
                }
                if( pos>=startPos+maxOptionsInView )
                {
                    startPos = pos;
                    if( startPos+maxOptionsInView >= (int)subOptions.size() )
                    {
                        startPos = (int)subOptions.size()-maxOptionsInView;
                        if( startPos < 0)
                        {
                            startPos = 0;
                        }
                    }
                }
                scrollHappened = true;
            }
            else if( gpe::input->check_kb_released(kb_space ) && showCheckboxes)
            {
                if( pos >=0 && pos < (int)subOptions.size() )
                {
                    selectbox_option *  optionToChange = subOptions[pos];
                    optionToChange->isClicked = !optionToChange->isClicked;
                }
            }
        }
        else
        {
            hasArrowkeyControl = false;
            hasScrollControl = false;
        }
        if( isInUse && hasScrollControl && optionsScroller->hasScrollControl==false )
        {
            if( gpe::input->mouse_scrolling_up > 0)
            {
                startPos--;
                scrollHappened = true;
            }
            if( gpe::input->mouse_scrolling_down > 0)
            {
                startPos++;
                scrollHappened = true;
            }
        }

        if( startPos >= (float)subOptions.size()-maxOptionsInView )
        {
            startPos = (float)subOptions.size()-maxOptionsInView;
        }

        if( startPos < 0 )
        {
            startPos= 0;
        }

        widget_basic::process_self(view_space,cam);
        if( isClicked )
        {
            isInUse = true;
            int checkBoxWidth = optionHeight+padding_default;
            int checkBoxHeight = optionHeight;
            if( showCheckboxes==false)
            {
                checkBoxWidth = 0;
                checkBoxHeight = 0;
            }
            //if( gpe::input->check_mouse_released( mb_left))
            {
                int relativeOptionX = view_space->x+widget_box.x-cam->x;
                int relativeOptionY = view_space->y+widget_box.y-cam->y;
                for( int i = 0; i <= maxOptionsInView && i < (int)subOptions.size(); i++)
                {
                    if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,relativeOptionX,relativeOptionY+(i)*optionHeight,
                                      relativeOptionX+widget_box.w,relativeOptionY+(i+1)*optionHeight) )
                    {
                        if( showCheckboxes)
                        {
                            if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,relativeOptionX,relativeOptionY+(i)*optionHeight,
                                              relativeOptionX+checkBoxWidth,relativeOptionY+(i+1)*optionHeight) )
                            {
                                selectbox_option * tOp = NULL;
                                int tPos = i+startPos;
                                if( tPos >=0 && tPos < (int)subOptions.size() )
                                {
                                    tOp = subOptions.at(tPos);
                                    if( tOp!=NULL)
                                    {
                                        tOp->isClicked = !tOp->isClicked;
                                    }
                                }
                            }
                            else
                            {
                                pos = i+startPos;
                            }
                        }
                        else
                        {
                            pos = i+startPos;
                        }
                    }
                }
            }
        }
        else if(clickedOutside)
        {
            isInUse = false;
            hasScrollControl = false;
            hasArrowkeyControl = false;
        }

        if( maxOptionsInView <= (int)subOptions.size() )
        {
            if( scrollHappened && optionsScroller!=NULL )
            {
                correct_camera();
                optionsScroller->process_self(NULL,NULL );
                startPos = optionsScroller->contextRect.y;
            }
        }
        intedPos = (int)pos;
        //gpe::error_log->report("Success on select box...");
    }

    void widget_selectbox::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( cam!=NULL && view_space!=NULL )
        {
            intedPos = (int)pos;
            int relativeOptionX = widget_box.x-cam->x;
            int relativeOptionY = widget_box.y-cam->y;
            int foundIconWidth = 0;
            if( optionIconWidth > 0 )
            {
                foundIconWidth = std::max( optionIconWidth,optionHeight )*3/4;
            }
            else
            {
                foundIconWidth = 0;
            }
            gpe::gcanvas->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+widget_box.w,relativeOptionY+widget_box.h,pawgui::theme_main->popup_box_color, false);
            maxOptionsInView = widget_box.h / optionHeight;
            int iStartPos = (int)startPos;

            if( pos>=iStartPos && pos < iStartPos+maxOptionsInView )
            {
                if( (int)subOptions.size() > 0)
                {
                    gpe::gcanvas->render_rectangle( relativeOptionX,relativeOptionY+(pos-iStartPos)*optionHeight,relativeOptionX+widget_box.w,relativeOptionY+(pos-iStartPos+1)*optionHeight,pawgui::theme_main->button_border_color,false);
                }
                else if( pos > 0)
                {
                    gpe::gcanvas->render_rectangle( relativeOptionX,relativeOptionY+(pos-iStartPos)*optionHeight,relativeOptionX+widget_box.w,relativeOptionY+(pos-iStartPos+1)*optionHeight,pawgui::theme_main->button_border_color,false);
                }
            }
            else if( pos >= startPos+maxOptionsInView )
            {
                //pos = startPos+maxOptionsInView -1;
            }
            selectbox_option * tOption = NULL;
            int checkBoxWidth = optionHeight+padding_default;
            int checkBoxHeight = optionHeight;
            if( showCheckboxes==false)
            {
                checkBoxWidth = 0;
                checkBoxHeight = 0;
            }

            gpe::color * fontRenderColor = pawgui::theme_main->main_box_faded_font_color;
            for( int i = iStartPos; i <= (int)(iStartPos+maxOptionsInView) &&  i < (int)subOptions.size(); i++)
            {
                tOption = subOptions.at( i );
                if( tOption!=NULL)
                {
                    if( !tOption->sectionHasContent && intedPos!=i)
                    {
                        fontRenderColor = pawgui::theme_main->popup_box_font_color;
                        gpe::gcanvas->render_rectangle( relativeOptionX,relativeOptionY+(i-iStartPos)*optionHeight,relativeOptionX+widget_box.w,relativeOptionY+(i-iStartPos+1)*optionHeight,pawgui::theme_main->program_color_header,false,228);
                    }
                    else if( intedPos ==i )
                    {
                        fontRenderColor = pawgui::theme_main->popup_box_highlight_font_color;
                    }
                    else
                    {
                        fontRenderColor = pawgui::theme_main->popup_box_font_color;
                    }

                    if( showCheckboxes)
                    {
                        gpe::gcanvas->render_rectangle( relativeOptionX+relativeOptionX+padding_default+optionHeight/8,relativeOptionY+1+(i-iStartPos)*optionHeight+optionHeight/8,relativeOptionX+relativeOptionX+padding_default+optionHeight/2+optionHeight/8,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight*5/8,pawgui::theme_main->button_box_color, false);
                        if( tOption->isClicked && checkmark_texture!=NULL )
                        {
                            checkmark_texture->render_tex_resized( relativeOptionX+relativeOptionX+padding_default+optionHeight/8,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight/8,optionHeight/2,optionHeight/2,NULL,pawgui::theme_main->checkbox_color );
                        }
                        //gpe::gcanvas->render_rectangle( relativeOptionX+relativeOptionX+padding_default,relativeOptionY+1+(i-iStartPos)*optionHeight,relativeOptionX+relativeOptionX+padding_default+optionHeight,relativeOptionY+(i-iStartPos+1)*optionHeight,pawgui::theme_main->button_box_selected_color, true);
                        gpe::gcanvas->render_rectangle( relativeOptionX+relativeOptionX+padding_default+optionHeight/8,relativeOptionY+1+(i-iStartPos)*optionHeight+optionHeight/8,relativeOptionX+relativeOptionX+padding_default+optionHeight/2+optionHeight/8,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight*5/8,pawgui::theme_main->button_box_selected_color, true);

                        gpe::gfs->render_text( relativeOptionX+padding_default+checkBoxWidth,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight/2,tOption->optionName,fontRenderColor,font_textinput,gpe::fa_left,gpe::fa_center,255);
                    }
                    else
                    {
                        gpe::gfs->render_text( relativeOptionX+foundIconWidth+padding_default*2,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight/2,tOption->optionName,fontRenderColor,FONT_LABEL,gpe::fa_left,gpe::fa_middle,255);
                    }
                    if( tOption->optionTexture!=NULL)
                    {
                        if( tOption->useGuiColor)
                        {
                            tOption->optionTexture->render_align_resized( relativeOptionX+padding_default,relativeOptionY+(i-iStartPos)*optionHeight+(optionHeight)/2,foundIconWidth,foundIconWidth,gpe::fa_left,gpe::fa_middle, NULL,pawgui::theme_main->popup_box_font_color );
                        }
                        else
                        {
                            tOption->optionTexture->render_align_resized( relativeOptionX+padding_default,relativeOptionY+(i-iStartPos)*optionHeight+(optionHeight)/2,foundIconWidth,foundIconWidth, gpe::fa_left, gpe::fa_middle, NULL );
                        }
                    }
                    else if( tOption->optionanimation!=NULL)
                    {
                        if( tOption->useGuiColor)
                        {
                            //tOption->optionanimation->render_special( tOption->subimageIndex,relativeOptionX+padding_default+checkBoxWidth,relativeOptionY+(i-iStartPos)*optionHeight +(optionHeight-foundIconWidth)/2,foundIconWidth,foundIconWidth,pawgui::theme_main->main_box_font_color,cam);
                        }
                        else
                        {
                            //tOption->optionanimation->render_resized( tOption->subimageIndex,relativeOptionX+padding_default+checkBoxWidth,relativeOptionY+(i-iStartPos)*optionHeight +(optionHeight-foundIconWidth)/2,foundIconWidth,foundIconWidth,cam);
                        }
                    }
                }
            }

            if( iStartPos > 0 )
            {
                gpe::gcanvas->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+widget_box.w,relativeOptionY+4,pawgui::theme_main->button_box_selected_color,false,228);
            }
            else if( iStartPos+maxOptionsInView < (int)subOptions.size() )
            {
                gpe::gcanvas->render_rectangle( relativeOptionX,relativeOptionY+widget_box.h-4,relativeOptionX+widget_box.w,relativeOptionY+widget_box.h,pawgui::theme_main->button_box_selected_color,false,228);
            }
            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+widget_box.w,relativeOptionY+widget_box.h,pawgui::theme_main->button_box_selected_color, true);
            }
            else
            {
                gpe::gcanvas->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+widget_box.w,relativeOptionY+widget_box.h,pawgui::theme_main->main_border_color, true );
            }

            if( maxOptionsInView < (int)subOptions.size() )
            {
                optionsScroller->render_self( view_space, cam );
            }
        }
    }

    void widget_selectbox::rename_option(int optionId, std::string new_name)
    {
        if( optionId >=0 && optionId < (int)subOptions.size() )
        {
            selectbox_option * tOption = subOptions.at(optionId);
            if(tOption!=NULL )
            {
                tOption->optionName = new_name;
            }
        }
    }

    void widget_selectbox::remove_option(int optionId)
    {
        if( optionId>=0 && optionId < (int)subOptions.size() )
        {
            selectbox_option * tOption = subOptions.at(optionId);
            if(tOption!=NULL )
            {
                delete tOption;
                tOption = NULL;
            }
            subOptions.erase(subOptions.begin() + optionId);
            set_selection(pos);
        }
    }

    void widget_selectbox::set_option_height( int newOptionHeight)
    {
        if( newOptionHeight > 0)
        {
            optionHeight = newOptionHeight;
        }
        maxOptionsInView = floor( (float)widget_box.h / (float)optionHeight );
    }

    void widget_selectbox::set_selected_option( std::string optionToSelect)
    {
        selectbox_option * tempOption = NULL;
        for( int i = (int)subOptions.size()-1; i >=0; i--)
        {
            tempOption = subOptions[i];
            if( tempOption!=NULL)
            {
                if( tempOption->optionName==optionToSelect)
                {
                    pos = i;
                    if( i < startPos)
                    {
                        startPos = i;
                    }
                    else
                    {
                        startPos = i;
                    }
                }
            }
        }
    }

    void widget_selectbox::set_selection( int selId )
    {
        if( selId >= (int)subOptions.size() )
        {
            selId = (int)subOptions.size()-1;
        }

        if( selId < 0)
        {
            selId = 0;
        }
        pos = selId;

        correct_camera();
    }

    void widget_selectbox::toggle_others_checked()
    {
        showHideOthersCheckboxToggle = !showHideOthersCheckboxToggle;
        selectbox_option * tOption = NULL;
        for(int i = 0; i < (int)subOptions.size(); i++)
        {
            if( i!=pos)
            {
                tOption = subOptions[i];
                if( tOption!=NULL)
                {
                    tOption->isClicked = showHideOthersCheckboxToggle;
                }
            }
        }
    }
}
