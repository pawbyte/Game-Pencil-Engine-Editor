/*
pawgui_themes.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/

#include "pawgui_themes.h"

namespace pawgui
{
    //Define GUI Themes to nullptr
    gui_theme * theme_main = nullptr;
    gui_theme * theme_default = nullptr;
    gui_theme * theme_system_based = nullptr;

    //Define GUI fonts to nullptr
    gpe::font_base * font_buttons = nullptr;
    //
    gpe::font_base * font_popup = nullptr;
    gpe::font_base * font_toolbar = nullptr;
    gpe::font_base * font_resourcebar = nullptr;
    gpe::font_base * font_textinput = nullptr;
    gpe::font_base * font_textinput_comment = nullptr;
    gpe::font_base * font_textinput_keyword = nullptr;
    gpe::font_base * font_textinput_function = nullptr;
    gpe::font_base * font_textinput_number = nullptr;
    gpe::font_base * font_textinput_quote = nullptr;
    gpe::font_base * font_textinput_symbol = nullptr;
    gpe::font_base * font_textinput_project_function = nullptr;
    gpe::font_base * FONT_TEXTinput_project_keyword = nullptr;
    gpe::font_base * font_tab = nullptr;

    gpe::font_base * font_term_name = nullptr;
    gpe::font_base * font_term_description = nullptr;
    gpe::font_base * font_term_scope = nullptr;
    gpe::font_base * FONT_TOOLTIP = nullptr;

    gpe::font_base * font_default_prompt = nullptr;
    gpe::font_base * FONT_LABEL = nullptr;
    gpe::font_base * FONT_LABEL_ANCHOR = nullptr;
    gpe::font_base * FONT_STATUSBAR = nullptr;
    gpe::font_base * FONT_HEADER = nullptr;
    gpe::font_base * FONT_LABEL_TITLE = nullptr;
    gpe::font_base * FONT_STREE_BRANCH = nullptr;


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
        theme_bg_location = "";
        theme_texture_bg = nullptr;
        //animations
        main_menu_animation = nullptr;

        //for bg and standard colors

        program_color = add_color("MainBackground",32,32,32);
        program_color_header = add_color("HeaderBackground",62,62,62);

        panel_color = add_color("Panel",20,21,22);
        panel_border_color = add_color("PanelBorder",43,44,47);

        tab_background_color = add_color("TabBackground",20,21,22);
        tab_color = add_color("Tab",20,21,22);
        tab_hover_color = add_color("TabHover",30,31,32);
        tab_selected_color = add_color("TabSelected",43,44,47);
        tab_text_color = add_color("TabText",220,224, 227);


        //_button colors and such
        button_box_color = add_color("_buttonBox",25,25,25);
        button_font_color = add_color("_buttonFont",249,249,249);
        button_font_highlight_color = add_color("_buttonFontHighlighted",240,240,240);
        button_border_color = add_color("_buttonBorder",16,16,16);
        button_box_highlight_color = add_color("_buttonBoxHighlighted",40,40,40);
        button_box_selected_color = add_color("_buttonBoxHHighlightedAlt",96,96,96);
        button_box_shadow_color = add_color("_buttonBoxShadow",gpe::c_ltgray );
        button_border_color = add_color( "_buttonBorder",gpe::c_dkgray );
        button_border_highlight_color = add_color( "_buttonBorderHighlighted", gpe::c_blgray );
        button_border_selected_color = add_color( "_buttonBorderselected", gpe::c_ltgray );

        //Used mainly for labels, urls and such
        checkbox_color = add_color("Checkbox", 210,180,40 );

        //For icons and Folders
        folder_color = add_color("FolderColor", 192,192,192 );
        folder_color_highlight = add_color("FolderHighlightedColor", 255, 99,71);

        //Icon_button colors and such
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
        gpe::color * color_temp = nullptr;
        for(int i = (int)theme_colors.size()-1; i >=0; i-- )
        {
            color_temp = theme_colors[i];
            if( color_temp!=nullptr )
            {
                delete color_temp;
                color_temp = nullptr;
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
        gpe::color * fColor = nullptr;
        if( saved_color!=nullptr )
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
        if( fColor!=nullptr )
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
        return nullptr;
    }

    gpe::color * gui_theme::get_color(int pos)
    {
        if( pos >=0 && pos < (int)theme_colors.size() )
        {
            return theme_colors[pos];
        }
        return nullptr;
    }

    int gui_theme::get_color_count()
    {
        return (int)theme_colors.size();
    }

    bool gui_theme::is_custom_theme()
    {
        return non_default_theme;
    }

    bool gui_theme::load_background( std::string bg_textureLocation)
    {
        if( (int)bg_textureLocation.size() == 0 )
        {
            theme_bg_location = "";
            theme_texture_bg = nullptr;
            return false;
        }
        theme_bg_location = stg_ex::get_local_from_global_file( bg_textureLocation );
        theme_texture_bg = rsm_gui->texture_add_filename( bg_textureLocation );
        if( theme_texture_bg == nullptr)
        {
            return false;
        }
        if( theme_texture_bg->get_width() > 0)
        {
            theme_texture_bg->set_blend_mode( gpe::blend_mode_blend );
            return true;
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
                std::string key_string="";
                std::string valstring="";
                std::string subValstring="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                //float foundFileVersion = 0;
                gpe::color * foundColor = nullptr;
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
                                key_string = currLineToBeProcessed.substr(0,equalPos);
                                valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                subValstring = valstring;
                                if( key_string=="Background")
                                {
                                    if( non_default_theme)
                                    {
                                        load_background( gpe::app_directory_name+"themes/custom/"+valstring);
                                    }
                                    else
                                    {
                                        load_background( gpe::app_directory_name+"themes/"+valstring);
                                    }
                                }
                                else
                                {
                                    foundColor = find_color( key_string );
                                    if( foundColor !=nullptr )
                                    {
                                        hashPos = valstring.find_first_of("#");
                                        if(hashPos!=(int)std::string::npos)
                                        {
                                            gpe::color_system->hex_to_rgb(valstring,rFound,gFound, bFound);
                                        }
                                        else
                                        {
                                            rFound = stg_ex::split_first_int(valstring,',');
                                            gFound = stg_ex::split_first_int(valstring,',');
                                            bFound = stg_ex::string_to_int(valstring);
                                        }
                                        foundColor->change_rgba( rFound, gFound, bFound );
                                        //gpe::error_log->report( "Found Color ["+key_string+"] with values: ["+subValstring+"]..." );
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

    bool gui_theme::render_background( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if(cam!=nullptr && view_space!=nullptr )
        {
            gpe::gcanvas->render_rectangle( cam->x, cam->y,cam->w,cam->h,pawgui::theme_main->program_color,false);
            if( theme_texture_bg!=nullptr )
            {
                theme_texture_bg->render_tex_resized( cam->x, cam->y, cam->w,cam->h );
                return true;
            }
        }
        return false;
    }

    bool gui_theme::save_theme()
    {
        return save_theme_as( theme_local_location );
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
                templateFileOut << "Background="+stg_ex::get_local_from_global_file( theme_bg_location )+"\n";
                gpe::color * color_temp = nullptr;
                for(int i = 0; i < (int)theme_colors.size(); i++ )
                {
                    color_temp = theme_colors[i];
                    if( color_temp!=nullptr )
                    {
                        templateFileOut << color_temp->get_name()+"="+ stg_ex::int_to_string( color_temp->get_r() )+","+ stg_ex::int_to_string( color_temp->get_g() )+","+ stg_ex::int_to_string( color_temp->get_b() ) << "\n";
                    }
                }
                templateFileOut.close();
                theme_local_location = themeGlobalLocation;
                return true;
            }
        }
        templateFileOut.close();
        return false;
    }

    bool load_default_fonts( std::string mono_font_location, int font_min_size )
    {
        if( font_min_size < 0 )
        {
            gpe::error_log->report("BEWARE load_default_fonts("+ mono_font_location + ","+ stg_ex::int_to_string( font_min_size) + ") function called!" );
        }
        std::string mainGuiFontLocation = gpe::app_directory_name+"resources/fonts/dejavu_sans_mono/DejaVuSansMono.ttf";
        std::string textEditorFontLocation = gpe::app_directory_name+"resources/fonts/dejavu_sans_mono/DejaVuSansMono.ttf";
        bool font_is_monospaced = true;

        if( sff_ex::file_exists( mainGuiFontLocation) )
        {
            font_is_monospaced = false;
        }
        else
        {
            gpe::error_log->report("FATAL ERROR, unable to find ["+ mainGuiFontLocation +"]. Program likely to crash or suffer performance issues!" );
            return false;
        }
        gpe::error_log->report("Using "+mainGuiFontLocation+" font..." );
        //Open the fonts
        font_buttons = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "_buttons Font");

        gpe::font_default = gpe::gfs->open_font(mainGuiFontLocation,font_min_size,font_is_monospaced, "gpe::font_default");
        font_tab = gpe::gfs->open_font(mainGuiFontLocation,font_min_size,font_is_monospaced, "GPE_TAB_FONT");

        font_popup = gpe::gfs->open_font(mainGuiFontLocation,font_min_size,font_is_monospaced, "font_popup");

        font_toolbar = gpe::gfs->open_font(mainGuiFontLocation,font_min_size,font_is_monospaced, "font_toolbar");
        font_resourcebar = gpe::gfs->open_font(mainGuiFontLocation,font_min_size,font_is_monospaced, "font_resourcebar");
        FONT_STREE_BRANCH = gpe::gfs->open_font(mainGuiFontLocation,font_min_size,font_is_monospaced, "FONT_STREE");

        font_textinput = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "FONT_TEXTinput_GENERAL");
        font_textinput_comment = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_textinput_comment");
        font_textinput_keyword = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_textinput_keyword");
        font_textinput_function = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_textinput_function");
        font_textinput_number = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_textinput_number");
        font_textinput_quote = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_textinput_quote");
        font_textinput_symbol = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_textinput_symbol");
        font_textinput_project_function = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_textinput_project_function");
        FONT_TEXTinput_project_keyword = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "FONT_TEXTinput_project_keyword");

        font_term_name = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_term_name");
        font_term_description = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_term_description");
        font_term_scope = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_term_scope");
        FONT_TOOLTIP = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "FONT_TOOLTIP");

        font_default_prompt = gpe::gfs->open_font(textEditorFontLocation,font_min_size,true, "font_default_prompt");

        FONT_HEADER = gpe::gfs->open_font( mainGuiFontLocation, font_min_size,font_is_monospaced, "FONT_HEADER");
        FONT_LABEL = gpe::gfs->open_font( mainGuiFontLocation, font_min_size,font_is_monospaced, "FONT_LABEL");
        FONT_LABEL_ANCHOR = gpe::gfs->open_font( mainGuiFontLocation, font_min_size,font_is_monospaced, "FONT_LABEL_ANCHOR");
        FONT_LABEL_TITLE = gpe::gfs->open_font( mainGuiFontLocation, font_min_size,font_is_monospaced, "FONT_LABEL_TITLE");
        FONT_STATUSBAR = gpe::gfs->open_font( textEditorFontLocation, font_min_size,true, "FONT_STATUSBAR" );

        if(gpe::font_default==nullptr )
        {
            gpe::error_log->report("Unable to load default font");
            return false;
        }

        if(font_toolbar==nullptr)
        {
            gpe::error_log->report("Unable to load TOOLLBAR font");
            return false;

        }

        if(font_textinput==nullptr)
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
        if( font_popup!=nullptr)
        {
            gpe::gfs->close_font( font_popup );
            font_popup = nullptr;
        }

        if( font_toolbar!=nullptr)
        {
            gpe::gfs->close_font( font_toolbar );
            font_toolbar = nullptr;
        }
        if( font_resourcebar!=nullptr)
        {
            gpe::gfs->close_font( font_resourcebar );
            font_resourcebar = nullptr;
        }
        //
        if( font_textinput!=nullptr)
        {
            gpe::gfs->close_font(font_textinput);
            font_textinput = nullptr;
        }

        if( font_term_name!=nullptr)
        {
            gpe::gfs->close_font(font_term_name);
            font_term_name = nullptr;
        }

        if( font_term_description!=nullptr)
        {
            gpe::gfs->close_font(font_term_description);
            font_term_description = nullptr;
        }

        if( font_term_scope!=nullptr)
        {
            gpe::gfs->close_font(font_term_scope);
            font_term_scope = nullptr;
        }
        if( font_default_prompt!=nullptr)
        {
            gpe::gfs->close_font(font_default_prompt);
            font_default_prompt = nullptr;
        }

        //
        if( FONT_HEADER!=nullptr)
        {
            gpe::gfs->close_font( FONT_HEADER );
            FONT_HEADER = nullptr;
        }

        if( FONT_LABEL!=nullptr)
        {
            gpe::gfs->close_font( FONT_LABEL );
            FONT_LABEL = nullptr;
        }
        if( FONT_LABEL_ANCHOR!=nullptr)
        {
            gpe::gfs->close_font( FONT_LABEL_ANCHOR );
            FONT_LABEL_ANCHOR = nullptr;
        }

        if( FONT_LABEL_TITLE!=nullptr)
        {
            gpe::gfs->close_font( FONT_LABEL_TITLE );
            FONT_LABEL_TITLE = nullptr;
        }
    }
}
