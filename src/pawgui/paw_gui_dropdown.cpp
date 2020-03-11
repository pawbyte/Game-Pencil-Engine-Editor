/*
paw_gui_dropdown.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "paw_gui_dropdown.h"


GPE_DropDown_Menu::GPE_DropDown_Menu( std::string name, bool justOptions)
{
    guiListTypeName = "dropdown";
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = 192;
    //int nameMinSize = name.size()*
    widget_box.h = 32;
    dropdownName = opName = name;
    opId = -1;
    selectedId = -1;
    selectedPair = NULL;
    selectedValue = -1;
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
    dropDownParentPair = new gpe::key_pair( -1,"" );
}

GPE_DropDown_Menu::~GPE_DropDown_Menu()
{
    clear_dropdown();
    if( dropDownParentPair!=NULL )
    {
        delete dropDownParentPair;
        dropDownParentPair = NULL;
    }
}

std::string GPE_DropDown_Menu::get_data()
{
    std::string dataString = guiListTypeName+":"+dropdownName+"==|||==[menu]";
    gpe::key_pair * tPair = NULL;
    for( int i = 0; i < (int)dropDownParentPair->subOptions.size(); i++ )
    {
        tPair = dropDownParentPair->subOptions[i];
        if( tPair!=NULL)
        {
            dataString+="[option]"+tPair->keyString+":"+tPair->keySubString+":"+ stg_ex::int_to_string(tPair->keyValue)+"[/option]";
        }
    }
    dataString+="[/menu]"+stg_ex::int_to_string(selectedId)+","+ stg_ex::int_to_string(showJustOptions)+",";
    return dataString;
}

bool GPE_DropDown_Menu::add_to_context_menu( GPE_PopUpMenu_Option * cLevel, gpe::key_pair * cKey )
{
    if( cKey == NULL || cLevel == NULL)
    {
        return true;
    }
    GPE_PopUpMenu_Option *  myLevel = cLevel->add_menu_option(cKey->keyString, cKey->keyId );

    gpe::key_pair * tempKey = NULL;
    int keyListSize = (int)cKey->subOptions.size();
    for( int i = 0; i < keyListSize; i++)
    {
        tempKey = cKey->subOptions[i];
        add_to_context_menu( myLevel, tempKey );
    }
    return true;
}

gpe::key_pair * GPE_DropDown_Menu::add_menu_option(std::string optionName, std::string optionSubStr,float optionValue, bool selectOption )
{

    if( dropDownParentPair->name_exists( optionName)  == false )
    {
        gpe::key_pair * kp = dropDownParentPair->add_keypair( optionName, optionSubStr, optionValue );
        if( selectOption )
        {
            set_id( kp->keyId );
        }
        return kp;
    }
    return NULL; //if everything breaks return nothing
}

void GPE_DropDown_Menu::clear_dropdown()
{
    selectedId = -1;
    selectedPair = NULL;
    selectedName = "";
    selectedTag = "";
    dropDownParentPair->remove_all();
}

gpe::key_pair *  GPE_DropDown_Menu::find_option_id(  int pairId )
{
    return dropDownParentPair->find_option_id( pairId );
}

gpe::key_pair *  GPE_DropDown_Menu::find_option_valie(  float pairValue )
{
    return dropDownParentPair->find_option_value( pairValue );
}

gpe::key_pair *  GPE_DropDown_Menu::find_selected_pair( gpe::key_pair *  pairIn, std::string pairName, int pairId )
{
    //Returns NULL if the pair is NULL;
    if( pairIn == NULL)
    {
        return NULL;
    }

    //Checks if the string is the same as the current pair
    if( pairIn->keyString == pairName)
    {
        //If we don't care about IDs ( less than 0), we have found our pair
        if( pairId < 0 )
        {
            return pairIn;
        }
        else if( pairIn->keyId == pairId )
        {
            //Otherwise our pairs must match the value
            return pairIn;
        }
    }
    else if( pairIn->keyId == pairId  && pairId >= 0 )
    {
        //Otherwise our pairs must match the value
        return pairIn;
    }
    //If we haven't found anything, iterate through the pair's suboptions and nest this function
    int pairListSize = (int)pairIn->subOptions.size();
    gpe::key_pair *  tempPair = NULL;
    gpe::key_pair *  foundPair = NULL;
    for( int i = 0; i < pairListSize; i++)
    {
        tempPair = pairIn->subOptions[i];
        foundPair = find_selected_pair( tempPair, pairName, pairId );
        if( foundPair !=NULL )
        {
            return foundPair;
        }
    }
    return NULL;
}

gpe::key_pair *  GPE_DropDown_Menu::find_selected_pair_sub( gpe::key_pair *  pairIn, std::string pairSubString )
{
    //Returns NULL if the pair is NULL;
    if( pairIn == NULL)
    {
        return NULL;
    }

    //Checks if the string is the same as the current pair
    if( pairIn->keySubString == pairSubString )
    {
        //If we don't care about IDs ( less than 0), we have found our pair
        return pairIn;
    }
    //If we haven't found anything, iterate through the pair's suboptions and nest this function
    int pairListSize = (int)pairIn->subOptions.size();
    gpe::key_pair *  tempPair = NULL;
    gpe::key_pair *  foundPair = NULL;
    for( int i = 0; i < pairListSize; i++)
    {
        tempPair = pairIn->subOptions[i];
        foundPair = find_selected_pair_sub( tempPair, pairSubString );
        if( foundPair !=NULL )
        {
            return foundPair;
        }
    }
    return NULL;
}

std::string GPE_DropDown_Menu::get_menu_option(int atNumb)
{
    gpe::key_pair * kp = NULL;
    if( selectedPair!=NULL  )
    {
        return selectedPair->keyString;
    }
    return "";
}

int GPE_DropDown_Menu::get_menu_size()
{
    return (int)dropDownParentPair->subOptions.size();
}

std::string GPE_DropDown_Menu::get_plain_string()
{
    if( selectedPair!=NULL  )
    {
        return "'"+selectedPair->keyString+"'";
    }
    return "''";
}

int GPE_DropDown_Menu::get_selected_id()
{
    return selectedId;
}

std::string GPE_DropDown_Menu::get_selected_name()
{
    if( selectedPair !=NULL )
    {
        return selectedPair->keyString;
    }
    return opName;
}


std::string GPE_DropDown_Menu::get_selected_tag()
{
    gpe::key_pair * kp = NULL;
    if( selectedPair !=NULL )
    {
        return selectedPair->keySubString;
    }
    return "";
}

float GPE_DropDown_Menu::get_selected_value()
{
    if( selectedPair !=NULL )
    {
        return selectedPair->keyValue;
    }
    return -1;
}

std::string GPE_DropDown_Menu::get_tag_from( std::string tagName, int tagId )
{

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
        int newOptionValue = -1;
        int beginOptionPos=dataString.find_first_of(optionTag);
        if(beginOptionPos!=(int)std::string::npos)
        {
            allOptionsString = stg_ex::split_first_string(dataString,"[/menu]");

            newOptionData = stg_ex::split_first_string(allOptionsString,"[menu]");
            while( (int)allOptionsString.size() > 0)
            {
                newOptionString = stg_ex::split_first_string(allOptionsString,"[/option]");

                beginOptionPos=newOptionString.find_first_of(optionTag);
                if(beginOptionPos!=(int)std::string::npos)
                {
                    //if the beginOptionPos is present, then parse on through and carryon
                    if( (int)newOptionString.size() > beginOptionPos+optionTagSize )
                    {
                        newOptionData = stg_ex::get_substring(newOptionString, beginOptionPos+optionTagSize );

                        newOptionName = stg_ex::split_first_string(newOptionData,":");
                        newOptionSubame = stg_ex::split_first_string(newOptionData,":");
                        newOptionValue = stg_ex::string_to_int(newOptionData,-1);
                        if( (int)newOptionName.size() > 0)
                        {
                            dropDownParentPair->add_keypair( newOptionName,newOptionSubame,newOptionValue );
                        }
                    }
                    else
                    {
                        return;
                    }
                }
            }
            set_id( stg_ex::split_first_int(dataString,',') );
            showJustOptions = stg_ex::string_to_bool( stg_ex::split_first_string(dataString,",") );
        }
    }
}

void GPE_DropDown_Menu::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    isHovered = false;
    justActivated = false;
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered || isInUse)
    {
        main_OVERLAY->update_tooltip(dropdownName);
    }
    if( isInUse)
    {
        hasArrowkeyControl = true;
    }
    else
    {
        hasArrowkeyControl = false;
    }
    if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
    {
        isClicked = true;
    }

    if( isClicked && cam!=NULL && viewedSpace!=NULL)
    {
        if( main_CONTEXT_MENU!=NULL)
        {
            if( main_CONTEXT_MENU->subMenuIsOpen == false)
            {
                isOpen = true;
                GPE_open_context_menu(viewedSpace->x+widget_box.x-cam->x, viewedSpace->y+widget_box.y+widget_box.h-cam->y);
                main_CONTEXT_MENU->set_width(widget_box.w);
                gpe::key_pair * kp = NULL;
                if( (int)dropDownParentPair->subOptions.size() > 0)
                {
                    if( showJustOptions ==false)
                    {
                        main_CONTEXT_MENU->add_menu_option(dropdownName,-1);
                    }

                    int optionsSize = (int)dropDownParentPair->subOptions.size();
                    for( int i = 0; i < optionsSize; i++)
                    {
                        kp = dropDownParentPair->subOptions[i];
                        add_to_context_menu( main_CONTEXT_MENU, kp );
                    }
                    if( showJustOptions ==false)
                    {
                        main_CONTEXT_MENU->add_menu_option(dropdownName, -1);
                    }
                }
                else
                {
                    main_CONTEXT_MENU->add_menu_option(dropdownName,-1);
                }
                isOpen = false;
                isClicked = false;
                justActivated = true;
                int foundResult = GPE_Get_Context_Result();
                GPE_close_context_menu();
                if( foundResult>=0 )
                {
                    set_id( foundResult );
                    GPE_close_context_menu();
                    return;
                }
                else
                {
                    set_id( -1 );

                    if(showJustOptions==false)

                    {
                        selectedId=-1;
                        opName=dropdownName;
                    }
                }
                GPE_close_context_menu();
            }
        }
    }
    else if( hasArrowkeyControl)
    {
        if( gpe::input->check_kb_down(kb_up) && selectedId > 0)
        {
            selectedId--;
        }
        else if( gpe::input->check_kb_down(kb_down) && selectedId < (int)dropDownParentPair->subOptions.size()-1)
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
            newOptionString = stg_ex::split_first_string(dataString,"[/option]");

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
    gpe::key_pair * tOption = NULL;
    if( selectedName == optionToRemove )
    {
        set_id( -1 );
    }
    dropDownParentPair->remove_option_named( optionToRemove );
}

void GPE_DropDown_Menu::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->input_color,false);

        if( selectedPair!=NULL )
        {
            gpe::gfs->render_text_resized( widget_box.x+widget_box.w/2-cam->x,widget_box.y+widget_box.h/2-cam->y,selectedPair->keyString,theme_main->input_font_color,FONT_POPUP,gpe::fa_center,gpe::fa_middle,widget_box.w-widget_box.h-12,-1);
        }
        else
        {
            gpe::gfs->render_text_resized( widget_box.x+widget_box.w/2-cam->x,widget_box.y+widget_box.h/2-cam->y,opName,theme_main->input_font_color,FONT_POPUP,gpe::fa_center,gpe::fa_middle,widget_box.w-widget_box.h-12,-1);
        }

        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->input_highlight_outline_color,true);
        }
        else if( isHovered)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->input_highlight_alt_color,true);
        }
        else
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->input_outline_color,true);
        }
        if( GPE_DROPDOWN_ARROW!=NULL)
        {
            GPE_DROPDOWN_ARROW->render_tex_resized( widget_box.x+widget_box.w-widget_box.h-cam->x,widget_box.y - cam->y,widget_box.h,widget_box.h, NULL, theme_main->input_font_color );
        }
    }
}

void GPE_DropDown_Menu::reset_suboptions()
{
    set_id(-1);

    dropDownParentPair->remove_all();
}

void GPE_DropDown_Menu::set_id(int new_id)
{
    if( new_id >= 0 )
    {
        selectedPair = dropDownParentPair->find_option_id( new_id );
        if( selectedPair!=NULL)
        {
            selectedId = new_id;
            selectedName = selectedPair->keyString;
            selectedTag = selectedPair->keySubString;
            selectedValue = selectedPair->keyValue;
            return;
        }
    }
    //If no pair was found we return to default mode
    selectedPair = NULL;
    selectedId = -1;
    selectedName = "";
    selectedTag = "";
    selectedValue = -1;
    opName = dropdownName;
}

void GPE_DropDown_Menu::set_option_named(std::string newselectedOptionName )
{
    gpe::key_pair *  tempPair = NULL;
    gpe::key_pair *  foundPair = NULL;
    int pairListSize = (int)dropDownParentPair->subOptions.size();
    for( int i = 0; i < pairListSize; i++)
    {
        tempPair = dropDownParentPair->subOptions[i];
        foundPair = find_selected_pair( tempPair, newselectedOptionName, -1 );
        if( foundPair !=NULL )
        {
            break;
        }
    }

    if( foundPair != NULL )
    {
        selectedPair = foundPair;
        selectedId = foundPair->keyValue;
        selectedName = foundPair->keyString;
        selectedTag = foundPair->keySubString;
        return;
    }

    //If no pair was found we return to default mode
    selectedPair = NULL;
    selectedId = -1;
    selectedName = "";
    selectedTag = "";
    opName = dropdownName;
}

void GPE_DropDown_Menu::set_option_subvalue(std::string newselectedOptionName )
{
    gpe::key_pair *  tempPair = NULL;
    gpe::key_pair *  foundPair = NULL;
    int pairListSize = (int)dropDownParentPair->subOptions.size();
    for( int i = 0; i < pairListSize; i++)
    {
        tempPair = dropDownParentPair->subOptions[i];
        foundPair = find_selected_pair_sub( tempPair, newselectedOptionName  );
        if( foundPair !=NULL )
        {
            break;
        }
    }

    if( foundPair != NULL )
    {
        selectedPair = foundPair;
        selectedId = foundPair->keyValue;
        selectedName = foundPair->keyString;
        selectedTag = foundPair->keySubString;
        return;
    }

    //If no pair was found we return to default mode
    selectedPair = NULL;
    selectedId = -1;
    selectedName = "";
    selectedTag = "";
    opName = dropdownName;
}

void GPE_DropDown_Menu::set_name(std::string new_name)
{
    dropdownName = opName = new_name;
}

void GPE_DropDown_Menu::set_option_value(float sValue )
{
    selectedPair = dropDownParentPair->find_option_value( sValue );
    if( selectedPair!=NULL)
    {
        selectedId = selectedPair->keyId;
        selectedName = selectedPair->keyString;
        selectedTag = selectedPair->keySubString;
    }
    else
    {
        selectedPair = NULL;
        selectedId = -1;
        selectedName = "";
        selectedTag = "";
    }
}

void GPE_DropDown_Menu::show_just_options(bool justOptions)
{
    showJustOptions = justOptions;
}
