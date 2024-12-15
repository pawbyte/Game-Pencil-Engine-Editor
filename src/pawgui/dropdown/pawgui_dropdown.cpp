/*
pawgui_dropdown.cpp
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

#include "pawgui_dropdown.h"

namespace pawgui
{
    widget_dropdown_menu::widget_dropdown_menu( std::string name, bool justOptions)
    {
        widget_type = "dropdown";
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 192;
        //int nameMinSize = name.size()*
        widget_box.h = 32;
        dropdownName = widget_name = name;
        opId = -1;
        selectedId = -1;
        selectedPair = nullptr;
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

    widget_dropdown_menu::~widget_dropdown_menu()
    {
        clear_dropdown();
        if( dropDownParentPair!=nullptr )
        {
            delete dropDownParentPair;
            dropDownParentPair = nullptr;
        }
    }

    std::string widget_dropdown_menu::get_data()
    {
        std::string datastring = widget_type+":"+dropdownName+"==|||==[menu]";
        gpe::key_pair * tPair = nullptr;
        for( int i = 0; i < (int)dropDownParentPair->sub_options.size(); i++ )
        {
            tPair = dropDownParentPair->sub_options[i];
            if( tPair!=nullptr)
            {
                datastring+="[option]"+tPair->key_string+":"+tPair->key_substring+":"+ stg_ex::int_to_string(tPair->key_value)+"[/option]";
            }
        }
        datastring+="[/menu]"+stg_ex::int_to_string(selectedId)+","+ stg_ex::int_to_string(showJustOptions)+",";
        return datastring;
    }

    bool widget_dropdown_menu::add_to_context_menu( popup_menu_option * cLevel, gpe::key_pair * cKey )
    {
        if( cKey == nullptr || cLevel == nullptr)
        {
            return true;
        }
        popup_menu_option *  myLevel = cLevel->add_menu_option(cKey->key_string, cKey->key_id );

        gpe::key_pair * tempKey = nullptr;
        int keyListSize = (int)cKey->sub_options.size();
        for( int i = 0; i < keyListSize; i++)
        {
            tempKey = cKey->sub_options[i];
            add_to_context_menu( myLevel, tempKey );
        }
        return true;
    }

    gpe::key_pair * widget_dropdown_menu::add_menu_option(std::string optionName, std::string optionSubStr,float optionValue, bool selectOption )
    {

        if( dropDownParentPair->name_exists( optionName)  == false )
        {
            gpe::key_pair * kp = dropDownParentPair->add_keypair( optionName, optionSubStr, optionValue );
            if( selectOption )
            {
                set_id( kp->key_id );
            }
            return kp;
        }
        return nullptr; //if everything breaks return nothing
    }

    void widget_dropdown_menu::clear_dropdown()
    {
        selectedId = -1;
        selectedPair = nullptr;
        selectedName = "";
        selectedTag = "";
        dropDownParentPair->remove_all();
    }

    gpe::key_pair *  widget_dropdown_menu::find_option_id(  int pairId )
    {
        return dropDownParentPair->find_option_id( pairId );
    }

    gpe::key_pair *  widget_dropdown_menu::find_option_valie(  float pairValue )
    {
        return dropDownParentPair->find_option_value( pairValue );
    }

    gpe::key_pair *  widget_dropdown_menu::find_selected_pair( gpe::key_pair *  pairIn, std::string pairName, int pairId )
    {
        //Returns nullptr if the pair is nullptr;
        if( pairIn == nullptr)
        {
            return nullptr;
        }

        //Checks if the string is the same as the current pair
        if( pairIn->key_string == pairName)
        {
            //If we don't care about IDs ( less than 0), we have found our pair
            if( pairId < 0 )
            {
                return pairIn;
            }
            else if( pairIn->key_id == pairId )
            {
                //Otherwise our pairs must match the value
                return pairIn;
            }
        }
        else if( pairIn->key_id == pairId  && pairId >= 0 )
        {
            //Otherwise our pairs must match the value
            return pairIn;
        }
        //If we haven't found anything, iterate through the pair's suboptions and nest this function
        int pairListSize = (int)pairIn->sub_options.size();
        gpe::key_pair *  tempPair = nullptr;
        gpe::key_pair *  foundPair = nullptr;
        for( int i = 0; i < pairListSize; i++)
        {
            tempPair = pairIn->sub_options[i];
            foundPair = find_selected_pair( tempPair, pairName, pairId );
            if( foundPair !=nullptr )
            {
                return foundPair;
            }
        }
        return nullptr;
    }

    gpe::key_pair *  widget_dropdown_menu::find_selected_pair_sub( gpe::key_pair *  pairIn, std::string pairSubstring )
    {
        //Returns nullptr if the pair is nullptr;
        if( pairIn == nullptr)
        {
            return nullptr;
        }

        //Checks if the string is the same as the current pair
        if( pairIn->key_substring == pairSubstring )
        {
            //If we don't care about IDs ( less than 0), we have found our pair
            return pairIn;
        }
        //If we haven't found anything, iterate through the pair's suboptions and nest this function
        int pairListSize = (int)pairIn->sub_options.size();
        gpe::key_pair *  tempPair = nullptr;
        gpe::key_pair *  foundPair = nullptr;
        for( int i = 0; i < pairListSize; i++)
        {
            tempPair = pairIn->sub_options[i];
            foundPair = find_selected_pair_sub( tempPair, pairSubstring );
            if( foundPair !=nullptr )
            {
                return foundPair;
            }
        }
        return nullptr;
    }

    std::string widget_dropdown_menu::get_menu_option(int atNumb)
    {
        gpe::key_pair * kp = nullptr;
        if( selectedPair!=nullptr  )
        {
            return selectedPair->key_string;
        }
        return "";
    }

    int widget_dropdown_menu::get_menu_size()
    {
        return (int)dropDownParentPair->sub_options.size();
    }

    std::string widget_dropdown_menu::get_plain_string()
    {
        if( selectedPair!=nullptr  )
        {
            return "'"+selectedPair->key_string+"'";
        }
        return "''";
    }

    int widget_dropdown_menu::get_selected_id()
    {
        return selectedId;
    }

    std::string widget_dropdown_menu::get_selected_name()
    {
        if( selectedPair !=nullptr )
        {
            return selectedPair->key_string;
        }
        return widget_name;
    }


    std::string widget_dropdown_menu::get_selected_tag()
    {
        gpe::key_pair * kp = nullptr;
        if( selectedPair !=nullptr )
        {
            return selectedPair->key_substring;
        }
        return "";
    }

    float widget_dropdown_menu::get_selected_value()
    {
        if( selectedPair !=nullptr )
        {
            return selectedPair->key_value;
        }
        return -1;
    }

    std::string widget_dropdown_menu::get_tag_from( std::string tagName, int tagId )
    {
        return ""; //WIPNOTFUNCTIONALYET
    }

    bool widget_dropdown_menu::just_activated()
    {
        return justActivated;
    }

    void widget_dropdown_menu::load_data(std::string datastring)
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
            int newOptionValue = -1;
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
                set_id( stg_ex::split_first_int(datastring,',') );
                showJustOptions = stg_ex::string_to_bool( stg_ex::split_first_string(datastring,",") );
            }
        }
    }

    void widget_dropdown_menu::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        isHovered = false;
        justActivated = false;
        widget_basic::process_self(view_space,cam);
        if( isHovered || isInUse)
        {
            main_overlay_system->update_tooltip(dropdownName);
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

        if( isClicked && cam!=nullptr && view_space!=nullptr)
        {
            if( main_context_menu!=nullptr)
            {
                if( main_context_menu->subMenuIsOpen == false)
                {
                    isOpen = true;
                    context_menu_open(view_space->x+widget_box.x-cam->x, view_space->y+widget_box.y+widget_box.h-cam->y);
                    main_context_menu->set_width(widget_box.w);
                    gpe::key_pair * kp = nullptr;
                    if( (int)dropDownParentPair->sub_options.size() > 0)
                    {
                        if( showJustOptions ==false)
                        {
                            main_context_menu->add_menu_option(dropdownName,-1);
                        }

                        int optionsSize = (int)dropDownParentPair->sub_options.size();
                        for( int i = 0; i < optionsSize; i++)
                        {
                            kp = dropDownParentPair->sub_options[i];
                            add_to_context_menu( main_context_menu, kp );
                        }
                        if( showJustOptions ==false)
                        {
                            main_context_menu->add_menu_option(dropdownName, -1);
                        }
                    }
                    else
                    {
                        main_context_menu->add_menu_option(dropdownName,-1);
                    }
                    isOpen = false;
                    isClicked = false;
                    justActivated = true;
                    int foundResult = context_menu_process();
                    context_menu_close();
                    if( foundResult>=0 )
                    {
                        set_id( foundResult );
                        context_menu_close();
                        return;
                    }
                    else
                    {
                        set_id( -1 );

                        if(showJustOptions==false)

                        {
                            selectedId=-1;
                            widget_name=dropdownName;
                        }
                    }
                    context_menu_close();
                }
            }
        }
        else if( hasArrowkeyControl)
        {
            if( gpe::input->check_kb_down(kb_up) && selectedId > 0)
            {
                selectedId--;
            }
            else if( gpe::input->check_kb_down(kb_down) && selectedId < (int)dropDownParentPair->sub_options.size()-1)
            {
                selectedId++;
            }
        }
    }

    void widget_dropdown_menu::remove_data(std::string datastring)
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

    void widget_dropdown_menu::remove_option(std::string optionToRemove)
    {
        gpe::key_pair * tOption = nullptr;
        if( selectedName == optionToRemove )
        {
            set_id( -1 );
        }
        dropDownParentPair->remove_option_named( optionToRemove );
    }

    void widget_dropdown_menu::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( cam!=nullptr && view_space!=nullptr)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->input_color,false);

            if( selectedPair!=nullptr )
            {
                gpe::gfs->render_text_clipped( widget_box.x+widget_box.w/2-cam->x,widget_box.y+widget_box.h/2-cam->y,selectedPair->key_string,pawgui::theme_main->input_font_color,font_popup,gpe::fa_center,gpe::fa_middle,widget_box.w-widget_box.h-12,-1);
            }
            else
            {
                gpe::gfs->render_text_clipped( widget_box.x+widget_box.w/2-cam->x,widget_box.y+widget_box.h/2-cam->y,widget_name,pawgui::theme_main->input_font_color,font_popup,gpe::fa_center,gpe::fa_middle,widget_box.w-widget_box.h-12,-1);
            }

            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->input_highlight_outline_color,true);
            }
            else if( isHovered)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->input_highlight_alt_color,true);
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->input_outline_color,true);
            }
            if( dropdown_arrow_texture!=nullptr)
            {
                dropdown_arrow_texture->render_tex_resized( widget_box.x+widget_box.w-widget_box.h-cam->x,widget_box.y - cam->y,widget_box.h,widget_box.h, nullptr, pawgui::theme_main->input_font_color );
            }
        }
    }

    void widget_dropdown_menu::reset_suboptions()
    {
        set_id(-1);

        dropDownParentPair->remove_all();
    }

    void widget_dropdown_menu::set_id(int new_id)
    {
        if( new_id >= 0 )
        {
            selectedPair = dropDownParentPair->find_option_id( new_id );
            if( selectedPair!=nullptr)
            {
                selectedId = new_id;
                selectedName = selectedPair->key_string;
                selectedTag = selectedPair->key_substring;
                selectedValue = selectedPair->key_value;
                return;
            }
        }
        //If no pair was found we return to default mode
        selectedPair = nullptr;
        selectedId = -1;
        selectedName = "";
        selectedTag = "";
        selectedValue = -1;
        widget_name = dropdownName;
    }

    void widget_dropdown_menu::set_option_named(std::string newselectedOptionName )
    {
        gpe::key_pair *  tempPair = nullptr;
        gpe::key_pair *  foundPair = nullptr;
        int pairListSize = (int)dropDownParentPair->sub_options.size();
        for( int i = 0; i < pairListSize; i++)
        {
            tempPair = dropDownParentPair->sub_options[i];
            foundPair = find_selected_pair( tempPair, newselectedOptionName, -1 );
            if( foundPair !=nullptr )
            {
                break;
            }
        }

        if( foundPair != nullptr )
        {
            selectedPair = foundPair;
            selectedId = foundPair->key_value;
            selectedName = foundPair->key_string;
            selectedTag = foundPair->key_substring;
            return;
        }

        //If no pair was found we return to default mode
        selectedPair = nullptr;
        selectedId = -1;
        selectedName = "";
        selectedTag = "";
        widget_name = dropdownName;
    }

    void widget_dropdown_menu::set_option_subvalue(std::string newselectedOptionName )
    {
        gpe::key_pair *  tempPair = nullptr;
        gpe::key_pair *  foundPair = nullptr;
        int pairListSize = (int)dropDownParentPair->sub_options.size();
        for( int i = 0; i < pairListSize; i++)
        {
            tempPair = dropDownParentPair->sub_options[i];
            foundPair = find_selected_pair_sub( tempPair, newselectedOptionName  );
            if( foundPair !=nullptr )
            {
                break;
            }
        }

        if( foundPair != nullptr )
        {
            selectedPair = foundPair;
            selectedId = foundPair->key_value;
            selectedName = foundPair->key_string;
            selectedTag = foundPair->key_substring;
            return;
        }

        //If no pair was found we return to default mode
        selectedPair = nullptr;
        selectedId = -1;
        selectedName = "";
        selectedTag = "";
        widget_name = dropdownName;
    }

    void widget_dropdown_menu::set_name(std::string new_name)
    {
        dropdownName = widget_name = new_name;
    }

    void widget_dropdown_menu::set_option_value(float option_value )
    {
        selectedPair = dropDownParentPair->find_option_value( option_value );
        if( selectedPair!=nullptr)
        {
            selectedId = selectedPair->key_id;
            selectedName = selectedPair->key_string;
            selectedTag = selectedPair->key_substring;
        }
        else
        {
            selectedPair = nullptr;
            selectedId = -1;
            selectedName = "";
            selectedTag = "";
        }
    }

    void widget_dropdown_menu::show_just_options(bool justOptions)
    {
        showJustOptions = justOptions;
    }
}
