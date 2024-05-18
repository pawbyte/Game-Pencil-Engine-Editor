/*
gpe_texture_sdl2.cpp
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

-Game Pencil Engine  - SDL2 Module <https://gamepencil.net/sdl2-module/>


*/


#include "gpe_texture_sdl2.h"

namespace gpe
{
    texture_sdl2::texture_sdl2( bool store_pixels )
    {
        //sdlTexSurface = NULL;
        texImg = NULL;
        //sdlTexSurface_effect = NULL;
        //texImg_effect = NULL;
        texture_type = "sdl";
        is_prerendered = false;
        pixel_values_are_stored = store_pixels;
    }

    texture_sdl2::~texture_sdl2()
    {
        if(texImg!= NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg=NULL;
        }
    }

    void texture_sdl2::change_alpha( Uint8 alpha  )
    {
        if( texImg!=NULL )
        {
            SDL_SetTextureAlphaMod( texImg,alpha);
        }
    }

    void texture_sdl2::change_color( color * color_new)
    {
        if(texImg!=NULL)
        {
            if( color_new!=NULL)
            {
                if( color_new->get_r() == current_color_r && color_new->get_g() == current_color_g && color_new->get_b() == current_color_b )
                {
                    return;
                }
                SDL_SetTextureColorMod( texImg, color_new->get_r(), color_new->get_g(), color_new->get_b() );
                //SDL_SetTextureColorMod( texImg_effect, color_new->get_r(), color_new->get_g(), color_new->get_b() );
                current_color_r = color_new->get_r();
                current_color_g = color_new->get_g();
                current_color_b = color_new->get_b();
            }
            else if( current_color_r != 255 &&  current_color_g != 255 &&  current_color_b != 255 )
            {
                SDL_SetTextureColorMod( texImg, 255,255,255 );
                //SDL_SetTextureColorMod( texImg_effect, 255,255,255 );
                current_color_r = 255;
                current_color_g = 255;
                current_color_b = 255;
            }
        }
    }

    void texture_sdl2::change_color( Uint8 red, Uint8 green, Uint8 blue )
    {
        if(texImg!=NULL)
        {
            if( red == current_color_r && green == current_color_g && blue == current_color_b )
            {
                return;
            }
            SDL_SetTextureColorMod( texImg, red, green, blue );
            //SDL_SetTextureColorMod( texImg_effect, red, green, blue );
            current_color_r = red;
            current_color_g = green;
            current_color_b = blue;
        }
    }

    void texture_sdl2::change_texture(SDL_Texture * newTexture)
    {
        tex_width = 0;
        tex_height = 0;
        if(texImg!=NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg = NULL;
        }
        texImg = newTexture;
        if( texImg!=NULL)
        {
            SDL_QueryTexture(texImg, NULL, NULL, &tex_width, &tex_height);
            blend_mode_current = blend_mode_blend;
        }
    }

    bool texture_sdl2::copy_image_source(std::string directory_output_name)
    {
        if( get_width()>0 )
        {
            std::string copyDestinationStr = directory_output_name+"/"+ stg_ex::get_short_filename(file_location,true);
            return sff_ex::file_copy(file_location,copyDestinationStr );
        }
        return false;
    }

    texture_base * texture_sdl2::create_new()
    {
        return new texture_sdl2();
    }

    uint32_t texture_sdl2::get_pixel_32bit(int x, int y )
    {
        /*
        if( pixel_values_are_stored && sdlTexSurface != NULL  )
        {
            return sdl2_surface_ex::get_pixel32_t( sdlTexSurface, x, y );
        }
        */
        return 0; //up to the child class to implement
    }

    void texture_sdl2::get_pixel_values(int x, int y, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a )
    {
        /*
        if( pixel_values_are_stored && sdlTexSurface != NULL  )
        {
            uint32_t pixel = sdl2_surface_ex::get_pixel32_t( sdlTexSurface, x, y );
            Uint8 rr = 0, gg = 0, bb = 0, aa = 0;
            SDL_GetRGBA( pixel,sdlTexSurface->format,&rr,&gg,&bb, &aa);
            *r = (uint8_t)rr;
            *g = (uint8_t)gg;
            *b = (uint8_t)bb;
            *a = (uint8_t)aa;
        }
        */
    }

