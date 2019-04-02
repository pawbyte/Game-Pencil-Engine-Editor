/*
paw_gui_labels.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef PAW_GUI_LABELS_H
#define PAW_GUI_LABELS_H

#include "paw_gui_base.h"


class GPE_Label_Image: public GPE_GeneralGuiElement
{
private:
    bool isHoldingCustomTexture;
    GPE_Texture * imgSrc;
public:
    bool allowOverSize;
    double resizeAspect;
    bool wasClicked;
    std::string webUrl;
    GPE_Label_Image(GPE_Texture * imgIn = NULL,std::string nameIn = "", std::string description = "", std::string urlIn = "");
    ~GPE_Label_Image();
    void load_label_image(std::string fileNameIn);
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void set_name(std::string nameIn);
    void set_image( GPE_Texture * imgIn);
    void set_width( int newW);
    void set_height( int newH);
    void set_size( double newW, double newH);
};

class GPE_Label_Text: public GPE_GeneralGuiElement
{
public:
    GPE_Label_Text(std::string nameIn, std::string description = "");
    ~GPE_Label_Text();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void set_name(std::string nameIn);
};

class GPE_Label_Error: public GPE_GeneralGuiElement
{
public:
    GPE_Label_Error(std::string nameIn = "", std::string description = "");
    ~GPE_Label_Error();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void set_name(std::string nameIn);
};

class GPE_Label_90Degree: public GPE_GeneralGuiElement
{
public:
    GPE_Label_90Degree(std::string nameIn = "", std::string description = "");
    ~GPE_Label_90Degree();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void set_name(std::string nameIn);
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
    GPE_Label_Paragraph(std::string nameIn, std::string paragraphIn, std::string descriptionIn = "");
    ~GPE_Label_Paragraph();
    void add_line(std::string newLine);
    void clear_all_lines();
    std::string get_paragraph();
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void set_name(std::string nameIn);
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
    GPE_Label_Title(std::string nameIn, std::string description = "");
    ~GPE_Label_Title();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void set_name(std::string nameIn);
};

#endif
