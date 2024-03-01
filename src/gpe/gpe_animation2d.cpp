/*
gpe_animation2d.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

    animation2d_frame_data::animation2d_frame_data()
    {
        collision_box = new shape_rect(0,0,32,32);
        collision_radius = 0;
        frame_box = new shape_rect(0,0,32,32);
        frame_message_id = -1;

    }

    animation2d_frame_data::~animation2d_frame_data()
    {
        if( frame_box!= nullptr)
        {
            delete frame_box;
            frame_box = nullptr;
        }

        if( collision_box!= nullptr)
        {
            delete collision_box;
            collision_box = nullptr;
        }
    }

    animation2d::animation2d()
    {
        animation_alignment = dir_top_left;
        animation_offset_x = 0;
        animation_offset_y = 0;
        animation_texture = nullptr;
        name = "";
        file_name = "";
        texture_is_transparent = false;
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
        ck_r = 255;
        ck_g = 0;
        ck_b = 255;
    }

    //adds a animation with only one row of subimages
    animation2d::animation2d( std::string anim_name, std::string anim_filename, bool transparent_image, uint8_t colorkeyR, uint8_t colorkeyG, uint8_t colorkeyB  )
    {
        animation_alignment = dir_top_left;
        animation_offset_x = 0;
        animation_offset_y = 0;
        ck_r = colorkeyR;
        ck_g = colorkeyG;
        ck_b = colorkeyB;
        animation_texture = nullptr;
        name = "";
        file_name = "";
        texture_is_transparent = false;
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
    }

    animation2d::~animation2d()
    {
        clean_up();
    }

    void animation2d::clean_up()
    {
        reset_frames();
    }

    bool animation2d::copy_image_source(std::string directory_output_name)
    {
        if( animation_texture!=nullptr )
        {
            return animation_texture->copy_image_source( directory_output_name );
        }
        return false;
    }

    animation2d *  animation2d::copy_self()
    {
        animation2d * new_animation = new animation2d(  name, file_name, texture_is_transparent, ck_r, ck_g, ck_b );
        return new_animation;
    }

    animation2d * animation2d::create_new( const std::string& anim_name, const std::string& anim_filename , bool transparent_image, uint8_t colorkeyR, uint8_t colorkeyG, uint8_t colorkeyB )
    {
        return new animation2d( anim_name, anim_filename, transparent_image, colorkeyR, colorkeyG, colorkeyB );
    }

    void animation2d::edit_collision_box(int frame_id,int cx, int cy, int cw, int ch)
    {
        if( frame_id < 0 || frame_id >= (int)frame_data.size() )
        {
            return;
        }
        frame_data[frame_id]->collision_box->update_box( cx, cy, cw, ch );
    }

    void animation2d::edit_collision_circle(int frame_id,int cx, int cy, int cr)
    {
        if( frame_id < 0 || frame_id >= (int)frame_data.size() || frame_id >= (int)frame_data.size() )
        {
            return;
        }

        frame_data[frame_id]->collision_box->update_box( cx, cy, cr, cr );
        frame_data[frame_id]->collision_radius=cr;

    }

    int animation2d::get_alignment()
    {
        return animation_alignment;
    }

    int animation2d::get_frame_count()
    {
        return (int) frame_data.size();
    }

    int animation2d::get_width()
    {
        return width;
    }

    int animation2d::get_height()
    {
        return height;
    }

    int animation2d::get_hpadding()
    {
        return hPadding;
    }

    int animation2d::get_vpadding()
    {
        return vPadding;
    }

    int animation2d::get_texture_width()
    {
        if( animation_texture!=nullptr)
        {
            return animation_texture->get_width();
        }
        return -1;
    }

    int animation2d::get_texture_height()
    {
        if( animation_texture!=nullptr)
        {
            return animation_texture->get_height();
        }
        return -1;
    }

    int animation2d::get_frame_xoffset()
    {
        return xoffset;
    }

    int animation2d::get_frame_yoffset()
    {
        return yoffset;
    }


    std::string animation2d::get_file_name()
    {
        return file_name;
    }

    std::string animation2d::get_name()
    {
        return name;
    }

    bool animation2d::has_texture()
    {
        return (animation_texture!=nullptr);
    }

    void animation2d::load_image( const std::string& anim_filename, bool transparent, uint8_t colorkeyR, uint8_t colorkeyG, uint8_t colorkeyB  )
    {
        //handled by child classes....
    }

    void animation2d::render(int sub_image_to_draw, int x_pos, int y_pos,  shape_rect *cam)
    {
        if(animation_texture!=nullptr)
        {
            if( (sub_image_to_draw < (int)frame_data.size() )&& (sub_image_to_draw>=0) )
            {
                if( cam!=nullptr)
                {
    //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                    {
                        animation_texture->render_tex( x_pos-cam->x+animation_offset_x,
                                                       y_pos-cam->y+animation_offset_y,
                                                       frame_data[sub_image_to_draw]->frame_box );
                    }
                }
                else
                {
                    animation_texture->render_tex(x_pos+animation_offset_x,
                                                  y_pos+animation_offset_y,
                                                  frame_data[sub_image_to_draw]->frame_box );
                }
            }
        }
    }

    void animation2d::render_colored(int sub_image_to_draw, int x_pos, int y_pos,color * render_color, int alpha ,  shape_rect *cam)
    {
        if(animation_texture!=nullptr)
        {
            if( (sub_image_to_draw < (int)frame_data.size() )&& (sub_image_to_draw>=0) )
            {
                if( cam!=nullptr)
                {
    //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                    {
                        animation_texture->render_tex_colored(x_pos-cam->x+animation_offset_x,
                                                              y_pos-cam->y+animation_offset_y,
                                                              render_color,alpha,frame_data[sub_image_to_draw]->frame_box  );
                    }
                }
                else
                {
                    animation_texture->render_tex_colored(x_pos+animation_offset_x,
                                                          y_pos+animation_offset_y,
                                                          render_color,alpha,frame_data[sub_image_to_draw]->frame_box );
                }
            }
        }
    }

    void animation2d::render_piece( int sub_image_to_draw, int x_pos, int y_pos, shape_rect * rect_piece, shape_rect * cam )
    {
        if(animation_texture!=nullptr  )
        {
            //if( animation_texture->get_width() < rect_piece->x + rect_piece->w && animation_texture->get_height() < rect_piece->y +rect_piece->h )
            if( rect_piece!=nullptr )
            {
                if( cam!=nullptr)
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
            else if( (int)frame_data.size() > 0 )
            {
                if( cam!=nullptr)
                {
    //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                    {
                        animation_texture->render_tex( x_pos-cam->x,y_pos-cam->y, frame_data[0]->frame_box );
                    }
                }
                else
                {
                    animation_texture->render_tex( x_pos,y_pos,frame_data[0]->frame_box );
                }
            }
        }
    }

    void animation2d::render_piece_resized( int sub_image_to_draw, int x_pos, int y_pos, int new_width, int new_height, shape_rect * rect_piece, shape_rect * cam )
    {
        if(animation_texture!=nullptr )
        {
            if( rect_piece!=nullptr )
            {
                //if( animation_texture->get_width() < rect_piece->x + rect_piece->w && animation_texture->get_height() < rect_piece->y +rect_piece->h )
                {
                    if( cam!=nullptr)
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
            else if( (int)frame_data.size() > 0 )
            {
                if( cam!=nullptr)
                {
    //                if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+width,y_pos+height) == true )
                    {
                        animation_texture->render_tex_resized( x_pos-cam->x,y_pos-cam->y, new_width, new_height,frame_data[0]->frame_box );
                    }
                }
                else
                {
                    animation_texture->render_tex_resized( x_pos,y_pos, new_width, new_height,frame_data[0]->frame_box );
                }
            }
        }
    }

    bool animation2d::render_quad( int sub_image_to_draw, shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha, shape_rect * cam )
    {
        sff_ex::append_to_file( "sprite_quad.txt", "base class..." );
        return true;
    }

    void animation2d::render_resized(int sub_image_to_draw, int x_pos, int y_pos, float new_width, float new_height, shape_rect * cam )
    {
        if( width !=0 && height!= 0 )
        {
            float foundX_scale = new_width/(float)width;
            float foundY_scale = new_height/(float)height;
            render_scaled(sub_image_to_draw, x_pos, y_pos, foundX_scale, foundY_scale, cam );
        }
    }

    void animation2d::render_rotated(int sub_image_to_draw, int x_pos, int y_pos, float new_angle, float x_scale, float y_scale, shape_rect * cam)
    {
        if( new_angle == 0 || new_angle==360 )
        {
            render_scaled(sub_image_to_draw, x_pos, y_pos, x_scale, y_scale, cam );
            return;
        }
        else if(animation_texture!=nullptr && (sub_image_to_draw < (int)frame_data.size() )&& (sub_image_to_draw>=0) )
        {
            int new_width = (float)width * x_scale;
            int new_height = (float)height * y_scale;
            if( cam!=nullptr)
            {
                //if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+animationToDraw->width,y_pos+animationToDraw->height) == true )
                {
                     animation_texture->render_tex_special_at_point( x_pos-cam->x,y_pos-cam->y, new_angle,
                                                                    animation_offset_x*x_scale,
                                                                    animation_offset_y*y_scale,
                                                                    new_width, new_height, nullptr,frame_data[sub_image_to_draw]->frame_box );
                }
            }
            else
            {
                animation_texture->render_tex_special_at_point( x_pos,y_pos, new_angle, animation_offset_x*x_scale,
                                                               animation_offset_y*y_scale, new_width, new_height,
                                                               nullptr,frame_data[sub_image_to_draw]->frame_box );
            }
            //animation_texture->change_color(c_white);
        }
    }

    void animation2d::render_scaled(int sub_image_to_draw, int x_pos, int y_pos, float x_scale, float y_scale, shape_rect * cam )
    {
        if( x_scale ==0 || y_scale ==0 )
        {
            return;
        }

        if(animation_texture!=nullptr && (sub_image_to_draw < (int)frame_data.size() )&& (sub_image_to_draw>=0) )
        {
            if( cam!=nullptr)
            {
                //if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+animationToDraw->width,y_pos+animationToDraw->height) == true )
                animation_texture->render_tex_scaled( x_pos-cam->x+( x_scale * animation_offset_x ),
                                                      y_pos-cam->y+( y_scale * animation_offset_y ),
                                                     x_scale, y_scale, frame_data[sub_image_to_draw]->frame_box );
            }
            else
            {
                animation_texture->render_tex_scaled(   x_pos + (x_scale * animation_offset_x ),
                                                        y_pos + (y_scale * animation_offset_y ), x_scale, y_scale,frame_data[sub_image_to_draw]->frame_box );
            }
        }
    }

    void animation2d::render_special( int sub_image_to_draw, int x_pos, int y_pos, float x_scale, float y_scale, float new_angle,color * render_color, int alpha, shape_rect * cam )
    {
        if( x_scale <=0 || y_scale <=0 )
        {
            return;
        }

        if(animation_texture!=nullptr && (sub_image_to_draw < (int)frame_data.size() )&& (sub_image_to_draw>=0) )
        {
            int new_width = (float)width * x_scale;
            int new_height = (float)height * y_scale;
            float xPivot = animation_offset_x * x_scale;
            float yPivot = animation_offset_y * y_scale;
            if( cam!=nullptr)
            {
                //if(check_collision(*cam,(int)x_pos,(int)y_pos,(int)x_pos+animationToDraw->width,y_pos+animationToDraw->height) == true )
                {
                    animation_texture->render_tex_special_at_point( x_pos + xPivot - cam->x,y_pos + yPivot - cam->y, new_angle,
                                                                   xPivot, yPivot, new_width, new_height,render_color,frame_data[sub_image_to_draw]->frame_box, alpha );
                }
            }
            else
            {
                animation_texture->render_tex_special_at_point( x_pos + xPivot,y_pos + yPivot, new_angle,
                                                               xPivot, yPivot, new_width, new_height,render_color,frame_data[sub_image_to_draw]->frame_box, alpha );
            }
            //animation_texture->change_color(c_white);
        }
    }

    void animation2d::reset_frames()
    {
        animation2d_frame_data * c_frame_data = nullptr;
        for( int i = (int)frame_data.size()-1; i >=0; i-- )
        {
            //Lets delete each animation frame 1 by one, collision and frame data
            c_frame_data = frame_data[i];
            if( c_frame_data!=nullptr )
            {
                delete c_frame_data;
                c_frame_data = nullptr;
            }
        }

        //All done, so now lets clear it.
        frame_data.clear();
    }

    void animation2d::set_alignment( int animAlign )
    {
        //Our failsafe if...
        if( width ==0 || height == 0)
        {
            animation_alignment = dir_top_left;
            animation_offset_x = 0;
            animation_offset_y = 0;
            return;
        }
        animation_alignment = animAlign;
        switch(animation_alignment )
        {
            case dir_top_center:
                animation_offset_x = -width/2;
                animation_offset_y = 0;
            break;

            case dir_top_right:
                animation_offset_x = -width;
                animation_offset_y = 0;
            break;

            case dir_middle_left:
                animation_offset_x = 0;
                animation_offset_y = -height/2;
            break;

            case dir_middle_center:
                animation_offset_x = -width/2;
                animation_offset_y = -height/2;
            break;

            //defaults to top-left alignment
            case dir_middle_right:
                animation_offset_x = -width;
                animation_offset_y = -height/2;
            break;

            case dir_bottom_left:
                animation_offset_x = 0;
                animation_offset_y = -height;
            break;

            case dir_bottom_center:
                animation_offset_x = -width/2;
                animation_offset_y = -height;
            break;

            //defaults to top-left alignment
            case dir_bottom_right:
                animation_offset_x = -width;
                animation_offset_y = -height;
            break;

            //defaults to top-left alignment
            default:
                animation_alignment = dir_top_left;
                animation_offset_x = 0;
                animation_offset_y = 0;
            break;
        }
    }

    void animation2d::setup_animation( int frame_count, int aw, int ah, int sofx, int sofy, int hPad, int vPad, int animAlign )
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
        if( animation_texture==nullptr)
        {
            frameCount = 0;
            return;
        }

        int i = 0, j = 0;

        animation2d_frame_data * c_frame_data = nullptr;
        for( i  = yoffset; i < animation_texture->get_height();  i += height+vPadding)
        {
            for( j = xoffset; j < animation_texture->get_width();  j += width+hPadding)
            {
                if( animationsAdded <  frameCount )
                {
                    c_frame_data = new animation2d_frame_data();
                    //Updates frame data
                    c_frame_data->frame_box->x = j;
                    c_frame_data->frame_box->y = i;
                    c_frame_data->frame_box->w = width;
                    c_frame_data->frame_box->h = height;

                    //Then updates the collision data
                    c_frame_data->collision_box->x = j;
                    c_frame_data->collision_box->y = i;
                    c_frame_data->collision_box->w = width;
                    c_frame_data->collision_box->h = height;

                    c_frame_data->collision_radius = std::min( width,height);

                    frame_data.push_back( c_frame_data );
                    animationsAdded++;
                }
                else
                {
                    set_alignment( animAlign);
                    return;
                }
            }
        }
        set_alignment( animAlign);

    }


    void animation2d::setup_fullimg_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad, int animAlign )
    {
        reset_frames();
        frameCount = 0;
        width = aw;
        height = ah;
        xoffset = sofx;
        yoffset = sofy;
        hPadding = hPad;
        vPadding = vPad;

        if( animation_texture==nullptr)
        {
            return;
        }

        animation2d_frame_data * c_frame_data = nullptr;
        int i = 0, j = 0;
        for( i  = yoffset; i < animation_texture->get_height();  i += height+vPadding)
        {
            for( j = xoffset; j < animation_texture->get_width();  j += width+hPadding)
            {
                c_frame_data = new animation2d_frame_data();
                //Updates frame data
                c_frame_data->frame_box->x = j;
                c_frame_data->frame_box->y = i;
                c_frame_data->frame_box->w = width;
                c_frame_data->frame_box->h = height;

                //Then updates the collision data
                c_frame_data->collision_box->x = j;
                c_frame_data->collision_box->y = i;
                c_frame_data->collision_box->w = width;
                c_frame_data->collision_box->h = height;

                c_frame_data->collision_radius = std::min( width,height);

                frame_data.push_back( c_frame_data );

                frameCount++;
            }
        }
        set_alignment( animAlign);

    }
}
