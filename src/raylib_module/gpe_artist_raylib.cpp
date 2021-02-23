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
        raylibRenderer = NULL;
        lightingOverlayTexture = new texture_target_raylib();

        artist_renderer = aRenderer;

        raylibRenderer = aRenderer;
        lightingOverlayTexture = NULL;

    }

    artist_raylib::~artist_raylib()
    {
        if( lightingOverlayTexture != NULL )
        {
            delete lightingOverlayTexture;
            lightingOverlayTexture = NULL;
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
        float step = (end_angle - start_angle) / arc_vertices;
        float theta = start_angle;
        float vx  = 0,  vy = 0;

        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;

        for(int arc_i = 0; arc_i < arc_vertices; arc_i++)
        {
            vx = arc_x + arc_radius * cos(theta);
            vy = arc_y + arc_radius * sin(theta);
            //line_render_points[arc_i].x = vx;
            //line_render_points[arc_i].y = vy;
            theta += step;
        }
        //set_color( render_color->get_r(),render_color->get_g(),render_color->get_b(),alpha_channel );

        //raylib_RenderDrawLinesF( raylibRenderer, line_render_points, arc_vertices );

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
        float step = (end_angle - start_angle) / arc_vertices;
        float theta = start_angle;// std::min(start_angle, end_angle);
        float vx  = 0,  vy = 0;

        set_color( render_color->get_r(),render_color->get_g(),render_color->get_b());
        set_render_alpha(alpha_channel  );
        for(int arc_i = 0; arc_i < arc_vertices; arc_i++)
        {
            vx = arc_x + arc_radius * cos(theta);
            vy = arc_y + arc_radius * sin(theta);
            theta += step;
            render_circle_filled_color( vx, vy, line_width, render_color, alpha_channel );
        }

    }

    bool artist_raylib::render_circle_filled( int x, int y, int rad )
    {
        return render_circle_filled_color(x,y,rad, color_current, alpha_current);
    }

    bool artist_raylib::render_circle_filled_color( int x, int y, int rad, color *render_color, int alpha_channel )
    {
        if( rad <= 0  || alpha_channel < 1 || render_color == NULL )
        {
            return false;
        }
        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;
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
        if( rad <= 0  || alpha_channel < 1 || render_color == NULL )
        {
            return false;
        }

        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;
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
        if( w <= 0  || h <= 0 || alpha_channel < 1 || render_color == NULL )
        {
            return false;
        }
        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;
        DrawEllipse(x, y, w,h, raylib_temp_color );
        return true;
    }

    bool artist_raylib::render_semi_circle( int x, int y, int rad, int direction, bool renderOutLine)
    {
        return render_semi_circle_color( x,y,rad, direction, color_current, alpha_current, renderOutLine );
    }

    bool artist_raylib::render_semi_circle_color( int x, int y, int rad, int direction, color *render_color, int alpha_channel, bool renderOutLine )
    {
        if( rad <= 0  || alpha_channel < 1 || render_color == NULL )
        {
            return false;
        }
        circle_center_pt.x = x;
        circle_center_pt.x = y;

        raylib_temp_color.r = render_color->get_r();
        raylib_temp_color.g = render_color->get_g();
        raylib_temp_color.b = render_color->get_b();
        raylib_temp_color.a = alpha_channel;
        DrawCircleSector(circle_center_pt,  rad, direction - 45, direction + 45, raylib_geometry_segments, raylib_temp_color );
        return true;
    }


    void artist_raylib::render_gradient_circle(  int radius, shape_rect * rendRect , color * render_color )
    {
        //Function not completed yet
    }

    void artist_raylib::render_gradient_vertical( shape_rect * rendRect, color * render_color)
    {
        if( raylibRenderer==NULL)
        {
            return;
        }
    }

    void artist_raylib::render_gradient_horizontal( shape_rect * rendRect, color * render_color)
    {
        if( raylibRenderer==NULL)
        {
            return;
        }
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
        if( raylibRenderer==NULL)
        {
            return;
        }
        if( rendRect!=NULL)
        {
            raylib_temp_color.r = render_color->get_r();
            raylib_temp_color.g = render_color->get_g();
            raylib_temp_color.b = render_color->get_b();
            raylib_temp_color.a = alpha_channel;
            if( outline)
            {
                DrawRectangleLines(rendRect->x, rendRect->y,rendRect->x, rendRect->h,raylib_temp_color );
            }
            else
            {
                 DrawRectangle(rendRect->x, rendRect->y,rendRect->x, rendRect->h,raylib_temp_color );
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
            DrawRectangleLines( x1, y1, x2-x1, y2-y1,raylib_temp_color );
        }
        else
        {
             DrawRectangle( x1, y1, x2-x1, y2-y1,raylib_temp_color );
        }
    }

    void artist_raylib::render_rotated_rectangle(int get_center(), int yCenter, int w, int h, int angle, color * render_color, int alpha_channel )
    {
        //DrawRectanglePro
    }

    void artist_raylib::render_roundrect_filled(int x1, int y1, int x2, int y2 )
    {
        int rad = std::min( std::abs( x1 - x2 ), std::abs( y2 - y1) ) /2;
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
        //DrawRectangleRounded()
    }

    void artist_raylib::render_roundrect_outline(int x1, int y1, int x2, int y2)
    {

    }

    void artist_raylib::render_roundrect_outline_radius(int x1, int y1, int x2, int y2, int rad)
    {

    }

    void artist_raylib::render_roundrect_outline_color( int x1, int y1,int x2, int y2, color * render_color, int alpha_channel  )
    {

    }

    void artist_raylib::render_roundrect_outline_color_radius( int x1, int y1,int x2, int y2, int rad,color * render_color, int alpha_channel  )
    {

    }

    void artist_raylib::render_triangle( shape_triangle2d * tri )
    {
        render_triangle_color( tri, color_current, alpha_current );
    }

    void artist_raylib::render_triangle_color( shape_triangle2d * tri, color * render_color, int alpha_channel )
    {
        if( tri == NULL )
        {
            return;
        }

        if( tri->vertices[0].x == tri->vertices[1].x || tri->vertices[0].x == tri->vertices[2].x || tri->vertices[1].x == tri->vertices[2].x )
        {
            return;
        }

        if( tri->vertices[0].y == tri->vertices[1].y || tri->vertices[0].y == tri->vertices[2].y || tri->vertices[1].y == tri->vertices[2].y )
        {
            return;
        }
        //DrawTriangle
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
        //Based on http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
        if( tri == NULL)
        {
            return;
        }
        if( line_width <= 1)
        {
            set_color( render_color->get_r(),render_color->get_g(),render_color->get_b() );
            set_render_alpha( alpha_channel );

            DrawLine( tri->vertices[0].x, tri->vertices[0].y, tri->vertices[1].x,tri->vertices[1].y, raylib_paint_color );
            DrawLine( tri->vertices[1].x,tri->vertices[1].y, tri->vertices[2].x,tri->vertices[2].y, raylib_paint_color );
            DrawLine( tri->vertices[0].x,tri->vertices[0].y, tri->vertices[2].x,tri->vertices[2].y, raylib_paint_color );
        }
        else
        {
            render_line_width_color(tri->vertices[0].x, tri->vertices[0].y, tri->vertices[1].x, tri->vertices[1].y, line_width,render_color, alpha_channel);
            render_line_width_color(tri->vertices[1].x, tri->vertices[1].y, tri->vertices[2].x, tri->vertices[2].y, line_width,render_color, alpha_channel);
            render_line_width_color(tri->vertices[2].x, tri->vertices[2].y, tri->vertices[0].x, tri->vertices[0].y, line_width,render_color, alpha_channel);
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

            DrawLine( x1, y1, x2, y2, raylib_paint_color );
            DrawLine( x2, y2, x3, y3, raylib_paint_color );
            DrawLine( x1, y1, x3, y3, raylib_paint_color );
        }
        else
        {
            render_line_width_color(x1, y1, x2, y2, line_width,render_color, alpha_channel);
            render_line_width_color(x2, y2, x3, y3, line_width,render_color, alpha_channel);
            render_line_width_color(x3, y3, x1, y1, line_width,render_color, alpha_channel);
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
            DrawLine( x1, y1, x2, y2, raylib_paint_color );
            return;
        }
        if( line_width < 0 )
        {
            return;
        }
        float lineAngle = semath::get_direction(x1, y1, x2, y2 );
        int lineSize  = ceil( semath::get_distance(x1, y1, x2, y2 ) );
        if( lineSize <= 0)
        {
            return;
        }


        x1  = x1 + semath::lengthdir_x( lineSize/2, lineAngle );
        y1  = y1 + semath::lengthdir_y( lineSize/2, lineAngle );
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

        if( raylibRenderer!=NULL )
        {
            raylibRenderer->set_render_blend_mode( blend_current_mode );
        }

    }

    void artist_raylib::set_color( color * color)
    {
        if( color==NULL )
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
        if( lightingOverlayTexture!=NULL )
        {
            lightingOverlayTexture->render_overlay(artist_renderer,-x, -y );
        }
    }

    void artist_raylib::render_ligting_overlay_scaled( int x, int y, float scale_size)
    {
        if( lightingOverlayTexture!=NULL )
        {
            lightingOverlayTexture->render_overlay_scaled( artist_renderer,-x, -y, scale_size, scale_size );
        }
    }

    void artist_raylib::resize_ligting_overlay( int w, int h)
    {
        if( lightingOverlayTexture==NULL )
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
        raylibRenderer->set_viewpoint( NULL );
        EndTextureMode();
        if( on && lightingOverlayTexture!=NULL )
        {
            if( lightingOverlayTexture->get_texture_type() == "target-raylib")
            {
                texture_target_raylib * raylibTextureTarget = (texture_target_raylib * ) lightingOverlayTexture;
                if( raylibTextureTarget !=NULL)
                {
                    BeginTextureMode( raylibTextureTarget->get_raylib_render_texture() );
                    renderer_main_raylib->reset_viewpoint();
                    set_color( 0, 0, 0 );
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
