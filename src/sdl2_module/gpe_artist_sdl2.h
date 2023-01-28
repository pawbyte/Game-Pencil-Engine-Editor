/*
gpe_artist_sdl2.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine  - SDL2 Module <https://gamepencil.net/sdl2-module/>


*/

#ifndef gpe_artist_sdl2_h
#define gpe_artist_sdl2_h

#include "../gpe/gpe_artist_base.h"
#include "../gpe/gpe_common_includes.h"
#include "../gpe/gpe_globals.h"
#include "../gpe/gpe_camera.h"
#include "../gpe/gpe_shared_resources.h"
#include "../gpe/internal_libs/semath.h"

#include "gpe_renderer_sdl2.h"
#include "thirdparty/sdl2_surface_ex.h"
#include "gpe_texture_sdl2.h"
#include "gpe_texture_target_sdl2.h"

namespace gpe
{
    const int render_points_giant_size = 6400000;
    //The Master class to render things to textures/the screen
    class artist_sdl2: public artist_base
    {
        protected:
            color * past_arc_color;
            bool first_arc_in_frame;
            int current_triangle;
            SDL_Renderer * sdlRenderer;
            renderer_system_sdl2 * gpeSDLRenderer;
            texture_sdl2 * prerenderedSquare;
            std::vector<texture_sdl2 * >prerenderedCircles;
            SDL_Texture * geometry_texture;
            shape_point2d triangle_midpoint;
            SDL_Point circle_render_points[ render_points_giant_size];
            SDL_Point line_render_points[ render_points_giant_size];
            SDL_FPoint line_render_fpoints[ render_points_giant_size];
            SDL_FRect rect_render_points[ render_points_giant_size];
            SDL_Vertex  geometry_render_points[ render_points_giant_size]; //Added for SDL_RenderGeometry from 2.0.18
            int line_render_point_position;
            int render_triangle_flatbottom( float x1, float y1, float x2, float y2, float x3, float y3 );
            int render_triangle_flattop( float x1, float y1, float x2, float y2, float x3, float y3 );

        public:
            artist_sdl2(renderer_system_sdl2 * aRenderer);
            ~artist_sdl2();

            void clear_render_points();

            //Frame based functions[START]
            virtual void begin_frame();
            virtual void end_frame();
            //Frame based functions[END]

            //Getters [START]
            virtual int primitive_supported( primitive_type primitive_to_check );
            //Getters [END]

            //Arc Rendering Functions
             virtual void render_arc( float arc_x, float arc_y, float arc_radius, float start_angle, float end_angle, float arc_vertices  );
             virtual void render_arc_color( float arc_x, float arc_y,float arc_radius, float start_angle, float end_angle, float arc_vertices,  color * render_color = NULL, int alpha_channel = 255   );

             virtual void render_arc_width( float arc_x, float arc_y, float arc_radius, float line_width, float start_angle, float end_angle, float arc_vertices  );
             virtual void render_arc_width_color( float arc_x, float arc_y,float arc_radius, float line_width, float start_angle, float end_angle, float arc_vertices,  color * render_color = NULL, int alpha_channel = 255 );

            //Bezier Curves Rendering [ END ]
            inline float ease_cubic_in_out(float t, float b, float c, float d);
            virtual void render_bezier_curve(  float x1, float y1, float x2, float y2 );
            virtual void render_bezier_curve_color(  float x1, float y1, float x2, float y2,  color *render_color, int alpha_channel = 255);
            virtual void render_bezier_curve_width_color(  float x1, float y1, float x2, float y2, int line_width, color *render_color, int alpha_channel = 255);

            virtual void render_bezier_curve_controlled(  shape_point2d point1, shape_point2d point2, shape_point2d control_point );
            virtual void render_bezier_curve_controlled_color(  shape_point2d point1, shape_point2d point2, shape_point2d control_point ,  color *render_color, int alpha_channel = 255);
            virtual void render_bezier_curve_controlled_width_color(  shape_point2d point1, shape_point2d point2, shape_point2d control_point , int line_width, color *render_color, int alpha_channel = 255);

            virtual void render_bezier_curve_quad(  shape_point2d point1, shape_point2d point2, shape_point2d control_point1, shape_point2d control_point2 );
            virtual void render_bezier_curve_quad_color(  shape_point2d point1, shape_point2d point2, shape_point2d control_point1, shape_point2d control_point2 ,  color *render_color, int alpha_channel = 255);
            virtual void render_bezier_curve_quad_width_color(  shape_point2d point1, shape_point2d point2, shape_point2d control_point1, shape_point2d control_point2 , int line_width, color *render_color, int alpha_channel = 255);
            //Bezier Curves  Rendering [ END ]

            //Circle and SemiCircles and Ovals Rendering Functions
             bool render_circle_filled( int x, int y, int rad);
             bool render_circle_filled_color( int x, int y, int rad, color * render_color = NULL, int alpha_channel = 255 );
             bool render_circle_outline(  int x, int y, int rad );
             bool render_circle_outline_color( int x, int y, int rad,  color * render_color = NULL, int alpha_channel = 255);

             bool render_oval( int x, int y, int w, int h, bool renderOutLine = false);

             bool render_oval_color( int x, int y, int w, int h, color *render_color, int alpha_channel = 255, bool renderOutLine = false);
             bool render_semi_circle( int x, int y, int rad, int direction = 0, bool renderOutLine = false);
             bool render_semi_circle_color( int x, int y, int rad, int direction = 0, color *render_color = NULL, int alpha_channel = 255, bool renderOutLine = false);

