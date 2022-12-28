/*
gpe_texture_sdl.cpp
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


#include "gpe_texture_sdl.h"

namespace gpe
{
    texture_sdl::texture_sdl()
    {
        texImg = NULL;
        textureType = "sdl";
    }

    texture_sdl::~texture_sdl()
    {
        if(texImg!= NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg=NULL;
        }
    }

    void texture_sdl::change_alpha( Uint8 alpha  )
    {
        if( texImg!=NULL )
        {
            SDL_SetTextureAlphaMod( texImg,alpha);
        }
    }

    void texture_sdl::change_color( color * color_new)
    {
        if(texImg!=NULL)
        {
            if( color_new!=NULL)
            {
                if( color_new->get_r() == currentR && color_new->get_g() == currentG && color_new->get_b() == currentB )
                {
                    return;
                }
                SDL_SetTextureColorMod( texImg, color_new->get_r(), color_new->get_g(), color_new->get_b() );
                currentR = color_new->get_r();
                currentG = color_new->get_g();
                currentB = color_new->get_b();
            }
            else if( currentR != 255 &&  currentG != 255 &&  currentB != 255 )
            {
                SDL_SetTextureColorMod( texImg, 255,255,255 );
                currentR = 255;
                currentG = 255;
                currentB = 255;
            }
        }
    }

    void texture_sdl::change_color( Uint8 red, Uint8 green, Uint8 blue )
    {
        if(texImg!=NULL)
        {
            if( red == currentR && green == currentG && blue == currentB )
            {
                return;
            }
            SDL_SetTextureColorMod( texImg, red, green, blue );
            currentR = red;
            currentG = green;
            currentB = blue;
        }
    }

    void texture_sdl::change_texture(SDL_Texture * newTexture)
    {
        texWid = 0;
        texHeight = 0;
        if(texImg!=NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg = NULL;
        }
        texImg = newTexture;
        if( texImg!=NULL)
        {
            SDL_QueryTexture(texImg, NULL, NULL, &texWid, &texHeight);
            currentBlendMode = blend_mode_blend;
        }
    }

    bool texture_sdl::copy_image_source(std::string directory_output_name)
    {
        if( get_width()>0 )
        {
            std::string copyDestinationStr = directory_output_name+"/"+ stg_ex::get_short_filename(fileLocation,true);
            return sff_ex::file_copy(fileLocation,copyDestinationStr );
        }
        return false;
    }

    texture_base * texture_sdl::create_new()
    {
        return new texture_sdl();
    }

    renderer_system_sdl * texture_sdl::get_gpe_renderer_sdl(renderer_base * renderer)
    {
        if( renderer == NULL)
        {
            return NULL;
        }
        if( renderer->get_renderer_type() == "sdl")
        {
            renderer_system_sdl * gpeSDLRenderer = (renderer_system_sdl * )renderer;
            return gpeSDLRenderer;
        }
        return NULL;
    }

    SDL_Renderer * texture_sdl::get_sdl_renderer(renderer_base * renderer)
    {
        renderer_system_sdl * sdlRenderer = get_gpe_renderer_sdl( renderer);

        if( sdlRenderer == NULL)
        {
            return NULL;
        }
        return sdlRenderer->get_sdl_renderer();
    }

    void texture_sdl::load_new_texture( renderer_base * renderer,std::string file_name, int id, bool transparent, bool useLinearScaling )
    {
        if(texImg!=NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg = NULL;
        }
        texId=id;
        isTransparent = transparent;
        //The image that's loaded
        if( sff_ex::file_exists(file_name ) == false )
        {
            error_log->report("[Bad] Unable to load filed  <"+file_name+">. Error: FILE_NOT_FOUND.");
            return;
        }
        currentBlendMode = blend_mode_blend;

        //Load the image
        SDL_Surface * loadedImage = sdl_surface_ex::load_surface_image( file_name.c_str() );

        //If the image loaded
        if( loadedImage == NULL )
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
            error_log->report("[Bad] Unable to load filed loacated at <"+file_name+">. Error: "+IMG_GetError()+".");
            return;
        }

        //Create an optimized surface
        if( useLinearScaling )
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        }
        else
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        }
        texWid = loadedImage->w;
        texHeight = loadedImage->h;
        texImg = sdl_surface_ex::create_texture_from_surface( get_sdl_renderer( renderer ),loadedImage);
        fileLocation = file_name;

        texId = id;
        if(texImg==NULL)
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
            error_log->report("[Bad] Unable to load file loacated at <"+file_name+">. Error: "+IMG_GetError()+".\n");
        }
    }

    SDL_Texture * texture_sdl::get_sdl_texture()
    {
        return texImg;
    }

    void texture_sdl::prerender_circle( renderer_base * renderer, int rad, color * circleColor,   Uint8 alpha, int id, bool transparent, bool useLinearScaling, bool isOutline )
    {
        SDL_Renderer * sdlRenderer = get_sdl_renderer( renderer);

        if( sdlRenderer == NULL || rad <=0 || circleColor == NULL)
        {
            return;
        }
        //SDL_Texture textImg
        if(texImg!=NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg = NULL;
        }
        currentBlendMode = blend_mode_blend;
        texId=id;
        isTransparent = transparent;
        //The image that's loaded

        texWid = rad*2;
        texHeight = rad*2;

        fileLocation = "circle:rad:"+ stg_ex::int_to_string(rad);
        SDL_Surface * loadedImage = sdl_surface_ex::create_filled_surface_rgba( rad * 2,  rad * 2, c_white->get_r(),c_white->get_g(),c_white->get_b() );
        SDL_SetSurfaceRLE( loadedImage,3 );
        SDL_SetColorKey( loadedImage, SDL_TRUE, SDL_MapRGB( loadedImage->format, c_white->get_r(), c_white->get_g(),c_white->get_b() ) );
        SDL_SetSurfaceBlendMode( loadedImage, SDL_BLENDMODE_BLEND );

        //If the image loaded
        if( loadedImage == NULL )
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
            return;
        }
        SDL_SetSurfaceBlendMode( loadedImage, SDL_BLENDMODE_BLEND );
        sdl_surface_ex::surface_render_circle_color_rgba( loadedImage, rad, rad, rad, circleColor->get_r(), circleColor->get_g(),circleColor->get_b(), 255  );
        //If the image loaded
        //Create an optimized surface
        texImg = sdl_surface_ex::create_texture_from_surface( sdlRenderer, loadedImage, SDL_PIXELFORMAT_RGBA8888);
        if( texImg!=NULL )
        {
            texWid = rad*2;
            texHeight = rad*2;
            SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_BLEND  );
        }
        else
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
        }
    }

    void texture_sdl::prerender_triangle( renderer_base * renderer, shape_triangle2d,  color * circleColor,  uint8_t alpha )
    {

    }

    void texture_sdl::prerender_rectangle( renderer_base * renderer, int w, int h, color * color_new, int id, bool transparent, bool useLinearScaling , bool isOutline)
    {
        if( renderer == NULL)
        {
            return;
        }
        SDL_Renderer * sdlRenderer = get_sdl_renderer( renderer);
        if( sdlRenderer == NULL)
        {
            return;
        }
        if(texImg!=NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg = NULL;
        }
        texId=id;
        isTransparent = transparent;
        //The image that's loaded
        if( w > 0 && h > 0  )
        {
            currentBlendMode = blend_mode_blend;
            SDL_Surface * loadedImage = NULL;
            if( color_new!=NULL )
            {
                loadedImage = sdl_surface_ex::create_filled_surface_rgba( w, h,color_new->get_r(),color_new->get_g(),color_new->get_b() );
            }
            else
            {
                loadedImage = sdl_surface_ex::create_filled_surface_rgba( w, h, c_gray->get_r(),c_gray->get_g(),c_gray->get_b() );
            }

            //If the image loaded
            if( loadedImage != NULL )
            {
                //Create an optimized surface
                if( useLinearScaling )
                {
                    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
                }
                else
                {
                    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
                }
                texWid = loadedImage->w;
                texHeight = loadedImage->h;
                texImg = sdl_surface_ex::create_texture_from_surface( sdlRenderer, loadedImage );
                fileLocation = "";

                texId = id;
                if(texImg==NULL)
                {
                    texWid = 0;
                    texHeight = 0;
                    fileLocation ="notfound.png";
                    //error_log->report("[Bad] Unable to load file loacated at <"+file_name+">. Error: "+IMG_GetError()+".\n");
                }
                else
                {
                    //error_log->report("[GOOD] Loaded filed loacated at <"+file_name+"> with <"+ stg_ex::int_to_string(texWid)+" , "+ stg_ex::int_to_string(texHeight)+"> dimensions.");
                }
            }
            else
            {
                texWid = 0;
                texHeight = 0;
                fileLocation ="notfound.png";
                //error_log->report("[Bad] Unable to load filed loacated at <"+file_name+">. Error: "+IMG_GetError()+".");
            }
        }
        else
        {
            //error_log->report("[Bad] Unable to load filed  <"+file_name+">. Error: FILE_NOT_FOUND.");
        }
    }

    void texture_sdl::render_align(  int x, int y, int alignment_h, int alignment_v, gpe::shape_rect* clip, color * render_color, int alpha )
    {
        if(alignment_h==gpe::fa_center)
        {
            x-=get_width()/2;
        }
        else if( alignment_h==gpe::fa_right)
        {
            x-= get_width();
        }

        if(alignment_v==gpe::fa_middle)
        {
            y-= get_height()/2;
        }
        else if( alignment_v==gpe::fa_bottom)
        {
            y-= get_height();
        }
        render_tex_colored( x,y, render_color, alpha, clip );
    }

    void texture_sdl::render_align_resized( int x, int y, int new_width, int new_height,  int alignment_h, int alignment_v, gpe::shape_rect* clip,color * render_color, int alpha )
    {
        if(alignment_h==gpe::fa_center)
        {
            x-=new_width/2;
        }
        else if( alignment_h==gpe::fa_right)
        {
            x-= new_width;
        }

        if(alignment_v==gpe::fa_middle)
        {
            y-= new_height/2;
        }
        else if( alignment_v==gpe::fa_bottom)
        {
            y-= new_height;
        }
        render_tex_resized( x,y,new_width, new_height,clip, render_color, alpha );
    }

    void texture_sdl::render_tex(  int x, int y, gpe::shape_rect* clip , int alpha)
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect render_rect = { x, y, texWid, texHeight };

            change_color( 255,255,255 );
            set_alpha( alpha );
            //Set clip rendering dimensions
            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;

                render_rect.w = clip->w;
                render_rect.h = clip->h;

                SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect);
            }
            else
            {
                SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &render_rect);
            }
        }
    }

    void texture_sdl::render_tex_colored(  int x, int y, color * render_color, int alpha , gpe::shape_rect* clip   )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect render_rect = { x, y, texWid, texHeight };
            change_color(render_color);
            set_alpha( alpha );
            //Set clip rendering dimensions
            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;

                render_rect.w = clip->w;
                render_rect.h = clip->h;

                SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect);
            }
            else
            {
                SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &render_rect);
            }
        }
    }

    void texture_sdl::render_tex_resized(  int x, int y, float new_width, float new_height, gpe::shape_rect* clip, color * render_color, int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            if( clip != NULL )
            {
                if( clip->w !=0 && clip->h!= 0 )
                {
                    float foundX_scale = new_width/clip->w;
                    float foundY_scale = new_height/clip->h;
                    render_tex_scaled( x, y, foundX_scale, foundY_scale,clip, render_color,alpha);
                }
            }
            else
            {
                if( texWid !=0 && texHeight!= 0 )
                {
                    float foundX_scale = new_width/(float)texWid;
                    float foundY_scale = new_height/(float)texHeight;
                    render_tex_scaled( x, y, foundX_scale, foundY_scale,clip, render_color,alpha);
                }
            }
            return;
        }
    }

    void texture_sdl::render_tex_scaled(  int x, int y, float x_scale, float y_scale, gpe::shape_rect* clip, color * render_color, int alpha )
    {
        if( x_scale == 0 || y_scale == 0 )
        {
            return;
        }

        if(texImg!=NULL)
        {
            change_color(render_color);
            set_alpha( alpha );

            int new_width = 0;
            int new_height = 0;
            bool flipHori = false,  flipVert = false;
            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;

                if( sdlClip.x > texWid)
                {
                    sdlClip.x = 0;
                }

                if( sdlClip.y > texHeight )
                {
                    sdlClip.y = 0;
                }

                if( sdlClip.w > texWid)
                {
                    sdlClip.w = texWid;
                }

                if( sdlClip.h > texHeight )
                {
                    sdlClip.h = texHeight;
                }

                if( sdlClip.x+sdlClip.w > texWid )
                {
                    sdlClip.w = texWid - sdlClip.x;
                }

                if( sdlClip.y+sdlClip.h > texHeight )
                {
                    sdlClip.h = texHeight - sdlClip.y;
                }

                if( x_scale < 0 )
                {
                    new_width = (float)sdlClip.w * -x_scale;
                    flipHori = true;
                }
                else
                {
                    new_width = (float)sdlClip.w * x_scale;
                }

                if( y_scale < 0 )
                {
                    new_height = (float)sdlClip.h * -y_scale;
                    flipVert = true;
                }
                else
                {
                    new_height = (float)sdlClip.h * y_scale;
                }
                SDL_Rect render_rect = { x, y, new_width, new_height };
                if( flipHori )
                {
                    if( flipVert )
                    {
                        SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, &sdlClip,&render_rect,0,NULL,renderer_main_sdl->bothFlip );

                    }
                    else
                    {
                        SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, &sdlClip,&render_rect,0,NULL,renderer_main_sdl->horiFlip );
                    }
                }
                else if( flipVert)
                {
                    SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, &sdlClip,&render_rect,0,NULL, renderer_main_sdl->vertFlip );
                }
                else
                {
                    SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect);
                }
            }
            else
            {
                if( x_scale < 0 )
                {
                    flipHori = true;
                    new_width = (float)texWid * -x_scale;
                }
                else
                {
                    new_width = (float)texWid * x_scale;
                }

                if( y_scale < 0 )
                {
                    flipVert = true;
                    new_height = (float)texHeight * -y_scale;
                }
                else
                {
                    new_height = (float)texHeight * y_scale;
                }
                SDL_Rect render_rect = { x, y, new_width, new_height };
                if( flipHori )
                {
                    if( flipVert )
                    {
                        SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, NULL,&render_rect,0,NULL, renderer_main_sdl->bothFlip );
                    }
                    else
                    {
                        SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, NULL,&render_rect,0,NULL, renderer_main_sdl->horiFlip );
                    }
                }
                else if( flipVert)
                {
                    SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, NULL,&render_rect,0,NULL, renderer_main_sdl->vertFlip );
                }
                else
                {
                    SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &render_rect);
                }
            }
        }
    }

    void texture_sdl::render_tex_rotated(  int x, int y, float render_angle, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect render_rect = { x-texWid/2, y-texHeight/2, texWid, texHeight };
            change_color(render_color);
            set_alpha( alpha );
            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;

                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect, -render_angle,NULL,SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &render_rect, -render_angle,NULL,SDL_FLIP_NONE );
            }
        }
    }

    void texture_sdl::render_tex_rotated_at_point(  int x, int y, float render_angle, int point_x, int point_y, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect render_rect = { x-texWid/2, y-texHeight/2, texWid, texHeight };
            change_color(render_color);
            set_alpha( alpha );

            if( point_x < 0 )
            {
                if( point_x == gpe::fa_center )
                {
                    renderer_main_sdl->defaultPoint.y = -texWid/2;
                }
                else if( point_x== gpe::fa_right )
                {
                    renderer_main_sdl->defaultPoint.y = -texWid;
                }

            }
            else if( point_x < texWid )
            {
                renderer_main_sdl->defaultPoint.y = point_x;
            }
            else
            {
                renderer_main_sdl->defaultPoint.y = 0;
            }

            if( point_y < 0 )
            {
                if( point_y == gpe::fa_middle )
                {
                    renderer_main_sdl->defaultPoint.y = texHeight/2;
                }
                else if( point_y== gpe::fa_bottom )
                {
                    renderer_main_sdl->defaultPoint.y = texHeight;
                }

            }
            else if( point_y < texHeight )
            {
                renderer_main_sdl->defaultPoint.y = point_y;
            }
            else
            {
                renderer_main_sdl->defaultPoint.y = 0;
            }

            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;

                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect, -render_angle,&renderer_main_sdl->defaultPoint,SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl->defaultPoint,SDL_FLIP_NONE);
            }
        }
    }

    void texture_sdl::render_tex_special(  int x, int y, float render_angle, int new_width, int new_height, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if( texImg==NULL || alpha <= 0 )
        {
            return;
        }

        if( new_width <=0.f)
        {
            new_width = texWid;
        }
        if( new_height<=0.f)
        {
            new_height = texHeight;
        }
        SDL_Rect render_rect = { x-new_width/2,y-new_height/2, new_width, new_height };


        change_color(render_color);
        set_alpha( alpha );

        renderer_main_sdl->defaultPoint.x = 0;
        renderer_main_sdl->defaultPoint.y = 0;

        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect, -render_angle, NULL,SDL_FLIP_NONE);
        }
        else
        {
            SDL_Rect sdlClip;
            sdlClip.x = 0;
            sdlClip.y = 0;
            sdlClip.w = texWid;
            sdlClip.h = texHeight;
            SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect, -render_angle, NULL,SDL_FLIP_NONE );
        }
    }

    void texture_sdl::render_tex_special_at_point(  int x, int y, float render_angle, int point_x, int point_y,int new_width, int new_height, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            bool flipHori = false,  flipVert = false;
            if( new_width < 0)
            {
                flipHori = true;
                new_width*=-1;
            }
            if( new_height < 0)
            {
                flipVert = true;
                new_height*=-1;
            }
            if( new_width ==0)
            {
                new_width = texWid;
            }
            if( new_height==0)
            {
                new_height = texHeight;
            }
            SDL_Rect render_rect = { x-new_width, y-new_height, new_width, new_height };
            change_color(render_color);
            set_alpha( alpha );
            if( point_x < 0 )
            {
                if( point_x == gpe::fa_center )
                {
                    renderer_main_sdl->defaultPoint.x = new_width/2;
                }
                else if( point_x == gpe::fa_right )
                {
                    renderer_main_sdl->defaultPoint.x = new_width;
                }
            }
            else if( point_x < new_width )
            {
                renderer_main_sdl->defaultPoint.x = point_x;
            }
            else
            {
                renderer_main_sdl->defaultPoint.x = new_width / 2;
            }

            if( point_y < 0 )
            {
                if( point_y == gpe::fa_middle )
                {
                    renderer_main_sdl->defaultPoint.y = new_height/2;
                }
                else if( point_y == gpe::fa_bottom )
                {
                    renderer_main_sdl->defaultPoint.y = new_height;
                }
            }
            else if( point_y < new_height )
            {
                renderer_main_sdl->defaultPoint.y = point_y;
            }
            else
            {
                renderer_main_sdl->defaultPoint.y = new_height / 2;
            }

            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;
                /*
                if( point_x < 0 )
                {
                    if( point_x == gpe::fa_center )
                    {
                        renderer_main_sdl->defaultPoint.x = sdlClip.w/2;
                    }
                    else if( point_x== gpe::fa_right )
                    {
                        renderer_main_sdl->defaultPoint.x = sdlClip.w;
                    }
                }
                else if( point_x < sdlClip.w )
                {
                    renderer_main_sdl->defaultPoint.x = point_x;
                }
                else
                {
                    renderer_main_sdl->defaultPoint.x = 0;
                }

                if( point_y < 0 )
                {
                    if( point_y == gpe::fa_middle )
                    {
                        renderer_main_sdl->defaultPoint.y = sdlClip.h /2;
                    }
                    else if( point_y == gpe::fa_bottom )
                    {
                        renderer_main_sdl->defaultPoint.y = -sdlClip.h ;
                    }

                }
                else if( point_y < sdlClip.h  )
                {
                    renderer_main_sdl->defaultPoint.y = point_y;
                }
                else
                {
                    renderer_main_sdl->defaultPoint.y = 0;
                }
                */
                if( flipHori )
                {
                    if( flipVert )
                    {
                        SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect, -render_angle,&renderer_main_sdl->defaultPoint,renderer_main_sdl->bothFlip);
                    }
                    else
                    {
                        SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect, -render_angle,&renderer_main_sdl->defaultPoint,renderer_main_sdl->horiFlip);
                    }
                }
                else if( flipVert)
                {
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect, -render_angle,&renderer_main_sdl->defaultPoint, renderer_main_sdl->vertFlip);
                }
                else
                {
                    //&renderer_main_sdl->defaultPoint
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &render_rect, -render_angle, NULL, SDL_FLIP_NONE);
                }
            }
            else if( flipHori )
            {
                if( flipVert )
                {
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl->defaultPoint,renderer_main_sdl->bothFlip);
                }
                else
                {
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl->defaultPoint,renderer_main_sdl->horiFlip);
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl->defaultPoint, renderer_main_sdl->vertFlip);
            }
            else
            {
                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl->defaultPoint,SDL_FLIP_NONE  );
            }
        }
    }

    void texture_sdl::set_alpha( int alpha )
    {
        if( alpha < 0 )
        {
            alpha = 0;
        }
        else if( alpha > 255)
        {
            alpha = 255;
        }
        if( texImg!=NULL && lastAlphaRendered!=alpha )
        {
            SDL_SetTextureAlphaMod(texImg,alpha);
            lastAlphaRendered = alpha;
        }
    }

    void texture_sdl::set_blend_mode( int blend_mode_new)
    {
        if( texImg!=NULL )//&& currentBlendMode!=blend_mode_new)
        {
            currentBlendMode = blend_mode_new;
            switch( blend_mode_new)
            {
                case blend_mode_add:
                    SDL_SetTextureBlendMode(texImg,SDL_BLENDMODE_ADD );
                break;

                case blend_mode_mod:
                    SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_MOD);
                break;

                case blend_mode_mul:
                    SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_MUL);
                break;

                case blend_mode_none:
                    SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_NONE  );
                    break;

                default:
                    SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_BLEND);
                    currentBlendMode = blend_mode_blend;
                break;
            }
        }
    }
}
