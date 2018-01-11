/*
ambitious_gui_library.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

//Define GUI Themes to NULL
GPE_Theme * GPE_MAIN_THEME = NULL;
GPE_Theme * GPE_DEFAULT_TEMPLATE = NULL;

GPE_StatusBar * GPE_Main_Statusbar = NULL;
GPE_GUI_Settings * MAIN_GUI_SETTINGS = NULL;
std::string CURRENT_PROJECT_NAME = "";

int POPUP_FONT_SIZE_WIDTH = 12;
int POPUP_FONT_SIZE_HEIGHT = 12;
int POPUP_MENU_VALUE = -1;

GPE_GUI_Settings::GPE_GUI_Settings()
{
    useShortProjectNames = false;
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
    findScope = new GPE_DropDown_Menu( "Scope");
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

bool PAW_GUI_LOAD_FONTS()
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
    record_error("Using "+mainGuiFontLocation+" font..." );
    //Open the fonts
    font = gpe->open_font( mainGuiFontLocation, 24,fontIsMonoSpaced, "Generic Font");
    textboxFont = gpe->open_font( mainGuiFontLocation, 24,fontIsMonoSpaced, "textboxFont");
    FONT_CATEGORY_BAR = gpe->open_font( mainGuiFontLocation, 14,fontIsMonoSpaced , "FONT_CATEGORY_BAR");

    LAST_USED_FONT = gpe->open_font(mainGuiFontLocation,24,fontIsMonoSpaced, "LAST_USED_FONT");
    DEFAULT_FONT = gpe->open_font(mainGuiFontLocation,14,fontIsMonoSpaced, "DEFAULT_FONT");

    FONT_POPUP = gpe->open_font(mainGuiFontLocation,14,fontIsMonoSpaced, "FONT_POPUP");
    FONT_TOOLBAR = gpe->open_font(mainGuiFontLocation,14,fontIsMonoSpaced, "FONT_TOOLBAR");
    FONT_RESOURCEBAR = gpe->open_font(mainGuiFontLocation,12,fontIsMonoSpaced, "FONT_RESOURCEBAR");

    FONT_TEXTINPUT = gpe->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_GENERAL");
    FONT_TEXTINPUT_COMMENT = gpe->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_COMMENT");
    FONT_TEXTINPUT_KEYWORD = gpe->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_KEYWORD");
    FONT_TEXTINPUT_FUNCTION = gpe->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_FUNCTION");
    FONT_TEXTINPUT_NUMBER = gpe->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_NUMBER");
    FONT_TEXTINPUT_QUOTE = gpe->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_QUOTE");
    FONT_TEXTINPUT_SYMBOL = gpe->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_SYMBOL");
    FONT_TEXTINPUT_PROJECT_FUNCTION = gpe->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_PROJECT_FUNCTION");
    FONT_TEXTINPUT_PROJECT_KEYWORD = gpe->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_PROJECT_KEYWORD");

    FONT_TERM_NAME = gpe->open_font(textEditorFontLocation,11,true, "FONT_TERM_NAME");
    FONT_TERM_DESCRIPTION = gpe->open_font(textEditorFontLocation,11,true, "FONT_TERM_DESCRIPTION");
    FONT_TERM_SCOPE = gpe->open_font(textEditorFontLocation,11,true, "FONT_TERM_SCOPE");
    FONT_TOOLTIP = gpe->open_font(textEditorFontLocation,12,true, "FONT_TOOLTIP");

    FONT_DEFAULT_PROMPT = gpe->open_font(textEditorFontLocation,16,true, "FONT_DEFAULT_PROMPT");
    FONT_CHECKBOX = gpe->open_font(mainGuiFontLocation,12,fontIsMonoSpaced, "FONT_CHECKBOX");

    FONT_HEADER = gpe->open_font( mainGuiFontLocation, 18,fontIsMonoSpaced , "FONT_HEADER");
    FONT_LABEL = gpe->open_font( mainGuiFontLocation, 14,fontIsMonoSpaced , "FONT_LABEL");
    FONT_LABEL_ANCHOR = gpe->open_font( mainGuiFontLocation, 14,fontIsMonoSpaced , "FONT_LABEL_ANCHOR");
    FONT_LABEL_TITLE = gpe->open_font( mainGuiFontLocation, 24,fontIsMonoSpaced , "FONT_LABEL_TITLE");
    FONT_PARAGRAGH = gpe->open_font( textEditorFontLocation, 14,true, "FONT_PARAGRAGH" );
    FONT_STATUSBAR = gpe->open_font( textEditorFontLocation, 12,true, "FONT_STATUSBAR" );

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
        gpe->close_font(font);
        font = NULL;
    }

    if( textboxFont!=NULL)
    {
        gpe->close_font( textboxFont );
        textboxFont = NULL;
    }

    if( FONT_CATEGORY_BAR!=NULL)
    {
        gpe->close_font( FONT_CATEGORY_BAR );
        FONT_CATEGORY_BAR = NULL;
    }
    //
    if( DEFAULT_FONT!=NULL)
    {
        gpe->close_font(DEFAULT_FONT);
        DEFAULT_FONT = NULL;
    }

    if( FONT_POPUP!=NULL)
    {
        gpe->close_font( FONT_POPUP );
        FONT_POPUP = NULL;
    }

    if( FONT_TOOLBAR!=NULL)
    {
        gpe->close_font( FONT_TOOLBAR );
        FONT_TOOLBAR = NULL;
    }
    if( FONT_RESOURCEBAR!=NULL)
    {
        gpe->close_font( FONT_RESOURCEBAR );
        FONT_RESOURCEBAR = NULL;
    }
    //
    if( FONT_TEXTINPUT!=NULL)
    {
        gpe->close_font(FONT_TEXTINPUT);
        FONT_TEXTINPUT = NULL;
    }

    if( FONT_TERM_NAME!=NULL)
    {
        gpe->close_font(FONT_TERM_NAME);
        FONT_TERM_NAME = NULL;
    }

    if( FONT_TERM_DESCRIPTION!=NULL)
    {
        gpe->close_font(FONT_TERM_DESCRIPTION);
        FONT_TERM_DESCRIPTION = NULL;
    }

    if( FONT_TERM_SCOPE!=NULL)
    {
        gpe->close_font(FONT_TERM_SCOPE);
        FONT_TERM_SCOPE = NULL;
    }
    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        gpe->close_font(FONT_DEFAULT_PROMPT);
        FONT_DEFAULT_PROMPT = NULL;
    }

    if( FONT_CHECKBOX!=NULL)
    {
        gpe->close_font( FONT_CHECKBOX );
        FONT_CHECKBOX = NULL;
    }
    //
    if( FONT_HEADER!=NULL)
    {
        gpe->close_font( FONT_HEADER );
        FONT_HEADER = NULL;
    }

    if( FONT_LABEL!=NULL)
    {
        gpe->close_font( FONT_LABEL );
        FONT_LABEL = NULL;
    }
    if( FONT_LABEL_ANCHOR!=NULL)
    {
        gpe->close_font( FONT_LABEL_ANCHOR );
        FONT_LABEL_ANCHOR = NULL;
    }

    if( FONT_LABEL_PARAGRAPH!=NULL)
    {
        gpe->close_font( FONT_LABEL_PARAGRAPH );
        FONT_LABEL_PARAGRAPH = NULL;
    }

    if( FONT_LABEL_TITLE!=NULL)
    {
        gpe->close_font( FONT_LABEL_TITLE );
        FONT_LABEL_TITLE = NULL;
    }

    if( FONT_PARAGRAGH!=NULL)
    {
        gpe->close_font( FONT_PARAGRAGH );
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
        rectIn->update_shape( nx, ny, nw, nh);
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

GPE_Theme::GPE_Theme(std::string name, bool isCustomTheme)
{
    themeName = name;
    nonDefaultTheme = isCustomTheme;
    if( nonDefaultTheme)
    {
        themeLocalLocation = name+".gpf";
    }
    else
    {
        themeLocalLocation = "custom/"+name+".gpf";
    }

    //Background info
    themeBgFileLocation = "";
    themeBgTexture = NULL;
    //sprites
     Main_Menu_Sprite = NULL;

    //for bg and standard colors
    Program_Color = add_color("MainBackground",32,32,32);
    Program_Header_Color = add_color("HeaderBackground",62,62,62);

    //Button colors and such
    Button_Box_Color = add_color("ButtonBox",25,25,25);
    Button_Font_Color = add_color("ButtonFont",249,249,249);
    Button_Font_Highlighted_Color = add_color("ButtonFontHighlighted",240,240,240);
    Button_Border_Color = add_color("ButtonBorder",16,16,16);
    Button_Box_Highlighted_Color = add_color("ButtonBoxHighlighted",40,40,40);
    Button_Box_Selected_Color = add_color("MainBoxHighlightedAlt",96,96,96);
    Button_Border_Color = add_color( "ButtonBorder",c_dkgray );
    Button_Border_Highlighted_Color = add_color( "ButtonBorderHighlighted", c_blgray );
    Button_Border_Selected_Color = add_color( "ButtonBorderSelected", c_ltgray );

    //Used mainly for labels, urls and such
    Checkbox_Color = add_color("Checkbox", 210,180,40 );

    //For icons and Folders
    Main_Folder_Color = add_color("FolderColor", 192,192,192 );
    Main_Folder_Highlighted_Color = add_color("FolderHighlightedColor", 255, 99,71);

    //IconButton colors and such
    Icon_Box_Color = add_color("IconBox",25,25,25);
    Icon_Font_Color = add_color("IconFont",224,224,224);
    Icon_Font_Highlighted_Color = add_color("IconFontHighlighted",255,255,255);
    Icon_Font_Selected_Color = add_color("IconFontSelected",235,235,235);
    Icon_Border_Color = add_color("IconBorder",16,16,16);
    Icon_Box_Highlighted_Color = add_color("IconBorderHighlighted",40,40,40);
    Icon_Box_Selected_Color = add_color("IconBoxSelected",96,96,96);
    Icon_Border_Highlighted_Color = add_color("IconBorderHighlighted",c_blgray );
    Icon_Border_Selected_Color = add_color("IconBorderSelected", 75, 82, 92 );

    //For input fields and drop down-related menus
    Input_Color = add_color("Input",27,27,29);
    Input_Error_Box_Color = add_color("InputError",255,29,29);
    Input_Outline_Color = add_color("InputOutline",c_ltgray );
    Input_Font_Color = add_color("InputFont",c_ltgray );
    Input_Faded_Font_Color = add_color("InputFontFaded",c_gray );
    Input_Selected_Color = add_color("InputSelected",c_olive );
    Input_Highlight_Font_Color = add_color("InputOutline",c_blgray );
    Input_Error_Font_Color = add_color("InputErrorFont",228,8,8);
    Input_Error_Outline_Color = add_color("InputErrorOutline",c_maroon );
    Input_Highlight_Color = add_color("InputHighlighted",Main_Folder_Color );
    Input_Highlight_Outline_Color = add_color("InputHighlightedOutline", Main_Folder_Color );
    Input_Highlight_Alt_Color = add_color("InputHighlightedAlt",218,140,16);

    Main_Box_Color = add_color("MainBox",16,16,16);
    Main_Box_Faded_Color = add_color("MainBoxFaded",32,32,32);
    Main_Box_Highlighted_Color = add_color("MainBoxHighlighted",24,24,24);
    Main_Border_Color = add_color("MainBorder",27,27,29);
    Main_Border_Highlighted_Color = add_color("MainBorderHighlighted", 75, 82, 92 );
    Main_Box_Font_Color = add_color( "MainBoxFont",c_white );
    Main_Box_Font_URL_Color = add_color( "MainBoxFontURL",115,161,196);
    Main_Box_Font_URL_Visited_Color = add_color( "MainBoxFontURLVisited", 115,161,196 );
    Main_Box_Font_Highlight_Color = add_color( "MainBoxFontHighlighted",c_white );
    Main_Box_Faded_Font_Color = add_color( "MainBoxFontFaded",c_ltgray );

    //for errors
    Main_Error_Font_Color = add_color( "Errors",  c_maroon );
    Main_Warning_Font_Color = add_color( "Warnings",  c_blue );
    Main_Suggestion_Font_Color = add_color( "Suggestions",  c_aqua );

    //for pop up boxes like toolbar options, context menu, tool tip, etc. Essentially top Z-layer
    PopUp_Box_Color =  add_color("PopUpBox",35,35,35);
    PopUp_Box_Border_Color = add_color( "PopUpBoxOutline",  c_blgray );
    PopUp_Box_Font_Color = add_color( "PopUpBoxFont",  c_silver );
    PopUp_Box_Highlight_Font_Color = add_color( "PopUpBoxFontHighlighted",  c_ltgray );
    PopUp_Box_Faded_Font_Color = add_color( "PopUpBoxFontHighlighted",  c_gray );

    PopUp_Box_Highlight_Color = add_color("PopUpBoxHighlighted",0,0,220);
    PopUp_Box_Highlight_Alt_Color = add_color("PopUpBoxHighlightedAlt",0,220,0);
    PopUp_Box_Close_Color = add_color( "PopUpBoxClose",  c_red );
    PopUp_Box_Close_Font_Color = add_color( "PopUpBoxCloseFont",  c_white );
    PopUp_Box_Close_Hightlight_Color = add_color( "PopUpBoxCloseHighlighted",  c_maroon );

    //For Scroll Boxes
    Scroll_Box_Color = add_color( "ScrollBox",55,55,55);
    Scroll_Box_Border_Color = add_color( "ScrollBoxBorder",0,0,0);
    Scroll_Box_Arrow_Color = add_color( "ScrollBoxArrow",245, 245, 245);
    Scroll_Box_Camera_Color = add_color( "ScrollBoxCamera",85, 85, 85);
    Scroll_Box_Camera_Highlight_Color = add_color( "ScrollBoxCameraHighlight",128, 128, 128);

    //for text boxes
    Text_Box_Color = add_color( "TextBox",  c_alblack );
    Text_Box_Outer_Color = add_color( "TextBoxOuter",  c_blgray );
    Text_Box_Outer_Font_Color = add_color( "TextBoxOuterFont",  c_ltgray );
    Text_Box_Outline_Color = add_color( "TextBoxOutline",  c_jetblack );
    Text_Box_Highlight_Color = add_color("TextBoxHighlighted",50,35,50);
    Text_Box_Font_Color = add_color( "TextBoxFont",  c_ltgray );
    Text_Box_Font_Highlight_Color = add_color( "TextFontHighlighted",  c_ltgray );
    Text_Box_Font_Comment_Color = add_color("TextBoxFontComment",117, 113, 94 );

    Text_Box_Font_DataType_Color = add_color("TextBoxFontDataType",121, 163, 39 );
    Text_Box_Font_DQuote_Color = add_color( "TextFontDQuote",  c_yellow );
    Text_Box_Font_SQuote_Color = add_color( "TextFontSQuote",  c_aqua );

    Text_Box_Font_Function_Color = add_color( "TextFontFunction",  c_teal );
    Text_Box_Font_Function_Alt_Color = add_color( "TextFontFunctionAlt",  c_lime );

    Text_Box_Font_Keyword_Color = add_color("TextFontKeyword",249,38,114 );
    Text_Box_Font_Keyword_Alt_Color = add_color("TextFontKeywordAlt",38,114,249);

    Text_Box_Font_Variable_Color = add_color("TextFontVariable",164,223, 46 );
    Text_Box_Font_Variable_Alt_Color = add_color( "TextFontVariableAlt",  c_green );

    Text_Box_Font_JavaScript_Color = add_color( "TextFontJavaScript",  c_violet );
    Text_Box_Font_JavaScript_Alt_Color = add_color( "TextFontJavaScriptAlt",  c_violet );

    Text_Box_Font_Number_Color =  add_color( "TextFontNumber", 170, 126, 249 );
    Text_Box_Font_Symbols_Color = add_color( "TextFontSymbol",75, 82, 92);

    //Project Variables
    Text_Box_Project_Function_Color = add_color( "TextFontProjectFunction",135,206,250);
    Text_Box_Project_Function_Alt_Color = add_color( "TextFontProjectFunctionAlt",206,135,250);

    Text_Box_Project_Keyword_Color = add_color( "TextFontProjectKeyword",200,131,30 );
    Text_Box_Project_Keyword_Alt_Color = add_color( "TextFontProjectKeywordAlt",50,35,150);
}

GPE_Theme::~GPE_Theme()
{
    GPE_Color * tempColor = NULL;
    for(int i = (int)themeColors.size()-1; i >=0; i-- )
    {
        tempColor = themeColors[i];
        if( tempColor!=NULL )
        {
            delete tempColor;
            tempColor = NULL;
        }
    }
    themeColors.clear();
}

GPE_Color * GPE_Theme::add_color(std::string name,Uint8 r, Uint8 g, Uint8 b)
{
    GPE_Color * fColor = find_color( name);
    /*if( fColor!=NULL)
    {
        fColor->change_rgba(r,g,b);
    }
    else
    {
        */fColor = new GPE_Color( name, r, g, b);
        themeColors.push_back(fColor);
    //}
    return fColor;
}

GPE_Color * GPE_Theme::add_color(std::string name, GPE_Color * savedColor )
{
    GPE_Color * fColor = find_color( name);
    /*
    if( fColor!=NULL)
    {
        if( savedColor!=NULL)
        fColor->change_rgba( savedColor->get_r() , savedColor->get_g() , savedColor->get_b() );
    }
    else
    {*/
        if( savedColor!=NULL)
        {
            fColor = new GPE_Color( name, savedColor->get_r() , savedColor->get_g() , savedColor->get_b() );
        }
        else
        {
            fColor = new GPE_Color( name, 0,0,0 );
        }
        themeColors.push_back(fColor);
    //}
    return fColor;
}

bool GPE_Theme::change_color(std::string name,Uint8 r, Uint8 g, Uint8 b)
{
    GPE_Color * fColor = find_color( name);
    if( fColor!=NULL )
    {
        fColor->change_rgba(r,g,b);
        return true;
    }
    return false;
}



GPE_Color * GPE_Theme::find_color(std::string name)
{
    if( (int)name.size() > 0)
    {
        for(int i = (int)themeColors.size()-1; i >=0; i-- )
        {
            if( themeColors[i]->get_name()==name )
            {
                return themeColors[i];
            }
        }
    }
    return NULL;
}

GPE_Color * GPE_Theme::get_color(int pos)
{
    if( pos >=0 && pos < (int)themeColors.size() )
    {
        return themeColors[pos];
    }
    return NULL;
}

int GPE_Theme::get_color_count()
{
    return (int)themeColors.size();
}

bool GPE_Theme::is_custom_theme()
{
    return nonDefaultTheme;
}

bool GPE_Theme::load_background( std::string bgTextureLocation)
{
    if( (int)bgTextureLocation.size() > 0 )
    {
        themeBgFileLocation = get_local_from_global_file( bgTextureLocation );
        if( themeBgTexture== NULL)
        {
            themeBgTexture = new GPE_Texture();
        }
        themeBgTexture->load_new_texture( bgTextureLocation );
        if( themeBgTexture->get_width() > 0)
        {
            themeBgTexture->set_blend_mode( blend_mode_blend );
            return true;
        }
    }
    //if all the loading failed, delete texture object
    if( themeBgTexture!=NULL)
    {
        delete themeBgTexture;
        themeBgTexture = NULL;
    }
    return false;
}

bool GPE_Theme::load_theme(std::string themeLocationIn)
{
    //If the level file could be loaded
    std::string themeGlobalLocation = "";
    if( file_exists(themeLocationIn) )
    {
        themeGlobalLocation = themeLocationIn;
    }
    else
    {
      themeGlobalLocation  = APP_DIRECTORY_NAME+"themes/"+ get_local_from_global_file( themeLocationIn );
    }

    std::ifstream templateFileIn( themeGlobalLocation.c_str() );
    if( !templateFileIn.fail() )
    {
        //makes sure the file is open
        if (templateFileIn.is_open())
        {
            themeLocalLocation = themeLocationIn;
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
            GPE_Color * foundColor = NULL;
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

                            if( keyString=="Background")
                            {
                                if( nonDefaultTheme)
                                {
                                    load_background(APP_DIRECTORY_NAME+"themes/custom/"+valString);
                                }
                                else
                                {
                                    load_background(APP_DIRECTORY_NAME+"themes/"+valString);
                                }
                            }
                            else
                            {
                                foundColor = find_color( keyString );
                                if( foundColor !=NULL )
                                {
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
                                    foundColor->change_rgba( rFound, gFound, bFound );
                                }
                            }
                        }
                    }
                }
            }
            templateFileIn.close();
            return true;
        }
    }
    return false;
}

bool GPE_Theme::render_background(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL )
    {
        gpe->render_rectangle( cam->x, cam->y,cam->w,cam->h,GPE_MAIN_THEME->Program_Color,false);
        if( themeBgTexture!=NULL )
        {
            themeBgTexture->render_tex_resized( cam->x, cam->y, cam->w,cam->h );
            return true;
        }
    }
    return false;
}

bool GPE_Theme::save_theme()
{
    save_theme_as( themeLocalLocation );
}

