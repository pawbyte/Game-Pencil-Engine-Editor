/*
pawgui_stree.cpp
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

#include "../stree/pawgui_stree.h"
#include "../popups/pawgui_popups.h"

namespace pawgui
{
    const int SPECIAL_BRANCH_HEIGHT = 24;
    widget_branch::widget_branch()
    {
        branchLevel = 1;
        rightClickedId = -1;
        isLocked = false;
        needsNewLine = true;
        iconTexture = nullptr;
        selectedBranch = nullptr;
        branch_parent = nullptr;
        treeParent = nullptr;

        selectedSubOption = -1;
        lastWidth = 0;
        foundX2Pos = 0;
        previouslySoughtId = -1;
        mouseInRange = false;
        widget_visible = true;

        widget_name = name = "unnamed_branch";
        branch_type_id = gpe::branch_type::DEFAULT;
        globalId = -1;

        barXPadding = 0;
        barYPadding = 0;
        subMenuIsOpen = false;
        widget_box.h = SPECIAL_BRANCH_HEIGHT;
    }

    widget_branch::~widget_branch()
    {
        remove_all_branches();
    }

    void widget_branch::add_to_list( widget_panel_list * tList )
    {
        if( tList!=nullptr )
        {
            tList->add_indented_element( indentationLevel, this );
            if( subMenuIsOpen )
            {
                int subSize = (int)sub_elements.size();
                widget_branch * current_branch = nullptr;
                for( int i = 0; i < subSize; i++ )
                {
                    current_branch= sub_elements.at( i );
                    if( current_branch!=nullptr )
                    {
                        current_branch->indentationLevel = indentationLevel+1;
                        current_branch->add_to_list( tList );
                    }
                }
            }
        }
    }

    void widget_branch::add_to_searched_list( widget_panel_list * tList, std::string needlestring  )
    {
        if( tList!=nullptr )
        {
            bool addedSelf = false;
            if( stg_ex::string_contains( stg_ex::string_lower( name ) , needlestring) )
            {
                tList->add_indented_element( indentationLevel, this );
                addedSelf = true;
            }
            int subSize = (int)sub_elements.size();

            if( subSize > 0  )
            {
                widget_branch * current_branch = nullptr;
                for( int i = 0; i < subSize; i++ )
                {
                    current_branch= sub_elements.at( i );
                    if( current_branch!=nullptr )
                    {
                        if( addedSelf )
                        {
                            current_branch->indentationLevel = indentationLevel+1;
                        }
                        current_branch->add_to_searched_list( tList, needlestring );
                    }
                }
            }
        }
    }

    void widget_branch::add_branch( widget_branch * branch, bool changeId)
    {
        if( branch!=nullptr )
        {
            if( treeParent == nullptr )
            {
                //should only be done by the tree...
                treeParent = this;
            }
            branch->branch_parent = this;
            branch->treeParent = treeParent;
            branch->branchLevel = branchLevel+1;
            if( changeId )
            {
                branch->set_global_id( increment_count() );
            }
            sub_elements.push_back( branch );
        }
    }


    bool widget_branch::widget_branch::can_add_new_branch()
    {
        //This is only changable via child classes
        return true;
    }

    int widget_branch::get_global_id()
    {
        return globalId;
    }

    widget_branch * widget_branch::get_first_branch()
    {
        if( (int)sub_elements.size() > 0 )
        {
            return sub_elements[0];
        }
        return nullptr;
    }

    widget_branch * widget_branch::get_last_branch()
    {
        if( (int)sub_elements.size() > 0 )
        {
            return sub_elements[ (int)sub_elements.size() - 1];
        }
        return nullptr;
    }
    std::string widget_branch::get_name()
    {
        return name;
    }

    int widget_branch::get_selected_id()
    {
        return selectedSubOption;
    }

    gpe::branch_type widget_branch::get_type()
    {
        return branch_type_id;
    }

    bool widget_branch::hovering_openclose( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        if( !isHovered)
        {
            return false;
        }
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );

        if( gpe::input->mouse_position_x <= view_space->x - cam->x + widget_box.x + widget_box.w ) //branchLevel * resource_container_default_height+default_icon_plusminus_size )
        {
            return true;
        }
        return false;
    }

    int widget_branch::increment_count()
    {
        if( treeParent!=nullptr )
        {
            return treeParent->increment_count();
        }
        else
        {
            gpe::error_log->report("Tree parent is nullptr..." );
        }
        return -1;
    }

    void widget_branch::insert_branch( widget_branch * branch, int pos)
    {
        if( branch!=nullptr && can_add_new_branch() )
        {
            if( pos > 0 && pos < (int)sub_elements.size() )
            {
                sub_elements.insert( sub_elements.begin()+ pos, branch );
            }
            else
            {
                sub_elements.push_back( branch );
            }
        }
    }

    bool widget_branch::is_visible()
    {
        return widget_visible;
    }

    int widget_branch::matches(widget_branch * otherBranch )
    {
        if( otherBranch!=nullptr)
        {
            //id and type check..
            if( get_global_id()==otherBranch->get_global_id() && otherBranch->get_type() == branch_type_id)
            {
                if( name.compare( otherBranch->get_name() )==0 )
                {
                    return 1; //return true
                }
            }
            return 0; //return false
        }
        return -1; //defaults if other branch doesnt exist...
    }
    void widget_branch::open_and_view()
    {
        subMenuIsOpen = true;
        if( branch_parent!=nullptr && branch_parent->matches( this)==false )
        {
            branch_parent->subMenuIsOpen = true;
            branch_parent->open_and_view();
        }
    }

    void widget_branch::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam  )
    {
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );
        widget_basic::process_self( view_space, cam );
        widget_box.w = cam->w;
        widget_box.h = SPECIAL_BRANCH_HEIGHT;
        if( isHovered )
        {
            if( isPressed )
            {

            }
            else if( isClicked )
            {
                if( hovering_openclose( view_space, cam) )
                {
                    subMenuIsOpen = !subMenuIsOpen;
                }
                if( treeParent!=nullptr )
                {
                    treeParent->selectedSubOption = globalId;
                    treeParent->selectedBranch = this;
                }
            }
            else if(  isRightClicked )
            {
                if( hovering_openclose( view_space, cam) )
                {
                    subMenuIsOpen = !subMenuIsOpen;
                }
                if( treeParent!=nullptr )
                {
                    treeParent->rightClickedId = globalId;
                    treeParent->selectedSubOption = globalId;
                    treeParent->selectedBranch = this;
                }
            }
        }
        else
        {
            isClicked = isPressed = isRightClicked = false;
        }
    }

    bool widget_branch::remove_branch( int branchId )
    {
        return true;
    }

    void widget_branch::remove_branch( std::string branch_name )
    {
        if( (int)branch_name.size() > 0 )
        {

        }
    }

    void widget_branch::remove_branch( widget_branch * branch )
    {
        if( branch!=nullptr )
        {
            if( treeParent!=nullptr && treeParent->selectedBranch!=nullptr )
            {
                if( branch->matches( treeParent->selectedBranch ) )
                {
                    treeParent->selectedBranch = nullptr;
                    treeParent->selectedSubOption = -1;

                }
            }
            widget_branch * foundBranch = nullptr;
            for( int i = (int)sub_elements.size()-1; i >=0; i-- )
            {
                foundBranch = sub_elements.at(  i );
                if( foundBranch!=nullptr &&  branch->matches( branch ) )
                {
                    //gpe::error_log->report( "Deleting :" + foundBranch->get_name() );
                    //foundBranch->remove_all_branches();
                    delete foundBranch;
                    foundBranch = nullptr;
                    break;
                }
            }
        }
    }

    void widget_branch::remove_all_branches()
    {
        widget_branch * foundBranch = nullptr;
        for( int i = (int)sub_elements.size()-1; i >=0; i-- )
        {
            foundBranch = sub_elements.at(  i );
            if( foundBranch!=nullptr )
            {
                //gpe::error_log->report( "Deleting :" + foundBranch->get_name() );
                //foundBranch->remove_all_branches();
                if( treeParent!=nullptr && treeParent->selectedBranch!=nullptr )
                {
                    if( foundBranch->matches( treeParent->selectedBranch ) )
                    {
                        treeParent->selectedBranch = nullptr;
                        treeParent->selectedSubOption = -1;

                    }
                }
                delete foundBranch;
                foundBranch = nullptr;
            }
        }
        sub_elements.clear();
    }

    void widget_branch::render_branch( )
    {
        int subElCount  = (int)sub_elements.size();
        widget_branch * current_branch = nullptr;
        for(  int myItr = 0; myItr < subElCount; myItr++ )
        {
            current_branch = sub_elements[myItr];
            if( current_branch!=nullptr)
            {
                current_branch->render_branch();
            }
        }
    }

    void widget_branch::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam   )
    {
        bool selfIsInView = false;
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( cam!=nullptr && view_space!=nullptr)
        {
            int rendY = widget_box.y;
            if( rendY>=cam->y && rendY <=cam->y+cam->h )
            {
                selfIsInView = true;
            }
            else if( rendY+SPECIAL_BRANCH_HEIGHT >=cam->y && rendY+SPECIAL_BRANCH_HEIGHT <=cam->y+cam->h  )
            {
                selfIsInView = true;
            }

            if( selfIsInView )
            {
                if(treeParent!=nullptr && treeParent->get_selected_id()==globalId && globalId > 0  )
                {
                    gpe::gcanvas->render_rectangle( 0,widget_box.y-cam->y,
                                           cam->w-cam->x,widget_box.y+16-cam->y,pawgui::theme_main->button_box_highlight_color,false,255 );
                }
            }
            if( (int)sub_elements.size() > 0 )
            {
                if( subMenuIsOpen )
                {
                    if( selfIsInView )
                    {
                        gpe::gfs->render_text( widget_box.x-cam->x, widget_box.y-cam->y,"-",pawgui::theme_main->main_box_font_color,FONT_STREE_BRANCH,gpe::fa_left,gpe::fa_top);
                    }
                    widget_branch * foundBranch = nullptr;

                    /*
                    for( int i=0; i< (int)sub_elements.size();i++ )
                    {
                        foundBranch = sub_elements[i];
                        if(foundBranch!=nullptr)
                        {
                            foundBranch->previouslySoughtId = selectedSubOption;
                            foundBranch->render_self( view_space,cam);

                        }
                    }*/
                    gpe::gfs->render_text( widget_box.x-cam->x,
                                     widget_box.y-cam->y,
                                     "-",pawgui::theme_main->main_box_font_color,FONT_STREE_BRANCH,gpe::fa_left,gpe::fa_top);

                }
                else if( (selfIsInView  ) )
                {
                    gpe::gfs->render_text( widget_box.x-cam->x,
                                     widget_box.y-cam->y,
                                     "+",pawgui::theme_main->main_box_font_color,FONT_STREE_BRANCH,gpe::fa_left,gpe::fa_top);

                }
            }

            if( selfIsInView )
            {
                if( iconTexture!=nullptr )
                {
                    iconTexture->render_tex_resized(widget_box.x+default_icon_plusminus_size-cam->x,widget_box.y-cam->y, SPECIAL_BRANCH_HEIGHT,SPECIAL_BRANCH_HEIGHT,nullptr,  pawgui::theme_main->main_box_font_color );
                }
                gpe::gfs->render_text( widget_box.x+SPECIAL_BRANCH_HEIGHT+default_icon_plusminus_size-cam->x,widget_box.y-cam->y+SPECIAL_BRANCH_HEIGHT/2, name,pawgui::theme_main->main_box_font_color,FONT_STREE_BRANCH,gpe::fa_left,gpe::fa_center);
            }
        }
    }

    bool widget_branch::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn  )
    {
        //Will add code later to fix.
        return true;
    }

    void widget_branch::set_global_id( int new_id )
    {
        globalId = new_id;
    }

    void widget_branch::set_name( std::string new_name )
    {
        widget_name = name = new_name;
    }


    widget_tree::widget_tree( std::string menuName,  gpe::branch_type optionSuperType, int optionId  )
    {
        useMetaTop = true;
        add_button = new widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/plus.png","Add Branch" );
        searchField = new widget_input_text("","Search..." );
        needsNewLine = true;
        treeList = new widget_panel_list();
        widget_name = name = menuName;
        branch_type_id = optionSuperType;
        globalId = optionId;

        hasScrollControl = false;
        hasArrowkeyControl = false;

        opTexture = nullptr;
        maxSuperFolderCount = 32;
        seekedX2Pos = 0;
        barTitleWidth = 0;
        barTitleHeight = 32;
        globalBranchCounter = 0;

        just_resized = false;
        showYScroll = false;
        being_resized = false;
        treeResized = false;
        branchMoved = false;

        widget_box.x = 0;
        widget_box.y = 48;
        widget_box.w = 192;
        widget_box.h = 128;

        barXPadding = 8;
        barYPadding = 8;

        treeParent = this;
    }

    widget_tree::~widget_tree( )
    {
        if( treeList!=nullptr)
        {
            delete treeList;
            treeList = nullptr;
        }

        remove_all_branches();
    }
    bool widget_tree::add_button_clicked()
    {
        if( useMetaTop )
        {
            return add_button->is_clicked();
        }
        return false;
    }

    bool widget_tree::can_add_new_branch()
    {
        if( (int)sub_elements.size() < maxSuperFolderCount || maxSuperFolderCount < 0 )
        {
            return true;
        }
        return false;
    }

    int widget_tree::increment_count()
    {
        globalBranchCounter+=1;
        return globalBranchCounter;
    }

    void widget_tree::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam  )
    {
        rightClickedId = -1;
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find(cam);
        widget_basic::process_self( view_space, cam );
        indentationLevel = 0;
        //if( widget_visible )
        {
            int sOpNumber = -1;
            just_resized = false;

            bool mouseInRange = false;

            if( widget_box.w > gpe::screen_width/2)
            {
                widget_box.w = gpe::screen_width/2;
                treeResized = just_resized = true;
            }

            if( widget_box.w < 32 && widget_visible)
            {
                widget_box.w = 32;
                treeResized = just_resized = true;
            }
            if( isHovered )
            {
                mouseInRange = true;
            }
            if( gpe::input->check_mouse_pressed(0) || gpe::input->check_mouse_pressed(1) || gpe::input->check_mouse_pressed(2) )
            {
                if( mouseInRange)
                {
                    hasScrollControl = true;
                    hasArrowkeyControl = true;
                }
                else
                {
                    hasScrollControl = false;
                    hasArrowkeyControl = false;
                }
            }

            //if( mouseInRange || menuResized || resourcebarMoved)
            {

                widget_branch * current_branch = nullptr;

                if( useMetaTop )
                {
                    add_button->set_coords( widget_box.x, widget_box.y );
                    searchField->set_coords( add_button->get_x2()+padding_default, widget_box.y );
                    searchField->set_width( widget_box.w - searchField->get_xpos()-padding_default );
                    add_button->process_self( view_space, cam  );
                    searchField->process_self( view_space, cam  );
                }
                else
                {
                    add_button->set_clicked( false );
                    searchField->set_string( "");
                }

                treeList->clear_list();
                treeList->set_coords( widget_box.x, widget_box.y+32 );
                treeList->set_width( widget_box.w);
                treeList->set_height( widget_box.h-32 );
                treeList->barXPadding = 0;
                treeList->barYPadding = 0;
                treeList->barXMargin = 0;
                treeList->barYMargin = 0;

                std::string searchedstring = searchField->get_string();

                int i = 0;
                int subElementsSize = (int)sub_elements.size();
                if( (int)searchedstring.size() > 0 && useMetaTop)
                {
                    searchedstring= stg_ex::string_lower( searchedstring );
                    for( i=0; i< subElementsSize; i++)
                    {
                        current_branch = sub_elements[i];
                        if(current_branch!=nullptr)
                        {
                            current_branch->indentationLevel = indentationLevel;
                            current_branch->add_to_searched_list( treeList, searchedstring );
                        }
                    }
                }
                else
                {
                    for( i=0; i< subElementsSize; i++)
                    {
                        current_branch = sub_elements[i];
                        if(current_branch!=nullptr)
                        {
                            current_branch->indentationLevel = indentationLevel;
                            current_branch->add_to_list( treeList );
                        }
                    }
                }
                treeList->process_self( view_space, cam );
            }
        }
    }

    void widget_tree::render_branch()
    {
        widget_branch::render_branch( );
    }

    void widget_tree::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );

        if( widget_visible )
        {

            if( pawgui::theme_main->theme_texture_bg == nullptr)
            {
                //gpe::gcanvas->render_rectangle( 0,0,widget_box.w,widget_box.h,pawgui::theme_main->program_color,false);
            }


            if( useMetaTop )
            {
                add_button->render_self( view_space, cam  );
                searchField->render_self( view_space, cam  );
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x - cam->x, widget_box.y - cam->y,
                                       widget_box.x + widget_box.w, widget_box.y + barTitleHeight - cam->x,pawgui::theme_main->popup_box_color,false);
                gpe::gfs->render_text( widget_box.x +padding_default - cam->x, widget_box.y - cam->y, name, pawgui::theme_main->popup_box_font_color,FONT_STREE_BRANCH,gpe::fa_left,gpe::fa_top);
            }
            if( hasScrollControl)
            {
                gpe::gcanvas->render_rectangle( widget_box.x -cam->x,widget_box.y -cam->y,
                                       widget_box.x+widget_box.w -cam->x, widget_box.y+widget_box.h-cam->y,pawgui::theme_main->button_box_highlight_color,true);
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x -cam->x,widget_box.y -cam->y,
                                       widget_box.x+widget_box.w -cam->x, widget_box.y+widget_box.h-cam->y,pawgui::theme_main->text_box_outline_color,true);
            }

            if( treeList!=nullptr )
            {
                ///current_branch->selectedSubOption = selectedSubOption;
                treeList->render_self(view_space, cam);
            }
        }
    }

    void widget_tree::toggle_self()
    {

    }

    bool widget_tree::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn  )
    {
        if( fileTarget!=nullptr && fileTarget->is_open() )
        {
            widget_branch * current_branch = nullptr;
            for( int i = 0; i < (int)sub_elements.size(); i++)
            {
                current_branch = sub_elements[i];
                if( current_branch!=nullptr )
                {
                    current_branch->save_branch_data( fileTarget, 0 );
                }
            }
        }
        return false;
    }

    void widget_tree::select_branch(widget_branch * branch )
    {
        if( branch!=nullptr)
        {
            selectedBranch = branch;
            selectedSubOption = branch->get_global_id();
        }
        else
        {
            selectedBranch = nullptr;
            selectedSubOption = -1;
        }
    }
}
