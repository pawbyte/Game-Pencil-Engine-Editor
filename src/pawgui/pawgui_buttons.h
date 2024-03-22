/*
pawgui_buttons.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PAWGUI contributors ( Contributors Page )

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


#ifndef PAWGUI_BUTTONS_H
#define PAWGUI_BUTTONS_H

#include "pawgui_base.h"

namespace pawgui
{
    class widget_button_icon: public widget_basic
    {
    private:
        bool endsSection;
        int opId;
        gpe::texture_base * buttonTexture;
        std::string webUrl;
        bool wasClicked;
    public:
        bool isTabbed;
        bool usesTabs;
        widget_button_icon(std::string buttonImgFile="",std::string name="", int id = -1, int buttonSize = 32, bool lastInCol = false);
        ~widget_button_icon();
        void change_texture( std::string file_name );
        std::string get_data();
        void load_data(std::string datastring);
        bool ends_section();
        int get_id();
        void set_id(int new_id);
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void set_website(std::string url_string);
    };

    class widget_button_iconbar: public widget_basic
    {
    private:
        std::vector < widget_button_icon * > barOptions;
        int new_buttonXPos;
        bool isTabBar;
        int tabPosition;
        bool widthIsLimited;
    public:
        int hoverOption;
        int selectedOption;
        int barPadding;
        int xPadding;
        bool widthAutoResizes;
        widget_button_iconbar( int buttonSize = 24, bool useTabs = false);
        ~widget_button_iconbar();
        std::string get_data();
        void load_data(std::string datastring);
        int get_tab_pos();
        int get_tab_id();
        void limit_width(bool isLimited = false);
        void set_width(int newWid);
        widget_button_icon * add_option( std::string buttonImgFile,std::string name, int id = -1, bool endsSection = false);
        void process_self( gpe::shape_rect * view_space =nullptr, gpe::shape_rect *render_camera = nullptr);
        void recalculate_width();
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void set_coords(int x_new = -1, int y_new = -1);
        void set_height(int newXH);
        void set_selection(int newSelection);
    };

    class widget_button_label: public widget_basic
    {
    private:
        int opId;
    public:
        widget_button_label( std::string name, std::string description,int id = -1);
        ~widget_button_label();
        void prerender_self( );
        void process_self( gpe::shape_rect * view_space =nullptr, gpe::shape_rect *render_camera = nullptr);
        void set_id(int new_id);
        void set_name(std::string new_name);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    };

    class widget_button_push: public widget_basic
    {
    private:
        int opId;
        int maxCharactersToRender;
        gpe::texture_base * opTexture;
        int iconPadding;
        bool wasClicked;
        std::string webUrl;
        int buttonTextWidth;
        int buttonTextHeight;
    public:
        bool showBackground;
        widget_button_push( std::string imgLocation,std::string name, std::string description = "",int id = -1);
        ~widget_button_push();
        void set_id(int new_id);
        void set_name(std::string new_name);
        void set_website(std::string url_string);
        void prerender_self( );
        void process_self( gpe::shape_rect * view_space =nullptr, gpe::shape_rect *render_camera = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    };

    class widget_button_card_vertical: public widget_basic
    {
    private:
        int opId;
        gpe::texture_base * opTexture;
        int iconPadding;
        bool wasClicked;
        std::string webUrl;
    public:
        bool showBackground;
        bool usingFlagIcon;
        int lineHeight;
        std::string cardTitle;
        std::string cardSubTitle;
        std::string cardShortNote;
        std::vector< std::string> buttonLines;
        std::string stringValue;
        widget_button_card_vertical(std::string imgLocation,std::string parsedLines, std::string strValue,int id = -1, int buttonSize = 128 );
        ~widget_button_card_vertical();
        void enable_background( bool enableValue);
        void enable_border( bool enableValue);
        int get_id();
        void set_id(int new_id);
        void set_name(std::string new_name);
        void set_website(std::string url_string);
        void prerender_self( );
        void process_self( gpe::shape_rect * view_space =nullptr, gpe::shape_rect *render_camera = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    };
}

#endif
