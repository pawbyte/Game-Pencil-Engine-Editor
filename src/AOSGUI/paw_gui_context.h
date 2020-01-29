/*
paw_gui_context.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef PAW_GUI_CONTEXT_H
#define PAW_GUI_CONTEXT_H

#include "paw_gui_base.h"

const int GPE_CONTEXT_ROW_HEIGHT = 28;

class GPE_PopUpMenu_Option
{
private:
    bool isContext;
    bool showShortCuts;
    std::string opName;
    int opId;
    GPE_Animation * opanimation;
    GPE_Texture_Base * opTexture;
    int animationFrameNumber;
    int startYPos;
    int rowsInView;
    int maxRowsInView;
    std::vector <GPE_PopUpMenu_Option *> subOptions;
    bool isSelectable;
    int optionWidth;
public:
    int menuDirection;
    float downDelay;
    float upDelay;
    float leftDelay;
    float rightDelay;
    bool isResourceOption;
    bool renderWhite;
    bool isFolderOption;
    GPE_Rect optionBox;
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
    std::string shortcutString;
    GPE_PopUpMenu_Option(std::string name = "", int id = -1,bool selectable = true, bool seeShortCut = true, bool makeContext = false, int kbS1 = -1, int kbS2 = -1, int kbS3 = -1 );
    ~GPE_PopUpMenu_Option();
    GPE_PopUpMenu_Option * add_option( GPE_PopUpMenu_Option * otherOption);
    GPE_PopUpMenu_Option * add_menu_option( std::string name, int id = -1, GPE_Texture_Base * gTexture = NULL,int animationImgNumb = -1,GPE_Animation * ganimation=NULL, bool endsSection=false, bool selectable = true, bool isResource = false, int kbS1 = -1, int kbS2 = -1, int kbS3 = -1 );
    int activate_hovered();
    void clear_menu();
    int get_width();
    GPE_PopUpMenu_Option * get_menu_option(int atNumb);
    int get_menu_size();
    std::string get_name();
    bool is_open();
    bool push_left();
    bool push_right();
    bool push_up();
    bool push_down();
    void reset_suboptions();
    void set_id(int newId);
    void set_image_data(GPE_Animation * newanimation,int newId);
    void set_position(int xPos = -1, int yPos = -1);
    void set_width(int newWidth);
    void set_texture(GPE_Texture_Base * newTexture);
    void open_menu();
    void prerender_self(  );
    int process_menu_option();
    void resize_self();
    void update_selectability(bool selectable);
    void render_self( GPE_Rect * cam = NULL);
};

extern GPE_PopUpMenu_Option * MAIN_CONTEXT_MENU;

class GPE_Toolbar: public GPE_GeneralGuiElement
{
private:
    GPE_Color *textColor;
    int barWidthTotal;
    bool toolBarIsOpen;
    std::vector <GPE_PopUpMenu_Option *> barOptions;
    bool justClosed;
    bool openRequested;
public:
    float downDelay;
    float upDelay;
    float leftDelay;
    float rightDelay;
    int hoverOption;
    int selectedOption;
    GPE_Toolbar();
    ~GPE_Toolbar();
    void close_toolbar();
    bool just_closed();
    bool is_open();
    bool open_requested();
    void reset_options();
    void set_width(int newWid);
    GPE_PopUpMenu_Option * add_menu_option( std::string name, int id = -1);
    void open_toolbar();
    void prerender_self(  );
    void process_toolbar();
    void render_toolbar( GPE_Rect *renderCam = NULL );
};

void GPE_open_context_menu(int menuXPos=-1,int menuYPos=-1, int newWidth = 128);
void GPE_close_context_menu();
int GPE_Get_Context_Result(GPE_Rect * camera = NULL, bool redrawScreen = true, bool autoResize = true);

#endif
