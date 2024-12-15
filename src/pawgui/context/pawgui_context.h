/*
pawgui_context.h
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

#ifndef PAWGUI_CONTEXT_H
#define PAWGUI_CONTEXT_H

#include "../base/pawgui_base.h"

namespace pawgui
{
    const int context_menu_height_default = 28;
    extern int popup_font_size_width;
    extern int popup_font_size_height;

    class popup_menu_option: public widget_basic
    {
    private:
        bool isContext;
        bool showShortCuts;
        std::string widget_name;
        int opId;
        gpe::animation2d * opanimation;
        gpe::texture_base * opTexture;
        int animationFrameNumber;
        int startYPos;
        int rowsInView;
        int maxRowsInView;
        std::vector <popup_menu_option *> sub_options;
        bool isSelectable;
        int optionWidth;
        int browsing_result;
    public:
        int menuDirection;
        float downDelay;
        float upDelay;
        float leftDelay;
        float rightDelay;
        bool isResourceOption;
        bool renderWhite;
        bool isFolderOption;
        bool isTopOfMenu;
        int optionWidthSpace;
        int subOptionWidthSpace;
        int widestOptionSpace;
        int hoverOption;
        int selectedOption;
        bool sectionBottom;
        bool subMenuIsOpen;
        int subMenuStartPos;
        int maxOptionsToView;
        int selfDistanceToBottom;
        int kbShortCut1;
        int kbShortCut2;
        int kbShortCut3;
        std::string shortcutstring;
        popup_menu_option(std::string name = "", int id = -1,bool selectable = true, bool seeShortCut = true, bool makeContext = false, int kbS1 = -1, int kbS2 = -1, int kbS3 = -1 );
        ~popup_menu_option();
        popup_menu_option * add_option( popup_menu_option * otherOption);
        popup_menu_option * add_menu_option( std::string name, int id = -1, gpe::texture_base * gTexture = nullptr,int animationImgNumb = -1, gpe::animation2d * ganimation=nullptr, bool endsSection=false, bool selectable = true, bool isResource = false, int kbS1 = -1, int kbS2 = -1, int kbS3 = -1 );
        int activate_hovered();
        void clear_menu();
        void close_menu();
        int get_browsing_results();
        int get_width();
        popup_menu_option * get_menu_option(int atNumb);
        int get_menu_size();
        std::string get_name();
        bool is_open();
        bool push_left();
        bool push_right();
        bool push_up();
        bool push_down();
        void reset_suboptions();
        void set_id(int new_id);
        void change_texture_data( gpe::animation2d * new_animation,int new_id);
        void set_position(int x_pos = -1, int y_pos = -1);
        void set_width(int new_width);
        void set_texture( gpe::texture_base * newTexture);
        void open_menu();
        void prerender_self(  );
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect *cam = nullptr );
        void resize_self();
        void update_selectability(bool selectable);
        void render_self(  gpe::shape_rect * view_space = nullptr, gpe::shape_rect *cam = nullptr);
    };

    extern popup_menu_option * main_context_menu;

    class widget_toolbar: public widget_basic
    {
    private:
        gpe::color *text_color;
        int barWidthTotal;
        bool toolBarIsOpen;
        std::vector <popup_menu_option *> barOptions;
        bool justClosed;
        bool openRequested;
    public:
        float downDelay;
        float upDelay;
        float leftDelay;
        float rightDelay;
        int hoverOption;
        int selectedOption;
        widget_toolbar();
        ~widget_toolbar();
        void close_toolbar();
        bool just_closed();
        bool is_open();
        bool open_requested();
        void reset_options();
        void set_width(int newWid);
        popup_menu_option * add_menu_option( std::string name, int id = -1);
        void open_toolbar();
        void prerender_self(  );
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect *cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect *cam = nullptr);
    };

    void context_menu_open(int menuXPos=-1,int menuYPos=-1, int new_width = 128);
    void context_menu_close();
    int context_menu_process( gpe::shape_rect * camera = nullptr, bool redrawScreen = true, bool autoResize = true);
}

#endif //PAWGUI_CONTEXT_H
