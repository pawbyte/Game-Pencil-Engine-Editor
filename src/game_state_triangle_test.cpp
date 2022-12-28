/*
game_state_triangle_test.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

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

#include "game_state_triangle_test.h"

colored_triangle::colored_triangle()
{
    my_triangle = new gpe::shape_triangle2d();
    my_color = new gpe::color("triangle_color",0,0,0,255);
}

colored_triangle::~colored_triangle()
{
    if( my_triangle != NULL )
    {
        delete my_triangle;
        my_triangle = NULL;
    }

    if( my_color != NULL )
    {
        delete my_color;
        my_color = NULL;
    }
}

triangle_test_state::triangle_test_state( std::string s_name )
{
    if( (int)s_name.size() > 0 )
    {
        state_name = s_name;
    }
    else
    {
        state_name = "gpe_splash_screen";
    }
    bg_color = new gpe::color( "bg_color", 0, 0, 0, 255 );
    mouse_triangle = new gpe::shape_triangle2d();
    mouse_triangle_position = 0;
    colored_triangle * c_triangle = NULL;
    triangle_seeked_count = 0;
    for( int i_triangle = 0; i_triangle < 15; i_triangle++ )
    {
        create_random_triangle();
    }
    triangle_seeked_count = (int)random_triangles.size();
}

triangle_test_state::~triangle_test_state()
{

}

void triangle_test_state::apply_logic()
{
    colored_triangle * c_triangle = NULL;
    gpe::shape_triangle2d * s_triangle = NULL;
    int i_triangle = 0;

    if( gpe::input->check_kb_pressed( kb_a) )
    {
        for( i_triangle = (int)random_triangles.size() -1; i_triangle >= 0; i_triangle --)
        {
            c_triangle = random_triangles[i_triangle];

            c_triangle->my_triangle->vertices[0].x = semath::random(0,  gpe::screen_width);
            c_triangle->my_triangle->vertices[0].y = semath::random(0, gpe::screen_height);

            c_triangle->my_triangle->vertices[1].x = semath::random(0, gpe::screen_width);
            c_triangle->my_triangle->vertices[1].y = semath::random(0, gpe::screen_height);

            c_triangle->my_triangle->vertices[2].x = semath::random(0, gpe::screen_width);
            c_triangle->my_triangle->vertices[2].y = semath::random(0, gpe::screen_height);
        }
    }
    if( gpe::input->check_kb_pressed( kb_s ) )
    {
        for( i_triangle = (int)random_triangles.size() -1; i_triangle >= 0; i_triangle --)
        {
            c_triangle = random_triangles[i_triangle];
            c_triangle->my_color->change_rgba( semath::random(0,255),semath::random(0,255),semath::random(0,255),semath::random(0,255) );
        }
    }
    else if( gpe::input->check_kb_pressed( kb_w) )
    {
        create_random_triangle();
        triangle_seeked_count = (int)random_triangles.size();
    }
    else if( gpe::input->check_kb_pressed( kb_r) )
    {
        for( int i_ten_tris = 0; i_ten_tris < 10; i_ten_tris++ )
        {
            create_random_triangle();
        }
        triangle_seeked_count = (int)random_triangles.size();
    }
    else if( gpe::input->check_kb_pressed( kb_d) )
    {
        int triangles_size = (int)random_triangles.size();
        if( triangles_size > 0 )
        {
            c_triangle = random_triangles[ triangles_size -1 ];
            delete c_triangle;
            c_triangle = NULL;
            random_triangles.erase( random_triangles.begin() + triangles_size-1 );
        }
    }
    else if( gpe::input->check_kb_pressed( kb_up) )
    {
        triangle_seeked_count--;
    }
    else if( gpe::input->check_kb_pressed( kb_down) )
    {
        triangle_seeked_count++;
    }

    if( triangle_seeked_count > (int)random_triangles.size() )
    {
        triangle_seeked_count = (int)random_triangles.size();
    }
    if( triangle_seeked_count < 0)
    {
        triangle_seeked_count = 0;
    }

    if( gpe::input->check_mouse_released( mb_left) )
    {
        mouse_triangle->vertices[ mouse_triangle_position].x = gpe::input->mouse_position_x;
        mouse_triangle->vertices[ mouse_triangle_position].y = gpe::input->mouse_position_y;
        mouse_triangle_position++;
    }
    if( mouse_triangle_position > 2 )
    {
        mouse_triangle_position = 0;
    }
    if( mouse_triangle_position < 0)
    {
        mouse_triangle_position = 0;
    }

    if( gpe::window_controller_main->window_closed || gpe::input->exit_requested  )
    {
        gpe::game_runtime->state_set( "exit" );
    }
}

void triangle_test_state::clean_up()
{

}

colored_triangle * triangle_test_state::create_random_triangle()
{
    colored_triangle * c_triangle = new colored_triangle();

    c_triangle->my_triangle->vertices[0].x= semath::random(0, gpe::screen_width);
    c_triangle->my_triangle->vertices[0].y = semath::random(0, gpe::screen_height);

    c_triangle->my_triangle->vertices[1].x = semath::random(0, gpe::screen_width);
    c_triangle->my_triangle->vertices[1].y = semath::random(0, gpe::screen_height);

    c_triangle->my_triangle->vertices[2].x = semath::random(0, gpe::screen_width);
    c_triangle->my_triangle->vertices[2].y = semath::random(0, gpe::screen_height);

    c_triangle->my_color->change_rgba( semath::random(0,255),semath::random(0,255),semath::random(0,255),semath::random(0,255) );

    c_triangle->my_color->change_rgba( semath::random(0,255),semath::random(0,255),semath::random(0,255),semath::random(0,255) );
    random_triangles.push_back( c_triangle );
    triangle_seeked_count++;
}

void triangle_test_state::end_state()
{

}

void triangle_test_state::process_input()
{

}

void triangle_test_state::render()
{
    //gpe::gcanvas->render_rectangle( 0, 0, gpe::screen_width, gpe::screen_height, bg_color, false , 255 );
    colored_triangle * c_triangle = NULL;
    gpe::shape_triangle2d * s_triangle = NULL;
    int triangle_count = (int)random_triangles.size();
    int max_viewable_triangles = std::min( triangle_count, triangle_seeked_count);

    for( int i_triangle =  max_viewable_triangles - 1; i_triangle >=0 ; i_triangle-- )
    {
        c_triangle = random_triangles[i_triangle];
        s_triangle = c_triangle->my_triangle;
        gpe::gcanvas->render_triangle_color(s_triangle,c_triangle->my_color,c_triangle->my_color->get_a() );
    }



    gpe::gcanvas->render_triangle_color(mouse_triangle,gpe::c_ltorange, 255 );

    gpe::gfs->render_text( mouse_triangle->vertices[0].x,mouse_triangle->vertices[0].y,"0", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    gpe::gfs->render_text( mouse_triangle->vertices[1].x,mouse_triangle->vertices[1].y,"1", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    gpe::gfs->render_text( mouse_triangle->vertices[2].x,mouse_triangle->vertices[2].y,"2", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    gpe::gfs->render_text( gpe::screen_width/2, gpe::screen_height - 64, "Viewing "+stg_ex::int_to_string( triangle_seeked_count) + " /" + stg_ex::int_to_string( triangle_count)+" triangles", gpe::c_white, gpe::font_default, gpe::fa_center, gpe::fa_middle, 228 );
    gpe::gfs->render_fps( gpe::font_default, gpe::c_white, gpe::screen_width, 32 );

}

void triangle_test_state::start_state()
{

}
