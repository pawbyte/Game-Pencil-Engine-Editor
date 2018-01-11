/*
GPE_Engine.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the ԓoftwareԩ, to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ԁS ISԬ WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "GPE_Engine.h"

GPE_Font * LAST_USED_FONT = NULL;
int GLOBAL_FONT_ID = 0;
std::vector < GPE_Font * > loadedGPEFonts;
/*!
\brief Draw filled circle with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the filled circle.
\param y Y coordinate of the center of the filled circle.
\param rad Radius in pixels of the filled circle.
\param r The red value of the filled circle to draw.
\param g The green value of the filled circle to draw.
\param b The blue value of the filled circle to draw.
\param a The alpha value of the filled circle to draw.

\returns Returns 0 on success, -1 on failure.
*/

bool render_circle_filled_rgba(GPE_Renderer * gRender, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    //Modified from SDL_GFX
    Sint16 cx = 0;
    Sint16 cy = rad;
    Sint16 ocx = (Sint16) 0xffff;
    Sint16 ocy = (Sint16) 0xffff;
    Sint16 df = 1 - rad;
    Sint16 d_e = 3;
    Sint16 d_se = -2 * rad + 5;
    Sint16 xpcx, xmcx, xpcy, xmcy;
    Sint16 ypcy, ymcy, ypcx, ymcx;

    /*
    * Sanity check radius
    */
    if (rad <= 0 || gRender==NULL)
    {
        return false;
    }

    /*
    * Special case for rad=0 - draw a point
    */


    SDL_Renderer * renderer = gRender->gpeRender;
    if( renderer!=NULL)
    {
        //result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
        /*
        * Set color
        */
        SDL_SetRenderDrawColor(renderer,r,g,b,a );

        /*
        * Draw
        */
        do
        {
            xpcx = x + cx;
            xmcx = x - cx;
            xpcy = x + cy;
            xmcy = x - cy;
            if (ocy != cy)
            {
                if (cy > 0)
                {
                    ypcy = y + cy;
                    ymcy = y - cy;
                    render_horizontal_line(gRender, ypcy,xmcx, xpcx);
                    render_horizontal_line(gRender, ymcy,xmcx, xpcx);
                }
                else
                {

                }
                ocy = cy;
            }
            if (ocx != cx)
            {
                if (cx != cy)
                {
                    if (cx > 0)
                    {
                        ypcx = y + cx;
                        ymcx = y - cx;
                        render_horizontal_line(gRender, ymcx,xmcy, xpcy);
                        render_horizontal_line(gRender, ypcx,xmcy, xpcy);
                    }
                    else
                    {
                        render_horizontal_line(gRender, y,xmcy, xpcy);
                    }
                }
                ocx = cx;
            }

            /*
            * Update
            */
            if (df < 0)
            {
                df += d_e;
                d_e += 2;
                d_se += 2;
            }
            else
            {
                df += d_se;
                d_e += 2;
                d_se += 4;
                cy--;
            }
            cx++;
        }
        while (cx <= cy);
    }
    return true;
}

