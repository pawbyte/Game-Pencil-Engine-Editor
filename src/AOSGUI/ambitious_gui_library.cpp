/*
ambitious_gui_library.cpp
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
#include "ambitious_gui_library.h"

SDL_Cursor * GPE_Cursor = NULL;
SDL_Cursor * GPE_LoadingCursor = NULL;
SDL_Cursor * GPE_HoverCursor = NULL;

SDL_SystemCursor GPE_PreviousCursor = SDL_SYSTEM_CURSOR_WAIT;
SDL_SystemCursor GPE_CurrentCursor = SDL_SYSTEM_CURSOR_ARROW;
PAW_GUI_SEARCH_CONTROLLER * MAIN_SEARCH_CONTROLLER = NULL;
GPE_Overlay_System * MAIN_OVERLAY = NULL;
GPE_PopUpMenu_Option * MAIN_CONTEXT_MENU = NULL;

//Define GUI fonts to NULL
GPE_Font * font = NULL;
GPE_Font * textboxFont = NULL;
GPE_Font * FONT_CATEGORY_BAR = NULL;
//
GPE_Font * DEFAULT_FONT = NULL;
GPE_Font * FONT_POPUP = NULL;
GPE_Font * FONT_TOOLBAR = NULL;
GPE_Font * FONT_RESOURCEBAR = NULL;
GPE_Font * FONT_TEXTINPUT = NULL;
GPE_Font * FONT_TEXTINPUT_COMMENT = NULL;
GPE_Font * FONT_TEXTINPUT_KEYWORD = NULL;
GPE_Font * FONT_TEXTINPUT_FUNCTION = NULL;
GPE_Font * FONT_TEXTINPUT_NUMBER = NULL;
GPE_Font * FONT_TEXTINPUT_QUOTE = NULL;
GPE_Font * FONT_TEXTINPUT_BOLD = NULL;
GPE_Font * FONT_TEXTINPUT_SYMBOL = NULL;
GPE_Font * FONT_TEXTINPUT_PROJECT_FUNCTION = NULL;
GPE_Font * FONT_TEXTINPUT_PROJECT_KEYWORD = NULL;

GPE_Font * FONT_TERM_NAME = NULL;
GPE_Font * FONT_TERM_DESCRIPTION = NULL;
GPE_Font * FONT_TERM_SCOPE = NULL;
GPE_Font * FONT_TOOLTIP = NULL;

GPE_Font * FONT_DEFAULT_PROMPT = NULL;
GPE_Font * FONT_CHECKBOX = NULL;
GPE_Font * FONT_PARAGRAGH = NULL;
GPE_Font * FONT_LABEL = NULL;
GPE_Font * FONT_LABEL_ANCHOR = NULL;
GPE_Font * FONT_LABEL_PARAGRAPH = NULL;
GPE_Font * FONT_STATUSBAR = NULL;
GPE_Font * FONT_HEADER = NULL;
GPE_Font * FONT_LABEL_TITLE = NULL;

//Define GUI Templates to NULL
GPE_Template * GPE_MAIN_TEMPLATE = NULL;

GPE_Template * GPE_CUSTOM_TEMPLATE = NULL;
GPE_Template * GPE_DEFAULT_TEMPLATE = NULL;

GPE_Template * GPE_DARK_TEMPLATE = NULL;
GPE_Template * GPE_LIGHT_TEMPLATE = NULL;
GPE_Template * GPE_RED_TEMPLATE = NULL;
GPE_Template * GPE_GREEN_TEMPLATE = NULL;
GPE_Template * GPE_BLUE_TEMPLATE = NULL;

GPE_StatusBar * GPE_Main_Statusbar = NULL;
GPE_GUI_Settings * MAIN_GUI_SETTINGS = NULL;
std::string CURRENT_PROJECT_NAME = "";

int POPUP_FONT_SIZE_WIDTH = 12;
int POPUP_FONT_SIZE_HEIGHT = 12;
int POPUP_MENU_VALUE = -1;

GPE_GUI_Settings::GPE_GUI_Settings()
{
    textAreaFindBox.x = 0;
    textAreaFindBox.y = 0;
    textAreaFindBox.w = 32;
    textAreaFindBox.h = 32;
    autoFindMouseTabs = true;
    textAreaDelayTime = 4;
    textInputDelayTime = 4;
    normalInputDelayTime = 0;
    tabSpaceCount = 4;

    defaultLineCountWidth = 48;
    showTextEditorLineCount = true;
    showTextEditorSyntaxHightlight = true;


    fileOpenAudioDir = "";
    fileOpenSpriteDir = "";
    fileOpenTextureDir = "";
    fileOpenTilesheetDir = "";
    fileOpenFontDir = "";
    fileOpenFunctionDir = "";
    fileOpenTextFileDir = "";
    fileOpenImportFileDir = "";
    fileSaveExportFileDir = "";
    fileSaveTextFileDir = "";
    fileOpenDefaultDir = "";
    fileSaveDefaultDir = "";
}

GPE_GUI_Settings::~GPE_GUI_Settings()
{


}



PAW_GUI_SEARCH_CONTROLLER::PAW_GUI_SEARCH_CONTROLLER()
{
    previousSearchMode = SEARCH_MODE_FINDALL;
    textSearchMode = 0;
    showFindAllResults = false;
    findTextStringBox = new GPE_TextInputBasic("");
    findTextStringBox->resubmissionAllowed = true;
    findTextStringBox->set_width(128);
    replaceTextStringBox = new GPE_TextInputBasic("");
    replaceTextStringBox->set_width(128);
    goToLineStringBox = new GPE_TextInputNumber("");
    findButton = new GPE_ToolLabelButton(0,0,"Find","Find String");
    goToButton = new GPE_ToolLabelButton(0,0,"Go To","Go To This Line in Text Area");

    findAllButton = new GPE_ToolLabelButton(0,0,"Find All","Find All Copies of String in Text Area");
    replaceButton = new GPE_ToolLabelButton(0,0,"Replace","Replace Next String in Text Area");
    replaceAllButton = new GPE_ToolLabelButton(0,0,"Replace All","Replace All Copries of Strings in Text Area");

    findMatchCase = new GPE_CheckBoxBasic("Match case","Search is case-sensitive",0,0,true);
    scopeLabel = new GPE_Label_Text("Scope:     ","Scope of your search");
    findScope = new GPE_DropDown_Menu(0,0,"Scope");
    findScope->add_menu_option("Open Tabs","Open Tabs",0,false);
    findScope->add_menu_option("Project Resources","Project Resources",1,true);
    findScope->add_menu_option("Workspace Resources","Workspace Resources",2);
}

PAW_GUI_SEARCH_CONTROLLER::~PAW_GUI_SEARCH_CONTROLLER()
{
     if( findTextStringBox!=NULL)
    {
        delete findTextStringBox;
        findTextStringBox = NULL;
    }
    if( goToLineStringBox!=NULL)
    {
        delete findTextStringBox;
        goToLineStringBox = NULL;
    }
    if( replaceTextStringBox!=NULL)
    {
        delete replaceTextStringBox;
        replaceTextStringBox = NULL;
    }
    if( findButton!=NULL)
    {
        delete findButton;
        findButton = NULL;
    }
    if( scopeLabel!=NULL)
    {
        delete scopeLabel;
        scopeLabel = NULL;
    }
    if( findScope!=NULL)
    {
        delete findScope;
        findScope = NULL;
    }

    if( findAllButton!=NULL)
    {
        delete findAllButton;
        findAllButton = NULL;
    }
    if( goToButton!=NULL)
    {
        delete goToButton;
        goToButton = NULL;
    }
    if( replaceButton!=NULL)
    {
        delete replaceButton;
        replaceButton = NULL;
    }
    if( replaceAllButton!=NULL)
    {
        delete replaceAllButton;
        replaceAllButton = NULL;
    }
    if( findMatchCase!=NULL)
    {
        delete findMatchCase;
        findMatchCase = NULL;
    }
}

void PAW_GUI_SEARCH_CONTROLLER::close_finder()
{
    if( textSearchMode >= SEARCH_MODE_FIND)
    {
        previousSearchMode = textSearchMode;
    }
    textSearchMode = -1;
    showFindAllResults = false;
}
bool PAW_GUI_SEARCH_CONTROLLER::using_search()
{
    switch( textSearchMode)
    {
        case SEARCH_MODE_FIND:
            if( findTextStringBox->is_inuse() )
            {
                return true;
            }
        break;

        case SEARCH_MODE_GOTO:
            if( goToLineStringBox->is_inuse() )
            {
                return true;
            }
        break;
        case SEARCH_MODE_FINDALL:
            if( findTextStringBox->is_inuse() )
            {
                return true;
            }
            if( replaceTextStringBox->is_inuse() )
            {
                return true;
            }
        break;
        default:
            return false;
        break;
    }
    return false;
}


void GPE_change_cursor(SDL_SystemCursor id)
{
    GPE_CurrentCursor = id;
}


GPE_Rect * GPE_find_camera(GPE_Rect * rectIn)
{
    if(rectIn==NULL)
    {
        return &GPE_DEFAULT_CAMERA;
    }
    return rectIn;
}

bool load_fonts()
{
    std::string mainGuiFontLocation = APP_DIRECTORY_NAME+"resources/fonts/dejavu_sans_mono/DejaVuSansMono.ttf";
    std::string textEditorFontLocation = APP_DIRECTORY_NAME+"resources/fonts/dejavu_sans_mono/DejaVuSansMono.ttf";
    bool fontIsMonoSpaced = true;

    if( file_exists("C:/Windows/Fonts/Arial.ttf") )
    {
        mainGuiFontLocation = "C:/Windows/Fonts/Arial.ttf";
        fontIsMonoSpaced = false;
    }
    else if( file_exists("C:/Windows/Fonts/Carlito.ttf") )
    {
        mainGuiFontLocation = "C:/Windows/Fonts/Carlito.ttf";
        fontIsMonoSpaced = false;
    }
    else if( file_exists("C:/Windows/Fonts/Candara.ttf") )
    {
        mainGuiFontLocation = "C:/Windows/Fonts/Candara.ttf";
        fontIsMonoSpaced = false;
    }

    //Open the fonts
    font = GPE_OpenFont( mainGuiFontLocation, 24,fontIsMonoSpaced, "Generic Font");
    textboxFont = GPE_OpenFont( mainGuiFontLocation, 24,fontIsMonoSpaced, "textboxFont");
    FONT_CATEGORY_BAR = GPE_OpenFont( mainGuiFontLocation, 14,fontIsMonoSpaced , "FONT_CATEGORY_BAR");

    LAST_USED_FONT = GPE_OpenFont(mainGuiFontLocation,24,fontIsMonoSpaced, "LAST_USED_FONT");
    DEFAULT_FONT = GPE_OpenFont(mainGuiFontLocation,14,fontIsMonoSpaced, "DEFAULT_FONT");

    FONT_POPUP = GPE_OpenFont(mainGuiFontLocation,14,fontIsMonoSpaced, "FONT_POPUP");
    FONT_TOOLBAR = GPE_OpenFont(mainGuiFontLocation,14,fontIsMonoSpaced, "FONT_TOOLBAR");
    FONT_RESOURCEBAR = GPE_OpenFont(textEditorFontLocation,12,fontIsMonoSpaced, "FONT_RESOURCEBAR");

    FONT_TEXTINPUT = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TEXTINPUT_GENERAL");
    FONT_TEXTINPUT_COMMENT = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TEXTINPUT_COMMENT");
    FONT_TEXTINPUT_KEYWORD = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TEXTINPUT_KEYWORD");
    FONT_TEXTINPUT_FUNCTION = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TEXTINPUT_FUNCTION");
    FONT_TEXTINPUT_NUMBER = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TEXTINPUT_NUMBER");
    FONT_TEXTINPUT_QUOTE = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TEXTINPUT_QUOTE");
    FONT_TEXTINPUT_SYMBOL = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TEXTINPUT_SYMBOL");
    FONT_TEXTINPUT_PROJECT_FUNCTION = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TEXTINPUT_PROJECT_FUNCTION");
    FONT_TEXTINPUT_PROJECT_KEYWORD = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TEXTINPUT_PROJECT_KEYWORD");

    FONT_TERM_NAME = GPE_OpenFont(textEditorFontLocation,11,true, "FONT_TERM_NAME");
    FONT_TERM_DESCRIPTION = GPE_OpenFont(textEditorFontLocation,11,true, "FONT_TERM_DESCRIPTION");
    FONT_TERM_SCOPE = GPE_OpenFont(textEditorFontLocation,11,true, "FONT_TERM_SCOPE");
    FONT_TOOLTIP = GPE_OpenFont(textEditorFontLocation,12,true, "FONT_TOOLTIP");

    FONT_DEFAULT_PROMPT = GPE_OpenFont(textEditorFontLocation,16,true, "FONT_DEFAULT_PROMPT");
    FONT_CHECKBOX = GPE_OpenFont(mainGuiFontLocation,12,fontIsMonoSpaced, "FONT_CHECKBOX");

    FONT_HEADER = GPE_OpenFont( mainGuiFontLocation, 18,fontIsMonoSpaced , "FONT_HEADER");
    FONT_LABEL = GPE_OpenFont( mainGuiFontLocation, 14,fontIsMonoSpaced , "FONT_LABEL");
    FONT_LABEL_ANCHOR = GPE_OpenFont( mainGuiFontLocation, 14,fontIsMonoSpaced , "FONT_LABEL_ANCHOR");
    FONT_LABEL_TITLE = GPE_OpenFont( mainGuiFontLocation, 24,fontIsMonoSpaced , "FONT_LABEL_TITLE");
    FONT_PARAGRAGH = GPE_OpenFont( textEditorFontLocation, 14,true, "FONT_PARAGRAGH" );
    FONT_STATUSBAR = GPE_OpenFont( textEditorFontLocation, 12,true, "FONT_STATUSBAR" );

    if(DEFAULT_FONT==NULL || LAST_USED_FONT==NULL)
    {
        record_error("Unable to load default fonts");
        return false;
    }

    if(FONT_TOOLBAR==NULL)
    {
        record_error("Unable to load TOOLLBAR font");
        return false;

    }

    if(FONT_TEXTINPUT==NULL)
    {
        record_error("Unable to load INPUT-FIELD font");
        return false;
    }
    return true;
}

void cleanup_fonts()
{
    //Close the fonts that was used
    if( font!=NULL)
    {
        GPE_CloseFont(font);
        font = NULL;
    }

    if( textboxFont!=NULL)
    {
        GPE_CloseFont( textboxFont );
        textboxFont = NULL;
    }

    if( FONT_CATEGORY_BAR!=NULL)
    {
        GPE_CloseFont( FONT_CATEGORY_BAR );
        FONT_CATEGORY_BAR = NULL;
    }
    //
    if( DEFAULT_FONT!=NULL)
    {
        GPE_CloseFont(DEFAULT_FONT);
        DEFAULT_FONT = NULL;
    }

    if( FONT_POPUP!=NULL)
    {
        GPE_CloseFont( FONT_POPUP );
        FONT_POPUP = NULL;
    }

    if( FONT_TOOLBAR!=NULL)
    {
        GPE_CloseFont( FONT_TOOLBAR );
        FONT_TOOLBAR = NULL;
    }
    if( FONT_RESOURCEBAR!=NULL)
    {
        GPE_CloseFont( FONT_RESOURCEBAR );
        FONT_RESOURCEBAR = NULL;
    }
    //
    if( FONT_TEXTINPUT!=NULL)
    {
        GPE_CloseFont(FONT_TEXTINPUT);
        FONT_TEXTINPUT = NULL;
    }

    if( FONT_TERM_NAME!=NULL)
    {
        GPE_CloseFont(FONT_TERM_NAME);
        FONT_TERM_NAME = NULL;
    }

    if( FONT_TERM_DESCRIPTION!=NULL)
    {
        GPE_CloseFont(FONT_TERM_DESCRIPTION);
        FONT_TERM_DESCRIPTION = NULL;
    }

    if( FONT_TERM_SCOPE!=NULL)
    {
        GPE_CloseFont(FONT_TERM_SCOPE);
        FONT_TERM_SCOPE = NULL;
    }
    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        GPE_CloseFont(FONT_DEFAULT_PROMPT);
        FONT_DEFAULT_PROMPT = NULL;
    }

    if( FONT_CHECKBOX!=NULL)
    {
        GPE_CloseFont( FONT_CHECKBOX );
        FONT_CHECKBOX = NULL;
    }
    //
    if( FONT_HEADER!=NULL)
    {
        GPE_CloseFont( FONT_HEADER );
        FONT_HEADER = NULL;
    }

    if( FONT_LABEL!=NULL)
    {
        GPE_CloseFont( FONT_LABEL );
        FONT_LABEL = NULL;
    }
    if( FONT_LABEL_ANCHOR!=NULL)
    {
        GPE_CloseFont( FONT_LABEL_ANCHOR );
        FONT_LABEL_ANCHOR = NULL;
    }

    if( FONT_LABEL_PARAGRAPH!=NULL)
    {
        GPE_CloseFont( FONT_LABEL_PARAGRAPH );
        FONT_LABEL_PARAGRAPH = NULL;
    }

    if( FONT_LABEL_TITLE!=NULL)
    {
        GPE_CloseFont( FONT_LABEL_TITLE );
        FONT_LABEL_TITLE = NULL;
    }

    if( FONT_PARAGRAGH!=NULL)
    {
        GPE_CloseFont( FONT_PARAGRAGH );
        FONT_PARAGRAGH = NULL;
    }
}

void update_action_message(std::string newMessage)
{
    GPE_Action_Message = newMessage;
}

void update_popup_info(std::string newMessage, int selectedOpId, bool popIsContext)
{
    GPE_Action_Message = newMessage;
    GPE_Action_ID = selectedOpId;
    GPE_ACTION_IS_CONTEXT = popIsContext;
}

void update_rectangle(GPE_Rect * rectIn, double nx, double ny, double nw, double nh)
{
    if( rectIn!=NULL)
    {
        rectIn->x = nx;
        rectIn->y = ny;
        rectIn->w = nw;
        rectIn->h = nh;
    }
}

GPE_KeyPair::GPE_KeyPair(double idNumb, std::string str, std::string ksubstr)
{
    keyString = str;
    keyValue = idNumb;
    keySubString = ksubstr;
}

GPE_KeyPair::~GPE_KeyPair()
{

}

GPE_Template::GPE_Template()
{
    templateLocalLocation = "default.gpf";
    //sprites
     Main_Menu_Sprite = NULL;
    //For icons and Folders
    Main_Folder_Color = new GPE_Color(1,86,231);
    Main_Folder_Highlighted_Color = new GPE_Color(181,95,62);
    //for bg and standard colors
    Program_Color = new GPE_Color(32,32,32);
    Program_Header_Color = new GPE_Color(62,62,62);

    //For input fields and drop down-related menus
    Input_Highlight_Color = Main_Folder_Color->duplicate_color();
    Input_Highlight_Outline_Color = Main_Folder_Color->duplicate_color();
    Input_Highlight_Alt_Color = new GPE_Color(218,140,16);

    Input_Color = new GPE_Color(27,27,29);
    Input_Error_Box_Color = new GPE_Color(255,29,29);
    Input_Outline_Color = c_ltgray->duplicate_color();
    Input_Font_Color = c_ltgray->duplicate_color();
    Input_Faded_Font_Color = c_gray->duplicate_color();
    Input_Selected_Color = c_olive->duplicate_color();
    Input_Highlight_Font_Color = c_blgray->duplicate_color();
    Input_Error_Font_Color = new GPE_Color(228,8,8);
    Input_Error_Outline_Color = c_maroon->duplicate_color();

    //Used mainly for labels, urls and such
    Checkbox_Color = new GPE_Color(1,86,231);

    //Button colors and such
    Button_Box_Color = new GPE_Color(25,25,25);
    Button_Font_Color = new GPE_Color(249,249,249);
    Button_Font_Highlighted_Color = new GPE_Color(240,240,240);
    Button_Border_Color = new GPE_Color(16,16,16);
    Button_Box_Highlighted_Color = new GPE_Color(40,40,40);
    Button_Box_Selected_Color = new GPE_Color(96,96,96);
    Button_Border_Color = c_dkgray->duplicate_color();
    Button_Border_Highlighted_Color = c_blgray->duplicate_color();
    Button_Border_Selected_Color = c_ltgray->duplicate_color();

    //IconButton colors and such
    Icon_Box_Color = new GPE_Color(25,25,25);
    Icon_Font_Color = new GPE_Color(224,224,224);
    Icon_Font_Highlighted_Color = new GPE_Color(255,255,255);
    Icon_Font_Selected_Color = new GPE_Color(235,235,235);
    Icon_Border_Color = new GPE_Color(16,16,16);
    Icon_Box_Highlighted_Color = new GPE_Color(40,40,40);
    Icon_Box_Selected_Color = new GPE_Color(96,96,96);
    Icon_Border_Color = c_ltgray->duplicate_color();
    Icon_Border_Highlighted_Color = c_blgray->duplicate_color();
    Icon_Border_Selected_Color = new GPE_Color(235, 235, 235);

    Main_Box_Color = new GPE_Color(16,16,16);
    Main_Box_Highlighted_Color = new GPE_Color(24,24,24);
    Main_Box_Faded_Color = new GPE_Color(32,32,32);
    Main_Border_Color = new GPE_Color(27,27,29);
    Main_Border_Highlighted_Color = new GPE_Color( 64,32,250 );
    Main_Box_Font_Color = c_white->duplicate_color();
    Main_Box_Font_URL_Color = new GPE_Color(115,161,196);
    Main_Box_Font_URL_Visited_Color = new GPE_Color(179,204,223);
    Main_Box_Font_Highlight_Color = c_white->duplicate_color();
    Main_Box_Faded_Font_Color = c_ltgray->duplicate_color();



    //for pop up boxes like toolbar options, context menu, tool tip, etc. Essentially top Z-layer
    PopUp_Box_Highlight_Color = new GPE_Color(0,0,220);
    PopUp_Box_Highlight_Alt_Color = new GPE_Color(0,220,0);
    PopUp_Box_Close_Color = c_red->duplicate_color();
    PopUp_Box_Close_Font_Color = c_white->duplicate_color();
    PopUp_Box_Close_Hightlight_Color = c_maroon->duplicate_color();

    PopUp_Box_Color =  new GPE_Color(35,35,35);
    PopUp_Box_Border_Color = c_blgray->duplicate_color();
    PopUp_Box_Font_Color = c_silver->duplicate_color();
    PopUp_Box_Highlight_Font_Color = c_ltgray->duplicate_color();
    PopUp_Box_Faded_Font_Color = c_gray->duplicate_color();

    //For Scroll Boxes
    Scroll_Box_Color = new GPE_Color(55,55,55);
    Scroll_Box_Border_Color = new GPE_Color(0,0,0);
    Scroll_Box_Arrow_Color = new GPE_Color(245, 245, 245);
    Scroll_Box_Camera_Color = new GPE_Color(85, 85, 85);
    Scroll_Box_Camera_Highlight_Color = new GPE_Color(128, 128, 128);

    //for text boxes
    Text_Box_Outer_Color = c_blgray->duplicate_color();
    Text_Box_Outer_Font_Color = c_ltgray->duplicate_color();
    Text_Box_Color = c_alblack->duplicate_color();
    Text_Box_Outline_Color = c_jetblack->duplicate_color();
    Text_Box_Highlight_Color = new GPE_Color(50,35,50);
    Text_Box_Font_Color = c_ltgray->duplicate_color();
    Text_Box_Font_Highlight_Color = c_ltgray->duplicate_color();
    Text_Box_Font_Comment_Color = c_lime->duplicate_color();

    Text_Box_Font_DataType_Color = c_ltpurple->duplicate_color();
    Text_Box_Font_DQuote_Color = c_yellow->duplicate_color();
    Text_Box_Font_Function_Color = c_teal->duplicate_color();
    Text_Box_Font_Function_Alt_Color = c_lime->duplicate_color();
    Text_Box_Font_Keyword_Color = c_ltblue->duplicate_color();
    Text_Box_Font_Variable_Color = new GPE_Color(183,183,255);
    Text_Box_Font_Variable_Alt_Color = new GPE_Color(183,255,183);

    Text_Box_Font_Keyword_Alt_Color = c_green->duplicate_color();
    Text_Box_Font_JavaScript_Color = c_violet->duplicate_color();
    Text_Box_Font_JavaScript_Alt_Color = c_violet->duplicate_color();
    Text_Box_Font_Number_Color = c_fuchsia->duplicate_color();
    Text_Box_Font_SQuote_Color = c_aqua->duplicate_color();
    Text_Box_Font_Symbols_Color = new GPE_Color(255,99,71);

    //Project Variables
    Text_Box_Project_Function_Color = new GPE_Color(135,206,250);
    Text_Box_Project_Function_Alt_Color = new GPE_Color(206,135,250);



    Text_Box_Project_Keyword_Color = new GPE_Color(218,140,16);
    Text_Box_Project_Keyword_Alt_Color = new GPE_Color(50,35,150);
    //for errors
    Main_Error_Font_Color = c_maroon->duplicate_color();
    Main_Warning_Font_Color = c_blue->duplicate_color();
    Main_Suggestion_Font_Color = c_aqua->duplicate_color();

}

GPE_Template::~GPE_Template()
{
    if( Program_Color!=NULL)
    {
        delete Program_Color;
        Program_Color = NULL;
    }
    if( Program_Header_Color!=NULL)
    {
        delete Program_Header_Color;
        Program_Header_Color = NULL;
    }
    if( Main_Folder_Color!=NULL)
    {
        delete Main_Folder_Color;
        Main_Folder_Color = NULL;
    }
    if( Main_Folder_Highlighted_Color!=NULL)
    {
        delete Main_Folder_Highlighted_Color;
        Main_Folder_Highlighted_Color = NULL;
    }

    //For Input Fields, Drop Down Menus & Resource Drop Downs
    if( Input_Highlight_Color!=NULL)
    {
        delete Input_Highlight_Color;
        Input_Highlight_Color = NULL;
    }
    if( Input_Highlight_Outline_Color!=NULL)
    {
        delete Input_Highlight_Outline_Color;
        Input_Highlight_Outline_Color = NULL;
    }
    if( Input_Highlight_Alt_Color!=NULL)
    {
        delete Input_Highlight_Alt_Color;
        Input_Highlight_Alt_Color = NULL;
    }
    if( Input_Color!=NULL)
    {
        delete Input_Color;
        Input_Color = NULL;
    }
    if( Input_Error_Font_Color!=NULL)
    {
        delete Input_Error_Font_Color;
        Input_Error_Font_Color = NULL;
    }
    if( Input_Error_Box_Color!=NULL)
    {
        delete Input_Error_Box_Color;
        Input_Error_Box_Color = NULL;
    }
    if( Input_Error_Outline_Color!=NULL)
    {
        delete Input_Error_Outline_Color;
        Input_Error_Outline_Color = NULL;
    }
    if( Input_Outline_Color!=NULL)
    {
        delete Input_Outline_Color;
        Input_Outline_Color = NULL;
    }
    if( Input_Font_Color!=NULL)
    {
        delete Input_Font_Color;
        Input_Font_Color = NULL;
    }
    if( Input_Highlight_Font_Color!=NULL)
    {
        delete Input_Highlight_Font_Color;
        Input_Highlight_Font_Color = NULL;
    }
    if( Input_Faded_Font_Color!=NULL)
    {
        delete Input_Faded_Font_Color;
        Input_Faded_Font_Color = NULL;
    }
    if( Input_Selected_Color!=NULL)
    {
        delete Input_Selected_Color;
        Input_Selected_Color = NULL;
    }
        //For Gui boxes, like main menu, toolbar and oontext menus
        if( Checkbox_Color!=NULL)
        {
            delete Checkbox_Color;
            Checkbox_Color = NULL;
        }

        if( Button_Box_Highlighted_Color!=NULL)
        {
            delete Button_Box_Highlighted_Color;
            Button_Box_Highlighted_Color = NULL;
        }

        if( Button_Border_Highlighted_Color!=NULL)
        {
            delete Button_Border_Highlighted_Color;
            Button_Border_Highlighted_Color = NULL;
        }

        if( Button_Box_Selected_Color!=NULL)
        {
            delete Button_Box_Selected_Color;
            Button_Box_Selected_Color = NULL;
        }

        if( Main_Box_Faded_Color!=NULL)
        {
            delete Main_Box_Faded_Color;
            Main_Box_Faded_Color = NULL;
        }

        if( Main_Border_Color!=NULL)
        {
            delete Main_Border_Color;
            Main_Border_Color = NULL;
        }

        if( Main_Box_Font_Color!=NULL)
        {
            delete Main_Box_Font_Color;
            Main_Box_Font_Color = NULL;
        }

        if( Main_Box_Font_URL_Color!=NULL)
        {
            delete Main_Box_Font_URL_Color;
            Main_Box_Font_URL_Color = NULL;
        }

        if( Main_Box_Font_URL_Visited_Color!=NULL)
        {
            delete Main_Box_Font_URL_Visited_Color;
            Main_Box_Font_URL_Visited_Color = NULL;
        }

        if( Main_Box_Font_Highlight_Color!=NULL)
        {
            delete Main_Box_Font_Highlight_Color;
            Main_Box_Font_Highlight_Color = NULL;
        }

        if( Main_Box_Faded_Font_Color!=NULL)
        {
            delete Main_Box_Faded_Font_Color;
            Main_Box_Faded_Font_Color = NULL;
        }

        //For Gui boxes, like main menu, toolbar and oontext menus

        if( PopUp_Box_Highlight_Color!=NULL)
        {
            delete PopUp_Box_Highlight_Color;
            PopUp_Box_Highlight_Color = NULL;
        }

        if( PopUp_Box_Highlight_Alt_Color!=NULL)
        {
            delete PopUp_Box_Highlight_Alt_Color;
            PopUp_Box_Highlight_Alt_Color = NULL;
        }

        if( PopUp_Box_Close_Color!=NULL)
        {
            delete PopUp_Box_Close_Color;
            PopUp_Box_Close_Color = NULL;
        }

        if( PopUp_Box_Close_Hightlight_Color!=NULL)
        {
            delete PopUp_Box_Close_Hightlight_Color;
            PopUp_Box_Close_Hightlight_Color = NULL;
        }


        if( PopUp_Box_Close_Font_Color!=NULL)
        {
            delete PopUp_Box_Close_Font_Color;
            PopUp_Box_Close_Font_Color = NULL;
        }


        if( PopUp_Box_Color!=NULL)
        {
            delete PopUp_Box_Color;
            PopUp_Box_Color = NULL;
        }


        if( PopUp_Box_Border_Color!=NULL)
        {
            delete PopUp_Box_Border_Color;
            PopUp_Box_Border_Color = NULL;
        }


        if( PopUp_Box_Font_Color!=NULL)
        {
            delete PopUp_Box_Font_Color;
            PopUp_Box_Font_Color = NULL;
        }


        if( PopUp_Box_Highlight_Font_Color!=NULL)
        {
            delete PopUp_Box_Highlight_Font_Color;
            PopUp_Box_Highlight_Font_Color = NULL;
        }


        if( PopUp_Box_Faded_Font_Color!=NULL)
        {
            delete PopUp_Box_Faded_Font_Color;
            PopUp_Box_Faded_Font_Color = NULL;
        }
        //for text boxes
        if( Text_Box_Outer_Color!=NULL)
        {
            delete Text_Box_Outer_Color;
            Text_Box_Outer_Color = NULL;
        }
        if( Text_Box_Outer_Font_Color!=NULL)
        {
            delete Text_Box_Outer_Font_Color;
            Text_Box_Outer_Font_Color = NULL;
        }
        if( Text_Box_Color!=NULL)
        {
            delete Text_Box_Color;
            Text_Box_Color = NULL;
        }
        if( Text_Box_Outline_Color!=NULL)
        {
            delete Text_Box_Outline_Color;
            Text_Box_Outline_Color = NULL;
        }
        if( Text_Box_Highlight_Color!=NULL)
        {
            delete Text_Box_Highlight_Color;
            Text_Box_Highlight_Color = NULL;
        }
        if( Text_Box_Font_Color!=NULL)
        {
            delete Text_Box_Font_Color;
            Text_Box_Font_Color = NULL;
        }
        if( Text_Box_Font_Comment_Color!=NULL)
        {
            delete Text_Box_Font_Comment_Color;
            Text_Box_Font_Comment_Color = NULL;
        }
        if( Text_Box_Font_Highlight_Color!=NULL)
        {
            delete Text_Box_Font_Highlight_Color;
            Text_Box_Font_Highlight_Color = NULL;
        }

        delete Text_Box_Font_DataType_Color;
        delete Text_Box_Font_DQuote_Color;
        delete Text_Box_Font_Function_Color;
        delete Text_Box_Font_Function_Alt_Color;
        delete Text_Box_Font_Keyword_Color;
        delete Text_Box_Font_Keyword_Alt_Color;
        delete Text_Box_Font_JavaScript_Color;
        delete Text_Box_Font_JavaScript_Alt_Color;
        delete Text_Box_Font_Number_Color;
        delete Text_Box_Font_SQuote_Color;
        delete Text_Box_Font_Symbols_Color;

        if( Text_Box_Project_Function_Color!=NULL )
        {
            delete Text_Box_Project_Function_Color;
            Text_Box_Project_Function_Color = NULL;
        }

        if( Text_Box_Project_Function_Alt_Color!=NULL )
        {
            delete Text_Box_Project_Function_Alt_Color;
            Text_Box_Project_Function_Alt_Color = NULL;
        }

        if( Text_Box_Project_Keyword_Color!=NULL )
        {
            delete Text_Box_Project_Keyword_Color;
            Text_Box_Project_Keyword_Color = NULL;
        }

        if( Text_Box_Project_Keyword_Alt_Color!=NULL )
        {
            delete Text_Box_Project_Keyword_Alt_Color;
            Text_Box_Project_Keyword_Alt_Color = NULL;
        }
        //for errors
        if( Main_Error_Font_Color!=NULL)
        {
            delete Main_Error_Font_Color;
            Main_Error_Font_Color = NULL;
        }
        if( Main_Warning_Font_Color!=NULL)
        {
            delete Main_Warning_Font_Color;
            Main_Warning_Font_Color = NULL;
        }
        if( Main_Suggestion_Font_Color!=NULL)
        {
            delete Main_Suggestion_Font_Color;
            Main_Suggestion_Font_Color = NULL;
        }
}

void GPE_Template::load_theme(std::string themeLocationIn)
{
    //If the level file could be loaded
    std::string themeGlobalLocation = APP_DIRECTORY_NAME+"themes/"+themeLocationIn;
    if( file_exists(themeGlobalLocation) )
    {
        std::ifstream templateFileIn( themeGlobalLocation.c_str() );
        if( !templateFileIn.fail() )
        {
            //makes sure the file is open
            if (templateFileIn.is_open())
            {
                templateLocalLocation = themeLocationIn;
                int equalPos = 0;
                int hashPos = 0;
                std::string firstChar="";
                std::string section="";
                std::string cur_layer="";
                std::string data_format="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                //float foundFileVersion = 0;

                int rFound = 0, gFound = 0, bFound = 0;
                while ( templateFileIn.good() )
                {
                    getline (templateFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = trim_left_inplace(currLine);
                    currLineToBeProcessed = trim_right_inplace(currLine);
                    //Empty Line skipping is only allowed at the top of the file
                    if(!currLineToBeProcessed.empty() )
                    {
                        //Comment skipping is only allowed at the top of the file
                        if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos = currLineToBeProcessed.find_first_of("=");
                            rFound = 0;
                            gFound = 0;
                            bFound = 0;

                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                                hashPos = valString.find_first_of("#");
                                if(hashPos!=(int)std::string::npos)
                                {
                                    HEXtoRGB(valString,rFound,gFound, bFound);
                                }
                                else
                                {
                                    rFound = split_first_int(valString,',');
                                    gFound = split_first_int(valString,',');
                                    bFound = string_to_int(valString);
                                }
                                if( keyString=="MainBackground")
                                {
                                    Program_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="HeaderBackground")
                                {
                                    Program_Header_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="FolderColor")
                                {
                                    Main_Folder_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="FolderHighlightedColor")
                                {
                                    Main_Folder_Highlighted_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="Input")
                                {
                                    Input_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="InputHighlighted")
                                {
                                    Input_Highlight_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="InputHighlightedOutline")
                                {
                                    Input_Highlight_Outline_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="InputHighlightedAlt")
                                {
                                    Input_Highlight_Alt_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="InputBackground")
                                {
                                    Input_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="InputOutline")
                                {
                                    Input_Outline_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="InputFont")
                                {
                                    Input_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="InputFontHighlighted")
                                {
                                    Input_Highlight_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="InputFontFaded")
                                {
                                    Input_Faded_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBorder")
                                {
                                    Main_Border_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBorderHighlighted")
                                {
                                    Main_Border_Highlighted_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBoxHighlightedAlt")
                                {
                                    Button_Box_Selected_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBox")
                                {
                                    Main_Box_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBoxFaded")
                                {
                                    Main_Box_Faded_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBoxOutline" || keyString=="MainBorder" )
                                {
                                    Main_Border_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBoxFont")
                                {
                                    Main_Box_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBoxFontHighlighted")
                                {
                                    Main_Box_Font_Highlight_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBoxFontFaded")
                                {
                                    Main_Box_Faded_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBoxFontURL")
                                {
                                    Main_Box_Font_URL_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="MainBoxFontURLVisited")
                                {
                                    Main_Box_Font_URL_Visited_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="ButtonBox")
                                {
                                    //Butons and such...
                                    Button_Box_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="ButtonBoxHighlighted")
                                {
                                    Button_Box_Highlighted_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="ButtonBoxSelected")
                                {
                                    Button_Box_Selected_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="ButtonBorder")
                                {
                                    Button_Border_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="ButtonBorderHighlighted")
                                {
                                    Button_Border_Highlighted_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="ButtonBorderSelected")
                                {
                                    Button_Border_Selected_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="ButtonFont")
                                {
                                    Button_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="ButtonFontHighlighted")
                                {
                                    Button_Font_Highlighted_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="IconBox")
                                {
                                    //Butons and such...
                                    Icon_Box_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="IconBoxHighlighted")
                                {
                                    Icon_Box_Highlighted_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="IconBoxSelected")
                                {
                                    Icon_Box_Selected_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="IconBorder")
                                {
                                    Icon_Border_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="IconBorderHighlighted")
                                {
                                    Icon_Border_Highlighted_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="IconBorderSelected")
                                {
                                    Icon_Border_Selected_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="IconFont")
                                {
                                    Icon_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="IconFontHighlighted")
                                {
                                    Icon_Font_Highlighted_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="IconFontSelected")
                                {
                                    Icon_Font_Selected_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="PopUpBox")
                                {
                                    PopUp_Box_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="PopUpBoxHighlighted")
                                {
                                    PopUp_Box_Highlight_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="PopUpBoxHighlightedAlt")
                                {
                                    PopUp_Box_Highlight_Alt_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="PopUpBoxOutline")
                                {
                                    PopUp_Box_Border_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="PopUpBoxFont")
                                {
                                    PopUp_Box_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="PopUpBoxFontHighlighted")
                                {
                                    PopUp_Box_Highlight_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="PopUpBoxFontFaded")
                                {
                                    PopUp_Box_Faded_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextBoxOuter")
                                {
                                    Text_Box_Outer_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextBoxOuterFont")
                                {
                                    Text_Box_Outer_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextBox")
                                {
                                    Text_Box_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextBoxOutline")
                                {
                                    Text_Box_Outline_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextBoxHighlighted")
                                {
                                    Text_Box_Highlight_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextBoxFont")
                                {
                                    Text_Box_Font_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextBoxFontComment")
                                {
                                    Text_Box_Font_Comment_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontHighlighted")
                                {
                                    Text_Box_Font_Highlight_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontFunction")
                                {
                                    Text_Box_Font_Function_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontKeyword")
                                {
                                    Text_Box_Font_Keyword_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontNumber")
                                {
                                    Text_Box_Font_Number_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontDQuote")
                                {
                                    Text_Box_Font_DQuote_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontSQuote")
                                {
                                    Text_Box_Font_SQuote_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontSymbol")
                                {
                                    Text_Box_Font_Symbols_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontJavaScript")
                                {
                                    Text_Box_Font_JavaScript_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontProjectFunction")
                                {
                                    Text_Box_Project_Function_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontProjectKeyword")
                                {
                                    Text_Box_Project_Keyword_Color->change_rgba(rFound,gFound,bFound);
                                }
                                else if( keyString=="TextFontVariable" )
                                {
                                    Text_Box_Font_Variable_Color->change_rgba(rFound,gFound, bFound );
                                }
                                else if( keyString=="TextFontVariableAlt" )
                                {
                                    Text_Box_Font_Variable_Alt_Color->change_rgba(rFound,gFound, bFound );
                                }
                                else if( keyString=="ScrollBox" )
                                {
                                    Scroll_Box_Color->change_rgba(rFound,gFound, bFound );
                                }
                                else if( keyString=="ScrollBoxBorder" )
                                {
                                    Scroll_Box_Border_Color->change_rgba(rFound,gFound, bFound );
                                }
                                else if( keyString=="ScrollBoxArrow" )
                                {
                                    Scroll_Box_Arrow_Color->change_rgba(rFound,gFound, bFound );
                                }
                                else if( keyString=="ScrollBoxCamera" )
                                {
                                    Scroll_Box_Camera_Color->change_rgba(rFound,gFound, bFound );
                                }
                                else if( keyString=="ScrollBoxCameraHighlight" )
                                {
                                    Scroll_Box_Camera_Highlight_Color->change_rgba(rFound,gFound, bFound );
                                }


                            }
                        }
                    }
                }
                templateFileIn.close();
            }
        }
    }
}

GPE_Overlay_System::GPE_Overlay_System()
{
    tempUserMessageTitle = "";
    tempUserMessageSubtitle = "";
    tempUserMessageText = "";
    temporayMessageDisplayTimer = 0;
    temporayMessageDisplayStartTimer = 0;
    tempTopLeftMessage = false;
    previousScreenshot = NULL;
    toolTipString = "";
    toolTipTexture = NULL;
    toolTipX = 0;
    toolTipY = 0;
}

GPE_Overlay_System::~GPE_Overlay_System()
{
    if( previousScreenshot!=NULL)
    {
        delete previousScreenshot;
        previousScreenshot = NULL;
    }
    if( previousScreenshot!=NULL)
    {
        delete previousScreenshot;
        previousScreenshot = NULL;
    }
    if( toolTipTexture!=NULL)
    {
        delete toolTipTexture;
        toolTipTexture = NULL;
    }
}

bool GPE_Overlay_System::overhead_used()
{
    if( tipTipJustUpdated)
    {
        return true;
    }
    return false;
}

void GPE_Overlay_System::process_cursor()
{
    if(GPE_CurrentCursor!=GPE_PreviousCursor)
    {
        if( GPE_Cursor!=NULL)
        {
            SDL_FreeCursor(GPE_Cursor);
            GPE_Cursor = NULL;
        }
        if(GPE_CurrentCursor<0)
        {
            GPE_Cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
        }
        else
        {
            GPE_Cursor = SDL_CreateSystemCursor(GPE_CurrentCursor);
        }
        SDL_SetCursor(GPE_Cursor);
    }
    GPE_PreviousCursor = GPE_CurrentCursor;
}

void GPE_Overlay_System::take_frozen_screenshot(GPE_Renderer * renderTarget)
{
    if( renderTarget==NULL)
    {
        renderTarget = MAIN_RENDERER;
    }
    if( previousScreenshot==NULL)
    {
        previousScreenshot = new GPE_Texture();
    }
    if( previousScreenshot!=NULL && renderTarget!=NULL)
    {
        renderTarget->save_screenshot(APP_DIRECTORY_NAME+"frozen_screenshot.png");
        previousScreenshot->load_new_texture(renderTarget,APP_DIRECTORY_NAME+"frozen_screenshot.png");
    }
}

void GPE_Overlay_System::update_tooltip(std::string newTip)
{
    if( newTip!=toolTipString)
    {
        toolTipString = newTip;
        if( toolTipTexture==NULL)
        {
            toolTipTexture = new GPE_Texture();
        }
        if( toolTipTexture!=NULL)
        {
            //toolTipTexture->loadFromRenderedText(MAIN_RENDERER,newTip,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT);
        }
        tipTipJustUpdated = true;
    }
}

void GPE_Overlay_System::update_temporary_message(std::string mTitle, std::string mSubTitle, std::string mText, float messDuration, bool topLeftMessage)
{
    if( messDuration >0 &&  ( (int)mTitle.size()>0 || (int)mSubTitle.size()>0 || (int)mText.size()>0 ) )
    {
        tempUserMessageTitle = mTitle;
        tempUserMessageSubtitle = mSubTitle;
        tempUserMessageText = mText;
        tempTopLeftMessage = topLeftMessage;
        temporayMessageDisplayStartTimer = temporayMessageDisplayTimer = messDuration*FPS_CAP;
    }
    else
    {
        tempUserMessageTitle = "";
        tempUserMessageSubtitle = "";
        tempUserMessageText = "";
        temporayMessageDisplayStartTimer = temporayMessageDisplayTimer = 0;
    }
}

void GPE_Overlay_System::render_frozen_screenshot(GPE_Renderer * renderTarget)
{
    if( renderTarget==NULL)
    {
        renderTarget = MAIN_RENDERER;
    }
    if( renderTarget!=NULL &&  previousScreenshot!=NULL)
    {
        previousScreenshot->render_tex_resized(renderTarget,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    }
}

void GPE_Overlay_System::render_temporary_message(GPE_Renderer * renderTarget)
{
    if( renderTarget==NULL)
    {
        renderTarget = MAIN_RENDERER;
    }
    if( renderTarget!=NULL && temporayMessageDisplayTimer > 0 && temporayMessageDisplayStartTimer > 0 )
    {

        int noticeAlphaValue = (temporayMessageDisplayTimer/temporayMessageDisplayStartTimer)*255;
        if( tempTopLeftMessage)
        {
            render_rectangle(renderTarget,0,0,256,96,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false, noticeAlphaValue);
            render_rectangle(renderTarget,0,0,256,96,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true,noticeAlphaValue);
            render_new_text(renderTarget,0,GENERAL_GPE_PADDING,tempUserMessageTitle,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,noticeAlphaValue);
            render_new_text(renderTarget,0,GENERAL_GPE_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,tempUserMessageSubtitle,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,noticeAlphaValue);
            render_new_text(renderTarget,0,GENERAL_GPE_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,tempUserMessageText,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,noticeAlphaValue);

        }
        else
        {
            render_rectangle(renderTarget,SCREEN_WIDTH/2-256,SCREEN_HEIGHT-128,SCREEN_WIDTH/2+256,SCREEN_HEIGHT-32,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false, noticeAlphaValue);
            render_rectangle(renderTarget,SCREEN_WIDTH/2-256,SCREEN_HEIGHT-128,SCREEN_WIDTH/2+256,SCREEN_HEIGHT-32,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true,noticeAlphaValue);
            render_new_text(renderTarget,SCREEN_WIDTH/2,SCREEN_HEIGHT-128+GENERAL_GPE_PADDING,tempUserMessageTitle,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP,noticeAlphaValue);
            render_new_text(renderTarget,SCREEN_WIDTH/2,SCREEN_HEIGHT-128+GENERAL_GPE_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,tempUserMessageSubtitle,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP,noticeAlphaValue);
            render_new_text(renderTarget,SCREEN_WIDTH/2,SCREEN_HEIGHT-128+GENERAL_GPE_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,tempUserMessageText,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP,noticeAlphaValue);
        }
        temporayMessageDisplayTimer--;
    }
}

void GPE_Overlay_System::render_tooltip(GPE_Renderer * cRender,int xPos, int yPos)
{
    if( (int)toolTipString.size()>0)
    {
        if( xPos < 0)
        {
            xPos = userInput->mouse_x;
        }
        if( yPos < 0 || yPos > SCREEN_HEIGHT-GENERAL_GPE_PADDING-toolTipTexture->get_height() )
        {
            yPos = userInput->mouse_y+32;
        }

        int TEXTBOX_FONT_SIZE_WIDTH = 12;
        int TEXTBOX_FONT_SIZE_HEIGHT = 12;
        if( FONT_TOOLTIP!=NULL)
        {
             FONT_TOOLTIP->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
        }

        int xPos2 = 0;
        int yPos2 = yPos+TEXTBOX_FONT_SIZE_HEIGHT+GENERAL_GPE_PADDING/2;

        int maxLength = (SCREEN_WIDTH - GENERAL_GPE_PADDING*4)/TEXTBOX_FONT_SIZE_WIDTH;
        int lineCountForTip = 1;
        std::vector< std::string > toolTipLines;
        std::string tempToolTipString = toolTipString;
        if( (int)tempToolTipString.size() >= maxLength)
        {
            xPos = 0;
            xPos2 = SCREEN_WIDTH;
            while ( (int)tempToolTipString.size() > 0 )
            {
                toolTipLines.push_back( get_substring(tempToolTipString,0,maxLength) );
                tempToolTipString = get_substring(tempToolTipString,maxLength);
            }
            yPos2= yPos+TEXTBOX_FONT_SIZE_HEIGHT*( (int)toolTipLines.size() )+GENERAL_GPE_PADDING/2;
        }
        else
        {
            toolTipLines.push_back(toolTipString);
            if(xPos+TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipString.size()+GENERAL_GPE_PADDING*2 > SCREEN_WIDTH)
            {
                xPos = SCREEN_WIDTH-( TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipString.size() )-GENERAL_GPE_PADDING*2;
                if( xPos < 0)
                {
                    xPos = 0;
                }
            }
            xPos2 = xPos+ TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipString.size()+GENERAL_GPE_PADDING*2;
        }

        render_rectangle(cRender,xPos-GENERAL_GPE_PADDING/2,yPos-GENERAL_GPE_PADDING/2,xPos2,yPos2,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false);
        for( int i = 0; i < (int)toolTipLines.size(); i++)
        {
            render_new_text(cRender,xPos+GENERAL_GPE_PADDING,yPos+TEXTBOX_FONT_SIZE_HEIGHT*i, toolTipLines[i],GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_TOOLTIP,FA_LEFT,FA_TOP);
        }

        render_rectangle(cRender,xPos-GENERAL_GPE_PADDING/2,yPos-GENERAL_GPE_PADDING/2,xPos2,yPos2,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
    }
    tipTipJustUpdated = false;
}

GPE_ScrollBar_XAxis::GPE_ScrollBar_XAxis()
{
    barBox.x = contextRect.x =  fullRect.x = 0;
    barBox.y = contextRect.y =  fullRect.y = 0;
    barBox.w = contextRect.w =  fullRect.w = 32;
    barBox.h = contextRect.h =  fullRect.h = 32;


    scrollWidth = 32;
    scrollPercent = 100;
    scrollPixels = 16;
    scrollXPos = 0;
    scrollDisplayPixels = 0;
    hasMoved = false;
    mouseXScrolPos =0;
    isOnScrollBox = false;
    isHeldOn = false;
}

GPE_ScrollBar_XAxis::~GPE_ScrollBar_XAxis()
{

}

int GPE_ScrollBar_XAxis::get_box_width()
{
    return barBox.w;
}

int GPE_ScrollBar_XAxis::get_box_height()
{
    return barBox.h;
}

bool GPE_ScrollBar_XAxis::has_moved()
{
    return hasMoved;
}

bool GPE_ScrollBar_XAxis::is_held()
{
    return isHeldOn;
}

bool GPE_ScrollBar_XAxis::is_scrolling()
{
    return isOnScrollBox;
}

void GPE_ScrollBar_XAxis::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool autoCorrect)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool doWork = true;
    //calculates the height needed to be scrolled.
    if( fullRect.w!=0)
    {
        if( contextRect.w < fullRect.w)
        {
            scrollPercent =  (float)contextRect.w/(float)fullRect.w;
            scrollDisplayPixels = (float)(barBox.w)*scrollPercent;
            scrollXPos = (float)(contextRect.x)/(float)(fullRect.w)*(float)barBox.w;
        }
        else
        {
            scrollPercent = 1;
            scrollDisplayPixels = barBox.w;
            scrollXPos = 0;
            doWork = false;
            contextRect.x = 0;
        }
    }

    float prevPosition = scrollXPos;
    hasMoved = false;
    isHeldOn = false;
    if( doWork)
    {
        if( point_within(userInput->mouse_x,userInput->mouse_y,viewedSpace->x+barBox.x-cam->x,viewedSpace->y+barBox.y-cam->y,viewedSpace->x+barBox.x+barBox.w-cam->x,viewedSpace->y+barBox.y+barBox.h-cam->y) )
        {
            if( userInput->check_mouse_pressed(0 ) )
            {
                isOnScrollBox = true;
                isHeldOn = false;
            }
            else if( userInput->check_mouse_down(0 ) )
            {
                if( !isOnScrollBox)
                {
                    isHeldOn = true;
                }
            }
        }
        else if( userInput->check_mouse_released(-1) )
        {
            isOnScrollBox = false;
            isHeldOn = false;
        }

        if(viewedSpace->x+barBox.x-cam->x <= userInput->mouse_x && userInput->mouse_x <= viewedSpace->x+barBox.x+barBox.w-cam->x )
        {
            if( userInput->check_mouse_down(0 ) && isOnScrollBox)
            {
                mouseXScrolPos = userInput->mouse_x - viewedSpace->x-barBox.x-cam->x; //translates mouse coords to viewed space coords
                if( isOnScrollBox)
                {
                    scrollXPos = mouseXScrolPos;
                }
            }
        }
        else
        {
            isOnScrollBox = false;
            isHeldOn = false;
        }

        if( userInput->check_mouse_released(0) || userInput->check_mouse_released(1))
        {
            if( isOnScrollBox)
            {
                userInput->reset_all_input();
            }
            isOnScrollBox = false;
            isHeldOn = false;
        }

        if( autoCorrect)
        {
            if( scrollXPos > barBox.w- scrollDisplayPixels)
            {
                scrollXPos = barBox.w- scrollDisplayPixels;
            }

            if( scrollXPos < 0)
            {
                scrollXPos = 0;
            }
        }

        prevPosition = contextRect.x;
        contextRect.x =  ( scrollXPos/barBox.w )*fullRect.w;

        if( contextRect.x!=prevPosition)
        {
            hasMoved = true;
        }
    }
    else
    {
        scrollPercent = 1;
        scrollDisplayPixels = barBox.w;
        scrollXPos = 0;
        contextRect.x = 0;
        //hasMoved = true;
    }

}

void GPE_ScrollBar_XAxis::render_self(GPE_Renderer *  cRender, GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cRender==NULL)
    {
        cRender = MAIN_RENDERER;
    }
    if( cRender!=NULL && cam!=NULL && viewedSpace!=NULL)
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Scroll_Box_Color,false);
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Scroll_Box_Border_Color,true);
        int scrollRenderPixels = scrollDisplayPixels;
        if( scrollRenderPixels < barBox.h)
        {
            scrollRenderPixels = barBox.h;
        }
        if( isOnScrollBox )
        {
            render_rectangle(cRender,barBox.x+scrollXPos-cam->x,barBox.y-cam->y,barBox.x+scrollXPos+scrollRenderPixels-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Scroll_Box_Camera_Highlight_Color,false);
        }
        else
        {
            render_rectangle(cRender,barBox.x+scrollXPos-cam->x,barBox.y-cam->y,barBox.x+scrollXPos+scrollRenderPixels-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Scroll_Box_Camera_Color,false);
        }
        render_sprite_ext(cRender,GPE_TRIANGLE,2,barBox.x+barBox.h/4-cam->x,barBox.y+barBox.h/4-cam->y,barBox.h/2,barBox.h/2, GPE_MAIN_TEMPLATE->Scroll_Box_Arrow_Color );
        render_sprite_ext(cRender,GPE_TRIANGLE,0,barBox.x+barBox.w-barBox.h*3/4-cam->x,  barBox.y+barBox.h/4-cam->y,barBox.h/2,barBox.h/2,GPE_MAIN_TEMPLATE->Scroll_Box_Arrow_Color );
    }
}

void GPE_ScrollBar_XAxis::reset_scroller()
{
    hasMoved = false;
    isOnScrollBox = false;
    barBox.x = 0;
    barBox.y = 0;
    barBox.w = 0;
    barBox.h = 0;
}

GPE_ScrollBar_YAxis::GPE_ScrollBar_YAxis()
{
    documentActions = false;
    barBox.x = 0;
    barBox.y = 0;
    barBox.w = 16;
    barBox.h = 16;

    contextRect.x = fullRect.x = 0;
    contextRect.y = fullRect.y = 0;
    contextRect.w = fullRect.w = 32;
    contextRect.h = fullRect.h = 32;

    scrollHeight = 32;
    scrollPercent = 100;
    scrollDisplayPixels = 16;
    scrollYPos = 0;

    scrollYPos = 0;
    mouseYScrolPos = scrollYPos;
    isOnScrollBox = false;
    hasMoved = false;
    isHeldOn = false;
}

GPE_ScrollBar_YAxis::~GPE_ScrollBar_YAxis()
{

}

int GPE_ScrollBar_YAxis::get_box_width()
{
    return barBox.w;
}

int GPE_ScrollBar_YAxis::get_box_height()
{
    return barBox.h;
}

bool GPE_ScrollBar_YAxis::has_moved()
{
    return hasMoved;
}

bool GPE_ScrollBar_YAxis::is_held()
{
    return isHeldOn;
}

bool GPE_ScrollBar_YAxis::is_scrolling()
{
    return isOnScrollBox;
}

void GPE_ScrollBar_YAxis::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool autoCorrect)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool doWork = true;
    if( viewedSpace!=NULL && cam!=NULL )
    {
        //calculates the height needed to be scrolled.
        if( fullRect.h!=0)
        {
            if( contextRect.h < fullRect.h)
            {
                scrollPercent =  contextRect.h/fullRect.h;
                scrollDisplayPixels = (barBox.h)*scrollPercent;
                scrollYPos = ( contextRect.y/fullRect.h )*barBox.h;
            }
            else
            {
                scrollPercent = 1;
                scrollDisplayPixels = barBox.h;
                scrollYPos = 0;
                doWork = false;
            }
        }
        else
        {
            doWork = false;
        }
        hasMoved = false;
        isHeldOn = false;
        if( doWork)
        {
            if( point_between(userInput->mouse_x,userInput->mouse_y,barBox.x+viewedSpace->x-cam->x,barBox.y+viewedSpace->y-cam->y,barBox.x+barBox.w+viewedSpace->x-cam->x,barBox.y+barBox.h+viewedSpace->y-cam->y) )
            {
                if( userInput->check_mouse_pressed(0 ) )
                {
                    isOnScrollBox = true;
                }
            }

            if(barBox.y+viewedSpace->y-cam->y <= userInput->mouse_y && userInput->mouse_y <= barBox.y+barBox.h+viewedSpace->y-cam->y )
            {
                if( userInput->check_mouse_down(0 ) && isOnScrollBox)
                {
                    mouseYScrolPos = userInput->mouse_y - viewedSpace->y-barBox.y+cam->y; //translates mouse coords to viewed space coords
                    if( isOnScrollBox)
                    {
                        scrollYPos = mouseYScrolPos;
                    }
                }
                else
                {
                    isOnScrollBox = false;
                    isHeldOn = false;
                }
            }
            else
            {
                isOnScrollBox = false;
                isHeldOn = false;
            }

            if( userInput->check_mouse_released(0) || userInput->check_mouse_released(1))
            {
                if( isOnScrollBox)
                {
                    userInput->reset_all_input();
                }
                isOnScrollBox = false;
                isHeldOn = false;
            }
            float prevPosition = contextRect.y;
            //if( scrollPercent)
            {
                if( scrollYPos+scrollDisplayPixels > barBox.h )
                {
                    scrollYPos = barBox.h- scrollDisplayPixels;
                }

                if( scrollYPos < 0)
                {
                    scrollYPos = 0;
                }
            }
            contextRect.y =  ( scrollYPos/barBox.h )*fullRect.h;

            //if( autoCorrect)
            {
                if( contextRect.y+contextRect.h > fullRect.h )
                {
                    contextRect.y = fullRect.h- contextRect.h;
                }

                if( contextRect.y < 0)
                {
                    contextRect.y = 0;
                }
            }
            if( contextRect.y!=prevPosition)
            {
                hasMoved = true;
            }
        }
        else
        {
            scrollPercent = 1;
            scrollDisplayPixels = barBox.h;
            scrollYPos = 0;
            contextRect.y = 0;
        }
    }
}

void GPE_ScrollBar_YAxis::render_self(GPE_Renderer *  cRender, GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Scroll_Box_Color,false);
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Scroll_Box_Border_Color,true);
        int scrollRenderPixels = scrollDisplayPixels;
        if( scrollRenderPixels < barBox.w)
        {
            scrollRenderPixels = barBox.w;
        }
        if( isOnScrollBox )
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y+scrollYPos-cam->y,barBox.x+barBox.w-cam->x,barBox.y+scrollYPos+scrollRenderPixels-cam->y,GPE_MAIN_TEMPLATE->Scroll_Box_Camera_Highlight_Color,false);
        }
        else
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y+scrollYPos-cam->y,barBox.x+barBox.w-cam->x,barBox.y+scrollYPos+scrollRenderPixels-cam->y,GPE_MAIN_TEMPLATE->Scroll_Box_Camera_Color,false);
        }
        render_sprite_ext(cRender,GPE_TRIANGLE,1,barBox.x+barBox.w/4-cam->x,barBox.y+barBox.w/4,barBox.w/2-cam->y,barBox.w/2,GPE_MAIN_TEMPLATE->Scroll_Box_Arrow_Color);
        render_sprite_ext(cRender,GPE_TRIANGLE,3,barBox.x+barBox.w/4-cam->x,barBox.y+barBox.h-barBox.w*3/4-cam->y,barBox.w/2,barBox.w/2,GPE_MAIN_TEMPLATE->Scroll_Box_Arrow_Color);
    }
}

void GPE_ScrollBar_YAxis::reset_scroller()
{
    isOnScrollBox = false;
    hasMoved = false;
    barBox.x = 0;
    barBox.y = 0;
    barBox.w = 0;
    barBox.h = 0;

}

GPE_GeneralGuiElement::GPE_GeneralGuiElement()
{
    barBox.x =0;
    barBox.y = 0;
    barBox.w = 0;
    barBox.h = 0;
    maxWidth = 0;
    maxHeight = 0;
    autoResizes = false;
    clickedOutside = false;
    clickedOutside = false;
    hasScrollControl = false;
    hasArrowkeyControl = false;
    isClicked = false;
    isRightClicked = false;
    isEnabled = true;
    isHovered = false;
    guiListTypeName = "";
    guiListTypeId = 0;
    opName = "";
    descriptionText = "";
    showBorderBox = true;
    hasLineBreak = false;
    isModified = false;
    isInUse = false;
}

GPE_GeneralGuiElement::~GPE_GeneralGuiElement()
{

}

bool GPE_GeneralGuiElement::is_modified()
{
    return isModified;
}

bool GPE_GeneralGuiElement::is_inuse()
{
    return isInUse;
}

void GPE_GeneralGuiElement::switch_inuse(bool newUse)
{
    isInUse = newUse;
}

void GPE_GeneralGuiElement::update_paragraph( int foundMaxWidth )
{

}

void GPE_GeneralGuiElement::disable_self()
{
    isEnabled = false;
}

void GPE_GeneralGuiElement::enable_self()
{
    isEnabled = true;
}

int GPE_GeneralGuiElement::get_height()
{
    return barBox.h;
}

int GPE_GeneralGuiElement::get_max_height()
{
    return maxHeight;
}

int GPE_GeneralGuiElement::get_max_width()
{
    return maxWidth;
}

std::string GPE_GeneralGuiElement::get_name()
{
    return opName;
}

std::string GPE_GeneralGuiElement::get_element_type()
{
    return guiListTypeName;
}

std::string GPE_GeneralGuiElement::get_data()
{
    return "";
}

std::string GPE_GeneralGuiElement::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_GeneralGuiElement::load_data(std::string dataString)
{
    dataString = dataString;
}

void GPE_GeneralGuiElement::remove_data(std::string dataString)
{
    dataString = dataString;
}

int GPE_GeneralGuiElement::get_width()
{
    return barBox.w;
}

int GPE_GeneralGuiElement::get_xpos()
{
    return barBox.x;

}

int GPE_GeneralGuiElement::get_x2pos()
{
    return barBox.x+barBox.w;
}

int GPE_GeneralGuiElement::get_ypos()
{
    return barBox.y;
}

int GPE_GeneralGuiElement::get_y2pos()
{
    return barBox.y+barBox.h;
}

bool GPE_GeneralGuiElement::is_clicked()
{
    return isClicked;
}

bool GPE_GeneralGuiElement::is_rightclicked()
{
    return isRightClicked;
}


bool GPE_GeneralGuiElement::is_enabled()
{
    return isEnabled;
}

bool GPE_GeneralGuiElement::is_hovered()
{
    return isHovered;
}

void GPE_GeneralGuiElement::move_pos(int newX, int newY)
{
    if( newX!=0)
    {
        barBox.x += newX;
    }
    if(newY!=0)
    {
        barBox.y += newY;
    }
}

void GPE_GeneralGuiElement::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    if( isEnabled)
    {
        bool wasHovered = isHovered;
        isClicked = false;
        isRightClicked = false;
        isHovered = false;
        clickedOutside = false;
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if(viewedSpace!=NULL && cam!=NULL)
        {
            if( point_between(userInput->mouse_x,userInput->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
            {
                if (point_between(userInput->mouse_x,userInput->mouse_y,barBox.x+viewedSpace->x-cam->x,barBox.y+viewedSpace->y-cam->y,barBox.x+barBox.w+viewedSpace->x-cam->x,barBox.y+barBox.h+viewedSpace->y-cam->y) )
                {
                    isHovered = true;
                    if( MAIN_OVERLAY!=NULL)
                    {
                        if( (int)descriptionText.size()>0 )
                        {
                            MAIN_OVERLAY->update_tooltip(descriptionText);
                        }
                        else
                        {
                            MAIN_OVERLAY->update_tooltip(opName);
                        }
                    }
                    if( userInput->check_mouse_released(0) )
                    {
                        isClicked = true;
                        isInUse = true;
                    }

                    if( userInput->check_mouse_pressed(1) )
                    {
                        isRightClicked = true;
                        isInUse = true;
                    }
                }
                else if( userInput->check_mouse_pressed(-1) || userInput->check_mouse_released(-1) )
                {
                    clickedOutside = true;
                    isInUse = false;
                    hasScrollControl = false;
                    hasArrowkeyControl = false;
                }
            }
            else if( userInput->check_mouse_pressed(-1)  || userInput->check_mouse_released(-1) )
            {
                clickedOutside = true;
                isInUse = false;
                hasScrollControl = false;
                hasArrowkeyControl = false;
            }
        }
    }
}

void GPE_GeneralGuiElement::prerender_self(GPE_Renderer * cRender)
{

}

void GPE_GeneralGuiElement::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{

}

void GPE_GeneralGuiElement::set_name(std::string newName)
{
    opName = newName;
}

void GPE_GeneralGuiElement::set_coords(int newX, int newY)
{
    if( newX!=-1)
    {
        barBox.x = newX;
    }
    if(newY!=-1)
    {
        barBox.y = newY;
    }
}

void GPE_GeneralGuiElement::set_max_width( int nMW)
{
    maxWidth = nMW;
    if( maxWidth > 0 && barBox.w > maxWidth)
    {
        barBox.w = maxWidth;
    }
}

void GPE_GeneralGuiElement::set_max_height( int nMH)
{
    maxHeight = nMH;
    if( maxHeight > 0 && barBox.h > maxHeight)
    {
        barBox.h = maxHeight;
    }
}

void GPE_GeneralGuiElement::set_clicked( bool clickedVal)
{
    isClicked = clickedVal;
}


void GPE_GeneralGuiElement::set_rightclicked( bool clickedVal)
{
    isRightClicked = clickedVal;
}

void GPE_GeneralGuiElement::set_height(int newHeight)
{
    barBox.h = newHeight;
    /*if(barBox.y+barBox.h > SCREEN_HEIGHT)
    {
        barBox.h = SCREEN_HEIGHT-barBox.y;
    }*/
}

