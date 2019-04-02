/*
paw_gui_dropdown.cpp
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

#include "paw_gui_dropdown.h"


GPE_DropDown_Menu::GPE_DropDown_Menu( std::string name, bool justOptions)
{
    guiListTypeName = "dropdown";
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    //int nameMinSize = name.size()*
    elementBox.h = 24;
    dropdownName = opName = name;
    opId = -1;
    selectedId = -1;
    isSelectable = true;
    showJustOptions = justOptions;
    if( showJustOptions)
    {
        opId = 0;
        selectedId = 0;
    }
    isOpen = false;
    isClicked = false;
    justActivated = false;
}

GPE_DropDown_Menu::~GPE_DropDown_Menu()
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

std::string GPE_DropDown_Menu::get_data()
{
    std::string dataString = guiListTypeName+":"+dropdownName+"==|||==[menu]";
    GPE_KeyPair * tPair = NULL;
    for( int i = 0; i < (int)subOptions.size(); i++ )
    {
        tPair = subOptions[i];
        if( tPair!=NULL)
        {
            dataString+="[option]"+tPair->keyString+":"+tPair->keySubString+":"+int_to_string(tPair->keyValue)+"[/option]";
        }
    }
    dataString+="[/menu]"+int_to_string(selectedId)+","+int_to_string(showJustOptions)+",";
    return dataString;
}


void GPE_DropDown_Menu::add_menu_option(std::string optionName, std::string optionSubStr,double optionValue, bool selectOption)
{
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
            set_id( (int)subOptions.size()-1 );
        }
    }
}

std::string GPE_DropDown_Menu::get_menu_option(int atNumb)
{
    GPE_KeyPair * kp = NULL;
    if( atNumb >=0 && atNumb < (int)subOptions.size() )
    {
        kp = subOptions.at(atNumb);
        return kp->keyString;
    }
    return "";
}

std::string GPE_DropDown_Menu::get_plain_string()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return "'"+kp->keyString+"'";
    }
    return "''";
}

int GPE_DropDown_Menu::get_selected_id()
{
    return selectedId;
}

std::string GPE_DropDown_Menu::get_selected_name()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keyString;
    }
    return opName;
}


std::string GPE_DropDown_Menu::get_selected_tag()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keySubString;
    }
    return "";
}

double GPE_DropDown_Menu::get_selected_value()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keyValue;
    }
    return -1;
}

bool GPE_DropDown_Menu::just_activated()
{
    return justActivated;
}

void GPE_DropDown_Menu::load_data(std::string dataString)
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
            set_id( split_first_int(dataString,',') );
            showJustOptions = string_to_bool( split_first_string(dataString,",") );
        }
    }
}

void GPE_DropDown_Menu::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    isHovered = false;
    justActivated = false;
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered || isInUse)
    {
        MAIN_OVERLAY->update_tooltip(dropdownName);
    }
    if( isInUse)
    {
        hasArrowkeyControl = true;
    }
    else
    {
        hasArrowkeyControl = false;
    }
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }

    if( isClicked && cam!=NULL && viewedSpace!=NULL)
    {
        if( MAIN_CONTEXT_MENU!=NULL)
        {
            if( MAIN_CONTEXT_MENU->subMenuIsOpen == false)
            {
                isOpen = true;
                GPE_open_context_menu(viewedSpace->x+elementBox.x-cam->x, viewedSpace->y+elementBox.y+elementBox.h-cam->y);
                MAIN_CONTEXT_MENU->set_width(elementBox.w);
                GPE_KeyPair * kp = NULL;
                if( (int)subOptions.size() > 0)
                {
                    if( showJustOptions ==false)
                    {
                        MAIN_CONTEXT_MENU->add_menu_option(dropdownName,-1);
                    }
                    for( int i = 0; i < (int)subOptions.size(); i++)
                    {
                        kp = subOptions[i];
                        MAIN_CONTEXT_MENU->add_menu_option(kp->keyString,i);
                    }
                    if( showJustOptions ==false)
                    {
                        MAIN_CONTEXT_MENU->add_menu_option(dropdownName,-1);
                    }
                }
                else
                {
                    MAIN_CONTEXT_MENU->add_menu_option(dropdownName,-1);
                }
                isOpen = false;
                isClicked = false;
                justActivated = true;
                int foundResult = GPE_Get_Context_Result();
                if( foundResult>=0)
                {
                    selectedId = foundResult;
                    opName = GPE_Action_Message;
                }
                else if(showJustOptions==false)
                {
                    selectedId=-1;
                    opName=dropdownName;
                }
                GPE_close_context_menu();
            }
        }
    }
    else if( hasArrowkeyControl)
    {
        if( input->check_keyboard_down(kb_up) && selectedId > 0)
        {
            selectedId--;
        }
        else if( input->check_keyboard_down(kb_down) && selectedId < (int)subOptions.size()-1)
        {
            selectedId++;
        }
    }
}

void GPE_DropDown_Menu::remove_data(std::string dataString)
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

void GPE_DropDown_Menu::remove_option(std::string optionToRemove)
{
    GPE_KeyPair * tOption = NULL;
    for( int i = (int)subOptions.size()-1; i>=0; i--)
    {
        tOption = subOptions[i];
        if( tOption!=NULL )
        {
            if( tOption->keyString==optionToRemove)
            {
                delete tOption;
                tOption = NULL;
                subOptions.erase( subOptions.begin()+i);

                if( selectedId==i)
                {
                    set_selection(-1);
                }
            }
        }
    }
}

void GPE_DropDown_Menu::reset_suboptions()
{
    GPE_KeyPair * tOption = NULL;
    for( int i = (int)subOptions.size()-1; i>=0; i--)
    {
        tOption = subOptions[i];
        if( tOption!=NULL )
        {
            delete tOption;
            tOption = NULL;
            subOptions.erase( subOptions.begin()+i);

            if( selectedId==i)
            {
                set_selection(-1);
            }
        }
    }
}

void GPE_DropDown_Menu::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && cam!=NULL && viewedSpace!=NULL)
    {
        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Input_Color,false);

        if( selectedId >= 0)
        {
            if( selectedId < (int)subOptions.size() )
            {
                GPE_KeyPair * kp = subOptions[selectedId];
                gfs->render_text_resized( elementBox.x+GENERAL_GPE_PADDING-cam->x,elementBox.y+GENERAL_GPE_PADDING-cam->y,kp->keyString,GPE_MAIN_THEME->Input_Font_Color,FONT_POPUP,FA_LEFT,FA_TOP,elementBox.w-elementBox.h-12,-1);
            }
            else
            {
                gfs->render_text_resized( elementBox.x+GENERAL_GPE_PADDING-cam->x,elementBox.y+GENERAL_GPE_PADDING-cam->y,opName,GPE_MAIN_THEME->Input_Font_Color,FONT_POPUP,FA_LEFT,FA_TOP,elementBox.w-elementBox.h-12,-1);
                selectedId = 0;
            }
        }
        else
        {
            gfs->render_text_resized( elementBox.x+GENERAL_GPE_PADDING-cam->x,elementBox.y+GENERAL_GPE_PADDING-cam->y,opName,GPE_MAIN_THEME->Input_Font_Color,FONT_POPUP,FA_LEFT,FA_TOP,elementBox.w-elementBox.h-12,-1);
        }

        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Input_Highlight_Outline_Color,true);
        }
        else if( isHovered)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Input_Highlight_Alt_Color,true);
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Input_Outline_Color,true);
        }
        if( GPE_DROPDOWN_ARROW!=NULL)
        {
            GPE_DROPDOWN_ARROW->render_tex_resized( elementBox.x+elementBox.w-elementBox.h-cam->x,elementBox.y - cam->y,elementBox.h,elementBox.h, NULL, GPE_MAIN_THEME->Input_Font_Color );
        }
    }
}

void GPE_DropDown_Menu::set_id(int newId)
{
    set_selection(newId);
}

void GPE_DropDown_Menu::set_option(std::string newSelectedOptionName )
{
    GPE_KeyPair * tOption = NULL;
    for( int i = (int)subOptions.size()-1; i>=0; i--)
    {
        tOption = subOptions[i];
        if( tOption!=NULL )
        {
            if( tOption->keyString==newSelectedOptionName)
            {
                set_selection(  i );
            }
        }
    }
}

void GPE_DropDown_Menu::set_option_subvalue(std::string newSelectedOptionName )
{
    GPE_KeyPair * tOption = NULL;
    for( int i = (int)subOptions.size()-1; i>=0; i--)
    {
        tOption = subOptions[i];
        if( tOption!=NULL )
        {
            if( tOption->keySubString==newSelectedOptionName)
            {
                set_selection(  i );
            }
        }
    }
}

void GPE_DropDown_Menu::set_selection(int newId, bool autoCorrect)
{
    if( newId>=0)
    {
        if( newId < (int)subOptions.size() )
        {
            GPE_KeyPair * kp = subOptions.at(newId);
            selectedId = newId;
            opName = kp->keyString;
        }
        else if( autoCorrect && (int)subOptions.size() >0 )
        {
            selectedId = (int)subOptions.size()-1;
            GPE_KeyPair * kp = subOptions.at(selectedId);
            opName = kp->keyString;
        }
        else
        {
            selectedId = -1;
            opName = dropdownName;
        }
    }
    else if( autoCorrect && (int)subOptions.size() >0 )
    {
        selectedId = 0;
        GPE_KeyPair * kp = subOptions.at(selectedId);
        opName = kp->keyString;
    }
    else
    {
        selectedId = -1;
        opName = dropdownName;
    }
}

void GPE_DropDown_Menu::set_name(std::string newName)
{
    dropdownName = opName = newName;
}

void GPE_DropDown_Menu::set_value(int valueToFind )
{
    GPE_KeyPair * kp = NULL;
    for( int i =(int)subOptions.size()-1; i>=0; i--)
    {
        kp = subOptions[i];
        if( kp!=NULL)
        {
            if( kp->keyValue==valueToFind )
            {
                selectedId = i;
                opName = kp->keyString;
                break;
            }
        }
    }
}

void GPE_DropDown_Menu::show_just_options(bool justOptions)
{
    showJustOptions = justOptions;
}
