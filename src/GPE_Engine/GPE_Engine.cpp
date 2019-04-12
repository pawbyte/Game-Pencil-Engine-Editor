/*
GPE_Engine.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the ԓoftwareԩ, to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ԁS ISԬ WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "GPE_Engine.h"
bool GPE_IS_LOADING = false;
int GPE_MAX_OBJECT_TYPES = 0;

GPE_Artist * gcanvas = NULL;
GPE_Artist * GCANVAS = NULL;
GPE_Runtime * gpe = NULL;
GPE_Runtime * GPE = NULL;

void limit_space_to_rect(GPE_Rect * rectIn,int * limitedX, int * limitedY)
{
    if( rectIn!=NULL && limitedX!=NULL && limitedY!=NULL)
    {
        if( *limitedX < rectIn->x)
        {
            *limitedX = 0;
        }

        if( *limitedX < rectIn->x+rectIn->w)
        {
            *limitedX = 0;
        }

        if( *limitedY < rectIn->y)
        {
            *limitedY = 0;
        }

        if( *limitedY < rectIn->y+rectIn->h)
        {
            *limitedY = 0;
        }
    }
}



//GPE Render calls [ Begin ]
GPE_Artist::GPE_Artist()
{
    defaultLineWidth = 1;
    tempCipRect = new GPE_Rect();
    //duplicates the color white
    currentColor = c_white->duplicate_color();
    tempColor = c_white->duplicate_color();
    currentAlpha = 255;

    lightingOverlayTexture = new GPE_TargetTexture();
    for( int i = 0; i < GPE_ARTIST_PRERENDER_SHAPE_COUNT; i++)
    {
        prerenderedCircles[i] = new GPE_Texture();
        prerenderedCirclesOutlines[i] = new GPE_Texture();
    }
    prerenderedCircles[0]->prerender_circle(8,c_white );
    prerenderedCircles[1]->prerender_circle(16,c_white );
    prerenderedCircles[2]->prerender_circle(32,c_white );
    prerenderedCircles[3]->prerender_circle(64,c_white );
    prerenderedCircles[4]->prerender_circle(96,c_white );
    prerenderedCircles[5]->prerender_circle(128,c_white );
    prerenderedCircles[6]->prerender_circle(256,c_white );
    prerenderedCircles[7]->prerender_circle(384,c_white );
    prerenderedCircles[8]->prerender_circle(512,c_white );
    prerenderedCircles[9]->prerender_circle(1024,c_white );
    prerenderedSquare = new GPE_Texture();
    prerenderedSquare->prerender_rectangle(16,16, c_white );
}

GPE_Artist::~GPE_Artist()
{
    if( lightingOverlayTexture!=NULL )
    {
        delete lightingOverlayTexture;
        lightingOverlayTexture = NULL;
    }
}

int GPE_Artist::get_artist_blend_mode()
{
    return currentArtistBlendMode;
}

void GPE_Artist::render_animation(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, GPE_Rect *cam)
{
    if(spriteToDraw!=NULL)
    {
        spriteToDraw->render(subImageToDraw,xPos,yPos, cam );
    }
}


void GPE_Artist::render_animation_resized(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, GPE_Rect * cam)
{
    if(spriteToDraw!=NULL)
    {
        spriteToDraw->render_resized(subImageToDraw,xPos,yPos,newWidth, newHeight, cam );
    }
}

void GPE_Artist::render_animation_special(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos,  int newWidth, int newHeight,double newAngle,GPE_Color * rendColor, int alpha, GPE_Rect * cam)
{
    if(spriteToDraw!=NULL)
    {
        spriteToDraw->render_special(subImageToDraw,xPos,yPos, newWidth, newHeight, newAngle,rendColor, alpha, cam );
    }
}

void GPE_Artist::render_animation_rotated(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, double newAngle, int newWidth, int newHeight, GPE_Rect *cam)
{
    if( cam==NULL)
    {
        cam = &GPE_DEFAULT_CAMERA;
    }
    if(spriteToDraw!=NULL)
    {
        spriteToDraw->render_rotated( subImageToDraw, xPos, yPos, newAngle, newWidth, newHeight, cam );
    }
}


void GPE_Artist::render_animation(int animId,  int subImageToDraw, int xPos, int yPos, GPE_Rect *cam)
{
    GPE_Animation* spriteToDraw = gcm->get_sprite( animId );
    if(spriteToDraw!=NULL)
    {
        spriteToDraw->render(subImageToDraw,xPos,yPos, cam );
    }
}


void GPE_Artist::render_animation_resized(int animId,  int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, GPE_Rect * cam)
{
    GPE_Animation* spriteToDraw = gcm->get_sprite( animId );
    if(spriteToDraw!=NULL)
    {
        spriteToDraw->render_resized(subImageToDraw,xPos,yPos,newWidth, newHeight, cam );
    }
}

void GPE_Artist::render_animation_special(int animId, int subImageToDraw, int xPos, int yPos,  int newWidth, int newHeight,double newAngle,GPE_Color * rendColor, int alpha, GPE_Rect * cam)
{
    GPE_Animation* spriteToDraw = gcm->get_sprite( animId );
    if(spriteToDraw!=NULL)
    {
        spriteToDraw->render_special(subImageToDraw,xPos,yPos, newWidth, newHeight, newAngle,rendColor,alpha, cam );
    }
}

void GPE_Artist::render_animation_rotated(int animId,  int subImageToDraw, int xPos, int yPos, double newAngle,int newWidth, int newHeight,  GPE_Rect *cam)
{
    GPE_Animation* spriteToDraw = gcm->get_sprite( animId );
    if(spriteToDraw!=NULL)
    {
        spriteToDraw->render_rotated( subImageToDraw, xPos, yPos, newAngle, newWidth, newHeight, cam );
    }
}

int GPE_Artist::sprite_get_number(GPE_Animation* spriteIn)
{
    if(spriteIn!=NULL)
    {
        return spriteIn->get_frame_count();
    }
    else
    {
        return -1;
    }
}

bool GPE_Artist::render_circle( int x, int y, int rad, bool renderOutLine )
{
    render_circle_color(x,y,rad, currentColor, currentAlpha, renderOutLine);
}

bool GPE_Artist::render_circle_color( int x, int y, int rad, GPE_Color *rendColor, int alphaChannel, bool renderOutLine )
{
    if( CURRENT_RENDERER!=NULL && rad > 0 )
    {
        GPE_Texture * tempCircleTexture = NULL;
        int circleId = -1;
        if( rad >= 96)
        {
            if( rad >= 384 )
            {
                if( rad > 1024)
                {
                    //1024 and up
                    circleId = 9;
                }
                else if( rad > 512)
                {
                    //512->1024
                   circleId = 8;
                }
                else
                {
                    //384->512
                    circleId = 7;
                }
            }
            else if( rad > 256)
            {
                //256->384
                circleId = 6;
            }
            else if( rad >= 128)
            {
                // 128-> 256
                circleId = 5;
            }
            else
            {
                //96->128
                circleId = 4;
            }
        }
        else
        {
            if( rad > 16 )
            {
                if( rad > 64)
                {
                    //64 -> 96
                    circleId = 3;
                }
                else
                {
                    //16 -> 32
                    circleId = 2;
                }
            }
            else
            {
                if( rad > 8)
                {
                    circleId = 1;
                }
                else
                {
                    circleId = 0;
                }
            }
        }

        if( circleId >=0 && circleId < GPE_ARTIST_PRERENDER_SHAPE_COUNT)
        {
            if( renderOutLine )
            {
                tempCircleTexture = prerenderedCirclesOutlines[circleId];
            }
            else
            {
                tempCircleTexture = prerenderedCircles[circleId];
            }
        }

        if( tempCircleTexture!=NULL )
        {
            tempCircleTexture->set_blend_mode( CURRENT_RENDERER->get_blend_mode() );
            tempCircleTexture->render_tex_resized(x-rad, y-rad, rad*2, rad*2, NULL,rendColor, alphaChannel );
        }
    }
}

bool GPE_Artist::render_oval( int x, int y, int w, int h,bool renderOutLine )
{

}

bool GPE_Artist::render_oval_color( int x, int y, int w, int h, GPE_Color *rendColor, int alphaChannel, bool renderOutLine )
{

}

bool GPE_Artist::render_semi_circle( int x, int y, int rad, int direction, bool renderOutLine)
{
    render_semi_circle_color( x,y,rad, direction, currentColor, currentAlpha, renderOutLine );
}

bool GPE_Artist::render_semi_circle_color( int x, int y, int rad, int direction, GPE_Color *rendColor, int alphaChannel, bool renderOutLine )
{
    if( CURRENT_RENDERER!=NULL && rad > 0 )
    {
        GPE_Texture * tempCircleTexture = NULL;
        int circleId = -1;

        //The temp size of the clip
        tempCipRect->y = 0;
        if( rad >= 96)
        {
            if( rad >= 384 )
            {
                if( rad > 1024)
                {
                    //1024 and up
                    circleId = 9;
                }
                else if( rad > 512)
                {
                    //512->1024
                   circleId = 8;
                }
                else
                {
                    //384->512
                    circleId = 7;
                }
            }
            else if( rad > 256)
            {
                //256->384
                circleId = 6;
            }
            else if( rad >= 128)
            {
                // 128-> 256
                circleId = 5;
            }
            else
            {
                //96->128
                circleId = 4;
            }
        }
        else
        {
            if( rad > 16 )
            {
                if( rad > 64)
                {
                    //64 -> 96
                    circleId = 3;
                }
                else
                {
                    //16 -> 32
                    circleId = 2;
                }
            }
            else
            {
                if( rad > 8)
                {
                    circleId = 1;
                }
                else
                {
                    circleId = 0;
                }
            }
        }



        if( circleId >=0 && circleId < GPE_ARTIST_PRERENDER_SHAPE_COUNT)
        {
            if( renderOutLine )
            {
                tempCircleTexture = prerenderedCirclesOutlines[circleId];
            }
            else
            {
                tempCircleTexture = prerenderedCircles[circleId];
            }
        }

        if( tempCircleTexture!=NULL )
        {
            tempCipRect->x = tempCipRect->w = tempCircleTexture->get_width()/2;
            tempCipRect->y = 0;
            tempCipRect->h = tempCircleTexture->get_height();

            tempCircleTexture->set_blend_mode( CURRENT_RENDERER->get_blend_mode() );
            /*
            x  = x - gpe->lengthdir_x( tempCipRect->x, direction );
            y  = y - gpe->lengthdir_y( tempCipRect->x, direction );
            */
            tempCircleTexture->render_tex_special_at_point(x, y, direction,FA_CENTER,FA_MIDDLE,rad, rad*2,rendColor, tempCipRect, alphaChannel );
        }
    }
}