bool GPE_Theme::save_theme_as(std::string themeLocationOut)
{
    //If the level file could be loaded
    std::string themeGlobalLocation = "";
    if( (int)themeLocationOut.size() ==0)
    {
        themeLocationOut = themeName+".gpf";
    }
    if( file_exists(themeLocationOut) )
    {
        themeGlobalLocation = themeLocationOut;
    }
    else if( nonDefaultTheme)
    {
        themeGlobalLocation  = APP_DIRECTORY_NAME+"themes/custom/"+ get_local_from_global_file( themeLocationOut );
    }
    else
    {
      themeGlobalLocation  = APP_DIRECTORY_NAME+"themes/"+ get_local_from_global_file( themeLocationOut );
    }

    std::ofstream templateFileOut( themeGlobalLocation.c_str() );
    if( !templateFileOut.fail() )
    {
        //makes sure the file is open
        if (templateFileOut.is_open())
        {
            templateFileOut << "Background="+get_local_from_global_file( themeBgFileLocation )+"\n";
            GPE_Color * tempColor = NULL;
            for(int i = 0; i < (int)themeColors.size(); i++ )
            {
                tempColor = themeColors[i];
                if( tempColor!=NULL )
                {
                    templateFileOut << tempColor->get_name()+"="+int_to_string( tempColor->get_r() )+","+int_to_string( tempColor->get_g() )+","+int_to_string( tempColor->get_b() ) << "\n";
                }
            }
        }
        themeLocalLocation = themeGlobalLocation;
    }
    templateFileOut.close();
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

void GPE_Overlay_System::take_frozen_screenshot()
{
    if( previousScreenshot==NULL)
    {
        previousScreenshot = new GPE_Texture();
    }
    if( previousScreenshot!=NULL && MAIN_RENDERER!=NULL)
    {
        MAIN_RENDERER->save_screenshot(APP_DIRECTORY_NAME+"frozen_screenshot.png");
        previousScreenshot->load_new_texture(APP_DIRECTORY_NAME+"frozen_screenshot.png");
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
            //toolTipTexture->loadFromRenderedText(MAIN_RENDERER,newTip,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT);
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

void GPE_Overlay_System::render_frozen_screenshot()
{
    if( MAIN_RENDERER!=NULL &&  previousScreenshot!=NULL)
    {
        previousScreenshot->render_tex_resized(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    }
}

void GPE_Overlay_System::render_temporary_message()
{
    if( temporayMessageDisplayTimer > 0 && temporayMessageDisplayStartTimer > 0 )
    {
        int noticeAlphaValue = (temporayMessageDisplayTimer/temporayMessageDisplayStartTimer)*255;
        if( tempTopLeftMessage)
        {
            gpe->render_rectangle( 0,0,256,96,GPE_MAIN_THEME->PopUp_Box_Color,false, noticeAlphaValue);
            gpe->render_rectangle( 0,0,256,96,GPE_MAIN_THEME->PopUp_Box_Border_Color,true,noticeAlphaValue);
            render_new_text( 0,GENERAL_GPE_PADDING,tempUserMessageTitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,noticeAlphaValue);
            render_new_text( 0,GENERAL_GPE_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,tempUserMessageSubtitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,noticeAlphaValue);
            render_new_text( 0,GENERAL_GPE_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,tempUserMessageText,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,noticeAlphaValue);

        }
        else
        {
            gpe->render_rectangle( SCREEN_WIDTH/2-256,SCREEN_HEIGHT-128,SCREEN_WIDTH/2+256,SCREEN_HEIGHT-32,GPE_MAIN_THEME->PopUp_Box_Color,false, noticeAlphaValue);
            gpe->render_rectangle( SCREEN_WIDTH/2-256,SCREEN_HEIGHT-128,SCREEN_WIDTH/2+256,SCREEN_HEIGHT-32,GPE_MAIN_THEME->PopUp_Box_Border_Color,true,noticeAlphaValue);
            render_new_text( SCREEN_WIDTH/2,SCREEN_HEIGHT-128+GENERAL_GPE_PADDING,tempUserMessageTitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP,noticeAlphaValue);
            render_new_text( SCREEN_WIDTH/2,SCREEN_HEIGHT-128+GENERAL_GPE_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,tempUserMessageSubtitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP,noticeAlphaValue);
            render_new_text( SCREEN_WIDTH/2,SCREEN_HEIGHT-128+GENERAL_GPE_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,tempUserMessageText,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP,noticeAlphaValue);
        }
        temporayMessageDisplayTimer--;
    }
}

void GPE_Overlay_System::render_tooltip(int xPos, int yPos)
{
    if( (int)toolTipString.size()>0)
    {
        if( xPos < 0)
        {
            xPos = input->mouse_x;
        }
        if( yPos < 0 || yPos > SCREEN_HEIGHT-GENERAL_GPE_PADDING-toolTipTexture->get_height() )
        {
            yPos = input->mouse_y+32;
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

        gpe->render_rectangle( xPos-GENERAL_GPE_PADDING/2,yPos-GENERAL_GPE_PADDING/2,xPos2,yPos2,GPE_MAIN_THEME->PopUp_Box_Color,false);
        for( int i = 0; i < (int)toolTipLines.size(); i++)
        {
            render_new_text( xPos+GENERAL_GPE_PADDING,yPos+TEXTBOX_FONT_SIZE_HEIGHT*i, toolTipLines[i],GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_TOOLTIP,FA_LEFT,FA_TOP);
        }

        gpe->render_rectangle( xPos-GENERAL_GPE_PADDING/2,yPos-GENERAL_GPE_PADDING/2,xPos2,yPos2,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
    }
    tipTipJustUpdated = false;
}

GPE_ScrollBar_XAxis::GPE_ScrollBar_XAxis()
{
    autoCorrect = true;
    elementBox.x = contextRect.x =  fullRect.x = 0;
    elementBox.y = contextRect.y =  fullRect.y = 0;
    elementBox.w = contextRect.w =  fullRect.w = 32;
    elementBox.h = contextRect.h =  fullRect.h = 32;


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

bool GPE_ScrollBar_XAxis::calculate_sizing()
{
    //calculates the height needed to be scrolled.
    if( fullRect.w!=0)
    {
        if( contextRect.w < fullRect.w)
        {
            scrollPercent =  (float)contextRect.w/(float)fullRect.w;
            scrollDisplayPixels = (float)(elementBox.w)*scrollPercent;
            scrollXPos = (float)(contextRect.x)/(float)(fullRect.w)*(float)elementBox.w;
        }
        else
        {
            scrollPercent = 1;
            scrollDisplayPixels = elementBox.w;
            scrollXPos = 0;
            contextRect.x = 0;
            return false;
        }
    }
    return true;
}

int GPE_ScrollBar_XAxis::get_box_width()
{
    return elementBox.w;
}

int GPE_ScrollBar_XAxis::get_box_height()
{
    return elementBox.h;
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

void GPE_ScrollBar_XAxis::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool doWork = true;

    calculate_sizing();


    float prevPosition = scrollXPos;
    hasMoved = false;
    isHeldOn = false;
    if( doWork)
    {
        if( point_within(input->mouse_x,input->mouse_y,viewedSpace->x+elementBox.x-cam->x,viewedSpace->y+elementBox.y-cam->y,viewedSpace->x+elementBox.x+elementBox.w-cam->x,viewedSpace->y+elementBox.y+elementBox.h-cam->y) )
        {
            if( input->check_mouse_pressed(0 ) )
            {
                isOnScrollBox = true;
                isHeldOn = false;
            }
            else if( input->check_mouse_down(0 ) )
            {
                if( !isOnScrollBox)
                {
                    isHeldOn = true;
                }
            }
        }
        else if( input->check_mouse_released(-1) )
        {
            isOnScrollBox = false;
            isHeldOn = false;
        }

        if(viewedSpace->x+elementBox.x-cam->x <= input->mouse_x && input->mouse_x <= viewedSpace->x+elementBox.x+elementBox.w-cam->x )
        {
            if( input->check_mouse_down(0 ) && isOnScrollBox)
            {
                mouseXScrolPos = input->mouse_x - viewedSpace->x-elementBox.x-cam->x; //translates mouse coords to viewed space coords
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

        if( input->check_mouse_released(0) || input->check_mouse_released(1))
        {
            if( isOnScrollBox)
            {
                input->reset_all_input();
            }
            isOnScrollBox = false;
            isHeldOn = false;
        }

        if( autoCorrect)
        {
            if( scrollXPos > elementBox.w- scrollDisplayPixels)
            {
                scrollXPos = elementBox.w- scrollDisplayPixels;
            }

            if( scrollXPos < 0)
            {
                scrollXPos = 0;
            }
        }

        prevPosition = contextRect.x;
        contextRect.x =  ( scrollXPos/elementBox.w )*fullRect.w;

        if( contextRect.x!=prevPosition)
        {
            hasMoved = true;
        }
    }
    else
    {
        scrollPercent = 1;
        scrollDisplayPixels = elementBox.w;
        scrollXPos = 0;
        contextRect.x = 0;
        //hasMoved = true;
    }

}

void GPE_ScrollBar_XAxis::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);

    if( cam!=NULL && viewedSpace!=NULL)
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Color,false);
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Border_Color,true);
        int scrollRenderPixels = scrollDisplayPixels;
        if( scrollRenderPixels < elementBox.h)
        {
            scrollRenderPixels = elementBox.h;
        }
        if( isOnScrollBox )
        {
            gpe->render_rectangle( elementBox.x+scrollXPos-cam->x,elementBox.y-cam->y,elementBox.x+scrollXPos+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Highlight_Color,false);
        }
        else
        {
            gpe->render_rectangle( elementBox.x+scrollXPos-cam->x,elementBox.y-cam->y,elementBox.x+scrollXPos+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Color,false);
        }
        render_animation_ext( GPE_TRIANGLE,2,
                             elementBox.x+elementBox.h/4-cam->x,elementBox.y+elementBox.h/4-cam->y,
                             elementBox.h/2,elementBox.h/2, GPE_MAIN_THEME->Scroll_Box_Arrow_Color );
        render_animation_ext( GPE_TRIANGLE,0,elementBox.x+elementBox.w-elementBox.h*3/4-cam->x,  elementBox.y+elementBox.h/4-cam->y,elementBox.h/2,elementBox.h/2,GPE_MAIN_THEME->Scroll_Box_Arrow_Color );
    }
}

void GPE_ScrollBar_XAxis::reset_scroller()
{
    hasMoved = false;
    isOnScrollBox = false;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 0;
    elementBox.h = 0;
}

GPE_ScrollBar_YAxis::GPE_ScrollBar_YAxis()
{
    autoCorrect = true;
    documentActions = false;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 16;
    elementBox.h = 16;

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

bool GPE_ScrollBar_YAxis::calculate_sizing()
{
    //calculates the height needed to be scrolled.
    if( fullRect.h!=0)
    {
        if( contextRect.h < fullRect.h)
        {
            scrollPercent =  contextRect.h/fullRect.h;
            scrollDisplayPixels = (elementBox.h)*scrollPercent;
            scrollYPos = ( contextRect.y/fullRect.h )*elementBox.h;
        }
        else
        {
            scrollPercent = 1;
            scrollDisplayPixels = elementBox.h;
            scrollYPos = 0;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

int GPE_ScrollBar_YAxis::get_box_width()
{
    return elementBox.w;
}

int GPE_ScrollBar_YAxis::get_box_height()
{
    return elementBox.h;
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

void GPE_ScrollBar_YAxis::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool doWork = true;
    if( viewedSpace!=NULL && cam!=NULL )
    {
        doWork = calculate_sizing();
        hasMoved = false;
        isHeldOn = false;
        if( doWork)
        {
            if( point_between(input->mouse_x,input->mouse_y,elementBox.x+viewedSpace->x-cam->x,elementBox.y+viewedSpace->y-cam->y,elementBox.x+elementBox.w+viewedSpace->x-cam->x,elementBox.y+elementBox.h+viewedSpace->y-cam->y) )
            {
                if( input->check_mouse_pressed(0 ) )
                {
                    isOnScrollBox = true;
                }
            }

            if(elementBox.y+viewedSpace->y-cam->y <= input->mouse_y && input->mouse_y <= elementBox.y+elementBox.h+viewedSpace->y-cam->y )
            {
                if( input->check_mouse_down(0 ) && isOnScrollBox)
                {
                    mouseYScrolPos = input->mouse_y - viewedSpace->y-elementBox.y+cam->y; //translates mouse coords to viewed space coords
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

            if( input->check_mouse_released(0) || input->check_mouse_released(1))
            {
                if( isOnScrollBox)
                {
                    input->reset_all_input();
                }
                isOnScrollBox = false;
                isHeldOn = false;
            }
            float prevPosition = contextRect.y;
            //if( scrollPercent)
            {
                if( scrollYPos+scrollDisplayPixels > elementBox.h )
                {
                    scrollYPos = elementBox.h- scrollDisplayPixels;
                }

                if( scrollYPos < 0)
                {
                    scrollYPos = 0;
                }
            }
            contextRect.y =  ( scrollYPos/elementBox.h )*fullRect.h;

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
            scrollDisplayPixels = elementBox.h;
            scrollYPos = 0;
            contextRect.y = 0;
        }
    }
}

void GPE_ScrollBar_YAxis::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Color,false);
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Border_Color,true);
        int scrollRenderPixels = scrollDisplayPixels;
        if( scrollRenderPixels < elementBox.w)
        {
            scrollRenderPixels = elementBox.w;
        }
        if( isOnScrollBox )
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y+scrollYPos-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+scrollYPos+scrollRenderPixels-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Highlight_Color,false);
        }
        else
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y+scrollYPos-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+scrollYPos+scrollRenderPixels-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Color,false);
        }
        render_animation_ext( GPE_TRIANGLE,1,
                             elementBox.x+elementBox.w/4-cam->x,elementBox.y+elementBox.w/4-cam->y,
                             elementBox.w/2,elementBox.w/2,
                             GPE_MAIN_THEME->Scroll_Box_Arrow_Color);
        render_animation_ext( GPE_TRIANGLE,3,elementBox.x+elementBox.w/4-cam->x,elementBox.y+elementBox.h-elementBox.w*3/4-cam->y,elementBox.w/2,elementBox.w/2,GPE_MAIN_THEME->Scroll_Box_Arrow_Color);
    }
}

void GPE_ScrollBar_YAxis::reset_scroller()
{
    isOnScrollBox = false;
    hasMoved = false;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 0;
    elementBox.h = 0;

}

GPE_GeneralGuiElement::GPE_GeneralGuiElement()
{
    elementBox.x =0;
    elementBox.y = 0;
    elementBox.w = 0;
    elementBox.h = 0;
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
    return elementBox.h;
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
    return elementBox.w;
}

int GPE_GeneralGuiElement::get_xpos()
{
    return elementBox.x;

}

int GPE_GeneralGuiElement::get_x2pos()
{
    return elementBox.x+elementBox.w;
}

int GPE_GeneralGuiElement::get_ypos()
{
    return elementBox.y;
}

int GPE_GeneralGuiElement::get_y2pos()
{
    return elementBox.y+elementBox.h;
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
        elementBox.x += newX;
    }
    if(newY!=0)
    {
        elementBox.y += newY;
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
            if( point_between(input->mouse_x,input->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
            {
                if (point_between(input->mouse_x,input->mouse_y,elementBox.x+viewedSpace->x-cam->x,elementBox.y+viewedSpace->y-cam->y,elementBox.x+elementBox.w+viewedSpace->x-cam->x,elementBox.y+elementBox.h+viewedSpace->y-cam->y) )
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
                    if( input->check_mouse_released(0) )
                    {
                        isClicked = true;
                        isInUse = true;
                    }

                    if( input->check_mouse_pressed(1) )
                    {
                        isRightClicked = true;
                        isInUse = true;
                    }
                }
                else if( input->check_mouse_pressed(-1) || input->check_mouse_released(-1) )
                {
                    clickedOutside = true;
                    isInUse = false;
                    hasScrollControl = false;
                    hasArrowkeyControl = false;
                }
            }
            else if( input->check_mouse_pressed(-1)  || input->check_mouse_released(-1) )
            {
                clickedOutside = true;
                isInUse = false;
                hasScrollControl = false;
                hasArrowkeyControl = false;
            }
        }
    }
}

void GPE_GeneralGuiElement::prerender_self( )
{

}

void GPE_GeneralGuiElement::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{

}

void GPE_GeneralGuiElement::scale_height(double scaleH)
{
    set_height( elementBox.h * scaleH);
}

void GPE_GeneralGuiElement::scale_width(double scaleW)
{
    set_width( elementBox.w* scaleW);
}

void GPE_GeneralGuiElement::set_name(std::string newName)
{
    opName = newName;
}

void GPE_GeneralGuiElement::set_coords(int newX, int newY)
{
    if( newX!=-1)
    {
        elementBox.x = newX;
    }
    if(newY!=-1)
    {
        elementBox.y = newY;
    }
}

void GPE_GeneralGuiElement::set_max_width( int nMW)
{
    maxWidth = nMW;
    if( maxWidth > 0 && elementBox.w > maxWidth)
    {
        elementBox.w = maxWidth;
    }
}

void GPE_GeneralGuiElement::set_max_height( int nMH)
{
    maxHeight = nMH;
    if( maxHeight > 0 && elementBox.h > maxHeight)
    {
        elementBox.h = maxHeight;
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
    elementBox.h = newHeight;
    /*if(elementBox.y+elementBox.h > SCREEN_HEIGHT)
    {
        elementBox.h = SCREEN_HEIGHT-elementBox.y;
    }*/
}

void GPE_GeneralGuiElement::set_hovered(bool hoverVal)
{
    isHovered = hoverVal;
}

void GPE_GeneralGuiElement::set_width(int newWid)
{
    elementBox.w = newWid;
}

GPE_GuiElementRow::GPE_GuiElementRow()
{
    inDebugMode = false;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 0;
    elementBox.h = 0;
    hAlign = FA_LEFT;
    vAlign = FA_TOP;
    barXPadding = 0;
    barYPadding = 0;
    outterWidth = 0;
    outterHeight = 0;
    /*barXMargin = 0;
    barYMargin = 0;*/
}

GPE_GuiElementRow::~GPE_GuiElementRow()
{
    clear_list();
}

void GPE_GuiElementRow::add_gui_element(GPE_GeneralGuiElement *  newElement )
{
    subOptions.push_back( newElement );
}

void GPE_GuiElementRow::clear_list()
{
    /*
    GPE_GeneralGuiElement * tempItem = NULL;
    for( int i = 0; (int)subOptions.size(); i++)
    {
        tempItem = subOptions[i];
        if( tempItem!=NULL )
        {
            delete tempItem;
            tempItem = NULL;
        }
    }*/
    subOptions.clear();
}

//Correct width issue
void GPE_GuiElementRow::set_coords(int newX, int newY)
{
    elementBox.x = newX;
    elementBox.y = newY;
    elementBox.w = 0;
    elementBox.h = 0;
    //For now we just gonna assume is all fa_left until its time to fix it
    GPE_GeneralGuiElement * cContainer = NULL;
    int tempX = newX;
    int tempY = newY;//+barYPadding;
    int i;
    int optionsSize = (int)subOptions.size();
    int maxHeight = 0;

    //Does horizontal align calculations
    if( hAlign==FA_RIGHT || hAlign == FA_CENTER  )
    {
        int foundRowWidth = 0;
        for( i=0; i<optionsSize;i++)
        {
            cContainer = subOptions[i];
            if(cContainer!=NULL)
            {
                if( cContainer->get_element_type() == "paragraph" )
                {
                    cContainer->set_width(elementBox.w - (barXPadding)*2 );
                    foundRowWidth = cContainer->get_width();
                    break;
                }
                else
                {
                    foundRowWidth+=cContainer->get_width()+barXPadding;
                }
            }
        }

        if( hAlign==FA_RIGHT)
        {
            tempX += outterWidth - foundRowWidth;
        }
        else if(hAlign == FA_CENTER  )
        {
            tempX += abs( outterWidth - foundRowWidth )/2;
        }
    }
    else
    {
        //defaults to left align( FA_LEFT )
        elementBox.w = outterWidth;
    }

    //Does vertical align calculations
    //and actually sets coordinates
    int cHeight = 0;
    elementBox.w = 0;
    if( vAlign == FA_MIDDLE)
    {
        for( i = 0; i < optionsSize; i++)
        {
            cContainer = subOptions[i];
            if( cContainer!=NULL )
            {
                cHeight = cContainer->get_height();
                if(cContainer->autoResizes)
                {
                    //cContainer->set_width( elementBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }

                if( cHeight > elementBox.h)
                {
                    elementBox.h = cContainer->get_height();
                    cContainer->set_coords(tempX, tempY  );
                }
                else
                {
                    cContainer->set_coords(tempX, tempY + abs( elementBox.h - cHeight ) /2 );
                }
                tempX+=barXPadding+cContainer->get_width();
                elementBox.w+= barXPadding + cContainer->get_width();
            }
        }
    }
    else if( vAlign == FA_BOTTOM )
    {
        for( i = 0; i < optionsSize; i++)
        {
            cContainer = subOptions[i];
            if( cContainer!=NULL )
            {
                cHeight = cContainer->get_height();
                if(cContainer->autoResizes)
                {
                    //cContainer->set_width( elementBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                if( cHeight > elementBox.h)
                {
                    elementBox.h = cHeight;
                    cContainer->set_coords(tempX, tempY );
                }
                else
                {
                    cContainer->set_coords(tempX, tempY + (elementBox.h - cHeight) );
                }
                tempX+=barXPadding+cContainer->get_width();
                elementBox.w+= barXPadding + cContainer->get_width();
            }
        }
    }
    else
    {
        //defaults to FA_TOP
        for( i = 0; i < optionsSize; i++)
        {
            cContainer = subOptions[i];
            if( cContainer!=NULL )
            {
                cContainer->set_coords(tempX, tempY );
                if(cContainer->autoResizes)
                {
                    //cContainer->set_width( elementBox.w - (barXPadding+barXMargin)*2 - yScroll->get_box_width() );
                }
                tempX+=barXPadding+cContainer->get_width();
                if( cContainer->get_height() > elementBox.h)
                {
                    elementBox.h = cContainer->get_height();
                }
                elementBox.w+= barXPadding + cContainer->get_width();
            }
        }
    }
}

void GPE_GuiElementRow::set_full_width( int maxRowWidth )
{

}

void GPE_GuiElementRow::set_horizontal_align(int hValue)
{
    hAlign = hValue;
}

void GPE_GuiElementRow::set_vertical_align(int vValue)
{
    vAlign = vValue;
}

void GPE_GuiElementRow::set_full_width()
{

}

void GPE_GuiElementRow::set_maxed_out_width()
{

}

void set_maxed_out_height()
{

}

GPE_GuiElementList::GPE_GuiElementList()
{
    inDebugMode = false;
    rowWithViewId = 0;
    optionWithViewId = 0;
    newRowRequested = false;
    hideXScroll = true;
    hideYScroll = true;
    cameraBox.x = menuBox.x = entireBox.x = elementBox.x = 0;
    cameraBox.y = menuBox.y = entireBox.y = elementBox.y = 0;
    cameraBox.w = menuBox.w = entireBox.w = elementBox.w;
    cameraBox.h = menuBox.h = entireBox.h = elementBox.h = RESOURCE_AREA_HEIGHT*3;
    xScroll = new GPE_ScrollBar_XAxis();
    yScroll = new GPE_ScrollBar_YAxis();
    cameraBox.w = menuBox.w = entireBox.w = elementBox.w - yScroll->get_box_width();
    barXPadding = GENERAL_GPE_PADDING*2;
    barYPadding = GENERAL_GPE_PADDING*2;
    barXMargin = 0;
    barYMargin = 0;
    isInUse = true;
    subElementsHasArrowControl = false;
    subElementsIsScrolling = false;
    hAlign = FA_LEFT;
    vAlign = FA_TOP;
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
    clear_list();
}

void GPE_GuiElementList::add_gui_element(GPE_GeneralGuiElement *  newElement, bool isNLElement)
{
    if( newElement!=NULL)
    {
        GPE_GuiElementRow *  currentRow = NULL;
        newElement->hasLineBreak = isNLElement;
        int rowSize = (int)subRows.size();

        if(  rowSize == 0 || newRowRequested )
        {
            currentRow = new GPE_GuiElementRow();
            currentRow->hAlign = hAlign;
            currentRow->vAlign = vAlign;
            subRows.push_back( currentRow );
        }
        else
        {
            currentRow = subRows[ rowSize -1 ];
        }

        if( currentRow !=NULL )
        {
            currentRow->add_gui_element( newElement );
        }
        //Reverts to old method temporarily
        allElements.push_back( newElement );
        if( newElement->hasLineBreak || newElement->get_element_type() == "labelimage" || newElement->get_element_type() == "paragraph" )
        {
            isNLElement = true;
        }
        newRowRequested = isNLElement;
    }
}

void GPE_GuiElementList::clear_list()
{
    allElements.clear();
    cameraBox.w = menuBox.w = entireBox.w = elementBox.w;
    cameraBox.h = menuBox.h = entireBox.h = elementBox.h;

    GPE_GuiElementRow * tempRow = NULL;
    for( int i = (int)subRows.size()-1; i>=0; i--)
    {
        tempRow = subRows[i];
        if( tempRow!=NULL )
        {
            delete tempRow;
            tempRow = NULL;
        }
    }
    subRows.clear();
    newRowRequested = true;
    subElementsHasArrowControl = false;
    subElementsIsScrolling = false;
    newRowRequested = false;
    rowWithViewId = -1;
    optionWithViewId = -1;
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
    else
    {
        cameraBox.x = 0;
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
    if( inDebugMode)
    {
        record_error("Starting to process GPE_GuiElementList...");
    }
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    xScroll->set_coords( elementBox.x, elementBox.h-16);
    xScroll->set_width( elementBox.w );
    xScroll->set_height( 16);

    yScroll->set_coords( elementBox.w-16, elementBox.y );
    yScroll->set_width( 16 );
    yScroll->set_height( elementBox.h );
     //Handles row / option in view
    GPE_GuiElementRow * cRow = NULL;
    GPE_GeneralGuiElement * cContainer = NULL;
    GPE_GeneralGuiElement * containerInControl = NULL;
    int lastContainerRowControlId = -1;
    int lastContainerInControlId = -1;
    int i = 0, j = 0;
    int pastRowChange = 0;
    int currentRowCount = (int)subRows.size();
    int rowOptionsCount = 0;
    if( inDebugMode)
    {
        record_error("["+int_to_string(currentRowCount)+"] unique rows gathered...");
    }
    if( isClicked )
    {
        //if( RESOURCE_TO_DRAG==NULL)
        {
            isInUse = true;
            hasScrollControl = true;
            hasArrowkeyControl = true;
        }
    }
    else if( clickedOutside )
    {
        isInUse = false;
        hasScrollControl = false;
        hasArrowkeyControl = false;
    }
    else if( !isInUse )
    {
        hasScrollControl = false;
    }
    entireBox.x = 0;
    entireBox.y = 0;
    entireBox.w = (barXMargin)*2;
    entireBox.h = (barYMargin)*2;

    menuBox.x = elementBox.x+viewedSpace->x;
    menuBox.y = elementBox.y+viewedSpace->y;
    if( hideXScroll )
    {
        cameraBox.w = menuBox.w = elementBox.w;
    }
    else
    {
        cameraBox.w = menuBox.w = elementBox.w-16;
    }

    if( hideYScroll)
    {
        cameraBox.h = menuBox.h = elementBox.h;
    }
    else
    {
        cameraBox.h = menuBox.h = elementBox.h-16;
    }
    subElementsHasArrowControl = false;
    subElementsIsScrolling = false;

    bool scrolingHasntOccurred = false;
    if( hideXScroll!=true && hideYScroll!=true )
    {
        scrolingHasntOccurred = true;
    }
    else if ( xScroll->is_scrolling()==false && yScroll->is_scrolling()==false  )
    {
        scrolingHasntOccurred = true;
    }


    //Handles keyboard/gamepad input as well as delays
    if( isInUse && hasArrowkeyControl )
    {
        if( input->down[kb_tab] && input->pressed[kb_tab]==false && input->released[kb_tab]==false )
        {
            tabDelay += 0.5;
        }
        else
        {
            tabDelay = -1;
        }
        if( input->down[kb_left] && input->pressed[kb_left]==false && input->released[kb_left]==false )
        {
            leftDelay+=1;
        }
        else
        {
            leftDelay = -1;
        }
        if( input->down[kb_right] && input->pressed[kb_right]==false && input->released[kb_right]==false )
        {
            rightDelay+=1;
        }
        else
        {
            rightDelay = -1;
        }
        if( input->down[kb_up] && input->pressed[kb_up]==false && input->released[kb_up]==false )
        {
            upDelay+=1;
        }
        else
        {
            upDelay = -1;
        }
        if( input->down[kb_down] && input->pressed[kb_down]==false && input->released[kb_down]==false )
        {
            downDelay+=1;
        }
        else
        {
            downDelay = -1;
        }
        if( input->down[kb_comma] && input->pressed[kb_comma]==false && input->released[kb_comma]==false )
        {
            leserKeyDelay+=1;
        }
        else
        {
            leserKeyDelay = -1;
        }

        if( input->down[kb_period] && input->pressed[kb_period]==false && input->released[kb_period]==false )
        {
            greaterKeyDelay+=1;
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

    if( inDebugMode)
    {
        record_error("[Calculating box size...");
    }

    int xPos = barXMargin+barXPadding;
    int yPos = barYMargin+barYPadding;
    int y2Pos = yPos;
    int rowWidth = 0;
    int maxRowWidth = 0;
    int totalMaxRowWidth = 0;
    int rowHeight = 0;
    //Sets up the coordinates for the rows and their content
    xPos = barXMargin+barXPadding;
    for( i=0; i < currentRowCount; i++)
    {
        cRow = subRows[i];
        if( cRow!=NULL )
        {
            cRow->barXPadding = barXPadding;
            cRow->barYPadding = barYPadding;

            cRow->outterWidth = menuBox.w;
            cRow->outterHeight = menuBox.h;
            cRow->set_coords(xPos, y2Pos );

            rowHeight = cRow->get_height();
            y2Pos+=rowHeight+barYPadding;
            rowWidth = cRow->get_width();
            if( maxRowWidth < rowWidth)
            {
                maxRowWidth = rowWidth;
            }
        }
    }

    entireBox.w = maxRowWidth + (barXMargin+barXPadding)*2;
    entireBox.h = y2Pos + barYMargin;

    if( entireBox.w > menuBox.w )
    {
        hideXScroll = false;
    }
    else
    {
        hideXScroll = true;
    }

    if( entireBox.h > menuBox.h )
    {
        hideYScroll = false;
    }
    else
    {
        hideYScroll = true;
    }
    if( inDebugMode)
    {
        record_error("["+int_to_string(entireBox.w)+"] X ["+int_to_string(entireBox.h)+"] size...");
    }
    //Processes each container if they are in focus or not...
    if( scrolingHasntOccurred )
    {
        if( inDebugMode)
        {
            record_error("Checking rolls for scrolling..." );
        }
        for( i=0; i < currentRowCount; i++ )
        {
            cRow = subRows[i];
            if( cRow!=NULL )
            {
                rowOptionsCount = (int)cRow->subOptions.size();
                for( j=0; j < rowOptionsCount; j++ )
                {
                    cContainer = cRow->subOptions[j];
                    if( cContainer!=NULL )
                    {
                        if( cContainer->hasScrollControl )
                        {
                            subElementsIsScrolling = true;
                            containerInControl = cContainer;
                            lastContainerRowControlId = i;
                            lastContainerInControlId = j;
                            break;
                        }
                        if( cContainer->hasArrowkeyControl )
                        {
                            subElementsHasArrowControl = true;
                            containerInControl = cContainer;
                            lastContainerRowControlId = i;
                            lastContainerInControlId = j;
                            break;
                        }
                        if( cContainer->is_inuse() )
                        {
                            rowWithViewId = i;
                            optionWithViewId = j;
                            containerInControl = cContainer;
                            lastContainerRowControlId = i;
                            lastContainerInControlId = j;
                            break;
                        }
                    }
                }
            }
        }
        if( inDebugMode)
        {
            record_error("Done..." );
        }
        //locks on one gui item until it is taken out of focus...
        if( containerInControl!=NULL )
        {
            if( inDebugMode)
            {
                record_error("Processing selected container..." );
                record_error("Trying to process "+cContainer->get_element_type() +"..." );
            }
            containerInControl->process_self(&menuBox,&cameraBox);
            if( containerInControl->hasScrollControl==false && containerInControl->hasArrowkeyControl )
            {
                containerInControl = NULL;
                lastContainerInControlId = -1;
                lastContainerRowControlId = -1;
            }
            if( containerInControl!=NULL && containerInControl->is_inuse() == false )
            {
                containerInControl = NULL;
                lastContainerInControlId = -1;
                lastContainerRowControlId = -1;
            }
            if( inDebugMode)
            {
                record_error("Done..." );
            }
        }
        //else
        if( inDebugMode)
        {
            record_error("Processing other containers..." );
        }
        bool canProcessSelf = true;
        for( i=0; i < currentRowCount; i++)
        {
            cRow = subRows[i];
            if( cRow!=NULL )
            {
                rowOptionsCount = (int)cRow->subOptions.size();
                for( j=0; j < rowOptionsCount; j++)
                {
                    cContainer = cRow->subOptions[j];
                    if(cContainer!=NULL)
                    {
                        canProcessSelf = true;
                        if( containerInControl != NULL && lastContainerRowControlId == i && lastContainerInControlId == j )
                        {
                            canProcessSelf = false;
                        }

                        if( canProcessSelf)
                        {
                            //record_error("Trying to process "+cContainer->get_element_type() +"..." );
                            if( inDebugMode)
                            {
                                record_error("Trying to process "+cContainer->get_element_type() +"..." );
                            }
                            cContainer->process_self(&menuBox,&cameraBox);
                            if( cContainer->hasScrollControl )
                            {
                                subElementsIsScrolling=true;
                                containerInControl = cContainer;

                            }
                            if( cContainer->hasArrowkeyControl )
                            {
                                subElementsHasArrowControl=true;
                                containerInControl = cContainer;
                            }
                            if( cContainer->is_inuse() )
                            {
                                optionWithViewId = j;
                                rowWithViewId = i;
                                containerInControl = cContainer;
                            }
                        }
                    }
                }
            }
        }
        if( inDebugMode)
        {
            record_error("Done..." );
        }
    }

    //if(  isHovered || (isInUse && subElementsIsScrolling==false && hasScrollControl ) )
    if( isHovered && subElementsIsScrolling==false )
    {
        if( input->mouseScrollingUp )
        {
            if( input->shiftKeyIsPressed)
            {
                scroll_left( cameraBox.w/4 );
            }
            else
            {
                scroll_up( cameraBox.h/4 );
            }
        }
        else if( input->mouseScrollingDown )
        {
            if( input->shiftKeyIsPressed )
            {
                scroll_right( cameraBox.w/4 );
            }
            else
            {
                scroll_down( cameraBox.h/4 );
            }
        }
    }

    bool directionChangeRequested = false;

    if( hasScrollControl && subElementsIsScrolling==false)
    {
        if( tabDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( !input->pressed[kb_tab] && input->released[kb_tab] ) && input->check_keyboard_down(kb_ctrl)==false )
        {
            if( input->shiftKeyIsPressed)
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
        else if( input->check_keyboard_down(kb_ctrl) )
        {
            if( leserKeyDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( !input->pressed[kb_comma] && input->released[kb_comma] ) )
            {
                optionWithViewId--;
                directionChangeRequested = true;
                leserKeyDelay = -1;
            }
            else if( greaterKeyDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( !input->pressed[kb_period] && input->released[kb_period] ) )
            {
                optionWithViewId++;
                directionChangeRequested = true;
                greaterKeyDelay = -1;
            }
        }

        if( subElementsHasArrowControl==false)
        {
            if( upDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( !input->pressed[kb_up] && input->released[kb_up] ) )
            {
                scroll_up( cameraBox.h/4 );
                upDelay = 0;
            }
            else if( downDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( input->pressed[kb_down] && !input->released[kb_down] ) )
            {
                scroll_down( cameraBox.h/4 );
                downDelay = 0;
            }
        }
    }
    else if( hasScrollControl && subElementsIsScrolling==false)
    {
        if( input->check_keyboard_down(kb_ctrl) )
        {
            if( leserKeyDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( input->pressed[kb_comma] && !input->released[kb_comma] ) && input->check_keyboard_down(kb_ctrl)==false )
            {
                optionWithViewId--;
                directionChangeRequested = true;
                leserKeyDelay = -1;
            }
            else if( greaterKeyDelay > (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO || ( input->pressed[kb_period] && !input->released[kb_period] ) && input->check_keyboard_down(kb_ctrl)==false )
            {
                optionWithViewId++;
                directionChangeRequested = true;
                greaterKeyDelay = -1;
            }
        }
    }

    if( rowWithViewId < 0 )
    {
        rowWithViewId = currentRowCount-1;
        optionWithViewId = 0;
    }

    if( rowWithViewId >=0 && rowWithViewId < currentRowCount )
    {
        selectedRow = subRows[rowWithViewId];
        if( selectedRow!=NULL )
        {
            //goes up a row or loops to bottom of content
            if( optionWithViewId < 0 )
            {
                rowWithViewId--;
                if( rowWithViewId >=0 )
                {
                    selectedRow = subRows[rowWithViewId];
                }
                else if( currentRowCount> 0 )
                {
                    rowWithViewId = currentRowCount-1;
                    selectedRow = subRows[rowWithViewId];
                }

                if( selectedRow !=NULL )
                {
                    optionWithViewId = (int)selectedRow->subOptions.size() -1;
                }
                else
                {
                    optionWithViewId = 0;
                }

                /*if( rowWithViewId < 0)
                {
                    rowWithViewId = 0;
                }
                else
                {
                    rowWithViewId = currentRowCount-1;
                }
                if( rowWithViewId < 0)
                {
                    rowWithViewId= 0;
                }
                */
            }
            else if( optionWithViewId >= (int)selectedRow->subOptions.size() )
            {
                optionWithViewId = 0;
                //goes down a row
                rowWithViewId++;
            }

            if( rowWithViewId >= currentRowCount)
            {
                optionWithViewId = 0;
                rowWithViewId = 0;
            }
        }
    }
    else
    {
        rowWithViewId = 0;
        optionWithViewId = 0;
    }
    //Revert to (0,0) if row is out of bounds somehow magically...
    if( rowWithViewId >= currentRowCount || rowWithViewId < 0 )
    {
        rowWithViewId = 0;
        optionWithViewId = 0;
    }

    if( directionChangeRequested)
    {
        for( i =0; i < (int)currentRowCount; i++)
        {
            cRow = subRows[i];
            if( cRow!=NULL )
            {
                rowOptionsCount = (int)cRow->subOptions.size();
                for( j =0; j < rowOptionsCount; j++)
                {
                    cContainer = cRow->subOptions[j];
                    if(cContainer!=NULL)
                    {
                        if( rowWithViewId == i && optionWithViewId == j )
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
        }
    }

    xScroll->elementBox.x = elementBox.x;
    xScroll->elementBox.y = elementBox.y+elementBox.h-16;
    xScroll->elementBox.w = elementBox.w-16;
    xScroll->elementBox.h = 16;

    xScroll->fullRect.x = 0;
    xScroll->fullRect.y = 0;
    xScroll->fullRect.w = entireBox.w;
    xScroll->fullRect.h = entireBox.h;

    xScroll->contextRect.x = cameraBox.x;
    xScroll->contextRect.y = cameraBox.y;
    xScroll->contextRect.w = cameraBox.w;
    xScroll->contextRect.h = cameraBox.h;

    update_rectangle(&yScroll->elementBox, elementBox.x+elementBox.w-16, elementBox.y, 16,elementBox.h);
    update_rectangle(&yScroll->fullRect, 0, 0, entireBox.w,entireBox.h);
    update_rectangle(&yScroll->contextRect, cameraBox.x, cameraBox.y, cameraBox.w,cameraBox.h);

    if( hasScrollControl && subElementsHasArrowControl==false &&  xScroll!=NULL && yScroll!=NULL)
    {
        if( entireBox.w >=elementBox.w  && hideXScroll!=true )
        {
            xScroll->process_self(viewedSpace,cam);
        }
        else
        {
            cameraBox.x =0;
            cameraBox.w = menuBox.w = elementBox.w;
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

        if( hideYScroll!=true )
        {
            yScroll->autoCorrect = false;
            yScroll->process_self(viewedSpace,cam );
        }
        else
        {
            cameraBox.y = 0;
            cameraBox.w = menuBox.w = elementBox.w;
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
    else
    {
        xScroll->process_self(viewedSpace, cam);
        yScroll->process_self(viewedSpace, cam);
    }
}

void GPE_GuiElementList::render_self(GPE_Rect * viewedSpace,GPE_Rect * cam , bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        menuBox.w+=16;
        menuBox.h+=16;
        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint( &menuBox);

        GPE_GuiElementRow * cRow = NULL;
        GPE_GeneralGuiElement * cResource = NULL;
        int i = 0,j = 0;
        for( i=0; i<(int)subRows.size(); i++)
        {
            cRow = subRows[i];
            if(cRow!=NULL)
            {
                for( j =0; j < (int)cRow->subOptions.size(); j++ )
                {
                    cResource = cRow->subOptions[j];
                    if(cResource!=NULL)
                    {
                        cResource->render_self( &menuBox,&cameraBox,forceRedraw);
                    }
                }
            }
        }

        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint( viewedSpace);
        menuBox.w-=16;
        menuBox.h-=16;
        if( xScroll!=NULL && forceRedraw && hideXScroll!=true )
        {
            //if( entireBox.w >elementBox.w)
            {
                xScroll->render_self( viewedSpace,cam);
            }
        }
        if( yScroll!=NULL && forceRedraw && hideYScroll!=true )
        {
            if( entireBox.h >elementBox.h )
            {
                yScroll->render_self( viewedSpace,cam);
            }
        }

        /*if( isInUse && subElementsIsScrolling==false && hasScrollControl && forceRedraw)
        {
            gpe->render_rect( &elementBox,GPE_MAIN_THEME->Button_Box_Highlighted_Color,true);
        }
        else
        {
            gpe->render_rect( &elementBox,GPE_MAIN_THEME->Main_Box_Faded_Color,true);
        }*/
        //if( forceRedraw)
        {
            gpe->render_rect( &elementBox,GPE_MAIN_THEME->Main_Border_Color,true);
        }
    }
}

void GPE_GuiElementList::reset_self()
{
    clear_list();
    cameraBox.x = menuBox.x = entireBox.x = 0;
    cameraBox.y = menuBox.y = entireBox.y = 0;
    cameraBox.w = menuBox.w = entireBox.w = 32;
    cameraBox.h = menuBox.h = entireBox.h = RESOURCE_AREA_HEIGHT*3;
}

void GPE_GuiElementList::set_horizontal_align(int hValue)
{
    hAlign = hValue;
    GPE_GuiElementRow * tguiRow = NULL;
    for( int i = (int)subRows.size()-1; i>=0; i--)
    {
        tguiRow = subRows[i];
        if( tguiRow!=NULL )
        {
            tguiRow->hAlign = hValue;
        }
    }
}

void GPE_GuiElementList::set_full_width()
{
    GPE_GuiElementRow * tguiRow = NULL;
    int maxWidth = elementBox.w-(barXMargin+barXPadding)*3;
    for( int i = (int)subRows.size()-1; i>=0; i--)
    {
        tguiRow = subRows[i];
        if( tguiRow!=NULL )
        {
            tguiRow->set_width(maxWidth);
        }
    }
}

void GPE_GuiElementList::set_maxed_out_width()
{
    GPE_GuiElementRow * tguiRow = NULL;
    int i = 0;
    int maxWidth = 0;
    for( i = (int)subRows.size()-1; i>=0; i--)
    {
        tguiRow = subRows[i];
        if( tguiRow!=NULL )
        {
            if( tguiRow->get_width() > maxWidth)
            {
                maxWidth = tguiRow->get_width();
            }
        }
    }

    for( i = (int)subRows.size()-1; i>=0; i--)
    {
        tguiRow = subRows[i];
        if( tguiRow!=NULL )
        {
            tguiRow->set_width(maxWidth);
        }
    }
}

void GPE_GuiElementList::set_maxed_out_height()
{
    GPE_GuiElementRow * tguiRow = NULL;
    int i = 0;
    int maxHeight = 0;
    for( i = (int)subRows.size()-1; i>=0; i--)
    {
        tguiRow = subRows[i];
        if( tguiRow!=NULL )
        {
            if( tguiRow->get_height() > maxHeight)
            {
                maxHeight = tguiRow->get_height();
            }
        }
    }

    for( i = (int)subRows.size()-1; i>=0; i--)
    {
        tguiRow = subRows[i];
        if( tguiRow!=NULL )
        {
            tguiRow->set_height(maxHeight);
        }
    }
}

GPE_Slider_XAxis::GPE_Slider_XAxis( int sVal, int sMin, int sMax )
{
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    elementBox.h = 24;
    scrollDisplayPixels = 24;
    currentSliderXPos = 0;
    isHeldOn = false;
    hasMoved = false;
    isOnScrollBox = false;
    mouseXScrolPos = 0;
    //Will let the function do optimizations needed below
    minValue = sMin;
    maxValue = sMax;
    myValue = sVal;
    set_data( sVal, sMin, sMax );
}

GPE_Slider_XAxis::~GPE_Slider_XAxis()
{

}

int GPE_Slider_XAxis::get_value()
{
    return myValue;
}
bool GPE_Slider_XAxis::has_moved()
{
    return hasMoved;
}

bool GPE_Slider_XAxis::is_held()
{
    return isHeldOn;
}

bool GPE_Slider_XAxis::is_scrolling()
{
    return isOnScrollBox;
}

void GPE_Slider_XAxis::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool doWork = false;
    //calculates the height needed to be scrolled.
    hasMoved = false;
    isHeldOn = false;

    if( point_within(input->mouse_x,input->mouse_y,viewedSpace->x+elementBox.x-cam->x,viewedSpace->y+elementBox.y-cam->y,viewedSpace->x+elementBox.x+elementBox.w-cam->x,viewedSpace->y+elementBox.y+elementBox.h-cam->y) )
    {
        if( input->check_mouse_pressed(0 ) )
        {
            isOnScrollBox = true;
            isHeldOn = false;
            hasArrowkeyControl = true;
            hasScrollControl = true;
        }
        else if( input->check_mouse_down(0 ) )
        {
            if( !isOnScrollBox)
            {
                isHeldOn = true;
                hasArrowkeyControl = true;
                hasScrollControl = true;
            }
        }
    }
    else if( input->check_mouse_released(-1) )
    {
        isOnScrollBox = false;
        isHeldOn = false;
        hasArrowkeyControl = false;
        hasScrollControl = false;
    }

    if(viewedSpace->x+elementBox.x-cam->x <= input->mouse_x && input->mouse_x <= viewedSpace->x+elementBox.x+elementBox.w-cam->x )
    {
        if( input->check_mouse_down(0 ) && isOnScrollBox)
        {
            mouseXScrolPos = input->mouse_x - viewedSpace->x-elementBox.x-cam->x; //translates mouse coords to viewed space coords
            if( isOnScrollBox)
            {
                currentSliderXPos = mouseXScrolPos;
                doWork = true;
            }
        }
    }
    else
    {
        isOnScrollBox = false;
        isHeldOn = false;
    }


    if( input->check_mouse_released(0) || input->check_mouse_released(1))
    {
        if( isOnScrollBox)
        {
            input->reset_all_input();
        }
        isOnScrollBox = false;
        isHeldOn = false;
    }


    if( currentSliderXPos > elementBox.w )
    {
        currentSliderXPos = elementBox.w;
        doWork = true;
    }

    if( currentSliderXPos < 0)
    {
        currentSliderXPos = 0;
        doWork = true;
    }

    if( doWork )
    {
        set_value( minValue+ ( ( (double)currentSliderXPos)/elementBox.w )*( maxValue-minValue ) );
    }

    if( hasArrowkeyControl )
    {
        int scrollRenderPixels = 8;

        if( input->check_keyboard_down(kb_left) && !input->check_keyboard_pressed(kb_left)  )
        {
            set_value( myValue-scrollRenderPixels );
        }
        else if( input->check_keyboard_down(kb_right)&& !input->check_keyboard_pressed(kb_right)   )
        {
            set_value( myValue+scrollRenderPixels );
        }
    }
}

void GPE_Slider_XAxis::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && cam!=NULL && viewedSpace!=NULL )
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y+4,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y-4 ,GPE_MAIN_THEME->Scroll_Box_Color,false);
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y+4,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y-4 ,GPE_MAIN_THEME->Scroll_Box_Border_Color,true);

        int scrollRenderPixels = 24;
        if( scrollRenderPixels < elementBox.h)
        {
            scrollRenderPixels = elementBox.h;
        }
        int scrollPreviewX = std::min( currentSliderXPos, (int)(elementBox.w-scrollRenderPixels) );

        if( isOnScrollBox )
        {
            gpe->render_rectangle( elementBox.x+scrollPreviewX-cam->x,elementBox.y-cam->y,elementBox.x+scrollPreviewX+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Highlight_Color,false);
        }
        else if( hasArrowkeyControl)
        {
            gpe->render_rectangle( elementBox.x+scrollPreviewX-cam->x,elementBox.y-cam->y,elementBox.x+scrollPreviewX+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y, c_red,false );
        }
        else
        {
            gpe->render_rectangle( elementBox.x+scrollPreviewX-cam->x,elementBox.y-cam->y,elementBox.x+scrollPreviewX+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Color,false);
        }
    }
}

void GPE_Slider_XAxis::set_data( int sVal  , int sMin , int sMax  )
{
    minValue = sMin;
    maxValue = sMax;
    if( sMin > sMax)
    {
        minValue = sMax;
        maxValue = sMin;
    }
    set_value( sVal);
}

bool GPE_Slider_XAxis::set_value( int sVal )
{
    myValue = sVal;
    if( myValue < minValue)
    {
        myValue = minValue;
    }
    else if( maxValue < myValue )
    {
        myValue = maxValue;
    }
    currentSliderXPos =  ( elementBox.w )*( ( (float)myValue-(float)minValue) / ( (float)maxValue-(float)minValue ) );
}

GPE_TabBar::GPE_TabBar()
{
    tabIsRightClicked = false;
    isInUse = false;
    guiListTypeName = "tabbar";
    elementBox.x = 16;
    elementBox.y = 16;
    elementBox.w = 0;
    elementBox.h = 28;

    barXPadding = GENERAL_GPE_PADDING;
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
        elementBox.w+=fontTextWidth*(int)newOption.size()+barXPadding;
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
    if( elementBox.w!=0 && viewedSpace!=NULL && cam!=NULL)
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
                int cTabXPos = elementBox.x+viewedSpace->x-cam->x;
                int cTabX2Pos = elementBox.x+viewedSpace->x-cam->x;
                int cTabYPos = elementBox.y+viewedSpace->y-cam->y;
                int cTabY2Pos = elementBox.y+elementBox.h+viewedSpace->y-cam->y;
                for(int i=0; i< (int)subOptions.size(); i++)
                {
                    cTabX2Pos+=fontTextWidth*(int)subOptions[i].size()+barXPadding;
                    if(point_within(input->mouse_x,input->mouse_y,cTabXPos,cTabYPos,cTabX2Pos,cTabY2Pos) )
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
            if( input->check_keyboard_released(kb_left) &&  tabInUse > 0)
            {
                tabInUse--;
            }
            else if( input->check_keyboard_released(kb_right) && tabInUse < (int)subOptions.size()-1 )
            {
                tabInUse++;
            }
        }
    }
}

void GPE_TabBar::render_self( GPE_Rect * viewedSpace,GPE_Rect * cam , bool forceRedraw)
{
    //gpe->render_rect(&elementBox,barColor,false);
    //gpe->render_rect(&elementBox,barOutlineColor,true);
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(forceRedraw && viewedSpace!=NULL && cam!=NULL)
    {
        if( (int)subOptions.size() >0 )
        {
            int cTabXPos = elementBox.x-cam->x;
            int cTabX2Pos = elementBox.x-cam->x;
            int thisTabWidth = 0;
            for(int i=0; i< (int)subOptions.size(); i++)
            {
                thisTabWidth=fontTextWidth*(int)subOptions[i].size()+barXPadding;
                cTabX2Pos+=thisTabWidth;
                if( tabInUse==i)
                {
                    gpe->render_rectangle( cTabXPos,elementBox.y-cam->y,cTabX2Pos,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Program_Color,false);
                    gpe->render_rectangle( cTabXPos,elementBox.y-cam->y,cTabX2Pos,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Color,true);
                    render_new_text( cTabXPos+thisTabWidth/2,elementBox.y+elementBox.h/2-cam->y,subOptions[i],GPE_MAIN_THEME->Main_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
                }
                else
                {
                    gpe->render_rectangle( cTabXPos,elementBox.y-cam->y,cTabX2Pos,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Program_Header_Color,false);
                    gpe->render_rectangle( cTabXPos,elementBox.y-cam->y,cTabX2Pos,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Color,true);
                    render_new_text( cTabXPos+thisTabWidth/2,elementBox.y+elementBox.h/2-cam->y,subOptions[i],GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
                }
                cTabXPos = cTabX2Pos;
            }
        }
        else
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Program_Color,false);
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Color,true);
        }
        /*if( isHovered)
        {
            //Uncomment in the even a tab bar randomly behaves weirdly
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Highlighted_Color,true);
        }*/
    }
}

GPE_StatusBar::GPE_StatusBar()
{
    elementBox.h = 24;
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

void GPE_StatusBar::render_self( GPE_Rect * viewedSpace,GPE_Rect *cam, bool forceRedraw)
{
    if( isEnabled && forceRedraw && elementBox.h >0 && FONT_STATUSBAR!=NULL)
    {
        gpe->render_rect( &elementBox,GPE_MAIN_THEME->Program_Color,false);
        int statusBarStringWidth = 0, statusBarStringHeight = 0;
        int projectNameWidth = 0, projectNameHeight = 0;
        if( (int)CURRENT_PROJECT_NAME.size() > 0)
        {
            std::string  projectNameToRender = CURRENT_PROJECT_NAME;
            //Commented out for Version 1.13 temporarily
            if( MAIN_GUI_SETTINGS && MAIN_GUI_SETTINGS->useShortProjectNames )
            {
                projectNameToRender = get_file_noext( get_local_from_global_file(CURRENT_PROJECT_NAME) );

            }

            FONT_STATUSBAR->get_metrics(projectNameToRender,&projectNameWidth, &projectNameHeight);

            if( (int)codeEditorStatusBarString.size() > 0)
            {
                FONT_STATUSBAR->get_numbered_metrics(codeEditorStatusBarString,&statusBarStringWidth, &statusBarStringHeight);
                if( elementBox.w > statusBarStringWidth+projectNameWidth +GENERAL_GPE_PADDING )
                {
                    render_new_text( elementBox.x,elementBox.y+elementBox.h/2,projectNameToRender,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE);
                    //gpe->render_rectangle( elementBox.x+projectNameWidth,elementBox.y,elementBox.x+elementBox.w,elementBox.y+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,false);
                    render_bitmap_text( elementBox.x+projectNameWidth+GENERAL_GPE_PADDING,elementBox.y+elementBox.h/2,codeEditorStatusBarString,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE);
                }
                else
                {
                    render_bitmap_text( elementBox.x+GENERAL_GPE_PADDING,elementBox.y+elementBox.h/2,codeEditorStatusBarString,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE);
                }
            }
            else if( elementBox.w > projectNameWidth +GENERAL_GPE_PADDING )
            {
                render_new_text( elementBox.x+GENERAL_GPE_PADDING,elementBox.y+elementBox.h/2,projectNameToRender,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE);
            }
            else
            {
                render_new_text( elementBox.x+elementBox.w-GENERAL_GPE_PADDING/2,elementBox.y+elementBox.h/2,projectNameToRender,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_RIGHT,FA_MIDDLE);
            }
        }
        else if( (int)codeEditorStatusBarString.size() > 0)
        {
            FONT_STATUSBAR->get_numbered_metrics(codeEditorStatusBarString,&statusBarStringWidth, &statusBarStringHeight);
            render_bitmap_text( elementBox.x+GENERAL_GPE_PADDING,elementBox.y+elementBox.h/2,codeEditorStatusBarString,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE );
        }
        gpe->render_rect( &elementBox,GPE_MAIN_THEME->Main_Border_Color,true);
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
    if( kbShortCut1 >=0 && kbShortCut1 < input->key_count )
    {
        shortcutString = input->binding_name[kbShortCut1];
        if( kbShortCut2 >=0 && kbShortCut2 < input->key_count )
        {
            shortcutString += "+"+input->binding_name[kbShortCut2];
            if( kbShortCut3 >=0 && kbShortCut3 < input->key_count )
            {
                shortcutString += "+"+input->binding_name[kbShortCut3];
            }
        }
    }
    //barColor = GPE_MAIN_THEME->PopUp_Box_Color;
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
    if( (int)opName.size()>0 )
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
    optionBox.h = GPE_AVERAGE_LINE_HEIGHT;
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

GPE_PopUpMenu_Option * GPE_PopUpMenu_Option::add_menu_option( std::string name, int id, GPE_Texture * gTexture, int spriteImgNumb,GPE_Animation * gSprite, bool endsSection, bool selectable, bool isResource, int kbS1, int kbS2 , int kbS3 )
{
    maxOptionsToView = SCREEN_HEIGHT/GPE_AVERAGE_LINE_HEIGHT;
    selfDistanceToBottom = (SCREEN_HEIGHT-optionBox.y-optionBox.h)/GPE_AVERAGE_LINE_HEIGHT;

    GPE_PopUpMenu_Option * newOp = new GPE_PopUpMenu_Option(name,id,selectable,showShortCuts,false, kbS1, kbS2, kbS3);
    newOp->optionBox.h = GPE_AVERAGE_LINE_HEIGHT;
    newOp->isTopOfMenu = false;
    newOp->sectionBottom = endsSection;
    newOp->subMenuIsOpen = false;
    newOp->isResourceOption = isResource;

    GPE_PopUpMenu_Option * tempFOption = NULL;
    int i = 0;
    if( id<0 )
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
            newOp->set_position(optionBox.x ,optionBox.y+(menuCountSize-selfDistanceToBottom)*GPE_AVERAGE_LINE_HEIGHT);
        }
        else
        {
            newOp->set_position(optionBox.x ,optionBox.y+menuCountSize*GPE_AVERAGE_LINE_HEIGHT );
        }
        //newOp->barColor = GPE_MAIN_THEME->PopUp_Box_Color;
        newOp->set_width(subOptionWidthSpace);
    }
    else if( isTopOfMenu)
    {
        newOp->set_position(optionBox.x ,optionBox.y+optionBox.h+ menuCountSize*GPE_AVERAGE_LINE_HEIGHT );
        //newOp->barColor = GPE_MAIN_THEME->PopUp_Box_Color;
        newOp->set_width(subOptionWidthSpace);
    }
    else
    {
        if( menuCountSize > selfDistanceToBottom)
        {
            newOp->set_position(optionBox.x+optionBox.w ,optionBox.y+(menuCountSize-selfDistanceToBottom)*GPE_AVERAGE_LINE_HEIGHT );
        }
        else
        {
            newOp->set_position(optionBox.x+optionBox.w ,optionBox.y+menuCountSize*GPE_AVERAGE_LINE_HEIGHT );
        }
        //newOp->barColor = barColor;
        newOp->set_width(subOptionWidthSpace);
    }
    subOptions.push_back(newOp);

    //resizes submenu in the event that the new option is wider
    if( newOp->get_width()>=widestOptionSpace )
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
        for( i=0; i< (int)subOptions.size(); i++ )
        {
            tempFOption = subOptions[i];
            //if( isTopOfMenu)
            {
                tempFOption->set_width(optionWidthSpace);
            }
            //else
            {
                tempFOption->set_width(subOptionWidthSpace);
            }
        }
    }
    return newOp;
}

void GPE_PopUpMenu_Option::prerender_self(  )
{
    /*GPE_PopUpMenu_Option * tempOption = NULL;
    for( int i = 0; i < (int)subOptions.size(); i++)
    {
        tempOption = subOptions[i];
        tempOption->prerender_self( );
    }*/
}

int GPE_PopUpMenu_Option::process_menu_option()
{
    int barAction = -1;
    int returnAction = -1;
    if( input->check_mouse_released(1) || input->check_mouse_released(2) || WINDOW_WAS_JUST_RESIZED || input->released[kb_esc])
    {
        returnAction = -2;
    }
    else
    {
        if( subMenuIsOpen )
        {
            if( isContext )
            {
                if( input->down[kb_up] && input->pressed[kb_up]==false && input->released[kb_up]==false )
                {
                    upDelay+=0.5;
                }
                else
                {
                    upDelay = -1;
                }

                if( input->down[kb_down] && input->pressed[kb_down]==false && input->released[kb_down]==false )
                {
                    downDelay+=0.5;
                }
                else
                {
                    downDelay = -1;
                }

                if( input->down[kb_left] && input->pressed[kb_left]==false && input->released[kb_left]==false )
                {
                    leftDelay+=0.5;
                }
                else
                {
                    leftDelay = -1;
                }

                if( input->down[kb_right] && input->pressed[kb_right]==false && input->released[kb_right]==false )
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
            maxOptionsToView = SCREEN_HEIGHT/GPE_AVERAGE_LINE_HEIGHT;
            selfDistanceToBottom = (SCREEN_HEIGHT-optionBox.y-optionBox.h)/GPE_AVERAGE_LINE_HEIGHT;
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
                if( point_between(input->mouse_x,input->mouse_y,optionBox.x+extraRenderSpace,0,optionBox.x+extraRenderSpace+subOptionsCurrWidth,SCREEN_HEIGHT) )
                {
                    if( input->mouseScrollingUp )
                    {
                        subMenuStartPos-=maxOptionsToView/8;
                    }

                    else if( input->mouseScrollingDown)
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

                if( leftDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !input->pressed[kb_left] && input->released[kb_left] ) )
                {
                    leftDelay = -1;
                    push_left();
                    keyActivityHappend = true;
                }
                else if( rightDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !input->pressed[kb_right] && input->released[kb_right] ) )
                {
                    push_right();
                    rightDelay = -1;
                    keyActivityHappend = true;

                }
                else if( upDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !input->pressed[kb_up] && input->released[kb_up] ) )
                {
                    push_up();
                    upDelay = -1;
                    keyActivityHappend = true;
                }
                else if( downDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !input->pressed[kb_down] && input->released[kb_down] ) )
                {
                    push_down();
                    downDelay = -1;
                    keyActivityHappend = true;
                }
                else if( input->released[kb_enter] || input->released[kb_space] && subMenuIsOpen  )
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
                                    fOption->set_position(subPosX,(i-subMenuStartPos)*GPE_AVERAGE_LINE_HEIGHT );
                                }
                                else
                                {
                                    //offset box, no scroller needed
                                    fOption->set_position(subPosX ,optionBox.y+(i-(menuCountSize-selfDistanceToBottom) )*GPE_AVERAGE_LINE_HEIGHT );
                                }
                            }
                            else
                            {
                                fOption->set_position(subPosX ,optionBox.y+i*GPE_AVERAGE_LINE_HEIGHT);
                            }
                        }
                        else if( isTopOfMenu)
                        {
                            fOption->set_position(optionBox.x+extraRenderSpace ,optionBox.y+optionBox.h+i*GPE_AVERAGE_LINE_HEIGHT);
                        }

                        barAction = fOption->process_menu_option();
                        if( input->mouseMovementInputReceivedInFrame )
                        {
                            if( point_between(input->mouse_x, input->mouse_y,fOption->optionBox.x,fOption->optionBox.y,fOption->optionBox.x+fOption->optionBox.w,fOption->optionBox.y+fOption->optionBox.h) )
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

        if(  isTopOfMenu==false && point_between(input->mouse_x, input->mouse_y,optionBox.x+1,optionBox.y+1,optionBox.x+optionBox.w-1,optionBox.y+optionBox.h-1) )
        {
            if( (int)subOptions.size()>0)
            {
                if( input->mouseMovementInputReceivedInFrame )
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
            else if( input->check_mouse_released(0) || input->released[kb_enter] )
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

void GPE_PopUpMenu_Option::set_image_data(GPE_Animation * newSprite,int newId)
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
            cOption->set_position(optionBox.x+optionBox.w,optionBox.y+i*GPE_AVERAGE_LINE_HEIGHT);
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
                cOption->set_position(optionBox.x+optionBox.w,optionBox.y+i*GPE_AVERAGE_LINE_HEIGHT);
                if( isContext)
                {
                    cOption->set_position(optionBox.x+subOptionWidthSpace,-1);
                    cOption->set_width(newWidth);
                }
            }
        }
    }
}

void GPE_PopUpMenu_Option::render_self( GPE_Rect *cam, bool forceRedraw)
{
    if( forceRedraw )
    {
        int i = 0;
        if(sectionBottom )
        {
            gpe->render_line( optionBox.x,optionBox.y+optionBox.h,optionBox.x+optionBox.w,optionBox.y+optionBox.h,c_dkgray);
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
                    int subRenderHeight = (int)subOptions.size()*GPE_AVERAGE_LINE_HEIGHT;
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
                                subRenderYPos = optionBox.y- ( abs(menuCountSize-selfDistanceToBottom) *GPE_AVERAGE_LINE_HEIGHT);
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
                            gpe->render_rectangle( optionBox.x,subRenderYPos,optionBox.x+optionBox.w,subRenderYPos+subRenderHeight,GPE_MAIN_THEME->PopUp_Box_Color, false);
                        }
                        else
                        {
                            gpe->render_rectangle( optionBox.x,subRenderYPos,optionBox.x+optionWidthSpace,subRenderYPos+subRenderHeight,GPE_MAIN_THEME->Program_Color, false);
                        }
                        for( i=subMenuStartPos; i<(int)subOptions.size() && i < subMenuStartPos+maxOptionsToView; i++)
                        {
                            fOption = subOptions[i];
                            if( fOption!=NULL)
                            {
                                if( hoverOption == i && hoverOption>=0)
                                {
                                    gpe->render_rect( &fOption->optionBox,GPE_MAIN_THEME->PopUp_Box_Highlight_Color, false);
                                }
                                fOption->render_self( cam, true);
                            }
                        }
                        if( isContext)
                        {
                            gpe->render_rectangle( optionBox.x,subRenderYPos,optionBox.x+optionBox.w,subRenderYPos+subRenderHeight,GPE_MAIN_THEME->PopUp_Box_Border_Color, true);
                        }
                        else
                        {
                            gpe->render_rectangle( optionBox.x,subRenderYPos,optionBox.x+subOptionWidthSpace,subRenderYPos+subRenderHeight,GPE_MAIN_THEME->Main_Border_Color, true);
                        }
                    }
                    else
                    {
                        gpe->render_rectangle( optionBox.x+optionBox.w,subRenderYPos,optionBox.x+optionBox.w+subOptionWidthSpace,subRenderYPos+subRenderHeight,GPE_MAIN_THEME->PopUp_Box_Color, false);
                        for( i=subMenuStartPos; i<(int)subOptions.size() && i < subMenuStartPos+maxOptionsToView; i++)
                        {
                            fOption = subOptions[i];
                            if( hoverOption == i  && hoverOption>=0)
                            {
                                gpe->render_rect( &fOption->optionBox,GPE_MAIN_THEME->PopUp_Box_Highlight_Color, false);
                            }
                            fOption->render_self(  cam,true);
                        }
                        gpe->render_rectangle( optionBox.x+optionBox.w,subRenderYPos,optionBox.x+optionBox.w+subOptionWidthSpace,subRenderYPos+subRenderHeight,GPE_MAIN_THEME->PopUp_Box_Border_Color, true);
                        gpe->render_rectangle( optionBox.x,optionBox.y,optionBox.x+optionBox.w,optionBox.y+optionBox.h,GPE_MAIN_THEME->PopUp_Box_Color, false);
                     }
                }
                if( !isTopOfMenu)
                {
                    render_new_text( optionBox.x+optionBox.w-GENERAL_GPE_PADDING,optionBox.y+optionBox.h/2,">",GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_TOOLBAR,FA_RIGHT,FA_MIDDLE);
                }
            }

            if( isTopOfMenu )
            {
                render_new_text( optionBox.x+4,optionBox.y,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_TOP);
            }
            else
            {
                if(subMenuIsOpen  )
                {
                    gpe->render_rectangle( optionBox.x,optionBox.y,optionBox.x+optionBox.w,optionBox.y+optionBox.h,GPE_MAIN_THEME->PopUp_Box_Highlight_Color, false);
                    render_new_text( optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING,optionBox.y+GPE_AVERAGE_LINE_HEIGHT/2,opName,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_MIDDLE);
                }
                /*else if( )
                {
                    render_new_text( optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING,optionBox.y+GPE_AVERAGE_LINE_HEIGHT/2,opName,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_MIDDLE);

                }*/
                else
                {
                    render_new_text( optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING,optionBox.y+GPE_AVERAGE_LINE_HEIGHT/2,opName,GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_MIDDLE);
                    //opTexture->render_tex( optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING, optionBox.y+16-(opTexture->get_height()/2),NULL);
                }
                if ( opSprite!=NULL && opSprite->animationTexture!=NULL )
                {
                    render_animation_resized( opSprite,spriteFrameNumber,optionBox.x+GENERAL_GPE_PADDING,optionBox.y+optionBox.h/4,optionBox.h/2,optionBox.h/2);
                }
                else if( opTexture!=NULL)
                {
                    if( isFolderOption)
                    {
                        render_texture_resized( opTexture,optionBox.x+GENERAL_GPE_PADDING,optionBox.y+optionBox.h/4,optionBox.h/2,optionBox.h/2,NULL,FA_LEFT,FA_TOP,GPE_MAIN_THEME->Main_Folder_Color );
                    }
                    else if( isResourceOption || renderWhite )
                    {
                        render_texture_resized( opTexture,optionBox.x+GENERAL_GPE_PADDING,optionBox.y+optionBox.h/4,optionBox.h/2,optionBox.h/2,NULL,FA_LEFT,FA_TOP, c_white );
                    }
                    else
                    {
                        render_texture_resized( opTexture,optionBox.x+GENERAL_GPE_PADDING,optionBox.y+optionBox.h/4,optionBox.h/2,optionBox.h/2,NULL,FA_LEFT,FA_TOP, GPE_MAIN_THEME->PopUp_Box_Font_Color  );
                    }
                }
                if( (int)shortcutString.size()>0 )
                {
                    render_new_text( optionBox.x+optionBox.w-GENERAL_GPE_PADDING,optionBox.y+optionBox.h/2,shortcutString,GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_TOOLBAR,FA_RIGHT,FA_MIDDLE);
                }
            }
        }
        else
        {
            render_new_text( optionBox.x+GENERAL_ICON_WIDTH_AND_PADDING,optionBox.y+optionBox.h/2,"null",GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_TOOLBAR,FA_LEFT,FA_MIDDLE);
        }
    }
}

