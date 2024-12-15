/*
pawgui_labels.h
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

#ifndef PAWGUI_LABELS_H
#define PAWGUI_LABELS_H

#include "../base/pawgui_base.h"

namespace pawgui
{
    class widget_label_image: public widget_basic
    {
    private:
        bool isHoldingCustomTexture;
        gpe::texture_base * imgSrc;
    public:
        bool allowOverSize;
        float resizeAspect;
        bool wasClicked;
        std::string webUrl;
        widget_label_image( std::string name_param , std::string file_name,std::string description = "", std::string url_string = "");
        ~widget_label_image();
        void load_label_image(std::string new_file_name);
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void set_name(std::string name_param);
        void change_texture( gpe::texture_base * imgIn);
        void set_width( int newW);
        void set_height( int newH);
        void set_size( float newW, float newH);
    };

    class widget_label_text: public widget_basic
    {
    public:
        widget_label_text(std::string name_param, std::string description = "");
        ~widget_label_text();
        std::string get_data();
        std::string get_plain_string();
        void load_data(std::string datastring);
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void set_name(std::string name_param);
    };

    class widget_label_error: public widget_basic
    {
    public:
        widget_label_error(std::string name_param = "", std::string description = "");
        ~widget_label_error();
        std::string get_data();
        std::string get_plain_string();
        void load_data(std::string datastring);
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void set_name(std::string name_param);
    };

    class widget_label_rotated_90: public widget_basic
    {
    public:
        widget_label_rotated_90(std::string name_param = "", std::string description = "");
        ~widget_label_rotated_90();
        std::string get_data();
        std::string get_plain_string();
        void load_data(std::string datastring);
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void set_name(std::string name_param);
    };

    class widget_label_paragraph: public widget_basic
    {
    protected:
        int defaultFontWidth;
        int defaultFontHeight;
        std::string paragraphText;
        std::vector< std::string> paragraphLines;
        int paragraphWidth;
    public:
        widget_label_paragraph(std::string name_param, std::string paragraphIn, std::string descriptionIn = "");
        ~widget_label_paragraph();
        void add_line(std::string newLine);
        void clear_all_lines();
        std::string get_paragraph();
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void set_name(std::string name_param);
        void set_width(int new_width);
        void set_height(int new_height);
        void set_max_width( int nMW);
        void set_max_height( int nMH);
        void update_paragraph();
        void update_text( std::string newParagraph);
    };

    class widget_label_title: public widget_basic
    {
    public:
        widget_label_title(std::string name_param, std::string description = "");
        ~widget_label_title();
        std::string get_data();
        std::string get_plain_string();
        void load_data(std::string datastring);
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void set_name(std::string name_param);
    };
}

#endif //PAWGUI_LABELS_H