void GPE_Artist::render_gradient_circle(  int radius, GPE_Rect * rendRect , GPE_Color * rendColor )
{

}

void GPE_Artist::render_gradient_vertical(GPE_Rect * rendRect, GPE_Color * rendColor)
{
    if( CURRENT_RENDERER==NULL)
    {
        CURRENT_RENDERER = MAIN_RENDERER;
    }
    if( rendRect!=NULL && rendColor!=NULL && CURRENT_RENDERER!=NULL)
    {
        double colorShadeDivision = 0;
        if( rendRect->w > 0 && rendRect->h > 0)
        {
            GPE_Color * colorShadeTempColor = new GPE_Color("color1",0,0,0);

            for( int i = 0; i <= rendRect->h; i++)
            {
                colorShadeDivision = (double)i/(double) rendRect->h;
                colorShadeTempColor->change_r( merge_channel(rendColor->get_r(),(double)( -colorShadeDivision+1.f)*255.f,(double) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_g( merge_channel(rendColor->get_g(),(double)( -colorShadeDivision+1.f)*255.f,(double) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_b( merge_channel(rendColor->get_b(),(double)( -colorShadeDivision+1.f)*255.f,(double) fabs( colorShadeDivision-0.5f)+0.5 ) );

                gcanvas->render_horizontal_line_color(rendRect->y+i,rendRect->x,rendRect->x+rendRect->w,colorShadeTempColor);
            }
            delete colorShadeTempColor;
            colorShadeTempColor = NULL;
        }
    }
}

void GPE_Artist::render_gradient_horizontal( GPE_Rect * rendRect, GPE_Color * rendColor)
{
    if( CURRENT_RENDERER==NULL)
    {
        CURRENT_RENDERER = MAIN_RENDERER;
    }
    if( rendRect!=NULL && rendColor!=NULL && CURRENT_RENDERER!=NULL)
    {
        double colorShadeDivision = 0;
        if( rendRect->w > 0 && rendRect->h > 0)
        {
            GPE_Color * colorShadeTempColor = new GPE_Color("color1",0,0,0);

            for( int i = 0; i <= rendRect->w; i++)
            {
                colorShadeDivision = (double)i/(double) rendRect->w;
                colorShadeTempColor->change_r( merge_channel(rendColor->get_r(),(double)( -colorShadeDivision+1.f)*255.f,(double) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_g( merge_channel(rendColor->get_g(),(double)( -colorShadeDivision+1.f)*255.f,(double) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_b( merge_channel(rendColor->get_b(),(double)( -colorShadeDivision+1.f)*255.f,(double) fabs( colorShadeDivision-0.5f)+0.5 ) );

                render_vertical_line_color(rendRect->x+i,rendRect->y,rendRect->y+rendRect->h,colorShadeTempColor);
            }
            delete colorShadeTempColor;
            colorShadeTempColor = NULL;
        }
    }
}


void GPE_Artist::render_point( int x, int y)
{
     render_point_color(x,y, currentColor, currentAlpha);
}

void GPE_Artist::render_point_color( int x, int y, GPE_Color *rendColor, int alphaChannel )
{

}

void GPE_Artist::render_rect( GPE_Rect * rendRect, GPE_Color * rendColor,bool outline, int alphaChannel)
{
    if( rendRect!=NULL)
    {
        SDL_Rect sdlRendRect = { (int)rendRect->x, (int)rendRect->y, (int)rendRect->w, (int)rendRect->h };
        SDL_SetRenderDrawColor( CURRENT_RENDERER->get_sdl_renderer(), rendColor->get_r(), rendColor->get_g(),rendColor->get_b(), alphaChannel );
        if( outline)
        {
            SDL_RenderDrawRect(CURRENT_RENDERER->get_sdl_renderer(), &sdlRendRect);
        }
        else
        {
            SDL_RenderFillRect( CURRENT_RENDERER->get_sdl_renderer(), &sdlRendRect );
        }
    }
}

void GPE_Artist::render_rectangle( int x1, int y1, int x2, int y2,  GPE_Color * rendColor,bool outline, int alphaChannel )
{
    if( alphaChannel < 0 )
    {
        alphaChannel = 0;
    }
    else if( alphaChannel > 255 )
    {
        alphaChannel = 255;
    }
    SDL_Rect rendRect = {0,0,0,0};
    rendRect.x = std::min(x1, x2);
    rendRect.y = std::min(y1, y2);
    rendRect.w = std::abs(x1- x2);
    rendRect.h = std::abs(y1- y2);
    SDL_SetRenderDrawColor( CURRENT_RENDERER->get_sdl_renderer(), rendColor->get_r(), rendColor->get_g(),rendColor->get_b(), alphaChannel );
    if( outline)
    {
        SDL_RenderDrawRect(CURRENT_RENDERER->get_sdl_renderer(),&rendRect);
    }
    else
    {
        SDL_RenderFillRect( CURRENT_RENDERER->get_sdl_renderer(), &rendRect );
    }
}

void GPE_Artist::render_rotated_rectangle(int xCenter, int yCenter, int w, int h, int angle, GPE_Color * rendColor, int alphaChannel )
{
    prerenderedSquare->render_tex_special( xCenter, yCenter,angle, w, h,rendColor, NULL, alphaChannel );
}

void GPE_Artist::render_roundrect(int x1, int y1, int x2, int y2, int rad,bool outline )
{
    render_roundrect_color( x1, y1, x2, y2, rad, outline, currentColor, currentAlpha );
}

void GPE_Artist::render_roundrect_color( int x1, int y1,int x2, int y2, int rad, bool outline,GPE_Color * rendColor , int alphaChannel )
{

}

void GPE_Artist::render_texture( int textureId, int x, int y,GPE_Rect* clip , int hAlign , int vAlign  )
{

}

void GPE_Artist::render_texture_resized( int textureId, int x, int y,int newWidth, int newHeight,GPE_Rect* clip, int hAlign, int vAlign, GPE_Color * renderColor )
{

}

void GPE_Artist::render_texture_rotated( int textureId, int x, int y,double newAngle,int newWidth, int newHeight,GPE_Rect* clip)
{

}

void GPE_Artist::render_triangle( int x1, int y1, int x2, int y2, int x3, int y3, bool isOutlne, int lineWidth )
{
    render_triangle_color( x1, y1, x2, y2, x3, y3, currentColor, currentAlpha, isOutlne, lineWidth );
}


void GPE_Artist::render_triangle_color( int x1, int y1, int x2, int y2, int x3, int y3, GPE_Color * rendColor , int alphaChannel, bool isOutlne, int lineWidth  )
{
    if( x1 == x2 && x2 == x3)
    {
        return;
    }

    if( y1==y2 && y2 ==y3 )
    {
        return;
    }


    if( isOutlne)
    {
        if( lineWidth < 1)
        {
            render_line_color(x1, y1, x2, y2, rendColor, alphaChannel);
            render_line_color(x1, y1, x2, y2, rendColor, alphaChannel);
            render_line_color(x1, y1, x3, y3, rendColor, alphaChannel);
        }
        else
        {
            render_line_width_color(x1, y1, x2, y2, lineWidth,rendColor, alphaChannel);
            render_line_width_color(x2, y2, x3, y3, lineWidth,rendColor, alphaChannel);
            render_line_width_color(x3, y3, x1, y1, lineWidth,rendColor, alphaChannel);
        }
        return;
    }

    //Sort by y-coordnates
    double xTop = x1;
    double yTop = y1;
    double xMiddle = x2;
    double yMiddle = y2;
    double xBottom = x3;
    double yBottom = y3;
    if( y1 >= y2 )
    {
        if( y2 >= y3)
        {
            //y1 = bottom
            //y2 = middle
            //y3 = top
            //(Swaps (X1,Y1) WITH (X3, Y3)
            xTop = x3;
            yTop = y3;
            xMiddle = x2;
            yMiddle = y2;
            xBottom = x1;
            yBottom = y1;
        }
        else
        {
            //y1 = bottom
            //y2 = top
            //y3 = middle
            //(Swaps (X1,Y1) WITH (X3, Y3)
            xTop = x1;
            yTop = y1;
            xMiddle = x3;
            yMiddle = y3;
            xBottom = x1;
            yBottom = y1;
        }
    }
    else if( y2 >= y3)
    {
        if( y1 >= y3)
        {
            //y1 = middle
            //y2 =  bottom
            //y3 = top
            xTop = x3;
            yTop = y3;
            xMiddle = x1;
            yMiddle = y1;
            xBottom = x2;
            yBottom = y2;
        }
        else
        {
            //y1 = top
            //y3 =  middle
            //y3 = bottom
            //Change Nothing...
        }
    }
    //Else: triangle is already pre-sorted yippie!

    if ( yMiddle == yBottom)
    {
        render_triangle_flatbottom(xTop,yTop, xMiddle,yMiddle, xBottom, yBottom, c_blue, alphaChannel);
    }
    /* check for trivial case of top-flat triangle */
    else if ( y1 == y2)
    {
        render_triangle_flattop(xTop,yTop, xMiddle,yMiddle, xBottom, yBottom, c_blue, alphaChannel);
    }
    else if( yTop!=yBottom )
    {
        /* general case - split the triangle in a topflat and bottom-flat one */
        double xHalf =  xTop + ( (yMiddle - yTop) / (yBottom - yTop) ) * (xBottom - xTop);
        //y4 = y2;
        render_triangle_flatbottom( xTop,yTop, xMiddle,yMiddle, xHalf, yMiddle, rendColor, alphaChannel );
        render_triangle_flattop(xMiddle,yMiddle, xHalf, yMiddle, xBottom, yBottom, rendColor, alphaChannel );
    }
}

void GPE_Artist::render_triangle_flatbottom ( double x1, double y1, double x2, double y2, double x3, double y3, GPE_Color * rendColor , int alphaChannel )
{
    return; // too unoptimzied for release....
    //Avoids dividing by 0
    if( y2==y1 || y1==y3)
    {
        //return;
    }
    double invslope1 = (x2 - x1) / (y2 - y1);
    double invslope2 = (x3 - x1) / (y3 - y1);

    double curx1 = x1;
    double curx2 = x1;

    for (int scanlineY = y1; scanlineY <= y2; scanlineY++)
    {
        render_horizontal_line_color( scanlineY, (int)curx1, (int)curx2, rendColor, alphaChannel );
        curx1 += invslope1;
        curx2 += invslope2;
    }
    GPE_Report("["+int_to_string(y2-y1)+"] lines rendered for 1st half of triangle" );
}

void GPE_Artist::render_triangle_flattop( double x1, double y1, double x2, double y2, double x3, double y3, GPE_Color * rendColor , int alphaChannel  )
{
    return; // too unoptimzied for release....
    //Avoids dividing by 0
    if( y1==y3 || y2==y3)
    {
        //return;
    }
    double invslope1 = (x3 - x1) / (y3 - y1);
    double invslope2 = (x3 - x2) / (y3 - y2);

    double curx1 = x3;
    double curx2 = x3;

    for (int scanlineY = y3; scanlineY > y1; scanlineY--)
    {
        render_horizontal_line_color( scanlineY, (int)curx1, (int)curx2,  rendColor, alphaChannel );
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
    //GPE_Report("["+int_to_string(y3-y1)+"] lines rendered for 2nd half of triangle" );
}


void GPE_Artist::render_square( int x, int y, int squareSize,  GPE_Color * rendColor,bool outline, int alphaChannel )
{
    render_rectangle(x, y, x+squareSize, y+squareSize, rendColor, outline, alphaChannel );
}

void GPE_Artist::render_line( int x1, int y1, int x2, int y2 )
{
    /*
    SDL_SetRenderDrawColor(CURRENT_RENDERER->get_sdl_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(CURRENT_RENDERER->get_sdl_renderer(), x1, y1, x2, y2);
    */
    render_line_width_color(x1, y1, x2, y2, defaultLineWidth, currentColor,  currentAlpha );
}

void GPE_Artist::render_line_color( int x1, int y1, int x2, int y2,  GPE_Color * rendColor, int alphaChannel)
{
    /*
    SDL_SetRenderDrawColor(CURRENT_RENDERER->get_sdl_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(CURRENT_RENDERER->get_sdl_renderer(), x1, y1, x2, y2);
    */
    render_line_width_color(x1, y1, x2, y2, defaultLineWidth, rendColor, alphaChannel);
}

void GPE_Artist::render_line_capped(  int x1, int y1, int x2, int y2, int lineWidth )
{

}

void GPE_Artist::render_line_capped_color(  int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel )
{
    render_line_width_color(x1, y1, x2, y2, lineWidth, rendColor, alphaChannel );

}


void GPE_Artist::render_line_width( int x1, int y1, int x2, int y2, int lineWidth)
{
    if( lineWidth > 0 && lineWidth < 256 && prerenderedSquare!=NULL  )
    {
        //lineWidth = 1;
        double lineAngle = gpe->get_direction(x1, y1, x2, y2 );
        int lineSize  = ceil( gpe->get_distance(x1, y1, x2, y2 ) );
        //render_line(x1,y1,x2,y2, currentColor, currentAlpha );

        x1  = x1 + gpe->lengthdir_x( lineSize/2, lineAngle );
        y1  = y1 + gpe->lengthdir_y( lineSize/2, lineAngle );
        prerenderedSquare->render_tex_special(x1,y1, lineAngle,lineSize,lineWidth, currentColor, NULL, currentAlpha );
    }
}

void GPE_Artist::render_line_width_color( int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel )
{
    if( lineWidth > 0 && lineWidth < 256 && prerenderedSquare!=NULL  )
    {
        //lineWidth = 1;
        double lineAngle = gpe->get_direction(x1, y1, x2, y2 );
        int lineSize  = ceil( gpe->get_distance(x1, y1, x2, y2 ) );
        if( lineSize <= 0)
        {
            return;
        }
        //render_line(x1,y1,x2,y2, rendColor, alphaChannel );

        x1  = x1 + gpe->lengthdir_x( lineSize/2, lineAngle );
        y1  = y1 + gpe->lengthdir_y( lineSize/2, lineAngle );
        prerenderedSquare->render_tex_special(x1,y1, lineAngle,lineSize,lineWidth, rendColor, NULL, alphaChannel );
    }
}

void GPE_Artist::render_horizontal_line(int y, int x1, int x2)
{
    //SDL_RenderDrawLine(CURRENT_RENDERER->get_sdl_renderer(), x1, y, x2, y);
    if( x1!=x2 && prerenderedSquare!=NULL  )
    {
        //render_line(x1,y1,x2,y2, rendColor, alphaChannel );
        render_line_width_color(x1, y, x2, y, defaultLineWidth, currentColor, currentAlpha);
    }
}

void GPE_Artist::render_horizontal_line_color( int y, int x1, int x2,  GPE_Color * rendColor, int alphaChannel)
{
    /*SDL_SetRenderDrawColor(CURRENT_RENDERER->get_sdl_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(CURRENT_RENDERER->get_sdl_renderer(), x1, y, x2, y);
    */
    if( x1!=x2 && prerenderedSquare!=NULL  )
    {
        render_line_width_color(x1, y, x2, y, defaultLineWidth, rendColor, alphaChannel);
    }
}

void GPE_Artist::render_vertical_line( int x, int y1, int y2)
{
    //SDL_RenderDrawLine(CURRENT_RENDERER->get_sdl_renderer(), x, y1, x, y2);
    if( y1!=y2 && prerenderedSquare!=NULL  )
    {
        //render_line_color(x,y1,x,y2, currentColor, currentAlpha );
        render_line_width_color(x, y1, x, y2, defaultLineWidth, currentColor, currentAlpha);
        //prerenderedSquare->render_tex_special(x,y1,0,1,y2-y1, currentColor, NULL, currentAlpha );
    }
}

void GPE_Artist::render_vertical_line_color( int x, int y1, int y2,  GPE_Color * rendColor, int alphaChannel)
{
    /*
    SDL_SetRenderDrawColor(CURRENT_RENDERER->get_sdl_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(CURRENT_RENDERER->get_sdl_renderer(), x, y1, x, y2);
    */
    if( y1!=y2 && prerenderedSquare!=NULL  )
    {
         render_line_width_color(x, y1, x, y2, defaultLineWidth, rendColor, alphaChannel);
    }
}

void GPE_Artist::set_artist_blend_mode( int newBlendMode )
{
    if( currentArtistBlendMode!=newBlendMode)
    {
        currentArtistBlendMode = newBlendMode;
        if( currentArtistBlendMode < 0 || currentArtistBlendMode > blend_mode_none )
        {
            currentArtistBlendMode = blend_mode_blend;
        }

        GPE_Texture *  tempCircleTexture = NULL;
        for( int i = 0; i < GPE_ARTIST_PRERENDER_SHAPE_COUNT; i++)
        {
            tempCircleTexture = prerenderedCircles[i];
            if( tempCircleTexture!=NULL )
            {
                tempCircleTexture->set_blend_mode( currentArtistBlendMode );
            }
        }
        if( prerenderedSquare!=NULL )
        {
            prerenderedSquare->set_blend_mode( currentArtistBlendMode );
        }
    }

    if( CURRENT_RENDERER!=NULL )
    {
        CURRENT_RENDERER->set_render_blend_mode( currentArtistBlendMode );
    }
}

void GPE_Artist::set_color( GPE_Color * color)
{
    if( color!=NULL )
    {
        currentColor->change_rgba( color->get_r(), color->get_g(), color->get_b(), currentAlpha );
        GPE_Texture *  tempCircleTexture = NULL;
        for( int i = 0; i < GPE_ARTIST_PRERENDER_SHAPE_COUNT; i++)
        {
            tempCircleTexture = prerenderedCircles[i];
            if( tempCircleTexture!=NULL )
            {
                tempCircleTexture->change_color( color );
            }
        }
    }
}

void GPE_Artist::set_color( int r, int g, int b )
{
    currentColor->change_rgba( r, g, b, currentAlpha );
    GPE_Texture *  tempCircleTexture = NULL;
    for( int i = 0; i < GPE_ARTIST_PRERENDER_SHAPE_COUNT; i++)
    {
        tempCircleTexture = prerenderedCircles[i];
        if( tempCircleTexture!=NULL )
        {
            tempCircleTexture->change_color( r,g,b );
        }
    }
}

void GPE_Artist::set_render_alpha( int newAlpha )
{
    currentAlpha = newAlpha;
}

void GPE_Artist::set_line_width( int newWidth )
{
   defaultLineWidth = newWidth;
}

void GPE_Artist::render_ligting_overlay( int x, int y)
{
    if( lightingOverlayTexture!=NULL )
    {
        lightingOverlayTexture->render_tex(-x, -y );
    }
}

void GPE_Artist::render_ligting_overlay_scaled( int x, int y, double scaleSize)
{
    if( lightingOverlayTexture!=NULL )
    {
        lightingOverlayTexture->render_tex_scaled(-x, -y, scaleSize, scaleSize );
    }
}

void GPE_Artist::resize_ligting_overlay( int w, int h)
{
    if( lightingOverlayTexture==NULL )
    {
        lightingOverlayTexture = new GPE_TargetTexture();
    }
    lightingOverlayTexture->resize_target(w, h);
    lightingOverlayTexture->change_color( 255, 255, 255 );
    lightingOverlayTexture->change_alpha( 255 );
    lightingOverlayTexture->set_blend_mode( blend_mode_mod );
}

void GPE_Artist::switch_ligting_overlay( bool on )
{
    if( CURRENT_RENDERER!=NULL)
    {
        CURRENT_RENDERER->set_viewpoint( NULL );
        if( on && lightingOverlayTexture!=NULL )
        {
            SDL_SetRenderTarget( CURRENT_RENDERER->get_sdl_renderer(),lightingOverlayTexture->get_sdl_texture() );
            CURRENT_RENDERER->reset_viewpoint();
            SDL_SetRenderDrawColor( CURRENT_RENDERER->get_sdl_renderer() , 0, 0, 0, 255 );
            SDL_RenderClear( CURRENT_RENDERER->get_sdl_renderer() );
            if( input->check_keyboard_pressed( kb_h ) )
            {
                GPE_Report("Setting Lighting to on["+int_to_string(lightingOverlayTexture->get_width() )+","+int_to_string(lightingOverlayTexture->get_height())+"]" );
            }
        }
        else
        {
            SDL_SetRenderTarget( CURRENT_RENDERER->get_sdl_renderer(), NULL );
            CURRENT_RENDERER->reset_viewpoint();
            if( input->check_keyboard_pressed( kb_h ) )
            {
                GPE_Report("Setting Lighting to off ");
            }
            //CURRENT_RENDERER->update_renderer();
        }
    }
}


GPE_Runtime::GPE_Runtime()
{
    loopStarted = false;

    gameTimer = new GPE_Timer();
    gameTimer->set_fps( 30 );
}

GPE_Runtime::~GPE_Runtime()
{
    if( gameTimer!=NULL )
    {
        delete gameTimer;
        gameTimer = NULL;
    }
}

double  GPE_Runtime::get_direction( double x1, double y1, double x2, double y2 )
{
    double returnVal =  atan2(y2-y1,x2-x1) * degreesMultiple;
    if( returnVal < 0 )
    {
        returnVal+=360;
    }
    return returnVal;
}

double  GPE_Runtime::get_distance( double x1, double y1, double x2, double y2 )
{
    if( x1==x2 && y1==y2 )
    {
        return 0;
    }
    return sqrt( pow(x2 - x1, 2) + pow(y2 - y1, 2) );
}

double GPE_Runtime::lengthdir_x( double length, double angle  )
{
    return degCos( angle  ) * length;
}

double GPE_Runtime::lengthdir_y( double length, double angle )
{
    return  degSin( angle  ) * length;
}


void GPE_Runtime::log_error( std::string errorReported)
{
    runtimeLog.push_back( errorReported );
}

int GPE_Runtime::get_delta_time()
{
    return gameTimer->get_delta_time();
}

int GPE_Runtime::get_fps()
{
    return gameTimer->get_fps();
}

int GPE_Runtime::get_ticks()
{
    return gameTimer->get_ticks();
}

void GPE_Runtime::start_loop()
{
    gameTimer->start();
    gcanvas->set_artist_blend_mode( blend_mode_blend );
    if( loopStarted )
    {
        MAIN_RENDERER->clear_renderer();
        end_loop();
    }
    //gets user input
    input->handle_input(true);
    loopStarted  = true;
}

void GPE_Runtime::end_loop(  bool capFPS, bool updateRenderer )
{
    if( !loopStarted )
    {
        start_loop();
        MAIN_RENDERER->clear_renderer();
    }
    if( updateRenderer )
    {
        MAIN_RENDERER->update_renderer();
    }
    gcanvas->set_artist_blend_mode( blend_mode_blend );
    if( capFPS )
    {
        gameTimer->cap_fps();
    }
    gameTimer->calculate_avg_fps();
    loopStarted = false;

    if( input->programJustStarted )
    {
        input->programStartTicks++;
        if( input->programStartTicks > FPS_CAP*10)
        {
            input->programJustStarted = false;
        }
    }
}

void GPE_Runtime::finish_loop()
{
    end_loop();
}

void GPE_Runtime::reset_loop()
{
    start_loop();
}

void GPE_Runtime::set_fps( int newFPS)
{
    gameTimer->set_fps( newFPS );
}

bool GPE_Init( int argc, char* args[], std::string publisher, std::string title )
{
    if( argc > 0)
    {
        APP_FILE_NAME = args[0];
        APP_DIRECTORY_NAME = get_path_from_file( APP_FILE_NAME );
        APP_DIRECTORY_NAME = string_replace_all( APP_DIRECTORY_NAME,"\\","/");
    }
    ARGS_PROCESSED = argc;

    //begins making random numbers...
    srand( time(NULL) );
    for(int rI = 0; rI < res_type_count; rI++)
    {
        RESOURCE_TYPE_NAMES[rI] = "";
    }
    if( (int)publisher.size() > 0)
    {
        GPE_PROGRAM_PUBLISHER = publisher;
    }
    else
    {
        GPE_PROGRAM_PUBLISHER = "Unknown_GPE_Exports";
    }
    GPE_PROGRAM_TITLE = title;
    //Sets the folder used in all get_user_settings_folder() calls based on the 2 paramers above
    GPE_Seek_Settings_Folder();

    //General Debug Info
    //GPE_Report("    WARNING: DO NOT CLOSE THE CONSOLE WINDOW. UNSAVED FILES AND DATA WILL BE LOST!");
    //Clears the error logs and such
    std::string errorLogName = get_user_settings_folder()+"gpe_error_log.txt";
    remove(errorLogName.c_str() );
    std::string buildEerrorLogName = get_user_settings_folder()+"build_errors.txt";
    remove( buildEerrorLogName.c_str() );
    buildEerrorLogName = get_user_settings_folder()+"resources_check.txt";
    remove( buildEerrorLogName.c_str() );

    GPE_Report("Attempting to initialize GPE-Engine( Version " + double_to_string(GPE_VERSION_DOUBLE_NUMBER) + ")");
    GPE_Report("Program Publisher:"+GPE_PROGRAM_PUBLISHER );
    GPE_Report("Program Title:"+GPE_PROGRAM_TITLE );
    GPE_Report("Settings Folder: "+get_user_settings_folder() );
    //Initialize all SDL subsystems

    GPE_Report("-Loading in dependencies...");
    GPE_Report("--SDL2...");
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        GPE_Report("---Error initializing SDL!");
        return false;
    }

    GPE_Report("--Attempting to initialize SDL_INIT_TIMER");
    if( SDL_InitSubSystem( SDL_INIT_TIMER ) == -1 )
    {
        GPE_Report("---Error initializing SDL_INIT_TIMER");
        return false;
    }

    GPE_Report("--Attempting to initialize SDL_INIT_EVENTS");
    if( SDL_InitSubSystem( SDL_INIT_EVENTS ) == -1 )
    {
        GPE_Report("---Error initializing SDL_INIT_EVENTS");
        return false;
    }


    GPE_Report("--Attempting to initialize SDL_INIT_GAMECONTROLLER");
    if( SDL_InitSubSystem( SDL_INIT_GAMECONTROLLER ) == -1 )
    {
        GPE_Report("---Error initializing SDL_INIT_GAMECONTROLLER");
        return false;
    }

    GPE_Report("--Attempting to initialize SDL_INIT_HAPTIC");
    if( SDL_InitSubSystem( SDL_INIT_HAPTIC ) == -1 )
    {
        GPE_Report("---Error initializing SDL_INIT_HAPTIC");
        return false;
    }



    GPE_Report("--SDL2 systems started...");
    GPE_Report("-Setting starting cursor...");
    SDL_SetCursor(SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_WAIT) );
    GPE_IS_LOADING = true;
    GPE_DEFAULT_CAMERA.x=0;
    GPE_DEFAULT_CAMERA.y=0;
    GPE_DEFAULT_CAMERA.w=(Uint16)SCREEN_WIDTH;
    GPE_DEFAULT_CAMERA.h=(Uint16)(SCREEN_HEIGHT);
    GPE_Report("-Setting event system..");
    SDL_EventState(SDL_DROPFILE,SDL_ENABLE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    /*
    if(SDL_Init(SDL_INIT_AUDIO))
    {
        SDL_putenv("SDL_AUDIODRIVER=dummy");
        SDL_Init(SDL_INIT_AUDIO);
    }
    */
    gpe_sdl = new GPE_SDL_Surface_Modifier();
    MAIN_RENDERER = new GPE_Renderer(SCREEN_WIDTH,SCREEN_HEIGHT,true,false,true);
    SCREEN_WIDTH = MAIN_RENDERER->get_window_width();
    SCREEN_HEIGHT = MAIN_RENDERER->get_window_height();

    CURRENT_RENDERER = MAIN_RENDERER;
    mainTranslator = new GPE_Translator("en");
    input = new GPE_InputManager();

    gcm = rsm = new GPE_DataManager();

    GPE_Report("-GPE_Color_System..");
    GPE_Init_Colors();
    GPE_Report("-GPE_Renderer.");
    SDL_ShowCursor(SDL_ENABLE);


    GPE_Report("-GPE_Img_System...");
    //loads support for JPG and other image formats.
    int flags=IMG_INIT_JPG|IMG_INIT_PNG |IMG_INIT_TIF;
    if( (IMG_Init(flags)&flags) != flags)
    {
        GPE_Report("--IMG_Init: Failed to init required jpg and png support!\n");
        GPE_Report("--IMG_Init: ", IMG_GetError() );
        GPE_Report("--Error initializing SDL_Mixer.");
        GPE_Report("--Error initializing SDL_Mixer.");
        return false;
        // handle error
    }

    GPE_Report("-GPE_Audio_System...");
    // load support for the OGG and MOD sample/music formats
    flags=MIX_INIT_OGG|MIX_INIT_MP3;
    if ((Mix_Init(flags)&flags) != flags)
    {
        GPE_Report("--Error loading SDL_Mixer");
        GPE_Report("--Mix_Init: Failed to init required ogg and mod support!\n");
        GPE_Report("--Mix_Init: ", Mix_GetError());
        return false;
        // handle error
    }
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024  ) == -1 )
    {
        GPE_Report("-- Error initializing SDL_Mixer.");
        AUDIO_WORKS = false;
    }
    GPE_Report("-Dependencies loaded...");
    GPE_Report("-Starting run-time...");

    GPE = gpe = new GPE_Runtime();
    GPE_Report("-Starting GPE Artist...");
    GCANVAS = gcanvas = new GPE_Artist();
    //Start counting frames per second
    gpe->start_loop();
    MAIN_RENDERER->clear_renderer();
    GPE_Report("-Run-time started ");
    gpe->end_loop();
    GPE_Report("-1st loop ended ");


    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SPRITE] = "Sprite";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ANIMATION] = "Animation";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE] = "Texture";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TILESHEET] = "Tilesheet";

    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_AUDIO] = "Audio";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_VIDEO] = "Video";

    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_FUNCTION] = "Function";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_CLASS] = "Class";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT] = "Object";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE] = "Scene";
    //RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ACHIEVEMENT] = "Achievement";

    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PATH] = "Path";
    //RESOURCE_TYPE_NAMES[RESOURCE_TYPE_DICTIONARY] = "Dictionary";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_FONT] = "Font";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_EMITTER] = "Particle";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_LIGHT] = "Light";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PLUGIN] = "Plugin";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_EVENT] = "Event";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_QUEST] = "Quest";
    RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PROJECT_SETTINGS] = "Project Properties";

    gpe->set_fps( FPS_CAP );
    return true;
}


