/*
gpe_artist_sdl2.cpp
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


#include "gpe_artist_sdl2.h"

namespace gpe
{

    //GPE Render calls [ Begin ]
    artist_sdl2::artist_sdl2( renderer_system_sdl2 * aRenderer)
    {
        past_arc_color = new color("past_arc_color",0,0,0,0);
        first_arc_in_frame = true;
        use_simple_geometry_rendering = true; //lets start with using geometry versus those "other methods" :-)
        line_render_point_position = 0;
        gpeSDLRenderer = NULL;
        sdlRenderer = NULL;
        lightingOverlayTexture = new texture_target_sdl2();

        artist_renderer = aRenderer;

        if( aRenderer != nullptr )
        {
            for( int i_rm = 0; i_rm < render_mode_max; i_rm++ )
            {
                render_mode_supported[ i_rm] = aRenderer->is_render_mode_supported( i_rm );
            }
        }
        gpeSDLRenderer = aRenderer;
        if( gpeSDLRenderer !=NULL )
        {
            sdlRenderer = gpeSDLRenderer->get_sdl2_renderer();
        }

        texture_sdl2 * tempPRCircleFilled = NULL;
        texture_sdl2 * tempPRCircleOutline = NULL;

        for( int circlePX = 32; circlePX <= 512; circlePX += 32 )
        {
            tempPRCircleFilled = new texture_sdl2();

            tempPRCircleFilled->prerender_circle( artist_renderer,circlePX, c_white );
            prerenderedCircles.push_back( tempPRCircleFilled );


        }

        for( int i_point = 0; i_point < render_points_giant_size; i_point++)
        {
            circle_render_points[ i_point] ={0,0};
            line_render_points[ i_point] ={0,0};
            line_render_fpoints[ i_point] ={0,0};
            rect_render_points[ i_point] = {0,0,0,1 };

            geometry_render_points[i_point].position.x = 0;
            geometry_render_points[i_point].position.y = 0;
            geometry_render_points[i_point].color = { 255, 255, 255, 255};
        }
        prerenderedSquare = new texture_sdl2();
        prerenderedSquare->prerender_rectangle( artist_renderer,16,16,c_white);
        geometry_texture = SDL_CreateTexture( sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 128, 128 );
    }

    artist_sdl2::~artist_sdl2()
    {
        int preRenderedCircleFilledCount = (int)prerenderedCircles.size();

        for( int i = preRenderedCircleFilledCount -1; i >=0 ; i--)
        {
            if( prerenderedCircles[i] = NULL )
            {
                delete prerenderedCircles[i];
                prerenderedCircles[i] = NULL;
            }
        }


        if( geometry_texture != NULL )
        {
            //SDL_LockTexture( geometry_texture, );
            SDL_DestroyTexture( geometry_texture );
            geometry_texture = NULL;
        }
    }

    void artist_sdl2::begin_frame()
    {
        artist_base::begin_frame();
    }
    void artist_sdl2::end_frame()
    {
        artist_base::end_frame();
    }

    int artist_sdl2::primitive_supported( primitive_type primitive_to_check )
    {
        return -1; //Will correct, just having as placeholder for now
    }

    void artist_sdl2::render_arc( float arc_x, float arc_y, float arc_radius, float start_angle, float end_angle, float arc_vertices )
    {
        render_arc_color(arc_x,arc_y,arc_radius, start_angle, end_angle, arc_vertices, color_current, alpha_current);
    }

    void artist_sdl2::render_arc_color( float arc_x, float arc_y,float arc_radius, float start_angle, float end_angle, float arc_vertices,  color * render_color, int alpha_channel  )
    {
        if( arc_radius < 1 || start_angle == end_angle || arc_vertices < 1 || alpha_channel < 1 )
        {
            return;
        }
        float step = (end_angle - start_angle) / (arc_vertices+1);
        float theta = start_angle;
        float vx  = 0,  vy = 0;

        int arc_i = 0;

        for( arc_i = 0; arc_i < arc_vertices ; arc_i++)
        {
            vx = arc_x + arc_radius * cos(theta);
            vy = arc_y + arc_radius * sin(theta);
            line_render_fpoints[arc_i].x = vx;
            line_render_fpoints[arc_i].y = vy;
            theta += step;
        }

        //Connects the last point to the start point
        vx = arc_x + arc_radius * cos(start_angle);
        vy = arc_y + arc_radius * sin(start_angle);
        line_render_fpoints[arc_i].x = vx;
        line_render_fpoints[arc_i].y = vy;

        if( arc_vertices == 0)
        {
            return;
        }

        SDL_SetRenderDrawColor( sdlRenderer,render_color->get_r(),render_color->get_g(),render_color->get_b(),alpha_channel );

        //Draws the entire arc
        SDL_RenderDrawLinesF( sdlRenderer, line_render_fpoints, arc_i+1 );
        arcs_in_frame++;
    }

    void artist_sdl2::render_arc_width( float arc_x, float arc_y, float arc_radius, float line_width, float start_angle, float end_angle, float arc_vertices )
    {
        render_arc_width_color(arc_x,arc_y,arc_radius,line_width, start_angle, end_angle, arc_vertices, color_current, alpha_current);
    }

    void artist_sdl2::render_arc_width_color( float arc_x, float arc_y,float arc_radius, float line_width, float start_angle, float end_angle, float arc_vertices,  color * render_color, int alpha_channel  )
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
        float step = (end_angle - start_angle) / ( arc_vertices);
        float theta = start_angle;// std::min(start_angle, end_angle);
        //float theta = std::min(start_angle, end_angle);
        float vx  = 0,  vy = 0;
        float vx2  = 0,  vy2 = 0;

        SDL_SetRenderDrawColor( sdlRenderer,render_color->get_r(),render_color->get_g(),render_color->get_b(),alpha_channel );

        line_width = line_width/ 2.f;

        vx = arc_x - arc_radius * cos(theta);
        vy = arc_y - arc_radius * sin(theta);
        vx2 = vx;
        vy2 = vy;

        int arc_i = 0;
        //render_circle_filled_color( vx, vy, line_width, render_color, alpha_channel );
        for( arc_i = arc_vertices ; arc_i >=0; arc_i-- )
        {
            vx = vx2;
            vy = vy2;
            theta += step;
            vx2 = arc_x - arc_radius * cos(theta);
            vy2 = arc_y - arc_radius * sin(theta);


            render_circle_filled_color( vx, vy, line_width, render_color, alpha_channel );

            //Experimental code commented out below:
            //render_line_capped_color(vx, vy, vx2, vy2, line_width, render_color, alpha_channel );
            //render_line_width_color(vx, vy, vx2, vy2, line_width, render_color, alpha_channel );
        }
        arcs_in_frame++;
    }

    void artist_sdl2::render_bezier_curve(  float x1, float y1, float x2, float y2 )
    {
        render_bezier_curve_width_color( x1, y1, x2, y2, 1, color_current, alpha_current );
    }

    void artist_sdl2::render_bezier_curve_color(  float x1, float y1, float x2, float y2,  color *render_color, int alpha_channel)
    {
        render_bezier_curve_width_color( x1, y1, x2, y2, 1, render_color, alpha_channel );
    }

    void artist_sdl2::render_bezier_curve_width_color(  float x1, float y1, float x2, float y2, int line_width, color *render_color, int alpha_channel)
    {
        //draw a straight line
        if( beizer_line_segments <= 2 )
        {
            render_line_width_color( x1, y1, x2, y2, line_width, render_color, alpha_channel );
            return;
        }
        else if( line_width < 1 || alpha_channel < 1)
        {
            return;
        }

        float previous_x = x1;
        float previous_y = y1;

        float current_x = 0, current_y = 0;
        int i = 0;

        if( line_width == 1 )
        {
            int point_pos = 0;

            for ( i= 1; i <= beizer_line_segments ; i++)
            {
                // Cubic easing in-out
                // NOTE: Easing is calculated only for y position value
                current_y = ease_cubic_in_out( (float)i, y1, y2 - y1, (float)beizer_line_segments);
                current_x = previous_x + (x2 - x1)/ (float)beizer_line_segments;

                render_line_width_color( previous_x, previous_y, current_x, current_y, line_width, render_color, alpha_channel );
                line_render_fpoints[point_pos].x = previous_x;
                line_render_fpoints[point_pos].y = previous_y;
                line_render_fpoints[point_pos+1].x = current_x;
                line_render_fpoints[point_pos+1].y = current_y;
                previous_x = current_x;
                previous_y = current_y;
                point_pos+=2;
            }
            SDL_RenderDrawLinesF( sdlRenderer, line_render_fpoints, point_pos );
        }
        else
        {
            int widthed_segments =  beizer_line_segments;//  * 128;
            int blend_mode_past = blend_current_mode;
            //blend_current_mode = blend_mode_blend;

            /*
            int max_circles = (int)prerenderedCircles.size();
            for( int i_circles = 0; i_circles < max_circles; i_circles++ )
            {
                prerenderedCircles[i_circles]->set_blend_mode( blend_mode_blend );
            }
            prerenderedSquare->set_blend_mode( blend_mode_blend );
            */

            for ( i= 1; i <= widthed_segments ; i++)
            {
                // Cubic easing in-out
                // NOTE: Easing is calculated only for y position value
                current_y = ease_cubic_in_out( (float)i, y1, y2 - y1, (float)widthed_segments);
                current_x = previous_x + (x2 - x1)/ (float)widthed_segments;

                render_circle_filled_color( previous_x, previous_y, line_width/2, render_color, alpha_channel );
                render_line_width_color( previous_x, previous_y, current_x, current_y, line_width, render_color, alpha_channel );
                previous_x = current_x;
                previous_y = current_y;
            }
            blend_current_mode = blend_mode_past;
        }
    }

    void artist_sdl2::render_bezier_curve_controlled(  shape_point2d point1, shape_point2d point2, shape_point2d control_point )
    {

    }

    void artist_sdl2::render_bezier_curve_controlled_color(  shape_point2d point1, shape_point2d point2, shape_point2d control_point ,  color *render_color, int alpha_channel)
    {

    }

    void artist_sdl2::render_bezier_curve_controlled_width_color(  shape_point2d point1, shape_point2d point2, shape_point2d control_point , int line_width, color *render_color, int alpha_channel )
    {

    }

    void artist_sdl2::render_bezier_curve_quad( shape_point2d point1, shape_point2d point2, shape_point2d control_point1, shape_point2d control_point2 )
    {

    }

    void artist_sdl2::render_bezier_curve_quad_color(  shape_point2d point1, shape_point2d point2, shape_point2d control_point1, shape_point2d control_point2 ,  color *render_color, int alpha_channel )
    {

    }

    void artist_sdl2::render_bezier_curve_quad_width_color(  shape_point2d point1, shape_point2d point2, shape_point2d control_point1, shape_point2d control_point2 , int line_width, color *render_color, int alpha_channel )
    {

    }


    bool artist_sdl2::render_circle_filled( int x, int y, int rad )
    {
        return render_circle_filled_color(x,y,rad, color_current, alpha_current);
    }

    bool artist_sdl2::render_circle_filled_color( int x, int y, int rad, color *render_color, int alpha_channel )
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
            tempCircleTexture = prerenderedCircles[circleId];
        }

        if( tempCircleTexture!=NULL )
        {
            tempCircleTexture->set_blend_mode( blend_current_mode );
            //tempCircleTexture->set_blend_mode( blend_mode_blend );
            tempCircleTexture->render_tex_resized(x-rad, y-rad, rad*2, rad*2, NULL,render_color, alpha_channel );
            circles_in_frame++;
            return true;
        }
        return false;
    }

    bool artist_sdl2::render_circle_outline( int x, int y, int rad )
    {
        return render_circle_outline_color( x, y, rad, color_current, alpha_current );
    }

    //Modified function from https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl#48291620
    //Stackoverflow users Scotty Stephens / Pharap

    bool artist_sdl2::render_circle_outline_color( int x, int y, int rad,  color * render_color, int alpha_channel )
    {
        int diameter = (rad * 2);

        int circle_x = (rad - 1);
        int circle_y = 0;
        int tx = 1;
        int ty = 1;
        int error = (tx - diameter);

        if( rad <= 0  || alpha_channel < 1 || render_color == NULL )
        {
            return false;
        }

        SDL_SetRenderDrawColor( sdlRenderer, render_color->get_r(), render_color->get_g(),render_color->get_b(), alpha_channel );

        //SDL_Point all_circle_points[9999999];
        int circle_point_total = 0;
        while (circle_x >= circle_y)
        {
          /*
          //  Each of the following renders an octant of the circle
          SDL_RenderDrawPoint(sdlRenderer, x + circle_x, y - circle_y );
          SDL_RenderDrawPoint(sdlRenderer, x + circle_x, y + circle_y );
          SDL_RenderDrawPoint(sdlRenderer, x - circle_x, y - circle_y );
          SDL_RenderDrawPoint(sdlRenderer, x - circle_x, y + circle_y);
          SDL_RenderDrawPoint(sdlRenderer, x + circle_y, y - circle_x);
          SDL_RenderDrawPoint(sdlRenderer, x + circle_y, y + circle_x);
          SDL_RenderDrawPoint(sdlRenderer, x - circle_y, y - circle_x);
          SDL_RenderDrawPoint(sdlRenderer, x - circle_y, y + circle_x);
          */

          circle_render_points[circle_point_total+0].x = x + circle_x;
          circle_render_points[circle_point_total+0].y = y - circle_y;

          circle_render_points[circle_point_total+1].x = x + circle_x;
          circle_render_points[circle_point_total+1].y = y + circle_y;

          circle_render_points[circle_point_total+2].x = x - circle_x;
          circle_render_points[circle_point_total+2].y = y - circle_y;

          circle_render_points[circle_point_total+3].x = x- circle_x;
          circle_render_points[circle_point_total+3].y = y + circle_y;

          circle_render_points[circle_point_total+4].x = x + circle_y;
          circle_render_points[circle_point_total+4].y = y - circle_x;

          circle_render_points[circle_point_total+5].x = x + circle_y;
          circle_render_points[circle_point_total+5].y = y + circle_x;

          circle_render_points[circle_point_total+6].x =  x - circle_y;
          circle_render_points[circle_point_total+6].y =  y - circle_x;

          circle_render_points[circle_point_total+7].x = x - circle_y;
          circle_render_points[circle_point_total+7].y = y + circle_x;

          circle_point_total+=8;

          if (error <= 0)
          {
             ++circle_y;
             error += ty;
             ty += 2;
          }

          if (error > 0)
          {
             --circle_x;
             tx += 2;
             error += (tx - diameter);
          }
        }
        SDL_RenderDrawPoints( sdlRenderer, circle_render_points, circle_point_total );

        circles_in_frame++;
        return true;
    }

    bool artist_sdl2::render_oval( int x, int y, int w, int h,bool renderOutLine )
    {
        //Function not completed yet
        return render_oval_color(x,y,w,h, color_current, alpha_current, renderOutLine);
    }

    bool artist_sdl2::render_oval_color( int x, int y, int w, int h, color *render_color, int alpha_channel, bool renderOutLine )
    {
        int preRenderedCountSize = (int)prerenderedCircles.size();
        int rad = std::max( w, h );
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
            tempCircleTexture = prerenderedCircles[circleId];
        }

        if( tempCircleTexture!=NULL )
        {
            tempCircleTexture->set_blend_mode( blend_current_mode );
            //tempCircleTexture->set_blend_mode( blend_mode_blend );
            tempCircleTexture->render_tex_resized(x, y, w, h, NULL,render_color, alpha_channel );
            return true;
        }
        circles_in_frame++;
        return false;
    }

    bool artist_sdl2::render_semi_circle( int x, int y, int rad, int direction, bool renderOutLine)
    {
        return render_semi_circle_color( x,y,rad, direction, color_current, alpha_current, renderOutLine );
    }

    bool artist_sdl2::render_semi_circle_color( int x, int y, int rad, int direction, color *render_color, int alpha_channel, bool renderOutLine )
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
            tempCircleTexture = prerenderedCircles[circleId];
        }

        if( circleId >=0 && circleId < preRenderedCountSize )
        {
           tempCircleTexture = prerenderedCircles[circleId];
        }

        if( tempCircleTexture!=NULL )
        {
            temp_clip_rect->x = temp_clip_rect->w = tempCircleTexture->get_width()/2;
            temp_clip_rect->y = 0;
            temp_clip_rect->h = tempCircleTexture->get_height();

            tempCircleTexture->set_blend_mode( blend_current_mode );
            /*
            x  = x - game_runtime->lengthdir_x( temp_clip_rect->x, direction );
            y  = y - game_runtime->lengthdir_y( temp_clip_rect->x, direction );
            */
            tempCircleTexture->render_tex_special_at_point(x, y, direction,gpe::fa_center,gpe::fa_middle,rad, rad*2,render_color, temp_clip_rect, alpha_channel );
            return true;
        }
        return false;
    }


    void artist_sdl2::render_gradient_circle(  int radius, shape_rect * rendRect , color * render_color )
    {
        //Function not completed yet
    }

    void artist_sdl2::render_gradient_vertical( shape_rect * rendRect, color * render_color)
    {
        if( sdlRenderer==NULL)
        {
            return;
        }
        if( rendRect!=NULL && render_color!=NULL )
        {
            float colorShadeDivision = 0;
            if( rendRect->w > 0 && rendRect->h > 0)
            {
                color * colorShadeTempColor = new color("color1",0,0,0);

                for( int i = 0; i <= rendRect->h; i++)
                {
                    colorShadeDivision = (float)i/(float) rendRect->h;
                    colorShadeTempColor->change_r( color_system->merge_channel(render_color->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_g( color_system->merge_channel(render_color->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_b( color_system->merge_channel(render_color->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                    render_horizontal_line_color(rendRect->y+i,rendRect->x,rendRect->x+rendRect->w,colorShadeTempColor);
                }
                delete colorShadeTempColor;
                colorShadeTempColor = NULL;
            }
        }
    }

    void artist_sdl2::render_gradient_horizontal( shape_rect * rendRect, color * render_color)
    {
        if( sdlRenderer==NULL)
        {
            return;
        }
        if( rendRect!=NULL && render_color!=NULL )
        {
            float colorShadeDivision = 0;
            if( rendRect->w > 0 && rendRect->h > 0)
            {
                color * colorShadeTempColor = new color("color1",0,0,0);

                for( int i = 0; i <= rendRect->w; i++)
                {
                    colorShadeDivision = (float)i/(float) rendRect->w;
                    colorShadeTempColor->change_r( color_system->merge_channel(render_color->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_g( color_system->merge_channel(render_color->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                    colorShadeTempColor->change_b( color_system->merge_channel(render_color->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                    render_vertical_line_color(rendRect->x+i,rendRect->y,rendRect->y+rendRect->h,colorShadeTempColor);
                }
                delete colorShadeTempColor;
                colorShadeTempColor = NULL;
            }
        }
    }


    void artist_sdl2::render_point( int x, int y)
    {
        render_point_color(x,y, color_current, alpha_current);
    }

    void artist_sdl2::render_point_color( int x, int y, color *render_color, int alpha_channel )
    {

    }

    void artist_sdl2::render_rect( shape_rect * rendRect, color * render_color,bool outline, int alpha_channel)
    {
        if( sdlRenderer==NULL)
        {
            return;
        }
        if( rendRect!=NULL)
        {
            SDL_Rect sdlRendRect = { (int)rendRect->x, (int)rendRect->y, (int)rendRect->w, (int)rendRect->h };
            SDL_SetRenderDrawColor( sdlRenderer, render_color->get_r(), render_color->get_g(),render_color->get_b(), alpha_channel );
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

    void artist_sdl2::render_rectangle( int x1, int y1, int x2, int y2,  color * render_color,bool outline, int alpha_channel )
    {
        if( alpha_channel < 0 )
        {
            return;
        }
        else if( alpha_channel > 255 )
        {
            alpha_channel = 255;
        }
        SDL_Rect rendRect = {0,0,0,0};
        rendRect.x = std::min(x1, x2);
        rendRect.y = std::min(y1, y2);
        rendRect.w = std::abs(x1- x2);
        rendRect.h = std::abs(y1- y2);

        SDL_SetRenderDrawColor( sdlRenderer, render_color->get_r(), render_color->get_g(),render_color->get_b(), alpha_channel );
        if( outline)
        {
            SDL_RenderDrawRect( sdlRenderer,&rendRect);
        }
        else
        {
            SDL_RenderFillRect( sdlRenderer, &rendRect );
        }
    }

    void artist_sdl2::render_rotated_rectangle(int get_center(), int yCenter, int w, int h, int angle, color * render_color, int alpha_channel )
    {
        prerenderedSquare->render_tex_special( get_center(), yCenter,angle, w, h,render_color, NULL, alpha_channel );
    }

    void artist_sdl2::render_roundrect_filled(int x1, int y1, int x2, int y2 )
    {
        int rad = std::min( std::abs( x1 - x2 ), std::abs( y2 - y1) ) /2;
        render_roundrect_filled_color_radius( x1, y1, x2, y2, rad,color_current, alpha_current );
    }

    void artist_sdl2::render_roundrect_filled_radius(int x1, int y1, int x2, int y2, int rad )
    {
        render_roundrect_filled_color_radius( x1, y1, x2, y2, rad, color_current, alpha_current );
    }

    void artist_sdl2::render_roundrect_filled_color( int x1, int y1,int x2, int y2, color * render_color , int alpha_channel )
    {
        int rad = std::min( std::abs( x1 - x2 ), std::abs( y2 - y1) ) / 2;
        rad = std::min( rad, 8);
        render_roundrect_filled_color_radius( x1, y1, x2, y2, rad, render_color, alpha_channel );
    }

    void artist_sdl2::render_roundrect_filled_color_radius( int x1, int y1,int x2, int y2, int rad, color * render_color , int alpha_channel )
    {
        if( rad <=0 )
        {
            return;
        }
        render_circle_filled_color( x1 + rad, y1 + rad, rad, render_color, alpha_channel );
        render_circle_filled_color( x2 - rad, y1 + rad, rad, render_color, alpha_channel);
        render_circle_filled_color( x1 + rad, y2 - rad, rad, render_color, alpha_channel );
        render_circle_filled_color( x2 - rad, y2 - rad, rad, render_color, alpha_channel );
        render_rectangle( x1, y1+rad, x2 , y2 - rad, render_color, false, alpha_channel );
        render_rectangle( x1 + rad, y1, x2 - rad, y2, render_color, false, alpha_channel );
    }

    void artist_sdl2::render_roundrect_outline(int x1, int y1, int x2, int y2)
    {

    }

    void artist_sdl2::render_roundrect_outline_radius(int x1, int y1, int x2, int y2, int rad)
    {

    }

    void artist_sdl2::render_roundrect_outline_color( int x1, int y1,int x2, int y2, color * render_color, int alpha_channel  )
    {

    }

    void artist_sdl2::render_roundrect_outline_color_radius( int x1, int y1,int x2, int y2, int rad,color * render_color, int alpha_channel  )
    {

    }

    void artist_sdl2::render_triangle( shape_triangle2d * tri )
    {
        render_triangle_color( tri, color_current, alpha_current );
    }

    //Based on http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo1
    void artist_sdl2::render_triangle_color( shape_triangle2d * tri, color * render_color, int alpha_channel )
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

        if ( use_simple_geometry_rendering )
        {
            render_triangle_gemoetry_color( tri, render_color, alpha_channel );
            return;
        }

        //Sort by y-coordnates
        float xTop = INT_MAX;
        float yTop = INT_MAX;
        float x_middle = INT_MIN;
        float y_middle = INT_MIN;
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
                x_middle = tri->vertices[ i_vert ].x;
                y_middle = tri->vertices[ i_vert ].y;
            }
        }

        int lines_rendered_count = 0;
        line_render_point_position = 0;
        SDL_SetRenderDrawColor( sdlRenderer,render_color->get_r(),render_color->get_g(),render_color->get_b(),alpha_channel );

        if ( y_middle == yBottom)
        {
            lines_rendered_count+= render_triangle_flatbottom(xTop,yTop, x_middle,y_middle, xBottom, yBottom);
        }
        /* check for trivial case of top-flat triangle */
        else if ( yTop == y_middle)
        {
            lines_rendered_count+= render_triangle_flattop(xTop,yTop, x_middle,y_middle, xBottom, yBottom);
        }
        else if( yTop!=yBottom )
        {
            /* general case - split the triangle in a topflat and bottom-flat one */
            triangle_midpoint.x = (float)(xTop + ((float)(y_middle - yTop) / (float)(yBottom - yTop)) * (xBottom - xTop));
            triangle_midpoint.y = y_middle;

            lines_rendered_count+= render_triangle_flatbottom( xTop,yTop,  x_middle,y_middle, triangle_midpoint.x, triangle_midpoint.y );
            lines_rendered_count+= render_triangle_flattop(x_middle,y_middle,triangle_midpoint.x, triangle_midpoint.y,  xBottom, yBottom );
        }
        //SDL_RenderDrawLines( sdlRenderer, line_render_points, line_render_point_position );
        SDL_RenderFillRectsF( sdlRenderer, rect_render_points, line_render_point_position );
        triangles_in_frame++;
        line_render_point_position = 0;
    }

    //Based on http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo1
    int artist_sdl2::render_triangle_flatbottom( float x1, float y1, float x2, float y2, float x3, float y3 )
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
            rect_render_points[line_render_point_position] = { curx1, (float)scanlineY, curx2 - curx1, 1.f };

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
    int artist_sdl2::render_triangle_flattop( float x1, float y1, float x2, float y2, float x3, float y3 )
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
            rect_render_points[line_render_point_position] = { curx1, (float)scanlineY, curx2 - curx1, 1.f };

            curx1 -= invslope1;
            curx2 -= invslope2;

            //line_render_point_position +=2;
            line_render_point_position ++;
            lines_rendered_count++;
        }
        return lines_rendered_count;
    }

    void artist_sdl2::render_triangle_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color , int alpha_channel )
    {
        render_triangle_color_coords( x1, y1, x2, y2, x3, y3, render_color, alpha_channel );
    }

    void artist_sdl2::render_triangle_coords( int x1, int y1, int x2, int y2, int x3, int y3)
    {
        render_triangle_color_coords( x1, y1, x2, y2, x3, y3, color_current, alpha_current );
    }

    void artist_sdl2::render_triangle_outline( shape_triangle2d * tri, int line_width )
    {
        render_triangle_outline_color( tri, color_current, alpha_current, line_width );
    }

    void artist_sdl2::render_triangle_outline_color( shape_triangle2d * tri, color * render_color , int alpha_channel,int line_width )
    {
        //Based on http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
        if( tri == NULL)
        {
            return;
        }
        if( line_width <= 1)
        {
            SDL_SetRenderDrawColor( sdlRenderer,render_color->get_r(),render_color->get_g(),render_color->get_b(),alpha_channel );

            line_render_fpoints[0] = { tri->vertices[0].x, tri->vertices[0].y};
            line_render_fpoints[1] = { tri->vertices[1].x, tri->vertices[2].y};
            line_render_fpoints[2] = { tri->vertices[1].x, tri->vertices[2].y};
            line_render_fpoints[2] = { tri->vertices[1].x, tri->vertices[2].y};

            SDL_RenderDrawLinesF( sdlRenderer, line_render_fpoints, 3 );
            line_render_point_position = 0;
        }
        else
        {
            render_line_width_color(tri->vertices[0].x, tri->vertices[0].y, tri->vertices[1].x, tri->vertices[1].y, line_width,render_color, alpha_channel);
            render_line_width_color(tri->vertices[1].x, tri->vertices[1].y, tri->vertices[2].x, tri->vertices[2].y, line_width,render_color, alpha_channel);
            render_line_width_color(tri->vertices[2].x, tri->vertices[2].y, tri->vertices[0].x, tri->vertices[0].y, line_width,render_color, alpha_channel);
        }
        return;
    }

    void artist_sdl2::render_triangle_outline_coords( int x1, int y1, int x2, int y2, int x3, int y3, int line_width )
    {
        render_triangle_outline_color_coords( x1, y1, x2, y2, x3, y3, color_current, alpha_current, line_width );
    }

    void artist_sdl2::render_triangle_outline_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color, int alpha_channel,int line_width )
    {
        if( line_width <= 1)
        {
            SDL_SetRenderDrawColor( sdlRenderer,render_color->get_r(),render_color->get_g(),render_color->get_b(),alpha_channel );

            line_render_points[0] = { x1, y1};
            line_render_points[1] ={ x2, y2};
            line_render_points[2] ={ x3, y3};

            SDL_RenderDrawLines( sdlRenderer, line_render_points, 3 );
            line_render_point_position = 0;
        }
        else
        {
            render_line_width_color(x1, y1, x2, y2, line_width,render_color, alpha_channel);
            render_line_width_color(x2, y2, x3, y3, line_width,render_color, alpha_channel);
            render_line_width_color(x3, y3, x1, y1, line_width,render_color, alpha_channel);
        }
        return;
    }


    void artist_sdl2::render_triangle_gemoetry( shape_triangle2d * tri )
    {
        if( tri == NULL)
        {
            return;
        }

        render_triangle_gemoetry_color( tri, color_current, alpha_current );
    }

    void artist_sdl2::render_triangle_gemoetry_color( shape_triangle2d * tri, color * render_color, int alpha_channel )
    {
        if( tri == NULL)
        {
            return;
        }
        if( render_color == NULL)
        {
            return;
        }

        geometry_render_points[current_triangle].position.x = tri->vertices[0].x;
        geometry_render_points[current_triangle].position.y = tri->vertices[0].y;
        geometry_render_points[current_triangle].color.r = render_color->get_r();
        geometry_render_points[current_triangle].color.g = render_color->get_g();
        geometry_render_points[current_triangle].color.b = render_color->get_b();
        geometry_render_points[current_triangle].color.a = alpha_channel;

        geometry_render_points[current_triangle+1].position.x = tri->vertices[1].x;
        geometry_render_points[current_triangle+1].position.y = tri->vertices[1].y;
        geometry_render_points[current_triangle+1].color.r = render_color->get_r();
        geometry_render_points[current_triangle+1].color.g = render_color->get_g();
        geometry_render_points[current_triangle+1].color.b = render_color->get_b();
        geometry_render_points[current_triangle+1].color.a = alpha_channel;

        geometry_render_points[current_triangle+2].position.x = tri->vertices[2].x;
        geometry_render_points[current_triangle+2].position.y = tri->vertices[2].y;
        geometry_render_points[current_triangle+2].color.r = render_color->get_r();
        geometry_render_points[current_triangle+2].color.g = render_color->get_g();
        geometry_render_points[current_triangle+2].color.b = render_color->get_b();
        geometry_render_points[current_triangle+2].color.a = alpha_channel;

        current_triangle+=3;
        triangles_in_frame++;

        if( current_triangle >= render_points_giant_size-2)
        {
            end_geometric_shape();
        }
        //SDL_RenderGeometry( sdlRenderer, NULL, geometry_render_points, 3, NULL, 0);
    }

    void artist_sdl2::render_triangle_gemoetry_coords( int x1, int y1, int x2, int y2, int x3, int y3)
    {
        geometry_render_points[current_triangle].position.x = x1;
        geometry_render_points[current_triangle].position.y = y1;
        geometry_render_points[current_triangle].color.r = color_current->get_r();
        geometry_render_points[current_triangle].color.g = color_current->get_g();
        geometry_render_points[current_triangle].color.b = color_current->get_b();
        geometry_render_points[current_triangle].color.a = alpha_current;


        geometry_render_points[current_triangle+1].position.x = x2;
        geometry_render_points[current_triangle+1].position.y = y2;
        geometry_render_points[current_triangle+1].color.r = color_current->get_r();
        geometry_render_points[current_triangle+1].color.g = color_current->get_g();
        geometry_render_points[current_triangle+1].color.b = color_current->get_b();
        geometry_render_points[current_triangle+1].color.a = alpha_current;

        geometry_render_points[current_triangle+2].position.x = x3;
        geometry_render_points[current_triangle+2].position.y = y3;
        geometry_render_points[current_triangle+2].color.r = color_current->get_r();
        geometry_render_points[current_triangle+2].color.g = color_current->get_g();
        geometry_render_points[current_triangle+2].color.b = color_current->get_b();
        geometry_render_points[current_triangle+2].color.a = alpha_current;

        //SDL_RenderGeometry(sdlRenderer, NULL, geometry_render_points, 3, NULL, 0);
        current_triangle+=3;

        if( current_triangle >= render_points_giant_size-2)
        {
            end_geometric_shape();
        }
    }

    void artist_sdl2::render_triangle_gemoetry_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * render_color, int alpha_channel )
    {
        if( render_color == NULL)
        {
            return;
        }

        geometry_render_points[current_triangle].position.x = x1;
        geometry_render_points[current_triangle].position.y = y1;
        geometry_render_points[current_triangle].color.r = render_color->get_r();
        geometry_render_points[current_triangle].color.g = render_color->get_g();
        geometry_render_points[current_triangle].color.b = render_color->get_b();
        geometry_render_points[current_triangle].color.a = alpha_channel;

        geometry_render_points[current_triangle+1].position.x = x2;
        geometry_render_points[current_triangle+1].position.y = y2;
        geometry_render_points[current_triangle+1].color.r = render_color->get_r();
        geometry_render_points[current_triangle+1].color.g = render_color->get_g();
        geometry_render_points[current_triangle+1].color.b = render_color->get_b();
        geometry_render_points[current_triangle+1].color.a = alpha_channel;

        geometry_render_points[current_triangle+2].position.x = x3;
        geometry_render_points[current_triangle+2].position.y = y3;
        geometry_render_points[current_triangle+2].color.r = render_color->get_r();
        geometry_render_points[current_triangle+2].color.g = render_color->get_g();
        geometry_render_points[current_triangle+2].color.b = render_color->get_b();
        geometry_render_points[current_triangle+2].color.a = alpha_channel;

        current_triangle+=3;

        if( current_triangle >= render_points_giant_size-2)
        {
            end_geometric_shape();
        }
    }

    void artist_sdl2::render_square( int x, int y, int squareSize,  color * render_color,bool outline, int alpha_channel )
    {
        render_rectangle(x, y, x+squareSize, y+squareSize, render_color, outline, alpha_channel );
    }

    void artist_sdl2::render_line( int x1, int y1, int x2, int y2 )
    {
        render_line_width_color(x1, y1, x2, y2, defaultLineWidth, color_current,  alpha_current );
    }

    void artist_sdl2::render_line_color( int x1, int y1, int x2, int y2,  color * render_color, int alpha_channel)
    {
        render_line_width_color(x1, y1, x2, y2, defaultLineWidth, render_color, alpha_channel);
    }

    void artist_sdl2::render_line_capped(  int x1, int y1, int x2, int y2, int line_width )
    {

    }

    void artist_sdl2::render_line_capped_color(  int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel )
    {
        render_circle_filled_color( x1, y1, line_width/2, render_color, alpha_channel );
        render_circle_filled_color( x2, y2, line_width/2, render_color, alpha_channel );
        render_line_width_color(x1, y1, x2, y2, line_width, render_color, alpha_channel );

    }

    void artist_sdl2::render_line_width( int x1, int y1, int x2, int y2, int line_width)
    {
        if( line_width <= 0 || line_width > 256 || prerenderedSquare==NULL  )
        {
            return;
        }
        //line_width = 1;
        float lineAngle = semath::get_direction(x1, y1, x2, y2 );
        int lineSize  = ceil( semath::get_distance(x1, y1, x2, y2 ) );
        //render_line(x1,y1,x2,y2, color_current, alpha_current );

        x1  = x1 - semath::lengthdir_x( (lineSize+line_width)/2, lineAngle );
        y1  = y1 - semath::lengthdir_y( (lineSize+line_width)/2, lineAngle );
        prerenderedSquare->render_tex_special(x1,y1, lineAngle,lineSize,line_width, color_current, NULL, alpha_current );
    }

    void artist_sdl2::render_line_width_color( int x1, int y1, int x2, int y2, int line_width, color *render_color, int alpha_channel )
    {
        if( line_width == 1 )
        {
            SDL_SetRenderDrawColor( sdlRenderer, render_color->get_r(),render_color->get_g(),render_color->get_b(), alpha_current );
            SDL_RenderDrawLine( sdlRenderer, x1, y1, x2, y2);
            return;
        }
        else if( line_width < 0 )
        {
            return;
        }
        float lineAngle = semath::get_direction(x1, y1, x2, y2 );
        int lineSize  =  semath::get_distance(x1, y1, x2, y2 ) ;
        if( lineSize <= 0)
        {
            return;
        }

        if( prerenderedSquare==NULL )
        {
            return;
        }

        int ogX1 = x1;
        int ogY1 = y1;

        int ogX2 = x2;
        int ogY2 = y2;

        x1  = x1 + semath::lengthdir_x( lineSize/2 , lineAngle );
        y1  = y1 + semath::lengthdir_y( lineSize/2 , lineAngle );


        x2  = x1 - semath::lengthdir_x( lineSize*3/2, lineAngle );
        y2  = y1 - semath::lengthdir_y( lineSize*3/2, lineAngle );

        //render_circle_filled_color( ogX1, ogY1, line_width/2, render_color, alpha_channel );
        //render_circle_filled_color( ogX2, ogY2, line_width/2, render_color, alpha_channel );

        prerenderedSquare->render_tex_special(x1,y1, lineAngle,lineSize,line_width, render_color, NULL, alpha_channel );

        /*
        SDL_Vertex line_width_vertices[4];
        for( int i_vert = 0; i_vert < 4; i_vert++ )
        {
            line_width_vertices[i_vert].position.x = 0;
            line_width_vertices[i_vert].position.y = 0;
            line_width_vertices[i_vert].color.r = render_color->get_r();
            line_width_vertices[i_vert].color.g = render_color->get_g();
            line_width_vertices[i_vert].color.b = render_color->get_b();
            line_width_vertices[i_vert].color.a = alpha_channel;
        }


        Not completed: SDL_RenderGeometry based method for line
        //x1/y1 coords
        line_width_vertices[0].position.x = ogX1 + semath::lengthdir_x( line_width/2, lineAngle -90 );
        line_width_vertices[0].position.y = ogY1 + semath::lengthdir_x( line_width/2, lineAngle -90 );

        line_width_vertices[1].position.x = ogX1 + semath::lengthdir_x( line_width/2, lineAngle +90);
        line_width_vertices[1].position.y = ogY1 + semath::lengthdir_x( line_width/2, lineAngle +90);

        //x2/y2 coords
        line_width_vertices[2].position.x = ogX2 - semath::lengthdir_x( line_width/2, lineAngle -45 );
        line_width_vertices[2].position.y = ogY2 - semath::lengthdir_x( line_width/2, lineAngle -45 );

        line_width_vertices[3].position.x = ogX2 + semath::lengthdir_x( line_width/2, lineAngle +45 );
        line_width_vertices[3].position.y = ogY2 + semath::lengthdir_x( line_width/2, lineAngle +45 );


        int line_indices[6];
        line_indices[0] = 0;
        line_indices[1] = 1;
        line_indices[2] = 2;
        line_indices[3] = 1;
        line_indices[4] = 2;
        line_indices[5] = 3;
        SDL_RenderGeometry( sdlRenderer, NULL, line_width_vertices, 4, line_indices, 6);
        */

    }

    void artist_sdl2::render_horizontal_line(int y, int x1, int x2)
    {
        if( x1!=x2 )
        {
            render_line_width_color(x1, y, x2, y, defaultLineWidth, color_current, alpha_current);
        }
    }

    void artist_sdl2::render_horizontal_line_color( int y, int x1, int x2,  color * render_color, int alpha_channel)
    {
        if( x1!=x2  )
        {
            render_line_width_color(x1, y, x2, y, defaultLineWidth, render_color, alpha_channel);
        }
    }

    void artist_sdl2::render_vertical_line( int x, int y1, int y2)
    {
        if( y1!=y2 )
        {
            //render_line_color(x,y1,x,y2, color_current, alpha_current );
            render_line_width_color(x, y1, x, y2, defaultLineWidth, color_current, alpha_current);
        }
    }

    void artist_sdl2::render_vertical_line_color( int x, int y1, int y2,  color * render_color, int alpha_channel)
    {
        if( y1!=y2  )
        {
             render_line_width_color(x, y1, x, y2, defaultLineWidth, render_color, alpha_channel);
        }
    }

    void artist_sdl2::set_artist_blend_mode( int blend_mode_new )
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

        if( gpeSDLRenderer!=NULL )
        {
            gpeSDLRenderer->set_render_blend_mode( blend_current_mode );
        }

        texture_base *  tempCircleTexture = NULL;
        int preRenderedCountSize = (int)prerenderedCircles.size();
        for( int i = 0; i < preRenderedCountSize; i++)
        {
            tempCircleTexture = prerenderedCircles[i];
            if( tempCircleTexture!=NULL )
            {
                tempCircleTexture->set_blend_mode( blend_current_mode );
            }
        }
        if( prerenderedSquare!=NULL )
        {
            prerenderedSquare->set_blend_mode( blend_current_mode );
        }
    }

    void artist_sdl2::set_color( color * color)
    {
        if( color==NULL )
        {
            return;
        }
        color_current->change_rgba( color->get_r(), color->get_g(), color->get_b(), alpha_current );
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

    void artist_sdl2::set_color( int r, int g, int b )
    {
        color_current->change_rgba( r, g, b, alpha_current );
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

    void artist_sdl2::set_render_alpha( int alpha_new )
    {
        alpha_current = alpha_new;
    }

    void artist_sdl2::set_line_width( int new_width )
    {
        defaultLineWidth = new_width;
    }

    bool artist_sdl2::enable_simple_geometry()
    {
        use_simple_geometry_rendering = true;
        return true;
    }


    bool artist_sdl2::disable_simple_geometry()
    {
        use_simple_geometry_rendering = false;
        return true;
    }

    bool artist_sdl2::begin_geometric_shape()
    {
        current_triangle = 0;
        return true;
    }


    bool artist_sdl2::end_geometric_shape()
    {
        if( current_triangle >= render_points_giant_size -2)
        {
            current_triangle = render_points_giant_size -2;
        }
        if( use_simple_geometry_rendering && current_triangle > 2 )
        {
            SDL_RenderGeometry( sdlRenderer, NULL, geometry_render_points, current_triangle, NULL, 0);
        }
        current_triangle = 0;
        return true;
    }

    void artist_sdl2::render_ligting_overlay( int x, int y)
    {
        if( lightingOverlayTexture!=NULL )
        {
            lightingOverlayTexture->render_overlay(artist_renderer,-x, -y );
        }
    }

    void artist_sdl2::render_ligting_overlay_scaled( int x, int y, float scale_size)
    {
        if( lightingOverlayTexture!=NULL )
        {
            lightingOverlayTexture->render_overlay_scaled( artist_renderer,-x, -y, scale_size, scale_size );
        }
    }

    void artist_sdl2::resize_ligting_overlay( int w, int h)
    {
        if( lightingOverlayTexture==NULL )
        {
            lightingOverlayTexture = new texture_target_sdl2();
        }
        lightingOverlayTexture->resize_target( gpeSDLRenderer,w, h);
        lightingOverlayTexture->change_color( 255, 255, 255 );
        lightingOverlayTexture->change_alpha( 255 );
        lightingOverlayTexture->set_blend_mode( blend_mode_mod );
    }

    void artist_sdl2::switch_ligting_overlay( bool on )
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
                texture_target_sdl2 * sdlTextureTarget = (texture_target_sdl2 * ) lightingOverlayTexture;
                if( sdlTextureTarget !=NULL)
                {
                    SDL_SetRenderTarget( sdlRenderer,sdlTextureTarget->get_sdl2_texture() );
                    renderer_main_sdl2->reset_viewpoint();
                    SDL_SetRenderDrawColor( sdlRenderer , 0, 0, 0, 255 );
                    SDL_RenderClear( sdlRenderer );
                    return;
                }
            }
        }
        else
        {
            SDL_SetRenderTarget( sdlRenderer, NULL );
            renderer_main_sdl2->reset_viewpoint();
        }
    }


    /*//----------------------------------------------------------------------------------
    // Cubic Easing out via raylib shapes.c
        https://easings.net/#easeInOutCubic?
        https://github.com/raysan5/raylib/blob/a940f41b4bfb681c9cbed9e95d1b73fee64d5bb8/src/rshapes.c

        LICENSE: zlib/libpng
        *
        *   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
        *
        *   This software is provided "as-is", without any express or implied warranty. In no event
        *   will the authors be held liable for any damages arising from the use of this software.
        *
        *   Permission is granted to anyone to use this software for any purpose, including commercial
        *   applications, and to alter it and redistribute it freely, subject to the following restrictions:
        *
        *     1. The origin of this software must not be misrepresented; you must not claim that you
        *     wrote the original software. If you use this software in a product, an acknowledgment
        *     in the product documentation would be appreciated but is not required.
        *
        *     2. Altered source versions must be plainly marked as such, and must not be misrepresented
        *     as being the original software.
        *
        *     3. This notice may not be removed or altered from any source distribution.
    //----------------------------------------------------------------------------------

    // Cubic easing in-out
    */

    float artist_sdl2::ease_cubic_in_out(float t, float b, float c, float d)
    {
        if ((t /= 0.5f*d) < 1) return 0.5f*c*t*t*t + b;

        t -= 2;

        return 0.5f*c*(t*t*t + 2.0f) + b;
    }
}
