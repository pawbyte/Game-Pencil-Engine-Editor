/*
ambitious_gui_library.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef AMBITIUOS_GUI_LIBRARY_H
#define AMBITIUOS_GUI_LIBRARY_H

#include "../GPE_Engine/GPE.h"

const int PGUI_MAJOR_VERSION = 1;
const int PGUI_MINOR_VERSION = 0;
const int PGUI_PATCHLEVEL = 2;

const int nestedFolderLimit = 4;
const int SEARCH_MODE_FIND = 1;
const int SEARCH_MODE_GOTO = 2;
const int SEARCH_MODE_REPLACE = 3;
const int SEARCH_MODE_FINDALL = 4;
const int SEARCH_MODE_REPLACEALL = 5;

const int DISPLAY_QUERY_YES = 1;
const int DISPLAY_QUERY_NO = 0;
const int DISPLAY_QUERY_CANCEL = 2;

extern int POPUP_FONT_SIZE_WIDTH;
extern int POPUP_FONT_SIZE_HEIGHT;
extern int POPUP_MENU_VALUE;

extern std::string CURRENT_PROJECT_NAME;
extern std::string GPE_Action_Message;
extern int GPE_Action_ID;
extern bool GPE_ACTION_IS_CONTEXT;

extern std::string displayMessageTitle;
extern std::string displayMessageSubtitle;
extern std::string displayMessageString;


class GPE_GUI_Settings
{
    public:
        GPE_Rect textAreaFindBox;

        //Text Editor Related Settings [ BEGIN ]
        float textInputDelayTime;
        float normalInputDelayTime;
        float textAreaDelayTime;
        bool autoFindMouseTabs;
        int tabSpaceCount;

        bool showTextEditorLineCount;
        bool showTextEditorSyntaxHightlight;
        int defaultLineCountWidth;
        //Text Editor Related Settings [ END ]
        //Default Folder Related Settings [ BEGIN ]
        std::string fileOpenAudioDir;
        std::string fileOpenVideoDir;
        std::string fileOpenSpriteDir;
        std::string fileOpenTextureDir;
        std::string fileOpenTilesheetDir;
        std::string fileOpenFontDir;
        std::string fileOpenFunctionDir;
        std::string fileOpenTextFileDir;
        std::string fileOpenImportFileDir;
        std::string fileSaveExportFileDir;

        std::string fileSaveTextFileDir;
        std::string fileOpenDefaultDir;
        std::string fileSaveDefaultDir;
        //Default Folder RelatedSettings [ END ]
        //std::string
        GPE_GUI_Settings();
        ~GPE_GUI_Settings();
};

extern GPE_GUI_Settings * MAIN_GUI_SETTINGS;

class GPE_KeyPair
{
    public:
        double keyValue;
        std::string keyString;
        std::string keySubString;
        GPE_KeyPair(double idNumb, std::string str, std::string ksubstr ="");
        ~GPE_KeyPair();
};

class GPE_ParsedText
{
    public:
        int textStart;
        int textEnd;
        GPE_ParsedText(int nStart, int nEnd);
        ~GPE_ParsedText();
};

class GPE_SyntaxLine
{
    public:
        std::vector <GPE_ParsedText*> foundParses;
        int firstPos;
        int lastPos;
        GPE_SyntaxLine();
        ~GPE_SyntaxLine();
        std::string get_parsed_line(std::string sIn);
        void render_tokens(GPE_Renderer * cRender,GPE_Font * fontIn,std::string sIn, int xPos, int yPos, int lineStartPos, int lineMaxPos, GPE_Color * renderColor = NULL, bool isNumber = false);
        void reset_self();
};

void update_action_message(std::string newMessage);

void update_popup_info(std::string newMessage="", int selectedOpId=-1, bool popIsContext = false);

class GPE_Template
{
    public:
        std::string templateLocalLocation;
        GPE_Sprite * Main_Menu_Sprite;

        //main bg colors ( defaults everywhere)
        GPE_Color * Program_Color;
        GPE_Color * Program_Header_Color;
        GPE_Color * Main_Box_Color;
        GPE_Color * Main_Box_Highlighted_Color;
        GPE_Color * Main_Box_Font_Color;
        GPE_Color * Main_Box_Font_URL_Color;
        GPE_Color * Main_Box_Font_URL_Visited_Color;
        GPE_Color * Main_Box_Font_Highlight_Color;
        GPE_Color * Main_Box_Faded_Font_Color;
        GPE_Color * Main_Box_Faded_Color;
        GPE_Color * Main_Border_Color;
        GPE_Color * Main_Border_Highlighted_Color;
        //For Folders and such...
        GPE_Color * Main_Folder_Color;
        GPE_Color * Main_Folder_Highlighted_Color;
        //For Input Fields, Drop Down Menus & Resource Drop Downs
        GPE_Color * Input_Highlight_Color;
        GPE_Color * Input_Highlight_Outline_Color;
        GPE_Color * Input_Highlight_Alt_Color;
        GPE_Color * Input_Color;
        GPE_Color * Input_Error_Font_Color;
        GPE_Color * Input_Error_Box_Color;
        GPE_Color * Input_Error_Outline_Color;
        GPE_Color * Input_Outline_Color;
        GPE_Color * Input_Font_Color;
        GPE_Color * Input_Highlight_Font_Color;
        GPE_Color * Input_Faded_Font_Color;
        GPE_Color * Input_Selected_Color;
        //For ButtonBar and Iconed Buttons
        GPE_Color * Icon_Box_Color;
        GPE_Color * Icon_Box_Highlighted_Color;
        GPE_Color * Icon_Box_Selected_Color;
        GPE_Color * Icon_Border_Color;
        GPE_Color * Icon_Border_Highlighted_Color;
        GPE_Color * Icon_Border_Selected_Color;
        GPE_Color * Icon_Font_Color;
        GPE_Color * Icon_Font_Highlighted_Color;
        GPE_Color * Icon_Font_Selected_Color;
        //For Button and such...
        GPE_Color * Checkbox_Color;
        GPE_Color * Button_Box_Color;
        GPE_Color * Button_Box_Highlighted_Color;
        GPE_Color * Button_Box_Selected_Color;
        GPE_Color * Button_Border_Color;
        GPE_Color * Button_Border_Highlighted_Color;
        GPE_Color * Button_Border_Selected_Color;
        GPE_Color * Button_Font_Color;
        GPE_Color * Button_Font_Highlighted_Color;
        //For Gui boxes, like main menu, toolbar and oontext menus
        GPE_Color * PopUp_Box_Highlight_Color;
        GPE_Color * PopUp_Box_Highlight_Alt_Color;
        GPE_Color * PopUp_Box_Close_Color;
        GPE_Color * PopUp_Box_Close_Hightlight_Color;
        GPE_Color * PopUp_Box_Close_Font_Color;
        GPE_Color * PopUp_Box_Color;
        GPE_Color * PopUp_Box_Border_Color;
        GPE_Color * PopUp_Box_Font_Color;
        GPE_Color * PopUp_Box_Highlight_Font_Color;
        GPE_Color * PopUp_Box_Faded_Font_Color;
        //Scroll box colors
        GPE_Color * Scroll_Box_Color;
        GPE_Color * Scroll_Box_Border_Color;
        GPE_Color * Scroll_Box_Arrow_Color;
        GPE_Color * Scroll_Box_Camera_Color;
        GPE_Color * Scroll_Box_Camera_Highlight_Color;
        //for text boxes
        GPE_Color * Text_Box_Outer_Color;
        GPE_Color * Text_Box_Outer_Font_Color;
        GPE_Color * Text_Box_Color;
        GPE_Color * Text_Box_Outline_Color;
        GPE_Color * Text_Box_Highlight_Color;
        GPE_Color * Text_Box_Font_Color;
        GPE_Color * Text_Box_Font_Comment_Color;
        GPE_Color * Text_Box_Font_Highlight_Color;
        GPE_Color * Text_Box_Font_DataType_Color;
        GPE_Color * Text_Box_Font_DQuote_Color;
        GPE_Color * Text_Box_Font_Function_Color;
        GPE_Color * Text_Box_Font_Function_Alt_Color;
        GPE_Color * Text_Box_Font_Keyword_Color;
        GPE_Color * Text_Box_Font_Keyword_Alt_Color;
        GPE_Color * Text_Box_Font_JavaScript_Color;
        GPE_Color * Text_Box_Font_JavaScript_Alt_Color;
        GPE_Color * Text_Box_Font_Variable_Color;
        GPE_Color * Text_Box_Font_Variable_Alt_Color;
        GPE_Color * Text_Box_Font_Number_Color;
        GPE_Color * Text_Box_Font_SQuote_Color;
        GPE_Color * Text_Box_Font_Symbols_Color;
        GPE_Color * Text_Box_Project_Function_Color;
        GPE_Color * Text_Box_Project_Function_Alt_Color;
        GPE_Color * Text_Box_Project_Keyword_Color;
        GPE_Color * Text_Box_Project_Keyword_Alt_Color;
        //for errors
        GPE_Color * Main_Error_Font_Color;
        GPE_Color * Main_Warning_Font_Color;
        GPE_Color * Main_Suggestion_Font_Color;
        GPE_Template();
        ~GPE_Template();
        void load_theme(std::string themeLocationIn);
};

extern GPE_Template * GPE_CUSTOM_TEMPLATE;
extern GPE_Template * GPE_DARK_TEMPLATE;
extern GPE_Template * GPE_DEFAULT_TEMPLATE;
extern GPE_Template * GPE_LIGHT_TEMPLATE;
extern GPE_Template * GPE_RED_TEMPLATE;
extern GPE_Template * GPE_GREEN_TEMPLATE;
extern GPE_Template * GPE_BLUE_TEMPLATE;

extern GPE_Template * GPE_MAIN_TEMPLATE;


extern GPE_Texture * GPE_LOGO;
extern GPE_Texture * GPE_TEXTURE_TRANSPARENT_BG;
extern SDL_Surface * GPE_SURFACE_COLOR_PICKER_GRADIENT;
extern GPE_Texture * GPE_TEXTURE_COLOR_PICKER_GRADIENT;
extern GPE_Texture * GPE_CHECKMARK_IMAGE;
extern GPE_Sprite  * GPE_TRIANGLE;

extern SDL_Cursor * GPE_Cursor;
extern SDL_SystemCursor GPE_PreviousCursor;
extern SDL_SystemCursor GPE_CurrentCursor;
extern SDL_Cursor * GPE_LoadingCursor;
extern SDL_Cursor * GPE_HoverCursor;
extern bool GPE_IS_LOADING;


//The font that's going to be used
extern GPE_Font * font;
extern GPE_Font * textboxFont;
extern GPE_Font * FONT_CATEGORY_BAR;
extern GPE_Font * DEFAULT_FONT;
extern GPE_Font * FONT_POPUP;
extern GPE_Font * FONT_TOOLBAR;
extern GPE_Font * FONT_RESOURCEBAR;
extern GPE_Font * FONT_TEXTINPUT;
extern GPE_Font * FONT_TEXTINPUT_COMMENT;
extern GPE_Font * FONT_TEXTINPUT_KEYWORD;
extern GPE_Font * FONT_TEXTINPUT_FUNCTION;
extern GPE_Font * FONT_TEXTINPUT_NUMBER;
extern GPE_Font * FONT_TEXTINPUT_QUOTE;
extern GPE_Font * FONT_TEXTINPUT_SYMBOL;
extern GPE_Font * FONT_TEXTINPUT_PROJECT_FUNCTION;
extern GPE_Font * FONT_TEXTINPUT_PROJECT_KEYWORD;

extern GPE_Font * FONT_TEXTINPUT_BOLD;
extern GPE_Font * FONT_TERM_NAME;
extern GPE_Font * FONT_TERM_DESCRIPTION;
extern GPE_Font * FONT_TERM_SCOPE;

extern GPE_Font * FONT_DEFAULT_PROMPT;
extern GPE_Font * FONT_CHECKBOX;

extern GPE_Font * FONT_HEADER;
extern GPE_Font * FONT_LABEL;
extern GPE_Font * FONT_LABEL_ANCHOR;
extern GPE_Font * FONT_LABEL_TITLE;
extern GPE_Font * FONT_PARAGRAGH;
extern GPE_Font * FONT_TOOLTIP;
extern GPE_Font * FONT_STATUSBAR;

void GPE_change_cursor(SDL_SystemCursor id);
bool load_fonts();
void cleanup_fonts();

class GPE_Overlay_System
{
    private:
        GPE_Texture * toolTipTexture;
        int toolTipX, toolTipY;
        int toolTip_halign, toolTip_valign;
        GPE_Texture * previousScreenshot;
        bool tipTipJustUpdated;
        std::string tempUserMessageTitle;
        std::string tempUserMessageSubtitle;
        std::string tempUserMessageText;
        bool tempTopLeftMessage;
        float temporayMessageDisplayTimer;
        float temporayMessageDisplayStartTimer;
    public:
        std::string toolTipString;
        GPE_Overlay_System();
        ~GPE_Overlay_System();
        bool overhead_used();
        void process_cursor();

        void update_tooltip(std::string newTip);
        void render_frozen_screenshot(GPE_Renderer * renderTarget = NULL);
        void render_temporary_message(GPE_Renderer * renderTarget = NULL);
        void render_tooltip(GPE_Renderer * renderTarget = NULL,int xPos = -1, int yPos = -1 );
        void take_frozen_screenshot(GPE_Renderer * renderTarget = NULL);
        void update_temporary_message(std::string mTitle="", std::string mSubTitle="", std::string mText="", float messDuration = 3, bool topLeftMessage = true );

};

extern GPE_Overlay_System * MAIN_OVERLAY;

class GPE_ScrollBar_XAxis
{
    protected:
        bool isHeldOn;
        bool isOnScrollBox;
        double mouseXScrolPos;
        bool hasMoved;
    public:
        GPE_Rect barBox;
        GPE_Rect contextRect;
        GPE_Rect fullRect;
        double scrollWidth, scrollPercent, scrollPixels, scrollXPos, scrollDisplayPixels;
        bool isNeeded;
        GPE_ScrollBar_XAxis();
        ~GPE_ScrollBar_XAxis();
        int get_box_width();
        int get_box_height();
        bool has_moved();
        bool is_held();
        bool is_scrolling();
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool autoCorrect = true);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void reset_scroller();
};

class GPE_ScrollBar_YAxis
{
    protected:
        bool isHeldOn;
        bool isOnScrollBox;
        double mouseYScrolPos;
        bool hasMoved;
    public:
        bool documentActions;
        GPE_Rect barBox;
        GPE_Rect contextRect;
        GPE_Rect fullRect;
        double scrollHeight, scrollPercent, scrollYPos, scrollDisplayPixels;
        bool isNeeded;
        GPE_ScrollBar_YAxis();
        ~GPE_ScrollBar_YAxis();
        int get_box_width();
        int get_box_height();
        bool has_moved();
        bool is_held();
        bool is_scrolling();
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool autoCorrect = true);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void reset_scroller();
};

class GPE_GeneralGuiElement
{
    protected:
        GPE_Rect barBox;
        bool clickedOutside;
        bool isClicked;
        bool isRightClicked;
        bool isEnabled;
        bool isHovered;
        bool isModified;
        bool isInUse;
        std::string opName;
        std::string guiListTypeName;
        int guiListTypeId;
        int maxWidth, maxHeight;
    public:
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
        GPE_GeneralGuiElement();
        virtual ~GPE_GeneralGuiElement() =0;
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
        bool is_hovered();
        void set_clicked( bool clickedVal = true);
        void set_rightclicked( bool clickedVal = true);
        virtual void set_height(int newHeight);
        void set_hovered(bool hoverVal);
        virtual void set_width(int newWidth);
        virtual void set_coords(int newX = -1, int newY = -1);
        virtual void prerender_self(GPE_Renderer * cRender = NULL);
        virtual void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect *cam = NULL);
        virtual void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        virtual void set_name(std::string newName);
        virtual void set_max_width( int nMW);
        virtual void set_max_height( int nMH);
        //Modifable properties
        bool is_inuse();
        bool is_modified();
        void switch_inuse(bool newUse);
        void update_paragraph(int foundMaxWidth = -1 );
};

class GPE_GuiElementList: public GPE_GeneralGuiElement
{
    public:
        float tabDelay;
        float leserKeyDelay;
        float greaterKeyDelay;
        float upDelay;
        float downDelay;
        float leftDelay;
        float rightDelay;
        GPE_Rect entireBox, cameraBox, menuBox;
        GPE_ScrollBar_XAxis * xScroll;
        GPE_ScrollBar_YAxis * yScroll;
        int hAlign, vAlign;
        std::vector < GPE_GeneralGuiElement * > subOptions;
        bool isInUse;
        bool hideXScroll;
        bool hideYScroll;
        bool subElementsHasArrowControl;
        bool subElementsIsScrolling;
        int barXPadding, barYPadding;
        int barXMargin, barYMargin;
        int optionWithViewId;
        GPE_GuiElementList();
        ~GPE_GuiElementList();
        void add_gui_element(GPE_GeneralGuiElement *  newElement, bool isNLElement );
        void clear_list();
        void scroll_left(int xToMove );
        void scroll_right(int xToMove );
        void scroll_down(int yToMove );
        void scroll_up(int yToMove );
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void reset_self();
        void set_horizontal_align(int hValue);
        void set_full_width();
        void set_maxed_out_width();
        void set_maxed_out_height();
};

class GPE_Scroller_X: public GPE_GeneralGuiElement
{
    protected:
        bool isOnScrollBox;
        int mouseXScrolPos;
        bool hasMoved;
    public:
        GPE_Rect barBox;
        GPE_Rect contextRect;
        GPE_Rect fullRect;
        double scrollWidth, scrollPercent, scrollPixels, scrollXPos, scrollDisplayPixels;
        bool isNeeded;
        GPE_Scroller_X();
        ~GPE_Scroller_X();
        int get_box_width();
        int get_box_height();
        bool has_moved();
        bool is_scrolling();
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void reset_scroller();
};

class GPE_Scroller_Y: public GPE_GeneralGuiElement
{
    protected:
        bool isOnScrollBox;
        int mouseYScrolPos;
        bool hasMoved;
    public:
        GPE_Rect barBox;
        GPE_Rect contextRect;
        GPE_Rect fullRect;
        double scrollHeight, scrollPercent, scrollYPos, scrollDisplayPixels;
        bool isNeeded;
        GPE_Scroller_Y();
        ~GPE_Scroller_Y();
        int get_box_width();
        int get_box_height();
        bool has_moved();
        bool is_scrolling();
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void reset_scroller();
};

class GPE_TabManager: public GPE_GeneralGuiElement
{
    private:
        bool openContextMenu;
        GPE_Rect tabBox;
        GPE_Rect tabHeaderBox;
        std::vector <generalGameResource *> subOptions;
    public:
        int barXPadding, barYPadding;
        int tabToClose;
        int tabSize;
        int tabXPAdding;
        int miniTabSize;
        int extraSmallSize;
        int smallTabSize;
        int medTabSize;
        int largeTabSize;
        int extraLargeTabSize;
        int xxLargeTabSize;
        int tabsPerView;
        int tabPos;
        int tabXHover;
        int tabInUse;
        GPE_TabManager();
        ~GPE_TabManager();

        void add_new_tab(generalGameResource * newTabResource);
        generalGameResource * get_selected_resource();
        void close_resource_tab( std::string projectParentName, int resIdIn);
        void close_tab( int tabIdToClose);
        void close_tabs_left( int tabIdToClose);
        void close_tabs_right( int tabIdToClose);
        void close_tabs_from_project( std::string projectDirNameIn);
        void close_all_tabs();
        void move_to_tab(int newTabId);
        void process_tabbar();
        void render_tabbar(GPE_Renderer * rendTarget, GPE_Rect * cam = NULL,bool forceRedraw = true);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void set_coords(int newX, int newY);
        void set_height(int newHeight);
        void set_width(int newWidth);
        void update_tabsizes();
};

//Essentially the same as the GPE_TabManager class, but tabs can not be closed by clicking an "X"
class GPE_TabBar: public GPE_GeneralGuiElement
{
    private:
        std::vector <std::string> subOptions;
    public:
        bool tabIsRightClicked;
        int fontTextWidth, fontTextHeight;
        int barXPadding, barYPadding;
        int tabSize;
        int miniTabSize;
        int extraSmallSize;
        int smallTabSize;
        int medTabSize;
        int largeTabSize;
        int extraLargeTabSize;
        int xxLargeTabSize;
        int tabsPerView;
        int tabPos;
        int tabInUse;
        bool isInUse;
        GPE_TabBar();
        ~GPE_TabBar();
        void add_new_tab(std::string newOption, bool switchToNew = true);
        void open_tab(int tabId);
        void open_tab(std::string tabName);
        std::string get_selected_name();
        void process_self(GPE_Rect * viewedSpace=NULL, GPE_Rect *cam = NULL);
        void render_self(GPE_Renderer * rendTarget, GPE_Rect * viewedSpace=NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
};

class GPE_StatusBar: public GPE_GeneralGuiElement
{
    private:
        std::string codeEditorStatusBarString;
        std::string insertModeString;
    public:
        GPE_StatusBar();
        ~GPE_StatusBar();
        void process_self(GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace=NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void reset_statusbar();
        void set_codestring(std::string newCodeString);
        void set_insertmode(std::string newInsertMode);
};

extern GPE_StatusBar * GPE_Main_Statusbar;

class GPE_Log_Entry
{
    public:
        std::string projectLogBelongsTo;
        std::string resourceNameRelated;
        std::string functionName; //where applicable
        std::string logText;
        int lineToReach, characterToReach;
        GPE_Log_Entry(std::string projectName, std::string resName, std::string logTxt, std::string funcName = "", int lineNumb = -1, int charNumb = -1);
        ~GPE_Log_Entry();
};

class GPE_PopUpMenu_Option
{
    private:
        bool isContext;
        bool showShortCuts;
        std::string opName;
        int opId;
        GPE_Sprite * opSprite;
        GPE_Texture * opTexture;
        int spriteFrameNumber;
        int startYPos;
        int rowsInView;
        int maxRowsInView;
        std::vector <GPE_PopUpMenu_Option *> subOptions;
        bool isSelectable;
        int optionWidth;
    public:
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
        void set_image_data(GPE_Sprite * newSprite,int newId);
        void set_position(int xPos = -1, int yPos = -1);
        void set_width(int newWidth);
        void set_texture(GPE_Texture * newTexture);
        GPE_PopUpMenu_Option * add_menu_option( std::string name, int id = -1, GPE_Texture * gTexture = NULL,int spriteImgNumb = -1,GPE_Sprite * gSprite=NULL, bool endsSection=false, bool selectable = true, bool isResource = false, int kbS1 = -1, int kbS2 = -1, int kbS3 = -1 );
        void open_menu();
        void prerender_self( GPE_Renderer * cRender = NULL);
        int process_menu_option();
        void update_selectability(bool selectable);
        void render_self(GPE_Renderer * cRender, GPE_Rect * cam = NULL, bool forceRedraw = true);
};

extern GPE_PopUpMenu_Option * MAIN_CONTEXT_MENU;

void GPE_open_context_menu(int menuXPos=-1,int menuYPos=-1);
void GPE_close_context_menu();
int get_popupmenu_result(GPE_Rect * camera = NULL, bool redrawScreen = false);

class GPE_Toolbar: public GPE_GeneralGuiElement
{
    private:
        GPE_Color *textColor;
        int barWidthTotal;
        bool toolBarIsOpen;
        std::vector <GPE_PopUpMenu_Option *> barOptions;
    public:
        float downDelay;
        float upDelay;
        float leftDelay;
        float rightDelay;
        int hoverOption;
        int selectedOption;
        GPE_Toolbar();
        GPE_Toolbar(std::string bName, GPE_Rect bRect);
        ~GPE_Toolbar();
        void close_toolbar();
        bool is_open();
        void reset_options();
        void set_width(int newWid);
        GPE_PopUpMenu_Option * add_menu_option( std::string name, int id = -1);
        void open_toolbar();
        void prerender_self( GPE_Renderer * cRender = NULL);
        void process_toolbar();
        void render_toolbar(GPE_Renderer *rendTarget=NULL, GPE_Rect *renderCam = NULL, bool forceRedraw = false);
};

class GPE_DropDown_Menu: public GPE_GeneralGuiElement
{
    private:
        std::vector <GPE_KeyPair *> subOptions;
        int widestOptionSpace;
        int maxOptionsAllowed;
        std::string dropdownName;
        int opId;
        bool isSelectable;
        int selectedId;
        bool isOpen;
        bool justActivated;
        bool showJustOptions;
    public:
        bool subMenuIsOpen;
        GPE_Color *barColor;
        GPE_DropDown_Menu(int xPos = 0, int yPos = 0,std::string name = "", bool justOptions = false);
        ~GPE_DropDown_Menu();

        std::string get_data();
        void load_data(std::string dataString);
        void remove_data(std::string dataString);

        void add_menu_option(std::string optionName, std::string optionSubStr= "",double optionValue = 0, bool selectOption = false);
        std::string get_menu_option(int atNumb);
        std::string get_plain_string();
        int get_selected_id();
        std::string get_selected_tag();
        std::string get_selected_name();

        double get_selected_value();

        int get_menu_size();
        bool just_activated();
        void remove_option(std::string optionToRemove);
        void reset_suboptions();
        void set_id(int newId);
        void set_value(int valueToFind);
        void show_just_options(bool justOptions);
        void process_self(GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace=NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_name(std::string newName);
        void set_selection(int newId, bool autoCorrect = true);
};

class GPE_ToolIconButton: public GPE_GeneralGuiElement
{
    private:
        bool endsSection;
        int opId;
        GPE_Texture * buttonTexture;
    public:
        bool isTabbed;
        bool usesTabs;
        GPE_ToolIconButton(int buttonX = 0, int buttonY = 0,std::string buttonImgFile="",std::string name="", int id = -1, int buttonSize = 32, bool lastInCol = false);
        ~GPE_ToolIconButton();
        std::string get_data();
        void load_data(std::string dataString);
        bool ends_section();
        int get_id();
        void set_id(int newId);
        void process_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void set_image( std::string buttonImgFile);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
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
        GPE_ToolIconButtonBar(int xPos, int yPos,int buttonSize = 32, bool useTabs = false);
        ~GPE_ToolIconButtonBar();
        std::string get_data();
        void load_data(std::string dataString);
        int get_tab_pos();
        int get_tab_id();
        void limit_width(bool isLimited = false);
        void set_width(int newWid);
        GPE_ToolIconButton * add_option( std::string buttonImgFile,std::string name, int id = -1, bool endsSection = false);
        void process_self(GPE_Rect * viewedSpace =NULL, GPE_Rect *renderCam = NULL);
        void render_self(GPE_Renderer *cRender=NULL, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_coords(int newX = -1, int newY = -1);
        void set_height(int newXH);
        void set_selection(int newSelection);
};

class GPE_ToolLabelButton: public GPE_GeneralGuiElement
{
    private:
        int opId;
    public:
        GPE_ToolLabelButton(int buttonX, int buttonY,std::string name, std::string description,int id = -1, int buttonSize = 24);
        ~GPE_ToolLabelButton();
        void prerender_self(GPE_Renderer * cRender);
        void process_self(GPE_Rect * viewedSpace =NULL, GPE_Rect *renderCam = NULL);
        void set_id(int newId);
        void set_name(std::string newName);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
};

class GPE_ToolPushButton: public GPE_GeneralGuiElement
{
    private:
        int opId;
        GPE_Texture * opTexture;
        int iconPadding;
        bool wasClicked;
        std::string webUrl;
        bool showBackground, showBorder;
    public:
        GPE_ToolPushButton(int buttonX, int buttonY,std::string imgLocation,std::string name, std::string description,int id = -1, int buttonSize = 24);
        ~GPE_ToolPushButton();
        void change_texture( GPE_Texture * newTexture);
        void enable_background( bool enableValue);
        void enable_border( bool enableValue);
        void set_id(int newId);
        void set_name(std::string newName);
        void set_website(std::string urlIn);
        void prerender_self(GPE_Renderer * cRender);
        void process_self(GPE_Rect * viewedSpace =NULL, GPE_Rect *renderCam = NULL);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
};

class GPE_ToolPushButtonMultiLine: public GPE_GeneralGuiElement
{
    private:
        int opId;
        GPE_Texture * opTexture;
        int iconPadding;
        bool wasClicked;
        std::string webUrl;
        bool showBackground, showBorder;
    public:
        int lineHeight;
        std::vector< std::string> buttonLines;
        std::string stringValue;
        GPE_ToolPushButtonMultiLine(int buttonX, int buttonY,std::string imgLocation,std::string parsedLines, std::string strValue,int id = -1, int buttonSize = 24);
        ~GPE_ToolPushButtonMultiLine ();
        void change_texture( GPE_Texture * newTexture);
        void enable_background( bool enableValue);
        void enable_border( bool enableValue);
        void set_id(int newId);
        void set_name(std::string newName);
        void set_website(std::string urlIn);
        void prerender_self(GPE_Renderer * cRender);
        void process_self(GPE_Rect * viewedSpace =NULL, GPE_Rect *renderCam = NULL);
        void render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
};

class GPE_SelectBoxBasic_Option
{
    public:
        bool sectionHasContent;
        bool useGuiColor;
        bool isChecked;
        std::string optionName;
        GPE_Texture * optionTexture;
        GPE_Sprite * optionSprite;
        int subimageIndex;
        double optionValue;
        GPE_SelectBoxBasic_Option();
        ~GPE_SelectBoxBasic_Option();
        void prerender_self(GPE_Renderer * cRender = NULL);
};

class GPE_SelectBoxBasic: public GPE_GeneralGuiElement
{
    private:
        std::vector <GPE_SelectBoxBasic_Option * > subOptions;
        int pos;
        int startPos;
        int maxOptionsInView;
        int maxHeight;
        int optionHeight;
        bool showHideOthersCheckboxToggle;
    public:
        int downDelay;
        int upDelay;
        bool showCheckboxes;
        int optionIconWidth;
        GPE_SelectBoxBasic(int x = 0, int y = 0, std::string name = "");
        ~GPE_SelectBoxBasic();
        std::string get_data();
        void load_data(std::string dataString);
        void add_option(std::string newOptionName, double newOpValue = -1,GPE_Texture * evRepIcon = NULL, GPE_Sprite * evRepSprite=NULL, int subimageInIndex = 0, bool selectNew = false, bool useGuiColor = true);
        void clear_list();
        std::string get_selected_name();
        double get_selected_value();
        int get_selection();
        int get_size();
        GPE_SelectBoxBasic_Option * get_option(int optionId);
        std::string get_option_name(int optionId);
        void insert_option(int optionId, std::string newName, GPE_Texture * evRepIcon = NULL, GPE_Sprite * evRepSprite=NULL, int subimageInIndex = 0,bool selectNew = false);
        void limit_height(int newH);
        void alter_content( int optionId, bool sectionHasContent);
        void alter_content_from_value( double valueId, bool sectionHasContent);
        bool move_down_space();
        bool move_up_space();
        void prerender_self(GPE_Renderer * cRender = NULL);
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void rename_option(int optionId, std::string newName);
        void remove_option(int optionId);
        void set_option_height( int newOptionHeight);
        void set_selected_option( std::string optionToSelect);
        void set_selection( int selId);
        void toggle_others_checked();
};

class GPE_Input_Field_Color: public GPE_GeneralGuiElement
{
    protected:
        int prevBarBoxW, prevBarBoxH;
        GPE_Color * storedColor;
        int fontSizeW, fontSizeH;
    public:
        GPE_Rect fieldBarBox;
        std::string inputLabel;
        bool showLabel;
        GPE_Input_Field_Color(int newX = 0, int newY = 0, int boxW=-1, int boxH=-1,std::string startName="",std::string placeHolderText="");
        ~GPE_Input_Field_Color();
        std::string get_data();
        void load_data(std::string dataString);
        GPE_Color *get_color();
        std::string get_hex_string();
        std::string get_rgb_string();
        std::string get_plain_string();
        int get_r();
        int get_g();
        int get_b();
        void set_label(std::string newLabel);
        void set_name(std::string newName);
        void set_string(std::string newString);
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_color_from_rgb(GPE_Color *newColor);
        void set_color_from_rgb(std::string newColorStr);
        void set_color_from_hex(std::string newColorStr);
        void set_r(int r);
        void set_g(int g);
        void set_b(int b);
        void set_height( int newHeight);
        void set_rgb(int r, int g, int b);
        void set_width(int newWidth);
};

const int MAX_STORED_TEXT_PREVIOUS_ENTRIES = 32;
class GPE_TextInputBasic: public GPE_GeneralGuiElement
{
    protected:
        int prevBarBoxW, prevBarBoxH;
        std::string textInputString;
        std::string placeHolderString;
        int displayMode;
        int cursorPos;
        int prevCursorPos;
        int startXPos;
        int maxCharactersInView;
        int fontSizeW, fontSizeH;
        int cursorTimer;
        bool showCursor;
        int maxLength;
        float bscDelay;
        float delDelay;
        float enterDelay;
        float upArrowDelay;
        int downArrowDelay;
        float leftArrowDelay;
        float rightArrowDelay;
        int inputFieldPos;
        bool hasValidInput;
        int cursorHistoryPos;
        std::vector< std::string> listOfPastStrings;
        GPE_Rect fieldBarBox;
    public:
        bool inputSubmitted;
        bool resubmissionAllowed;
        std::string linkedProjectFileName;
        std::string inputLabel;
        bool showLabel;
        bool showPlaceHolder;
        int lastDoubleClickAction;
        int selectionCursorPos, selectionEndCursorPos;
        GPE_TextInputBasic(std::string startName="",std::string placeHolderText="");
        ~GPE_TextInputBasic();
        std::string get_data();
        void load_data(std::string dataString);
        void copy_selection();
        void cut_selection();
        void delete_selection();
        void get_cursor_from_mouse(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        std::string get_label();
        std::string get_plain_string();
        std::string get_string();
        bool has_content();
        bool is_valid();
        void move_left(int xToMove = 1);
        void move_right(int xToMove = 1);
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void record_string( std::string strToRecord);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void reset_selection(int resetDirection = 0);
        void select_all();
        void set_height(int newHeight);
        void set_hovered(bool hoverVal);
        void set_number( double numberIn);
        void set_max_width( int nMW);
        void set_max_height( int nMH);
        void set_width(int newWidth);
        void set_label(std::string newLabel);
        void set_name(std::string newName);
        void set_string(std::string newString);
        void set_placeholder(std::string newString);
        bool was_submitted();
};

class GPE_TextInputNumber: public GPE_TextInputBasic
{
    protected:
        bool onlyWholeNumbers;
        int heldNumber;
        int minInputNumber, maxInputNumber;
    public:
        bool forceValidNumber;
        GPE_TextInputNumber( std::string placeHolderText, bool onlyWholeNums = true, int hMin = INT_MIN+1, int hMax = INT_MAX-1);
        ~GPE_TextInputNumber();
        int get_held_number();
        std::string get_plain_string();
        int make_valid_number(int fallbackInt);
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
};

const int GPE_ANCHOR_REGULAR = 0;
const int GPE_ANCHOR_ERROR = 1;
const int GPE_ANCHOR_WARNING = 2;
const int GPE_ANCHOR_COMMENT = 3;

class GPE_TextAnchor: public GPE_GeneralGuiElement
{
    public:
        int anchorType, lineNumber, characterNumber;
        std::string lineMessage;
        std::string lineAlert;
        std::string anchorProjectName;
        std::string anchorProjectResourceName;
        int anchorProjectResourceId;
        GPE_TextAnchor(int lineN, int charN, std::string messageIn, std::string alertInfo, int anchorType = 0);
        ~GPE_TextAnchor();
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);

};

class GPE_Label_Image: public GPE_GeneralGuiElement
{
    private:
        bool isHoldingCustomTexture;
        GPE_Texture * imgSrc;
    public:
        bool allowOverSize;
        float resizeAspect;
        bool wasClicked;
        std::string webUrl;
        GPE_Label_Image(GPE_Texture * imgIn = NULL);
        GPE_Label_Image(GPE_Texture * imgIn,std::string nameIn, std::string description, std::string urlIn);
        ~GPE_Label_Image();
        void load_label_image(std::string fileNameIn);
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_name(std::string nameIn);
        void set_image( GPE_Texture * imgIn);
        void set_width( int newW);
        void set_height( int newH);
        void set_size( int newW, int newH);
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
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
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
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
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
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_name(std::string nameIn);
};

class GPE_Label_Paragraph: public GPE_GeneralGuiElement
{
    protected:
        int defaultFontWidth;
        int defaultFontHeight;
        std::string paragraphText;
        std::vector< std::string> paragraphLines;
    public:
        GPE_Label_Paragraph(std::string nameIn, std::string paragraphIn, std::string descriptionIn = "");
        ~GPE_Label_Paragraph();
        void add_line(std::string newLine);
        void clear_all_lines();
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_name(std::string nameIn);
        void set_width(int newWidth);
        void set_height(int newHeight);
        void set_max_width( int nMW);
        void set_max_height( int nMH);
        void update_paragraph();
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
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_name(std::string nameIn);
};

class GPE_TextURL: public GPE_GeneralGuiElement
{
    public:
        bool wasClicked;
        std::string webUrl;
        std::string webId;
        GPE_TextURL(std::string nameIn="", std::string description="", std::string urlIn="" );
        ~GPE_TextURL();
        std::string get_data();
        std::string get_plain_string();
        void load_data(std::string dataString);
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_name(std::string nameIn);
};

class GPE_CheckBoxBasic: public GPE_GeneralGuiElement
{
    protected:
        int clickBoxW, clickBoxH;
        int labelBoxW, labelBoxH;
    public:
        std::string descriptionText;
        GPE_CheckBoxBasic(std::string name = "", std::string descriptor = "", int xx = 0, int yy= 0, bool isChecked = true);
        ~GPE_CheckBoxBasic();
        std::string get_data();
        void load_data(std::string dataString);
        void prerender_self(GPE_Renderer * cRender = NULL);
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_checkbox_size(int nBoxSize, bool resizeBarBox = true);
};

class GPE_CheckBoxBasicList: public GPE_GeneralGuiElement
{
    private:
        int opXPadding, opYPadding;
        int opWidth, opHeight;
        bool keepAlphabetized;
    protected:
        std::vector < std::string > subOptions;
        std::vector <bool > posIsChecked;
    public:
        bool allowDuplicates;
        GPE_CheckBoxBasicList();
        ~GPE_CheckBoxBasicList();
        std::string get_data();
        void load_data(std::string dataString);
        void add_opton(std::string newOption);
        void remove_opton(std::string optionToCut);
        void remove_opton_id(int optionToCut);
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);

};

extern GPE_Sprite * GPE_RadioButton_GFX;

class GPE_RadioButtonControllerBasic: public GPE_GeneralGuiElement
{
    private:
        int selectedId;
        int hoveredOption;
        int opXPadding, opYPadding;
        int opWidth, opHeight;
        bool keepAlphabetized;
        int numCols;
        int rowsPerCol;
    protected:
        std::vector <GPE_KeyPair *> subOptions;
    public:
        bool allowDuplicates;
        GPE_RadioButtonControllerBasic(std::string cName = "", int cX=0, int cY = 0, bool alphabetize = false, int colNum=1);
        ~GPE_RadioButtonControllerBasic();
        std::string get_data();
        void load_data(std::string dataString);
        void add_opton(std::string newOption);
        void add_menu_option(std::string optionName, std::string optionSubStr= "",int optionValue = 0, bool selectOption = false);
        std::string get_menu_option(int atNumb);
        std::string get_plain_string();
        int get_selected_id();
        std::string get_selected_tag();
        int get_selected_value();

        void organize_options();
        void remove_data(std::string dataString);
        void remove_option(std::string optionToCut);
        void remove_option_id(int optionToCut);
        void set_selection( int newSelection);
        bool set_from_tag(std::string newSelectedTag);
        void set_value( int valueToSelect);
        void prerender_self(GPE_Renderer * cRender = NULL);
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
};


const int restype_regular = -1;
const int restype_superfolder = 0;
const int restype_projfolder = 1;

class GPE_ResourceContainer
{
    protected:
        std::string opName;
        int globalResouceId;
        int resourceId;
        int resourceType;
        bool isFolder, isSuperFolder, isSuperProjectFolder;
        bool subMenuIsOpen;
        GPE_Sprite * containerSprite;
        GPE_Texture * containerTexture;
        int spriteFrameNumber;
        generalGameResource * heldResource;
        std::vector <GPE_ResourceContainer *> subOptions;
        bool resouceNameChanged;
        bool subContentsModified;
        int strTexWidth, strTexHeight;
    public:
        int foundX2Pos;
        int html5BuildGlobalId;
        std::string parentProjectDirectory;
        std::string projectParentFileName;
        std::string projectParentName;
        GPE_Rect optionBox;
        GPE_ResourceContainer * parentResource;
        GPE_ResourceContainer(std::string projFolderName, int  rezPropValue = -1);
        GPE_ResourceContainer(std::string projFolderName, std::string newName, int rType, int rId,bool folder = true, int globalIdVal = -1,int  rezPropValue = -1);
        ~GPE_ResourceContainer();
        void add_resource_container( GPE_ResourceContainer * newResource, bool changeGlobalId = false, GPE_ResourceContainer * referenceObject = NULL );
        GPE_ResourceContainer * add_resource_folder( std::string resourceTypeName, int gResId = -1, int rezPropValue = -1);
        GPE_ResourceContainer * add_newtype_folder( int rType,std::string resourceTypeName, int gResId = -1,int rezPropValue = -1);
        void delete_resource(GPE_ResourceContainer * otherContainer);
        bool detect_name_change(bool autoChange = true);
        int get_global_id();
        generalGameResource * get_held_resource();
        std::string get_name();
        int get_resource_id();
        int get_resource_type();
        GPE_ResourceContainer * find_resource_from_id(int resourceIdToFind, bool nestDown = true, bool includeSelf = true);
        GPE_ResourceContainer * find_resource_from_name(std::string rNameToFind , bool nestDown = true);
        GPE_ResourceContainer * find_resource_target(std::string rNameToFind , bool nestDown = true);
        GPE_ResourceContainer * get_usable_resource();
        void grab_useable_resources(std::vector <GPE_ResourceContainer * > &rVector);
        GPE_ResourceContainer * get_resource_at(int resourcePos, bool nestDown = false);
        std::string get_project_name();
        int get_resource_count();
        int get_resource_image_frame();
        GPE_Sprite * get_resource_sprite();
        GPE_Texture * get_resource_texture();
        int get_size();
        int get_options_width();
        void integrate_into_syntax();
        bool is_folder();
        bool is_super_folder();
        bool is_super_project_folder();
        bool is_child_of(GPE_ResourceContainer * otherContainer);
        bool can_obtain(GPE_ResourceContainer * otherContainer);
        int matches(GPE_ResourceContainer * otherContainer);
        void open_folder();
        void preprocess_container(std::string alternatePath = "", int backupId = -1);
        void prerender_self(GPE_Renderer * cRender);
        int process_container(int xPos, int yPos, int selectedId = -1, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool mouseInRange = false);
        bool read_data_from_projectfile(std::ofstream * fileTarget);
        void remove_resource(GPE_ResourceContainer * otherContainer, bool deleteResource = true);
        void save_container(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void set_basic_image_value();
        void set_global_id(int newId);
        void set_global_html5id(int newId);
        void set_held_resource(generalGameResource * newGenResource);
        void set_name(std::string newName);
        void set_project_parent_name(std::string newParentName);
        void render_contained_object(GPE_Renderer * cRender,GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_option(GPE_Renderer * cRender, int xPos, int yPos, int selectedIdNumber=-1,GPE_Rect * viewedSpace = NULL ,GPE_Rect * cam = NULL, bool renderSubOptions = true, bool renderAutomatically = false);
        void render_image(GPE_Renderer * cRender, int xPos, int yPos, int rWidth = -1, int rHeight = -1,GPE_Rect * viewedSpace = NULL ,GPE_Rect * cam = NULL, GPE_Color * renderColor = NULL);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const int RESOURCE_TYPE_PROJECT_SETTINGS = 1;
const int RESOURCE_TYPE_SPRITE = 2;
const int RESOURCE_TYPE_TEXTURE = 3;
const int RESOURCE_TYPE_TILESHEET = 4;
const int RESOURCE_TYPE_AUDIO = 5;
const int RESOURCE_TYPE_VIDEO = 6;
const int RESOURCE_TYPE_FUNCTION = 7;
const int RESOURCE_TYPE_OBJECT = 8;
const int RESOURCE_TYPE_SCENE = 9;
const int RESOURCE_TYPE_ACHIEVEMENT = 10;
const int RESOURCE_TYPE_PATH = 11;
const int RESOURCE_TYPE_FONT = 12;
const int RESOURCE_TYPE_PLUGIN = 13;

const int RESOURCE_TYPE_DICTIONARY = 13;
const int RESOURCE_TYPE_SPREADSHEET = 14;

const int RESOURCE_TYPE_EVENT = 15;
const int RESOURCE_TYPE_QUEST = 16;

const int RESOURCE_TYPE_CLASS = 17;


extern GPE_ResourceContainer * RESOURCE_TO_DRAG;
extern GPE_ResourceContainer * LAST_CLICKED_RESOURCE;
extern bool RESOURCEMENU_WAS_RIGHTCLICKED;
extern GPE_ResourceContainer * RESOURCE_BEINGRENAMED;
extern int LAST_CLICKED_RESTYPE;
extern int DRAGGED_RESTYPE;

class PAW_GUI_SEARCH_CONTROLLER
{
    public:
        //Search Settings
        int findAllStartXPos, findAllStartYPos;
        int previousSearchMode;
        int textSearchMode;
        bool showFindAllResults;
        //For Find/Replace Box of TextArea
        GPE_TextInputBasic * findTextStringBox;
        GPE_TextInputNumber * goToLineStringBox;
        GPE_TextInputBasic * replaceTextStringBox;
        GPE_ToolLabelButton * findButton;
        GPE_ToolLabelButton * findAllButton;
        GPE_ToolLabelButton * goToButton;
        GPE_ToolLabelButton * replaceButton;
        GPE_ToolLabelButton * replaceAllButton;
        GPE_CheckBoxBasic * findMatchCase;

        GPE_Label_Text * scopeLabel;
        GPE_DropDown_Menu * findScope;
        PAW_GUI_SEARCH_CONTROLLER();
        ~PAW_GUI_SEARCH_CONTROLLER();
        void close_finder();
        bool using_search();
};

extern PAW_GUI_SEARCH_CONTROLLER * MAIN_SEARCH_CONTROLLER;

GPE_Color * GPE_Get_Color_PopUp(std::string popUpCaption = "",GPE_Color * currColor = NULL);
std::string  GPE_GetPlainFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory, bool isSaving = false, bool isDirectorySearch = false);
std::string  GPE_GetDirectoryName( const std::string & prompt, std::string &previousDirectory, bool isSaving = false);
std::string  GPE_GetOpenFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );
std::string  GPE_GetSaveFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );
int GPE_GetMessageBoxResult(const std::string title, const std::string prompt, int flagType);

void display_user_messaage();

void display_user_alert(std::string messageTitle, std::string messageContent, int messageType = 0,GPE_Rect * camera = NULL );
int display_get_prompt(std::string messageTitle, std::string messageContent, bool showCancelButton = false );
std::string get_string_from_popup(std::string messageTitle, std::string messageContent, std::string startName = "");

//Tab Related Functions

bool has_early_tab(std::string strIn);
std::string untab_string(std::string strIn);
std::string generate_tabs(int tabCount);
int get_tab_space_count();
#endif