bool GPE_Quit( )
{
    //Deletes in the order of dependencies from top/down
    GPE_Report("Deleting resource manager....");
    if( rsm!=NULL)
    {
        rsm->clean_up();
        delete rsm;
        rsm = NULL;
    }
    GPE_Report("Deleting gpe....");
    if( gpe!=NULL)
    {
        delete gpe;
        gpe = NULL;
    }

    GPE_Report("Deleting input object...");
    if( input!=NULL)
    {
        delete input;
        input = NULL;
    }



    GPE_Report("Quitting SDL_IMG....");
    IMG_Quit();

    GPE_Report("Quitting SDL_Mixer....");
    Mix_Quit();
    return true;
}

void GPE_Report(std::string stringIn,std::string stringIn2,std::string stringIn3,std::string stringIn4, std::string stringIn5, std::string stringIn6, std::string stringIn7, std::string stringIn8, std::string stringIn9)
{
    stringIn+=stringIn2;
    stringIn+=stringIn3;
    stringIn+=stringIn4;
    stringIn+=stringIn5;
    stringIn+=stringIn6;
    stringIn+=stringIn7;
    stringIn+=stringIn8;
    stringIn+=stringIn9;

    //std::cout << stringIn+" \n";

    std::string errorFileName = get_user_settings_folder()+"gpe_error_log.txt";

    std::ofstream filestr(errorFileName.c_str(), std::ios::out | std::ios::app);
    if( gpe!=NULL )
    {
        gpe->log_error( stringIn );
    }
    if( filestr.is_open() )
    {
        filestr << stringIn+" \n";
        filestr.close();
    }
}

//GPE Render calls [ End ]
