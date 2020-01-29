/*
GPE_Resources_Controller.cpp
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

#include "GPE_Resources_Controller.h"

std::string RESOURCE_TYPE_NAMES[RESOURCE_TYPE_MAX];

GPE_DataManager::GPE_DataManager(GPE_Render_Package * mRenderPackage, std::string mName)
{
    randomVariableOn=false;
    rRenderPackage = mRenderPackage;
    managerName = mName;
}

GPE_DataManager::~GPE_DataManager()
{
    randomVariableOn=false;
    //loops and deletes music
    clean_up();
}

//adds a new game animation with one row
GPE_Animation* GPE_DataManager::animation_add(std::string animationFileName,int imgnumb,bool transparent,int xorig,int yorig, bool addMirror)
{
    GPE_Animation * newAnimation = get_animation(animationFileName);
    if(newAnimation!=NULL)
        return newAnimation;
    //makes sure the xorig and yorig is not negative
    if(( xorig>=0)&&(yorig>=0) )
    {
        //GPE_Report("Loading "+animationFileName+" animation with "+int_to_string(imgnumb)+" needed images.");
        //loads in the animation's texture
        newAnimation = new GPE_Animation( rRenderPackage, animationFileName, animationFileName, transparent );
        rAnimations[animationFileName] = newAnimation;
        return newAnimation;
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load animation from file location: (" << animationFileName << ") due to negative dimensions given";
        GPE_Report( outString.str());
        return NULL;
    }
}

GPE_Animation* GPE_DataManager::animation_add_collision(std::string animationFileName,int imgnumb,bool transparent,int xorig,int yorig, int cx, int cy, int cw, int ch, bool addMirror)
{
    GPE_Animation * newanimation = get_animation(animationFileName);
    if(newanimation!=NULL)
        return newanimation;
    //makes sure the xorig and yorig is not negative
    if(( xorig>=0)&&(yorig>=0) )
    {
        GPE_Report("Loading" +animationFileName+" animation...");
        //loads in the animation's texture
        newanimation = new GPE_Animation( rRenderPackage, animationFileName,animationFileName, transparent);
        newanimation->edit_collision_box(cx, cy, cw, ch);
        rAnimations[animationFileName] = newanimation;
        return newanimation;
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load animation from file location: (" << animationFileName << ") due to negative dimensions given";
        GPE_Report( outString.str());
        return NULL;
    }
}

GPE_Animation* GPE_DataManager::animation_addsheet(std::string animationFileName, bool transparent, int width, int height)
{
    GPE_Animation * newAnimation = get_animation(animationFileName);
    if(newAnimation!=NULL)
        return newAnimation;
    if(( height>0)&&(width>0) )
    {
        newAnimation = new GPE_Animation(rRenderPackage, animationFileName,animationFileName, transparent);
        rAnimations[animationFileName] = newAnimation;
        return newAnimation;
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load animation from file location: (" << animationFileName << " due to negative dimensions given)";
        GPE_Report( outString.str());
        return NULL;
    }
}

GPE_Animation* GPE_DataManager::animation_addsheet_ext(std::string animationFileName, bool transparent, unsigned short imgnumb, unsigned short imgPerRow, unsigned short width, unsigned short height, unsigned short cellOffX, unsigned short cellOffY, unsigned short pixelOffX, unsigned short pixOffY, unsigned short hSep, unsigned short vSep)
{
    GPE_Animation * newAnimation = get_animation(animationFileName);
    if(newAnimation!=NULL)
        return newAnimation;
    if(( height!=0)&&(width!=0) )
    {
        newAnimation = new GPE_Animation(rRenderPackage, animationFileName, animationFileName, transparent);
        rAnimations[animationFileName] = newAnimation;
        return newAnimation;
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load animation from file location: (" << animationFileName << " due to negative dimensions given)";
        GPE_Report( outString.str());
        return NULL;
    }
}

GPE_Texture_Base * GPE_DataManager::texture_add(std::string textureFileName)
{
    GPE_Texture_Base * newTexture = get_texture(textureFileName);
    if( newTexture!=NULL )
    {
        return newTexture;
    }
    else if( file_exists( textureFileName) )
    {
        //GPE_Report("Loading "+textureFileName+" texture image.");
        //loads in the animation's texture
        newTexture = rRenderPackage->create_texture();
        if( newTexture!=NULL)
        {
            newTexture->load_new_texture( rRenderPackage->packageRenderer,textureFileName, -1,true );
            if(newTexture->get_width() > 0)
            {
                GPE_Report( "Texture_Add "+get_local_from_global_file( textureFileName )+ "["+int_to_string( (int)rTextures.size() )+"]");
                rTextures[textureFileName] = newTexture;
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


GPE_Animation * GPE_DataManager::get_animation(int idIn)
{
    if( rAnimations.empty() )
        return NULL;
    /*
    if( (int)rAnimations.size()<idIn)
    {
        if (rAnimations.at(idIn) != NULL)
        {
            if(rAnimations.at(idIn)->animId == idIn)
            {
                return rAnimations.at(idIn);
            }
        }
    }
    for(int i=0; (int)i< (int)rAnimations.size(); i++)
    {
        if (rAnimations[i] != NULL)
        {
            if(rAnimations[i]->animId == idIn)
            {
                return rAnimations[i];
            }
        }
    }
    */
    return NULL;
}