void GPE_PopUpMenu_Option::resize_self()
{
    set_width( subOptionWidthSpace );
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
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = SCREEN_WIDTH;
    elementBox.h = 16;
    guiListTypeName = "toolbar";
    opName = bName;
    bRect = elementBox;
    barWidthTotal = 0;
    hoverOption = -1;
    selectedOption = -1;
    toolBarIsOpen = false;
}

GPE_Toolbar::GPE_Toolbar()
{
    guiListTypeName = "toolbar";
    opName = "untitled";
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = SCREEN_WIDTH;
    elementBox.h = 16;

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
    newOp->set_position(barWidthTotal,elementBox.y);
    newOp->optionBox.h = elementBox.h;
    barOptions.push_back(newOp);
    barWidthTotal+=newOp->get_width()+TOOLKEY_OPTION_PADDING;
    return newOp;
}

void GPE_Toolbar::set_width(int newWid)
{
    elementBox.w = newWid;
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

void GPE_Toolbar::prerender_self(  )
{
    GPE_PopUpMenu_Option * tempOption = NULL;
    for( int i = 0; i < (int)barOptions.size(); i++)
    {
        tempOption = barOptions[i];
        tempOption->prerender_self();
    }
}

void GPE_Toolbar::process_toolbar()
{
    int i;
    int drawXPos;
    int actionHappened = false;
    int barAction = -1;

    update_popup_info(); //resets the popup option to process
    if( input->windowEventHappendInFrame == true)
    {
        hoverOption = -1;
        selectedOption = -1;
        reset_options();
        toolBarIsOpen = false;
    }

    GPE_PopUpMenu_Option * fOption = NULL;
    if( input->check_keyboard_only_released(kb_alt) )
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
        if( point_between_rect(input->mouse_x, input->mouse_y,&elementBox) )
        {
            if( input->check_mouse_released(0) )
            {
                close_toolbar();
            }
        }*/

        drawXPos = elementBox.x;
        if( toolBarIsOpen )
        {
            if( input->down[kb_up] && input->pressed[kb_up]==false && input->released[kb_up]==false )
            {
                upDelay+=0.5;
            }
            else
            {
                upDelay = -1;
            }

            if( input->down[kb_down] && input->pressed[kb_down]==false && input->released[kb_down]==false )
            {
                downDelay+=0.5;
            }
            else
            {
                downDelay = -1;
            }

            if( input->down[kb_left] && input->pressed[kb_left]==false && input->released[kb_left]==false )
            {
                leftDelay+=0.5;
            }
            else
            {
                leftDelay = -1;
            }

            if( input->down[kb_right] && input->pressed[kb_right]==false && input->released[kb_right]==false )
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
            if( leftDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !input->pressed[kb_left] && input->released[kb_left] ) )
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
            else if( rightDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !input->pressed[kb_right] && input->released[kb_right] ) )
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
            else if( upDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !input->pressed[kb_up] && input->released[kb_up] ) )
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
            else if( downDelay >= (MAIN_GUI_SETTINGS->normalInputDelayTime+3)*FPS_RATIO  || ( !input->pressed[kb_down] && input->released[kb_down] ) )
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
            else if( input->released[kb_enter] || input->released[kb_space] && toolBarIsOpen  )
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
                if( point_between(input->mouse_x, input->mouse_y,drawXPos,elementBox.y,drawXPos+fOption->get_width()+TOOLKEY_OPTION_PADDING,elementBox.y+elementBox.h) )
                {
                    if( input->mouseMovementInputReceivedInFrame || hoverOption < 0 )
                    {
                        hoverOption = i;
                    }
                    if( selectedOption>=0 && toolBarIsOpen && input->mouseMovementInputReceivedInFrame )
                    {
                        reset_options();
                        hoverOption = i;

                        selectedOption = i;
                        //resets the bar options
                        fOption->open_menu();
                        actionHappened = 2;
                        open_toolbar();
                    }
                    else if( input->check_mouse_released(0) )
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

            if( input->check_mouse_released(0) )
            {
                //resets the toolbar if the user clicks on the far right side of it.
                if(actionHappened==false || actionHappened==1)
                {
                    close_toolbar();
                }
            }
        }
    }

    if( input->check_mouse_released(1) || input->check_keyboard_released(kb_esc) )
    {
        close_toolbar();
    }

    if((int)GPE_Action_Message.size() > 0 )
    {
        close_toolbar();
        input->reset_all_input();
    }
}

