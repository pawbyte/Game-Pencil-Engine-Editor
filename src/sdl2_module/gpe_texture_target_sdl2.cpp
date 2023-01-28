/*
gpe_texture_target_sdl2.cpp
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

#include "gpe_texture_target_sdl2.h"

namespace gpe
{
    texture_target_sdl2::texture_target_sdl2( bool store_pixels )
    {

        texture_type = "target-sdl";
        sdlTargetTexture = NULL;
        //sdlTexSurface = NULL;
        pixel_values_are_stored = store_pixels;
    }

    texture_target_sdl2::~texture_target_sdl2()
    {
        if( sdlTargetTexture != NULL )
        {
            SDL_DestroyTexture( sdlTargetTexture );
            sdlTargetTexture = NULL;
        }
    }

    void texture_target_sdl2::change_alpha( Uint8 alpha  )
    {
        if( sdlTargetTexture!=NULL )
        {
            SDL_SetTextureAlphaMod( sdlTargetTexture,alpha);
        }
    }

    void texture_target_sdl2::change_color( color * color_new)
    {
        if( sdlTargetTexture==NULL || color_new == NULL)
        {
            return;
        }
        if( color_new->get_r() == current_color_r && color_new->get_g() == current_color_g && color_new->get_b() == current_color_b )
        {
                return;
        }
        SDL_SetTextureColorMod( sdlTargetTexture, color_new->get_r(), color_new->get_g(), color_new->get_b() );
        current_color_r = color_new->get_r();
        current_color_g = color_new->get_g();
        current_color_b = color_new->get_b();
    }

    void texture_target_sdl2::change_color( Uint8 red, Uint8 green, Uint8 blue )
    {
        if(sdlTargetTexture==NULL)
        {
            return;
        }
        if( red == current_color_r && green == current_color_g && blue == current_color_b )
        {
            return;
        }
        SDL_SetTextureColorMod( sdlTargetTexture, red, green, blue );
        current_color_r = red;
        current_color_g = green;
        current_color_b = blue;
    }

    uint32_t texture_target_sdl2::get_pixel_32bit(int x, int y )
    {
        /*
        if( pixel_values_are_stored && sdlTexSurface != NULL  )
        {
            return sdl2_surface_ex::get_pixel32( sdlTexSurface, x, y );
        }
        */
        return 0;
    }

    void texture_target_sdl2::get_pixel_values(int x, int y, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a )
    {
        /*
        if( pixel_values_are_stored && sdlTexSurface != NULL  )
        {
            Uint32 pixel = sdl2_surface_ex::get_pixel32( sdlTexSurface, x, y );
            Uint8 rr = 0, gg = 0, bb = 0, aa = 0;
            SDL_GetRGBA( pixel,sdlTexSurface->format,&rr,&gg,&bb, &aa);
            *r = (uint8_t)rr;
            *g = (uint8_t)gg;
            *b = (uint8_t)bb;
            *a = (uint8_t)aa;
        }
        */
    }

    renderer_system_sdl2 * texture_target_sdl2::get_gpe_renderer_sdl2(renderer_base * renderer)
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

    SDL_Renderer * texture_target_sdl2::get_sdl2_renderer(renderer_base * renderer)
    {
        renderer_system_sdl2 * sdlRenderer = get_gpe_renderer_sdl2( renderer);

        if( sdlRenderer == NULL)
        {
            return NULL;
        }
        return sdlRenderer->get_sdl2_renderer();
    }

    SDL_Texture * texture_target_sdl2::get_sdl2_texture()
    {
        return sdlTargetTexture;
    }

    texture_base * texture_target_sdl2::create_new()
    {
        return new texture_target_sdl2();
    }

    void texture_target_sdl2::render_overlay(  renderer_base * renderer,int x, int y, gpe::shape_rect* clip , int alpha  )
    {
        if( sdlTargetTexture == NULL || alpha == 0 ||  renderer == NULL)
        {
            return;
        }
        SDL_Renderer * sdlRenderer = get_sdl2_renderer(renderer);

        if( sdlRenderer == NULL)
        {
            return;
        }
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

            SDL_RenderCopy( sdlRenderer,sdlTargetTexture,&sdlClip, &render_rect);
        }
        else
        {
            SDL_RenderCopy( sdlRenderer,sdlTargetTexture,NULL, &render_rect);
        }
    }

    void texture_target_sdl2::render_overlay_scaled( renderer_base * renderer, int x, int y,float x_scale, float y_scale, gpe::shape_rect* clip, color * render_color, int alpha )
    {
        if( x_scale <= 0 || y_scale <= 0 || sdlTargetTexture == NULL || alpha == 0 || renderer == NULL)
        {
            return;
        }
        SDL_Renderer * sdlRenderer = get_sdl2_renderer(renderer);

        if( sdlRenderer == NULL)
        {
            return;
        }
        SDL_Rect render_rect = { x, y, tex_width, tex_height };

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
                    SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, &sdlClip,&render_rect,0,NULL,renderer_main_sdl2->bothFlip );

                }
                else
                {
                    SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, &sdlClip,&render_rect,0,NULL,renderer_main_sdl2->horiFlip );
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, &sdlClip,&render_rect,0,NULL, renderer_main_sdl2->vertFlip );
            }
            else
            {
                SDL_RenderCopy( sdlRenderer,sdlTargetTexture,&sdlClip, &render_rect);
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
                    SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, NULL,&render_rect,0,NULL, renderer_main_sdl2->bothFlip );
                }
                else
                {
                    SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, NULL,&render_rect,0,NULL, renderer_main_sdl2->horiFlip );
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, NULL,&render_rect,0,NULL, renderer_main_sdl2->vertFlip );
            }
            else
            {
                SDL_RenderCopy( sdlRenderer,sdlTargetTexture,NULL, &render_rect);
            }
        }
    }

    bool texture_target_sdl2::reset_effect_texture()
    {
        return false;
    }

    void texture_target_sdl2::resize_target(renderer_base * renderer, int w, int h, int id, bool useLinearScaling  )
    {
        if(sdlTargetTexture!=NULL)
        {
            //only creates on need
            if( tex_width > w && tex_height > h)
            {
                return;
            }
            SDL_DestroyTexture(sdlTargetTexture);
            sdlTargetTexture = NULL;
        }
        tex_id=id;
        //The image that's loaded
        blend_mode_current = blend_mode_blend;
        if( w > 0 && h > 0  )
        {
            tex_width = w + 128;
            tex_height = h + 128;
            sdlTargetTexture = SDL_CreateTexture( get_sdl2_renderer(renderer), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tex_width, tex_height );
            tex_id = id;
            if(sdlTargetTexture==NULL)
            {
                tex_width = 0;
                tex_height = 0;
                //error_log->report("[Bad] Unable to load file loacated at <"+file_name+">. Error: "+IMG_GetError()+".\n");
            }
            else
            {
                SDL_SetTextureBlendMode( sdlTargetTexture, SDL_BLENDMODE_BLEND  );
            }
        }
        else
        {
            //error_log->report("[Bad] Unable to load filed  <"+file_name+">. Error: FILE_NOT_FOUND.");
        }
    }


    int texture_target_sdl2::save_as_bmp( const std::string& file_location, bool save_effect )
    {
        return 0;
    }

    int texture_target_sdl2::save_as_png( const std::string& file_location, bool save_effect )
    {
        return 0;
    }


    void texture_target_sdl2::set_blend_mode( int blend_mode_new)
    {
        if( sdlTargetTexture == NULL )//&& blend_mode_current!=blend_mode_new)
        {
            return;
        }
        blend_mode_current = blend_mode_new;
        switch( blend_mode_new)
        {
            case blend_mode_add:
                SDL_SetTextureBlendMode(sdlTargetTexture,SDL_BLENDMODE_ADD );
            break;

            case blend_mode_mod:
                SDL_SetTextureBlendMode(sdlTargetTexture, SDL_BLENDMODE_MOD);
            break;

            case blend_mode_mul:
                SDL_SetTextureBlendMode(sdlTargetTexture, SDL_BLENDMODE_MUL);
            break;

            case blend_mode_none:
                SDL_SetTextureBlendMode(sdlTargetTexture, SDL_BLENDMODE_NONE  );
                break;

            default:
                SDL_SetTextureBlendMode(sdlTargetTexture, SDL_BLENDMODE_BLEND);
                blend_mode_current = blend_mode_blend;
            break;
        }
    }
}