void GPE_GeneralGuiElement::set_hovered(bool hoverVal)
{
    isHovered = hoverVal;
}

void GPE_GeneralGuiElement::set_width(int newWid)
{
    barBox.w = newWid;
}


GPE_GuiElementList::GPE_GuiElementList()
{
    hideXScroll = false;
    hideYScroll = false;
    cameraBox.x = menuBox.x = entireBox.x = barBox.x = 0;
    cameraBox.y = menuBox.y = entireBox.y = barBox.y = 0;
    cameraBox.w = menuBox.w = entireBox.w = barBox.w;
    cameraBox.h = menuBox.h = entireBox.h = barBox.h = RESOURCE_kb_AREA_HEIGHT*3;
    xScroll = new GPE_ScrollBar_XAxis();
    yScroll = new GPE_ScrollBar_YAxis();
    cameraBox.w = menuBox.w = entireBox.w = barBox.w - yScroll->get_box_width();
    barXPadding = GENERAL_GPE_PADDING*2;
    barYPadding = GENERAL_GPE_PADDING*2;
    barXMargin = 0;
    barYMargin = 0;
    isInUse = true;
    subElementsHasArrowControl = false;
    subElementsIsScrolling = false;
    hAlign = FA_LEFT;
    vAlign = FA_TOP;
    optionWithViewId = 0;
    guiListTypeName = "guilist";
    tabDelay = 0;
    leserKeyDelay = 0;
    greaterKeyDelay = 0;
    upDelay = 0;
    downDelay = 0;
    leftDelay = 0;
    rightDelay = 0;
}

GPE_GuiElementList::~GPE_GuiElementList()
{
    if( xScroll!=NULL)
    {
        delete xScroll;
        xScroll = NULL;
    }
    if( yScroll!=NULL)
    {
        delete yScroll;
        yScroll = NULL;
    }
    subOptions.clear();
}

void GPE_GuiElementList::add_gui_element(GPE_GeneralGuiElement *  newElement, bool isNLElement)
{
    if( newElement!=NULL)
    {
        newElement->hasLineBreak = isNLElement;
        /*
        if( isNLElement)
        {
            newElement->hasLineBreak = true;
        }
        else
        {

        }
        */
        subOptions.push_back( newElement);
    }
}

void GPE_GuiElementList::clear_list()
{
    subOptions.clear();
    cameraBox.w = menuBox.w = entireBox.w = barBox.w;
    cameraBox.h = menuBox.h = entireBox.h = barBox.h;

}

void GPE_GuiElementList::scroll_down(int yToMove )
{
    if( yToMove > 0 && hideYScroll!=true )
    {
        cameraBox.y+=yToMove;
    }
    if( cameraBox.y+cameraBox.h > entireBox.h )
    {
        cameraBox.y = entireBox.h-cameraBox.h;
    }
    if( cameraBox.y < 0)
    {
        cameraBox.y = 0;
    }
}

void GPE_GuiElementList::scroll_up(int yToMove )
{
    if( yToMove > 0 && hideYScroll!=true )
    {
        cameraBox.y-=yToMove;
    }

    if( cameraBox.y+cameraBox.h > entireBox.h )
    {
        cameraBox.y = entireBox.h-cameraBox.h;
    }
    if( cameraBox.y < 0)
    {
        cameraBox.y = 0;
    }
}

void GPE_GuiElementList::scroll_left(int xToMove )
{
    if( xToMove > 0  && hideXScroll!=true )
    {
        cameraBox.x-=xToMove;
    }

    if( cameraBox.x+cameraBox.w > entireBox.w )
    {
        cameraBox.x = entireBox.w-cameraBox.w;
    }
    if( cameraBox.x < 0)
    {
        cameraBox.x = 0;
    }
}

void GPE_GuiElementList::scroll_right(int xToMove )
{
    if( xToMove > 0 && hideXScroll!=true )
    {
        cameraBox.x+=xToMove;
    }
    if( cameraBox.x+cameraBox.w > entireBox.w )
    {
        cameraBox.x = entireBox.w-cameraBox.w;
    }
    if( cameraBox.x < 0)
    {
        cameraBox.x = 0;
    }
}