void GPE_Toolbar::render_toolbar(GPE_Rect *renderCam, bool forceRedraw)
{
    if( forceRedraw)
    {
        if( toolBarIsOpen)
        {
            MAIN_OVERLAY->render_frozen_screenshot();
        }
        gpe->render_rect(&elementBox,GPE_MAIN_THEME->Program_Header_Color,false);
        if( barOptions.size()==0)
        {
            render_new_text( elementBox.x+4,elementBox.y+8,"Empty Toolbar();",GPE_MAIN_THEME->Main_Box_Font_Color,NULL,FA_LEFT,FA_TOP);
        }
        else
        {
            GPE_PopUpMenu_Option * fOption = NULL;
            int drawXPos = 0;
            int y2 = elementBox.y+elementBox.h;
            for(int i=0; i<(int)barOptions.size(); i++)
            {
                fOption = barOptions[i];
                if(fOption->subMenuIsOpen || hoverOption==i)
                {
                    gpe->render_rectangle( drawXPos,elementBox.y,drawXPos+fOption->get_width()+TOOLKEY_OPTION_PADDING,y2,GPE_MAIN_THEME->PopUp_Box_Highlight_Color, false);
                }
                fOption->render_self(NULL,true);
                drawXPos+=fOption->get_width()+TOOLKEY_OPTION_PADDING;
            }
        }
    }
}

