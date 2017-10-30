/*
GPE_Resources_Controller.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "GPE_Resources_Controller.h"

generalGameResource::generalGameResource()
{
    resourceName = "";
    resourceFileName = "";
    parentProjectName = "";
    parentProjectDirectory = "";
    globalResouceIdNumber = -1;
    subResouceId = -1;
    resourceType = -1;
    encapBox.x = 0;
    encapBox.y = 0;
    encapBox.w = 32;
    encapBox.h = 32;;
    isModified = false;
    editorMode = 0;
    html5BuildGlobalId = -1;
    justOpenedThisFrame = false;
}

generalGameResource::~generalGameResource()
{

}

void generalGameResource::compile_self()
{

}

void generalGameResource::integrate_into_syntax()
{

}

std::string generalGameResource::get_current_name()
{
    return resourceName;
}

std::string generalGameResource::get_name()
{
    return resourceName;
}

int generalGameResource::get_global_rid()
{
    return globalResouceIdNumber;
}

bool generalGameResource::is_modified()
{
    return isModified;
}

bool generalGameResource::matches(generalGameResource * otherContainer)
{
    if( otherContainer!=NULL)
    {
        if( globalResouceIdNumber==otherContainer->globalResouceIdNumber)
        {
            if( resourceType== otherContainer->resourceType)
            {
                if( subResouceId==otherContainer->subResouceId)
                {
                    if( parentProjectName.compare(otherContainer->parentProjectName)==0)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void generalGameResource::open_code( int lineNumb, int colNumb, std::string codeTitle )
{

}

void generalGameResource::preprocess_self(std::string alternatePath)
{

}

void generalGameResource::prerender_self(GPE_Renderer * cRender)
{

}

void generalGameResource::process_resource(GPE_Rect * viewedSpace,GPE_Rect *cam)
{

}

void generalGameResource::process_self(GPE_Rect * viewedSpace,GPE_Rect *cam)
{

}

void generalGameResource::render_resource(GPE_Renderer * cRender,GPE_Rect * viewedSpace,GPE_Rect *cam, bool forceRedraw)
{

}

void generalGameResource::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace,GPE_Rect *cam, bool forceRedraw)
{

}

void generalGameResource::save_resource(std::string alternatePath, int backupId)
{
    record_error("Old save resource used..");
    isModified = false;
}

void generalGameResource::set_name(std::string newName)
{
    resourceName = newName;
}

void generalGameResource::set_parent_project(std::string newName)
{
    parentProjectName = newName;
}

void generalGameResource::set_global_rid(int newRId)
{
    globalResouceIdNumber = newRId;
}
void generalGameResource::update_box(int newX, int newY, int newW, int newH)
{
    if( newX!=-1)
    {
        encapBox.x = newX;
    }
    if(newY!=-1)
    {
        encapBox.y = newY;
    }

    if( newW!=-1)
    {
        encapBox.w = newW;
    }
    if(newH!=-1)
    {
        encapBox.h = newH;
    }
}

void generalGameResource::update_string(std::string newName)
{
    resourceName = newName;
}

bool generalGameResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    return true;
}

int generalGameResource::search_for_string(std::string needle)
{
    return 0;
}

int generalGameResource::search_and_replace_string(std::string needle, std::string newStr )
{
    return 0;
}

bool define_colors()
{
    MASTER_OF_COLORS->addColor(c_aqua,"aqua");
    MASTER_OF_COLORS->addColor(c_black,"black");
    MASTER_OF_COLORS->addColor(c_blue,"blue");
    MASTER_OF_COLORS->addColor(c_brown,"brown");
    MASTER_OF_COLORS->addColor(c_dkgray,"dkgray");
    MASTER_OF_COLORS->addColor(c_fuchsia,"fuchsia");
    MASTER_OF_COLORS->addColor(c_gray,"gray");
    MASTER_OF_COLORS->addColor(c_green,"green");
    MASTER_OF_COLORS->addColor(c_lime,"lime");
    MASTER_OF_COLORS->addColor(c_ltgray,"ltgray");
    MASTER_OF_COLORS->addColor(c_maroon,"maroon");
    MASTER_OF_COLORS->addColor(c_navy,"navy");
    MASTER_OF_COLORS->addColor(c_olive,"olive");
    MASTER_OF_COLORS->addColor(c_orange,"orange");
    MASTER_OF_COLORS->addColor(c_orangered,"orangered");
    MASTER_OF_COLORS->addColor(c_purple,"purple");
    MASTER_OF_COLORS->addColor(c_red,"red");
    MASTER_OF_COLORS->addColor(c_silver,"silver");
    MASTER_OF_COLORS->addColor(c_teal,"teal");
    MASTER_OF_COLORS->addColor(c_white,"white");
    MASTER_OF_COLORS->addColor(c_yellow,"yellow");
    return true;
}


ResourceController::ResourceController()
{
    randomVariableOn=false;
}

ResourceController::~ResourceController()
{
    randomVariableOn=false;
    //loops and deletes music
    int it;

    if( rTextures.size() >0 )
    {
        GPE_Texture * tTexure = NULL;
        for(it  = 0; it < (int) rTextures.size(); it++)
        {
            tTexure = rTextures.at(it);
            if( tTexure!=NULL)
            {
                delete tTexure;
                tTexure = NULL;
            }
        }
        rTextures.clear();
    }

    if( rMusic.size() >0 )
    {
        Mix_Music * fMusic = NULL;
        for(it  = 0; it < (int) rMusic.size(); it++)
        {
            fMusic = rMusic.at(it);
            if( fMusic!=NULL)
            {
                Mix_FreeMusic(fMusic);
            }
        }
        rMusic.clear();
    }
    //loops and deletes sounds
    if( rSounds.size() >0 )
    {
        Mix_Chunk * fChunk = NULL;
        for(it  = 0; it < (int) rSounds.size(); it++)
        {
            fChunk = rSounds.at(it);
            if( fChunk!=NULL)
            {
                Mix_FreeChunk(fChunk);
            }
        }
        rSounds.clear();
    }
}

//adds a new game sprite with one row
GPE_Sprite* ResourceController::sprite_add(std::string spriteFileName,int imgnumb,bool transparent,int xorig,int yorig, bool addMirror)
{
    GPE_Sprite * newSprite = get_sprite(spriteFileName);
    if(newSprite!=NULL)
        return newSprite;
    //makes sure the xorig and yorig is not negative
    if(( xorig>=0)&&(yorig>=0) )
    {
        //record_error("Loading "+spriteFileName+" sprite with "+int_to_string(imgnumb)+" needed images.");
        //loads in the sprite's texture
        GPE_Texture * newSurface = new GPE_Texture();
        newSurface->load_new_texture( MAIN_RENDERER,spriteFileName, -1,transparent );
        if(newSurface!=NULL)
        {
            //gets the height and width of spritesheet
            int nW = newSurface->get_width();
            int nH =  newSurface->get_height();
            if( (nH >= yorig )&&(nW  >=xorig) )
            {
                if(imgnumb<1)
                {
                     imgnumb=1;
                }
                newSprite = new GPE_Sprite();
                newSprite->width = (nW-xorig)/imgnumb;
                newSprite->height = nH-yorig;
                newSprite->boundingBox = new GPE_Rect;
                newSprite->boundingBox->x=0;
                newSprite->boundingBox->y=0;
                newSprite->boundingBox->w=newSprite->width;
                newSprite->boundingBox->h=newSprite->height;
                newSprite->colBox = new GPE_Rect;
                newSprite->colBox->x=0;
                newSprite->colBox->y=0;
                newSprite->colBox->w=newSprite->width;
                newSprite->colBox->h=newSprite->height;

                newSprite->spriteTexture= newSurface;
                //newSprite->spriteColor = c_white;
                //newSprite->spritePhase = 0;
                if(addMirror)
                {
                  //  GPE_Texture newMirSurface = flip_surface( newSurface, FLIP_HORIZONTAL );
                  //  newSprite->spriteMirSurface=newMirSurface;
                }
                newSprite->framesForSprite = 0;
                //newSprite->spriteImagesLeft[newSprite->framesForSprite];
                //newSprite->spriteImagesNight[newSprite->framesForSprite];
                //newSprite->spriteImagesNightLeft[newSprite->framesForSprite];

                newSprite->fileName=spriteFileName; //the name along with path
                newSprite->name=getShortFileName(spriteFileName); //gets rid of path

                //Go through rows for the spritesheet
                for( int x = 0; x < nW; x+=newSprite->width)
                {
                     GPE_Rect * newSubImage = new GPE_Rect;
                     newSubImage->x = x;
                     newSubImage->y = yorig;
                     if(nW>=x+newSprite->width)
                     {
                          newSubImage->w = newSprite->width;
                     }
                     else
                     {
                          newSubImage->w = x+newSprite->width-nW;
                     }
                     newSubImage->h = newSprite->height;
                     newSprite->framesForSprite+=1;

                     newSprite->spriteImages.push_back(*newSubImage);
                }
                //big ole duh!
                //gpe_error_logFile << "The Sprite with the file name '" << spriteFileName << "' was added successfully!";
                //gpe_error_logFile << " \n";
                std::stringstream outString;
                rSprites.push_back(newSprite);
                return newSprite;
            }
            else
            {
                std::stringstream outString;
                outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to invalid dimensions given";
                record_error( outString.str());
                return NULL;
            }
        }
        else
        {
            std::stringstream outString;
            outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to invalid dimensions given";
            record_error( outString.str());
            return NULL;
        }
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to negative dimensions given";
        record_error( outString.str());
        return NULL;
    }
}

GPE_Sprite* ResourceController::sprite_add_collision(std::string spriteFileName,int imgnumb,bool transparent,int xorig,int yorig, int cx, int cy, int cw, int ch, bool addMirror)
{
    GPE_Sprite * newSprite = get_sprite(spriteFileName);
    if(newSprite!=NULL)
        return newSprite;
    //makes sure the xorig and yorig is not negative
    if(( xorig>=0)&&(yorig>=0) )
    {
        record_error("Loading" +spriteFileName+" sprite...");
        //loads in the sprite's texture
        GPE_Texture * newSurface = new GPE_Texture();
        newSurface->load_new_texture( MAIN_RENDERER,spriteFileName, -1,transparent );
        if(newSurface!=NULL)
        {
            //gets the height and width of spritesheet
            int nW = newSurface->get_width();
            int nH =  newSurface->get_height();
            if( (nH >= yorig )&&(nW  >=xorig) )
            {
                if(imgnumb<1)
                {
                     imgnumb=1;
                }
                newSprite = new GPE_Sprite();
                newSprite->width = (nW-xorig)/imgnumb;
                newSprite->height = nH-yorig;
                if(cx<=0 || cx>newSprite->width)
                {
                    cx=0;
                }
                if(cy<=0 || cy>newSprite->height)
                {
                    cy=0;
                }
                if(cw<=0 || cw>newSprite->width)
                {
                    cw=newSprite->width;
                }
                if(ch<=0 || cw>=newSprite->height)
                {
                    ch=newSprite->height;
                }
                newSprite->boundingBox = new GPE_Rect;
                newSprite->boundingBox->x=0;
                newSprite->boundingBox->y=0;
                newSprite->boundingBox->w=newSprite->width;
                newSprite->boundingBox->h=newSprite->height;
                newSprite->colBox = new GPE_Rect;
                newSprite->colBox->x=cx;
                newSprite->colBox->y=cy;
                newSprite->colBox->w=cw;
                newSprite->colBox->h=ch;

                newSprite->spriteTexture= newSurface;
                //newSprite->spriteColor = c_white;
                //newSprite->spritePhase = 0;

                newSprite->framesForSprite = 0;
                //newSprite->spriteImagesLeft[newSprite->framesForSprite];
                //newSprite->spriteImagesNight[newSprite->framesForSprite];
                //newSprite->spriteImagesNightLeft[newSprite->framesForSprite];

                newSprite->fileName=spriteFileName; //the name along with path
                newSprite->name=getShortFileName(spriteFileName); //gets rid of path

                //Go through rows for the spritesheet
                for( int x = 0; x < nW; x+=newSprite->width)
                {
                     GPE_Rect * newSubImage = new GPE_Rect;
                     newSubImage->x = x;
                     newSubImage->y = yorig;
                     if(nW>=x+newSprite->width)
                     {
                          newSubImage->w = newSprite->width;
                     }
                     else
                     {
                          newSubImage->w = x+newSprite->width-nW;
                     }
                     newSubImage->h = newSprite->height;
                     newSprite->framesForSprite+=1;

                     newSprite->spriteImages.push_back(*newSubImage);
                }
                //big ole duh!
                //gpe_error_logFile << "The Sprite with the file name '" << spriteFileName << "' was added successfully!";
                //gpe_error_logFile << " \n";
                rSprites.push_back(newSprite);
                return newSprite;
            }
            else
            {
                std::stringstream outString;
                outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to invalid dimensions given";
                record_error( outString.str());
                return NULL;
            }
        }
        else
        {
            std::stringstream outString;
            outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to invalid dimensions given";
            record_error( outString.str());
            return NULL;
        }
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to negative dimensions given";
        record_error( outString.str());
        return NULL;
    }
}

GPE_Sprite* ResourceController::sprite_addsheet(std::string spriteFileName, bool transparent, int width, int height)
{
    GPE_Sprite * newSprite = get_sprite(spriteFileName);
    if(newSprite!=NULL)
        return newSprite;
    if(( height>0)&&(width>0) )
    {
        GPE_Texture * newSurface = new GPE_Texture();
        newSurface->load_new_texture( MAIN_RENDERER,spriteFileName, -1,transparent );
        if(newSurface!=NULL)
        {
            int nW = newSurface->get_width();
            int nH =  newSurface->get_height();
            if( (nH >= height )&&(nW  >=width) )
            {
                newSprite = new GPE_Sprite();
                newSprite->width = width;
                newSprite->height = height;
                newSprite->boundingBox = new GPE_Rect();
                newSprite->boundingBox->x=0;
                newSprite->boundingBox->y=0;
                newSprite->boundingBox->w=width;
                newSprite->boundingBox->h=height;
                newSprite->colBox = new GPE_Rect;
                newSprite->colBox->x=0;
                newSprite->colBox->y=0;
                newSprite->colBox->w=width;
                newSprite->colBox->h=height;
                newSprite->spriteTexture= newSurface;
                newSprite->fileName=spriteFileName; //the name along with path
                newSprite->name=getShortFileName(spriteFileName); //gets rid of path
               //newSprite->spriteColor = c_white;
               //newSprite->spritePhase = 0;

                newSprite->framesForSprite = (nH / height)*(nW / width);
                //newSprite->spriteImagesLeft[newSprite->framesForSprite];
                //newSprite->spriteImagesNight[newSprite->framesForSprite];
                //newSprite->spriteImagesNightLeft[newSprite->framesForSprite];
                //Go through rows
                int heightDecided=0;
                for( int y = 0; y < nH; y+=height)
                {
                    if(nH>=y+height)
                    {
                        heightDecided = height;
                    }
                    else
                    {
                        heightDecided = y+height-nH;
                    }
                    //Go through columns
                    for( int x = 0; x < nW; x+=width)
                    {
                        GPE_Rect * newSubImage = new GPE_Rect;
                        newSubImage->x = x;
                        newSubImage->y = y;
                        if(nW>=x+width)
                        {
                                newSubImage->w = width;
                        }
                        else
                        {
                            newSubImage->w = x+width-nW;
                        }
                        newSubImage->h = heightDecided;
                        newSprite->spriteImages.push_back(*newSubImage);
                    }
                }
                //big ole duh!
                std::stringstream outString;
                outString << "Successfully loaded Sprite from file location: (" << spriteFileName << ")";
                rSprites.push_back(newSprite);
                return newSprite;
            }
            else
            {
                std::stringstream outString;
                outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to invalid dimensions given";
                record_error( outString.str());
                return NULL;
            }
        }
        else
        {
            std::stringstream outString;
            outString << "Unable to load Sprite from file location: (" << spriteFileName << ")Read in as NULL";
            record_error( outString.str());
            return NULL;
        }
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load Sprite from file location: (" << spriteFileName << " due to negative dimensions given)";
        record_error( outString.str());
        return NULL;
    }
}

GPE_Sprite* ResourceController::sprite_addsheet_ext(std::string spriteFileName, bool transparent, unsigned short imgnumb, unsigned short imgPerRow, unsigned short width, unsigned short height, unsigned short cellOffX, unsigned short cellOffY, unsigned short pixelOffX, unsigned short pixOffY, unsigned short hSep, unsigned short vSep)
{
    GPE_Sprite * newSprite = get_sprite(spriteFileName);
    if(newSprite!=NULL)
        return newSprite;
    if(( height!=0)&&(width!=0) )
    {
        GPE_Texture * newSurface = new GPE_Texture();
        newSurface->load_new_texture( MAIN_RENDERER,spriteFileName, -1,transparent );
        if(newSurface!=NULL)
        {
            int nW = newSurface->get_width();
            int nH =  newSurface->get_height();
            if( (nH >= height )&&(nW  >=width) )
            {
                newSprite = new GPE_Sprite();
                newSprite->width = width;
                newSprite->height = height;
                newSprite->boundingBox = new GPE_Rect();
                newSprite->boundingBox->x=0;
                newSprite->boundingBox->y=0;
                newSprite->boundingBox->w=width;
                newSprite->boundingBox->h=height;
                newSprite->colBox = new GPE_Rect;
                newSprite->colBox->x=0;
                newSprite->colBox->y=0;
                newSprite->colBox->w=width;
                newSprite->colBox->h=height;
                newSprite->fileName=spriteFileName; //the name along with path
                newSprite->name=getShortFileName(spriteFileName); //gets rid of path
                newSprite->spriteTexture= newSurface;
               //newSprite->spriteColor = c_white;
               //newSprite->spritePhase = 0;

                newSprite->framesForSprite = (nH / height)*(nW / width);
                //newSprite->spriteImagesLeft[newSprite->framesForSprite];
                //newSprite->spriteImagesNight[newSprite->framesForSprite];
                //newSprite->spriteImagesNightLeft[newSprite->framesForSprite];
                //Go through rows
                int heightDecided=0;
                for( int y = 0; y < nH; y+=height)
                {
                    if(nH>=y+height)
                    {
                        heightDecided = height;
                    }
                    else
                    {
                        heightDecided = y+height-nH;
                    }
                    //Go through columns
                    for( int x = 0; x < nW; x+=width)
                    {
                        GPE_Rect * newSubImage = new GPE_Rect;
                        newSubImage->x = x;
                        newSubImage->y = y;
                        if(nW>=x+width)
                        {
                                newSubImage->w = width;
                        }
                        else
                        {
                            newSubImage->w = x+width-nW;
                        }
                        newSubImage->h = heightDecided;
                        newSprite->spriteImages.push_back(*newSubImage);
                    }
                }
                //big ole duh!
                std::stringstream outString;
                outString << "Successfully loaded Sprite from file location: (" << spriteFileName << ")";
                record_error( outString.str());
                rSprites.push_back(newSprite);
                return newSprite;
            }
            else
            {
                std::stringstream outString;
                outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to invalid dimensions given";
                record_error( outString.str());
                return NULL;
            }
        }
        else
        {
            std::stringstream outString;
            outString << "Unable to load Sprite from file location: (" << spriteFileName << ")Read in as NULL";
            record_error( outString.str());
            return NULL;
        }
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load Sprite from file location: (" << spriteFileName << " due to negative dimensions given)";
        record_error( outString.str());
        return NULL;
    }
}

GPE_Texture* ResourceController::texture_add(std::string textureFileName)
{
    GPE_Texture * newTexture = get_texture(textureFileName);
    if(newTexture!=NULL)
    {
        return newTexture;
    }
    else if( file_exists(textureFileName) )
    {
        //record_error("Loading "+textureFileName+" texture image.");
        //loads in the sprite's texture
        newTexture = new GPE_Texture();
        if( newTexture!=NULL)
        {
            newTexture->load_new_texture( MAIN_RENDERER,textureFileName, -1,true );
            if(newTexture->get_sdl_texture()!=NULL)
            {
                rTextures.push_back(newTexture);
                return newTexture;
            }
            else
            {
                delete newTexture;
                newTexture = NULL;
            }
        }
    }
    return NULL;
}

void ResourceController::edit_collision_box(GPE_Sprite *spriteIn, int cx, int cy, int cw, int ch)
{
    if(spriteIn!=NULL)
    {
        spriteIn->colBox->x=cx;
        spriteIn->colBox->y=cy;
        spriteIn->colBox->w=cw;
        spriteIn->colBox->h=ch;
    }
}

GPE_Sprite * ResourceController::get_sprite(int idIn)
{
    if( rSprites.empty() )
        return NULL;
    if( (int)rSprites.size()<idIn)
    {
        if (rSprites.at(idIn) != NULL)
        {
            if(rSprites.at(idIn)->spriteId == idIn)
            {
                return rSprites.at(idIn);
            }
        }
    }
    for(int i=0;(int)i< (int)rSprites.size();i++)
    {
        if (rSprites[i] != NULL)
        {
            if(rSprites[i]->spriteId == idIn)
            {
                return rSprites[i];
            }
        }
    }
    return NULL;
}

GPE_Sprite* ResourceController::get_sprite(std::string nameIn)
{
    if( rSprites.empty() )
        return NULL;
    for(int i=0;i< (int)rSprites.size();i++)
    {
        if (rSprites[i] != NULL)
        {
            if(rSprites[i]->name==nameIn || rSprites[i]->fileName==nameIn)
            {
                return rSprites[i];
            }
        }
    }
    return NULL;
}

GPE_Texture* ResourceController::get_texture(std::string textureFileName)
{
    GPE_Texture * tTexture = NULL;
    for(int i=0;i< (int)rTextures.size();i++)
    {
        tTexture = rTextures[i];
        if ( tTexture!= NULL)
        {
            if(tTexture->get_filename() == textureFileName)
            {
                return tTexture;
            }
        }
    }
    return NULL;
}

int ResourceController::get_sound(int idIn)
{
    /*
    if( rSounds.empty() )
        return -1;
    if( rSounds.size()<idIn  && idIn>=0)
    {
        if (rSounds.at(idIn) != NULL)
        {
            return idIn;
        }
    }
    for(int i=0;i<rSounds.size();i++)
    {
        if (rSounds[i] != NULL)
        {
            if(rSounds[i]->soundId == idIn)
            {
                return i;
            }
        }
    }*/
    return -1;
}

