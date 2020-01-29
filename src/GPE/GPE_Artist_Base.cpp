/*
GPE_Artist_Base.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "GPE_Artist_Base.h"

GPE_Artist_Base * gcanvas  = NULL;

GPE_Artist_Base::GPE_Artist_Base()
{
    artistRenderer = NULL;

    defaultLineWidth = 1;
    tempCipRect = new GPE_Rect();
    //duplicates the color white
    currentColor = c_white->duplicate_color();
    tempColor = c_white->duplicate_color();
    currentAlpha = 255;

    lightingOverlayTexture = NULL;

}

GPE_Artist_Base::GPE_Artist_Base( GPE_Renderer_Base * aRenderer )
{
    artistRenderer = aRenderer;

    defaultLineWidth = 1;
    tempCipRect = new GPE_Rect();
    //duplicates the color white
    currentColor = c_white->duplicate_color();
    tempColor = c_white->duplicate_color();
    currentAlpha = 255;

    lightingOverlayTexture = NULL;
}

GPE_Artist_Base::~GPE_Artist_Base()
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

int GPE_Artist_Base::animation_get_number(GPE_Animation* animationIn)
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

int GPE_Artist_Base::get_artist_blend_mode()
{
    return currentArtistBlendMode;
}

void GPE_Artist_Base::render_animation_id(int animId,  int subImageToDraw, int xPos, int yPos, GPE_Rect *cam)
{
    GPE_Animation* animationToDraw = gcm->get_animation( animId );
    if(animationToDraw!=NULL)
    {
        animationToDraw->render(subImageToDraw,xPos,yPos, cam );
    }
}


void GPE_Artist_Base::render_animation_id_resized(int animId,  int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, GPE_Rect * cam)
{
    GPE_Animation* animationToDraw = gcm->get_animation( animId );
    if(animationToDraw!=NULL)
    {
        animationToDraw->render_resized(subImageToDraw,xPos,yPos,newWidth, newHeight, cam );
    }
}

void GPE_Artist_Base::render_animation_id_special(int animId, int subImageToDraw, int xPos, int yPos,  int newWidth, int newHeight,float newAngle,GPE_Color * rendColor, int alpha, GPE_Rect * cam)
{
    GPE_Animation* animationToDraw = gcm->get_animation( animId );
    if(animationToDraw!=NULL)
    {
        animationToDraw->render_special(subImageToDraw,xPos,yPos, newWidth, newHeight, newAngle,rendColor,alpha, cam );
    }
}

void GPE_Artist_Base::render_animation_id_rotated(int animId,  int subImageToDraw, int xPos, int yPos, float newAngle,int newWidth, int newHeight,  GPE_Rect *cam)
{
    GPE_Animation* animationToDraw = gcm->get_animation( animId );
    if(animationToDraw!=NULL)
    {
        animationToDraw->render_rotated( subImageToDraw, xPos, yPos, newAngle, newWidth, newHeight, cam );
    }
}

void GPE_Artist_Base::render_animation(GPE_Animation* animationToDraw, int subImageToDraw, int xPos, int yPos, GPE_Rect *cam)
{
    if(animationToDraw!=NULL)
    {
        animationToDraw->render(subImageToDraw,xPos,yPos, cam );
    }
}


void GPE_Artist_Base::render_animation_resized(GPE_Animation* animationToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, GPE_Rect * cam)
{
    if(animationToDraw!=NULL)
    {
        animationToDraw->render_resized(subImageToDraw,xPos,yPos,newWidth, newHeight, cam );
    }
}

void GPE_Artist_Base::render_animation_special(GPE_Animation* animationToDraw, int subImageToDraw, int xPos, int yPos,  int newWidth, int newHeight,float newAngle,GPE_Color * rendColor, int alpha, GPE_Rect * cam)
{
    if(animationToDraw!=NULL)
    {
        animationToDraw->render_special(subImageToDraw,xPos,yPos, newWidth, newHeight, newAngle,rendColor, alpha, cam );
    }
}

void GPE_Artist_Base::render_animation_rotated(GPE_Animation* animationToDraw, int subImageToDraw, int xPos, int yPos, float newAngle, int newWidth, int newHeight, GPE_Rect *cam)
{
    if( cam==NULL)
    {
        cam = &GPE_CAMERA_DEFAULT;
    }
    if(animationToDraw!=NULL)
    {
        animationToDraw->render_rotated( subImageToDraw, xPos, yPos, newAngle, newWidth, newHeight, cam );
    }
}



void GPE_Artist_Base::render_texture( int textureId, int x, int y,GPE_Rect* clip, int hAlign, int vAlign )
{

}

void GPE_Artist_Base::render_texture_resized( int textureId, int x, int y,int newWidth, int newHeight,GPE_Rect* clip, int hAlign, int vAlign, GPE_Color * renderColor )
{

}

void GPE_Artist_Base::render_texture_rotated( int textureId, int x, int y,float newAngle,int newWidth, int newHeight,GPE_Rect* clip )
{

}

//Circle and SemiCircles and Ovals Rendering Functions
 bool GPE_Artist_Base::render_circle( int x, int y, int rad, bool renderOutLine )
{
    return false;
}

bool GPE_Artist_Base::render_circle_color( int x, int y, int rad, GPE_Color * rendColor, int alphaChannel,bool renderOutLine )
{
     return false;
}

bool GPE_Artist_Base::render_oval( int x, int y, int w, int h, bool renderOutLine )
{
     return false;
}

bool GPE_Artist_Base::render_oval_color( int x, int y, int w, int h, GPE_Color *rendColor, int alphaChannel, bool renderOutLine)
{
    return false;
}

bool GPE_Artist_Base::render_semi_circle( int x, int y, int rad, int direction, bool renderOutLine)
{
    return false;
}

bool GPE_Artist_Base::render_semi_circle_color( int x, int y, int rad, int direction, GPE_Color *rendColor, int alphaChannel, bool renderOutLine)
{
    return false;
}


//Gradients Rendering [ BEGIN ]
void GPE_Artist_Base::render_gradient_circle(  int radius, GPE_Rect * rendRect , GPE_Color * rendColor )
{

}

void GPE_Artist_Base::render_gradient_horizontal(  GPE_Rect * rendRect, GPE_Color * rendColor )
{

}

void GPE_Artist_Base::render_gradient_vertical(  GPE_Rect * rendRect, GPE_Color * rendColor )
{

}

//Gradients Rendering [ END ]

void GPE_Artist_Base::render_ligting_overlay( int x, int y)
{

}

void GPE_Artist_Base::render_ligting_overlay_scaled( int x, int y, float scaleSize)
{

}

void GPE_Artist_Base::resize_ligting_overlay( int w, int h)
{

}

void GPE_Artist_Base::switch_ligting_overlay(  bool on  )
{

}

//Line Rendering [ BEGIN ]
void GPE_Artist_Base::render_horizontal_line(int y, int x1, int x2)
{

}

void GPE_Artist_Base::render_horizontal_line_color( int y, int x1, int x2, GPE_Color *rendColor, int alphaChannel )
{

}


void GPE_Artist_Base::render_line(  int x1, int y1, int x2, int y2 )
{

}

void GPE_Artist_Base::render_line_color(  int x1, int y1, int x2, int y2,  GPE_Color *rendColor, int alphaChannel )
{

}

void GPE_Artist_Base::render_line_capped(  int x1, int y1, int x2, int y2, int lineWidth )
{

}

void GPE_Artist_Base::render_line_capped_color(  int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel )
{

}

void GPE_Artist_Base::render_line_width(  int x1, int y1, int x2, int y2, int lineWidth)
{

}

void GPE_Artist_Base::render_line_width_color(  int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel )
{

}

void GPE_Artist_Base::render_vertical_line( int x, int y1, int y2)
{

}

void GPE_Artist_Base::render_vertical_line_color( int x, int y1, int y2, GPE_Color *rendColor, int alphaChannel )
{

}

//Line Rendering [ END ]

//Points Rendering [ BEGIN ]
void GPE_Artist_Base::render_point( int x, int y)
{

}

void GPE_Artist_Base::render_point_color( int x, int y, GPE_Color *rendColor, int alphaChannel )
{

}

//Points Rendering [ END ]

//Rectangles Rendering [ BEGIN ]
void GPE_Artist_Base::render_rect(  GPE_Rect * rendRect,GPE_Color * rendColor,bool outline, int alphaChannel )
{

}

void GPE_Artist_Base::render_rectangle( int x1, int y1, int x2, int y2,  GPE_Color *rendColor,bool outline , int alphaChannel )
{

}

void GPE_Artist_Base::render_square( int x, int y, int squareSize,  GPE_Color *rendColor,bool outline , int alphaChannel )
{

}

void GPE_Artist_Base::render_rotated_rectangle(int xCenter, int yCenter, int w, int h, int angle, GPE_Color * rendColor , int alphaChannel )
{

}

void GPE_Artist_Base::render_roundrect(int x1, int y1, int x2, int y2, int rad, bool outline )
{

}

void GPE_Artist_Base::render_roundrect_color( int x1, int y1,int x2, int y2, int rad, bool outline,GPE_Color * rendColor, int alphaChannel  )
{

}

//Rectangles Rendering [ END ]

//Triangle Rendering [ BEGIN ]
void GPE_Artist_Base::render_triangle( int x1, int y1, int x2, int y2, int x3, int y3, bool isOutlne, int lineWidth )
{

}

void GPE_Artist_Base::render_triangle_color( int x1, int y1, int x2, int y2, int x3, int y3, GPE_Color * rendColor , int alphaChannel , bool isOutlne, int lineWidth )
{

}

void GPE_Artist_Base::render_triangle_flatbottom( float x1, float y1, float x2, float y2, float x3, float y3, GPE_Color * rendColor , int alphaChannel )
{

}

void GPE_Artist_Base::render_triangle_flattop( float x1, float y1, float x2, float y2, float x3, float y3, GPE_Color * rendColor, int alphaChannel )
{

}

//Triangle Rendering [ END ]


void GPE_Artist_Base::set_color( GPE_Color * color)
{

}

void GPE_Artist_Base::set_color( int r, int g, int b )
{

}

void GPE_Artist_Base::set_render_alpha( int newAlpha )
{

}

void GPE_Artist_Base::set_line_width( int newWidth )
{

}

void GPE_Artist_Base::set_artist_blend_mode( int newBlendMode )
{

}