GPE_DropDown_Menu::GPE_DropDown_Menu( std::string name, bool justOptions)
{
    guiListTypeName = "dropdown";
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 228;
    //int nameMinSize = name.size()*
    elementBox.h = 32;
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
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
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
                GPE_open_context_menu(viewedSpace->x+elementBox.x-cam->x, viewedSpace->y+elementBox.y+elementBox.h-cam->y);
                MAIN_CONTEXT_MENU->set_width(elementBox.w);
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
        if( input->check_keyboard_down(kb_up) && selectedId > 0)
        {
            selectedId--;
        }
        else if( input->check_keyboard_down(kb_down) && selectedId < (int)subOptions.size()-1)
        {
            selectedId++;
        }
    }
}

void GPE_DropDown_Menu::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && cam!=NULL && viewedSpace!=NULL)
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Input_Color,false);

        if( selectedId >= 0)
        {
            if( selectedId < (int)subOptions.size() )
            {
                GPE_KeyPair * kp = subOptions[selectedId];
                render_new_text_ext( elementBox.x+GENERAL_GPE_PADDING-cam->x,elementBox.y+GENERAL_GPE_PADDING-cam->y,kp->keyString,GPE_MAIN_THEME->Input_Font_Color,FONT_POPUP,FA_LEFT,FA_TOP,elementBox.w-elementBox.h-12,-1);
            }
            else
            {
                render_new_text_ext( elementBox.x+GENERAL_GPE_PADDING-cam->x,elementBox.y+GENERAL_GPE_PADDING-cam->y,opName,GPE_MAIN_THEME->Input_Font_Color,FONT_POPUP,FA_LEFT,FA_TOP,elementBox.w-elementBox.h-12,-1);
                selectedId = 0;
            }
        }
        else
        {
            render_new_text_ext( elementBox.x+GENERAL_GPE_PADDING-cam->x,elementBox.y+GENERAL_GPE_PADDING-cam->y,opName,GPE_MAIN_THEME->Input_Font_Color,FONT_POPUP,FA_LEFT,FA_TOP,elementBox.w-elementBox.h-12,-1);
        }
        if( isInUse)
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Input_Highlight_Outline_Color,true);
        }
        else if( isHovered)
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Input_Highlight_Alt_Color,true);
        }
        else
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Input_Outline_Color,true);
        }

        gpe->render_rectangle( elementBox.x+elementBox.w-elementBox.h*3/4-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Input_Outline_Color ,false);
        render_animation_ext( GPE_TRIANGLE,3,elementBox.x+elementBox.w-elementBox.h/2-cam->x,elementBox.y+elementBox.h/3 - cam->y,elementBox.h/3,elementBox.h/3,GPE_MAIN_THEME->Input_Color );
    }
}

void GPE_DropDown_Menu::set_id(int newId)
{
    set_selection(newId);
}

void GPE_DropDown_Menu::set_option(std::string newSelectedOptionName )
{
    GPE_KeyPair * tOption = NULL;
    for( int i = (int)subOptions.size()-1; i>=0; i--)
    {
        tOption = subOptions[i];
        if( tOption!=NULL )
        {
            if( tOption->keyString==newSelectedOptionName)
            {
                set_selection(  i );
            }
        }
    }
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

GPE_ToolIconButton::GPE_ToolIconButton( std::string buttonImgFile,std::string name, int id, int buttonSize, bool lastInCol)
{
    webUrl = "";
    wasClicked = false;
    guiListTypeName = "iconbutton";
    endsSection = lastInCol;
    opName = name;
    descriptionText = name;
    opId = id;
    buttonTexture = rsm->texture_add( buttonImgFile );
    if( buttonTexture == NULL)
    {
        record_error("Unable to add texture with img ["+buttonImgFile+"]..." );
    }
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = buttonSize;
    elementBox.h = buttonSize;
    isTabbed = false;
    usesTabs = false;
}

GPE_ToolIconButton::~GPE_ToolIconButton()
{

}

void GPE_ToolIconButton::change_texture( GPE_Texture * newTexture)
{
    buttonTexture = newTexture;
}

std::string GPE_ToolIconButton::get_data()
{
    if( buttonTexture!=NULL)
    {
        return guiListTypeName+":"+opName+"==|||=="+int_to_string(elementBox.w)+","+buttonTexture->get_filename()+",,,"+int_to_string(elementBox.w)+","+int_to_string(elementBox.h)+","+int_to_string(opId);
    }
    else
    {
        return guiListTypeName+":"+opName+"==|||=="+int_to_string(elementBox.w)+",0,,,0,0,"+int_to_string(opId);
    }
}

void GPE_ToolIconButton::load_data(std::string dataString)
{
    int buttonSize = split_first_int(dataString,',');
    if( buttonSize <=0)
    {
        elementBox.w = buttonSize;
        elementBox.h = buttonSize;
    }
    std::string textureFileName = split_first_string(dataString,",,,");
    int spriteWidth = split_first_int(dataString,',');
    int spriteHeight = split_first_int(dataString,',');
    elementBox.w = spriteWidth;
    elementBox.h = spriteHeight;
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
    set_clicked( false );
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
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

    if( isClicked && (int)webUrl.size() > 3)
    {
         GPE_OpenURL(webUrl);
    }
}

void GPE_ToolIconButton::set_id(int newId)
{
    opId = newId;
}

void GPE_ToolIconButton::set_image( std::string buttonImgFile)
{
    /*if( buttonTexture!=NULL )
    {
        buttonTexture->load_new_texture(buttonImgFile,-1,true);
    }
    else
    {
        */buttonTexture = rsm->texture_add(buttonImgFile);
    //}
}

void GPE_ToolIconButton::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);

    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( forceRedraw && cam!=NULL && viewedSpace!=NULL)
    {
        if( buttonTexture!=NULL)
        {
            GPE_Color * renderColor = GPE_MAIN_THEME->Icon_Font_Color;
            if( isInUse)
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Icon_Box_Highlighted_Color,false);
                renderColor = GPE_MAIN_THEME->Icon_Font_Highlighted_Color;
            }
            else if(isHovered)
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Icon_Box_Highlighted_Color,false);
                renderColor = GPE_MAIN_THEME->Icon_Font_Highlighted_Color;
            }
            else if(isTabbed)
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Icon_Box_Selected_Color,false);
                renderColor = GPE_MAIN_THEME->Icon_Font_Selected_Color;
            }
            render_texture_resized( buttonTexture,elementBox.x-cam->x,elementBox.y-cam->y,elementBox.w,elementBox.h,NULL,FA_LEFT,FA_TOP,renderColor);
        }
        if( endsSection)
        {
            //gpe->render_rectangle( elementBox.x+elementBox.w-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w+1-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Icon_Font_Color);
        }
    }
}

void GPE_ToolIconButton::set_website(std::string urlIn)
{
    webUrl = urlIn;
    wasClicked = false;
    prerender_self();
}

GPE_ToolIconButtonBar::GPE_ToolIconButtonBar( int buttonSize,bool useTabs)
{
    guiListTypeName = "iconbuttonbar";
    if( buttonSize<=0)
    {
        buttonSize = 24;
    }

    hoverOption = 0;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 32;
    elementBox.h = buttonSize;
    barPadding = 8;
    xPadding = GENERAL_GPE_PADDING;
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
        elementBox.w = xPadding*3+( (int)barOptions.size() )*(xPadding+elementBox.h);
    }
}

void GPE_ToolIconButtonBar::set_width(int newWid)
{
    elementBox.w = newWid;
}

GPE_ToolIconButton * GPE_ToolIconButtonBar::add_option( std::string buttonImgFile,std::string name, int id, bool endsSection)
{
    int barSize = (int)barOptions.size();
    GPE_ToolIconButton * newOp = new GPE_ToolIconButton(  buttonImgFile,name,id,elementBox.h, endsSection);
    newOp->descriptionText =  name;
    newOp->usesTabs = isTabBar;
    if( isTabBar && barSize==0)
    {
        newOp->isTabbed = true;
        tabPosition = 0;
    }

    barOptions.push_back(newOp);

    newButtonXPos+= xPadding+elementBox.h;
    if( endsSection)
    {
        newButtonXPos+=xPadding;
    }
    if( widthAutoResizes)
    {
        elementBox.w = barPadding*2+(xPadding+elementBox.h)*( (int)barOptions.size() );
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
    int newButtonXPos = elementBox.x;
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
            cButton->set_coords( newButtonXPos, elementBox.y );
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
            newButtonXPos+= xPadding+elementBox.h;
        }
    }
    if( isTabBar && hasArrowkeyControl)
    {
        bool tabMoved = false;
        if( tabPosition > 0 && (input->check_keyboard_pressed(kb_left) || input->check_keyboard_pressed(kb_up) ) )
        {
            tabPosition--;
            tabMoved = true;
        }
        else if(input->check_keyboard_pressed(kb_right) || input->check_keyboard_pressed(kb_down) )
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

void GPE_ToolIconButtonBar::render_self( GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);

    if( forceRedraw && elementBox.h > 0)
    {
        //gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->PopUp_Box_Color,false);
        GPE_ToolIconButton * cButton = NULL;
        for(int i=0; i< (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if(cButton!=NULL)
            {
                cButton->render_self( viewedSpace, cam,forceRedraw);
            }
        }
        if( isInUse)
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
        }
        //gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
    }
}

void GPE_ToolIconButtonBar::set_coords(int newX, int newY)
{
    int pastX = elementBox.x;
    int pastY = elementBox.y;
    GPE_GeneralGuiElement::set_coords(newX,newY);
    if( elementBox.x!=pastX || elementBox.y!=pastY)
    {
        GPE_ToolIconButton * cButton = NULL;
        int xNewPos = elementBox.x+barPadding;
        for( int i = 0; i < (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if( cButton!=NULL)
            {
                cButton->set_coords(xNewPos, elementBox.y);
                cButton->set_height( elementBox.h);
                xNewPos+=xPadding+elementBox.h;
                if( cButton->ends_section() )
                {
                    xNewPos+=xPadding;
                }
            }
        }
    }
    if( widthAutoResizes)
    {
        elementBox.w = barPadding*2+(xPadding+elementBox.h)*( (int)barOptions.size() );
    }
}


void GPE_ToolIconButtonBar::set_height(int newHeight)
{
    int pastH = elementBox.h;
    if( elementBox.h!=newHeight)
    {
        elementBox.h = newHeight;
        newButtonXPos = xPadding;
        GPE_ToolIconButton * cButton = NULL;
        for( int i = 0; i < (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if( cButton!=NULL)
            {
                cButton->set_coords(elementBox.x+newButtonXPos,elementBox.y);
                cButton->set_width(newHeight);
                cButton->set_height(newHeight);
                newButtonXPos+=elementBox.h;
                if( cButton->ends_section() )
                {
                    newButtonXPos+=xPadding;
                }
            }
        }
        if( widthAutoResizes)
        {
            elementBox.w = newButtonXPos;
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

    elementBox.x = buttonX;
    elementBox.y = buttonY;
    elementBox.w = 228;
    elementBox.h = buttonSize;
    if( DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(name,&textW, &textH);
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_POPUP);
        if( textH > elementBox.h)
        {
            elementBox.h=textH+GENERAL_GPE_PADDING*2;
        }
    }
    isEnabled = true;
}

GPE_ToolLabelButton::~GPE_ToolLabelButton()
{

}



void GPE_ToolLabelButton::prerender_self( )
{

    if( DEFAULT_FONT!=NULL)
    {
        /*
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        //elementBox.w=textW+GENERAL_GPE_PADDING*2;
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_POPUP);
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
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }
}

void GPE_ToolLabelButton::render_self( GPE_Rect * viewedSpace, GPE_Rect  * cam,bool forceRedraw )
{
    if( forceRedraw && isEnabled)
    {
        cam = GPE_find_camera(cam);
        viewedSpace = GPE_find_camera(viewedSpace);
        if(cam!=NULL && viewedSpace!=NULL )
        {
            if( isClicked)
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Box_Selected_Color,false);
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Border_Selected_Color,true);
            }
            else if( isHovered)
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Box_Highlighted_Color,false);
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
            }
            else
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Box_Color,false);
            }
            if(DEFAULT_FONT!=NULL)
            {
                if( isHovered)
                {
                    render_new_text( elementBox.x-cam->x+elementBox.w/2, elementBox.y-cam->y+elementBox.h/2,opName,GPE_MAIN_THEME->Button_Font_Highlighted_Color,DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
                }
                else
                {
                    render_new_text( elementBox.x-cam->x+elementBox.w/2, elementBox.y-cam->y+elementBox.h/2,opName,GPE_MAIN_THEME->Button_Font_Color,DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
                }
                /*if( elementBox.w > opStringTexture->get_width()+GENERAL_GPE_PADDING*2 )
                {
                    opStringTexture->render_tex( elementBox.x-cam->x+(elementBox.w-opStringTexture->get_width() )/2,elementBox.y-cam->y+GENERAL_GPE_PADDING,NULL);
                }
                else
                {
                    opStringTexture->render_tex( elementBox.x-cam->x+GENERAL_GPE_PADDING,elementBox.y-cam->y+GENERAL_GPE_PADDING,NULL);
                }*/
            }
            /*
            if( isInUse)
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
            }
            else if( isHovered)
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
            }
            else
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Color,true);
            }
            */

            if( isInUse )
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Main_Border_Highlighted_Color,true);
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
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_THEME->Main_Box_Font_Color,DEFAULT_FONT);
        elementBox.w=textW+GENERAL_GPE_PADDING*2;
        elementBox.h=textH+GENERAL_GPE_PADDING*2;
    }
    opName = newName;
}

///
GPE_ToolPushButton::GPE_ToolPushButton( std::string imgLocation,std::string name, std::string description,int id, int buttonSize)
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
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 228;
    if(buttonSize <=0)
    {
        elementBox.h = 32;
    }
    else
    {
        elementBox.h = buttonSize;
    }
    if( DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(name,&textW, &textH);
        if( textW + iconPadding*2 +elementBox.h > elementBox.w)
        {
            elementBox.w = textW+iconPadding*2 + elementBox.h;
        }
        //opStringTexture->loadFromRenderedText(newName,GPE_MAIN_THEME->Main_Box_Font_Color,DEFAULT_FONT);
    }
    isEnabled = true;
    wasClicked = false;
    webUrl = "";
    showBackground = true;
    showBorder = true;
    prerender_self();
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
        prerender_self();
    }
}

void GPE_ToolPushButton::enable_border( bool enableValue)
{
    if( showBorder!=enableValue)
    {
        showBorder=enableValue;
        prerender_self();
    }
}

