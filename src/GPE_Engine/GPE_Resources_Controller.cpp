/*
GPE_Resources_Controller.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

std::string RESOURCE_TYPE_NAMES[res_type_count];

generalGameResource::generalGameResource()
{
    isFullScreenResource = false;
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
    encapBox.h = 32;
    isModified = false;
    editorMode = 0;
    exportBuildGlobalId = -1;
    justOpenedThisFrame = false;
    requestedMode = editorMode = 0;
}

generalGameResource::~generalGameResource()
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

void generalGameResource::prerender_self( )
{

}

void generalGameResource::process_resource(GPE_Rect * viewedSpace,GPE_Rect *cam)
{
    process_self(viewedSpace,cam);
}

void generalGameResource::process_self(GPE_Rect * viewedSpace,GPE_Rect *cam)
{

}

void generalGameResource::render_resource(GPE_Rect * viewedSpace,GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL)
    {
        if( forceRedraw || justOpenedThisFrame )
        {
            //gcanvas->render_rectangle( 0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_THEME->Program_Color,false);
            forceRedraw = true;
        }
        render_self( viewedSpace,cam,forceRedraw);
    }
    justOpenedThisFrame = false;
}

void generalGameResource::render_self(GPE_Rect * viewedSpace,GPE_Rect *cam, bool forceRedraw)
{

}

void generalGameResource::save_resource(std::string alternatePath, int backupId)
{
    GPE_Report("Old save resource used..");
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

bool generalGameResource::write_header_on_file(std::ofstream * fileTarget)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        *fileTarget << "#    --------------------------------------------------  # \n";
        *fileTarget << "#     \n";
        *fileTarget << "#     \n";
        if( resourceType >=0 && resourceType < res_type_count)
        {
            *fileTarget << "#    Game Pencil Engine Project Game " << RESOURCE_TYPE_NAMES[resourceType] <<" DataFile \n";
        }
        *fileTarget << "#    Created automatically via the Game Pencil Engine Editor \n";
        *fileTarget << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
        *fileTarget << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
        *fileTarget << "#     \n";
        *fileTarget << "#     \n";
        *fileTarget << "#    --------------------------------------------------  # \n";
        *fileTarget << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
        *fileTarget << "ResourceName=" << resourceName << "\n";
        *fileTarget << "#     \n";
        return true;
    }
    return false;
}


GPE_DataManager::GPE_DataManager()
{
    randomVariableOn=false;
}

GPE_DataManager::~GPE_DataManager()
{
    randomVariableOn=false;
    //loops and deletes music
    clean_up();
}

//adds a new game sprite with one row
GPE_Animation* GPE_DataManager::sprite_add(std::string spriteFileName,int imgnumb,bool transparent,int xorig,int yorig, bool addMirror)
{
    GPE_Animation * newSprite = get_sprite(spriteFileName);
    if(newSprite!=NULL)
        return newSprite;
    //makes sure the xorig and yorig is not negative
    if(( xorig>=0)&&(yorig>=0) )
    {
        //GPE_Report("Loading "+spriteFileName+" sprite with "+int_to_string(imgnumb)+" needed images.");
        //loads in the sprite's texture
        newSprite = new GPE_Animation( spriteFileName, spriteFileName, transparent );
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to negative dimensions given";
        GPE_Report( outString.str());
        return NULL;
    }
}

GPE_Animation* GPE_DataManager::sprite_add_collision(std::string spriteFileName,int imgnumb,bool transparent,int xorig,int yorig, int cx, int cy, int cw, int ch, bool addMirror)
{
    GPE_Animation * newSprite = get_sprite(spriteFileName);
    if(newSprite!=NULL)
        return newSprite;
    //makes sure the xorig and yorig is not negative
    if(( xorig>=0)&&(yorig>=0) )
    {
        GPE_Report("Loading" +spriteFileName+" sprite...");
        //loads in the sprite's texture
        newSprite = new GPE_Animation(spriteFileName,spriteFileName, transparent);
        newSprite->edit_collision_box(cx, cy, cw, ch);
        return newSprite;
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to negative dimensions given";
        GPE_Report( outString.str());
        return NULL;
    }
}

GPE_Animation* GPE_DataManager::sprite_addsheet(std::string spriteFileName, bool transparent, int width, int height)
{
    GPE_Animation * newSprite = get_sprite(spriteFileName);
    if(newSprite!=NULL)
        return newSprite;
    if(( height>0)&&(width>0) )
    {
        newSprite = new GPE_Animation(spriteFileName,spriteFileName, transparent);
        return newSprite;
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load Sprite from file location: (" << spriteFileName << " due to negative dimensions given)";
        GPE_Report( outString.str());
        return NULL;
    }
}

GPE_Animation* GPE_DataManager::sprite_addsheet_ext(std::string spriteFileName, bool transparent, unsigned short imgnumb, unsigned short imgPerRow, unsigned short width, unsigned short height, unsigned short cellOffX, unsigned short cellOffY, unsigned short pixelOffX, unsigned short pixOffY, unsigned short hSep, unsigned short vSep)
{
    GPE_Animation * newSprite = get_sprite(spriteFileName);
    if(newSprite!=NULL)
        return newSprite;
    if(( height!=0)&&(width!=0) )
    {
        newSprite = new GPE_Animation(spriteFileName, spriteFileName, transparent);
        return newSprite;
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load Sprite from file location: (" << spriteFileName << " due to negative dimensions given)";
        GPE_Report( outString.str());
        return NULL;
    }
}

GPE_Texture* GPE_DataManager::texture_add(std::string textureFileName)
{
    GPE_Texture * newTexture = get_texture(textureFileName);
    if( newTexture!=NULL )
    {
        return newTexture;
    }
    else if( file_exists(textureFileName) )
    {
        //GPE_Report("Loading "+textureFileName+" texture image.");
        //loads in the sprite's texture
        newTexture = new GPE_Texture();
        if( newTexture!=NULL)
        {
            newTexture->load_new_texture( textureFileName, -1,true );
            if(newTexture->get_sdl_texture()!=NULL)
            {
                GPE_Report( "Texture_Add "+get_local_from_global_file( textureFileName )+ "["+int_to_string( (int)rTextures.size() )+"]");
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


GPE_Animation * GPE_DataManager::get_sprite(int idIn)
{
    if( rAnimations.empty() )
        return NULL;
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
    return NULL;
}

GPE_Animation* GPE_DataManager::get_sprite(std::string nameIn)
{
    if( rAnimations.empty() )
        return NULL;
    for(int i=0; i< (int)rAnimations.size(); i++)
    {
        if (rAnimations[i] != NULL)
        {
            if(rAnimations[i]->get_name()==nameIn || rAnimations[i]->get_file_name()==nameIn)
            {
                return rAnimations[i];
            }
        }
    }
    return NULL;
}

GPE_Texture* GPE_DataManager::get_texture(std::string textureFileName)
{
    GPE_Texture * tTexture = NULL;
    for(int i=0; i< (int)rTextures.size(); i++)
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
}

void GPE_DataManager::remove_texture( GPE_Texture * tex )
{
    if( tex!=NULL )
    {
        GPE_Texture * tTex;
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
}


bool GPE_DataManager::load_files()
{
    //If everything loaded fine
    return true;
}

void GPE_DataManager::clean_up()
{
    int it;
    GPE_Animation * cAnimation = NULL;
    //loads through all of the sprites and deletes them
    for(int it=0; it<(int)rAnimations.size(); it++)
    {
        cAnimation = rAnimations[it];
        if( cAnimation!=NULL)
        {
            delete cAnimation;
            cAnimation = NULL;
        }
    }
    rAnimations.clear();

    if( (int)rTextures.size() >0 )
    {
        GPE_Texture * tTexure = NULL;
        for(it  = (int) rTextures.size()-1 ; it >=0; it-- )
        {
            tTexure = rTextures.at(it);
            if( tTexure!=NULL)
            {
                //GPE_Report( "Deleting Texture["+int_to_string( it )+"]" );
                delete tTexure;
                tTexure = NULL;
            }
        }
        rTextures.clear();
    }

    if( (int)rTilesheets.size() >0 )
    {
        GPE_Tilesheet * fTlesheet = NULL;
        for(it  = (int) rTilesheets.size()-1 ; it >=0; it-- )
        {
            fTlesheet = rTilesheets.at(it);
            if( fTlesheet!=NULL)
            {
                //GPE_Report( "Deleting Tilesheet["+int_to_string( it )+"]" );
                delete fTlesheet;
                fTlesheet = NULL;
            }
        }
        rTilesheets.clear();
    }

    if( (int)rAudio.size() >0 )
    {
        GPE_Audio * fAudio = NULL;
        for(it  = (int) rAudio.size()-1 ; it >=0; it-- )
        {
            fAudio = rAudio.at(it);
            if( fAudio!=NULL)
            {
                //GPE_Report( "Deleting Audio["+int_to_string( it )+"]" );
                delete fAudio;
                fAudio = NULL;
            }
        }
        rAudio.clear();
    }
}
