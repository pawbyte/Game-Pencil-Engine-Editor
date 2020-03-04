/*
gpe_texture_sdl.cpp
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

    void texture_sdl::change_color( color * newColor)
    {
        if(texImg!=NULL)
        {
            if( newColor!=NULL)
            {
                if( newColor->get_r() == currentR && newColor->get_g() == currentG && newColor->get_b() == currentB )
                {
                    return;
                }
                SDL_SetTextureColorMod( texImg, newColor->get_r(), newColor->get_g(), newColor->get_b() );
                currentR = newColor->get_r();
                currentG = newColor->get_g();
                currentB = newColor->get_b();
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

    bool texture_sdl::copy_image_source(std::string outDirectoryName)
    {
        if( get_width()>0)
        {
            std::string copyDestinationStr = outDirectoryName+"/"+ stg_ex::get_short_filename(fileLocation,true);
            return sff_ex::file_copy(fileLocation,copyDestinationStr );
        }
        return false;
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

    void texture_sdl::load_new_texture( renderer_base * renderer,std::string fileName, int id, bool transparent, bool useLinearScaling )
    {
        if(texImg!=NULL)
        {
            SDL_DestroyTexture(texImg);
            texImg = NULL;
        }
        texId=id;
        isTransparent = transparent;
        //The image that's loaded
        if( sff_ex::file_exists(fileName ) == false )
        {
            error_log->report("[Bad] Unable to load filed  <"+fileName+">. Error: FILE_NOT_FOUND.");
            return;
        }
        currentBlendMode = blend_mode_blend;

        //Load the image
        SDL_Surface * loadedImage = sdl_surface_ex::load_surface_image( fileName.c_str() );

        //If the image loaded
        if( loadedImage == NULL )
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
            error_log->report("[Bad] Unable to load filed loacated at <"+fileName+">. Error: "+IMG_GetError()+".");
            return;
        }

        //Create an optimized surface
        if( useLinearScaling )
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        }
        else
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        }
        texWid = loadedImage->w;
        texHeight = loadedImage->h;
        texImg = sdl_surface_ex::create_texture_from_surface( get_sdl_renderer( renderer ),loadedImage);
        fileLocation = fileName;

        texId = id;
        if(texImg==NULL)
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
            error_log->report("[Bad] Unable to load file loacated at <"+fileName+">. Error: "+IMG_GetError()+".\n");
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
        SDL_Surface * loadedImage = sdl_surface_ex::create_filled_surface_rgba( rad * 2,  rad * 2, c_gray->get_r(),c_gray->get_g(),c_gray->get_b(), 0 );
        SDL_SetColorKey( loadedImage, SDL_TRUE, SDL_MapRGBA( loadedImage->format, c_gray->get_r(), c_gray->get_g(),c_gray->get_b(),0 ) );
        SDL_SetSurfaceBlendMode( loadedImage, SDL_BLENDMODE_BLEND );

        //If the image loaded
        if( loadedImage == NULL )
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
            return;
        }

        sdl_surface_ex::surface_render_circle_color_rgba( loadedImage, rad, rad, rad, circleColor->get_r(), circleColor->get_g(),circleColor->get_b(), 255  );
        SDL_SetSurfaceBlendMode( loadedImage, SDL_BLENDMODE_BLEND );
        //If the image loaded
        //Create an optimized surface
        texImg = sdl_surface_ex::create_texture_from_surface( sdlRenderer, loadedImage);
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

    void texture_sdl::prerender_rectangle( renderer_base * renderer, int w, int h, color * newColor, int id, bool transparent, bool useLinearScaling , bool isOutline)
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
            if( newColor!=NULL )
            {
                loadedImage = sdl_surface_ex::create_filled_surface_rgba( w, h,newColor->get_r(),newColor->get_g(),newColor->get_b() );
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
                    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
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
                    //error_log->report("[Bad] Unable to load file loacated at <"+fileName+">. Error: "+IMG_GetError()+".\n");
                }
                else
                {
                    //error_log->report("[GOOD] Loaded filed loacated at <"+fileName+"> with <"+ stg_ex::int_to_string(texWid)+" , "+ stg_ex::int_to_string(texHeight)+"> dimensions.");
                }
            }
            else
            {
                texWid = 0;
                texHeight = 0;
                fileLocation ="notfound.png";
                //error_log->report("[Bad] Unable to load filed loacated at <"+fileName+">. Error: "+IMG_GetError()+".");
            }
        }
        else
        {
            //error_log->report("[Bad] Unable to load filed  <"+fileName+">. Error: FILE_NOT_FOUND.");
        }
    }

    texture_base * texture_sdl::create_new()
    {
        return new texture_sdl();
    }


    void texture_sdl::render_align(  int x, int y, int hAlign, int vAlign, gpe::shape_rect* clip, color * rendColor, int alpha )
    {
        if(hAlign==gpe::fa_center)
        {
            x-=get_width()/2;
        }
        else if( hAlign==gpe::fa_right)
        {
            x-= get_width();
        }

        if(vAlign==gpe::fa_middle)
        {
            y-= get_height()/2;
        }
        else if( vAlign==gpe::fa_bottom)
        {
            y-= get_height();
        }
        render_tex_colored( x,y, rendColor, alpha, clip );
    }

    void texture_sdl::render_align_resized( int x, int y, int newWidth, int newHeight,  int hAlign, int vAlign, gpe::shape_rect* clip,color * renderColor, int alpha )
    {
        if(hAlign==gpe::fa_center)
        {
            x-=newWidth/2;
        }
        else if( hAlign==gpe::fa_right)
        {
            x-= newWidth;
        }

        if(vAlign==gpe::fa_middle)
        {
            y-= newHeight/2;
        }
        else if( vAlign==gpe::fa_bottom)
        {
            y-= newHeight;
        }
        render_tex_resized( x,y,newWidth, newHeight,clip, renderColor, alpha );
    }

    void texture_sdl::render_tex(  int x, int y, gpe::shape_rect* clip , int alpha)
    {
        if(texImg!=NULL && alpha > 0 )
        {
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

                SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect);
            }
            else
            {
                SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &renderRect);
            }
        }
    }

    void texture_sdl::render_tex_colored(  int x, int y, color * rendColor, int alpha , gpe::shape_rect* clip   )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect renderRect = { x, y, texWid, texHeight };
            change_color(rendColor);
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

                SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect);
            }
            else
            {
                SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &renderRect);
            }
        }
    }

    void texture_sdl::render_tex_resized(  int x, int y, float newWidth, float newHeight, gpe::shape_rect* clip, color * renderColor, int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            if( clip != NULL )
            {
                if( clip->w !=0 && clip->h!= 0 )
                {
                    float foundXScale = newWidth/clip->w;
                    float foundYScale = newHeight/clip->h;
                    render_tex_scaled( x, y, foundXScale, foundYScale,clip, renderColor,alpha);
                }
            }
            else
            {
                if( texWid !=0 && texHeight!= 0 )
                {
                    float foundXScale = newWidth/(float)texWid;
                    float foundYScale = newHeight/(float)texHeight;
                    render_tex_scaled( x, y, foundXScale, foundYScale,clip, renderColor,alpha);
                }
            }
            return;
        }
    }

    void texture_sdl::render_tex_scaled(  int x, int y, float xScale, float yScale, gpe::shape_rect* clip, color * renderColor, int alpha )
    {
        if( xScale == 0 || yScale == 0 )
        {
            return;
        }

        if(texImg!=NULL)
        {
            change_color(renderColor);
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
                        SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, &sdlClip,&renderRect,0,NULL,renderer_main_sdl->bothFlip );

                    }
                    else
                    {
                        SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, &sdlClip,&renderRect,0,NULL,renderer_main_sdl->horiFlip );
                    }
                }
                else if( flipVert)
                {
                    SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, &sdlClip,&renderRect,0,NULL, renderer_main_sdl->vertFlip );
                }
                else
                {
                    SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect);
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
                        SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, NULL,&renderRect,0,NULL, renderer_main_sdl->bothFlip );
                    }
                    else
                    {
                        SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, NULL,&renderRect,0,NULL, renderer_main_sdl->horiFlip );
                    }
                }
                else if( flipVert)
                {
                    SDL_RenderCopyEx(renderer_main_sdl->get_sdl_renderer(), texImg, NULL,&renderRect,0,NULL, renderer_main_sdl->vertFlip );
                }
                else
                {
                    SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &renderRect);
                }
            }
        }
    }

    void texture_sdl::render_tex_rotated(  int x, int y, float renderAngle, color * renderColor, gpe::shape_rect* clip , int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect renderRect = { x-texWid/2, y-texHeight/2, texWid, texHeight };
            change_color(renderColor);
            set_alpha( alpha );
            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;

                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,NULL,SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,NULL,SDL_FLIP_NONE );
            }
        }
    }

    void texture_sdl::render_tex_rotated_at_point(  int x, int y, float renderAngle, int pointX, int pointY, color * renderColor, gpe::shape_rect* clip , int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            SDL_Rect renderRect = { x-texWid/2, y-texHeight/2, texWid, texHeight };
            change_color(renderColor);
            set_alpha( alpha );

            if( pointX < 0 )
            {
                if( pointX == gpe::fa_center )
                {
                    renderer_main_sdl->defaultPoint.y = -texWid/2;
                }
                else if( pointX== gpe::fa_right )
                {
                    renderer_main_sdl->defaultPoint.y = -texWid;
                }

            }
            else if( pointX < texWid )
            {
                renderer_main_sdl->defaultPoint.y = pointX;
            }
            else
            {
                renderer_main_sdl->defaultPoint.y = 0;
            }

            if( pointY < 0 )
            {
                if( pointY == gpe::fa_middle )
                {
                    renderer_main_sdl->defaultPoint.y = texHeight/2;
                }
                else if( pointY== gpe::fa_bottom )
                {
                    renderer_main_sdl->defaultPoint.y = texHeight;
                }

            }
            else if( pointY < texHeight )
            {
                renderer_main_sdl->defaultPoint.y = pointY;
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

                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&renderer_main_sdl->defaultPoint,SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&renderer_main_sdl->defaultPoint,SDL_FLIP_NONE);
            }
        }
    }

    void texture_sdl::render_tex_special(  int x, int y, float renderAngle, int newWidth, int newHeight, color * renderColor, gpe::shape_rect* clip , int alpha )
    {
        if( texImg==NULL || alpha <= 0 )
        {
            return;
        }

        if( newWidth <=0.f)
        {
            newWidth = texWid;
        }
        if( newHeight<=0.f)
        {
            newHeight = texHeight;
        }
        SDL_Rect renderRect = { x-newWidth/2,y-newHeight/2, newWidth, newHeight };


        change_color(renderColor);
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

            SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle, NULL,SDL_FLIP_NONE);
        }
        else
        {
            SDL_Rect sdlClip;
            sdlClip.x = 0;
            sdlClip.y = 0;
            sdlClip.w = texWid;
            sdlClip.h = texHeight;
            SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle, NULL,SDL_FLIP_NONE );
        }
    }

    void texture_sdl::render_tex_special_at_point(  int x, int y, float renderAngle, int pointX, int pointY,int newWidth, int newHeight, color * renderColor, gpe::shape_rect* clip , int alpha )
    {
        if(texImg!=NULL && alpha > 0 )
        {
            bool flipHori = false,  flipVert = false;
            if( newWidth < 0)
            {
                flipHori = true;
                newWidth*=-1;
            }
            if( newHeight < 0)
            {
                flipVert = true;
                newHeight*=-1;
            }
            if( newWidth ==0)
            {
                newWidth = texWid;
            }
            if( newHeight==0)
            {
                newHeight = texHeight;
            }
            SDL_Rect renderRect = { x-newWidth, y-newHeight, newWidth, newHeight };
            change_color(renderColor);
            set_alpha( alpha );
            if( pointX < 0 )
            {
                if( pointX == gpe::fa_center )
                {
                    renderer_main_sdl->defaultPoint.x = newWidth/2;
                }
                else if( pointX == gpe::fa_right )
                {
                    renderer_main_sdl->defaultPoint.x = newWidth;
                }
            }
            else if( pointX < newWidth )
            {
                renderer_main_sdl->defaultPoint.x = pointX;
            }
            else
            {
                renderer_main_sdl->defaultPoint.x = newWidth / 2;
            }

            if( pointY < 0 )
            {
                if( pointY == gpe::fa_middle )
                {
                    renderer_main_sdl->defaultPoint.y = newHeight/2;
                }
                else if( pointY == gpe::fa_bottom )
                {
                    renderer_main_sdl->defaultPoint.y = newHeight;
                }
            }
            else if( pointY < newHeight )
            {
                renderer_main_sdl->defaultPoint.y = pointY;
            }
            else
            {
                renderer_main_sdl->defaultPoint.y = newHeight / 2;
            }

            if( clip != NULL )
            {
                SDL_Rect sdlClip;

                sdlClip.x = clip->x;
                sdlClip.y = clip->y;
                sdlClip.w = clip->w;
                sdlClip.h = clip->h;
                /*
                if( pointX < 0 )
                {
                    if( pointX == gpe::fa_center )
                    {
                        renderer_main_sdl->defaultPoint.x = sdlClip.w/2;
                    }
                    else if( pointX== gpe::fa_right )
                    {
                        renderer_main_sdl->defaultPoint.x = sdlClip.w;
                    }
                }
                else if( pointX < sdlClip.w )
                {
                    renderer_main_sdl->defaultPoint.x = pointX;
                }
                else
                {
                    renderer_main_sdl->defaultPoint.x = 0;
                }

                if( pointY < 0 )
                {
                    if( pointY == gpe::fa_middle )
                    {
                        renderer_main_sdl->defaultPoint.y = sdlClip.h /2;
                    }
                    else if( pointY == gpe::fa_bottom )
                    {
                        renderer_main_sdl->defaultPoint.y = -sdlClip.h ;
                    }

                }
                else if( pointY < sdlClip.h  )
                {
                    renderer_main_sdl->defaultPoint.y = pointY;
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
                        SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&renderer_main_sdl->defaultPoint,renderer_main_sdl->bothFlip);
                    }
                    else
                    {
                        SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&renderer_main_sdl->defaultPoint,renderer_main_sdl->horiFlip);
                    }
                }
                else if( flipVert)
                {
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&renderer_main_sdl->defaultPoint, renderer_main_sdl->vertFlip);
                }
                else
                {
                    //&renderer_main_sdl->defaultPoint
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle, NULL, SDL_FLIP_NONE);
                }
            }
            else if( flipHori )
            {
                if( flipVert )
                {
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&renderer_main_sdl->defaultPoint,renderer_main_sdl->bothFlip);
                }
                else
                {
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&renderer_main_sdl->defaultPoint,renderer_main_sdl->horiFlip);
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&renderer_main_sdl->defaultPoint, renderer_main_sdl->vertFlip);
            }
            else
            {
                SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&renderer_main_sdl->defaultPoint,SDL_FLIP_NONE  );
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

    void texture_sdl::set_blend_mode( int newBlendMode)
    {
        if( texImg!=NULL )//&& currentBlendMode!=newBlendMode)
        {
            currentBlendMode = newBlendMode;
            switch( newBlendMode)
            {
                case blend_mode_add:
                    SDL_SetTextureBlendMode(texImg,SDL_BLENDMODE_ADD );
                    break;

                case blend_mode_mod:
                    SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_MOD);
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