void GPE_ToolPushButton::prerender_self( )
{
    /*
    int prevWidth = 0;

    if( DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        elementBox.w = iconPadding*3+elementBox.h+textW;
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
        if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked && (int)webUrl.size() > 3)
        {
             GPE_OpenURL(webUrl);
        }
    }
}

void GPE_ToolPushButton::render_self( GPE_Rect * viewedSpace, GPE_Rect  * cam,bool forceRedraw )
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
                    gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Box_Selected_Color,false);
                }
                //if( showBorder)
                {
                    gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Box_Highlighted_Color,true);
                }
            }
            else
            {
                if( showBackground)
                {
                    if( isHovered)
                    {
                        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Box_Highlighted_Color,false);
                        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
                    }
                    else
                    {
                        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,false);
                    }
                }
                else if( isHovered)
                {
                    gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Main_Border_Color,true);
                }
            }
            int iconSize = 0;
            if( showBackground)
            {
                iconSize = std::min(elementBox.w,elementBox.h);
                iconPadding = 0;
            }
            else
            {
                iconSize = std::min(elementBox.w,elementBox.h);
                iconPadding = 0;
            }
            GPE_Color * renderColor = GPE_MAIN_THEME->Button_Font_Color;
            if( isHovered)
            {
                renderColor = GPE_MAIN_THEME->Button_Font_Highlighted_Color;
            }
            if( showBackground==false)
            {
                if( wasClicked)
                {
                    renderColor = GPE_MAIN_THEME->Main_Box_Faded_Font_Color;
                }
                else
                {
                    renderColor = GPE_MAIN_THEME->Main_Box_Faded_Font_Color;
                }
            }
            if( opTexture!=NULL)
            {
                render_texture_resized( opTexture,elementBox.x-cam->x+iconPadding*2, elementBox.y-cam->y,iconSize,iconSize,NULL,FA_LEFT,FA_TOP,renderColor);
                if( DEFAULT_FONT!=NULL)
                {
                    render_new_text( elementBox.x-cam->x+iconSize+iconPadding*2, elementBox.y-cam->y+elementBox.h/2,opName,renderColor,DEFAULT_FONT,FA_LEFT,FA_MIDDLE);
                }
            }
            else if(DEFAULT_FONT!=NULL)
            {
                render_new_text( elementBox.x-cam->x+iconSize+iconPadding*2, elementBox.y-cam->y+elementBox.h/2,opName,renderColor,DEFAULT_FONT,FA_LEFT,FA_MIDDLE);
            }

            if( isInUse )
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Main_Border_Highlighted_Color,true);
            }
        }
    }
}

void GPE_ToolPushButton::set_name(std::string newName)
{
    opName = newName;
    prerender_self();
}

void GPE_ToolPushButton::set_website(std::string urlIn)
{
    webUrl = urlIn;
    wasClicked = false;
    prerender_self();
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
    elementBox.x = buttonX;
    elementBox.y = buttonY;
    elementBox.w = 228;

    elementBox.h = GENERAL_GPE_PADDING+( (int)buttonLines.size() *(lineHeight+GENERAL_GPE_PADDING) );

    isEnabled = true;
    wasClicked = false;
    webUrl = "";
    showBackground = true;
    showBorder = true;
    prerender_self();
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
        prerender_self();
    }
}

void GPE_ToolPushButtonMultiLine::enable_border( bool enableValue)
{
    if( showBorder!=enableValue)
    {
        showBorder=enableValue;
        prerender_self();
    }
}

void GPE_ToolPushButtonMultiLine::prerender_self( )
{
    /*
    int prevWidth = 0;

    if( DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        elementBox.w = iconPadding*3+elementBox.h+textW;
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
        if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked && (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
}

void GPE_ToolPushButtonMultiLine::render_self( GPE_Rect * viewedSpace, GPE_Rect  * cam,bool forceRedraw )
{
    if( forceRedraw && isEnabled)
    {
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);

        if(viewedSpace!=NULL && cam!=NULL)
        {
            GPE_Color * textRenderColor = GPE_MAIN_THEME->Button_Font_Color;
            GPE_Color * bgRenderColor = GPE_MAIN_THEME->Button_Box_Color;
            GPE_Color * textureRenderColor = c_white;
            if( isHovered || isInUse)
            {
                textRenderColor = GPE_MAIN_THEME->Button_Font_Highlighted_Color;
            }
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w+GENERAL_GPE_PADDING/2,elementBox.y-cam->y+elementBox.h+GENERAL_GPE_PADDING/2,c_black,false,64);

            if( showBackground)
            {
                if( isClicked )
                {
                   bgRenderColor = GPE_MAIN_THEME->Button_Box_Selected_Color;
                   textRenderColor= GPE_MAIN_THEME->Button_Font_Highlighted_Color;
                }
                else if( isHovered)
                {
                    bgRenderColor = GPE_MAIN_THEME->Button_Box_Highlighted_Color;
                    textRenderColor= GPE_MAIN_THEME->Button_Font_Highlighted_Color;
                }
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,bgRenderColor,false);
            }
            else
            {
                textRenderColor= GPE_MAIN_THEME->Button_Box_Color;
                if( isClicked )
                {
                   textRenderColor= GPE_MAIN_THEME->Button_Box_Selected_Color;
                }
                else if( isHovered)
                {
                    textRenderColor= GPE_MAIN_THEME->Button_Box_Highlighted_Color;
                }
            }
            if(  isInUse )
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Border_Selected_Color,true);
            }
            else if(isHovered)
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
            }
            int iconSize = 0;
            if( showBackground)
            {
                iconSize = std::min(elementBox.w,elementBox.h);
                iconPadding = 0;
            }
            else
            {
                iconSize = std::min(elementBox.w,elementBox.h);
                iconPadding = 0;
            }
            if( opTexture!=NULL)
            {

                /*if( showBackground==false)
                {
                    if( wasClicked)
                    {
                        renderColor = GPE_MAIN_THEME->Main_Box_Faded_Font_Color;
                    }
                    else
                    {
                        renderColor = GPE_MAIN_THEME->Main_Box_Faded_Font_Color;
                    }
                }*/
                render_texture_resized( opTexture,elementBox.x-cam->x+iconPadding*2, elementBox.y-cam->y,iconSize,iconSize,NULL,FA_LEFT,FA_TOP,textureRenderColor );
            }


            if(DEFAULT_FONT!=NULL)
            {
                for( int ipLine = 0; ipLine < (int)buttonLines.size(); ipLine++)
                {
                    render_new_text( elementBox.x-cam->x+iconSize+iconPadding*2+GENERAL_GPE_PADDING, elementBox.y-cam->y+GENERAL_GPE_PADDING*2+(ipLine * (GENERAL_GPE_PADDING+lineHeight) ),buttonLines[ipLine] ,textRenderColor,DEFAULT_FONT,FA_LEFT,FA_MIDDLE);
                }
            }

            if( isInUse )
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Main_Border_Highlighted_Color,true);
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
    //elementBox.h = GENERAL_GPE_PADDING*4+( (int)buttonLines.size() ) * (GENERAL_GPE_PADDING+lineHeight);
    elementBox.h = GENERAL_GPE_PADDING+( (int)buttonLines.size() *(lineHeight+GENERAL_GPE_PADDING) );
    prerender_self();
}

void GPE_ToolPushButtonMultiLine::set_website(std::string urlIn)
{
    webUrl = urlIn;
    wasClicked = false;
    prerender_self();
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


void GPE_SelectBoxBasic_Option::prerender_self(  )
{

}

GPE_SelectBoxBasic::GPE_SelectBoxBasic( std::string name)
{
    upDelay = 0;
    downDelay = 0;
    guiListTypeName = "selectbox";
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 228;
    elementBox.h = GENERAL_GPE_PADDING;
    maxHeight = -1;
    optionHeight = -1;
    opName = name;
    intedPos = 0;
    pos = 0;
    startPos = 0;
    maxOptionsInView = 10;
    optionHeight = GPE_TITLE_BPADDING+GENERAL_GPE_PADDING*2;
    optionIconWidth = GPE_AVERAGE_LINE_HEIGHT;
    showCheckboxes = false;
    showHideOthersCheckboxToggle = true;
    optionsScroller = new GPE_ScrollBar_YAxis();
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

    if( optionsScroller!=NULL )
    {
        delete optionsScroller;
        optionsScroller = NULL;
    }
}

void GPE_SelectBoxBasic::correct_camera()
{
    maxOptionsInView = floor( (double)elementBox.h / (double)optionHeight );
    //checks if pos is no longer in view
    if( pos < 0 )
    {
        pos = 0;
    }
    if( pos < startPos )
    {
        //startPos = pos-maxOptionsInView;
    }
    else if( pos >= startPos+maxOptionsInView )
    {
        //startPos = pos - maxOptionsInView-1;
    }

    if( startPos < 0)
    {
        startPos = 0;
    }

    //correctos the start pos if its out of view
    if( startPos+maxOptionsInView > (double)subOptions.size() )
    {
        startPos = (double)subOptions.size() - maxOptionsInView;
    }
    if( startPos < 0)
    {
        startPos = 0;
    }

    update_rectangle(&optionsScroller->elementBox, elementBox.x+elementBox.w-16, elementBox.y, 16,elementBox.h);
    update_rectangle(&optionsScroller->fullRect, 0, 0, 0,(double)subOptions.size() );
    update_rectangle(&optionsScroller->contextRect, 0, (double)startPos,0, (double)maxOptionsInView );
}

std::string GPE_SelectBoxBasic::get_data()
{
    return "";
}

void GPE_SelectBoxBasic::load_data(std::string dataString)
{

}


void GPE_SelectBoxBasic::add_option(std::string newOptionName, double newOpValue,GPE_Texture * evRepIcon, GPE_Animation * evRepSprite, int subimageInIndex, bool selectNew, bool useGuiColor )
{
    if( (int)newOptionName.size() > 0 || evRepIcon!=NULL || evRepSprite!=NULL )
    {
        GPE_SelectBoxBasic_Option * newOption = new GPE_SelectBoxBasic_Option();
        newOption->optionName = newOptionName;
        newOption->optionValue = newOpValue;
        newOption->optionSprite = evRepSprite;
        newOption->optionTexture = evRepIcon;
        newOption->useGuiColor = useGuiColor;
        //newOption->optionNameTexture->loadFromRenderedText(MAIN_RENDERER,newOptionName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_CATEGORY_BAR);

        newOption->subimageIndex = subimageInIndex;

        subOptions.push_back(newOption);
        if( maxHeight < 0)
        {
            elementBox.h+=optionHeight;
        }
        if( selectNew)
        {
            set_selection( (int)subOptions.size() - 1 );
        }
        startPos++;
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

void GPE_SelectBoxBasic::insert_option(int optionId, std::string newOptionName, GPE_Texture * evRepIcon, GPE_Animation * evRepSprite, int subimageInIndex, bool selectNew )
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
            startPos++;
        }
        else
        {
            subOptions.push_back( newOption );
        }
        if( maxHeight < 0)
        {
            elementBox.h+=optionHeight;
        }
        if( selectNew)
        {
            set_selection( optionId );
        }
        else if( pos <= optionId )
        {
            set_selection( pos -1 );
        }
    }
}

