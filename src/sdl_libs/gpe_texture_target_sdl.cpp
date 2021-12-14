/*
gpe_texture_target_sdl.cpp
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

#include "gpe_texture_target_sdl.h"

namespace gpe
{
    texture_target_sdl::texture_target_sdl()
    {
        textureType = "target-sdl";
        sdlTargetTexture = NULL;
    }

    texture_target_sdl::~texture_target_sdl()
    {
        if( sdlTargetTexture != NULL )
        {
            SDL_DestroyTexture( sdlTargetTexture );
            sdlTargetTexture = NULL;
        }
    }

    void texture_target_sdl::change_alpha( Uint8 alpha  )
    {
        if( sdlTargetTexture!=NULL )
        {
            SDL_SetTextureAlphaMod( sdlTargetTexture,alpha);
        }
    }

    void texture_target_sdl::change_color( color * color_new)
    {
        if( sdlTargetTexture==NULL || color_new == NULL)
        {
            return;
        }
        if( color_new->get_r() == currentR && color_new->get_g() == currentG && color_new->get_b() == currentB )
        {
                return;
        }
        SDL_SetTextureColorMod( sdlTargetTexture, color_new->get_r(), color_new->get_g(), color_new->get_b() );
        currentR = color_new->get_r();
        currentG = color_new->get_g();
        currentB = color_new->get_b();
    }

    void texture_target_sdl::change_color( Uint8 red, Uint8 green, Uint8 blue )
    {
        if(sdlTargetTexture==NULL)
        {
            return;
        }
        if( red == currentR && green == currentG && blue == currentB )
        {
            return;
        }
        SDL_SetTextureColorMod( sdlTargetTexture, red, green, blue );
        currentR = red;
        currentG = green;
        currentB = blue;
    }

    renderer_system_sdl * texture_target_sdl::get_gpe_renderer_sdl(renderer_base * renderer)
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

    SDL_Renderer * texture_target_sdl::get_sdl_renderer(renderer_base * renderer)
    {
        renderer_system_sdl * sdlRenderer = get_gpe_renderer_sdl( renderer);

        if( sdlRenderer == NULL)
        {
            return NULL;
        }
        return sdlRenderer->get_sdl_renderer();
    }

    SDL_Texture * texture_target_sdl::get_sdl_texture()
    {
        return sdlTargetTexture;
    }

    texture_base * texture_target_sdl::create_new()
    {
        return new texture_target_sdl();
    }

    void texture_target_sdl::render_overlay(  renderer_base * renderer,int x, int y, gpe::shape_rect* clip , int alpha  )
    {
        if( sdlTargetTexture == NULL || alpha == 0 ||  renderer == NULL)
        {
            return;
        }
        SDL_Renderer * sdlRenderer = get_sdl_renderer(renderer);

        if( sdlRenderer == NULL)
        {
            return;
        }
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

            SDL_RenderCopy( sdlRenderer,sdlTargetTexture,&sdlClip, &render_rect);
        }
        else
        {
            SDL_RenderCopy( sdlRenderer,sdlTargetTexture,NULL, &render_rect);
        }
    }

    void texture_target_sdl::render_overlay_scaled( renderer_base * renderer, int x, int y,float x_scale, float y_scale, gpe::shape_rect* clip, color * render_color, int alpha )
    {
        if( x_scale <= 0 || y_scale <= 0 || sdlTargetTexture == NULL || alpha == 0 || renderer == NULL)
        {
            return;
        }
        SDL_Renderer * sdlRenderer = get_sdl_renderer(renderer);

        if( sdlRenderer == NULL)
        {
            return;
        }
        SDL_Rect render_rect = { x, y, texWid, texHeight };

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
                    SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, &sdlClip,&render_rect,0,NULL,renderer_main_sdl->bothFlip );

                }
                else
                {
                    SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, &sdlClip,&render_rect,0,NULL,renderer_main_sdl->horiFlip );
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, &sdlClip,&render_rect,0,NULL, renderer_main_sdl->vertFlip );
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
                    SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, NULL,&render_rect,0,NULL, renderer_main_sdl->bothFlip );
                }
                else
                {
                    SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, NULL,&render_rect,0,NULL, renderer_main_sdl->horiFlip );
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, NULL,&render_rect,0,NULL, renderer_main_sdl->vertFlip );
            }
            else
            {
                SDL_RenderCopy( sdlRenderer,sdlTargetTexture,NULL, &render_rect);
            }
        }
    }

    void texture_target_sdl::resize_target(renderer_base * renderer, int w, int h, int id, bool useLinearScaling  )
    {
        if(sdlTargetTexture!=NULL)
        {
            //only creates on need
            if( texWid > w && texHeight > h)
            {
                return;
            }
            SDL_DestroyTexture(sdlTargetTexture);
            sdlTargetTexture = NULL;
        }
        texId=id;
        //The image that's loaded
        currentBlendMode = blend_mode_blend;
        if( w > 0 && h > 0  )
        {
            texWid = w + 128;
            texHeight = h + 128;
            sdlTargetTexture = SDL_CreateTexture( get_sdl_renderer(renderer), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texWid, texHeight );
            texId = id;
            if(sdlTargetTexture==NULL)
            {
                texWid = 0;
                texHeight = 0;
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


    void texture_target_sdl::set_blend_mode( int blend_mode_new)
    {
        if( sdlTargetTexture == NULL )//&& currentBlendMode!=blend_mode_new)
        {
            return;
        }
        currentBlendMode = blend_mode_new;
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
                currentBlendMode = blend_mode_blend;
            break;
        }
    }
}
