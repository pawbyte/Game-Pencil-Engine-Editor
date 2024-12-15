/*
pawgui_panel_list.cpp
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

#include "pawgui_panel_list.h"

namespace pawgui
{
    widget_content_row::widget_content_row()
    {
        rowClassType = row_type_general;
        lastColumnFloatsRight = true;
        calculatedRowWidth = 0;
        calculatedRowHeight = 0;
        inDebugMode = false;
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 0;
        widget_box.h = 0;
        alignment_h = gpe::fa_left;
        alignment_v = gpe::fa_top;
        barXPadding = pawgui::padding_default;
        barYPadding = 0;
        outterWidth = 0;
        outterHeight = 0;
        rowSizingStyle = panel_align_default;
        /*barXMargin = 0;
        barYMargin = 0;*/
    }

    widget_content_row::~widget_content_row()
    {
        clear_list();
    }

    void widget_content_row::add_gui_element(widget_basic *  newElement )
    {
        if( newElement == nullptr )
        {
            return;
        }

        if( newElement->indentationLevel > indentationLevel )
        {
            indentationLevel = newElement->indentationLevel;
        }
        sub_options.push_back( newElement );
    }

    void widget_content_row::calculate_width()
    {
        calculatedRowWidth = indentationLevel*padding_default + barXPadding;
        widget_basic * tempOption = nullptr;
        int optionsSize = (int)sub_options.size();
        bool breakHappened = false;
        for( int i = 0; i < optionsSize; i++ )
        {
            tempOption = sub_options[i];
            if( tempOption !=nullptr )
            {
                if( tempOption->requires_newline() || tempOption->is_full_width())
                {
                    calculatedRowWidth = outterWidth - calculatedRowWidth;
                    //gpe::error_log->report("Calculated outer width:"+ stg_ex::int_to_string(outterWidth) );
                    breakHappened = true;
                    break;
                }
                else if( tempOption->autoResizes == false )
                {
                    calculatedRowWidth+=tempOption->get_width()+barXPadding;
                }
            }
        }

        /*if( optionsSize > 2 && !breakHappened )
        {
            calculatedRowWidth-= barXPadding;
        }*/
        //gpe::error_log->report("Calculated width:"+ stg_ex::int_to_string(calculatedRowWidth) );
    }

    void widget_content_row::clear_list()
    {
        sub_options.clear();
    }

    int widget_content_row::get_sub_width()
    {
        calculate_width();
        return calculatedRowWidth;
    }
    //Correct width issue
    void widget_content_row::set_coords(int x_new, int y_new)
    {
        outterWidth = widget_box.w;
        widget_box.x = x_new;
        widget_box.y = y_new;
        widget_box.w = 0;
        widget_box.h = 0;
        //For now we just gonna assume is all gpe::fa_left until its time to fix it
        widget_basic * cContainer = nullptr;

        int indentWidth = indentationLevel*padding_default;
        int itemIndentWidth = indentationLevel*padding_default;
        int tempX = indentWidth + barXPadding;
        int tempY = y_new;//+barYPadding;
        int i;
        int optionsSize = (int)sub_options.size();

        if( optionsSize == 0)
        {
            return;
        }

        int maxHeight = 0;

        //Does horizontal align calculations
        bool keepCalculatingWidth = true;

        int remainderWidth = indentWidth;
        int found_row_width = barXPadding + indentWidth;
        int resizable_elements_total = 0;
        int regular_elements_total = 0;
        int regular_elements_width = 0;
        int resizble_element_width = 0;

        if( rowSizingStyle == panel_align_full )
        {
            widget_box.w = outterWidth  - indentationLevel;
            for( i=0; i<optionsSize; i++)
            {
                cContainer = sub_options[i];
                if(cContainer!=nullptr)
                {
                    if( cContainer->autoResizes )
                    {
                        cContainer->set_width( outterWidth - (barXPadding+indentWidth)*2 );
                        cContainer->set_coords( ( barXPadding*2) +indentWidth, 0 );
                    }
                }
            }
        }
        else
        {
            for( i=0; i<optionsSize; i++)
            {
                cContainer = sub_options[i];
                if(cContainer!=nullptr)
                {
                    if( !cContainer->is_full_width() )
                    {
                        if( cContainer->autoResizes )
                        {
                            cContainer->set_width( 0 ); // we'll resize it later.
                            resizable_elements_total++;
                        }
                        else
                        {
                            regular_elements_total++;
                            found_row_width += cContainer->get_width()+barXPadding - indentWidth;
                        }
                    }
                }
            }

            //if no relements are available to resize it's easy
            if( resizable_elements_total > 0 )
            {
                if( found_row_width >= outterWidth - barXPadding*2 - indentWidth   )
                {
                    resizble_element_width = ( outterWidth - barXPadding*2 - indentWidth   ) / resizable_elements_total;
                }
                else
                {
                    resizble_element_width = ( outterWidth - found_row_width - barXPadding*2 - indentWidth ) / resizable_elements_total;
                }
                resizble_element_width -= ( optionsSize )*barXPadding + indentWidth;

                if( resizble_element_width < 32 + barXPadding  - indentWidth )
                {
                    resizble_element_width = 32 + barXPadding  - indentWidth;
                }
                //We re-loop through elements and set the width of these resizable elements to be even.
                for( i=0; i<optionsSize; i++)
                {
                    cContainer = sub_options[i];
                    if(cContainer!=nullptr)
                    {
                        if( cContainer->autoResizes )
                        {
                            cContainer->set_width(  resizble_element_width ); // we'll resize it later.
                        }
                    }
                }
            }
        }

        //Does vertical align calculations
        //and actually sets coordinates
        int cHeight = 0;

        if( optionsSize == 1)
        {
            cContainer = sub_options[0];
            if( cContainer!=nullptr )
            {
                if( cContainer->is_full_width() )
                {
                    cContainer->set_width( outterWidth - barXPadding*2 - indentWidth  );
                }
                else if( cContainer->autoResizes )
                {
                    cContainer->set_width( outterWidth - barXPadding*2 - indentWidth );
                }

                if( alignment_h == gpe::fa_center)
                {
                    tempX = ( outterWidth - cContainer->get_width() )/2 + indentWidth;
                    cContainer->set_coords( tempX  , y_new );
                }
                else if( alignment_h == gpe::fa_right)
                {
                    tempX =  outterWidth - cContainer->get_width()  + indentWidth;
                    cContainer->set_coords(  tempX, y_new );
                }
                else
                {
                    cContainer->set_coords( indentWidth + barXPadding, y_new );
                }
            }
            widget_box.w = outterWidth - barXPadding*2 - indentWidth ;
            widget_box.h = cContainer->get_height()+barYPadding;
            return;
        }

        widget_box.w = 0;

        //If elements are resizble or left aligned or our row is larger than the screen space we will default to left align.
        if( resizable_elements_total > 0 || alignment_h == gpe::fa_left || found_row_width  >= outterWidth )
        {
            tempX = indentWidth + barXPadding;
        }
        else if( alignment_h == gpe::fa_center )
        {
            tempX = barXPadding + indentWidth + ( outterWidth - found_row_width)/2;
        }
        else
        {
            //right align
            tempX = barXPadding + indentWidth + (outterWidth - found_row_width );
        }

        for( i = 0; i < optionsSize; i++)
        {
            cContainer = sub_options[i];
            if( cContainer!=nullptr )
            {
                cHeight = cContainer->get_height();

                if(cContainer->autoResizes)
                {
                    //cContainer->set_width( widget_box.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }

                if( (optionsSize > 1 ) && lastColumnFloatsRight && (i == optionsSize -1)  )
                {
                    cContainer->set_coords( outterWidth - cContainer->get_width() - barXPadding + indentWidth, tempY );
                }
                else
                {
                    cContainer->set_coords(tempX, tempY );
                }
                tempX += barXPadding+cContainer->get_width();
                widget_box.w+= cContainer->get_width();

                if( cContainer->get_height() > widget_box.h)
                {
                    widget_box.h = cContainer->get_height();
                }
            }
        }
    }

    void widget_content_row::set_full_width( int maxRowWidth )
    {

    }

    void widget_content_row::set_horizontal_align(int height_valueue)
    {
        alignment_h = height_valueue;
    }

    void widget_content_row::set_vertical_align(int vValue)
    {
        alignment_v = vValue;
    }

    void widget_content_row::set_full_width()
    {

    }

    void widget_content_row::set_maxed_out_width()
    {

    }

    void widget_content_row::set_maxed_out_height()
    {

    }

    widget_panel_section::widget_panel_section( std::string section_name, bool auto_indent_elements)
    {
        widget_name = section_name;
        autoIndents = auto_indent_elements;
        widget_type = "panel_section";
        autoResizes = true;
        needsNewLine = true;
        sectionIsOpen = true;
        widget_box.w = 256;
        widget_box.h = 32;
        indentationLevel = 0;
    }

    widget_panel_section::~widget_panel_section()
    {
        sub_options.clear();
    }

    bool widget_panel_section::add_widget( widget_basic * widget_element, bool requires_nl  )
    {
        if( widget_element == nullptr )
        {
            return false;
        }

        panel_section_item new_secton;
        if( autoIndents )
        {
            widget_element->indentationLevel = indentationLevel +1;
        }
        new_secton.widget_element = widget_element;

        if( widget_element->hasLineBreak )
        {
            new_secton.requiresNewLine = true;
        }
        else
        {
            new_secton.requiresNewLine = requires_nl;
        }
        sub_options.push_back( new_secton );
        return true;
    }

    bool widget_panel_section::auto_indents()
    {
        return autoIndents;
    }

    bool widget_panel_section::is_section_open()
    {
        return sectionIsOpen;
    }

    void widget_panel_section::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        widget_basic::process_self( view_space, cam );
        if( is_clicked() )
        {
            sectionIsOpen = !sectionIsOpen;
        }
    }

    void widget_panel_section::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);

        if( view_space==nullptr || cam==nullptr  )
        {
            return;
        }

        gpe::gcanvas->render_rectangle( widget_box.x - cam->x, widget_box.y - cam->y, widget_box.x + widget_box.w- cam->x, widget_box.y + widget_box.h - cam->y, pawgui::theme_main->text_box_color, false, 255 );


        if( (int)widget_name.size() > 0 )
        {

            if( isInUse)
            {
                gpe::gfs->render_text_clipped( widget_box.x-cam->x + padding_default,widget_box.y+widget_box.h/2-cam->y,widget_name,pawgui::theme_main->text_box_highlight_color,FONT_LABEL_TITLE,gpe::fa_left,gpe::fa_middle, widget_box.w - 32, widget_box.h, 255);
            }
            else
            {
                gpe::gfs->render_text_clipped( widget_box.x-cam->x+ padding_default,widget_box.y+widget_box.h/2-cam->y,widget_name,pawgui::theme_main->main_box_font_color,FONT_LABEL_TITLE,gpe::fa_left,gpe::fa_middle, widget_box.w - 32, widget_box.h, 255);
            }
        }



        if(  sectionIsOpen )
        {
            gpe::gfs->render_text_clipped( widget_box.x + widget_box.w - 16 -cam->x ,widget_box.y+widget_box.h/2-cam->y,"▲",pawgui::theme_main->text_box_highlight_color,FONT_LABEL_TITLE,gpe::fa_center,gpe::fa_middle, widget_box.w - 32, widget_box.h, 255);
        }
        else
        {
            gpe::gfs->render_text_clipped( widget_box.x + widget_box.w - 16 -cam->x,widget_box.y+widget_box.h/2-cam->y,"▼",pawgui::theme_main->main_box_font_color,FONT_LABEL_TITLE,gpe::fa_center,gpe::fa_middle, widget_box.w - 32, widget_box.h, 255);
        }

        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x - cam->x, widget_box.y - cam->y, widget_box.x + widget_box.w- cam->x, widget_box.y + widget_box.h - cam->y, pawgui::theme_main->text_box_highlight_color, true, 255 );
        }
        else
        {
            gpe::gcanvas->render_rectangle( widget_box.x - cam->x, widget_box.y - cam->y, widget_box.x + widget_box.w- cam->x, widget_box.y + widget_box.h - cam->y, pawgui::theme_main->text_box_outline_color, true, 255 );
        }


    }

    widget_panel_list::widget_panel_list()
    {
        lastColumnFloatsRight = false;
        panelAlignType = panel_align_default;
        usingFullSizeElement = false;
        selectedElement = nullptr;
        needsNewLine = true;
        inDebugMode = false;
        selectedId = -1;
        newRowRequested = false;
        hideXScroll = true;
        hideYScroll = true;
        cameraBox.x = menuBox.x = entireBox.x = widget_box.x = 0;
        cameraBox.y = menuBox.y = entireBox.y = widget_box.y = 0;
        cameraBox.w = menuBox.w = entireBox.w = widget_box.w;
        cameraBox.h = menuBox.h = entireBox.h = widget_box.h = 32;
        xScroll = new widget_scrollbar_xaxis();
        yScroll = new widget_scrollbar_yaxis();
        cameraBox.w = menuBox.w = entireBox.w = widget_box.w - yScroll->get_box_width();
        barXPadding = padding_default;
        barYPadding = padding_default;
        barXMargin = 0;
        barYMargin = 0;
        isInUse = true;
        subElementsHasArrowControl = false;
        subElementsIsScrolling = false;
        alignment_h = gpe::fa_left;
        alignment_v = gpe::fa_top;
        widget_type = "guilist";
        tabDelay = 0;
        leserKeyDelay = 0;
        greaterKeyDelay = 0;
        upDelay = 0;
        downDelay = 0;
        leftDelay = 0;
        rightDelay = 0;
    }

    widget_panel_list::~widget_panel_list()
    {
        if( xScroll!=nullptr)
        {
            delete xScroll;
            xScroll = nullptr;
        }
        if( yScroll!=nullptr)
        {
            delete yScroll;
            yScroll = nullptr;
        }
        clear_list();
    }

    /*
    void widget_panel_list::add_gui_element(widget_basic *  newElement, bool isNLElement)
    {
        if( newElement!=nullptr )
        {
            widget_content_row *  current_color_row = nullptr;
            if( newElement->requires_newline() || newElement->is_full_width() )
            {
                newRowRequested = true;
            }

            if( panelAlignType == panel_align_full )
            {
                isNLElement = true;
            }
            newElement->hasLineBreak = isNLElement;


            int rowSize = (int)subRows.size();

            if(  rowSize == 0 || newRowRequested )
            {
                current_color_row = new widget_content_row();
                current_color_row->alignment_h = alignment_h;
                current_color_row->alignment_v = alignment_v;
                current_color_row->barXPadding = barXPadding;
                current_color_row->barYPadding = barYPadding;
                subRows.push_back( current_color_row );
            }
            else
            {
                current_color_row = subRows[ rowSize -1 ];
            }

            if( current_color_row !=nullptr )
            {
                current_color_row->add_gui_element( newElement );
            }

            if( newElement->hasLineBreak || newElement->requires_newline() )
            {
                isNLElement = true;
            }
            newRowRequested = isNLElement;

            //Adds content to giant list of objects within list.
            //Then gives it a unique id
            newElement->dynamic_id = (int)allElements.size();
            allElements.push_back( newElement );
        }
    }

    void widget_panel_list::add_gui_auto(widget_basic *  newElement )
    {
        if( newElement!=nullptr)
        {
            widget_content_row *  current_color_row = nullptr;
            newElement->hasLineBreak = false;
            int rowSize = (int)subRows.size();

            if( newElement->requires_newline() || newElement->is_full_width() || panelAlignType == panel_align_full  )
            {
                newRowRequested = true;
                newElement->hasLineBreak = true;
            }

            if(  rowSize == 0 || newRowRequested )
            {
                current_color_row = new widget_content_row();
                current_color_row->alignment_h = alignment_h;
                current_color_row->alignment_v = alignment_v;
                current_color_row->barXPadding = barXPadding;
                current_color_row->barYPadding = barYPadding;
                subRows.push_back( current_color_row );
                current_color_row->add_gui_element( newElement );
                newRowRequested = false;
            }
            else
            {
                //Finds last row
                current_color_row = subRows[ rowSize -1 ];
                if( current_color_row !=nullptr )
                {
                    current_color_row->add_gui_element( newElement );
                    //gpe::error_log->report("Able to squeeze in...");
                    newRowRequested = false;
                }
            }

            //Adds content to giant list of objects within list.
            //Then gives it a unique id
            newElement->dynamic_id = (int)allElements.size();
            allElements.push_back( newElement );

        }
    }
    */

    void widget_panel_list::add_gui_element(widget_basic *  newElement, bool isNLElement)
    {
        if( newElement!=nullptr )
        {
            widget_content_row *  currentRow = nullptr;
            if( newElement->requires_newline() || newElement->is_full_width() )
            {
                newRowRequested = true;
            }

            if( panelAlignType == panel_align_full )
            {
                isNLElement = true;
            }
            newElement->hasLineBreak = isNLElement;


            int rowSize = (int)subRows.size();

            if(  rowSize == 0 || newRowRequested )
            {
                currentRow = new widget_content_row();
                currentRow->indentationLevel = newElement->indentationLevel;
                currentRow->alignment_h = alignment_h;
                currentRow->alignment_v = alignment_v;
                currentRow->barXPadding = barXPadding;
                currentRow->barYPadding = barYPadding;
                subRows.push_back( currentRow );
            }
            else
            {
                currentRow = subRows[ rowSize -1 ];
            }

            if( currentRow !=nullptr )
            {
                currentRow->add_gui_element( newElement );
            }

            if( newElement->hasLineBreak || newElement->requires_newline() )
            {
                isNLElement = true;
            }
            newRowRequested = isNLElement;

            //Adds content to giant list of objects within list.
            //Then gives it a unique id
            newElement->dynamic_id = (int)allElements.size();
            allElements.push_back( newElement );
        }
    }

    void widget_panel_list::add_gui_auto(widget_basic *  newElement )
    {
        if( newElement!=nullptr)
        {
            widget_content_row *  currentRow = nullptr;
            newElement->hasLineBreak = false;
            int rowSize = (int)subRows.size();

            if( newElement->requires_newline() || newElement->is_full_width() || panelAlignType == panel_align_full  )
            {
                newRowRequested = true;
                newElement->hasLineBreak = true;
            }

            if(  rowSize == 0 || newRowRequested )
            {
                currentRow = new widget_content_row();
                currentRow->alignment_h = alignment_h;
                currentRow->alignment_v = alignment_v;
                currentRow->barXPadding = barXPadding;
                currentRow->barYPadding = barYPadding;
                subRows.push_back( currentRow );
                currentRow->add_gui_element( newElement );
                newRowRequested = false;
            }
            else
            {
                //Finds last row
                currentRow = subRows[ rowSize -1 ];
                if( currentRow !=nullptr )
                {
                    //Checks if a new element can fit inside of same row
                    if( currentRow->get_sub_width()+ newElement->get_width() > widget_box.w-32 )
                    {
                        //Add a new row and add element to it.
                        currentRow = new widget_content_row();
                        currentRow->alignment_h = alignment_h;
                        currentRow->alignment_v = alignment_v;
                        currentRow->barXPadding = barXPadding;
                        currentRow->barYPadding = barYPadding;
                        subRows.push_back( currentRow );
                        currentRow->add_gui_element( newElement );
                        //gpe::error_log->report("Not able to squeeze in...");
                        newRowRequested = false;
                    }
                    else
                    {
                        //Able to squeeze this element into view
                        currentRow->add_gui_element( newElement );
                        //gpe::error_log->report("Able to squeeze in...");
                        newRowRequested = false;
                    }
                }
                else
                {
                    //gpe::error_log->report("Current row = nullptr...");
                }
            }

            //Adds content to giant list of objects within list.
            //Then gives it a unique id
            newElement->dynamic_id = (int)allElements.size();
            allElements.push_back( newElement );

        }
    }

    void widget_panel_list::add_gui_element_fullsize( widget_basic * newElement )
    {
        if( newElement !=nullptr )
        {
            clear_list();
            newElement->set_coords( widget_box.y, widget_box.y );
            newElement->set_width( widget_box.w );
            newElement->set_height( widget_box.h );
            newElement->set_full_width( true );
            barXPadding = 0;
            barYPadding = 0;
            barXMargin = 0;
            barYMargin = 0;
            add_gui_auto( newElement );
            usingFullSizeElement = true;
        }
    }

    void widget_panel_list::add_gui_section(widget_panel_section *  newElement )
    {
        if( newElement == nullptr)
        {
            return;
        }
        add_gui_element( newElement, true );

        int maxEntries = newElement->sub_options.size();
        int nextIndexLevel = newElement->indentationLevel + 1;

        if( newElement->is_section_open() && maxEntries !=0 )
        {
            panel_section_item temp_section;
            for( int i = 0; i < maxEntries; i++ )
            {
                temp_section = newElement->sub_options[i];

                if( temp_section.widget_element != nullptr )
                {
                    temp_section.widget_element->indentationLevel = nextIndexLevel;
                    if( temp_section.requiresNewLine )
                    {

                        add_gui_element( temp_section.widget_element, true );
                    }
                    else
                    {
                        add_indented_element( nextIndexLevel, temp_section.widget_element );
                    }
                }
            }
        }
    }

    void widget_panel_list::add_indented_element( int level, widget_basic * newElement)
    {
        if( newElement!=nullptr )
        {
            widget_content_row * newRow = new widget_content_row();
            newRow->indentationLevel = level;
            newElement->indentationLevel = level;
            newRow->alignment_h = alignment_h;
            newRow->alignment_v = alignment_v;
            newRow->barXPadding = barXPadding;
            newRow->barYPadding = barYPadding;
            newRow->add_gui_element( newElement );
            subRows.push_back( newRow );
            //Adds content to giant list of objects within list.
            //Then gives it a unique id
            newElement->dynamic_id = (int)allElements.size();
            allElements.push_back( newElement );
        }
    }

    void widget_panel_list::clear_list()
    {
        usingFullSizeElement = false;
        selectedElement = nullptr;
        allElements.clear();
        cameraBox.w = menuBox.w = entireBox.w = widget_box.w;
        cameraBox.h = menuBox.h = entireBox.h = widget_box.h;

        widget_content_row * tempRow = nullptr;
        for( int i = (int)subRows.size()-1; i>=0; i--)
        {
            tempRow = subRows[i];
            if( tempRow!=nullptr )
            {
                delete tempRow;
                tempRow = nullptr;
            }
        }
        subRows.clear();
        newRowRequested = true;
        subElementsHasArrowControl = false;
        subElementsIsScrolling = false;
        newRowRequested = false;
        selectedId = -1;
    }


    void widget_panel_list::scroll_down(int yToMove )
    {
        if( yToMove > 0 && hideYScroll!=true )
        {
            cameraBox.y+=yToMove;
        }
        if( cameraBox.y+cameraBox.h > entireBox.h )
        {
            cameraBox.y = entireBox.h-cameraBox.h;
        }
        if( cameraBox.y < 0)
        {
            cameraBox.y = 0;
        }
    }

    void widget_panel_list::scroll_up(int yToMove )
    {
        if( yToMove > 0 && hideYScroll!=true )
        {
            cameraBox.y-=yToMove;
        }

        if( cameraBox.y+cameraBox.h > entireBox.h )
        {
            cameraBox.y = entireBox.h-cameraBox.h;
        }
        if( cameraBox.y < 0)
        {
            cameraBox.y = 0;
        }
    }

    void widget_panel_list::scroll_left(int xToMove )
    {
        if( xToMove > 0  && hideXScroll!=true )
        {
            cameraBox.x-=xToMove;
        }

        if( cameraBox.x+cameraBox.w > entireBox.w )
        {
            cameraBox.x = entireBox.w-cameraBox.w;
        }
        if( cameraBox.x < 0)
        {
            cameraBox.x = 0;
        }
    }

    void widget_panel_list::scroll_right(int xToMove )
    {
        if( xToMove > 0 && hideXScroll!=true )
        {
            cameraBox.x+=xToMove;
        }
        else
        {
            cameraBox.x = 0;
        }

        if( cameraBox.x+cameraBox.w > entireBox.w )
        {
            cameraBox.x = entireBox.w-cameraBox.w;
        }
        if( cameraBox.x < 0)
        {
            cameraBox.x = 0;
        }
    }

    void widget_panel_list::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        selectedElement = nullptr;
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( inDebugMode)
        {
            gpe::error_log->report("Starting to process widget_panel_list...");
        }
        widget_basic::process_self(view_space,cam);
        xScroll->set_coords( widget_box.x, widget_box.h - xScroll->get_height() );
        xScroll->set_width( widget_box.w );

        yScroll->set_coords( widget_box.w - yScroll->get_width(), widget_box.y );
        yScroll->set_height( widget_box.h );

        //Handles row / option in view
        widget_content_row * cRow = nullptr;
        widget_basic * cContainer = nullptr;
        widget_basic * containerInControl = nullptr;
        int lastselectedId = -1;
        int i = 0, j = 0;
        int pastRowChange = 0;
        int current_row_count = (int)subRows.size();
        int rowOptionsCount = 0;
        bool directionChangeRequested = false;

        if( inDebugMode)
        {
            gpe::error_log->report("["+ stg_ex::int_to_string(current_row_count)+"] unique rows gathered...");
        }
        if( isClicked )
        {
            //if( resource_dragged==nullptr)
            {
                isInUse = true;
                hasScrollControl = true;
                hasArrowkeyControl = true;
            }
        }
        else if( clickedOutside )
        {
            isInUse = false;
            hasScrollControl = false;
            hasArrowkeyControl = false;
        }

        if( !isInUse )
        {
            hasScrollControl = false;
            hasArrowkeyControl = false;
        }
        entireBox.x = 0;
        entireBox.y = 0;
        entireBox.w = (barXMargin)*2;
        entireBox.h = (barYMargin)*2;

        menuBox.x = widget_box.x+view_space->x;
        menuBox.y = widget_box.y+view_space->y;
        menuBox.w = widget_box.w;
        menuBox.h = widget_box.h;

        if( hideXScroll )
        {
            cameraBox.w = menuBox.w = widget_box.w;
        }
        else
        {
            cameraBox.w = menuBox.w = widget_box.w-16;
        }

        if( hideYScroll)
        {
            cameraBox.h = menuBox.h = widget_box.h;
        }
        else
        {
            cameraBox.h = menuBox.h = widget_box.h-16;
        }
        subElementsHasArrowControl = false;
        subElementsIsScrolling = false;

        bool scrolingHasntOccurred = false;
        if( hideXScroll!=true && hideYScroll!=true )
        {
            scrolingHasntOccurred = true;
        }
        else if ( xScroll->is_scrolling()==false && yScroll->is_scrolling()==false  )
        {
            scrolingHasntOccurred = true;
        }


        //Handles keyboard/gamepad input as well as delays
        if( isInUse)
        {

            if( gpe::input->kb_button_down[kb_ctrl] && gpe::input->kb_button_down[kb_comma] && gpe::input->kb_button_pressed[kb_comma]==false && gpe::input->kb_button_released[kb_comma]==false )
            {
                leserKeyDelay+= gpe::time_keeper->get_delta_performance();
            }
            else
            {
                leserKeyDelay = 0;
            }

            if( gpe::input->kb_button_down[kb_ctrl] && gpe::input->kb_button_down[kb_period] && gpe::input->kb_button_pressed[kb_period]==false && gpe::input->kb_button_released[kb_period]==false )
            {
                greaterKeyDelay+= gpe::time_keeper->get_delta_performance();
            }
            else
            {
                greaterKeyDelay = 0;
            }
        }
        if( isInUse && hasArrowkeyControl )
        {
            if( gpe::input->kb_button_down[kb_tab] && gpe::input->kb_button_pressed[kb_tab]==false && gpe::input->kb_button_released[kb_tab]==false )
            {
                tabDelay += gpe::time_keeper->get_delta_performance();
            }
            else
            {
                tabDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_left] && gpe::input->kb_button_pressed[kb_left]==false && gpe::input->kb_button_released[kb_left]==false )
            {
                leftDelay+= gpe::time_keeper->get_delta_performance();
            }
            else
            {
                leftDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_right] && gpe::input->kb_button_pressed[kb_right]==false && gpe::input->kb_button_released[kb_right]==false )
            {
                rightDelay+= gpe::time_keeper->get_delta_performance();
            }
            else
            {
                rightDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_up] && gpe::input->kb_button_pressed[kb_up]==false && gpe::input->kb_button_released[kb_up]==false )
            {
                upDelay+= gpe::time_keeper->get_delta_performance();
            }
            else
            {
                upDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_down] && gpe::input->kb_button_pressed[kb_down]==false && gpe::input->kb_button_released[kb_down]==false )
            {
                downDelay+= gpe::time_keeper->get_delta_performance();
            }
            else
            {
                downDelay = 0;
            }
        }
        else
        {
            tabDelay = 0;
            upDelay = 0;
            downDelay = 0;
            leftDelay = 0;
            rightDelay = 0;
        }

        if( inDebugMode)
        {
            gpe::error_log->report("[Calculating box size...");
        }

        //Sets up the coordinates for the rows and their content
        if( usingFullSizeElement && (int)allElements.size() == 1 )
        {
            cContainer = allElements[0];
            cContainer->set_coords(widget_box.x, widget_box.y);
            cContainer->set_width( widget_box.w);
            cContainer->set_height( widget_box.h);
            cContainer->process_self( view_space, cam );
            return;
        }
        int x_pos = barXMargin+barXPadding;
        int y_pos = barYMargin+barYPadding;
        int y2Pos = y_pos;
        int rowWidth = 0;
        int maxRowWidth = 0;
        int totalMaxRowWidth = 0;
        int rowHeight = 0;

        x_pos = barXMargin+barXPadding;

        for( i=0; i < current_row_count; i++)
        {
            cRow = subRows[i];
            if( cRow!=nullptr )
            {
                cRow->lastColumnFloatsRight = lastColumnFloatsRight;
                cRow->rowSizingStyle = panelAlignType;
                cRow->barXPadding = barXPadding;
                cRow->barYPadding = barYPadding;

                cRow->outterWidth  = menuBox.w - barXMargin - yScroll->get_width();

                cRow->outterHeight = menuBox.h - barYMargin - xScroll->get_height();
                cRow->set_width( menuBox.w  - cRow->indentationLevel*padding_default );
                cRow->set_height( menuBox.h );
                cRow->set_coords(x_pos, y2Pos );

                rowHeight = cRow->get_height();
                y2Pos+=rowHeight+barYPadding;
                rowWidth = cRow->get_width();
                if( maxRowWidth < rowWidth)
                {
                    maxRowWidth = rowWidth;
                }
            }
        }

        entireBox.w = maxRowWidth + barXMargin;
        entireBox.h = y2Pos + barYMargin;

        if( entireBox.w > menuBox.w )
        {
            hideXScroll = false;
        }
        else
        {
            hideXScroll = true;
        }

        if( entireBox.h > menuBox.h )
        {
            hideYScroll = false;
        }
        else
        {
            hideYScroll = true;
        }
        if( inDebugMode)
        {
            gpe::error_log->report("["+ stg_ex::int_to_string(entireBox.w)+"] X ["+ stg_ex::int_to_string(entireBox.h)+"] size...");
        }
        //Processes each container if they are in focus or not...
        if( scrolingHasntOccurred )
        {
            if( inDebugMode)
            {
                gpe::error_log->report("Checking rolls for scrolling..." );
            }

            for( i =(int)allElements.size()-1; i >=0 ; i-- )
            {
                cContainer = allElements[i];
                if( cContainer!=nullptr )
                {
                    cContainer->dynamic_id = i;
                    if( cContainer->hasScrollControl )
                    {
                        subElementsIsScrolling = true;
                        containerInControl = cContainer;
                        lastselectedId = selectedId = cContainer->dynamic_id;
                        break;
                    }
                    if( cContainer->hasArrowkeyControl )
                    {
                        subElementsHasArrowControl = true;
                        containerInControl = cContainer;
                        lastselectedId = selectedId = cContainer->dynamic_id;
                        break;
                    }
                    if( cContainer->is_inuse() )
                    {
                        containerInControl = cContainer;
                        lastselectedId = selectedId = cContainer->dynamic_id;
                        break;
                    }
                }
            }

            if( inDebugMode)
            {
                gpe::error_log->report("Done..." );
            }

            //locks on one gui item until it is taken out of focus...
            lastselectedId = selectedId;
            if( containerInControl!=nullptr )
            {
                if( inDebugMode)
                {
                    gpe::error_log->report("Processing selected container..." );
                    gpe::error_log->report("Trying to process "+containerInControl->get_element_type() +"..." );
                }
                containerInControl->process_self(&menuBox,&cameraBox);
                /*!containerInControl->hasScrollControl==false  !containerInControl->hasArrowkeyControl  */
                if(  !containerInControl->is_inuse() )
                {
                    containerInControl = nullptr;
                    selectedId = -1;
                }
                if( inDebugMode)
                {
                    gpe::error_log->report("Done..." );
                }
            }

            //else
            if( inDebugMode)
            {
                gpe::error_log->report("Processing other containers..." );
            }

            for( i =(int)allElements.size()-1; i >=0 ; i-- )
            {
                cContainer = allElements[i];
                if( cContainer!=nullptr )
                {
                    cContainer->dynamic_id = i;
                    if( lastselectedId != cContainer->dynamic_id )
                    {
                        //gpe::error_log->report("Trying to process "+cContainer->get_element_type() +"..." );
                        if( inDebugMode)
                        {
                            gpe::error_log->report("Trying to process "+cContainer->get_element_type() +"..." );
                        }
                        cContainer->alignment_h = alignment_h;
                        cContainer->process_self(&menuBox,&cameraBox);

                        if( cContainer->hasScrollControl )
                        {
                            subElementsIsScrolling = true;
                            containerInControl = cContainer;
                            selectedId = cContainer->dynamic_id;
                            directionChangeRequested = true;
                        }

                        if( cContainer->hasArrowkeyControl )
                        {
                            subElementsHasArrowControl=true;
                            containerInControl = cContainer;
                            selectedId = cContainer->dynamic_id;
                            directionChangeRequested = true;
                        }

                        if( cContainer->is_inuse() )
                        {
                            selectedId = cContainer->dynamic_id;
                            containerInControl = cContainer;
                            directionChangeRequested = true;
                        }
                    }
                }
            }

            if( containerInControl!=nullptr )
            {
                if( inDebugMode)
                {
                    gpe::error_log->report("Processing selected container..." );
                    gpe::error_log->report("Trying to process "+cContainer->get_element_type() +"..." );
                }

                if( containerInControl!=nullptr && containerInControl->is_inuse() == false )
                {
                    containerInControl = nullptr;
                    //selectedId++;
                    /*
                    if( inDebugMode)
                    gpe::error_log->report("Container "+cContainer->get_element_type() +" lost control..." );
                    */
                }
                if( inDebugMode)
                {
                    gpe::error_log->report("Done..." );
                }
            }

            if( inDebugMode)
            {
                gpe::error_log->report("Done..." );
            }
        }

        //if(  isHovered || (isInUse && subElementsIsScrolling==false && hasScrollControl ) )
        if( isHovered && subElementsIsScrolling==false )
        {
            if( gpe::input->mouse_scrolling_up )
            {
                if( gpe::input->kb_shift_pressed)
                {
                    scroll_left( cameraBox.w/4 );
                }
                else
                {
                    scroll_up( cameraBox.h/4 );
                }
            }
            else if( gpe::input->mouse_scrolling_down )
            {
                if( gpe::input->kb_shift_pressed )
                {
                    scroll_right( cameraBox.w/4 );
                }
                else
                {
                    scroll_down( cameraBox.h/4 );
                }
            }
        }


        //The secondary way to scroll for when in other contexts that have control over keyboard
        if( leserKeyDelay > main_settings->normalInputDelayTime )
        {
            selectedId--;
            directionChangeRequested = true;
            leserKeyDelay = -1;
        }
        else if( greaterKeyDelay > main_settings->normalInputDelayTime )
        {
            selectedId++;
            directionChangeRequested = true;
            greaterKeyDelay = -1;
        }
        else if( hasScrollControl && subElementsIsScrolling==false)
        {
            if( tabDelay > main_settings->normalInputDelayTime  && gpe::input->check_kb_down(kb_ctrl)==false )
            {
                if( gpe::input->kb_shift_pressed)
                {
                    selectedId--;
                    directionChangeRequested = true;
                }
                else
                {
                    selectedId++;
                    directionChangeRequested = true;
                }
                tabDelay = 0;
            }

            if( subElementsHasArrowControl==false)
            {
                if( upDelay > main_settings->normalInputDelayTime )
                {
                    scroll_up( cameraBox.h/4 );
                    upDelay = 0;
                }
                else if( downDelay > main_settings->normalInputDelayTime )
                {
                    scroll_down( cameraBox.h/4 );
                    downDelay = 0;
                }
            }
        }

        if( directionChangeRequested )
        {
            if( selectedId < 0 )
            {
                selectedId = (int)allElements.size()-1;
                //directionChangeRequested = true;
            }

            if( selectedId >= (int)allElements.size() )
            {
                selectedId = 0;
                //directionChangeRequested = true;
            }
        }

        for( int iTemp = 0; iTemp < (int)allElements.size(); iTemp++ )
        {
            cContainer = allElements.at(iTemp);
            if(cContainer!=nullptr)
            {
                if( selectedId  == cContainer->dynamic_id )
                {
                    cContainer->switch_inuse( true );
                    if( directionChangeRequested )
                    {
                        ///cameraBox.x = cContainer->get_xpos()-(barXPadding+barXMargin-totalMaxRowWidth);
                        if( cameraBox.y > cContainer->get_ypos() || cContainer->get_ypos() > cameraBox.y+cameraBox.h )
                        {
                            cameraBox.y = cContainer->get_ypos()-(barYPadding+barYMargin);
                        }
                    }
                }
                else
                {
                    cContainer->switch_inuse( false );
                }
            }
        }

        xScroll->set_coords( widget_box.x, widget_box.y+widget_box.h-xScroll->get_height() );
        xScroll->set_width( widget_box.w - yScroll->get_width() );

        xScroll->fullRect.x = 0;
        xScroll->fullRect.y = 0;
        xScroll->fullRect.w = entireBox.w;
        xScroll->fullRect.h = entireBox.h;

        xScroll->contextRect.x = cameraBox.x;
        xScroll->contextRect.y = cameraBox.y;
        xScroll->contextRect.w = cameraBox.w;
        xScroll->contextRect.h = cameraBox.h;

        yScroll->set_coords( widget_box.x+widget_box.w-yScroll->get_width(), widget_box.y );
        yScroll->set_height( widget_box.h - xScroll->get_height() );

        gpe::update_rectangle(&yScroll->fullRect, 0, 0, entireBox.w,entireBox.h);
        gpe::update_rectangle(&yScroll->contextRect, cameraBox.x, cameraBox.y, cameraBox.w,cameraBox.h);

        if( hasScrollControl && subElementsHasArrowControl==false &&  xScroll!=nullptr && yScroll!=nullptr)
        {
            if( entireBox.w >=widget_box.w  && hideXScroll!=true )
            {
                xScroll->process_self(view_space,cam);
            }
            else
            {
                cameraBox.x =0;
                cameraBox.w = menuBox.w = widget_box.w;
            }

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
            }

            if( hideYScroll!=true )
            {
                yScroll->autoCorrect = false;
                yScroll->process_self(view_space,cam );
            }
            else
            {
                cameraBox.y = 0;
                cameraBox.w = menuBox.w = widget_box.w;
                yScroll->contextRect.y = 0;
                yScroll->contextRect.h = entireBox.h;
            }
            if( entireBox.h <= cameraBox.h)
            {
                yScroll->contextRect.y = 0;
                yScroll->contextRect.h = entireBox.h;
                cameraBox.y = 0;
            }
            if( isInUse )
            {

            }

            if( yScroll->has_moved() )
            {
                cameraBox.y = yScroll->contextRect.y;
                /*
                if( cameraBox.y +cameraBox.h > entireBox.h)
                {
                    cameraBox.y = entireBox.h - cameraBox.h;
                }
                if( cameraBox.y < 0)
                {
                    cameraBox.y = 0;
                }*/
            }
        }
        else
        {
            xScroll->process_self(view_space, cam);
            yScroll->process_self(view_space, cam);
        }
        selectedElement = containerInControl;
    }

    void widget_panel_list::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( view_space!=nullptr && cam!=nullptr )
        {
            /*
            menuBox.x = view_space->x+ widget_box.x;
            menuBox.y = view_space->x+ widget_box.y;
            menuBox.w = widget_box.w;
            menuBox.h = widget_box.h;
            */
            gpe::renderer_main->reset_viewpoint();
            gpe::renderer_main->set_viewpoint( view_space );

            widget_basic * cResource = nullptr;

            if( usingFullSizeElement && (int)allElements.size() == 1 )
            {
                cResource = allElements[0];
                if( cResource!=nullptr )
                {
                    cResource->set_coords( 0,0 );
                    cResource->set_width( widget_box.w);
                    cResource->set_height( widget_box.h);
                    cResource->render_self( view_space, cam );
                    return;
                }
            }

            gpe::renderer_main->set_viewpoint( &menuBox);
            widget_content_row * cRow = nullptr;
            int i = 0,j = 0;
            for( i=0; i<(int)subRows.size(); i++)
            {
                cRow = subRows[i];
                if(cRow!=nullptr)
                {
                    for( j =0; j < (int)cRow->sub_options.size(); j++ )
                    {
                        cResource = cRow->sub_options[j];
                        if(cResource!=nullptr)
                        {
                            cResource->render_self( &menuBox,&cameraBox);
                        }
                    }
                }
            }

            gpe::renderer_main->reset_viewpoint();
            gpe::renderer_main->set_viewpoint( view_space);
            /*menuBox.w-=16;
            menuBox.h-=16;*/
            if( xScroll!=nullptr&& hideXScroll!=true )
            {
                //if( entireBox.w >widget_box.w)
                {
                    xScroll->render_self( view_space,cam);
                }
            }
            if( yScroll!=nullptr&& hideYScroll!=true )
            {
                if( entireBox.h >widget_box.h )
                {
                    yScroll->render_self( view_space,cam);
                }
            }

            gpe::renderer_main->reset_viewpoint();
            gpe::renderer_main->set_viewpoint( view_space);
        }
        clear_list();
    }

    void widget_panel_list::reset_self()
    {
        clear_list();
        cameraBox.x = menuBox.x = entireBox.x = 0;
        cameraBox.y = menuBox.y = entireBox.y = 0;
        cameraBox.w = menuBox.w = entireBox.w = 32;
        cameraBox.h = menuBox.h = entireBox.h = 32;
    }

    void widget_panel_list::set_horizontal_align(int height_valueue)
    {
        alignment_h = height_valueue;
        widget_content_row * tguiRow = nullptr;
        for( int i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=nullptr )
            {
                tguiRow->alignment_h = height_valueue;
            }
        }
        widget_basic * cElement = nullptr;
        for( int i = (int)allElements.size()-1; i>=0; i--)
        {
            cElement = allElements[i];
            if( cElement!=nullptr )
            {
                cElement->alignment_h = height_valueue;
            }
        }
    }

    void widget_panel_list::set_full_width()
    {
        widget_content_row * tguiRow = nullptr;
        int maxWidth = widget_box.w-(barXMargin+barXPadding)*3;
        for( int i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=nullptr )
            {
                tguiRow->set_width(maxWidth);
            }
        }
    }

    void widget_panel_list::set_maxed_out_width()
    {
        widget_content_row * tguiRow = nullptr;
        int i = 0;
        int maxWidth = 0;
        for( i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=nullptr )
            {
                if( tguiRow->get_width() > maxWidth)
                {
                    maxWidth = tguiRow->get_width();
                }
            }
        }

        for( i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=nullptr )
            {
                tguiRow->set_width(maxWidth);
            }
        }
    }

    void widget_panel_list::set_maxed_out_height()
    {
        widget_content_row * tguiRow = nullptr;
        int i = 0;
        int maxHeight = 0;
        for( i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=nullptr )
            {
                if( tguiRow->get_height() > maxHeight)
                {
                    maxHeight = tguiRow->get_height();
                }
            }
        }

        for( i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=nullptr )
            {
                tguiRow->set_height(maxHeight);
            }
        }
    }
}