GPE_Animation* GPE_DataManager::get_animation(std::string nameIn)
{
    auto pairExists = rAnimations.find( nameIn );
    if (pairExists == rAnimations.end() )
    {
        return NULL;
    }
    return rAnimations[nameIn];
}

std::string GPE_DataManager::get_name()
{
    return managerName;
}

GPE_Texture_Base * GPE_DataManager::get_texture(std::string textureFileName)
{
    auto pairExists = rTextures.find( textureFileName );
    if (pairExists == rTextures.end() )
    {
        return NULL;
    }
    return rTextures[textureFileName];
}

GPE_Audio * GPE_DataManager::get_audio(int audioId)
{
    /*
    if( rAudio.empty() )
        return NULL;
    if( rAudio.size()<audioId  && audioId>=0)
    {
        if (rAudio.at(audioId) != NULL)
        {
            return audioId;
        }
    }
    for(int i=0;i<rAudio.size();i++)
    {
        if (rAudio[i] != NULL)
        {
            if(rAudio[i]->get_id() == audioId)
            {
                return i;
            }
        }
    }*/
    return NULL;
}

GPE_Audio * GPE_DataManager::get_audio(std::string nameIn)
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
    return NULL;
}

GPE_Audio * GPE_DataManager::add_audio(std::string soundFileName, int soundTypeIn)
{
    GPE_Audio * returnVal = get_audio(soundFileName);
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
            nSound->name = get_short_filename(soundFileName);
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

void GPE_DataManager::play_audio(int audioId, int loops)
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

void GPE_DataManager::remove_animation( GPE_Animation * anim )
{
    /*
    if( anim!=NULL && !rAnimations.empty() )
    {
        GPE_Animation * tAnim;
        for( int i= (int)rAnimations.size()-1; i>=0; i-- )
        {
            tAnim = rAnimations[i];
            if ( tAnim!= NULL)
            {
                if( tAnim->get_name()==anim->get_name() || tAnim->get_file_name()==anim->get_file_name() )
                {
                    delete tAnim;
                    tAnim = NULL;
                    rAnimations.erase( rAnimations.begin()+i );
                    return;
                }
            }
        }
    }
    */
}

void GPE_DataManager::remove_texture( GPE_Texture_Base * tex )
{
    /*
    if( tex!=NULL )
    {
        GPE_Texture_Base * tTex;
        for( int i= (int)rTextures.size()-1; i>=0; i-- )
        {
            tTex = rTextures[i];
            if ( tTex!= NULL)
            {
                if( tTex->get_name()==tex->get_name() || tTex->get_filename()==tex->get_filename() )
                {
                    delete tTex;
                    tTex = NULL;
                    rTextures.erase( rTextures.begin()+i );
                }
            }
        }
    }
    */
}


bool GPE_DataManager::load_files()
{
    //If everything loaded fine
    return true;
}

void GPE_DataManager::clean_up()
{
    std::map< std::string, GPE_Animation *>::iterator itAnim = rAnimations.begin();
    GPE_Animation * fAnim = NULL;
    while( itAnim != rAnimations.end() )
    {
        fAnim = itAnim->second;

        if( fAnim !=NULL)
        {
            delete fAnim;
            fAnim =NULL;
        }
        ++itAnim;
    }
    rAnimations.clear();

    std::map< std::string, GPE_Audio *>::iterator itAudio = rAudio.begin();
    GPE_Audio * fAudio = NULL;
    while( itAudio != rAudio.end() )
    {
        fAudio = itAudio->second;

        if( fAudio !=NULL)
        {
            delete fAudio;
            fAudio =NULL;
        }
        ++itAudio;
    }
    rAudio.clear();

    std::map< std::string, GPE_Texture_Base *>::iterator itTextures = rTextures.begin();
    GPE_Texture_Base * fTexture = NULL;
    while( itTextures != rTextures.end() )
    {
        fTexture = itTextures->second;

        if( fTexture !=NULL)
        {
            delete fTexture;
            fTexture =NULL;
        }
        ++itTextures;
    }
    rTextures.clear();

    std::map< std::string, GPE_Tilesheet *>::iterator itTilesheets = rTilesheets.begin();
    GPE_Tilesheet * fTilesheet = NULL;
    while( itTilesheets != rTilesheets.end() )
    {
        fTilesheet = itTilesheets->second;

        if( fTilesheet !=NULL)
        {
            delete fTilesheet;
            fTilesheet =NULL;
        }
        ++itTilesheets;
    }
    rTilesheets.clear();
}