void GPE_GuiElementList::process_self(GPE_Rect * viewedSpace ,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);

    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isClicked)
    {
        //if( RESOURCE_TO_DRAG==NULL)
        {
            isInUse = true;
            hasScrollControl = true;
            hasArrowkeyControl = true;
        }
    }
    else if( clickedOutside)
    {
        isInUse = false;
        hasScrollControl = false;
        hasArrowkeyControl = false;
    }
    else if( !isInUse)
    {
        hasScrollControl = false;
    }
    entireBox.x = 0;
    entireBox.y = 0;
    entireBox.w = (barXPadding+barXMargin)*2;
    entireBox.h = (barYPadding+barYMargin)*2;

    menuBox.x = barBox.x+viewedSpace->x;
    menuBox.y = barBox.y+viewedSpace->y;
    if( hideXScroll)
    {
        cameraBox.w = menuBox.w = barBox.w;
    }
    else
    {
        cameraBox.w = menuBox.w = barBox.w-16;
    }
    if( hideYScroll)
    {
        cameraBox.h = menuBox.h = barBox.h;
    }
    else
    {
        cameraBox.h = menuBox.h = barBox.h-16;
    }
    subElementsHasArrowControl = false;
    subElementsIsScrolling = false;
    int xPos = barXMargin+barXPadding;
    int yPos = barYMargin+barYPadding;
    int y2Pos = yPos;
    int rowWidth = 0;
    int maxRowWidth = 0;
    int totalMaxRowWidth = 0;
    int rowHeight = 0;
    bool scrolingHasntOccurred = false;
    if( hideXScroll!=true && hideYScroll!=true )
    {
        scrolingHasntOccurred = true;
    }
    else if ( xScroll->is_scrolling()==false && yScroll->is_scrolling()==false  )
    {
        scrolingHasntOccurred = true;
    }

    GPE_GeneralGuiElement * cContainer = NULL;
    GPE_GeneralGuiElement * dContainer = NULL;
    int i = 0;
    int j = 0;
    int pastRowChange = 0;
    if( isInUse && hasArrowkeyControl )
    {
        if( userInput->down[kb_tab] && userInput->pressed[kb_tab]==false && userInput->released[kb_tab]==false )
        {
            tabDelay += 0.5;
        }
        else
        {
            tabDelay = -1;
        }
        if( userInput->down[kb_left] && userInput->pressed[kb_left]==false && userInput->released[kb_left]==false )
        {
            leftDelay+=0.5;
        }
        else
        {
            leftDelay = -1;
        }
        if( userInput->down[kb_right] && userInput->pressed[kb_right]==false && userInput->released[kb_right]==false )
        {
            rightDelay+=0.5;
        }
        else
        {
            rightDelay = -1;
        }
        if( userInput->down[kb_up] && userInput->pressed[kb_up]==false && userInput->released[kb_up]==false )
        {
            upDelay+=0.5;
        }
        else
        {
            upDelay = -1;
        }
        if(userInput->down[kb_down] && userInput->pressed[kb_down]==false && userInput->released[kb_down]==false )
        {
            downDelay+=0.5;
        }
        else
        {
            downDelay = -1;
        }
        if( userInput->down[kb_comma] && userInput->pressed[kb_comma]==false && userInput->released[kb_comma]==false )
        {
            leserKeyDelay+=0.5;
        }
        else
        {
            leserKeyDelay = -1;
        }

        if( userInput->down[kb_period] && userInput->pressed[kb_period]==false && userInput->released[kb_period]==false )
        {
            greaterKeyDelay+=0.5;
        }
        else
        {
            greaterKeyDelay = -1;
        }
    }
    else
    {
        tabDelay = -1;
        leserKeyDelay = -1;
        greaterKeyDelay = -1;
        upDelay = -1;
        downDelay = -1;
        leftDelay = -1;
        rightDelay = -1;
    }

    if( hAlign==FA_CENTER)
    {
        xPos = barXPadding;
        yPos = barYPadding;
        y2Pos = yPos;
        int optionsSize = (int)subOptions.size();
        for( i=0; i<optionsSize;i++)
        {
            cContainer = subOptions[i];
            if(cContainer!=NULL)
            {
                if( cContainer->get_element_type() == "labelimage" )
                {
                    cContainer->set_width(barBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                else if( cContainer->get_element_type() == "paragraph" )
                {
                    cContainer->set_width(barBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                if( cContainer->get_height() > rowHeight)
                {
                    rowHeight = cContainer->get_height();
                }
                rowWidth+=cContainer->get_width()+barXPadding;
                if( maxRowWidth < rowWidth)
                {
                    maxRowWidth = rowWidth;
                }
                if( totalMaxRowWidth < maxRowWidth)
                {
                    totalMaxRowWidth = maxRowWidth;
                }
                if( cContainer->hasLineBreak || i>=optionsSize-1 )
                {
                    xPos = barXPadding+abs(maxRowWidth-cameraBox.w)/2;

                    for( j = pastRowChange; j <=i; j++)
                    {
                        dContainer = subOptions[j];
                        if(dContainer!=NULL)
                        {
                            dContainer->set_coords(xPos, y2Pos);
                            xPos+=barXPadding+dContainer->get_width();
                        }
                    }
                    pastRowChange = i+1;
                    //pastRowChange++;
                    entireBox.h+=rowHeight+barYPadding;
                    y2Pos+=rowHeight+barYPadding;
                    rowWidth = 0;
                    rowHeight = 0;
                    maxRowWidth = 0;
                }
            }
        }
        xPos = barXPadding+abs(maxRowWidth-cameraBox.w)/2;
        y2Pos+=rowHeight+barYPadding;
        entireBox.h+=rowHeight+barYPadding;
        pastRowChange++;
        for( j = pastRowChange; j < (int)subOptions.size(); j++)
        {
            cContainer = subOptions[j];
            if(cContainer!=NULL)
            {
                cContainer->set_coords(xPos, y2Pos);
                /*
                if( cContainer->get_element_type() == "labelimage" )
                {
                    cContainer->set_width(barBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                else if( cContainer->get_element_type() == "paragraph" )
                {
                    cContainer->set_width(barBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                */
                if(cContainer->autoResizes)
                {
                    cContainer->set_width(barBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                xPos+=barXPadding+cContainer->get_width();
            }
        }
        entireBox.w = totalMaxRowWidth;
    }
    else
    {
        for( i=0; i < (int)subOptions.size();i++)
        {
            cContainer = subOptions[i];
            if(cContainer!=NULL)
            {
                cContainer->set_coords(xPos, y2Pos);
                /*
                if( cContainer->get_element_type() == "labelimage" )
                {
                    cContainer->set_width(barBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                else if( cContainer->get_element_type() == "paragraph" )
                {
                    cContainer->set_width(barBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }*/
                if(cContainer->autoResizes)
                {
                    cContainer->set_width(barBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                if( cContainer->get_height() > rowHeight)
                {
                    rowHeight = cContainer->get_height();
                }
                rowWidth+=cContainer->get_width()+barXPadding;
                if( maxRowWidth < rowWidth)
                {
                    maxRowWidth = rowWidth+barXPadding;
                }
                if( cContainer->hasLineBreak || i==(int)subOptions.size()-1 )
                {
                    entireBox.h+=barYMargin+rowHeight+barYPadding;
                    y2Pos+=rowHeight+barYPadding;
                    rowWidth = 0;
                    rowHeight = 0;
                    xPos = barXMargin+barXPadding;
                }
                else
                {
                    xPos = barXMargin+barXPadding+rowWidth;
                }
            }
        }
        entireBox.w = barXMargin+maxRowWidth;
    }

    if( scrolingHasntOccurred)
    {
        for( i=0; i<(int)subOptions.size();i++)
        {
            cContainer = subOptions[i];
            if(cContainer!=NULL)
            {
                cContainer->process_self(&menuBox,&cameraBox);
                if( cContainer->hasScrollControl )
                {
                    subElementsIsScrolling=true;
                }
                if( cContainer->hasArrowkeyControl )
                {
                    subElementsHasArrowControl=true;
                }
                if( cContainer->is_inuse() )
                {
                    optionWithViewId = i;
                }
            }
        }
    }

    //if(  isHovered || (isInUse && subElementsIsScrolling==false && hasScrollControl ) )
    if( isHovered && subElementsIsScrolling==false)
    {
        if( userInput->mouseScrollingUp )
        {
            if( userInput->shiftKeyIsPressed)
            {
                scroll_left( cameraBox.w/8);
            }
            else
            {
                scroll_up( cameraBox.h/8);
            }
        }
        else if( userInput->mouseScrollingDown )
        {
            if( userInput->shiftKeyIsPressed )
            {
                scroll_right( cameraBox.w/8);
            }
            else
            {
                scroll_down( cameraBox.h/8);
            }
        }
    }

    bool directionChangeRequested = false;

    if( hasScrollControl && subElementsIsScrolling==false)
    {
        if( tabDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( !userInput->pressed[kb_tab] && userInput->released[kb_tab] ) && userInput->check_keyboard_down(kb_ctrl)==false )
        {
            if( userInput->shiftKeyIsPressed)
            {
                optionWithViewId--;
                directionChangeRequested = true;
            }
            else
            {
                optionWithViewId++;
                directionChangeRequested = true;
            }
            tabDelay = -1;
        }
        else if( userInput->check_keyboard_down(kb_ctrl) )
        {
            if( leserKeyDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( !userInput->pressed[kb_comma] && userInput->released[kb_comma] ) )
            {
                optionWithViewId--;
                directionChangeRequested = true;
                leserKeyDelay = -1;
            }
            else if( greaterKeyDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( !userInput->pressed[kb_period] && userInput->released[kb_period] ) )
            {
                optionWithViewId++;
                directionChangeRequested = true;
                greaterKeyDelay = -1;
            }
        }

        if( optionWithViewId < 0)
        {
            optionWithViewId = (int)subOptions.size() -1;
        }

        if( optionWithViewId >= (int)subOptions.size() )
        {
            optionWithViewId = 0;
        }

        if( subElementsHasArrowControl==false)
        {
            if( upDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( !userInput->pressed[kb_up] && userInput->released[kb_up] ) )
            {
                scroll_up( cameraBox.h/4 );
                upDelay = 0;
            }
            else if( downDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( userInput->pressed[kb_down] && !userInput->released[kb_down] ) )
            {
                scroll_down( cameraBox.h/4 );
                downDelay = 0;
            }
        }
    }
    else if( hasScrollControl && subElementsIsScrolling==false)
    {
        if( userInput->check_keyboard_down(kb_ctrl) )
        {
            if( leserKeyDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( userInput->pressed[kb_comma] && !userInput->released[kb_comma] ) && userInput->check_keyboard_down(kb_ctrl)==false )
            {
                optionWithViewId--;
                directionChangeRequested = true;
                leserKeyDelay = -1;
            }
            else if( greaterKeyDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( userInput->pressed[kb_period] && !userInput->released[kb_period] ) && userInput->check_keyboard_down(kb_ctrl)==false )
            {
                optionWithViewId++;
                directionChangeRequested = true;
                greaterKeyDelay = -1;
            }
        }
        if( optionWithViewId < 0)
        {
            optionWithViewId = (int)subOptions.size() -1;
        }

        if( optionWithViewId >= (int)subOptions.size() )
        {
            optionWithViewId = 0;
        }
    }

    if( directionChangeRequested)
    {
        for( i=0; i<(int)subOptions.size();i++)
        {
            cContainer = subOptions[i];
            if(cContainer!=NULL)
            {
                if( optionWithViewId == i)
                {
                    cContainer->switch_inuse( true );
                    ///cameraBox.x = cContainer->get_xpos()-(barXPadding+barXMargin-totalMaxRowWidth);
                    if( cameraBox.y > cContainer->get_ypos() || cContainer->get_ypos() > cameraBox.y+cameraBox.h )
                    {
                        cameraBox.y = cContainer->get_ypos()-(barYPadding+barYMargin);
                    }
                }
                else
                {
                    cContainer->switch_inuse( false );
                }
            }
        }
    }

    if( hasScrollControl && subElementsHasArrowControl==false)
    {
        if( leftDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+1)*FPS_RATIO || ( userInput->pressed[kb_left] && !userInput->released[kb_left] ) )
        {
            scroll_left( cameraBox.w/4 );
            leftDelay =-1;
        }
        else if( rightDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+1)*FPS_RATIO || ( userInput->pressed[kb_right] && !userInput->released[kb_right] ) )
        {
            scroll_right( cameraBox.w/4 );
            rightDelay = -1;
        }

        if( upDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+1)*FPS_RATIO || ( !userInput->pressed[kb_up] && userInput->released[kb_up] ) )
        {
            scroll_up( cameraBox.h/4 );
            upDelay = 0;
        }
        else if( downDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+1)*FPS_RATIO || ( userInput->pressed[kb_down] && !userInput->released[kb_down] ) )
        {
            scroll_down( cameraBox.h/4 );
            downDelay = -1;
        }
    }

    if( optionWithViewId < 0)
    {
        optionWithViewId = (int)subOptions.size() -1;
    }

    if( optionWithViewId >= (int)subOptions.size() )
    {
        optionWithViewId = 0;
    }

    if( xScroll!=NULL && yScroll!=NULL)
    {
        xScroll->barBox.x = barBox.x;
        xScroll->barBox.y = barBox.y+barBox.h;
        xScroll->barBox.w = barBox.w-16;
        xScroll->barBox.h = 16;

        xScroll->fullRect.x = 0;
        xScroll->fullRect.y = 0;
        xScroll->fullRect.w = entireBox.w;
        xScroll->fullRect.h = entireBox.h;

        xScroll->contextRect.x = cameraBox.x;
        xScroll->contextRect.y = cameraBox.y;
        xScroll->contextRect.w = cameraBox.w;
        xScroll->contextRect.h = cameraBox.h;
        if( entireBox.w >=barBox.w  && hideXScroll!=true )
        {
            xScroll->process_self(viewedSpace,cam,true);
        }
        else
        {
            cameraBox.x =0;
            cameraBox.w = menuBox.w = barBox.w;
        }
        if( xScroll->has_moved() || xScroll->is_scrolling() )
        {
            cameraBox.x = xScroll->contextRect.x;
            if( cameraBox.x +cameraBox.w > entireBox.w)
            {
                cameraBox.x = entireBox.w - cameraBox.w;
            }
            if( cameraBox.x < 0)
            {
                cameraBox.x = 0;
            }
        }

        update_rectangle(&yScroll->barBox, barBox.x+barBox.w-16, barBox.y, 16,barBox.h);
        update_rectangle(&yScroll->fullRect, 0, 0, entireBox.w,entireBox.h);
        update_rectangle(&yScroll->contextRect, cameraBox.x, cameraBox.y, cameraBox.w,cameraBox.h);

        //yScroll->process_self( viewedSpace,cam );

        if( hideYScroll!=true )
        {
            yScroll->process_self(viewedSpace,cam,false);
        }
        else
        {
            cameraBox.y = 0;
            cameraBox.w = menuBox.w = barBox.w;
            yScroll->contextRect.y = 0;
            yScroll->contextRect.h = entireBox.h;
        }
        if( entireBox.h <= cameraBox.h)
        {
            yScroll->contextRect.y = 0;
            yScroll->contextRect.h = entireBox.h;
            cameraBox.y = 0;
        }
        if( isInUse )
        {

        }

        if( yScroll->has_moved() )
        {
            cameraBox.y = yScroll->contextRect.y;
            /*
            if( cameraBox.y +cameraBox.h > entireBox.h)
            {
                cameraBox.y = entireBox.h - cameraBox.h;
            }
            if( cameraBox.y < 0)
            {
                cameraBox.y = 0;
            }*/
        }
    }

}

void GPE_GuiElementList::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace,GPE_Rect * cam , bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        cRender->reset_viewpoint();
        cRender->reset_viewpoint();
        menuBox.w+=16;
        menuBox.h+=16;
        cRender->set_viewpoint( &menuBox);

        GPE_GeneralGuiElement * cResource = NULL;
        for(int i=0; i<(int)subOptions.size();i++)
        {
            cResource = subOptions[i];
            if(cResource!=NULL)
            {
                cResource->render_self(cRender,&menuBox,&cameraBox,forceRedraw);
            }
        }

        cRender->reset_viewpoint();
        cRender->set_viewpoint( viewedSpace);
        menuBox.w-=16;
        menuBox.h-=16;
        if( xScroll!=NULL && forceRedraw && hideXScroll!=true )
        {
            if( entireBox.w >barBox.w)
            {
                xScroll->render_self(cRender,viewedSpace,cam);
            }
        }
        if( yScroll!=NULL && forceRedraw && hideYScroll!=true )
        {
            if( entireBox.h >barBox.h )
            {
                yScroll->render_self(cRender,viewedSpace,cam);
            }
        }

        /*if( isInUse && subElementsIsScrolling==false && hasScrollControl && forceRedraw)
        {
            render_rect(cRender,&barBox,GPE_MAIN_TEMPLATE->Button_Box_Highlighted_Color,true);
        }
        else
        {
            render_rect(cRender,&barBox,GPE_MAIN_TEMPLATE->Main_Box_Faded_Color,true);
        }*/
        //if( forceRedraw)
        {
            render_rect(cRender,&barBox,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
        }
    }
}

void GPE_GuiElementList::reset_self()
{
    subOptions.clear();
    cameraBox.x = menuBox.x = entireBox.x = 0;
    cameraBox.y = menuBox.y = entireBox.y = 0;
    cameraBox.w = menuBox.w = entireBox.w = 32;
    cameraBox.h = menuBox.h = entireBox.h = RESOURCE_kb_AREA_HEIGHT*3;
}

void GPE_GuiElementList::set_horizontal_align(int hValue)
{
    hAlign = hValue;
}

void GPE_GuiElementList::set_full_width()
{
    GPE_GeneralGuiElement * tguiElement = NULL;
    int maxWidth = barBox.w-(barXMargin+barXPadding)*2;
    for( int i = (int)subOptions.size()-1; i>=0; i--)
    {
        tguiElement = subOptions[i];
        if( tguiElement!=NULL )
        {
            tguiElement->set_width(maxWidth);
        }
    }
}

void GPE_GuiElementList::set_maxed_out_width()
{
    GPE_GeneralGuiElement * tguiElement = NULL;
    int i = 0;
    int maxWidth = 0;
    for( i = (int)subOptions.size()-1; i>=0; i--)
    {
        tguiElement = subOptions[i];
        if( tguiElement!=NULL )
        {
            if( tguiElement->get_width() > maxWidth)
            {
                maxWidth = tguiElement->get_width();
            }
        }
    }

    for( i = (int)subOptions.size()-1; i>=0; i--)
    {
        tguiElement = subOptions[i];
        if( tguiElement!=NULL )
        {
            tguiElement->set_width(maxWidth);
        }
    }
}

void GPE_GuiElementList::set_maxed_out_height()
{
    GPE_GeneralGuiElement * tguiElement = NULL;
    int i = 0;
    int maxHeight = 0;
    for( i = (int)subOptions.size()-1; i>=0; i--)
    {
        tguiElement = subOptions[i];
        if( tguiElement!=NULL )
        {
            if( tguiElement->get_height() > maxHeight)
            {
                maxHeight = tguiElement->get_height();
            }
        }
    }

    for( i = (int)subOptions.size()-1; i>=0; i--)
    {
        tguiElement = subOptions[i];
        if( tguiElement!=NULL )
        {
            tguiElement->set_height(maxHeight);
        }
    }
}

GPE_TabBar::GPE_TabBar()
{
    tabIsRightClicked = false;
    isInUse = false;
    guiListTypeName = "tabbar";
    barBox.x = 16;
    barBox.y = 16;
    barBox.w = 0;
    barBox.h = 28;

    barXPadding = GENERAL_GPE_PADDING*2;
    barYPadding = 4;
    fontTextWidth = 12;
    fontTextHeight = 12;
    if( DEFAULT_FONT!=NULL)
    {
         DEFAULT_FONT->get_metrics("A",&fontTextWidth,&fontTextHeight);
    }
    else
    {
        fontTextWidth = 12;
        fontTextHeight = 12;
    }
    tabInUse = 0;
    tabSize = -1; //defaults to equally divide the tab onto the screen

    miniTabSize = 48;
    extraSmallSize = 64;
    smallTabSize = 96;
    medTabSize = 128;
    largeTabSize = 160;
    extraLargeTabSize = 192;
    xxLargeTabSize = 256;
    tabsPerView = -1;
    tabPos = 0;
}

GPE_TabBar::~GPE_TabBar()
{
    subOptions.clear();
}

void GPE_TabBar::add_new_tab(std::string newOption, bool switchToNew )
{
    if( (int)newOption.size() > 0)
    {
        //automatically add  resource to when the tab bar is empty
        if( switchToNew)
        {
            tabInUse = (int)subOptions.size();
        }
        subOptions.push_back(newOption);
        barBox.w+=fontTextWidth*(int)newOption.size()+barXPadding;
    }
}

void GPE_TabBar::open_tab(int tabId)
{
    if( tabId >=0 && tabId < (int)subOptions.size() )
    {
        tabInUse = tabId;
    }
    else
    {
        tabInUse = 0;
    }
}

void GPE_TabBar::open_tab(std::string tabName)
{
    if( (int)tabName.size() > 0)
    {
        for( int i = 0; i < (int)subOptions.size(); i++)
        {
            if( tabName==subOptions[i] )
            {
                tabInUse = i;
                break;
            }
        }
    }
}

std::string GPE_TabBar::get_selected_name()
{
    if( (int)subOptions.size() > 0)
    {
        if( tabInUse >=0 && tabInUse < (int)subOptions.size() )
        {
            return subOptions[tabInUse];
        }
    }
    return "";
}

void GPE_TabBar::process_self(GPE_Rect * viewedSpace,GPE_Rect *cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    tabIsRightClicked = false;
    if( barBox.w!=0 && viewedSpace!=NULL && cam!=NULL)
    {
        GPE_GeneralGuiElement::process_self(viewedSpace, cam);
        if( isClicked || isRightClicked)
        {
            isInUse = true;
        }
        else if( clickedOutside)
        {
            isInUse = false;
        }
        if( isInUse)
        {
            if( isClicked || isRightClicked)
            {
                int cTabXPos = barBox.x+viewedSpace->x-cam->x;
                int cTabX2Pos = barBox.x+viewedSpace->x-cam->x;
                int cTabYPos = barBox.y+viewedSpace->y-cam->y;
                int cTabY2Pos = barBox.y+barBox.h+viewedSpace->y-cam->y;
                for(int i=0; i< (int)subOptions.size(); i++)
                {
                    cTabX2Pos+=fontTextWidth*(int)subOptions[i].size()+barXPadding;
                    if(point_within(userInput->mouse_x,userInput->mouse_y,cTabXPos,cTabYPos,cTabX2Pos,cTabY2Pos) )
                    {
                        tabInUse = i;
                        if( isRightClicked )
                        {
                            tabIsRightClicked = true;
                        }
                    }
                    cTabXPos = cTabX2Pos+1;
                }
            }
            if( userInput->check_keyboard_released(kb_left) &&  tabInUse > 0)
            {
                tabInUse--;
            }
            else if( userInput->check_keyboard_released(kb_right) && tabInUse < (int)subOptions.size()-1 )
            {
                tabInUse++;
            }
        }
    }
}

void GPE_TabBar::render_self(GPE_Renderer * rendTarget, GPE_Rect * viewedSpace,GPE_Rect * cam , bool forceRedraw)
{
    //render_rect(rendTarget,&barBox,barColor,false);
    //render_rect(rendTarget,&barBox,barOutlineColor,true);
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(forceRedraw && viewedSpace!=NULL && cam!=NULL)
    {
        if( (int)subOptions.size() >0 )
        {
            int cTabXPos = barBox.x-cam->x;
            int cTabX2Pos = barBox.x-cam->x;
            int thisTabWidth = 0;
            for(int i=0; i< (int)subOptions.size(); i++)
            {
                thisTabWidth=fontTextWidth*(int)subOptions[i].size()+barXPadding;
                cTabX2Pos+=thisTabWidth;
                if( tabInUse==i)
                {
                    render_rectangle(rendTarget,cTabXPos,barBox.y-cam->y,cTabX2Pos,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Program_Color,false);
                    render_rectangle(rendTarget,cTabXPos,barBox.y-cam->y,cTabX2Pos,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
                    render_new_text(rendTarget,cTabXPos+thisTabWidth/2,barBox.y+barBox.h/2-cam->y,subOptions[i],GPE_MAIN_TEMPLATE->Main_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
                }
                else
                {
                    render_rectangle(rendTarget,cTabXPos,barBox.y-cam->y,cTabX2Pos,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Program_Header_Color,false);
                    render_rectangle(rendTarget,cTabXPos,barBox.y-cam->y,cTabX2Pos,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
                    render_new_text(rendTarget,cTabXPos+thisTabWidth/2,barBox.y+barBox.h/2-cam->y,subOptions[i],GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
                }
                cTabXPos = cTabX2Pos+1;
            }
        }
        else
        {
            render_rectangle(rendTarget,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Program_Color,false);
            render_rectangle(rendTarget,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
        }
        /*if( isHovered)
        {
            //Uncomment in the even a tab bar randomly behaves weirdly
            render_rectangle(rendTarget,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Border_Highlighted_Color,true);
        }*/
    }
}

GPE_StatusBar::GPE_StatusBar()
{
    guiListTypeName = "statusbar";
    codeEditorStatusBarString = "";
    insertModeString = "";
}

GPE_StatusBar::~GPE_StatusBar()
{

}

void GPE_StatusBar::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    codeEditorStatusBarString= "";
    insertModeString = "";
}

void GPE_StatusBar::render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace,GPE_Rect *cam, bool forceRedraw)
{
    if( isEnabled && forceRedraw && cRender!=NULL && barBox.h >0 && FONT_STATUSBAR!=NULL)
    {
        render_rect(cRender,&barBox,GPE_MAIN_TEMPLATE->Program_Color,false);
        int statusBarStringWidth = 0, statusBarStringHeight = 0;
        int projectNameWidth = 0, projectNameHeight = 0;
        if( (int)CURRENT_PROJECT_NAME.size() > 0)
        {
            std::string  projectNameToRender = CURRENT_PROJECT_NAME;
            /*Commented out for Version 1.13 temporarily
            if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->showShortProjectNames!=NULL  )
            {
                if( MAIN_EDITOR_SETTINGS->showShortProjectNames->is_clicked() )
                {
                    projectNameToRender = get_file_noext( get_local_from_global_file(CURRENT_PROJECT_NAME) );

                }
            }
            */
            FONT_STATUSBAR->get_metrics(projectNameToRender,&projectNameWidth, &projectNameHeight);

            if( (int)codeEditorStatusBarString.size() > 0)
            {
                FONT_STATUSBAR->get_numbered_metrics(codeEditorStatusBarString,&statusBarStringWidth, &statusBarStringHeight);
                if( barBox.w > statusBarStringWidth+projectNameWidth +GENERAL_GPE_PADDING )
                {
                    render_new_text(cRender,barBox.x,barBox.y+GENERAL_GPE_PADDING,projectNameToRender,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_TOP);
                    //render_rectangle(cRender,barBox.x+projectNameWidth,barBox.y,barBox.x+barBox.w,barBox.y+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Color,false);
                    render_bitmap_text(cRender,barBox.x+projectNameWidth+GENERAL_GPE_PADDING,barBox.y+GENERAL_GPE_PADDING,codeEditorStatusBarString,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_TOP);
                }
                else
                {
                    render_bitmap_text(cRender,barBox.x+GENERAL_GPE_PADDING,barBox.y+GENERAL_GPE_PADDING,codeEditorStatusBarString,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_TOP);
                }
            }
            else if( barBox.w > projectNameWidth +GENERAL_GPE_PADDING )
            {
                render_new_text(cRender,barBox.x+GENERAL_GPE_PADDING,barBox.y+GENERAL_GPE_PADDING,projectNameToRender,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_TOP);
            }
            else
            {
                render_new_text(cRender,barBox.x+barBox.w-GENERAL_GPE_PADDING/2,barBox.y+GENERAL_GPE_PADDING,projectNameToRender,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_STATUSBAR,FA_RIGHT,FA_TOP);
            }
        }
        else if( (int)codeEditorStatusBarString.size() > 0)
        {
            FONT_STATUSBAR->get_numbered_metrics(codeEditorStatusBarString,&statusBarStringWidth, &statusBarStringHeight);
            render_bitmap_text(cRender,barBox.x+GENERAL_GPE_PADDING,barBox.y+GENERAL_GPE_PADDING,codeEditorStatusBarString,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_TOP);
        }
        render_rect(cRender,&barBox,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
    }
}

void GPE_StatusBar::reset_statusbar()
{
    codeEditorStatusBarString = "";
    insertModeString = "";
}

void GPE_StatusBar::set_codestring(std::string newCodeString)
{
    codeEditorStatusBarString = newCodeString;
}

void GPE_StatusBar::set_insertmode(std::string newInsertMode)
{
    insertModeString = newInsertMode;
}


GPE_PopUpMenu_Option::GPE_PopUpMenu_Option(std::string name, int id, bool selectable, bool seeShortCut, bool makeContext, int kbS1, int kbS2 , int kbS3 )
{
    upDelay = 0;
    downDelay = 0;
    leftDelay = 0;
    rightDelay = 0;
    kbShortCut1 = kbS1;
    kbShortCut2 = kbS2;
    kbShortCut3 = kbS3;
    shortcutString = "";
    if( kbShortCut1 >=0 && kbShortCut1 < userInput->key_count )
    {
        shortcutString = userInput->binding_name[kbShortCut1];
        if( kbShortCut2 >=0 && kbShortCut2 < userInput->key_count )
        {
            shortcutString += "+"+userInput->binding_name[kbShortCut2];
            if( kbShortCut3 >=0 && kbShortCut3 < userInput->key_count )
            {
                shortcutString += "+"+userInput->binding_name[kbShortCut3];
            }
        }
    }
    //barColor = GPE_MAIN_TEMPLATE->PopUp_Box_Color;
    renderWhite = false;
    isResourceOption = false;
    sectionBottom = false;
    opName = name;
    opId = id;
    optionWidth = 0;

    optionWidthSpace = INT_MIN;
    subOptionWidthSpace = INT_MIN;

    int textW = 0;
    int textH = 0;
    if( (int)opName.size()>0)
    {
        textW = 0;
        textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        optionWidth = textW;
        optionWidthSpace = textW;
        subOptionWidthSpace = optionWidthSpace;
    }
    if( (int)shortcutString.size()>0 )
    {
        textW = 0;
        textH = 0;
        DEFAULT_FONT->get_metrics(shortcutString,&textW, &textH);
        optionWidth += textW+GENERAL_GPE_PADDING;
        optionWidthSpace += textW+GENERAL_GPE_PADDING;
        subOptionWidthSpace += optionWidthSpace+GENERAL_GPE_PADDING;
    }
    widestOptionSpace = INT_MIN;
    subMenuIsOpen = false;
    isTopOfMenu = true;
    isContext = makeContext;
    hoverOption = -1;
    opSprite = NULL;
    spriteFrameNumber = 0;
    optionBox.x = 0;
    optionBox.y = 0;
    optionBox.w = 0;
    optionBox.h = GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING;
    isSelectable = selectable;
    showShortCuts = seeShortCut;
    subMenuStartPos= 0;
    maxOptionsToView = SCREEN_HEIGHT/optionBox.h;
    selfDistanceToBottom = 0;
    isFolderOption = false;
}

GPE_PopUpMenu_Option::~GPE_PopUpMenu_Option()
{
    GPE_PopUpMenu_Option * tOption = NULL;
    for (int i = 0; i < (int)subOptions.size(); i++)
    {
        tOption = subOptions[i];
        if( tOption!=NULL)
        {
            delete tOption;
            tOption = NULL;
        }
    }
    subOptions.clear();
}

int GPE_PopUpMenu_Option::activate_hovered()
{
    GPE_PopUpMenu_Option * gOption = NULL;
    if( hoverOption >=0 && hoverOption < (int)subOptions.size() )
    {
        gOption = subOptions[hoverOption];
    }
    else if( selectedOption >=0 && selectedOption < (int)subOptions.size() )
    {
        gOption = subOptions[selectedOption];
    }
    //if its possible to open up the suboption do it/return mkay
    if( gOption!=NULL )
    {
        return gOption->activate_hovered();
    }
    else
    {
        if( get_menu_size() == 0 )
        {
            if(isSelectable)
            {
                 //select, execute and exit
                update_popup_info(opName,opId, isContext);
                return opId;
            }
            else
            {
                return -3; //unselectable
            }
        }
    }
    return -1;
}

void GPE_PopUpMenu_Option::clear_menu()
{
    GPE_PopUpMenu_Option * tOption = NULL;
    for (int i = 0; i < (int)subOptions.size(); i++)
    {
        tOption = subOptions[i];
        if( tOption!=NULL)
        {
            delete tOption;
            tOption = NULL;
        }
    }
    subOptions.clear();
    hoverOption = -1;
    selectedOption = -1;
    subMenuIsOpen = false;
    subMenuStartPos = 0;
    maxOptionsToView = 0;
    selfDistanceToBottom = 0;
}

std::string GPE_PopUpMenu_Option::get_name()
{
    return opName;
}

int GPE_PopUpMenu_Option::get_menu_size()
{
    return (int)subOptions.size();
}

GPE_PopUpMenu_Option * GPE_PopUpMenu_Option::get_menu_option(int atNumb)
{
    if( atNumb>=0 && atNumb<(int)subOptions.size())
    {
        return subOptions[atNumb];
    }
    return NULL;
}

int GPE_PopUpMenu_Option::get_width()
{
    return optionWidth;
}

bool GPE_PopUpMenu_Option::is_open()
{
    return subMenuIsOpen;
}

bool GPE_PopUpMenu_Option::push_left()
{
    GPE_PopUpMenu_Option * gOption = NULL;
    if( selectedOption >=0 && selectedOption < (int)subOptions.size() )
    {
        gOption = subOptions[selectedOption];
    }
    else if( hoverOption >=0 && hoverOption < (int)subOptions.size() )
    {
        gOption = subOptions[hoverOption];
    }
    else
    {
        hoverOption = -1;
        selectedOption = -1;
        if( isContext)
        {
            hoverOption = 0;
        }
        else
        {
            subMenuIsOpen = false;
        }
    }
    //if its possible to open up the suboption do it/return mkay
    if( gOption!=NULL )
    {
        if( gOption->is_open()==false || gOption->get_menu_size() == 0 )
        {
            gOption->subMenuIsOpen = false;
            hoverOption = -1;
            selectedOption = -1;
            if( isContext)
            {
                hoverOption = 0;
            }
            else
            {
                subMenuIsOpen = false;
            }
            return true;
        }
        else
        {
            return gOption->push_left();
        }
    }
    return false;
}

bool GPE_PopUpMenu_Option::push_right()
{
    if( subMenuIsOpen==false && (int)subOptions.size() > 0 )
    {
        reset_suboptions();
        subMenuIsOpen = true;
        selectedOption =  hoverOption = 0;
        return true;
    }
    else
    {
        GPE_PopUpMenu_Option * gOption = NULL;
        int tempCursorPos = 0;
        if( selectedOption >=0 && selectedOption < (int)subOptions.size() )
        {
            tempCursorPos = selectedOption;
            gOption = subOptions[selectedOption];
            hoverOption = -1;
        }
        else if( hoverOption >=0 && hoverOption < (int)subOptions.size() )
        {
             tempCursorPos = hoverOption;
            gOption = subOptions[hoverOption];
            selectedOption = -1;
        }
        else if( (int)subOptions.size() > 0)
        {
            selectedOption = hoverOption = 0;
            gOption = subOptions[0];
            selectedOption = -1;
        }
        if( gOption!=NULL )
        {
            //if its possible to open up the suboption do it/return mkay
            if( gOption->push_right() )
            {
                selectedOption = hoverOption =  tempCursorPos;
                return true;
            }
        }
    }
    return false;
}

bool GPE_PopUpMenu_Option::push_up()
{
    if( subMenuIsOpen==false && isContext==false)
    {
        return false;
    }
    if( (int)subOptions.size() > 0 )
    {
        GPE_PopUpMenu_Option * fOption = NULL;
        int currentCursorPos = 0;
        if( selectedOption >=0 && selectedOption < (int)subOptions.size() )
        {
            currentCursorPos = hoverOption = selectedOption;
            fOption = subOptions[currentCursorPos];

        }
        else if( hoverOption >=0 && hoverOption < (int)subOptions.size() )
        {
            currentCursorPos = hoverOption;
            fOption = subOptions[currentCursorPos];
        }
        else if( (int)subOptions.size() > 0 )
        {
            if( subMenuIsOpen == false && isContext==false )
            {
                reset_suboptions();
                subMenuIsOpen = true;
            }
            else if( isContext)
            {
                subMenuIsOpen = true;
            }
            currentCursorPos = hoverOption = selectedOption = 0;
            fOption = subOptions[currentCursorPos];
        }
        /*else
        {
            currentCursorPos = hoverOption = selectedOption = -1;
        }*/

        if( fOption!=NULL)
        {
            bool moveUpward = false;
            if( fOption->get_menu_size() > 0 && fOption->subMenuIsOpen  )
            {
                fOption->push_up();
                /*if( fOption->push_up() == false )
                {
                    moveUpward = true;
                }*/
            }
            else
            {
                moveUpward = true;
            }

            if( moveUpward)
            {
                currentCursorPos--;
                if( currentCursorPos < 0)
                {
                    currentCursorPos = (int)subOptions.size()-1;
                }
                if( currentCursorPos < 0)
                {
                    currentCursorPos = 0;
                }
                //reset_suboptions();
                hoverOption = selectedOption = currentCursorPos;
            }
            return moveUpward;
        }
    }
    return false;
}

bool GPE_PopUpMenu_Option::push_down()
{
    if( subMenuIsOpen==false && isContext==false)
    {
        return false;
    }
    if( (int)subOptions.size() > 0 )
    {
        GPE_PopUpMenu_Option * fOption = NULL;
        int currentCursorPos = 0;
        if( selectedOption>=0 && selectedOption < (int)subOptions.size() )
        {
            currentCursorPos = hoverOption =  selectedOption;
            fOption = subOptions[currentCursorPos];
        }
        else if( hoverOption>=0 && hoverOption < (int)subOptions.size() )
        {
            currentCursorPos = selectedOption =  hoverOption;
            fOption = subOptions[currentCursorPos];
        }
        else if( (int)subOptions.size() > 0 )
        {
            if( subMenuIsOpen == false)
            {
                //reset_suboptions();
                subMenuIsOpen = true;
            }
            currentCursorPos = selectedOption =  hoverOption = 0;
            fOption = subOptions[currentCursorPos];
        }
        /*else
        {
            currentCursorPos = hoverOption = selectedOption = -1;
        }*/

        if( fOption!=NULL)
        {
            bool moveDownward = false;
            if( fOption->get_menu_size() > 0 && fOption->subMenuIsOpen )
            {
                fOption->push_down();
                /*
                if( fOption->push_down() == false )
                {
                    moveDownward = true;
                }
                */
            }
            else
            {
                moveDownward = true;
            }

            if( moveDownward)
            {
                currentCursorPos++;
                if( currentCursorPos >= (int)subOptions.size() )
                {
                    currentCursorPos = 0;
                }
                selectedOption = hoverOption = currentCursorPos;
            }
            return moveDownward;
        }
    }
    return false;
}


void GPE_PopUpMenu_Option::set_id(int newId)
{
    opId = newId;
}

GPE_PopUpMenu_Option * GPE_PopUpMenu_Option::add_menu_option( std::string name, int id, GPE_Texture * gTexture, int spriteImgNumb,GPE_Sprite * gSprite, bool endsSection, bool selectable, bool isResource, int kbS1, int kbS2 , int kbS3 )
{
    maxOptionsToView = SCREEN_HEIGHT/GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING;
    selfDistanceToBottom = (SCREEN_HEIGHT-optionBox.y-optionBox.h)/GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING;

    GPE_PopUpMenu_Option * newOp = new GPE_PopUpMenu_Option(name,id,selectable,showShortCuts,false, kbS1, kbS2, kbS3);
    newOp->optionBox.h = GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING;
    newOp->isTopOfMenu = false;
    newOp->sectionBottom = endsSection;
    newOp->subMenuIsOpen = false;
    newOp->isResourceOption = isResource;

    GPE_PopUpMenu_Option * tempFOption = NULL;
    int i = 0;
    if( id<0)
    {
        //newOp->set_id( (int)subOptions.size() *-1);
        newOp->set_id(  -1 );
    }
    newOp->set_image_data(gSprite, spriteImgNumb);
    newOp->set_texture(gTexture);
    int menuCountSize = (int)subOptions.size();
    if(isContext)
    {
        if( menuCountSize > selfDistanceToBottom)
        {
            newOp->set_position(optionBox.x ,optionBox.y+(menuCountSize-selfDistanceToBottom)*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING);
        }
        else
        {
            newOp->set_position(optionBox.x ,optionBox.y+menuCountSize*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING );
        }
        //newOp->barColor = GPE_MAIN_TEMPLATE->PopUp_Box_Color;
        newOp->set_width(optionWidthSpace);
    }
    else if( isTopOfMenu)
    {
        newOp->set_position(optionBox.x ,optionBox.y+optionBox.h+ menuCountSize*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING );
        //newOp->barColor = GPE_MAIN_TEMPLATE->PopUp_Box_Color;
        newOp->set_width(optionWidthSpace);
    }
    else
    {
        if( menuCountSize > selfDistanceToBottom)
        {
            newOp->set_position(optionBox.x+optionBox.w ,optionBox.y+(menuCountSize-selfDistanceToBottom)*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING );
        }
        else
        {
            newOp->set_position(optionBox.x+optionBox.w ,optionBox.y+menuCountSize*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING );
        }
        //newOp->barColor = barColor;
        newOp->set_width(subOptionWidthSpace);
    }
    subOptions.push_back(newOp);

    //resizes submenu in the event that the new option is wider
    if( newOp->get_width()>=widestOptionSpace)
    {
        widestOptionSpace = newOp->get_width();
        if( showShortCuts)
        {
             subOptionWidthSpace = widestOptionSpace+TOOLKEY_SHORTCUT_WIDTH+GENERAL_ICON_WIDTH_AND_PADDING*2;
        }
        else
        {
            subOptionWidthSpace = widestOptionSpace+GENERAL_ICON_WIDTH_AND_PADDING*2;
        }
        //if( isContext || isTopOfMenu)
        {
            optionWidthSpace = subOptionWidthSpace;
        }
        tempFOption = NULL;
        for( i=0; i<(int)subOptions.size(); i++)
        {
            tempFOption = subOptions[i];
            if( isContext || isTopOfMenu)
            {
                tempFOption->set_width(optionWidthSpace);
            }
            else
            {
                tempFOption->set_width(subOptionWidthSpace);
            }
        }
    }
    return newOp;
}

void GPE_PopUpMenu_Option::prerender_self( GPE_Renderer * cRender)
{
    /*GPE_PopUpMenu_Option * tempOption = NULL;
    for( int i = 0; i < (int)subOptions.size(); i++)
    {
        tempOption = subOptions[i];
        tempOption->prerender_self(cRender);
    }*/
}

int GPE_PopUpMenu_Option::process_menu_option()
{
    int barAction = -1;
    int returnAction = -1;
    if( userInput->check_mouse_released(1) || userInput->check_mouse_released(2) || WINDOW_WAS_JUST_RESIZED || userInput->released[kb_esc])
    {
        returnAction = -2;
    }
    else
    {
        if( subMenuIsOpen )
        {
            if( isContext )
            {
                if( userInput->down[kb_up] && userInput->pressed[kb_up]==false && userInput->released[kb_up]==false )
                {
                    upDelay+=0.5;
                }
                else
                {
                    upDelay = -1;
                }

                if( userInput->down[kb_down] && userInput->pressed[kb_down]==false && userInput->released[kb_down]==false )
                {
                    downDelay+=0.5;
                }
                else
                {
                    downDelay = -1;
                }

                if( userInput->down[kb_left] && userInput->pressed[kb_left]==false && userInput->released[kb_left]==false )
                {
                    leftDelay+=0.5;
                }
                else
                {
                    leftDelay = -1;
                }

                if( userInput->down[kb_right] && userInput->pressed[kb_right]==false && userInput->released[kb_right]==false )
                {
                    rightDelay+=0.5;
                }
                else
                {
                    rightDelay = -1;
                }
            }
            else
            {
                upDelay = -1;
                downDelay = -1;
                leftDelay = -1;
                rightDelay = -1;
            }
            int subPosX = optionBox.x;
            maxOptionsToView = SCREEN_HEIGHT/GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING;
            selfDistanceToBottom = (SCREEN_HEIGHT-optionBox.y-optionBox.h)/GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING;
            GPE_PopUpMenu_Option * fOption = NULL;
            int menuCountSize = (int)subOptions.size() ;
            int extraRenderSpace = 0;
            int subOptionsCurrWidth = optionWidthSpace;
            if( isTopOfMenu==false && isContext==false)
            {
                extraRenderSpace = optionBox.w;
                subOptionsCurrWidth = subOptionWidthSpace;
            }

            if( menuCountSize > maxOptionsToView)
            {
                if( point_between(userInput->mouse_x,userInput->mouse_y,optionBox.x+extraRenderSpace,0,optionBox.x+extraRenderSpace+subOptionsCurrWidth,SCREEN_HEIGHT) )
                {
                    if( userInput->mouseScrollingUp )
                    {
                        subMenuStartPos-=maxOptionsToView/8;
                    }

                    else if( userInput->mouseScrollingDown)
                    {
                        subMenuStartPos+=maxOptionsToView/8;
                    }
                }
                if( subMenuStartPos+maxOptionsToView > (int)subOptions.size() )
                {
                    subMenuStartPos = (int)subOptions.size()-maxOptionsToView;
                }
                if( subMenuStartPos < 0)
                {
                    subMenuStartPos = 0;
                }
            }

            bool keyActivityHappend = false;
            if( isContext)
            {
                int currentCursorPos = 0;
                if( selectedOption>=0 && selectedOption < (int)subOptions.size() )
                {
                    fOption = subOptions[selectedOption];
                    currentCursorPos = selectedOption;
                }
                else if( hoverOption>=0 && hoverOption < (int)subOptions.size() )
                {
                    fOption = subOptions[hoverOption];
                    currentCursorPos = hoverOption;
                }
                /*else if( (int)subOptions.size() > 0 && subOptions[0]!=NULL )
                {
                    currentCursorPos = hoverOption = 0;
                    selectedOption = -1;
                    fOption = subOptions[hoverOption];
                }*/

                if( leftDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !userInput->pressed[kb_left] && userInput->released[kb_left] ) )
                {
                    leftDelay = -1;
                    push_left();
                    keyActivityHappend = true;
                }
                else if( rightDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !userInput->pressed[kb_right] && userInput->released[kb_right] ) )
                {
                    push_right();
                    rightDelay = -1;
                    keyActivityHappend = true;

                }
                else if( upDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !userInput->pressed[kb_up] && userInput->released[kb_up] ) )
                {
                    push_up();
                    upDelay = -1;
                    keyActivityHappend = true;
                }
                else if( downDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !userInput->pressed[kb_down] && userInput->released[kb_down] ) )
                {
                    push_down();
                    downDelay = -1;
                    keyActivityHappend = true;
                }
                else if( userInput->released[kb_enter] || userInput->released[kb_space] && subMenuIsOpen  )
                {
                    returnAction = activate_hovered();
                    hoverOption = -1;
                    selectedOption = -1;
                    reset_suboptions();
                    subMenuIsOpen = false;
                    keyActivityHappend = true;
                }
                else
                {
                    keyActivityHappend = false;
                }
            }
            else
            {
                keyActivityHappend = false;
            }

            if( keyActivityHappend == false )
            {
                for( int i=subMenuStartPos; i<menuCountSize && i < subMenuStartPos+maxOptionsToView; i++)
                {
                    if( barAction<0)
                    {
                        fOption = subOptions[i];
                        if( isTopOfMenu==false || isContext)
                        {
                            if( isContext)
                            {
                                fOption->set_width(optionBox.w);
                                subPosX = optionBox.x;
                            }
                            else
                            {
                                subPosX = optionBox.x+optionBox.w;
                            }
                            if( menuCountSize > selfDistanceToBottom)
                            {
                                if( menuCountSize > maxOptionsToView)
                                {
                                    //scroller will be required here
                                    fOption->set_position(subPosX,(i-subMenuStartPos)*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING );
                                }
                                else
                                {
                                    //offset box, no scroller needed
                                    fOption->set_position(subPosX ,optionBox.y+(i-(menuCountSize-selfDistanceToBottom) )*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING );
                                }
                            }
                            else
                            {
                                fOption->set_position(subPosX ,optionBox.y+i*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING);
                            }
                        }
                        else if( isTopOfMenu)
                        {
                            fOption->set_position(optionBox.x+extraRenderSpace ,optionBox.y+optionBox.h+i*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING);
                        }

                        barAction = fOption->process_menu_option();
                        if( userInput->mouseMovementInputReceivedInFrame )
                        {
                            if( point_between(userInput->mouse_x, userInput->mouse_y,fOption->optionBox.x,fOption->optionBox.y,fOption->optionBox.x+fOption->optionBox.w,fOption->optionBox.y+fOption->optionBox.h) )
                            {
                                reset_suboptions();
                                hoverOption = i;
                                fOption->subMenuIsOpen = true;
                            }
                        }
                        if(barAction!=-1)
                        {
                            returnAction = barAction;
                        }
                    }
                }

            }
        }
        else
        {
            upDelay = -1;
            downDelay = -1;
            leftDelay = -1;
            rightDelay = -1;
        }

        if(  isTopOfMenu==false && point_between(userInput->mouse_x, userInput->mouse_y,optionBox.x+1,optionBox.y+1,optionBox.x+optionBox.w-1,optionBox.y+optionBox.h-1) )
        {
            if( (int)subOptions.size()>0)
            {
                if( userInput->mouseMovementInputReceivedInFrame )
                {
                    reset_suboptions();
                    subMenuIsOpen = true;
                    GPE_PopUpMenu_Option * fOptionOpened = NULL;
                    for( int iN = 0; iN < (int)subOptions.size(); iN++)
                    {
                        fOptionOpened = subOptions.at(iN);
                        if( fOptionOpened!=NULL)
                        {
                            fOptionOpened->optionBox.x = optionBox.x+optionBox.w;
                        }
                    }
                }
            }
            else if( userInput->check_mouse_released(0) || userInput->released[kb_enter] )
            {
                //actual action happened...
                if(isSelectable)
                {
                    returnAction = opId; //select, execute and exit
                    update_popup_info(opName,opId, isContext);
                }
                else
                {
                    returnAction = -3; //unselectable
                }
            }
        }
        if( returnAction==1)
        {
            hoverOption = -1;
            selectedOption = -1;
            subMenuIsOpen = false;
            reset_suboptions();
        }
    }
    return returnAction;
}

void GPE_PopUpMenu_Option::open_menu()
{
    reset_suboptions();
    subMenuIsOpen = true;
    if( (int)subOptions.size() > 0 )
    {
        hoverOption = selectedOption = 0;
    }
}
void GPE_PopUpMenu_Option::reset_suboptions()
{
    GPE_PopUpMenu_Option * gOption = NULL;
    for(int j=0; j<(int)subOptions.size(); j++)
    {
        gOption = subOptions[j];
        gOption->selectedOption = -1;
        gOption->hoverOption = -1;
        gOption->subMenuIsOpen = false;
        gOption->subMenuStartPos = 0;
        gOption->reset_suboptions();
    }
    /*subMenuIsOpen = false;
    selectedOption = -1;
    hoverOption = -1;*/
}

void GPE_PopUpMenu_Option::set_image_data(GPE_Sprite * newSprite,int newId)
{
    opSprite = newSprite;
    spriteFrameNumber = newId;
}

void GPE_PopUpMenu_Option::set_position(int xPos, int yPos)
{
    if( xPos !=-1)
    {
        optionBox.x = xPos;
    }
    if( yPos !=-1)
    {
        optionBox.y = yPos;
    }
    GPE_PopUpMenu_Option* cOption = NULL;
    int subBoxTotal = (int)subOptions.size();
    for( int i=0; i< subBoxTotal;i++)
    {
        cOption = subOptions[i];
        if(cOption!=NULL)
        {
            cOption->set_position(optionBox.x+optionBox.w,optionBox.y+i*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING);
        }
    }
}

void GPE_PopUpMenu_Option::set_texture(GPE_Texture * newTexture)
{
    opTexture = newTexture;
}

void GPE_PopUpMenu_Option::set_width(int newWidth)
{
    optionBox.w = newWidth;
    if(!isTopOfMenu)
    {
        GPE_PopUpMenu_Option * cOption = NULL;
        int subBoxTotal = (int)subOptions.size();
        for( int i=0; i< subBoxTotal;i++)
        {
            cOption = subOptions[i];
            if(cOption!=NULL)
            {
                cOption->set_position(optionBox.x+optionBox.w,optionBox.y+i*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING);
                if( isContext)
                {
                    cOption->set_position(optionBox.x+subOptionWidthSpace,-1);
                    cOption->set_width(newWidth);
                }
            }
        }
    }
}

void GPE_PopUpMenu_Option::render_self(GPE_Renderer * cRender, GPE_Rect *cam, bool forceRedraw)
{
    if( forceRedraw )
    {
        int i = 0;
        if(sectionBottom )
        {
            render_line(cRender,optionBox.x,optionBox.y+optionBox.h,optionBox.x+optionBox.w,optionBox.y+optionBox.h,c_dkgray);
        }
        if( (int)opName.size()>0 )
        {
            if( (int)subOptions.size()>0)
            {
                if( subMenuIsOpen)
                {
                    GPE_PopUpMenu_Option * fOption = NULL;
                    int menuCountSize = (int)subOptions.size();
                    int subRenderYPos = optionBox.y;
                    int subRenderHeight = (int)subOptions.size()*GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING;
                    if( menuCountSize > selfDistanceToBottom)
                    {
                        if( menuCountSize > maxOptionsToView)
                        {
                            //scroller will be required here
                            subRenderYPos = 0;
                            subRenderHeight = SCREEN_HEIGHT;
                        }
                        else
                        {
                            //offset box, no scroller needed
                            if( isTopOfMenu && isContext==false)
                            {
                                subRenderYPos = optionBox.y+optionBox.h;
                            }
                            else
                            {
                                subRenderYPos = optionBox.y- ( abs(menuCountSize-selfDistanceToBottom) *GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING);
                            }
                        }
                    }
                    else
                    {
                        if( isTopOfMenu && isContext==false )
                        {
                            subRenderYPos = optionBox.y+optionBox.h;
                        }
                        else if( isContext)
                        {
                            subRenderYPos = optionBox.y;
                        }
                    }

                    if( isTopOfMenu || isContext)
                    {
                        if( isContext)
                        {
                            render_rectangle(cRender,optionBox.x,subRenderYPos,optionBox.x+optionBox.w,subRenderYPos+subRenderHeight,GPE_MAIN_TEMPLATE->PopUp_Box_Color, false);
                        }
                        else
                        {
                            render_rectangle(cRender,optionBox.x,subRenderYPos,optionBox.x+optionWidthSpace,subRenderYPos+subRenderHeight,GPE_MAIN_TEMPLATE->Program_Color, false);
                        }
                        for( i=subMenuStartPos; i<(int)subOptions.size() && i < subMenuStartPos+maxOptionsToView; i++)
                        {
                            fOption = subOptions[i];
                            if( fOption!=NULL)
                            {
                                if( hoverOption == i && hoverOption>=0)
                                {
                                    render_rect(cRender,&fOption->optionBox,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color, false);
                                }
                                fOption->render_self(cRender,cam, true);
                            }
                        }
                        if( isContext)
                        {
                            render_rectangle(cRender,optionBox.x,subRenderYPos,optionBox.x+optionBox.w,subRenderYPos+subRenderHeight,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color, true);
                        }
                        else
                        {
                            render_rectangle(cRender,optionBox.x,subRenderYPos,optionBox.x+subOptionWidthSpace,subRenderYPos+subRenderHeight,GPE_MAIN_TEMPLATE->Main_Border_Color, true);
                        }
                    }
                    else
                    {
                        render_rectangle(cRender,optionBox.x+optionBox.w,subRenderYPos,optionBox.x+optionBox.w+subOptionWidthSpace,subRenderYPos+subRenderHeight,GPE_MAIN_TEMPLATE->PopUp_Box_Color, false);
                        for( i=subMenuStartPos; i<(int)subOptions.size() && i < subMenuStartPos+maxOptionsToView; i++)
                        {
                            fOption = subOptions[i];
                            if( hoverOption == i  && hoverOption>=0)
                            {
                                render_rect(cRender,&fOption->optionBox,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color, false);
                            }
                            fOption->render_self(cRender, cam,true);
                        }
                        render_rectangle(cRender,optionBox.x+optionBox.w,subRenderYPos,optionBox.x+optionBox.w+subOptionWidthSpace,subRenderYPos+subRenderHeight,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color, true);
                        render_rectangle(cRender,optionBox.x,optionBox.y,optionBox.x+optionBox.w,optionBox.y+optionBox.h,GPE_MAIN_TEMPLATE->PopUp_Box_Color, false);
                     }
                }
                if( !isTopOfMenu)
                {
                    render_new_text(cRender,optionBox.x+optionBox.w-GENERAL_GPE_PADDING,optionBox.y+16,">",GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_TOOLBAR,FA_RIGHT,FA_MIDDLE);
                }
            }

            if( isTopOfMenu )
            {
                render_new_text(cRender,optionBox.x+4,optionBox.y+4,opName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_TOP);
            }
            else
            {
                if(subMenuIsOpen  )
                {
                    render_rectangle(cRender,optionBox.x,optionBox.y,optionBox.x+optionBox.w,optionBox.y+optionBox.h,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color, false);
                    render_new_text(cRender,optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING,optionBox.y+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING/2,opName,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_MIDDLE);
                }
                /*else if( )
                {
                    render_new_text(cRender,optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING,optionBox.y+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING/2,opName,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_MIDDLE);

                }*/
                else
                {
                    render_new_text(cRender,optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING,optionBox.y+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING/2,opName,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_MIDDLE);
                    //opTexture->render_tex(cRender,optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING, optionBox.y+16-(opTexture->get_height()/2),NULL,NULL);
                }
                if ( opSprite!=NULL && opSprite->spriteTexture!=NULL )
                {
                    render_sprite_resized(cRender,opSprite,spriteFrameNumber,optionBox.x+GENERAL_GPE_PADDING,optionBox.y+optionBox.h/4,optionBox.h/2,optionBox.h/2);
                }
                else if( opTexture!=NULL)
                {
                    if( isFolderOption)
                    {
                        render_texture_resized(cRender,opTexture,optionBox.x+GENERAL_GPE_PADDING,optionBox.y+optionBox.h/4,optionBox.h/2,optionBox.h/2,NULL,NULL,FA_LEFT,FA_TOP,GPE_MAIN_TEMPLATE->Main_Folder_Color );
                    }
                    else if( isResourceOption || renderWhite)
                    {
                        render_texture_resized(cRender,opTexture,optionBox.x+GENERAL_GPE_PADDING,optionBox.y+optionBox.h/4,optionBox.h/2,optionBox.h/2,NULL,NULL,FA_LEFT,FA_TOP, c_white );
                    }
                    else
                    {
                        render_texture_resized(cRender,opTexture,optionBox.x+GENERAL_GPE_PADDING,optionBox.y+optionBox.h/4,optionBox.h/2,optionBox.h/2,NULL,NULL,FA_LEFT,FA_TOP, GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color  );
                    }
                }
                if( (int)shortcutString.size()>0 )
                {
                    render_new_text(cRender,optionBox.x+optionBox.w-GENERAL_GPE_PADDING,optionBox.y+16,shortcutString,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_TOOLBAR,FA_RIGHT,FA_MIDDLE);
                }
            }
        }
        else
        {
            render_new_text(cRender,optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING,optionBox.y+optionBox.h/2,"null",GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_MIDDLE);
        }
    }
}

void GPE_PopUpMenu_Option::update_selectability( bool selectable )
{
    if( selectable!=isSelectable)
    {
        isSelectable = selectable;
    }
}

GPE_Toolbar::GPE_Toolbar(std::string bName,GPE_Rect bRect)
{
    upDelay = 0;
    downDelay = 0;
    leftDelay = 0;
    rightDelay = 0;
    barBox.x = 0;
    barBox.y = 0;
    barBox.w = SCREEN_WIDTH;
    barBox.h = 24;
    guiListTypeName = "toolbar";
    opName = bName;
    bRect = barBox;
    barWidthTotal = 0;
    hoverOption = -1;
    selectedOption = -1;
    toolBarIsOpen = false;
}

GPE_Toolbar::GPE_Toolbar()
{
    guiListTypeName = "toolbar";
    opName = "untitled";
    barBox.x = 0;
    barBox.y = 0;
    barBox.w = SCREEN_WIDTH;
    barBox.h = 24;

    barWidthTotal = 0;
    hoverOption = -1;
    selectedOption = -1;
    toolBarIsOpen = false;
    upDelay = 0;
    downDelay = 0;
    leftDelay = 0;
    rightDelay = 0;
}

GPE_Toolbar::~GPE_Toolbar()
{

}

void GPE_Toolbar::close_toolbar()
{
    reset_options();
    hoverOption = -1;
    selectedOption = -1;
    toolBarIsOpen = false;
    upDelay = 0;
    downDelay = 0;
    leftDelay = 0;
    rightDelay = 0;
}

bool GPE_Toolbar::is_open()
{
    return toolBarIsOpen;
}

GPE_PopUpMenu_Option * GPE_Toolbar::add_menu_option( std::string name, int id)
{
    GPE_PopUpMenu_Option * newOp = new GPE_PopUpMenu_Option(name,id);
    if( id<0)
    {
        newOp->set_id( (int)barOptions.size() );
    }
    newOp->set_position(barWidthTotal,barBox.y);
    newOp->optionBox.h = barBox.h;
    barOptions.push_back(newOp);
    barWidthTotal+=newOp->get_width()+TOOLKEY_OPTION_PADDING;
    return newOp;
}

void GPE_Toolbar::set_width(int newWid)
{
    barBox.w = newWid;
}

void GPE_Toolbar::reset_options()
{
    GPE_PopUpMenu_Option * gOption = NULL;
    for(int j=0; j<(int)barOptions.size(); j++)
    {
        gOption = barOptions[j];
        gOption->subMenuIsOpen = false;
        gOption->hoverOption = -1;
        gOption->selectedOption = -1;
        gOption->reset_suboptions();
    }
}

void GPE_Toolbar::open_toolbar()
{
    if( toolBarIsOpen==false)
    {
        toolBarIsOpen = true;
        MAIN_OVERLAY->take_frozen_screenshot();
    }
}

void GPE_Toolbar::prerender_self( GPE_Renderer * cRender)
{
    GPE_PopUpMenu_Option * tempOption = NULL;
    for( int i = 0; i < (int)barOptions.size(); i++)
    {
        tempOption = barOptions[i];
        tempOption->prerender_self(cRender);
    }
}

void GPE_Toolbar::process_toolbar()
{
    int i;
    int drawXPos;
    int actionHappened = false;
    int barAction = -1;

    update_popup_info(); //resets the popup option to process
    if( userInput->windowEventHappendInFrame == true)
    {
        hoverOption = -1;
        selectedOption = -1;
        reset_options();
        toolBarIsOpen = false;
    }

    GPE_PopUpMenu_Option * fOption = NULL;
    if( userInput->released[kb_alt] & userInput->down[kb_ctrl]==false && userInput->down[kb_shift]==false )
    {
        if( toolBarIsOpen)
        {
            hoverOption = -1;
            selectedOption = -1;
            reset_options();
            toolBarIsOpen = false;
        }
        else
        {
            reset_options();
            hoverOption = 0;

            selectedOption = 0;
            //resets the bar options
            if( (int)barOptions.size()>0)
            {
                fOption = barOptions[0];
                fOption->open_menu();
            }
            actionHappened = 2;
            open_toolbar();
            /*
            reset_options();
            hoverOption = 0;
            selectedOption = 0;
            toolBarIsOpen = true;
            if( barOptions.size()>0)
            {
                fOption = barOptions[0];
                fOption->subMenuIsOpen = true;
            }
            if( GPE_MAIN_GUI!=NULL )
            {
                //MAIN_OVERLAY->take_frozen_screenshot();
            }
            */

        }
    }


    if( barOptions.size()>0)
    {
        /*
        if( point_between_rect(userInput->mouse_x, userInput->mouse_y,&barBox) )
        {
            if( userInput->check_mouse_released(0) )
            {
                close_toolbar();
            }
        }*/

        drawXPos = barBox.x;
        if( toolBarIsOpen )
        {
            if( userInput->down[kb_up] && userInput->pressed[kb_up]==false && userInput->released[kb_up]==false )
            {
                upDelay+=0.5;
            }
            else
            {
                upDelay = -1;
            }

            if( userInput->down[kb_down] && userInput->pressed[kb_down]==false && userInput->released[kb_down]==false )
            {
                downDelay+=0.5;
            }
            else
            {
                downDelay = -1;
            }

            if( userInput->down[kb_left] && userInput->pressed[kb_left]==false && userInput->released[kb_left]==false )
            {
                leftDelay+=0.5;
            }
            else
            {
                leftDelay = -1;
            }

            if( userInput->down[kb_right] && userInput->pressed[kb_right]==false && userInput->released[kb_right]==false )
            {
                rightDelay+=0.5;
            }
            else
            {
                rightDelay = -1;
            }
        }
        else
        {
            upDelay = -1;
            downDelay = -1;
            leftDelay = -1;
            rightDelay = -1;
        }

        int currentCursorPos = 0;
        if( selectedOption>=0 && selectedOption < (int)barOptions.size() )
        {
            fOption = barOptions[selectedOption];
            currentCursorPos = selectedOption;
        }
        else if( hoverOption>=0 && hoverOption < (int)barOptions.size() )
        {
            fOption = barOptions[hoverOption];
            currentCursorPos = hoverOption;
        }

        bool toolBarActionHappened = false;
        if( toolBarIsOpen )
        {
            if( leftDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !userInput->pressed[kb_left] && userInput->released[kb_left] ) )
            {
                leftDelay = -1;
                toolBarActionHappened = true;
                if( fOption!=NULL)
                {
                    if( fOption->push_left() ==false)
                    {
                        currentCursorPos--;
                        if( currentCursorPos < 0)
                        {
                            currentCursorPos = (int)barOptions.size()-1;
                        }
                        if(currentCursorPos < 0 )
                        {
                            currentCursorPos = 0;
                        }
                        reset_options();
                        hoverOption = selectedOption = currentCursorPos;
                        if( hoverOption>=0 && hoverOption < (int)barOptions.size() )
                        {
                            fOption = barOptions[currentCursorPos];
                            if( fOption!=NULL)
                            {
                                fOption->subMenuIsOpen = true;
                            }
                        }
                    }
                }
            }
            else if( rightDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !userInput->pressed[kb_right] && userInput->released[kb_right] ) )
            {
                toolBarActionHappened = true;
                rightDelay = -1;
                if( fOption!=NULL)
                {
                    if( fOption->push_right() ==false )
                    {
                        currentCursorPos++;
                        if( currentCursorPos >=(int)barOptions.size() )
                        {
                            currentCursorPos = 0;
                        }
                        reset_options();
                        hoverOption = selectedOption = currentCursorPos;
                        if( hoverOption>=0 && hoverOption < (int)barOptions.size() )
                        {
                            fOption = barOptions[currentCursorPos];
                            if( fOption!=NULL)
                            {
                                fOption->subMenuIsOpen = true;
                            }
                        }
                    }
                }
            }
            else if( upDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !userInput->pressed[kb_up] && userInput->released[kb_up] ) )
            {
                toolBarActionHappened = true;
                if( fOption!=NULL)
                {
                    if( fOption->push_up() ==false )
                    {
                    }
                }
                upDelay = -1;
            }
            else if( downDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !userInput->pressed[kb_down] && userInput->released[kb_down] ) )
            {
                toolBarActionHappened = true;
                if( fOption!=NULL)
                {
                    if( fOption->push_down() ==false )
                    {

                    }
                }
                downDelay = -1;
            }
            else if( userInput->released[kb_enter] || userInput->released[kb_space] && toolBarIsOpen  )
            {
                toolBarActionHappened = true;
                if( fOption!=NULL)
                {
                    if( fOption->activate_hovered() )
                    {
                        hoverOption = -1;
                        selectedOption = -1;
                        reset_options();
                        toolBarIsOpen = false;
                        close_toolbar();
                    }
                }
            }
        }

        if( !toolBarActionHappened )
        {
            for(i=0; i<(int)barOptions.size(); i++)
            {
                fOption = barOptions[i];
                if( point_between(userInput->mouse_x, userInput->mouse_y,drawXPos,barBox.y,drawXPos+fOption->get_width()+TOOLKEY_OPTION_PADDING,barBox.y+barBox.h) )
                {
                    if( userInput->mouseMovementInputReceivedInFrame || hoverOption < 0 )
                    {
                        hoverOption = i;
                    }
                    if( selectedOption>=0 && toolBarIsOpen && userInput->mouseMovementInputReceivedInFrame )
                    {
                        reset_options();
                        hoverOption = i;

                        selectedOption = i;
                        //resets the bar options
                        fOption->open_menu();
                        actionHappened = 2;
                        open_toolbar();
                    }
                    else if( userInput->check_mouse_released(0) )
                    {
                        //resets the bar options
                        reset_options();
                        fOption->open_menu();
                        open_toolbar();
                        selectedOption=i;
                        hoverOption = -1;
                        actionHappened = 2;
                    }
                }

                barAction = fOption->process_menu_option();
                if(barAction!=-1)
                {
                    actionHappened = barAction;
                }
                drawXPos+=fOption->get_width()+TOOLKEY_OPTION_PADDING;
            }

            if( userInput->check_mouse_released(0) )
            {
                //resets the toolbar if the user clicks on the far right side of it.
                if(actionHappened==false || actionHappened==1)
                {
                    close_toolbar();
                }
            }
        }
    }

    if( userInput->check_mouse_released(1) || userInput->check_keyboard_released(kb_esc) )
    {
        close_toolbar();
    }

    if((int)GPE_Action_Message.size() > 0 )
    {
        close_toolbar();
        userInput->reset_all_input();
    }
}

