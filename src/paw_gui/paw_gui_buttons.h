/*
paw_gui_buttons.h
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


#ifndef PAW_GUI_BUTTONS_H
#define PAW_GUI_BUTTONS_H

#include "paw_gui_base.h"

class GPE_ToolIconButton: public GPE_GeneralGuiElement
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
    GPE_ToolIconButton(std::string buttonImgFile="",std::string name="", int id = -1, int buttonSize = 24, bool lastInCol = false);
    ~GPE_ToolIconButton();
    void change_texture( std::string file_name );
    std::string get_data();
    void load_data(std::string dataString);
    bool ends_section();
    int get_id();
    void set_id(int new_id);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_website(std::string urlIn);
};

class GPE_ToolIconButtonBar: public GPE_GeneralGuiElement
{
private:
    std::vector < GPE_ToolIconButton * > barOptions;
    int newButtonXPos;
    bool isTabBar;
    int tabPosition;
    bool widthIsLimited;
public:
    int hoverOption;
    int selectedOption;
    int barPadding;
    int xPadding;
    bool widthAutoResizes;
    GPE_ToolIconButtonBar( int buttonSize = 24, bool useTabs = false);
    ~GPE_ToolIconButtonBar();
    std::string get_data();
    void load_data(std::string dataString);
    int get_tab_pos();
    int get_tab_id();
    void limit_width(bool isLimited = false);
    void set_width(int newWid);
    GPE_ToolIconButton * add_option( std::string buttonImgFile,std::string name, int id = -1, bool endsSection = false);
    void process_self( gpe::shape_rect * viewedSpace =NULL, gpe::shape_rect *renderCam = NULL);
    void recalculate_width();
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_coords(int newX = -1, int newY = -1);
    void set_height(int newXH);
    void set_selection(int newSelection);
};

class GPE_ToolLabelButton: public GPE_GeneralGuiElement
{
private:
    int opId;
public:
    GPE_ToolLabelButton( std::string name, std::string description,int id = -1);
    ~GPE_ToolLabelButton();
    void prerender_self( );
    void process_self( gpe::shape_rect * viewedSpace =NULL, gpe::shape_rect *renderCam = NULL);
    void set_id(int new_id);
    void set_name(std::string new_name);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
};

class GPE_ToolPushButton: public GPE_GeneralGuiElement
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
    GPE_ToolPushButton( std::string imgLocation,std::string name, std::string description = "",int id = -1);
    ~GPE_ToolPushButton();
    void set_id(int new_id);
    void set_name(std::string new_name);
    void set_website(std::string urlIn);
    void prerender_self( );
    void process_self( gpe::shape_rect * viewedSpace =NULL, gpe::shape_rect *renderCam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
};

class GPE_VerticalCardButton: public GPE_GeneralGuiElement
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
    GPE_VerticalCardButton(std::string imgLocation,std::string parsedLines, std::string strValue,int id = -1, int buttonSize = 128 );
    ~GPE_VerticalCardButton();
    void enable_background( bool enableValue);
    void enable_border( bool enableValue);
    int get_id();
    void set_id(int new_id);
    void set_name(std::string new_name);
    void set_website(std::string urlIn);
    void prerender_self( );
    void process_self( gpe::shape_rect * viewedSpace =NULL, gpe::shape_rect *renderCam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
};

#endif