int  ResourceController::get_sound(std::string nameIn)
{
    /*
    if( rSounds.empty() )
        return -1;
    for(int i=0;i<rSounds.size();i++)
    {
        if (rSounds[i] != NULL)
        {
            if(rSounds[i]->get_name()==nameIn || rSounds[i]->resourceFileName==nameIn)
            {
                return i;
            }
        }
    }*/
    return -1;
}


int ResourceController::get_music(int idIn)
{
    /*
    if( rMusic.empty() )
        return -1;
    if( rMusic.size()>idIn && idIn>=0)
    {
        if (rMusic.at(idIn) != NULL)
        {
            if(rMusic.at(idIn)->musicId == idIn)
            {
                return idIn;
            }
        }
    }
    else
    {
        for(int i=0;i<rMusic.size();i++)
        {
            if (rMusic[i] != NULL)
            {
                if(rMusic[i]->musicId == idIn)
                {
                    return i;
                }
            }
        }
    }
    */
    return -1;
}

int ResourceController::get_music(std::string nameIn)
{
    /*
    if( rMusic.empty() )
        return -1;
    for(int i=0;i<rMusic.size();i++)
    {
        if (rMusic[i] != NULL)
        {
            if(rMusic[i]->get_name()==nameIn || rMusic[i]->resourceFileName==nameIn)
            {
                return i;
            }
        }
    }
    */
    return -1;
}

