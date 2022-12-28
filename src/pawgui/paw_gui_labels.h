/*
paw_gui_labels.h
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

#ifndef PAW_GUI_LABELS_H
#define PAW_GUI_LABELS_H

#include "paw_gui_base.h"


class GPE_Label_Image: public GPE_GeneralGuiElement
{
private:
    bool isHoldingCustomTexture;
    gpe::texture_base * imgSrc;
public:
    bool allowOverSize;
    float resizeAspect;
    bool wasClicked;
    std::string webUrl;
    GPE_Label_Image( std::string name_param , std::string file_name,std::string description = "", std::string urlIn = "");
    ~GPE_Label_Image();
    void load_label_image(std::string new_file_name);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_name(std::string name_param);
    void change_texture( gpe::texture_base * imgIn);
    void set_width( int newW);
    void set_height( int newH);
    void set_size( float newW, float newH);
};

class GPE_Label_Text: public GPE_GeneralGuiElement
{
public:
    GPE_Label_Text(std::string name_param, std::string description = "");
    ~GPE_Label_Text();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_name(std::string name_param);
};

class GPE_Label_Error: public GPE_GeneralGuiElement
{
public:
    GPE_Label_Error(std::string name_param = "", std::string description = "");
    ~GPE_Label_Error();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_name(std::string name_param);
};

class GPE_Label_90Degree: public GPE_GeneralGuiElement
{
public:
    GPE_Label_90Degree(std::string name_param = "", std::string description = "");
    ~GPE_Label_90Degree();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_name(std::string name_param);
};

class GPE_Label_Paragraph: public GPE_GeneralGuiElement
{
protected:
    int defaultFontWidth;
    int defaultFontHeight;
    std::string paragraphText;
    std::vector< std::string> paragraphLines;
    int paragraphWidth;
public:
    GPE_Label_Paragraph(std::string name_param, std::string paragraphIn, std::string descriptionIn = "");
    ~GPE_Label_Paragraph();
    void add_line(std::string newLine);
    void clear_all_lines();
    std::string get_paragraph();
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_name(std::string name_param);
    void set_width(int newWidth);
    void set_height(int newHeight);
    void set_max_width( int nMW);
    void set_max_height( int nMH);
    void update_paragraph();
    void update_text( std::string newParagraph);
};

class GPE_Label_Title: public GPE_GeneralGuiElement
{
public:
    GPE_Label_Title(std::string name_param, std::string description = "");
    ~GPE_Label_Title();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_name(std::string name_param);
};

#endif
