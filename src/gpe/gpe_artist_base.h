/*
gpe_artist_base.h
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
            renderer_base * artistRenderer;
            shape_rect * tempCipRect;
            int currentArtistBlendMode;
            color * currentColor;
            color * tempColor;
            int currentAlpha;
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
            void render_animation_named( std::string resource_name, int subImageToDraw, int xPos, int yPos,  shape_rect * cam= NULL);
            void render_animation_named_resized( std::string resource_name, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, shape_rect * cam = NULL);
            void render_animation_named_special( std::string resource_name, int subImageToDraw, int xPos, int yPos,  int newWidth, int newHeight,float newAngle,color * renderColor, int alpha = 255, shape_rect * cam= NULL);
            void render_animation_named_rotated( std::string resource_name, int subImageToDraw, int xPos, int yPos,  float newAngle,int newWidth, int newHeight, shape_rect * cam= NULL);

            void render_animation(animaton2d* animationToDraw, int subImageToDraw, int xPos, int yPos,  shape_rect * cam= NULL);
            void render_animation_resized(animaton2d* animationToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, shape_rect * cam = NULL);
            void render_animation_special(animaton2d* animationToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,float newAngle,color * renderColor, int alpha = 255, shape_rect * cam= NULL);
            void render_animation_rotated(animaton2d* animationToDraw, int subImageToDraw, int xPos, int yPos, float newAngle, int newWidth, int newHeight,  shape_rect * cam= NULL);

            //Animation/animation  Rendering [ END ]

            //Texture Rendering Functions [ BEGIN ]
            void render_texture_named( std::string resource_name, int x, int y, shape_rect* clip = NULL, int hAlign = -1, int vAlign = -1 );
            void render_texture_named_resized( std::string resource_name, int x, int y,int newWidth, int newHeight, shape_rect* clip = NULL, int hAlign = -1, int vAlign = -1, color * renderColor=NULL );
            void render_texture_named_rotated( std::string resource_name, int x, int y,float newAngle,int newWidth, int newHeight, shape_rect* clip = NULL );
            //Texture Rendering Functions[ END ]

            //Circle and SemiCircles and Ovals Rendering Functions
            virtual bool render_circle( int x, int y, int rad, bool renderOutLine = false);
            virtual bool render_circle_color( int x, int y, int rad, color * rendColor = NULL, int alphaChannel = 255,bool renderOutLine = false);

            virtual bool render_oval( int x, int y, int w, int h, bool renderOutLine = false);

            virtual bool render_oval_color( int x, int y, int w, int h, color *rendColor, int alphaChannel = 255, bool renderOutLine = false);
            virtual bool render_semi_circle( int x, int y, int rad, int direction = 0, bool renderOutLine = false);
            virtual bool render_semi_circle_color( int x, int y, int rad, int direction = 0, color *rendColor = NULL, int alphaChannel = 255, bool renderOutLine = false);


            //Gradients Rendering [ BEGIN ]
            virtual void render_gradient_circle(  int radius, shape_rect * rendRect = NULL, color * rendColor = NULL);
            virtual void render_gradient_horizontal(  shape_rect * rendRect = NULL, color * rendColor = NULL);
            virtual void render_gradient_vertical(  shape_rect * rendRect = NULL, color * rendColor = NULL);
            //Gradients Rendering [ END ]

            //Line Rendering [ BEGIN ]
            virtual void render_horizontal_line(int y, int x1, int x2);
            virtual void render_horizontal_line_color( int y, int x1, int x2, color *rendColor, int alphaChannel = 255);

            virtual void render_line(  int x1, int y1, int x2, int y2 );
            virtual void render_line_color(  int x1, int y1, int x2, int y2,  color *rendColor, int alphaChannel = 255);
            virtual void render_line_capped(  int x1, int y1, int x2, int y2, int lineWidth );
            virtual void render_line_capped_color(  int x1, int y1, int x2, int y2, int lineWidth, color *rendColor, int alphaChannel = 255);
            virtual void render_line_width(  int x1, int y1, int x2, int y2, int lineWidth);
            virtual void render_line_width_color(  int x1, int y1, int x2, int y2, int lineWidth, color *rendColor, int alphaChannel = 255);

            virtual void render_vertical_line( int x, int y1, int y2);
            virtual void render_vertical_line_color( int x, int y1, int y2, color *rendColor, int alphaChannel = 255);
            //Line Rendering [ END ]

            //Points Rendering [ BEGIN ]
            virtual void render_point( int x, int y);
            virtual void render_point_color( int x, int y, color *rendColor, int alphaChannel = 255);
             //Points Rendering [ END ]

            //Rectangles Rendering [ BEGIN ]
            virtual void render_rect(  shape_rect * rendRect,color * rendColor,bool outline = false, int alphaChannel = 255);
            virtual void render_rectangle( int x1, int y1, int x2, int y2,  color *rendColor,bool outline = false, int alphaChannel = 255);
            virtual void render_square( int x, int y, int squareSize,  color *rendColor,bool outline = false, int alphaChannel = 255);
            virtual void render_rotated_rectangle(int get_center(), int yCenter, int w, int h, int angle, color * rendColor = NULL, int alphaChannel = 255);
            virtual void render_roundrect(int x1, int y1, int x2, int y2, int rad, bool outline = false);
            virtual void render_roundrect_color( int x1, int y1,int x2, int y2, int rad, bool outline = false,color * rendColor = NULL, int alphaChannel  = 255);
            //Rectangles Rendering [ END ]

            //Triangle Rendering [ BEGIN ]
            virtual void render_triangle( shape_triangle2d * tri );

            virtual void render_triangle_color( shape_triangle2d * tri, color * rendColor = NULL, int alphaChannel = 255 );

            virtual void render_triangle_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * rendColor = NULL, int alphaChannel = 255 );
            virtual void render_triangle_coords( int x1, int y1, int x2, int y2, int x3, int y3);

            virtual void render_triangle_outline( shape_triangle2d * tri, int lineWidth = 1);
            virtual void render_triangle_outline_coords( int x1, int y1, int x2, int y2, int x3, int y3, int lineWidth = 1);
            virtual void render_triangle_outline_color( shape_triangle2d * tri, color * rendColor = NULL, int alphaChannel = 255,int lineWidth = 1);
            virtual void render_triangle_outline_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * rendColor = NULL, int alphaChannel = 255,int lineWidth = 1);
            //Triangle Rendering [ END ]

            //Color / blend related functions
            virtual void set_color( color * color);
            virtual void set_color( int r, int g, int b );
            virtual void set_render_alpha( int newAlpha = 255);
            virtual void set_line_width( int newWidth = 1);
            virtual void set_artist_blend_mode( int newBlendMode );
            //Render Mode Related Functions [ END ]

            //Lighting Overlay [ BEGIN ]
            virtual void render_ligting_overlay( int x, int y);
            virtual void render_ligting_overlay_scaled( int x, int y, float scaleSize);
            virtual void resize_ligting_overlay( int w, int h);
            virtual void switch_ligting_overlay(  bool on  = true );
            //Lighting Overlay [ END ]

    };

    extern artist_base * gcanvas; //game pencil artist?
}

#endif //artist_base_h
