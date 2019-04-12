/*
paw_gui_panel_list.cpp
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

#include "paw_gui_panel_list.h"

GPE_GuiElementRow::GPE_GuiElementRow()
{
    calculatedRowWidth = 0;
    calculatedRowHeight = 0;
    inDebugMode = false;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 0;
    elementBox.h = 0;
    hAlign = FA_LEFT;
    vAlign = FA_TOP;
    barXPadding = 0;
    barYPadding = 0;
    outterWidth = 0;
    outterHeight = 0;
    /*barXMargin = 0;
    barYMargin = 0;*/
}

GPE_GuiElementRow::~GPE_GuiElementRow()
{
    clear_list();
}

void GPE_GuiElementRow::add_gui_element(GPE_GeneralGuiElement *  newElement )
{
    subOptions.push_back( newElement );
}

void GPE_GuiElementRow::calculate_width()
{
    calculatedRowWidth = (barXPadding)*2 + indentationLevel*GENERAL_GPE_PADDING;
    GPE_GeneralGuiElement * tempOption = NULL;
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
                //GPE_Report("Calculated outer width:"+int_to_string(outterWidth) );
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
    //GPE_Report("Calculated width:"+int_to_string(calculatedRowWidth) );
}

void GPE_GuiElementRow::clear_list()
{
    /*
    GPE_GeneralGuiElement * tempItem = NULL;
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

int GPE_GuiElementRow::get_sub_width()
{
    calculate_width();
    return calculatedRowWidth;
}
//Correct width issue
void GPE_GuiElementRow::set_coords(int newX, int newY)
{
    elementBox.x = newX;
    elementBox.y = newY;
    elementBox.w = 0;
    elementBox.h = 0;
    //For now we just gonna assume is all fa_left until its time to fix it
    GPE_GeneralGuiElement * cContainer = NULL;
    int indentWidth = indentationLevel*GENERAL_GPE_PADDING;
    int tempX = newX + indentWidth;
    int tempY = newY;//+barYPadding;
    int i;
    int optionsSize = (int)subOptions.size();
    int maxHeight = 0;

    //Does horizontal align calculations
    bool keepCalculatingWidth = true;
    if( hAlign==FA_RIGHT || hAlign == FA_CENTER  )
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

        if( hAlign==FA_RIGHT)
        {
            tempX += outterWidth - foundRowWidth - barXPadding;
        }
        else if(hAlign == FA_CENTER  )
        {
            tempX += abs( outterWidth - foundRowWidth )/2;
        }
    }
    else
    {
        //defaults to left align( FA_LEFT )
        elementBox.w = outterWidth;
        for( i=0; i<optionsSize; i++)
        {
            cContainer = subOptions[i];
            if(cContainer!=NULL)
            {
                if( cContainer->is_full_width() )
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
                //elementBox.w
            }
            if( hAlign == FA_CENTER)
            {
                cContainer->set_coords( elementBox.x + indentWidth+( outterWidth - cContainer->get_width() ) /2, newY );
            }
            else if( hAlign == FA_RIGHT)
            {
                cContainer->set_coords(  elementBox.x+indentWidth+outterWidth - cContainer->get_width() - barXPadding, newY );
            }
            else
            {
                cContainer->set_coords(newX+indentWidth, newY );
            }
        }
        elementBox.w = cContainer->get_width();
        elementBox.h = cContainer->get_height();
        return;
    }

    elementBox.w = 0;
    if( vAlign == FA_MIDDLE)
    {
        for( i = 0; i < optionsSize; i++)
        {
            cContainer = subOptions[i];
            if( cContainer!=NULL )
            {
                cHeight = cContainer->get_height();
                if(cContainer->autoResizes)
                {
                    //cContainer->set_width( elementBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }

                if( cHeight > elementBox.h)
                {
                    elementBox.h = cContainer->get_height();
                    cContainer->set_coords(tempX, tempY  );
                }
                else
                {
                    cContainer->set_coords(tempX, tempY + abs( elementBox.h - cHeight ) /2 );
                }
                tempX+=barXPadding+cContainer->get_width();
                elementBox.w+=  cContainer->get_width();
            }
        }
    }
    else if( vAlign == FA_BOTTOM )
    {
        for( i = 0; i < optionsSize; i++)
        {
            cContainer = subOptions[i];
            if( cContainer!=NULL )
            {
                cHeight = cContainer->get_height();
                if(cContainer->autoResizes)
                {
                    //cContainer->set_width( elementBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                if( cHeight > elementBox.h)
                {
                    elementBox.h = cHeight;
                    cContainer->set_coords(tempX, tempY );
                }
                else
                {
                    cContainer->set_coords(tempX, tempY + (elementBox.h - cHeight) );
                }
                tempX+=barXPadding+cContainer->get_width();
                elementBox.w+= cContainer->get_width();
            }
        }
    }
    else
    {
        //defaults to FA_TOP
        for( i = 0; i < optionsSize; i++)
        {
            cContainer = subOptions[i];
            if( cContainer!=NULL )
            {
                cContainer->set_coords(tempX, tempY );
                if(cContainer->autoResizes)
                {
                    //cContainer->set_width( elementBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                tempX+=barXPadding+cContainer->get_width();
                if( cContainer->get_height() > elementBox.h)
                {
                    elementBox.h = cContainer->get_height();
                }
                elementBox.w+= barXPadding + cContainer->get_width();
            }
        }
    }
}

void GPE_GuiElementRow::set_full_width( int maxRowWidth )
{

}

void GPE_GuiElementRow::set_horizontal_align(int hValue)
{
    hAlign = hValue;
}

void GPE_GuiElementRow::set_vertical_align(int vValue)
{
    vAlign = vValue;
}

void GPE_GuiElementRow::set_full_width()
{

}

void GPE_GuiElementRow::set_maxed_out_width()
{

}

void set_maxed_out_height()
{

}

GPE_GuiElementList::GPE_GuiElementList()
{
    usingFullSizeElement = false;
    selectedElement = NULL;
    needsNewLine = true;
    inDebugMode = false;
    selectedId = -1;
    newRowRequested = false;
    hideXScroll = true;
    hideYScroll = true;
    cameraBox.x = menuBox.x = entireBox.x = elementBox.x = 0;
    cameraBox.y = menuBox.y = entireBox.y = elementBox.y = 0;
    cameraBox.w = menuBox.w = entireBox.w = elementBox.w;
    cameraBox.h = menuBox.h = entireBox.h = elementBox.h = RESOURCE_AREA_HEIGHT*3;
    xScroll = new GPE_ScrollBar_XAxis();
    yScroll = new GPE_ScrollBar_YAxis();
    cameraBox.w = menuBox.w = entireBox.w = elementBox.w - yScroll->get_box_width();
    barXPadding = GENERAL_GPE_PADDING;
    barYPadding = GENERAL_GPE_PADDING;
    barXMargin = 0;
    barYMargin = 0;
    isInUse = true;
    subElementsHasArrowControl = false;
    subElementsIsScrolling = false;
    hAlign = FA_LEFT;
    vAlign = FA_TOP;
    guiListTypeName = "guilist";
    tabDelay = 0;
    leserKeyDelay = 0;
    greaterKeyDelay = 0;
    upDelay = 0;
    downDelay = 0;
    leftDelay = 0;
    rightDelay = 0;
}

GPE_GuiElementList::~GPE_GuiElementList()
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

void GPE_GuiElementList::add_gui_element(GPE_GeneralGuiElement *  newElement, bool isNLElement)
{
    if( newElement!=NULL )
    {
        GPE_GuiElementRow *  currentRow = NULL;
        if( newElement->requires_newline() || newElement->is_full_width() )
        {
            newRowRequested = true;
        }
        newElement->hasLineBreak = isNLElement;
        int rowSize = (int)subRows.size();

        if(  rowSize == 0 || newRowRequested )
        {
            currentRow = new GPE_GuiElementRow();
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

void GPE_GuiElementList::add_gui_auto(GPE_GeneralGuiElement *  newElement )
{
    if( newElement!=NULL)
    {
        GPE_GuiElementRow *  currentRow = NULL;
        newElement->hasLineBreak = false;
        int rowSize = (int)subRows.size();

        if( newElement->requires_newline() || newElement->is_full_width() )
        {
            newRowRequested = true;
            newElement->hasLineBreak = true;
        }

        if(  rowSize == 0 || newRowRequested )
        {
            currentRow = new GPE_GuiElementRow();
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
                if( currentRow->get_sub_width()+ newElement->get_width() > elementBox.w-32 )
                {
                    //Add a new row and add element to it.
                    currentRow = new GPE_GuiElementRow();
                    currentRow->hAlign = hAlign;
                    currentRow->vAlign = vAlign;
                    currentRow->barXPadding = barXPadding;
                    currentRow->barYPadding = barYPadding;
                    subRows.push_back( currentRow );
                    currentRow->add_gui_element( newElement );
                    //GPE_Report("Not able to squeeze in...");
                    newRowRequested = false;
                }
                else
                {
                    //Able to squeeze this element into view
                    currentRow->add_gui_element( newElement );
                    //GPE_Report("Able to squeeze in...");
                    newRowRequested = false;
                }
            }
            else
            {
                //GPE_Report("Current row = NULL...");
            }
        }

        //Adds content to giant list of objects within list.
        //Then gives it a unique id
        newElement->dynamicId = (int)allElements.size();
        allElements.push_back( newElement );

    }
}

void GPE_GuiElementList::add_gui_element_fullsize( GPE_GeneralGuiElement * newElement )
{
    if( newElement !=NULL )
    {
        clear_list();
        newElement->set_coords( elementBox.y, elementBox.y );
        newElement->set_width( elementBox.w );
        newElement->set_height( elementBox.h );
        newElement->set_full_width( true );
        barXPadding = 0;
        barYPadding = 0;
        barXMargin = 0;
        barYMargin = 0;
        add_gui_auto( newElement );
        usingFullSizeElement = true;
    }
}

void GPE_GuiElementList::add_indented_element( int level, GPE_GeneralGuiElement * newElement)
{
    if( newElement!=NULL )
    {
        GPE_GuiElementRow * newRow = new GPE_GuiElementRow();
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

void GPE_GuiElementList::clear_list()
{
    usingFullSizeElement = false;
    selectedElement = NULL;
    allElements.clear();
    cameraBox.w = menuBox.w = entireBox.w = elementBox.w;
    cameraBox.h = menuBox.h = entireBox.h = elementBox.h;

    GPE_GuiElementRow * tempRow = NULL;
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


void GPE_GuiElementList::scroll_down(int yToMove )
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

void GPE_GuiElementList::scroll_up(int yToMove )
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

void GPE_GuiElementList::scroll_left(int xToMove )
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

void GPE_GuiElementList::scroll_right(int xToMove )
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

void GPE_GuiElementList::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    selectedElement = NULL;
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( inDebugMode)
    {
        GPE_Report("Starting to process GPE_GuiElementList...");
    }
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    xScroll->set_coords( elementBox.x, elementBox.h-8);
    xScroll->set_width( elementBox.w );
    xScroll->set_height( 8 );

    yScroll->set_coords( elementBox.w-8, elementBox.y );
    yScroll->set_width( 8 );
    yScroll->set_height( elementBox.h );
    //Handles row / option in view
    GPE_GuiElementRow * cRow = NULL;
    GPE_GeneralGuiElement * cContainer = NULL;
    GPE_GeneralGuiElement * containerInControl = NULL;
    int lastSelectedId = -1;
    int i = 0, j = 0;
    int pastRowChange = 0;
    int currentRowCount = (int)subRows.size();
    int rowOptionsCount = 0;
    bool directionChangeRequested = false;

    if( inDebugMode)
    {
        GPE_Report("["+int_to_string(currentRowCount)+"] unique rows gathered...");
    }
    if( isClicked )
    {
        //if( RESOURCE_TO_DRAG==NULL)
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

    menuBox.x = elementBox.x+viewedSpace->x;
    menuBox.y = elementBox.y+viewedSpace->y;
    if( hideXScroll )
    {
        cameraBox.w = menuBox.w = elementBox.w;
    }
    else
    {
        cameraBox.w = menuBox.w = elementBox.w-16;
    }

    if( hideYScroll)
    {
        cameraBox.h = menuBox.h = elementBox.h;
    }
    else
    {
        cameraBox.h = menuBox.h = elementBox.h-16;
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


    //Handles keyboard/gamecontroller input as well as delays
    if( isInUse)
    {

        if( input->down[kb_ctrl] && input->down[kb_comma] && input->pressed[kb_comma]==false && input->released[kb_comma]==false )
        {
            leserKeyDelay+= gpe->get_delta_time();
        }
        else
        {
            leserKeyDelay = -1;
        }

        if( input->down[kb_ctrl] && input->down[kb_period] && input->pressed[kb_period]==false && input->released[kb_period]==false )
        {
            greaterKeyDelay+= gpe->get_delta_time();
        }
        else
        {
            greaterKeyDelay = -1;
        }
    }
    if( isInUse && hasArrowkeyControl )
    {
        if( input->down[kb_tab] && input->pressed[kb_tab]==false && input->released[kb_tab]==false )
        {
            tabDelay += gpe->get_delta_time();
        }
        else
        {
            tabDelay = -1;
        }
        if( input->down[kb_left] && input->pressed[kb_left]==false && input->released[kb_left]==false )
        {
            leftDelay+= gpe->get_delta_time();
        }
        else
        {
            leftDelay = -1;
        }
        if( input->down[kb_right] && input->pressed[kb_right]==false && input->released[kb_right]==false )
        {
            rightDelay+= gpe->get_delta_time();
        }
        else
        {
            rightDelay = -1;
        }
        if( input->down[kb_up] && input->pressed[kb_up]==false && input->released[kb_up]==false )
        {
            upDelay+= gpe->get_delta_time();
        }
        else
        {
            upDelay = -1;
        }
        if( input->down[kb_down] && input->pressed[kb_down]==false && input->released[kb_down]==false )
        {
            downDelay+= gpe->get_delta_time();
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
        GPE_Report("[Calculating box size...");
    }

    //Sets up the coordinates for the rows and their content
    if( usingFullSizeElement && (int)allElements.size() == 1 )
    {
        cContainer = allElements[0];
        cContainer->set_coords(elementBox.x, elementBox.y);
        cContainer->set_width( elementBox.w);
        cContainer->set_height( elementBox.h);
        cContainer->process_self( viewedSpace, cam );
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
            cRow->barXPadding = barXPadding+barXMargin;
            cRow->barYPadding = barYPadding+barYPadding;

            cRow->outterWidth = menuBox.w - barXMargin - barXPadding - yScroll->get_width();
            cRow->outterHeight = menuBox.h - barYMargin - barYPadding - xScroll->get_height();
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
        GPE_Report("["+int_to_string(entireBox.w)+"] X ["+int_to_string(entireBox.h)+"] size...");
    }
    //Processes each container if they are in focus or not...
    if( scrolingHasntOccurred )
    {
        if( inDebugMode)
        {
            GPE_Report("Checking rolls for scrolling..." );
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
                    lastSelectedId = selectedId = cContainer->dynamicId;
                    break;
                }
                if( cContainer->hasArrowkeyControl )
                {
                    subElementsHasArrowControl = true;
                    containerInControl = cContainer;
                    lastSelectedId = selectedId = cContainer->dynamicId;
                    break;
                }
                if( cContainer->is_inuse() )
                {
                    containerInControl = cContainer;
                    lastSelectedId = selectedId = cContainer->dynamicId;
                    break;
                }
            }
        }

        if( inDebugMode)
        {
            GPE_Report("Done..." );
        }

        //locks on one gui item until it is taken out of focus...
        lastSelectedId = selectedId;
        if( containerInControl!=NULL )
        {
            if( inDebugMode)
            {
                GPE_Report("Processing selected container..." );
                GPE_Report("Trying to process "+containerInControl->get_element_type() +"..." );
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
                GPE_Report("Done..." );
            }
        }

        //else
        if( inDebugMode)
        {
            GPE_Report("Processing other containers..." );
        }

        for( i =(int)allElements.size()-1; i >=0 ; i-- )
        {
            cContainer = allElements[i];
            if( cContainer!=NULL )
            {
                cContainer->dynamicId = i;
                if( lastSelectedId != cContainer->dynamicId )
                {
                    //GPE_Report("Trying to process "+cContainer->get_element_type() +"..." );
                    if( inDebugMode)
                    {
                        GPE_Report("Trying to process "+cContainer->get_element_type() +"..." );
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
                GPE_Report("Processing selected container..." );
                GPE_Report("Trying to process "+cContainer->get_element_type() +"..." );
            }

            if( containerInControl!=NULL && containerInControl->is_inuse() == false )
            {
                containerInControl = NULL;
                //selectedId++;
                /*
                if( inDebugMode)
                GPE_Report("Container "+cContainer->get_element_type() +" lost control..." );
                */
            }
            if( inDebugMode)
            {
                GPE_Report("Done..." );
            }
        }

        if( inDebugMode)
        {
            GPE_Report("Done..." );
        }
    }

    //if(  isHovered || (isInUse && subElementsIsScrolling==false && hasScrollControl ) )
    if( isHovered && subElementsIsScrolling==false )
    {
        if( input->mouseScrollingUp )
        {
            if( input->shiftKeyIsPressed)
            {
                scroll_left( cameraBox.w/4 );
            }
            else
            {
                scroll_up( cameraBox.h/4 );
            }
        }
        else if( input->mouseScrollingDown )
        {
            if( input->shiftKeyIsPressed )
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
    if( leserKeyDelay > MAIN_GUI_SETTINGS->normalInputDelayTime )
    {
        selectedId--;
        directionChangeRequested = true;
        leserKeyDelay = -1;
    }
    else if( greaterKeyDelay > MAIN_GUI_SETTINGS->normalInputDelayTime )
    {
        selectedId++;
        directionChangeRequested = true;
        greaterKeyDelay = -1;
    }
    else if( hasScrollControl && subElementsIsScrolling==false)
    {
        if( tabDelay > MAIN_GUI_SETTINGS->normalInputDelayTime  && input->check_keyboard_down(kb_ctrl)==false )
        {
            if( input->shiftKeyIsPressed)
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
            if( upDelay > MAIN_GUI_SETTINGS->normalInputDelayTime )
            {
                scroll_up( cameraBox.h/4 );
                upDelay = -1;
            }
            else if( downDelay > MAIN_GUI_SETTINGS->normalInputDelayTime )
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

    xScroll->set_coords( elementBox.x, elementBox.y+elementBox.h-xScroll->get_height() );
    xScroll->set_width( elementBox.w );

    xScroll->fullRect.x = 0;
    xScroll->fullRect.y = 0;
    xScroll->fullRect.w = entireBox.w;
    xScroll->fullRect.h = entireBox.h;

    xScroll->contextRect.x = cameraBox.x;
    xScroll->contextRect.y = cameraBox.y;
    xScroll->contextRect.w = cameraBox.w;
    xScroll->contextRect.h = cameraBox.h;

    yScroll->update_box(  elementBox.x+elementBox.w-8, elementBox.y, 8,elementBox.h);
    update_rectangle(&yScroll->fullRect, 0, 0, entireBox.w,entireBox.h);
    update_rectangle(&yScroll->contextRect, cameraBox.x, cameraBox.y, cameraBox.w,cameraBox.h);

    if( hasScrollControl && subElementsHasArrowControl==false &&  xScroll!=NULL && yScroll!=NULL)
    {
        if( entireBox.w >=elementBox.w  && hideXScroll!=true )
        {
            xScroll->process_self(viewedSpace,cam);
        }
        else
        {
            cameraBox.x =0;
            cameraBox.w = menuBox.w = elementBox.w;
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
            yScroll->process_self(viewedSpace,cam );
        }
        else
        {
            cameraBox.y = 0;
            cameraBox.w = menuBox.w = elementBox.w;
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
        xScroll->process_self(viewedSpace, cam);
        yScroll->process_self(viewedSpace, cam);
    }
    selectedElement = containerInControl;
}

void GPE_GuiElementList::render_self(GPE_Rect * viewedSpace,GPE_Rect * cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        /*
        menuBox.x = viewedSpace->x+ elementBox.x;
        menuBox.y = viewedSpace->x+ elementBox.y;
        menuBox.w = elementBox.w;
        menuBox.h = elementBox.h;
        */
        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint( viewedSpace );

        GPE_GeneralGuiElement * cResource = NULL;

        if( usingFullSizeElement && (int)allElements.size() == 1 )
        {
            cResource = allElements[0];
            if( cResource!=NULL )
            {
                cResource->set_coords(elementBox.x, elementBox.y);
                cResource->set_width( elementBox.w);
                cResource->set_height( elementBox.h);
                cResource->render_self( viewedSpace, cam, forceRedraw );
                return;
            }
        }

        MAIN_RENDERER->set_viewpoint( &menuBox);
        GPE_GuiElementRow * cRow = NULL;
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
                        cResource->render_self( &menuBox,&cameraBox,forceRedraw);
                    }
                }
            }
        }

        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint( viewedSpace);
        //render_text_boxed( elementBox.x+elementBox.w-32,elementBox.y+GENERAL_GPE_PADDING,"Selection id:"+int_to_string(selectedId)+" / "+ int_to_string( (int)allElements.size() ),c_white,c_black,GPE_DEFAULT_FONT,FA_RIGHT,FA_TOP, 255 );
        /*menuBox.w-=16;
        menuBox.h-=16;*/
        if( xScroll!=NULL && forceRedraw && hideXScroll!=true )
        {
            //if( entireBox.w >elementBox.w)
            {
                xScroll->render_self( viewedSpace,cam);
            }
        }
        if( yScroll!=NULL && forceRedraw && hideYScroll!=true )
        {
            if( entireBox.h >elementBox.h )
            {
                yScroll->render_self( viewedSpace,cam);
            }
        }

        /*if( isInUse && subElementsIsScrolling==false && hasScrollControl && forceRedraw)
        {
            gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Button_Box_Highlighted_Color,true);
        }
        else
        {
            gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Main_Box_Faded_Color,true);
        }*/
        if( forceRedraw)
        {
            gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Main_Border_Color,true);
        }
        MAIN_RENDERER->reset_viewpoint();
    }
}