    renderer_system_sdl2 * texture_sdl2::get_gpe_renderer_sdl2(renderer_base * renderer)
    {
        if( renderer == NULL)
        {
            return NULL;
        }
        if( renderer->get_renderer_type() == "sdl")
        {
            renderer_system_sdl2 * gpeSDLRenderer = (renderer_system_sdl2 * )renderer;
            return gpeSDLRenderer;
        }
        return NULL;
    }

    SDL_Renderer * texture_sdl2::get_sdl2_renderer(renderer_base * renderer)
    {
        renderer_system_sdl2 * sdlRenderer = get_gpe_renderer_sdl2( renderer);

        if( sdlRenderer == NULL)
        {
            return NULL;
        }
        return sdlRenderer->get_sdl2_renderer();
    }

    SDL_Texture * texture_sdl2::get_sdl2_texture()
    {
        /*
        if( render_texture_effect && texImg_effect != NULL )
        {
            return texImg_effect;
        }
        */
        return texImg;
    }

    void texture_sdl2::load_new_texture( const std::string& file_name, int id, bool transparent, bool useLinearScaling, uint8_t colorkeyR, uint8_t colorkeyG, uint8_t colorkeyB )
    {
        if(texImg!=NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg = NULL;
        }
        is_prerendered = false;
        tex_id=id;
        is_transparent = transparent;
        //The image that's loaded
        if( sff_ex::file_exists(file_name ) == false )
        {
            error_log->report("[Bad] Unable to load filed  <"+file_name+">. Error: FILE_NOT_FOUND.");
            return;
        }
        blend_mode_current = blend_mode_blend;

        //Load the image
        SDL_Surface * loadedImage = sdl2_surface_ex::load_surface_image( file_name.c_str() );

        //If the image loaded
        if( loadedImage == NULL )
        {
            tex_width = 0;
            tex_height = 0;
            file_location ="notfound.png";
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

        if( is_transparent )
        {
            SDL_SetColorKey( loadedImage, SDL_TRUE, SDL_MapRGB( loadedImage->format, colorkeyR, colorkeyG, colorkeyB ) );
        }
        tex_width = loadedImage->w;
        tex_height = loadedImage->h;

        /*
        SDL_Color gameboy3 = { 155,188,15, 255};
        SDL_Color gameboy2 = { 139,172,15, 255};
        SDL_Color gameboy1 = { 48,98,48, 255};
        SDL_Color gameboy0 = { 15,56,15, 255};

        SDL_Surface * sephia_image = sdl2_surface_ex::surface_modify_2bit( loadedImage, gameboy0, gameboy1, gameboy2, gameboy3 );

        if( loadedImage != NULL )
        {
            SDL_FreeSurface( loadedImage );
            loadedImage = NULL;
        }*/

        texImg = sdl2_surface_ex::create_texture_from_surface( renderer_main_sdl2->get_sdl2_renderer() , loadedImage );
        file_location = file_name;

        tex_id = id;

        if( loadedImage != NULL )
        {
            SDL_FreeSurface( loadedImage );
            loadedImage = NULL;
        }

        if(texImg==NULL)
        {
            tex_width = 0;
            tex_height = 0;
            file_location ="notfound.png";
            error_log->report("[Bad] Unable to load file loacated at <"+file_name+">. Error: "+IMG_GetError()+".\n");
        }
    }

    bool texture_sdl2::modify_texture_grayscale( )
    {
        return false;
    }

    bool texture_sdl2::modify_texture_invert( )
    {
        return false;
    }

    bool texture_sdl2::modify_texture_merge_color_rgba( color * color_key, float amount )
    {
        return false;
    }

    bool texture_sdl2::modify_texture_recolor_rgba(  color * color_key, float amount )
    {
        return false;
    }

    bool texture_sdl2::modify_texture_remove_color_rgba( color * color_key )
    {
        return false;
    }

    bool texture_sdl2::modify_texture_selective_color( color * selected_color, float difference_allowed )
    {
        return false;
    }

    bool texture_sdl2::modify_texture_selective_color_duo( color * selected_color1, color * selected_color2, float difference_allowed )
    {
        return false;
    }

    bool texture_sdl2::modify_texture_selective_color_trio( color * selected_color1, color * selected_color2, color * selected_color3, float difference_allowed  )
    {
        return false;
    }

    bool texture_sdl2::modify_texture_flip( int flags )
    {
        return false;
    }

    void texture_sdl2::prerender_circle( renderer_base * renderer, int rad, color * circleColor,   Uint8 alpha, int id, bool transparent, bool useLinearScaling, bool isOutline )
    {
        SDL_Renderer * sdlRenderer = get_sdl2_renderer( renderer);

        if( sdlRenderer == NULL || rad <=0 || circleColor == NULL)
        {
            return;
        }
        is_prerendered = true;
        //SDL_Texture textImg
        if(texImg!=NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg = NULL;
        }
        blend_mode_current = blend_mode_blend;
        tex_id=id;
        is_transparent = transparent;
        //The image that's loaded

        tex_width = rad*2;
        tex_height = rad*2;

        file_location = "circle:rad:"+ stg_ex::int_to_string(rad);

        int circle_off_white = 254;

        if( circleColor->get_r() == 254 && circleColor->get_g() == 254 && circleColor->get_b() == 254 )
        {
            circle_off_white = 255;
        }
        SDL_Surface * loadedImage = sdl2_surface_ex::create_clear_surface( rad * 2,  rad * 2, circle_off_white, circle_off_white, circle_off_white, circle_off_white );
        SDL_SetSurfaceRLE( loadedImage,3 );
        SDL_SetColorKey( loadedImage, SDL_TRUE, SDL_MapRGB( loadedImage->format, circle_off_white, circle_off_white, circle_off_white) );

        SDL_SetSurfaceBlendMode( loadedImage, SDL_BLENDMODE_BLEND );

        //If the image loaded
        if( loadedImage == NULL )
        {
            tex_width = 0;
            tex_height = 0;
            file_location ="notfound.png";
            return;
        }
        SDL_SetSurfaceBlendMode( loadedImage, SDL_BLENDMODE_BLEND );
        sdl2_surface_ex::surface_render_circle_color_rgba( loadedImage, rad, rad, rad, circleColor->get_r(), circleColor->get_g(),circleColor->get_b(), 255  );
        //If the image loaded
        //Create an optimized surface
        texImg = sdl2_surface_ex::create_texture_from_surface( sdlRenderer, loadedImage, SDL_PIXELFORMAT_RGBA8888);

        if( loadedImage != NULL )
        {
            SDL_FreeSurface( loadedImage );
            loadedImage = NULL;
        }

        if( texImg!=NULL )
        {
            tex_width = rad*2;
            tex_height = rad*2;
            SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_BLEND  );
        }
        else
        {
            tex_width = 0;
            tex_height = 0;
            file_location ="notfound.png";
        }
    }