void GPE_SelectBoxBasic::limit_height(int newH)
{
    if( newH >=0)
    {
        maxHeight = newH;
        elementBox.h = newH;
    }
    else
    {
        maxHeight = -1;
    }
    maxOptionsInView = floor( (float)elementBox.h / (float)optionHeight );
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
        if( subOptions[i]!=NULL && subOptions[i]->optionValue==valueId )
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

void GPE_SelectBoxBasic::prerender_self(  )
{
    GPE_SelectBoxBasic_Option * tOption = NULL;
    for( int i = 0; i < (int)subOptions.size() && i < (int)subOptions.size(); i++)
    {
        tOption = subOptions[i];
        if( tOption!=NULL)
        {
            tOption->prerender_self( );
        }
    }

}

void GPE_SelectBoxBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);

    correct_camera();

    //only works if scrollbar is actually needed
    if( maxOptionsInView <= (int)subOptions.size() )
    {
        optionsScroller->process_self( viewedSpace, cam );
        startPos = (double)(optionsScroller->contextRect.y);
    }

    bool scrollHappened = false;
    if( isInUse)
    {
        hasArrowkeyControl = true;
        hasScrollControl = true;
        if( input->released[kb_up] )
        {
            pos-=1;
            if( pos < 0 )
            {
                pos= 0;
            }
            if( pos == startPos )
            {
                startPos--;
            }
            scrollHappened = true;
        }
        else if( input->released[kb_down] )
        {
            pos+=1;
            if( pos >= (int)subOptions.size() )
            {
                pos=(int)subOptions.size()-1;
            }
            if( pos>=startPos+maxOptionsInView )
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
            scrollHappened = true;
        }
        else if( input->check_keyboard_released(kb_space ) && showCheckboxes)
        {
            if( pos >=0 && pos < (int)subOptions.size() )
            {
                GPE_SelectBoxBasic_Option *  optionToChange = subOptions[pos];
                optionToChange->isChecked = !optionToChange->isChecked;
            }
        }
    }
    else
    {
        hasArrowkeyControl = true;
        hasScrollControl = true;
    }
    if( isInUse && hasScrollControl && optionsScroller->hasScrollControl==false )
    {
        if( input->mouseScrollingUp > 0)
        {
            startPos--;
            scrollHappened = true;
        }
        if( input->mouseScrollingDown > 0)
        {
            startPos++;
            scrollHappened = true;
        }
    }

    if( startPos >= (double)subOptions.size()-maxOptionsInView )
    {
        startPos = (double)subOptions.size()-maxOptionsInView;
    }

    if( startPos < 0 )
    {
        startPos= 0;
    }

    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isClicked )
    {
        isInUse = true;
        int checkBoxWidth = optionHeight+GENERAL_GPE_PADDING;
        int checkBoxHeight = optionHeight;
        if( showCheckboxes==false)
        {
            checkBoxWidth = 0;
            checkBoxHeight = 0;
        }
        //if( input->check_mouse_released(0) )
        {
            int relativeOptionX = viewedSpace->x+elementBox.x-cam->x;
            int relativeOptionY = viewedSpace->y+elementBox.y-cam->y;
            for( int i = 0; i <= maxOptionsInView && i < (int)subOptions.size(); i++)
            {
                if( point_between(input->mouse_x,input->mouse_y,relativeOptionX,relativeOptionY+(i)*optionHeight,
                                  relativeOptionX+elementBox.w,relativeOptionY+(i+1)*optionHeight) )
                {
                    if( showCheckboxes)
                    {
                        if( point_between(input->mouse_x,input->mouse_y,relativeOptionX,relativeOptionY+(i)*optionHeight,
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

    if( maxOptionsInView <= (int)subOptions.size() )
    {
        if( scrollHappened && optionsScroller!=NULL )
        {
            correct_camera();
            optionsScroller->process_self(NULL,NULL );
            startPos = optionsScroller->contextRect.y;
        }
    }
    intedPos = (int)pos;
    //record_error("Success on select box...");
}

void GPE_SelectBoxBasic::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && cam!=NULL && viewedSpace!=NULL )
    {
        intedPos = (int)pos;
        int relativeOptionX = elementBox.x-cam->x;
        int relativeOptionY = elementBox.y-cam->y;
        int foundIconWidth = std::max( optionIconWidth,optionHeight )*3/4;
        gpe->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+elementBox.w,relativeOptionY+elementBox.h,GPE_MAIN_THEME->PopUp_Box_Color, false);

        if( pos>=startPos && pos < startPos+maxOptionsInView )
        {
            if(pos==0 && (int)subOptions.size() > 0)
            {
                gpe->render_rectangle( relativeOptionX,relativeOptionY+(pos-startPos)*optionHeight,relativeOptionX+elementBox.w,relativeOptionY+(pos-startPos+1)*optionHeight,GPE_MAIN_THEME->Button_Border_Color,false);
            }
            else if( pos > 0)
            {
                gpe->render_rectangle( relativeOptionX,relativeOptionY+(pos-startPos)*optionHeight,relativeOptionX+elementBox.w,relativeOptionY+(pos-startPos+1)*optionHeight,GPE_MAIN_THEME->Button_Border_Color,false);
            }
        }
        else if( pos >= startPos+maxOptionsInView )
        {
            //pos = startPos+maxOptionsInView -1;
        }
        GPE_SelectBoxBasic_Option * tOption = NULL;
        int checkBoxWidth = optionHeight+GENERAL_GPE_PADDING;
        int checkBoxHeight = optionHeight;
        if( showCheckboxes==false)
        {
            checkBoxWidth = 0;
            checkBoxHeight = 0;
        }

        GPE_Color * fontRenderColor = GPE_MAIN_THEME->Main_Box_Faded_Font_Color;
        int iStartPos = (int)startPos;
        for( int i = iStartPos; i < (int)(iStartPos+maxOptionsInView) &&  i < (int)subOptions.size(); i++)
        {
            tOption = subOptions.at( i );
            if( tOption!=NULL)
            {
                if( !tOption->sectionHasContent && intedPos!=i)
                {
                    fontRenderColor = GPE_MAIN_THEME->PopUp_Box_Font_Color;
                    gpe->render_rectangle( relativeOptionX,relativeOptionY+(i-iStartPos)*optionHeight,relativeOptionX+elementBox.w,relativeOptionY+(i-iStartPos+1)*optionHeight,GPE_MAIN_THEME->Program_Header_Color,false,228);
                }
                else if( intedPos ==i )
                {
                    fontRenderColor = GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color;
                }
                else
                {
                    fontRenderColor = GPE_MAIN_THEME->PopUp_Box_Font_Color;
                }

                if( showCheckboxes)
                {
                    gpe->render_rectangle( relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING+optionHeight/8,relativeOptionY+1+(i-iStartPos)*optionHeight+optionHeight/8,relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING+optionHeight/2+optionHeight/8,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight*5/8,GPE_MAIN_THEME->Button_Box_Color, false);
                    if( tOption->isChecked)
                    {
                        render_texture_resized(  GPE_CHECKMARK_IMAGE, relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING+optionHeight/8,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight/8,optionHeight/2,optionHeight/2,NULL,-1,-1,GPE_MAIN_THEME->Checkbox_Color );
                    }
                    //gpe->render_rectangle( relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING,relativeOptionY+1+(i-iStartPos)*optionHeight,relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING+optionHeight,relativeOptionY+(i-iStartPos+1)*optionHeight,GPE_MAIN_THEME->Button_Box_Selected_Color, true);
                    gpe->render_rectangle( relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING+optionHeight/8,relativeOptionY+1+(i-iStartPos)*optionHeight+optionHeight/8,relativeOptionX+relativeOptionX+GENERAL_GPE_PADDING+optionHeight/2+optionHeight/8,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight*5/8,GPE_MAIN_THEME->Button_Box_Selected_Color, true);

                    render_new_text( relativeOptionX+GENERAL_GPE_PADDING+checkBoxWidth,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight/2,tOption->optionName,fontRenderColor,FONT_CATEGORY_BAR,FA_LEFT,FA_CENTER,255);
                }
                else
                {
                    render_new_text( relativeOptionX+foundIconWidth+GENERAL_GPE_PADDING*2,relativeOptionY+(i-iStartPos)*optionHeight+optionHeight/2,tOption->optionName,fontRenderColor,FONT_CATEGORY_BAR,FA_LEFT,FA_MIDDLE,255);
                }
                if( tOption->optionTexture!=NULL)
                {
                    if( tOption->useGuiColor)
                    {
                        render_texture_resized( tOption->optionTexture,relativeOptionX+GENERAL_GPE_PADDING,relativeOptionY+(i-iStartPos)*optionHeight+(optionHeight)/2,foundIconWidth,foundIconWidth,NULL,FA_LEFT,FA_MIDDLE,GPE_MAIN_THEME->PopUp_Box_Font_Color );
                    }
                    else
                    {
                        render_texture_resized( tOption->optionTexture,relativeOptionX+GENERAL_GPE_PADDING,relativeOptionY+(i-iStartPos)*optionHeight+(optionHeight)/2,foundIconWidth,foundIconWidth,NULL, FA_LEFT, FA_MIDDLE );
                    }
                }
                else if( tOption->optionSprite!=NULL)
                {
                    if( tOption->useGuiColor)
                    {
                        render_animation_ext( tOption->optionSprite,tOption->subimageIndex,relativeOptionX+GENERAL_GPE_PADDING+checkBoxWidth,relativeOptionY+(i-iStartPos)*optionHeight +(optionHeight-foundIconWidth)/2,foundIconWidth,foundIconWidth,GPE_MAIN_THEME->Main_Box_Font_Color,cam);
                    }
                    else
                    {
                        render_animation_resized( tOption->optionSprite,tOption->subimageIndex,relativeOptionX+GENERAL_GPE_PADDING+checkBoxWidth,relativeOptionY+(i-iStartPos)*optionHeight +(optionHeight-foundIconWidth)/2,foundIconWidth,foundIconWidth,cam);
                    }
                }
            }
        }

        if( iStartPos > 0 )
        {
            gpe->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+elementBox.w,relativeOptionY+4,GPE_MAIN_THEME->Button_Box_Selected_Color,false,228);
        }
        else if( iStartPos+maxOptionsInView < (int)subOptions.size() )
        {
            gpe->render_rectangle( relativeOptionX,relativeOptionY+elementBox.h-4,relativeOptionX+elementBox.w,relativeOptionY+elementBox.h,GPE_MAIN_THEME->Button_Box_Selected_Color,false,228);
        }
        if( isInUse)
        {
            gpe->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+elementBox.w,relativeOptionY+elementBox.h,GPE_MAIN_THEME->Button_Box_Selected_Color, true);
        }
        else
        {
            gpe->render_rectangle( relativeOptionX,relativeOptionY,relativeOptionX+elementBox.w,relativeOptionY+elementBox.h,GPE_MAIN_THEME->Main_Border_Color, true );
        }
    }
    if( maxOptionsInView < (int)subOptions.size() )
    {
        optionsScroller->render_self( viewedSpace, cam );
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
    maxOptionsInView = floor( (float)elementBox.h / (float)optionHeight );
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

void GPE_SelectBoxBasic::set_selection( int selId )
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

    correct_camera();
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


GPE_Input_Field_Color::GPE_Input_Field_Color( std::string startName,std::string placeHolderText)
{
    opName = startName;
    guiListTypeName = "colorpicker";
    guiListTypeId = 2;
    storedColor = new GPE_Color();
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 256;
    elementBox.h = 24;
    fieldElementBox.x = elementBox.x;
    fieldElementBox.y = elementBox.y;
    fieldElementBox.w = elementBox.w;
    fieldElementBox.h = elementBox.h;
    prevelementBoxW = elementBox.w;
    prevelementBoxH = elementBox.h;

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

        /*
        elementBox.h=prevelementBoxH+fontSizeH;
        elementBox.w = std::max( prevelementBoxW, ( fontSizeW *(int)inputLabel.size()  ) );
        fieldElementBox.x = elementBox.x;
        fieldElementBox.y = elementBox.y+fontSizeH;
        fieldElementBox.w = prevelementBoxW;
        fieldElementBox.h = prevelementBoxH;*/
    }
    else
    {
        /*if( showLabel==true)
        {
            elementBox.w = ;
            elementBox.h =
        }*/
        inputLabel = "";
        showLabel = false;
    }
}

void GPE_Input_Field_Color::set_name(std::string newName)
{
    if( (int)newName.size() > 0)
    {
        if( opName == inputLabel )
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
    fieldElementBox.x = elementBox.x+(elementBox.w-32);
    fieldElementBox.y = elementBox.y;
    fieldElementBox.w = 32;
    fieldElementBox.h = prevelementBoxH;
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
            if( point_between(input->mouse_x,input->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
            {
                if (point_between(input->mouse_x,input->mouse_y,fieldElementBox.x+viewedSpace->x-cam->x,fieldElementBox.y+viewedSpace->y-cam->y,fieldElementBox.x+fieldElementBox.w+viewedSpace->x-cam->x,fieldElementBox.y+fieldElementBox.h+viewedSpace->y-cam->y) )
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
                    if( input->check_mouse_released(0) )
                    {
                        isClicked = true;
                    }
                }
                else if( input->check_mouse_released(-1) )
                {
                    clickedOutside = true;
                }
            }
            else if( input->check_mouse_released(-1) )
            {
                clickedOutside = true;
            }
        }
    }

    //
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
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

void GPE_Input_Field_Color::render_self( GPE_Rect * viewedSpace , GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && isEnabled && cam!=NULL)
    {
        if( MAIN_RENDERER->screen_was_cleared() || input->input_received() || forceRedraw)
        {
            if( showLabel && (int)inputLabel.size() > 0)
            {
                render_new_text_ext( elementBox.x-cam->x,elementBox.y-cam->y,inputLabel,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_TOP);
            }

            gpe->render_rectangle( fieldElementBox.x-cam->x,fieldElementBox.y-cam->y,fieldElementBox.x+fieldElementBox.w-cam->x,fieldElementBox.y+fieldElementBox.h-cam->y,storedColor,false);
            if( isInUse)
            {
                gpe->render_rectangle( fieldElementBox.x-cam->x,fieldElementBox.y-cam->y,fieldElementBox.x+fieldElementBox.w-cam->x,fieldElementBox.y+fieldElementBox.h-cam->y,GPE_MAIN_THEME->Button_Box_Highlighted_Color,true);
            }
            else if( isHovered )
            {
                    gpe->render_rectangle( fieldElementBox.x-cam->x,fieldElementBox.y-cam->y,fieldElementBox.x+fieldElementBox.w-cam->x,fieldElementBox.y+fieldElementBox.h-cam->y,GPE_MAIN_THEME->Button_Box_Selected_Color,true);
            }
            else
            {
                    gpe->render_rectangle( fieldElementBox.x-cam->x,fieldElementBox.y-cam->y,fieldElementBox.x+fieldElementBox.w-cam->x,fieldElementBox.y+fieldElementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Color,true);
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
        fieldElementBox.h = newHeight;
    }
    else
    {
        elementBox.h = newHeight;
        fieldElementBox.h = newHeight;
    }
    prevelementBoxH = newHeight;
}

void GPE_Input_Field_Color::set_width(int newWidth)
{
    if( newWidth < 192)
    {
        newWidth = 192;
    }
    if( showLabel)
    {
        fieldElementBox.w = newWidth;
    }
    else
    {
        elementBox.w = newWidth;
        fieldElementBox.w = newWidth;
    }
    prevelementBoxW = newWidth;
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
         placeHolderText = placeHolderText.substr(0,512);
    }
    placeHolderString = placeHolderText;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    elementBox.h = 24;

    FONT_TEXTINPUT->get_metrics("A",&fontSizeW,&fontSizeH);
    if( fontSizeW<=0)
    {
        fontSizeW = 12;
    }
    maxCharactersInView = elementBox.w/fontSizeW;

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
    fieldElementBox.x = elementBox.x;
    fieldElementBox.y = elementBox.y;
    prevelementBoxW = fieldElementBox.w = elementBox.w;
    prevelementBoxH = fieldElementBox.h = elementBox.h;
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
    cursorPos = floor( startXPos+(input->mouse_x-viewedSpace->x-fieldElementBox.x)/fontSizeW);
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
    maxCharactersInView = elementBox.w/fontSizeW;
    cam = GPE_find_camera(cam);
    fieldElementBox.x = elementBox.x;
    fieldElementBox.y = elementBox.y;
    fieldElementBox.w = prevelementBoxW;
    fieldElementBox.h = prevelementBoxH;
    if( showLabel )
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
            if( point_between(input->mouse_x,input->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
            {
                if (point_between(input->mouse_x,input->mouse_y,fieldElementBox.x+viewedSpace->x-cam->x,fieldElementBox.y+viewedSpace->y-cam->y,fieldElementBox.x+fieldElementBox.w+viewedSpace->x-cam->x,fieldElementBox.y+fieldElementBox.h+viewedSpace->y-cam->y) )
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
                    if( input->check_mouse_released(0) )
                    {
                        isClicked = true;
                        hasArrowkeyControl = true;
                        hasScrollControl = false;
                    }
                }
                else if( input->check_mouse_released(-1) )
                {
                    clickedOutside = true;
                    hasScrollControl = false;
                    hasArrowkeyControl = false;
                }
            }
            else if( input->check_mouse_released(-1) )
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
        input->inkeys = "";
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
        if( input->check_mouse_doubleclicked(0) )
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
            input->reset_all_input();

        }
        else if( input->check_mouse_pressed(0) && RESOURCE_TO_DRAG==NULL )
        {
            get_cursor_from_mouse(viewedSpace,cam);
            selectionCursorPos = cursorPos;
            selectionEndCursorPos = cursorPos;
        }
        else if( input->check_mouse_down(0)  && RESOURCE_TO_DRAG==NULL )
        {
            get_cursor_from_mouse(viewedSpace,cam);
            selectionEndCursorPos = cursorPos;
        }
        else if( input->check_mouse_down(1) )
        {
            GPE_open_context_menu(-1,-1,128);
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
            if( input->check_mouse_released(0) )
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
        if( input->down[kb_backspace]  && !input->pressed[kb_backspace]  )
        {
            bscDelay+=0.5;
        }
        else
        {
            bscDelay = -1;
        }
        if( input->down[kb_delete] && !input->pressed[kb_delete] )
        {
            delDelay+=0.5;
        }
        else
        {
            delDelay = -1;
        }
        if( input->down[kb_up] && !input->released[kb_up] && !input->pressed[kb_up] )
        {
            upArrowDelay+=0.5;
        }
        else
        {
            upArrowDelay = -1;
        }
        if( input->down[kb_down] && !input->released[kb_down] && !input->pressed[kb_down] )
        {
            downArrowDelay+=0.5;
        }
        else
        {
            downArrowDelay = -1;
        }
        if( input->down[kb_left] && !input->released[kb_left] && !input->pressed[kb_left] )
        {
            leftArrowDelay+=0.5;
        }
        else
        {
            leftArrowDelay = -1;
        }
        if( input->down[kb_right] && !input->released[kb_right] && !input->pressed[kb_right] )
        {
            rightArrowDelay+=0.5;
        }
        else
        {
            rightArrowDelay = -1;
        }
        if( input->down[kb_enter] && !input->released[kb_enter] && !input->pressed[kb_enter] )
        {
            enterDelay+=0.5;
        }
        else
        {
            enterDelay = -1;
        }
        if( input->down[kb_ctrl]  )
        {
            if( input->down[kb_a])
            {
                select_all();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->down[kb_c])
            {
                copy_selection();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->released[kb_v])
            {
                pasteCommandGiven = true;
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->down[kb_x])
            {
                cut_selection();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->mouseScrollingUp > 0)
            {
                move_left(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->mouseScrollingDown > 0)
            {
                move_right(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( leftArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime)*FPS_RATIO || ( input->pressed[kb_left]  && !input->released[kb_left]  ) )
            {
                if( input->shiftKeyIsPressed)
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
                if( input->shiftKeyIsPressed)
                {
                    selectionEndCursorPos = cursorPos;
                }
                leftArrowDelay = 0;
            }
            else if( rightArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime)*FPS_RATIO || ( input->pressed[kb_right]  && !input->released[kb_right]  ) )
            {
                if( input->shiftKeyIsPressed)
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
                if( input->shiftKeyIsPressed)
                {
                    selectionEndCursorPos = cursorPos;
                }
                rightArrowDelay = 0;
            }
        }
        else if( input->mouseScrollingUp > 0)
        {
            move_left(4);
            showCursor = true;
            cursorTimer = 0;
        }
        else if( input->mouseScrollingDown > 0)
        {
            move_right(4);
            showCursor = true;
            cursorTimer = 0;
        }
        else if( input->down[kb_alt])
        {
            //special alt action
            showCursor = true;
            cursorTimer = 0;
        }
        else if( bscDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( input->pressed[kb_backspace]  && !input->released[kb_backspace] ) )
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
            //input->reset_all_input();
        }
        else if( delDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( input->pressed[kb_delete] && !input->released[kb_delete] ) )
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
                input->released[kb_delete] = false;
            }
            showCursor = true;
            cursorTimer = 0;
        }
        else if( leftArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( input->pressed[kb_left]  && !input->released[kb_left]  ) )
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
        else if( rightArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( input->pressed[kb_right]  && !input->released[kb_right] ) )
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
        else if( (upArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( input->pressed[kb_up]  && !input->released[kb_up] ) ) && cursorHistoryPos >0 )
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
        else if( downArrowDelay > (MAIN_GUI_SETTINGS->textInputDelayTime+1)*FPS_RATIO || ( input->pressed[kb_down]  )  )
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
        else if( input->pressed[kb_enter] )
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
            int moreStringSize = (int)input->inkeys.size();
            if( moreStringSize > 0)
            {
                delete_selection();
                textInputString=get_substring(textInputString,0,cursorPos)+input->inkeys+get_substring(textInputString,cursorPos);
                cursorPos+=(int)input->inkeys.size();
                if( cursorPos>startXPos+maxCharactersInView)
                {
                    startXPos+=moreStringSize;
                }
                showCursor = true;
                cursorTimer = 0;
            }
            input->inkeys = "";
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

void GPE_TextInputBasic::render_self(GPE_Rect * viewedSpace , GPE_Rect * cam ,bool forceRedraw )
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
                    gpe->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Color,false, 128 );
                }
                else
                {
                    gpe->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Error_Box_Color,false, 128 );
                }
            }

            if( showLabel && (int)inputLabel.size() > 0)
            {
                render_new_text( elementBox.x-cam->x,elementBox.y-2-cam->y,inputLabel,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_TOP);
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

                gpe->render_rectangle(
                     fieldElementBox.x+(minHighlightPos-startXPos)*fontSizeW - cam->x,
                     fieldElementBox.y+1 - cam->y,
                     fieldElementBox.x+2+(maxHighlightPos-startXPos)*fontSizeW- cam->x,
                     fieldElementBox.y+fieldElementBox.h-1 - cam->y,
                     GPE_MAIN_THEME->Input_Selected_Color,false, 128 );
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
                render_new_text( fieldElementBox.x+4-cam->x,fieldElementBox.y+fieldElementBox.h/2- cam->y,get_substring(textInputString,startXPos,subLength),GPE_MAIN_THEME->Input_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE,255);
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
                render_new_text( fieldElementBox.x+4-cam->x,fieldElementBox.y+fieldElementBox.h/2 - cam->y,get_substring(placeHolderString,0,subLength),GPE_MAIN_THEME->Input_Faded_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE);
            }
            if(showBorderBox)
            {
                if( isInUse )
                {
                    gpe->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Highlight_Outline_Color,true);
                }
                else if( isHovered )
                {
                    gpe->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Highlight_Alt_Color,true);
                }
                else
                {
                    gpe->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Outline_Color,true);
                }
            }
        }

        if(isInUse && prevCursorPos!=cursorPos && prevCursorPos >=startXPos && prevCursorPos <= startXPos+maxCharactersInView )
        {
            gpe->render_vertical_line_color( fieldElementBox.x+GENERAL_GPE_PADDING/2+(prevCursorPos-startXPos)*fontSizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Color);
        }

        if(isInUse && cursorPos >=startXPos && cursorPos <= startXPos+maxCharactersInView )
        {
            if(showCursor)
            {
                gpe->render_vertical_line_color( fieldElementBox.x+GENERAL_GPE_PADDING/2+(cursorPos-startXPos)*fontSizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Font_Color);
            }
            else
            {
                gpe->render_vertical_line_color( fieldElementBox.x+GENERAL_GPE_PADDING/2+(cursorPos-startXPos)*fontSizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Color);
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
    input->reset_all_input();
}

void GPE_TextInputBasic::set_height(int newHeight)
{
    if( showLabel)
    {
        fieldElementBox.h = newHeight;
    }
    else
    {
        elementBox.h = newHeight;
        fieldElementBox.h = newHeight;
    }
    prevelementBoxH = newHeight;
}

void GPE_TextInputBasic::set_label(std::string newLabel)
{
    if( (int)newLabel.size() > 0)
    {
        inputLabel = newLabel;
        showLabel = true;

        elementBox.h=prevelementBoxH+fontSizeH;
        elementBox.w = std::max( prevelementBoxW, ( fontSizeW *(int)inputLabel.size()  ) );
        fieldElementBox.x = elementBox.x;
        fieldElementBox.y = elementBox.y+fontSizeH;
        fieldElementBox.w = prevelementBoxW;
        fieldElementBox.h = prevelementBoxH;
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
        /*if( opName == inputLabel )
        {
            set_label(newName);
        }*/
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
        fieldElementBox.w = newWidth;
    }
    else
    {
        elementBox.w = newWidth;
        fieldElementBox.w = newWidth;
    }
    prevelementBoxW = newWidth;
}

void GPE_TextInputBasic::set_max_width( int nMW)
{
    maxWidth = nMW;
    if( maxWidth > 0 )
    {
        if( elementBox.w > maxWidth || fieldElementBox.w > maxWidth)
        {
            fieldElementBox.w = maxWidth;
            elementBox.w = maxWidth;
            prevelementBoxW = maxWidth;
        }
    }
}

void GPE_TextInputBasic::set_max_height( int nMH)
{
    maxHeight = nMH;
    if( maxHeight > 0 && elementBox.h > maxHeight)
    {
        elementBox.h = maxHeight;
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
         placeHolderText = placeHolderText.substr(0,512);
    }
    placeHolderString = placeHolderText;
    showPlaceHolder = true;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    elementBox.h = 24;

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
    maxCharactersInView = elementBox.w/fontSizeW;

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

void GPE_SyntaxLine::render_tokens(GPE_Font * fontIn, std::string sIn,int xPos, int yPos, int lineStartPos, int lineMaxPos, GPE_Color * renderColor, bool isNumber)
{
    int maxParseSize = (int)foundParses.size();
    if( maxParseSize > 0)
    {
        std::string rString = "";
        GPE_ParsedText* tempParseContent = NULL;
        if( renderColor==NULL)
        {
            renderColor = GPE_MAIN_THEME->Text_Box_Font_Color;
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
                        render_bitmap_text( xPos+fontWidth*(tempParseContent->textStart-lineStartPos),yPos, rString,renderColor,fontIn,FA_LEFT,FA_TOP );
                    }
                    else
                    {
                        render_new_text( xPos+fontWidth*(tempParseContent->textStart-lineStartPos),yPos, rString,renderColor,fontIn,FA_LEFT,FA_TOP );
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
        elementBox.w = imgSrc->get_width();
        elementBox.h = imgSrc->get_height();
    }
    else
    {
        elementBox.w = 32;
        elementBox.h = 32;
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
        imgSrc->load_new_texture(fileNameIn);
        elementBox.w = imgSrc->get_width();
        elementBox.h = imgSrc->get_height();
    }
}

void GPE_Label_Image::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered && (int)webUrl.size() > 3)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
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

void GPE_Label_Image::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && imgSrc!=NULL && cam!=NULL )
    {
        render_texture_resized( imgSrc,elementBox.x-cam->x,elementBox.y-cam->y,elementBox.w ,elementBox.h,NULL,FA_LEFT,FA_TOP);
    }
    else
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_Color,true);
    }

    if( isInUse)
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
    }
}

void GPE_Label_Image::set_image( GPE_Texture * imgIn )
{
    imgSrc = imgIn;
    if( imgSrc!=NULL )
    {
        elementBox.w = imgSrc->get_width();
        elementBox.h = imgSrc->get_height();
    }
}

void GPE_Label_Image::set_width( int newW)
{
    //if( newW!=elementBox.w)
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
            elementBox.w = (float)newW*resizeAspect;
            if( imgSrc!=NULL )
            {
                elementBox.h = ceil( (float)newW * (float)imgSrc->get_height()/(float)imgSrc->get_width() );
            }
        }
        else
        {
            elementBox.w = 0;
            elementBox.h = 0;
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
        elementBox.h = (float)newH*resizeAspect;
        if( imgSrc!=NULL )
        {
            elementBox.w = ceil( (float)newH * (float)imgSrc->get_width()/(float)imgSrc->get_height() );
        }
    }
    else
    {
        elementBox.w = 0;
        elementBox.h = 0;
    }
}

void GPE_Label_Image::set_size( int newW, int newH)
{
    elementBox.w = newW;
    elementBox.h = newH;
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
        elementBox.w = bWid;
        elementBox.h = bHgt;
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
        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
}

void GPE_Label_Text::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        render_new_text( elementBox.x-cam->x,elementBox.y-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_LABEL,FA_LEFT,FA_TOP);
    }
    if( isInUse)
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
    }
}

void GPE_Label_Text::set_name(std::string nameIn)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
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
        elementBox.w = bWid;
        elementBox.h = bHgt;
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
        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
}

void GPE_Label_Error::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        render_new_text( elementBox.x-cam->x,elementBox.y-cam->y,opName,GPE_MAIN_THEME->Input_Error_Font_Color,FONT_LABEL,FA_LEFT,FA_TOP);
    }
    if( isInUse)
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,true);
    }
}

void GPE_Label_Error::set_name(std::string nameIn)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
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
        elementBox.w = bHgt;
        elementBox.h = bWid;
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
        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
}

void GPE_Label_90Degree::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        render_new_text_rotated( elementBox.x-cam->x,elementBox.y-cam->y+elementBox.h,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_LABEL,90);
    }
    if( isInUse)
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
    }
}

void GPE_Label_90Degree::set_name(std::string nameIn)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bHgt;
        elementBox.h = bWid;
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

void GPE_Label_Paragraph::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && viewedSpace!=NULL && cam!=NULL )
    {
        for( int i =0; i < (int)paragraphLines.size(); i++)
        {
            render_new_text( elementBox.x-cam->x,elementBox.y-cam->y+(GENERAL_GPE_PADDING+defaultFontHeight)*i,paragraphLines[i],GPE_MAIN_THEME->Main_Box_Font_Color,FONT_PARAGRAGH,FA_LEFT,FA_TOP);
        }
    }
    if( isInUse)
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
    }
}

void GPE_Label_Paragraph::set_name(std::string nameIn)
{

}

