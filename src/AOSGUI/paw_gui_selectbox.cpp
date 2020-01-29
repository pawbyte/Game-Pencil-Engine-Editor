/*
paw_gui_selectbox.cpp
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

#include "paw_gui_selectbox.h"


GPE_SelectBoxBasic_Option::GPE_SelectBoxBasic_Option()
{
    optionValue = 0;
    optionName = "";
    optionTexture = NULL;
    optionanimation = NULL;
    subimageIndex = 0;
    isChecked = true;
    useGuiColor= false;
    sectionHasContent = true;
}

GPE_SelectBoxBasic_Option::~GPE_SelectBoxBasic_Option()
{

}


void GPE_SelectBoxBasic_Option::prerender_self(  )
{

}

GPE_SelectBoxBasic::GPE_SelectBoxBasic( std::string name)
{
    upDelay = 0;
    downDelay = 0;
    guiListTypeName = "selectbox";
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    elementBox.h = GENERAL_GPE_GUI_PADDING;
    maxHeight = -1;
    optionHeight = -1;
    opName = name;
    intedPos = 0;
    pos = 0;
    startPos = 0;
    maxOptionsInView = 10;
    optionHeight = GPE_TITLE_BPADDING+GENERAL_GPE_GUI_PADDING*2;
    optionIconWidth = GPE_AVERAGE_LINE_HEIGHT;
    showCheckboxes = false;
    showHideOthersCheckboxToggle = true;
    optionsScroller = new GPE_ScrollBar_YAxis();
}

GPE_SelectBoxBasic::~GPE_SelectBoxBasic()
{
    GPE_SelectBoxBasic_Option * tOption = NULL;
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

void GPE_SelectBoxBasic::correct_camera()
{
    maxOptionsInView = floor( (float)elementBox.h / (float)optionHeight );
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

    optionsScroller->update_box(  elementBox.x+elementBox.w-16, elementBox.y, 16,elementBox.h);
    update_rectangle(&optionsScroller->fullRect, 0, 0, 0,(float)subOptions.size() );
    update_rectangle(&optionsScroller->contextRect, 0, (float)startPos,0, (float)maxOptionsInView );
}

std::string GPE_SelectBoxBasic::get_data()
{
    return "";
}

void GPE_SelectBoxBasic::load_data(std::string dataString)
{

}


void GPE_SelectBoxBasic::add_option(std::string newOptionName, float newOpValue,GPE_Texture_Base * evRepIcon, GPE_Animation * evRepanimation, int subimageInIndex, bool selectNew, bool useGuiColor )
{
    if( (int)newOptionName.size() > 0 || evRepIcon!=NULL || evRepanimation!=NULL )
    {
        GPE_SelectBoxBasic_Option * newOption = new GPE_SelectBoxBasic_Option();
        newOption->optionName = newOptionName;
        newOption->optionValue = newOpValue;
        newOption->optionanimation = evRepanimation;
        newOption->optionTexture = evRepIcon;
        newOption->useGuiColor = useGuiColor;
        //newOption->optionNameTexture->loadFromRenderedText(GPE_MAIN_RENDERER,newOptionName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_TEXTINPUT);

        newOption->subimageIndex = subimageInIndex;

        subOptions.push_back(newOption);
        if( maxHeight < 0)
        {
            elementBox.h+=optionHeight;
        }
        if( selectNew)
        {
            set_selection( (int)subOptions.size() - 1 );
        }
        startPos++;
    }
}

void GPE_SelectBoxBasic::clear_list()
{
    GPE_SelectBoxBasic_Option * tempOption = NULL;
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

int GPE_SelectBoxBasic::get_option_id(std::string optionName )
{
    GPE_SelectBoxBasic_Option * tempOption = NULL;
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

GPE_SelectBoxBasic_Option * GPE_SelectBoxBasic::get_option(int optionId)
{
    if( optionId >=0 && optionId < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tOption = subOptions.at(optionId);
        if( tOption!=NULL)
        {
            return tOption;
        }
    }
    return NULL;
}

std::string GPE_SelectBoxBasic::get_option_name(int optionId)
{
    if( optionId >=0 && optionId < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tOption = subOptions.at(optionId);
        if( tOption!=NULL)
        {
            return tOption->optionName;
        }
    }
    return "";
}

std::string GPE_SelectBoxBasic::get_selected_name()
{
    if( pos >=0 && pos < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * nOption = subOptions[pos];
        if( nOption!=NULL)
        {
            return nOption->optionName;
        }
    }
    return "";
}

float GPE_SelectBoxBasic::get_selected_value()
{
    if( pos >=0 && pos < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * nOption = subOptions[pos];
        if( nOption!=NULL)
        {
            return nOption->optionValue;
        }
    }
    return 0;
}

int GPE_SelectBoxBasic::get_selection()
{
    return pos;
}

int GPE_SelectBoxBasic::get_size()
{
    return (int)subOptions.size();
}

void GPE_SelectBoxBasic::insert_option(int optionId, std::string newOptionName, GPE_Texture_Base * evRepIcon, GPE_Animation * evRepanimation, int subimageInIndex, bool selectNew )
{
    if( (int)newOptionName.size() > 0 || evRepIcon!=NULL || evRepanimation!=NULL )
    {
        GPE_SelectBoxBasic_Option * newOption = new GPE_SelectBoxBasic_Option();
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
            elementBox.h+=optionHeight;
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

void GPE_SelectBoxBasic::limit_height(int newH)
{
    if( newH >=0)
    {
        maxHeight = newH;
        elementBox.h = newH;
    }
    else
    {
        maxHeight = -1;
    }
    maxOptionsInView = floor( (float)elementBox.h / (float)optionHeight );
}

void GPE_SelectBoxBasic::alter_content( int optionId, bool sectionHasContent)
{
    if( optionId >=0 && optionId < (int)subOptions.size() )
    {
        if( subOptions[optionId]!=NULL)
        {
            subOptions[optionId]->sectionHasContent = sectionHasContent;
        }
    }
}

void GPE_SelectBoxBasic::alter_content_from_value( float valueId, bool sectionHasContent)
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

bool GPE_SelectBoxBasic::move_down_space()
{
    if( pos +1 < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tempVar = subOptions[pos+1];
        subOptions[pos+1] = subOptions[pos];
        subOptions[pos] = tempVar;
        pos++;
        return true;
    }
    return false;
}

bool GPE_SelectBoxBasic::move_up_space()
{
    if( pos > 0 && (int)subOptions.size() > pos-1)
    {
        GPE_SelectBoxBasic_Option * tempVar = subOptions[pos-1];
        subOptions[pos-1] = subOptions[pos];
        subOptions[pos] = tempVar;
        pos--;
        return true;
    }
    return false;
}

void GPE_SelectBoxBasic::prerender_self(  )
{
    GPE_SelectBoxBasic_Option * tOption = NULL;
    for( int i = 0; i < (int)subOptions.size() && i < (int)subOptions.size(); i++)
    {
        tOption = subOptions[i];
        if( tOption!=NULL)
        {
            tOption->prerender_self( );
        }
    }

}

void GPE_SelectBoxBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);

    correct_camera();

    //only works if scrollbar is actually needed
    if( maxOptionsInView <= (int)subOptions.size() )
    {
        optionsScroller->process_self( viewedSpace, cam );
        startPos = (float)(optionsScroller->contextRect.y);
    }

    bool scrollHappened = false;
    if( input->check_keyboard_pressed(kb_esc) )
    {
        hasArrowkeyControl = false;
        hasScrollControl = false;
        isInUse = false;
    }

    if( isInUse)
    {
        hasArrowkeyControl = true;
        hasScrollControl = true;

        if( input->released[kb_up] )
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
        else if( input->released[kb_down] )
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
        else if( input->check_keyboard_released(kb_space ) && showCheckboxes)
        {
            if( pos >=0 && pos < (int)subOptions.size() )
            {
                GPE_SelectBoxBasic_Option *  optionToChange = subOptions[pos];
                optionToChange->isChecked = !optionToChange->isChecked;
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
        if( input->mouseScrollingUp > 0)
        {
            startPos--;
            scrollHappened = true;
        }
        if( input->mouseScrollingDown > 0)
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

    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isClicked )
    {
        isInUse = true;
        int checkBoxWidth = optionHeight+GENERAL_GPE_GUI_PADDING;
        int checkBoxHeight = optionHeight;
        if( showCheckboxes==false)
        {
            checkBoxWidth = 0;
            checkBoxHeight = 0;
        }
        //if( input->check_mouse_released( mb_left))
        {
            int relativeOptionX = viewedSpace->x+elementBox.x-cam->x;
            int relativeOptionY = viewedSpace->y+elementBox.y-cam->y;
            for( int i = 0; i <= maxOptionsInView && i < (int)subOptions.size(); i++)
            {
                if( point_between(input->mouse_x,input->mouse_y,relativeOptionX,relativeOptionY+(i)*optionHeight,
                                  relativeOptionX+elementBox.w,relativeOptionY+(i+1)*optionHeight) )
                {
                    if( showCheckboxes)
                    {
                        if( point_between(input->mouse_x,input->mouse_y,relativeOptionX,relativeOptionY+(i)*optionHeight,
                                          relativeOptionX+checkBoxWidth,relativeOptionY+(i+1)*optionHeight) )
                        {
                            GPE_SelectBoxBasic_Option * tOp = NULL;
                            int tPos = i+startPos;
                            if( tPos >=0 && tPos < (int)subOptions.size() )
                            {
                                tOp = subOptions.at(tPos);
                                if( tOp!=NULL)
                                {
                                    tOp->isChecked = !tOp->isChecked;
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
    //GPE_Report("Success on select box...");
}

void GPE_SelectBoxBasic::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && viewedSpace!=NULL )
    {
        intedPos = (int)pos;
        int relativeOptionX = elementBox.x-cam->x;
        int relativeOptionY = elementBox.y-cam->y;
        int foundIconWidth = 0;
        if( optionIconWidth > 0 )
        {
            foundIconWidth = std::max( optionIconWidth,optionHeight )*3/4;
        }
        else
        {
            foundIconWidth = 0;
        }
        gcanvas->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+elementBox.w,relativeOptionY+elementBox.h,GPE_MAIN_THEME->PopUp_Box_Color, false);
        maxOptionsInView = elementBox.h / optionHeight;
        int iStartPos = (int)startPos;

        if( pos>=iStartPos && pos < iStartPos+maxOptionsInView )
        {
            if( (int)subOptions.size() > 0)
            {
                gcanvas->render_rectangle( relativeOptionX,relativeOptionY+(pos-iStartPos)*optionHeight,relativeOptionX+elementBox.w,relativeOptionY+(pos-iStartPos+1)*optionHeight,GPE_MAIN_THEME->Button_Border_Color,false);
            }
            else if( pos > 0)
            {
                gcanvas->render_rectangle( relativeOptionX,relativeOptionY+(pos-iStartPos)*optionHeight,relativeOptionX+elementBox.w,relativeOptionY+(pos-iStartPos+1)*optionHeight,GPE_MAIN_THEME->Button_Border_Color,false);
            }
        }
        else if( pos >= startPos+maxOptionsInView )
        {
            //pos = startPos+maxOptionsInView -1;
        }
        GPE_SelectBoxBasic_Option * tOption = NULL;
        int checkBoxWidth = optionHeight+GENERAL_GPE_GUI_PADDING;
        int checkBoxHeight = optionHeight;
        if( showCheckboxes==false)
        {
            checkBoxWidth = 0;
            checkBoxHeight = 0;
        }

        GPE_Color * fontRenderColor = GPE_MAIN_THEME->Main_Box_Faded_Font_Color;
        for( int i = iStartPos; i <= (int)(iStartPos+maxOptionsInView) &&  i < (int)subOptions.size(); i++)
        {
            tOption = subOptions.at( i );
            if( tOption!=NULL)
            {
                if( !tOption->sectionHasContent && intedPos!=i)
                {
                    fontRenderColor = GPE_MAIN_THEME->PopUp_Box_Font_Color;
                    gcanvas->render_rectangle( relativeOptionX,relativeOptionY+(i-iStartPos)*optionHeight,relativeOptionX+elementBox.w,relativeOptionY+(i-iStartPos+1)*optionHeight,GPE_MAIN_THEME->Program_Header_Color,false,228);
                }
                else if( intedPos ==i )
                {
                    fontRenderColor = GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color;
                }
                else
                {
                    fontRenderColor = GPE_MAIN_THEME->PopUp_Box_Font_Color;
                }

                if( showCheckboxes)
                {
                    gcanvas->render_rectangle( relativeOptionX+relativeOptionX+GENERAL_GPE_GUI_PADDING+optionHeight/8,relativeOptionY+1+(i-iStartPos)*optionHeight+optionHeight/8,relativeOptionX+relativeOptionX+GENERAL_GPE_GUI_PADDING+optionHeight/2+optionHeight/8,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight*5/8,GPE_MAIN_THEME->Button_Box_Color, false);
                    if( tOption->isChecked && GPE_CHECKMARK_IMAGE!=NULL )
                    {
                        GPE_CHECKMARK_IMAGE->render_tex_resized( relativeOptionX+relativeOptionX+GENERAL_GPE_GUI_PADDING+optionHeight/8,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight/8,optionHeight/2,optionHeight/2,NULL,GPE_MAIN_THEME->Checkbox_Color );
                    }
                    //gcanvas->render_rectangle( relativeOptionX+relativeOptionX+GENERAL_GPE_GUI_PADDING,relativeOptionY+1+(i-iStartPos)*optionHeight,relativeOptionX+relativeOptionX+GENERAL_GPE_GUI_PADDING+optionHeight,relativeOptionY+(i-iStartPos+1)*optionHeight,GPE_MAIN_THEME->Button_Box_Selected_Color, true);
                    gcanvas->render_rectangle( relativeOptionX+relativeOptionX+GENERAL_GPE_GUI_PADDING+optionHeight/8,relativeOptionY+1+(i-iStartPos)*optionHeight+optionHeight/8,relativeOptionX+relativeOptionX+GENERAL_GPE_GUI_PADDING+optionHeight/2+optionHeight/8,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight*5/8,GPE_MAIN_THEME->Button_Box_Selected_Color, true);

                    gfs->render_text( relativeOptionX+GENERAL_GPE_GUI_PADDING+checkBoxWidth,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight/2,tOption->optionName,fontRenderColor,FONT_TEXTINPUT,FA_LEFT,FA_CENTER,255);
                }
                else
                {
                    gfs->render_text( relativeOptionX+foundIconWidth+GENERAL_GPE_GUI_PADDING*2,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight/2,tOption->optionName,fontRenderColor,FONT_LABEL,FA_LEFT,FA_MIDDLE,255);
                }
                if( tOption->optionTexture!=NULL)
                {
                    if( tOption->useGuiColor)
                    {
                        tOption->optionTexture->render_align_resized( relativeOptionX+GENERAL_GPE_GUI_PADDING,relativeOptionY+(i-iStartPos)*optionHeight+(optionHeight)/2,foundIconWidth,foundIconWidth,FA_LEFT,FA_MIDDLE, NULL,GPE_MAIN_THEME->PopUp_Box_Font_Color );
                    }
                    else
                    {
                        tOption->optionTexture->render_align_resized( relativeOptionX+GENERAL_GPE_GUI_PADDING,relativeOptionY+(i-iStartPos)*optionHeight+(optionHeight)/2,foundIconWidth,foundIconWidth, FA_LEFT, FA_MIDDLE, NULL );
                    }
                }
                else if( tOption->optionanimation!=NULL)
                {
                    if( tOption->useGuiColor)
                    {
                        //tOption->optionanimation->render_special( tOption->subimageIndex,relativeOptionX+GENERAL_GPE_GUI_PADDING+checkBoxWidth,relativeOptionY+(i-iStartPos)*optionHeight +(optionHeight-foundIconWidth)/2,foundIconWidth,foundIconWidth,GPE_MAIN_THEME->Main_Box_Font_Color,cam);
                    }
                    else
                    {
                        //tOption->optionanimation->render_resized( tOption->subimageIndex,relativeOptionX+GENERAL_GPE_GUI_PADDING+checkBoxWidth,relativeOptionY+(i-iStartPos)*optionHeight +(optionHeight-foundIconWidth)/2,foundIconWidth,foundIconWidth,cam);
                    }
                }
            }
        }

        if( iStartPos > 0 )
        {
            gcanvas->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+elementBox.w,relativeOptionY+4,GPE_MAIN_THEME->Button_Box_Selected_Color,false,228);
        }
        else if( iStartPos+maxOptionsInView < (int)subOptions.size() )
        {
            gcanvas->render_rectangle( relativeOptionX,relativeOptionY+elementBox.h-4,relativeOptionX+elementBox.w,relativeOptionY+elementBox.h,GPE_MAIN_THEME->Button_Box_Selected_Color,false,228);
        }
        if( isInUse)
        {
            gcanvas->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+elementBox.w,relativeOptionY+elementBox.h,GPE_MAIN_THEME->Button_Box_Selected_Color, true);
        }
        else
        {
            gcanvas->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+elementBox.w,relativeOptionY+elementBox.h,GPE_MAIN_THEME->Main_Border_Color, true );
        }

        if( maxOptionsInView < (int)subOptions.size() )
        {
            optionsScroller->render_self( viewedSpace, cam );
        }
    }
}

void GPE_SelectBoxBasic::rename_option(int optionId, std::string newName)
{
    if( optionId >=0 && optionId < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tOption = subOptions.at(optionId);
        if(tOption!=NULL )
        {
            tOption->optionName = newName;
        }
    }
}

void GPE_SelectBoxBasic::remove_option(int optionId)
{
    if( optionId>=0 && optionId < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tOption = subOptions.at(optionId);
        if(tOption!=NULL )
        {
            delete tOption;
            tOption = NULL;
        }
        subOptions.erase(subOptions.begin() + optionId);
        set_selection(pos);
    }
}

void GPE_SelectBoxBasic::set_option_height( int newOptionHeight)
{
    if( newOptionHeight > 0)
    {
        optionHeight = newOptionHeight;
    }
    maxOptionsInView = floor( (float)elementBox.h / (float)optionHeight );
}

void GPE_SelectBoxBasic::set_selected_option( std::string optionToSelect)
{
    GPE_SelectBoxBasic_Option * tempOption = NULL;
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

void GPE_SelectBoxBasic::set_selection( int selId )
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

void GPE_SelectBoxBasic::toggle_others_checked()
{
    showHideOthersCheckboxToggle = !showHideOthersCheckboxToggle;
    GPE_SelectBoxBasic_Option * tOption = NULL;
    for(int i = 0; i < (int)subOptions.size(); i++)
    {
        if( i!=pos)
        {
            tOption = subOptions[i];
            if( tOption!=NULL)
            {
                tOption->isChecked = showHideOthersCheckboxToggle;
            }
        }
    }
}