    void texture_sdl2::prerender_triangle( renderer_base * renderer, shape_triangle2d,  color * circleColor,  uint8_t alpha )
    {
        is_prerendered = true;
    }

    void texture_sdl2::prerender_rectangle( renderer_base * renderer, int w, int h, color * color_new, int id, bool transparent, bool useLinearScaling , bool isOutline)
    {

        if( renderer == NULL)
        {
            return;
        }
        SDL_Renderer * sdlRenderer = get_sdl2_renderer( renderer);
        if( sdlRenderer == NULL)
        {
            return;
        }

        //is_prerendered = true;
        if(texImg!=NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg = NULL;
        }
        tex_id=id;
        is_transparent = transparent;
        //The image that's loaded
        if( w > 0 && h > 0  )
        {
            blend_mode_current = blend_mode_blend;
            SDL_Surface * loadedImage = NULL;
            if( color_new!=NULL )
            {
                loadedImage = sdl2_surface_ex::create_filled_surface_rgba( w, h,color_new->get_r(),color_new->get_g(),color_new->get_b() );
            }
            else
            {
                loadedImage = sdl2_surface_ex::create_filled_surface_rgba( w, h, c_gray->get_r(),c_gray->get_g(),c_gray->get_b() );
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
                tex_width = loadedImage->w;
                tex_height = loadedImage->h;
                texImg = sdl2_surface_ex::create_texture_from_surface( sdlRenderer, loadedImage );
                file_location = "";

                tex_id = id;
                if(texImg==NULL)
                {
                    tex_width = 0;
                    tex_height = 0;
                    file_location ="notfound.png";
                    //error_log->report("[Bad] Unable to load file loacated at <"+file_name+">. Error: "+IMG_GetError()+".\n");
                }
                else
                {
                    //error_log->report("[GOOD] Loaded filed loacated at <"+file_name+"> with <"+ stg_ex::int_to_string(tex_width)+" , "+ stg_ex::int_to_string(tex_height)+"> dimensions.");
                }
                if( loadedImage != NULL )
                {
                    SDL_FreeSurface( loadedImage );
                    loadedImage = NULL;
                }
            }
            else
            {
                tex_width = 0;
                tex_height = 0;
                file_location ="notfound.png";
                //error_log->report("[Bad] Unable to load filed loacated at <"+file_name+">. Error: "+IMG_GetError()+".");
            }


        }
        else
        {
            //error_log->report("[Bad] Unable to load filed  <"+file_name+">. Error: FILE_NOT_FOUND.");
        }
    }

