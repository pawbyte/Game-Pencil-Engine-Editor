/*
pawgui_stree.h
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

#ifndef PAWGUI_STREE_H
#define PAWGUI_STREE_H

#include "../panel_list/pawgui_panel_list.h"
#include "../buttons/pawgui_buttons.h"
#include "../text_input/pawgui_text_input.h"

namespace pawgui
{
    class widget_branch: public widget_basic
    {
        protected:
            gpe::branch_type branch_type_id;
            int barXPadding, barYPadding;
            bool subMenuIsOpen;
            int globalId;
            bool widget_visible;
            int lastWidth;
            std::string name;
            int optionSuperType;
            //gpe::texture_base * textTexture;
            gpe::texture_base * iconTexture;
        public:
            int rightClickedId;
            int branchLevel;
            std::vector < widget_branch * > sub_elements;
            bool isLocked;
            int selectedSubOption;
            widget_branch * selectedBranch;
            widget_branch * branch_parent;
            widget_branch * treeParent;
            int foundX2Pos;
            int previouslySoughtId;
            bool mouseInRange;
            widget_branch();
            virtual ~widget_branch();
            void add_to_list( widget_panel_list * tList = nullptr);
            void add_to_searched_list( widget_panel_list * tList, std::string needlestring );
            virtual void add_branch( widget_branch * branch, bool changeId = true  );
            virtual bool can_add_new_branch();
            widget_branch * get_branch_from_pos( int pos );
            widget_branch * get_branch_from_id( int id );
            widget_branch * get_first_branch();
            widget_branch * get_last_branch();
            int get_global_id();
            std::string get_name();
            int get_selected_id();
            gpe::branch_type get_type();
            bool hovering_openclose( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr );
            virtual int increment_count();
            void insert_branch( widget_branch * branch, int pos = -1 );
            int matches(widget_branch * otherBranch );
            bool is_visible();
            void open_and_view();
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect *cam = nullptr);
            bool remove_branch( int branchId );
            void remove_branch(std::string branch_name );
            void remove_branch( widget_branch * branch );
            void remove_all_branches();
            virtual void render_branch();
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            virtual bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);
            void set_global_id( int new_id );
            virtual void set_name( std::string new_name );
    };

    //Trees aren't parents of branches, but for this class they are :-)
    class widget_tree: public widget_branch
    {
        private:
            int maxSuperFolderCount;
            int seekedX2Pos;
            int barTitleWidth;
            int barTitleHeight;
            int globalBranchCounter;
            widget_button_icon * add_button;
            widget_input_text * searchField;
            std::vector < widget_branch * > searchResultsElements;
        public:
            bool useMetaTop;
            widget_panel_list * treeList;
            bool just_resized;
            bool showYScroll;
            bool being_resized;
            bool treeResized;
            bool branchMoved;
            gpe::texture_base * opTexture;
            std::string needleBranch;
            widget_tree( std::string menuName,  gpe::branch_type optionSuperType, int optionId = -1 );
            ~widget_tree( );
            bool add_button_clicked();
            bool can_add_new_branch();
            int increment_count();
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr );
            void render_branch();
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void toggle_self();
            bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);
            void select_branch(widget_branch * branch );
    };
}
#endif //PAWGUI_STREE_H
