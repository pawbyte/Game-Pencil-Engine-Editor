/*
pawgui_resource_dropdown.cpp
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

#include "pawgui_resource_dropdown.h"

namespace pawgui
{
    widget_drop_down_resource_menu::widget_drop_down_resource_menu( std::string name, widget_resource_container * cTree,int id,bool selectable )
    {
        widget_type = "resourcedropdown";
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 196;
        //int nameMinSize = name.size()*
        widget_box.h = 32;
        displayedResult = dropdownName = widget_name = name;
        containerTree = cTree;
        opId = id;
        isSelectable = selectable;
        selectedId = -1;
        isOpen = false;
        isClicked = false;
        justActivated = false;
        selectedResource= nullptr;
    }

    widget_drop_down_resource_menu::~widget_drop_down_resource_menu()
    {

    }

    std::string widget_drop_down_resource_menu::get_data()
    {
        if( containerTree!=nullptr)
        {
            std::string datastring = widget_type+":"+dropdownName+"==|||==";
            datastring+= containerTree->get_name()+",,,";
            if( selectedResource!=nullptr)
            {
                datastring+=stg_ex::int_to_string(selectedResource->get_global_id() )+",";
            }
            else
            {
                datastring+="-1,";
            }
            return datastring;
        }
        return "";
    }

    void widget_drop_down_resource_menu::load_data(std::string datastring)
    {
        if( (int)datastring.size() > 0)
        {
            std::string projectResourceFolder = "";
            int projectselectedContainer = -1;
            projectResourceFolder = stg_ex::split_first_string(datastring,",,,");

            if( containerTree!=nullptr )
            {
                widget_resource_container * foundResContainer  = containerTree->find_resource_from_name(projectResourceFolder, false );
                if( foundResContainer!=nullptr)
                {
                    containerTree = foundResContainer;
                    projectselectedContainer = stg_ex::split_first_int(datastring,',');
                    set_selection( projectselectedContainer);
                }
                else
                {
                    gpe::error_log->report("Can't find project resource container["+projectResourceFolder+"] using...["+project_current_name+"]");
                }
            }
            else
            {
                gpe::error_log->report("Can't find project GPE resource container using...["+project_current_name+"]");
            }
        }
    }

    void widget_drop_down_resource_menu::add_folder_contents(widget_resource_container * fFolder, popup_menu_option * fOptionFolder)
    {
        if( fFolder!=nullptr && fOptionFolder!=nullptr)
        {
            widget_resource_container * cContainer = nullptr;
            popup_menu_option * cOption = nullptr;
            for( int i = 0; i < fFolder->get_size(); i++)
            {
                cContainer = fFolder->get_resource_at(i);
                if( cContainer!=nullptr)
                {
                    cOption = fOptionFolder->add_menu_option(cContainer->get_name(),cContainer->get_global_id(),cContainer->get_resource_texture(),0,cContainer->get_resource_animation(),false,true,true);
                    cOption->isResourceOption = true;
                    if( cContainer->is_folder() )
                    {
                        cOption->isFolderOption = true;
                        add_folder_contents(cContainer,cOption);
                    }
                }
            }
        }
    }


    widget_resource_container * widget_drop_down_resource_menu::get_selected_container()
    {
        if( containerTree!=nullptr)
        {
            return containerTree->find_resource_from_id(selectedId,true,false);
        }
        return nullptr;
    }

    std::string widget_drop_down_resource_menu::get_selected_name()
    {
        if( containerTree!=nullptr)
        {
            widget_resource_container * fRes = containerTree->find_resource_from_id(selectedId,true,false);
            if( fRes!=nullptr)
            {
                std::string fStr = fRes->get_name();
                if( stg_ex::is_alnum( fStr, false, true) )
                {
                    return fStr;
                }
                else
                {
                    //defaults to the object's id instead.
                    return stg_ex::int_to_string(fRes->get_global_id() );
                }
            }
        }
        return "";
    }

    std::string widget_drop_down_resource_menu::get_plain_string()
    {
        if( selectedResource!=nullptr)
        {
            if( selectedResource->get_resource_type()== gpe::resource_type_function)
            {
                return ""+selectedResource->get_name()+"()";
            }
            else
            {
                return selectedResource->get_name();
            }
        }
        return "''";
    }

    int widget_drop_down_resource_menu::get_selected_id()
    {
        return selectedId;
    }

    bool widget_drop_down_resource_menu::just_activated()
    {
        return justActivated;
    }

    void widget_drop_down_resource_menu::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
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
        if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
        {
            isClicked = true;
        }

        if( isClicked && cam!=nullptr && view_space!=nullptr && containerTree!=nullptr )
        {
            if( resource_dragged!=nullptr)
            {
                if( resource_dragged->is_folder()==false && resource_dragged->projectParentFileName.compare(project_current_name)==0 )
                {
                    set_selection(resource_dragged->get_global_id() );
                    isOpen = false;
                    isClicked = false;
                    justActivated = true;
                }
                resource_dragged = nullptr;
            }
            else if( main_context_menu!=nullptr)
            {
                if( main_context_menu->subMenuIsOpen == false)
                {
                    isOpen = true;
                    context_menu_open(view_space->x+widget_box.x-cam->x, view_space->y+widget_box.y+widget_box.h-cam->y);
                    int estimatedMenuSize = containerTree->get_options_width()+default_icon_width_padded ;
                    int dropdownNameWidth = 0;
                    int dropdownNameHeight = 0;
                    if( (int)widget_name.size()>0 && font_toolbar!=nullptr)
                    {
                        font_toolbar->get_metrics(dropdownName,&dropdownNameWidth, &dropdownNameHeight);
                    }
                    dropdownNameWidth+=default_icon_width_padded+padding_default;
                    estimatedMenuSize = std::max( estimatedMenuSize, dropdownNameWidth );
                    main_context_menu->set_width( estimatedMenuSize );
                    main_context_menu->add_menu_option(dropdownName,-1,nullptr,-1,nullptr,false,true);
                    widget_resource_container * cContainer = nullptr;
                    popup_menu_option * cOption = nullptr;
                    if(containerTree!=nullptr)
                    {
                        for( int i = 0; i < containerTree->get_size(); i++)
                        {
                            cContainer = containerTree->get_resource_at(i);
                            if( cContainer!=nullptr)
                            {
                                cOption = main_context_menu->add_menu_option(cContainer->get_name(),cContainer->get_global_id(),cContainer->get_resource_texture(),cContainer->get_resource_image_frame(),cContainer->get_resource_animation(),false,true);
                                cOption->isResourceOption = true;
                                if( cContainer->is_folder() )
                                {
                                    add_folder_contents(cContainer,cOption);
                                    cOption->isFolderOption = true;
                                }
                            }
                        }
                    }
                    isOpen = false;
                    isClicked = false;
                    justActivated = true;
                    int foundResult = context_menu_process();
                    if( foundResult>=0)
                    {
                        selectedId = foundResult;
                        selectedResource = containerTree->find_resource_from_id(selectedId);
                        displayedResult = action_message_text;
                    }
                    else
                    {
                        selectedId=-1;
                        displayedResult=dropdownName;
                    }
                    context_menu_close();
                }
            }
        }
    }

    void widget_drop_down_resource_menu::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( cam!=nullptr && view_space!=nullptr)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->input_color,false);
            if( selectedId >= 0)
            {
                selectedResource = containerTree->find_resource_from_id(selectedId);
                if( selectedResource!=nullptr)
                {
                    gpe::gfs->render_text_clipped( widget_box.x+32+padding_default-cam->x,widget_box.y+widget_box.h/2-cam->y,selectedResource->get_name(),pawgui::theme_main->input_font_color,font_popup,gpe::fa_left,gpe::fa_middle,widget_box.w-widget_box.h-12,-1);
                    selectedResource->render_image( widget_box.x,widget_box.y,widget_box.h-2,widget_box.h-2,view_space,cam);
                }
                else
                {
                    gpe::gfs->render_text_clipped( widget_box.x-cam->x,widget_box.y+widget_box.h/2-cam->y,widget_name,pawgui::theme_main->input_font_color,font_popup,gpe::fa_left,gpe::fa_middle,widget_box.w-widget_box.h-12,-1);
                }
            }
            else
            {
                gpe::gfs->render_text_clipped( widget_box.x+padding_default-cam->x,widget_box.y+widget_box.h/2-cam->y,widget_name,pawgui::theme_main->input_font_color,font_popup,gpe::fa_left,gpe::fa_middle,widget_box.w-widget_box.h-12,-1);
            }
            //gpe::gcanvas->render_rectangle( widget_box.x+widget_box.w-widget_box.h*3/4-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->input_outline_color ,false);
            if( dropdown_arrow_texture!=nullptr )
            {
                dropdown_arrow_texture->render_tex_resized(  widget_box.x+widget_box.w-widget_box.h-cam->x,widget_box.y - cam->y,widget_box.h,widget_box.h, nullptr, pawgui::theme_main->input_font_color );
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
        }
    }

    void widget_drop_down_resource_menu::set_name(std::string new_name)
    {
        widget_name = new_name;
    }

    void widget_drop_down_resource_menu::set_selection(int new_id)
    {
        if( new_id>0 && containerTree!=nullptr)
        {
            widget_resource_container * tRes = containerTree->find_resource_from_id(new_id,true);
            if( tRes!=nullptr)
            {
                selectedId = new_id;
                displayedResult = tRes->get_name();
                selectedResource = tRes;
            }
        }
        else
        {
            selectedId = -1;
            displayedResult = widget_name = dropdownName;
            selectedResource = nullptr;
        }
    }

    void widget_drop_down_resource_menu::set_selected_target(std::string new_name)
    {
        int new_id = stg_ex::string_to_int(new_name, -1);
        if( new_id > 0 && containerTree!=nullptr )
        {
            widget_resource_container * tRes = containerTree->find_resource_from_id(new_id,true);
            if( tRes!=nullptr)
            {
                selectedId = new_id;
                displayedResult = tRes->get_name();
                selectedResource = tRes;
            }
        }
        else if(  containerTree!=nullptr )
        {
            widget_resource_container * tRes = containerTree->find_resource_from_name(new_name,true);
            if( tRes!=nullptr)
            {
                selectedId = tRes->get_global_id();
                displayedResult = tRes->get_name();
                selectedResource = tRes;
            }
            else
            {
                selectedId = -1;
                displayedResult = widget_name = dropdownName;
                selectedResource = nullptr;
            }
        }
        else
        {
            selectedId = -1;
            displayedResult = widget_name = dropdownName;
            selectedResource = nullptr;
        }
    }
}