            //Gradients Rendering [ BEGIN ]
             void render_gradient_circle(  int radius, shape_rect * rendRect = NULL, color * render_color = NULL);
             void render_gradient_horizontal(  shape_rect * rendRect = NULL, color * render_color = NULL);
             void render_gradient_vertical(  shape_rect * rendRect = NULL, color * render_color = NULL);
            //Gradients Rendering [ END ]

            //Line Rendering [ BEGIN ]
             void render_horizontal_line(int y, int x1, int x2);
             void render_horizontal_line_color( int y, int x1, int x2, color *render_color, int alpha_channel = 255);

             void render_line(  int x1, int y1, int x2, int y2 );
             void render_line_color(  int x1, int y1, int x2, int y2,  color *render_color, int alpha_channel = 255);
             void render_line_capped(  int x1, int y1, int x2, int y2, int line_width );
             void render_line_capped_color(  int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel = 255);
             void render_line_width(  int x1, int y1, int x2, int y2, int line_width);
             void render_line_width_color(  int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel = 255);

             void render_vertical_line( int x, int y1, int y2);
             void render_vertical_line_color( int x, int y1, int y2, color *render_color, int alpha_channel = 255);
            //Line Rendering [ END ]

            //Points Rendering [ BEGIN ]
             void render_point( int x, int y);
             void render_point_color( int x, int y, color *render_color, int alpha_channel = 255);
             //Points Rendering [ END ]

            //Rectangles Rendering [ BEGIN ]
             void render_rect(  shape_rect * rendRect,color * render_color,bool outline = false, int alpha_channel = 255);
             void render_rectangle( int x1, int y1, int x2, int y2,  color *render_color,bool outline = false, int alpha_channel = 255);
             void render_square( int x, int y, int squareSize,  color *render_color,bool outline = false, int alpha_channel = 255);
             void render_rotated_rectangle(int get_center(), int yCenter, int w, int h, int angle, color * render_color = NULL, int alpha_channel = 255);
             //Rectangles Rendering [ END ]

             //Roundrects Rendering [ BEGIN ]
             void render_roundrect_filled(int x1, int y1, int x2, int y2);
             void render_roundrect_filled_radius(int x1, int y1, int x2, int y2, int rad);
             void render_roundrect_filled_color( int x1, int y1,int x2, int y2,color * render_color = NULL, int alpha_channel  = 255);
             void render_roundrect_filled_color_radius( int x1, int y1,int x2, int y2, int rad, color * render_color = NULL, int alpha_channel  = 255);

             void render_roundrect_outline(int x1, int y1, int x2, int y2);
             void render_roundrect_outline_radius(int x1, int y1, int x2, int y2, int rad);
             void render_roundrect_outline_color( int x1, int y1,int x2, int y2, color * render_color = NULL, int alpha_channel  = 255);
             void render_roundrect_outline_color_radius( int x1, int y1,int x2, int y2, int rad,color * render_color = NULL, int alpha_channel  = 255);
             //Rectangles Rendering [ END ]


             //Triangle Rendering [ BEGIN ]
             void render_triangle( shape_triangle2d * tri );
             void render_triangle_color( shape_triangle2d * tri, color * render_color = NULL, int alpha_channel = 255 );

             void render_triangle_coords( int x1, int y1, int x2, int y2, int x3, int y3);
             void render_triangle_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color = NULL, int alpha_channel = 255 );

             void render_triangle_outline( shape_triangle2d * tri, int line_width = 1);
             void render_triangle_outline_color( shape_triangle2d * tri, color * render_color = NULL, int alpha_channel = 255,int line_width = 1);

             void render_triangle_outline_coords( int x1, int y1, int x2, int y2, int x3, int y3, int line_width = 1);
             void render_triangle_outline_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color = NULL, int alpha_channel = 255,int line_width = 1);

             void render_triangle_gemoetry( shape_triangle2d * tri );
             void render_triangle_gemoetry_color( shape_triangle2d * tri, color * render_color = NULL, int alpha_channel = 255 );

             void render_triangle_gemoetry_coords( int x1, int y1, int x2, int y2, int x3, int y3);
             void render_triangle_gemoetry_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color = NULL, int alpha_channel = 255 );
            //Triangle Rendering [ END ]

            //Color / blend related functions
            void set_color( color * color);
            void set_color( int r, int g, int b );
            void set_render_alpha( int alpha_new = 255);
            void set_line_width( int new_width = 1);
            void set_artist_blend_mode( int blend_mode_new );
            //Render Mode Related Functions [ END ]

            //Other setters [ END ]
            bool enable_simple_geometry(); //enables simple geometry, defaulting triangles, circles to use SDL_RenderGeometry and enables other polygonal rendering(hexagons, octogons, etc)
            bool disable_simple_geometry();//disables simple geometry, defaulting to render pre-rendered circles, line based triangles and disables polygonal rendering(hexagons, octogons, etc)
            //Other setters [ END ]

            //Geometry rendering [ BEGIN]
            bool begin_geometric_shape();
            bool end_geometric_shape();
            //Geometry rendering [ END ]

            //Lighting Overlay [ BEGIN ]
            void render_ligting_overlay( int x, int y);
            void render_ligting_overlay_scaled( int x, int y, float scale_size);
            void resize_ligting_overlay( int w, int h);
            void switch_ligting_overlay(  bool on  = true );
            //Lighting Overlay [ END ]
    };
}

#endif //gpe_artist_sdl2_h
