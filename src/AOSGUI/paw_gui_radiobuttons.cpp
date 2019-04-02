/*
paw_gui_radiobuttons.cpp
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


#include "paw_gui_radiobuttons.h"


GPE_RadioButtonControllerBasic::GPE_RadioButtonControllerBasic(std::string cName, bool alphabetize, int colNum)
{
    guiListTypeName = "radio";
    guiListTypeId = 5;
    hoveredOption = -1;
    selectedId = 0;
    opName = cName;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 0;
    elementBox.h = 24;
    opWidth = elementBox.w;
    opHeight = elementBox.h;

    if( (int)opName.size()>0)
    {
        int textW = 0;
        int textH = 0;
        GPE_DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        opWidth = textW;
        if( elementBox.w < textW )
        {
            elementBox.w = textW+GENERAL_GPE_PADDING*2;
        }
    }
    allowDuplicates = false;
    keepAlphabetized = alphabetize;
    numCols = std::max(0,colNum);
    rowsPerCol = -1;
}

GPE_RadioButtonControllerBasic::~GPE_RadioButtonControllerBasic()
{
    GPE_KeyPair * kp = NULL;
    for( int i = subOptions.size()-1; i >=0; i--)
    {
        kp = subOptions[i];
        if( kp!=NULL )
        {
            delete kp;
            kp = NULL;
        }
    }
    subOptions.clear();
}


std::string GPE_RadioButtonControllerBasic::get_data()
{
    std::string dataString = guiListTypeName+":"+opName+"==|||==[menu]";
    GPE_KeyPair * tPair = NULL;
    for( int i = 0; i < (int)subOptions.size(); i++ )
    {
        tPair = subOptions[i];
        if( tPair!=NULL)
        {
            dataString+="[option]"+tPair->keyString+":"+tPair->keySubString+":"+int_to_string(tPair->keyValue)+"[/option]";
        }
    }
    dataString+="[/menu]"+int_to_string(selectedId)+",";
    return dataString;
}

void GPE_RadioButtonControllerBasic::load_data(std::string dataString)
{
    if( (int)dataString.size() > 0 )
    {
        std::string optionTag = "[option]";
        int optionTagSize = (int)optionTag.size();

        std::string allOptionsString = "";
        std::string newOptionData = "";
        std::string newOptionString = "";
        std::string newOptionName = "";
        std::string newOptionSubame = "";
        int newOptionId = -1;
        int beginOptionPos=dataString.find_first_of(optionTag);
        if(beginOptionPos!=(int)std::string::npos)
        {
            allOptionsString = split_first_string(dataString,"[/menu]");

            newOptionData = split_first_string(allOptionsString,"[menu]");
            while( (int)allOptionsString.size() > 0)
            {
                newOptionString = split_first_string(allOptionsString,"[/option]");

                beginOptionPos=newOptionString.find_first_of(optionTag);
                if(beginOptionPos!=(int)std::string::npos)
                {
                    //if the beginOptionPos is present, then parse on through and carryon
                    if( (int)newOptionString.size() > beginOptionPos+optionTagSize )
                    {
                        newOptionData = get_substring(newOptionString, beginOptionPos+optionTagSize );

                        newOptionName = split_first_string(newOptionData,":");
                        newOptionSubame = split_first_string(newOptionData,":");
                        newOptionId = string_to_int(newOptionData,-1);
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
            set_selection( split_first_int(dataString,',') );
            //showJustOptions = string_to_bool( split_first_string(dataString,",") );
        }
    }
}

void GPE_RadioButtonControllerBasic::remove_data(std::string dataString)
{
    if( (int)dataString.size() > 0)
    {
        std::string optionTag = "[option]";
        int optionTagSize = (int)optionTag.size();

        std::string newOptionData = "";
        std::string newOptionString = "";
        std::string foundOptionName = "";
        int beginOptionPos = -1;
        while( (int)dataString.size() > 0)
        {
            newOptionString = split_first_string(dataString,"[/option]");

            beginOptionPos=newOptionString.find_first_of(optionTag);
            if(beginOptionPos!=(int)std::string::npos)
            {
                //if the beginOptionPos is present, then parse on through and carryon
                foundOptionName = newOptionString.substr(beginOptionPos+optionTagSize,newOptionString.length());
                if( (int)foundOptionName.size() > 0)
                {
                    remove_option(foundOptionName);
                }
            }
        }
    }
}

void GPE_RadioButtonControllerBasic::add_opton(std::string newOption)
{
    if( (int)newOption.size()>0 )
    {
        int textW = 0;
        int textH = 0;
        GPE_DEFAULT_FONT->get_metrics(newOption,&textW, &textH);
        if( opWidth < textW )
        {
            opWidth = textW;
            elementBox.w = textW+GENERAL_GPE_PADDING*2;
            //newTex->loadFromRenderedText(MAIN_RENDERER,newOption,GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT);
        }
        GPE_KeyPair * kp = NULL;
        GPE_KeyPair * newOptionPair = new GPE_KeyPair(-1,newOption,newOption);
        if(keepAlphabetized)
        {
            std::string fString = "";
            bool isInserted = false;
            if( rowsPerCol>0)
                for(int i=0; i< (int)subOptions.size() && !isInserted; i++)
                {
                    kp = subOptions[i];
                    if( kp!=NULL)
                    {
                        fString = kp->keyString;
                        if( fString > newOption && !isInserted )
                        {
                            subOptions.insert( subOptions.begin()+i,newOptionPair);
                            isInserted = true;
                            organize_options();
                            return;
                        }
                    }
                }
        }
        subOptions.push_back(newOptionPair);
        elementBox.h +=32;
        organize_options();
    }
}

void GPE_RadioButtonControllerBasic::add_menu_option(std::string optionName, std::string optionSubStr,int optionValue, bool selectOption)
{
    if( (int)optionName.size()>0 )
    {
        int textW = 0;
        int textH = 0;
        GPE_DEFAULT_FONT->get_metrics(optionName,&textW, &textH);
        if( opWidth < textW )
        {
            opWidth = textW;
            elementBox.w = textW+GENERAL_GPE_PADDING*2;
            //newTex->loadFromRenderedText(MAIN_RENDERER,optionName,GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT);
        }
        bool optionExists = false;
        GPE_KeyPair * tOption = NULL;
        for( int i = 0; i < (int)subOptions.size(); i++)
        {
            tOption = subOptions[i];
            if( tOption!=NULL )
            {
                if( tOption->keyString==optionName)
                {
                    optionExists = false;
                    return;
                }
            }
        }
        if( !optionExists)
        {
            GPE_KeyPair * kp = new GPE_KeyPair(optionValue,optionName,optionSubStr);
            subOptions.push_back(kp);
            if( selectOption )
            {
                set_selection( (int)subOptions.size()-1 );
            }
            elementBox.h +=32;
            organize_options();
        }
    }
}

std::string GPE_RadioButtonControllerBasic::get_menu_option(int atNumb)
{
    GPE_KeyPair * kp = NULL;
    if( atNumb >=0 && atNumb < (int)subOptions.size() )
    {
        kp = subOptions.at(atNumb);
        if( kp!=NULL)
        {
            return kp->keyString;
        }
    }
    return "";
}

std::string GPE_RadioButtonControllerBasic::get_plain_string()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        if( kp!=NULL)
        {
            return "'"+kp->keyString+"'";
        }
    }
    return "''";
}

int GPE_RadioButtonControllerBasic::get_selected_id()
{
    return selectedId;
}

std::string GPE_RadioButtonControllerBasic::get_selected_tag()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keySubString;
    }
    return "";
}

int GPE_RadioButtonControllerBasic::get_selected_value()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keyValue;
    }
    return 0;
}

void GPE_RadioButtonControllerBasic::organize_options()
{
    elementBox.w = GENERAL_GPE_PADDING*2;
    elementBox.h = 24;
    int maxWidthText = 0;
    int textW = 0;
    int textH = 0;
    if( (int)opName.size()>0)
    {
        textW = 0;
        textH = 0;
        GPE_DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        opWidth = textW;
        maxWidthText = textW;
        elementBox.w += maxWidthText;
    }

    std::string opStr = "";
    GPE_KeyPair * tPair = NULL;
    if( numCols<=1)
    {
        for(int i =0; i < (int)subOptions.size(); i++)
        {
            tPair = subOptions[i];
            if( tPair!=NULL)
            {
                opStr = tPair->keyString;
                if( (int)opStr.size() > 0)
                {
                    textW = 0;
                    textH = 0;
                    GPE_DEFAULT_FONT->get_metrics(opStr,&textW, &textH);
                    if( maxWidthText < textW )
                    {
                        maxWidthText = textW;
                    }
                }
                elementBox.h+=32;
            }
        }
        elementBox.w+=maxWidthText;
    }
}

void GPE_RadioButtonControllerBasic::remove_option(std::string optionToCut)
{
    if( (int)subOptions.size()>0 )
    {
        int removePos = -1;
        GPE_KeyPair * kp = NULL;
        for(int i=(int)subOptions.size()-1; i>=0 && removePos < 0; i--)
        {
            kp = subOptions[i];
            if( kp!=NULL)
            {
                if(  kp->keyString == optionToCut)
                {
                    delete kp;
                    kp = NULL;
                    subOptions.erase(subOptions.begin()+i);
                    if( i==selectedId)
                    {
                        i = -1;
                    }
                    else if( selectedId > i)
                    {
                        selectedId-=1;
                    }
                    removePos = i;
                    elementBox.h -=32;
                }
            }
        }
    }
}

void GPE_RadioButtonControllerBasic::remove_option_id(int optionToCut)
{
    if( (int)subOptions.size()>0 )
    {
        if(optionToCut >=0 && optionToCut < (int)subOptions.size() )
        {
            GPE_KeyPair * kp = subOptions.at( optionToCut );
            if( kp!=NULL)
            {
                delete kp;
                kp = NULL;
            }
            subOptions.erase(subOptions.begin()+optionToCut);
            if( optionToCut==selectedId)
            {
                optionToCut = -1;
            }
            else if( selectedId > optionToCut)
            {
                selectedId-=1;
            }
            elementBox.h -=32;
        }
    }
}

void GPE_RadioButtonControllerBasic::set_selection(int newSelection)
{
    if( newSelection>=0 && newSelection < (int)subOptions.size() )
    {
        selectedId = newSelection;
    }
}

bool GPE_RadioButtonControllerBasic::set_from_tag(std::string newSelectedTag)
{
    GPE_KeyPair * tempPair = NULL;
    for( int i = (int)(subOptions.size())-1; i >=0; i--)
    {
        tempPair = subOptions[i];
        if( tempPair!=NULL)
        {
            if( tempPair->keySubString == newSelectedTag)
            {
                selectedId = i;
                return true;
            }
        }
    }
    return false;
}

void GPE_RadioButtonControllerBasic::set_value(int valueToSelect)
{
    GPE_KeyPair * tempPair = NULL;
    for( int i = (int)subOptions.size()-1; i>=0; i--)
    {
        tempPair = subOptions[i];
        if( tempPair!=NULL)
        {
            if( tempPair->keyValue==valueToSelect)
            {
                selectedId = i;
                break;
            }
        }
    }
}

void GPE_RadioButtonControllerBasic::prerender_self(  )
{

}

void GPE_RadioButtonControllerBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    hoveredOption = -1;
    if( cam!=NULL && viewedSpace!=NULL)
    {
        int buttonXPos = elementBox.x+viewedSpace->x+16-cam->x;
        int buttonYPos = elementBox.y+viewedSpace->y+32-cam->y;
        GPE_GeneralGuiElement::process_self(viewedSpace,cam);
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
            for(int i = 0; i  < (int)subOptions.size(); i++)
            {
                if(point_between( input->mouse_x,input->mouse_y,buttonXPos,buttonYPos,buttonXPos+32,buttonYPos+32) )
                {
                    hoveredOption = i;
                    if( input->check_mouse_released( mb_left))
                    {
                        selectedId = i;
                        selectionClicked = true;
                    }
                }
                buttonYPos+=32;
            }
            if( hasArrowkeyControl && isInUse )
            {
                if( input->check_keyboard_down(kb_left) || input->check_keyboard_down(kb_up) )
                {
                    selectedId--;
                }
                if( input->check_keyboard_down(kb_right) || input->check_keyboard_down(kb_down) )
                {
                    selectedId++;
                }
                if( selectedId >=(int)subOptions.size() )
                {
                    selectedId =(int)subOptions.size()-1;
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

void GPE_RadioButtonControllerBasic::render_self(  GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    if( forceRedraw )
    {
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if( viewedSpace!=NULL && cam!=NULL)
        {
            int buttonXPos = elementBox.x-cam->x;
            int buttonYPos = elementBox.y-cam->y;
            if(showBorderBox)
            {
                if((int)opName.size()>0 )
                {
                    gcanvas->render_line_color( buttonXPos,buttonYPos+GPE_TITLE_BPADDING,buttonXPos+GPE_TITLE_BPADDING,buttonYPos+GPE_TITLE_BPADDING,GPE_MAIN_THEME->Button_Box_Color,255);
                    gcanvas->render_line_color( buttonXPos+GPE_TITLE_BPADDING*2+opWidth,buttonYPos+GPE_TITLE_BPADDING,
                                      buttonXPos+elementBox.w,buttonYPos+GPE_TITLE_BPADDING,GPE_MAIN_THEME->Button_Box_Color,255);

                    gcanvas->render_line_color( buttonXPos,buttonYPos+GPE_TITLE_BPADDING,buttonXPos,buttonYPos+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,255);
                    gcanvas->render_line_color( buttonXPos,buttonYPos+elementBox.h,buttonXPos+elementBox.w,buttonYPos+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,255);
                    gcanvas->render_line_color( buttonXPos+elementBox.w,buttonYPos+GPE_TITLE_BPADDING,buttonXPos+elementBox.w,buttonYPos+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,255);
                    gfs->render_text(  buttonXPos+GPE_TITLE_BPADDING*1.5,buttonYPos+GPE_TITLE_BPADDING/2,opName,GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP,255);
                }
                else
                {
                    gcanvas->render_rectangle( buttonXPos,buttonYPos,buttonXPos+elementBox.w,buttonYPos+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,255);
                }
                buttonXPos += 16;
                buttonYPos += 32;
                int renderedInCol = 0;

                GPE_KeyPair * kp = NULL;
                for(int i = 0; i  < (int)subOptions.size(); i++)
                {
                    if( i == hoveredOption )
                    {
                        gcanvas->render_circle_color( buttonXPos+16,buttonYPos+8,10, GPE_MAIN_THEME->Button_Border_Highlighted_Color, 255 );
                    }
                    gcanvas->render_circle_color( buttonXPos+16,buttonYPos+8,8, GPE_MAIN_THEME->Button_Box_Color, 255 );

                    if( i == selectedId )
                    {
                        gcanvas->render_circle_color( buttonXPos+16,buttonYPos+8,4, GPE_MAIN_THEME->Button_Box_Selected_Color, 255 );
                    }
                    kp = subOptions[i];
                    if( kp!=NULL)
                    {
                        gfs->render_text(  buttonXPos+32,buttonYPos,kp->keyString,GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP,255);
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
                            buttonYPos = elementBox.y+32;
                        }
                    }
                    */
                }
            }
            if( isInUse)
            {
                gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
            }
        }
    }
}
