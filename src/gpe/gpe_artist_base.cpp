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

#include "gpe_artist_base.h"

namespace gpe
{
    artist_base * gcanvas  = NULL;

    artist_base::artist_base()
    {
        artistRenderer = NULL;

        defaultLineWidth = 1;
        tempCipRect = new shape_rect();
        //duplicates the color white
        currentColor = c_white->duplicate_color();
        tempColor = c_white->duplicate_color();
        currentAlpha = 255;

        lightingOverlayTexture = NULL;
    }

    artist_base::artist_base( renderer_base * aRenderer )
    {
        artistRenderer = aRenderer;

        defaultLineWidth = 1;
        tempCipRect = new shape_rect();
        //duplicates the color white
        currentColor = c_white->duplicate_color();
        tempColor = c_white->duplicate_color();
        currentAlpha = 255;

        lightingOverlayTexture = NULL;
    }

    artist_base::~artist_base()
    {
        if( tempCipRect!=NULL )
        {
            delete tempCipRect;
            tempCipRect = NULL;
        }

        if( currentColor!=NULL )
        {
            delete currentColor;
            currentColor = NULL;
        }

        if( tempColor!=NULL )
        {
            delete tempColor;
            tempColor = NULL;
        }

        if( lightingOverlayTexture!=NULL )
        {
            delete lightingOverlayTexture;
            lightingOverlayTexture = NULL;
        }
    }

    int artist_base::animation_get_number(animaton2d* animationIn)
    {
        if(animationIn!=NULL)
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
        return currentArtistBlendMode;
    }

    void artist_base::render_animation_named( std::string resource_name,  int subImageToDraw, int xPos, int yPos, shape_rect *cam)
    {
        animaton2d* animationToDraw = rsm->get_animation( resource_name );
        if(animationToDraw!=NULL)
        {
            animationToDraw->render(subImageToDraw,xPos,yPos, cam );
        }
    }

    void artist_base::render_animation_named_resized( std::string resource_name,  int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, shape_rect * cam)
    {
        animaton2d * animationToDraw = rsm->get_animation( resource_name );
        if(animationToDraw!=NULL)
        {
            animationToDraw->render_resized(subImageToDraw,xPos,yPos,newWidth, newHeight, cam );
        }
    }

    void artist_base::render_animation_named_special( std::string resource_name, int subImageToDraw, int xPos, int yPos,  int newWidth, int newHeight,float newAngle,color * rendColor, int alpha, shape_rect * cam)
    {
        animaton2d * animationToDraw = rsm->get_animation( resource_name );
        if(animationToDraw!=NULL)
        {
            animationToDraw->render_special(subImageToDraw,xPos,yPos, newWidth, newHeight, newAngle,rendColor,alpha, cam );
        }
    }

    void artist_base::render_animation_named_rotated( std::string resource_name,  int subImageToDraw, int xPos, int yPos, float newAngle,int newWidth, int newHeight,  shape_rect *cam)
    {
        animaton2d * animationToDraw = rsm->get_animation( resource_name );
        if(animationToDraw!=NULL)
        {
            animationToDraw->render_rotated( subImageToDraw, xPos, yPos, newAngle, newWidth, newHeight, cam );
        }
    }

    void artist_base::render_animation(animaton2d* animationToDraw, int subImageToDraw, int xPos, int yPos, shape_rect *cam)
    {
        if(animationToDraw!=NULL)
        {
            animationToDraw->render(subImageToDraw,xPos,yPos, cam );
        }
    }


    void artist_base::render_animation_resized(animaton2d* animationToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, shape_rect * cam)
    {
        if(animationToDraw!=NULL)
        {
            animationToDraw->render_resized(subImageToDraw,xPos,yPos,newWidth, newHeight, cam );
        }
    }

    void artist_base::render_animation_special(animaton2d* animationToDraw, int subImageToDraw, int xPos, int yPos,  int newWidth, int newHeight,float newAngle,color * rendColor, int alpha, shape_rect * cam)
    {
        if(animationToDraw!=NULL)
        {
            animationToDraw->render_special(subImageToDraw,xPos,yPos, newWidth, newHeight, newAngle,rendColor, alpha, cam );
        }
    }

    void artist_base::render_animation_rotated(animaton2d* animationToDraw, int subImageToDraw, int xPos, int yPos, float newAngle, int newWidth, int newHeight, shape_rect *cam)
    {
        if( cam==NULL)
        {
            cam = camera_default;
        }
        if(animationToDraw!=NULL)
        {
            animationToDraw->render_rotated( subImageToDraw, xPos, yPos, newAngle, newWidth, newHeight, cam );
        }
    }



    void artist_base::render_texture_named( std::string resource_name, int x, int y, shape_rect* clip, int hAlign, int vAlign )
    {

    }

    void artist_base::render_texture_named_resized( std::string resource_name, int x, int y,int newWidth, int newHeight, shape_rect* clip, int hAlign, int vAlign, color * renderColor )
    {

    }

    void artist_base::render_texture_named_rotated( std::string resource_name, int x, int y,float newAngle,int newWidth, int newHeight, shape_rect* clip )
    {

    }