    void texture_sdl2::render_align(  int x, int y, int alignment_h, int alignment_v, gpe::shape_rect* clip, color * render_color, int alpha )
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

    void texture_sdl2::render_align_resized( int x, int y, int new_width, int new_height,  int alignment_h, int alignment_v, gpe::shape_rect* clip,color * render_color, int alpha )
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

    void texture_sdl2::render_tex(  int x, int y, gpe::shape_rect* clip , int alpha)
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect render_rect = { x, y, tex_width, tex_height };

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

                SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect);
            }
            else
            {
                SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),texImg,NULL, &render_rect);
            }
        }
    }

    void texture_sdl2::render_tex_colored(  int x, int y, color * render_color, int alpha , gpe::shape_rect* clip   )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect render_rect = { x, y, tex_width, tex_height };
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

                SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect);
            }
            else
            {
                SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),texImg,NULL, &render_rect);
            }
        }
    }

    bool texture_sdl2::render_tex_quad(   shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha  )
    {
        return render_tex_quad_clipped( p1, p2, p3, p4, render_color, alpha, nullptr );
    }

    bool texture_sdl2::render_tex_quad_clipped(   shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha, shape_rect * clip )
    {
        //This function assumes there are at least 4 vertices
		if( texImg == NULL )
		{
			return false;
		}

		if( tex_width < 8 || tex_height < 8 )
		{
			return false; //For this function we need at least 16x16px...
		}

		//The 6 vertices used to draw 2 triangles to create a quad
		SDL_Vertex vert[4];

		//Vertices must move in a clock-wise manner, thus the last 2 are essentially flipped.

        //Top left vert
		vert[0].position.x = p1.x;
		vert[0].position.y = p1.y;

		//Top right vert
		vert[1].position.x = p2.x;
		vert[1].position.y = p2.y;

		//Bottom left vert
		vert[2].position.x = p3.x;
		vert[2].position.y = p3.y;

		//Second triangle
		//Top left ( 2nd triangle) vertex
		vert[3].position.x = p4.x;
		vert[3].position.y = p4.y;

		if( clip != nullptr )
        {
            //top-left
            vert[0].tex_coord.x = clip->get_x() / tex_width;
            vert[0].tex_coord.y = clip->get_y() / tex_height;

            //top-right
            vert[1].tex_coord.x = clip->get_x2() / tex_width;
            vert[1].tex_coord.y = clip->get_y()  / tex_height;
            //Bottom right vert
            vert[2].tex_coord.x = clip->get_x2() / tex_width;;
            vert[2].tex_coord.y = clip->get_y2() / tex_height;

            //Bottom left vert
            vert[3].tex_coord.x = clip->get_x()  / tex_width;
            vert[3].tex_coord.y = clip->get_y2() / tex_height;
        }
        else
        {
            //top-left
            vert[0].tex_coord.x = 0;
            vert[0].tex_coord.y = 0;

            //top-right
            vert[1].tex_coord.x = 1;
            vert[1].tex_coord.y = 0;

            //Bottom right vert
            vert[2].tex_coord.x = 1;
            vert[2].tex_coord.y = 1;

            //Bottom left vert
            vert[3].tex_coord.x = 0;
            vert[3].tex_coord.y = 1;
        }

		int i_col = 0;
		if( render_color != NULL )
		{
			for(  i_col =0; i_col < 4; i_col++ )
			{
				vert[i_col].color.r = render_color->get_r();
				vert[i_col].color.g = render_color->get_g();
				vert[i_col].color.b = render_color->get_b();
				vert[i_col].color.a = alpha;
			}
		}
		else
		{
			for( i_col = 0; i_col < 4; i_col++ )
			{
				vert[i_col].color.r = 255;
				vert[i_col].color.g = 255;
				vert[i_col].color.b = 255;
				vert[i_col].color.a = 255;
			}
		}

		int new_indices[ 6];
		new_indices[0] = 0;
		new_indices[1] = 1;
		new_indices[2] = 3;
		new_indices[3] = 1;
		new_indices[4] = 2;
		new_indices[5] = 3;

    	return ( SDL_RenderGeometry( renderer_main_sdl2->get_sdl2_renderer(), texImg, vert, 4, new_indices, 6 ) == 0 );
    }

    void texture_sdl2::render_tex_resized(  int x, int y, float new_width, float new_height, gpe::shape_rect* clip, color * render_color, int alpha )
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
                if( tex_width !=0 && tex_height!= 0 )
                {
                    float foundX_scale = new_width/(float)tex_width;
                    float foundY_scale = new_height/(float)tex_height;
                    render_tex_scaled( x, y, foundX_scale, foundY_scale,clip, render_color,alpha);
                }
            }
            return;
        }
    }

    void texture_sdl2::render_tex_scaled(  int x, int y, float x_scale, float y_scale, gpe::shape_rect* clip, color * render_color, int alpha )
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

                if( sdlClip.x > tex_width)
                {
                    sdlClip.x = 0;
                }

                if( sdlClip.y > tex_height )
                {
                    sdlClip.y = 0;
                }

                if( sdlClip.w > tex_width)
                {
                    sdlClip.w = tex_width;
                }

                if( sdlClip.h > tex_height )
                {
                    sdlClip.h = tex_height;
                }

                if( sdlClip.x+sdlClip.w > tex_width )
                {
                    sdlClip.w = tex_width - sdlClip.x;
                }

                if( sdlClip.y+sdlClip.h > tex_height )
                {
                    sdlClip.h = tex_height - sdlClip.y;
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
                        SDL_RenderCopyEx(renderer_main_sdl2->get_sdl2_renderer(), texImg, &sdlClip,&render_rect,0,NULL,renderer_main_sdl2->bothFlip );

                    }
                    else
                    {
                        SDL_RenderCopyEx(renderer_main_sdl2->get_sdl2_renderer(), texImg, &sdlClip,&render_rect,0,NULL,renderer_main_sdl2->horiFlip );
                    }
                }
                else if( flipVert)
                {
                    SDL_RenderCopyEx(renderer_main_sdl2->get_sdl2_renderer(), texImg, &sdlClip,&render_rect,0,NULL, renderer_main_sdl2->vertFlip );
                }
                else
                {
                    SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect);
                }
            }
            else
            {
                if( x_scale < 0 )
                {
                    flipHori = true;
                    new_width = (float)tex_width * -x_scale;
                }
                else
                {
                    new_width = (float)tex_width * x_scale;
                }

                if( y_scale < 0 )
                {
                    flipVert = true;
                    new_height = (float)tex_height * -y_scale;
                }
                else
                {
                    new_height = (float)tex_height * y_scale;
                }
                SDL_Rect render_rect = { x, y, new_width, new_height };
                if( flipHori )
                {
                    if( flipVert )
                    {
                        SDL_RenderCopyEx(renderer_main_sdl2->get_sdl2_renderer(), texImg, NULL,&render_rect,0,NULL, renderer_main_sdl2->bothFlip );
                    }
                    else
                    {
                        SDL_RenderCopyEx(renderer_main_sdl2->get_sdl2_renderer(), texImg, NULL,&render_rect,0,NULL, renderer_main_sdl2->horiFlip );
                    }
                }
                else if( flipVert)
                {
                    SDL_RenderCopyEx(renderer_main_sdl2->get_sdl2_renderer(), texImg, NULL,&render_rect,0,NULL, renderer_main_sdl2->vertFlip );
                }
                else
                {
                    SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),texImg,NULL, &render_rect);
                }
            }
        }
    }

    void texture_sdl2::render_tex_rotated(  int x, int y, float render_angle, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect render_rect = { x-tex_width/2, y-tex_height/2, tex_width, tex_height };
            change_color(render_color);
            set_alpha( alpha );
            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;

                SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect, -render_angle,NULL,SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,NULL, &render_rect, -render_angle,NULL,SDL_FLIP_NONE );
            }
        }
    }

    void texture_sdl2::render_tex_rotated_at_point(  int x, int y, float render_angle, int point_x, int point_y, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect render_rect = { x-tex_width/2, y-tex_height/2, tex_width, tex_height };
            change_color(render_color);
            set_alpha( alpha );

            if( point_x < 0 )
            {
                if( point_x == gpe::fa_center )
                {
                    renderer_main_sdl2->defaultPoint.y = -tex_width/2;
                }
                else if( point_x== gpe::fa_right )
                {
                    renderer_main_sdl2->defaultPoint.y = -tex_width;
                }

            }
            else if( point_x < tex_width )
            {
                renderer_main_sdl2->defaultPoint.y = point_x;
            }
            else
            {
                renderer_main_sdl2->defaultPoint.y = 0;
            }

            if( point_y < 0 )
            {
                if( point_y == gpe::fa_middle )
                {
                    renderer_main_sdl2->defaultPoint.y = tex_height/2;
                }
                else if( point_y== gpe::fa_bottom )
                {
                    renderer_main_sdl2->defaultPoint.y = tex_height;
                }

            }
            else if( point_y < tex_height )
            {
                renderer_main_sdl2->defaultPoint.y = point_y;
            }
            else
            {
                renderer_main_sdl2->defaultPoint.y = 0;
            }

            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;

                SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect, -render_angle,&renderer_main_sdl2->defaultPoint,SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl2->defaultPoint,SDL_FLIP_NONE);
            }
        }
    }

    void texture_sdl2::render_tex_special(  int x, int y, float render_angle, int new_width, int new_height, color * render_color, gpe::shape_rect* clip , int alpha )
    {
        if( texImg==NULL || alpha <= 0 )
        {
            return;
        }

        if( new_width <=0.f)
        {
            new_width = tex_width;
        }
        if( new_height<=0.f)
        {
            new_height = tex_height;
        }

        SDL_Rect render_rect = { x-new_width/2,y-new_height/2, new_width, new_height };

        change_color(render_color);
        set_alpha( alpha );

        renderer_main_sdl2->defaultPoint.x = 0;
        renderer_main_sdl2->defaultPoint.y = 0;

        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect, -render_angle, NULL,SDL_FLIP_NONE);
        }
        else
        {
            SDL_Rect sdlClip;
            sdlClip.x = 0;
            sdlClip.y = 0;
            sdlClip.w = tex_width;
            sdlClip.h = tex_height;
            SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect, -render_angle, NULL ,SDL_FLIP_NONE );
        }
    }

    void texture_sdl2::render_tex_special_at_point(  int x, int y, float render_angle, int point_x, int point_y,int new_width, int new_height, color * render_color, gpe::shape_rect* clip , int alpha )
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
                new_width = tex_width;
            }
            if( new_height==0)
            {
                new_height = tex_height;
            }
            SDL_Rect render_rect = { x-new_width, y-new_height, new_width, new_height };
            change_color(render_color);
            set_alpha( alpha );
            if( point_x < 0 )
            {
                if( point_x == gpe::fa_center )
                {
                    renderer_main_sdl2->defaultPoint.x = new_width/2;
                }
                else if( point_x == gpe::fa_right )
                {
                    renderer_main_sdl2->defaultPoint.x = new_width;
                }
            }
            else if( point_x < new_width )
            {
                renderer_main_sdl2->defaultPoint.x = point_x;
            }
            else
            {
                renderer_main_sdl2->defaultPoint.x = new_width / 2;
            }

            if( point_y < 0 )
            {
                if( point_y == gpe::fa_middle )
                {
                    renderer_main_sdl2->defaultPoint.y = new_height/2;
                }
                else if( point_y == gpe::fa_bottom )
                {
                    renderer_main_sdl2->defaultPoint.y = new_height;
                }
            }
            else if( point_y < new_height )
            {
                renderer_main_sdl2->defaultPoint.y = point_y;
            }
            else
            {
                renderer_main_sdl2->defaultPoint.y = new_height / 2;
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
                        renderer_main_sdl2->defaultPoint.x = sdlClip.w/2;
                    }
                    else if( point_x== gpe::fa_right )
                    {
                        renderer_main_sdl2->defaultPoint.x = sdlClip.w;
                    }
                }
                else if( point_x < sdlClip.w )
                {
                    renderer_main_sdl2->defaultPoint.x = point_x;
                }
                else
                {
                    renderer_main_sdl2->defaultPoint.x = 0;
                }

                if( point_y < 0 )
                {
                    if( point_y == gpe::fa_middle )
                    {
                        renderer_main_sdl2->defaultPoint.y = sdlClip.h /2;
                    }
                    else if( point_y == gpe::fa_bottom )
                    {
                        renderer_main_sdl2->defaultPoint.y = -sdlClip.h ;
                    }

                }
                else if( point_y < sdlClip.h  )
                {
                    renderer_main_sdl2->defaultPoint.y = point_y;
                }
                else
                {
                    renderer_main_sdl2->defaultPoint.y = 0;
                }
                */
                if( flipHori )
                {
                    if( flipVert )
                    {
                        SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect, -render_angle,&renderer_main_sdl2->defaultPoint,renderer_main_sdl2->bothFlip);
                    }
                    else
                    {
                        SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect, -render_angle,&renderer_main_sdl2->defaultPoint,renderer_main_sdl2->horiFlip);
                    }
                }
                else if( flipVert)
                {
                    SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect, -render_angle,&renderer_main_sdl2->defaultPoint, renderer_main_sdl2->vertFlip);
                }
                else
                {
                    //&renderer_main_sdl2->defaultPoint
                    SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,&sdlClip, &render_rect, -render_angle, NULL, SDL_FLIP_NONE);
                }
            }
            else if( flipHori )
            {
                if( flipVert )
                {
                    SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl2->defaultPoint,renderer_main_sdl2->bothFlip);
                }
                else
                {
                    SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl2->defaultPoint,renderer_main_sdl2->horiFlip);
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl2->defaultPoint, renderer_main_sdl2->vertFlip);
            }
            else
            {
                SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),texImg,NULL, &render_rect, -render_angle,&renderer_main_sdl2->defaultPoint,SDL_FLIP_NONE  );
            }
        }
    }


    bool texture_sdl2::reset_effect_texture()
    {
        return false;
    }

    int texture_sdl2::save_as_bmp( const std::string& file_location, bool save_effect )
    {
        return 0;
    }

    int texture_sdl2::save_as_png( const std::string& file_location, bool save_effect )
    {
        return 0;
    }

    void texture_sdl2::set_alpha( int alpha )
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

    void texture_sdl2::set_blend_mode( int blend_mode_new)
    {
        if( texImg!=NULL )//&& blend_mode_current!=blend_mode_new)
        {
            blend_mode_current = blend_mode_new;
            switch( blend_mode_new)
            {
                case blend_mode_add:
                    SDL_SetTextureBlendMode(texImg,SDL_BLENDMODE_ADD );
                break;

                case blend_mode_mod:
                    if( is_prerendered )
                    {
                        SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_MOD);
                        //SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_MUL);
                    }
                    else
                    {
                        SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_MOD);
                    }
                break;

                case blend_mode_mul:
                    SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_MUL);
                break;

                case blend_mode_none:
                    if( is_prerendered )
                    {
                        SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_BLEND);
                        set_alpha(255);
                    }
                    else
                    {
                        SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_NONE  );
                    }
                    break;

                default:
                    SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_BLEND);
                    blend_mode_current = blend_mode_blend;
                break;
            }
        }
    }

    void texture_sdl2::set_pixel_32bit( int x, int y, uint32_t pixel )
    {

    }

    void texture_sdl2::set_pixel_values( int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a )
    {

    }

    bool texture_sdl2::update_pixels()
    {
        return false;
    }
}
