/*
gpe_artist_sdl.cpp
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


#include "gpe_artist_sdl.h"

namespace gpe
{

    //GPE Render calls [ Begin ]
    artist_sdl::artist_sdl( renderer_system_sdl * aRenderer)
    {
        line_render_point_position = 0;
        gpeSDLRenderer = NULL;
        sdlRenderer = NULL;
        lightingOverlayTexture = new texture_target_sdl();

        artistRenderer = aRenderer;

        gpeSDLRenderer = aRenderer;
        if( gpeSDLRenderer !=NULL )
        {
            sdlRenderer = gpeSDLRenderer->get_sdl_renderer();
        }

        texture_sdl * tempPRCircleFilled = NULL;
        texture_sdl * tempPRCircleOutline = NULL;

        for( int circlePX = 32; circlePX <= 512; circlePX += 32 )
        {
            tempPRCircleFilled = new texture_sdl();
            tempPRCircleOutline = new texture_sdl();

            tempPRCircleFilled->prerender_circle( artistRenderer,circlePX, c_white);
            prerenderedCircles.push_back( tempPRCircleFilled );

            //tempPRCircleOutline->prerender_circle( artistRenderer,circlePX, c_white);
            //prerenderedCirclesOutlines.push_back( tempPRCircleOutline);
        }

        for( int i_point = 0; i_point < render_points_giant_size; i_point++)
        {
            line_render_points[ i_point] ={0,0};
            rect_render_points[ i_point] = {0,0,0,1 };
        }
        prerenderedSquare = new texture_sdl();
        prerenderedSquare->prerender_rectangle( artistRenderer,1,1,c_white);
        geometry_texture = SDL_CreateTexture( sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 1024 );
    }

    artist_sdl::~artist_sdl()
    {
        int preRenderedCircleFilledCount = (int)prerenderedCircles.size();
        int preRenderedOutlineCircleCount = (int)prerenderedCirclesOutlines.size();

        for( int i = preRenderedCircleFilledCount -1; i >=0 ; i--)
        {
            if( prerenderedCircles[i] = NULL )
            {
                delete prerenderedCircles[i];
                prerenderedCircles[i] = NULL;
            }
        }

        for( int j = preRenderedOutlineCircleCount -1; j>=0 ; j--)
        {
            if( prerenderedCirclesOutlines[j] = NULL )
            {
                delete prerenderedCirclesOutlines[j];
                prerenderedCirclesOutlines[j] = NULL;
            }
        }

        if( geometry_texture != NULL )
        {
            //SDL_LockTexture( geometry_texture, );
            SDL_DestroyTexture( geometry_texture );
            geometry_texture = NULL;
        }
    }

    bool artist_sdl::render_circle( int x, int y, int rad, bool renderOutLine )
    {
        return render_circle_color(x,y,rad, currentColor, currentAlpha, renderOutLine);
    }

    bool artist_sdl::render_circle_color( int x, int y, int rad, color *rendColor, int alphaChannel, bool renderOutLine )
    {
        int preRenderedCountSize = (int)prerenderedCircles.size();
        if( sdlRenderer==NULL || rad <= 0 || preRenderedCountSize<= 0 )
        {
            return false;
        }

        texture_base * tempCircleTexture = NULL;
        int circleId = -1;
        if( rad > 512)
        {
            circleId = preRenderedCountSize -1;
        }
        else
        {
            circleId  = rad/32;
        }
        if( circleId >=0 && circleId < preRenderedCountSize )
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
            tempCircleTexture->set_blend_mode( currentArtistBlendMode );
            //tempCircleTexture->set_blend_mode( blend_mode_blend );
            tempCircleTexture->render_tex_resized(x-rad, y-rad, rad*2, rad*2, NULL,rendColor, alphaChannel );
            return true;
        }
        return false;
    }

    bool artist_sdl::render_oval( int x, int y, int w, int h,bool renderOutLine )
    {
        //Function not completed yet
        return render_oval_color(x,y,w,h, currentColor, currentAlpha, renderOutLine);
    }

    bool artist_sdl::render_oval_color( int x, int y, int w, int h, color *rendColor, int alphaChannel, bool renderOutLine )
    {
        //Function not completed yet
        return false;
    }

    bool artist_sdl::render_semi_circle( int x, int y, int rad, int direction, bool renderOutLine)
    {
        return render_semi_circle_color( x,y,rad, direction, currentColor, currentAlpha, renderOutLine );
    }

    bool artist_sdl::render_semi_circle_color( int x, int y, int rad, int direction, color *rendColor, int alphaChannel, bool renderOutLine )
    {
        int preRenderedCountSize = (int)prerenderedCircles.size();
        if( sdlRenderer==NULL || rad <= 0 || preRenderedCountSize <= 0 )
        {
            return false;
        }

        texture_base * tempCircleTexture = NULL;
        int circleId = -1;
        if( rad >=512 )
        {
            circleId = preRenderedCountSize -1;
        }
        else
        {
            circleId = rad /32;
        }

        if( circleId >=0 && circleId < preRenderedCountSize )
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

        if( circleId >=0 && circleId < preRenderedCountSize )
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

            tempCircleTexture->set_blend_mode( currentArtistBlendMode );
            /*
            x  = x - game_runtime->lengthdir_x( tempCipRect->x, direction );
            y  = y - game_runtime->lengthdir_y( tempCipRect->x, direction );
            */
            tempCircleTexture->render_tex_special_at_point(x, y, direction,gpe::fa_center,gpe::fa_middle,rad, rad*2,rendColor, tempCipRect, alphaChannel );
            return true;
        }
        return false;
    }


    void artist_sdl::render_gradient_circle(  int radius, shape_rect * rendRect , color * rendColor )
    {
        //Function not completed yet
    }

    void artist_sdl::render_gradient_vertical( shape_rect * rendRect, color * rendColor)
    {
        if( sdlRenderer==NULL)
        {
            return;
        }
        if( rendRect!=NULL && rendColor!=NULL )
        {
            float colorShadeDivision = 0;
            if( rendRect->w > 0 && rendRect->h > 0)
            {
                color * colorShadeTempColor = new color("color1",0,0,0);

                for( int i = 0; i <= rendRect->h; i++)
                {
                    colorShadeDivision = (float)i/(float) rendRect->h;
                    colorShadeTempColor->change_r( color_system->merge_channel(rendColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_g( color_system->merge_channel(rendColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_b( color_system->merge_channel(rendColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                    render_horizontal_line_color(rendRect->y+i,rendRect->x,rendRect->x+rendRect->w,colorShadeTempColor);
                }
                delete colorShadeTempColor;
                colorShadeTempColor = NULL;
            }
        }
    }

    void artist_sdl::render_gradient_horizontal( shape_rect * rendRect, color * rendColor)
    {
        if( sdlRenderer==NULL)
        {
            return;
        }
        if( rendRect!=NULL && rendColor!=NULL )
        {
            float colorShadeDivision = 0;
            if( rendRect->w > 0 && rendRect->h > 0)
            {
                color * colorShadeTempColor = new color("color1",0,0,0);

                for( int i = 0; i <= rendRect->w; i++)
                {
                    colorShadeDivision = (float)i/(float) rendRect->w;
                    colorShadeTempColor->change_r( color_system->merge_channel(rendColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_g( color_system->merge_channel(rendColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_b( color_system->merge_channel(rendColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                    render_vertical_line_color(rendRect->x+i,rendRect->y,rendRect->y+rendRect->h,colorShadeTempColor);
                }
                delete colorShadeTempColor;
                colorShadeTempColor = NULL;
            }
        }
    }


    void artist_sdl::render_point( int x, int y)
    {
        render_point_color(x,y, currentColor, currentAlpha);
    }

    void artist_sdl::render_point_color( int x, int y, color *rendColor, int alphaChannel )
    {

    }

    void artist_sdl::render_rect( shape_rect * rendRect, color * rendColor,bool outline, int alphaChannel)
    {
        if( sdlRenderer==NULL)
        {
            return;
        }
        if( rendRect!=NULL)
        {
            SDL_Rect sdlRendRect = { (int)rendRect->x, (int)rendRect->y, (int)rendRect->w, (int)rendRect->h };
            SDL_SetRenderDrawColor( sdlRenderer, rendColor->get_r(), rendColor->get_g(),rendColor->get_b(), alphaChannel );
            if( outline)
            {
                SDL_RenderDrawRect( sdlRenderer, &sdlRendRect);
            }
            else
            {
                SDL_RenderFillRect( sdlRenderer, &sdlRendRect );
            }
        }
    }

    void artist_sdl::render_rectangle( int x1, int y1, int x2, int y2,  color * rendColor,bool outline, int alphaChannel )
    {
        if( alphaChannel < 0 )
        {
            return;
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

        SDL_SetRenderDrawColor( sdlRenderer, rendColor->get_r(), rendColor->get_g(),rendColor->get_b(), alphaChannel );
        if( outline)
        {
            SDL_RenderDrawRect( sdlRenderer,&rendRect);
        }
        else
        {
            SDL_RenderFillRect( sdlRenderer, &rendRect );
        }
    }

    void artist_sdl::render_rotated_rectangle(int get_center(), int yCenter, int w, int h, int angle, color * rendColor, int alphaChannel )
    {
        prerenderedSquare->render_tex_special( get_center(), yCenter,angle, w, h,rendColor, NULL, alphaChannel );
    }

    void artist_sdl::render_roundrect(int x1, int y1, int x2, int y2, int rad,bool outline )
    {
        render_roundrect_color( x1, y1, x2, y2, rad, outline, currentColor, currentAlpha );
    }

    void artist_sdl::render_roundrect_color( int x1, int y1,int x2, int y2, int rad, bool outline,color * rendColor , int alphaChannel )
    {

    }


    void artist_sdl::render_triangle( shape_triangle2d * tri )
    {
        render_triangle_color( tri, currentColor, currentAlpha );
    }

    //Based on http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo1
    void artist_sdl::render_triangle_color( shape_triangle2d * tri, color * rendColor, int alphaChannel )
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

        //Sort by y-coordnates
        float xTop = INT_MAX;
        float yTop = INT_MAX;
        float xMiddle = INT_MIN;
        float yMiddle = INT_MIN;
        float xBottom = INT_MIN;
        float yBottom = INT_MIN;

        int i_vert = 0;
        for( i_vert = 0; i_vert < 3; i_vert++)
        {
            if( tri->vertices[ i_vert ].y < yTop )
            {
                xTop = tri->vertices[ i_vert ].x;
                yTop = tri->vertices[ i_vert ].y;
            }

            if( tri->vertices[ i_vert ].y > yBottom )
            {
                xBottom = tri->vertices[ i_vert ].x;
                yBottom = tri->vertices[ i_vert ].y;
            }
        }

        for( i_vert = 0; i_vert < 3; i_vert++)
        {
            if( tri->vertices[ i_vert ].y != yTop && tri->vertices[ i_vert ].y != yBottom )
            {
                xMiddle = tri->vertices[ i_vert ].x;
                yMiddle = tri->vertices[ i_vert ].y;
            }
        }

        int lines_rendered_count = 0;
        line_render_point_position = 0;
        SDL_SetRenderDrawColor( sdlRenderer,rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );

        if ( yMiddle == yBottom)
        {
            lines_rendered_count+= render_triangle_flatbottom(xTop,yTop, xMiddle,yMiddle, xBottom, yBottom);
        }
        /* check for trivial case of top-flat triangle */
        else if ( yTop == yMiddle)
        {
            lines_rendered_count+= render_triangle_flattop(xTop,yTop, xMiddle,yMiddle, xBottom, yBottom);
        }
        else if( yTop!=yBottom )
        {
            /* general case - split the triangle in a topflat and bottom-flat one */
            triangle_midpoint.x = (float)(xTop + ((float)(yMiddle - yTop) / (float)(yBottom - yTop)) * (xBottom - xTop));
            triangle_midpoint.y = yMiddle;

            lines_rendered_count+= render_triangle_flatbottom( xTop,yTop,  xMiddle,yMiddle, triangle_midpoint.x, triangle_midpoint.y );
            lines_rendered_count+= render_triangle_flattop(xMiddle,yMiddle,triangle_midpoint.x, triangle_midpoint.y,  xBottom, yBottom );
        }
        //SDL_RenderDrawLines( sdlRenderer, line_render_points, line_render_point_position );
        SDL_RenderFillRectsF( sdlRenderer, rect_render_points, line_render_point_position );
        line_render_point_position = 0;
    }

    //Based on http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo1
    int artist_sdl::render_triangle_flatbottom( float x1, float y1, float x2, float y2, float x3, float y3 )
    {
        //Avoids dividing by 0
        if( y2==y1 || y3==y1)
        {
            return 0;
        }

        //Calculates the slopes
        float invslope1 = (x2 - x1) / (y2 - y1);
        float invslope2 = (x3 - x1) / (y3 - y1);

        //Sets the starting x position
        float curx1 = x1;
        float curx2 = x1+1;

        int lines_rendered_count =0;
        bool use_left_coord = true; //This is used to connect SDL lines
        for ( int scanlineY = y1; scanlineY <= y2 ;scanlineY++ )
        {
            //Checks if we should use the coordinate, since SDL connects the current line with the past line
            /*
            if( use_left_coord )
            {
                line_render_points[line_render_point_position] = {(int)curx1, scanlineY};
                line_render_points[line_render_point_position+1] = {(int)curx2, scanlineY};

            }
            else
            {
                line_render_points[line_render_point_position] = {(int)curx2, scanlineY};
                line_render_points[line_render_point_position+1] = {(int)curx1, scanlineY};
            }
            */
            rect_render_points[line_render_point_position] = {(int)curx1, scanlineY, curx2 - curx1, 1 };

            use_left_coord = !use_left_coord;
            curx2 += invslope2;
            curx1 += invslope1;

            lines_rendered_count++;
            //line_render_point_position +=2;
            line_render_point_position ++;
        }
        return lines_rendered_count;
    }

    //Based on http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo1
    int artist_sdl::render_triangle_flattop( float x1, float y1, float x2, float y2, float x3, float y3 )
    {
        //Avoids dividing by 0
        if( y1==y3 || y2==y3)
        {
            return 0;
        }
        float invslope1 = (x3 - x1) / (y3 - y1);
        float invslope2 = (x3 - x2) / (y3 - y2);

        float curx1 = x3-1;
        float curx2 = x3;

        int lines_rendered_count = 0;
        bool use_left_coord = true; //Used to alternate the connnecting endpoint
        for( int scanlineY = y3; scanlineY > y1; scanlineY-- )
        {
            //Checks if we should use the coordinate, since SDL connects the current line with the past line
            /*
            if( use_left_coord )
            {
                line_render_points[line_render_point_position] = { (int)curx1, scanlineY};
                line_render_points[line_render_point_position+1] = { (int)curx2, scanlineY};
            }
            else
            {
                line_render_points[line_render_point_position] = { (int)curx2, scanlineY};
                line_render_points[line_render_point_position+1] = { (int)curx1, scanlineY};
            }
            */
            use_left_coord = !use_left_coord;
            rect_render_points[line_render_point_position] = {(int)curx1, scanlineY, curx2 - curx1, 1 };

            curx1 -= invslope1;
            curx2 -= invslope2;

            //line_render_point_position +=2;
            line_render_point_position ++;
            lines_rendered_count++;
        }
        return lines_rendered_count;
    }

    void artist_sdl::render_triangle_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * rendColor , int alphaChannel )
    {

    }

    void artist_sdl::render_triangle_coords( int x1, int y1, int x2, int y2, int x3, int y3)
    {
        render_triangle_color_coords( x1, y1, x2, y2, x3, y3, currentColor, currentAlpha );
    }

    void artist_sdl::render_triangle_outline( shape_triangle2d * tri, int lineWidth )
    {
        render_triangle_outline_color( tri, currentColor, currentAlpha, lineWidth );
    }

    void artist_sdl::render_triangle_outline_color( shape_triangle2d * tri, color * rendColor , int alphaChannel,int lineWidth )
    {
        //Based on http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
        if( tri == NULL)
        {
            return;
        }
        if( lineWidth <= 1)
        {
            SDL_SetRenderDrawColor( sdlRenderer,rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );

            line_render_points[0] = { tri->vertices[0].x, tri->vertices[0].y};
            line_render_points[1] = { tri->vertices[1].x, tri->vertices[2].y};
            line_render_points[2] = { tri->vertices[1].x, tri->vertices[2].y};

            SDL_RenderDrawLines( sdlRenderer, line_render_points, 3 );
            line_render_point_position = 0;
        }
        else
        {
            render_line_width_color(tri->vertices[0].x, tri->vertices[0].y, tri->vertices[1].x, tri->vertices[1].y, lineWidth,rendColor, alphaChannel);
            render_line_width_color(tri->vertices[1].x, tri->vertices[1].y, tri->vertices[2].x, tri->vertices[2].y, lineWidth,rendColor, alphaChannel);
            render_line_width_color(tri->vertices[2].x, tri->vertices[2].y, tri->vertices[0].x, tri->vertices[0].y, lineWidth,rendColor, alphaChannel);
        }
        return;
    }

    void artist_sdl::render_triangle_outline_coords( int x1, int y1, int x2, int y2, int x3, int y3, int lineWidth )
    {
        render_triangle_outline_color_coords( x1, y1, x2, y2, x3, y3, currentColor, currentAlpha, lineWidth );
    }

    void artist_sdl::render_triangle_outline_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * rendColor, int alphaChannel,int lineWidth )
    {
        if( lineWidth <= 1)
        {
            SDL_SetRenderDrawColor( sdlRenderer,rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );

            line_render_points[0] = { x1, y1};
            line_render_points[1] ={ x2, y2};
            line_render_points[2] ={ x3, y3};

            SDL_RenderDrawLines( sdlRenderer, line_render_points, 3 );
            line_render_point_position = 0;
        }
        else
        {
            render_line_width_color(x1, y1, x2, y2, lineWidth,rendColor, alphaChannel);
            render_line_width_color(x2, y2, x3, y3, lineWidth,rendColor, alphaChannel);
            render_line_width_color(x3, y3, x1, y1, lineWidth,rendColor, alphaChannel);
        }
        return;
    }


    void artist_sdl::render_square( int x, int y, int squareSize,  color * rendColor,bool outline, int alphaChannel )
    {
        render_rectangle(x, y, x+squareSize, y+squareSize, rendColor, outline, alphaChannel );
    }

    void artist_sdl::render_line( int x1, int y1, int x2, int y2 )
    {
        render_line_width_color(x1, y1, x2, y2, defaultLineWidth, currentColor,  currentAlpha );
    }

    void artist_sdl::render_line_color( int x1, int y1, int x2, int y2,  color * rendColor, int alphaChannel)
    {
        render_line_width_color(x1, y1, x2, y2, defaultLineWidth, rendColor, alphaChannel);
    }

    void artist_sdl::render_line_capped(  int x1, int y1, int x2, int y2, int lineWidth )
    {

    }

    void artist_sdl::render_line_capped_color(  int x1, int y1, int x2, int y2, int lineWidth, color *rendColor, int alphaChannel )
    {
        render_line_width_color(x1, y1, x2, y2, lineWidth, rendColor, alphaChannel );
    }

    void artist_sdl::render_line_width( int x1, int y1, int x2, int y2, int lineWidth)
    {
        if( lineWidth <= 0 || lineWidth > 256 || prerenderedSquare==NULL  )
        {
            return;
        }
        //lineWidth = 1;
        float lineAngle = semath::get_direction(x1, y1, x2, y2 );
        int lineSize  = ceil( semath::get_distance(x1, y1, x2, y2 ) );
        //render_line(x1,y1,x2,y2, currentColor, currentAlpha );

        x1  = x1 + semath::lengthdir_x( lineSize/2, lineAngle );
        y1  = y1 + semath::lengthdir_y( lineSize/2, lineAngle );
        prerenderedSquare->render_tex_special(x1,y1, lineAngle,lineSize,lineWidth, currentColor, NULL, currentAlpha );
    }

    void artist_sdl::render_line_width_color( int x1, int y1, int x2, int y2, int lineWidth, color *rendColor, int alphaChannel )
    {
        if( lineWidth == 1 )
        {
            SDL_SetRenderDrawColor( sdlRenderer, rendColor->get_r(),rendColor->get_g(),rendColor->get_b(), currentAlpha );
            SDL_RenderDrawLine( sdlRenderer, x1, y1, x2, y2);
            return;
        }
        if( lineWidth < 0 )
        {
            return;
        }
        float lineAngle = semath::get_direction(x1, y1, x2, y2 );
        int lineSize  = ceil( semath::get_distance(x1, y1, x2, y2 ) );
        if( lineSize <= 0)
        {
            return;
        }

        if( prerenderedSquare==NULL )
        {
            return;
        }
        x1  = x1 + semath::lengthdir_x( lineSize/2, lineAngle );
        y1  = y1 + semath::lengthdir_y( lineSize/2, lineAngle );
        prerenderedSquare->render_tex_special(x1,y1, lineAngle,lineSize,lineWidth, rendColor, NULL, alphaChannel );
    }

    void artist_sdl::render_horizontal_line(int y, int x1, int x2)
    {
        if( x1!=x2 )
        {
            render_line_width_color(x1, y, x2, y, defaultLineWidth, currentColor, currentAlpha);
        }
    }

    void artist_sdl::render_horizontal_line_color( int y, int x1, int x2,  color * rendColor, int alphaChannel)
    {
        if( x1!=x2  )
        {
            render_line_width_color(x1, y, x2, y, defaultLineWidth, rendColor, alphaChannel);
        }
    }

    void artist_sdl::render_vertical_line( int x, int y1, int y2)
    {
        if( y1!=y2 )
        {
            //render_line_color(x,y1,x,y2, currentColor, currentAlpha );
            render_line_width_color(x, y1, x, y2, defaultLineWidth, currentColor, currentAlpha);
        }
    }

    void artist_sdl::render_vertical_line_color( int x, int y1, int y2,  color * rendColor, int alphaChannel)
    {
        if( y1!=y2  )
        {
             render_line_width_color(x, y1, x, y2, defaultLineWidth, rendColor, alphaChannel);
        }
    }

    void artist_sdl::set_artist_blend_mode( int newBlendMode )
    {
        if( currentArtistBlendMode==newBlendMode)
        {
            return;
        }
        currentArtistBlendMode = newBlendMode;
        if( currentArtistBlendMode < 0 || currentArtistBlendMode > blend_mode_none )
        {
            currentArtistBlendMode = blend_mode_blend;
        }

        texture_base *  tempCircleTexture = NULL;
        int preRenderedCountSize = (int)prerenderedCircles.size();
        for( int i = 0; i < preRenderedCountSize; i++)
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

        if( gpeSDLRenderer!=NULL )
        {
            gpeSDLRenderer->set_render_blend_mode( currentArtistBlendMode );
        }
    }

    void artist_sdl::set_color( color * color)
    {
        if( color==NULL )
        {
            return;
        }
        currentColor->change_rgba( color->get_r(), color->get_g(), color->get_b(), currentAlpha );
        texture_base *  tempCircleTexture = NULL;

        int preRenderedCountSize = (int)prerenderedCircles.size();
        for( int i = 0; i < preRenderedCountSize; i++)
        {
            tempCircleTexture = prerenderedCircles[i];
            if( tempCircleTexture!=NULL )
            {
                tempCircleTexture->change_color( color );
            }
        }
    }

    void artist_sdl::set_color( int r, int g, int b )
    {
        currentColor->change_rgba( r, g, b, currentAlpha );
        texture_base *  tempCircleTexture = NULL;

        int preRenderedCircleCount = (int)prerenderedCircles.size();

        for( int i = 0; i < preRenderedCircleCount; i++)
        {
            tempCircleTexture = prerenderedCircles[i];
            if( tempCircleTexture!=NULL )
            {
                tempCircleTexture->change_color( r,g,b );
            }
        }
    }

    void artist_sdl::set_render_alpha( int newAlpha )
    {
        currentAlpha = newAlpha;
    }

    void artist_sdl::set_line_width( int newWidth )
    {
        defaultLineWidth = newWidth;
    }

    void artist_sdl::render_ligting_overlay( int x, int y)
    {
        if( lightingOverlayTexture!=NULL )
        {
            lightingOverlayTexture->render_overlay(artistRenderer,-x, -y );
        }
    }

    void artist_sdl::render_ligting_overlay_scaled( int x, int y, float scaleSize)
    {
        if( lightingOverlayTexture!=NULL )
        {
            lightingOverlayTexture->render_overlay_scaled( artistRenderer,-x, -y, scaleSize, scaleSize );
        }
    }

    void artist_sdl::resize_ligting_overlay( int w, int h)
    {
        if( lightingOverlayTexture==NULL )
        {
            lightingOverlayTexture = new texture_target_sdl();
        }
        lightingOverlayTexture->resize_target( gpeSDLRenderer,w, h);
        lightingOverlayTexture->change_color( 255, 255, 255 );
        lightingOverlayTexture->change_alpha( 255 );
        lightingOverlayTexture->set_blend_mode( blend_mode_mod );
    }

    void artist_sdl::switch_ligting_overlay( bool on )
    {
        if( gpeSDLRenderer == NULL )
        {
            return;
        }
        gpeSDLRenderer->set_viewpoint( NULL );
        if( on && lightingOverlayTexture!=NULL )
        {
            if( lightingOverlayTexture->get_texture_type() == "target-sdl")
            {
                texture_target_sdl * sdlTextureTarget = (texture_target_sdl * ) lightingOverlayTexture;
                if( sdlTextureTarget !=NULL)
                {
                    SDL_SetRenderTarget( sdlRenderer,sdlTextureTarget->get_sdl_texture() );
                    renderer_main_sdl->reset_viewpoint();
                    SDL_SetRenderDrawColor( sdlRenderer , 0, 0, 0, 255 );
                    SDL_RenderClear( sdlRenderer );
                    return;
                }
            }
        }
        else
        {
            SDL_SetRenderTarget( sdlRenderer, NULL );
            renderer_main_sdl->reset_viewpoint();
        }
    }

}