int ResourceController::add_music(std::string musicFileName)
{
    int returnVal = -1;
    /*
    returnVal = get_music(musicFileName);
    if(returnVal>=0)
    {
        return returnVal;
    }
    else
    {
        int prevMusicSize = (int)rMusic.size();
        Mix_Music* newMusic = Mix_LoadMUS(musicFileName.c_str() );
        if(newMusic!=NULL)
        {
            audioResource*  nMusic = new audioResource();
            nMusic->musicVal = newMusic;
            nMusic->resourceFileName = musicFileName;
            nMusic->get_name() = getShortFileName(musicFileName);
            nMusic->musicId = prevMusicSize;
            rMusic.push_back(nMusic);
            //if(prevMusicSize==(int)rMusic.size() )
            return nMusic->musicId;
        }
    }
    */
    return returnVal;
}

int ResourceController::add_sound(std::string soundFileName, int soundTypeIn)
{
   int returnVal = get_sound(soundFileName);
    /*
    if(returnVal>0)
    {
        return returnVal;
    }
    else
    {
        int prevSoundSize = (int)rSounds.size();

        Mix_Chunk* newSound = Mix_LoadWAV(soundFileName.c_str() );
        if(newSound!=NULL)
        {
            audioResource*  nSound = new audioResource();
            nSound->soundVal = newSound;
            nSound->fileName = soundFileName;
            nSound->name = getShortFileName(soundFileName);
            nSound->soundId = prevSoundSize;
            nSound->soundType = soundTypeIn;
            rSounds.push_back(nSound);
            if(prevSoundSize==(int)rSounds.size() )
            return (int)rSounds.size()-1;
        }
    }
    */
    return returnVal;
}

void ResourceController::play_music(int musicId, int loops)
{
    /*
    int fMusic = get_music(musicId);
    audioResource* pMusic = NULL;
    if(fMusic>=0 && fMusic < rMusic.size() )
    {
        pMusic  = rMusic.at(fMusic);
        if(pMusic!=NULL)
        {
            if(pMusic->musicVal!=NULL)
            {
                Mix_PlayMusic(pMusic->musicVal, loops );
            }
        }
    }
    */
}

void ResourceController::play_sound(int soundId, int soundChannel,int loops)
{
    /*
    int fSound = get_sound(soundId);
    if(fSound>=0)
    {

    }
    */
}



bool ResourceController::load_files()
{
    define_colors();
   // get the loaded font's face fixed status
    //GPE_Font *font;

    //If everything loaded fine
    return true;
}

void ResourceController::clean_up()
{
    //loads through all of the sprites and deletes them
    for(int i=0;i<(int)rSprites.size();i++)
    {
        if(rSprites[i]!=NULL)
        {
            rSprites[i]->clean_up();
            //delete rSprites[i];
        }
    }
    rSprites.clear();
    //Quit SDL
    SDL_Quit();
}