void GPE_Toolbar::render_toolbar(GPE_Renderer *rendTarget, GPE_Rect *renderCam, bool forceRedraw)
{
    if( rendTarget==NULL)
    {
        rendTarget = MAIN_RENDERER;
    }

    if( forceRedraw)
    {
        if( toolBarIsOpen)
        {
            MAIN_OVERLAY->render_frozen_screenshot(rendTarget);
        }
        render_rect(rendTarget,&barBox,GPE_MAIN_TEMPLATE->Program_Header_Color,false);
        if( barOptions.size()==0)
        {
            render_new_text(rendTarget,barBox.x+4,barBox.y+8,"Empty Toolbar();",GPE_MAIN_TEMPLATE->Main_Box_Font_Color,NULL,FA_LEFT,FA_TOP);
        }
        else
        {
            GPE_PopUpMenu_Option * fOption = NULL;
            int drawXPos = 0;
            int y2 = barBox.y+barBox.h;
            for(int i=0; i<(int)barOptions.size(); i++)
            {
                fOption = barOptions[i];
                if(fOption->subMenuIsOpen || hoverOption==i)
                {
                    render_rectangle(rendTarget,drawXPos,barBox.y,drawXPos+fOption->get_width()+TOOLKEY_OPTION_PADDING,y2,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color, false);
                }
                fOption->render_self(rendTarget,NULL,true);
                drawXPos+=fOption->get_width()+TOOLKEY_OPTION_PADDING;
            }
        }
    }
}

GPE_DropDown_Menu::GPE_DropDown_Menu(int xPos, int yPos,std::string name, bool justOptions)
{
    guiListTypeName = "dropdown";
    barBox.x = xPos;
    barBox.y = yPos;
    barBox.w = 228;
    //int nameMinSize = name.size()*
    barBox.h = 32;
    dropdownName = opName = name;
    opId = -1;
    selectedId = -1;
    isSelectable = true;
    showJustOptions = justOptions;
    if( showJustOptions)
    {
        opId = 0;
        selectedId = 0;
    }
    isOpen = false;
    isClicked = false;
    justActivated = false;
}

GPE_DropDown_Menu::~GPE_DropDown_Menu()
{
    GPE_KeyPair * kp = NULL;
    for( int i = subOptions.size()-1; i >=0; i--)
    {
        kp = subOptions[i];
        if( kp!=NULL )
        {
            delete kp;
            kp = NULL;
        }
    }
    subOptions.clear();
}

std::string GPE_DropDown_Menu::get_data()
{
    std::string dataString = guiListTypeName+":"+dropdownName+"==|||==[menu]";
    GPE_KeyPair * tPair = NULL;
    for( int i = 0; i < (int)subOptions.size(); i++ )
    {
        tPair = subOptions[i];
        if( tPair!=NULL)
        {
            dataString+="[option]"+tPair->keyString+":"+tPair->keySubString+":"+int_to_string(tPair->keyValue)+"[/option]";
        }
    }
    dataString+="[/menu]"+int_to_string(selectedId)+","+int_to_string(showJustOptions)+",";
    return dataString;
}

void GPE_DropDown_Menu::load_data(std::string dataString)
{
    if( (int)dataString.size() > 0 )
    {
        std::string optionTag = "[option]";
        int optionTagSize = (int)optionTag.size();

        std::string allOptionsString = "";
        std::string newOptionData = "";
        std::string newOptionString = "";
        std::string newOptionName = "";
        std::string newOptionSubame = "";
        int newOptionId = -1;
        int beginOptionPos=dataString.find_first_of(optionTag);
        if(beginOptionPos!=(int)std::string::npos)
        {
            allOptionsString = split_first_string(dataString,"[/menu]");

            newOptionData = split_first_string(allOptionsString,"[menu]");
            while( (int)allOptionsString.size() > 0)
            {
                newOptionString = split_first_string(allOptionsString,"[/option]");

                beginOptionPos=newOptionString.find_first_of(optionTag);
                if(beginOptionPos!=(int)std::string::npos)
                {
                    //if the beginOptionPos is present, then parse on through and carryon
                    if( (int)newOptionString.size() > beginOptionPos+optionTagSize )
                    {
                        newOptionData = get_substring(newOptionString, beginOptionPos+optionTagSize );

                        newOptionName = split_first_string(newOptionData,":");
                        newOptionSubame = split_first_string(newOptionData,":");
                        newOptionId = string_to_int(newOptionData,-1);
                        if( (int)newOptionName.size() > 0)
                        {
                            add_menu_option(newOptionName,newOptionSubame,newOptionId,false);
                        }
                    }
                    else
                    {
                        return;
                    }
                }
            }
            set_id( split_first_int(dataString,',') );
            showJustOptions = is_bool( split_first_string(dataString,",") );
        }
    }
}

void GPE_DropDown_Menu::remove_data(std::string dataString)
{
    if( (int)dataString.size() > 0)
    {
        std::string optionTag = "[option]";
        int optionTagSize = (int)optionTag.size();

        std::string newOptionData = "";
        std::string newOptionString = "";
        std::string foundOptionName = "";
        int beginOptionPos = -1;
        while( (int)dataString.size() > 0)
        {
            newOptionString = split_first_string(dataString,"[/option]");

            beginOptionPos=newOptionString.find_first_of(optionTag);
            if(beginOptionPos!=(int)std::string::npos)
            {
                //if the beginOptionPos is present, then parse on through and carryon
                foundOptionName = newOptionString.substr(beginOptionPos+optionTagSize,newOptionString.length());
                if( (int)foundOptionName.size() > 0)
                {
                    remove_option(foundOptionName);
                }
            }
        }
    }
}

void GPE_DropDown_Menu::remove_option(std::string optionToRemove)
{
    GPE_KeyPair * tOption = NULL;
    for( int i = (int)subOptions.size()-1; i>=0; i--)
    {
        tOption = subOptions[i];
        if( tOption!=NULL )
        {
            if( tOption->keyString==optionToRemove)
            {
                delete tOption;
                tOption = NULL;
                subOptions.erase( subOptions.begin()+i);

                if( selectedId==i)
                {
                    set_selection(-1);
                }
            }
        }
    }
}

void GPE_DropDown_Menu::add_menu_option(std::string optionName, std::string optionSubStr,double optionValue, bool selectOption)
{
    bool optionExists = false;
    GPE_KeyPair * tOption = NULL;
    for( int i = 0; i < (int)subOptions.size(); i++)
    {
        tOption = subOptions[i];
        if( tOption!=NULL )
        {
            if( tOption->keyString==optionName)
            {
                optionExists = false;
                return;
            }
        }
    }
    if( !optionExists)
    {
        GPE_KeyPair * kp = new GPE_KeyPair(optionValue,optionName,optionSubStr);
        subOptions.push_back(kp);
        if( selectOption )
        {
            set_id( (int)subOptions.size()-1 );
        }
    }
}

std::string GPE_DropDown_Menu::get_menu_option(int atNumb)
{
    GPE_KeyPair * kp = NULL;
    if( atNumb >=0 && atNumb < (int)subOptions.size() )
    {
        kp = subOptions.at(atNumb);
        return kp->keyString;
    }
    return "";
}

std::string GPE_DropDown_Menu::get_plain_string()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return "'"+kp->keyString+"'";
    }
    return "''";
}

int GPE_DropDown_Menu::get_selected_id()
{
    return selectedId;
}

std::string GPE_DropDown_Menu::get_selected_name()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keyString;
    }
    return opName;
}


std::string GPE_DropDown_Menu::get_selected_tag()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keySubString;
    }
    return "";
}

double GPE_DropDown_Menu::get_selected_value()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keyValue;
    }
    return 0;
}

bool GPE_DropDown_Menu::just_activated()
{
    return justActivated;
}

void GPE_DropDown_Menu::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    isHovered = false;
    justActivated = false;
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered || isInUse)
    {
        MAIN_OVERLAY->update_tooltip(dropdownName);
    }
    if( isInUse)
    {
        hasArrowkeyControl = true;
    }
    else
    {
        hasArrowkeyControl = false;
    }
    if( isInUse &&( userInput->check_keyboard_down( kb_enter ) || userInput->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }

    if( isClicked && cam!=NULL && viewedSpace!=NULL)
    {
        if( MAIN_CONTEXT_MENU!=NULL)
        {
            if( MAIN_CONTEXT_MENU->subMenuIsOpen == false)
            {
                isOpen = true;
                GPE_open_context_menu(viewedSpace->x+barBox.x-cam->x, viewedSpace->y+barBox.y+barBox.h-cam->y);
                MAIN_CONTEXT_MENU->set_width(barBox.w);
                GPE_KeyPair * kp = NULL;
                if( (int)subOptions.size() > 0)
                {
                    if( showJustOptions ==false)
                    {
                        MAIN_CONTEXT_MENU->add_menu_option(dropdownName,-1);
                    }
                    for( int i = 0; i < (int)subOptions.size(); i++)
                    {
                        kp = subOptions[i];
                        MAIN_CONTEXT_MENU->add_menu_option(kp->keyString,i);
                    }
                    if( showJustOptions ==false)
                    {
                        MAIN_CONTEXT_MENU->add_menu_option(dropdownName,-1);
                    }
                }
                else
                {
                    MAIN_CONTEXT_MENU->add_menu_option(dropdownName,-1);
                }
                isOpen = false;
                isClicked = false;
                justActivated = true;
                int foundResult = get_popupmenu_result();
                if( foundResult>=0)
                {
                    selectedId = foundResult;
                    opName = GPE_Action_Message;
                }
                else if(showJustOptions==false)
                {
                    selectedId=-1;
                    opName=dropdownName;
                }
                GPE_close_context_menu();
            }
        }
    }
    else if( hasArrowkeyControl)
    {
        if( userInput->check_keyboard_down(kb_up) && selectedId > 0)
        {
            selectedId--;
        }
        else if( userInput->check_keyboard_down(kb_down) && selectedId < (int)subOptions.size()-1)
        {
            selectedId++;
        }
    }
}

void GPE_DropDown_Menu::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && cam!=NULL && viewedSpace!=NULL)
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Input_Color,false);

        if( selectedId >= 0)
        {
            if( selectedId < (int)subOptions.size() )
            {
                GPE_KeyPair * kp = subOptions[selectedId];
                render_new_text_ext(cRender,barBox.x+GENERAL_GPE_PADDING-cam->x,barBox.y+GENERAL_GPE_PADDING-cam->y,kp->keyString,GPE_MAIN_TEMPLATE->Input_Font_Color,FONT_POPUP,FA_LEFT,FA_TOP,barBox.w-barBox.h-12,-1);
            }
            else
            {
                render_new_text_ext(cRender,barBox.x+GENERAL_GPE_PADDING-cam->x,barBox.y+GENERAL_GPE_PADDING-cam->y,opName,GPE_MAIN_TEMPLATE->Input_Font_Color,FONT_POPUP,FA_LEFT,FA_TOP,barBox.w-barBox.h-12,-1);
                selectedId = 0;
            }
        }
        else
        {
            render_new_text_ext(cRender,barBox.x+GENERAL_GPE_PADDING-cam->x,barBox.y+GENERAL_GPE_PADDING-cam->y,opName,GPE_MAIN_TEMPLATE->Input_Font_Color,FONT_POPUP,FA_LEFT,FA_TOP,barBox.w-barBox.h-12,-1);
        }
        if( isInUse)
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Input_Highlight_Outline_Color,true);
        }
        else if( isHovered)
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Input_Highlight_Alt_Color,true);
        }
        else
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Input_Outline_Color,true);
        }

        render_rectangle(cRender,barBox.x+barBox.w-barBox.h*3/4-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Input_Outline_Color ,false);
        render_sprite_ext(cRender,GPE_TRIANGLE,3,barBox.x+barBox.w-barBox.h/2-cam->x,barBox.y+barBox.h/3 - cam->y,barBox.h/3,barBox.h/3,GPE_MAIN_TEMPLATE->Input_Color );
    }
}

void GPE_DropDown_Menu::set_id(int newId)
{
    set_selection(newId);
}

void GPE_DropDown_Menu::set_selection(int newId, bool autoCorrect)
{
    if( newId>=0)
    {
        if( newId < (int)subOptions.size() )
        {
            GPE_KeyPair * kp = subOptions.at(newId);
            selectedId = newId;
            opName = kp->keyString;
        }
        else if( autoCorrect && (int)subOptions.size() >0 )
        {
            selectedId = (int)subOptions.size()-1;
            GPE_KeyPair * kp = subOptions.at(selectedId);
            opName = kp->keyString;
        }
        else
        {
            selectedId = -1;
            opName = dropdownName;
        }
    }
    else if( autoCorrect && (int)subOptions.size() >0 )
    {
        selectedId = 0;
        GPE_KeyPair * kp = subOptions.at(selectedId);
        opName = kp->keyString;
    }
    else
    {
        selectedId = -1;
        opName = dropdownName;
    }
}

void GPE_DropDown_Menu::set_name(std::string newName)
{
    dropdownName = opName = newName;
}

void GPE_DropDown_Menu::set_value(int valueToFind )
{
    GPE_KeyPair * kp = NULL;
    for( int i =(int)subOptions.size()-1; i>=0; i--)
    {
        kp = subOptions[i];
        if( kp!=NULL)
        {
            if( kp->keyValue==valueToFind )
            {
                selectedId = i;
                opName = kp->keyString;
                break;
            }
        }
    }
}

void GPE_DropDown_Menu::show_just_options(bool justOptions)
{
    showJustOptions = justOptions;
}

GPE_ToolIconButton::GPE_ToolIconButton(int buttonX, int buttonY,std::string buttonImgFile,std::string name, int id, int buttonSize, bool lastInCol)
{
    guiListTypeName = "iconbutton";
    endsSection = lastInCol;
    opName = name;
    descriptionText = name;
    opId = id;
    buttonTexture = rsm->texture_add(buttonImgFile);
    barBox.x = buttonX;
    barBox.y = buttonY;
    barBox.w = buttonSize;
    barBox.h = buttonSize;
    isTabbed = false;
    usesTabs = false;
}

GPE_ToolIconButton::~GPE_ToolIconButton()
{

}