void GPE_GuiElementList::reset_self()
{
    clear_list();
    cameraBox.x = menuBox.x = entireBox.x = 0;
    cameraBox.y = menuBox.y = entireBox.y = 0;
    cameraBox.w = menuBox.w = entireBox.w = 32;
    cameraBox.h = menuBox.h = entireBox.h = RESOURCE_AREA_HEIGHT*3;
}

void GPE_GuiElementList::set_horizontal_align(int hValue)
{
    hAlign = hValue;
    GPE_GuiElementRow * tguiRow = NULL;
    for( int i = (int)subRows.size()-1; i>=0; i--)
    {
        tguiRow = subRows[i];
        if( tguiRow!=NULL )
        {
            tguiRow->hAlign = hValue;
        }
    }
    GPE_GeneralGuiElement * cElement = NULL;
    for( int i = (int)allElements.size()-1; i>=0; i--)
    {
        cElement = allElements[i];
        if( cElement!=NULL )
        {
            cElement->hAlign = hValue;
        }
    }
}

void GPE_GuiElementList::set_full_width()
{
    GPE_GuiElementRow * tguiRow = NULL;
    int maxWidth = elementBox.w-(barXMargin+barXPadding)*3;
    for( int i = (int)subRows.size()-1; i>=0; i--)
    {
        tguiRow = subRows[i];
        if( tguiRow!=NULL )
        {
            tguiRow->set_width(maxWidth);
        }
    }
}

void GPE_GuiElementList::set_maxed_out_width()
{
    GPE_GuiElementRow * tguiRow = NULL;
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

void GPE_GuiElementList::set_maxed_out_height()
{
    GPE_GuiElementRow * tguiRow = NULL;
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
