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

#ifndef gpe_artist_base_h
#define gpe_artist_base_h

#include "gpe_camera.h"
#include "gpe_common_includes.h"
#include "gpe_globals.h"
#include "gpe_renderer_base.h"
#include "gpe_shared_resources.h"
#include "gpe_texture_target_base.h"

namespace gpe
{
    //The Master class to render things to textures/the screen
    class artist_base
    {
        protected:
            renderer_base * artist_renderer;
            shape_rect * temp_clip_rect;
            int blend_current_mode;
            color * color_current;
            color * color_temp;
            int alpha_current;
            int defaultLineWidth;
            texture_target_base * lightingOverlayTexture;
        public:

            artist_base();
            artist_base( renderer_base * aRenderer );
            virtual ~artist_base();

            //Getters [START]
            int animation_get_number(animaton2d* animationIn);
            int get_artist_blend_mode();
            //Getters [ END ]

            //Animation/animation Rendering Functions [ BEGIN ]
            void render_animation_named( std::string resource_name, int sub_image_to_draw, int x_pos, int y_pos,  shape_rect * cam= NULL);
            void render_animation_named_resized( std::string resource_name, int sub_image_to_draw, int x_pos, int y_pos, int new_width, int new_height, shape_rect * cam = NULL);
            void render_animation_named_special( std::string resource_name, int sub_image_to_draw, int x_pos, int y_pos,  int new_width, int new_height,float new_angle,color * render_color, int alpha = 255, shape_rect * cam= NULL);
            void render_animation_named_rotated( std::string resource_name, int sub_image_to_draw, int x_pos, int y_pos,  float new_angle,int new_width, int new_height, shape_rect * cam= NULL);

            void render_animation(animaton2d* animationToDraw, int sub_image_to_draw, int x_pos, int y_pos,  shape_rect * cam= NULL);
            void render_animation_resized(animaton2d* animationToDraw, int sub_image_to_draw, int x_pos, int y_pos, int new_width, int new_height, shape_rect * cam = NULL);
            void render_animation_special(animaton2d* animationToDraw, int sub_image_to_draw, int x_pos, int y_pos, int new_width, int new_height,float new_angle,color * render_color, int alpha = 255, shape_rect * cam= NULL);
            void render_animation_rotated(animaton2d* animationToDraw, int sub_image_to_draw, int x_pos, int y_pos, float new_angle, int new_width, int new_height,  shape_rect * cam= NULL);

            //Animation/animation  Rendering [ END ]

            //Texture Rendering Functions [ BEGIN ]
            void render_texture_named( std::string resource_name, int x, int y, shape_rect* clip = NULL, int alignment_h = -1, int alignment_v = -1 );
            void render_texture_named_resized( std::string resource_name, int x, int y,int new_width, int new_height, shape_rect* clip = NULL, int alignment_h = -1, int alignment_v = -1, color * render_color=NULL );
            void render_texture_named_rotated( std::string resource_name, int x, int y,float new_angle,int new_width, int new_height, shape_rect* clip = NULL );
            //Texture Rendering Functions[ END ]

            virtual void render_arc( float arc_x, float arc_y, float arc_radius, float start_angle, float end_angle, float arc_vertices );
            virtual void render_arc_color( float arc_x, float arc_y,float arc_radius, float start_angle, float end_angle, float arc_vertices,  color * render_color = NULL, int alpha_channel = 255   );

            virtual void render_arc_width( float arc_x, float arc_y, float arc_radius, int line_width, float start_angle, float end_angle, float arc_vertices);
            virtual void render_arc_width_color( float arc_x, float arc_y,float arc_radius, int line_width, float start_angle, float end_angle, float arc_vertices,  color * render_color = NULL, int alpha_channel = 255 );

            //Circle and SemiCircles and Ovals Rendering Functions
            virtual bool render_circle_filled( int x, int y, int rad );
            virtual bool render_circle_filled_color( int x, int y, int rad, color * render_color = NULL, int alpha_channel = 255 );

            virtual bool render_circle_outline( int x, int y, int rad);
            virtual bool render_circle_outline_color( int x, int y, int rad, color * render_color = NULL, int alpha_channel = 255);

            virtual bool render_oval( int x, int y, int w, int h, bool renderOutLine = false);

            virtual bool render_oval_color( int x, int y, int w, int h, color *render_color, int alpha_channel = 255, bool renderOutLine = false);
            virtual bool render_semi_circle( int x, int y, int rad, int direction = 0, bool renderOutLine = false);
            virtual bool render_semi_circle_color( int x, int y, int rad, int direction = 0, color *render_color = NULL, int alpha_channel = 255, bool renderOutLine = false);


