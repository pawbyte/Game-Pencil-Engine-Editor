/*
gpe_artist_raylib.cpp
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


#include "gpe_artist_raylib.h"

namespace gpe
{

    //GPE Render calls [ Begin ]
    artist_raylib::artist_raylib( renderer_system_raylib * aRenderer)
    {
        raylibRenderer = nullptr;
        lightingOverlayTexture = new texture_target_raylib();

        artist_renderer = aRenderer;

        raylibRenderer = aRenderer;

    }

    artist_raylib::~artist_raylib()
    {
        if( lightingOverlayTexture != nullptr )
        {
            delete lightingOverlayTexture;
            lightingOverlayTexture = nullptr;
        }
    }

    void artist_raylib::render_arc( float arc_x, float arc_y, float arc_radius, float start_angle, float end_angle, float arc_vertices )
    {
        render_arc_color(arc_x,arc_y,arc_radius, start_angle, end_angle, arc_vertices, color_current, alpha_current);
    }

    void artist_raylib::render_arc_color( float arc_x, float arc_y,float arc_radius, float start_angle, float end_angle, float arc_vertices,  color * render_color, int alpha_channel  )
    {
        if( arc_radius < 1 || start_angle == end_angle || arc_vertices < 1 || alpha_channel < 1 )
        {
            return;
        }

        start_angle = start_angle * semath::math_degrees_multiplier;
        end_angle = end_angle * semath::math_degrees_multiplier;
        float step = (end_angle - start_angle) / arc_vertices;



        if( render_color != nullptr )
        {
            raylib_temp_color.r = render_color->get_r();
            raylib_temp_color.g = render_color->get_g();
            raylib_temp_color.b = render_color->get_b();
        }
        raylib_temp_color.a = alpha_channel;
        arc_x += renderer_main_raylib->scissor_mode_offset.x;
        arc_y += renderer_main_raylib->scissor_mode_offset.y;
        DrawRing( (Vector2){ arc_x, arc_y}, arc_radius - 1.f, arc_radius, start_angle, end_angle , step, raylib_temp_color );

    }

    void artist_raylib::render_arc_width( float arc_x, float arc_y, float arc_radius, int line_width, float start_angle, float end_angle, float arc_vertices )
    {
        render_arc_width_color(arc_x,arc_y,arc_radius,line_width, start_angle, end_angle, arc_vertices, color_current, alpha_current);
    }

    void artist_raylib::render_arc_width_color( float arc_x, float arc_y,float arc_radius, int line_width, float start_angle, float end_angle, float arc_vertices,  color * render_color, int alpha_channel  )
    {
        if( arc_radius < 1 || start_angle == end_angle || arc_vertices < 1 || alpha_channel < 1 )
        {
            return;
        }
        if( line_width < 1)
        {
            return;
        }

        if( line_width == 1)
        {
            render_arc_color( arc_x, arc_y, arc_radius, start_angle, end_angle, arc_vertices, render_color, alpha_channel );
            return;
        }

        start_angle = start_angle * semath::math_degrees_multiplier;
        end_angle = end_angle * semath::math_degrees_multiplier;
        float step = (end_angle - start_angle) / arc_vertices;

        if( render_color != nullptr )
        {
            raylib_temp_color.r = render_color->get_r();
            raylib_temp_color.g = render_color->get_g();
            raylib_temp_color.b = render_color->get_b();
        }
        raylib_temp_color.a = alpha_channel;
        arc_x += renderer_main_raylib->scissor_mode_offset.x;
        arc_y += renderer_main_raylib->scissor_mode_offset.y;
        DrawRing( (Vector2){ arc_x, arc_y}, arc_radius - line_width, arc_radius, start_angle, end_angle , step, raylib_temp_color );
    }

    bool artist_raylib::render_circle_filled( int x, int y, int rad )
    {
        return render_circle_filled_color(x,y,rad, color_current, alpha_current);
    }

    bool artist_raylib::render_circle_filled_color( int x, int y, int rad, color *render_color, int alpha_channel )
    {
        if( rad <= 0  || alpha_channel < 1 || render_color == nullptr )
        {
            return false;
        }
        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;

        x += renderer_main_raylib->scissor_mode_offset.x;
        y += renderer_main_raylib->scissor_mode_offset.y;

        DrawCircle(x, y, rad, raylib_temp_color );
        return true;
    }

    bool artist_raylib::render_circle_outline( int x, int y, int rad )
    {
        return render_circle_outline_color( x, y, rad, color_current, alpha_current );
    }

    //Modified function from https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-raylib#48291620
    //Stackoverflow users Scotty Stephens / Pharap

    bool artist_raylib::render_circle_outline_color( int x, int y, int rad,  color * render_color, int alpha_channel )
    {
        if( rad <= 0  || alpha_channel < 1 || render_color == nullptr )
        {
            return false;
        }

        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;
        x += renderer_main_raylib->scissor_mode_offset.x;
        y += renderer_main_raylib->scissor_mode_offset.y;

        DrawCircleLines(x, y, rad, raylib_temp_color );
        return true;
    }

    bool artist_raylib::render_oval( int x, int y, int w, int h,bool renderOutLine )
    {
        //Function not completed yet
        return render_oval_color(x,y,w,h, color_current, alpha_current, renderOutLine);
    }

    bool artist_raylib::render_oval_color( int x, int y, int w, int h, color *render_color, int alpha_channel, bool renderOutLine )
    {
        if( w <= 0  || h <= 0 || alpha_channel < 1 || render_color == nullptr )
        {
            return false;
        }
        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;

        x += renderer_main_raylib->scissor_mode_offset.x;
        y += renderer_main_raylib->scissor_mode_offset.y;

        DrawEllipse(x, y, w,h, raylib_temp_color );
        return true;
    }

    bool artist_raylib::render_semi_circle( int x, int y, int rad, int direction, bool renderOutLine)
    {
        return render_semi_circle_color( x,y,rad, direction, color_current, alpha_current, renderOutLine );
    }

    bool artist_raylib::render_semi_circle_color( int x, int y, int rad, int direction, color *render_color, int alpha_channel, bool renderOutLine )
    {
        if( rad <= 0  || alpha_channel < 1 || render_color == nullptr )
        {
            return false;
        }

        x += renderer_main_raylib->scissor_mode_offset.x;
        y += renderer_main_raylib->scissor_mode_offset.y;

        circle_center_pt.x = x;
        circle_center_pt.x = y;

        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;
        DrawCircleSector(circle_center_pt,  rad, direction - 45, direction + 45, raylib_geometry_segments, raylib_temp_color );
        return true;
    }


    void artist_raylib::render_gradient_circle(  int radius, shape_rect * rendRect , color * render_color1, color * render_color2 )
    {
        //Function not completed yet
    }

    void artist_raylib::render_gradient_vertical( shape_rect * rendRect, color * render_color1, color * render_color2)
    {
        if( raylibRenderer==nullptr)
        {
            return;
        }
        if( rendRect == nullptr)
        {
            return;
        }

        if( render_color1 == nullptr || render_color2 == nullptr )
        {
            return;
        }

        raylib_gradient_color1.r = render_color1->get_r();
        raylib_gradient_color1.g = render_color1->get_g();
        raylib_gradient_color1.b = render_color1->get_b();
        raylib_gradient_color1.a = 255;

        raylib_gradient_color2.r = render_color2->get_r();
        raylib_gradient_color2.g = render_color2->get_g();
        raylib_gradient_color2.b = render_color2->get_b();
        raylib_gradient_color2.a = 255;


        DrawRectangleGradientV( rendRect->get_x() + renderer_main_raylib->scissor_mode_offset.x,
                               rendRect->get_y() +  renderer_main_raylib->scissor_mode_offset.y,
                               rendRect->get_x2() +  renderer_main_raylib->scissor_mode_offset.x,
                               rendRect->get_y2() +  renderer_main_raylib->scissor_mode_offset.y,
                                raylib_gradient_color1, raylib_gradient_color2);
    }

    void artist_raylib::render_gradient_horizontal( shape_rect * rendRect, color * render_color1, color * render_color2)
    {
        if( raylibRenderer==nullptr)
        {
            return;
        }
        if( rendRect == nullptr)
        {
            return;

        }

        if( render_color1 == nullptr || render_color2 == nullptr )
        {
            return;
        }
        raylib_gradient_color1.r = render_color1->get_r();
        raylib_gradient_color1.g = render_color1->get_g();
        raylib_gradient_color1.b = render_color1->get_b();
        raylib_gradient_color1.a = 255;

        raylib_gradient_color2.r = render_color2->get_r();
        raylib_gradient_color2.g = render_color2->get_g();
        raylib_gradient_color2.b = render_color2->get_b();
        raylib_gradient_color2.a = 255;

        DrawRectangleGradientH( rendRect->get_x() +  renderer_main_raylib->scissor_mode_offset.x,
                               rendRect->get_y()  +  renderer_main_raylib->scissor_mode_offset.y,
                               rendRect->get_x2() +  renderer_main_raylib->scissor_mode_offset.x,
                               rendRect->get_y2() +  renderer_main_raylib->scissor_mode_offset.y,
                                raylib_gradient_color1, raylib_gradient_color2);
    }


    void artist_raylib::render_point( int x, int y)
    {
        render_point_color(x,y, color_current, alpha_current);
    }

    void artist_raylib::render_point_color( int x, int y, color *render_color, int alpha_channel )
    {
        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;
        DrawPixel( x, y, raylib_temp_color );
    }

    void artist_raylib::render_rect( shape_rect * rendRect, color * render_color,bool outline, int alpha_channel)
    {
        if( raylibRenderer==nullptr)
        {
            return;
        }
        if( rendRect!=nullptr)
        {
            raylib_temp_color.r = render_color->get_r();
            raylib_temp_color.g = render_color->get_g();
            raylib_temp_color.b = render_color->get_b();
            raylib_temp_color.a = alpha_channel;
            if( outline)
            {
                DrawRectangleLines(rendRect->x + renderer_main_raylib->scissor_mode_offset.x, rendRect->y + renderer_main_raylib->scissor_mode_offset.y,rendRect->w, rendRect->h,raylib_temp_color );
            }
            else
            {
                 DrawRectangle(rendRect->x + renderer_main_raylib->scissor_mode_offset.x, rendRect->y + renderer_main_raylib->scissor_mode_offset.y,rendRect->w, rendRect->h,raylib_temp_color );
            }
        }
    }

    void artist_raylib::render_rectangle( int x1, int y1, int x2, int y2,  color * render_color,bool outline, int alpha_channel )
    {
        if( alpha_channel < 0 )
        {
            return;
        }
        else if( alpha_channel > 255 )
        {
            alpha_channel = 255;
        }
        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;
        if( outline)
        {
            DrawRectangleLines( x1 + renderer_main_raylib->scissor_mode_offset.x, y1 +renderer_main_raylib->scissor_mode_offset.y, x2-x1, y2-y1,raylib_temp_color );
        }
        else
        {
             DrawRectangle( x1 + renderer_main_raylib->scissor_mode_offset.x, y1 + renderer_main_raylib->scissor_mode_offset.y, x2-x1, y2-y1 ,raylib_temp_color );
        }
    }

    void artist_raylib::render_rotated_rectangle(int get_center(), int yCenter, int w, int h, int angle, color * render_color, int alpha_channel )
    {
        //DrawRectanglePro
    }

    void artist_raylib::render_roundrect_filled(int x1, int y1, int x2, int y2 )
    {
        int rad = std::min( std::abs( x1 - x2 ), std::abs( y2 - y1) ) /4;
        render_roundrect_filled_color_radius( x1, y1, x2, y2, rad,color_current, alpha_current );
    }

    void artist_raylib::render_roundrect_filled_radius(int x1, int y1, int x2, int y2, int rad )
    {
        render_roundrect_filled_color_radius( x1, y1, x2, y2, rad, color_current, alpha_current );
    }

    void artist_raylib::render_roundrect_filled_color( int x1, int y1,int x2, int y2, color * render_color , int alpha_channel )
    {
        int rad = std::min( std::abs( x1 - x2 ), std::abs( y2 - y1) ) / 2;
        rad = std::min( rad, 8);
        render_roundrect_filled_color_radius( x1, y1, x2, y2, rad, render_color, alpha_channel );
    }

    void artist_raylib::render_roundrect_filled_color_radius( int x1, int y1,int x2, int y2, int rad, color * render_color , int alpha_channel )
    {
        if( rad <=0 )
        {
            return;
        }
        if(render_color != nullptr )
        {
            raylib_paint_color.r = render_color->get_r();
            raylib_paint_color.g = render_color->get_g();
            raylib_paint_color.b = render_color->get_b();

        }
        raylib_paint_color.a = alpha_channel;
        rectangle_box.x = x1 + renderer_main_raylib->scissor_mode_offset.x;
        rectangle_box.y = y1 + renderer_main_raylib->scissor_mode_offset.y;
        rectangle_box.width = x2 - x1;
        rectangle_box.height = y2 - y1;
        DrawRectangleRounded(rectangle_box, 0.05f, 0,  raylib_paint_color);
    }

    void artist_raylib::render_roundrect_outline(int x1, int y1, int x2, int y2)
    {
        render_roundrect_outline_color_radius( x1, y1, x2, y2, 1, color_current,alpha_current );
    }

    void artist_raylib::render_roundrect_outline_radius(int x1, int y1, int x2, int y2, int rad)
    {
        render_roundrect_outline_color_radius( x1, y1, x2, y2, rad, color_current,alpha_current  );
    }

    void artist_raylib::render_roundrect_outline_color( int x1, int y1,int x2, int y2, color * render_color, int alpha_channel  )
    {
        render_roundrect_outline_color_radius( x1, y1, x2, y2, 1, render_color, alpha_channel );
    }

    void artist_raylib::render_roundrect_outline_color_radius( int x1, int y1,int x2, int y2, int rad,color * render_color, int alpha_channel  )
    {
        if( rad <=0 )
        {
            return;
        }
        if(render_color != nullptr )
        {
            raylib_paint_color.r = render_color->get_r();
            raylib_paint_color.g = render_color->get_g();
            raylib_paint_color.b = render_color->get_b();

        }
        raylib_paint_color.a = alpha_channel;
        rectangle_box.x = x1 + renderer_main_raylib->scissor_mode_offset.x;
        rectangle_box.y = y1 + renderer_main_raylib->scissor_mode_offset.y;
        rectangle_box.width = x2 - x1;
        rectangle_box.height = y2 - y1;
        DrawRectangleRoundedLines(rectangle_box, rad, 0, rad,   raylib_paint_color);
    }

    void artist_raylib::render_triangle( shape_triangle2d * tri )
    {
        render_triangle_color( tri, color_current, alpha_current );
    }

    void artist_raylib::render_triangle_color( shape_triangle2d * tri, color * render_color, int alpha_channel )
    {
        if( tri == nullptr )
        {
            return;
        }

        if( tri->vertices[0].x == tri->vertices[1].x || tri->vertices[0].x == tri->vertices[2].x || tri->vertices[1].x == tri->vertices[2].x )
        {
            //return;
        }

        if( tri->vertices[0].y == tri->vertices[1].y || tri->vertices[0].y == tri->vertices[2].y || tri->vertices[1].y == tri->vertices[2].y )
        {
            //return;
        }
        if( render_color != nullptr )
        {
            raylib_temp_color.r = render_color->get_r();
            raylib_temp_color.g = render_color->get_g();
            raylib_temp_color.b = render_color->get_b();
        }
        raylib_temp_color.a = alpha_channel;
        DrawTriangle( (Vector2){ tri->vertices[0].x + renderer_main_raylib->scissor_mode_offset.x,  tri->vertices[0].y+ renderer_main_raylib->scissor_mode_offset.y} ,
                         (Vector2){tri->vertices[1].x + renderer_main_raylib->scissor_mode_offset.x,  tri->vertices[1].y + renderer_main_raylib->scissor_mode_offset.y},
                         (Vector2){tri->vertices[2].x + renderer_main_raylib->scissor_mode_offset.x,  tri->vertices[2].y + renderer_main_raylib->scissor_mode_offset.y}, raylib_temp_color );
    }


    void artist_raylib::render_triangle_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color , int alpha_channel )
    {

    }

    void artist_raylib::render_triangle_coords( int x1, int y1, int x2, int y2, int x3, int y3)
    {
        render_triangle_color_coords( x1, y1, x2, y2, x3, y3, color_current, alpha_current );
    }

    void artist_raylib::render_triangle_outline( shape_triangle2d * tri, int line_width )
    {
        render_triangle_outline_color( tri, color_current, alpha_current, line_width );
    }

    void artist_raylib::render_triangle_outline_color( shape_triangle2d * tri, color * render_color , int alpha_channel,int line_width )
    {
        if( tri == nullptr)
        {
            return;
        }
        if( line_width <= 1)
        {
            set_color( render_color->get_r(),render_color->get_g(),render_color->get_b() );
            set_render_alpha( alpha_channel );

            DrawLine( tri->vertices[0].x + renderer_main_raylib->scissor_mode_offset.x, tri->vertices[0].y + renderer_main_raylib->scissor_mode_offset.y, tri->vertices[1].x + renderer_main_raylib->scissor_mode_offset.x,tri->vertices[1].y + renderer_main_raylib->scissor_mode_offset.y, raylib_paint_color );
            DrawLine( tri->vertices[1].x + renderer_main_raylib->scissor_mode_offset.x,tri->vertices[1].y + renderer_main_raylib->scissor_mode_offset.y, tri->vertices[2].x + renderer_main_raylib->scissor_mode_offset.x,tri->vertices[2].y + renderer_main_raylib->scissor_mode_offset.y, raylib_paint_color );
            DrawLine( tri->vertices[0].x + renderer_main_raylib->scissor_mode_offset.x,tri->vertices[0].y + renderer_main_raylib->scissor_mode_offset.y, tri->vertices[2].x + renderer_main_raylib->scissor_mode_offset.x,tri->vertices[2].y + renderer_main_raylib->scissor_mode_offset.y, raylib_paint_color );
        }
        else
        {
            render_line_width_color(tri->vertices[0].x + renderer_main_raylib->scissor_mode_offset.x, tri->vertices[0].y + renderer_main_raylib->scissor_mode_offset.y, tri->vertices[1].x + renderer_main_raylib->scissor_mode_offset.x, tri->vertices[1].y + renderer_main_raylib->scissor_mode_offset.y, line_width,render_color, alpha_channel);
            render_line_width_color(tri->vertices[1].x + renderer_main_raylib->scissor_mode_offset.x, tri->vertices[1].y + renderer_main_raylib->scissor_mode_offset.y, tri->vertices[2].x + renderer_main_raylib->scissor_mode_offset.x, tri->vertices[2].y + renderer_main_raylib->scissor_mode_offset.y, line_width,render_color, alpha_channel);
            render_line_width_color(tri->vertices[2].x + renderer_main_raylib->scissor_mode_offset.x, tri->vertices[2].y + renderer_main_raylib->scissor_mode_offset.y, tri->vertices[0].x + renderer_main_raylib->scissor_mode_offset.x, tri->vertices[0].y + renderer_main_raylib->scissor_mode_offset.y, line_width,render_color, alpha_channel);
        }
        return;
    }

    void artist_raylib::render_triangle_outline_coords( int x1, int y1, int x2, int y2, int x3, int y3, int line_width )
    {
        render_triangle_outline_color_coords( x1, y1, x2, y2, x3, y3, color_current, alpha_current, line_width );
    }

    void artist_raylib::render_triangle_outline_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color, int alpha_channel,int line_width )
    {
        if( line_width == 1)
        {
            set_color( render_color->get_r(),render_color->get_g(),render_color->get_b() );
            set_render_alpha( alpha_channel );

            DrawLine( x1 + renderer_main_raylib->scissor_mode_offset.x, y1 + renderer_main_raylib->scissor_mode_offset.y, x2 + renderer_main_raylib->scissor_mode_offset.x, y2 + renderer_main_raylib->scissor_mode_offset.y, raylib_paint_color );
            DrawLine( x2 + renderer_main_raylib->scissor_mode_offset.x, y2 + renderer_main_raylib->scissor_mode_offset.y, x3 + renderer_main_raylib->scissor_mode_offset.x, y3 + renderer_main_raylib->scissor_mode_offset.y, raylib_paint_color );
            DrawLine( x1 + renderer_main_raylib->scissor_mode_offset.x, y1 + renderer_main_raylib->scissor_mode_offset.y, x3 + renderer_main_raylib->scissor_mode_offset.x, y3 + renderer_main_raylib->scissor_mode_offset.y, raylib_paint_color );
        }
        else
        {
            render_line_width_color(x1 + renderer_main_raylib->scissor_mode_offset.x, y1 + renderer_main_raylib->scissor_mode_offset.y, x2 + renderer_main_raylib->scissor_mode_offset.x, y2 + renderer_main_raylib->scissor_mode_offset.y, line_width,render_color, alpha_channel);
            render_line_width_color(x2 + renderer_main_raylib->scissor_mode_offset.x, y2 + renderer_main_raylib->scissor_mode_offset.y, x3 + renderer_main_raylib->scissor_mode_offset.x, y3 + renderer_main_raylib->scissor_mode_offset.y, line_width,render_color, alpha_channel);
            render_line_width_color(x3 + renderer_main_raylib->scissor_mode_offset.x, y3 + renderer_main_raylib->scissor_mode_offset.y, x1 + renderer_main_raylib->scissor_mode_offset.x, y1 + renderer_main_raylib->scissor_mode_offset.y, line_width,render_color, alpha_channel);
        }
        return;
    }


    void artist_raylib::render_square( int x, int y, int squareSize,  color * render_color,bool outline, int alpha_channel )
    {
        render_rectangle(x, y, x+squareSize, y+squareSize, render_color, outline, alpha_channel );
    }

    void artist_raylib::render_line( int x1, int y1, int x2, int y2 )
    {
        render_line_width_color(x1, y1, x2, y2, defaultLineWidth, color_current,  alpha_current );
    }

    void artist_raylib::render_line_color( int x1, int y1, int x2, int y2,  color * render_color, int alpha_channel)
    {
        render_line_width_color(x1, y1, x2, y2, defaultLineWidth, render_color, alpha_channel);
    }

    void artist_raylib::render_line_capped(  int x1, int y1, int x2, int y2, int line_width )
    {

    }

    void artist_raylib::render_line_capped_color(  int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel )
    {
        render_line_width_color(x1, y1, x2, y2, line_width, render_color, alpha_channel );
    }

    void artist_raylib::render_line_width( int x1, int y1, int x2, int y2, int line_width)
    {
        if( line_width <= 0 || line_width > 256  )
        {
            return;
        }
        //line_width = 1;
        float lineAngle = semath::get_direction(x1, y1, x2, y2 );
        int lineSize  = ceil( semath::get_distance(x1, y1, x2, y2 ) );
        //render_line(x1,y1,x2,y2, color_current, alpha_current );

        x1  = x1 + semath::lengthdir_x( lineSize/2, lineAngle );
        y1  = y1 + semath::lengthdir_y( lineSize/2, lineAngle );

    }

    void artist_raylib::render_line_width_color( int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel )
    {
        if( line_width == 1 )
        {
            set_color( render_color->get_r(),render_color->get_g(),render_color->get_b() );
            set_render_alpha( alpha_current );
            DrawLine( x1 + renderer_main_raylib->scissor_mode_offset.x, y1 + renderer_main_raylib->scissor_mode_offset.y, x2 + renderer_main_raylib->scissor_mode_offset.x, y2 + renderer_main_raylib->scissor_mode_offset.y, raylib_paint_color );
            return;
        }

        if( line_width < 0 )
        {
            return;
        }


        if( render_color != nullptr )
        {
            raylib_temp_color.r = render_color->get_r();
            raylib_temp_color.g = render_color->get_g();
            raylib_temp_color.b = render_color->get_b();
        }

        raylib_temp_color.a = alpha_channel;

       triangle_pt1.x = x1;
       triangle_pt1.y = y1;

       triangle_pt2.x = x2;
       triangle_pt2.y = y2;

       DrawLineEx( triangle_pt1, triangle_pt2, line_width, raylib_temp_color );
    }

    void artist_raylib::render_horizontal_line(int y, int x1, int x2)
    {
        if( x1!=x2 )
        {
            render_line_width_color(x1, y, x2, y, defaultLineWidth, color_current, alpha_current);
        }
    }

    void artist_raylib::render_horizontal_line_color( int y, int x1, int x2,  color * render_color, int alpha_channel)
    {
        if( x1!=x2  )
        {
            render_line_width_color(x1, y, x2, y, defaultLineWidth, render_color, alpha_channel);
        }
    }

    void artist_raylib::render_vertical_line( int x, int y1, int y2)
    {
        if( y1!=y2 )
        {
            //render_line_color(x,y1,x,y2, color_current, alpha_current );
            render_line_width_color(x, y1, x, y2, defaultLineWidth, color_current, alpha_current);
        }
    }

    void artist_raylib::render_vertical_line_color( int x, int y1, int y2,  color * render_color, int alpha_channel)
    {
        if( y1!=y2  )
        {
             render_line_width_color(x, y1, x, y2, defaultLineWidth, render_color, alpha_channel);
        }
    }

    void artist_raylib::set_artist_blend_mode( int blend_mode_new )
    {
        if( blend_current_mode==blend_mode_new)
        {
            return;
        }
        blend_current_mode = blend_mode_new;
        if( blend_current_mode < 0 || blend_current_mode > blend_mode_none )
        {
            blend_current_mode = blend_mode_blend;
        }

        if( raylibRenderer!=nullptr )
        {
            raylibRenderer->set_render_blend_mode( blend_current_mode );
        }

    }

    void artist_raylib::set_color( color * color)
    {
        if( color==nullptr )
        {
            return;
        }
       color_current->change_rgba( color->get_r(), color->get_g(), color->get_b(), alpha_current );
       raylib_paint_color.r = color->get_r();
       raylib_paint_color.g = color->get_g();
       raylib_paint_color.b = color->get_b();
       raylib_paint_color.a = alpha_current;
    }

    void artist_raylib::set_color( int r, int g, int b )
    {
        color_current->change_rgba( r, g, b, alpha_current );
        raylib_paint_color.r = r;
        raylib_paint_color.g = g;
        raylib_paint_color.b = b;
        raylib_paint_color.a = alpha_current;

    }

    void artist_raylib::set_render_alpha( int alpha_new )
    {
        alpha_current = alpha_new;
        raylib_paint_color.a = alpha_current;
        color_current->change_rgba( raylib_paint_color.r, raylib_paint_color.g, raylib_paint_color.b, alpha_current );
    }

    void artist_raylib::set_line_width( int new_width )
    {
        defaultLineWidth = new_width;
    }

    void artist_raylib::render_ligting_overlay( int x, int y)
    {
        if( lightingOverlayTexture!=nullptr )
        {
            lightingOverlayTexture->render_overlay(artist_renderer,x, y );
        }
    }

    void artist_raylib::render_ligting_overlay_scaled( int x, int y, float scale_size)
    {
        if( lightingOverlayTexture!=nullptr )
        {
            lightingOverlayTexture->render_overlay_scaled( artist_renderer,x, y, scale_size, scale_size );
        }
    }

    void artist_raylib::resize_ligting_overlay( int w, int h)
    {
        if( lightingOverlayTexture==nullptr )
        {
            lightingOverlayTexture = new texture_target_raylib();
        }
        lightingOverlayTexture->resize_target( raylibRenderer,w, h);
        lightingOverlayTexture->change_color( 255, 255, 255 );
        lightingOverlayTexture->change_alpha( 255 );
        lightingOverlayTexture->set_blend_mode( blend_mode_mod );
    }

    void artist_raylib::switch_ligting_overlay( bool on )
    {
        raylibRenderer->set_viewpoint( nullptr );
        EndTextureMode();
        if( on && lightingOverlayTexture!=nullptr )
        {
            if( lightingOverlayTexture->get_texture_type() == "target-raylib")
            {
                texture_target_raylib * raylibTextureTarget = (texture_target_raylib * ) lightingOverlayTexture;
                if( raylibTextureTarget !=nullptr)
                {
                    BeginTextureMode( raylibTextureTarget->get_raylib_render_texture() );
                    renderer_main_raylib->reset_viewpoint();
                    set_color( 0, 0, 0 );
                    raylib_paint_color.r = 0;
                    raylib_paint_color.g = 0;
                    raylib_paint_color.b = 0;
                    raylib_paint_color.a = 255;
                    ClearBackground( raylib_paint_color );
                    return;
                }
            }
        }
        else
        {
            renderer_main_raylib->reset_viewpoint();
        }
    }
}