void GPE_Label_Paragraph::set_width(int newWidth)
{
    if( elementBox.w!=newWidth)
    {
        elementBox.w = newWidth;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_height( int newHeight)
{
    if( elementBox.h!=newHeight)
    {
        elementBox.h = newHeight;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_max_width( int nMW)
{
    if( elementBox.w!=nMW)
    {
        elementBox.w = nMW;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_max_height( int nMH)
{
    if( elementBox.h!=nMH)
    {
        elementBox.h = nMH;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::update_paragraph()
{
    if( elementBox.w > 0 )
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
                maxMessageWidth = ( elementBox.w -GENERAL_GPE_PADDING)/ defaultFontWidth;

                if( (int)paragraphText.size() > 0)
                {
                    wrap_string(paragraphText,messageSubTitles,maxMessageWidth,-1);
                }

                maxMessageWidth = 0;
                if( messageSubTitles.size() <= 0)
                {
                    add_line("");
                    elementBox.h = 0;
                }
                else
                {
                    for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                    {
                        add_line( messageSubTitles.at(iSubMessage) );
                    }
                    elementBox.h = ( (int)messageSubTitles.size() ) *(GENERAL_GPE_PADDING+defaultFontHeight);
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
        elementBox.w = bWid;
        elementBox.h = bHgt;
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
        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_c) )
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

void GPE_Label_Title::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        render_new_text( elementBox.x-cam->x,elementBox.y+elementBox.h/2-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_LABEL_TITLE,FA_LEFT,FA_MIDDLE);
    }
    if( isInUse)
    {
        gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
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
        elementBox.w = bWid;
        elementBox.h = bHgt;
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
        elementBox.w = bWid;
        elementBox.h = bHgt;
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
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
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

void GPE_TextURL::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    if( forceRedraw && (int)opName.size() > 0)
    {
        if( wasClicked)
        {
            render_new_text( elementBox.x-cam->x,elementBox.y-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_URL_Visited_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
            gpe->render_horizontal_line_color( elementBox.y+elementBox.h-cam->y,elementBox.x-cam->x,elementBox.x+elementBox.w-cam->x,GPE_MAIN_THEME->Main_Box_Font_URL_Visited_Color);
        }
        else
        {
            render_new_text( elementBox.x-cam->x,elementBox.y-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_URL_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
            gpe->render_horizontal_line_color( elementBox.y+elementBox.h-cam->y,elementBox.x-cam->x,elementBox.x+elementBox.w-cam->x,GPE_MAIN_THEME->Main_Box_Font_URL_Color);
        }
        if( isInUse)
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
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
        elementBox.w = bWid;
        elementBox.h = bHgt;
    }
    opName = nameIn;
}


GPE_CheckBoxBasic::GPE_CheckBoxBasic(std::string name, std::string descriptor, int xx, int yy, bool isChecked )
{
    guiListTypeName = "checkbox";
    guiListTypeId = 1;
    opName = name;
    descriptionText = descriptor;

    elementBox.x = xx;
    elementBox.y = yy;
    elementBox.w = clickBoxW = 16;
    elementBox.h = clickBoxH = 16;
    labelBoxW = 0;
    labelBoxH = 0;
    if( (int)opName.size()>0)
    {
        DEFAULT_FONT->get_metrics(opName,&labelBoxW, &labelBoxH);
        elementBox.w+= labelBoxW;
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


void GPE_CheckBoxBasic::prerender_self(  )
{
    //elementBox.w = clickBoxW;
    //elementBox.h = clickBoxH;
}

void GPE_CheckBoxBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    isHovered = false;
    if(viewedSpace!=NULL && cam!=NULL)
    {
        if( point_between(input->mouse_x,input->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
        {
            if (point_between(input->mouse_x,input->mouse_y,elementBox.x+viewedSpace->x-cam->x,elementBox.y+viewedSpace->y-cam->y,elementBox.x+elementBox.w+viewedSpace->x-cam->x,elementBox.y+elementBox.h+viewedSpace->y-cam->y) )
            {
                isHovered = true;
                if( isHovered)
                {
                    MAIN_OVERLAY->update_tooltip(descriptionText);
                    if( input->check_mouse_released(0) )
                    {
                        isClicked = !isClicked;
                        isInUse = true;
                    }
                }
            }
            else if( input->check_mouse_released(-1) )
            {
                isInUse = false;
            }
        }
        else if( input->check_mouse_released(-1) )
        {
            isInUse = false;
        }
        if( (isInUse || isHovered ) &&( input->check_keyboard_pressed( kb_enter ) || input->check_keyboard_pressed( kb_space )  ) )
        {
            isClicked = !isClicked;
        }

    }
}

void GPE_CheckBoxBasic::render_self( GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && forceRedraw )
    {
        if( isInUse)
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Button_Box_Highlighted_Color,false);
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
        }
        else if( isHovered)
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Button_Box_Selected_Color,false);
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
        }
        else
        {
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Main_Border_Color,false);
            gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Main_Box_Font_Color,true);
        }
        if( isClicked)
        {
            render_texture_resized(  GPE_CHECKMARK_IMAGE, elementBox.x-cam->x+clickBoxW/8,elementBox.y-cam->y+clickBoxW/8,clickBoxW*3/4,clickBoxH*3/4,NULL,-1,-1,GPE_MAIN_THEME->Checkbox_Color);
        }
        render_new_text( elementBox.x+clickBoxW+GENERAL_GPE_PADDING-cam->x,elementBox.y+elementBox.h-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_CHECKBOX,FA_LEFT,FA_BOTTOM,255);
    }
}

void GPE_CheckBoxBasic::set_checkbox_size(int nBoxSize, bool resizeelementBox)
{
    if( resizeelementBox)
    {
        elementBox.w = nBoxSize+labelBoxW;
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
    elementBox.x = cX;
    elementBox.y = cY;
    elementBox.w = 0;
    elementBox.h = 32;
    opWidth = elementBox.w;
    opHeight = elementBox.h;

    if( (int)opName.size()>0)
    {
        int textW = 0;
        int textH = 0;
        DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        opWidth = textW;
        if( elementBox.w < textW )
        {
            elementBox.w = textW+GENERAL_GPE_PADDING*2;
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
            elementBox.w = textW+GENERAL_GPE_PADDING*2;
            //newTex->loadFromRenderedText(MAIN_RENDERER,newOption,GPE_MAIN_THEME->Main_Box_Font_Color,DEFAULT_FONT);
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
        elementBox.h +=32;
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
            elementBox.w = textW+GENERAL_GPE_PADDING*2;
            //newTex->loadFromRenderedText(MAIN_RENDERER,optionName,GPE_MAIN_THEME->Main_Box_Font_Color,DEFAULT_FONT);
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
            elementBox.h +=32;
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
    elementBox.w = GENERAL_GPE_PADDING*2;
    elementBox.h = 32;
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
        elementBox.w += maxWidthText;
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
                elementBox.h+=32;
            }
        }
        elementBox.w+=maxWidthText;
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
                    elementBox.h -=32;
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
            elementBox.h -=32;
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

void GPE_RadioButtonControllerBasic::prerender_self(  )
{

}

void GPE_RadioButtonControllerBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        int buttonXPos = elementBox.x+viewedSpace->x+16-cam->x;
        int buttonYPos = elementBox.y+viewedSpace->y+32-cam->y;
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
                if(point_between( input->mouse_x,input->mouse_y,buttonXPos,buttonYPos,buttonXPos+32,buttonYPos+32) )
                {
                    hoveredOption = i;
                    if( input->check_mouse_released(0) )
                    {
                        selectedId = i;
                        selectionClicked = true;
                    }
                }
                buttonYPos+=32;
            }
            if( hasArrowkeyControl && isInUse )
            {
                if( input->check_keyboard_down(kb_left) || input->check_keyboard_down(kb_up) )
                {
                    selectedId--;
                }
                if( input->check_keyboard_down(kb_right) || input->check_keyboard_down(kb_down) )
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

void GPE_RadioButtonControllerBasic::render_self(  GPE_Rect * viewedSpace, GPE_Rect * cam ,bool forceRedraw )
{
    if( forceRedraw )
    {
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if( viewedSpace!=NULL && cam!=NULL)
        {
            int buttonXPos = elementBox.x-cam->x;
            int buttonYPos = elementBox.y-cam->y;
            if(showBorderBox)
            {
                if((int)opName.size()>0 )
                {
                    gpe->render_line( buttonXPos,buttonYPos+GPE_TITLE_BPADDING,buttonXPos+GPE_TITLE_BPADDING,buttonYPos+GPE_TITLE_BPADDING,GPE_MAIN_THEME->Button_Box_Color,255);
                    gpe->render_line( buttonXPos+GPE_TITLE_BPADDING*2+opWidth,buttonYPos+GPE_TITLE_BPADDING,
                                buttonXPos+elementBox.w,buttonYPos+GPE_TITLE_BPADDING,GPE_MAIN_THEME->Button_Box_Color,255);

                    gpe->render_line( buttonXPos,buttonYPos+GPE_TITLE_BPADDING,buttonXPos,buttonYPos+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,255);
                    gpe->render_line( buttonXPos,buttonYPos+elementBox.h,buttonXPos+elementBox.w,buttonYPos+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,255);
                    gpe->render_line( buttonXPos+elementBox.w,buttonYPos+GPE_TITLE_BPADDING,buttonXPos+elementBox.w,buttonYPos+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,255);
                    render_new_text(  buttonXPos+GPE_TITLE_BPADDING*1.5,buttonYPos+GPE_TITLE_BPADDING/2,opName,GPE_MAIN_THEME->Main_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,255);
                }
                else
                {
                    gpe->render_rectangle( buttonXPos,buttonYPos,buttonXPos+elementBox.w,buttonYPos+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,255);
                }
                buttonXPos += 16;
                buttonYPos += 32;
                int renderedInCol = 0;

                GPE_KeyPair * kp = NULL;
                for(int i = 0; i  < (int)subOptions.size(); i++)
                {
                    if( i == hoveredOption )
                    {
                        render_animation_resized(  GPE_RadioButton_GFX,1, buttonXPos,buttonYPos, 16,16,NULL );
                    }
                    else
                    {
                        render_animation_resized(  GPE_RadioButton_GFX, 0,buttonXPos,buttonYPos, 16,16,NULL );
                    }
                    if( i == selectedId )
                    {
                        render_animation_resized(  GPE_RadioButton_GFX, 2, buttonXPos,buttonYPos, 16,16,NULL );
                    }
                    kp = subOptions[i];
                    if( kp!=NULL)
                    {
                        render_new_text(  buttonXPos+32,buttonYPos,kp->keyString,GPE_MAIN_THEME->Main_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP,255);
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
                            buttonYPos = elementBox.y+32;
                        }
                    }
                    */
                }
            }
            if( isInUse)
            {
                gpe->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
            }
        }
    }
}

void GPE_open_context_menu(int menuXPos,int menuYPos, int newWidth )
{
    if( menuXPos<0)
    {
        menuXPos = input->mouse_x;
    }
    if( menuYPos<0)
    {
        menuYPos = input->mouse_y;
    }
    if( MAIN_CONTEXT_MENU!=NULL)
    {
        MAIN_CONTEXT_MENU->clear_menu();
        MAIN_CONTEXT_MENU->subMenuIsOpen = true;
        MAIN_CONTEXT_MENU->set_position(menuXPos, menuYPos);
        if( newWidth > 0)
        {
            MAIN_CONTEXT_MENU->set_width(newWidth);
        }
        else
        {
            if( MAIN_CONTEXT_MENU->get_width() < 128 )
            {
                MAIN_CONTEXT_MENU->set_width(128);
            }
        }
    }
}

void GPE_close_context_menu()
{
    if( MAIN_CONTEXT_MENU!=NULL && MAIN_CONTEXT_MENU->subMenuIsOpen)
    {
        MAIN_CONTEXT_MENU->clear_menu();
        MAIN_CONTEXT_MENU->subMenuIsOpen = false;
        input->reset_all_input();
        if( MAIN_OVERLAY!=NULL)
        {
            MAIN_OVERLAY->render_frozen_screenshot();
        }
    }
}

int get_popupmenu_result(GPE_Rect * cam, bool redrawScreen, bool autoResize )
{
    //RESOURCE_TO_DRAG = NULL;
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    MAIN_OVERLAY->take_frozen_screenshot();
    input->reset_all_input();
    int returnValue = -1;
    bool exitOperation = false;
    if( MAIN_CONTEXT_MENU!=NULL)
    {
        MAIN_CONTEXT_MENU->hoverOption = MAIN_CONTEXT_MENU->selectedOption = 0;
        if( autoResize )
        {
            //Resize popup 1st layer
            MAIN_CONTEXT_MENU->resize_self();
        }
    }
    else
    {
        return -1;
    }
    bool firstFrame = true;
    while(exitOperation==false)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
        //Start the frame timer
        GPE_TIMER_CAP->start();
        //gets user input
        if( !firstFrame)
        input->handle(true,true);
        //record_error("Viewing context menu...");
        if( input->windowEventHappendInFrame)
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
                    if( input->check_mouse_released(kb_anykey)  )
                    {
                        record_error("Releasing due to mouse input...");
                        GPE_close_context_menu();
                        exitOperation = true;
                    }
                    else if( WINDOW_WAS_JUST_RESIZED  )
                    {
                        record_error("Releasing due to WINDOW_WAS_JUST_RESIZED...");
                        GPE_close_context_menu();
                        exitOperation = true;
                    }
                    else if( input->released[kb_esc] || input->released[kb_enter] )
                    {
                        record_error("Releasing due esc/enter...");
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
                    MAIN_RENDERER->set_viewpoint( NULL );
                    //if( redrawScreen)
                    {
                        MAIN_OVERLAY->render_frozen_screenshot( );
                    }
                    //Update screen
                    MAIN_CONTEXT_MENU->render_self(cam, true);
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
        firstFrame = false;
    }
    GPE_close_context_menu();
    input->reset_all_input();
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
    //if( input->input_received() )
    {
        //Update screen
        gpe->render_rectangle( 0,0,256,96,GPE_MAIN_THEME->PopUp_Box_Color,false);
        gpe->render_rectangle( 0,0,256,96,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
        render_new_text( GENERAL_GPE_PADDING,GENERAL_GPE_PADDING,displayMessageTitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
        render_new_text( GENERAL_GPE_PADDING,GENERAL_GPE_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,displayMessageSubtitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
        render_new_text( GENERAL_GPE_PADDING,GENERAL_GPE_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,displayMessageString,GPE_MAIN_THEME->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
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
    MAIN_OVERLAY->take_frozen_screenshot( );
    input->reset_all_input();
    bool exitOperation = false;
    bool windowBeingDragged = false;

    GPE_Rect elementBox;

    elementBox.w = SCREEN_WIDTH*3/4 -128 - GENERAL_GPE_PADDING*4;
    elementBox.h = SCREEN_HEIGHT*3/4 - 64 - GENERAL_GPE_PADDING*4;
    elementBox.x = SCREEN_WIDTH - elementBox.w;
    elementBox.y = SCREEN_HEIGHT - elementBox.h;
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
        maxMessageWidth = (elementBox.w-64) / defaultFontWidth;
        maxMessageHeight = (elementBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);
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
            elementBox.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
            elementBox.y = (SCREEN_HEIGHT - elementBox.h)/2;
        }
        else
        {
            elementBox.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
            elementBox.y = (SCREEN_HEIGHT - elementBox.h)/2;
        }

        maxMessageWidth = (int)messageTitle.size();
        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
        {
            if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
            {
                maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
            }
        }

        elementBox.w = (maxMessageWidth*defaultFontWidth)+128 + GENERAL_GPE_PADDING*4;
        elementBox.x = (SCREEN_WIDTH - elementBox.w)/2;

        while(exitOperation==false)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            //Start the frame timer
            GPE_TIMER_CAP->start();
            //gets user input
            input->handle(true,true);
            //GPE_MAIN_GUI->reset_gui_info();

            if( windowBeingDragged )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZEALL);
                if( input->check_mouse_down(0) )
                {
                    elementBox.x = input->mouse_x-elementBox.w/2;
                    elementBox.y = input->mouse_y;
                }
                else
                {
                    windowBeingDragged = false;
                }

                if( elementBox.x+elementBox.w > SCREEN_WIDTH )
                {
                    elementBox.x = SCREEN_WIDTH - elementBox.w;
                }

                if( elementBox.y+elementBox.h > SCREEN_HEIGHT )
                {
                    elementBox.y = SCREEN_HEIGHT - elementBox.h;
                }

                if( elementBox.x < 0)
                {
                    elementBox.x = 0;
                }

                if( elementBox.y < 0)
                {
                    elementBox.y = 0;
                }
            }
            else
            {
                if( input->check_mouse_released(0) )
                {
                    if( point_between(input->mouse_x,input->mouse_y,elementBox.x+elementBox.w-32,elementBox.y,elementBox.x+elementBox.w,elementBox.y+30) )
                    {
                        exitOperation = true;
                    }
                }
                else if( input->check_mouse_down(0) )
                {
                    if( point_between(input->mouse_x,input->mouse_y,elementBox.x,elementBox.y,elementBox.x+elementBox.w-32,elementBox.y+32) )
                    {
                        windowBeingDragged = true;
                    }
                }
            }
            if( okButton!=NULL)
            {
                okButton->set_coords(elementBox.x+( elementBox.w-okButton->get_width() )/2,elementBox.y+elementBox.h-GENERAL_GPE_PADDING-okButton->get_height() );
                okButton->process_self( NULL, NULL);
                if( okButton->is_clicked() && !windowBeingDragged )
                {
                    exitOperation = true;
                }
            }
            if( input->check_keyboard_released(kb_esc) || input->check_keyboard_released(kb_space) || input->check_keyboard_released(kb_enter) || WINDOW_WAS_JUST_RESIZED)
            {
                exitOperation = true;
            }

            calculate_avg_fps();
            MAIN_RENDERER->reset_viewpoint();
            //if( WINDOW_WAS_JUST_RESIZED )
            {
                gpe->render_rectangle( 0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GPE_MAIN_THEME->Program_Color, false, 255);
                MAIN_OVERLAY->render_frozen_screenshot( );
            }
            //Update screen
            gpe->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Color,false);

            gpe->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,false);

            render_new_text( elementBox.x+elementBox.w/2,elementBox.y+GENERAL_GPE_PADDING,messageTitle,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP);
            render_new_text( elementBox.x+elementBox.w-GENERAL_GPE_PADDING,elementBox.y+GENERAL_GPE_PADDING,"X",GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,DEFAULT_FONT,FA_RIGHT,FA_TOP);

            for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
            {
                render_new_text( elementBox.x+32,elementBox.y+GENERAL_GPE_PADDING+32+iSubMessage*(defaultFontHeight+GENERAL_GPE_PADDING),messageSubTitles.at(iSubMessage),GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
            }
            if( okButton!=NULL)
            {
                okButton->render_self(  NULL, NULL, true);
            }
            if( point_within_rect(input->mouse_x,input->mouse_y,&elementBox) )
            {
                gpe->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Highlight_Alt_Color,true);
            }
            else
            {
                gpe->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
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
    input->reset_all_input();
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
    MAIN_OVERLAY->take_frozen_screenshot( );
    input->reset_all_input();
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

    GPE_Rect elementBox;
    elementBox.w = 528;
    elementBox.h = 320;
    elementBox.x = (SCREEN_WIDTH - elementBox.w)/2;
    elementBox.y = 72;
    elementBox.x = (SCREEN_WIDTH - elementBox.w)/2;
    elementBox.y = 72;
    maxMessageWidth = (elementBox.w-32) / defaultFontWidth;
    maxMessageHeight = (elementBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);
    int newBarX2Pos = elementBox.x+elementBox.w;
    int newBarY2Pos = elementBox.w+elementBox.h;
    MAIN_OVERLAY->render_frozen_screenshot( );

    if( defaultFontWidth > 0 && defaultFontHeight > 0)
    {
        maxMessageWidth = (elementBox.w-32) / defaultFontWidth;
        maxMessageHeight = (elementBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);

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
            elementBox.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
        }
        else
        {
            elementBox.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
        }

        maxMessageWidth = (int)messageTitle.size();
        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
        {
            if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
            {
                maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
            }
        }

        elementBox.x = (SCREEN_WIDTH-elementBox.w)/2;
        newBarX2Pos = elementBox.x+elementBox.w;
        newBarY2Pos = elementBox.w+elementBox.h;
        int elementBoxMinWidth = 64;
        int elementBoxMinHeight = elementBox.h;

        while(exitOperation==false )
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            //Start the frame timer
            GPE_TIMER_CAP->start();
            //gets user input
            input->handle(true,true);
            //GPE_MAIN_GUI->reset_gui_info();

            if( input->check_mouse_down(0) && !input->check_mouse_pressed(0) )
            {
                if( boxIsMoving)
                {
                    GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZEALL);
                    elementBox.x = input->mouse_x-elementBox.w/2;
                    elementBox.y = input->mouse_y;
                }
                else if( boxBeingResized)
                {
                    GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
                    newBarX2Pos = input->mouse_x;
                    if( newBarX2Pos > elementBox.x+elementBoxMinWidth && newBarX2Pos < SCREEN_WIDTH-32 && newBarX2Pos> 0 )
                    {
                        elementBox.w = newBarX2Pos-elementBox.x;
                        boxWasResized = true;
                    }
                    newBarY2Pos = input->mouse_y;
                    if( newBarY2Pos > elementBox.y+elementBoxMinHeight && newBarY2Pos < SCREEN_HEIGHT-32 && newBarY2Pos > 0 )
                    {
                        elementBox.h = newBarY2Pos-elementBox.y;
                        boxWasResized = true;
                    }
                }
            }

            if( point_between(input->mouse_x,input->mouse_y,elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32) )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZEALL);
                if( input->check_mouse_pressed(0) )
                {
                    boxIsMoving = true;
                }
            }
            else if( point_between(input->mouse_x,input->mouse_y,elementBox.x+elementBox.w-32,elementBox.y+elementBox.h-32,elementBox.x+elementBox.w,elementBox.y+elementBox.h) )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
                if( input->check_mouse_pressed(0) )
                {
                    boxBeingResized = true;
                }
            }

            if( input->check_mouse_released(-1) )
            {
                boxIsMoving = false;
                boxBeingResized = false;
                GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            }
            if( elementBox.w < elementBoxMinWidth)
            {
                elementBox.w = elementBoxMinWidth;
            }
            if( elementBox.h < elementBoxMinHeight)
            {
                elementBox.h = elementBoxMinHeight;
            }
            if(  elementBox.x+elementBox.w > SCREEN_WIDTH)
            {
                elementBox.x = SCREEN_WIDTH-elementBox.w;
                boxWasResized = true;
            }
            if( elementBox.x < 0 )
            {
                elementBox.x = 0;
                boxWasResized = true;
            }

            if(  elementBox.y+elementBox.h > SCREEN_HEIGHT )
            {
                elementBox.y = SCREEN_HEIGHT-elementBox.h;
                boxWasResized = true;
            }
            if( elementBox.y < 0 )
            {
                elementBox.y = 0;
                boxWasResized = true;
            }

            if( boxWasResized)
            {
                maxMessageWidth = (elementBox.w-32) / defaultFontWidth;
                maxMessageHeight = (elementBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);

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
                    elementBox.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
                }
                else
                {
                    elementBox.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
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

            yesButton->set_coords( GENERAL_GPE_PADDING+64,elementBox.h-32);
            noButon->set_coords(yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_PADDING,yesButton->get_ypos() );
            if( cancelButton!=NULL)
            {
                cancelButton->set_coords( noButon->get_xpos()+noButon->get_width()+GENERAL_GPE_PADDING,noButon->get_ypos() );
                cancelButton->process_self(&elementBox,&GPE_DEFAULT_CAMERA);
            }
            yesButton->process_self(&elementBox,&GPE_DEFAULT_CAMERA);
            noButon->process_self(&elementBox,&GPE_DEFAULT_CAMERA) ;


            if( input->check_keyboard_released(kb_esc) || MAIN_RENDERER->windowClosed )
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
            else if( input->check_keyboard_released(kb_n) )
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
            else if( input->check_keyboard_released(kb_enter) || yesButton->is_clicked() )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_YES;
            }
            calculate_avg_fps();
            if( !WINDOW_WAS_JUST_RESIZED)
            {
                MAIN_RENDERER->reset_viewpoint();
                //Update screen
                //if( input->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot( );
                }
                MAIN_RENDERER->set_viewpoint( &elementBox);

                gpe->render_rectangle( 0,0,elementBox.w,elementBox.h,GPE_MAIN_THEME->PopUp_Box_Color,false);
                gpe->render_rectangle( 0,0,elementBox.w,32,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,false);

                render_new_text( GENERAL_GPE_PADDING*2,GENERAL_GPE_PADDING,messageTitle,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
                //render_new_text( elementBox.x+GENERAL_GPE_PADDING+32,elementBox.y+64,messageContent,GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
                for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                {
                    render_new_text( 32,GENERAL_GPE_PADDING+32+iSubMessage*(defaultFontHeight+GENERAL_GPE_PADDING),messageSubTitles.at(iSubMessage),GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
                }

                yesButton->render_self( &elementBox,&GPE_DEFAULT_CAMERA);
                noButon->render_self( &elementBox,&GPE_DEFAULT_CAMERA);
                if( cancelButton!=NULL)
                {
                    cancelButton->render_self( &elementBox,&GPE_DEFAULT_CAMERA);
                }
                gpe->render_rectangle( 1,1,elementBox.w-1,elementBox.h-1,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
                gpe->render_rectangle( 0,0,elementBox.w,elementBox.h,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,true);
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
    input->reset_all_input();
    messageSubTitles.clear();
    return returnVal;
}

std::string get_string_from_popup(std::string messageTitle, std::string messageContent, std::string startName)
{
    RESOURCE_TO_DRAG = NULL;
    MAIN_OVERLAY->update_tooltip("");
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    MAIN_OVERLAY->take_frozen_screenshot( );
    input->reset_all_input();
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
    GPE_Rect elementBox;
    elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2-GENERAL_GPE_PADDING;
    elementBox.y = SCREEN_HEIGHT/2-64-GENERAL_GPE_PADDING;
    elementBox.w = promptBoxWidth+128;
    elementBox.h = 192;
    MAIN_OVERLAY->render_frozen_screenshot( );
    while(exitOperation==false)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
        //Start the frame timer
        GPE_TIMER_CAP->start();
        //gets user input
        input->handle(true,true);

        //GPE_MAIN_GUI->reset_gui_info();

        elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2-GENERAL_GPE_PADDING;
        elementBox.y = SCREEN_HEIGHT/2-64-GENERAL_GPE_PADDING;

        newStringBox->set_coords( elementBox.x+GENERAL_GPE_PADDING,elementBox.y+64);
        newStringBox->set_width(elementBox.w - 64);

        yesButton->set_coords( elementBox.x+GENERAL_GPE_PADDING,newStringBox->get_ypos()+newStringBox->get_height() + GENERAL_GPE_PADDING);
        cancelButton->set_coords( yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_PADDING,yesButton->get_ypos() );

        newStringBox->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        yesButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        cancelButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);

        if( input->check_keyboard_released(kb_esc) || cancelButton->is_clicked() )
        {
            exitOperation = true;
            returnVal = "";
        }
        else if( input->check_keyboard_released(kb_enter) || yesButton->is_clicked() )
        {
            exitOperation = true;
            if( newStringBox!=NULL)
            {
                returnVal = newStringBox->get_string();
            }
        }

        calculate_avg_fps();

        //Update screen
        //if( input->windowEventHappendInFrame )
        {
            MAIN_OVERLAY->render_frozen_screenshot( );
        }
        gpe->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Color,false);
        gpe->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,false);

        render_new_text( (SCREEN_WIDTH-promptBoxWidth)/2+GENERAL_GPE_PADDING,SCREEN_HEIGHT/2-64,messageTitle,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
        render_new_text( (SCREEN_WIDTH-promptBoxWidth)/2+GENERAL_GPE_PADDING,SCREEN_HEIGHT/2-32,messageContent,GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
        yesButton->render_self( &GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        cancelButton->render_self( &GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        newStringBox->render_self( &GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
        gpe->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
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

    input->reset_all_input();
    return returnVal;
}

/*
    if( MAIN_CONTEXT_MENU!=NULL && MAIN_CONTEXT_MENU->subMenuIsOpen)
    {
        MAIN_CONTEXT_MENU->clear_menu();
        MAIN_CONTEXT_MENU->subMenuIsOpen = false;
        input->reset_all_input();
        if( GPE_MAIN_GUI!=NULL)
        {
            MAIN_OVERLAY->render_frozen_screenshot( );
        }
    }
}
*/