            //Gradients Rendering [ BEGIN ]
            virtual void render_gradient_circle(  int radius, shape_rect * rendRect = NULL, color * render_color = NULL);
            virtual void render_gradient_horizontal(  shape_rect * rendRect = NULL, color * render_color = NULL);
            virtual void render_gradient_vertical(  shape_rect * rendRect = NULL, color * render_color = NULL);
            //Gradients Rendering [ END ]

            //Line Rendering [ BEGIN ]
            virtual void render_horizontal_line(int y, int x1, int x2);
            virtual void render_horizontal_line_color( int y, int x1, int x2, color *render_color, int alpha_channel = 255);

            virtual void render_line(  int x1, int y1, int x2, int y2 );
            virtual void render_line_color(  int x1, int y1, int x2, int y2,  color *render_color, int alpha_channel = 255);
            virtual void render_line_capped(  int x1, int y1, int x2, int y2, int line_width );
            virtual void render_line_capped_color(  int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel = 255);
            virtual void render_line_width(  int x1, int y1, int x2, int y2, int line_width);
            virtual void render_line_width_color(  int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel = 255);

            virtual void render_vertical_line( int x, int y1, int y2);
            virtual void render_vertical_line_color( int x, int y1, int y2, color *render_color, int alpha_channel = 255);
            //Line Rendering [ END ]

            //Points Rendering [ BEGIN ]
            virtual void render_point( int x, int y);
            virtual void render_point_color( int x, int y, color *render_color, int alpha_channel = 255);
             //Points Rendering [ END ]

            //Rectangles Rendering [ BEGIN ]
            virtual void render_rect(  shape_rect * rendRect,color * render_color,bool outline = false, int alpha_channel = 255);
            virtual void render_rectangle( int x1, int y1, int x2, int y2,  color *render_color,bool outline = false, int alpha_channel = 255);
            virtual void render_square( int x, int y, int squareSize,  color *render_color,bool outline = false, int alpha_channel = 255);
            virtual void render_rotated_rectangle(int get_center(), int yCenter, int w, int h, int angle, color * render_color = NULL, int alpha_channel = 255);

            //Rectangles Rendering [ END ]


            virtual void render_roundrect_filled(int x1, int y1, int x2, int y2);
            virtual void render_roundrect_filled_radius(int x1, int y1, int x2, int y2, int rad);
            virtual void render_roundrect_filled_color( int x1, int y1,int x2, int y2,color * render_color = NULL, int alpha_channel  = 255);
            virtual void render_roundrect_filled_color_radius( int x1, int y1,int x2, int y2, int rad, color * render_color = NULL, int alpha_channel  = 255);

            virtual void render_roundrect_outline(int x1, int y1, int x2, int y2);
            virtual void render_roundrect_outline_radius(int x1, int y1, int x2, int y2, int rad);
            virtual void render_roundrect_outline_color( int x1, int y1,int x2, int y2, color * render_color = NULL, int alpha_channel  = 255);
            virtual void render_roundrect_outline_color_radius( int x1, int y1,int x2, int y2, int rad,color * render_color = NULL, int alpha_channel  = 255);
            //Roundrect Rendering [ END ]

            //Triangle Rendering [ BEGIN ]
            virtual void render_triangle( shape_triangle2d * tri );

            virtual void render_triangle_color( shape_triangle2d * tri, color * render_color = NULL, int alpha_channel = 255 );

            virtual void render_triangle_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color = NULL, int alpha_channel = 255 );
            virtual void render_triangle_coords( int x1, int y1, int x2, int y2, int x3, int y3);

            virtual void render_triangle_outline( shape_triangle2d * tri, int line_width = 1);
            virtual void render_triangle_outline_coords( int x1, int y1, int x2, int y2, int x3, int y3, int line_width = 1);
            virtual void render_triangle_outline_color( shape_triangle2d * tri, color * render_color = NULL, int alpha_channel = 255,int line_width = 1);
            virtual void render_triangle_outline_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color = NULL, int alpha_channel = 255,int line_width = 1);
            //Triangle Rendering [ END ]

            //Color / blend related functions
            virtual void set_color( color * color);
            virtual void set_color( int r, int g, int b );
            virtual void set_render_alpha( int alpha_new = 255);
            virtual void set_line_width( int new_width = 1);
            virtual void set_artist_blend_mode( int blend_mode_new );
            //Render Mode Related Functions [ END ]

            //Lighting Overlay [ BEGIN ]
            virtual void render_ligting_overlay( int x, int y);
            virtual void render_ligting_overlay_scaled( int x, int y, float scale_size);
            virtual void resize_ligting_overlay( int w, int h);
            virtual void switch_ligting_overlay(  bool on  = true );
            //Lighting Overlay [ END ]

    };

    extern artist_base * gcanvas; //game pencil artist?
}

#endif //artist_base_h