void render_gradient_vertical(GPE_Renderer * gRender, GPE_Rect * rendRect, GPE_Color * rendColor)
{
    if( gRender==NULL)
    {
        gRender = MAIN_RENDERER;
    }
    if( rendRect!=NULL && rendColor!=NULL && gRender!=NULL)
    {
        float colorShadeDivision = 0;
        if( rendRect->w > 0 && rendRect->h > 0)
        {
            GPE_Color * colorShadeTempColor = new GPE_Color();

            for( int i = 0; i <= rendRect->h; i++)
            {
                colorShadeDivision = (float)i/(float) rendRect->h;
                colorShadeTempColor->change_r( merge_channel(rendColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_g( merge_channel(rendColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_b( merge_channel(rendColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                render_horizontal_line_color(gRender,rendRect->y+i,rendRect->x,rendRect->x+rendRect->w,colorShadeTempColor);
            }
            delete colorShadeTempColor;
            colorShadeTempColor = NULL;
        }
    }
}

void render_gradient_horizontal(GPE_Renderer * gRender, GPE_Rect * rendRect, GPE_Color * rendColor)
{
    if( gRender==NULL)
    {
        gRender = MAIN_RENDERER;
    }
    if( rendRect!=NULL && rendColor!=NULL && gRender!=NULL)
    {
        float colorShadeDivision = 0;
        if( rendRect->w > 0 && rendRect->h > 0)
        {
            GPE_Color * colorShadeTempColor = new GPE_Color();

            for( int i = 0; i <= rendRect->w; i++)
            {
                colorShadeDivision = (float)i/(float) rendRect->w;
                colorShadeTempColor->change_r( merge_channel(rendColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_g( merge_channel(rendColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_b( merge_channel(rendColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                render_vertical_line_color(gRender,rendRect->x+i,rendRect->y,rendRect->y+rendRect->h,colorShadeTempColor);
            }
            delete colorShadeTempColor;
            colorShadeTempColor = NULL;
        }
    }
}

void render_rect(GPE_Renderer * gRender, GPE_Rect * rendRect, GPE_Color * rendColor,bool outline, int alphaChannel)
{
    if( rendRect!=NULL)
    {
        SDL_Rect sdlRendRect = {rendRect->x, rendRect->y, rendRect->w, rendRect->h };
        SDL_SetRenderDrawColor( gRender->get_renderer(), rendColor->get_r(), rendColor->get_g(),rendColor->get_b(), alphaChannel );
        if( outline)
        {
            SDL_RenderDrawRect(gRender->get_renderer(), &sdlRendRect);
        }
        else
        {
            SDL_RenderFillRect( gRender->get_renderer(), &sdlRendRect );
        }
    }
}

void render_rectangle(GPE_Renderer * gRender, int x1, int y1, int x2, int y2,  GPE_Color * rendColor,bool outline, int alphaChannel )
{
    SDL_Rect rendRect = {0,0,0,0};
    rendRect.x = std::min(x1, x2);
    rendRect.y = std::min(y1, y2);
    rendRect.w = std::abs(x1- x2);
    rendRect.h = std::abs(y1- y2);
    SDL_SetRenderDrawColor( gRender->get_renderer(), rendColor->get_r(), rendColor->get_g(),rendColor->get_b(), alphaChannel );
    if( outline)
    {
        SDL_RenderDrawRect(gRender->get_renderer(),&rendRect);
    }
    else
    {
        SDL_RenderFillRect( gRender->get_renderer(), &rendRect );
    }
}

void render_line(GPE_Renderer * gRender, int x1, int y1, int x2, int y2,  GPE_Color * rendColor, int alphaChannel)
{
    SDL_SetRenderDrawColor(gRender->get_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(gRender->get_renderer(), x1, y1, x2, y2);
}

void render_line_width(GPE_Renderer * gRender, int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel )
{
    SDL_SetRenderDrawColor(gRender->get_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
}

void render_horizontal_line(GPE_Renderer * gRender, int y, int x1, int x2)
{
    SDL_RenderDrawLine(gRender->get_renderer(), x1, y, x2, y);
}

void render_horizontal_line_color(GPE_Renderer * gRender, int y, int x1, int x2,  GPE_Color * rendColor, int alphaChannel)
{
    SDL_SetRenderDrawColor(gRender->get_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(gRender->get_renderer(), x1, y, x2, y);
}

void render_vertical_line(GPE_Renderer * gRender, int x, int y1, int y2)
{
    SDL_RenderDrawLine(gRender->get_renderer(), x, y1, x, y2);
}

void render_vertical_line_color(GPE_Renderer * gRender, int x, int y1, int y2,  GPE_Color * rendColor, int alphaChannel)
{
    SDL_SetRenderDrawColor(gRender->get_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(gRender->get_renderer(), x, y1, x, y2);
}

fontPair::fontPair(TTF_Font * fontIn,std::string strIn)
{
    lastAlphaRendered = 255;
    str = strIn;
    strTexture = NULL;
    textWidth = 0;
    textHeight = 0;
    if( fontIn!=NULL && (int)strIn.size() > 0)
    {
        //Render text surface
        SDL_Surface * textSurface = NULL;
        textSurface = TTF_RenderUTF8_Blended( fontIn, strIn.c_str(),c_white->get_sdl_color() );
        if( textSurface == NULL )
        {
            record_error( "Unable to render text surface! Error:  ",TTF_GetError() );
        }
        else
        {
            //Create texture from surface pixels
            strTexture = SDL_CreateTextureFromSurface( MAIN_RENDERER->get_renderer(), textSurface );
            if( strTexture != NULL )
            {
                //Get image dimensions
                textWidth = textSurface->w;
                textHeight = textSurface->h;
                SDL_QueryTexture(strTexture,NULL, NULL,&textWidth, &textHeight);
                SDL_FreeSurface(textSurface);
            }
            else
            {
                record_error( "Unable to create texture from rendered text! SDL Error: ", SDL_GetError() );
            }
        }
    }
}

fontPair::~fontPair()
{
    if( strTexture!=NULL)
    {
        SDL_DestroyTexture(strTexture);
        strTexture = NULL;
    }
}

bool fontPair::texture_matches(std::string strCheck)
{
    if( str==strCheck)
    {
        return true;
    }
    return false;
}

SDL_Texture * fontPair::get_texture()
{
    return strTexture;
}

int fontPair::get_width()
{
    return textWidth;
}

int fontPair::get_height()
{
    return textHeight;
}

GPE_Font::GPE_Font(std::string fFileLocation, int fSize, bool isMonospaced, std::string fNickName )
{
    customFontId = 0;
    monoWidth = 0;
    monoHeight = 0;
    numberWidth = 0;
    numberHeight = 0;
    fontIsMonoSpaced = isMonospaced;
    heldSDLFont = TTF_OpenFontIndex(fFileLocation.c_str(), fSize,0);
    fontFamilyName = "";
    fontNickName = fNickName;
    int i = 0;
    if( heldSDLFont!=NULL)
    {
        fontFamilyName = TTF_FontFaceFamilyName(heldSDLFont);
        fontFamilyName+=" "+int_to_string(fSize)+"pt";
        std::string longCharString = "";
        for( i = 0; i < 256; i++)
        {
            longCharString+=i;
        }

        if( isMonospaced)
        {
            TTF_SetFontHinting(heldSDLFont,TTF_HINTING_MONO);
            fontPair * monoSpacedPair = find_texture("A");
            if( monoSpacedPair!=NULL)
            {
                numberWidth = monoWidth = monoSpacedPair->get_width();
                numberHeight = monoHeight = monoSpacedPair->get_height();
            }
        }
        else
        {
            TTF_SetFontHinting(heldSDLFont,TTF_HINTING_LIGHT );
        }

        for( i = 0; i < 10; i++)
        {
            characterPairs.push_back( new fontPair(heldSDLFont,int_to_string(i) ) );
        }
        characterPairs.push_back( new fontPair(heldSDLFont,"." ) );
        characterPairs.push_back(  new fontPair(heldSDLFont,"," ) );
        characterPairs.push_back(  new fontPair(heldSDLFont,"-" ) );
        characterPairs.push_back(  new fontPair(heldSDLFont,"(" ) );
        characterPairs.push_back( new fontPair(heldSDLFont,")" ) );
        if( isMonospaced == false)
        {
            fontPair * tempFontPair = NULL;
            for( i = (int)characterPairs.size()-1; i>=0; i--)
            {
                tempFontPair = characterPairs[i];
                if( tempFontPair!=NULL)
                {
                    if( tempFontPair->get_width() > numberWidth)
                    {
                        numberWidth = tempFontPair->get_width();
                    }
                    if( tempFontPair->get_height() > numberHeight)
                    {
                        numberHeight = tempFontPair->get_height();
                    }
                }
            }
        }
    }
}

GPE_Font::~GPE_Font()
{
    if( heldSDLFont!=NULL)
    {
        TTF_CloseFont(heldSDLFont);
        heldSDLFont = NULL;
    }
    fontPair * tempFontPair = NULL;
    int i = 0;
    for( i = (int)textPairs.size()-1; i>=0 ; i--)
    {
        tempFontPair = textPairs[i];
        if( tempFontPair!=NULL)
        {
            delete tempFontPair;
            tempFontPair = NULL;
        }
    }
    for( i = (int)characterPairs.size()-1; i>=0 ; i--)
    {
        tempFontPair = characterPairs[i];
        if( tempFontPair!=NULL)
        {
            delete tempFontPair;
            tempFontPair = NULL;
        }
        characterPairs[i] = NULL;
    }
}

void GPE_Font::clear_cache()
{
    fontPair * tempFontPair = NULL;
    for(int i = (int)textPairs.size()-1; i>=0 ; i--)
    {
        tempFontPair = textPairs[i];
        if( tempFontPair!=NULL)
        {
            delete tempFontPair;
            tempFontPair = NULL;
        }
    }
    textPairs.clear();
}

int GPE_Font::get_cache_count()
{
    return (int)textPairs.size();
}

void GPE_Font::get_metrics(std::string textToRender, int * wVal, int * hVal)
{
    int tSize = (int)textToRender.size();
    if( tSize > 0)
    {
        if( fontIsMonoSpaced)
        {
            *wVal = monoWidth*tSize;
            *hVal = monoHeight;
        }
        else if(heldSDLFont!=NULL)
        {
            fontPair *  newFoundPair = find_texture( textToRender);
            if( newFoundPair!=NULL)
            {
                *wVal = newFoundPair->get_width();
                *hVal = newFoundPair->get_height();
            }

        }
    }
    else
    {
        *wVal = 0;
        *hVal = 0;
    }
}

void GPE_Font::get_numbered_metrics(std::string textToRender, int * wVal, int * hVal)
{
    int tSize = (int)textToRender.size();
    if( tSize > 0)
    {
        *wVal = numberWidth*tSize;
        *hVal = numberHeight;
    }
    else
    {
        *wVal = 0;
        *hVal = 0;
    }
}

int GPE_Font::get_mono_width()
{
    return monoWidth;
}

std::string GPE_Font::get_family_name()
{
    return fontFamilyName;
}

TTF_Font * GPE_Font::get_sdl_font()
{
    return heldSDLFont;
}

fontPair * GPE_Font::find_character_texture( std::string numbId)
{
    fontPair * fPair = NULL;
    SDL_Texture * fSDLTexture = NULL;
    for( int i = (int)characterPairs.size()-1; i>=0 ; i--)
    {
        fPair = characterPairs[i];
        if( fPair!=NULL)
        {
            if( fPair->texture_matches(numbId) )
            {
                return fPair;
            }
        }
    }
    if( numbId!=" ")
    {
        fPair = new fontPair(heldSDLFont,numbId);
        characterPairs.push_back(fPair);
        return fPair;
    }
    return NULL;
}

fontPair * GPE_Font::find_texture( std::string textToRender)
{
    fontPair * fPair = NULL;
    SDL_Texture * fSDLTexture = NULL;
    for( int i = (int)textPairs.size()-1;  i >=0; i--)
    {
        fPair = textPairs[i];
        if( fPair!=NULL)
        {
            if( fPair->texture_matches(textToRender) )
            {
                return fPair;
            }
        }
        else
        {
            textPairs.erase( textPairs.begin()+i);
        }
    }
    fPair = new fontPair(heldSDLFont,textToRender);
    textPairs.push_back(fPair);
    return fPair;
}

void GPE_Font::render_bitmapped_text( GPE_Renderer * cRenderer,int xPos, int yPos, std::string numberToRender, GPE_Color * textColor, int hAlign,int vAlign, int renderAlpha )
{
    int numberSize = (int)numberToRender.size();
    if( renderAlpha > 0 && numberSize >0 )
    {
        switch(hAlign)
        {
        case FA_CENTER:
            xPos=xPos-(numberWidth*numberSize)/2;
            //LAST_USED_HALIGNMENT=FA_CENTER;
            break;

        case FA_RIGHT:
            xPos=xPos-(numberWidth*numberSize);
            // LAST_USED_HALIGNMENT=FA_RIGHT;
            break;

        //rendering left will be the default
        default:
            // LAST_USED_HALIGNMENT=FA_LEFT;
            break;
        }
        switch(vAlign)
        {
        case FA_MIDDLE:
            yPos=yPos-numberHeight/2;
            // LAST_USED_VALIGNMENT=FA_MIDDLE;
            break;

        case FA_BOTTOM:
            yPos=yPos-numberHeight;
            // LAST_USED_VALIGNMENT=FA_MIDDLE;
            break;

        //rendering left will be the default
        default:
            //LAST_USED_VALIGNMENT=FA_TOP;
            break;
        }
        fontPair * strTex = NULL;
        SDL_Texture * fPairTex = NULL;
        int i = 0;
        std::string currCharacter = "";
        for( i = 0; i < numberSize; i++)
        {
            currCharacter = get_substring(numberToRender,i,1);
            strTex = find_character_texture( currCharacter );
            if( strTex!=NULL)
            {
                fPairTex = strTex->get_texture();
                if( renderAlpha > 255)
                {
                    renderAlpha = 255;
                }
                if( fPairTex!=NULL && renderAlpha >0)
                {
                    int texWid = 0;
                    int texHeight =0;
                    SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                    if( strTex->lastAlphaRendered!=renderAlpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                        strTex->lastAlphaRendered = renderAlpha;
                    }
                    //Get image dimensions
                    texWid = strTex->get_width();
                    texHeight = strTex->get_height();

                    if(hAlign<FA_LEFT || hAlign<0)
                    {
                        hAlign=LAST_USED_HALIGNMENT;
                    }
                    if(vAlign <FA_TOP || vAlign<0)
                    {
                        vAlign=LAST_USED_VALIGNMENT;
                    }
                    SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                    SDL_RenderCopy(cRenderer->gpeRender,fPairTex, NULL, &clip);
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
            xPos+=numberWidth;
        }
    }
}

void GPE_Font::render_text( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign,int vAlign, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        SDL_Texture * fPairTex=  NULL;
        if( strTex!=NULL)
        {
            fPairTex = strTex->get_texture();
            if( renderAlpha > 255)
            {
                renderAlpha = 255;
            }
            if( fPairTex!=NULL && renderAlpha >0)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                if(hAlign<FA_LEFT || hAlign<0)
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }
                if(vAlign <FA_TOP || vAlign<0)
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                case FA_CENTER:
                    xPos=xPos-texWid/2;
                    //LAST_USED_HALIGNMENT=FA_CENTER;
                    break;

                case FA_RIGHT:
                    xPos=xPos-texWid;
                    // LAST_USED_HALIGNMENT=FA_RIGHT;
                    break;

                //rendering left will be the default
                default:
                    // LAST_USED_HALIGNMENT=FA_LEFT;
                    break;
                }
                switch(vAlign)
                {
                case FA_MIDDLE:
                    yPos=yPos-texHeight/2;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                case FA_BOTTOM:
                    yPos=yPos-texHeight;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                //rendering left will be the default
                default:
                    //LAST_USED_VALIGNMENT=FA_TOP;
                    break;
                }
                SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                SDL_RenderCopy(cRenderer->gpeRender,fPairTex, NULL, &clip);
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );

            }
        }
    }
}

void GPE_Font::render_text_ext( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign,int vAlign, int rendWid, int rendHeight, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        if( strTex!=NULL)
        {
            SDL_Texture * fPairTex = strTex->get_texture();
            if( fPairTex!=NULL)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );
                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                //Get image dimensions
                if( rendWid<=0 || rendWid > strTex->get_width())
                {
                    texWid = strTex->get_width();
                }
                else
                {
                    texWid = rendWid;
                }

                if( rendHeight<=0 || rendHeight > strTex->get_height() )
                {
                    texHeight = strTex->get_height();
                }
                else
                {
                    texHeight = rendHeight;
                }

                if(hAlign<FA_LEFT || hAlign<0)
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }
                if(vAlign <FA_TOP || vAlign<0)
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                case FA_CENTER:
                    xPos=xPos-texWid/2;
                    //LAST_USED_HALIGNMENT=FA_CENTER;
                    break;

                case FA_RIGHT:
                    xPos=xPos-texWid;
                    // LAST_USED_HALIGNMENT=FA_RIGHT;
                    break;

                //rendering left will be the default
                default:
                    // LAST_USED_HALIGNMENT=FA_LEFT;
                    break;
                }
                switch(vAlign)
                {
                case FA_MIDDLE:
                    yPos=yPos-texHeight/2;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                case FA_BOTTOM:
                    yPos=yPos-texHeight;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                //rendering left will be the default
                default:
                    //LAST_USED_VALIGNMENT=FA_TOP;
                    break;
                }

                SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                SDL_Rect renderRect = {0, 0, texWid, texHeight};
                SDL_RenderCopy(cRenderer->gpeRender,fPairTex, &renderRect, &clip);
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
            }
        }
    }
}

void GPE_Font::render_text_boxed( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor,int hAlign,int vAlign, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        SDL_Texture * fPairTex=  NULL;
        if( strTex!=NULL)
        {
            fPairTex = strTex->get_texture();
            if( renderAlpha > 255)
            {
                renderAlpha = 255;
            }
            if( fPairTex!=NULL && renderAlpha >0)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                if(hAlign<FA_LEFT || hAlign<0)
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }
                if(vAlign <FA_TOP || vAlign<0)
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                case FA_CENTER:
                    xPos=xPos-texWid/2;
                    //LAST_USED_HALIGNMENT=FA_CENTER;
                    break;

                case FA_RIGHT:
                    xPos=xPos-texWid;
                    // LAST_USED_HALIGNMENT=FA_RIGHT;
                    break;

                //rendering left will be the default
                default:
                    // LAST_USED_HALIGNMENT=FA_LEFT;
                    break;
                }
                switch(vAlign)
                {
                case FA_MIDDLE:
                    yPos=yPos-texHeight/2;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                case FA_BOTTOM:
                    yPos=yPos-texHeight;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                //rendering left will be the default
                default:
                    //LAST_USED_VALIGNMENT=FA_TOP;
                    break;
                }
                GPE_Rect gpeClip;
                gpeClip.x = xPos;
                gpeClip.y = yPos;
                gpeClip.w = texWid;
                gpeClip.h = texHeight;
                SDL_Rect sdlClip = {xPos, yPos, texWid,texHeight};
                render_rect( cRenderer,&gpeClip,boxColor,false,renderAlpha );
                SDL_RenderCopy(cRenderer->gpeRender,fPairTex, NULL, &sdlClip);
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
            }
        }
    }
}

void GPE_Font::render_text_rotated( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, double textAngle, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        SDL_Texture * fPairTex=  NULL;
        if( strTex!=NULL)
        {
            fPairTex = strTex->get_texture();
            if( renderAlpha > 255)
            {
                renderAlpha = 255;
            }
            if( fPairTex!=NULL && renderAlpha >0)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                SDL_Rect sdlDstrect = {xPos-texWid, yPos-texHeight, texWid,texHeight};
                SDL_Point sdlCenterPoint = {texWid, texHeight};
                SDL_RenderCopyEx(cRenderer->gpeRender,fPairTex, NULL,&sdlDstrect,textAngle,&sdlCenterPoint, SDL_FLIP_NONE );
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
            }
        }
    }
}

GPE_Font * GPE_OpenFont(std::string fontLocation, int fontSize, bool isMonospaced, std::string fNickName )
{
    GPE_Font *  returnVal;
    returnVal = new GPE_Font(fontLocation.c_str(),fontSize, isMonospaced, fNickName );
    if( returnVal==NULL)
    {
        record_error("[FONT ERROR] Unable to load font located at "+fontLocation);
    }
    else
    {
        loadedGPEFonts.push_back(returnVal);
        returnVal->customFontId = GLOBAL_FONT_ID;
        GLOBAL_FONT_ID++;
    }
    return returnVal;
}

void GPE_CloseFont(GPE_Font * fontIn)
{
    if( fontIn!=NULL)
    {
        GPE_Font * cFont = NULL;
        for( int i = (int)loadedGPEFonts.size()-1; i >=0; i--)
        {
            cFont = loadedGPEFonts[i];
            if( cFont!=NULL)
            {
                if( cFont->customFontId==fontIn->customFontId)
                {
                    loadedGPEFonts.erase( loadedGPEFonts.begin()+i);
                }
            }
        }
        delete fontIn;
        fontIn = NULL;
    }
}

void GPE_ClearFontCache()
{
    record_error("Clearing Font Cache...");
    GPE_Font * cFont = NULL;
    for( int i = (int)loadedGPEFonts.size()-1; i >=0; i--)
    {
        cFont = loadedGPEFonts[i];
        if( cFont!=NULL)
        {
            cFont->clear_cache();
        }
    }
    record_error("Font Cache total: ["+int_to_string(GPE_GetFontCacheCount() )+"]...");
}

int GPE_GetFontCacheCount(bool recordCache)
{
    GPE_Font * cFont = NULL;
    int cacheCount = 0;
    int i = 0;
    int foundCache = 0;
    if( recordCache)
    {
        record_error("\nCalculating Font Cache...");
        for( i = (int)loadedGPEFonts.size()-1; i >=0; i--)
        {
            cFont = loadedGPEFonts[i];
            if( cFont!=NULL)
            {
                foundCache =cFont->get_cache_count();
                cacheCount+=foundCache;
                record_error("      Font["+cFont->fontNickName+"] Cache total: ["+int_to_string(foundCache )+"]...");
            }
        }
        record_error("Font Cache total: ["+int_to_string(cacheCount )+"]...");
    }
    else
    {
        for( i = (int)loadedGPEFonts.size()-1; i >=0; i--)
        {
            cFont = loadedGPEFonts[i];
            if( cFont!=NULL)
            {
                cacheCount+=cFont->get_cache_count();
            }
        }
    }

    return cacheCount;
}

bool render_bitmap_text( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( textFont!=NULL)
    {
        textFont->render_bitmapped_text(cRenderer,xPos,yPos,mainTranslator->get_str(textureText ),textColor,hAlign,vAlign,renderAlpha);
        return true;
    }
    else
    {
        std::cout << "Unable to render number ("+ textureText + ").\n";
    }
    return false;
}

bool render_new_text( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( (int)textureText.size() > 0)
    {
        if( textFont!=NULL)
        {
            textFont->render_text(cRenderer,xPos,yPos,mainTranslator->get_str(textureText ),textColor,hAlign,vAlign,renderAlpha);
            return true;
        }
        else
        {
            std::cout << "Unable to render text (" << textureText << ").\n";
        }
    }
    return false;
}

bool render_only_text( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( (int)textureText.size() > 0)
    {
        if( textFont!=NULL)
        {
            textFont->render_text(cRenderer,xPos,yPos,textureText,textColor,hAlign,vAlign,renderAlpha);
            return true;
        }
        else
        {
            std::cout << "Unable to render text (" << textureText << ").\n";
        }
    }
    return false;
}

bool render_new_text_ext( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int rendWid, int rendHeight, int renderAlpha )
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( textFont!=NULL)
    {
        textFont->render_text_ext(cRenderer,xPos,yPos,mainTranslator->get_str(textureText ),textColor,hAlign,vAlign,rendWid,rendHeight,renderAlpha);
        return true;
    }
    else
    {
        std::cout << "Unable to render text (" << textureText << ").\n";
    }
    return false;
}

bool render_new_boxed( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor, GPE_Font * textFont, int hAlign,int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( textFont!=NULL)
    {
        textFont->render_text_boxed(cRenderer,xPos,yPos,mainTranslator->get_str(textureText ),textColor,boxColor,hAlign,vAlign,renderAlpha);
        return true;
    }
    else
    {
        std::cout << "Unable to render text (" << textureText << ").\n";
    }
    return false;
}

bool render_new_text_rotated( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, double textAngle, int renderAlpha )
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( textFont!=NULL)
    {
        textFont->render_text_rotated(cRenderer,xPos,yPos,mainTranslator->get_str(textureText ),textColor,textAngle,renderAlpha);
        return true;
    }
    else
    {
        std::cout << "Unable to render text rotated (" << textureText << ").\n";
    }
    return false;
}

void limit_space_to_rect(GPE_Rect * rectIn,int * limitedX, int * limitedY)
{
    if( rectIn!=NULL && limitedX!=NULL && limitedY!=NULL)
    {
        if( *limitedX < rectIn->x)
        {
            *limitedX = 0;
        }

        if( *limitedX < rectIn->x+rectIn->w)
        {
            *limitedX = 0;
        }

        if( *limitedY < rectIn->y)
        {
            *limitedY = 0;
        }

        if( *limitedY < rectIn->y+rectIn->h)
        {
            *limitedY = 0;
        }
    }
}