    //Circle and SemiCircles and Ovals Rendering Functions
     bool artist_base::render_circle( int x, int y, int rad, bool renderOutLine )
    {
        return false;
    }

    bool artist_base::render_circle_color( int x, int y, int rad, color * rendColor, int alphaChannel,bool renderOutLine )
    {
         return false;
    }

    bool artist_base::render_oval( int x, int y, int w, int h, bool renderOutLine )
    {
         return false;
    }

    bool artist_base::render_oval_color( int x, int y, int w, int h, color *rendColor, int alphaChannel, bool renderOutLine)
    {
        return false;
    }

    bool artist_base::render_semi_circle( int x, int y, int rad, int direction, bool renderOutLine)
    {
        return false;
    }

    bool artist_base::render_semi_circle_color( int x, int y, int rad, int direction, color *rendColor, int alphaChannel, bool renderOutLine)
    {
        return false;
    }


    //Gradients Rendering [ BEGIN ]
    void artist_base::render_gradient_circle(  int radius, shape_rect * rendRect , color * rendColor )
    {

    }

    void artist_base::render_gradient_horizontal(  shape_rect * rendRect, color * rendColor )
    {

    }

    void artist_base::render_gradient_vertical(  shape_rect * rendRect, color * rendColor )
    {

    }

    //Gradients Rendering [ END ]

    //Line Rendering [ BEGIN ]
    void artist_base::render_horizontal_line(int y, int x1, int x2)
    {

    }

    void artist_base::render_horizontal_line_color( int y, int x1, int x2, color *rendColor, int alphaChannel )
    {

    }


    void artist_base::render_line(  int x1, int y1, int x2, int y2 )
    {

    }

    void artist_base::render_line_color(  int x1, int y1, int x2, int y2,  color *rendColor, int alphaChannel )
    {

    }

    void artist_base::render_line_capped(  int x1, int y1, int x2, int y2, int lineWidth )
    {

    }

    void artist_base::render_line_capped_color(  int x1, int y1, int x2, int y2, int lineWidth, color *rendColor, int alphaChannel )
    {

    }

    void artist_base::render_line_width(  int x1, int y1, int x2, int y2, int lineWidth)
    {

    }

    void artist_base::render_line_width_color(  int x1, int y1, int x2, int y2, int lineWidth, color *rendColor, int alphaChannel )
    {

    }

    void artist_base::render_vertical_line( int x, int y1, int y2)
    {

    }

    void artist_base::render_vertical_line_color( int x, int y1, int y2, color *rendColor, int alphaChannel )
    {

    }

    //Line Rendering [ END ]

    //Points Rendering [ BEGIN ]
    void artist_base::render_point( int x, int y)
    {

    }

    void artist_base::render_point_color( int x, int y, color *rendColor, int alphaChannel )
    {

    }

    //Points Rendering [ END ]

    //Rectangles Rendering [ BEGIN ]
    void artist_base::render_rect(  shape_rect * rendRect,color * rendColor,bool outline, int alphaChannel )
    {

    }

    void artist_base::render_rectangle( int x1, int y1, int x2, int y2,  color *rendColor,bool outline , int alphaChannel )
    {

    }

    void artist_base::render_square( int x, int y, int squareSize,  color *rendColor,bool outline , int alphaChannel )
    {

    }

    void artist_base::render_rotated_rectangle(int xCenter, int yCenter, int w, int h, int angle, color * rendColor , int alphaChannel )
    {

    }

    void artist_base::render_roundrect(int x1, int y1, int x2, int y2, int rad, bool outline )
    {

    }

    void artist_base::render_roundrect_color( int x1, int y1,int x2, int y2, int rad, bool outline,color * rendColor, int alphaChannel  )
    {

    }

    //Rectangles Rendering [ END ]

    //Triangle Rendering [ BEGIN ]
    void artist_base::render_triangle( int x1, int y1, int x2, int y2, int x3, int y3, bool isOutlne, int lineWidth )
    {

    }

    void artist_base::render_triangle_color( int x1, int y1, int x2, int y2, int x3, int y3, color * rendColor , int alphaChannel , bool isOutlne, int lineWidth )
    {

    }

    void artist_base::render_triangle_flatbottom( float x1, float y1, float x2, float y2, float x3, float y3, color * rendColor , int alphaChannel )
    {

    }

    void artist_base::render_triangle_flattop( float x1, float y1, float x2, float y2, float x3, float y3, color * rendColor, int alphaChannel )
    {

    }

    //Triangle Rendering [ END ]


    void artist_base::set_color( color * color)
    {

    }

    void artist_base::set_color( int r, int g, int b )
    {

    }

    void artist_base::set_render_alpha( int newAlpha )
    {

    }

    void artist_base::set_line_width( int newWidth )
    {

    }

    void artist_base::set_artist_blend_mode( int newBlendMode )
    {

    }


    void artist_base::render_ligting_overlay( int x, int y)
    {

    }

    void artist_base::render_ligting_overlay_scaled( int x, int y, float scaleSize)
    {

    }

    void artist_base::resize_ligting_overlay( int w, int h)
    {

    }

    void artist_base::switch_ligting_overlay(  bool on  )
    {

    }

}