std::string GPE_ToolIconButton::get_data()
{
    if( buttonTexture!=NULL)
    {
        return guiListTypeName+":"+opName+"==|||=="+int_to_string(barBox.w)+","+buttonTexture->get_filename()+",,,"+int_to_string(barBox.w)+","+int_to_string(barBox.h)+","+int_to_string(opId);
    }
    else
    {
        return guiListTypeName+":"+opName+"==|||=="+int_to_string(barBox.w)+",0,,,0,0,"+int_to_string(opId);
    }
}

void GPE_ToolIconButton::load_data(std::string dataString)
{
    int buttonSize = split_first_int(dataString,',');
    if( buttonSize <=0)
    {
        barBox.w = buttonSize;
        barBox.h = buttonSize;
    }
    std::string textureFileName = split_first_string(dataString,",,,");
    int spriteWidth = split_first_int(dataString,',');
    int spriteHeight = split_first_int(dataString,',');
    barBox.w = spriteWidth;
    barBox.h = spriteHeight;
    opId = split_first_int(dataString,',');
    buttonTexture = rsm->texture_add(textureFileName);
}

bool GPE_ToolIconButton::ends_section()
{
    return endsSection;
}

int GPE_ToolIconButton::get_id()
{
    return opId;
}

void GPE_ToolIconButton::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    isClicked = false;
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isInUse &&( userInput->check_keyboard_down( kb_enter ) || userInput->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }
    else if( clickedOutside)
    {
        isClicked = false;
    }
    if( isClicked && usesTabs )
    {
        isTabbed = true;
    }
}

void GPE_ToolIconButton::set_id(int newId)
{
    opId = newId;
}

void GPE_ToolIconButton::set_image( std::string buttonImgFile)
{
    if( buttonTexture!=NULL)
    {
        buttonTexture->load_new_texture(MAIN_RENDERER,buttonImgFile,-1,true);
    }
    else
    {
        buttonTexture = rsm->texture_add(buttonImgFile);
    }
}

void GPE_ToolIconButton::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cRender==NULL)
    {
        cRender = MAIN_RENDERER;
    }
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( forceRedraw && cam!=NULL && viewedSpace!=NULL)
    {
        if( buttonTexture!=NULL)
        {
            GPE_Color * renderColor = GPE_MAIN_TEMPLATE->Icon_Font_Color;
            if( isInUse)
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Icon_Box_Highlighted_Color,false);
                renderColor = GPE_MAIN_TEMPLATE->Icon_Font_Highlighted_Color;
            }
            else if(isHovered)
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Icon_Box_Highlighted_Color,false);
                renderColor = GPE_MAIN_TEMPLATE->Icon_Font_Highlighted_Color;
            }
            else if(isTabbed)
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Icon_Box_Selected_Color,false);
                renderColor = GPE_MAIN_TEMPLATE->Icon_Font_Selected_Color;
            }
            render_texture_resized(cRender,buttonTexture,barBox.x-cam->x,barBox.y-cam->y,barBox.w,barBox.h,NULL,NULL,FA_LEFT,FA_TOP,renderColor);
        }
        if( endsSection)
        {
            render_rectangle(cRender,barBox.x+barBox.w-cam->x,barBox.y-cam->y,barBox.x+barBox.w+4-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Icon_Font_Color);
        }
    }
}

GPE_ToolIconButtonBar::GPE_ToolIconButtonBar(int xPos, int yPos,int buttonSize,bool useTabs)
{
    guiListTypeName = "iconbuttonbar";
    if( buttonSize<=0)
    {
        buttonSize = 24;
    }

    hoverOption = 0;
    barBox.x = xPos;
    barBox.y = yPos;
    barBox.w = 32;
    barBox.h = buttonSize;
    barPadding = 8;
    xPadding = 4;
    newButtonXPos = barPadding;
    isTabBar = useTabs;
    tabPosition = 0;
    widthAutoResizes = false;
}

GPE_ToolIconButtonBar::~GPE_ToolIconButtonBar()
{
    for( int i = barOptions.size()-1; i >=0; i--)
    {
        if( barOptions[i]!=NULL)
        {
            delete barOptions[i];
            barOptions[i] = NULL;
            barOptions.erase(barOptions.begin()+i);
        }
    }
    barOptions.clear();
}

std::string GPE_ToolIconButtonBar::get_data()
{
    return "";
}

void GPE_ToolIconButtonBar::load_data(std::string dataString)
{

}

void GPE_ToolIconButtonBar::limit_width( bool isLimited)
{
    widthIsLimited = isLimited;
    if( widthIsLimited)
    {
        barBox.w = xPadding*3+( (int)barOptions.size() )*(xPadding+barBox.h);
    }
}

void GPE_ToolIconButtonBar::set_width(int newWid)
{
    barBox.w = newWid;
}

GPE_ToolIconButton * GPE_ToolIconButtonBar::add_option( std::string buttonImgFile,std::string name, int id, bool endsSection)
{
    int barSize = (int)barOptions.size();
    GPE_ToolIconButton * newOp = new GPE_ToolIconButton( barBox.x+newButtonXPos, barBox.y,buttonImgFile,name,id,barBox.h, endsSection);
    newOp->descriptionText =  name;
    newOp->usesTabs = isTabBar;
    if( isTabBar && barSize==0)
    {
        newOp->isTabbed = true;
        tabPosition = 0;
    }
    if( id<0 )
    {
        newOp->set_id( barSize );
    }
    barOptions.push_back(newOp);
    newButtonXPos+= xPadding+barBox.h;
    if( endsSection)
    {
        newButtonXPos+=xPadding;
    }
    if( widthAutoResizes)
    {
        barBox.w = barPadding*2+(xPadding+barBox.h)*( (int)barOptions.size() );
    }
    return newOp;
}


int GPE_ToolIconButtonBar::get_tab_pos()
{
    return tabPosition;
}

int GPE_ToolIconButtonBar::get_tab_id()
{
    if( tabPosition >=0 && tabPosition < (int)barOptions.size() )
    {
        if( barOptions[tabPosition]!=NULL)
        {
            return barOptions[tabPosition]->get_id();
        }
    }
    return 0;
}

void GPE_ToolIconButtonBar::process_self(GPE_Rect * viewedSpace , GPE_Rect *cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    GPE_ToolIconButton * cButton = NULL;
    GPE_ToolIconButton * dButton = NULL;
    int i=0;
    int j=0;

    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isTabBar)
    {
        if( isClicked)
        {
            hasArrowkeyControl = true;
        }
        else if( clickedOutside)
        {
            hasArrowkeyControl = false;
        }
        else if( isInUse)
        {
            hasArrowkeyControl = true;
        }
        else
        {
            hasArrowkeyControl = false;
        }
    }
    else
    {
        selectedOption = -1;
        hasArrowkeyControl = false;
    }
    for(i=0; i<(int)barOptions.size(); i++)
    {
        cButton = barOptions[i];
        if(cButton!=NULL)
        {
            if( !isTabBar)
            {
                cButton->set_clicked(false);
                cButton->switch_inuse(false);
            }
            cButton->process_self(viewedSpace, cam);
            if( cButton->is_clicked()  )
            {
                selectedOption = cButton->get_id();
                if( isTabBar)
                {
                    tabPosition = i;
                    for( j=0; j<(int)barOptions.size(); j++)
                    {
                        dButton = barOptions[j];
                        if(dButton!=NULL)
                        {
                            dButton->isTabbed = false;
                        }
                    }
                    cButton->isTabbed = true;
                }
            }
            else if( cButton->is_hovered() )
            {
                MAIN_OVERLAY->update_tooltip( cButton->get_name() );
            }
        }
    }
    if( isTabBar && hasArrowkeyControl)
    {
        bool tabMoved = false;
        if( tabPosition > 0 && (userInput->check_keyboard_pressed(kb_left) || userInput->check_keyboard_pressed(kb_up) ) )
        {
            tabPosition--;
            tabMoved = true;
        }
        else if(userInput->check_keyboard_pressed(kb_right) || userInput->check_keyboard_pressed(kb_down) )
        {
            if( tabPosition < (int)barOptions.size()-1 )
            {
                tabPosition++;
                tabMoved = true;
            }
        }
        if( tabMoved )
        {
            for( i=0; i<(int)barOptions.size(); i++)
            {
                cButton = barOptions[i];
                if(cButton!=NULL)
                {
                    if(i==tabPosition)
                    {
                        cButton->isTabbed =  true;
                        selectedOption = cButton->get_id();
                    }
                    else
                    {
                        cButton->isTabbed =  false;
                        cButton->set_clicked( false );
                        cButton->switch_inuse( false );
                    }
                }
            }
        }
    }
}

void GPE_ToolIconButtonBar::render_self(GPE_Renderer *cRender,GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cRender==NULL)
    {
        cRender = MAIN_RENDERER;
    }
    if( forceRedraw && barBox.h > 0)
    {
        //render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false);
        GPE_ToolIconButton * cButton = NULL;
        for(int i=0; i<(int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if(cButton!=NULL)
            {
                cButton->render_self(cRender,viewedSpace, cam,forceRedraw);
            }
        }
        if( isInUse)
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color,true);
        }
        //render_rectangle(rendTarget,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
    }
}

void GPE_ToolIconButtonBar::set_coords(int newX, int newY)
{
    int pastX = barBox.x;
    int pastY = barBox.y;
    GPE_GeneralGuiElement::set_coords(newX,newY);
    if( barBox.x!=pastX || barBox.y!=pastY)
    {
        GPE_ToolIconButton * cButton = NULL;
        int xNewPos = barBox.x+barPadding;
        for( int i = 0; i < (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if( cButton!=NULL)
            {
                cButton->set_coords(xNewPos, barBox.y);
                cButton->set_height( barBox.h);
                xNewPos+=xPadding+barBox.h;
                if( cButton->ends_section() )
                {
                    xNewPos+=xPadding;
                }
            }
        }
    }
    if( widthAutoResizes)
    {
        barBox.w = barPadding*2+(xPadding+barBox.h)*( (int)barOptions.size() );
    }
}


void GPE_ToolIconButtonBar::set_height(int newHeight)
{
    int pastH = barBox.h;
    if( barBox.h!=newHeight)
    {
        barBox.h = newHeight;
        newButtonXPos = xPadding;
        GPE_ToolIconButton * cButton = NULL;
        for( int i = 0; i < (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if( cButton!=NULL)
            {
                cButton->set_coords(barBox.x+newButtonXPos,barBox.y);
                cButton->set_width(newHeight);
                cButton->set_height(newHeight);
                newButtonXPos+=barBox.h;
                if( cButton->ends_section() )
                {
                    newButtonXPos+=xPadding;
                }
            }
        }
        if( widthAutoResizes)
        {
            barBox.w = newButtonXPos;
        }
    }
}

void GPE_ToolIconButtonBar::set_selection(int newSelection)
{
    GPE_ToolIconButton * cButton= NULL;
    for(int i=0; i<(int)barOptions.size(); i++)
    {
        cButton = barOptions[i];
        if(cButton!=NULL)
        {
            if( cButton->get_id()==newSelection)
            {
                tabPosition = i;
                cButton->switch_inuse( true );
            }
            else
            {
                cButton->switch_inuse( false );
            }
        }
    }
}

GPE_ToolLabelButton::GPE_ToolLabelButton(int buttonX, int buttonY,std::string name, std::string description,int id, int buttonSize)
{
    guiListTypeName = "labelbutton";
    opName = name;
    descriptionText = description;
    opId = -1;

    barBox.x = buttonX;
    barBox.y = buttonY;
    barBox.w = 228;
    barBox.h = buttonSize;
    if( DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(name,&textW, &textH);
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_POPUP);
        if( textH > barBox.h)
        {
            barBox.h=textH+GENERAL_GPE_PADDING*2;
        }
    }
    isEnabled = true;
}

GPE_ToolLabelButton::~GPE_ToolLabelButton()
{

}



void GPE_ToolLabelButton::prerender_self(GPE_Renderer * cRender)
{

    if( DEFAULT_FONT!=NULL)
    {
        /*
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        //barBox.w=textW+GENERAL_GPE_PADDING*2;
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_POPUP);
        */
    }
}

void GPE_ToolLabelButton::process_self(GPE_Rect * viewedSpace, GPE_Rect  * cam )
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isInUse &&( userInput->check_keyboard_down( kb_enter ) || userInput->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }
}

void GPE_ToolLabelButton::render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace, GPE_Rect  * cam,bool forceRedraw )
{
    if( forceRedraw && isEnabled)
    {
        cam = GPE_find_camera(cam);
        viewedSpace = GPE_find_camera(viewedSpace);
        if(cam!=NULL && viewedSpace!=NULL )
        {
            if( isClicked)
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Button_Box_Selected_Color,false);
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Button_Border_Selected_Color,true);
            }
            else if( isHovered)
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Button_Box_Highlighted_Color,false);
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Button_Border_Highlighted_Color,true);
            }
            else
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Button_Box_Color,false);
            }
            if(DEFAULT_FONT!=NULL)
            {
                if( isHovered)
                {
                    render_new_text(cRender,barBox.x-cam->x+barBox.w/2, barBox.y-cam->y+barBox.h/2,opName,GPE_MAIN_TEMPLATE->Button_Font_Highlighted_Color,DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
                }
                else
                {
                    render_new_text(cRender,barBox.x-cam->x+barBox.w/2, barBox.y-cam->y+barBox.h/2,opName,GPE_MAIN_TEMPLATE->Button_Font_Color,DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
                }
                /*if( barBox.w > opStringTexture->get_width()+GENERAL_GPE_PADDING*2 )
                {
                    opStringTexture->render_tex(cRender,barBox.x-cam->x+(barBox.w-opStringTexture->get_width() )/2,barBox.y-cam->y+GENERAL_GPE_PADDING,NULL,NULL);
                }
                else
                {
                    opStringTexture->render_tex(cRender,barBox.x-cam->x+GENERAL_GPE_PADDING,barBox.y-cam->y+GENERAL_GPE_PADDING,NULL,NULL);
                }*/
            }
            /*
            if( isInUse)
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Button_Border_Highlighted_Color,true);
            }
            else if( isHovered)
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Button_Border_Highlighted_Color,true);
            }
            else
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
            }
            */

            if( isInUse )
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Main_Border_Highlighted_Color,true);
            }
        }
    }
}


void GPE_ToolLabelButton::set_name(std::string newName)
{
    if( DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(newName,&textW, &textH);
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,DEFAULT_FONT);
        barBox.w=textW+GENERAL_GPE_PADDING*2;
        barBox.h=textH+GENERAL_GPE_PADDING*2;
    }
    opName = newName;
}

///
GPE_ToolPushButton::GPE_ToolPushButton(int buttonX, int buttonY,std::string imgLocation,std::string name, std::string description,int id, int buttonSize)
{
    guiListTypeName = "pushbutton";
    if( buttonSize <=0)
    {
        buttonSize = 16;
    }
    opName = name;
    descriptionText = description;
    opId = -1;
    opTexture = rsm->texture_add(imgLocation);
    iconPadding = 8;
    barBox.x = buttonX;
    barBox.y = buttonY;
    barBox.w = 228;
    if(buttonSize <=0)
    {
        barBox.h = 32;
    }
    else
    {
        barBox.h = buttonSize;
    }
    if( DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(name,&textW, &textH);
        if( textW + iconPadding*2 +barBox.h > barBox.w)
        {
            barBox.w = textW+iconPadding*2 + barBox.h;
        }
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,DEFAULT_FONT);
    }
    isEnabled = true;
    wasClicked = false;
    webUrl = "";
    showBackground = true;
    showBorder = true;
    prerender_self(MAIN_RENDERER);
}

GPE_ToolPushButton::~GPE_ToolPushButton()
{

}

void GPE_ToolPushButton::change_texture( GPE_Texture * newTexture)
{
    opTexture = newTexture;
}

void GPE_ToolPushButton::enable_background( bool enableValue)
{
    if( showBackground!=enableValue)
    {
        showBackground = enableValue;
        prerender_self(MAIN_RENDERER);
    }
}

void GPE_ToolPushButton::enable_border( bool enableValue)
{
    if( showBorder!=enableValue)
    {
        showBorder=enableValue;
        prerender_self(MAIN_RENDERER);
    }
}

void GPE_ToolPushButton::prerender_self(GPE_Renderer * cRender)
{
    /*
    int prevWidth = 0;

    if( DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        barBox.w = iconPadding*3+barBox.h+textW;
    }
    */
}

void GPE_ToolPushButton::process_self(GPE_Rect * viewedSpace, GPE_Rect  * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(viewedSpace!=NULL && cam!=NULL)
    {
        GPE_GeneralGuiElement::process_self(viewedSpace,cam);
        if( isHovered)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
        }
        if( isInUse &&( userInput->check_keyboard_down( kb_enter ) || userInput->check_keyboard_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked && (int)webUrl.size() > 3)
        {
             GPE_OpenURL(webUrl);
        }
    }
}

void GPE_ToolPushButton::render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace, GPE_Rect  * cam,bool forceRedraw )
{
    if( forceRedraw && isEnabled)
    {
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if(viewedSpace!=NULL && cam!=NULL)
        {
            if( isClicked )
            {
                if( showBackground)
                {
                    render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Selected_Color,false);
                }
                //if( showBorder)
                {
                    render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Highlighted_Color,true);
                }
            }
            else
            {
                if( showBackground)
                {
                    if( isHovered)
                    {
                        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Highlighted_Color,false);
                        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Button_Border_Highlighted_Color,true);
                    }
                    else
                    {
                        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Color,false);
                    }
                }
                else if( isHovered)
                {
                    render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
                }
            }
            int iconSize = 0;
            if( showBackground)
            {
                iconSize = std::min(barBox.w,barBox.h);
                iconPadding = 0;
            }
            else
            {
                iconSize = std::min(barBox.w,barBox.h);
                iconPadding = 0;
            }
            GPE_Color * renderColor = GPE_MAIN_TEMPLATE->Button_Font_Color;
            if( isHovered)
            {
                renderColor = GPE_MAIN_TEMPLATE->Button_Font_Highlighted_Color;
            }
            if( showBackground==false)
            {
                if( wasClicked)
                {
                    renderColor = GPE_MAIN_TEMPLATE->Main_Box_Faded_Font_Color;
                }
                else
                {
                    renderColor = GPE_MAIN_TEMPLATE->Main_Box_Faded_Font_Color;
                }
            }
            if( opTexture!=NULL)
            {
                render_texture_resized(cRender,opTexture,barBox.x-cam->x+iconPadding*2, barBox.y-cam->y,iconSize,iconSize,NULL,NULL,FA_LEFT,FA_TOP,renderColor);
                if( DEFAULT_FONT!=NULL)
                {
                    render_new_text(cRender,barBox.x-cam->x+iconSize+iconPadding*2, barBox.y-cam->y+barBox.h/2,opName,renderColor,DEFAULT_FONT,FA_LEFT,FA_MIDDLE);
                }
            }
            else if(DEFAULT_FONT!=NULL)
            {
                render_new_text(cRender,barBox.x-cam->x+iconSize+iconPadding*2, barBox.y-cam->y+barBox.h/2,opName,renderColor,DEFAULT_FONT,FA_LEFT,FA_MIDDLE);
            }

            if( isInUse )
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Main_Border_Highlighted_Color,true);
            }
        }
    }
}

void GPE_ToolPushButton::set_name(std::string newName)
{
    opName = newName;
    prerender_self(MAIN_RENDERER);
}

void GPE_ToolPushButton::set_website(std::string urlIn)
{
    webUrl = urlIn;
    wasClicked = false;
    prerender_self(MAIN_RENDERER);
}

///
GPE_Log_Entry::GPE_Log_Entry(std::string projectName, std::string resName, std::string logTxt, std::string funcName, int lineNumb, int charNumb)
{
    projectLogBelongsTo = projectName;
    resourceNameRelated = resName;
    functionName = funcName; //where applicable
    logText = logTxt;
    lineToReach = lineNumb;
    characterToReach = charNumb;
}


GPE_Log_Entry::~GPE_Log_Entry()
{
    projectLogBelongsTo = "";
    resourceNameRelated = "";
    functionName = ""; //where applicable
    logText = "";
}



GPE_ToolPushButtonMultiLine::GPE_ToolPushButtonMultiLine(int buttonX, int buttonY, std::string imgLocation, std::string parsedLines, std::string strValue, int id, int buttonSize)
{
    int ww = 0;
    lineHeight = 0;
    if(DEFAULT_FONT !=NULL)
    {
        DEFAULT_FONT->get_numbered_metrics("A",&ww,&lineHeight);
    }
    guiListTypeName = "multilinepushbutton";
    if( buttonSize <=0)
    {
        buttonSize = 16;
    }
    opName = parsedLines;

    if( (int)parsedLines.size() > 0 )
    {
        int newLinePos=parsedLines.find_first_of("\n");
        if(newLinePos!=(int)std::string::npos)
        {
            std::string cbNLString = split_first_string(parsedLines,'\n');
            buttonLines.push_back(cbNLString);
            newLinePos=parsedLines.find_first_of('\n');
            while (newLinePos!=(int)std::string::npos)
            {
                cbNLString = split_first_string(parsedLines,'\n');
                newLinePos=parsedLines.find_first_of('\n');
                buttonLines.push_back(cbNLString);
            }
            if( (int)parsedLines.size() > 0 )
            {
                buttonLines.push_back(parsedLines);
            }
        }
        else
        {
            buttonLines.push_back(parsedLines);
        }
    }

    descriptionText = strValue;
    opId = -1;
    opTexture = rsm->texture_add(imgLocation);
    iconPadding = 8;
    barBox.x = buttonX;
    barBox.y = buttonY;
    barBox.w = 228;

    barBox.h = GENERAL_GPE_PADDING+( (int)buttonLines.size() *(lineHeight+GENERAL_GPE_PADDING) );

    isEnabled = true;
    wasClicked = false;
    webUrl = "";
    showBackground = true;
    showBorder = true;
    prerender_self(MAIN_RENDERER);
}

GPE_ToolPushButtonMultiLine::~GPE_ToolPushButtonMultiLine()
{

}

void GPE_ToolPushButtonMultiLine::change_texture( GPE_Texture * newTexture)
{
    opTexture = newTexture;
}

void GPE_ToolPushButtonMultiLine::enable_background( bool enableValue)
{
    if( showBackground!=enableValue)
    {
        showBackground = enableValue;
        prerender_self(MAIN_RENDERER);
    }
}

void GPE_ToolPushButtonMultiLine::enable_border( bool enableValue)
{
    if( showBorder!=enableValue)
    {
        showBorder=enableValue;
        prerender_self(MAIN_RENDERER);
    }
}

void GPE_ToolPushButtonMultiLine::prerender_self(GPE_Renderer * cRender)
{
    /*
    int prevWidth = 0;

    if( DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        barBox.w = iconPadding*3+barBox.h+textW;
    }
    */
}

void GPE_ToolPushButtonMultiLine::process_self(GPE_Rect * viewedSpace, GPE_Rect  * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(viewedSpace!=NULL && cam!=NULL)
    {
        GPE_GeneralGuiElement::process_self(viewedSpace,cam);
        if( isHovered)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
        }
        if( isInUse &&( userInput->check_keyboard_down( kb_enter ) || userInput->check_keyboard_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked && (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
}

void GPE_ToolPushButtonMultiLine::render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace, GPE_Rect  * cam,bool forceRedraw )
{
    if( forceRedraw && isEnabled)
    {
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);

        if(viewedSpace!=NULL && cam!=NULL)
        {
            GPE_Color * textRenderColor = GPE_MAIN_TEMPLATE->Button_Font_Color;
            GPE_Color * bgRenderColor = GPE_MAIN_TEMPLATE->Button_Box_Color;
            GPE_Color * textureRenderColor = c_white;
            if( isHovered || isInUse)
            {
                textRenderColor = GPE_MAIN_TEMPLATE->Button_Font_Highlighted_Color;
            }
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w+GENERAL_GPE_PADDING/2,barBox.y-cam->y+barBox.h+GENERAL_GPE_PADDING/2,c_black,false,64);

            if( showBackground)
            {
                if( isClicked )
                {
                   bgRenderColor = GPE_MAIN_TEMPLATE->Button_Box_Selected_Color;
                   textRenderColor= GPE_MAIN_TEMPLATE->Button_Font_Highlighted_Color;
                }
                else if( isHovered)
                {
                    bgRenderColor = GPE_MAIN_TEMPLATE->Button_Box_Highlighted_Color;
                    textRenderColor= GPE_MAIN_TEMPLATE->Button_Font_Highlighted_Color;
                }
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,bgRenderColor,false);
            }
            else
            {
                textRenderColor= GPE_MAIN_TEMPLATE->Button_Box_Color;
                if( isClicked )
                {
                   textRenderColor= GPE_MAIN_TEMPLATE->Button_Box_Selected_Color;
                }
                else if( isHovered)
                {
                    textRenderColor= GPE_MAIN_TEMPLATE->Button_Box_Highlighted_Color;
                }
            }
            if(  isInUse )
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Button_Border_Selected_Color,true);
            }
            else if(isHovered)
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Button_Border_Highlighted_Color,true);
            }
            int iconSize = 0;
            if( showBackground)
            {
                iconSize = std::min(barBox.w,barBox.h);
                iconPadding = 0;
            }
            else
            {
                iconSize = std::min(barBox.w,barBox.h);
                iconPadding = 0;
            }
            if( opTexture!=NULL)
            {

                /*if( showBackground==false)
                {
                    if( wasClicked)
                    {
                        renderColor = GPE_MAIN_TEMPLATE->Main_Box_Faded_Font_Color;
                    }
                    else
                    {
                        renderColor = GPE_MAIN_TEMPLATE->Main_Box_Faded_Font_Color;
                    }
                }*/
                render_texture_resized(cRender,opTexture,barBox.x-cam->x+iconPadding*2, barBox.y-cam->y,iconSize,iconSize,NULL,NULL,FA_LEFT,FA_TOP,textureRenderColor );
            }


            if(DEFAULT_FONT!=NULL)
            {
                for( int ipLine = 0; ipLine < (int)buttonLines.size(); ipLine++)
                {
                    render_new_text(cRender,barBox.x-cam->x+iconSize+iconPadding*2+GENERAL_GPE_PADDING, barBox.y-cam->y+GENERAL_GPE_PADDING*2+(ipLine * (GENERAL_GPE_PADDING+lineHeight) ),buttonLines[ipLine] ,textRenderColor,DEFAULT_FONT,FA_LEFT,FA_MIDDLE);
                }
            }

            if( isInUse )
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x-cam->x+barBox.w,barBox.y-cam->y+barBox.h,GPE_MAIN_TEMPLATE->Main_Border_Highlighted_Color,true);
            }
        }
    }
}

void GPE_ToolPushButtonMultiLine::set_name(std::string newName)
{
    if( (int)newName.size() > 0 )
    {
        buttonLines.clear();
        int newLinePos=newName.find_first_of("\n");
        if(newLinePos!=(int)std::string::npos)
        {
            std::string cbNLString = split_first_string(newName,'\n');
            buttonLines.push_back(cbNLString);
            newLinePos=newName.find_first_of('\n');
            while (newLinePos!=(int)std::string::npos)
            {
                buttonLines.push_back(cbNLString);
                cbNLString = split_first_string(newName,'\n');
                newLinePos=newName.find_first_of('\n');
            }
            if( (int)newName.size() > 0 )
            {
                buttonLines.push_back(newName);
            }
        }
        else
        {
            buttonLines.push_back(newName);
        }
    }
    //barBox.h = GENERAL_GPE_PADDING*4+( (int)buttonLines.size() ) * (GENERAL_GPE_PADDING+lineHeight);
    barBox.h = GENERAL_GPE_PADDING+( (int)buttonLines.size() *(lineHeight+GENERAL_GPE_PADDING) );
    prerender_self(MAIN_RENDERER);
}

void GPE_ToolPushButtonMultiLine::set_website(std::string urlIn)
{
    webUrl = urlIn;
    wasClicked = false;
    prerender_self(MAIN_RENDERER);
}

GPE_SelectBoxBasic_Option::GPE_SelectBoxBasic_Option()
{
    optionValue = 0;
    optionName = "";
    optionTexture = NULL;
    optionSprite = NULL;
    subimageIndex = 0;
    isChecked = true;
    useGuiColor= false;
    sectionHasContent = true;
}

GPE_SelectBoxBasic_Option::~GPE_SelectBoxBasic_Option()
{

}


void GPE_SelectBoxBasic_Option::prerender_self(GPE_Renderer * cRender )
{
    if( cRender!=NULL)
    {
        //optionNameTexture->loadFromRenderedText(MAIN_RENDERER,optionName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_CATEGORY_BAR);
    }
}

GPE_SelectBoxBasic::GPE_SelectBoxBasic(int x, int y, std::string name)
{
    upDelay = 0;
    downDelay = 0;
    guiListTypeName = "selectbox";
    barBox.x = x;
    barBox.y = y;
    barBox.w = 228;
    barBox.h = GENERAL_GPE_PADDING;
    maxHeight = -1;
    opName = name;
    pos = 0;
    startPos = 0;
    maxOptionsInView = 10;
    optionHeight = GPE_TITLE_BPADDING+GENERAL_GPE_PADDING*2;
    optionIconWidth = GPE_AVERAGE_LINE_HEIGHT;
    showCheckboxes = false;
    showHideOthersCheckboxToggle= true;
}

GPE_SelectBoxBasic::~GPE_SelectBoxBasic()
{
    GPE_SelectBoxBasic_Option * tOption = NULL;
    for( int i = subOptions.size()-1; i>=0; i--)
    {
        tOption = subOptions[i];
        if( tOption!=NULL)
        {
            delete tOption;
            tOption = NULL;
        }
    }
    subOptions.clear();
}

std::string GPE_SelectBoxBasic::get_data()
{
    return "";
}

void GPE_SelectBoxBasic::load_data(std::string dataString)
{

}


void GPE_SelectBoxBasic::add_option(std::string newOptionName, double newOpValue,GPE_Texture * evRepIcon, GPE_Sprite * evRepSprite, int subimageInIndex, bool selectNew, bool useGuiColor )
{
    if( (int)newOptionName.size() > 0 || evRepIcon!=NULL || evRepSprite!=NULL )
    {
        GPE_SelectBoxBasic_Option * newOption = new GPE_SelectBoxBasic_Option();
        newOption->optionName = newOptionName;
        newOption->optionValue = newOpValue;
        newOption->optionSprite = evRepSprite;
        newOption->optionTexture = evRepIcon;
        newOption->useGuiColor = useGuiColor;
        //newOption->optionNameTexture->loadFromRenderedText(MAIN_RENDERER,newOptionName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_CATEGORY_BAR);

        newOption->subimageIndex = subimageInIndex;

        subOptions.push_back(newOption);
        if( maxHeight < 0)
        {
            barBox.h+=optionHeight;
        }
        if( selectNew)
        {
            set_selection( (int)subOptions.size() - 1 );
        }
    }
}

void GPE_SelectBoxBasic::clear_list()
{
    GPE_SelectBoxBasic_Option * tempOption = NULL;
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        tempOption = subOptions[i];
        if( tempOption!=NULL)
        {
            delete tempOption;
            tempOption = NULL;
        }
    }
    subOptions.clear();
    pos = 0;
    startPos = 0;
}

GPE_SelectBoxBasic_Option * GPE_SelectBoxBasic::get_option(int optionId)
{
    if( optionId >=0 && optionId < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tOption = subOptions.at(optionId);
        if( tOption!=NULL)
        {
            return tOption;
        }
    }
    return NULL;
}

std::string GPE_SelectBoxBasic::get_option_name(int optionId)
{
    if( optionId >=0 && optionId < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tOption = subOptions.at(optionId);
        if( tOption!=NULL)
        {
            return tOption->optionName;
        }
    }
    return "";
}

std::string GPE_SelectBoxBasic::get_selected_name()
{
    if( pos >=0 && pos < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * nOption = subOptions[pos];
        if( nOption!=NULL)
        {
            return nOption->optionName;
        }
    }
    return "";
}

double GPE_SelectBoxBasic::get_selected_value()
{
    if( pos >=0 && pos < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * nOption = subOptions[pos];
        if( nOption!=NULL)
        {
            return nOption->optionValue;
        }
    }
    return 0;
}

int GPE_SelectBoxBasic::get_selection()
{
    return pos;
}

int GPE_SelectBoxBasic::get_size()
{
    return (int)subOptions.size();
}

void GPE_SelectBoxBasic::insert_option(int optionId, std::string newOptionName, GPE_Texture * evRepIcon, GPE_Sprite * evRepSprite, int subimageInIndex, bool selectNew )
{
    if( (int)newOptionName.size() > 0 || evRepIcon!=NULL || evRepSprite!=NULL )
    {
        GPE_SelectBoxBasic_Option * newOption = new GPE_SelectBoxBasic_Option();
        newOption->optionName = newOptionName;
        newOption->optionSprite = evRepSprite;
        newOption->optionTexture = evRepIcon;
        newOption->subimageIndex = subimageInIndex;

        if( optionId >=0 && optionId < (int)subOptions.size() )
        {
            subOptions.insert( subOptions.begin()+optionId ,newOption );
        }
        else
        {
            subOptions.push_back( newOption );
        }
        if( maxHeight < 0)
        {
            barBox.h+=optionHeight;
        }
        if( selectNew)
        {
            set_selection( optionId+1);
        }
    }
}

void GPE_SelectBoxBasic::limit_height(int newH)
{
    if( newH >=0)
    {
        maxHeight = newH;
        barBox.h = newH;
    }
    else
    {
        maxHeight = -1;
    }
    maxOptionsInView = ceil( (float)barBox.h / (float)optionHeight );
}

void GPE_SelectBoxBasic::alter_content( int optionId, bool sectionHasContent)
{
    if( optionId >=0 && optionId < (int)subOptions.size() )
    {
        if( subOptions[optionId]!=NULL)
        {
            subOptions[optionId]->sectionHasContent = sectionHasContent;
        }
    }
}

void GPE_SelectBoxBasic::alter_content_from_value( double valueId, bool sectionHasContent)
{
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        if( subOptions[i]!=NULL && subOptions[i]->optionValue==valueId)
        {
            subOptions[i]->sectionHasContent = sectionHasContent;
            break;
        }
    }
}

bool GPE_SelectBoxBasic::move_down_space()
{
    if( pos +1 < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tempVar = subOptions[pos+1];
        subOptions[pos+1] = subOptions[pos];
        subOptions[pos] = tempVar;
        pos++;
        return true;
    }
    return false;
}

bool GPE_SelectBoxBasic::move_up_space()
{
    if( pos > 0 && (int)subOptions.size() > pos-1)
    {
        GPE_SelectBoxBasic_Option * tempVar = subOptions[pos-1];
        subOptions[pos-1] = subOptions[pos];
        subOptions[pos] = tempVar;
        pos--;
        return true;
    }
    return false;
}

void GPE_SelectBoxBasic::prerender_self(GPE_Renderer * cRender )
{

    GPE_SelectBoxBasic_Option * tOption = NULL;
    for( int i = 0; i < (int)subOptions.size() && i < (int)subOptions.size(); i++)
    {
        tOption = subOptions[i];
        if( tOption!=NULL)
        {
            tOption->prerender_self(cRender);
        }
    }

}

void GPE_SelectBoxBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    maxOptionsInView = ceil( (float)barBox.h / (float)optionHeight );

    if( isInUse)
    {
        hasArrowkeyControl = true;
        hasScrollControl = true;
        if( userInput->released[kb_up] )
        {
            pos-=1;
            if( pos < 0 )
            {
                pos= 0;
            }
            if( pos == startPos)
            {
                startPos--;
            }
        }
        else if( userInput->released[kb_down] )
        {
            pos+=1;
            if( pos >= (int)subOptions.size() )
            {
                pos=(int)subOptions.size()-1;
            }
            if( pos>=startPos+maxOptionsInView)
            {
                startPos = pos;
                if( startPos+maxOptionsInView >= (int)subOptions.size() )
                {
                    startPos = (int)subOptions.size()-maxOptionsInView;
                    if( startPos < 0)
                    {
                        startPos = 0;
                    }
                }
            }
        }
        else if( userInput->check_keyboard_released(kb_space ) && showCheckboxes)
        {
            if( pos >=0 && pos < (int)subOptions.size() )
            {
                GPE_SelectBoxBasic_Option *  optionToChange = subOptions[pos];
                optionToChange->isChecked = !optionToChange->isChecked;
            }
        }
    }

    if( isInUse && hasScrollControl )
    {
        if( userInput->mouseScrollingUp > 0)
        {
            startPos-=1;
        }
        if( userInput->mouseScrollingDown > 0)
        {
            startPos+=1;
        }
    }

    if( startPos >= (int)subOptions.size()-maxOptionsInView )
    {
        startPos=(int)subOptions.size()-maxOptionsInView;
    }

    if( startPos < 0 )
    {
        startPos= 0;
    }

    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isClicked)
    {
        isInUse = true;
        int checkBoxWidth = optionHeight+GENERAL_GPE_PADDING;
        int checkBoxHeight = optionHeight;
        if( showCheckboxes==false)
        {
            checkBoxWidth = 0;
            checkBoxHeight = 0;
        }
        //if( userInput->check_mouse_released(0) )
        {
            int relativeOptionX = viewedSpace->x+barBox.x-cam->x;
            int relativeOptionY = viewedSpace->y+barBox.y-cam->y;
            for( int i = 0; i <= maxOptionsInView && i < (int)subOptions.size(); i++)
            {
                if( point_between(userInput->mouse_x,userInput->mouse_y,relativeOptionX,relativeOptionY+(i)*optionHeight,
                                  relativeOptionX+barBox.w,relativeOptionY+(i+1)*optionHeight) )
                {
                    if( showCheckboxes)
                    {
                        if( point_between(userInput->mouse_x,userInput->mouse_y,relativeOptionX,relativeOptionY+(i)*optionHeight,
                                  relativeOptionX+checkBoxWidth,relativeOptionY+(i+1)*optionHeight) )
                        {
                            GPE_SelectBoxBasic_Option * tOp = NULL;
                            int tPos = i+startPos;
                            if( tPos >=0 && tPos < (int)subOptions.size() )
                            {
                                tOp = subOptions.at(tPos);
                                if( tOp!=NULL)
                                {
                                    tOp->isChecked = !tOp->isChecked;
                                }
                            }
                        }
                        else
                        {
                            pos = i+startPos;
                        }
                    }
                    else
                    {
                        pos = i+startPos;
                    }
                }
            }
        }
    }
    else if(clickedOutside)
    {
        isInUse = false;
        hasScrollControl = false;
        hasArrowkeyControl = false;
    }
}

