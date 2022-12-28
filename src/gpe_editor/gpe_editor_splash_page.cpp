/*
gpe_editor_splash_page.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.
Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_editor_splash_page.h"


gpe_splash_page * main_gpe_splash_page = nullptr;

gpe_splash_page::gpe_splash_page()
{
    item1Max = item1Value = 0;
    item2Max = item2Value = 0;
    displayMessageTitle = "Starting...";
    displayMessageSubtitle = "";
    displayMessagestring = "";
    gpe_logo = gpe::rsm->texture_add("gpe_logo", gpe::app_directory_name+"resources/gfx/textures/logo_white.png");
    in_startup_mode = true;
}

gpe_splash_page::~gpe_splash_page()
{

}

void gpe_splash_page::increment_bar1()
{
    item1Value++;
    if( item1Value >= item1Max )
    {
        item1Value = item1Max;
    }
}
void gpe_splash_page::increment_bar2()
{
    item2Value++;
    if( item2Value >= item2Max )
    {
        item2Value = item2Max;
    }
}
void gpe_splash_page::increment_and_update( std::string subTitle, std::string message, int barNumber, bool renderUpdate )
{
    if( barNumber == 2 )
    {
        increment_bar2();
    }
    else
    {
        //defaults to bar 1
        increment_bar1();
    }
    update_submessages( subTitle, message, renderUpdate );
}


void gpe_splash_page::render_loader()
{
    //if( gpe::input->has_new_input() )
    {
        //Update screen
        gpe::game_runtime->end_loop( );
        gpe::game_runtime->start_loop();

        gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::wait) );

        gpe::renderer_main->reset_viewpoint();

        if( in_startup_mode )
        {
            gpe::gcanvas->render_rectangle( 0, 0, gpe::screen_width,gpe::screen_height,gpe::c_blgray, false);

            int x_padding = 16, y_padding = 16;
            float current_y_pos = y_padding;
            int line_height_with_padding = gpe::font_default->get_mono_height() + y_padding;
            if( gpe_logo!=nullptr && gpe_logo->get_width() > 0 )
            {
                //gpe_logo->RENDER
                float logo_width = (float) gpe_logo->get_width();
                float logo_height = (float) gpe_logo->get_height();

                float s_width = gpe::screen_width / 2.f;
                float logo_scale_amount = s_width / logo_width;
                gpe_logo->render_tex_scaled( gpe::screen_width / 4, current_y_pos, logo_scale_amount, logo_scale_amount );
                current_y_pos += logo_scale_amount * logo_height;
                gpe::gfs->render_text( gpe::screen_width*3 /4 + x_padding,current_y_pos - y_padding, "Version" + stg_ex::float_to_string( gpe::version_number_total ) ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_bottom );
            }
            else
            {
                    gpe::gfs->render_text( gpe::screen_width/2 + x_padding,current_y_pos - y_padding, "Game Penciil Engine" ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top );
                    gpe::gfs->render_text( gpe::screen_width/2 + x_padding,current_y_pos - y_padding, "Version" + stg_ex::float_to_string( gpe::version_number_total ) ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top );
                    current_y_pos += (line_height_with_padding) * 2;

            }

            current_y_pos += y_padding;

            gpe::gfs->render_text( gpe::screen_width/2,current_y_pos,"Copyright (c) 2014-2021 PawByte LLC. All rights reserved" ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_bottom );
            current_y_pos += line_height_with_padding * 2;

            gpe::gfs->render_text( gpe::screen_width/2,current_y_pos,"Game Pencil Engine is possible thanks to our sponsors" ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_bottom );
            current_y_pos += line_height_with_padding * 2;


            gpe::gfs->render_text( gpe::screen_width/2,current_y_pos,"Sponsored by" ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_bottom );
            current_y_pos += line_height_with_padding * 3;


            gpe::gcanvas->render_rectangle( gpe::screen_width * 2 / 3,gpe::screen_height - 96,gpe::screen_width,gpe::screen_height,pawgui::theme_main->popup_box_color,false);
            gpe::gfs->render_text( gpe::screen_width - 32 ,gpe::screen_height - 80,displayMessageTitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_right,gpe::fa_bottom );
            gpe::gfs->render_text( gpe::screen_width - 32,gpe::screen_height - 48,displayMessageSubtitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_right,gpe::fa_bottom );
            gpe::gfs->render_text( gpe::screen_width - 32,gpe::screen_height - 16,displayMessagestring,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_right, gpe::fa_bottom );
            gpe::gcanvas->render_rectangle( gpe::screen_width * 2 / 3,gpe::screen_height - 96,gpe::screen_width,gpe::screen_height ,pawgui::theme_main->popup_box_border_color,true);
        }
        else
        {
            gpe::gcanvas->render_rectangle( 32,gpe::screen_height/2 - 48,gpe::screen_width-32,gpe::screen_height/2 + 48,pawgui::theme_main->popup_box_color,false);
            gpe::gfs->render_text( gpe::screen_width/2,gpe::screen_height/2 - 32,displayMessageTitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top );
            gpe::gfs->render_text( gpe::screen_width/2,gpe::screen_height/2 ,displayMessageSubtitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_center );
            gpe::gfs->render_text( gpe::screen_width/2,gpe::screen_height/2 +  32,displayMessagestring,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center, gpe::fa_bottom );
            gpe::gcanvas->render_rectangle( 32,gpe::screen_height - 48 ,gpe::screen_width-32,gpe::screen_height/2 + 48 ,pawgui::theme_main->popup_box_border_color,true);

        }
        gpe::game_runtime->end_loop();
        gpe::game_runtime->start_loop();
    }
}



void gpe_splash_page::reset()
{
    item1Max = item1Value = 0;
    item2Max =  item2Value = 0;
    displayMessageTitle = "";
    displayMessageSubtitle = "";
    displayMessagestring = "";
}

void gpe_splash_page::set_bar1( float max_value)
{
    if( max_value> 0 )
    {
        item1Max = max_value;
        item1Value = 0;
    }
    else
    {
        item1Max = item1Value  = 0;
    }
}

void gpe_splash_page::set_bar2( float max_value)
{
    if( max_value> 0 )
    {
        item2Max = max_value;
        item2Value = 0;
    }
    else
    {
        item1Max = item2Value  = 0;
    }
}

void gpe_splash_page::update_messages(std::string title, std::string subTitle, std::string message, bool renderUpdate  )
{
    displayMessageTitle = title;
    update_submessages( subTitle, message, renderUpdate );
}

void gpe_splash_page::update_submessages(  std::string subTitle, std::string message, bool renderUpdate  )
{
    displayMessageSubtitle = subTitle;
    displayMessagestring = message;
    if( renderUpdate )
    {
        render_loader();
    }
}
