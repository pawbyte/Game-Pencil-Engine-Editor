/*
pawgui_panel_list.cpp
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

#include "pawgui_panel_list.h"

namespace pawgui
{
    widget_content_row::widget_content_row()
    {
        calculatedRowWidth = 0;
        calculatedRowHeight = 0;
        inDebugMode = false;
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 0;
        widget_box.h = 0;
        hAlign = gpe::fa_left;
        vAlign = gpe::fa_top;
        barXPadding = 0;
        barYPadding = 0;
        outterWidth = 0;
        outterHeight = 0;
        /*barXMargin = 0;
        barYMargin = 0;*/
    }

    widget_content_row::~widget_content_row()
    {
        clear_list();
    }

    void widget_content_row::add_gui_element(widget_basic *  newElement )
    {
        subOptions.push_back( newElement );
    }

    void widget_content_row::calculate_width()
    {
        calculatedRowWidth = (barXPadding)*2 + indentationLevel*padding_default;
        widget_basic * tempOption = NULL;
        int optionsSize = (int)subOptions.size();
        bool breakHappened = false;
        for( int i = 0; i < optionsSize; i++ )
        {
            tempOption = subOptions[i];
            if( tempOption !=NULL )
            {
                if( tempOption->requires_newline() || tempOption->is_full_width())
                {
                    calculatedRowWidth = outterWidth;
                    //gpe::error_log->report("Calculated outer width:"+ stg_ex::int_to_string(outterWidth) );
                    breakHappened = true;
                    break;
                }
                else
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
        /*
        widget_basic * tempItem = NULL;
        for( int i = 0; (int)subOptions.size(); i++)
        {
            tempItem = subOptions[i];
            if( tempItem!=NULL )
            {
                delete tempItem;
                tempItem = NULL;
            }
        }*/
        subOptions.clear();
    }

    int widget_content_row::get_sub_width()
    {
        calculate_width();
        return calculatedRowWidth;
    }
    //Correct width issue
    void widget_content_row::set_coords(int newX, int newY)
    {
        widget_box.x = newX;
        widget_box.y = newY;
        widget_box.w = 0;
        widget_box.h = 0;
        //For now we just gonna assume is all gpe::fa_left until its time to fix it
        widget_basic * cContainer = NULL;
        int indentWidth = indentationLevel*barXPadding;
        int tempX = newX + indentWidth;
        int tempY = newY;//+barYPadding;
        int i;
        int optionsSize = (int)subOptions.size();

        if( optionsSize == 0)
        {
            return;
        }

        int maxHeight = 0;

        //Does horizontal align calculations
        bool keepCalculatingWidth = true;

        int remainderWidth = indentWidth;
        if( rowSizingStyle == panel_align_full )
        {
            widget_box.w = outterWidth;
            for( i=0; i<optionsSize; i++)
            {
                cContainer = subOptions[i];
                if(cContainer!=NULL)
                {
                    if( cContainer->autoResizes )
                    {
                        cContainer->set_width( outterWidth - barXPadding*2 );
                    }
                }
            }
        }
        else if( rowSizingStyle == panel_align_left)
        {
            widget_box.w = outterWidth;
            if( optionsSize > 1)
            {
                for( i=optionsSize -1; i > 0; i--)
                {
                    cContainer = subOptions[i];

                    if( cContainer->get_width() > outterWidth )
                    {
                        cContainer->set_width( outterWidth );
                    }
                    if(cContainer!=NULL)
                    {
                        remainderWidth+= cContainer->get_width() + barXPadding *2;
                    }
                }
                cContainer = subOptions[0];
                if(cContainer->autoResizes )
                {
                    if(cContainer->autoResizes )
                    {
                        cContainer->set_width( outterWidth - remainderWidth -  barXPadding );
                    }
                    else
                    {
                        cContainer->set_width( 0 );
                    }
                }
            }
            else
            {
                //equal to 1, since function would've returned if 0
                cContainer = subOptions[ 0 ];
                if(cContainer->autoResizes )
                {
                    cContainer->set_width( outterWidth - barXPadding*2 );
                }
                else if( cContainer->get_width() > outterWidth - barXPadding*2  )
                {
                    cContainer->set_width( outterWidth - barXPadding*2 );
                }
            }
        }
        else if( rowSizingStyle == panel_align_right)
        {
            if( optionsSize > 1)
            {
                for( i=0; i < optionsSize -1; i++)
                {
                    cContainer = subOptions[i];
                    if(cContainer!=NULL)
                    {
                        remainderWidth+= cContainer->get_width()  + barXPadding*2;
                    }
                }
                cContainer = subOptions[optionsSize -1];
                if(cContainer->autoResizes )
                {
                    if( remainderWidth < outterWidth)
                    {
                        cContainer->set_width( outterWidth - remainderWidth - barXPadding*2 );
                    }
                    else
                    {
                        cContainer->set_width( 0 );
                    }
                }
            }
            else
            {
                //equal to 1, since function would've returned if 0
                cContainer = subOptions[ 0 ];
                if(cContainer->autoResizes )
                {
                    cContainer->set_width( outterWidth - barXPadding*2  );
                }
            }
        }
        else if( rowSizingStyle == panel_align_equal)
        {
            widget_box.w = outterWidth;
            if( optionsSize > 1)
            {
                for( i=optionsSize -1; i > 0; i--)
                {
                    cContainer = subOptions[i];

                    if( cContainer->get_width() > outterWidth )
                    {
                        cContainer->set_width( outterWidth );
                    }
                    if(cContainer!=NULL && !cContainer->autoResizes )
                    {
                        remainderWidth+= cContainer->get_width() + barXPadding *2;
                    }
                }
                cContainer = subOptions[0];
                if(cContainer->autoResizes )
                {
                    if(cContainer->autoResizes )
                    {
                        cContainer->set_width( outterWidth - remainderWidth -  barXPadding );
                    }
                    else
                    {
                        cContainer->set_width( 0 );
                    }
                }
            }
            else
            {
                //equal to 1, since function would've returned if 0
                cContainer = subOptions[ 0 ];
                if(cContainer->autoResizes )
                {
                    cContainer->set_width( outterWidth - barXPadding*2 );
                }
                else if( cContainer->get_width() > outterWidth - barXPadding*2  )
                {
                    cContainer->set_width( outterWidth - barXPadding*2 );
                }
            }
        }
        else if( hAlign==gpe::fa_right || hAlign == gpe::fa_center  )
        {
            int foundRowWidth = 0;
            for( i=0; i<optionsSize; i++)
            {
                cContainer = subOptions[i];
                if(cContainer!=NULL)
                {
                    if( cContainer->is_full_width() )
                    {
                        cContainer->set_width( outterWidth - barXPadding );
                        foundRowWidth = cContainer->get_width();
                        keepCalculatingWidth = false;
                    }
                    else if( keepCalculatingWidth )
                    {
                        foundRowWidth+=cContainer->get_width()+barXPadding;
                    }
                }
            }

            if( hAlign==gpe::fa_right)
            {
                tempX += outterWidth - foundRowWidth - barXPadding;
            }
            else if(hAlign == gpe::fa_center  )
            {
                tempX += abs( outterWidth - foundRowWidth )/2;
            }
        }
        else
        {
            //defaults to left align( gpe::fa_left )
            widget_box.w = outterWidth;
            for( i=0; i<optionsSize; i++)
            {
                cContainer = subOptions[i];
                if(cContainer!=NULL)
                {
                    if( cContainer->is_full_width() && cContainer->autoResizes )
                    {
                        cContainer->set_width( outterWidth );
                    }
                }
            }
        }

        //Does vertical align calculations
        //and actually sets coordinates
        int cHeight = 0;

        if( optionsSize == 1)
        {
            cContainer = subOptions[0];
            if( cContainer!=NULL )
            {
                if( cContainer->is_full_width() == false )
                {
                    //widget_box.w
                }
                if( hAlign == gpe::fa_center)
                {
                    cContainer->set_coords( widget_box.x + indentWidth+( outterWidth - cContainer->get_width() ) /2, newY );
                }
                else if( hAlign == gpe::fa_right)
                {
                    cContainer->set_coords(  widget_box.x+indentWidth+outterWidth - cContainer->get_width() - barXPadding, newY );
                }
                else
                {
                    cContainer->set_coords(newX+indentWidth, newY );
                }
            }
            widget_box.w = cContainer->get_width();
            widget_box.h = cContainer->get_height();
            return;
        }

        widget_box.w = 0;
        if( vAlign == gpe::fa_middle)
        {
            for( i = 0; i < optionsSize; i++)
            {
                cContainer = subOptions[i];
                if( cContainer!=NULL )
                {
                    cHeight = cContainer->get_height();
                    if(cContainer->autoResizes)
                    {
                        //cContainer->set_width( widget_box.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                    }

                    if( cHeight > widget_box.h)
                    {
                        widget_box.h = cContainer->get_height();
                        cContainer->set_coords(tempX, tempY  );
                    }
                    else
                    {
                        cContainer->set_coords(tempX, tempY + abs( widget_box.h - cHeight ) /2 );
                    }
                    tempX+=barXPadding+cContainer->get_width();
                    widget_box.w+=  cContainer->get_width();
                }
            }
        }
        else if( vAlign == gpe::fa_bottom )
        {
            for( i = 0; i < optionsSize; i++)
            {
                cContainer = subOptions[i];
                if( cContainer!=NULL )
                {
                    cHeight = cContainer->get_height();
                    if(cContainer->autoResizes)
                    {
                        //cContainer->set_width( widget_box.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                    }
                    if( cHeight > widget_box.h)
                    {
                        widget_box.h = cHeight;
                        cContainer->set_coords(tempX, tempY );
                    }
                    else
                    {
                        cContainer->set_coords(tempX, tempY + (widget_box.h - cHeight) );
                    }
                    tempX+=barXPadding+cContainer->get_width();
                    widget_box.w+= cContainer->get_width();
                }
            }
        }
        else
        {
            //defaults to gpe::fa_top
            for( i = 0; i < optionsSize; i++)
            {
                cContainer = subOptions[i];
                if( cContainer!=NULL )
                {
                    cContainer->set_coords(tempX, tempY );
                    if( cContainer->autoResizes )
                    {
                        //cContainer->set_width( widget_box.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                    }
                    tempX+=barXPadding+cContainer->get_width();
                    if( cContainer->get_height() > widget_box.h)
                    {
                        widget_box.h = cContainer->get_height();
                    }
                    widget_box.w+= barXPadding + cContainer->get_width();
                }
            }
        }
    }

    void widget_content_row::set_full_width( int maxRowWidth )
    {

    }

    void widget_content_row::set_horizontal_align(int hValue)
    {
        hAlign = hValue;
    }

    void widget_content_row::set_vertical_align(int vValue)
    {
        vAlign = vValue;
    }

    void widget_content_row::set_full_width()
    {

    }

    void widget_content_row::set_maxed_out_width()
    {

    }

    void set_maxed_out_height()
    {

    }

    widget_panel_list::widget_panel_list()
    {
        panelAlignType = panel_align_default;
        usingFullSizeElement = false;
        selectedElement = NULL;
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
        hAlign = gpe::fa_left;
        vAlign = gpe::fa_top;
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
        clear_list();
    }

    void widget_panel_list::add_gui_element(widget_basic *  newElement, bool isNLElement)
    {
        if( newElement!=NULL )
        {
            widget_content_row *  currentRow = NULL;
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
                currentRow->hAlign = hAlign;
                currentRow->vAlign = vAlign;
                currentRow->barXPadding = barXPadding;
                currentRow->barYPadding = barYPadding;
                subRows.push_back( currentRow );
            }
            else
            {
                currentRow = subRows[ rowSize -1 ];
            }

            if( currentRow !=NULL )
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
            newElement->dynamicId = (int)allElements.size();
            allElements.push_back( newElement );
        }
    }

    void widget_panel_list::add_gui_auto(widget_basic *  newElement )
    {
        if( newElement!=NULL)
        {
            widget_content_row *  currentRow = NULL;
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
                currentRow->hAlign = hAlign;
                currentRow->vAlign = vAlign;
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
                if( currentRow !=NULL )
                {
                    //Checks if a new element can fit inside of same row
                    if( currentRow->get_sub_width()+ newElement->get_width() > widget_box.w-32 )
                    {
                        //Add a new row and add element to it.
                        currentRow = new widget_content_row();
                        currentRow->hAlign = hAlign;
                        currentRow->vAlign = vAlign;
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
                    //gpe::error_log->report("Current row = NULL...");
                }
            }

            //Adds content to giant list of objects within list.
            //Then gives it a unique id
            newElement->dynamicId = (int)allElements.size();
            allElements.push_back( newElement );

        }
    }

    void widget_panel_list::add_gui_element_fullsize( widget_basic * newElement )
    {
        if( newElement !=NULL )
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

    void widget_panel_list::add_indented_element( int level, widget_basic * newElement)
    {
        if( newElement!=NULL )
        {
            widget_content_row * newRow = new widget_content_row();
            newRow->indentationLevel = level;
            newElement->indentationLevel = level;
            newRow->hAlign = hAlign;
            newRow->vAlign = vAlign;
            newRow->barXPadding = barXPadding;
            newRow->barYPadding = barYPadding;
            newRow->add_gui_element( newElement );
            subRows.push_back( newRow );
            //Adds content to giant list of objects within list.
            //Then gives it a unique id
            newElement->dynamicId = (int)allElements.size();
            allElements.push_back( newElement );
        }
    }

    void widget_panel_list::clear_list()
    {
        usingFullSizeElement = false;
        selectedElement = NULL;
        allElements.clear();
        cameraBox.w = menuBox.w = entireBox.w = widget_box.w;
        cameraBox.h = menuBox.h = entireBox.h = widget_box.h;

        widget_content_row * tempRow = NULL;
        for( int i = (int)subRows.size()-1; i>=0; i--)
        {
            tempRow = subRows[i];
            if( tempRow!=NULL )
            {
                delete tempRow;
                tempRow = NULL;
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
        selectedElement = NULL;
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
        widget_content_row * cRow = NULL;
        widget_basic * cContainer = NULL;
        widget_basic * containerInControl = NULL;
        int lastselectedId = -1;
        int i = 0, j = 0;
        int pastRowChange = 0;
        int currentRowCount = (int)subRows.size();
        int rowOptionsCount = 0;
        bool directionChangeRequested = false;

        if( inDebugMode)
        {
            gpe::error_log->report("["+ stg_ex::int_to_string(currentRowCount)+"] unique rows gathered...");
        }
        if( isClicked )
        {
            //if( resource_dragged==NULL)
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
                leserKeyDelay+= gpe::time_keeper->get_delta_ticks();
            }
            else
            {
                leserKeyDelay = -1;
            }

            if( gpe::input->kb_button_down[kb_ctrl] && gpe::input->kb_button_down[kb_period] && gpe::input->kb_button_pressed[kb_period]==false && gpe::input->kb_button_released[kb_period]==false )
            {
                greaterKeyDelay+= gpe::time_keeper->get_delta_ticks();
            }
            else
            {
                greaterKeyDelay = -1;
            }
        }
        if( isInUse && hasArrowkeyControl )
        {
            if( gpe::input->kb_button_down[kb_tab] && gpe::input->kb_button_pressed[kb_tab]==false && gpe::input->kb_button_released[kb_tab]==false )
            {
                tabDelay += gpe::time_keeper->get_delta_ticks();
            }
            else
            {
                tabDelay = -1;
            }
            if( gpe::input->kb_button_down[kb_left] && gpe::input->kb_button_pressed[kb_left]==false && gpe::input->kb_button_released[kb_left]==false )
            {
                leftDelay+= gpe::time_keeper->get_delta_ticks();
            }
            else
            {
                leftDelay = -1;
            }
            if( gpe::input->kb_button_down[kb_right] && gpe::input->kb_button_pressed[kb_right]==false && gpe::input->kb_button_released[kb_right]==false )
            {
                rightDelay+= gpe::time_keeper->get_delta_ticks();
            }
            else
            {
                rightDelay = -1;
            }
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
                downDelay+= gpe::time_keeper->get_delta_ticks();
            }
            else
            {
                downDelay = -1;
            }
        }
        else
        {
            tabDelay = -1;
            upDelay = -1;
            downDelay = -1;
            leftDelay = -1;
            rightDelay = -1;
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
        int xPos = barXMargin+barXPadding;
        int yPos = barYMargin+barYPadding;
        int y2Pos = yPos;
        int rowWidth = 0;
        int maxRowWidth = 0;
        int totalMaxRowWidth = 0;
        int rowHeight = 0;

        xPos = barXMargin+barXPadding;

        for( i=0; i < currentRowCount; i++)
        {
            cRow = subRows[i];
            if( cRow!=NULL )
            {
                cRow->rowSizingStyle = panelAlignType;
                cRow->barXPadding = barXPadding;
                cRow->barYPadding = barYPadding;

                cRow->outterWidth  = menuBox.w - barXMargin - yScroll->get_width();

                cRow->outterHeight = menuBox.h - barYMargin - xScroll->get_height();
                cRow->set_width( menuBox.w );
                cRow->set_height( menuBox.h );
                cRow->set_coords(xPos, y2Pos );

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
                if( cContainer!=NULL )
                {
                    cContainer->dynamicId = i;
                    if( cContainer->hasScrollControl )
                    {
                        subElementsIsScrolling = true;
                        containerInControl = cContainer;
                        lastselectedId = selectedId = cContainer->dynamicId;
                        break;
                    }
                    if( cContainer->hasArrowkeyControl )
                    {
                        subElementsHasArrowControl = true;
                        containerInControl = cContainer;
                        lastselectedId = selectedId = cContainer->dynamicId;
                        break;
                    }
                    if( cContainer->is_inuse() )
                    {
                        containerInControl = cContainer;
                        lastselectedId = selectedId = cContainer->dynamicId;
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
            if( containerInControl!=NULL )
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
                    containerInControl = NULL;
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
                if( cContainer!=NULL )
                {
                    cContainer->dynamicId = i;
                    if( lastselectedId != cContainer->dynamicId )
                    {
                        //gpe::error_log->report("Trying to process "+cContainer->get_element_type() +"..." );
                        if( inDebugMode)
                        {
                            gpe::error_log->report("Trying to process "+cContainer->get_element_type() +"..." );
                        }
                        cContainer->hAlign = hAlign;
                        cContainer->process_self(&menuBox,&cameraBox);

                        if( cContainer->hasScrollControl )
                        {
                            subElementsIsScrolling = true;
                            containerInControl = cContainer;
                            selectedId = cContainer->dynamicId;
                            directionChangeRequested = true;
                        }

                        if( cContainer->hasArrowkeyControl )
                        {
                            subElementsHasArrowControl=true;
                            containerInControl = cContainer;
                            selectedId = cContainer->dynamicId;
                            directionChangeRequested = true;
                        }

                        if( cContainer->is_inuse() )
                        {
                            selectedId = cContainer->dynamicId;
                            containerInControl = cContainer;
                            directionChangeRequested = true;
                        }
                    }
                }
            }

            if( containerInControl!=NULL )
            {
                if( inDebugMode)
                {
                    gpe::error_log->report("Processing selected container..." );
                    gpe::error_log->report("Trying to process "+cContainer->get_element_type() +"..." );
                }

                if( containerInControl!=NULL && containerInControl->is_inuse() == false )
                {
                    containerInControl = NULL;
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
                tabDelay = -1;
            }

            if( subElementsHasArrowControl==false)
            {
                if( upDelay > main_settings->normalInputDelayTime )
                {
                    scroll_up( cameraBox.h/4 );
                    upDelay = -1;
                }
                else if( downDelay > main_settings->normalInputDelayTime )
                {
                    scroll_down( cameraBox.h/4 );
                    downDelay = -1;
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
            if(cContainer!=NULL)
            {
                if( selectedId  == cContainer->dynamicId )
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

        if( hasScrollControl && subElementsHasArrowControl==false &&  xScroll!=NULL && yScroll!=NULL)
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
        if( view_space!=NULL && cam!=NULL )
        {
            /*
            menuBox.x = view_space->x+ widget_box.x;
            menuBox.y = view_space->x+ widget_box.y;
            menuBox.w = widget_box.w;
            menuBox.h = widget_box.h;
            */
            gpe::renderer_main->reset_viewpoint();
            gpe::renderer_main->set_viewpoint( view_space );

            widget_basic * cResource = NULL;

            if( usingFullSizeElement && (int)allElements.size() == 1 )
            {
                cResource = allElements[0];
                if( cResource!=NULL )
                {
                    cResource->set_coords( 0,0 );
                    cResource->set_width( widget_box.w);
                    cResource->set_height( widget_box.h);
                    cResource->render_self( view_space, cam );
                    return;
                }
            }

            gpe::renderer_main->set_viewpoint( &menuBox);
            widget_content_row * cRow = NULL;
            int i = 0,j = 0;
            for( i=0; i<(int)subRows.size(); i++)
            {
                cRow = subRows[i];
                if(cRow!=NULL)
                {
                    for( j =0; j < (int)cRow->subOptions.size(); j++ )
                    {
                        cResource = cRow->subOptions[j];
                        if(cResource!=NULL)
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
            if( xScroll!=NULL&& hideXScroll!=true )
            {
                //if( entireBox.w >widget_box.w)
                {
                    xScroll->render_self( view_space,cam);
                }
            }
            if( yScroll!=NULL&& hideYScroll!=true )
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

    void widget_panel_list::set_horizontal_align(int hValue)
    {
        hAlign = hValue;
        widget_content_row * tguiRow = NULL;
        for( int i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=NULL )
            {
                tguiRow->hAlign = hValue;
            }
        }
        widget_basic * cElement = NULL;
        for( int i = (int)allElements.size()-1; i>=0; i--)
        {
            cElement = allElements[i];
            if( cElement!=NULL )
            {
                cElement->hAlign = hValue;
            }
        }
    }

    void widget_panel_list::set_full_width()
    {
        widget_content_row * tguiRow = NULL;
        int maxWidth = widget_box.w-(barXMargin+barXPadding)*3;
        for( int i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=NULL )
            {
                tguiRow->set_width(maxWidth);
            }
        }
    }

    void widget_panel_list::set_maxed_out_width()
    {
        widget_content_row * tguiRow = NULL;
        int i = 0;
        int maxWidth = 0;
        for( i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=NULL )
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
            if( tguiRow!=NULL )
            {
                tguiRow->set_width(maxWidth);
            }
        }
    }

    void widget_panel_list::set_maxed_out_height()
    {
        widget_content_row * tguiRow = NULL;
        int i = 0;
        int maxHeight = 0;
        for( i = (int)subRows.size()-1; i>=0; i--)
        {
            tguiRow = subRows[i];
            if( tguiRow!=NULL )
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
            if( tguiRow!=NULL )
            {
                tguiRow->set_height(maxHeight);
            }
        }
    }
}
