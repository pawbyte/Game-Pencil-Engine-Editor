/*
pawgui_base.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PAWGUI contributors ( Contributors Page )

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

-PAWGUI <https://www.pawbyte.com/pawgui>
*/

#ifndef PAWGUI_BASE_H
#define PAWGUI_BASE_H

#include "pawgui_constants.h"
#include "pawgui_globals.h"
#include "pawgui_settings.h"
#include "pawgui_themes.h"
#include "pawgui_overlay.h"

namespace pawgui
{
    class widget_basic
    {
        protected:
            bool windowInView;
            gpe::shape_rect widget_box;
            bool clickedOutside;
            bool isClicked;
            bool isPressed;
            bool isRightClicked;
            bool isEnabled;
            bool isHovered;
            bool isModified;
            bool isInUse;
            std::string opName;
            std::string guiListTypeName;
            int guiListTypeId;
            int maxWidth, maxHeight;
            bool isFullWidth;
            std::string renderPackageName;
        public:
            int hAlign, vAlign;
            int dynamicId;
            bool needsNewLine;
            int indentationLevel;
            bool autoResizes;
            int upDelayTime;
            int downDelayTime;
            int leftDelayTime;
            int rightDelayTime;
            bool hasArrowkeyControl;
            bool hasScrollControl;
            bool hasLineBreak;
            std::string descriptionText;
            bool showBorderBox;
            widget_basic();
            virtual ~widget_basic() =0;
            void disable_self();
            void enable_self();
            int get_height();
            int get_width();
            int get_max_height();
            int get_max_width();
            int get_xpos();
            int get_x2pos();
            int get_ypos();
            int get_y2pos();
            std::string get_name();
            std::string get_element_type();
            virtual std::string get_data();
            virtual std::string get_plain_string();
            virtual void load_data(std::string dataString);
            virtual void remove_data(std::string dataString);
            bool is_clicked();
            bool is_rightclicked();
            bool is_enabled();
            virtual void move_pos(int newX = 0, int newY = 0);
            bool is_full_width();
            bool is_hovered();
            void set_clicked( bool clickedVal = true);
            void set_rightclicked( bool clickedVal = true);
            virtual void set_height(int newHeight);
            void set_hovered(bool hoverVal);
            virtual void set_width(int newWidth);
            virtual void set_coords(int newX = -1, int newY = -1);
            virtual void prerender_self( );
            virtual void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect *cam = NULL);
            virtual void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
            bool requires_newline();
            void scale_height(float scaleH);
            void scale_width(float scaleW);
            void scale_object(float scaleValue );
            void set_full_width( bool makeFullWidth );
            virtual void set_name(std::string new_name);
            virtual void set_max_width( int nMW);
            virtual void set_max_height( int nMH);
            //Modifable properties
            bool is_inuse();
            bool is_modified();
            void switch_inuse(bool newUse);
            void update_box( float nx, float ny, float nw, float nh);
            void update_paragraph(int foundMaxWidth = -1 );
    };

    void update_action_message(std::string newMessage);
    void update_popup_info(std::string newMessage="", int selectedOpId=-1, bool popIsContext = false);
}

#endif //PAWGUI_BASE_H
