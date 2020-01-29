/*
GPE_Texture_Target_SDL.cpp
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

#include "GPE_Texture_Target_SDL.h"

GPE_Texture_Target_SDL::GPE_Texture_Target_SDL()
{
    textureType = "target-sdl";
    sdlTargetTexture = NULL;
}

GPE_Texture_Target_SDL::~GPE_Texture_Target_SDL()
{
    if( sdlTargetTexture != NULL )
    {
        SDL_DestroyTexture( sdlTargetTexture );
        sdlTargetTexture = NULL;
    }
}

void GPE_Texture_Target_SDL::change_alpha( Uint8 alpha  )
{
    if( sdlTargetTexture!=NULL )
    {
        SDL_SetTextureAlphaMod( sdlTargetTexture,alpha);
    }
}

void GPE_Texture_Target_SDL::change_color(GPE_Color * newColor)
{
    if( sdlTargetTexture==NULL || newColor == NULL)
    {
        return;
    }
    if( newColor->get_r() == currentR && newColor->get_g() == currentG && newColor->get_b() == currentB )
    {
            return;
    }
    SDL_SetTextureColorMod( sdlTargetTexture, newColor->get_r(), newColor->get_g(), newColor->get_b() );
    currentR = newColor->get_r();
    currentG = newColor->get_g();
    currentB = newColor->get_b();
}

void GPE_Texture_Target_SDL::change_color( Uint8 red, Uint8 green, Uint8 blue )
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

GPE_Renderer_SDL * GPE_Texture_Target_SDL::get_gpe_renderer_sdl(GPE_Renderer_Base * renderer)
{
    if( renderer == NULL)
    {
        return NULL;
    }
    if( renderer->get_renderer_type() == "sdl")
    {
        GPE_Renderer_SDL * gpeSDLRenderer = (GPE_Renderer_SDL * )renderer;
        return gpeSDLRenderer;
    }
    return NULL;
}

SDL_Renderer * GPE_Texture_Target_SDL::get_sdl_renderer(GPE_Renderer_Base * renderer)
{
    GPE_Renderer_SDL * sdlRenderer = get_gpe_renderer_sdl( renderer);

    if( sdlRenderer == NULL)
    {
        return NULL;
    }
    return sdlRenderer->get_sdl_renderer();
}

SDL_Texture * GPE_Texture_Target_SDL::get_sdl_texture()
{
    return sdlTargetTexture;
}

GPE_Texture_Base * GPE_Texture_Target_SDL::produce_clone()
{
    return new GPE_Texture_Target_SDL();
}

void GPE_Texture_Target_SDL::render_overlay(  GPE_Renderer_Base * renderer,int x, int y,GPE_Rect* clip , int alpha  )
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
    SDL_Rect renderRect = { x, y, texWid, texHeight };

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

        renderRect.w = clip->w;
        renderRect.h = clip->h;

        SDL_RenderCopy( sdlRenderer,sdlTargetTexture,&sdlClip, &renderRect);
    }
    else
    {
        SDL_RenderCopy( sdlRenderer,sdlTargetTexture,NULL, &renderRect);
    }
}

void GPE_Texture_Target_SDL::render_overlay_scaled( GPE_Renderer_Base * renderer, int x, int y,float xScale, float yScale, GPE_Rect* clip, GPE_Color * rendColor, int alpha )
{
    if( xScale <= 0 || yScale <= 0 || sdlTargetTexture == NULL || alpha == 0 || renderer == NULL)
    {
        return;
    }
    SDL_Renderer * sdlRenderer = get_sdl_renderer(renderer);

    if( sdlRenderer == NULL)
    {
        return;
    }
    SDL_Rect renderRect = { x, y, texWid, texHeight };

    change_color(rendColor);
    set_alpha( alpha );

    int newWidth = 0;
    int newHeight = 0;
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

        if( xScale < 0 )
        {
            newWidth = (float)sdlClip.w * -xScale;
            flipHori = true;
        }
        else
        {
            newWidth = (float)sdlClip.w * xScale;
        }

        if( yScale < 0 )
        {
            newHeight = (float)sdlClip.h * -yScale;
            flipVert = true;
        }
        else
        {
            newHeight = (float)sdlClip.h * yScale;
        }
        SDL_Rect renderRect = { x, y, newWidth, newHeight };
        if( flipHori )
        {
            if( flipVert )
            {
                SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, &sdlClip,&renderRect,0,NULL,GPE_MAIN_RENDERER_SDL->bothFlip );

            }
            else
            {
                SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, &sdlClip,&renderRect,0,NULL,GPE_MAIN_RENDERER_SDL->horiFlip );
            }
        }
        else if( flipVert)
        {
            SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, &sdlClip,&renderRect,0,NULL, GPE_MAIN_RENDERER_SDL->vertFlip );
        }
        else
        {
            SDL_RenderCopy( sdlRenderer,sdlTargetTexture,&sdlClip, &renderRect);
        }
    }
    else
    {
        if( xScale < 0 )
        {
            flipHori = true;
            newWidth = (float)texWid * -xScale;
        }
        else
        {
            newWidth = (float)texWid * xScale;
        }

        if( yScale < 0 )
        {
            flipVert = true;
            newHeight = (float)texHeight * -yScale;
        }
        else
        {
            newHeight = (float)texHeight * yScale;
        }
        SDL_Rect renderRect = { x, y, newWidth, newHeight };
        if( flipHori )
        {
            if( flipVert )
            {
                SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, NULL,&renderRect,0,NULL, GPE_MAIN_RENDERER_SDL->bothFlip );
            }
            else
            {
                SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, NULL,&renderRect,0,NULL, GPE_MAIN_RENDERER_SDL->horiFlip );
            }
        }
        else if( flipVert)
        {
            SDL_RenderCopyEx( sdlRenderer, sdlTargetTexture, NULL,&renderRect,0,NULL, GPE_MAIN_RENDERER_SDL->vertFlip );
        }
        else
        {
            SDL_RenderCopy( sdlRenderer,sdlTargetTexture,NULL, &renderRect);
        }
    }
}

void GPE_Texture_Target_SDL::resize_target(GPE_Renderer_Base * renderer, int w, int h, int id, bool useLinearScaling  )
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
            //GPE_Report("[Bad] Unable to load file loacated at <"+fileName+">. Error: "+IMG_GetError()+".\n");
        }
        else
        {
            SDL_SetTextureBlendMode( sdlTargetTexture, SDL_BLENDMODE_BLEND  );
        }
    }
    else
    {
        //GPE_Report("[Bad] Unable to load filed  <"+fileName+">. Error: FILE_NOT_FOUND.");
    }
}


void GPE_Texture_Target_SDL::set_blend_mode( int newBlendMode)
{
    if( sdlTargetTexture == NULL )//&& currentBlendMode!=newBlendMode)
    {
        return;
    }
    currentBlendMode = newBlendMode;
    switch( newBlendMode)
    {
        case blend_mode_add:
            SDL_SetTextureBlendMode(sdlTargetTexture,SDL_BLENDMODE_ADD );
            break;

        case blend_mode_mod:
            SDL_SetTextureBlendMode(sdlTargetTexture, SDL_BLENDMODE_MOD);
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
