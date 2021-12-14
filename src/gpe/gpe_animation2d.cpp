/*
gpe_animation2d.cpp
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

#include "gpe_animation2d.h"

namespace gpe
{
    //adds a animation with only one row of subimages
    animaton2d::animaton2d(render_package * r_package,std::string anim_name, std::string anim_filename, bool transparent_image)
    {
        animation_texture = NULL;
        name = anim_name;
        file_name = anim_filename;
        load_image( r_package, anim_filename, transparent_image );
        width=0;
        height=0;
        xoffset=0;
        yoffset=0;
        hPadding = 0;
        vPadding = 0;
        frameCount=0;
        name = "";
        file_name = "";
        animation_id = -1;
        collision_radius = 0;
        collision_box = new shape_rect();
        collision_box->x = 0;
        collision_box->y = 0;
        collision_box->w = 0;
        collision_box->h = 0;
    }

    animaton2d::~animaton2d()
    {
        clean_up();
    }

    void animaton2d::clean_up()
    {
        reset_frames();
    }

    bool animaton2d::copy_image_source(std::string directory_output_name)
    {
        if( animation_texture!=NULL )
        {
            return animation_texture->copy_image_source( directory_output_name );
        }
        return false;
    }

    void animaton2d::edit_collision_box(int cx, int cy, int cw, int ch)
    {
        collision_box->update_box( cx, cy, cw, ch );
    }

    void animaton2d::edit_collision_circle(int cx, int cy, int cr)
    {
        collision_box->x=cx;
        collision_box->y=cy;
        collision_box->w = 0;
        collision_box->h = 0;
        collision_radius=cr;

    }

    int animaton2d::get_frame_count()
    {
        return (int) animation_frames.size();
    }

    int animaton2d::get_width()
    {
        return width;
    }

    int animaton2d::get_height()
    {
        return height;
    }

    int animaton2d::get_hpadding()
    {
        return hPadding;
    }

    int animaton2d::get_vpadding()
    {
        return vPadding;
    }

    int animaton2d::get_texture_width()
    {
        if( animation_texture!=NULL)
        {
            return animation_texture->get_width();
        }
        return -1;
    }

    int animaton2d::get_texture_height()
    {
        if( animation_texture!=NULL)
        {
            return animation_texture->get_height();
        }
        return -1;
    }

    int animaton2d::get_frame_xoffset()
    {
        return xoffset;
    }

    int animaton2d::get_frame_yoffset()
    {
        return yoffset;
    }


    std::string animaton2d::get_file_name()
    {
        return file_name;
    }

    std::string animaton2d::get_name()
    {
        return name;
    }

    bool animaton2d::has_texture()
    {
        return (animation_texture!=NULL);
    }

    void animaton2d::load_image( render_package * r_package, std::string anim_filename, bool transparent )
    {
        if( r_package == NULL)
        {
            return;
        }
        clean_up();
        if( animation_texture != NULL )
        {
            delete animation_texture;
            animation_texture = NULL;
        }
        animation_texture = r_package->create_texture();
        file_name = anim_filename;
        if( sff_ex::file_exists( anim_filename) )
        {
            animation_texture->load_new_texture(r_package->packageRenderer,anim_filename, -1,transparent );
        }
    }

    void animaton2d::render(int sub_image_to_draw, int x_pos, int y_pos,  shape_rect *cam)
    {
        if(animation_texture!=NULL)
        {
            if( (sub_image_to_draw < (int)animation_frames.size() )&& (sub_image_to_draw>=0) )
            {
                if( cam!=NULL)
                {
    //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                    {
                        animation_texture->render_tex( x_pos-cam->x-collision_box->get_center(),y_pos-cam->y-collision_box->get_middle(), (animation_frames.at(sub_image_to_draw)) );
                    }
                }
                else
                {
                    animation_texture->render_tex( x_pos-collision_box->get_center(),y_pos-collision_box->get_middle(), (animation_frames.at(sub_image_to_draw)) );
                }
            }
        }
    }

    void animaton2d::render_colored(int sub_image_to_draw, int x_pos, int y_pos,color * render_color, int alpha ,  shape_rect *cam)
    {
        if(animation_texture!=NULL)
        {
            if( (sub_image_to_draw < (int)animation_frames.size() )&& (sub_image_to_draw>=0) )
            {
                if( cam!=NULL)
                {
    //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                    {
                        animation_texture->render_tex_colored( x_pos-cam->x-collision_box->get_center(),y_pos-cam->y-collision_box->get_middle(),render_color,alpha,(animation_frames.at(sub_image_to_draw)) );
                    }
                }
                else
                {
                    animation_texture->render_tex_colored( x_pos-collision_box->get_center(),y_pos-collision_box->get_middle(), render_color,alpha,animation_frames.at(sub_image_to_draw) );
                }
            }
        }
    }

    void animaton2d::render_piece( int x_pos, int y_pos, shape_rect * rect_piece, shape_rect * cam )
    {
        if(animation_texture!=NULL  )
        {
            //if( animation_texture->get_width() < rect_piece->x + rect_piece->w && animation_texture->get_height() < rect_piece->y +rect_piece->h )
            if( rect_piece!=NULL )
            {
                if( cam!=NULL)
                {
    //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                    {
                        animation_texture->render_tex( x_pos-cam->x,y_pos-cam->y, rect_piece );
                    }
                }
                else
                {
                    animation_texture->render_tex( x_pos,y_pos, rect_piece );
                }
            }
            else if( (int)animation_frames.size() > 0 )
            {
                if( cam!=NULL)
                {
    //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                    {
                        animation_texture->render_tex( x_pos-cam->x,y_pos-cam->y, animation_frames[0] );
                    }
                }
                else
                {
                    animation_texture->render_tex( x_pos,y_pos,animation_frames[0] );
                }
            }
        }
    }

    void animaton2d::render_piece_resized( int x_pos, int y_pos, int new_width, int new_height, shape_rect * rect_piece, shape_rect * cam )
    {
        if(animation_texture!=NULL )
        {
            if( rect_piece!=NULL )
            {
                //if( animation_texture->get_width() < rect_piece->x + rect_piece->w && animation_texture->get_height() < rect_piece->y +rect_piece->h )
                {
                    if( cam!=NULL)
                    {
        //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                        {
                            animation_texture->render_tex_resized( x_pos-cam->x,y_pos-cam->y, new_width, new_height,rect_piece );
                        }
                    }
                    else
                    {
                        animation_texture->render_tex_resized( x_pos,y_pos, new_width, new_height,rect_piece );
                    }
                }
            }
            else if( (int)animation_frames.size() > 0 )
            {
                if( cam!=NULL)
                {
    //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                    {
                        animation_texture->render_tex_resized( x_pos-cam->x,y_pos-cam->y, new_width, new_height,animation_frames[0] );
                    }
                }
                else
                {
                    animation_texture->render_tex_resized( x_pos,y_pos, new_width, new_height,animation_frames[0] );
                }
            }
        }
    }

    void animaton2d::render_resized(int sub_image_to_draw, int x_pos, int y_pos, float new_width, float new_height, shape_rect * cam )
    {
        if( width !=0 && height!= 0 )
        {
            float foundX_scale = new_width/(float)width;
            float foundY_scale = new_height/(float)height;
            render_scaled(sub_image_to_draw, x_pos, y_pos, foundX_scale, foundY_scale, cam );
        }
    }

    void animaton2d::render_rotated(int sub_image_to_draw, int x_pos, int y_pos, float new_angle, float x_scale, float y_scale, shape_rect * cam)
    {
        if( new_angle == 0 || new_angle==360 )
        {
            render_scaled(sub_image_to_draw, x_pos, y_pos, x_scale, y_scale, cam );
            return;
        }
        else if(animation_texture!=NULL && (sub_image_to_draw < (int)animation_frames.size() )&& (sub_image_to_draw>=0) )
        {
            int new_width = (float)width * x_scale;
            int new_height = (float)height * y_scale;
            if( cam!=NULL)
            {
                //if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+animationToDraw->width,y_pos+animationToDraw->height) == true )
                {
                     animation_texture->render_tex_special_at_point( x_pos-cam->x,y_pos-cam->y, new_angle,abs(collision_box->get_center()*x_scale),abs(collision_box->get_middle()*y_scale), new_width, new_height, NULL,animation_frames.at(sub_image_to_draw) );
                }
            }
            else
            {
                animation_texture->render_tex_special_at_point( x_pos,y_pos, new_angle, abs(collision_box->get_center()*x_scale),abs(collision_box->get_middle()*y_scale), new_width, new_height,NULL,animation_frames.at(sub_image_to_draw) );
            }
            //animation_texture->change_color(c_white);
        }
    }

    void animaton2d::render_scaled(int sub_image_to_draw, int x_pos, int y_pos, float x_scale, float y_scale, shape_rect * cam )
    {
        if( x_scale ==0 || y_scale ==0 )
        {
            return;
        }

        if(animation_texture!=NULL && (sub_image_to_draw < (int)animation_frames.size() )&& (sub_image_to_draw>=0) )
        {
            if( cam!=NULL)
            {
                //if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+animationToDraw->width,y_pos+animationToDraw->height) == true )
                animation_texture->render_tex_scaled( x_pos-cam->x-abs(collision_box->get_center()*x_scale),y_pos-cam->y-abs(collision_box->get_middle()*y_scale),x_scale, y_scale, animation_frames.at(sub_image_to_draw) );
            }
            else
            {
                animation_texture->render_tex_scaled(   x_pos-abs(collision_box->get_center()*x_scale),y_pos-abs(collision_box->get_middle()*y_scale), x_scale, y_scale,animation_frames.at(sub_image_to_draw) );
            }
        }
    }

    void animaton2d::render_special( int sub_image_to_draw, int x_pos, int y_pos, float x_scale, float y_scale, float new_angle,color * render_color, int alpha, shape_rect * cam )
    {
        if( x_scale <=0 || y_scale <=0 )
        {
            return;
        }

        if(animation_texture!=NULL && (sub_image_to_draw < (int)animation_frames.size() )&& (sub_image_to_draw>=0) )
        {
            int new_width = (float)width * x_scale;
            int new_height = (float)height * y_scale;
            float xPivot = collision_box->get_center() * x_scale;
            float yPivot = collision_box->get_middle() * y_scale;
            if( cam!=NULL)
            {
                //if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+animationToDraw->width,y_pos+animationToDraw->height) == true )
                {
                    animation_texture->render_tex_special_at_point( x_pos + xPivot - cam->x,y_pos + yPivot - cam->y, new_angle, xPivot, yPivot, new_width, new_height,render_color,animation_frames.at(sub_image_to_draw), alpha );
                }
            }
            else
            {
                animation_texture->render_tex_special_at_point( x_pos + xPivot,y_pos + yPivot, new_angle, xPivot, yPivot, new_width, new_height,render_color,animation_frames.at(sub_image_to_draw), alpha );
            }
            //animation_texture->change_color(c_white);
        }
    }

    void animaton2d::reset_frames()
    {
        shape_rect * cRect = NULL;
        for( int i = (int)animation_frames.size()-1; i >=0; i-- )
        {
            cRect = animation_frames[i];
            if( cRect!=NULL )
            {
                delete cRect;
                cRect = NULL;
            }
        }
        animation_frames.clear();
    }

    void animaton2d::setup_animation( int frame_count, int aw, int ah, int sofx, int sofy, int hPad, int vPad )
    {
        reset_frames();
        frameCount = frame_count;
        width = aw;
        height = ah;
        xoffset = sofx;
        yoffset = sofy;
        hPadding = hPad;
        vPadding = vPad;

        int animationsAdded = 0;
        if( animation_texture==NULL)
        {
            frameCount = 0;
            return;
        }
        shape_rect * newRect = NULL;

        int i = 0, j = 0;
        for( i  = yoffset; i < animation_texture->get_height();  i += height+vPadding)
        {
            for( j = xoffset; j < animation_texture->get_width();  j += width+hPadding)
            {
                if( animationsAdded <  frameCount )
                {
                    newRect = new shape_rect();
                    newRect->x = j;
                    newRect->y = i;
                    newRect->w = width;
                    newRect->h = height;
                    animation_frames.push_back( newRect );
                    animationsAdded++;
                }
                else
                {
                    return;
                }
            }
        }
    }


    void animaton2d::setup_fullimg_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad )
    {
        reset_frames();
        frameCount = 0;
        width = aw;
        height = ah;
        xoffset = sofx;
        yoffset = sofy;
        hPadding = hPad;
        vPadding = vPad;

        if( animation_texture==NULL)
        {
            return;
        }

        shape_rect * newRect = NULL;
        int i = 0, j = 0;
        for( i  = yoffset; i < animation_texture->get_height();  i += height+vPadding)
        {
            for( j = xoffset; j < animation_texture->get_width();  j += width+hPadding)
            {
                newRect = new shape_rect();
                newRect->x = j;
                newRect->y = i;
                newRect->w = width;
                newRect->h = height;
                animation_frames.push_back( newRect );
                frameCount++;
            }
        }
    }
}
