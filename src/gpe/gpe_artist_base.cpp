/*
gpe_artist_base.h
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

#include "gpe_artist_base.h"

namespace gpe
{
    artist_base * gcanvas  = nullptr;

    artist_base::artist_base()
    {
        artist_renderer = nullptr;

        defaultLineWidth = 1;
        temp_clip_rect = new shape_rect();
        //duplicates the color white
        color_current = c_white->duplicate_color();
        color_temp = c_white->duplicate_color();
        alpha_current = 255;

        lightingOverlayTexture = nullptr;
    }

    artist_base::artist_base( renderer_base * aRenderer )
    {
        artist_renderer = aRenderer;

        defaultLineWidth = 1;
        temp_clip_rect = new shape_rect();
        //duplicates the color white
        color_current = c_white->duplicate_color();
        color_temp = c_white->duplicate_color();
        alpha_current = 255;

        lightingOverlayTexture = nullptr;
    }

    artist_base::~artist_base()
    {
        if( temp_clip_rect!=nullptr )
        {
            delete temp_clip_rect;
            temp_clip_rect = nullptr;
        }

        if( color_current!=nullptr )
        {
            delete color_current;
            color_current = nullptr;
        }

        if( color_temp!=nullptr )
        {
            delete color_temp;
            color_temp = nullptr;
        }

        if( lightingOverlayTexture!=nullptr )
        {
            delete lightingOverlayTexture;
            lightingOverlayTexture = nullptr;
        }
    }

    int artist_base::animation_get_number(animaton2d* animationIn)
    {
        if(animationIn!=nullptr)
        {
            return animationIn->get_frame_count();
        }
        else
        {
            return -1;
        }
    }

    int artist_base::get_artist_blend_mode()
    {
        return blend_current_mode;
    }

    void artist_base::render_animation_named( std::string resource_name,  int sub_image_to_draw, int x_pos, int y_pos, shape_rect *cam)
    {
        animaton2d* animationToDraw = rsm->get_animation( resource_name );
        if(animationToDraw!=nullptr)
        {
            animationToDraw->render(sub_image_to_draw,x_pos,y_pos, cam );
        }
    }

    void artist_base::render_animation_named_resized( std::string resource_name,  int sub_image_to_draw, int x_pos, int y_pos, int new_width, int new_height, shape_rect * cam)
    {
        animaton2d * animationToDraw = rsm->get_animation( resource_name );
        if(animationToDraw!=nullptr)
        {
            animationToDraw->render_resized(sub_image_to_draw,x_pos,y_pos,new_width, new_height, cam );
        }
    }

    void artist_base::render_animation_named_special( std::string resource_name, int sub_image_to_draw, int x_pos, int y_pos,  int new_width, int new_height,float new_angle,color * render_color, int alpha, shape_rect * cam)
    {
        animaton2d * animationToDraw = rsm->get_animation( resource_name );
        if(animationToDraw!=nullptr)
        {
            animationToDraw->render_special(sub_image_to_draw,x_pos,y_pos, new_width, new_height, new_angle,render_color,alpha, cam );
        }
    }

    void artist_base::render_animation_named_rotated( std::string resource_name,  int sub_image_to_draw, int x_pos, int y_pos, float new_angle,int new_width, int new_height,  shape_rect *cam)
    {
        animaton2d * animationToDraw = rsm->get_animation( resource_name );
        if(animationToDraw!=nullptr)
        {
            animationToDraw->render_rotated( sub_image_to_draw, x_pos, y_pos, new_angle, new_width, new_height, cam );
        }
    }

    void artist_base::render_animation(animaton2d* animationToDraw, int sub_image_to_draw, int x_pos, int y_pos, shape_rect *cam)
    {
        if(animationToDraw!=nullptr)
        {
            animationToDraw->render(sub_image_to_draw,x_pos,y_pos, cam );
        }
    }


    void artist_base::render_animation_resized(animaton2d* animationToDraw, int sub_image_to_draw, int x_pos, int y_pos, int new_width, int new_height, shape_rect * cam)
    {
        if(animationToDraw!=nullptr)
        {
            animationToDraw->render_resized(sub_image_to_draw,x_pos,y_pos,new_width, new_height, cam );
        }
    }

    void artist_base::render_animation_special(animaton2d* animationToDraw, int sub_image_to_draw, int x_pos, int y_pos,  int new_width, int new_height,float new_angle,color * render_color, int alpha, shape_rect * cam)
    {
        if(animationToDraw!=nullptr)
        {
            animationToDraw->render_special(sub_image_to_draw,x_pos,y_pos, new_width, new_height, new_angle,render_color, alpha, cam );
        }
    }

    void artist_base::render_animation_rotated(animaton2d* animationToDraw, int sub_image_to_draw, int x_pos, int y_pos, float new_angle, int new_width, int new_height, shape_rect *cam)
    {
        if( cam==nullptr)
        {
            cam = camera_default;
        }
        if(animationToDraw!=nullptr)
        {
            animationToDraw->render_rotated( sub_image_to_draw, x_pos, y_pos, new_angle, new_width, new_height, cam );
        }
    }



    void artist_base::render_texture_named( std::string resource_name, int x, int y, shape_rect* clip, int alignment_h, int alignment_v )
    {

    }

    void artist_base::render_texture_named_resized( std::string resource_name, int x, int y,int new_width, int new_height, shape_rect* clip, int alignment_h, int alignment_v, color * render_color )
    {

    }

    void artist_base::render_texture_named_rotated( std::string resource_name, int x, int y,float new_angle,int new_width, int new_height, shape_rect* clip )
    {

    }

    void artist_base::render_arc( float arc_x, float arc_y, float arc_radius, float start_angle, float end_angle, float arc_vertices  )
    {

    }

    void artist_base::render_arc_color( float arc_x, float arc_y,float arc_radius, float start_angle, float end_angle, float arc_vertices,  color * render_color, int alpha_channel  )
    {

    }

    void artist_base::render_arc_width( float arc_x, float arc_y, float arc_radius, int line_width, float start_angle, float end_angle, float arc_vertices  )
    {

    }

    void artist_base::render_arc_width_color( float arc_x, float arc_y,float arc_radius, int line_width, float start_angle, float end_angle, float arc_vertices,  color * render_color, int alpha_channel  )
    {

    }

    //Circle and SemiCircles and Ovals Rendering Functions
    bool artist_base::render_circle_filled( int x, int y, int rad)
    {
        return false;
    }

    bool artist_base::render_circle_filled_color( int x, int y, int rad, color * render_color, int alpha_channel )
    {
         return false;
    }

    bool artist_base::render_circle_outline( int x, int y, int rad)
    {
        return false;
    }

    bool artist_base::render_circle_outline_color( int x, int y, int rad, color * render_color, int alpha_channel )
    {
         return false;
    }

    bool artist_base::render_oval( int x, int y, int w, int h, bool renderOutLine )
    {
         return false;
    }

    bool artist_base::render_oval_color( int x, int y, int w, int h, color *render_color, int alpha_channel, bool renderOutLine)
    {
        return false;
    }

    bool artist_base::render_semi_circle( int x, int y, int rad, int direction, bool renderOutLine)
    {
        return false;
    }

    bool artist_base::render_semi_circle_color( int x, int y, int rad, int direction, color *render_color, int alpha_channel, bool renderOutLine)
    {
        return false;
    }


    //Gradients Rendering [ BEGIN ]
    void artist_base::render_gradient_circle(  int radius, shape_rect * rendRect , color * render_color )
    {

    }

    void artist_base::render_gradient_horizontal(  shape_rect * rendRect, color * render_color )
    {

    }

    void artist_base::render_gradient_vertical(  shape_rect * rendRect, color * render_color )
    {

    }

    //Gradients Rendering [ END ]

    //Line Rendering [ BEGIN ]
    void artist_base::render_horizontal_line(int y, int x1, int x2)
    {

    }

    void artist_base::render_horizontal_line_color( int y, int x1, int x2, color *render_color, int alpha_channel )
    {

    }


    void artist_base::render_line(  int x1, int y1, int x2, int y2 )
    {

    }

    void artist_base::render_line_color(  int x1, int y1, int x2, int y2,  color *render_color, int alpha_channel )
    {

    }

    void artist_base::render_line_capped(  int x1, int y1, int x2, int y2, int line_width )
    {

    }

    void artist_base::render_line_capped_color(  int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel )
    {

    }

    void artist_base::render_line_width(  int x1, int y1, int x2, int y2, int line_width)
    {

    }

    void artist_base::render_line_width_color(  int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel )
    {

    }

    void artist_base::render_vertical_line( int x, int y1, int y2)
    {

    }

    void artist_base::render_vertical_line_color( int x, int y1, int y2, color *render_color, int alpha_channel )
    {

    }

    //Line Rendering [ END ]

    //Points Rendering [ BEGIN ]
    void artist_base::render_point( int x, int y)
    {

    }

    void artist_base::render_point_color( int x, int y, color *render_color, int alpha_channel )
    {

    }

    //Points Rendering [ END ]

    //Roundrect Rendering [ BEGIN ]
    void artist_base::render_rect(  shape_rect * rendRect,color * render_color,bool outline, int alpha_channel )
    {

    }

    void artist_base::render_rectangle( int x1, int y1, int x2, int y2,  color *render_color,bool outline , int alpha_channel )
    {

    }

    void artist_base::render_square( int x, int y, int squareSize,  color *render_color,bool outline , int alpha_channel )
    {

    }

    void artist_base::render_rotated_rectangle(int get_center(), int yCenter, int w, int h, int angle, color * render_color , int alpha_channel )
    {

    }

    void artist_base::render_roundrect_filled(int x1, int y1, int x2, int y2)
    {

    }

    void artist_base::render_roundrect_filled_radius(int x1, int y1, int x2, int y2, int rad )
    {

    }

    void artist_base::render_roundrect_filled_color( int x1, int y1,int x2, int y2,color * render_color, int alpha_channel  )
    {

    }

    void artist_base::render_roundrect_filled_color_radius( int x1, int y1,int x2, int y2, int rad,color * render_color, int alpha_channel  )
    {

    }

    void artist_base::render_roundrect_outline(int x1, int y1, int x2, int y2)
    {

    }

    void artist_base::render_roundrect_outline_radius(int x1, int y1, int x2, int y2, int rad)
    {

    }
    void artist_base::render_roundrect_outline_color( int x1, int y1,int x2, int y2, color * render_color , int alpha_channel)
    {

    }

    void artist_base::render_roundrect_outline_color_radius( int x1, int y1,int x2, int y2, int rad,color * render_color, int alpha_channel )
    {

    }

    //Roundrect Rendering [ END ]

    //Triangle Rendering [ BEGIN ]
    void artist_base::render_triangle( shape_triangle2d * tri )
    {

    }

    void artist_base::render_triangle_color( shape_triangle2d * tri, color * render_color , int alpha_channel )
    {

    }

    void artist_base::render_triangle_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color , int alpha_channel )
    {

    }

    void artist_base::render_triangle_coords( int x1, int y1, int x2, int y2, int x3, int y3)
    {

    }

    void artist_base::render_triangle_outline( shape_triangle2d * tri, int line_width )
    {

    }

    void artist_base::render_triangle_outline_coords( int x1, int y1, int x2, int y2, int x3, int y3, int line_width )
    {

    }

    void artist_base::render_triangle_outline_color( shape_triangle2d * tri, color * render_color , int alpha_channel ,int line_width )
    {

    }

    void artist_base::render_triangle_outline_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color, int alpha_channel,int line_width )
    {

    }
    //Triangle Rendering [ END ]


    void artist_base::set_color( color * color)
    {

    }

    void artist_base::set_color( int r, int g, int b )
    {

    }

    void artist_base::set_render_alpha( int alpha_new )
    {

    }

    void artist_base::set_line_width( int new_width )
    {

    }

    void artist_base::set_artist_blend_mode( int blend_mode_new )
    {

    }


    void artist_base::render_ligting_overlay( int x, int y)
    {

    }

    void artist_base::render_ligting_overlay_scaled( int x, int y, float scale_size)
    {

    }

    void artist_base::resize_ligting_overlay( int w, int h)
    {

    }

    void artist_base::switch_ligting_overlay(  bool on  )
    {

    }

}
