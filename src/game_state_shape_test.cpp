/*
game_state_shape_test.cpp
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

#include "game_state_shape_test.h"

colored_circle::colored_circle( int x, int y, int r )
{
    my_circle = new gpe::shape_circle( x, y, r );
    my_color = new gpe::color("circle_color",semath::random(0,255),semath::random(0,255),semath::random(0,255),255);
}

colored_circle::~colored_circle()
{
    if( my_circle != NULL)
    {
        delete my_circle;
        my_circle = NULL;
    }

    if( my_color != NULL)
    {
        delete my_color;
        my_color = NULL;
    }
}

colored_arc::colored_arc( int x, int y, int r, float s_angle, float e_angle, float a_verts, int a_width )
{
    my_circle = new gpe::shape_circle( x, y, r );
    my_color = new gpe::color("arc_color",semath::random(0,255),semath::random(0,255),semath::random(0,255),255);
    arc_alpha = 255;
    start_arc_angle = s_angle * semath::math_radians_multiplier;
    end_arc_angle = e_angle* semath::math_radians_multiplier;
    arc_vertices = a_verts;
    arc_width = a_width;
}

colored_arc::~colored_arc()
{
    if( my_circle != NULL)
    {
        delete my_circle;
        my_circle = NULL;
    }

    if( my_color != NULL)
    {
        delete my_color;
        my_color = NULL;
    }
}

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

colored_rectangle::colored_rectangle( int x, int y, int w, int h )
{
    my_rectangle = new gpe::shape_rect( x, y, w, h );
    my_color = new gpe::color("circle_color",0,0,0,255);
}

colored_rectangle::~colored_rectangle()
{
    if( my_rectangle != NULL)
    {
        delete my_rectangle;
        my_rectangle = NULL;
    }

    if( my_color != NULL)
    {
        delete my_color;
        my_color = NULL;
    }
}

shape_test_state::shape_test_state( std::string s_name )
{
    if( (int)s_name.size() > 0 )
    {
        state_name = s_name;
    }
    else
    {
        state_name = "gpe_splash_screen";
    }

    for( int i_point = 0; i_point < gpe::render_points_giant_size; i_point++)
    {
        line_render_points[ i_point] ={0,0};
        //rect_render_points[ i_point] = {0,0,0,1 };
    }

    average_circles = 0;
    average_triangles = 0;
    current_frame = 0;
    current_circles = 0;
    current_triangles = 0;

    show_circles = true;
    show_triangles = true;
    show_rectangles = true;
    triangle_alpha_enabled = false;
    gpe::window_controller_main->set_window_title( "GPE SDL2 TRIANGLE TEST...");

    bg_color = new gpe::color( "bg_color", 0, 0, 0, 255 );

    arc_seeked_count = 0;
    arc_degree_ctr = 180;
    for( int i_arc = 0; i_arc < 64; i_arc++ )
    {
        create_random_arc();
    }
    arc_seeked_count = (int)random_arcs.size();

    circle_seeked_count = 0;
    mouse_circle = new gpe::shape_circle(0,0, 16 );
    mouse_circle_position = 0;

    mouse_triangle = new gpe::shape_triangle2d();
    mouse_triangle_position = 0;
    colored_triangle * c_triangle = NULL;
    triangle_seeked_count = 0;
    for( int i_triangle = 0; i_triangle < 15; i_triangle++ )
    {
        create_random_triangle();
    }
    triangle_seeked_count = (int)random_triangles.size();

    rectangle_seeked_count = 0;
    mouse_rectangle = new gpe::shape_rect( 0, 0, 64, 32 );
    mouse_rectangle_position = 0;
}

shape_test_state::~shape_test_state()
{

}

void shape_test_state::apply_logic()
{
    arc_degree_ctr -= gpe::time_keeper->get_delta_performance() / 8.f;

    if( arc_degree_ctr < -210 )
    {
        arc_degree_ctr = 180;
    }
    colored_triangle * c_triangle = NULL;
    gpe::shape_triangle2d * s_triangle = NULL;
    int i_triangle = 0;

    float c_fps = gpe::time_keeper->get_fps_cap();

    if( gpe::gcanvas != NULL )
    {
        current_circles+= gpe::gcanvas->get_circles_in_frame();
        current_triangles+= gpe::gcanvas->get_triangles_in_frame();
        current_frame ++;

        if( current_frame >= c_fps )
        {
           average_circles = current_circles / current_frame;
           average_circles = current_triangles / current_frame;

           current_frame = 0;
           current_circles = 0;
           current_triangles = 0;
        }
    }
    if( gpe::input->check_kb_pressed( kb_m ))
    {
        if( gpe::gcanvas->using_simple_geometry() )
        {
            gpe::gcanvas->disable_simple_geometry();
        }
        else
        {
            gpe::gcanvas->enable_simple_geometry();
        }
    }
    if( gpe::input->check_kb_pressed( kb_z ) )
    {
        if( c_fps >= 30 )
        {
            gpe::time_keeper->set_fps( c_fps - 15 );
        }
    }
    else if( gpe::input->check_kb_pressed( kb_x ) )
    {
        gpe::time_keeper->set_fps( c_fps + 15 );
    }
    if( gpe::input->check_kb_pressed( kb_c) )
    {
        show_circles = !show_circles;
    }
    if( gpe::input->check_kb_pressed( kb_v) )
    {
        triangle_alpha_enabled = !triangle_alpha_enabled;
    }
    else if( gpe::input->check_kb_pressed( kb_a) )
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
    else if( gpe::input->check_kb_pressed( kb_e) )
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
    else if( gpe::input->check_kb_pressed( kb_t) )
    {
        for( int i_ten_tris = 0; i_ten_tris < 100; i_ten_tris++ )
        {
            create_random_triangle();
        }
        triangle_seeked_count = (int)random_triangles.size();
    }
    else if( gpe::input->check_kb_pressed( kb_y) )
    {
        for( int i_ten_tris = 0; i_ten_tris < 1000; i_ten_tris++ )
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
    else if( gpe::input->check_kb_pressed( kb_f) )
    {
        int triangles_size = (int)random_triangles.size();
        if( triangles_size >= 10 )
        {
            for( int f_triangle = 0; f_triangle < 10; f_triangle++ )
            {
                c_triangle = random_triangles[ triangles_size -1 ];
                delete c_triangle;
                c_triangle = NULL;
                random_triangles.erase( random_triangles.begin() + triangles_size-1 );
                triangles_size--;
            }
        }
    }
    else if( gpe::input->check_kb_pressed( kb_g) )
    {
        int triangles_size = (int)random_triangles.size();
        if( triangles_size >= 100 )
        {
            for( int f_triangle = 0; f_triangle < 100; f_triangle++ )
            {
                c_triangle = random_triangles[ triangles_size -1 ];
                delete c_triangle;
                c_triangle = NULL;
                random_triangles.erase( random_triangles.begin() + triangles_size-1 );
                triangles_size--;
            }
        }
    }
    else if( gpe::input->check_kb_pressed( kb_h  ) )
    {
        int triangles_size = (int)random_triangles.size();
        if( triangles_size >= 1000 )
        {
            for( int f_triangle = 0; f_triangle < 1000; f_triangle++ )
            {
                c_triangle = random_triangles[ triangles_size -1 ];
                delete c_triangle;
                c_triangle = NULL;
                random_triangles.erase( random_triangles.begin() + triangles_size-1 );
                triangles_size--;
            }
        }
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

void shape_test_state::clean_up()
{

}

void shape_test_state::clear_circle()
{

}

void shape_test_state::clear_triangles()
{

}

void shape_test_state::clear_rectangles()
{

}

colored_arc * shape_test_state::create_random_arc()
{
    colored_arc * c_arc = new colored_arc( semath::random(0, gpe::screen_width), semath::random(0, gpe::screen_height), semath::random(0, 1024 ), semath::random(0,360),semath::random(0,360), semath::random(32,128), semath::random(0,64) );
    c_arc->my_color->change_rgba( semath::random(0,255),semath::random(0,255),semath::random(0,255),semath::random(0,255) );
    c_arc->arc_vertices = c_arc->my_circle->radius / 2; //semath::random(64,128 );
    c_arc->arc_alpha = semath::random(0, 255 );
    random_arcs.push_back( c_arc );
    arc_seeked_count++;
    return c_arc;
}

colored_circle * shape_test_state::create_random_circle()
{
    colored_circle * c_circle = new colored_circle( semath::random(0, gpe::screen_width), semath::random(0, gpe::screen_height), semath::random(0, 1024 ) );
    c_circle->my_color->change_rgba( semath::random(0,255),semath::random(0,255),semath::random(0,255),semath::random(0,255) );
    random_circles.push_back( c_circle );
    circle_seeked_count++;
    return c_circle;
}

colored_triangle * shape_test_state::create_random_triangle()
{
    colored_triangle * c_triangle = new colored_triangle();

    c_triangle->my_triangle->vertices[0].x= semath::random(0, gpe::screen_width);
    c_triangle->my_triangle->vertices[0].y = semath::random(0, gpe::screen_height);

    c_triangle->my_triangle->vertices[1].x = semath::random(0, gpe::screen_width);
    c_triangle->my_triangle->vertices[1].y = semath::random(0, gpe::screen_height);

    c_triangle->my_triangle->vertices[2].x = semath::random(0, gpe::screen_width);
    c_triangle->my_triangle->vertices[2].y = semath::random(0, gpe::screen_height);

    c_triangle->my_color->change_rgba( semath::random(0,255),semath::random(0,255),semath::random(0,255),semath::random(0,255) );

    random_triangles.push_back( c_triangle );
    triangle_seeked_count++;
    return c_triangle;
}

colored_rectangle * shape_test_state::create_random_rectangle()
{
    colored_rectangle * c_rectangle = new colored_rectangle(

    semath::random(0, gpe::screen_width),
    semath::random(0, gpe::screen_height),

    semath::random(0, gpe::screen_width),
    semath::random(0, gpe::screen_height) );

    c_rectangle->my_color->change_rgba( semath::random(0,255),semath::random(0,255),semath::random(0,255),semath::random(0,255) );

    random_rectangles.push_back( c_rectangle );
    rectangle_seeked_count++;
    return c_rectangle;
}


void shape_test_state::end_state()
{

}

void shape_test_state::process_input()
{

}

void shape_test_state::render()
{
    //gpe::gcanvas->render_rectangle( 0, 0, gpe::screen_width, gpe::screen_height, bg_color, false , 255 );
    colored_triangle * c_triangle = NULL;
    gpe::shape_triangle2d * s_triangle = NULL;
    int circle_count = (int)random_circles.size();
    int triangle_count = (int)random_triangles.size();
    int rectangle_count = (int)random_rectangles.size();
    int max_viewable_triangles = std::min( triangle_count, triangle_seeked_count);

    gpe::gcanvas->begin_geometric_shape();

    int i_triangle = 0;
    if( triangle_alpha_enabled )
    {
        for(  i_triangle =  max_viewable_triangles - 1; i_triangle >=0 ; i_triangle-- )
        {
            c_triangle = random_triangles[i_triangle];
            s_triangle = c_triangle->my_triangle;
            gpe::gcanvas->render_triangle_color(s_triangle,c_triangle->my_color,c_triangle->my_color->get_a() );
        }
    }
    else
    {
        for(  i_triangle =  max_viewable_triangles - 1; i_triangle >=0 ; i_triangle-- )
        {
            c_triangle = random_triangles[i_triangle];
            s_triangle = c_triangle->my_triangle;
            gpe::gcanvas->render_triangle_color(s_triangle,c_triangle->my_color,255 );
        }
    }


    gpe::gcanvas->render_triangle_color(mouse_triangle,gpe::c_ltorange, 255 );
    gpe::gcanvas->end_geometric_shape();

    colored_arc * c_arc;
    gpe::gcanvas->set_artist_blend_mode( gpe::blend_mode_blend );

    if( show_circles )
    {
        //renders randomized arcs on screen
        for( int i_arc =  arc_seeked_count - 1; i_arc >=0 ; i_arc-- )
        {
            c_arc = random_arcs[i_arc];
            gpe::gcanvas->render_arc_width_color( c_arc->my_circle->position.x,c_arc->my_circle->position.y,c_arc->my_circle->radius, c_arc->arc_width,
                                                c_arc->start_arc_angle,c_arc->end_arc_angle, c_arc->arc_vertices, c_arc->my_color, 255 );
        }


        //Renders arc animation around mouse cursor
        gpe::gcanvas->render_arc_width_color(gpe::input->mouse_position_x,gpe::input->mouse_position_y,32,8,180.f * semath::math_radians_multiplier,arc_degree_ctr * semath::math_radians_multiplier, 8, gpe::c_red, 255 );
        gpe::gcanvas->render_arc_width_color(gpe::input->mouse_position_x,gpe::input->mouse_position_y,64,8,180.f * semath::math_radians_multiplier,arc_degree_ctr * semath::math_radians_multiplier, 128, gpe::c_white, 255 );
        gpe::gcanvas->render_arc_width_color(gpe::input->mouse_position_x,gpe::input->mouse_position_y,96,8,180.f * semath::math_radians_multiplier,arc_degree_ctr * semath::math_radians_multiplier, 128, gpe::c_blue, 255 );
        gpe::gcanvas->render_arc_color(gpe::input->mouse_position_x,gpe::input->mouse_position_y,128,0,360.f * semath::math_radians_multiplier, 128, gpe::c_lime, 255 );
    }

    gpe::gcanvas->set_artist_blend_mode( gpe::blend_mode_blend );

    gpe::gfs->render_fps( gpe::font_default, gpe::c_white, gpe::screen_width, gpe::screen_height, gpe::fa_right, gpe::fa_bottom );

    //Renders controls
    gpe::gfs->render_text( 16, 16, "Circle Controls - JIKL", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    gpe::gfs->render_text( 16, 48, "Triangle Controls - WASD", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    gpe::gfs->render_text( 16, 80, "Rectangle Controls - ZXCV", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    gpe::gfs->render_text( 16, 112, "Toggle Shapes - Arrow Keys", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );

    if( gpe::gcanvas->using_simple_geometry() )
    {
        gpe::gfs->render_text( 16, 144, "Native-Rendered Geometry (Toggle by Pressing M-Key)", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    }
    else
    {
        gpe::gfs->render_text( 16, 144, "Line-Rendered Geometry (Toggle by Pressing M-Key)", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    }

    gpe::gfs->render_text( mouse_triangle->vertices[0].x,mouse_triangle->vertices[0].y,"0", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    gpe::gfs->render_text( mouse_triangle->vertices[1].x,mouse_triangle->vertices[1].y,"1", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );
    gpe::gfs->render_text( mouse_triangle->vertices[2].x,mouse_triangle->vertices[2].y,"2", gpe::c_white, gpe::font_default, gpe::fa_left, gpe::fa_top, 228 );

    gpe::gfs->render_text( gpe::screen_width/2, gpe::screen_height - 96, "Circle Count "+stg_ex::long_to_string( current_circles ), gpe::c_white, gpe::font_default, gpe::fa_center, gpe::fa_bottom, 228 );
    gpe::gfs->render_text( gpe::screen_width/2, gpe::screen_height - 64, "Viewing "+stg_ex::long_to_string( triangle_seeked_count) + " /" + stg_ex::long_to_string( gpe::gcanvas->get_triangles_in_frame())+" triangles", gpe::c_white, gpe::font_default, gpe::fa_center, gpe::fa_bottom, 228 );
    gpe::gfs->render_text( gpe::screen_width/2, gpe::screen_height - 32, "Viewing "+stg_ex::long_to_string( rectangle_seeked_count ) + " /" + stg_ex::long_to_string( rectangle_count)+" rectangles", gpe::c_white, gpe::font_default, gpe::fa_center, gpe::fa_bottom, 228 );

}

void shape_test_state::start_state()
{
    if( gpe::window_controller_main !=NULL )
    {
        gpe::window_controller_main->disable_scaling();
    }
}
