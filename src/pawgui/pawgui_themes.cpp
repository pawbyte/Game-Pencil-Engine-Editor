/*
pawgui_themes.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_themes.h"

namespace pawgui
{
    //Define GUI Themes to NULL
    gui_theme * theme_main = NULL;
    gui_theme * theme_default = NULL;

    //Define GUI fonts to NULL
    gpe::font_base * FONT_BUTTONS_FONT = NULL;
    //
    gpe::font_base * FONT_POPUP = NULL;
    gpe::font_base * FONT_TOOLBAR = NULL;
    gpe::font_base * FONT_RESOURCEBAR = NULL;
    gpe::font_base * FONT_TEXTINPUT = NULL;
    gpe::font_base * FONT_TEXTinput_COMMENT = NULL;
    gpe::font_base * FONT_TEXTinput_KEYWORD = NULL;
    gpe::font_base * FONT_TEXTinput_FUNCTION = NULL;
    gpe::font_base * FONT_TEXTinput_NUMBER = NULL;
    gpe::font_base * FONT_TEXTinput_QUOTE = NULL;
    gpe::font_base * FONT_TEXTinput_BOLD = NULL;
    gpe::font_base * FONT_TEXTinput_SYMBOL = NULL;
    gpe::font_base * FONT_TEXTinput_project_FUNCTION = NULL;
    gpe::font_base * FONT_TEXTinput_project_keyword = NULL;
    gpe::font_base * GUI_TAB_FONT = NULL;

    gpe::font_base * FONT_TERM_NAME = NULL;
    gpe::font_base * FONT_TERM_DESCRIPTION = NULL;
    gpe::font_base * FONT_TERM_SCOPE = NULL;
    gpe::font_base * FONT_TOOLTIP = NULL;

    gpe::font_base * font_default_prompt = NULL;
    gpe::font_base * FONT_LABEL = NULL;
    gpe::font_base * FONT_LABEL_ANCHOR = NULL;
    gpe::font_base * FONT_STATUSBAR = NULL;
    gpe::font_base * FONT_HEADER = NULL;
    gpe::font_base * FONT_LABEL_TITLE = NULL;
    gpe::font_base * FONT_STREE_BRANCH = NULL;


    gui_theme::gui_theme(std::string name, bool isCustomTheme)
    {
        theme_name = name;
        non_default_theme = isCustomTheme;
        if( non_default_theme)
        {
            theme_local_location = name+".gpf";
        }
        else
        {
            theme_local_location = "custom/"+name+".gpf";
        }

        //Background info
        themeBgFileLocation = "";
        theme_texture_bg = NULL;
        //animations
        main_menu_animation = NULL;

        //for bg and standard colors
        program_color = add_color("MainBackground",32,32,32);
        program_color_header = add_color("HeaderBackground",62,62,62);

        //Button colors and such
        button_box_color = add_color("ButtonBox",25,25,25);
        button_font_color = add_color("ButtonFont",249,249,249);
        button_font_highlight_color = add_color("ButtonFontHighlighted",240,240,240);
        button_border_color = add_color("ButtonBorder",16,16,16);
        button_box_highlight_color = add_color("ButtonBoxHighlighted",40,40,40);
        button_box_selected_color = add_color("ButtonBoxHHighlightedAlt",96,96,96);
        button_box_shadow_color = add_color("ButtonBoxShadow",gpe::c_ltgray );
        button_border_color = add_color( "ButtonBorder",gpe::c_dkgray );
        button_border_highlight_color = add_color( "ButtonBorderHighlighted", gpe::c_blgray );
        button_border_selected_color = add_color( "ButtonBorderselected", gpe::c_ltgray );

        //Used mainly for labels, urls and such
        checkbox_color = add_color("Checkbox", 210,180,40 );

        //For icons and Folders
        folder_color = add_color("FolderColor", 192,192,192 );
        folder_color_highlight = add_color("FolderHighlightedColor", 255, 99,71);

        //IconButton colors and such
        icon_box_color = add_color("IconBox",25,25,25);
        icon_font_color = add_color("IconFont",224,224,224);
        icon_font_highlight_color = add_color("IconFontHighlighted",255,255,255);
        icon_font_selected_color = add_color("IconFontselected",235,235,235);
        icon_border_color = add_color("IconBorder",16,16,16);
        icon_box_highlight_color = add_color("IconBoxHighlighted",40,40,40);
        icon_box_selected_color = add_color("IconBoxselected",96,96,96);
        icon_border_highlight_color = add_color("IconBorderHighlighted",gpe::c_blgray );
        icon_border_selected_color = add_color("IconBorderselected", 75, 82, 92 );

        //For input fields and drop down-related menus
        input_color = add_color("Input",27,27,29);
        input_error_Box_color = add_color("InputError",255,29,29);
        input_outline_color = add_color("InputOutline", gpe::c_ltgray );
        input_font_color = add_color("InputFont",gpe::c_ltgray );
        input_faded_font_color = add_color("InputFontFaded",gpe::c_gray );
        input_selected_color = add_color("Inputselected",gpe::c_olive );
        input_highlight_font_color = add_color("InputFontHighlighted", gpe::c_blgray );
        input_error_font_color = add_color("InputErrorFont",228,8,8);
        input_error_outline_color = add_color("InputErrorOutline", gpe::c_maroon );
        input_highlight_color = add_color("InputHighlighted", gpe::c_blue );
        input_highlight_outline_color = add_color("InputHighlightedOutline", gpe::c_blue );
        input_highlight_alt_color = add_color("InputHighlightedAlt",218,140,16);

        main_box_color = add_color("MainBox",16,16,16);
        main_box_faded_color = add_color("MainBoxFaded",32,32,32);
        main_box_highlight_color = add_color("MainBoxHighlighted",24,24,24);
        main_border_color = add_color("MainBorder",27,27,29);
        main_border_highlight_color = add_color("MainBorderHighlighted", 75, 82, 92 );
        main_box_font_color = add_color( "MainBoxFont",gpe::c_white );
        main_box_font_highlight_color = add_color( "MainBoxFontHighlighted",gpe::c_white );
        main_box_faded_font_color = add_color( "MainBoxFontFaded", gpe::c_ltgray );
        main_box_shadow_color = add_color( "MainBoxFontShadow", gpe::c_ltgray );
        main_box_font_url_color = add_color( "MainBoxFontURL",115,161,196);
        main_box_font_url_hovered_color = add_color( "MainBoxFontURLHovered", 115,161,196 );
        main_box_font_url_visited_color = add_color( "MainBoxFontURLVisited", 115,161,196 );


        //for errors
        main_error_font_color = add_color( "Errors",  gpe::c_maroon );
        main_warning_font_color = add_color( "Warnings",  gpe::c_blue );
        main_suggestion_font_color = add_color( "Suggestions",  gpe::c_aqua );

        //for pop up boxes like toolbar options, context menu, tool tip, etc. Essentially top Z-layer
        popup_box_color =  add_color("PopUpBox",35,35,35);
        popup_box_border_color = add_color( "PopUpBoxOutline",  gpe::c_blgray );
        popup_box_font_color = add_color( "PopUpBoxFont",  gpe::c_silver );
        popup_box_highlight_font_color = add_color( "PopUpBoxFontHighlighted",  gpe::c_ltgray );
        popup_box_faded_font_color = add_color( "PopUpBoxFontFaded",  gpe::c_gray );

        popup_box_highlight_color = add_color("PopUpBoxHighlighted",0,0,220);
        popup_box_highlight_alt_color = add_color("PopUpBoxHighlightedAlt",0,220,0);
        popup_box_close_color = add_color( "PopUpBoxClose",  gpe::c_red );
        popup_box_close_font_color = add_color( "PopUpBoxCloseFont",  gpe::c_white );
        popup_box_close_hightlight_color = add_color( "PopUpBoxCloseHighlighted",  gpe::c_maroon );
        popup_box_shadow_color = add_color( "PopUpBoxShadow",  gpe::c_ltgray );

        //For Scroll Boxes
        scroll_box_color = add_color( "ScrollBox",55,55,55);
        scroll_box_border_color = add_color( "ScrollBoxBorder",0,0,0);
        scroll_box_arrow_color = add_color( "ScrollBoxArrow",245, 245, 245);
        scroll_box_camera_color = add_color( "ScrollBoxCamera",85, 85, 85);
        scroll_box_camera_highlight_color = add_color( "ScrollBoxCameraHighlight",128, 128, 128);

        //for text boxes
        text_box_color = add_color( "TextBox",  gpe::c_alblack );
        text_box_outer_color = add_color( "TextBoxOuter",  gpe::c_blgray );
        text_box_outer_font_color = add_color( "TextBoxOuterFont",  gpe::c_ltgray );
        text_box_outline_color = add_color( "TextBoxOutline",  gpe::c_jetblack );
        text_box_highlight_color = add_color("TextBoxHighlighted",50,35,50);
        text_box_font_color = add_color( "TextBoxFont",  gpe::c_blgray );
        text_box_font_highlight_color = add_color( "TextFontHighlighted",  gpe::c_ltgray );
        text_box_font_comment_color = add_color("TextBoxFontComment",117, 113, 94 );

        text_box_font_datatype_color = add_color("TextBoxFontDataType",121, 163, 39 );
        text_box_font_dquote_color = add_color( "TextFontDQuote",  gpe::c_yellow );
        text_box_font_squote_color = add_color( "TextFontSQuote",  gpe::c_aqua );

        text_box_font_function_color = add_color( "TextFontFunction",  gpe::c_teal );
        text_box_font_function_alt_color = add_color( "TextFontFunctionAlt",  gpe::c_lime );

        text_box_font_keyword_color = add_color("TextFontKeyword",249,38,114 );
        text_box_font_keyword_alt_color = add_color("TextFontKeywordAlt",38,114,249);

        text_box_font_variable_color = add_color("TextFontVariable",164,223, 46 );
        text_box_font_variable_alt_color = add_color( "TextFontVariableAlt",  gpe::c_green );

        text_box_font_js_color = add_color( "TextFontJavaScript",  gpe::c_violet );
        text_box_font_js_alt_color = add_color( "TextFontJavaScriptAlt",  gpe::c_violet );

        text_box_font_number_color =  add_color( "TextFontNumber", 170, 126, 249 );
        text_box_font_symbols_color = add_color( "TextFontSymbol",75, 82, 92);

        //Project Variables
        text_box_project_function_color = add_color( "TextFontProjectFunction",135,206,250);
        text_box_project_function_alt_color = add_color( "TextFontProjectFunctionAlt",206,135,250);

        text_box_project_keyword_color = add_color( "TextFontProjectKeyword",200,131,30 );
        text_box_project_keyword_alt_color = add_color( "TextFontProjectKeywordAlt",50,35,150);
    }

    gui_theme::~gui_theme()
    {
        gpe::color * tempColor = NULL;
        for(int i = (int)theme_colors.size()-1; i >=0; i-- )
        {
            tempColor = theme_colors[i];
            if( tempColor!=NULL )
            {
                delete tempColor;
                tempColor = NULL;
            }
        }
        theme_colors.clear();
    }

    gpe::color * gui_theme::add_color(std::string name,uint8_t r, uint8_t g, uint8_t b)
    {
        gpe::color * fColor =  new gpe::color( name, r, g, b);
        theme_colors.push_back(fColor);
        return fColor;
    }

    gpe::color * gui_theme::add_color(std::string name, gpe::color * saved_color )
    {
        gpe::color * fColor = NULL;
        if( saved_color!=NULL )
        {
            fColor = new gpe::color( name, saved_color->get_r(), saved_color->get_g(), saved_color->get_b() );
        }
        else
        {
            fColor = new gpe::color( name, 0,0,0 );
        }
        theme_colors.push_back(fColor);
        return fColor;
    }

    bool gui_theme::change_color(std::string name,uint8_t r, uint8_t g, uint8_t b)
    {
        gpe::color * fColor = find_color( name);
        if( fColor!=NULL )
        {
            fColor->change_rgba(r,g,b);
            return true;
        }
        return false;
    }



    gpe::color * gui_theme::find_color(std::string name)
    {
        if( (int)name.size() > 0)
        {
            for(int i = (int)theme_colors.size()-1; i >=0; i-- )
            {
                if( theme_colors[i]->get_name()==name )
                {
                    return theme_colors[i];
                }
            }
        }
        return NULL;
    }

    gpe::color * gui_theme::get_color(int pos)
    {
        if( pos >=0 && pos < (int)theme_colors.size() )
        {
            return theme_colors[pos];
        }
        return NULL;
    }

    int gui_theme::get_color_count()
    {
        return (int)theme_colors.size();
    }

    bool gui_theme::is_custom_theme()
    {
        return non_default_theme;
    }

    bool gui_theme::load_background( std::string bgTextureLocation)
    {
        if( (int)bgTextureLocation.size() > 0 )
        {
            themeBgFileLocation = stg_ex::get_local_from_global_file( bgTextureLocation );
            theme_texture_bg = rsm_gui->texture_add_filename( bgTextureLocation );
            if( theme_texture_bg == NULL)
            {
                return false;
            }
            if( theme_texture_bg->get_width() > 0)
            {
                theme_texture_bg->set_blend_mode( gpe::blend_mode_blend );
                return true;
            }
        }
        return false;
    }

    bool gui_theme::load_theme(std::string themeLocationIn)
    {
        //If the level file could be loaded
        std::string themeGlobalLocation = "";
        if( sff_ex::file_exists(themeLocationIn) )
        {
            themeGlobalLocation = themeLocationIn;
        }
        else
        {
            themeGlobalLocation  = gpe::app_directory_name+"themes/"+ stg_ex::get_local_from_global_file( themeLocationIn );
        }

        std::ifstream templateFileIn( themeGlobalLocation.c_str() );
        if( !templateFileIn.fail() )
        {
            //makes sure the file is open
            if (templateFileIn.is_open())
            {
                //gpe::error_log->report("Loading Theme ["+themeGlobalLocation+"]...");
                theme_local_location = themeLocationIn;
                int equalPos = 0;
                int hashPos = 0;
                std::string firstChar="";
                std::string section="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                //float foundFileVersion = 0;
                gpe::color * foundColor = NULL;
                int rFound = 0, gFound = 0, bFound = 0;
                while ( templateFileIn.good() )
                {
                    getline (templateFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                    currLineToBeProcessed = stg_ex::trim_right_inplace(currLine);
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
                                    if( non_default_theme)
                                    {
                                        load_background( gpe::app_directory_name+"themes/custom/"+valString);
                                    }
                                    else
                                    {
                                        load_background( gpe::app_directory_name+"themes/"+valString);
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
                                            gpe::color_system->hex_to_rgb(valString,rFound,gFound, bFound);
                                        }
                                        else
                                        {
                                            rFound = stg_ex::split_first_int(valString,',');
                                            gFound = stg_ex::split_first_int(valString,',');
                                            bFound = stg_ex::string_to_int(valString);
                                        }
                                        foundColor->change_rgba( rFound, gFound, bFound );
                                        //gpe::error_log->report( "Found Color ["+keyString+"] with values: ["+subValString+"]..." );
                                    }
                                }
                            }
                        }
                    }
                }
                templateFileIn.close();
                return true;
            }
            //gpe::error_log->report( "Theme loaded..." );
        }
        else
        {
            gpe::error_log->report("Unable to open file ["+themeGlobalLocation+"]"+" from ["+themeLocationIn+"] file" );
        }
        return false;
    }

    bool gui_theme::render_background( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
    {
        viewedSpace = gpe::camera_find(viewedSpace);
        cam = gpe::camera_find(cam);
        if(cam!=NULL && viewedSpace!=NULL )
        {
            gpe::gcanvas->render_rectangle( cam->x, cam->y,cam->w,cam->h,pawgui::theme_main->program_color,false);
            if( theme_texture_bg!=NULL )
            {
                theme_texture_bg->render_tex_resized( cam->x, cam->y, cam->w,cam->h );
                return true;
            }
        }
        return false;
    }

    bool gui_theme::save_theme()
    {
        save_theme_as( theme_local_location );
    }

    bool gui_theme::save_theme_as(std::string themeLocationOut)
    {
        //If the level file could be loaded
        std::string themeGlobalLocation = "";
        if( (int)themeLocationOut.size() ==0)
        {
            themeLocationOut = theme_name+".gpf";
        }
        if( sff_ex::file_exists(themeLocationOut) )
        {
            themeGlobalLocation = themeLocationOut;
        }
        else if( non_default_theme)
        {
            themeGlobalLocation  = gpe::app_directory_name+"themes/custom/"+ stg_ex::get_local_from_global_file( themeLocationOut );
        }
        else
        {
            themeGlobalLocation  = gpe::app_directory_name+"themes/"+ stg_ex::get_local_from_global_file( themeLocationOut );
        }
        if( !sff_ex::file_exists(themeGlobalLocation) )
        {
            gpe::error_log->report("Theme File ["+themeGlobalLocation+"] not found...");
            return false;
        }
        std::ofstream templateFileOut( themeGlobalLocation.c_str() );
        if( !templateFileOut.fail() )
        {
            //makes sure the file is open
            if (templateFileOut.is_open())
            {
                templateFileOut << "Background="+stg_ex::get_local_from_global_file( themeBgFileLocation )+"\n";
                gpe::color * tempColor = NULL;
                for(int i = 0; i < (int)theme_colors.size(); i++ )
                {
                    tempColor = theme_colors[i];
                    if( tempColor!=NULL )
                    {
                        templateFileOut << tempColor->get_name()+"="+ stg_ex::int_to_string( tempColor->get_r() )+","+ stg_ex::int_to_string( tempColor->get_g() )+","+ stg_ex::int_to_string( tempColor->get_b() ) << "\n";
                    }
                }
            }
            theme_local_location = themeGlobalLocation;
        }
        templateFileOut.close();
    }

    bool load_default_fonts()
    {
        std::string mainGuiFontLocation = gpe::app_directory_name+"resources/fonts/dejavu_sans_mono/DejaVuSansMono.ttf";
        std::string textEditorFontLocation = gpe::app_directory_name+"resources/fonts/dejavu_sans_mono/DejaVuSansMono.ttf";
        bool fontIsMonoSpaced = true;

        if( sff_ex::file_exists("C:/Windows/Fonts/Arial.ttf") )
        {
            mainGuiFontLocation = "C:/Windows/Fonts/Arial.ttf";
            fontIsMonoSpaced = false;
        }
        else if( sff_ex::file_exists("C:/Windows/Fonts/Carlito.ttf") )
        {
            mainGuiFontLocation = "C:/Windows/Fonts/Carlito.ttf";
            fontIsMonoSpaced = false;
        }
        else if( sff_ex::file_exists("C:/Windows/Fonts/Candara.ttf") )
        {
            mainGuiFontLocation = "C:/Windows/Fonts/Candara.ttf";
            fontIsMonoSpaced = false;
        }
        gpe::error_log->report("Using "+mainGuiFontLocation+" font..." );
        //Open the fonts
        FONT_BUTTONS_FONT = gpe::gfs->open_font(textEditorFontLocation,12,true, "Buttons Font");

        gpe::font_default = gpe::gfs->open_font(mainGuiFontLocation,14,fontIsMonoSpaced, "gpe::font_default");
        GUI_TAB_FONT = gpe::gfs->open_font(mainGuiFontLocation,11,fontIsMonoSpaced, "GPE_TAB_FONT");

        FONT_POPUP = gpe::gfs->open_font(mainGuiFontLocation,14,fontIsMonoSpaced, "FONT_POPUP");

        FONT_TOOLBAR = gpe::gfs->open_font(mainGuiFontLocation,14,fontIsMonoSpaced, "FONT_TOOLBAR");
        FONT_RESOURCEBAR = gpe::gfs->open_font(mainGuiFontLocation,12,fontIsMonoSpaced, "FONT_RESOURCEBAR");
        FONT_STREE_BRANCH = gpe::gfs->open_font(mainGuiFontLocation,12,fontIsMonoSpaced, "FONT_STREE");

        FONT_TEXTINPUT = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTinput_GENERAL");
        FONT_TEXTinput_COMMENT = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTinput_COMMENT");
        FONT_TEXTinput_KEYWORD = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTinput_KEYWORD");
        FONT_TEXTinput_FUNCTION = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTinput_FUNCTION");
        FONT_TEXTinput_NUMBER = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTinput_NUMBER");
        FONT_TEXTinput_QUOTE = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTinput_QUOTE");
        FONT_TEXTinput_SYMBOL = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTinput_SYMBOL");
        FONT_TEXTinput_project_FUNCTION = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTinput_project_FUNCTION");
        FONT_TEXTinput_project_keyword = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TEXTinput_project_keyword");

        FONT_TERM_NAME = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TERM_NAME");
        FONT_TERM_DESCRIPTION = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TERM_DESCRIPTION");
        FONT_TERM_SCOPE = gpe::gfs->open_font(textEditorFontLocation,11,true, "FONT_TERM_SCOPE");
        FONT_TOOLTIP = gpe::gfs->open_font(textEditorFontLocation,12,true, "FONT_TOOLTIP");

        font_default_prompt = gpe::gfs->open_font(textEditorFontLocation,16,true, "font_default_prompt");

        FONT_HEADER = gpe::gfs->open_font( mainGuiFontLocation, 18,fontIsMonoSpaced, "FONT_HEADER");
        FONT_LABEL = gpe::gfs->open_font( mainGuiFontLocation, 14,fontIsMonoSpaced, "FONT_LABEL");
        FONT_LABEL_ANCHOR = gpe::gfs->open_font( mainGuiFontLocation, 14,fontIsMonoSpaced, "FONT_LABEL_ANCHOR");
        FONT_LABEL_TITLE = gpe::gfs->open_font( mainGuiFontLocation, 18,fontIsMonoSpaced, "FONT_LABEL_TITLE");
        FONT_STATUSBAR = gpe::gfs->open_font( textEditorFontLocation, 12,true, "FONT_STATUSBAR" );

        if(gpe::font_default==NULL )
        {
            gpe::error_log->report("Unable to load default font");
            return false;
        }

        if(FONT_TOOLBAR==NULL)
        {
            gpe::error_log->report("Unable to load TOOLLBAR font");
            return false;

        }

        if(FONT_TEXTINPUT==NULL)
        {
            gpe::error_log->report("Unable to load INPUT-FIELD font");
            return false;
        }
        return true;
    }

    void cleanup_fonts()
    {
        //Close the fonts that was used
        //
        if( FONT_POPUP!=NULL)
        {
            gpe::gfs->close_font( FONT_POPUP );
            FONT_POPUP = NULL;
        }

        if( FONT_TOOLBAR!=NULL)
        {
            gpe::gfs->close_font( FONT_TOOLBAR );
            FONT_TOOLBAR = NULL;
        }
        if( FONT_RESOURCEBAR!=NULL)
        {
            gpe::gfs->close_font( FONT_RESOURCEBAR );
            FONT_RESOURCEBAR = NULL;
        }
        //
        if( FONT_TEXTINPUT!=NULL)
        {
            gpe::gfs->close_font(FONT_TEXTINPUT);
            FONT_TEXTINPUT = NULL;
        }

        if( FONT_TERM_NAME!=NULL)
        {
            gpe::gfs->close_font(FONT_TERM_NAME);
            FONT_TERM_NAME = NULL;
        }

        if( FONT_TERM_DESCRIPTION!=NULL)
        {
            gpe::gfs->close_font(FONT_TERM_DESCRIPTION);
            FONT_TERM_DESCRIPTION = NULL;
        }

        if( FONT_TERM_SCOPE!=NULL)
        {
            gpe::gfs->close_font(FONT_TERM_SCOPE);
            FONT_TERM_SCOPE = NULL;
        }
        if( font_default_prompt!=NULL)
        {
            gpe::gfs->close_font(font_default_prompt);
            font_default_prompt = NULL;
        }

        //
        if( FONT_HEADER!=NULL)
        {
            gpe::gfs->close_font( FONT_HEADER );
            FONT_HEADER = NULL;
        }

        if( FONT_LABEL!=NULL)
        {
            gpe::gfs->close_font( FONT_LABEL );
            FONT_LABEL = NULL;
        }
        if( FONT_LABEL_ANCHOR!=NULL)
        {
            gpe::gfs->close_font( FONT_LABEL_ANCHOR );
            FONT_LABEL_ANCHOR = NULL;
        }

        if( FONT_LABEL_TITLE!=NULL)
        {
            gpe::gfs->close_font( FONT_LABEL_TITLE );
            FONT_LABEL_TITLE = NULL;
        }
    }
}
