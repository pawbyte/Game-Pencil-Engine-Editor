/*
pawgui_color_picker.cpp
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

#include "../text_input/pawgui_text_input.h"
#include "../buttons/pawgui_buttons.h"
#include "../color_picker/pawgui_color_picker.h"
#include "../labels/pawgui_labels.h"

namespace pawgui
{
    gpe::texture_base * texture_color_picker_gradient = nullptr;

    gpe_widget_color_picker::gpe_widget_color_picker( std::string name,std::string description, int r, int g, int b )
    {
        widget_name = name;
        set_label( name );
        descriptionText = description;
        widget_type = "colorpicker";
        widget_type_id = 2;
        storedColor = new gpe::color("customColor", r,g,b );
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 196;
        widget_box.h = 32;
        fieldElementBox.x = widget_box.x+128;
        fieldElementBox.y = widget_box.y;
        fieldElementBox.w = widget_box.w-128;
        fieldElementBox.h = widget_box.h;

        font_sizeH = 12;
        font_sizeW = 12;
        font_textinput->get_metrics("A",&font_sizeW,&font_sizeH);
        if( font_sizeW<=0)
        {
            font_sizeW = 12;
        }
        if( font_sizeH<=0)
        {
            font_sizeH = 12;
        }
    }

    gpe_widget_color_picker::~gpe_widget_color_picker()
    {
        if( storedColor!=nullptr)
        {
            delete storedColor;
            storedColor = nullptr;
        }
    }

    std::string gpe_widget_color_picker::get_data()
    {
        return widget_type+":"+widget_name+"==|||=="+get_rgb_string()+",,,";
    }

    void gpe_widget_color_picker::load_data(std::string datastring)
    {
        std::string rgbColor = stg_ex::split_first_string(datastring,",,,");
        set_color_from_rgb(rgbColor);
    }

    gpe::color * gpe_widget_color_picker::get_color()
    {
        if( storedColor!=nullptr)
        {
            return storedColor;
        }
        return nullptr;
    }

    gpe::color * gpe_widget_color_picker::get_duplicate_color()
    {
        if( storedColor!=nullptr)
        {
            return storedColor->duplicate_color();
        }
        return nullptr;
    }

    std::string gpe_widget_color_picker::get_hex_string()
    {
        if( storedColor!=nullptr)
        {
            return gpe::color_system->rgb_to_hex(storedColor->get_r(),storedColor->get_g(),storedColor->get_b() );
        }
        return "000";
    }

    std::string gpe_widget_color_picker::get_rgb_string()
    {
        if( storedColor!=nullptr)
        {
            return stg_ex::int_to_string(storedColor->get_r() )+","+stg_ex::int_to_string(storedColor->get_g() )+","+ stg_ex::int_to_string(storedColor->get_b() );
        }
        return "0,0,0";
    }

    std::string gpe_widget_color_picker::get_plain_string()
    {
        return "\""+get_hex_string()+"\"";

    }

    int gpe_widget_color_picker::get_r()
    {
        if( storedColor!=nullptr)
        {
            return storedColor->get_r();
        }
        return 0;
    }

    int gpe_widget_color_picker::get_g()
    {
        if( storedColor!=nullptr)
        {
            return storedColor->get_g();
        }
        return 0;
    }

    int gpe_widget_color_picker::get_b()
    {
        if( storedColor!=nullptr)
        {
            return storedColor->get_b();
        }
        return 0;
    }

    void gpe_widget_color_picker::set_label(std::string newLabel)
    {
        if( (int)newLabel.size() > 0)
        {
            inputLabel = newLabel;
            showLabel = true;
        }
        else
        {
            inputLabel = "";
            showLabel = false;
        }
    }

    void gpe_widget_color_picker::set_name(std::string new_name)
    {
        if( (int)new_name.size() > 0)
        {
            if( widget_name == inputLabel )
            {
                set_label(new_name);
            }
            widget_name = new_name;
        }
    }

    void gpe_widget_color_picker::set_string(std::string newstring)
    {

    }

    void gpe_widget_color_picker::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        if( widget_box.w >= 256 )
        {
            fieldElementBox.x = widget_box.x+widget_box.w-128;
            fieldElementBox.y = widget_box.y;
            fieldElementBox.w = 128;
            fieldElementBox.h = widget_box.h;
        }
        else if( widget_box.w >= 64 )
        {
            fieldElementBox.x = widget_box.x+widget_box.w-32;
            fieldElementBox.y = widget_box.y;
            fieldElementBox.w = 32;
            fieldElementBox.h = widget_box.h;
        }
        else
        {
            fieldElementBox.x = widget_box.x+widget_box.w/2;
            fieldElementBox.y = widget_box.y;
            fieldElementBox.w = widget_box.w/2;
            fieldElementBox.h = widget_box.h;
        }
        if( showLabel)
        {
            set_label(inputLabel);
        }
        /*
        Old Protocol for Label Fields
        widget_basic::process_self(view_space,cam);

        New Protocol for labeled fields

        */
        //
        if( isEnabled)
        {
            isClicked = false;
            isHovered = false;
            clickedOutside = false;
            view_space = gpe::camera_find(view_space);
            cam = gpe::camera_find(cam);
            if(view_space!=nullptr && cam!=nullptr)
            {
                if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,view_space->x,view_space->y,view_space->x+view_space->w,view_space->y+view_space->h) )
                {
                    if ( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,fieldElementBox.x+view_space->x-cam->x,fieldElementBox.y+view_space->y-cam->y,fieldElementBox.x+fieldElementBox.w+view_space->x-cam->x,fieldElementBox.y+fieldElementBox.h+view_space->y-cam->y) )
                    {
                        isHovered = true;
                        if( (int)descriptionText.size()>0 )
                        {
                            main_overlay_system->update_tooltip(descriptionText);
                        }
                        else
                        {
                            main_overlay_system->update_tooltip(widget_name);
                        }
                        if( gpe::input->check_mouse_released( mb_left))
                        {
                            isClicked = true;
                        }
                    }
                    else
                    {
                        if( showLabel )
                        {
                            if (gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x+view_space->x-cam->x,fieldElementBox.y+view_space->y-cam->y,fieldElementBox.x+view_space->x-cam->x,fieldElementBox.y+widget_box.h+view_space->y-cam->y) )
                            {
                                if( (int)inputLabel.size() > 0 )
                                {
                                    main_overlay_system->update_tooltip( inputLabel);
                                }
                                else if( (int)descriptionText.size()>0 )
                                {
                                    main_overlay_system->update_tooltip(descriptionText);
                                }
                                else
                                {
                                    main_overlay_system->update_tooltip(widget_name);
                                }
                            }
                        }
                        if( gpe::input->check_mouse_released( mb_anybutton ) )
                        {
                            clickedOutside = true;
                        }
                    }
                }
                else if( gpe::input->check_mouse_released( mb_anybutton ) )
                {
                    clickedOutside = true;
                }
            }
        }

        //
        if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked)
        {
            std::string popUpstring = "";
            if( (int)inputLabel.size() > 0)
            {
                popUpstring = "Change "+inputLabel;
            }
            else
            {
                popUpstring = "Select a Color";
            }
            get_color_from_popup(popUpstring,storedColor);
        }
    }

    void gpe_widget_color_picker::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( isEnabled && cam!=nullptr)
        {
            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y, pawgui::theme_main->main_box_highlight_color, false );
                if( showLabel && (int)inputLabel.size() > 0)
                {
                    gpe::gfs->render_text_clipped( widget_box.x-cam->x+padding_default,widget_box.y+widget_box.h/2-cam->y,inputLabel,pawgui::theme_main->main_box_font_highlight_color,font_textinput,gpe::fa_left,gpe::fa_middle );
                }
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y, pawgui::theme_main->main_box_color, false );

                if( showLabel && (int)inputLabel.size() > 0)
                {
                    gpe::gfs->render_text_clipped( widget_box.x-cam->x+padding_default,widget_box.y+widget_box.h/2-cam->y,inputLabel,pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_left,gpe::fa_middle );
                }
            }
            gpe::gcanvas->render_rectangle( fieldElementBox.x-cam->x,fieldElementBox.y-cam->y,fieldElementBox.x+fieldElementBox.w-cam->x,fieldElementBox.y+fieldElementBox.h-cam->y,storedColor,false);
            //gpe::gcanvas->render_vertical_line_color( widget_box.x-cam->x,widget_box.y-cam->y, widget_box.y+widget_box.h-cam->y, pawgui::theme_main->main_border_color );
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y, pawgui::theme_main->main_border_color, true );
        }
    }

    void gpe_widget_color_picker::set_color_from_rgb( gpe::color *color_new)
    {
        if( storedColor==nullptr)
        {
            storedColor = new gpe::color("custom",0,0,0);
        }
        if( storedColor!=nullptr)
        {
            if( color_new!=nullptr)
            {
                storedColor->change_rgba(color_new->get_r(),color_new->get_g(),color_new->get_b() );
            }
        }
    }

    void gpe_widget_color_picker::set_color_from_rgb(std::string color_newStr)
    {
        int r = stg_ex::split_first_int(color_newStr,',');
        int g = stg_ex::split_first_int(color_newStr,',');

        //For the last variable we automate if a commma was used to end the string
        int b = 0;
        if( stg_ex::string_contains( color_newStr,",") )
        {
            b = stg_ex::split_first_int(color_newStr,',');
        }
        else
        {
            b = stg_ex::string_to_int(color_newStr,0);
        }

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
        if( storedColor==nullptr)
        {
            storedColor = new gpe::color( "custom",0,0,0 );
        }
        if( storedColor!=nullptr)
        {
            storedColor->change_rgba(r,g,b);
        }
    }

    void gpe_widget_color_picker::set_color_from_hex(std::string color_newStr)
    {
        int r = 0, g = 0, b = 0;
        gpe::color_system->hex_to_rgb( color_newStr,r, g, b );
        if( storedColor==nullptr)
        {
            storedColor = new gpe::color( "custom",0,0,0 );
        }
        if( storedColor!=nullptr)
        {
            storedColor->change_rgba(r,g,b);
        }
    }

    void gpe_widget_color_picker::set_r(int r)
    {
        if( r < 0 || r > 255)
        {
            r = 0;
        }
        if( storedColor!=nullptr)
        {
            storedColor->change_r(r);
        }
    }

    void gpe_widget_color_picker::set_g(int g)
    {
        if( g < 0 || g > 255)
        {
            g = 0;
        }
        if( storedColor!=nullptr)
        {
            storedColor->change_g(g);
        }
    }

    void gpe_widget_color_picker::set_b(int b)
    {
        if( b < 0 || b > 255)
        {
            b = 0;
        }
        if( storedColor!=nullptr)
        {
            storedColor->change_b(b);
        }
    }

    void gpe_widget_color_picker::set_rgb(int r, int g, int b)
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
        if( storedColor!=nullptr)
        {
            storedColor->change_rgba(r,g,b,255);
        }
    }

    bool get_color_from_popup(std::string popUpCaption, gpe::color * currColor)
    {
        if( currColor == nullptr)
        {
            return false;
        }
        gpe::game_runtime->end_loop();
        //resource_dragged = nullptr;
        if( (int)popUpCaption.size() ==0)
        {
            popUpCaption = "Select a Color";
        }
        main_overlay_system->take_frozen_screenshot( );

        int promptBoxWidth = 500;
        int promptBoxHeight = 400;
        gpe::shape_rect widget_box;
        gpe::shape_rect colorShadeBox;
        colorShadeBox.w = 32;
        colorShadeBox.h = 200;

        bool exitOperation = false;
        bool operationCancelled = false;
        gpe::input->reset_all_input();
        gpe::color * alteredColor = nullptr, *colorShadeTempColor = nullptr;
        colorShadeTempColor = new gpe::color( "custom",0,0,0 );
        int rV = 0,  gV = 0,  bV = 0;
        uint8_t rU8 = 0, gU8 = 0, bU8 = 0, uU8 = 0;
        uint64_t foundPixelColor = 0;
        int foundColorPickerX = 0, foundColorPickerY = 0;
        int colorPickerShaderI = 0;
        //float colorShadeDifference = 0;
        float colorShadeDivision = 0;
        int selectedColorShade = colorShadeBox.h/2;
        widget_input_number * color_newRValue = new widget_input_number("",true,0,255);
        color_newRValue->set_label("Red");
        widget_input_number * color_newGValue = new widget_input_number("",true,0,255);
        color_newGValue->set_label("Green");
        widget_input_number * color_newBValue = new widget_input_number("",true,0,255);
        color_newBValue->set_label("Blue");

        widget_input_number * color_newheight_valueue = new widget_input_number("",true,0,240);
        color_newheight_valueue->set_label("Hue");
        widget_input_number * color_newSValue = new widget_input_number("",true,0,240);
        color_newSValue->set_label("Sat");
        widget_input_number * color_newLValue = new widget_input_number("",true,0,240);
        color_newLValue->set_label("Lum");


        widget_input_text * color_newHexValue = new widget_input_text("#FFFFFF");
        color_newHexValue->set_label("Hex: #");

        alteredColor = new gpe::color("CurrentColor",currColor->get_r(),currColor->get_g(),currColor->get_b() );
        color_newRValue->set_string( stg_ex::int_to_string( currColor->get_r() ) );
        color_newGValue->set_string( stg_ex::int_to_string( currColor->get_g() ) );
        color_newBValue->set_string( stg_ex::int_to_string( currColor->get_b() ) );
        color_newHexValue->set_string( gpe::color_system->rgb_to_hex( currColor->get_r(),currColor->get_g(),currColor->get_b() ) ); //Convert RGB to Hex here

        widget_button_label * yes_button = new widget_button_label( "Okay","Confirm Color Change");
        widget_button_label * cancel_button = new widget_button_label( "Cancel","Cancel Color Request");

        while(exitOperation==false)
        {
            gpe::game_runtime->start_loop();

            widget_box.x = (gpe::screen_width-promptBoxWidth)/2;
            widget_box.y = (gpe::screen_height-promptBoxHeight)/2;
            widget_box.w = promptBoxWidth;
            widget_box.h = promptBoxHeight;

            colorShadeBox.x = widget_box.x+padding_default*2+texture_color_picker_gradient->get_width();
            colorShadeBox.y = widget_box.y+padding_default+32;

            //editor_gui_main->reset_gui_info();
            yes_button->set_coords( widget_box.x+padding_default, widget_box.y+widget_box.h-32);
            cancel_button->set_coords( yes_button->get_xpos()+yes_button->get_width()+padding_default,yes_button->get_ypos() );

            color_newRValue->set_coords(colorShadeBox.x+colorShadeBox.w+padding_default,widget_box.y+padding_default+48);
            color_newRValue->set_width(128);

            color_newGValue->set_coords(colorShadeBox.x+colorShadeBox.w+padding_default,color_newRValue->get_ypos()+color_newRValue->get_height()+padding_default);
            color_newGValue->set_width(128);

            color_newBValue->set_coords(colorShadeBox.x+colorShadeBox.w+padding_default,color_newGValue->get_ypos()+color_newGValue->get_height()+padding_default);
            color_newBValue->set_width(128);

            color_newHexValue->set_coords(colorShadeBox.x+colorShadeBox.w+padding_default,color_newBValue->get_ypos()+color_newBValue->get_height()+padding_default);
            color_newHexValue->set_width(128);

            yes_button->process_self();
            cancel_button->process_self();
            color_newRValue->process_self();
            color_newGValue->process_self();
            color_newBValue->process_self();
            color_newHexValue->process_self();
            if( color_newRValue->is_inuse() || color_newGValue->is_inuse() || color_newBValue->is_inuse() )
            {
                alteredColor->change_rgba(color_newRValue->get_held_number(),color_newGValue->get_held_number(),color_newBValue->get_held_number(),255 );
                color_newHexValue->set_string( gpe::color_system->rgb_to_hex(alteredColor->get_r(),alteredColor->get_g(),alteredColor->get_b() ) ); //Convert RGB to Hex here
            }
            else if( color_newHexValue->is_inuse() )
            {
                //convert hex to rgb is possibre
                gpe::color_system->hex_to_rgb(color_newHexValue->get_string(),rV,gV,bV);
                color_newRValue->set_number(rV);
                color_newGValue->set_number(gV);
                color_newBValue->set_number(bV);
                alteredColor->change_rgba(color_newRValue->get_held_number(),color_newGValue->get_held_number(),color_newBValue->get_held_number(),255 );
                //color_newHexValue->set_string( RGBtoHEX(currColor->get_r(),currColor->get_g(),currColor->get_b() ) ); //Convert RGB to Hex here
            }
            if( gpe::input->check_kb_released(kb_esc) || cancel_button->is_clicked() || gpe::window_controller_main->is_resized() )
            {
                exitOperation = true;
                operationCancelled = true;
            }
            else if( gpe::input->check_kb_released(kb_enter) || yes_button->is_clicked() )
            {
                exitOperation = true;
            }
            else if( gpe::input->check_mouse_released( mb_left))
            {
                if( gpe::point_within_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&colorShadeBox) )
                {
                    selectedColorShade = gpe::input->mouse_position_y - colorShadeBox.y;

                    colorShadeDivision = (float)selectedColorShade/(float)colorShadeBox.h;

                    color_newRValue->set_number( gpe::color_system->merge_channel(alteredColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    color_newGValue->set_number( gpe::color_system->merge_channel(alteredColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    color_newBValue->set_number( gpe::color_system->merge_channel(alteredColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                    alteredColor->change_rgba(color_newRValue->get_held_number(),color_newGValue->get_held_number(),color_newBValue->get_held_number(),255 );
                    color_newHexValue->set_string( gpe::color_system->rgb_to_hex(alteredColor->get_r(),alteredColor->get_g(),alteredColor->get_b() ) ); //Convert RGB to Hex here

                }
            }
            gpe::renderer_main->reset_viewpoint();
            if( !gpe::window_controller_main->is_resized() )
            {
                main_overlay_system->render_frozen_screenshot( );

                //Update screen
                gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_color,false);
                gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_border_color,true);
                if( texture_color_picker_gradient!=nullptr)
                {
                    texture_color_picker_gradient->render_tex( widget_box.x+padding_default,widget_box.y+32+padding_default );
                }
                int preiewColorYPos = yes_button->get_ypos()-padding_default-64;

                gpe::gcanvas->render_rect( &colorShadeBox,pawgui::theme_main->program_color,false);
                gpe::gcanvas->render_rect( &colorShadeBox,pawgui::theme_main->main_border_color,true);

                for( colorPickerShaderI = 0; colorPickerShaderI <= colorShadeBox.h; colorPickerShaderI++)
                {
                    colorShadeDivision = (float)colorPickerShaderI/(float)colorShadeBox.h;
                    colorShadeTempColor->change_r( gpe::color_system->merge_channel(alteredColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_g( gpe::color_system->merge_channel(alteredColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_b( gpe::color_system->merge_channel(alteredColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                    gpe::gcanvas->render_horizontal_line_color( colorShadeBox.y+colorPickerShaderI,colorShadeBox.x,colorShadeBox.x+colorShadeBox.w,colorShadeTempColor);
                }
                gpe::gcanvas->render_horizontal_line_color( colorShadeBox.y+selectedColorShade,colorShadeBox.x-4,colorShadeBox.x+colorShadeBox.w+4,pawgui::theme_main->main_border_color );


                if( currColor!=nullptr)
                {
                    preiewColorYPos = yes_button->get_ypos()-padding_default-40;
                    gpe::gfs->render_text( widget_box.x+padding_default,preiewColorYPos,"Older Color:",pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top);
                    gpe::gcanvas->render_rectangle( widget_box.x+128,preiewColorYPos,widget_box.x+256,preiewColorYPos+32,currColor,false);

                    preiewColorYPos = yes_button->get_ypos()-padding_default-80;
                    gpe::gfs->render_text( widget_box.x+padding_default,preiewColorYPos,"New Color:",pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top);
                    gpe::gcanvas->render_rectangle( widget_box.x+128,preiewColorYPos,widget_box.x+256,preiewColorYPos+32,alteredColor,false);
                }
                else
                {
                    preiewColorYPos = yes_button->get_ypos()-padding_default-40;
                    gpe::gfs->render_text( widget_box.x+padding_default,preiewColorYPos,"New Color:",pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top);
                    gpe::gcanvas->render_rectangle( widget_box.x+128,preiewColorYPos,widget_box.x+256,preiewColorYPos+32,alteredColor,false);
                }
                gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,pawgui::theme_main->popup_box_highlight_color,false);
                gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_border_color,true);
                gpe::gfs->render_text( widget_box.x+widget_box.w/2,widget_box.y+padding_default,popUpCaption,pawgui::theme_main->popup_box_highlight_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top);
                yes_button->render_self( );
                cancel_button->render_self( );
                color_newRValue->render_self( );
                color_newGValue->render_self( );
                color_newBValue->render_self( );
                color_newHexValue->render_self( );
            }
            gpe::game_runtime->end_loop();
        }

        if( yes_button!=nullptr)
        {
            delete yes_button;
            yes_button = nullptr;
        }
        if( cancel_button!=nullptr)
        {
            delete cancel_button;
            cancel_button = nullptr;
        }
        if( color_newRValue!=nullptr)
        {
            delete color_newRValue;
            color_newRValue = nullptr;
        }
        if( color_newGValue!=nullptr)
        {
            delete color_newGValue;
            color_newGValue = nullptr;
        }
        if( color_newBValue!=nullptr)
        {
            delete color_newBValue;
            color_newBValue = nullptr;
        }
        if( color_newHexValue!=nullptr)
        {
            delete color_newHexValue;
            color_newHexValue = nullptr;
        }
        if( colorShadeTempColor!=nullptr)
        {
            delete colorShadeTempColor;
            colorShadeTempColor = nullptr;
        }

        if( !operationCancelled)
        {
            if( alteredColor!=nullptr)
            {
                currColor->change_rgba( alteredColor->get_r(),alteredColor->get_g(),alteredColor->get_b() , 255 );
                delete alteredColor;
                alteredColor = nullptr;
                gpe::input->reset_all_input();
                gpe::game_runtime->start_loop();
                return true;
            }
        }

        if( alteredColor!=nullptr)
        {
            delete alteredColor;
            alteredColor = nullptr;
        }
        gpe::input->reset_all_input();
        gpe::game_runtime->start_loop();
        return false;
    }
}