void GPE_SelectBoxBasic::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && cam!=NULL && viewedSpace!=NULL )
    {
        int relativeOptionX = barBox.x-cam->x;
        int relativeOptionY = barBox.y-cam->y;
        int foundIconWidth = std::min( optionIconWidth,optionHeight );
        render_rectangle(cRender,relativeOptionX,relativeOptionY,relativeOptionX+barBox.w,relativeOptionY+barBox.h,GPE_MAIN_TEMPLATE->PopUp_Box_Color, false);
        if( pos>=startPos && pos <= startPos+maxOptionsInView )
        {
            if(pos==0 && (int)subOptions.size() > 0)
            {
                render_rectangle(cRender,relativeOptionX,relativeOptionY+(pos-startPos)*optionHeight,relativeOptionX+barBox.w,relativeOptionY+(pos-startPos+1)*optionHeight,GPE_MAIN_TEMPLATE->Button_Border_Color,false);
            }
            else if( pos > 0)
            {
                render_rectangle(cRender,relativeOptionX,relativeOptionY+(pos-startPos)*optionHeight,relativeOptionX+barBox.w,relativeOptionY+(pos-startPos+1)*optionHeight,GPE_MAIN_TEMPLATE->Button_Border_Color,false);
            }
        }
        GPE_SelectBoxBasic_Option * tOption = NULL;
        int checkBoxWidth = optionHeight+GENERAL_GPE_PADDING;
        int checkBoxHeight = optionHeight;
        if( showCheckboxes==false)
        {
            checkBoxWidth = 0;
            checkBoxHeight = 0;
        }

        GPE_Color * fontRenderColor = GPE_MAIN_TEMPLATE->Main_Box_Faded_Font_Color;
        for( int i = startPos; i < startPos+maxOptionsInView &&  i < (int)subOptions.size(); i++)
        {
            tOption = subOptions.at( i );
            if( tOption!=NULL)
            {
                if( !tOption->sectionHasContent && pos!=i)
                {
                    fontRenderColor = GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color;
                    render_rectangle(cRender,relativeOptionX,relativeOptionY+(i-startPos)*optionHeight,relativeOptionX+barBox.w,relativeOptionY+(i-startPos+1)*optionHeight,GPE_MAIN_TEMPLATE->Program_Header_Color,false,228);
                }
                else if( pos ==i )
                {
                    fontRenderColor = GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color;
                }
                else
                {
                    fontRenderColor = GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color;
                }

                if( showCheckboxes)
                {
                    render_rectangle(cRender,relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING,relativeOptionY+1+(i-startPos)*optionHeight,relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING+optionHeight,relativeOptionY+(i-startPos+1)*optionHeight,GPE_MAIN_TEMPLATE->Button_Box_Color, false);
                    if( tOption->isChecked)
                    {
                        render_texture_resized(cRender, GPE_CHECKMARK_IMAGE, relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING,relativeOptionY+(i-startPos)*optionHeight,optionHeight,optionHeight,NULL,NULL,-1,-1,GPE_MAIN_TEMPLATE->Checkbox_Color );
                    }
                    render_rectangle(cRender,relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING,relativeOptionY+1+(i-startPos)*optionHeight,relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING+optionHeight,relativeOptionY+(i-startPos+1)*optionHeight,GPE_MAIN_TEMPLATE->Button_Box_Selected_Color, true);
                    render_new_text(cRender,relativeOptionX+GENERAL_GPE_PADDING+checkBoxWidth,relativeOptionY+(i-startPos)*optionHeight+optionHeight/2,tOption->optionName,fontRenderColor,FONT_CATEGORY_BAR,FA_LEFT,FA_CENTER,255);
                }
                else
                {
                    render_new_text(cRender,relativeOptionX+optionIconWidth+GENERAL_GPE_PADDING*2+checkBoxWidth,relativeOptionY+(i-startPos)*optionHeight+optionHeight/2,tOption->optionName,fontRenderColor,FONT_CATEGORY_BAR,FA_LEFT,FA_CENTER,255);
                }
                if( tOption->optionTexture!=NULL)
                {
                    if( tOption->useGuiColor)
                    {
                        render_texture_resized(cRender,tOption->optionTexture,relativeOptionX+GENERAL_GPE_PADDING+checkBoxWidth,relativeOptionY+(i-startPos)*optionHeight+foundIconWidth/4,foundIconWidth,foundIconWidth,NULL,NULL,FA_LEFT,FA_TOP,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color );
                    }
                    else
                    {
                        render_texture_resized(cRender,tOption->optionTexture,relativeOptionX+GENERAL_GPE_PADDING+checkBoxWidth,relativeOptionY+(i-startPos)*optionHeight+foundIconWidth/4,foundIconWidth,foundIconWidth );
                    }
                }
                else if( tOption->optionSprite!=NULL)
                {
                    if( tOption->useGuiColor)
                    {
                        render_sprite_ext(cRender,tOption->optionSprite,tOption->subimageIndex,relativeOptionX+GENERAL_GPE_PADDING+checkBoxWidth,relativeOptionY+(i-startPos)*optionHeight +(optionHeight-foundIconWidth)/2,foundIconWidth,foundIconWidth,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,NULL,cam);
                    }
                    else
                    {
                        render_sprite_resized(cRender,tOption->optionSprite,tOption->subimageIndex,relativeOptionX+GENERAL_GPE_PADDING+checkBoxWidth,relativeOptionY+(i-startPos)*optionHeight +(optionHeight-foundIconWidth)/2,foundIconWidth,foundIconWidth,NULL,cam);
                    }
                }
            }
        }

        if( startPos > 0)
        {
            render_rectangle(cRender,relativeOptionX,relativeOptionY,relativeOptionX+barBox.w,relativeOptionY+4,GPE_MAIN_TEMPLATE->Button_Box_Selected_Color,false,228);
        }
        else if( startPos+maxOptionsInView < (int)subOptions.size() )
        {
            render_rectangle(cRender,relativeOptionX,relativeOptionY+barBox.h-4,relativeOptionX+barBox.w,relativeOptionY+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Selected_Color,false,228);
        }
        if( isInUse)
        {
            render_rectangle(cRender,relativeOptionX,relativeOptionY,relativeOptionX+barBox.w,relativeOptionY+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Selected_Color, true);
        }
        else
        {
            render_rectangle(cRender,relativeOptionX,relativeOptionY,relativeOptionX+barBox.w,relativeOptionY+barBox.h,GPE_MAIN_TEMPLATE->Main_Border_Color, true );
        }
    }
}

void GPE_SelectBoxBasic::rename_option(int optionId, std::string newName)
{
    if( optionId >=0 && optionId < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tOption = subOptions.at(optionId);
        if(tOption!=NULL )
        {
            tOption->optionName = newName;
        }
    }
}

void GPE_SelectBoxBasic::remove_option(int optionId)
{
    if( optionId>=0 && optionId < (int)subOptions.size() )
    {
        GPE_SelectBoxBasic_Option * tOption = subOptions.at(optionId);
        if(tOption!=NULL )
        {
            delete tOption;
            tOption = NULL;
        }
        subOptions.erase(subOptions.begin() + optionId);
        set_selection(pos);
    }
}

void GPE_SelectBoxBasic::set_option_height( int newOptionHeight)
{
    if( newOptionHeight > 0)
    {
        optionHeight = newOptionHeight;
    }
    maxOptionsInView = ceil( (float)barBox.h / (float)optionHeight );
}

void GPE_SelectBoxBasic::set_selected_option( std::string optionToSelect)
{
    GPE_SelectBoxBasic_Option * tempOption = NULL;
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        tempOption = subOptions[i];
        if( tempOption!=NULL)
        {
            if( tempOption->optionName==optionToSelect)
            {
                pos = i;
                if( i < startPos)
                {
                    startPos = i;
                }
                else
                {
                    startPos = i;
                }
            }
        }
    }
}

void GPE_SelectBoxBasic::set_selection( int selId)
{
    if( selId >= (int)subOptions.size() )
    {
        selId = (int)subOptions.size()-1;
    }

    if( selId < 0)
    {
        selId = 0;
    }
    pos = selId;

    startPos = pos-maxOptionsInView;
    if( startPos < 0)
    {
        startPos = 0;
    }
}

void GPE_SelectBoxBasic::toggle_others_checked()
{
    showHideOthersCheckboxToggle = !showHideOthersCheckboxToggle;
    GPE_SelectBoxBasic_Option * tOption = NULL;
    for(int i = 0; i < (int)subOptions.size(); i++)
    {
        if( i!=pos)
        {
            tOption = subOptions[i];
            if( tOption!=NULL)
            {
                tOption->isChecked = showHideOthersCheckboxToggle;
            }
        }
    }
}


GPE_Input_Field_Color::GPE_Input_Field_Color(int newX, int newY, int boxW, int boxH,std::string startName,std::string placeHolderText)
{
    opName = startName;
    guiListTypeName = "colorpicker";
    guiListTypeId = 2;
    storedColor = new GPE_Color();
    barBox.x = 0;
    barBox.y = 0;
    barBox.w = 228;
    barBox.h = 24;
    fieldBarBox.x = barBox.x;
    fieldBarBox.y = barBox.y;
    fieldBarBox.w = barBox.w;
    fieldBarBox.h = barBox.h;
    prevBarBoxW = barBox.w;
    prevBarBoxH = barBox.h;

    fontSizeH = 12;
    fontSizeW = 12;
    FONT_TEXTINPUT->get_metrics("A",&fontSizeW,&fontSizeH);
    if( fontSizeW<=0)
    {
        fontSizeW = 12;
    }
    if( fontSizeH<=0)
    {
        fontSizeH = 12;
    }
}

GPE_Input_Field_Color::~GPE_Input_Field_Color()
{
    if( storedColor!=NULL)
    {
        delete storedColor;
        storedColor = NULL;
    }
}

std::string GPE_Input_Field_Color::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+get_rgb_string()+",,,";
}

void GPE_Input_Field_Color::load_data(std::string dataString)
{
    std::string rgbColor = split_first_string(dataString,",,,");
    set_color_from_rgb(rgbColor);
}

GPE_Color * GPE_Input_Field_Color::get_color()
{
    if( storedColor!=NULL)
    {
        return storedColor->duplicate_color();
    }
    return NULL;
}

std::string GPE_Input_Field_Color::get_hex_string()
{
    if( storedColor!=NULL)
    {
        return RGBtoHEX(storedColor->get_r(),storedColor->get_g(),storedColor->get_b() );
    }
    return "000";
}

std::string GPE_Input_Field_Color::get_rgb_string()
{
    if( storedColor!=NULL)
    {
        return int_to_string(storedColor->get_r() )+","+int_to_string(storedColor->get_g() )+","+int_to_string(storedColor->get_b() );
    }
    return "0,0,0";
}

std::string GPE_Input_Field_Color::get_plain_string()
{
    return "\""+get_hex_string()+"\"";

}

int GPE_Input_Field_Color::get_r()
{
    if( storedColor!=NULL)
    {
        return storedColor->get_r();
    }
    return 0;
}

int GPE_Input_Field_Color::get_g()
{
    if( storedColor!=NULL)
    {
        return storedColor->get_g();
    }
    return 0;
}

int GPE_Input_Field_Color::get_b()
{
    if( storedColor!=NULL)
    {
        return storedColor->get_b();
    }
    return 0;
}

void GPE_Input_Field_Color::set_label(std::string newLabel)
{
    if( (int)newLabel.size() > 0)
    {
        inputLabel = newLabel;
        showLabel = true;

        barBox.h=prevBarBoxH+fontSizeH;
        barBox.w = std::max( prevBarBoxW, ( fontSizeW *(int)inputLabel.size()  ) );
        fieldBarBox.x = barBox.x;
        fieldBarBox.y = barBox.y+fontSizeH;
        fieldBarBox.w = prevBarBoxW;
        fieldBarBox.h = prevBarBoxH;
    }
    else
    {
        /*if( showLabel==true)
        {
            barBox.w = ;
            barBox.h =
        }*/
        inputLabel = "";
        showLabel = false;
    }
}

void GPE_Input_Field_Color::set_name(std::string newName)
{
    if( (int)newName.size() > 0)
    {
        if( opName == inputLabel)
        {
            set_label(newName);
        }
        opName = newName;
    }
}

void GPE_Input_Field_Color::set_string(std::string newString)
{

}

void GPE_Input_Field_Color::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    fieldBarBox.x = barBox.x;
    fieldBarBox.y = barBox.y;
    fieldBarBox.w = prevBarBoxW;
    fieldBarBox.h = prevBarBoxH;
    if( showLabel)
    {
        set_label(inputLabel);
    }
    /*
    Old Protocol for Label Fields
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);

    New Protocol for labeled fields

    */
    //
    if( isEnabled)
    {
        isClicked = false;
        isHovered = false;
        clickedOutside = false;
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if(viewedSpace!=NULL && cam!=NULL)
        {
            if( point_between(userInput->mouse_x,userInput->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
            {
                if (point_between(userInput->mouse_x,userInput->mouse_y,fieldBarBox.x+viewedSpace->x-cam->x,fieldBarBox.y+viewedSpace->y-cam->y,fieldBarBox.x+fieldBarBox.w+viewedSpace->x-cam->x,fieldBarBox.y+fieldBarBox.h+viewedSpace->y-cam->y) )
                {
                    isHovered = true;
                    if( (int)descriptionText.size()>0 )
                    {
                        MAIN_OVERLAY->update_tooltip(descriptionText);
                    }
                    else
                    {
                        MAIN_OVERLAY->update_tooltip(opName);
                    }
                    if( userInput->check_mouse_released(0) )
                    {
                        isClicked = true;
                    }
                }
                else if( userInput->check_mouse_released(-1) )
                {
                    clickedOutside = true;
                }
            }
            else if( userInput->check_mouse_released(-1) )
            {
                clickedOutside = true;
            }
        }
    }

    //
    if( isInUse &&( userInput->check_keyboard_down( kb_enter ) || userInput->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }
    if( isClicked)
    {
        std::string popUpString = "";
        if( (int)inputLabel.size() > 0)
        {
            popUpString = "Change "+inputLabel;
        }
        else
        {
            popUpString = "Select a Color";
        }
        GPE_Color * nColor = GPE_Get_Color_PopUp(popUpString,storedColor);
        if( nColor!=NULL)
        {
            delete storedColor;
            storedColor = nColor;
        }
    }
}

void GPE_Input_Field_Color::render_self(GPE_Renderer * cRender ,GPE_Rect * viewedSpace , GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && isEnabled && cam!=NULL)
    {
        if( cRender->screen_was_cleared() || userInput->input_received() || forceRedraw)
        {
            if( showLabel && (int)inputLabel.size() > 0)
            {
                render_new_text_ext(cRender,barBox.x-cam->x,barBox.y-cam->y,inputLabel,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_TOP);
            }
            render_rectangle(cRender,fieldBarBox.x-cam->x,fieldBarBox.y-cam->y,fieldBarBox.x+fieldBarBox.w-cam->x,fieldBarBox.y+fieldBarBox.h-cam->y,storedColor,false);
            if( isInUse)
            {
                render_rectangle(cRender,fieldBarBox.x-cam->x,fieldBarBox.y-cam->y,fieldBarBox.x+fieldBarBox.w-cam->x,fieldBarBox.y+fieldBarBox.h-cam->y,GPE_MAIN_TEMPLATE->Button_Box_Highlighted_Color,true);
            }
            else if( isHovered )
            {
                    render_rectangle(cRender,fieldBarBox.x-cam->x,fieldBarBox.y-cam->y,fieldBarBox.x+fieldBarBox.w-cam->x,fieldBarBox.y+fieldBarBox.h-cam->y,GPE_MAIN_TEMPLATE->Button_Box_Selected_Color,true);
            }
            else
            {
                    render_rectangle(cRender,fieldBarBox.x-cam->x,fieldBarBox.y-cam->y,fieldBarBox.x+fieldBarBox.w-cam->x,fieldBarBox.y+fieldBarBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
            }
        }
    }
}

void GPE_Input_Field_Color::set_color_from_rgb(GPE_Color *newColor)
{
    if( storedColor==NULL)
    {
        storedColor = new GPE_Color();
    }
    if( storedColor!=NULL)
    {
        if( newColor!=NULL)
        {
            storedColor->change_rgba(newColor->get_r(),newColor->get_g(),newColor->get_b() );
        }
    }
}

void GPE_Input_Field_Color::set_color_from_rgb(std::string newColorStr)
{
    int r = split_first_int(newColorStr,',');
    int g = split_first_int(newColorStr,',');
    int b = string_to_int(newColorStr,0);

    if( r < 0 || r > 255)
    {
        r = 0;
    }
    if( g < 0 || g > 255)
    {
        g = 0;
    }
    if( b < 0 || g > 255)
    {
        b = 0;
    }
    if( storedColor==NULL)
    {
        storedColor = new GPE_Color();
    }
    if( storedColor!=NULL)
    {
        storedColor->change_rgba(r,g,b);
    }
}

void GPE_Input_Field_Color::set_color_from_hex(std::string newColorStr)
{
    int r = 0, g = 0, b = 0;
    HEXtoRGB(newColorStr,r, g, b);
    if( storedColor==NULL)
    {
        storedColor = new GPE_Color();
    }
    if( storedColor!=NULL)
    {
        storedColor->change_rgba(r,g,b);
    }
}

void GPE_Input_Field_Color::set_r(int r)
{
    if( r < 0 || r > 255)
    {
        r = 0;
    }
    if( storedColor!=NULL)
    {
        storedColor->change_r(r);
    }
}

void GPE_Input_Field_Color::set_g(int g)
{
    if( g < 0 || g > 255)
    {
        g = 0;
    }
    if( storedColor!=NULL)
    {
        storedColor->change_g(g);
    }
}

void GPE_Input_Field_Color::set_b(int b)
{
    if( b < 0 || b > 255)
    {
        b = 0;
    }
    if( storedColor!=NULL)
    {
        storedColor->change_b(b);
    }
}

void GPE_Input_Field_Color::set_rgb(int r, int g, int b)
{
    if( r < 0 || r > 255)
    {
        r = 0;
    }

    if( g < 0 || g > 255)
    {
        g = 0;
    }

    if( b < 0 || b > 255)
    {
        b = 0;
    }
    if( storedColor!=NULL)
    {
        storedColor->change_rgba(r,g,b,255);
    }
}

void GPE_Input_Field_Color::set_height(int newHeight)
{
    if( showLabel)
    {
        fieldBarBox.h = newHeight;
    }
    else
    {
        barBox.h = newHeight;
        fieldBarBox.h = newHeight;
    }
    prevBarBoxH = newHeight;
}

void GPE_Input_Field_Color::set_width(int newWidth)
{
    if( showLabel)
    {
        fieldBarBox.w = newWidth;
    }
    else
    {
        barBox.w = newWidth;
        fieldBarBox.w = newWidth;
    }
    prevBarBoxW = newWidth;
}


GPE_TextInputBasic::GPE_TextInputBasic( std::string startName,std::string placeHolderText)
{
    resubmissionAllowed = false;
    opName = "";
    lastDoubleClickAction = 0;
    guiListTypeName = "inputtext";
    cursorPos = 0;
    prevCursorPos =0;
    cursorHistoryPos = -1;
    showPlaceHolder = true;
    if( (int)startName.size()>512)
    {
        startName = startName.substr(0,512);
    }
    textInputString = startName;
    cursorPos = (int) textInputString.size();
    showLabel = false;
    inputLabel = "";
    if( (int)placeHolderText.size()>512)
    {
        inputLabel = placeHolderText = placeHolderText.substr(0,512);
    }
    placeHolderString = placeHolderText;
    barBox.x = 0;
    barBox.y = 0;
    barBox.w = 192;
    barBox.h = 24;

    FONT_TEXTINPUT->get_metrics("A",&fontSizeW,&fontSizeH);
    if( fontSizeW<=0)
    {
        fontSizeW = 12;
    }
    maxCharactersInView = barBox.w/fontSizeW;

    inputFieldPos = 0;
    hasValidInput = true;
    showCursor = false;
    cursorTimer = 0;
    startXPos = 0;
    upArrowDelay = 0;
    downArrowDelay = 0;
    leftArrowDelay = 0;
    rightArrowDelay = 0;
    enterDelay = 0;
    bscDelay = 0;
    delDelay = 0;
    selectionCursorPos = selectionEndCursorPos = 0;
    fieldBarBox.x = barBox.x;
    fieldBarBox.y = barBox.y;
    prevBarBoxW = fieldBarBox.w = barBox.w;
    prevBarBoxH = fieldBarBox.h = barBox.h;
}

GPE_TextInputBasic::~GPE_TextInputBasic()
{

}

std::string GPE_TextInputBasic::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+textInputString;
}

void GPE_TextInputBasic::load_data(std::string dataString)
{
    textInputString = dataString;
}

void GPE_TextInputBasic::copy_selection()
{
    if( selectionCursorPos >=0 && selectionCursorPos < (int)textInputString.size() && selectionEndCursorPos >=0 && selectionEndCursorPos <= (int)textInputString.size() )
    {
        int minHighlightPos = std::min(selectionCursorPos,selectionEndCursorPos);
        int maxHighlightPos = std::max(selectionCursorPos,selectionEndCursorPos);
        std::string strToCopy = get_substring(textInputString,minHighlightPos,maxHighlightPos-minHighlightPos);
        SDL_SetClipboardText(strToCopy.c_str() );
        //MAIN_OVERLAY->update_temporary_message("Selection copied","","",0.25);
    }
}

void GPE_TextInputBasic::cut_selection()
{
    copy_selection();
    delete_selection();
}

void GPE_TextInputBasic::delete_selection()
{
    if(  (int)textInputString.size() > 0 && selectionCursorPos!=selectionEndCursorPos )
    {
        int minHighlightPos = std::min(selectionCursorPos,selectionEndCursorPos);
        int maxHighlightPos = std::max(selectionCursorPos,selectionEndCursorPos);
        if(minHighlightPos >=0 && minHighlightPos < (int)textInputString.size() && maxHighlightPos >0 && maxHighlightPos <= (int)textInputString.size() )
        {
            std::string leftSideString = get_substring(textInputString,0,minHighlightPos);
            std::string rightSideString = get_substring(textInputString,maxHighlightPos);
            textInputString = leftSideString+rightSideString;
            selectionCursorPos = 0;
            selectionEndCursorPos = 0;
            cursorPos = minHighlightPos;
            if( startXPos >minHighlightPos)
            {
                startXPos = minHighlightPos;
            }
        }
    }
}

void GPE_TextInputBasic::get_cursor_from_mouse( GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    cursorPos = floor( startXPos+(userInput->mouse_x-viewedSpace->x-fieldBarBox.x)/fontSizeW);
    if( cursorPos < 0)
    {
        cursorPos = 0;
    }
    if( cursorPos > (int)textInputString.size() )
    {
        cursorPos = (int)textInputString.size();
    }
}

std::string GPE_TextInputBasic::get_plain_string()
{
    if( is_alnum(textInputString,true,true) )
    {
        return textInputString;
    }
    else
    {
        return "''";
    }
}

std::string GPE_TextInputBasic::get_label()
{
    return inputLabel;
}

std::string GPE_TextInputBasic::get_string()
{
    return textInputString;
}

bool GPE_TextInputBasic::has_content()
{
    return ( (int)textInputString.size() > 0);
}

bool GPE_TextInputBasic::is_valid()
{
    return hasValidInput;
}

void GPE_TextInputBasic::move_left(int xToMove )
{
    if( xToMove > 0)
    {
        startXPos-=xToMove;
        if( startXPos < 0 )
        {
            startXPos = 0;
        }
    }
}

void GPE_TextInputBasic::move_right(int xToMove )
{
    int currStrSize = (int)textInputString.size();
    if( xToMove > 0 && currStrSize > maxCharactersInView )
    {
        startXPos+=xToMove;
        if( startXPos >= currStrSize-maxCharactersInView )
        {
            startXPos = currStrSize-maxCharactersInView;
        }
        if( startXPos >= currStrSize )
        {
            startXPos = currStrSize-1;
        }
        if( startXPos < 0 )
        {
            startXPos = 0;
        }
    }
}

void GPE_TextInputBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    prevCursorPos = cursorPos;
    inputSubmitted = false;
    bool pasteCommandGiven = false;
    viewedSpace = GPE_find_camera(viewedSpace);
    maxCharactersInView = barBox.w/fontSizeW;
    cam = GPE_find_camera(cam);
    fieldBarBox.x = barBox.x;
    fieldBarBox.y = barBox.y;
    fieldBarBox.w = prevBarBoxW;
    fieldBarBox.h = prevBarBoxH;
    if( showLabel)
    {
        set_label(inputLabel);
    }
    /*
    Old Protocol for Label Fields
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);

    New Protocol for labeled fields

    */
    //
    if( isEnabled)
    {
        isClicked = false;
        isHovered = false;
        clickedOutside = false;
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if(viewedSpace!=NULL && cam!=NULL)
        {
            if( point_between(userInput->mouse_x,userInput->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
            {
                if (point_between(userInput->mouse_x,userInput->mouse_y,fieldBarBox.x+viewedSpace->x-cam->x,fieldBarBox.y+viewedSpace->y-cam->y,fieldBarBox.x+fieldBarBox.w+viewedSpace->x-cam->x,fieldBarBox.y+fieldBarBox.h+viewedSpace->y-cam->y) )
                {
                    isHovered = true;
                    if( (int)descriptionText.size()>0 )
                    {
                        MAIN_OVERLAY->update_tooltip(descriptionText);
                    }
                    else
                    {
                        MAIN_OVERLAY->update_tooltip(opName);
                    }
                    if( userInput->check_mouse_released(0) )
                    {
                        isClicked = true;
                        hasArrowkeyControl = true;
                        hasScrollControl = false;
                    }
                }
                else if( userInput->check_mouse_released(-1) )
                {
                    clickedOutside = true;
                    hasScrollControl = false;
                    hasArrowkeyControl = false;
                }
            }
            else if( userInput->check_mouse_released(-1) )
            {
                clickedOutside = true;
                hasScrollControl = false;
                hasArrowkeyControl = false;
            }
        }
    }

    //
    if( isClicked)
    {
        isInUse = true;
        inputFieldPos = 0;
        userInput->inkeys = "";
        get_cursor_from_mouse(viewedSpace,cam);
    }
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_IBEAM);
    }
    if(clickedOutside)
    {
        isInUse = false;
        reset_selection();
    }
    if( isHovered)
    {
        if( userInput->check_mouse_doubleclicked(0) )
        {
            if( lastDoubleClickAction ==0)
            {
                get_cursor_from_mouse(viewedSpace,cam);
                selectionCursorPos = cursorPos;
                selectionEndCursorPos = cursorPos;
                int iPrev = 0;
                int jNext = 0;
                if( cursorPos>=0 &&  cursorPos <= (int)textInputString.size() )
                {
                    if( textInputString[cursorPos]==' ')
                    {
                        for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                        {
                            if( textInputString[iPrev]==' ')
                            {
                                selectionCursorPos--;
                            }
                            else
                            {
                                break;
                            }
                        }
                        for(jNext = cursorPos; jNext < (int)textInputString.size(); jNext++ )
                        {
                            if( textInputString[jNext]==' ')
                            {
                                selectionEndCursorPos++;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else if( char_is_alnum( textInputString[cursorPos],false,false) )
                    {
                        for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                        {
                            if( char_is_alnum( textInputString[iPrev],false,false) )
                            {
                                selectionCursorPos--;
                            }
                            else
                            {
                                break;
                            }
                        }

                        for(jNext = cursorPos; jNext < (int)textInputString.size(); jNext++ )
                        {
                            if( char_is_alnum( textInputString[jNext],false,false) )
                            {
                                selectionEndCursorPos++;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        //for symbols
                        for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                        {
                            if( char_is_alnum( textInputString[iPrev],false,false)==false)
                            {
                                selectionCursorPos--;
                            }
                            else
                            {
                                break;
                            }
                        }
                        for(jNext = cursorPos; jNext < (int)textInputString.size(); jNext++ )
                        {
                            if( char_is_alnum( textInputString[jNext],false,false)==false)
                            {
                                selectionEndCursorPos++;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
                lastDoubleClickAction = 1;
            }
            else
            {
                select_all();
                lastDoubleClickAction = 0;
            }
            userInput->reset_all_input();

        }
        else if( userInput->check_mouse_pressed(0) && RESOURCE_TO_DRAG==NULL )
        {
            get_cursor_from_mouse(viewedSpace,cam);
            selectionCursorPos = cursorPos;
            selectionEndCursorPos = cursorPos;
        }
        else if( userInput->check_mouse_down(0)  && RESOURCE_TO_DRAG==NULL )
        {
            get_cursor_from_mouse(viewedSpace,cam);
            selectionEndCursorPos = cursorPos;
        }
        else if( userInput->check_mouse_down(1) )
        {
            GPE_open_context_menu();
            MAIN_CONTEXT_MENU->set_width(128);
            MAIN_CONTEXT_MENU->add_menu_option("Undo",0,rsm->texture_add(APP_DIRECTORY_NAME+"resources/buttons/backward.png"),-1,NULL,true,true);
            MAIN_CONTEXT_MENU->add_menu_option("Cut",1,rsm->texture_add(APP_DIRECTORY_NAME+"resources/buttons/cut.png"),-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Copy",2,rsm->texture_add(APP_DIRECTORY_NAME+"resources/buttons/copy.png"),-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Paste",3,rsm->texture_add(APP_DIRECTORY_NAME+"resources/buttons/paste.png"),-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Delete",4,rsm->texture_add(APP_DIRECTORY_NAME+"resources/buttons/remove.png"),-1,NULL,true,true);
            MAIN_CONTEXT_MENU->add_menu_option("Select All",5,rsm->texture_add(APP_DIRECTORY_NAME+"resources/buttons/sticky-note.png"),-1,NULL,true,true);
            int menuSelection = get_popupmenu_result();
            if( menuSelection==5)
            {
                select_all();
            }
            else
            {
                switch(menuSelection)
                {
                    case 1:
                        cut_selection();
                    break;
                    case 2:
                        copy_selection();
                    break;
                    case 3:
                        pasteCommandGiven = true;
                    break;
                    case 4:
                        delete_selection();
                    break;
                    default:
                    break;
                }
                reset_selection();
            }
        }
        else if( RESOURCE_TO_DRAG!=NULL )
        {
            if( userInput->check_mouse_released(0) )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_IBEAM);
                get_cursor_from_mouse(viewedSpace, cam);
                if( cursorPos >=0 && cursorPos <= (int)textInputString.size() )
                {
                    set_string( get_substring(textInputString,0,cursorPos)+RESOURCE_TO_DRAG->get_name()+get_substring(textInputString,cursorPos) );
                    RESOURCE_TO_DRAG = NULL;
                }
            }
        }
    }

    //limit_space_to_rect(viewedSpace,&INP_MOUSE_X,&INP_MOUSE_Y);
    if( isEnabled &&  isInUse )
    {
        cursorTimer+=1;
        if( cursorTimer > 15*FPS_RATIO )
        {
            showCursor = true;
        }
        if( cursorTimer > 30*FPS_RATIO )
        {
            showCursor = false;
            cursorTimer = 0;
        }
        //special control action happening
        //Process Input Field delay timers
        if( userInput->down[kb_backspace]  && !userInput->pressed[kb_backspace]  )
        {
            bscDelay+=0.5;
        }
        else
        {
            bscDelay = -1;
        }
        if( userInput->down[kb_delete] && !userInput->pressed[kb_delete] )
        {
            delDelay+=0.5;
        }
        else
        {
            delDelay = -1;
        }
        if( userInput->down[kb_up] && !userInput->released[kb_up] && !userInput->pressed[kb_up] )
        {
            upArrowDelay+=0.5;
        }
        else
        {
            upArrowDelay = -1;
        }
        if( userInput->down[kb_down] && !userInput->released[kb_down] && !userInput->pressed[kb_down] )
        {
            downArrowDelay+=0.5;
        }
        else
        {
            downArrowDelay = -1;
        }
        if( userInput->down[kb_left] && !userInput->released[kb_left] && !userInput->pressed[kb_left] )
        {
            leftArrowDelay+=0.5;
        }
        else
        {
            leftArrowDelay = -1;
        }
        if( userInput->down[kb_right] && !userInput->released[kb_right] && !userInput->pressed[kb_right] )
        {
            rightArrowDelay+=0.5;
        }
        else
        {
            rightArrowDelay = -1;
        }
        if( userInput->down[kb_enter] && !userInput->released[kb_enter] && !userInput->pressed[kb_enter] )
        {
            enterDelay+=0.5;
        }
        else
        {
            enterDelay = -1;
        }
        if( userInput->down[kb_ctrl]  )
        {
            if( userInput->down[kb_a])
            {
                select_all();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( userInput->down[kb_c])
            {
                copy_selection();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( userInput->released[kb_v])
            {
                pasteCommandGiven = true;
                showCursor = true;
                cursorTimer = 0;
            }
            else if( userInput->down[kb_x])
            {
                cut_selection();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( userInput->mouseScrollingUp > 0)
            {
                move_left(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( userInput->mouseScrollingDown > 0)
            {
                move_right(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( leftArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*2*FPS_RATIO || ( userInput->pressed[kb_left]  && !userInput->released[kb_left]  ) )
            {
                if( userInput->shiftKeyIsPressed)
                {
                    if( selectionCursorPos==selectionEndCursorPos )
                    {
                        selectionCursorPos = cursorPos;
                    }
                }
                if( cursorPos >=0 && cursorPos <= (int)textInputString.size() )
                {
                    if( cursorPos >=(int)textInputString.size() )
                    {
                        cursorPos = (int)textInputString.size()-1;
                    }
                    if( cursorPos < 0)
                    {
                        cursorPos = 0;
                    }
                    else if( cursorPos < (int)textInputString.size() )
                    {
                        int iNCursorX = cursorPos-1;
                        for( iNCursorX = cursorPos-1; iNCursorX >=0; iNCursorX--)
                        {
                            if( char_is_alnum(textInputString[iNCursorX],false,false)==false )
                            {
                                break;
                            }
                        }
                        if( iNCursorX< 0)
                        {
                            iNCursorX = 0;
                        }
                        cursorPos = iNCursorX;
                    }
                    else
                    {
                        cursorPos = 0;
                    }
                }
                //scroll_to_cursor();
                showCursor = true;
                cursorTimer = 0;
                if( userInput->shiftKeyIsPressed)
                {
                    selectionEndCursorPos = cursorPos;
                }
                leftArrowDelay = 0;
            }
            else if( rightArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*2*FPS_RATIO || ( userInput->pressed[kb_right]  && !userInput->released[kb_right]  ) )
            {
                if( userInput->shiftKeyIsPressed)
                {
                    if( selectionCursorPos==selectionEndCursorPos )
                    {
                        selectionCursorPos = cursorPos;
                    }
                }
                if( cursorPos >=0 && cursorPos < (int)textInputString.size() )
                {
                    if( cursorPos >=(int)textInputString.size() )
                    {
                        cursorPos = (int)textInputString.size()-1;
                    }
                    if( cursorPos < 0)
                    {
                        cursorPos = 0;
                    }
                    else if( cursorPos < (int)textInputString.size()-1 )
                    {
                        int iNCursorX = cursorPos-1;
                        for( iNCursorX = cursorPos+1; iNCursorX < (int)textInputString.size(); iNCursorX++)
                        {
                            if( char_is_alnum(textInputString[iNCursorX],false,false)==false )
                            {
                                break;
                            }
                        }
                        if( iNCursorX< 0)
                        {
                            iNCursorX = 0;
                        }
                        cursorPos = iNCursorX;
                    }
                    else
                    {
                        cursorPos = 0;
                    }
                }
                //scroll_to_cursor();
                showCursor = true;
                cursorTimer = 0;
                if( userInput->shiftKeyIsPressed)
                {
                    selectionEndCursorPos = cursorPos;
                }
                rightArrowDelay = 0;
            }
        }
        else if( userInput->mouseScrollingUp > 0)
        {
            move_left(4);
            showCursor = true;
            cursorTimer = 0;
        }
        else if( userInput->mouseScrollingDown > 0)
        {
            move_right(4);
            showCursor = true;
            cursorTimer = 0;
        }
        else if( userInput->down[kb_alt])
        {
            //special alt action
            showCursor = true;
            cursorTimer = 0;
        }
        else if( bscDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( userInput->pressed[kb_backspace]  && !userInput->released[kb_backspace] ) )
        {
            bscDelay = 0;
            if( selectionCursorPos!=selectionEndCursorPos)
            {
                delete_selection();
            }
            else
            {
                int prevSize = (int)textInputString.size();
                if( prevSize>0  && cursorPos>0)
                {
                    cursorPos-=1;
                    std::string lineToEdit = textInputString+"";
                    textInputString = get_substring(lineToEdit,0,cursorPos );
                    std::string rightSide = get_substring(lineToEdit,cursorPos+1);
                    textInputString+=rightSide;
                    startXPos-=1;
                    if( startXPos < 0)
                    {
                        startXPos=0;
                    }
                }
            }
            showCursor = true;
            cursorTimer = 0;
            bscDelay = 0;
            //userInput->reset_all_input();
        }
        else if( delDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( userInput->pressed[kb_delete] && !userInput->released[kb_delete] ) )
        {
            if( selectionCursorPos!=selectionEndCursorPos)
            {
                delete_selection();
            }
            else
            {
                int prevSize = (int)textInputString.size();
                if( prevSize>0  && cursorPos>0)
                {
                    std::string lineToEdit = textInputString+"";
                    textInputString = get_substring(lineToEdit,0,cursorPos );
                    std::string rightSide = get_substring(lineToEdit,cursorPos+1);
                    textInputString+=rightSide;
                    startXPos-=1;
                    if( startXPos < 0)
                    {
                        startXPos=0;
                    }
                }
                delDelay = 0;
                userInput->released[kb_delete] = false;
            }
            showCursor = true;
            cursorTimer = 0;
        }
        else if( leftArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( userInput->pressed[kb_left]  && !userInput->released[kb_left]  ) )
        {
            reset_selection();
            if( cursorPos > 0)
            {
                cursorPos-=1;
                if( cursorPos <= startXPos)
                {
                    move_left(1);
                    showCursor = true;
                    cursorTimer = 0;
                }
            }
            showCursor = true;
            cursorTimer = 0;
            leftArrowDelay = 0;
        }
        else if( rightArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( userInput->pressed[kb_right]  && !userInput->released[kb_right] ) )
        {
            reset_selection(1);
            if( cursorPos < (int)textInputString.size() )
            {
                cursorPos+=1;
                if( cursorPos >= startXPos+maxCharactersInView)
                {
                    move_right(1);
                    showCursor = true;
                    cursorTimer = 0;
                }
            }
            showCursor = true;
            cursorTimer = 0;
            rightArrowDelay = 0;
        }
        else if( (upArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( userInput->pressed[kb_up]  && !userInput->released[kb_up] ) ) && cursorHistoryPos >0 )
        {
            cursorHistoryPos--;
            cursorPos = 0;
            if( cursorHistoryPos >=0 && (int)listOfPastStrings.size() > 0 )
            {
                record_string(textInputString);
                set_string(listOfPastStrings[cursorHistoryPos]);
                select_all();
                showCursor = true;
                cursorTimer = 0;
            }
            showCursor = true;
            cursorTimer = 0;
            upArrowDelay = 0;
        }
        else if( downArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( userInput->pressed[kb_down]  )  )
        {
            if( (int)listOfPastStrings.size() >0 && cursorHistoryPos>=-1 && cursorHistoryPos < (int)listOfPastStrings.size() -1 )
            {
                cursorHistoryPos++;
                cursorPos = 0;
                record_string(textInputString);
                set_string(listOfPastStrings[cursorHistoryPos]);
                select_all();
            }
            else
            {
                record_error("Unable to navigate from ["+int_to_string(cursorHistoryPos)+" with ["+int_to_string( (int)listOfPastStrings.size() )+"] past strings....");
            }
            showCursor = true;
            cursorTimer = 0;
            downArrowDelay = 0;
        }
        else if( userInput->pressed[kb_enter] )
        {
            isInUse = resubmissionAllowed;
            inputSubmitted = true;
            showCursor = true;
            cursorTimer = 0;
            enterDelay = 0;
            //record_error("Submitting self data...");
        }
        else
        {
            int moreStringSize = (int)userInput->inkeys.size();
            if( moreStringSize > 0)
            {
                delete_selection();
                textInputString=get_substring(textInputString,0,cursorPos)+userInput->inkeys+get_substring(textInputString,cursorPos);
                cursorPos+=(int)userInput->inkeys.size();
                if( cursorPos>startXPos+maxCharactersInView)
                {
                    startXPos+=moreStringSize;
                }
                showCursor = true;
                cursorTimer = 0;
            }
            userInput->inkeys = "";
        }

        if( pasteCommandGiven && SDL_HasClipboardText()==SDL_TRUE )
        {
            delete_selection();
            std::string clipboardStr = SDL_GetClipboardText();
            clipboardStr = string_replace_all(clipboardStr,"\r\n","\n");
            clipboardStr = string_replace_all(clipboardStr,"\r","\n");
            clipboardStr = string_replace_all(clipboardStr,"\t","    ");
            std::string leftBeforePaste = get_substring(textInputString,0,cursorPos);
            std::string rightBeforePaste = "";
            if( cursorPos < (int)textInputString.size() )
            {
                rightBeforePaste = get_substring(textInputString,cursorPos+1);
            }
            textInputString=leftBeforePaste+clipboardStr+rightBeforePaste;
            cursorPos+=(int)clipboardStr.size();
            move_right( (int)clipboardStr.size() );
            showCursor = true;
            cursorTimer = 0;
        }
    }
}

void GPE_TextInputBasic::record_string( std::string strToRecord)
{
    if( (int)strToRecord.size() > 0)
    {
        int recordListSize = (int)listOfPastStrings.size();
        if( recordListSize > 0)
        {
            bool duplicateStr = false;
            for( int i = 0; i < recordListSize; i++)
            {
                if( strToRecord ==listOfPastStrings[i])
                {
                    duplicateStr = true;
                }
            }
            if( !duplicateStr )
            {
                listOfPastStrings.insert( listOfPastStrings.begin(),strToRecord);
                /*if( recordListSize > MAX_STORED_TEXT_PREVIOUS_ENTRIES)
                {
                    listOfPastStrings.erase(listOfPastStrings.begin() );
                }*/

            }
        }
        else
        {
            listOfPastStrings.push_back( strToRecord);
        }
    }
}

void GPE_TextInputBasic::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace , GPE_Rect * cam ,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( isEnabled && cam!=NULL)
    {
        if( forceRedraw)
        {
            if(showBorderBox)
            {
                if( hasValidInput )
                {
                    render_rectangle(cRender,fieldBarBox.x - cam->x,fieldBarBox.y - cam->y,fieldBarBox.x+fieldBarBox.w - cam->x,fieldBarBox.y+fieldBarBox.h - cam->y,GPE_MAIN_TEMPLATE->Input_Color,false);
                }
                else
                {
                    render_rectangle(cRender,fieldBarBox.x - cam->x,fieldBarBox.y - cam->y,fieldBarBox.x+fieldBarBox.w - cam->x,fieldBarBox.y+fieldBarBox.h - cam->y,GPE_MAIN_TEMPLATE->Input_Error_Box_Color,false);
                }
            }
            if( showLabel && (int)inputLabel.size() > 0)
            {
                render_new_text(cRender,barBox.x-cam->x,barBox.y-2-cam->y,inputLabel,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_TOP);
            }
            if(selectionCursorPos!=selectionEndCursorPos )
            {
                int minHighlightPos = std::min(selectionCursorPos, selectionEndCursorPos);
                if( minHighlightPos < startXPos)
                {
                    minHighlightPos = startXPos;
                }
                int maxHighlightPos = std::max(selectionCursorPos, selectionEndCursorPos);
                if( maxHighlightPos < startXPos)
                {
                    maxHighlightPos = startXPos;
                }
                maxHighlightPos = std::min(startXPos+maxCharactersInView,maxHighlightPos);

                render_rectangle(cRender,
                     fieldBarBox.x+(minHighlightPos-startXPos)*fontSizeW - cam->x,
                     fieldBarBox.y+1 - cam->y,
                     fieldBarBox.x+2+(maxHighlightPos-startXPos)*fontSizeW- cam->x,
                     fieldBarBox.y+fieldBarBox.h-1 - cam->y,
                     GPE_MAIN_TEMPLATE->Input_Selected_Color,false);
            }
            int subLength = 0;
            if( (int)textInputString.size() > maxCharactersInView)
            {
                subLength = (int)textInputString.size()-startXPos;
                if( subLength >maxCharactersInView)
                {
                    subLength = maxCharactersInView;
                }
            }
            else
            {
                subLength =  (int)textInputString.size();
            }

            if( subLength < 0)
            {
                subLength = 0;
            }
            if( (int)textInputString.size()>0)
            {
                render_new_text(cRender,fieldBarBox.x+4-cam->x,fieldBarBox.y+fieldBarBox.h/2- cam->y,get_substring(textInputString,startXPos,subLength),GPE_MAIN_TEMPLATE->Input_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE,255);
            }
            else if(showPlaceHolder && (int)placeHolderString.size() > 0 )
            {
                if( (int)placeHolderString.size() > maxCharactersInView)
                {
                    subLength = (int)placeHolderString.size()-startXPos;
                    if( subLength >maxCharactersInView)
                    {
                        subLength = maxCharactersInView;
                    }
                }
                else
                {
                    subLength =  (int)placeHolderString.size();
                }
                render_new_text(cRender,fieldBarBox.x+4-cam->x,fieldBarBox.y+fieldBarBox.h/2 - cam->y,get_substring(placeHolderString,0,subLength),GPE_MAIN_TEMPLATE->Input_Faded_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE);
            }
            if(showBorderBox)
            {
                if( isInUse )
                {
                    render_rectangle(cRender,fieldBarBox.x - cam->x,fieldBarBox.y - cam->y,fieldBarBox.x+fieldBarBox.w - cam->x,fieldBarBox.y+fieldBarBox.h - cam->y,GPE_MAIN_TEMPLATE->Input_Highlight_Outline_Color,true);
                }
                else if( isHovered )
                {
                    render_rectangle(cRender,fieldBarBox.x - cam->x,fieldBarBox.y - cam->y,fieldBarBox.x+fieldBarBox.w - cam->x,fieldBarBox.y+fieldBarBox.h - cam->y,GPE_MAIN_TEMPLATE->Input_Highlight_Alt_Color,true);
                }
                else
                {
                    render_rectangle(cRender,fieldBarBox.x - cam->x,fieldBarBox.y - cam->y,fieldBarBox.x+fieldBarBox.w - cam->x,fieldBarBox.y+fieldBarBox.h - cam->y,GPE_MAIN_TEMPLATE->Input_Outline_Color,true);
                }
            }
        }

        if(isInUse && prevCursorPos!=cursorPos && prevCursorPos >=startXPos && prevCursorPos <= startXPos+maxCharactersInView )
        {
            render_vertical_line_color(cRender,fieldBarBox.x+GENERAL_GPE_PADDING/2+(prevCursorPos-startXPos)*fontSizeW- cam->x,fieldBarBox.y - cam->y,fieldBarBox.y+fieldBarBox.h - cam->y,GPE_MAIN_TEMPLATE->Input_Color);
        }

        if(isInUse && cursorPos >=startXPos && cursorPos <= startXPos+maxCharactersInView )
        {
            if(showCursor)
            {
                render_vertical_line_color(cRender,fieldBarBox.x+GENERAL_GPE_PADDING/2+(cursorPos-startXPos)*fontSizeW- cam->x,fieldBarBox.y - cam->y,fieldBarBox.y+fieldBarBox.h - cam->y,GPE_MAIN_TEMPLATE->Input_Font_Color);
            }
            else
            {
                render_vertical_line_color(cRender,fieldBarBox.x+GENERAL_GPE_PADDING/2+(cursorPos-startXPos)*fontSizeW- cam->x,fieldBarBox.y - cam->y,fieldBarBox.y+fieldBarBox.h - cam->y,GPE_MAIN_TEMPLATE->Input_Color);
            }
        }
    }
}

void GPE_TextInputBasic::reset_selection(int resetDirection)
{
    if( selectionCursorPos!=selectionEndCursorPos )
    {
        if( resetDirection < 0)
        {
            cursorPos = std::min(selectionCursorPos, selectionEndCursorPos);
        }
        else
        {
            cursorPos = std::max(selectionCursorPos, selectionEndCursorPos);
        }
    }
    selectionCursorPos =0;
    selectionEndCursorPos = 0;
}

void GPE_TextInputBasic::select_all()
{
    selectionCursorPos = 0;
    selectionEndCursorPos = textInputString.size();
    cursorPos = 0;
    userInput->reset_all_input();
}

void GPE_TextInputBasic::set_height(int newHeight)
{
    if( showLabel)
    {
        fieldBarBox.h = newHeight;
    }
    else
    {
        barBox.h = newHeight;
        fieldBarBox.h = newHeight;
    }
    prevBarBoxH = newHeight;
}

void GPE_TextInputBasic::set_label(std::string newLabel)
{
    if( (int)newLabel.size() > 0)
    {
        inputLabel = newLabel;
        showLabel = true;

        barBox.h=prevBarBoxH+fontSizeH;
        barBox.w = std::max( prevBarBoxW, ( fontSizeW *(int)inputLabel.size()  ) );
        fieldBarBox.x = barBox.x;
        fieldBarBox.y = barBox.y+fontSizeH;
        fieldBarBox.w = prevBarBoxW;
        fieldBarBox.h = prevBarBoxH;
    }
    else
    {
        inputLabel = "";
        showLabel = false;
    }
}

void GPE_TextInputBasic::set_name(std::string newName)
{
    if( (int)newName.size() > 0)
    {
        if( opName == inputLabel)
        {
            set_label(newName);
        }
        opName = newName;
    }
}

void GPE_TextInputBasic::set_number( double numberIn)
{
    std::string newString = double_to_string(numberIn);
    textInputString = newString;
    record_string(textInputString);
    cursorPos = (int) textInputString.size();

}

void GPE_TextInputBasic::set_string(std::string newString)
{
    if( (int)textInputString.size() > 0)
    {
        record_string( textInputString );
    }
    if( (int)newString.size() > 0)
    {
        record_string( newString );
    }
    textInputString = newString;
    cursorPos = (int) textInputString.size();
}

void GPE_TextInputBasic::set_placeholder(std::string newString)
{
        placeHolderString = newString;
}

void GPE_TextInputBasic::set_width(int newWidth)
{
    if( showLabel)
    {
        fieldBarBox.w = newWidth;
    }
    else
    {
        barBox.w = newWidth;
        fieldBarBox.w = newWidth;
    }
    prevBarBoxW = newWidth;
}

void GPE_TextInputBasic::set_max_width( int nMW)
{
    maxWidth = nMW;
    if( maxWidth > 0 )
    {
        if( barBox.w > maxWidth || fieldBarBox.w > maxWidth)
        {
            fieldBarBox.w = maxWidth;
            barBox.w = maxWidth;
            prevBarBoxW = maxWidth;
        }
    }
}

void GPE_TextInputBasic::set_max_height( int nMH)
{
    maxHeight = nMH;
    if( maxHeight > 0 && barBox.h > maxHeight)
    {
        barBox.h = maxHeight;
    }
}

bool GPE_TextInputBasic::was_submitted()
{
    return inputSubmitted;
}


GPE_TextInputNumber::GPE_TextInputNumber( std::string placeHolderText, bool onlyWholeNums,int hMin, int hMax)
{
    resubmissionAllowed = false;
    textInputString = "";
    guiListTypeName = "inputnumber";
    guiListTypeId = 5;
    forceValidNumber = false;
    showLabel = false;
    inputLabel = "";
    if( (int)placeHolderText.size()>512)
    {
        inputLabel = placeHolderText = placeHolderText.substr(0,512);
    }
    placeHolderString = placeHolderText;
    showPlaceHolder = true;
    barBox.x = 0;
    barBox.y = 0;
    barBox.w = 192;
    barBox.h = 24;

    inputFieldPos = 0;
    onlyWholeNumbers = onlyWholeNums;

    minInputNumber = hMin;
    maxInputNumber = hMax;


    fontSizeH = 12;
    fontSizeW = 12;
    FONT_TEXTINPUT->get_metrics("A",&fontSizeW,&fontSizeH);
    if( fontSizeW<=0)
    {
        fontSizeW = 12;
    }
    maxCharactersInView = barBox.w/fontSizeW;

    hasValidInput = true;
    heldNumber = 0;
    startXPos = 0;
    cursorPos = 0;
}

GPE_TextInputNumber::~GPE_TextInputNumber()
{

}

int GPE_TextInputNumber::get_held_number()
{
    heldNumber = string_to_int(textInputString, INT_MIN);
    return heldNumber;
}

std::string GPE_TextInputNumber::get_plain_string()
{
    heldNumber = string_to_int(textInputString, INT_MIN);
    return int_to_string(heldNumber);
}

int GPE_TextInputNumber::make_valid_number(int fallbackInt)
{
    heldNumber = string_to_int(textInputString, INT_MIN);
    if( heldNumber== INT_MIN || heldNumber < minInputNumber || heldNumber > maxInputNumber)
    {
        heldNumber = fallbackInt;
        hasValidInput = true;
        set_string(""+int_to_string(fallbackInt) );
    }
    return heldNumber;
}

void GPE_TextInputNumber::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    hasValidInput = true;
    GPE_TextInputBasic::process_self(viewedSpace,cam);
    if( (int)textInputString.size() > 0)
    {
        heldNumber = string_to_int(textInputString, INT_MIN);
        if( heldNumber== INT_MIN || heldNumber < minInputNumber || heldNumber > maxInputNumber)
        {
            heldNumber = 0;
            if( forceValidNumber)
            {
                set_string("0");
            }
            hasValidInput = false;
        }
    }
    else
    {
        heldNumber = 0;
        if( forceValidNumber)
        {
            set_string("0");
        }
        hasValidInput = false;
    }
}

GPE_ParsedText::GPE_ParsedText(int nStart, int nEnd)
{
    textStart = nStart;
    textEnd = nEnd;
}

GPE_ParsedText::~GPE_ParsedText()
{

}

GPE_SyntaxLine::GPE_SyntaxLine()
{
    firstPos = -1;
    lastPos = -1;
}

GPE_SyntaxLine::~GPE_SyntaxLine()
{
    for( int i = (int)foundParses.size()-1; i >=0; i--)
    {
        if( foundParses[i]!=NULL)
        {
            delete foundParses[i];
            foundParses[i] = NULL;
        }
    }
    foundParses.clear();
}

std::string GPE_SyntaxLine::get_parsed_line(std::string sIn)
{
    int startParsingPos = 0;
    int maxParseSize = (int)sIn.size();
    GPE_ParsedText* tempParseContent = NULL;
    if( maxParseSize>0 && (int)foundParses.size()>0)
    {
        std::string rString = "";
        for( int i = 0; i < (int)foundParses.size(); i++)
        {
            tempParseContent = foundParses[i];
            if( tempParseContent!=NULL)
            {
                if( tempParseContent->textStart < 0 || tempParseContent->textStart > maxParseSize )
                {
                    tempParseContent->textStart = 0;
                }
                if( tempParseContent->textStart < tempParseContent->textEnd && tempParseContent->textEnd >0 && tempParseContent->textEnd <= maxParseSize && tempParseContent->textStart >= startParsingPos  && tempParseContent->textEnd > startParsingPos )
                {
                    rString+=string_repeat(" ",tempParseContent->textStart-startParsingPos)+sIn.substr(tempParseContent->textStart,tempParseContent->textEnd-tempParseContent->textStart);
                    startParsingPos = tempParseContent->textEnd;
                }
            }
        }
        return rString;
    }
    return "";
}

void GPE_SyntaxLine::render_tokens(GPE_Renderer * cRender,GPE_Font * fontIn, std::string sIn,int xPos, int yPos, int lineStartPos, int lineMaxPos, GPE_Color * renderColor, bool isNumber)
{
    if( cRender!=NULL )
    {
        int maxParseSize = (int)foundParses.size();
        if( maxParseSize > 0)
        {
            std::string rString = "";
            GPE_ParsedText* tempParseContent = NULL;
            if( renderColor==NULL)
            {
                renderColor = GPE_MAIN_TEMPLATE->Text_Box_Font_Color;
            }
            int fontWidth = fontIn->get_mono_width();
            for( int i = 0; i < maxParseSize; i++)
            {
                tempParseContent = foundParses[i];
                if( tempParseContent!=NULL)
                {
                    if( tempParseContent->textEnd >= lineStartPos )
                    {
                        rString = get_substring(sIn,tempParseContent->textStart,tempParseContent->textEnd-tempParseContent->textStart );
                        if( isNumber)
                        {
                            render_bitmap_text(cRender,xPos+fontWidth*(tempParseContent->textStart-lineStartPos),yPos, rString,renderColor,fontIn,FA_LEFT,FA_TOP );
                        }
                        else
                        {
                            render_new_text(cRender,xPos+fontWidth*(tempParseContent->textStart-lineStartPos),yPos, rString,renderColor,fontIn,FA_LEFT,FA_TOP );
                        }
                    }
                }
            }
        }
    }
}

void GPE_SyntaxLine::reset_self()
{
    for( int i = (int)foundParses.size()-1; i >=0; i--)
    {
        if( foundParses[i]!=NULL)
        {
            delete foundParses[i];
            foundParses[i] = NULL;
        }
    }
    foundParses.clear();
    firstPos = -1;
    lastPos = -1;
}

GPE_Label_Image::GPE_Label_Image(GPE_Texture * imgIn)
{
    autoResizes = false;
    allowOverSize = false;
    resizeAspect = 1;
    guiListTypeName = "labelimage";
    opName = "";
    descriptionText = "";
    wasClicked = false;
    isHoldingCustomTexture = false;
    webUrl = "";
    imgSrc = imgIn;
    if( imgSrc!=NULL )
    {
        barBox.w = imgSrc->get_width();
        barBox.h = imgSrc->get_height();
    }
    else
    {
        barBox.w = 32;
        barBox.h = 32;
        imgSrc = NULL;
    }
}

GPE_Label_Image::GPE_Label_Image(GPE_Texture * imgIn,std::string nameIn, std::string description, std::string urlIn)
{
    allowOverSize = false;
    resizeAspect = 1;
    guiListTypeName = "labelimage";
    opName = nameIn;
    descriptionText = description;
    wasClicked = false;
    isHoldingCustomTexture = false;
    webUrl = urlIn;
    imgSrc = imgIn;
    if( imgSrc!=NULL )
    {
        barBox.w = imgSrc->get_width();
        barBox.h = imgSrc->get_height();
    }
    else
    {
        barBox.w = 32;
        barBox.h = 32;
        imgSrc = NULL;
    }
}

GPE_Label_Image::~GPE_Label_Image()
{

}

void GPE_Label_Image::load_label_image(std::string fileNameIn)
{
    if( isHoldingCustomTexture)
    {
        if( imgSrc==NULL )
        {
            imgSrc = new GPE_Texture();
        }
    }
    else
    {
        imgSrc = new GPE_Texture();
        isHoldingCustomTexture = true;
    }
    if( imgSrc!=NULL )
    {
        imgSrc->load_new_texture(MAIN_RENDERER,fileNameIn);
        barBox.w = imgSrc->get_width();
        barBox.h = imgSrc->get_height();
    }
}

void GPE_Label_Image::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered && (int)webUrl.size() > 3)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
}

void GPE_Label_Image::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && imgSrc!=NULL && cam!=NULL )
    {
        render_texture_resized(cRender,imgSrc,barBox.x-cam->x,barBox.y-cam->y,barBox.w ,barBox.h,NULL,NULL,FA_LEFT,FA_TOP);
    }
    else
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,true);
    }

    if( isInUse)
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color,true);
    }
}

void GPE_Label_Image::set_image( GPE_Texture * imgIn )
{
    imgSrc = imgIn;
    if( imgSrc!=NULL )
    {
        barBox.w = imgSrc->get_width();
        barBox.h = imgSrc->get_height();
    }
}

void GPE_Label_Image::set_width( int newW)
{
    //if( newW!=barBox.w)
    {
        if( newW > 0)
        {
            if( imgSrc!=NULL && !allowOverSize)
            {
                if( newW > imgSrc->get_width() )
                {
                    newW = imgSrc->get_width();
                }
            }
            barBox.w = (float)newW*resizeAspect;
            if( imgSrc!=NULL )
            {
                barBox.h = ceil( (float)newW * (float)imgSrc->get_height()/(float)imgSrc->get_width() );
            }
        }
        else
        {
            barBox.w = 0;
            barBox.h = 0;
        }
    }
}

void GPE_Label_Image::set_height( int newH)
{
    if( newH > 0)
    {
        if( imgSrc!=NULL && !allowOverSize)
        {
            if( newH > imgSrc->get_height() )
            {
                newH = imgSrc->get_height();
            }
        }
        barBox.h = (float)newH*resizeAspect;
        if( imgSrc!=NULL )
        {
            barBox.w = ceil( (float)newH * (float)imgSrc->get_width()/(float)imgSrc->get_height() );
        }
    }
    else
    {
        barBox.w = 0;
        barBox.h = 0;
    }
}

void GPE_Label_Image::set_size( int newW, int newH)
{
    barBox.w = newW;
    barBox.h = newH;
}

void GPE_Label_Image::set_name(std::string nameIn)
{
    opName = nameIn;
}

//

//

GPE_Label_Text::GPE_Label_Text(std::string nameIn, std::string description)
{
    guiListTypeName = "labeltext";
    guiListTypeId = 7;
    opName = nameIn;
    descriptionText = descriptionText;
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        barBox.w = bWid;
        barBox.h = bHgt;
    }
    opName = nameIn;
    hasLineBreak = true;
}

GPE_Label_Text::~GPE_Label_Text()
{

}

std::string GPE_Label_Text::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText;
}

std::string GPE_Label_Text::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_Label_Text::load_data(std::string dataString)
{
    descriptionText = dataString;
}

void GPE_Label_Text::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    /*
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
    */
    if( isInUse)
    {
        if( userInput->check_keyboard_down(kb_ctrl) && userInput->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
}

void GPE_Label_Text::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        render_new_text(cRender,barBox.x-cam->x,barBox.y-cam->y,opName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_LABEL,FA_LEFT,FA_TOP);
    }
    if( isInUse)
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color,true);
    }
}

