/*
paw_gui_themes.cpp
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

#include "paw_gui_themes.h"

//Define GUI Themes to NULL
GPE_Theme * GPE_MAIN_THEME = NULL;
GPE_Theme * GPE_DEFAULT_TEMPLATE = NULL;

//Define GUI fonts to NULL
GPE_Font * font = NULL;
GPE_Font * textboxFont = NULL;
GPE_Font * FONT_BUTTONS_FONT = NULL;
GPE_Font * FONT_CATEGORY_BAR = NULL;
//
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
GPE_Font * GUI_TAB_FONT = NULL;

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
GPE_Font * FONT_STREE_BRANCH = NULL;


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
    Button_Box_Selected_Color = add_color("ButtonBoxHHighlightedAlt",96,96,96);
    Button_Box_Shadow_Color = add_color("ButtonBoxShadow",c_ltgray );
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
    Icon_Box_Highlighted_Color = add_color("IconBoxHighlighted",40,40,40);
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
    Input_Highlight_Font_Color = add_color("InputFontHighlighted",c_blgray );
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
    Main_Box_Font_Highlight_Color = add_color( "MainBoxFontHighlighted",c_white );
    Main_Box_Faded_Font_Color = add_color( "MainBoxFontFaded",c_ltgray );
    Main_Box_Shadow_Color = add_color( "MainBoxFontShadow",c_ltgray );
    Main_Box_Font_URL_Color = add_color( "MainBoxFontURL",115,161,196);
    Main_Box_Font_URL_Hovered_Color = add_color( "MainBoxFontURLHovered", 115,161,196 );
    Main_Box_Font_URL_Visited_Color = add_color( "MainBoxFontURLVisited", 115,161,196 );


    //for errors
    Main_Error_Font_Color = add_color( "Errors",  c_maroon );
    Main_Warning_Font_Color = add_color( "Warnings",  c_blue );
    Main_Suggestion_Font_Color = add_color( "Suggestions",  c_aqua );

    //for pop up boxes like toolbar options, context menu, tool tip, etc. Essentially top Z-layer
    PopUp_Box_Color =  add_color("PopUpBox",35,35,35);
    PopUp_Box_Border_Color = add_color( "PopUpBoxOutline",  c_blgray );
    PopUp_Box_Font_Color = add_color( "PopUpBoxFont",  c_silver );
    PopUp_Box_Highlight_Font_Color = add_color( "PopUpBoxFontHighlighted",  c_ltgray );
    PopUp_Box_Faded_Font_Color = add_color( "PopUpBoxFontFaded",  c_gray );

    PopUp_Box_Highlight_Color = add_color("PopUpBoxHighlighted",0,0,220);
    PopUp_Box_Highlight_Alt_Color = add_color("PopUpBoxHighlightedAlt",0,220,0);
    PopUp_Box_Close_Color = add_color( "PopUpBoxClose",  c_red );
    PopUp_Box_Close_Font_Color = add_color( "PopUpBoxCloseFont",  c_white );
    PopUp_Box_Close_Hightlight_Color = add_color( "PopUpBoxCloseHighlighted",  c_maroon );
    PopUp_Box_Shadow_Color = add_color( "PopUpBoxShadow",  c_ltgray );

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
    Text_Box_Font_Color = add_color( "TextBoxFont",  c_blgray );
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
    GPE_Color * fColor =  new GPE_Color( name, r, g, b);
    themeColors.push_back(fColor);
    return fColor;
}

GPE_Color * GPE_Theme::add_color(std::string name, GPE_Color * savedColor )
{
    GPE_Color * fColor = NULL;
    if( savedColor!=NULL )
    {
        fColor = new GPE_Color( name, savedColor->get_r(), savedColor->get_g(), savedColor->get_b() );
    }
    else
    {
        fColor = new GPE_Color( name, 0,0,0 );
    }
    themeColors.push_back(fColor);
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
            //GPE_Report("Loading Theme ["+themeGlobalLocation+"]...");
            themeLocalLocation = themeLocationIn;
            int equalPos = 0;
            int hashPos = 0;
            std::string firstChar="";
            std::string section="";
            std::string keyString="";
            std::string valString="";
            std::string subValString="";
            std::string currLine="";
            std::string currLineToBeProcessed;
            //double foundFileVersion = 0;
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
                            subValString = valString;
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
                                    //GPE_Report( "Found Color ["+keyString+"] with values: ["+subValString+"]..." );
                                }
                            }
                        }
                    }
                }
            }
            templateFileIn.close();
            return true;
        }
        //GPE_Report( "Theme loaded..." );
    }
    else
    {
        GPE_Report("Unable to open file ["+themeGlobalLocation+"]"+" from ["+themeLocationIn+"] file" );
    }
    return false;
}

bool GPE_Theme::render_background(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL )
    {
        gcanvas->render_rectangle( cam->x, cam->y,cam->w,cam->h,GPE_MAIN_THEME->Program_Color,false);
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
    if( !file_exists(themeGlobalLocation) )
    {
        GPE_Report("Theme File ["+themeGlobalLocation+"] not found...");
        return false;
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

void GPE_change_cursor(SDL_SystemCursor id)
{
    GPE_CurrentCursor = id;
}

bool PAW_GUI_LOAD_FONTS()
{
    if( GPE_Font_Init() == false )
    {
        GPE_Report("    Unable to properly initialize  GPE Font System!\n");
        return false;
    }
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
    gpe->log_error("Using "+mainGuiFontLocation+" font..." );
    //Open the fonts
    font = gfs->open_font( mainGuiFontLocation, 24,fontIsMonoSpaced, "Generic Font");
    FONT_BUTTONS_FONT = gfs->open_font(textEditorFontLocation,12,true, "Buttons Font");

    textboxFont = gfs->open_font( mainGuiFontLocation, 24,fontIsMonoSpaced, "textboxFont");
    FONT_CATEGORY_BAR = gfs->open_font( mainGuiFontLocation, 14,fontIsMonoSpaced, "FONT_CATEGORY_BAR");

    GPE_DEFAULT_FONT = gfs->open_font(mainGuiFontLocation,14,fontIsMonoSpaced, "GPE_DEFAULT_FONT");
    GUI_TAB_FONT = gfs->open_font(mainGuiFontLocation,11,fontIsMonoSpaced, "GPE_TAB_FONT");

    FONT_POPUP = gfs->open_font(mainGuiFontLocation,14,fontIsMonoSpaced, "FONT_POPUP");
    FONT_TOOLBAR = gfs->open_font(mainGuiFontLocation,14,fontIsMonoSpaced, "FONT_TOOLBAR");
    FONT_RESOURCEBAR = gfs->open_font(mainGuiFontLocation,12,fontIsMonoSpaced, "FONT_RESOURCEBAR");
    FONT_STREE_BRANCH = gfs->open_font(mainGuiFontLocation,12,fontIsMonoSpaced, "FONT_STREE");


    FONT_TEXTINPUT = gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_GENERAL");
    FONT_TEXTINPUT_COMMENT = gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_COMMENT");
    FONT_TEXTINPUT_KEYWORD = gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_KEYWORD");
    FONT_TEXTINPUT_FUNCTION = gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_FUNCTION");
    FONT_TEXTINPUT_NUMBER = gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_NUMBER");
    FONT_TEXTINPUT_QUOTE = gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_QUOTE");
    FONT_TEXTINPUT_SYMBOL = gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_SYMBOL");
    FONT_TEXTINPUT_PROJECT_FUNCTION = gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_PROJECT_FUNCTION");
    FONT_TEXTINPUT_PROJECT_KEYWORD = gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTINPUT_PROJECT_KEYWORD");

    FONT_TERM_NAME = gfs->open_font(textEditorFontLocation,11,true, "FONT_TERM_NAME");
    FONT_TERM_DESCRIPTION = gfs->open_font(textEditorFontLocation,11,true, "FONT_TERM_DESCRIPTION");
    FONT_TERM_SCOPE = gfs->open_font(textEditorFontLocation,11,true, "FONT_TERM_SCOPE");
    FONT_TOOLTIP = gfs->open_font(textEditorFontLocation,12,true, "FONT_TOOLTIP");

    FONT_DEFAULT_PROMPT = gfs->open_font(textEditorFontLocation,16,true, "FONT_DEFAULT_PROMPT");
    FONT_CHECKBOX = gfs->open_font(mainGuiFontLocation,12,fontIsMonoSpaced, "FONT_CHECKBOX");

    FONT_HEADER = gfs->open_font( mainGuiFontLocation, 18,fontIsMonoSpaced, "FONT_HEADER");
    FONT_LABEL = gfs->open_font( mainGuiFontLocation, 14,fontIsMonoSpaced, "FONT_LABEL");
    FONT_LABEL_ANCHOR = gfs->open_font( mainGuiFontLocation, 14,fontIsMonoSpaced, "FONT_LABEL_ANCHOR");
    FONT_LABEL_TITLE = gfs->open_font( mainGuiFontLocation, 18,fontIsMonoSpaced, "FONT_LABEL_TITLE");
    FONT_PARAGRAGH = gfs->open_font( textEditorFontLocation, 14,true, "FONT_PARAGRAGH" );
    FONT_STATUSBAR = gfs->open_font( textEditorFontLocation, 12,true, "FONT_STATUSBAR" );

    if(GPE_DEFAULT_FONT==NULL )
    {
        GPE_Report("Unable to load default font");
        return false;
    }

    if(FONT_TOOLBAR==NULL)
    {
        GPE_Report("Unable to load TOOLLBAR font");
        return false;

    }

    if(FONT_TEXTINPUT==NULL)
    {
        GPE_Report("Unable to load INPUT-FIELD font");
        return false;
    }
    return true;
}

void cleanup_fonts()
{
    //Close the fonts that was used
    if( font!=NULL)
    {
        gfs->close_font(font);
        font = NULL;
    }

    if( textboxFont!=NULL)
    {
        gfs->close_font( textboxFont );
        textboxFont = NULL;
    }

    if( FONT_CATEGORY_BAR!=NULL)
    {
        gfs->close_font( FONT_CATEGORY_BAR );
        FONT_CATEGORY_BAR = NULL;
    }
    //

    if( FONT_POPUP!=NULL)
    {
        gfs->close_font( FONT_POPUP );
        FONT_POPUP = NULL;
    }

    if( FONT_TOOLBAR!=NULL)
    {
        gfs->close_font( FONT_TOOLBAR );
        FONT_TOOLBAR = NULL;
    }
    if( FONT_RESOURCEBAR!=NULL)
    {
        gfs->close_font( FONT_RESOURCEBAR );
        FONT_RESOURCEBAR = NULL;
    }
    //
    if( FONT_TEXTINPUT!=NULL)
    {
        gfs->close_font(FONT_TEXTINPUT);
        FONT_TEXTINPUT = NULL;
    }

    if( FONT_TERM_NAME!=NULL)
    {
        gfs->close_font(FONT_TERM_NAME);
        FONT_TERM_NAME = NULL;
    }

    if( FONT_TERM_DESCRIPTION!=NULL)
    {
        gfs->close_font(FONT_TERM_DESCRIPTION);
        FONT_TERM_DESCRIPTION = NULL;
    }

    if( FONT_TERM_SCOPE!=NULL)
    {
        gfs->close_font(FONT_TERM_SCOPE);
        FONT_TERM_SCOPE = NULL;
    }
    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        gfs->close_font(FONT_DEFAULT_PROMPT);
        FONT_DEFAULT_PROMPT = NULL;
    }

    if( FONT_CHECKBOX!=NULL)
    {
        gfs->close_font( FONT_CHECKBOX );
        FONT_CHECKBOX = NULL;
    }
    //
    if( FONT_HEADER!=NULL)
    {
        gfs->close_font( FONT_HEADER );
        FONT_HEADER = NULL;
    }

    if( FONT_LABEL!=NULL)
    {
        gfs->close_font( FONT_LABEL );
        FONT_LABEL = NULL;
    }
    if( FONT_LABEL_ANCHOR!=NULL)
    {
        gfs->close_font( FONT_LABEL_ANCHOR );
        FONT_LABEL_ANCHOR = NULL;
    }

    if( FONT_LABEL_PARAGRAPH!=NULL)
    {
        gfs->close_font( FONT_LABEL_PARAGRAPH );
        FONT_LABEL_PARAGRAPH = NULL;
    }

    if( FONT_LABEL_TITLE!=NULL)
    {
        gfs->close_font( FONT_LABEL_TITLE );
        FONT_LABEL_TITLE = NULL;
    }

    if( FONT_PARAGRAGH!=NULL)
    {
        gfs->close_font( FONT_PARAGRAGH );
        FONT_PARAGRAGH = NULL;
    }
}
