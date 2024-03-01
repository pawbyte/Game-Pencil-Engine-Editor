/*
gpe_editor_splash_page.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.
Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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
    startup_mode = true;
    post_load_c_ticks = 0;
    post_load_c_ticks_max = 3000; //3 seconds to thank our patrons/spoonsors :-

    random_splash_top_four[0] = 0;
    random_splash_top_four[1] = 1;
    random_splash_top_four[2] = 2;
    random_splash_top_four[3] = 3;
    current_supporter_pos = 0;

    //Hard-coded for now....

    //Code contributors, educators, etc:
    top_supporters_credits.push_back("YellowAfterLife");
    top_supporters_credits.push_back("raysan");
    top_supporters_credits.push_back("icculus");
    top_supporters_credits.push_back("javidx9");
    top_supporters_credits.push_back("Acegikmo");

    //Patreon Sponsors:

    //Active
    top_supporters_credits.push_back("Anthony Gal");
    top_supporters_credits.push_back("Dennis Guardado");
    top_supporters_credits.push_back("Angelo Theodorou");
    top_supporters_credits.push_back("Marcus Guiterrez");
    top_supporters_credits.push_back("Ulises Bustamente");
    top_supporters_credits.push_back("Link X Peach");
    top_supporters_credits.push_back("Chris Munn");
    top_supporters_credits.push_back("Joe Davis");
    top_supporters_credits.push_back("JoeyGhost");
    top_supporters_credits.push_back("Raidenspawn");
    top_supporters_credits.push_back("Aerikos");
    top_supporters_credits.push_back("Kay");

    //Cancelled pledges
    top_supporters_credits.push_back("Eric Ellis");
    top_supporters_credits.push_back("Terver Myadze");
    top_supporters_credits.push_back("Mudra Zgat");
    top_supporters_credits.push_back("Victor Huynh");
    top_supporters_credits.push_back("Noah Rothermund");
    top_supporters_credits.push_back("Keiretsu");
    top_supporters_credits.push_back("James Thomas from Kerala");
    top_supporters_credits.push_back("Thomas Ingham");
    top_supporters_credits.push_back("Jen Anderson");
    top_supporters_credits.push_back("Kyle Schwartz");

    //Declined pledges
    top_supporters_credits.push_back("David Ballenger");
    top_supporters_credits.push_back("JM Lions");
    top_supporters_credits.push_back("Chad Davis");
    top_supporters_credits.push_back("Roshin Varghese");
    top_supporters_credits.push_back("Feral Monk");


    for( int i_sponsor = 0; i_sponsor < 4; i_sponsor++ )
    {
        random_splash_top_four[i_sponsor] = semath::random_int( (int)top_supporters_credits.size() -1);
    }
    current_supporter_pos = random_splash_top_four[0];
}

gpe_splash_page::~gpe_splash_page()
{

}

bool gpe_splash_page::exit_startup_mode()
{
    post_load_c_ticks+= gpe::time_keeper->get_delta_performance();

    displayMessageTitle = "Shouting out the homies";
    load_progress = 50.f + ( post_load_c_ticks / post_load_c_ticks_max )*50.f;

    if( post_load_c_ticks < post_load_c_ticks_max / 3 )
    {
        current_supporter_pos = random_splash_top_four[1];
        displayMessageSubtitle = "You rock!";
        displayMessagestring = "";
    }
    else if( post_load_c_ticks > post_load_c_ticks_max * 2 /3)
    {
        current_supporter_pos = random_splash_top_four[3];
        displayMessageSubtitle = "Thank you for being a supporter!";
        displayMessagestring = "";
    }
    else
    {
        current_supporter_pos = random_splash_top_four[2];
        displayMessageSubtitle = "Your rule!";
        displayMessagestring = "";
    }

    if( post_load_c_ticks >= post_load_c_ticks_max )
    {
        //Exists startup mode and enables window to be resized
        startup_mode = false;
        if( gpe::window_controller_main != nullptr )
        {
            gpe::window_controller_main->enable_window_resize();
            gpe::window_controller_main->maximize_window();
        }
        return true;
    }

    return false;
}

bool gpe_splash_page::in_startup_mode()
{
    return startup_mode;
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


void gpe_splash_page::render()
{
    //if( gpe::input->has_new_input() )
    {
        //Update screen
        gpe::game_runtime->end_loop( );
        gpe::game_runtime->start_loop();
        gpe::renderer_main->reset_viewpoint();

        std::string version_string = "Version" + stg_ex::float_to_string( gpe::version_number_total );
        if ( gpe::version_is_lts )
        {
            version_string+= " [LTS]";
        }
        if( startup_mode )
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
                gpe::gfs->render_text( gpe::screen_width*3 /4 + x_padding,current_y_pos - y_padding, version_string ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_bottom );
            }
            else
            {
                current_y_pos += line_height_with_padding;
                gpe::gfs->render_text( gpe::screen_width/2 + x_padding,current_y_pos - y_padding, "Game Penciil Engine" ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top );
                current_y_pos += line_height_with_padding;

                gpe::gfs->render_text( gpe::screen_width/2 + x_padding,current_y_pos - y_padding, version_string ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top );
                current_y_pos += line_height_with_padding;
            }

            current_y_pos += y_padding;

            gpe::gfs->render_text( gpe::screen_width/2,current_y_pos,"Copyright (c) 2014-2024 PawByte LLC. All rights reserved" ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_bottom );
            current_y_pos += line_height_with_padding * 2;

            gpe::gfs->render_text( gpe::screen_width/2,current_y_pos,"Game Pencil Engine is possible thanks to our code&wiki contributors, sponsors & friends " ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_bottom );
            current_y_pos += line_height_with_padding * 2;


            current_y_pos += line_height_with_padding * 3;


            gpe::gcanvas->render_rectangle( 0,gpe::screen_height - 176,gpe::screen_width,gpe::screen_height,pawgui::theme_main->popup_box_color,false);

            gpe::gfs->render_text( 32, gpe::screen_height - 144,"Thank you for your support",pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_bottom );
            gpe::gfs->render_text( 32, gpe::screen_height - 96,top_supporters_credits[ current_supporter_pos] ,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_bottom );
            gpe::gfs->render_text( 32, gpe::screen_height - 48,"You keep Open Source alive!",pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left, gpe::fa_bottom );

            if( post_load_c_ticks <= 0 )
            {
                gpe::gfs->render_text( gpe::screen_width - 32 ,gpe::screen_height - 144,displayMessageTitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_right,gpe::fa_bottom );
                gpe::gfs->render_text( gpe::screen_width - 32,gpe::screen_height - 96,displayMessageSubtitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_right,gpe::fa_bottom );
                gpe::gfs->render_text( gpe::screen_width - 32,gpe::screen_height - 48,displayMessagestring,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_right, gpe::fa_bottom );
            }
            else
            {
                gpe::gfs->render_text( gpe::screen_width - 32 ,gpe::screen_height - 144,displayMessageTitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_right,gpe::fa_bottom );
                gpe::gfs->render_text( gpe::screen_width - 32,gpe::screen_height - 96,displayMessageSubtitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_right,gpe::fa_bottom );
                gpe::gfs->render_text( gpe::screen_width - 32,gpe::screen_height - 48,displayMessagestring,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_right, gpe::fa_bottom );
            }

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

        //The loading bar:
        if( startup_mode )
        {
            gpe::gcanvas->render_rectangle( 0, gpe::screen_height - 32, gpe::screen_width ,gpe::screen_height, gpe::c_dkgray,false);
            gpe::gcanvas->render_rectangle( 0, gpe::screen_height - 32, gpe::screen_width * (load_progress/100.f),gpe::screen_height, gpe::c_navy,false);
            gpe::gcanvas->render_rectangle( 0, gpe::screen_height - 32, gpe::screen_width * (load_progress/100.f),gpe::screen_height, gpe::c_ltgray, true);
            gpe::gfs->render_text( gpe::screen_width/2,gpe::screen_height  - 8,"Starting Editor" ,gpe::c_ltgray, gpe::font_default,gpe::fa_center,gpe::fa_bottom );
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

void gpe_splash_page::set_load_percent( float load_percent)
{
    float past_load_value = load_progress;
    if ( load_percent > 0 && load_percent <= 50 )
    {
        load_progress = load_percent;
    }

    if( past_load_value != load_percent )
    {
        render();
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
        render();
    }
}