void GPE_Label_Text::set_name(std::string nameIn)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        barBox.w = bWid;
        barBox.h = bHgt;
    }
    opName = nameIn;
}

//
//

GPE_Label_Error::GPE_Label_Error(std::string nameIn, std::string description)
{
    guiListTypeName = "labelerror";
    guiListTypeId = 7;
    opName = nameIn;
    descriptionText = descriptionText;
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        barBox.w = bWid;
        barBox.h = bHgt;
    }
    opName = nameIn;
    hasLineBreak = true;
}

GPE_Label_Error::~GPE_Label_Error()
{

}

std::string GPE_Label_Error::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText;
}

std::string GPE_Label_Error::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_Label_Error::load_data(std::string dataString)
{
    descriptionText = dataString;
}

void GPE_Label_Error::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    /*
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
    */
    if( isInUse)
    {
        if( userInput->check_keyboard_down(kb_ctrl) && userInput->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
}

void GPE_Label_Error::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        render_new_text(cRender,barBox.x-cam->x,barBox.y-cam->y,opName,GPE_MAIN_TEMPLATE->Input_Error_Font_Color,FONT_LABEL,FA_LEFT,FA_TOP);
    }
    if( isInUse)
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_Highlight_Color,true);
    }
}

void GPE_Label_Error::set_name(std::string nameIn)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        barBox.w = bWid;
        barBox.h = bHgt;
    }
    opName = nameIn;
}

//

GPE_Label_90Degree::GPE_Label_90Degree(std::string nameIn, std::string description)
{
    guiListTypeName = "labeltext";
    guiListTypeId = 7;
    opName = nameIn;
    descriptionText = descriptionText;
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        barBox.w = bHgt;
        barBox.h = bWid;
    }
    opName = nameIn;
    hasLineBreak = true;
}

GPE_Label_90Degree::~GPE_Label_90Degree()
{

}

std::string GPE_Label_90Degree::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText;
}

std::string GPE_Label_90Degree::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_Label_90Degree::load_data(std::string dataString)
{
    descriptionText = dataString;
}

void GPE_Label_90Degree::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    /*
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
    */
    if( isInUse)
    {
        if( userInput->check_keyboard_down(kb_ctrl) && userInput->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
}

void GPE_Label_90Degree::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        render_new_text_rotated(cRender,barBox.x-cam->x,barBox.y-cam->y+barBox.h,opName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_LABEL,90);
    }
    if( isInUse)
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color,true);
    }
}

void GPE_Label_90Degree::set_name(std::string nameIn)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        barBox.w = bHgt;
        barBox.h = bWid;
    }
    opName = nameIn;
}

GPE_Label_Paragraph::GPE_Label_Paragraph(std::string nameIn, std::string paragraphIn, std::string descriptionIn)
{
    autoResizes= true;
    opName = nameIn;
    paragraphText = paragraphIn;
    descriptionText = descriptionIn;
    update_paragraph();
}

GPE_Label_Paragraph::~GPE_Label_Paragraph()
{

}

void GPE_Label_Paragraph::add_line(std::string newLine)
{
    paragraphLines.push_back(newLine);
}

void GPE_Label_Paragraph::clear_all_lines()
{
    paragraphLines.clear();
}

void GPE_Label_Paragraph::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{

}

void GPE_Label_Paragraph::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && viewedSpace!=NULL && cam!=NULL )
    {
        for( int i =0; i < (int)paragraphLines.size(); i++)
        {
            render_new_text(cRender,barBox.x-cam->x,barBox.y-cam->y+(GENERAL_GPE_PADDING+defaultFontHeight)*i,paragraphLines[i],GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_PARAGRAGH,FA_LEFT,FA_TOP);
        }
    }
    if( isInUse)
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color,true);
    }
}

void GPE_Label_Paragraph::set_name(std::string nameIn)
{

}

void GPE_Label_Paragraph::set_width(int newWidth)
{
    if( barBox.w!=newWidth)
    {
        barBox.w = newWidth;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_height( int newHeight)
{
    if( barBox.h!=newHeight)
    {
        barBox.h = newHeight;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_max_width( int nMW)
{
    if( barBox.w!=nMW)
    {
        barBox.w = nMW;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_max_height( int nMH)
{
    if( barBox.h!=nMH)
    {
        barBox.h = nMH;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::update_paragraph()
{
    if( barBox.w > 0 )
    {
        clear_all_lines();
        if( (int)paragraphText.size() > 0)
        {
            int maxMessageWidth = 0;
            //int maxMessageHeight = 0;
            defaultFontWidth = 0;;
            defaultFontHeight = 0;
            int iSubMessage = 0;
            if( FONT_PARAGRAGH!=NULL)
            {
                FONT_PARAGRAGH->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
                FONT_PARAGRAGH->clear_cache();
            }

            std::vector < std::string > messageSubTitles;

            if( defaultFontWidth > 0 && defaultFontHeight > 0)
            {
                maxMessageWidth = ( barBox.w -GENERAL_GPE_PADDING)/ defaultFontWidth;

                if( (int)paragraphText.size() > 0)
                {
                    wrap_string(paragraphText,messageSubTitles,maxMessageWidth,-1);
                }

                maxMessageWidth = 0;
                if( messageSubTitles.size() <= 0)
                {
                    add_line("");
                    barBox.h = 0;
                }
                else
                {
                    for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                    {
                        add_line( messageSubTitles.at(iSubMessage) );
                    }
                    barBox.h = ( (int)messageSubTitles.size() ) *(GENERAL_GPE_PADDING+defaultFontHeight);
                }
            }
        }
        else
        {
            add_line("");
        }
    }
}


GPE_Label_Title::GPE_Label_Title(std::string nameIn, std::string description)
{
    guiListTypeName = "labeltitle";
    opName = nameIn;
    descriptionText = descriptionText;
    if( FONT_LABEL_TITLE!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL_TITLE->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        barBox.w = bWid;
        barBox.h = bHgt;
    }
    opName = nameIn;
    hasLineBreak = true;
}

GPE_Label_Title::~GPE_Label_Title()
{

}

std::string GPE_Label_Title::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText;
}

std::string GPE_Label_Title::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_Label_Title::load_data(std::string dataString)
{
    descriptionText = dataString;
}

void GPE_Label_Title::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isInUse)
    {
        if( userInput->check_keyboard_down(kb_ctrl) && userInput->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
    /*
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
    */
}

void GPE_Label_Title::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        render_new_text(cRender,barBox.x-cam->x,barBox.y+barBox.h/2-cam->y,opName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_LABEL_TITLE,FA_LEFT,FA_MIDDLE);
    }
    if( isInUse)
    {
        render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color,true);
    }
}

void GPE_Label_Title::set_name(std::string nameIn)
{
    guiListTypeName = "labeltexturl";
    if( FONT_LABEL_TITLE!=NULL && nameIn!=opName)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL_TITLE->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        barBox.w = bWid;
        barBox.h = bHgt;
    }
    opName = nameIn;
}

//

GPE_TextURL::GPE_TextURL(std::string nameIn, std::string description, std::string urlIn)
{
    webId = "";
    guiListTypeName = "texturl";
    opName = nameIn;
    descriptionText = description;
    wasClicked = false;
    webUrl = urlIn;
    if( DEFAULT_FONT!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        DEFAULT_FONT->get_metrics(nameIn, &bWid, &bHgt);
        barBox.w = bWid;
        barBox.h = bHgt;
    }
}

GPE_TextURL::~GPE_TextURL()
{

}

std::string GPE_TextURL::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText+",,,"+webUrl+",,,";
}

std::string GPE_TextURL::get_plain_string()
{
    return "\""+webUrl+"\"";
}

void GPE_TextURL::load_data(std::string dataString)
{
    descriptionText = split_first_string(dataString,",,,");
    webUrl = split_first_string(dataString,",,,");
}

void GPE_TextURL::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isInUse &&( userInput->check_keyboard_down( kb_enter ) || userInput->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
}

void GPE_TextURL::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    if( forceRedraw && (int)opName.size() > 0)
    {
        if( wasClicked)
        {
            render_new_text(cRender,barBox.x-cam->x,barBox.y-cam->y,opName,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Visited_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
            render_horizontal_line_color(cRender,barBox.y+barBox.h-cam->y,barBox.x-cam->x,barBox.x+barBox.w-cam->x,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Visited_Color);
        }
        else
        {
            render_new_text(cRender,barBox.x-cam->x,barBox.y-cam->y,opName,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
            render_horizontal_line_color(cRender,barBox.y+barBox.h-cam->y,barBox.x-cam->x,barBox.x+barBox.w-cam->x,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color);
        }
        if( isInUse)
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color,true);
        }
    }
}

void GPE_TextURL::set_name(std::string nameIn)
{
    if( DEFAULT_FONT!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        DEFAULT_FONT->get_metrics(nameIn, &bWid, &bHgt);
        barBox.w = bWid;
        barBox.h = bHgt;
    }
    opName = nameIn;
}


GPE_CheckBoxBasic::GPE_CheckBoxBasic(std::string name, std::string descriptor, int xx, int yy, bool isChecked )
{
    guiListTypeName = "checkbox";
    guiListTypeId = 1;
    opName = name;
    descriptionText = descriptor;

    barBox.x = xx;
    barBox.y = yy;
    barBox.w = clickBoxW = 24;
    barBox.h = clickBoxH = 24;
    labelBoxW = 0;
    labelBoxH = 0;
    if( (int)opName.size()>0)
    {
        DEFAULT_FONT->get_metrics(opName,&labelBoxW, &labelBoxH);
        barBox.w+= labelBoxW;
    }

    isClicked = isChecked;
}

GPE_CheckBoxBasic::~GPE_CheckBoxBasic()
{

}

std::string GPE_CheckBoxBasic::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+int_to_string(isClicked);
}

void GPE_CheckBoxBasic::load_data(std::string dataString)
{
    isClicked = is_bool(dataString);
}


void GPE_CheckBoxBasic::prerender_self(GPE_Renderer * cRender )
{
    //barBox.w = clickBoxW;
    //barBox.h = clickBoxH;
}

void GPE_CheckBoxBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    isHovered = false;
    if(viewedSpace!=NULL && cam!=NULL)
    {
        if( point_between(userInput->mouse_x,userInput->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
        {
            if (point_between(userInput->mouse_x,userInput->mouse_y,barBox.x+viewedSpace->x-cam->x,barBox.y+viewedSpace->y-cam->y,barBox.x+barBox.w+viewedSpace->x-cam->x,barBox.y+barBox.h+viewedSpace->y-cam->y) )
            {
                isHovered = true;
                if( isHovered)
                {
                    MAIN_OVERLAY->update_tooltip(descriptionText);
                    if( userInput->check_mouse_released(0) )
                    {
                        isClicked = !isClicked;
                        isInUse = true;
                    }
                }
            }
            else if( userInput->check_mouse_released(-1) )
            {
                isInUse = false;
            }
        }
        else if( userInput->check_mouse_released(-1) )
        {
            isInUse = false;
        }
        if( (isInUse || isHovered ) &&( userInput->check_keyboard_pressed( kb_enter ) || userInput->check_keyboard_pressed( kb_space )  ) )
        {
            isClicked = !isClicked;
        }

    }
}

void GPE_CheckBoxBasic::render_self(GPE_Renderer * cRender, GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && forceRedraw )
    {
        if( isInUse)
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+clickBoxW-cam->x,barBox.y+clickBoxH-cam->y,GPE_MAIN_TEMPLATE->Button_Box_Highlighted_Color,false);
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+clickBoxW-cam->x,barBox.y+clickBoxH-cam->y,GPE_MAIN_TEMPLATE->Button_Border_Highlighted_Color,true);
        }
        else if( isHovered)
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+clickBoxW-cam->x,barBox.y+clickBoxH-cam->y,GPE_MAIN_TEMPLATE->Button_Box_Selected_Color,false);
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+clickBoxW-cam->x,barBox.y+clickBoxH-cam->y,GPE_MAIN_TEMPLATE->Button_Border_Highlighted_Color,true);
        }
        else
        {
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+clickBoxW-cam->x,barBox.y+clickBoxH-cam->y,GPE_MAIN_TEMPLATE->Main_Border_Color,false);
            render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+clickBoxW-cam->x,barBox.y+clickBoxH-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,true);
        }
        if( isClicked)
        {
            render_texture_resized(cRender, GPE_CHECKMARK_IMAGE, barBox.x-cam->x,barBox.y-cam->y,clickBoxW,clickBoxH,NULL,NULL,-1,-1,GPE_MAIN_TEMPLATE->Checkbox_Color);
        }
        render_new_text(cRender,barBox.x+clickBoxW+GENERAL_GPE_PADDING-cam->x,barBox.y+barBox.h-cam->y,opName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_CHECKBOX,FA_LEFT,FA_BOTTOM,255);
    }
}

void GPE_CheckBoxBasic::set_checkbox_size(int nBoxSize, bool resizeBarBox)
{
    if( resizeBarBox)
    {
        barBox.w = nBoxSize+labelBoxW;
    }
    labelBoxW = labelBoxH = nBoxSize;
}

GPE_RadioButtonControllerBasic::GPE_RadioButtonControllerBasic(std::string cName, int cX, int cY, bool alphabetize, int colNum)
{
    guiListTypeName = "radio";
    guiListTypeId = 5;
    hoveredOption = -1;
    selectedId = 0;
    opName = cName;
    barBox.x = cX;
    barBox.y = cY;
    barBox.w = 0;
    barBox.h = 32;
    opWidth = barBox.w;
    opHeight = barBox.h;

    if( (int)opName.size()>0)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        opWidth = textW;
        if( barBox.w < textW )
        {
            barBox.w = textW+GENERAL_GPE_PADDING*2;
        }
    }
    allowDuplicates = false;
    keepAlphabetized = alphabetize;
    numCols = std::max(0,colNum);
    rowsPerCol = -1;
}

GPE_RadioButtonControllerBasic::~GPE_RadioButtonControllerBasic()
{
    GPE_KeyPair * kp = NULL;
    for( int i = subOptions.size()-1; i >=0; i--)
    {
        kp = subOptions[i];
        if( kp!=NULL )
        {
            delete kp;
            kp = NULL;
        }
    }
    subOptions.clear();
}


std::string GPE_RadioButtonControllerBasic::get_data()
{
    std::string dataString = guiListTypeName+":"+opName+"==|||==[menu]";
    GPE_KeyPair * tPair = NULL;
    for( int i = 0; i < (int)subOptions.size(); i++ )
    {
        tPair = subOptions[i];
        if( tPair!=NULL)
        {
            dataString+="[option]"+tPair->keyString+":"+tPair->keySubString+":"+int_to_string(tPair->keyValue)+"[/option]";
        }
    }
    dataString+="[/menu]"+int_to_string(selectedId)+",";
    return dataString;
}

void GPE_RadioButtonControllerBasic::load_data(std::string dataString)
{
    if( (int)dataString.size() > 0 )
    {
        std::string optionTag = "[option]";
        int optionTagSize = (int)optionTag.size();

        std::string allOptionsString = "";
        std::string newOptionData = "";
        std::string newOptionString = "";
        std::string newOptionName = "";
        std::string newOptionSubame = "";
        int newOptionId = -1;
        int beginOptionPos=dataString.find_first_of(optionTag);
        if(beginOptionPos!=(int)std::string::npos)
        {
            allOptionsString = split_first_string(dataString,"[/menu]");

            newOptionData = split_first_string(allOptionsString,"[menu]");
            while( (int)allOptionsString.size() > 0)
            {
                newOptionString = split_first_string(allOptionsString,"[/option]");

                beginOptionPos=newOptionString.find_first_of(optionTag);
                if(beginOptionPos!=(int)std::string::npos)
                {
                    //if the beginOptionPos is present, then parse on through and carryon
                    if( (int)newOptionString.size() > beginOptionPos+optionTagSize )
                    {
                        newOptionData = get_substring(newOptionString, beginOptionPos+optionTagSize );

                        newOptionName = split_first_string(newOptionData,":");
                        newOptionSubame = split_first_string(newOptionData,":");
                        newOptionId = string_to_int(newOptionData,-1);
                        if( (int)newOptionName.size() > 0)
                        {
                            add_menu_option(newOptionName,newOptionSubame,newOptionId,false);
                        }
                    }
                    else
                    {
                        return;
                    }
                }
            }
            set_selection( split_first_int(dataString,',') );
            //showJustOptions = is_bool( split_first_string(dataString,",") );
        }
    }
}

void GPE_RadioButtonControllerBasic::remove_data(std::string dataString)
{
    if( (int)dataString.size() > 0)
    {
        std::string optionTag = "[option]";
        int optionTagSize = (int)optionTag.size();

        std::string newOptionData = "";
        std::string newOptionString = "";
        std::string foundOptionName = "";
        int beginOptionPos = -1;
        while( (int)dataString.size() > 0)
        {
            newOptionString = split_first_string(dataString,"[/option]");

            beginOptionPos=newOptionString.find_first_of(optionTag);
            if(beginOptionPos!=(int)std::string::npos)
            {
                //if the beginOptionPos is present, then parse on through and carryon
                foundOptionName = newOptionString.substr(beginOptionPos+optionTagSize,newOptionString.length());
                if( (int)foundOptionName.size() > 0)
                {
                    remove_option(foundOptionName);
                }
            }
        }
    }
}

