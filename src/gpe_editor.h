/*
gpe_editor.h
This file is part of:
GAME PENCI ENGINE
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

#ifndef GPE_EDITOR_H
#define GPE_EDITOR_H

#include "GPE_CIncludes.h"
#include "shared_resources.h"
#include "functions.h"
#include "GPE_Engine.h"
#include "program_state.h"
#include "stateManager.h"
#include "SDL2/SDL_version.h"
#include "timer.h"

extern bool GPE_VERSION_COMMERCIAL_LICENSE;

const int GPE_MAINICON_SPRCOUNT = 74;


extern int POPUP_FONT_SIZE_WIDTH;
extern int POPUP_FONT_SIZE_HEIGHT;
extern int POPUP_MENU_VALUE;

const int SPRITE_DATA_FIELD_COUNT = 7;
extern std::string spriteDataLabels[SPRITE_DATA_FIELD_COUNT];
extern int spriteDataLabelWidth;

const int TILESHEET_DATA_FIELD_COUNT = 6;
extern std::string tsDataLabels[TILESHEET_DATA_FIELD_COUNT];
extern int tsDataLabelWidth;

extern std::string CURRENT_PROJECT_NAME;

typedef void (*CallbackType)(void);

class GPE_FileFilter
{
    public:
        std::string desc;
        std::string exts;
        GPE_FileFilter(std::string fDesc = "", std::string fExts = "");
        ~GPE_FileFilter();
};

std::string generate_filters(const std::vector<GPE_FileFilter *> filters);
std::string parse_file_types(std::string fileTypesIn, std::vector <std::string> &fileTypeVector);
bool file_passes_filter(std::string fileTypeIn, std::vector <std::string> &fileTypeVector);

class GPE_File
{
    private:
        std::string fileName;
        std::string fileType;
        bool isDirectory;
    public:
        std::string fileLocationDirectory;
        GPE_Texture * fileThumbnail;
        int fileSizeInfo;
        std::string dataCreatedInfo;
        std::string dataModifiedInfo;
        GPE_File(std::string fName,bool fileIsDirectory);
        ~GPE_File();
        bool is_directory();
        std::string get_name();
        std::string get_type();
};

class GPE_FileDirectory
{
    private:
        std::vector< GPE_File * >myFiles;
        int fdPosition;
        int fileCount;
        int subdirectoryCount;
    public:
        GPE_FileDirectory();
        ~GPE_FileDirectory();
        bool open_directory(std::string directoryIn);
        bool open_directory_sorted(std::string directoryIn);
        void close_directory();
        void filter_directory(bool onlyFolders, std::vector <std::string> &fTypesToUse);
        GPE_File * get_file(int position);
        GPE_File * get_next_file();
        bool has_next_file();
        int get_count();
        int get_subdirectory_count();
        int get_file_count();
        bool set_position(int position);
};

typedef enum
{
    CTERM_VARIABLE,
    CTERM_CONSTANT,
    CTERM_KEYWORD,
    CTERM_FUNCTION,
    CTERM_CLASS
}compileTermTypes;

class GPE_Compiler_Term
{
    private:
        std::string termParametersString;
    public:
        std::string termString;
        std::vector < std::string > termParameters;
        std::string termFunctionReturnType;
        std::string termDescription;
        std::string termScope;
        int termType;
        GPE_Compiler_Term();
        GPE_Compiler_Term(std::string tName,std::string tParameters, std::string tFunctionReturnType ,std::string tDescription, int tType = -1, std::string tScope="");
        ~GPE_Compiler_Term();
        std::string get_parameters();
};

class GPE_Gui_Engine_Language
{
    public:
        std::vector <GPE_Compiler_Term *> languageVariables;
        std::vector <GPE_Compiler_Term *> languageConstants;
        std::vector <GPE_Compiler_Term *> languageFunctions;
        std::vector <GPE_Compiler_Term *> languageClasses;
        std::vector <GPE_Compiler_Term *> languageKeywords;
        std::vector <GPE_Compiler_Term *> languageDataTypes;
        std::vector <char> symbolLibrary;
        std::string commentChar;
        std::string blockcommentStartStr;
        std::string blockcommentEndStr;
        std::string languageName;
        std::string languageShortName;
        bool isCodingLanguage;
        GPE_Gui_Engine_Language(std::string langName, std::string langShortName, bool projectCodeLanguage = true, bool useDefaultSymbols = true);
        ~GPE_Gui_Engine_Language();
        bool add_language_variable(std::string nName, std::string nDescription = "", std::string nScopre = "");
        bool add_language_constant(std::string nName, std::string nDescription = "", std::string nParameters = "");
        bool add_language_function(std::string nName, std::string nDescription, std::string nParameters, std::string tFunctionReturnType, std::string nScope);
        bool add_language_class(std::string nName, std::string nDescription = "", std::string nParameters = "");
        bool add_language_keyword(std::string nName, std::string nDescription = "", std::string nScope = "");
        bool add_language_data_type(std::string nName, std::string nDescription = "", std::string nParameters = "");

        void erase_language_variable(std::string nName);
        void erase_language_constant(std::string nName);
        void erase_language_function(std::string nName);
        void erase_language_class(std::string nName);
        void erase_language_keyword(std::string nName);
        void erase_language_data_type(std::string nName);

        std::string find_gpe_datatype(std::string strIn, int startPos = 0);
        std::string find_gpe_function(std::string strIn, int startPos = 0);
        std::string find_gpe_keyword(std::string strIn, int startPos = 0);
        std::string find_gpe_variable(std::string strIn, int startPos = 0);

        bool is_reserved_word(std::string strIn);
        bool is_compilable_word(std::string strIn);

        bool charIsSymbol( char  charIn);
        int firstSymbolInString(std::string inString, int pos=0);
        GPE_Compiler_Term * find_matching_variable(std::string nameIn);
        GPE_Compiler_Term * find_matching_function(std::string nameIn);
};

class GPE_Gui_Engine_MiniCompiler
{
    public:
        std::vector <std::string> foundParameters;
        std::vector <std::string> defaultParameterLines;
        std::string newParametersString;
        std::vector <std::string> functionParameterErrors;
        bool parametersAreValid;
        std::vector <std::string> compilerErrors;
        std::vector <std::string> compilerWarnings;
        std::vector <GPE_Gui_Engine_Language * > editorLanguages;
        GPE_Gui_Engine_Language *defaultLanguage;
        //creates the vector for all of the symbols

        GPE_Gui_Engine_MiniCompiler();
        ~GPE_Gui_Engine_MiniCompiler();

        GPE_Gui_Engine_Language * add_programming_language(std::string langName, std::string langShortName, bool useDefaultSymbols = true,bool  isCodingLanguage = true, bool  isDefaultCodeLanguage= false);
        bool is_reserved_word(std::string strIn);
        bool is_compilable_word(std::string strIn);

        bool charIsSymbol( char  charIn);
        int firstSymbolInString(std::string inString, int pos=0);

        std::string find_gpe_datatype(std::string strIn, int startPos = 0);
        std::string find_gpe_function(std::string strIn, int startPos = 0);
        std::string find_gpe_keyword(std::string strIn, int startPos = 0);
        std::string find_gpe_variable(std::string strIn, int startPos = 0);
        GPE_Compiler_Term * find_matching_variable(std::string nameIn);
        GPE_Compiler_Term * find_matching_function(std::string nameIn);
        bool process_parameters_string( std::string paramToProcess);
        void begin_compiling();

};

extern GPE_Gui_Engine_MiniCompiler * GPE_MINI_COMPILER;


std::string  GPE_GetPlainFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory, bool isSaving = false, bool isDirectorySearch = false);
std::string  GPE_GetDirectoryName( const std::string & prompt, std::string &previousDirectory, bool isSaving = false);
std::string  GPE_GetOpenFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );
std::string  GPE_GetSaveFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );
int GPE_GetMessageBoxResult(const std::string title, const std::string prompt, int flagType);
void GPE_OpenProgram(std::string programLocation, std::string programInfo="", bool openProgrmaInfoOnFail = false);
void GPE_OpenURL(std::string urlIn);
GPE_Color * GPE_Get_Color_PopUp(std::string popUpCaption = "",GPE_Color * currColor = NULL);
void GPE_Show_Tip_Of_Day();
bool GPE_Open_Support_Center();

SDL_Rect * GPE_find_camera(SDL_Rect * rectIn=NULL);

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

extern std::string displayMessageTitle;
extern std::string displayMessageSubtitle;
extern std::string displayMessageString;

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

typedef enum
{
    gpv_int,
    gpv_float,
    gpv_double,
    gpv_char,
    gpv_string,
    gpv_array,
    gpv_vector,
    gpv_gameobject
}gpvTypes;

class gpv
{
    private:
        int actualDataType;
        int heldInt;
        float heldFloat;
        double heldDouble;
        char heldChar;
        std::string heldString;
        std::vector < gpv * > heldArray;
        std::vector < gpv * > heldVector;
        //GPE_GameObject * heldObject;
     public:
        gpv();
        ~gpv();
        gpv & operator=(const gpv &other);
        gpv & operator=(int other);
        gpv & operator=(std::string other);
        gpv & operator=(double other);
        gpv & operator=(float other);
        gpv & operator=(char other);
};

class GPE_KeyPair
{
    public:
        double keyValue;
        std::string keyString;
        std::string keySubString;
        GPE_KeyPair(double idNumb, std::string str, std::string ksubstr ="");
        ~GPE_KeyPair();
};

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

extern std::string GPE_Action_Message;
extern int GPE_Action_ID;
extern bool GPE_ACTION_IS_CONTEXT;
void update_action_message(std::string newMessage);

void update_popup_info(std::string newMessage="", int selectedOpId=-1, bool popIsContext = false);

extern generalGameResource * SELECTED_GENERAL_RESOUCE;
void GPE_set_selected_gresource(generalGameResource * newResource = NULL);
generalGameResource * GPE_get_selected_gresource();

struct GPE_Rect
{
    public:
        double x;
        double y;
        double w;
        double h;
        GPE_Rect()
        {
            x = 0;
            y = 0;
            w = 0;
            h = 0;
        }
};

void update_rectangle(GPE_Rect * rectIn, double nx, double ny, double nw, double nh);

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
        void process_self(SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL, bool autoCorrect = true);
        void render_self(GPE_Renderer * cRender, SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL);
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
        void process_self(SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL, bool autoCorrect = true);
        void render_self(GPE_Renderer * cRender, SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL);
        void reset_scroller();
};

class GPE_GeneralGuiElement
{
    protected:
        SDL_Rect barBox;
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
        virtual void prerender_self(GPE_Renderer * cRender=NULL);
        virtual void process_self(SDL_Rect *viewedSpace=NULL, SDL_Rect *cam=NULL);
        virtual void render_self(GPE_Renderer * cRender=NULL,SDL_Rect *viewedSpace=NULL, SDL_Rect *cam=NULL, bool forceRedraw = true);
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
        SDL_Rect entireBox, cameraBox, menuBox;
        GPE_ScrollBar_XAxis * xScroll;
        GPE_ScrollBar_YAxis * yScroll;
        int hAlign, vAlign;
        std::vector < GPE_GeneralGuiElement * > subOptions;
        bool isInUse;
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
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void reset_self();
        void set_horizontal_align(int hValue);
        void set_full_width();
        void set_maxed_out_width();
        void set_maxed_out_height();
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
        SDL_Rect optionBox;
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
        void integrate_into_synthax();
        bool is_folder();
        bool is_super_folder();
        bool is_super_project_folder();
        bool is_child_of(GPE_ResourceContainer * otherContainer);
        bool can_obtain(GPE_ResourceContainer * otherContainer);
        int matches(GPE_ResourceContainer * otherContainer);
        void open_folder();
        void preprocess_container(std::string alternatePath = "", int backupId = -1);
        void prerender_self(GPE_Renderer * cRender);
        int process_container(int xPos, int yPos, int selectedId = -1, SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL, bool mouseInRange = false);
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
        void render_contained_object(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_option(GPE_Renderer * cRender, int xPos, int yPos, int selectedIdNumber=-1,SDL_Rect * viewedSpace = NULL ,SDL_Rect *cam = NULL, bool renderSubOptions = true, bool renderAutomatically = false);
        void render_image(GPE_Renderer * cRender, int xPos, int yPos, int rWidth = -1, int rHeight = -1,SDL_Rect * viewedSpace = NULL ,SDL_Rect *cam = NULL, GPE_Color * renderColor = NULL);
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

const int res_type_count = 20;
extern std::string RESOURCE_TYPE_NAMES[res_type_count];
extern std::string GPE_BUILD_NAMES[GPE_BUILD_OPTIONS];

extern GPE_ResourceContainer * RESOURCE_TO_DRAG;
extern GPE_ResourceContainer * LAST_CLICKED_RESOURCE;
extern bool RESOURCEMENU_WAS_RIGHTCLICKED;
extern GPE_ResourceContainer * RESOURCE_BEINGRENAMED;
extern int LAST_CLICKED_RESTYPE;
extern int DRAGGED_RESTYPE;

const int PROJECT_LANGUAGE_JS = 0;
const int PROJECT_LANGUAGE_CS = 1;
const int PROJECT_LANGUAGE_CPP = 2;
const int PROJECT_LANGUAGE_TST = 3;
const int PROJECT_LANGUAGE_LUA = 4;
const int PROJECT_LANGUAGE_MAX = 5;

extern std::string PROJECT_LANGUAGE_NAMES[PROJECT_LANGUAGE_MAX];

class GPE_ProjectFolder
{
    private:
        std::string projectName;
        std::string projectStartDirectoryName;
        std::string projectDirectory;
        std::string projectFileName;
        int GLOBAL_REZ_ID_COUNT;
        double projectFilePreviousVersion;
        double projectFileVersion;
    public:
        std::string projectIconName;
        int myProjectLanguage;
        GPE_ResourceContainer * RESC_PROJECT_FOLDER;
        GPE_ResourceContainer * RESC_SPRITES;
        GPE_ResourceContainer * RESC_TEXTURES;
        GPE_ResourceContainer * RESC_TILESHEETS;
        GPE_ResourceContainer * RESC_3DMODELS;
        GPE_ResourceContainer * RESC_AUDIO;
        GPE_ResourceContainer * RESC_VIDEOS;
        GPE_ResourceContainer * RESC_FUNCTIONS;
        GPE_ResourceContainer * RESC_OBJECTS;
        GPE_ResourceContainer * RESC_PATHS;
        GPE_ResourceContainer * RESC_DICTIONARIES;
        GPE_ResourceContainer * RESC_SCENES;
        GPE_ResourceContainer * RESC_ACHIEVEMENTS;
        GPE_ResourceContainer * RESC_FONTS;
        GPE_ResourceContainer * RESC_SHADERS;
        GPE_ResourceContainer * RESC_PROJECT_SETTINGS;
        GPE_ResourceContainer * RESC_ALL[res_type_count];
        int CREATED_RESOURCE_COUNT[res_type_count];

        //Used for code editor and hightlights of functions, resources and objects.
        std::vector <GPE_Compiler_Term *> projectFunctions;
        std::vector <GPE_Compiler_Term *> projectKeywords;

        //used for buld scripts
        std::vector <int >currentObjParents;
        std::vector <int> nextObjParents;
        std::vector <int> finalObjParents;

        time_t lastTimeBackedUp;
        std::string projectLayerNames[32];

        GPE_ProjectFolder(std::string name, std::string directoryIn, std::string fileNameIn);
        ~GPE_ProjectFolder();

        //Used for synthax highlights and such
        bool add_project_function(std::string nName, std::string nDescription = "", std::string nParameters = "", std::string tFunctionReturnType = "", std::string nScope="");
        bool add_project_keyword(std::string nName, std::string nDescription = "", int nType = -1, std::string nScope="");
        void clear_project_functions();
        void clear_project_keywords();
        std::string find_project_function(std::string strIn, int startPos = 0);
        std::string find_project_keyword(std::string strIn, int startPos = 0);
        //Creating Resources and such
        bool clean_build_folder( int buildMetaTemplate = -1 );
        //creates sub folders all sexy like
        GPE_ResourceContainer * create_blank_folder(GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_resource(int rNewType = -1,GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_audio( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_video( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_path( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_font( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_function( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_gameobject( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_scene( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_sprite( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_texture( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_tilesheet( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        bool export_and_play_native(bool launchProgram = true);
        bool export_project_html5(std::string projectBuildDirectory = "", int buildMetaTemplate = -1, bool runGameOnCompile = false, bool inDebugMode = false);
        bool export_project_wiiu( bool inDebugMode = false);
        bool export_project_windows(std::string projectBuildDirectory = "",int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
        bool export_project_osx(std::string projectBuildDirectory = "",int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
        bool export_project_linux(std::string projectBuildDirectory = "",int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
        std::string get_project_name();
        std::string get_project_directory();
        std::string get_project_file_name();
        int get_resource_count();
        bool load_project_file(std::string projectFileIn);

        //For build scripts and such
        bool check_obj_inlist(int objTypeIdIn);
        void swap_obj_lists();
        void begin_obj_reverse_inheritence();
        //used to give each resource a unique global id, like a boss
        int increment_resouce_count();
        void integrate_synthax();
        bool run_project(std::string projectBuildDirectory = "", int buildMetaTemplate = -1,int buildBits = 32, bool inDebugMode = false);
        bool save_project();
        bool save_project_settings();
        bool save_project_as(std::string projectFileNewName);

};

extern GPE_ProjectFolder * CURRENT_PROJECT;

class GPE_Scroller_X: public GPE_GeneralGuiElement
{
    protected:
        bool isOnScrollBox;
        int mouseXScrolPos;
        bool hasMoved;
    public:
        SDL_Rect barBox;
        SDL_Rect contextRect;
        SDL_Rect fullRect;
        double scrollWidth, scrollPercent, scrollPixels, scrollXPos, scrollDisplayPixels;
        bool isNeeded;
        GPE_Scroller_X();
        ~GPE_Scroller_X();
        int get_box_width();
        int get_box_height();
        bool has_moved();
        bool is_scrolling();
        void process_self(SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender, SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL);
        void reset_scroller();
};

class GPE_Scroller_Y: public GPE_GeneralGuiElement
{
    protected:
        bool isOnScrollBox;
        int mouseYScrolPos;
        bool hasMoved;
    public:
        SDL_Rect barBox;
        SDL_Rect contextRect;
        SDL_Rect fullRect;
        double scrollHeight, scrollPercent, scrollYPos, scrollDisplayPixels;
        bool isNeeded;
        GPE_Scroller_Y();
        ~GPE_Scroller_Y();
        int get_box_width();
        int get_box_height();
        bool has_moved();
        bool is_scrolling();
        void process_self(SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender, SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL);
        void reset_scroller();
};

extern int lastResTypeRendered;
class GPE_ResourceManagementBar: public GPE_GeneralGuiElement
{
    private:
        std::vector <GPE_ResourceContainer *> subOptions;
        int seekedX2Pos;

        bool isVisible;
        int lastWidth;
        int barTitleWidth;
        int barTitleHeight;
    public:
        bool justResized;
        bool showYScroll;
        bool beingResized;
        bool menuResized;
        bool resourcebarMoved;
        SDL_Rect entireBox, cameraBox, menuBox;
        GPE_ScrollBar_XAxis * xScroll;
        GPE_ScrollBar_YAxis * yScroll;
        int barXPadding, barYPadding;
        bool subMenuIsOpen;
        int selectedSubOption;
        GPE_Texture * opTexture;
        GPE_ResourceManagementBar();
        ~GPE_ResourceManagementBar();
        GPE_ResourceContainer * add_resource_folder(int resourceType, std::string projFolderName,std::string resourceTypeName);
        GPE_ResourceContainer * add_project_folder(int resourceType, std::string projFolderName,std::string resourceTypeName);
        void delete_project_resources(std::string projectFileName );
        bool is_visible();
        void prerender_self(GPE_Renderer * cRender);
        void process_managementbar( );
        void render_resourcebar(GPE_Renderer * cRender, SDL_Rect *cam = NULL, bool forceRedraw = true);
        void remove_project_resources(std::string projectFileName );
        void set_height(int newHeight);
        void toggle_self();
};

class GPE_TabManager: public GPE_GeneralGuiElement
{
    private:
        bool openContextMenu;
        SDL_Rect tabBox;
        SDL_Rect tabHeaderBox;
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
        void render_tabbar(GPE_Renderer * rendTarget, SDL_Rect *cam = NULL,bool forceRedraw = true);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void set_coords(int newX, int newY);
        void set_height(int newHeight);
        void set_width(int newWidth);
        void update_tabsizes();
};
extern GPE_TabManager * GPE_Main_TabManager;

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
        void process_self(SDL_Rect *viewedSpace=NULL, SDL_Rect *cam=NULL);
        void render_self(GPE_Renderer * rendTarget, SDL_Rect *viewedSpace=NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
};

class GPE_StatusBar: public GPE_GeneralGuiElement
{
    private:
        std::string codeEditorStatusBarString;
        std::string insertModeString;
    public:
        GPE_StatusBar();
        ~GPE_StatusBar();
        void process_self(SDL_Rect *viewedSpace=NULL, SDL_Rect *cam=NULL);
        void render_self(GPE_Renderer * cRender, SDL_Rect *viewedSpace=NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        SDL_Rect optionBox;
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
        void render_self(GPE_Renderer * cRender, SDL_Rect *cam = NULL, bool forceRedraw = true);
};

extern GPE_PopUpMenu_Option * MAIN_CONTEXT_MENU;
extern GPE_PopUpMenu_Option * MAIN_TOOLBAR_RECENT_PROJECTS;
void GPE_open_context_menu(int menuXPos=-1,int menuYPos=-1);
void GPE_close_context_menu();

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
        GPE_Toolbar(std::string bName, SDL_Rect bRect);
        ~GPE_Toolbar();
        void close_toolbar();
        bool is_open();
        void reset_options();
        void set_width(int newWid);
        GPE_PopUpMenu_Option * add_menu_option( std::string name, int id = -1);
        void open_toolbar();
        void prerender_self( GPE_Renderer * cRender = NULL);
        void process_toolbar();
        void render_toolbar(GPE_Renderer *rendTarget=NULL, SDL_Rect *renderCam = NULL, bool forceRedraw = false);
};

const int nestedFolderLimit = 4;

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
        void process_self(SDL_Rect *viewedSpace=NULL, SDL_Rect *cam=NULL);
        void render_self(GPE_Renderer * cRender, SDL_Rect *viewedSpace=NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void set_name(std::string newName);
        void set_selection(int newId, bool autoCorrect = true);
};

class GPE_DropDown_Resouce_Menu: public GPE_GeneralGuiElement
{
    private:
        GPE_ResourceContainer * selectedResource;
        GPE_ResourceContainer * containerTree;
        int maxHeightToRender;
        GPE_PopUpMenu_Option * subOptions;
        std::string dropdownName;
        std::string displayedResult;
        GPE_Sprite * selectedSprite;
        GPE_Texture *selectedImage;
        int opId;
        bool isSelectable;
        int selectedId;
        bool isOpen;
        bool justActivated;
    public:
        GPE_DropDown_Resouce_Menu(int xPos, int yPos, std::string name, GPE_ResourceContainer * cTree = NULL,int id = -1,bool selectable = true);
        ~GPE_DropDown_Resouce_Menu();
        std::string get_data();
        void load_data(std::string dataString);
        std::string get_menu_option(int layerNumb, int atNumb);
        GPE_ResourceContainer * get_container_at(int layerNumb, int atNumb);
        GPE_ResourceContainer * get_selected_container();
        std::string get_plain_string();
        int get_selected_id();
        std::string get_selected_name();
        int get_menu_size();
        bool just_activated();
        //std::string get_name();
        void process_self(SDL_Rect *viewedSpace=NULL, SDL_Rect *cam=NULL);
        void add_folder_contents(GPE_ResourceContainer * fFolder = NULL, GPE_PopUpMenu_Option * fOptionFolder = NULL);
        void render_self(GPE_Renderer * cRender=NULL,SDL_Rect *viewedSpace=NULL, SDL_Rect *cam=NULL, bool forceRedraw = true);
        void set_name(std::string newName);
        void set_selected_target(std::string newName);
        void set_selection(int newId);
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
        void process_self( SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void set_image( std::string buttonImgFile);
        void render_self(GPE_Renderer * cRender, SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        GPE_ToolIconButton * adkb_dton( std::string buttonImgFile,std::string name, int id = -1, bool endsSection = false);
        void process_self(SDL_Rect * viewedSpace =NULL, SDL_Rect *renderCam = NULL);
        void render_self(GPE_Renderer *cRender=NULL, SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect * viewedSpace =NULL, SDL_Rect *renderCam = NULL);
        void set_id(int newId);
        void set_name(std::string newName);
        void render_self(GPE_Renderer * cRender, SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect * viewedSpace =NULL, SDL_Rect *renderCam = NULL);
        void render_self(GPE_Renderer * cRender, SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect * viewedSpace =NULL, SDL_Rect *renderCam = NULL);
        void render_self(GPE_Renderer * cRender, SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        SDL_Rect fieldBarBox;
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        SDL_Rect fieldBarBox;
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
        void get_cursor_from_mouse(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        std::string get_label();
        std::string get_plain_string();
        std::string get_string();
        bool has_content();
        bool is_valid();
        void move_left(int xToMove = 1);
        void move_right(int xToMove = 1);
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void record_string( std::string strToRecord);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
};

const int TEXTAREkb_aTONkb_OPTION_EXPORT = 0;
const int TEXTAREkb_aTONkb_OPTION_IMPORT = 1;
const int TEXTAREkb_aTONkb_OPTION_CLEAR = 2;
const int TEXTAREkb_aTONkb_OPTION_UNDO = 3;
const int TEXTAREkb_aTONkb_OPTION_REDO = 4;
const int TEXTAREkb_aTONkb_OPTION_CUT = 5;
const int TEXTAREkb_aTONkb_OPTION_COPY = 6;
const int TEXTAREkb_aTONkb_OPTION_PASTE = 7;
const int TEXTAREkb_aTONkb_OPTION_TEXTINFO = 8;
const int TEXTAREkb_aTONkb_OPTION_MAX_OPTIONS = 9;

class GPE_ParsedText
{
    public:
        int textStart;
        int textEnd;
        GPE_ParsedText(int nStart, int nEnd);
        ~GPE_ParsedText();
};

class GPE_SynthaxLine
{
    public:
        std::vector <GPE_ParsedText*> foundParses;
        int firstPos;
        int lastPos;
        GPE_SynthaxLine();
        ~GPE_SynthaxLine();
        std::string get_parsed_line(std::string sIn);
        void render_tokens(GPE_Renderer * cRender,GPE_Font * fontIn,std::string sIn, int xPos, int yPos, int lineStartPos, int lineMaxPos, GPE_Color * renderColor = NULL, bool isNumber = false);
        void reset_self();
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);

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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
        void set_name(std::string nameIn);
};

class GPE_TextAreaInputBasic: public GPE_GeneralGuiElement
{
    protected:
        bool undoableActionOccurred;
        time_t lastUndoableActionTime;
        int lastDoubleClickAction;
        int currentPositionInHistory;
        std::vector<GPE_TextAreaInputBasic *> savedHistory;
        bool isReadOnly;
        int inputFieldPos;
        bool useWordWrap;
        bool showXScroll, showYScroll;
        int displayMode;
        int cursorXPos, cursorYPos;
        int prevCursorXPos, prevCursorYPos;
        int cursorTimer;
        bool showCursor;
        int selectionCursorXPos, selectionCursorYPos;
        int selectionEndCursorXPos, selectionEndCursorYPos;

        int symbolCursorXPos, symbolCursorYPos;
        int symbolEndCursorXPos, symbolEndCursorYPos;

        int maxLength;
        int maxLineCount;
        int lineCountBoxWidth;
        int longestString;
        int linesWithinView;
        int prevLinesWithinView;
        int lineStartXPos;
        int charactersWithinView;
        int prevCharactersWithinView;
        int lineStartYPos;

        //Text Area Delays
        float delDelay;
        float bscDelay;
        float tabDelay;
        float enterDelay;
        float upDelay;
        float downDelay;
        float leftDelay;
        float rightDelay;
        float dKeyDelay;
        int TEXTBOX_FONT_SIZE_WIDTH, TEXTBOX_FONT_SIZE_HEIGHT;
        GPE_Font * editorFont;
        std::string placeHolderString;
        std::string textInputString;
        std::string lineToEdit;
        std::vector <std::string> listOfStrings;
        std::vector < GPE_TextAnchor *> anchorPositions;
        GPE_ScrollBar_XAxis * textXScroll;
        GPE_ScrollBar_YAxis * textYScroll;
        SDL_Rect textSpaceRect;
        GPE_SynthaxLine * commentLineText;
        GPE_SynthaxLine * datatypeLineText;
        GPE_SynthaxLine * dQuoteLineText;
        GPE_SynthaxLine * functionLineText;
        GPE_SynthaxLine * keywordLineText;
        GPE_SynthaxLine * variableLineText;
        GPE_SynthaxLine * projectFunctionLineText;
        GPE_SynthaxLine * projectKeywordLineText;
        GPE_SynthaxLine * normalLineText;
        GPE_SynthaxLine * numberLineText;
        GPE_SynthaxLine * sQuoteLineText;
        GPE_SynthaxLine * symbolLineText;
        SDL_Rect renderBox;
    public:
        int parseForErrorsTimerGoal;
        int parseForErrorsTimerPos;
        bool missingSymbolDetected;
        int missingSymbolStartYPos;
        int missingSymbolStartXPos;
        std::vector<std::string> foundSynthaxErrors;
        int misingSymbolSuggestedEndYPos;
        int misingSymbolSuggestedEndXPos;
        //Used for code highlights and suggestions.
        std::vector <GPE_Compiler_Term *> suggestedCompilerTerms;
        GPE_Compiler_Term * highlightedTerm;
        int highlightXPos, highlightYPos;
        bool documentationIsBeingShown;
        bool codeBeingSuggested;
        int iSuggestedStartPos;
        int iSuggestionPos;
        int suggestedTextMaxInViewCount;
        int maxSuggestedTextWidth;
        //End of code highlights and suggestion related variables

        GPE_ToolIconButtonBar * textEditorButtonBar;
        bool showPlaceHolder;
        bool isTextLog;
        bool isCodeEditor;
        int codeEditorType;
        int redrawDelay;
        int redrawDelayMax;
        int tempCLineXStartPos;
        int tempCLineXEndPos;
        GPE_TextAreaInputBasic(bool saveFirstEdit = false);
        GPE_TextAreaInputBasic(int newX, int newY , int boxW=-1, int boxH=-1,std::string startName="",std::string placeHolderText="", bool saveFirstEdit = false);
        ~GPE_TextAreaInputBasic();
        void activate_self();
        void add_line( std::string newLine, bool scrollToBottom = false);
        void adjust_fortabs();
        bool can_redo();
        bool can_undo();
        bool can_undo_or_redo();
        void clear_line(int lineNumber);
        void clear_all_lines();
        void clear_text_anchors();
        void copy_selection();
        bool compile_into_code(std::ofstream * fileTarget, int leftTabAmount = 0, bool useNewLines = true, bool useLastNewLine = false);
        int compare_with(GPE_TextAreaInputBasic * other = NULL);
        bool copy_all();
        void copy_source(GPE_TextAreaInputBasic * other = NULL, bool overwriteSelf = false);
        void cut_selection();
        void create_buttonbar();
        void delete_content();
        void delete_selection();
        void duplicate_text();
        bool find_connected_symbols();
        int get_line_count();
        int get_most_anchors_characters();
        int get_most_characters_used();
        int get_renderbox_height();
        int get_xcursor();
        int get_ycursor();
        std::string get_line_string(int lineNumberIn);
        std::string get_short_hightlighted();
        std::string get_string();
        std::string get_string_spaces();
        std::vector <std::string> get_all_strings();
        void handle_scrolling();
        bool has_content();
        bool has_selection();
        void init_save_history();
        bool is_read_only();
        void log_editable_action();
        void move_left(int xToMove = 1);
        void move_right(int xToMove = 1);
        void move_up(int yToMove = 1);
        void move_down(int yToMove = 1);
        void push_to_nextline();
        void use_backspace();
        void use_control();
        void use_shift();
        void export_text(std::string newFileName);
        void export_text_anchors(std::string newFileName);
        int find_all_strings(std::string strTarget, bool matchCase = true, bool addAnchor = false, std::string areaTitle = "");
        void find_mouse_cursor(int *mXCursor, int *mYCursor, SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        bool find_string(std::string strTarget, bool searchDown = true, bool matchCase = true, bool startAtFront = false);
        void find_documentation_description(int tCursorX = -1, int tCursorY = -1);
        void find_suggested_text();
        bool write_data_into_file(std::ofstream * fileTarget, int leftTabAmount = 0, bool useNewLines = true, bool useLastNewLine = false);
        bool write_short_data_into_file(std::ofstream * fileTarget, int leftTabAmount = 0, bool useNewLines = true, bool useLastNewLine = false);
        bool import_text(std::string newFileName);
        void paste_clipboard();
        bool parse_code_javascript();
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void redo_edit();
        void reset_selection(int resetDirection = 0);
        void reset_self();
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = false);
        void render_code_css(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_code_javascript(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);

        int replace_all_found(std::string strToReplace, std::string newString);
        void revise_edit(int editId = -1);
        void save_edit();
        void select_all();
        void scroll_to_pos( int lineNumberIn, int characterNumberIn );
        void scroll_to_selection();
        void scroll_to_cursor();
        void set_read_only();
        void set_writeable();
        void set_string(std::string newString);
        void set_placeholder(std::string newString);
        void set_xcursor(int newPos);
        void set_ycursor(int newPos);
        void undo_edit();
        void update_cursor_to_mouse(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
};

class GPE_WrappedTextArea: public GPE_GeneralGuiElement
{
    protected:
        std::string paragraphText;
    public:
        GPE_TextAreaInputBasic * heldTextArea;
        GPE_WrappedTextArea();
        ~GPE_WrappedTextArea();
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
        void set_string( std::string strIn);
        void set_width(int newWidth);
        void set_height( int newHeight);
        void set_max_width( int nMW);
        void set_max_height( int nMH);
        void update_paragraph( int foundMaxWidth = -1 );
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
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
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);

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
        void process_self(SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect * viewedSpace = NULL, SDL_Rect * cam = NULL, bool forceRedraw = true);
};

class GPE_LogManager: public GPE_GeneralGuiElement
{
    private:
        std::vector<GPE_Log_Entry *> generalLog;
        GPE_TextAreaInputBasic * generalTextLog;

        std::vector<GPE_Log_Entry *> debugLog;
        GPE_TextAreaInputBasic * debugTextLog;

        std::vector<GPE_Log_Entry *> buildLog;
        GPE_TextAreaInputBasic * buildTextLog;
        GPE_TextAreaInputBasic * otherLog;
        GPE_TextAreaInputBasic * logToView;
        GPE_Label_Text * emptyResultsLabel;
        GPE_GuiElementList * bottomInfoList;
        int seekedY1Pos;
        GPE_TabBar * logTabs;
        GPE_ScrollBar_XAxis * xScroll;
        GPE_ScrollBar_YAxis * yScroll;
        bool showXLogScroll, showLogYScroll;
    public:
        bool isVisible;
        bool menuResized;
        bool beingResized;
        bool justResized;
        std::vector< GPE_TextAnchor * > searchAnchors;
        GPE_LogManager();
        ~GPE_LogManager();
        void clear_search_anchors();
        void process_self(SDL_Rect *viewedSpace=NULL, SDL_Rect *cam=NULL);
        void render_self(GPE_Renderer * cRender, SDL_Rect *viewedSpace=NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        bool being_resized();
        void clear_all_logs();
        void clear_debug_log();
        void clear_build_log();
        void clear_general_log();
        void log_general_line(std::string newLogLine);

        void log_general_comment(std::string newLogLine);
        void log_general_warning(std::string newLogLine);
        void log_general_error(std::string newLogLine);

        void log_debug_line(std::string newLogLine);
        //The core 3 special messages to show for build problems and suggestions
        void log_build_line(std::string newLogLine);
        void log_build_comment(std::string newLogLine);
        void log_build_warning(std::string newLogLine);
        void log_build_error(std::string newLogLine);

        void log_other_line(std::string newLogLine);
        void open_general_log();
        void open_build_log();
        void open_other_log();
        void open_replace_mode();
        void open_search_mode();
        void open_search_results();
};

extern GPE_LogManager * GPE_Main_Logs;

const int RELEASE_TYPE_ALPHA = 0;
const int RELEASE_TYPE_BETA = 1;
const int RELEASE_TYPE_RELEASE = 2;

const int SEARCH_MODE_FIND = 1;
const int SEARCH_MODE_GOTO = 2;
const int SEARCH_MODE_REPLACE = 3;
const int SEARCH_MODE_FINDALL = 4;
const int SEARCH_MODE_REPLACEALL = 5;

class GPE_Gui_Engine
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
        std::vector<std::string> gpeRecentProjects;
        std::vector<std::string> gpeTips;
    public:
        float textInputDelayTime;
        float normalInputDelayTime;
        float textAreaDelayTime;

        bool includeNintendoWiiUExport;
        bool includeNintendoSwitchExport;
        bool includePlaystation4Export;
        bool includeXboxOneExport;
        int editorReleaseType;
        bool showStartPageAtStartUp;
        bool showTipsAtStartUp;
        std::string toolTipString;
        std::string fileOpenProjectDir;
        std::string fileOpenProjectIconDir;
        std::string fileSaveProjectDir;
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

        bool showTextEditorLineCount;
        bool showTextEditorSynthaxHightlight;
        int defaultLineCountWidth;

        //For code highlights and suggested text
        std::vector <GPE_Compiler_Term *> suggestedCompilerTerms;
        GPE_Compiler_Term * highlightedTerm;
        int highlightXPos, highlightYPos;
        bool documentationIsBeingShown;
        bool codeBeingSuggested;
        int iSuggestedStartPos;
        int iSuggestionPos;
        int suggestedTextMaxInViewCount;
        int maxSuggestedTextWidth;
        //End of suggested text/code highlights related variables

        //For Find/Replace Box of TextArea
        GPE_TextInputBasic * findTextStringBox;
        GPE_TextInputNumber * goToLineStringBox;
        GPE_TextInputBasic * replaceTextStringBox;
        GPE_ToolLabelButton * findButton;
        GPE_ToolLabelButton * findAllButton;
        GPE_ToolLabelButton * goToButton;
        GPE_ToolLabelButton * replaceButton;
        GPE_ToolLabelButton * replaceAllButton;

        int previousSearchMode;
        int textSearchMode;
        bool showFindAllResults;

        std::string searchResultProjectName;
        std::string searchResultResourceName;
        int searchResultResourceId;
        SDL_Rect textAreaFindBox;
        GPE_CheckBoxBasic * findMatchCase;
        GPE_Label_Text * scopeLabel;
        GPE_DropDown_Menu * findScope;
        int findAllStartXPos, findAllStartYPos;
        std::vector<GPE_ProjectFolder * > gpeProjects;
        GPE_Toolbar * mainToolBar;
        GPE_ResourceManagementBar * mainResourceBar;
        GPE_Gui_Engine();
        ~GPE_Gui_Engine();
        void add_to_recent_project_list(std::string newProjectFileName, bool saveData = false);
        void apply_logic();
        void clean_current_project_build_folder(int buildMetaTemplate);
        void clear_recent_project_list();
        void close_finder();
        void close_project(std::string projectFileName );
        GPE_ProjectFolder * find_project_from_name(std::string projectName);
        GPE_ProjectFolder * find_project_from_filename(std::string projectFileName);
        int find_project_id_from_name(std::string projectName, int ignoreId = -1);
        int find_project_id_from_filename(std::string projectFileName, int ignoreId = -1);
        int get_recent_project_list_size();
        int get_tip_count();
        int get_random_tip();
        int get_previous_tip(int tipId);
        int get_next_tip(int tipId);
        std::string get_recent_project_name(int pId);
        std::string get_tip(int tipId);

        GPE_ResourceManagementBar  * init_resourcebar();
        GPE_Toolbar * init_toolbar();
        GPE_Toolbar * init_toolbar(std::string bName, SDL_Rect bRect);
        void launch_new_project();
        void load_tips();
        void load_settings();
        GPE_Toolbar * get_main_toolbar();
        GPE_ToolIconButtonBar * get_main_buttonbar();
        void open_project(std::string projName);
        void open_new_project();
        bool overhead_used();
        void prerender_gui(GPE_Renderer * cRender);
        void process_cursor();
        void process_overlay_message();
        void process_window_title();
        void render_code_highlights(GPE_Renderer * renderTarget = NULL, bool forceRedraw = true);
        void render_foreground_engine(GPE_Renderer * renderTarget = NULL, bool forceRedraw = true);
        void render_gui_info(GPE_Renderer * renderTarget = NULL, bool forceRedraw = true);
        void render_frozen_screenshot(GPE_Renderer * renderTarget = NULL);
        void render_temporary_message(GPE_Renderer * renderTarget = NULL);
        void render_tooltip(GPE_Renderer * renderTarget = NULL,int xPos = -1, int yPos = -1 );
        void remove_project(std::string projectFileName );
        void reset_gui_info();
        void reset_settings();
        void export_current_project_html5(bool runGameOnCompile = false);
        void export_current_project_wiiu();
        void rum_current_project(std::string projectBuildDirectory, int buildMetaTemplate);

        void save_all_projects();
        void save_current_project();
        void save_settings();
        void set_main_toolbar(GPE_Toolbar * newToolbar);
        void setup_project_directory(std::string newProjectDir);
        std::string setup_build_folder(std::string buildDirectory, int buildType = GPE_BUILD_WINDOWS, int buildBits = 32, bool inDebugMode = false, int nativeBuildType = Native_None );
        void take_live_screenshor(GPE_Renderer * renderTarget = NULL);
        void take_frozen_screenshot(GPE_Renderer * renderTarget = NULL);
        bool using_search();
        void update_tooltip(std::string newTip);
        void update_temporary_message(std::string mTitle="", std::string mSubTitle="", std::string mText="", float messDuration = 3, bool topLeftMessage = true );
        void update_recent_project_list(bool saveData);
};

extern GPE_Gui_Engine * GPE_MAIN_GUI;

class standardEditableGameResource: public generalGameResource
{
    protected:
        GPE_ToolPushButton * loadResourceButton;
        GPE_ToolPushButton * exportResourceButton;
        GPE_ToolPushButton * saveResourceButton;
        GPE_ToolPushButton * confirmResourceButton;
        GPE_ToolPushButton * cancelResourceButton;
        GPE_TextInputBasic * renameBox;
        GPE_GuiElementList * editorPaneList;
    public:
        bool resourcePostProcessed;
        int commonButtonAlignment;
        GPE_ResourceContainer * projectParentFolder;
        standardEditableGameResource(GPE_ResourceContainer * ppFolder = NULL);
        virtual ~standardEditableGameResource() = 0;
        std::string get_current_name();
        GPE_GeneralGuiElement * add_gui_component(std::string componentData);
        void process_export();
        virtual void integrate_into_synthax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        virtual void preprocess_self(std::string alternatePath = "" );
        virtual void prerender_self(GPE_Renderer * cRender);
        virtual void process_self(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        virtual void process_resource(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_resource(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        virtual void render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void seek_parent_name();
        void set_name(std::string newName);
        void set_parent_name(std::string newName);
        virtual int search_for_string(std::string needle);
        virtual int search_and_replace_string(std::string needle, std::string newStr = "");
};

const int aacFileName = 0;
const int mp3FileName = 1;
const int oggFileName = 2;
const int wavFileName = 3;

const int SUPPORTED_AUDIO_FORMAT_COUNT = 4;

extern std::string SUPPORTED_AUDIO_EXT[SUPPORTED_AUDIO_FORMAT_COUNT];

class audioResource: public standardEditableGameResource
{
    public:
        bool isPreloaded;
        int audioId;
        int audioType;
        Mix_Chunk * soundVal;
        Mix_Music * musicVal;
        GPE_ToolPushButton * playButton;
        GPE_ToolPushButton * openExternalEditorButton;
        GPE_ToolPushButton * refreshResourceDataButton;
        GPE_RadioButtonControllerBasic * audioTypeButtonController;
        std::string audioFileName[SUPPORTED_AUDIO_FORMAT_COUNT];
        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_TextInputBasic * audioGroupName;
        GPE_TextInputNumber * defaultVolume;
        GPE_Label_Text * audioEditorMainNote;
        audioResource(GPE_ResourceContainer * pFolder = NULL);
        ~audioResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        bool copy_audio_source(std::string outDirectoryName);
        void load_audio(std::string newFileName);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self(GPE_Renderer * cRender);
        void process_self(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const int mp4FileName = 0;
const int webmFileName = 1;
const int oggVideoFileName = 2;

const int SUPPORTED_VIDEO_FORMAT_COUNT = 3;

extern std::string SUPPORTED_VIDEO_EXT[SUPPORTED_VIDEO_FORMAT_COUNT];

class videoResource: public standardEditableGameResource
{
    public:
        int videoId;
        int videoType;
        GPE_RadioButtonControllerBasic * audioTypeButtonController;
        std::string videoFileName[SUPPORTED_VIDEO_FORMAT_COUNT];
        GPE_Label_Text * videoEditorMainNote;
        GPE_TextInputBasic * videoGroupName;
        GPE_TextInputNumber * defaultVolume;
        GPE_ToolPushButton * openExternalEditorButton;
        GPE_ToolPushButton * refreshResourceDataButton;
        videoResource(GPE_ResourceContainer * pFolder = NULL);
        ~videoResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        bool copy_video_source(std::string outDirectoryName);
        void load_video(std::string newFileName);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self(GPE_Renderer * cRender);
        void process_self(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const int FONT_FILE_TYPES = 5;
const int FONT_EOT = 0;
const int FONT_SVG = 1;
const int FONT_TTF = 2;
const int FONT_WOFF = 3;
const int FONT_WOFF2 = 4;

extern std::string SUPPORTED_FONT_EXT[FONT_FILE_TYPES];

class fontResource: public standardEditableGameResource
{
    private:
        GPE_RadioButtonControllerBasic * fontTypeButtonController;
        GPE_ToolPushButton * openExternalEditorButton;
        std::string fontInEditorFileName;
        std::string fontFamilyName;
        GPE_Font * fontInEditor;
    public:
        int fontSize;
        int fontId;
        int fontType;
        int fontWidth;
        int fontHeight;
        GPE_TextInputNumber * fontSizeField;
        GPE_TextInputBasic * fontPreviewTextField;
        std::string storedFontFileNames[FONT_FILE_TYPES];
        fontResource(GPE_ResourceContainer * pFolder = NULL);
        ~fontResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        bool build_css3_file(std::ofstream * fileTarget, int leftTabAmount);
        void load_font(std::string newFileName, int newFontSize = 12);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self(GPE_Renderer * cRender);
        void process_self(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class functionResource: public standardEditableGameResource
{
    public:
        GPE_TextInputBasic * parametersField;
        GPE_TextInputBasic * functionReturnType;
        GPE_TextAreaInputBasic * functionCode;
        GPE_ToolIconButton * saveButton;
        functionResource(GPE_ResourceContainer * pFolder = NULL);
        ~functionResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        void integrate_into_synthax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void preprocess_self(std::string alternatePath = "");
        void prerender_self(GPE_Renderer * cRender);
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const double ZOOM_LEVEL_MIN = 0.0625;
const double ZOOM_LEVEL_MAX = 16;
class spriteResource: public standardEditableGameResource
{
    public:
        bool isPreloaded;
        double zoomLevel;
        int autoAnimationPos;
        GPE_Sprite * spriteInEditor;
        GPE_Label_Text * labelSpriteDimensions;
        GPE_Label_Text * labelFrameInfo;
        GPE_Label_Text * labelSpriteMessage;
        GPE_ToolPushButton * editResourceButton;
        GPE_ToolPushButton * transformResourceButton;
        GPE_ToolPushButton * playPauseResourceButton;
        GPE_ToolPushButton * openExternalEditorButton;
        GPE_ToolPushButton * refreshResourceDataButton;
        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_CheckBoxBasic * showCollisionShapeCheckBox;
        GPE_CheckBoxBasic * showAnimationCheckBox;

        GPE_TextInputNumber * spriteDataFields[SPRITE_DATA_FIELD_COUNT];

        GPE_TextInputNumber * spriteCollisionRectX;
        GPE_TextInputNumber * spriteCollisionRectY;
        GPE_TextInputNumber * spriteCollisionRectW;
        GPE_TextInputNumber * spriteCollisionRectH;

        GPE_TextInputNumber * spriteCollisionCircleX;
        GPE_TextInputNumber * spriteCollisionCircleY;
        GPE_TextInputNumber * spriteCollisionCircleR;

        GPE_ToolLabelButton * spriteCenterCollisionButton;

        GPE_DropDown_Menu * spriteCollisionShapeMenu;

        GPE_ToolIconButton * previousSubImageButton;
        GPE_ToolIconButton * nextSubImageButton;
        GPE_TextInputNumber * subImageNumberField;
        GPE_TextInputNumber * subImageEditorPreviewId;
        int subImagePreviewNumber;
        int subImageMiniAnimationNumber;

        spriteResource(GPE_ResourceContainer * pFolder = NULL);
        ~spriteResource();
        GPE_Label_Text * labelInfoMaxTextureSize;
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        int get_preview_frame();
        void load_image(std::string newFileName, bool autoProcess = false);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self(GPE_Renderer * cRender);
        void process_data_fields(float versionToProcess = -1);
        void process_self(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class textureResource: public standardEditableGameResource
{
    public:
        bool isPreloaded;
        GPE_Texture * textureInEditor;
        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_Label_Text * labelImageDimensions;
        GPE_Label_Text * labelTextureMessage;
        GPE_ToolPushButton * transformResourceButton;
        GPE_ToolPushButton * openExternalEditorButton;
        GPE_ToolPushButton * refreshResourceDataButton;
        textureResource(GPE_ResourceContainer * pFolder = NULL);
        ~textureResource();
        GPE_Label_Text * labelInfoMaxTextureSize;
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        int load_image(std::string newFileName);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self(GPE_Renderer * cRender);
        void process_self(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class tilesheetPreviewer: public GPE_GeneralGuiElement
{
    protected:
        GPE_ScrollBar_XAxis * xTileScroll;
        GPE_ScrollBar_YAxis * yTileScroll;
    public:
        GPE_Label_Text * labelImageDimensions;
        GPE_Label_Text * labelFrameInfo;
        GPE_Label_Text * labelSpriteMessage;
        int tileToPrevX1,tileToPrevY1, tileToPrevX2,tileToPrevY2;
        int tilesToPlacePerRow;
        SDL_Rect tsRenderRect;
        SDL_Rect tsSelectedArea;
        GPE_Tilesheet * tileSheetToPreview;
        std::vector< int > tilesIdsInPreview;
        bool allowDuplicates;
        bool showGrid;
        tilesheetPreviewer();
        ~tilesheetPreviewer();
        GPE_Label_Text * labelInfoMaxTextureSize;
        void process_self(SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender = NULL,SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true);
        void render_selection(GPE_Renderer * cRender = NULL,int xPos = 0, int yPos = 0, SDL_Rect *viewedSpace = NULL, SDL_Rect *cam = NULL, bool forceRedraw = true, double scaleSize = 1, GPE_Color * fColor = NULL);
        void reset_preview(bool moveCamera);
};

class tilesheetResource: public standardEditableGameResource
{
    public:
        bool isPreloaded;
        tilesheetPreviewer * tsPreviewer;
        GPE_Tilesheet * tilesheetInEditor;
        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_Label_Text * tilesheetDimensionsStr;
        GPE_ToolPushButton * transformResourceButton;
        GPE_ToolPushButton * openExternalEditorButton;
        GPE_ToolPushButton * refreshResourceDataButton;
        GPE_TextInputNumber * tsDataFields[TILESHEET_DATA_FIELD_COUNT];
        GPE_Label_Text * labelInfoMaxTextureSize;
        tilesheetResource(GPE_ResourceContainer * pFolder = NULL);
        ~tilesheetResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        void load_image( std::string newFileName);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self(GPE_Renderer * cRender);
        void process_data_fields();
        void process_self(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class GPE_NewsScreen: public generalGameResource
{

};

class GPE_WelcomeScreen: public generalGameResource
{

};

class collisionContainer
{
    public:
        int otherObjectType;
        std::string otherObjectName;
        GPE_TextAreaInputBasic * textEditor;
        collisionContainer();
        ~collisionContainer();

};

class customFunctionContainer
{
    public:
        std::string functionName;
        std::string functionReturnType;
        std::string functionParameters;
        GPE_TextInputBasic * returnTypeField;
        GPE_TextInputBasic * parametersField;
        GPE_TextAreaInputBasic * textEditor;
        customFunctionContainer();
        ~customFunctionContainer();

};

const int DEFAULT_OBJECT_FUNCTIONS_COUNT = 16;
const int DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT = 10;
const int DEFAULT_SPECIAL_FUNCTIONS_COUNT= 6;


const int objectFunctionTypes = 5; //normal, timed, collisions, user added, special funcions
extern std::string DEFAULT_OBJECT_FUNCTIONS_NAMES[DEFAULT_OBJECT_FUNCTIONS_COUNT];
extern std::string DEFAULT_OBJECT_SPECIALS_NAMES[DEFAULT_SPECIAL_FUNCTIONS_COUNT];

extern std::string DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[DEFAULT_OBJECT_FUNCTIONS_COUNT];
extern std::string DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[DEFAULT_SPECIAL_FUNCTIONS_COUNT];


const int FUNC_ID_PRELOGIC = 2;
const int FUNC_ID_LOGIC = 3;
const int FUNC_ID_ENDLOGIC = 5;


const int FUNC_ID_PRERENDER = 6;
const int FUNC_ID_RENDER = 7;
const int FUNC_ID_POSTRENDER = 8;
const int FUNC_ID_HUD_RENDER = 9;

class objectResParentChainLink
{
    public:
        int chainObjectType;
        objectResParentChainLink( int origObjectType);
        ~objectResParentChainLink();
        std::vector< int> heldChains;
        void grab_chain( objectResParentChainLink * chainIn = NULL);
        bool is_inchain(int chainId);
};

class GPE_ObjectComponent
{
    private:
        GPE_GeneralGuiElement * component;
        GPE_ToolIconButton * settingsGear;
    public:
        GPE_ObjectComponent();
        ~GPE_ObjectComponent();

        void load_data(std::string dataString);
        void remove_data(std::string dataString);
        void rename_component(std::string newName);
        std::string get_data();
        std::string get_name();
        std::string get_plain_string();
        std::string get_type();
        GPE_GeneralGuiElement * get_component();
        GPE_ToolIconButton * get_gear();
        void set_component(GPE_GeneralGuiElement * newComponent);
};

const int OBJ_EDITOR_MODE_COMPONENTS = 0;
const int OBJ_EDITOR_MODE_BASIC = 1;
const int OBJ_EDITOR_MODE_TIMED = 2;
const int OBJ_EDITOR_MODE_COLLISIONS = 3;
const int OBJ_EDITOR_MODE_CUSTOM = 4;
const int OBJ_EDITOR_MODE_SPECIAL = 5;

class gameObjectResource: public standardEditableGameResource
{
    public:
        GPE_Label_Title * resourceNameLabel;
        GPE_ToolIconButtonBar * editorButtonBar;
        SDL_Rect objEventManagebarBox;
        GPE_SelectBoxBasic * basicFunctionsSelector;
        GPE_SelectBoxBasic * timedFunctionsSelector;
        GPE_SelectBoxBasic * colliderFunctionsSelector;
        GPE_SelectBoxBasic * customFunctionsSelector;
        GPE_SelectBoxBasic * specialFunctionsSelector;
        GPE_CheckBoxBasic * checkBoxNeedsCamera;
        GPE_CheckBoxBasic * checkBoxIsMoveable;
        GPE_CheckBoxBasic * checkBoxIsVisible;
        GPE_CheckBoxBasic * checkBoxIsContinuous;
        GPE_TextAreaInputBasic * classDeclarationBox;
        GPE_TextAreaInputBasic * objectFunctions [DEFAULT_OBJECT_FUNCTIONS_COUNT];
        GPE_TextAreaInputBasic * timedObjectFunctions [DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT];
        GPE_TextAreaInputBasic * specialObjectFunctions [DEFAULT_SPECIAL_FUNCTIONS_COUNT];
        int objectFunctionInEdit[objectFunctionTypes];
        std::vector< collisionContainer * > collisionFunctions;
        std::vector< customFunctionContainer * > customFunctions;
        int spriteIndex;
        int imageIndex;
        int codeSection;
        int parentObjectId;
        GPE_Sprite * spriteInEditor;
        GPE_DropDown_Resouce_Menu * spriteField;
        GPE_DropDown_Resouce_Menu * parentObjectField;
        GPE_DropDown_Resouce_Menu * newObjectToCollideDropDown;
        GPE_DropDown_Resouce_Menu * changeObjectToCollideDropDown;
        GPE_ToolPushButton * removeCollideFunctionButton;
        GPE_ToolIconButton * newCustomObjectFunctionButton;
        GPE_ToolIconButton * newCustomObjectFunctionSettingsButton;
        GPE_ToolIconButton * removeCustomObjectFunctionButton;

        //Component Related Variables
        std::vector < GPE_ObjectComponent * > customComponentRealList;
        GPE_GuiElementList * customComponentsGuiList;
        GPE_Label_Text * componentsMainGuiLabel;
        GPE_Label_Text * componentsGuiLabel;
        GPE_ToolPushButton * inheritParentComponentButton;
        GPE_ToolPushButton * resetComponentsButton;
        GPE_DropDown_Menu * addNewComponentDropDown;
        GPE_ToolIconButton * removeComponentButton;
        GPE_ToolIconButton * editCompnentButton;
        GPE_ToolIconButton * componentSettingsButton;

        gameObjectResource(GPE_ResourceContainer * pFolder = NULL);
        ~gameObjectResource();
        void add_component(std::string newComponentData);
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        collisionContainer *  find_collision_event( int otherObjId);
        customFunctionContainer * find_custom_function( std::string functionNameIn, bool createNewOne = true);
        GPE_TextAreaInputBasic * find_function_textarea(std::string functionNameIn,int functionTypeIn = 0);
        GPE_ResourceContainer * get_parent_resource();
        void integrate_into_synthax();
        void manage_components(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const int LEDITOR_MODE_SETTINGS = 0;
const int LEDITOR_MODE_LAYERS = 1;

const int LAYER_TYPE_TILES = 0;
const int LAYER_TYPE_BACKGROUND = 1;
const int LAYER_TYPE_OBJECTS = 2;
const int LAYER_TYPE_RESOURCES = 3;
const int LAYER_TYPE_MAX = 2;

const int LEDITOR_MODE_META = 4;

const int MAX_BACKGROUND_NUMBER = 7;

class GPE_SceneGameObject
{
    public:
        bool isBeingMoved;
        int customObjId;
        int objTypeId;
        std::string objTypeName;
        gameObjectResource * objBeingPlaced;
        SDL_Rect objRect;
        GPE_Input_Field_Color * objColor;
        GPE_TextInputBasic * objSubName;
        GPE_TextInputNumber * objAngleField;
        std::string objCustomVariables;
        std::vector< GPE_ObjectComponent * >objCustomValPairs;
        int objAngle;
        float objXScale;
        float objYScale;
        GPE_SceneGameObject();
        ~GPE_SceneGameObject();
        GPE_ObjectComponent *  add_variable(GPE_GeneralGuiElement * newVariable);
        void reset_components();
};

class GPE_SceneTexture
{
    public:
        GPE_ResourceContainer * projectParentFolder;
        GPE_DropDown_Resouce_Menu * backgroundInEditor;
        GPE_TextInputNumber * bgPosXField;
        GPE_TextInputNumber * bgPosYField;
        GPE_TextInputNumber * bgHorSpeedField;
        GPE_TextInputNumber * bgVertSpeedField;

        GPE_CheckBoxBasic * checkTileHori;
        GPE_CheckBoxBasic * checkTileVert;
        GPE_CheckBoxBasic * checkStretch;
        int textureId;
        textureResource * texRes;
        SDL_Rect bgRect;
        bool inFront;
        bool tileHori;
        bool tileVert;
        bool stretchBG;
        int bgXSpeed;
        int bgYSpeed;
        GPE_SceneTexture(GPE_ResourceContainer *pFolder);
        ~GPE_SceneTexture();
        void process_background_data();
};

class GPE_SceneTile
{
    public:

        int tileTypeId;
        int tileIndexId;
        int tilesheetIndexId;
        SDL_Rect tileRect;
        GPE_SceneTile();
        ~GPE_SceneTile();
};

class sceneLayer
{
    public:
        GPE_ResourceContainer * projectParentFolder;
        int layerType;
        int tileAmountX, tileAmountY;
        //int layerId;
        int layerAlpha;
        int projectLayerId;
        int prevTileAmountX, prevTileAmountY;
        std::vector< GPE_SceneTexture *> layerBackgrounds;
        std::vector <GPE_SceneTile*> mapTiles;
        std::vector< GPE_SceneGameObject * > layerObjects;
        GPE_SelectBoxBasic * layerBGSelectBox;
        sceneLayer(int newLayerType, int foundProjectLayerId, GPE_ResourceContainer *pFolder = NULL);
        ~sceneLayer();
        void add_background(GPE_SceneTexture * newBG);
        GPE_SceneTile* get_tile_at(int x, int y);
        void clear_all_objects();
        void clear_objects(int objectTypeToClear);
        void create_new_map(int newTX=8, int newTY=8, int ntileType=0);
        void resize_tilemap(int newTX=8, int newTY=8, int ntileType=0);
        int get_map_size();
};

extern std::string DEFAULT_SCENE_SUBEDITOR_NAMES[5];

class gameSceneResource: public standardEditableGameResource
{
    protected:
        int rightPaneWidth;
        SDL_Rect editorPane;
        SDL_Rect rightEditorPane;
        SDL_Rect editorCommentPane;
        GPE_Rect editorView;
        GPE_Rect sceneEditorViewRect;
        GPE_ToolIconButtonBar * editorButtonBar;
        GPE_Label_Title * sceneEditorSubTitle;
        //used for the settings/options tab
        GPE_TextInputBasic * levelTitleField;
        GPE_TextInputBasic * sceneCaptionField;
        GPE_TextInputBasic * levelHintField;

        GPE_DropDown_Resouce_Menu * musicAudioDropDown;
        GPE_DropDown_Resouce_Menu * startAudioDropDown;
        GPE_DropDown_Resouce_Menu * endAudioDropDown;

        //
        sceneLayer * currentLayer;
        GPE_DropDown_Resouce_Menu * northSceneDropDown;
        GPE_DropDown_Resouce_Menu * southSceneDropDown;
        GPE_DropDown_Resouce_Menu * eastSceneDropDown;
        GPE_DropDown_Resouce_Menu * westSceneDropDown;
        //
        GPE_TextInputNumber * levelPixelWidthField;
        GPE_TextInputNumber * levelPixelHeightField;
        GPE_TextInputNumber * levelTileWidthField;
        GPE_TextInputNumber * levelTileHeightField;

        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_CheckBoxBasic * isometricCheckBox;
        GPE_CheckBoxBasic * checkBoxIsContinuous;
        GPE_Input_Field_Color * sceneBackgroundColor;
        //used for the background tab
        int bgIdInEdit;
        GPE_SelectBoxBasic * backgroundToSelect;

        textureResource * texRes;
        GPE_DropDown_Menu * sceneZoomLevel;
        double sceneZoomAmount;
        //used for the object placement tab
        GPE_ToolLabelButton * inheritParentComponentButton;
        GPE_ToolLabelButton * resetComponentsButton;
        GPE_DropDown_Menu * addNewComponentDropDown;

        GPE_DropDown_Resouce_Menu * objectInEditor;
        GPE_DropDown_Resouce_Menu * objectNewType;
        GPE_DropDown_Resouce_Menu * objectReplacerInEditor;
        GPE_SceneGameObject * selectedSceneObject;
        int customObjCreationId;
        GPE_ToolLabelButton * closeObjectProperties;
        GPE_TextInputNumber * levelObjGridWidthField;
        GPE_TextInputNumber * levelObjGridHeightField;
        GPE_CheckBoxBasic * useObjGridCheckBox;
        GPE_CheckBoxBasic * viewTileGridCheckBox;
        GPE_ToolLabelButton * forceSnapButton;
        GPE_CheckBoxBasic * objectLeftClickModeButton;
        GPE_CheckBoxBasic * onlyRemoveThisObjectTypeButton;
        GPE_ToolLabelButton * removeObjectButton;
        GPE_ToolLabelButton * clearObjectsButton;

        GPE_Label_Text * objectEditorNameTitle;
        GPE_Label_Text * objectEditorPropertiesTitle;
        int objSnapX, objSnapY;
        bool showObjLines;
        GPE_TextInputNumber * selectedObjXPos;
        GPE_TextInputNumber * selectedObjYPos;

        GPE_ToolIconButton * objCustomVariableSettingsButtton;
        GPE_ToolIconButton * objCustomVariableAddButtton;
        GPE_ToolIconButton * objCustomVariableRemoveButton;
        GPE_ToolIconButton * objCustomVariableRefeshButton;
        //used for the tile placement tab
        bool showTileLines;
        float lvlTileWidth, lvlTileHeight;
        float lvlTileAmountX, lvlTileAmountY;
        float tileToPlaceX1,tileToPlaceY1, tileToPlaceX2,tileToPlaceY2;
        int tilesToPlacePerRow;
        SDL_Rect tsPlacementArea;
        GPE_SelectBoxBasic * layerMenu;
        GPE_DropDown_Resouce_Menu * tilesheetDropDown;

        int layerInEdit;
        int tileIdPlace;
        float sceneMouseXPos, sceneMouseYPos;
        int lastCreatedObjXPos, lastCreatedObjYPos;
        int lastCreatedObjTypeId;
        SDL_Rect tsRenderRect;
        tilesheetPreviewer * tSPreviewer;
        int tilesheetRenderXPos,tilesheetRenderYPos;
        tilesheetResource * tsRes;
        std::vector <sceneLayer* >  sceneLayers;
        sceneLayer * defaultObjectLayer;
        sceneLayer * defaultBackgroundLayer;
        std::vector< int > tileIdsToPlace;
        GPE_ToolIconButton * layerSettingsButtton;
        GPE_ToolIconButton * layerAddButtton;
        GPE_ToolIconButton * layerRemoveButton;
        GPE_ToolIconButton * layerMoveUpButtton;
        GPE_ToolIconButton * layerMoveDownButton;
        GPE_ToolIconButton * layerToggleHideButton;

        GPE_ToolIconButton * layerBackgroundSettingsButtton;
        GPE_ToolIconButton * layerBackgroundAddButtton;
        GPE_ToolIconButton * layerBackgroundRemoveButton;
        GPE_ToolIconButton * layerBackgroundMoveUpButtton;
        GPE_ToolIconButton * layerBackgroundMoveDownButton;

        GPE_Label_Text * layerErrorMessage;
        bool sceneAreaScrollable;
        bool isDraggingObject;
    public:
        GPE_Rect sceneRect;
        GPE_GuiElementList * layerPaneList;
        bool scnPostProcessed;
        GPE_ScrollBar_XAxis * sceneXScroll;
        GPE_ScrollBar_YAxis * sceneYScroll;

        GPE_ToolIconButton * saveButton;
        gameSceneResource(GPE_ResourceContainer * pFolder = NULL);
        ~gameSceneResource();
        sceneLayer * add_layer(int layerType, int newLayerProjectLayerId );
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);

        sceneLayer * find_layer(int layeToFind);

        bool get_mouse_coords(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void handle_scrolling();
        void inherit_components(GPE_SceneGameObject * objectToInherit,gameObjectResource * objParent);
        void manage_components(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_scene_layers(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, GPE_Rect * renderZone = NULL, GPE_Rect * sceneCamera = NULL, double renderScale = -1,bool showEditorPreviews = true, bool checkListDependent = true,  bool forceRedraw = true);
        void render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void reset_placement_info();
        void save_resource(std::string alternatePath = "", int backupId = -1);
        void select_object(GPE_SceneGameObject * objToSelect);
        bool unselect_object();
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        void update_project_layers();
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class GPE_PathPoint
{
    public:

        float xPos;
        float yPos;
        float pointSpeed;
        GPE_PathPoint(int pointX, int pointY, float speed = 1);
        ~GPE_PathPoint();
};

class gamePathResource: public standardEditableGameResource
{
    public:
        int pointPos;
        int selectedPointPos;
        GPE_PathPoint * selectedPathPoint;
        GPE_TextInputNumber * currentPointX;
        GPE_TextInputNumber * currentPointY;
        GPE_TextInputNumber * currentPointSpeed;
        GPE_Rect sceneEditorView;
        GPE_Rect scenePreviewRect;
        GPE_Rect sceneRect;
        SDL_Rect editorPane;
        SDL_Rect editorCommentPane;
        GPE_DropDown_Resouce_Menu * sceneToPreview;
        GPE_DropDown_Menu * sceneZoomLevel;
        double sceneZoomAmount;
        std::vector <GPE_PathPoint * >  pathPoints;
        GPE_SelectBoxBasic * pathOptions;
        GPE_ToolIconButton * pointSettingsButtton;
        GPE_ToolIconButton * pointRemoveButton;
        GPE_ToolIconButton * pointMoveUpButtton;
        GPE_ToolIconButton * pointMoveDownButton;

        GPE_Color * pathLineColor;
        GPE_Color * pathPointColor;
        GPE_RadioButtonControllerBasic * pathOpenType;
        GPE_RadioButtonControllerBasic * pathShapeType;
        bool scnPostProcessed;
        GPE_ScrollBar_XAxis * sceneXScroll;
        GPE_ScrollBar_YAxis * sceneYScroll;
        bool sceneAreaScrollable;
        float sceneMouseXPos, sceneMouseYPos;
        gamePathResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePathResource();
        GPE_PathPoint * add_point( int pointX, int pointY, float pointSpeed = 1);
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        void clear_points();
        bool export_and_play_native( bool launchProgram = true);
        bool get_mouse_coords(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void handle_scrolling();
        void integrate_into_synthax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        bool remove_point( int pointId );
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void update_project_layers();
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class definitionResource
{
    public:
        std::string myKey;
        std::string myValue;
        GPE_TextInputBasic * keyField;
        GPE_TextInputBasic * valueField;
        GPE_ToolIconButton * removeSelfButton;
        definitionResource(std::string key, std::string value);
        ~definitionResource();
};

class dictionaryResource: public standardEditableGameResource
{
    public:
        std::vector <definitionResource * >  myDefinitions;
        GPE_ToolPushButton * addDefinitionButton;
        GPE_ToolPushButton * clearDictionaryButton;
        dictionaryResource(GPE_ResourceContainer * pFolder = NULL);
        ~dictionaryResource();
        definitionResource * add_definition( std::string key, std::string value ="");
        void clear_dictionary();
        bool export_and_play_native( bool launchProgram = true);
        bool get_mouse_coords(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void integrate_into_synthax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void update_project_layers();
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
        bool word_exists( std::string needleWord, int allowedPos = -1);
};


class spreadSheetResource: public standardEditableGameResource
{
    public:
        int pointPos;
        GPE_Rect sceneEditorView;
        GPE_Rect scenePreviewRect;
        GPE_Rect sceneRect;
        SDL_Rect editorPane;
        SDL_Rect editorCommentPane;
        GPE_DropDown_Resouce_Menu * sceneToPreview;
        GPE_DropDown_Menu * sceneZoomLevel;
        double sceneZoomAmount;
        std::vector <GPE_PathPoint * >  pathPoints;
        GPE_SelectBoxBasic * pathOptions;
        GPE_ToolIconButton * pointSettingsButtton;
        GPE_ToolIconButton * pointRemoveButton;
        GPE_ToolIconButton * pointMoveUpButtton;
        GPE_ToolIconButton * pointMoveDownButton;

        GPE_Color * pathLineColor;
        GPE_Color * pathPointColor;
        GPE_RadioButtonControllerBasic * pathOpenType;
        GPE_RadioButtonControllerBasic * pathShapeType;
        bool scnPostProcessed;
        GPE_ScrollBar_XAxis * sceneXScroll;
        GPE_ScrollBar_YAxis * sceneYScroll;
        bool sceneAreaScrollable;
        float sceneMouseXPos, sceneMouseYPos;
        spreadSheetResource(GPE_ResourceContainer * pFolder = NULL);
        ~spreadSheetResource();
        GPE_PathPoint * add_point( int pointX, int pointY, float pointSpeed = 1);
        void clear_points();
        bool export_and_play_native( bool launchProgram = true);
        bool get_mouse_coords(SDL_Rect *viewedSpace = NULL,SDL_Rect *cam = NULL);
        void handle_scrolling();
        void integrate_into_synthax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void update_project_layers();
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};


class projectPropertiesResource: public standardEditableGameResource
{
    public:
        //Icon data and such
        std::string iconFileName;
        GPE_Label_Text * projectIconLabel;
        GPE_Label_Image * projectIconImg;
        GPE_ToolLabelButton * browseIconButton;
        //Meta information to be built into the project
        GPE_TextAreaInputBasic * projectGameMacros;
        GPE_TextAreaInputBasic * projectGameNotes;
        GPE_CheckBoxBasic * showMetaData;

        //Export Settings Tab
        GPE_SelectBoxBasic * exportSettingsBar;
        //Export Settings [ HTML5]
        GPE_TabBar * projectHTML5SettingsTabBar;

        GPE_TextAreaInputBasic * projectCSSCode;
        GPE_TextAreaInputBasic * projectHtmlCode;
        GPE_TextAreaInputBasic * projectHtmlHeaderCode;

        GPE_CheckBoxBasic * obfuscateCodeCheckbox;
        GPE_CheckBoxBasic * minifyCodeCheckbox;
        GPE_CheckBoxBasic * warnOnCloseCheckbox;
        GPE_CheckBoxBasic * headerCodeBeforeGPECheckbox;

        GPE_ToolPushButton * exportPushButton;
        GPE_ToolPushButton * exportAndPlayPushButton;
        GPE_ToolPushButton * cleanBuildFolderPushButton;
        GPE_ToolPushButton * cleanBuildAllFolderPushButton;
        GPE_ToolPushButton * playProgramPushButton;

        //[Export Settings Desktop]
        GPE_TabBar * desktopSettingsTabBar;
        GPE_Label_Text * exportApplicationLabel;
        GPE_Label_Text * exportWindowsLabel;
        GPE_Label_Text * exportOSXLabel;
        GPE_Label_Text * exportLinuxLabel;
        GPE_CheckBoxBasic * win64BitCheckbox;
        GPE_CheckBoxBasic * osx64BitCheckbox;
        GPE_CheckBoxBasic * linux64BitCheckbox;

        GPE_CheckBoxBasic * html5DebugModeCheckbox;
        GPE_CheckBoxBasic * wiiuDebugModeCheckbox;
        GPE_CheckBoxBasic * winDebugModeCheckbox;
        GPE_CheckBoxBasic * osxDebugModeCheckbox;
        GPE_CheckBoxBasic * linuxDebugModeCheckbox;

        GPE_RadioButtonControllerBasic * html5nativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * windowsNativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * osxNativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * linuxNativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * wiiuNativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * switchNativeBuildTypeOptions;
        GPE_Label_Text * electronCompileNote;
        GPE_TextURL * electronCompileUrl;

        GPE_Label_Title * comingSoonTitle;
        GPE_Label_Text * wiiuCompileNote;
        GPE_TextURL * wiiuCompileUrl;

        GPE_Label_Paragraph * switchComingSoonNote;
        GPE_Label_Paragraph * nSwitchSideNote;
        GPE_Label_Paragraph * nSwitchThirdNote;
        GPE_TextURL * nSwitchContactUrl;

        GPE_Label_Paragraph * ps4ComingSoonNote;
        GPE_Label_Paragraph * ps4SideNote;
        GPE_TextURL * ps4ContactUrl;

        GPE_Label_Paragraph * xboxOneComingSoonNote;
        GPE_Label_Paragraph * xboxOneSideNote;
        GPE_TextURL * xboxOneContactUrl;

        //Settings Tab
        GPE_TextInputNumber * projectScreenWidthField;
        GPE_TextInputNumber * projectScreenHeightField;
        GPE_TextInputNumber * projectGameVersionField;
        GPE_TextInputBasic * projectGameSubVersionField;
        GPE_RadioButtonControllerBasic * projectScaleSettings;
        GPE_Label_Text * projectSettingsFPSRateLabel;
        GPE_DropDown_Menu * projectSettingsFPSRate;
        //Publisher Setting
        GPE_Label_Title * Title_Publisher;
        GPE_Label_Title * Title_GameSettings;
        GPE_TextInputBasic * projectGameTitleField;
        GPE_TextInputBasic * projectGameShortTitleField;
        GPE_TextInputBasic * projectGameDateField;
        GPE_TextInputBasic * projectGameDescriptionField;
        GPE_TextInputBasic * projectGamePublisherField;
        GPE_TextInputBasic * projectGameDeveloperField;
        GPE_TextInputBasic * projectGameCopyrightField;
        GPE_TextInputBasic * projectGameWebsiteField;
        GPE_TextInputBasic * projectGameEmailField;
        GPE_CheckBoxBasic * checkBoxShowPublisherInfo;

        //Colors Tab
        GPE_Input_Field_Color * projectBorderColor;
        GPE_Input_Field_Color * projectGameBackgroundColor;
        GPE_Input_Field_Color * projectWebsiteBackgroundColor;
        GPE_Input_Field_Color * projectTextHeaderColor;
        GPE_Input_Field_Color * projectTextParagraphColor;
        GPE_Input_Field_Color * projectTextLinkColor;
        GPE_Input_Field_Color * projectTextLinkActiveColor;
        GPE_Input_Field_Color * projectTextLinkHoverColor;
        GPE_Input_Field_Color * projectTextLinkVisitedColor;
        //Extras Tab
        GPE_ToolPushButton * transformImageDirectoryButton;
        SDL_Rect subViewedSpace;
        GPE_TabBar * projectSettingsTabBar;
        GPE_ToolIconButtonBar * sceneEditorPaneMode;

        GPE_GuiElementList * projectSettingsList;
        GPE_CheckBoxBasic * projectCollisionLayerMatrix[1024];
        GPE_Label_90Degree * projectLayerNamesColumnTitles[32];
        GPE_Label_Text * projectLayerNamesRowTitles[32];
        GPE_Label_Text * projectLayerEmptyTitle;
        GPE_ToolLabelButton * selectAllCollisionBoxes;
        GPE_ToolLabelButton * unselectAllCollisionBoxes;

        projectPropertiesResource(GPE_ResourceContainer * pFolder = NULL);
        ~projectPropertiesResource();
        bool export_and_play_native( bool launchProgram = true);
        void integrate_into_synthax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void update_project_layers();
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const int GPE_EXTERNAL_EDITOR_MAX = 4;

const int GPE_EXTERNAL_EDITOR_IMG = 0;
const int GPE_EXTERNAL_EDITOR_AUD = 1;
const int GPE_EXTERNAL_EDITOR_VID = 2;
const int GPE_EXTERNAL_EDITOR_FONT = 3;

class gamePencilEditorSettingsResource: public standardEditableGameResource
{
    public:
        //Compiler Settings
        GPE_CheckBoxBasic * stopCompileOnError;
        GPE_CheckBoxBasic * useStrictMode;
        GPE_CheckBoxBasic * showShortProjectNames;
        //General Editor Settings
        GPE_CheckBoxBasic * autoSaveScreenshots;
        GPE_CheckBoxBasic * makeMetaScreenshots;
        GPE_Label_Title * editorGuiSettingsLabel;
        GPE_Label_Text * ideSettingsFPSRateLabel;
        GPE_DropDown_Menu * ideSettingsFPSRate;
        GPE_Label_Text *    ideButtonBarSizeLabel;
        GPE_DropDown_Menu * ideButtonBarSize;
        GPE_RadioButtonControllerBasic * ideButtonBarAlignment;
        GPE_TextInputBasic * pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_MAX];
        GPE_ToolIconButton * pencilExternalEditorsLoadButton[GPE_EXTERNAL_EDITOR_MAX];
        GPE_SelectBoxBasic * subEditorSelectBox;

        GPE_TextInputNumber * editorTextAreaDelayTime;
        GPE_TextInputNumber * editorTextInputDelayTime;
        GPE_TextInputNumber * editorNormalDelayTime;
        GPE_TextInputNumber * editorScrollSpeed;

        GPE_Label_Title * editorStatupTipsLabel;
        GPE_CheckBoxBasic * showStatupTipsBox;
        GPE_CheckBoxBasic * launchLastProjectBox;
        GPE_DropDown_Menu * projectAutoSaveRate;
        GPE_Label_Text * projectAutoSaveRateLabel;

        //Tab Section
        GPE_Label_Title * tabSectionLabel;
        GPE_CheckBoxBasic * mouseAutoFindTabs;
        GPE_TextInputNumber * tabSpaceSize;
        int tabSpaceCount;

        //Advanced Section
        GPE_Label_Title * advancedAreaLabel;
        GPE_CheckBoxBasic * showHiddenFilesInBrowser;
        GPE_CheckBoxBasic * forceFrameRedraw;
        GPE_ToolPushButton * clearCacheButton;

        SDL_Rect gpeLogoSpace;
        SDL_Rect subViewedSpace;
        GPE_TabBar * editorPageTabBar;
        GPE_GuiElementList * editorPageList;
        std::string projectFolderListLocation;
        gamePencilEditorSettingsResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilEditorSettingsResource();
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gamePencilEditorSettingsResource * MAIN_EDITOR_SETTINGS;

class gamePencilAboutPageResource: public standardEditableGameResource
{
    public:
        //Meta information to be built into the project
        GPE_TextAreaInputBasic * pencilCredits;
        GPE_TextAreaInputBasic * pencilDescription;
        GPE_TextAreaInputBasic * pencilInformation;
        GPE_TextAreaInputBasic * pencilLicense;
        GPE_TextAreaInputBasic * pencilMissionStatement;
        SDL_Rect subViewedSpace;
        GPE_TabBar * aboutPageTabBar;
        GPE_GuiElementList * aboutPageList;
        gamePencilAboutPageResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilAboutPageResource();
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilAboutPageResource * MAIN_ABOUT_PAGE;

class gamePencilHelpPageResource: public standardEditableGameResource
{
    public:
        //General Help Tab
        GPE_Label_Title * helpfulButtonsTitle;
        GPE_Label_Title * helpfulLinksTitle;
        GPE_Label_Title * socialMediaTitle;
        std::vector< GPE_ToolPushButton * > helpfulButtons;
        std::vector< GPE_TextURL * > helpfulLinks;
        std::vector< GPE_ToolPushButton * > socialMediaSites;

        GPE_TextAreaInputBasic * pencilInformation;
        GPE_TextAreaInputBasic * pencilChangelog;
        GPE_TextAreaInputBasic * pencilShortkeys;
        GPE_ToolPushButton * updateCheckerButton;
        SDL_Rect subViewedSpace;
        GPE_TabBar * helpPageTabBar;
        GPE_GuiElementList * helpPageList;
        gamePencilHelpPageResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilHelpPageResource();
        void add_helpfulbutton(GPE_ToolPushButton * newButton);
        void add_helpfullink(GPE_TextURL * newLink);
        void add_socialmedia_url(GPE_ToolPushButton * newLink);
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilHelpPageResource * MAIN_HELP_PAGE;

class gamePencilStartPageResource: public standardEditableGameResource
{
    public:
        //Meta information to be built into the project
        SDL_Rect subViewedSpace;
        GPE_WrappedTextArea * testText;
        GPE_Label_Title * labelEditorShortcuts;
        //GPE_Label_Title * labelPatreon;
        //GPE_Label_Image * labelPatreonLogo;
        //GPE_Label_Text * labelPatreonDescription;
        //GPE_TextURL * urlPatreonPage;

        GPE_Label_Title * labelHelpfulLinks;
        GPE_Label_Image * labelImgGPELogo;
        GPE_Label_Text * labelVersionInfo;
        GPE_Label_Text * labelCopyrightInfo;
        //Social Media
        GPE_Label_Title * labelSocialMedia;
        GPE_ToolPushButton * socialFacebookButton;
        GPE_ToolPushButton * socialGooglePlusButton;
        GPE_ToolPushButton * socialTwitterButton;
        GPE_ToolPushButton * socialTilweButton;
        GPE_ToolPushButton * socialYouTubeButton;
        GPE_TabBar * startPageTabBar;
        std::vector< GPE_TextURL * > editorShortcuts;
        std::vector< GPE_TextURL * > helpfulWebLinks;
        GPE_ToolPushButton * projectBrowserButton;
        GPE_ToolPushButton * forumPageButton;
        GPE_ToolPushButton * helpPageButton;
        GPE_ToolPushButton * tipOfTheDayButton;

        GPE_GuiElementList * startPageList;
        gamePencilStartPageResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilStartPageResource();
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilStartPageResource * MAIN_START_PAGE;

class quickProjectReader
{
    public:
        int resourcesInProject;
        std::string detectedName;
        std::string detectedIcon;
        std::string detectedLanguage;
        std::string detectedLocation;
        std::string detectedSaveTime;
        std::string detectedVersion;
        quickProjectReader();
        ~quickProjectReader();
        bool review_project(std::string projectFileName);
};

class gamePencilProjectBrowserResource: public standardEditableGameResource
{
    public:
        bool processedFirst;
        //Meta information to be built into the project
        GPE_Label_Title * projectsBrowserTitle;
        GPE_Label_Text * projectFolderLocationLabel;
        GPE_Label_Text * projectsTitle;
        GPE_Label_Text * recentProjectsTitle;
        GPE_GuiElementList * projectBrowserList;
        GPE_ToolPushButton * scanProjectFolderButton;
        GPE_ToolPushButton * newProjectButton;
        GPE_ToolPushButton * loadProjectButton;
        GPE_ToolPushButton * refreshButton;
        GPE_Label_Text * emptyProjectsListLabel;
        std::string foundProjectsDirectory;
        std::vector< GPE_ToolPushButtonMultiLine * > foundProjectsList;
        std::vector< GPE_ToolPushButtonMultiLine * > recentProjectsList;
        gamePencilProjectBrowserResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilProjectBrowserResource();
        void prerender_self(GPE_Renderer * cRender);
        void preprocess_self(std::string alternatePath = "");
        void process_self(SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL);
        void render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace = NULL,SDL_Rect *cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
        void load_projecs_folder( std::string projectsDirectory);
};

extern gamePencilProjectBrowserResource * PROJECT_BROWSER_PAGE;

class GPE_Editor_State : public ProgramState
{
    private:
        //Intro message
        GPE_Texture * message;
        GPE_Toolbar * mainToolBar;
        GPE_ToolIconButtonBar * mainButtonBar;
        GPE_ResourceManagementBar * mainResourceBar;
    protected:
        //Intro background
        GPE_Texture *background;
    public:
        GPE_Texture * country_language_image;
        SDL_version SDL_compiled_version;
        std::string SDL_VersionText;
        //Loads intro resources
        GPE_Editor_State();
        //Frees intro resources
        ~GPE_Editor_State();

        //Main loop functions
        virtual void process_input();
        virtual void apply_logic();
        virtual void clean_up();
        virtual void render();
};

const int DISPLAY_QUERY_YES = 1;
const int DISPLAY_QUERY_NO = 0;
const int DISPLAY_QUERY_CANCEL = 2;
int get_popupmenu_result(SDL_Rect * camera = NULL, bool redrawScreen = false);

void display_user_messaage();
void display_user_alert(std::string messageTitle, std::string messageContent, int messageType = 0,SDL_Rect * camera = NULL );
int display_get_prompt(std::string messageTitle, std::string messageContent, bool showCancelButton = false );
std::string get_string_from_popup(std::string messageTitle, std::string messageContent, std::string startName = "");

int modify_folder_images(std::string folderLocation, int modifcationType);
int clean_folder(std::string folderName);
bool copy_folder(std::string folderName, std::string targetFolder, bool copySubFolders = false);
std::string get_user_settings_folder();
std::string get_user_temp_folder();
std::string get_user_screenshot_folder();

bool has_early_tab(std::string strIn);
std::string untab_string(std::string strIn);
std::string generate_tabs(int tabCount);
int get_tab_space_count();
#endif