void GPE_RadioButtonControllerBasic::add_opton(std::string newOption)
{
    if( (int)newOption.size()>0 )
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(newOption,&textW, &textH);
        if( opWidth < textW )
        {
            opWidth = textW;
            barBox.w = textW+GENERAL_GPE_PADDING*2;
            //newTex->loadFromRenderedText(MAIN_RENDERER,newOption,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,DEFAULT_FONT);
        }
        GPE_KeyPair * kp = NULL;
        GPE_KeyPair * newOptionPair = new GPE_KeyPair(-1,newOption,newOption);
        if(keepAlphabetized)
        {
            std::string fString = "";
            bool isInserted = false;
            if( rowsPerCol>0)
            for(int i=0; i< (int)subOptions.size() && !isInserted; i++)
            {
                kp = subOptions[i];
                if( kp!=NULL)
                {
                    fString = kp->keyString;
                    if( fString > newOption && !isInserted )
                    {
                        subOptions.insert( subOptions.begin()+i,newOptionPair);
                        isInserted = true;
                        organize_options();
                        return;
                    }
                }
            }
        }
        subOptions.push_back(newOptionPair);
        barBox.h +=32;
        organize_options();
    }
}

void GPE_RadioButtonControllerBasic::add_menu_option(std::string optionName, std::string optionSubStr,int optionValue, bool selectOption)
{
    if( (int)optionName.size()>0 )
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(optionName,&textW, &textH);
        if( opWidth < textW )
        {
            opWidth = textW;
            barBox.w = textW+GENERAL_GPE_PADDING*2;
            //newTex->loadFromRenderedText(MAIN_RENDERER,optionName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,DEFAULT_FONT);
        }
        bool optionExists = false;
        GPE_KeyPair * tOption = NULL;
        for( int i = 0; i < (int)subOptions.size(); i++)
        {
            tOption = subOptions[i];
            if( tOption!=NULL )
            {
                if( tOption->keyString==optionName)
                {
                    optionExists = false;
                    return;
                }
            }
        }
        if( !optionExists)
        {
            GPE_KeyPair * kp = new GPE_KeyPair(optionValue,optionName,optionSubStr);
            subOptions.push_back(kp);
            if( selectOption )
            {
                set_selection( (int)subOptions.size()-1 );
            }
            barBox.h +=32;
            organize_options();
        }
    }
}

std::string GPE_RadioButtonControllerBasic::get_menu_option(int atNumb)
{
    GPE_KeyPair * kp = NULL;
    if( atNumb >=0 && atNumb < (int)subOptions.size() )
    {
        kp = subOptions.at(atNumb);
        if( kp!=NULL)
        {
            return kp->keyString;
        }
    }
    return "";
}

std::string GPE_RadioButtonControllerBasic::get_plain_string()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        if( kp!=NULL)
        {
            return "'"+kp->keyString+"'";
        }
    }
    return "''";
}

int GPE_RadioButtonControllerBasic::get_selected_id()
{
    return selectedId;
}

std::string GPE_RadioButtonControllerBasic::get_selected_tag()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keySubString;
    }
    return "";
}

int GPE_RadioButtonControllerBasic::get_selected_value()
{
    GPE_KeyPair * kp = NULL;
    if( selectedId >=0 && selectedId < (int)subOptions.size() )
    {
        kp = subOptions[selectedId];
        return kp->keyValue;
    }
    return 0;
}

void GPE_RadioButtonControllerBasic::organize_options()
{
    barBox.w = GENERAL_GPE_PADDING*2;
    barBox.h = 32;
    int maxWidthText = 0;
    int textW = 0;
    int textH = 0;
    if( (int)opName.size()>0)
    {
        textW = 0;
        textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        opWidth = textW;
        maxWidthText = textW;
        barBox.w += maxWidthText;
    }

    std::string opStr = "";
    GPE_KeyPair * tPair = NULL;
    if( numCols<=1)
    {
        for(int i =0; i < (int)subOptions.size(); i++)
        {
            tPair = subOptions[i];
            if( tPair!=NULL)
            {
                opStr = tPair->keyString;
                if( (int)opStr.size() > 0)
                {
                    textW = 0;
                    textH = 0;
                    DEFAULT_FONT->get_metrics(opStr,&textW, &textH);
                    if( maxWidthText < textW )
                    {
                        maxWidthText = textW;
                    }
                }
                barBox.h+=32;
            }
        }
        barBox.w+=maxWidthText;
    }
}

void GPE_RadioButtonControllerBasic::remove_option(std::string optionToCut)
{
    if( (int)subOptions.size()>0 )
    {
        int removePos = -1;
        GPE_KeyPair * kp = NULL;
        for(int i=(int)subOptions.size()-1; i>=0 && removePos < 0; i--)
        {
            kp = subOptions[i];
            if( kp!=NULL)
            {
                if(  kp->keyString == optionToCut)
                {
                    delete kp;
                    kp = NULL;
                    subOptions.erase(subOptions.begin()+i);
                    if( i==selectedId)
                    {
                        i = -1;
                    }
                    else if( selectedId > i)
                    {
                        selectedId-=1;
                    }
                    removePos = i;
                    barBox.h -=32;
                }
            }
        }
    }
}

void GPE_RadioButtonControllerBasic::remove_option_id(int optionToCut)
{
    if( (int)subOptions.size()>0 )
    {
        if(optionToCut >=0 && optionToCut < (int)subOptions.size() )
        {
            GPE_KeyPair * kp = subOptions.at( optionToCut );
            if( kp!=NULL)
            {
                delete kp;
                kp = NULL;
            }
            subOptions.erase(subOptions.begin()+optionToCut);
            if( optionToCut==selectedId)
            {
                optionToCut = -1;
            }
            else if( selectedId > optionToCut)
            {
                selectedId-=1;
            }
            barBox.h -=32;
        }
    }
}

void GPE_RadioButtonControllerBasic::set_selection(int newSelection)
{
    if( newSelection>=0 && newSelection < (int)subOptions.size() )
    {
        selectedId = newSelection;
    }
}

bool GPE_RadioButtonControllerBasic::set_from_tag(std::string newSelectedTag)
{
    GPE_KeyPair * tempPair = NULL;
    for( int i = (int)(subOptions.size())-1;i >=0; i--)
    {
        tempPair = subOptions[i];
        if( tempPair!=NULL)
        {
            if( tempPair->keySubString == newSelectedTag)
            {
                selectedId = i;
                return true;
            }
        }
    }
    return false;
}

void GPE_RadioButtonControllerBasic::set_value(int valueToSelect)
{
    GPE_KeyPair * tempPair = NULL;
    for( int i = (int)subOptions.size()-1; i>=0; i--)
    {
        tempPair = subOptions[i];
        if( tempPair!=NULL)
        {
            if( tempPair->keyValue==valueToSelect)
            {
                selectedId = i;
                break;
            }
        }
    }
}

void GPE_RadioButtonControllerBasic::prerender_self(GPE_Renderer * cRender )
{

}

void GPE_RadioButtonControllerBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        int buttonXPos = barBox.x+viewedSpace->x+16-cam->x;
        int buttonYPos = barBox.y+viewedSpace->y+32-cam->y;
        GPE_GeneralGuiElement::process_self(viewedSpace,cam);
        bool selectionClicked = false;
        if( isInUse)
        {
            hasArrowkeyControl = true;
        }
        else
        {
            hasArrowkeyControl = false;
        }
        //currently only handles single columns
        if( numCols<=1)
        {
            for(int i = 0; i  < (int)subOptions.size(); i++)
            {
                if(point_between( userInput->mouse_x,userInput->mouse_y,buttonXPos,buttonYPos,buttonXPos+32,buttonYPos+32) )
                {
                    hoveredOption = i;
                    if( userInput->check_mouse_released(0) )
                    {
                        selectedId = i;
                        selectionClicked = true;
                    }
                }
                buttonYPos+=32;
            }
            if( hasArrowkeyControl && isInUse )
            {
                if( userInput->check_keyboard_down(kb_left) || userInput->check_keyboard_down(kb_up) )
                {
                    selectedId--;
                }
                if( userInput->check_keyboard_down(kb_right) || userInput->check_keyboard_down(kb_down) )
                {
                    selectedId++;
                }
                if( selectedId >=(int)subOptions.size() )
                {
                    selectedId =(int)subOptions.size()-1;
                }
                if( selectedId < 0)
                {
                    selectedId = 0;
                }
            }
        }
        else
        {

        }
        if( isClicked && !selectionClicked)
        {
            hasArrowkeyControl = true;
        }
    }
}

void GPE_RadioButtonControllerBasic::render_self(GPE_Renderer * cRender , GPE_Rect * viewedSpace, GPE_Rect * cam ,bool forceRedraw )
{
    if( forceRedraw )
    {
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if( viewedSpace!=NULL && cam!=NULL)
        {
            int buttonXPos = barBox.x-cam->x;
            int buttonYPos = barBox.y-cam->y;
            if(showBorderBox)
            {
                if((int)opName.size()>0 )
                {
                    render_line(cRender,buttonXPos,buttonYPos+GPE_TITLE_BPADDING,buttonXPos+GPE_TITLE_BPADDING,buttonYPos+GPE_TITLE_BPADDING,GPE_MAIN_TEMPLATE->Button_Box_Color,255);
                    render_line(cRender,buttonXPos+GPE_TITLE_BPADDING*2+opWidth,buttonYPos+GPE_TITLE_BPADDING,
                                buttonXPos+barBox.w,buttonYPos+GPE_TITLE_BPADDING,GPE_MAIN_TEMPLATE->Button_Box_Color,255);

                    render_line(cRender,buttonXPos,buttonYPos+GPE_TITLE_BPADDING,buttonXPos,buttonYPos+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Color,255);
                    render_line(cRender,buttonXPos,buttonYPos+barBox.h,buttonXPos+barBox.w,buttonYPos+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Color,255);
                    render_line(cRender,buttonXPos+barBox.w,buttonYPos+GPE_TITLE_BPADDING,buttonXPos+barBox.w,buttonYPos+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Color,255);
                    render_new_text(cRender, buttonXPos+GPE_TITLE_BPADDING*1.5,buttonYPos+GPE_TITLE_BPADDING/2,opName,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,255);
                }
                else
                {
                    render_rectangle(cRender,buttonXPos,buttonYPos,buttonXPos+barBox.w,buttonYPos+barBox.h,GPE_MAIN_TEMPLATE->Button_Box_Color,255);
                }
                buttonXPos += 16;
                buttonYPos += 32;
                int renderedInCol = 0;

                GPE_KeyPair * kp = NULL;
                for(int i = 0; i  < (int)subOptions.size(); i++)
                {
                    if( i == hoveredOption )
                    {
                        render_sprite_resized(cRender, GPE_RadioButton_GFX,1, buttonXPos,buttonYPos, 16,16,NULL,NULL);
                    }
                    else
                    {
                        render_sprite_resized(cRender, GPE_RadioButton_GFX, 0,buttonXPos,buttonYPos, 16,16,NULL,NULL);
                    }
                    if( i == selectedId )
                    {
                        render_sprite_resized(cRender, GPE_RadioButton_GFX, 2, buttonXPos,buttonYPos, 16,16,NULL,NULL);
                    }
                    kp = subOptions[i];
                    if( kp!=NULL)
                    {
                        render_new_text(cRender, buttonXPos+32,buttonYPos,kp->keyString,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,255);
                    }
                    buttonYPos+=32;
                    renderedInCol+=1;
                    /*
                    if( rowsPerCol>0 && numCols>1 )
                    {
                        if(renderedInCol > rowsPerCol)
                        {
                            renderedInCol = 0;
                            buttonXPos+=160; //will be changed later from constant
                            buttonYPos = barBox.y+32;
                        }
                    }
                    */
                }
            }
            if( isInUse)
            {
                render_rectangle(cRender,barBox.x-cam->x,barBox.y-cam->y,barBox.x+barBox.w-cam->x,barBox.y+barBox.h-cam->y,GPE_MAIN_TEMPLATE->Main_Box_Font_URL_Color,true);
            }
        }
    }
}

void GPE_open_context_menu(int menuXPos,int menuYPos)
{
    if( menuXPos<0)
    {
        menuXPos = userInput->mouse_x;
    }
    if( menuYPos<0)
    {
        menuYPos = userInput->mouse_y;
    }
    if( MAIN_CONTEXT_MENU!=NULL)
    {
        MAIN_CONTEXT_MENU->clear_menu();
        MAIN_CONTEXT_MENU->subMenuIsOpen = true;
        MAIN_CONTEXT_MENU->set_position(menuXPos, menuYPos);
    }
}

void GPE_close_context_menu()
{
    if( MAIN_CONTEXT_MENU!=NULL && MAIN_CONTEXT_MENU->subMenuIsOpen)
    {
        MAIN_CONTEXT_MENU->clear_menu();
        MAIN_CONTEXT_MENU->subMenuIsOpen = false;
        userInput->reset_all_input();
        if( MAIN_OVERLAY!=NULL)
        {
            MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
        }
    }
}

int get_popupmenu_result(GPE_Rect * cam, bool redrawScreen)
{
    //RESOURCE_TO_DRAG = NULL;
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    MAIN_OVERLAY->take_frozen_screenshot(MAIN_RENDERER);
    userInput->reset_all_input();
    int returnValue = -1;
    bool exitOperation = false;
    if( MAIN_CONTEXT_MENU!=NULL)
    {
        MAIN_CONTEXT_MENU->hoverOption = MAIN_CONTEXT_MENU->selectedOption = 0;
    }
    while(exitOperation==false)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
        //Start the frame timer
        capTimer->start();
        //gets user input
        userInput->handle(true,true);

        if( userInput->windowEventHappendInFrame)
        {
            exitOperation = true;
            returnValue = -1;
        }
        else
        {
            if( MAIN_CONTEXT_MENU!=NULL)
            {
                if( MAIN_CONTEXT_MENU->is_open() )
                {
                    returnValue = POPUP_MENU_VALUE = MAIN_CONTEXT_MENU->process_menu_option();
                    if( userInput->check_mouse_released(kb_anykey) || WINDOW_WAS_JUST_RESIZED || userInput->released[kb_esc] || userInput->released[kb_enter] )
                    {
                        GPE_close_context_menu();
                        exitOperation = true;
                    }
                }
                else
                {
                    exitOperation = true;
                }
                calculate_avg_fps();
                if( !WINDOW_WAS_JUST_RESIZED)
                {
                    //if( redrawScreen)
                    {
                        MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
                    }
                    //Update screen
                    MAIN_CONTEXT_MENU->render_self(MAIN_RENDERER,cam);
                    MAIN_RENDERER->update_renderer();
                }
                cap_fps();
            }
            else
            {
                exitOperation = true;
                returnValue = -1;
            }
        }
        MAIN_OVERLAY->process_cursor();
    }
    GPE_close_context_menu();
    userInput->reset_all_input();
    return returnValue;
}


bool has_early_tab(std::string strIn)
{
    if( (int)strIn.size() > MAIN_GUI_SETTINGS->tabSpaceCount && MAIN_GUI_SETTINGS->tabSpaceCount > 0)
    {
        for( int ii= 0; ii < MAIN_GUI_SETTINGS->tabSpaceCount; ii++ )
        {
            if( strIn[ii]!=' ')
            {
                return false;
            }
        }
        return true;
    }
    else if( strIn.size() >= 4 )
    {
        if( strIn[0]==' ' && strIn[1]==' ' && strIn[2]==' ' && strIn[3]==' ')
        {
            return true;
        }
    }
    return false;
}

std::string untab_string(std::string strIn)
{
    if( has_early_tab(strIn) )
    {
        if( MAIN_GUI_SETTINGS!=NULL && MAIN_GUI_SETTINGS->tabSpaceCount > 0 )
        {
            strIn = get_substring(strIn, MAIN_GUI_SETTINGS->tabSpaceCount );
        }
        else
        {
            strIn = get_substring(strIn,  4 );
        }
    }
    return strIn;
}

std::string generate_tabs(int tabCount)
{
    if( tabCount > 0)
    {
        if( MAIN_GUI_SETTINGS!=NULL && MAIN_GUI_SETTINGS->tabSpaceCount > 0)
        {
            return string_repeat(" ", tabCount * MAIN_GUI_SETTINGS->tabSpaceCount );
        }
        else
        {
            return string_repeat("    ", tabCount);
        }
    }
    //record_error("0 tabs detected...");
    return "";
}

int get_tab_space_count()
{
    if( MAIN_GUI_SETTINGS!=NULL && MAIN_GUI_SETTINGS->tabSpaceCount > 0)
    {
        return MAIN_GUI_SETTINGS->tabSpaceCount;
    }
    return 4;
}

void display_user_messaage()
{
    //if( userInput->input_received() )
    {
        //Update screen
        render_rectangle(MAIN_RENDERER,0,0,256,96,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false);
        render_rectangle(MAIN_RENDERER,0,0,256,96,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
        render_new_text(MAIN_RENDERER,GENERAL_GPE_PADDING,GENERAL_GPE_PADDING,displayMessageTitle,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
        render_new_text(MAIN_RENDERER,GENERAL_GPE_PADDING,GENERAL_GPE_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,displayMessageSubtitle,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
        render_new_text(MAIN_RENDERER,GENERAL_GPE_PADDING,GENERAL_GPE_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,displayMessageString,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
        MAIN_RENDERER->update_renderer();
    }

}

void display_user_alert(std::string messageTitle, std::string messageContent, int messageType ,GPE_Rect * cam)
{
    RESOURCE_TO_DRAG = NULL;
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    //GPE_MAIN_GUI->reset_gui_info();
    MAIN_OVERLAY->update_tooltip("");
    MAIN_OVERLAY->take_frozen_screenshot(MAIN_RENDERER);
    userInput->reset_all_input();
    bool exitOperation = false;
    bool windowBeingDragged = false;

    GPE_Rect barBox;

    barBox.w = SCREEN_WIDTH*3/4 -128 - GENERAL_GPE_PADDING*4;
    barBox.h = SCREEN_HEIGHT*3/4 - 64 - GENERAL_GPE_PADDING*4;
    barBox.x = SCREEN_WIDTH - barBox.w;
    barBox.y = SCREEN_HEIGHT - barBox.h;
    int maxMessageWidth = 0;
    int maxMessageHeight = 0;
    int defaultFontWidth = 0;;
    int defaultFontHeight = 0;
    int iSubMessage = 0;
    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        FONT_DEFAULT_PROMPT->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
    }

    std::vector < std::string > messageSubTitles;

    GPE_ToolLabelButton * okButton = new GPE_ToolLabelButton(0,0,"Okay","");
    if( defaultFontWidth > 0 && defaultFontHeight > 0)
    {
        maxMessageWidth = (barBox.w-64) / defaultFontWidth;
        maxMessageHeight = (barBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);
        if( (int)messageTitle.size() > 0)
        {
            if((int)messageTitle.size() > maxMessageWidth )
            {
                messageTitle = messageTitle.substr(0,maxMessageWidth-3)+"...";
            }
        }
        else
        {
            messageTitle = "Alert";
        }

        if( (int)messageContent.size() > 0)
        {
            wrap_string(messageContent,messageSubTitles,maxMessageWidth,maxMessageHeight);
        }
        else
        {
            messageContent = "...";
            messageSubTitles.push_back(messageContent);
        }

        if( (int) messageSubTitles.size() < maxMessageHeight )
        {
            barBox.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
            barBox.y = (SCREEN_HEIGHT - barBox.h)/2;
        }
        else
        {
            barBox.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
            barBox.y = (SCREEN_HEIGHT - barBox.h)/2;
        }

        maxMessageWidth = (int)messageTitle.size();
        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
        {
            if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
            {
                maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
            }
        }

        barBox.w = (maxMessageWidth*defaultFontWidth)+128 + GENERAL_GPE_PADDING*4;
        barBox.x = (SCREEN_WIDTH - barBox.w)/2;

        while(exitOperation==false)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            //Start the frame timer
            capTimer->start();
            //gets user input
            userInput->handle(true,true);
            //GPE_MAIN_GUI->reset_gui_info();

            if( windowBeingDragged )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZEALL);
                if( userInput->check_mouse_down(0) )
                {
                    barBox.x = userInput->mouse_x-barBox.w/2;
                    barBox.y = userInput->mouse_y;
                }
                else
                {
                    windowBeingDragged = false;
                }

                if( barBox.x+barBox.w > SCREEN_WIDTH )
                {
                    barBox.x = SCREEN_WIDTH - barBox.w;
                }

                if( barBox.y+barBox.h > SCREEN_HEIGHT )
                {
                    barBox.y = SCREEN_HEIGHT - barBox.h;
                }

                if( barBox.x < 0)
                {
                    barBox.x = 0;
                }

                if( barBox.y < 0)
                {
                    barBox.y = 0;
                }
            }
            else
            {
                if( userInput->check_mouse_released(0) )
                {
                    if( point_between(userInput->mouse_x,userInput->mouse_y,barBox.x+barBox.w-32,barBox.y,barBox.x+barBox.w,barBox.y+30) )
                    {
                        exitOperation = true;
                    }
                }
                else if( userInput->check_mouse_down(0) )
                {
                    if( point_between(userInput->mouse_x,userInput->mouse_y,barBox.x,barBox.y,barBox.x+barBox.w-32,barBox.y+32) )
                    {
                        windowBeingDragged = true;
                    }
                }
            }
            if( okButton!=NULL)
            {
                okButton->set_coords(barBox.x+( barBox.w-okButton->get_width() )/2,barBox.y+barBox.h-GENERAL_GPE_PADDING-okButton->get_height() );
                okButton->process_self( NULL, NULL);
                if( okButton->is_clicked() && !windowBeingDragged )
                {
                    exitOperation = true;
                }
            }
            if( userInput->check_keyboard_released(kb_esc) || userInput->check_keyboard_released(kb_space) || userInput->check_keyboard_released(kb_enter) || WINDOW_WAS_JUST_RESIZED)
            {
                exitOperation = true;
            }

            calculate_avg_fps();
            MAIN_RENDERER->reset_viewpoint();
            //if( WINDOW_WAS_JUST_RESIZED )
            {
                render_rectangle(MAIN_RENDERER,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GPE_MAIN_TEMPLATE->Program_Color, false, 255);
                MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
            }
            //Update screen
            render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false);

            render_rectangle(MAIN_RENDERER,barBox.x,barBox.y,barBox.x+barBox.w,barBox.y+32,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color,false);

            render_new_text(MAIN_RENDERER,barBox.x+barBox.w/2,barBox.y+GENERAL_GPE_PADDING,messageTitle,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP);
            render_new_text(MAIN_RENDERER,barBox.x+barBox.w-GENERAL_GPE_PADDING,barBox.y+GENERAL_GPE_PADDING,"X",GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color,DEFAULT_FONT,FA_RIGHT,FA_TOP);

            for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
            {
                render_new_text(MAIN_RENDERER,barBox.x+32,barBox.y+GENERAL_GPE_PADDING+32+iSubMessage*(defaultFontHeight+GENERAL_GPE_PADDING),messageSubTitles.at(iSubMessage),GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
            }
            if( okButton!=NULL)
            {
                okButton->render_self(MAIN_RENDERER, NULL, NULL, true);
            }
            if( point_within_rect(userInput->mouse_x,userInput->mouse_y,&barBox) )
            {
                render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Alt_Color,true);
            }
            else
            {
                render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
            }
            MAIN_OVERLAY->process_cursor();
            MAIN_RENDERER->update_renderer();

            cap_fps();
        }
    }
    if( okButton!=NULL )
    {
        delete okButton;
        okButton = NULL;
    }
    userInput->reset_all_input();
}

int display_get_prompt(std::string messageTitle, std::string messageContent, bool showCancelButton )
{
    RESOURCE_TO_DRAG = NULL;
    int maxMessageWidth = 0;
    int maxMessageHeight = 0;
    int defaultFontWidth = 12;
    int defaultFontHeight = 12;
    std::vector < std::string > messageSubTitles;
    int iSubMessage = 0;
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    MAIN_OVERLAY->take_frozen_screenshot(MAIN_RENDERER);
    userInput->reset_all_input();
    bool exitOperation = false;
    int returnVal = DISPLAY_QUERY_CANCEL;
    GPE_ToolLabelButton * yesButton = new GPE_ToolLabelButton(0,32,"Yes[ENTER]","");
    yesButton->set_width(96);
    GPE_ToolLabelButton * noButon = NULL;
    GPE_ToolLabelButton * cancelButton = NULL;
    if( showCancelButton)
    {
        cancelButton = new GPE_ToolLabelButton(0,32,"Cancel[ESC]","");
        cancelButton->set_width(96);
        noButon = new GPE_ToolLabelButton(0,32,"No[N]","");
        noButon->set_width(96);
    }
    else
    {
        noButon = new GPE_ToolLabelButton(0,32,"No[ESC]","");
        noButon->set_width(96);
    }

    bool boxIsMoving = false;
    bool boxWasResized = true;
    bool boxBeingResized  = false;

    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        FONT_DEFAULT_PROMPT->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
    }

    GPE_Rect barBox;
    barBox.w = 528;
    barBox.h = 320;
    barBox.x = (SCREEN_WIDTH - barBox.w)/2;
    barBox.y = 72;
    barBox.x = (SCREEN_WIDTH - barBox.w)/2;
    barBox.y = 72;
    maxMessageWidth = (barBox.w-32) / defaultFontWidth;
    maxMessageHeight = (barBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);
    int newBarX2Pos = barBox.x+barBox.w;
    int newBarY2Pos = barBox.w+barBox.h;
    MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);

    if( defaultFontWidth > 0 && defaultFontHeight > 0)
    {
        maxMessageWidth = (barBox.w-32) / defaultFontWidth;
        maxMessageHeight = (barBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);

        messageSubTitles.clear();
        if( (int)messageTitle.size() > 0)
        {
            if((int)messageTitle.size() > maxMessageWidth )
            {
                messageTitle = messageTitle.substr(0,maxMessageWidth-3)+"...";
            }
        }
        else
        {
            messageTitle = "Alert";
        }

        if( (int)messageContent.size() > 0)
        {
            wrap_string(messageContent,messageSubTitles,maxMessageWidth,maxMessageHeight);
        }
        else
        {
            messageContent = "...";
            messageSubTitles.push_back(messageContent);
        }

        if( (int) messageSubTitles.size() < maxMessageHeight )
        {
            barBox.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
        }
        else
        {
            barBox.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
        }

        maxMessageWidth = (int)messageTitle.size();
        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
        {
            if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
            {
                maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
            }
        }

        barBox.x = (SCREEN_WIDTH-barBox.w)/2;
        newBarX2Pos = barBox.x+barBox.w;
        newBarY2Pos = barBox.w+barBox.h;
        int barBoxMinWidth = 64;
        int barBoxMinHeight = barBox.h;

        while(exitOperation==false )
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            //Start the frame timer
            capTimer->start();
            //gets user input
            userInput->handle(true,true);
            //GPE_MAIN_GUI->reset_gui_info();

            if( userInput->check_mouse_down(0) && !userInput->check_mouse_pressed(0) )
            {
                if( boxIsMoving)
                {
                    GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZEALL);
                    barBox.x = userInput->mouse_x-barBox.w/2;
                    barBox.y = userInput->mouse_y;
                }
                else if( boxBeingResized)
                {
                    GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
                    newBarX2Pos = userInput->mouse_x;
                    if( newBarX2Pos > barBox.x+barBoxMinWidth && newBarX2Pos < SCREEN_WIDTH-32 && newBarX2Pos> 0 )
                    {
                        barBox.w = newBarX2Pos-barBox.x;
                        boxWasResized = true;
                    }
                    newBarY2Pos = userInput->mouse_y;
                    if( newBarY2Pos > barBox.y+barBoxMinHeight && newBarY2Pos < SCREEN_HEIGHT-32 && newBarY2Pos > 0 )
                    {
                        barBox.h = newBarY2Pos-barBox.y;
                        boxWasResized = true;
                    }
                }
            }

            if( point_between(userInput->mouse_x,userInput->mouse_y,barBox.x,barBox.y,barBox.x+barBox.w,barBox.y+32) )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZEALL);
                if( userInput->check_mouse_pressed(0) )
                {
                    boxIsMoving = true;
                }
            }
            else if( point_between(userInput->mouse_x,userInput->mouse_y,barBox.x+barBox.w-32,barBox.y+barBox.h-32,barBox.x+barBox.w,barBox.y+barBox.h) )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
                if( userInput->check_mouse_pressed(0) )
                {
                    boxBeingResized = true;
                }
            }

            if( userInput->check_mouse_released(-1) )
            {
                boxIsMoving = false;
                boxBeingResized = false;
                GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            }
            if( barBox.w < barBoxMinWidth)
            {
                barBox.w = barBoxMinWidth;
            }
            if( barBox.h < barBoxMinHeight)
            {
                barBox.h = barBoxMinHeight;
            }
            if(  barBox.x+barBox.w > SCREEN_WIDTH)
            {
                barBox.x = SCREEN_WIDTH-barBox.w;
                boxWasResized = true;
            }
            if( barBox.x < 0 )
            {
                barBox.x = 0;
                boxWasResized = true;
            }

            if(  barBox.y+barBox.h > SCREEN_HEIGHT )
            {
                barBox.y = SCREEN_HEIGHT-barBox.h;
                boxWasResized = true;
            }
            if( barBox.y < 0 )
            {
                barBox.y = 0;
                boxWasResized = true;
            }

            if( boxWasResized)
            {
                maxMessageWidth = (barBox.w-32) / defaultFontWidth;
                maxMessageHeight = (barBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);

                messageSubTitles.clear();
                /*if( (int)messageTitle.size() > 0)
                {
                    if((int)messageTitle.size() > maxMessageWidth )
                    {
                        messageTitle = messageTitle.substr(0,maxMessageWidth-3)+"...";
                    }
                }
                else
                {
                    messageTitle = "Alert";
                }*/

                if( (int)messageContent.size() > 0)
                {
                    wrap_string(messageContent,messageSubTitles,maxMessageWidth,maxMessageHeight);
                }
                else
                {
                    messageContent = "...";
                    messageSubTitles.push_back(messageContent);
                }
                /*if( (int) messageSubTitles.size() < maxMessageHeight )
                {
                    barBox.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
                }
                else
                {
                    barBox.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
                }*/

                maxMessageWidth = (int)messageTitle.size();
                for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                {
                    if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
                    {
                        maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
                    }
                }
                boxWasResized = false;
            }

            yesButton->set_coords( GENERAL_GPE_PADDING+64,barBox.h-32);
            noButon->set_coords(yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_PADDING,yesButton->get_ypos() );
            if( cancelButton!=NULL)
            {
                cancelButton->set_coords( noButon->get_xpos()+noButon->get_width()+GENERAL_GPE_PADDING,noButon->get_ypos() );
                cancelButton->process_self(&barBox,&GPE_DEFAULT_CAMERA);
            }
            yesButton->process_self(&barBox,&GPE_DEFAULT_CAMERA);
            noButon->process_self(&barBox,&GPE_DEFAULT_CAMERA) ;


            if( userInput->check_keyboard_released(kb_esc) || MAIN_RENDERER->windowClosed )
            {
                if( cancelButton!=NULL)
                {
                    exitOperation = true;
                    returnVal = DISPLAY_QUERY_CANCEL;
                }
                else
                {
                    exitOperation = true;
                    returnVal = DISPLAY_QUERY_NO;
                }
            }
            else if( userInput->check_keyboard_released(kb_n) )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_NO;
            }
            else if( noButon->is_clicked() )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_NO;
            }
            else if( cancelButton!=NULL &&  cancelButton->is_clicked() )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_CANCEL;
            }
            else if( userInput->check_keyboard_released(kb_enter) || yesButton->is_clicked() )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_YES;
            }
            calculate_avg_fps();
            if( !WINDOW_WAS_JUST_RESIZED)
            {
                MAIN_RENDERER->reset_viewpoint();
                //Update screen
                //if( userInput->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
                }
                MAIN_RENDERER->set_viewpoint( &barBox);

                render_rectangle(MAIN_RENDERER,0,0,barBox.w,barBox.h,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false);
                render_rectangle(MAIN_RENDERER,0,0,barBox.w,32,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color,false);

                render_new_text(MAIN_RENDERER,GENERAL_GPE_PADDING*2,GENERAL_GPE_PADDING,messageTitle,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
                //render_new_text(MAIN_RENDERER,barBox.x+GENERAL_GPE_PADDING+32,barBox.y+64,messageContent,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
                for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                {
                    render_new_text(MAIN_RENDERER,32,GENERAL_GPE_PADDING+32+iSubMessage*(defaultFontHeight+GENERAL_GPE_PADDING),messageSubTitles.at(iSubMessage),GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
                }

                yesButton->render_self(MAIN_RENDERER,&barBox,&GPE_DEFAULT_CAMERA);
                noButon->render_self(MAIN_RENDERER,&barBox,&GPE_DEFAULT_CAMERA);
                if( cancelButton!=NULL)
                {
                    cancelButton->render_self(MAIN_RENDERER,&barBox,&GPE_DEFAULT_CAMERA);
                }
                render_rectangle(MAIN_RENDERER,1,1,barBox.w-1,barBox.h-1,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
                render_rectangle(MAIN_RENDERER,0,0,barBox.w,barBox.h,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color,true);
                //GPE_MAIN_GUI-render_gui_info(MAIN_RENDERER, true);
                MAIN_OVERLAY->process_cursor();
                MAIN_RENDERER->update_renderer();
            }
            cap_fps();
        }
    }
    MAIN_RENDERER->reset_viewpoint();
    //GPE_MAIN_GUI->render_gui_info(MAIN_RENDERER, true);
    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        FONT_DEFAULT_PROMPT->clear_cache();
    }
    if( yesButton!=NULL)
    {
        delete yesButton;
        yesButton = NULL;
    }
    if( noButon!=NULL)
    {
        delete noButon;
        noButon = NULL;
    }
    if( cancelButton!=NULL)
    {
        delete cancelButton;
        cancelButton = NULL;
    }
    userInput->reset_all_input();
    messageSubTitles.clear();
    return returnVal;
}

std::string get_string_from_popup(std::string messageTitle, std::string messageContent, std::string startName)
{
    RESOURCE_TO_DRAG = NULL;
    MAIN_OVERLAY->update_tooltip("");
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    MAIN_OVERLAY->take_frozen_screenshot(MAIN_RENDERER);
    userInput->reset_all_input();
    bool exitOperation = false;
    int frame = 0;
    int TEXTBOX_FONT_SIZE_WIDTH = 12;
    int TEXTBOX_FONT_SIZE_HEIGHT = 12;
    if( FONT_DEFAULT_PROMPT!=NULL)
    {
         FONT_DEFAULT_PROMPT->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
    }
    int titleWidth = TEXTBOX_FONT_SIZE_WIDTH * (int)messageTitle.size();
    int messageWidth = TEXTBOX_FONT_SIZE_WIDTH * (int)messageContent.size();
    int biggestStringWidth = std::max(titleWidth, messageWidth);
    std::string returnVal = startName;
    GPE_ToolLabelButton * yesButton = new GPE_ToolLabelButton(0,32,"Okay","");
    GPE_ToolLabelButton * cancelButton = new GPE_ToolLabelButton(0,32,"Cancel","");
    GPE_TextInputBasic * newStringBox = new GPE_TextInputBasic(startName);
    newStringBox->switch_inuse(true);
    int buttonsWidth = yesButton->get_width()+cancelButton->get_width();
    int promptBoxWidth = buttonsWidth;
    if( promptBoxWidth < biggestStringWidth)
    {
        promptBoxWidth =biggestStringWidth;
    }
    promptBoxWidth+=GENERAL_GPE_PADDING*3;
    GPE_Rect barBox;
    barBox.x = (SCREEN_WIDTH-promptBoxWidth)/2-GENERAL_GPE_PADDING;
    barBox.y = SCREEN_HEIGHT/2-64-GENERAL_GPE_PADDING;
    barBox.w = promptBoxWidth+128;
    barBox.h = 192;
    MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
    while(exitOperation==false)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
        //Start the frame timer
        capTimer->start();
        //gets user input
        userInput->handle(true,true);

        //GPE_MAIN_GUI->reset_gui_info();

        barBox.x = (SCREEN_WIDTH-promptBoxWidth)/2-GENERAL_GPE_PADDING;
        barBox.y = SCREEN_HEIGHT/2-64-GENERAL_GPE_PADDING;

        newStringBox->set_coords( barBox.x+GENERAL_GPE_PADDING,barBox.y+64);
        newStringBox->set_width(barBox.w - 64);

        yesButton->set_coords( barBox.x+GENERAL_GPE_PADDING,newStringBox->get_ypos()+newStringBox->get_height() + GENERAL_GPE_PADDING);
        cancelButton->set_coords( yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_PADDING,yesButton->get_ypos() );

        newStringBox->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        yesButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        cancelButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);

        if( userInput->check_keyboard_released(kb_esc) || cancelButton->is_clicked() )
        {
            exitOperation = true;
            returnVal = "";
        }
        else if( userInput->check_keyboard_released(kb_enter) || yesButton->is_clicked() )
        {
            exitOperation = true;
            if( newStringBox!=NULL)
            {
                returnVal = newStringBox->get_string();
            }
        }

        calculate_avg_fps();

        //Update screen
        //if( userInput->windowEventHappendInFrame )
        {
            MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
        }
        render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false);
        render_rectangle(MAIN_RENDERER,barBox.x,barBox.y,barBox.x+barBox.w,barBox.y+32,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color,false);

        render_new_text(MAIN_RENDERER,(SCREEN_WIDTH-promptBoxWidth)/2+GENERAL_GPE_PADDING,SCREEN_HEIGHT/2-64,messageTitle,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
        render_new_text(MAIN_RENDERER,(SCREEN_WIDTH-promptBoxWidth)/2+GENERAL_GPE_PADDING,SCREEN_HEIGHT/2-32,messageContent,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
        yesButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        cancelButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        newStringBox->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
        render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
        //GPE_MAIN_GUI-render_gui_info(MAIN_RENDERER, true);
        MAIN_OVERLAY->process_cursor();
        //GPE_MAIN_GUI->render_gui_info(MAIN_RENDERER, true);
        MAIN_RENDERER->update_renderer();

        cap_fps();
    }
    if( yesButton!=NULL)
    {
        delete yesButton;
        yesButton = NULL;
    }
    if( cancelButton!=NULL)
    {
        delete cancelButton;
        cancelButton = NULL;
    }
    if( newStringBox!=NULL)
    {
        delete newStringBox;
        newStringBox = NULL;
    }

    userInput->reset_all_input();
    return returnVal;
}

/*
    if( MAIN_CONTEXT_MENU!=NULL && MAIN_CONTEXT_MENU->subMenuIsOpen)
    {
        MAIN_CONTEXT_MENU->clear_menu();
        MAIN_CONTEXT_MENU->subMenuIsOpen = false;
        userInput->reset_all_input();
        if( GPE_MAIN_GUI!=NULL)
        {
            MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
        }
    }
}
*/
