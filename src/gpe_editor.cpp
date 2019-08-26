/*
gpe_editor.cpp
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
#include "GPE_Engine/GPE_Program_State.h"
#include "gpe_editor.h"
#include "gpe_editor_start_page.h"
#include "gpe_project_resources.h"
#include "gpe_editor_helppage.h"
#include "gpe_cpp_compiler_settings.h"
#include "gpe_gamecontroller_tester.h"
#include "gpe_cpp_compiler.h"

int lastResTypeRendered = -1;

GPE_ProjectFolder * CURRENT_PROJECT = NULL;

GPE_Gui_Engine * GPE_MAIN_GUI = NULL;

GPE_PopUpMenu_Option * MAIN_TOOLBAR_RECENT_PROJECTS = NULL;

//generalGameResource * SELECTED_GENERAL_RESOUCE = NULL;
GPE_Texture * GPE_LOGO = NULL;
GPE_Texture * GPE_TEXTURE_TRANSPARENT_BG = NULL;

std::string GPE_BUILD_NAMES[GPE_BUILD_OPTIONS];


int GPE_GetMessageBoxResult(const std::string  title, const std::string  prompt, int flagType)
{
    return 0;
}

GPE_GeneralResourceContainer::GPE_GeneralResourceContainer(std::string projFolderName, std::string newName, int rType, int rId,bool folder, int globalIdVal,  int rezPropValue)
{
    foundX2Pos = 0;
    resouceNameChanged = false;
    subContentsModified = false;
    parentResource = NULL;
    projectParentFileName = projectParentName = projFolderName;
    opName = newName;
    resourceType = rType;
    resourceId = rId;
    isFolder = folder;
    exportBuildGlobalId = -1;
    globalResouceId = -1;
    if(globalIdVal>=0)
    {
        globalResouceId = globalIdVal;
    }
    optionBox.x = 0;
    optionBox.y = 0;
    optionBox.w = 0;
    optionBox.h = RESOURCE_AREA_HEIGHT;
    containerSprite = NULL;
    containerTexture = NULL;
    spriteFrameNumber = 0;
    heldResource = NULL;
    subMenuIsOpen = false;
    int textW = 0;
    int textH = 0;

    if( (int)opName.size()>0 && FONT_TOOLBAR!=NULL)
    {
        FONT_TOOLBAR->get_metrics(opName,&textW, &textH);
    }
    strTexWidth = textW;
    strTexHeight = textH;

    set_basic_image_value();
    if( rezPropValue==restype_projfolder)
    {
        isSuperProjectFolder = true;
        isSuperFolder = false;
    }
    else if( rezPropValue==restype_superfolder)
    {
        isSuperFolder = true;
        isSuperProjectFolder = false;
    }
    else
    {
        isSuperFolder = false;
        isSuperProjectFolder = false;
    }

}

GPE_GeneralResourceContainer::~GPE_GeneralResourceContainer()
{
    GPE_Report("Removing Resource named ["+opName+"].");
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Removing Resource", opName );
    }
    if( heldResource!=NULL)
    {
        GPE_Report("Removing Held-Resource named ["+opName+"].");
        if( GPE_Main_TabManager!=NULL)
        {
            GPE_Main_TabManager->close_resource_tab( projectParentName,heldResource->globalResouceIdNumber );
        }
        GPE_Report("Held-Resource deleted ["+opName+"].");
        delete heldResource;
        heldResource = NULL;
    }
    GPE_Report("Held-Resource's sub-options deleted ["+opName+"].");
    GPE_GeneralResourceContainer * tempSubOp = NULL;
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        tempSubOp = subOptions[i];
        if( tempSubOp!=NULL)
        {
            delete tempSubOp;
            tempSubOp = NULL;
        }
    }

    if( containerSprite!=NULL)
    {
        //delete containerSprite;
        //containerSprite = NULL;
    }
    if( containerTexture!=NULL)
    {
        /*delete containerTexture;
        containerTexture = NULL;*/
    }
    subOptions.clear();
    GPE_Report("Resource ["+opName+"] deleted.");

}

void GPE_GeneralResourceContainer::add_resource_container( GPE_GeneralResourceContainer * newResource, bool changeGlobalId, GPE_GeneralResourceContainer * referenceObject)
{
    if( newResource!=NULL)
    {
        newResource->parentResource = this;
        newResource->parentProjectDirectory = parentProjectDirectory;
        newResource->projectParentFileName = projectParentFileName;
        newResource->set_project_parent_name( projectParentFileName);
        if( subMenuIsOpen)
        {
            optionBox.h += newResource->optionBox.h;
        }
        if( changeGlobalId)
        {
            int newId = (int)subOptions.size()+1;
            newResource->set_global_id(newId);
        }
        if( referenceObject!=NULL )
        {
            int foundRef = -1;
            GPE_GeneralResourceContainer * tempResource = NULL;
            for( int i = (int)subOptions.size()-1; i>=0; i--)
            {
                tempResource = subOptions[i];
                if( tempResource!=NULL )
                {
                    if( tempResource->matches(referenceObject)==1 )
                    {
                        foundRef = i;
                        break;
                    }
                }
            }
            if( foundRef >=0 && foundRef < (int)subOptions.size() )
            {
                subOptions.insert(subOptions.begin()+foundRef+1,newResource);
            }
            else
            {
                subOptions.push_back(newResource);
            }
        }
        else
        {
            if( newResource->get_resource_type()==RESOURCE_TYPE_PROJECT_SETTINGS)
            {
                subOptions.push_back( newResource );
            }
            else if( RESOURCE_TO_DRAG!=NULL)
            {
                if( RESOURCE_TO_DRAG->matches(newResource) )
                {
                    subOptions.insert(subOptions.begin(),newResource);
                }
                else
                {
                    subOptions.push_back( newResource );
                }
            }
            else
            {
                subOptions.push_back( newResource );
            }
        }
    }
}

GPE_GeneralResourceContainer * GPE_GeneralResourceContainer::add_resource_folder( std::string resourceTypeName,  int gResId,int rezPropValue)
{
    int newId = (int)subOptions.size()+1;
    GPE_GeneralResourceContainer * newFolder = new GPE_GeneralResourceContainer(projectParentFileName,resourceTypeName,resourceType,newId,true, gResId, rezPropValue);
    newFolder->optionBox.x = optionBox.x;
    newFolder->optionBox.y = optionBox.y+newId*RESOURCE_AREA_HEIGHT;
    //optionBox.h+=RESOURCE_AREA_HEIGHT;
    newFolder->parentResource = this;
    subOptions.push_back(newFolder);
    if( subMenuIsOpen)
    {
        optionBox.h += RESOURCE_AREA_HEIGHT;
    }
    return newFolder;
}

GPE_GeneralResourceContainer * GPE_GeneralResourceContainer::add_newtype_folder( int rType,std::string resourceTypeName, int gResId, int rezPropValue)
{
    int newId = (int)subOptions.size()+1;
    GPE_GeneralResourceContainer * newFolder = new GPE_GeneralResourceContainer(projectParentFileName,resourceTypeName,rType,newId,true, gResId,rezPropValue);
    newFolder->optionBox.x = optionBox.x;
    newFolder->optionBox.y = optionBox.y+newId*RESOURCE_AREA_HEIGHT;
    //optionBox.h+=RESOURCE_AREA_HEIGHT;
    newFolder->parentResource = this;
    subOptions.push_back(newFolder);
    optionBox.h += RESOURCE_AREA_HEIGHT;
    return newFolder;
}

void GPE_GeneralResourceContainer::delete_resource(GPE_GeneralResourceContainer * otherContainer)
{
    remove_resource(otherContainer);
}

bool GPE_GeneralResourceContainer::detect_name_change(bool autoChange)
{
    if( heldResource!=NULL)
    {
        std::string fName = heldResource->get_current_name();
        if( fName!=opName)
        {
            if( autoChange)
            {
                set_name( fName  );
            }
            return true;
        }
    }
    return false;
}

GPE_GeneralResourceContainer * GPE_GeneralResourceContainer::get_resource_at(int resourcePos, bool nestDown)
{
    if((int)subOptions.size()>0)
    {
        if(resourcePos>=0 && resourcePos<(int)subOptions.size() )
        {
            return subOptions.at(resourcePos);
        }
    }
    return NULL;
}

GPE_GeneralResourceContainer * GPE_GeneralResourceContainer::find_resource_from_id(int resourceIdToFind, bool nestDown, bool includeSelf)
{
    GPE_GeneralResourceContainer * rFound = NULL;
    GPE_GeneralResourceContainer * nResource = NULL;
    if( globalResouceId==resourceIdToFind && includeSelf)
    {
        return this;
    }
    if( (int)subOptions.size()>0 )
    {
        for( int i= 0 ; i < (int)subOptions.size(); i++)
        {
            nResource = subOptions[i];
            if( nResource!=NULL)
            {
                rFound = nResource->find_resource_from_id(resourceIdToFind);
                if( rFound!=NULL)
                {
                    break;
                }
            }
        }
    }
    return rFound;
}

GPE_GeneralResourceContainer * GPE_GeneralResourceContainer::get_usable_resource()
{
    GPE_GeneralResourceContainer * rFound = NULL;
    GPE_GeneralResourceContainer * nResource = NULL;
    if( heldResource!=NULL )
    {
        return this;
    }
    if( (int)subOptions.size()>0 )
    {
        for( int i= 0 ; i < (int)subOptions.size(); i++)
        {
            nResource = subOptions[i];
            if( nResource!=NULL)
            {
                rFound = nResource->get_usable_resource();
                if( rFound!=NULL)
                {
                    break;
                }
            }
        }
    }
    return rFound;

}

GPE_GeneralResourceContainer * GPE_GeneralResourceContainer::find_resource_from_name(std::string rNameToFind, bool nestDown)
{
    bool foundResult = false;
    GPE_GeneralResourceContainer * fContainer = NULL;
    GPE_GeneralResourceContainer * soughtContainer = NULL;
    if( rNameToFind.size()> 0 )
    {
        if((int)subOptions.size()>0)
        {
            for( int i = 0; i < (int)subOptions.size() && foundResult == false; i++)
            {
                fContainer = subOptions[i];
                if( fContainer!=NULL)
                {
                    if( rNameToFind.compare( fContainer->get_name() ) ==0)
                    {
                        foundResult = true;
                        soughtContainer= fContainer;
                    }
                    else if( nestDown)
                    {
                        if( fContainer->find_resource_from_name(rNameToFind,nestDown)!=NULL)
                        {
                            foundResult = true;
                            soughtContainer = fContainer->find_resource_from_name(rNameToFind,nestDown);
                        }
                    }
                }
            }
        }
    }
    if( foundResult)
    {
        return soughtContainer;
    }
    return NULL;
}

GPE_GeneralResourceContainer * GPE_GeneralResourceContainer::find_resource_target(std::string rNameToFind, bool nestDown)
{
    int foundId = string_to_int(rNameToFind,-1);
    if( foundId > 0)
    {
        return find_resource_from_id(foundId,nestDown,false);
    }
    else
    {
        return find_resource_from_name(rNameToFind,nestDown);
    }
    return NULL;
}

std::string GPE_GeneralResourceContainer::get_name()
{
    return opName;
}

std::string GPE_GeneralResourceContainer::get_project_name()
{
    return projectParentName;
}

int GPE_GeneralResourceContainer::get_global_id()
{
    return globalResouceId;
}

generalGameResource * GPE_GeneralResourceContainer::get_held_resource()
{
    return heldResource;
}

int GPE_GeneralResourceContainer::get_resource_count()
{
    int returnNumb = 0;
    if( !isFolder &&  !isSuperFolder &&  !isSuperProjectFolder)
    {
        returnNumb = 1;
    }
    GPE_GeneralResourceContainer * tempContainer = NULL;
    for( int i = subOptions.size()-1; i >=0; i--)
    {
        tempContainer = subOptions[i];
        if( tempContainer!=NULL)
        {
            returnNumb+=tempContainer->get_resource_count();
        }
    }
    return returnNumb;
}


int GPE_GeneralResourceContainer::get_resource_id()
{
    return resourceId;
}

int GPE_GeneralResourceContainer::get_resource_type()
{
    return resourceType;
}


int GPE_GeneralResourceContainer::get_resource_image_frame()
{
    return spriteFrameNumber;
}

GPE_Animation * GPE_GeneralResourceContainer::get_resource_sprite()
{
    if( !isFolder && !isSuperProjectFolder)
    {
        if( resourceType == RESOURCE_TYPE_OBJECT )
        {
            gameObjectResource * heldGOResource = (gameObjectResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                GPE_GeneralResourceContainer * allSpritesFolder = heldGOResource->projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SPRITE]+"s");
                if( allSpritesFolder!=NULL)
                {
                    GPE_GeneralResourceContainer *objTypeContainer = allSpritesFolder->find_resource_from_id(heldGOResource->spriteIndex);
                    if( objTypeContainer!=NULL)
                    {
                        animationResource * heldSpriteResouce = (animationResource*) objTypeContainer->heldResource;
                        if( heldSpriteResouce!=NULL)
                        {
                            if( heldSpriteResouce->animInEditor!=NULL)
                            {
                                return heldSpriteResouce->animInEditor;
                            }
                        }
                    }
                }
            }
        }
        else if( resourceType== RESOURCE_TYPE_SPRITE )
        {
            animationResource * heldGOResource = (animationResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                if( heldGOResource->animInEditor!=NULL)
                {
                    return heldGOResource->animInEditor;
                }
            }
        }
    }
    return NULL;
}

GPE_Texture * GPE_GeneralResourceContainer::get_resource_texture()
{
    if( !isFolder && !isSuperProjectFolder)
    {
        if( resourceType == RESOURCE_TYPE_TEXTURE )
        {
            textureResource * heldGOResource = (textureResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                if( heldGOResource->textureInEditor!=NULL && heldGOResource->textureInEditor->get_width() > 0 )
                {
                    return heldGOResource->textureInEditor;
                }
            }
        }
        else if( resourceType== RESOURCE_TYPE_TILESHEET )
        {
            tilesheetResource * heldGOResource = (tilesheetResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                if( heldGOResource->tilesheetInEditor!=NULL && heldGOResource->tilesheetInEditor->tsImage!=NULL)
                {
                    return heldGOResource->tilesheetInEditor->tsImage;
                }
            }
        }
    }
    return containerTexture;
}

int GPE_GeneralResourceContainer::get_options_width()
{
    int maxFoundWith = 0;
    GPE_GeneralResourceContainer * tempOption = NULL;
    for( int i = subOptions.size()-1; i>=0; i--)
    {
        tempOption = subOptions[i];
        if( tempOption!=NULL)
        {
            if( tempOption->strTexWidth > maxFoundWith)
            {
                maxFoundWith = tempOption->strTexWidth;
            }
        }
    }
    return maxFoundWith;
}

int GPE_GeneralResourceContainer::get_size()
{
    return (int)subOptions.size();
}

void GPE_GeneralResourceContainer::grab_useable_resources(std::vector <GPE_GeneralResourceContainer * > &rVector)
{
    GPE_GeneralResourceContainer * nResource = NULL;
    if( heldResource!=NULL )
    {
        rVector.push_back(this);
    }
    if( (int)subOptions.size()>0 )
    {
        for( int i= 0 ; i < (int)subOptions.size(); i++)
        {
            nResource = subOptions[i];
            if( nResource!=NULL)
            {
                nResource->grab_useable_resources(rVector);
            }
        }
    }
}

void GPE_GeneralResourceContainer::integrate_into_syntax()
{
    if( heldResource!=NULL)
    {
        heldResource->integrate_into_syntax();
    }

    GPE_GeneralResourceContainer * tSub = NULL;
    for( int i = 0; i < (int)subOptions.size(); i++)
    {
        tSub = subOptions[i];
        if( tSub!=NULL)
        {
            tSub->integrate_into_syntax();
        }
    }
}

bool GPE_GeneralResourceContainer::is_folder()
{

    return isFolder;
}

bool GPE_GeneralResourceContainer::is_super_folder()
{
    return isSuperFolder;
}

bool GPE_GeneralResourceContainer::is_super_project_folder()
{
    return isSuperProjectFolder;
}

bool GPE_GeneralResourceContainer::is_child_of(GPE_GeneralResourceContainer * otherContainer)
{
    if( otherContainer!=NULL)
    {
        int i;
        GPE_GeneralResourceContainer * tempSubOption = NULL;
        if( matches(otherContainer)==1 )
        {
            return true;
        }
        for( i = subOptions.size()-1; i >=0; i--)
        {
            tempSubOption = subOptions[i];
            if( tempSubOption!=NULL)
            {
                if( tempSubOption->is_child_of(otherContainer) )
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GPE_GeneralResourceContainer::can_obtain(GPE_GeneralResourceContainer * otherContainer)
{
    if( otherContainer!=NULL)
    {
        if( otherContainer->is_super_folder()==false && otherContainer->is_super_project_folder()==false )
        {
            if(  projectParentFileName.compare(otherContainer->projectParentFileName )==0 && get_resource_type()==otherContainer->get_resource_type() )
            {
                if( otherContainer->is_child_of(this) ==false )
                {
                    return true;
                }
                else
                {
                    GPE_Report("Can not move["+otherContainer->get_name()+"] into ["+get_name()+"] folder.");
                }
            }
            else
            {
                GPE_Report("Can not move["+otherContainer->get_name()+"] into ["+get_name()+"] folder[Different Folder Type].");
            }
        }
        else
        {
            GPE_Report("Can not move super folder["+otherContainer->get_name()+".Illegal editor operation.");
        }
    }
    return false;
}

int GPE_GeneralResourceContainer::matches(GPE_GeneralResourceContainer * otherContainer)
{
    if( otherContainer!=NULL)
    {
        if( get_global_id()==otherContainer->get_global_id() )
        {
            if( otherContainer->isFolder== isFolder)
            {
                if( projectParentFileName.compare(otherContainer->projectParentFileName )==0)
                {
                    if( otherContainer->heldResource!=NULL && heldResource!=NULL)
                    {
                        if( resourceType == otherContainer->resourceType)
                        {
                            if( resourceId==otherContainer->resourceId)
                            {
                                return 1;
                            }
                        }
                    }
                    else if( otherContainer->heldResource==NULL && heldResource==NULL)
                    {
                        if( resourceType == otherContainer->resourceType)
                        {
                            if( resourceId==otherContainer->resourceId)
                            {
                                return 1;
                            }
                        }

                    }
                }
            }
            return 0;
        }
    }
    return -1;
}

void GPE_GeneralResourceContainer::open_folder()
{
    subMenuIsOpen = true;
}

void GPE_GeneralResourceContainer::preprocess_container(std::string alternatePath, int backupId)
{
    if( heldResource!=NULL)
    {
        heldResource->preprocess_self(alternatePath);
    }
    GPE_GeneralResourceContainer * cResource = NULL;
    for(int i = 0; i<(int)subOptions.size(); i+=1)
    {
        cResource = subOptions[i];
        if(cResource != NULL)
        {
            cResource->preprocess_container();
            cResource->detect_name_change(true);
        }
    }
}

void GPE_GeneralResourceContainer::prerender_self( )
{

    if( heldResource!=NULL)
    {
        heldResource->prerender_self( );
    }
    GPE_GeneralResourceContainer * tSubOption= NULL;
    for( int i = 0; i < (int)subOptions.size(); i++)
    {
        tSubOption = subOptions[i];
        if( tSubOption!=NULL)
        {
            tSubOption->prerender_self( );
        }
    }
}

int GPE_GeneralResourceContainer::process_container(int xPos, int yPos, int selectedId, GPE_Rect * viewedSpace, GPE_Rect * cam, bool mouseInRange )
{
    int returnAnswer = -1;
    int lxPos = xPos-cam->x+viewedSpace->x;
    int lyPos = yPos-cam->y+viewedSpace->y;
    int lx2Pos = lxPos+cam->w;
    int ly2Pos = lyPos+RESOURCE_AREA_HEIGHT;
    bool folderOpened = false;
    subContentsModified = false;
    foundX2Pos = xPos+strTexWidth+RESOURCE_AREA_HEIGHT*3/2;
    if( isFolder || isSuperFolder || isSuperProjectFolder )
    {
        GPE_GeneralResourceContainer * cResource = NULL;
        int subYPos = yPos+RESOURCE_AREA_HEIGHT;
        if( mouseInRange && input->check_mouse_pressed(0) )
        {
            if(point_between(input->mouse_x,input->mouse_y,lxPos,lyPos,lx2Pos,ly2Pos) )
            {
                set_current_gpe_project_from_name( projectParentFileName);
                if( CURRENT_PROJECT ==NULL)
                {
                    GPE_Report("NULL Project Found for"+opName+".");
                }
                if( !isSuperFolder && !isSuperProjectFolder)
                {
                    RESOURCE_TO_DRAG = this;
                }
            }
        }
        else if(mouseInRange && input->check_mouse_released( mb_left))
        {
            if(point_between(input->mouse_x,input->mouse_y,lxPos,lyPos,lx2Pos,ly2Pos) )
            {
                returnAnswer = globalResouceId;
                set_current_gpe_project_from_name(  projectParentFileName );
                if( CURRENT_PROJECT ==NULL )
                {
                    GPE_Report("NULL Project Found for"+opName+".");
                }
                bool openAllowed = true;
                if( RESOURCE_TO_DRAG!=NULL)
                {
                    LAST_CLICKED_RESOURCE = this;
                    if( RESOURCE_TO_DRAG->matches(this)!=1)
                    {
                        openAllowed = false;
                    }
                }
                if( openAllowed)
                {
                    if( !subMenuIsOpen)
                    {
                        folderOpened = true;
                    }
                    else
                    {
                        subMenuIsOpen = false;
                    }
                    if(folderOpened  )
                    {
                        subMenuIsOpen = true;
                        //opens folder
                        /*
                        cResource = NULL;
                        for(int i = 0; i<(int)subOptions.size();i+=1)
                        {
                            cResource = subOptions[i];
                            if(cResource != NULL)
                            {
                                cResource->process_container(xPos+RESOURCE_AREA_HEIGHT,subYPos,selectedId,viewedSpace,cam,mouseInRange);
                                subYPos += cResource->optionBox.h;
                                optionBox.h += cResource->optionBox.h;
                            }
                        }*/
                    }
                }
            }
        }
        else if( mouseInRange && input->check_mouse_released(1) )
        {
            if(point_within(input->mouse_x,input->mouse_y,lxPos,lyPos,lx2Pos,ly2Pos) )
            {
                returnAnswer = globalResouceId;
                set_current_gpe_project_from_name ( CURRENT_PROJECT_NAME = projectParentFileName );
                if( CURRENT_PROJECT ==NULL)
                {
                    GPE_Report("NULL Project Found for"+opName+".");
                }
                LAST_CLICKED_RESOURCE = this;
                LAST_CLICKED_RESTYPE = resourceType;
                RESOURCEMENU_WAS_RIGHTCLICKED = true;
                RESOURCE_TO_DRAG = NULL;
            }
        }
        else if( mouseInRange && point_between(input->mouse_x,input->mouse_y,lxPos,lyPos,lx2Pos,ly2Pos) )
        {
            MAIN_OVERLAY->update_tooltip( opName );
        }

        if( subMenuIsOpen )
        {
            cResource = NULL;
            subYPos = yPos+RESOURCE_AREA_HEIGHT;
            optionBox.h = RESOURCE_AREA_HEIGHT;
            int subAnswer = -1;
            for(int i = 0; i<(int)subOptions.size(); i+=1)
            {
                cResource = subOptions[i];
                if(cResource != NULL)
                {
                    subAnswer = cResource->process_container(xPos,subYPos,selectedId,viewedSpace,cam,mouseInRange);
                    if(subAnswer>=0)
                    {
                        returnAnswer = subAnswer;
                    }
                    optionBox.h+= cResource->optionBox.h;
                    subYPos += cResource->optionBox.h;
                    if( cResource->resouceNameChanged)
                    {
                        subContentsModified= true;
                    }
                    if ( cResource->heldResource!=NULL)
                    {
                        if( cResource->heldResource->is_modified() )
                        {
                            subContentsModified = true;
                        }
                    }
                    if( foundX2Pos <  cResource->foundX2Pos )
                    {
                        foundX2Pos = cResource->foundX2Pos;
                    }
                }
            }
        }
        else
        {
            optionBox.h = RESOURCE_AREA_HEIGHT;
            foundX2Pos = xPos+strTexWidth+RESOURCE_AREA_HEIGHT/2;
        }
    }
    else
    {
        optionBox.h = RESOURCE_AREA_HEIGHT;
        if( resourceType == RESOURCE_TYPE_SPRITE)
        {
            animationResource * heldSpriteC = (animationResource *)heldResource;
            if( heldSpriteC!=NULL)
            {
                spriteFrameNumber = 0;
                containerSprite = heldSpriteC->animInEditor;
                if( containerSprite==NULL)
                {
                    containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resouces/buttons/magnet.png");
                }
            }
        }
        else
        {
            set_basic_image_value();
        }

        if( mouseInRange && input->check_mouse_doubleclicked(0))
        {
            if( point_within(input->mouse_x,input->mouse_y,lxPos,lyPos,lx2Pos,ly2Pos ) )
            {
                if( globalResouceId == selectedId && GPE_Main_TabManager!=NULL )
                {
                    GPE_Main_TabManager->set_selected_gresource( heldResource );
                }
                returnAnswer = globalResouceId;
                set_current_gpe_project_from_name( projectParentFileName );
                if( CURRENT_PROJECT ==NULL)
                {
                    GPE_Report("NULL Project Found for"+opName+".");
                }
                LAST_CLICKED_RESOURCE = this;
                RESOURCE_TO_DRAG = NULL;
            }
        }
        else if( mouseInRange && input->check_mouse_released( mb_left))
        {
            if( point_within(input->mouse_x,input->mouse_y,lxPos,lyPos,lx2Pos,ly2Pos ) )
            {
                if(globalResouceId == selectedId)
                {
                    //GPE_set_selected_gresource(heldResource);
                }
                returnAnswer = globalResouceId;
                set_current_gpe_project_from_name( CURRENT_PROJECT_NAME = projectParentFileName );
                if( CURRENT_PROJECT ==NULL)
                {
                    GPE_Report("NULL Project Found for"+opName+".");
                }
                LAST_CLICKED_RESOURCE = this;
            }
        }
        else if( mouseInRange && input->check_mouse_released(1) )
        {
            if( point_within(input->mouse_x,input->mouse_y,viewedSpace->x-cam->x,lyPos,lx2Pos,ly2Pos) )
            {
                returnAnswer = globalResouceId;
                set_current_gpe_project_from_name(CURRENT_PROJECT_NAME = projectParentFileName );
                if( CURRENT_PROJECT ==NULL)
                {
                    GPE_Report("NULL Project Found for"+opName+".");
                }
                LAST_CLICKED_RESOURCE = this;
                RESOURCEMENU_WAS_RIGHTCLICKED = true;
            }
        }
        else if( mouseInRange && input->check_mouse_pressed(0) )
        {
            if( point_within(input->mouse_x,input->mouse_y,viewedSpace->x-cam->x,lyPos,lx2Pos,ly2Pos) )
            {
                RESOURCE_TO_DRAG = this;
            }
        }
        else if(mouseInRange && point_within(input->mouse_x,input->mouse_y,lxPos,lyPos,lx2Pos,ly2Pos) )
        {
            MAIN_OVERLAY->update_tooltip( opName );
        }

        if(heldResource!=NULL)
        {
            std::string heldResName = heldResource->get_name();
            if( opName!=heldResName )
            {
                if( heldResName.size()>0 && is_alnum(heldResName,false,true)  )
                {
                    resouceNameChanged = true;
                    opName = heldResName;
                }
            }
        }
    }
    return returnAnswer;
}

bool GPE_GeneralResourceContainer::read_data_from_projectfile(std::ofstream * fileTarget)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            return true;
        }
    }
    return false;
}

void GPE_GeneralResourceContainer::render_contained_object(GPE_Rect * viewedSpace,GPE_Rect *cam)
{

}

void GPE_GeneralResourceContainer::render_option( int xPos, int yPos, int selectedIdNumber, GPE_Rect * viewedSpace, GPE_Rect * cam, bool renderSubOptions, bool renderAutomatically)
{
    bool selfIsInView = false;
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        int rendY = yPos;
        if( rendY>=cam->y && rendY <=cam->y+cam->h )
        {
            selfIsInView = true;
        }
        else if( rendY+RESOURCE_AREA_HEIGHT >=cam->y && rendY+RESOURCE_AREA_HEIGHT <=cam->y+cam->h  )
        {
            selfIsInView = true;
        }

        if( isFolder && renderSubOptions )
        {
            if(subMenuIsOpen)
            {
                if( (selfIsInView || renderAutomatically) && (int)subOptions.size() > 0 )
                {
                    gfs->render_text( xPos-cam->x,yPos-cam->y,"-",GPE_MAIN_THEME->Main_Box_Font_Color,FONT_RESOURCEBAR,FA_LEFT,FA_TOP);
                }
                GPE_GeneralResourceContainer * foundContainer;
                int subXPos = xPos+RESOURCE_AREA_HEIGHT;
                int subYPos = yPos+RESOURCE_AREA_HEIGHT;
                for(int i=0; i< (int)subOptions.size(); i+=1)
                {
                    foundContainer = subOptions[i];
                    if(foundContainer!=NULL)
                    {
                        foundContainer->render_option( subXPos,subYPos,selectedIdNumber,viewedSpace,cam,renderSubOptions,renderAutomatically);
                        subYPos+=foundContainer->optionBox.h;
                    }
                }
            }
            else if( (selfIsInView || renderAutomatically ) && (int)subOptions.size() > 0 )
            {
                gfs->render_text( xPos-cam->x,yPos-cam->y,"+",GPE_MAIN_THEME->Main_Box_Font_Color,FONT_RESOURCEBAR,FA_LEFT,FA_TOP);
            }
        }

        if( selfIsInView || renderAutomatically)
        {
            if(globalResouceId ==selectedIdNumber && projectParentName.compare(CURRENT_PROJECT_NAME)==0 )
            {
                //gcanvas->render_rectangle( xPos-cam->x,yPos-cam->y,xPos+RESOURCE_AREA_HEIGHT*2+GENERAL_NEAR_ICON_YPADDING+strTexWidth-cam->x,yPos+16-cam->y,GPE_MAIN_THEME->Button_Box_Highlighted_Color,false,64);
                gcanvas->render_rectangle( -cam->x,yPos-cam->y,elementBox.w-cam->x,yPos+16-cam->y,GPE_MAIN_THEME->Button_Box_Highlighted_Color,false,64);
            }
            if( (isFolder || isSuperFolder || isSuperProjectFolder ) && containerTexture!=NULL )
            {
                if(projectParentName.compare(CURRENT_PROJECT_NAME)==0 && isSuperProjectFolder)
                {
                    containerTexture->render_tex_resized( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,RESOURCE_AREA_HEIGHT,RESOURCE_AREA_HEIGHT,NULL,GPE_MAIN_THEME->Main_Folder_Highlighted_Color);
                }
                else
                {
                    containerTexture->render_tex_resized( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,RESOURCE_AREA_HEIGHT,RESOURCE_AREA_HEIGHT,NULL,GPE_MAIN_THEME->Main_Folder_Color);
                }
            }
            else if( !isSuperProjectFolder && !isFolder && !isSuperFolder )
            {
                render_image( xPos,yPos,RESOURCE_AREA_HEIGHT,RESOURCE_AREA_HEIGHT,NULL,cam );
            }

            gfs->render_text( xPos+RESOURCE_AREA_HEIGHT+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y+RESOURCE_AREA_HEIGHT/2,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_RESOURCEBAR,FA_LEFT,FA_CENTER);
            /*if(heldResource!=NULL)
            {
                if(heldResource->is_modified() || resouceNameChanged)
                {
                    render_text( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,"!",GPE_MAIN_THEME->Button_Box_Selected_Color,FONT_RESOURCEBAR,FA_RIGHT,FA_TOP);
                }
            }
            else if( subContentsModified)
            {
                render_text( xPos-cam->x,yPos-cam->y,"!!",GPE_MAIN_THEME->Button_Box_Selected_Color,FONT_RESOURCEBAR,FA_RIGHT,FA_TOP);
            }*/

            if( (isSuperFolder || isSuperProjectFolder || resourceType == RESOURCE_TYPE_PROJECT_SETTINGS )&& renderAutomatically==false )
            {
                gcanvas->render_horizontal_line_color( rendY-cam->y,0,viewedSpace->w,GPE_MAIN_THEME->Main_Box_Font_Color,32);
            }
            lastResTypeRendered = resourceType;
        }
    }

}

void GPE_GeneralResourceContainer::render_image( int xPos, int yPos, int rWidth, int rHeight,GPE_Rect * viewedSpace,GPE_Rect *cam, GPE_Color * renderColor)
{
    if( renderColor == NULL )
    {
        renderColor = c_white;
    }
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool imageWasRendered = false;

    if( (isFolder || isSuperProjectFolder) && containerTexture!=NULL )
    {
        if(projectParentName.compare(CURRENT_PROJECT_NAME)==0 && isSuperProjectFolder)
        {
            containerTexture->render_tex_resized( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,rWidth,rHeight,NULL,GPE_MAIN_THEME->Main_Folder_Highlighted_Color);
        }
        else
        {
            containerTexture->render_tex_resized( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,rWidth,rHeight,NULL,GPE_MAIN_THEME->Main_Folder_Color);
        }
    }
    else if( !isSuperProjectFolder)
    {
        if( resourceType == RESOURCE_TYPE_OBJECT )
        {
            gameObjectResource * heldGOResource = (gameObjectResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                GPE_GeneralResourceContainer * allSpritesFolder = heldGOResource->projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SPRITE]+"s");
                if( allSpritesFolder!=NULL)
                {
                    GPE_GeneralResourceContainer * heldSprieOfObj = allSpritesFolder->find_resource_from_id(heldGOResource->spriteIndex);
                    if( heldSprieOfObj!=NULL)
                    {
                        heldSprieOfObj->render_image( xPos, yPos, rWidth, rHeight,viewedSpace,cam, renderColor);
                        imageWasRendered= true;
                    }
                }
            }
        }
        else if( resourceType == RESOURCE_TYPE_TEXTURE )
        {
            textureResource * heldGOResource = (textureResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                if( heldGOResource->textureInEditor!=NULL && heldGOResource->textureInEditor->get_width() > 0 )
                {
                    heldGOResource->textureInEditor->render_tex_resized( xPos-cam->x, yPos-cam->y, rWidth, rHeight,NULL);
                    imageWasRendered= true;
                }
            }
        }
        else if( resourceType == RESOURCE_TYPE_TILESHEET )
        {
            tilesheetResource * heldGOResource = (tilesheetResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                if( heldGOResource->tilesheetInEditor!=NULL && heldGOResource->tilesheetInEditor->tsImage!=NULL)
                {
                    heldGOResource->tilesheetInEditor->tsImage->render_tex_resized( xPos-cam->x, yPos-cam->y, rWidth, rHeight,NULL);
                    imageWasRendered= true;
                }
            }
        }
        else if( resourceType == RESOURCE_TYPE_ANIMATION || resourceType == RESOURCE_TYPE_SPRITE )
        {
            animationResource * heldAnimResource = (animationResource*) heldResource;
            if( heldAnimResource!=NULL && heldAnimResource->projectParentFolder!=NULL)
            {
                if( heldAnimResource->animInEditor!=NULL && heldAnimResource->animInEditor->has_texture()  )
                {
                    if( rWidth <=0 || rHeight <= 0 )
                    {
                        rWidth = heldAnimResource->animInEditor->get_width();
                        rHeight = heldAnimResource->animInEditor->get_height();
                        GPE_Report("Resizing sprite to "+ int_to_string(rWidth)+","+int_to_string(rHeight)+" px." );
                    }
                    heldAnimResource->animInEditor->render_piece_resized( xPos-cam->x,yPos-cam->y,rWidth,rHeight, NULL);
                    imageWasRendered= true;
                }
            }
        }
    }

    if( imageWasRendered==false && containerTexture!=NULL )
    {
        GPE_Color * imageColor  = GPE_MAIN_THEME->Main_Box_Font_Color;

        if( rWidth <=0 && rHeight <= 0 )
        {
            rWidth = rHeight = 32;
        }

        if(projectParentName.compare(CURRENT_PROJECT_NAME)==0 && isSuperProjectFolder)
        {
            containerTexture->render_tex_resized(xPos-cam->x,yPos-cam->y,rWidth,rHeight,NULL,GPE_MAIN_THEME->Main_Box_Font_Color);
        }
        else
        {
            containerTexture->render_tex_resized(  xPos-cam->x,yPos-cam->y,rWidth,rHeight,NULL,GPE_MAIN_THEME->Main_Box_Font_Color);
        }
    }

}

void GPE_GeneralResourceContainer::render_image_scaled( int xPos, int yPos, double xScale, double yScale,GPE_Rect * viewedSpace,GPE_Rect *cam, GPE_Color * renderColor)
{
    if( renderColor == NULL )
    {
        renderColor = c_white;
    }
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool imageWasRendered = false;

    if( (isFolder || isSuperProjectFolder) && containerTexture!=NULL )
    {
        if(projectParentName.compare(CURRENT_PROJECT_NAME)==0 && isSuperProjectFolder)
        {
            containerTexture->render_tex_scaled( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y, xScale,yScale,NULL, GPE_MAIN_THEME->Main_Folder_Highlighted_Color);
        }
        else
        {
            containerTexture->render_tex_scaled( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,xScale,yScale,NULL, GPE_MAIN_THEME->Main_Folder_Color );
        }
    }
    else if( !isSuperProjectFolder)
    {
        if( resourceType == RESOURCE_TYPE_OBJECT )
        {
            gameObjectResource * heldGOResource = (gameObjectResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                GPE_GeneralResourceContainer * allSpritesFolder = heldGOResource->projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SPRITE]+"s");
                if( allSpritesFolder!=NULL)
                {
                    GPE_GeneralResourceContainer * heldSprieOfObj = allSpritesFolder->find_resource_from_id(heldGOResource->spriteIndex);
                    if( heldSprieOfObj!=NULL)
                    {
                        heldSprieOfObj->render_image_scaled( xPos, yPos, xScale, yScale,viewedSpace,cam, renderColor);
                        imageWasRendered= true;
                    }
                }
            }
        }
        else if( resourceType == RESOURCE_TYPE_TEXTURE )
        {
            textureResource * heldGOResource = (textureResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                if( heldGOResource->textureInEditor!=NULL && heldGOResource->textureInEditor->get_width() > 0 )
                {
                    heldGOResource->textureInEditor->render_tex_scaled( xPos-cam->x, yPos-cam->y, xScale, yScale,NULL );
                    imageWasRendered= true;
                }
            }
        }
        else if( resourceType == RESOURCE_TYPE_TILESHEET )
        {
            tilesheetResource * heldGOResource = (tilesheetResource*) heldResource;
            if( heldGOResource!=NULL && heldGOResource->projectParentFolder!=NULL)
            {
                if( heldGOResource->tilesheetInEditor!=NULL && heldGOResource->tilesheetInEditor->tsImage!=NULL)
                {
                    heldGOResource->tilesheetInEditor->tsImage->render_tex_scaled( xPos-cam->x, yPos-cam->y, xScale, yScale,NULL);
                    imageWasRendered= true;
                }
            }
        }
        else if( resourceType == RESOURCE_TYPE_ANIMATION || resourceType == RESOURCE_TYPE_SPRITE )
        {
            animationResource * heldAnimResource = (animationResource*) heldResource;
            if( heldAnimResource!=NULL && heldAnimResource->projectParentFolder!=NULL)
            {
                if( heldAnimResource->animInEditor!=NULL && heldAnimResource->animInEditor->has_texture() )
                {
                    heldAnimResource->animInEditor->render_scaled( heldAnimResource->get_preview_frame(),xPos-cam->x,yPos-cam->y, xScale,yScale );
                    imageWasRendered= true;
                }
            }
        }
    }

    if( imageWasRendered==false && containerTexture!=NULL )
    {
        GPE_Color * imageColor  = GPE_MAIN_THEME->Main_Box_Font_Color;

        if(projectParentName.compare(CURRENT_PROJECT_NAME)==0 && isSuperProjectFolder)
        {
            containerTexture->render_tex_scaled( xPos-cam->x,yPos-cam->y, xScale, yScale, NULL,GPE_MAIN_THEME->Main_Box_Font_Color);
        }
        else
        {
            containerTexture->render_tex_scaled( xPos-cam->x,yPos-cam->y,xScale, yScale, NULL,GPE_MAIN_THEME->Main_Box_Font_Color);
        }
    }
}

void GPE_GeneralResourceContainer::remove_resource(GPE_GeneralResourceContainer * otherContainer, bool deleteResource)
{
    if( otherContainer!=NULL )
    {
        GPE_GeneralResourceContainer * tContainer = NULL;
        int resIdToDelete = otherContainer->get_global_id();
        for( int i = (int)subOptions.size()-1; i>=0; i--)
        {
            tContainer = subOptions[i];
            if( tContainer!=NULL && tContainer->get_global_id()==resIdToDelete )
            {
                optionBox.h-=otherContainer->optionBox.h;
                //otherContainer->parentResource = NULL;
                if( deleteResource )
                {
                    GPE_Main_TabManager->close_resource_tab(projectParentName, resIdToDelete );
                    delete tContainer;
                    tContainer = NULL;
                    otherContainer = NULL;
                    //GPE_Report("Rez deletedish...");
                }
                subOptions.erase( subOptions.begin()+i );
                //GPE_Report("Rez deleted...");
            }
        }
    }
}

void GPE_GeneralResourceContainer::save_container(std::string alternatePath, int backupId)
{
    if( heldResource!=NULL)
    {
        heldResource->save_resource(alternatePath, backupId);
    }
}

int GPE_GeneralResourceContainer::search_for_string(std::string needle)
{
    int foundStrings = 0;
    GPE_GeneralResourceContainer * fContainer  = NULL;
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        fContainer = subOptions[i];
        if( fContainer!=NULL)
        {
            foundStrings+=fContainer->search_for_string(needle);
        }
    }
    if( heldResource!=NULL)
    {
        foundStrings+= heldResource->search_for_string(needle);
    }
    return foundStrings;
}

int GPE_GeneralResourceContainer::search_and_replace_string(std::string needle, std::string newStr)
{
    int foundStrings = 0;
    GPE_GeneralResourceContainer * fContainer  = NULL;
    for( int i = (int)subOptions.size()-1; i >=0; i--)
    {
        fContainer = subOptions[i];
        if( fContainer!=NULL)
        {
            foundStrings+=fContainer->search_and_replace_string(needle, newStr);
        }
    }
    if( heldResource!=NULL)
    {
        foundStrings+=heldResource->search_and_replace_string(needle, newStr);
    }
    return foundStrings;
}

void GPE_GeneralResourceContainer::set_basic_image_value()
{
    if( isFolder)
    {
        containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/folder.png");
    }
    else
    {
        switch( resourceType)
        {
        case RESOURCE_TYPE_SPRITE:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/magnet.png");
            break;
        case RESOURCE_TYPE_TEXTURE:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/image.png");
            break;
        case RESOURCE_TYPE_TILESHEET:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/th-large.png");
            break;
        case RESOURCE_TYPE_AUDIO:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/volume-up.png");
            break;
        case RESOURCE_TYPE_VIDEO:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/film.png");
            break;
        case RESOURCE_TYPE_OBJECT:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/automobile.png");
            break;
        case RESOURCE_TYPE_FUNCTION:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/code.png");
            break;
        case RESOURCE_TYPE_FONT:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/font.png");
            break;
        case RESOURCE_TYPE_SCENE:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/area-chart.png");
            break;
        default:
            containerTexture = guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/diamond.png");
            break;
        }
    }
}

void GPE_GeneralResourceContainer::set_held_resource(generalGameResource * newGenResource)
{
    heldResource = newGenResource;
}

void GPE_GeneralResourceContainer::set_global_id(int newId)
{
    globalResouceId = newId;
    if( heldResource!=NULL)
    {
        heldResource->set_global_rid(newId);
    }
}

void GPE_GeneralResourceContainer::set_container_gameid(int newId)
{
    exportBuildGlobalId = newId;
    if( heldResource!=NULL)
    {
        heldResource->exportBuildGlobalId = newId;
    }
}

void GPE_GeneralResourceContainer::set_name(std::string newName)
{
    opName = newName;
    int textW = 0;
    int textH = 0;

    if( (int)opName.size()>0 && FONT_TOOLBAR!=NULL)
    {
        FONT_TOOLBAR->get_metrics(opName,&textW, &textH);
    }
    strTexWidth = textW;
    strTexHeight = textH;
    if( heldResource!=NULL)
    {
        heldResource->set_name(newName);
    }
}

void GPE_GeneralResourceContainer::set_project_parent_name(std::string newParentName)
{
    projectParentFileName = newParentName;
    if( heldResource!=NULL)
    {
        heldResource->parentProjectName = projectParentFileName;
    }
    int sSize = (int)subOptions.size();
    if( sSize > 0)
    {
        GPE_GeneralResourceContainer * cResource = NULL;
        for( int i = 0; i < sSize; i++)
        {
            cResource = subOptions[i];
            if( cResource!=NULL)
            {
                cResource->set_project_parent_name(newParentName);
            }
        }
    }
}

bool GPE_GeneralResourceContainer::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    bool foundProblem = false;
    std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            if( isSuperProjectFolder)
            {
                *fileTarget << "[Project=" << opName<< "]\n";
            }
            else if(isSuperFolder)
            {
                *fileTarget << nestedTabsStr << "[SuperFolder=" << opName<< "]\n";
                if( GPE_LOADER != NULL )
                {
                    GPE_LOADER->update_submessages( "Saving "+opName+" resources", "Please wait..." );
                }
            }
            else if( isFolder)
            {
                *fileTarget << nestedTabsStr << "[Folder=" << opName << "," << globalResouceId << "]\n";
            }
            if( heldResource!=NULL)
            {
                set_name( heldResource->get_current_name() );
                heldResource->save_resource();
                foundProblem = heldResource->write_data_into_projectfile(fileTarget,nestedFoldersIn);
            }
            int sSize = (int)subOptions.size();
            if( sSize > 0)
            {
                GPE_GeneralResourceContainer * cResource = NULL;
                for( int i = 0; i < sSize; i++)
                {
                    cResource = subOptions[i];
                    if( cResource!=NULL)
                    {
                        if( cResource->write_data_into_projectfile(fileTarget,nestedFoldersIn+1) )
                        {
                            foundProblem = true;
                        }
                    }
                }
            }
            if( isSuperProjectFolder)
            {
                *fileTarget << nestedTabsStr << "[/Project]\n";
            }
            else if(isSuperFolder)
            {
                *fileTarget << nestedTabsStr << "[/SuperFolder]\n";
            }
            else if( isFolder)
            {
                *fileTarget << nestedTabsStr << "[/Folder]\n";
            }
        }
    }
    if( foundProblem==false)
    {
        resouceNameChanged = false;
    }
    return !foundProblem;
}

GPE_ProjectFolder::GPE_ProjectFolder(std::string name, std::string directoryIn, std::string fileNameIn, std::string projectLanguageIn, bool createBlankScene )
{
    projectRSM = new GPE_DataManager();
    int iLayerN = 0;
    for( iLayerN = 0; iLayerN < 32; iLayerN++)
    {
        projectLayerNames[iLayerN] = "";
    }

    projectLayerNames[0] = "Default Background Layer";
    projectLayerNames[1] = "Default Object Layer";
    for( iLayerN = 2; iLayerN < 24; iLayerN++)
        for( iLayerN = 2; iLayerN < 24; iLayerN++)
        {
            projectLayerNames[iLayerN] = "Custom Layer"+int_to_string(iLayerN-2);
        }
    for( iLayerN = 24; iLayerN < 32; iLayerN++)
    {
        projectLayerNames[iLayerN] = "Default Tile Layer"+int_to_string(iLayerN-24);
    }


    //Defaults to JS Project Language
    myProjectLanguage = PROGRAM_LANGUAGE_JS;

    //Changes Project Language if name is found...
    if( (int)projectLanguageIn.size() > 0 )
    {
        GPE_Gui_Engine_Language * tLanguage  = NULL;
        int maxLanguageCount = GPE_MAIN_HIGHLIGHTER->get_language_count();
        for( int cLanguage = 0; cLanguage < maxLanguageCount; cLanguage++ )
        {
            tLanguage = GPE_MAIN_HIGHLIGHTER->get_language_object(cLanguage);
            //checks if the found lanuage isnt NULL and is a coding language
            if( tLanguage!=NULL && tLanguage->isCodingLanguage)
            {
                //if its regular name or short name matches. We end loop and change our project language.
                if( projectLanguageIn == tLanguage->languageName || projectLanguageIn == tLanguage->languageShortName )
                {
                    myProjectLanguage = cLanguage;
                    break;
                }
            }
        }
    }
    time( &lastTimeBackedUp);
    GLOBAL_REZ_ID_COUNT = 0;
    projectName = name;
    projectStartDirectoryName = projectDirectory = directoryIn;
    projectFileName = fileNameIn;
    RESC_PROJECT_FOLDER = NULL;
    RESC_SPRITES = NULL;
    RESC_TEXTURES = NULL;
    RESC_TILESHEETS = NULL;
    RESC_3DMODELS = NULL;
    RESC_AUDIO = NULL;
    RESC_VIDEOS = NULL;
    RESC_FUNCTIONS = NULL;
    RESC_OBJECTS = NULL;
    RESC_CLASSES = NULL;
    RESC_PATHS = NULL;
    RESC_SCENES = NULL;
    RESC_ACHIEVEMENTS = NULL;
    RESC_FONTS = NULL;
    RESC_SHADERS = NULL;
    for( int i = 0; i < res_type_count; i++)
    {
        RESC_ALL[i] = NULL;
        CREATED_RESOURCE_COUNT[i] = 0;
    }
    RESC_PROJECT_FOLDER = GPE_MAIN_GUI->mainResourceTree->add_project_folder(0,projectFileName,projectName);
    RESC_PROJECT_FOLDER->parentProjectDirectory = projectDirectory;
    RESC_PROJECT_FOLDER->projectParentFileName = projectFileName;
    RESC_PROJECT_FOLDER->open_folder();

    RESC_TEXTURES =  RESC_ALL[RESOURCE_TYPE_TEXTURE] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_TEXTURE,"Textures", increment_resouce_count(), restype_superfolder);
    RESC_TILESHEETS =  RESC_ALL[RESOURCE_TYPE_TILESHEET] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_TILESHEET,"Tilesheets", increment_resouce_count(), restype_superfolder);
    RESC_ANIMATIONS = RESC_SPRITES =  RESC_ALL[RESOURCE_TYPE_SPRITE] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_ANIMATION,"Animations", increment_resouce_count(), restype_superfolder);
    RESC_AUDIO =  RESC_ALL[RESOURCE_TYPE_AUDIO] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_AUDIO,"Audio", increment_resouce_count(), restype_superfolder);
    RESC_VIDEOS =  RESC_ALL[RESOURCE_TYPE_VIDEO] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_VIDEO,"Videos", increment_resouce_count(), restype_superfolder);
    RESC_FUNCTIONS =  RESC_ALL[RESOURCE_TYPE_FUNCTION] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_FUNCTION,"Functions", increment_resouce_count(), restype_superfolder);
    RESC_PATHS =  RESC_ALL[RESOURCE_TYPE_PATH] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_PATH,"Paths", increment_resouce_count(), restype_superfolder);
    RESC_EMITTERS =  RESC_ALL[RESOURCE_TYPE_EMITTER] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_EMITTER,"Particles", increment_resouce_count(), restype_superfolder);
    RESC_LIGHTS =  RESC_ALL[RESOURCE_TYPE_LIGHT] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_LIGHT,"Lights", increment_resouce_count(), restype_superfolder);
    RESC_OBJECTS =  RESC_ALL[RESOURCE_TYPE_OBJECT] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_OBJECT,"Objects", increment_resouce_count(), restype_superfolder);
    RESC_CLASSES =  RESC_ALL[RESOURCE_TYPE_CLASS] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_CLASS,"Classes", increment_resouce_count(), restype_superfolder);
    RESC_SCENES =  RESC_ALL[RESOURCE_TYPE_SCENE] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_SCENE,"Scenes", increment_resouce_count(), restype_superfolder);
    //RESC_ACHIEVEMENTS =  RESC_ALL[RESOURCE_TYPE_ACHIEVEMENT] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_ACHIEVEMENT,"Achievements", increment_resouce_count(), restype_superfolder);
    RESC_FONTS =  RESC_ALL[RESOURCE_TYPE_FONT] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_FONT,"Fonts", increment_resouce_count(), restype_superfolder);
    RESC_DICTIONARIES =  RESC_ALL[RESOURCE_TYPE_DICTIONARY]  = NULL;//=RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_DICTIONARY,"Dictionaries", increment_resouce_count(), restype_superfolder);

    int projectPropertiesId = increment_resouce_count();
    RESC_PROJECT_SETTINGS = new GPE_GeneralResourceContainer(projectFileName, RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PROJECT_SETTINGS], RESOURCE_TYPE_PROJECT_SETTINGS,projectPropertiesId,false, projectPropertiesId,-1);
    RESC_PROJECT_SETTINGS->parentProjectDirectory = projectDirectory;

    RESC_PROJECT_FOLDER->add_resource_container(RESC_PROJECT_SETTINGS,true);
    projectPropertiesResource * projectProps = new projectPropertiesResource(RESC_PROJECT_FOLDER);
    projectProps->set_name( RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PROJECT_SETTINGS] );
    projectProps->set_parent_project( projectFileName );
    RESC_PROJECT_SETTINGS->set_held_resource(projectProps);
    GLOBAL_REZ_ID_COUNT = 1000;
    projectFilePreviousVersion = projectFileVersion = GPE_VERSION_DOUBLE_NUMBER;
    if( createBlankScene )
    {
        create_blank_scene(RESC_SCENES,"start_scene");
    }
}

GPE_ProjectFolder::~GPE_ProjectFolder()
{
    GPE_Report("Deleting RESC_PROJECT_FOLDER" );
    if( RESC_PROJECT_FOLDER!=NULL )
    {
        delete RESC_PROJECT_FOLDER;
        RESC_PROJECT_FOLDER = NULL;
    }

    if( projectRSM!=NULL )
    {
        delete projectRSM;
        projectRSM = NULL;
    }
}

bool GPE_ProjectFolder::add_project_function(std::string nName, std::string nDescription,std::string  nParameters, std::string tFunctionReturnType,std::string nScope)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)projectFunctions.size()-1; i>=0; i--)
    {
        tTerm = projectFunctions[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    tTerm = new GPE_Compiler_Term(nName, nParameters, tFunctionReturnType, nDescription, CTERM_FUNCTION,nScope);
    projectFunctions.push_back( tTerm);
    return true;
}

bool GPE_ProjectFolder::add_project_keyword(std::string nName, std::string nDescription,int nType,std::string nScope)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)projectKeywords.size()-1; i>=0; i--)
    {
        tTerm = projectKeywords[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    tTerm = new GPE_Compiler_Term(nName, "", "", nDescription, nType,nScope);
    projectKeywords.push_back( tTerm);
    return true;
}

void GPE_ProjectFolder::clear_project_functions()
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)projectFunctions.size()-1; i>=0; i--)
    {
        tTerm = projectFunctions[i];
        if( tTerm!=NULL)
        {
            delete tTerm;
            tTerm = NULL;
        }
    }
    projectFunctions.clear();
}

void GPE_ProjectFolder::clear_project_keywords()
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)projectKeywords.size()-1; i>=0; i--)
    {
        tTerm = projectKeywords[i];
        if( tTerm!=NULL)
        {
            delete tTerm;
            tTerm = NULL;
        }
    }
    projectKeywords.clear();
}

bool GPE_ProjectFolder::export_and_play_native(bool launchProgram)
{
    if( RESC_PROJECT_SETTINGS!=NULL && RESC_PROJECT_SETTINGS->get_held_resource()!=NULL)
    {
        projectPropertiesResource * projectSettingsObject = (projectPropertiesResource*)RESC_PROJECT_SETTINGS->get_held_resource();
        return projectSettingsObject->export_and_play_native(launchProgram);
    }
    return false;
}

bool GPE_ProjectFolder::clean_build_folder( int buildMetaTemplate )
{
    std::string projectBuildDirectory = "";
    std::string folderDeletionName  = "";
    if( buildMetaTemplate == GPE_BUILD_HTML5)
    {
        projectBuildDirectory  = fileToDir(projectFileName)+"/gpe_project/builds/html5";
        folderDeletionName = "HTML5";
    }
    else if( buildMetaTemplate == GPE_BUILD_WIIU)
    {
        projectBuildDirectory  = fileToDir(projectFileName)+"/gpe_project/builds/wiiu";
        folderDeletionName = "WiiU";
    }
    else if( buildMetaTemplate == GPE_BUILD_WINDOWS)
    {
        projectBuildDirectory  = fileToDir(projectFileName)+"/gpe_project/builds/windows";
        folderDeletionName = "WINDOWS";
    }
    else if( buildMetaTemplate == GPE_BUILD_LINUX)
    {
        projectBuildDirectory  = fileToDir(projectFileName)+"/gpe_project/builds/linux";
        folderDeletionName = "LINUX";
    }
    else if( buildMetaTemplate == GPE_BUILD_MAC)
    {
        projectBuildDirectory  = fileToDir(projectFileName)+"/gpe_project/builds/osx";
        folderDeletionName = "OSX";
    }
    if( (int)projectBuildDirectory.size() > 0 )
    {
        if( path_exists(projectBuildDirectory) )
        {
            if( GPE_Display_Basic_Prompt("[WARNING]Function deletion of "+folderDeletionName+" build folder?","Are you sure you will like to delete all the contents of this build directory? This action is irreversible!")==DISPLAY_QUERY_YES)
            {
                GPE_Main_Logs->log_build_line("---");
                GPE_Main_Logs->log_build_line("Cleaning Project ["+projectName+" build folder:");
                GPE_Main_Logs->log_build_line("["+projectBuildDirectory+"]...");
                int filesDeleted =  clean_folder(projectBuildDirectory);
                if( filesDeleted > 0)
                {
                    GPE_Main_Logs->log_build_line("Folder content files were deleted successfully. Estimated ["+int_to_string(filesDeleted)+"] files deleted...");
                }
                else
                {
                    GPE_Main_Logs->log_build_line("0 files were prepared for deleted.");
                }
                GPE_Main_Logs->log_build_line("---");
                return filesDeleted;
            }
        }
        else if( GPE_Main_Logs!=NULL )
        {
            GPE_Main_Logs->log_build_line("---");
            GPE_Main_Logs->log_build_line("Cleaning Project ["+projectName+" build folder:");
            GPE_Main_Logs->log_build_line("["+projectBuildDirectory+"] was not detected.");
            GPE_Main_Logs->log_build_line("Cleaning process aborted.");
        }
    }

    return false;
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_folder(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    if( folderContainer!=NULL )
    {
        if( newResId < 0)
        {
            newResId = increment_resouce_count();
        }
        if( (int)newName.size() <= 0)
        {
            newName = "New Folder";
        }
        GPE_GeneralResourceContainer *  newFolder= folderContainer->add_resource_folder(newName,newResId,-1);
        newFolder->projectParentFileName = projectFileName;
        //folderContainer->open_folder();
        return newFolder;
    }
    return NULL;
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_resource(int rNewType, GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    if( rNewType >= 0 && rNewType <= RESOURCE_TYPE_ACHIEVEMENT )
    {
        GPE_GeneralResourceContainer * RES_FOLDER_HOLDER = RESC_ALL[rNewType];
        if(RES_FOLDER_HOLDER!=NULL)
        {
            CREATED_RESOURCE_COUNT[rNewType]+=1;
            int resourceNumb = CREATED_RESOURCE_COUNT[rNewType];
            if( (int)newName.size() <= 0)
            {
                newName = "new"+RESOURCE_TYPE_NAMES[rNewType]+int_to_string(resourceNumb );
            }
            standardEditableGameResource * newProjectResource;
            switch( rNewType)
            {
            case RESOURCE_TYPE_AUDIO:
                newProjectResource = new audioResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_VIDEO:
                newProjectResource = new videoResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_FONT:
                newProjectResource = new fontResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_FUNCTION:
                newProjectResource = new functionResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_CLASS:
                newProjectResource = new classResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_LIGHT:
                newProjectResource = new lightResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_OBJECT:
                newProjectResource = new gameObjectResource(RESC_PROJECT_FOLDER);
                break;
            //Added since Version 1.12 [BEGIN]
            case RESOURCE_TYPE_PATH:
                newProjectResource = new gamePathResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_EMITTER:
                newProjectResource = new particleResource(RESC_PROJECT_FOLDER);
                break;
            //Added since Version 1.12 [END]
            case RESOURCE_TYPE_SCENE:
                newProjectResource = new gameSceneResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_SPRITE:
                newProjectResource = new animationResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_TEXTURE:
                newProjectResource = new textureResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_TILESHEET:
                newProjectResource = new tilesheetResource(RESC_PROJECT_FOLDER);
                break;
            default:
                newProjectResource = NULL;
                break;
            }

            if( folderContainer==NULL )
            {
                folderContainer = RES_FOLDER_HOLDER;
            }

            if( folderContainer!=NULL )
            {
                if( newProjectResource!=NULL)
                {
                    if( newResId < 0)
                    {
                        newProjectResource->set_global_rid( increment_resouce_count() );
                    }
                    else
                    {
                        newProjectResource->set_global_rid( newResId );
                    }
                    newProjectResource->set_name( newName);
                    newProjectResource->set_parent_name( projectFileName);
                    newProjectResource->parentProjectName = projectFileName;
                    newProjectResource->parentProjectDirectory = projectDirectory;
                    newProjectResource->resourceType = rNewType;
                    GPE_GeneralResourceContainer * newContainer = new GPE_GeneralResourceContainer(projectFileName,newName,rNewType,resourceNumb,false,newProjectResource->get_global_rid() );
                    newContainer->set_held_resource(newProjectResource);
                    folderContainer->add_resource_container(newContainer);
                    //folderContainer->open_folder();
                    newContainer->parentProjectDirectory = projectDirectory;
                    return newContainer;
                }
            }
        }
    }
    return NULL;
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_audio( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_AUDIO, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_video( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_VIDEO, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_emitter( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_EMITTER, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_light( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_LIGHT, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_path( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_PATH, folderContainer, newName, newResId);
}

//
GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_font(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_FONT, folderContainer, newName, newResId);
}
//

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_function(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_FUNCTION, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_class(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_CLASS, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_sprite(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_SPRITE, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_texture(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_TEXTURE, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_tilesheet(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_TILESHEET, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_gameobject(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_OBJECT, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_scene(GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_SCENE, folderContainer, newName, newResId);
}

//Export C++
bool GPE_ProjectFolder::export_project_cpp(std::string projectBuildDirectory, int buildMetaTemplate, bool runGameOnCompile, bool inDebugMode)
{

}
//Export HTML5
//Build HTML5
bool GPE_ProjectFolder::export_project_html5(std::string projectBuildDirectory, int buildMetaTemplate, bool runGameOnCompile, bool inDebugMode)
{
    GPE_GeneralResourceContainer * firstObjectContainer = RESC_OBJECTS->get_usable_resource();
    GPE_GeneralResourceContainer * firstSceneContainer = RESC_SCENES->get_usable_resource();
    bool hadSaveErrors = false;
    int BUILD_SCREEN_WIDTH = 640;
    int BUILD_SCREEN_HEIGHT = 480;

    GPE_MAIN_HIGHLIGHTER->begin_compiling();
    if( GPE_Main_Logs!=NULL)
    {
        if( buildMetaTemplate!=GPE_BUILD_WINDOWS && buildMetaTemplate!=GPE_BUILD_MAC  && buildMetaTemplate!=GPE_BUILD_LINUX )
        {
            GPE_Main_Logs->clear_build_log();
            GPE_Main_Logs->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[buildMetaTemplate]+" Export] (Compiler: Pawbitious Compiler)---------------");
        }
        GPE_Main_Logs->log_build_line("Exporting JS Based Project...");

        if( firstSceneContainer!=NULL)
        {
            std::string projectFirstLevelName = firstSceneContainer->get_name();
            if( firstObjectContainer!=NULL && GPE_MAIN_HIGHLIGHTER!=NULL)
            {

                if( projectBuildDirectory.empty() )
                {
                    projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/html5";
                }
                if( buildMetaTemplate==GPE_BUILD_HTML5)
                {
                    projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/html5";
                    GPE_MAIN_GUI->setup_build_folder(projectBuildDirectory,GPE_BUILD_HTML5,0,inDebugMode );
                }
                //Grabs the list of resources and such.
                std::vector <GPE_GeneralResourceContainer *> buildTextureOptions;
                std::vector <GPE_GeneralResourceContainer *> buildTilesheetOptions;
                std::vector <GPE_GeneralResourceContainer *> buildSpriteOptions;
                std::vector <GPE_GeneralResourceContainer *> buildAudioOptions;
                std::vector <GPE_GeneralResourceContainer *> buildVideoOptions;
                std::vector <GPE_GeneralResourceContainer *> buildPathsOptions;
                std::vector <GPE_GeneralResourceContainer *> buildFunctionsOptions;
                std::vector <GPE_GeneralResourceContainer *> buildFontOptions;
                //Added as of 1.14 [ BEGIN ]
                std::vector <GPE_GeneralResourceContainer *> buildClassesOptions;
                //Added as of 1.14 [ END ]
                std::vector <GPE_GeneralResourceContainer *> buildGameObjectOptions;
                std::vector <GPE_GeneralResourceContainer *> buildGameSceneOptions;
                projectPropertiesResource * projectSettingsObject = (projectPropertiesResource*)RESC_PROJECT_SETTINGS->get_held_resource();
                RESC_TEXTURES->grab_useable_resources(buildTextureOptions);
                RESC_TILESHEETS->grab_useable_resources(buildTilesheetOptions);
                RESC_SPRITES->grab_useable_resources(buildSpriteOptions);
                RESC_AUDIO->grab_useable_resources(buildAudioOptions);
                RESC_VIDEOS->grab_useable_resources(buildVideoOptions);
                RESC_PATHS->grab_useable_resources(buildPathsOptions);
                RESC_FUNCTIONS->grab_useable_resources(buildFunctionsOptions);
                RESC_FONTS->grab_useable_resources(buildFontOptions);
                //Added as of 1.14 [ BEGIN ]
                RESC_CLASSES->grab_useable_resources(buildClassesOptions);
                //Added as of 1.14 [ END ]
                RESC_OBJECTS->grab_useable_resources(buildGameObjectOptions);
                RESC_SCENES->grab_useable_resources(buildGameSceneOptions);

                //For Mega Looking of resources:
                std::vector <GPE_GeneralResourceContainer *> buildGameBuildAllOptions;
                RESC_TEXTURES->grab_useable_resources(buildGameBuildAllOptions);
                RESC_TILESHEETS->grab_useable_resources(buildGameBuildAllOptions);
                RESC_SPRITES->grab_useable_resources(buildGameBuildAllOptions);
                RESC_AUDIO->grab_useable_resources(buildGameBuildAllOptions);
                RESC_VIDEOS->grab_useable_resources(buildGameBuildAllOptions);
                RESC_PATHS->grab_useable_resources(buildGameBuildAllOptions);
                RESC_FUNCTIONS->grab_useable_resources(buildGameBuildAllOptions);
                RESC_FONTS->grab_useable_resources(buildGameBuildAllOptions);
                //Added as of 1.14 [ BEGIN ]
                RESC_CLASSES->grab_useable_resources(buildGameBuildAllOptions);
                //Added as of 1.14 [ END ]
                RESC_OBJECTS->grab_useable_resources(buildGameBuildAllOptions);
                RESC_SCENES->grab_useable_resources(buildGameBuildAllOptions);

                currentObjParents.clear();
                nextObjParents.clear();
                //temp variables for looping through containers
                int iRes = 0;
                int paramLine = 0;
                int jFontType = 0;
                std::string copyFileDestination;
                int tempParentHTML5Id;
                int foundHtml5BuildId;
                GPE_GeneralResourceContainer * tempContainer = NULL;
                for( iRes = 0; iRes < (int)buildGameBuildAllOptions.size(); iRes++)
                {
                    tempContainer = buildGameBuildAllOptions[iRes];
                    if( tempContainer!=NULL)
                    {
                        if( is_alnum( tempContainer->get_name(),false,true ) )
                        {
                            if( !GPE_MAIN_HIGHLIGHTER->is_compilable_word( tempContainer->get_name() ) )
                            {
                                hadSaveErrors = true;
                                GPE_Main_Logs->log_build_error("Invalid Resource Name Given ["+tempContainer->get_name()+"] conflicts with the GPE engine variable. Please rename your resource!");
                            }
                        }
                        else
                        {
                            hadSaveErrors = true;
                            GPE_Main_Logs->log_build_error("Invalid Resource Name Given ["+tempContainer->get_name()+"].");
                        }
                    }
                }

                if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                {
                    if( projectSettingsObject->projectScreenWidthField->get_held_int() > 0)
                    {
                        BUILD_SCREEN_WIDTH = std::max(160, projectSettingsObject->projectScreenWidthField->get_held_int() );
                    }

                    if( projectSettingsObject->projectScreenHeightField->get_held_int() > 0)
                    {
                        BUILD_SCREEN_HEIGHT = std::max(120,projectSettingsObject->projectScreenHeightField->get_held_int() );
                    }
                    //temp variables for held resources and containers

                    //for preloading of surface and such
                    std::vector <std::string > preloadImgTextures;
                    std::vector <std::string > preloadImgTilesheets;
                    std::vector <std::string > preloadImgSprites;
                    std::vector <std::string > preloadAudio;

                    animationResource * tempSprRes = NULL;
                    textureResource * tempTexRes = NULL;
                    tilesheetResource * tempTstRes = NULL;
                    audioResource * tempAudRes = NULL;
                    videoResource * tempVidRes = NULL;
                    gamePathResource * tempPathRes = NULL;
                    functionResource * tempFuncRes = NULL;
                    //Added as of 1.14 [ BEGIN ]
                    classResource * tempClassRes = NULL;
                    //Added as of 1.14 [ END ]
                    gameObjectResource * tempObjRes = NULL;
                    gameSceneResource * tempScnRes = NULL;
                    //achievementResource * tempAchRes = NULL;
                    fontResource * tempFntRes = NULL;

                    appendToFile(get_user_settings_folder()+"resources_check.txt","Checking all resources..");
                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Textures]");

                    bool hasTextureToUse = false;
                    int amountOfObjectsCompiled = 0;
                    for( iRes = 0; iRes < (int)buildTextureOptions.size(); iRes++)
                    {
                        tempContainer = buildTextureOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                            hasTextureToUse = false;
                            if( tempContainer->get_held_resource()!=NULL)
                            {
                                tempTexRes = (textureResource * )tempContainer->get_held_resource();
                                if( ( tempTexRes!=NULL) && ( tempTexRes->textureInEditor!=NULL) && ( tempTexRes->textureInEditor->get_width() > 0 ) )
                                {
                                    hasTextureToUse = true;
                                    if( tempTexRes->textureInEditor->copy_image_source(projectBuildDirectory+"/resources/textures")  )
                                    {
                                        preloadImgTextures.push_back( getShortFileName(tempTexRes->textureInEditor->get_filename()) );
                                    }
                                }
                                if( hasTextureToUse == false)
                                {
                                    appendToFile(get_user_settings_folder()+"resources_check.txt","Does not contain texture...");
                                }
                            }
                        }
                    }

                    appendToFile(get_user_settings_folder()+"resources_check.txt","[TileSheets]");
                    for( iRes = 0; iRes < (int)buildTilesheetOptions.size(); iRes++)
                    {
                        tempContainer = buildTilesheetOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                            hasTextureToUse = false;
                            if( tempContainer->get_held_resource()!=NULL)
                            {
                                tempTstRes = (tilesheetResource * )tempContainer->get_held_resource();
                                if( ( tempTstRes!=NULL) && ( tempTstRes->tilesheetInEditor!=NULL) && ( tempTstRes->tilesheetInEditor->tsImage!=NULL )&& (tempTstRes->tilesheetInEditor->tsImage->get_width() > 0) )
                                {
                                    hasTextureToUse = true;
                                    if( tempTstRes->tilesheetInEditor->tsImage->copy_image_source(projectBuildDirectory+"/resources/tilesheets")  )
                                    {
                                        preloadImgTilesheets.push_back( getShortFileName(tempTstRes->tilesheetInEditor->tsImage->get_filename()) );
                                    }
                                }
                                if( hasTextureToUse == false)
                                {
                                    appendToFile(get_user_settings_folder()+"resources_check.txt","Tilesheet Does not contain texture image...");
                                }
                            }
                        }
                    }
                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Sprites]");

                    for( iRes = 0; iRes < (int)buildSpriteOptions.size(); iRes++)
                    {
                        tempContainer = buildSpriteOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                            hasTextureToUse = false;
                            if( tempContainer->get_held_resource()!=NULL)
                            {
                                tempSprRes = (animationResource * )tempContainer->get_held_resource();
                                if( ( tempSprRes!=NULL) && ( tempSprRes->animInEditor!=NULL) && ( tempSprRes->animInEditor->has_texture() )&& (tempSprRes->animInEditor->get_width() > 0) )
                                {
                                    hasTextureToUse = true;
                                    tempSprRes->save_resource();
                                    if( tempSprRes->animInEditor->copy_image_source(projectBuildDirectory+"/resources/animations")  )
                                    {
                                        preloadImgSprites.push_back( getShortFileName(tempSprRes->animInEditor->get_file_name() ) );
                                    }
                                }
                                if( hasTextureToUse == false)
                                {
                                    appendToFile(get_user_settings_folder()+"resources_check.txt","Sprite Does not contain texture image...");
                                }
                            }
                        }
                    }

                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Audio]");
                    for( iRes = 0; iRes < (int)buildAudioOptions.size(); iRes++)
                    {
                        tempContainer = buildAudioOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                        }
                    }

                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Videos]");
                    for( iRes = 0; iRes < (int)buildVideoOptions.size(); iRes++)
                    {
                        tempContainer = buildVideoOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                        }
                    }

                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Paths]");
                    for( iRes = 0; iRes < (int)buildPathsOptions.size(); iRes++)
                    {
                        tempContainer = buildPathsOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +" - "+int_to_string(iRes)+"...");
                        }
                    }

                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Functions]");
                    for( iRes = 0; iRes < (int)buildFunctionsOptions.size(); iRes++)
                    {
                        tempContainer = buildFunctionsOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                        }
                    }

                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Fonts]");
                    for( iRes = 0; iRes < (int)buildFontOptions.size(); iRes++)
                    {
                        tempContainer = buildFontOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempFntRes = (fontResource * ) tempContainer->get_held_resource();
                            tempContainer->set_container_gameid ( iRes);
                            if( tempFntRes!=NULL)
                            {
                                for( jFontType = 0; jFontType < FONT_FILE_TYPES; jFontType++)
                                {
                                    if( (int)tempFntRes->storedFontFileNames[jFontType].size() > 3)
                                    {
                                        copyFileDestination = projectBuildDirectory+"/resources/fonts/"+getShortFileName(tempFntRes->storedFontFileNames[jFontType],true);
                                        if( copy_file(tempFntRes->storedFontFileNames[jFontType],copyFileDestination)==false )
                                        {
                                            appendToFile(get_user_settings_folder()+"resources_check.txt","Unable to copy ["+tempFntRes->storedFontFileNames[jFontType]+"] to ["+copyFileDestination+"]...");
                                        }
                                    }
                                }
                            }
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                        }
                    }

                    //Added as of 1.14 [ BEGIN ]
                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Custom Classes]");
                    for( iRes = 0; iRes < (int)buildClassesOptions.size(); iRes++)
                    {
                        tempContainer = buildClassesOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                        }
                    }
                    //Added as of 1.14 [ END ]

                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Game Objects]");
                    for( iRes = 0; iRes < (int)buildGameObjectOptions.size(); iRes++)
                    {
                        tempContainer = buildGameObjectOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                        }
                    }

                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Scenes]");
                    for( iRes = 0; iRes < (int)buildGameSceneOptions.size(); iRes++)
                    {
                        tempContainer = buildGameSceneOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                        }
                    }

                    //
                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Fonts]");
                    for( iRes = 0; iRes < (int)buildFontOptions.size(); iRes++)
                    {
                        tempContainer = buildFontOptions[iRes];
                        if( tempContainer!=NULL)
                        {
                            tempContainer->set_container_gameid ( iRes);
                            appendToFile(get_user_settings_folder()+"resources_check.txt",tempContainer->get_name() +"...");
                        }
                    }


                    if( buildMetaTemplate >=0 && buildMetaTemplate < GPE_BUILD_OPTIONS)
                    {
                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_messages( GPE_BUILD_NAMES[buildMetaTemplate]+" Export", "Please Wait...", "DO NOT CLOSE" );
                        }
                    }
                    else
                    {
                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_messages( "Unknown Export", "Please Wait...", "DO NOT CLOSE" );
                        }
                    }
                    //displayMessageString = projectName;

                    //GPE Engine Files
                    std::string buildFileStyleCssFilleName = APP_DIRECTORY_NAME+"build_files/css/style.css";
                    std::string indexStyleCssFileName = projectBuildDirectory+"/css/style.css";
                    if( copy_file( buildFileStyleCssFilleName, indexStyleCssFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy [style.css] file");
                    }

                    std::string buildFileGPEPowerFilleName = "";
                    std::string indexFileGPEPowerFileName = "";
                    if( inDebugMode )
                    {
                        GPE_Main_Logs->log_build_line("Using DEBUG mode...");
                        buildFileGPEPowerFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/gpepower_strict_v1_2_2_debug.txt";
                        indexFileGPEPowerFileName =projectBuildDirectory+"/js/lib/gpepower_strict_v1_2_2_debug.js";
                    }
                    else
                    {
                        GPE_Main_Logs->log_build_line("Using RELEASE mode...");
                        buildFileGPEPowerFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/gpepower_strict_v1_2_2.txt";
                        indexFileGPEPowerFileName =projectBuildDirectory+"/js/lib/gpepower_strict_v1_2_2.js";
                    }

                    if( buildMetaTemplate ==GPE_BUILD_WIIU)
                    {
                        //Nintendo Web Franmework Export Code
                        //#For those licensed devs see #Wiiu0001
                    }
                    else if( buildMetaTemplate ==GPE_BUILD_WINDOWS || buildMetaTemplate ==GPE_BUILD_MAC || buildMetaTemplate ==GPE_BUILD_LINUX )
                    {
                        std::string buildFileElectronAddonFile = APP_DIRECTORY_NAME+"build_files/js/lib/electron_addon.txt";
                        std::string indexFileElectronAddonFile = projectBuildDirectory+"/js/lib/electron_addon.js";
                        if( copy_file( buildFileElectronAddonFile, indexFileElectronAddonFile)==false )
                        {
                            hadSaveErrors = true;
                            GPE_Main_Logs->log_build_error("Unable to copy Electron Addon");
                        }
                    }

                    if( copy_file( buildFileGPEPowerFilleName, indexFileGPEPowerFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy GPE HTML5 engine");
                    }

                    std::string buildFileGPESuperArrayFileName = APP_DIRECTORY_NAME+"build_files/js/lib/gpe_superarray.txt";
                    std::string indexFileGPESuperArrayFileName = projectBuildDirectory+"/js/lib/gpe_superarray.js";
                    if( copy_file( buildFileGPESuperArrayFileName, indexFileGPESuperArrayFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy array library");

                    }
                    std::string buildFileGPESuperStringFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/gpe_superstring.txt";
                    std::string indexFileGPESuperStringFileName = projectBuildDirectory+"/js/lib/gpe_superstring.js";
                    if( copy_file( buildFileGPESuperStringFilleName, indexFileGPESuperStringFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to superstring library");
                    }

                    /*std::string buildFileScreenfullgFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/screenfull.txt";
                    std::string indexFileScreenfullFileName = projectBuildDirectory+"/js/lib/screenfull.js";
                    if( copy_file( buildFileScreenfullgFilleName, indexFileScreenfullFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to screenfull library");
                    }*/

                    std::string buildFileGPEFrameManagerFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/frame_management.txt";
                    std::string indexFileGPEFrameManagerFileName = projectBuildDirectory+"/js/lib/frame_management.js";
                    if( copy_file( buildFileGPEFrameManagerFilleName, indexFileGPEFrameManagerFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy frame management library");
                    }
                    ///GPE Intro Files

                    std::string buildFileGPELogoStartFilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_logo_start.png";
                    std::string indexFileGPELogoStartFileName = projectBuildDirectory+"/res/gpe_logo_start.png";
                    if( copy_file( buildFileGPELogoStartFilleName, indexFileGPELogoStartFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copyGPE logo.");
                    }
                    std::string buildFileIntroAudioStartAACFilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_intro.aac";
                    std::string indexFileGPEIntroAudioStartAACFileName = projectBuildDirectory+"/res/gpe_intro.aac";
                    if( copy_file( buildFileIntroAudioStartAACFilleName, indexFileGPEIntroAudioStartAACFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy gpe_intro.aac");
                    }
                    std::string buildFileIntroAudioStartMP3FilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_intro.mp3";
                    std::string indexFileIntroAudioStartMP3FileName = projectBuildDirectory+"/res/gpe_intro.mp3";
                    if( copy_file( buildFileIntroAudioStartMP3FilleName, indexFileIntroAudioStartMP3FileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy gpe_intro.mp3");
                    }
                    std::string buildFileIntroAudioStartOOGFilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_intro.ogg";
                    std::string indexFileGPEIntroAudioStartOOGFileName = projectBuildDirectory+"/res/gpe_intro.ogg";
                    if( copy_file( buildFileIntroAudioStartOOGFilleName, indexFileGPEIntroAudioStartOOGFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy gpe_intro.ogg");
                    }
                    std::string buildFileIntroAudioStartWAVFilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_intro.wav";
                    std::string indexFileGPEIntroAudioStartWAVFileName = projectBuildDirectory+"/res/gpe_intro.wav";
                    if( copy_file( buildFileIntroAudioStartWAVFilleName, indexFileGPEIntroAudioStartWAVFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy gpe_intro.wav");
                    }

                    if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                    {
                        std::string indexHTML5FileName = projectBuildDirectory+"/index.html";
                        GPE_Main_Logs->log_build_line("Creating [index.html] file.");
                        std::ofstream indexHTML5FILE ( indexHTML5FileName.c_str() );

                        if (indexHTML5FILE.is_open())
                        {
                            indexHTML5FILE << "<!DOCTYPE HTML> \n";
                            indexHTML5FILE << "<html style='margin: 0px; padding: 0px;'> \n";
                            indexHTML5FILE << "<!--  Game Pencil Engine Project File --> \n";
                            indexHTML5FILE << "<!--  Created automatically via the Game Pencil Engine Editor -->\n";
                            indexHTML5FILE << "<!--  Warning: Manually editing this file may cause unexpected bugs and errors. -->\n";
                            indexHTML5FILE << "<!--  If you have any problems reading this file please report it to help@pawbyte.com -->\n";
                            indexHTML5FILE << "    <head>\n";
                            /*
                            if( buildMetaTemplate ==GPE_BUILD_HTML5 && projectSettingsObject->projectHtmlHeaderCode!=NULL)
                            {
                                if( projectSettingsObject->headerCodeBeforeGPECheckbox!=NULL && projectSettingsObject->headerCodeBeforeGPECheckbox->is_clicked() )
                                {
                                    if( projectSettingsObject->projectHtmlHeaderCode->has_content() )
                                    {
                                        projectSettingsObject->projectHtmlHeaderCode->write_data_into_file(&indexHTML5FILE,2,true);
                                        indexHTML5FILE << "\n";
                                    }
                                }
                            }
                            */

                            indexHTML5FILE << "        <meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>\n";
                            indexHTML5FILE << "        <meta name='viewport' content='width=device-width, user-scalable=no, initial-scale=1, maximum-scale=1.0'> \n";
                            //Modified as of 1.14 [ BEGIN ]
                            indexHTML5FILE << "        <title>";
                            if( inDebugMode)
                            {
                                indexHTML5FILE << "[DEBUG MODE]";
                            }
                            indexHTML5FILE << projectSettingsObject->projectGameShortTitleField->get_string() << "</title> \n";
                            //Added as of 1.14 [ END ]
                            indexHTML5FILE << "        <link rel='stylesheet' type='text/css'  href='css/style.css'> \n";
                            indexHTML5FILE << "        <link rel='stylesheet' type='text/css'  href='css/gpe_custom_style.css'> \n";

                            if( buildMetaTemplate ==GPE_BUILD_WIIU)
                            {
                                //Nintendo Web Franmework Export Code
                                //#For those licensed devs see #Wiiu0002
                            }
                            else if( buildMetaTemplate ==GPE_BUILD_WINDOWS || buildMetaTemplate ==GPE_BUILD_MAC || buildMetaTemplate ==GPE_BUILD_LINUX )
                            {
                                indexHTML5FILE << "        <script src='js/lib/electron_addon.js'></script> \n";
                            }
                            indexHTML5FILE << "        <script src='js/lib/frame_management.js'></script> \n";
                            //indexHTML5FILE << "        <script src='js/lib/screenfull.js'></script> \n";
                            indexHTML5FILE << "        <script src='js/lib/gpe_superarray.js'></script> \n";
                            indexHTML5FILE << "        <script src='js/gpe_settings.js'></script> \n";
                            //Modified as of 1.14 [ BEGIN ]
                            if( inDebugMode)
                            {
                                indexHTML5FILE << "        <script src='js/lib/gpepower_strict_v1_2_2_debug.js'></script>	 \n";
                            }
                            else
                            {
                                indexHTML5FILE << "        <script src='js/lib/gpepower_strict_v1_2_2.js'></script>	 \n";
                            }
                            //Modified as of 1.14 [ END ]
                            if( buildMetaTemplate ==GPE_BUILD_WIIU)
                            {
                                //Nintendo Web Franmework Export Code
                                //#For those licensed devs see #Wiiu0003

                            }
                            //indexHTML5FILE << "        <script src='js/gpe_app.js'></script>	 \n";
                            indexHTML5FILE << "     </head>\n";
                            indexHTML5FILE << "     <body>\n";
                            indexHTML5FILE << "        <canvas id='gpeCanvas' width='auto' height='"<< BUILD_SCREEN_HEIGHT << "' oncontextmenu='return false;'></canvas> \n";
                            if( buildMetaTemplate ==GPE_BUILD_HTML5 && projectSettingsObject->checkBoxShowPublisherInfo!=NULL)
                            {
                                if(projectSettingsObject->checkBoxShowPublisherInfo->is_clicked() )
                                {
                                    if( projectSettingsObject->projectGameTitleField->has_content() )
                                    {
                                        indexHTML5FILE << "     <h2>" << projectSettingsObject->projectGameTitleField->get_string() << "</h2>\n";
                                    }

                                    if( projectSettingsObject->projectGameDescriptionField->has_content() )
                                    {
                                        indexHTML5FILE << "     <h3>" << projectSettingsObject->projectGameDescriptionField->get_string() << "</h3>\n";
                                    }

                                    if( projectSettingsObject->projectGameNotes!=NULL)
                                    {
                                        if( projectSettingsObject->projectGameNotes->has_content() )
                                        {
                                            indexHTML5FILE << "     <p>";
                                            projectSettingsObject->projectGameNotes->write_data_into_file(&indexHTML5FILE,2,true);
                                            indexHTML5FILE << "</p>\n";
                                        }
                                    }

                                    if( projectSettingsObject->projectGamePublisherField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p>Published by " << projectSettingsObject->projectGamePublisherField->get_string() << "</p>\n";
                                    }

                                    if( projectSettingsObject->projectGameDeveloperField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p>Developed by " << projectSettingsObject->projectGameDeveloperField->get_string() << "</p>\n";
                                    }

                                    if( projectSettingsObject->projectGameCopyrightField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p>Copyright " << projectSettingsObject->projectGameCopyrightField->get_string() << " "<<projectSettingsObject->projectGameDateField->get_string() <<"</p>\n";
                                    }

                                    if( projectSettingsObject->projectGameWebsiteField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p> <a href='" << projectSettingsObject->projectGameWebsiteField->get_string() << "'>For more information on game click here</a></p>\n";
                                    }

                                    if( projectSettingsObject->projectGameEmailField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p> Email <a href='mailto:" << projectSettingsObject->projectGameEmailField->get_string() << "?Subject='Hello'>"<< projectSettingsObject->projectGameEmailField->get_string() << " for assistance</a></p>\n";
                                    }
                                }
                            }

                            if( buildMetaTemplate==GPE_BUILD_HTML5 && projectSettingsObject->warnOnCloseCheckbox->is_clicked() )
                            {
                                indexHTML5FILE << "<script type='text/javascript'>\n";
                                indexHTML5FILE << "window.onbeforeunload = function () {return 'Are you sure that you want to leave this page? Game Data may be lost';}\n";
                                indexHTML5FILE << "</script>\n";
                            }
                            indexHTML5FILE << "    </body>\n";
                            indexHTML5FILE << "</html> \n";

                            indexHTML5FILE.close();
                            std::string indexGameSettingsFileName = projectBuildDirectory+"/js/gpe_settings.js";
                            std::ofstream indexJSSettingsFILE ( indexGameSettingsFileName.c_str() );
                            if (indexJSSettingsFILE.is_open())
                            {
                                indexJSSettingsFILE << "/*     \n";
                                indexJSSettingsFILE << "#    --------------------------------------------------  #\n";
                                indexJSSettingsFILE << "#       \n";
                                indexJSSettingsFILE << "#       \n";
                                indexJSSettingsFILE << "#  Game Pencil Engine Game File \n";
                                indexJSSettingsFILE << "#  Created automatically via the Game Pencil Engine Editor \n";
                                indexJSSettingsFILE << "#  Warning: Manually editing this file may cause unexpected bugs and errors. \n";
                                indexJSSettingsFILE << "#  If you have any problems reading this file please report it to help@pawbyte.com . \n";
                                indexJSSettingsFILE << "#     \n";
                                indexJSSettingsFILE << "#      \n";
                                indexJSSettingsFILE << "#    --------------------------------------------------  # \n";
                                indexJSSettingsFILE << "*/     \n\n";
                                indexJSSettingsFILE << "var game = game || {};\n \n";
                                int fpsValue = projectSettingsObject->projectSettingsFPSRate->get_selected_value();
                                if( fpsValue > 0)
                                {
                                    if( buildMetaTemplate == GPE_BUILD_WIIU && fpsValue > 30)
                                    {
                                        /* Wiiu0004
                                        Changing the FPS to 30 is completely optional here, but die to lack of support I decided to limit FPS to 30 manually.
                                        */
                                        indexJSSettingsFILE << "GPE_STANDARD_FPS = 30;\n \n";

                                    }
                                    else
                                    {
                                        indexJSSettingsFILE << "GPE_STANDARD_FPS ="+int_to_string(fpsValue)+";\n \n";
                                    }
                                }
                                else
                                {
                                    indexJSSettingsFILE << "GPE_STANDARD_FPS = 60;\n \n";
                                    indexJSSettingsFILE << "GPE_FPS_UNCAPPED = true;\n \n";
                                }
                                indexJSSettingsFILE << "GPE_STANDARD_INTERVAL =  1000/GPE_STANDARD_FPS ; \n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_APP_LOGO_LOCATION = 'res/gpe_logo_start.png'; \n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_START_SCENE = '"+projectFirstLevelName+"'; \n";


                                indexJSSettingsFILE << "var GPE_SETTINGS_SCREEN_WIDTH = " << BUILD_SCREEN_WIDTH << ";\n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_SCREEN_HEIGHT = " << BUILD_SCREEN_HEIGHT << ";\n";

                                indexJSSettingsFILE << "var GPE_SETTINGS_PROG_NAME = '" << projectSettingsObject->projectGameTitleField->get_string() << "';\n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_VERSION_NUMBER = "<< projectSettingsObject->projectGameVersionField->get_string()<< ";\n";
                                indexJSSettingsFILE << "var GPE_MAX_OBJECT_TYPES = "<< (int)buildGameObjectOptions.size() << ";\n";
                                if( buildMetaTemplate ==GPE_BUILD_WIIU)
                                {
                                    indexJSSettingsFILE << "var GPE_SETTINGS_SYSTEM_OS = 'WiiU';\n";
                                }
                                else
                                {
                                    indexJSSettingsFILE << "var GPE_SETTINGS_SYSTEM_OS = 'Browser';\n";
                                }
                                indexJSSettingsFILE << "var GPE_GAME_PUBLISHER = '" << projectSettingsObject->projectGamePublisherField->get_string()<< "';\n";
                                indexJSSettingsFILE << "var GPE_GAME_DEVELOPER = '" << projectSettingsObject->projectGameDeveloperField->get_string() << "';\n";
                                indexJSSettingsFILE << "var GPE_GAME_OBJECTS_COUNT = "+int_to_string( (int)buildGameObjectOptions.size() )+";\n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_IS_DEBUGGING = false;\n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_SHOW_FPS = true;\n";

                                indexJSSettingsFILE << "var GPE_GAME_SCALING_SETTING = " << projectSettingsObject->projectScaleSettings->get_selected_id()<< ";\n";


                                indexJSSettingsFILE << "var GPE_PreloadImageResource = [];\n";
                                int preloadCount = 0;

                                int preloadAudioCount[SUPPORTED_AUDIO_FORMAT_COUNT];
                                preloadAudioCount[aacFileName] = 0;
                                preloadAudioCount[mp3FileName] = 0;
                                preloadAudioCount[oggFileName] = 0;
                                preloadAudioCount[wavFileName] = 0;

                                int iPreloadRes = 0;
                                int jPreloadSubFileType = 0;
                                for( iPreloadRes = 0; iPreloadRes < (int)preloadImgTextures.size(); iPreloadRes++ )
                                {
                                    indexJSSettingsFILE << "GPE_PreloadImageResource[" << int_to_string(preloadCount) << "] = 'resources/textures/" << preloadImgTextures.at( iPreloadRes ) << "';\n";
                                    preloadCount+=1;
                                }
                                for( iPreloadRes = 0; iPreloadRes < (int)preloadImgTilesheets.size(); iPreloadRes++ )
                                {
                                    indexJSSettingsFILE << "GPE_PreloadImageResource[" << int_to_string(preloadCount) << "] = 'resources/tilesheets/" << preloadImgTilesheets.at( iPreloadRes ) << "';\n";
                                    preloadCount+=1;
                                }
                                for( iPreloadRes = 0; iPreloadRes < (int)preloadImgSprites.size(); iPreloadRes++ )
                                {
                                    indexJSSettingsFILE << "GPE_PreloadImageResource[" << int_to_string(preloadCount) << "] = 'resources/animations/" << preloadImgSprites.at( iPreloadRes ) << "';\n";
                                    preloadCount+=1;
                                }
                                indexJSSettingsFILE << "var GPE_PreloadAudioResource = [];\n";
                                for( jPreloadSubFileType = 0; jPreloadSubFileType < SUPPORTED_AUDIO_FORMAT_COUNT; jPreloadSubFileType++)
                                {
                                    indexJSSettingsFILE << "var GPE_Preload" << SUPPORTED_AUDIO_EXT[jPreloadSubFileType] << "Resource = [];\n";
                                    indexJSSettingsFILE << "GPE_PreloadAudioResource["<< jPreloadSubFileType << "] = GPE_Preload" << SUPPORTED_AUDIO_EXT[jPreloadSubFileType] << "Resource;\n";
                                }

                                for( iPreloadRes = 0; iPreloadRes < (int)buildAudioOptions.size(); iPreloadRes++ )
                                {
                                    tempContainer = buildAudioOptions.at(iPreloadRes);
                                    if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                                    {
                                        tempAudRes = (audioResource * ) tempContainer->get_held_resource();
                                        if( tempAudRes!=NULL)
                                        {
                                            for( jPreloadSubFileType = 0; jPreloadSubFileType < SUPPORTED_AUDIO_FORMAT_COUNT; jPreloadSubFileType++)
                                            {
                                                if( tempAudRes->audioFileName[jPreloadSubFileType].size() > 3)
                                                {
                                                    indexJSSettingsFILE << "GPE_Preload" << SUPPORTED_AUDIO_EXT[jPreloadSubFileType] << "Resource[" << int_to_string(preloadAudioCount[jPreloadSubFileType]) << "] = 'resources/audio/" << getShortFileName(tempAudRes->audioFileName[jPreloadSubFileType],true ) << "';\n";
                                                    preloadAudioCount[jPreloadSubFileType]++;
                                                }
                                            }
                                        }
                                    }
                                }

                                if( RESC_PROJECT_SETTINGS!=NULL )
                                {
                                    indexJSSettingsFILE << "var Game_LayerMatrix = [];\n";
                                    indexJSSettingsFILE << "var Game_SelfCollideLayerList = [];\n";
                                    projectPropertiesResource * tProjectProps = (projectPropertiesResource *)RESC_PROJECT_SETTINGS->get_held_resource();
                                    if( tProjectProps!=NULL)
                                    {
                                        int jLayerCol = 0;
                                        for( int iLayerRow = 0; iLayerRow < 32; iLayerRow++)
                                        {
                                            indexJSSettingsFILE << "Game_LayerMatrix.push( [] );\n";
                                            for(  jLayerCol = 0; jLayerCol < 32-iLayerRow; jLayerCol++)
                                            {
                                                if( iLayerRow==jLayerCol)
                                                {
                                                    if( tProjectProps->projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->is_clicked() )
                                                    {
                                                        indexJSSettingsFILE << "Game_SelfCollideLayerList.push(1);\n";
                                                    }
                                                    else
                                                    {
                                                        indexJSSettingsFILE << "Game_SelfCollideLayerList.push(0);\n";
                                                    }
                                                }
                                                else if( tProjectProps->projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->is_clicked() )
                                                {
                                                    indexJSSettingsFILE << "Game_LayerMatrix["+int_to_string(iLayerRow)+"].push("+int_to_string( jLayerCol )+");\n";
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        indexJSSettingsFILE << "//RESC_PROJECT_SETTINGS local not reachable...\n";
                                    }
                                }
                                else
                                {
                                    indexJSSettingsFILE << "//RESC_PROJECT_SETTINGS not reachable...\n";
                                }

                                indexJSSettingsFILE.close();
                            }
                            else
                            {
                                hadSaveErrors = true;
                                GPE_Main_Logs->log_build_error("Unable to create [gpe_settings.js]...[Build canceled]");
                            }

                            if( buildMetaTemplate==GPE_BUILD_WINDOWS || buildMetaTemplate==GPE_BUILD_MAC || buildMetaTemplate==GPE_BUILD_LINUX )
                            {
                                std::string indexJsonPackageFileName = projectBuildDirectory+"/package.json";
                                std::ofstream indexJsonPackageFile ( indexJsonPackageFileName.c_str() );
                                GPE_Main_Logs->log_build_line("Creating [package.json] file...");
                                if (indexJsonPackageFile.is_open())
                                {
                                    indexJsonPackageFile << "{\n";
                                    indexJsonPackageFile << "\"name\"             :   \"" << projectSettingsObject->projectGameShortTitleField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"productName\"      :   \"" << projectSettingsObject->projectGameTitleField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"description\"      :   \"" << projectSettingsObject->projectGameDescriptionField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"version\"          :   \"" << projectSettingsObject->projectGameVersionField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"main\"             :   \"main.js\"\n";
                                    indexJsonPackageFile << "}\n";
                                    indexJsonPackageFile.close();
                                }
                                else
                                {
                                    hadSaveErrors = true;
                                    GPE_Main_Logs->log_build_error("Unable to create [package.json]...[Build canceled]");
                                }
                            }

                            std::string indexJSCustomGameFileName = projectBuildDirectory+"/js/gpe_app.js";
                            std::ofstream indexJSCustomGameFILE ( indexJSCustomGameFileName.c_str() );
                            GPE_Main_Logs->log_build_line("Creating [gpe_app.js] file...");
                            if (indexJSCustomGameFILE.is_open())
                            {
                                indexJSCustomGameFILE << "/*     \n";
                                indexJSCustomGameFILE << "#    --------------------------------------------------  #\n";
                                indexJSCustomGameFILE << "#       \n";
                                indexJSCustomGameFILE << "#       \n";
                                indexJSCustomGameFILE << "#  Game Pencil Engine Game File \n";
                                indexJSCustomGameFILE << "#  Created automatically via the Game Pencil Engine Editor Version "<< int_to_string(GPE_VERSION_DOUBLE_NUMBER) << "\n";
                                indexJSCustomGameFILE << "#  Warning: Manually editing this file may cause unexpected bugs and errors. \n";
                                indexJSCustomGameFILE << "#  If you have any problems reading this file please report it to help@pawbyte.com . \n";
                                indexJSCustomGameFILE << "#     \n";
                                indexJSCustomGameFILE << "#      \n";
                                indexJSCustomGameFILE << "#    --------------------------------------------------  # \n";
                                indexJSCustomGameFILE << "*/     \n\n";

                                //Modified as of 1.14 [ BEGIN ]
                                indexJSCustomGameFILE << "'use strict';\n";
                                //Modified as of 1.14 [ END ]
                                currentObjParents.clear();
                                nextObjParents.clear();

                                for( iRes = 0; iRes < (int)buildTextureOptions.size(); iRes++)
                                {
                                    tempContainer = buildTextureOptions[iRes];
                                    if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                                    {
                                        tempTexRes = (textureResource * ) tempContainer->get_held_resource();
                                        tempTexRes->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                    }
                                }

                                for( iRes = 0; iRes < (int)buildTilesheetOptions.size(); iRes++)
                                {
                                    tempContainer = buildTilesheetOptions[iRes];
                                    if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                                    {
                                        tempTstRes = (tilesheetResource * ) tempContainer->get_held_resource();
                                        tempTstRes->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                    }
                                }


                                for( iRes = 0; iRes < (int)buildSpriteOptions.size(); iRes++)
                                {
                                    tempContainer = buildSpriteOptions[iRes];
                                    if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                                    {
                                        tempSprRes = (animationResource * ) tempContainer->get_held_resource();
                                        tempSprRes->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                    }
                                }


                                for( iRes = 0; iRes < (int)buildAudioOptions.size(); iRes++)
                                {
                                    tempContainer = buildAudioOptions[iRes];
                                    if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                                    {
                                        tempAudRes = (audioResource * ) tempContainer->get_held_resource();
                                        tempAudRes->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                        tempAudRes->copy_audio_source(projectBuildDirectory+"/resources/audio");
                                    }
                                }

                                for( iRes = 0; iRes < (int)buildVideoOptions.size(); iRes++)
                                {
                                    tempContainer = buildVideoOptions[iRes];
                                    if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                                    {
                                        tempVidRes = (videoResource * ) tempContainer->get_held_resource();
                                        tempVidRes->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                        tempVidRes->copy_video_source(projectBuildDirectory+"/resources/videos");
                                    }
                                }

                                for( iRes = 0; iRes < (int)buildPathsOptions.size(); iRes++)
                                {
                                    tempContainer = buildPathsOptions[iRes];
                                    if( tempContainer!=NULL)
                                    {
                                        indexJSCustomGameFILE << "var " << tempContainer->get_name() << " =  " << tempContainer->get_held_resource()->exportBuildGlobalId << "; \n";
                                        if( tempContainer->get_held_resource()!=NULL )
                                        {
                                            tempPathRes = (gamePathResource * ) tempContainer->get_held_resource();
                                            tempPathRes->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                        }
                                    }
                                }

                                for( iRes = 0; iRes < (int)buildFontOptions.size(); iRes++)
                                {
                                    tempContainer = buildFontOptions[iRes];
                                    if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                                    {
                                        tempFntRes = (fontResource * ) tempContainer->get_held_resource();
                                        tempFntRes->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                    }
                                }

                                for( iRes = 0; iRes < (int)buildGameObjectOptions.size(); iRes++)
                                {
                                    tempContainer = buildGameObjectOptions[iRes];
                                    if( tempContainer!=NULL)
                                    {
                                        indexJSCustomGameFILE << "var " << tempContainer->get_name() << " =  " << tempContainer->get_held_resource()->exportBuildGlobalId << "; \n";
                                    }
                                }

                                for( iRes = 0; iRes < (int)buildGameSceneOptions.size(); iRes++)
                                {
                                    tempContainer = buildGameSceneOptions[iRes];
                                    if( tempContainer!=NULL)
                                    {
                                        foundHtml5BuildId = tempContainer->get_held_resource()->exportBuildGlobalId;
                                        indexJSCustomGameFILE << "var " << tempContainer->get_name() << " =  " << foundHtml5BuildId << "; \n";
                                        indexJSCustomGameFILE << "var  _scn_" << tempContainer->get_name() << " =  GPE.add_gamescene("+int_to_string(foundHtml5BuildId)+" , '"+tempContainer->get_name()+"'); \n";
                                    }
                                }

                                /*if( projectSettingsObject->projectGameMacros!=NULL)
                                {
                                    indexJSCustomGameFILE << "//Start of Project Macros \n";
                                    projectSettingsObject->projectGameMacros->write_data_into_file(&indexJSCustomGameFILE,0,true);
                                    indexJSCustomGameFILE << "//End of Project Macros \n";
                                }*/

                                int iErrorLine = 0;

                                //Added as of 1.14 [ BEGIN ]
                                for( iRes = 0; iRes < (int)buildClassesOptions.size() && ( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false ); iRes++)
                                {
                                    tempContainer = buildClassesOptions[iRes];
                                    if( tempContainer!=NULL &&tempContainer->get_held_resource()!=NULL)
                                    {
                                        tempClassRes = (classResource * )tempContainer->get_held_resource();
                                        if( ( tempClassRes!=NULL) && ( tempClassRes->classSourceCode!=NULL )  )
                                        {
                                            indexJSCustomGameFILE << "{\n\n";
                                            if( !tempClassRes->classSourceCode->compile_into_code( &indexJSCustomGameFILE, 1,true,true) )
                                            {
                                                for( iErrorLine=0; iErrorLine < (int)tempClassRes->classSourceCode->foundSyntaxErrors.size(); iErrorLine++)
                                                {
                                                    GPE_Main_Logs->log_build_error( tempContainer->get_name()+": "+tempClassRes->classSourceCode->foundSyntaxErrors[iErrorLine] );
                                                }
                                                hadSaveErrors = true;
                                                if( GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked() )
                                                {
                                                    break;
                                                }
                                            }
                                            indexJSCustomGameFILE << "}\n\n";
                                        }
                                    }
                                }
                                indexJSCustomGameFILE << "\n";
                                //Added as of 1.14 [ END ]

                                iErrorLine = 0;
                                for( iRes = 0; iRes < (int)buildFunctionsOptions.size() && ( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false ); iRes++)
                                {
                                    tempContainer = buildFunctionsOptions[iRes];
                                    if( tempContainer!=NULL)
                                    {
                                        hasTextureToUse = false;
                                        if( tempContainer->get_held_resource()!=NULL)
                                        {
                                            tempFuncRes = (functionResource * )tempContainer->get_held_resource();
                                            if( ( tempFuncRes!=NULL) && ( tempFuncRes->functionCode!=NULL && tempFuncRes->parametersField!=NULL)  )
                                            {
                                                //GPE_Report("Processing "+tempFuncRes->get_name()+"['s function"+tempFuncRes->parametersField->get_string() +"].");
                                                if( GPE_MAIN_HIGHLIGHTER->process_parameters_string( tempFuncRes->parametersField->get_string() ) )
                                                {
                                                    indexJSCustomGameFILE << "function " << tempContainer->get_name() << "( " << GPE_MAIN_HIGHLIGHTER->newParametersString << " ) \n";
                                                    indexJSCustomGameFILE << "{\n";
                                                    hasTextureToUse = true;
                                                }
                                                else
                                                {
                                                    indexJSCustomGameFILE << "function " << tempContainer->get_name() << "(invaliGPEParameters) \n";
                                                    indexJSCustomGameFILE << "{\n";

                                                    for( iErrorLine = 0; iErrorLine < (int)GPE_MAIN_HIGHLIGHTER->functionParameterErrors.size(); iErrorLine++)
                                                    {
                                                        GPE_Main_Logs->log_build_error("Invalid parameters given for "+tempContainer->get_name()+": "+GPE_MAIN_HIGHLIGHTER->functionParameterErrors[iErrorLine] );
                                                        GPE_Report("Invalid parameters given for "+tempContainer->get_name()+": "+GPE_MAIN_HIGHLIGHTER->functionParameterErrors[iErrorLine] );
                                                    }
                                                    hadSaveErrors = true;
                                                    if( GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked() )
                                                    {
                                                        break;
                                                    }
                                                }

                                                for( paramLine = 0; paramLine < (int)GPE_MAIN_HIGHLIGHTER->defaultParameterLines.size(); paramLine++)
                                                {
                                                    indexJSCustomGameFILE << "     "+GPE_MAIN_HIGHLIGHTER->defaultParameterLines[paramLine] << "\n";
                                                }
                                                if( !tempFuncRes->functionCode->compile_into_code( &indexJSCustomGameFILE, 1,true,true) )
                                                {
                                                    for( iErrorLine=0; iErrorLine < (int)tempFuncRes->functionCode->foundSyntaxErrors.size(); iErrorLine++)
                                                    {
                                                        GPE_Main_Logs->log_build_error( tempContainer->get_name()+": "+tempFuncRes->functionCode->foundSyntaxErrors[iErrorLine] );
                                                    }
                                                    hadSaveErrors = true;
                                                    if( GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked() )
                                                    {
                                                        break;
                                                    }
                                                }
                                                indexJSCustomGameFILE << "}\n\n";
                                            }
                                        }
                                    }
                                }

                                indexJSCustomGameFILE << "\n";

                                if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                                {
                                    std::string tempObjName = "";
                                    std::string tempParentName = "";

                                    //The Glorious object creation code
                                    std::vector< objectResParentChainLink *> ALL_OBJECTS_CHAINS;
                                    objectResParentChainLink * tObjectChainLink = NULL;
                                    for( iRes = 0; iRes < (int)buildGameObjectOptions.size(); iRes++)
                                    {
                                        ALL_OBJECTS_CHAINS.push_back( new objectResParentChainLink(iRes) );
                                    }
                                    GPE_TextAreaInputBasic * tempObjCreationFunc = new GPE_TextAreaInputBasic();
                                    tempObjCreationFunc->clear_all_lines();
                                    tempObjCreationFunc->add_line("GPE.add_new_object = function(newObjType, newX, newY, objectLayerId )");
                                    tempObjCreationFunc->add_line("{");
                                    tempObjCreationFunc->add_line("if( typeof objectLayerId!=\"number\"){objectLayerId = 1;}");
                                    tempObjCreationFunc->add_line("     var newObjOut = IS_NULL;");
                                    tempObjCreationFunc->add_line("     switch( newObjType )");
                                    tempObjCreationFunc->add_line("     {");

                                    //first compiles all objects whose parent is the standard default game object
                                    GPE_GeneralResourceContainer * currObjParent = NULL;
                                    for( iRes = 0; iRes < (int)buildGameObjectOptions.size(); iRes++)
                                    {
                                        tempContainer = buildGameObjectOptions[iRes];
                                        if( tempContainer!=NULL)
                                        {
                                            hasTextureToUse = false;
                                            if( tempContainer->get_held_resource()!=NULL)
                                            {
                                                tempObjRes = (gameObjectResource * )tempContainer->get_held_resource();
                                                if( tempObjRes!=NULL && tempObjRes->get_parent_resource()==NULL)
                                                {
                                                    GPE_Report("Building: ["+tempObjRes->get_name()+"]...");
                                                    if( tempObjRes->build_intohtml5_file(&indexJSCustomGameFILE,0)==false )
                                                    {
                                                        hadSaveErrors = true;
                                                        //GPE_Main_Logs->log_build_error("Error building "+tempObjRes->get_name()+".");
                                                        GPE_Report("Error building "+tempObjRes->get_name()+" (Default Object Parent).");
                                                        if( GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked() )
                                                        {
                                                            break;
                                                        }

                                                    }
                                                    else
                                                    {
                                                        GPE_Report("Built "+tempObjRes->get_name()+" (Default Object Parent).");
                                                    }
                                                    nextObjParents.push_back( tempContainer->get_global_id() );
                                                    tempObjCreationFunc->add_line("         case "+int_to_string(iRes)+":");
                                                    tempObjCreationFunc->add_line("             newObjOut = new GPE._obj_"+tempObjRes->get_name()+"(newX, newY, objectLayerId);");
                                                    tempObjCreationFunc->add_line("         break;");
                                                    amountOfObjectsCompiled++;
                                                }
                                            }
                                        }
                                    }

                                    swap_obj_lists();
                                    //then compiles all objects whose parent is user defined
                                    while( currentObjParents.size() > 0 && ( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false ) )
                                    {
                                        for( iRes = 0; iRes < (int)buildGameObjectOptions.size(); iRes++)
                                        {
                                            tempContainer = buildGameObjectOptions[iRes];
                                            if( tempContainer!=NULL)
                                            {
                                                hasTextureToUse = false;
                                                if( tempContainer->get_held_resource()!=NULL)
                                                {
                                                    tempObjRes = (gameObjectResource * )tempContainer->get_held_resource();
                                                    if( tempObjRes!=NULL)
                                                    {
                                                        currObjParent = tempObjRes->parentObjectField->get_selected_container();
                                                        if( currObjParent !=NULL && check_obj_inlist(currObjParent->get_global_id() ) )
                                                        {
                                                            GPE_Report("Building: ["+tempObjRes->get_name()+" - child]...");
                                                            if( tempObjRes->build_intohtml5_file(&indexJSCustomGameFILE,0)==false )
                                                            {
                                                                hadSaveErrors = true;
                                                                //GPE_Main_Logs->log_build_error("Error building "+tempObjRes->get_name()+".");
                                                                GPE_Report("Error building "+tempObjRes->get_name()+".");
                                                                if(  GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked() )
                                                                {
                                                                    break;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                GPE_Report("Built: ["+tempObjRes->get_name()+" - child]...");
                                                            }
                                                            tempObjCreationFunc->add_line("         case "+int_to_string(iRes)+":");
                                                            tempObjCreationFunc->add_line("             newObjOut = new GPE._obj_"+tempObjRes->get_name()+"( newX, newY,objectLayerId);");
                                                            tempObjCreationFunc->add_line("         break;");
                                                            nextObjParents.push_back( tempContainer->get_global_id() );
                                                            amountOfObjectsCompiled++;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        swap_obj_lists();
                                    }

                                    //reverse inherits parents for creation of object families
                                    begin_obj_reverse_inheritence();
                                    GPE_Report("Creating inheritance trees...");
                                    while( currentObjParents.size() > 0)
                                    {
                                        for( iRes = 0; iRes < (int)buildGameObjectOptions.size(); iRes++)
                                        {
                                            tempContainer = buildGameObjectOptions[iRes];
                                            if( tempContainer!=NULL &&  tempContainer->get_held_resource()!=NULL )
                                            {
                                                tempObjRes = (gameObjectResource * )tempContainer->get_held_resource();
                                                if( tempObjRes!=NULL && check_obj_inlist(tempContainer->get_global_id() ) )
                                                {
                                                    currObjParent = tempObjRes->parentObjectField->get_selected_container();
                                                    if( currObjParent !=NULL)
                                                    {
                                                        tempParentHTML5Id = currObjParent->exportBuildGlobalId;
                                                        if( tempParentHTML5Id >=0 && tempParentHTML5Id < (int)ALL_OBJECTS_CHAINS.size() )
                                                        {
                                                            nextObjParents.push_back( currObjParent->get_global_id() );

                                                            if( tempContainer->exportBuildGlobalId >=0 && tempContainer->exportBuildGlobalId < (int)ALL_OBJECTS_CHAINS.size() )
                                                            {
                                                                tObjectChainLink = ALL_OBJECTS_CHAINS[tempContainer->exportBuildGlobalId];
                                                                if( tObjectChainLink->is_inchain(tempParentHTML5Id)==false)
                                                                {
                                                                    tObjectChainLink = ALL_OBJECTS_CHAINS[tempParentHTML5Id];
                                                                    tObjectChainLink->grab_chain(ALL_OBJECTS_CHAINS[tempContainer->exportBuildGlobalId]);
                                                                }
                                                                else
                                                                {
                                                                    //illogcal inheritence circle is occuring, stop it like a Jedi
                                                                    tempObjRes = (gameObjectResource * )currObjParent->get_held_resource();
                                                                    tempObjRes->parentObjectField->set_selection(-1);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        swap_obj_lists();
                                    }

                                    //Cleans up all of that messy backward tracing of parental families
                                    indexJSCustomGameFILE << "\n";
                                    indexJSCustomGameFILE << "GPE.activate_object_families= function()\n";
                                    indexJSCustomGameFILE << "{\n";
                                    indexJSCustomGameFILE << "  GPR.GAME_OBJECTS_FAMILIES = [];\n";
                                    int subChainPos = 0;
                                    for( iRes = 0; iRes < (int)ALL_OBJECTS_CHAINS.size(); iRes++)
                                    {
                                        tObjectChainLink = ALL_OBJECTS_CHAINS[iRes];
                                        indexJSCustomGameFILE << "  GPR.GAME_OBJECTS_FAMILIES["+int_to_string(iRes)+"] = [];\n";
                                        if( tObjectChainLink!=NULL)
                                        {
                                            if( (int)tObjectChainLink->heldChains.size()== 0)
                                            {
                                                indexJSCustomGameFILE << "  GPR.GAME_OBJECTS_FAMILIES["+int_to_string(iRes)+"].push("+int_to_string( iRes )+");\n";
                                            }
                                            else
                                            {
                                                for( subChainPos = 0; subChainPos < (int)tObjectChainLink->heldChains.size(); subChainPos++)
                                                {
                                                    //tempSubObjChainLink = ;
                                                    indexJSCustomGameFILE << "  GPR.GAME_OBJECTS_FAMILIES["+int_to_string(iRes)+"].push("+int_to_string( tObjectChainLink->heldChains[subChainPos] )+");\n";
                                                }
                                            }
                                            indexJSCustomGameFILE <<  "\n";
                                        }
                                    }
                                    indexJSCustomGameFILE << "}\n";
                                    indexJSCustomGameFILE << "\n\n";

                                    GPE_Report("Creating Object Chains...");
                                    //Cleans up all of that messy backward tracing of parental families
                                    for( iRes = (int)ALL_OBJECTS_CHAINS.size()-1; iRes >=0; iRes--)
                                    {
                                        tObjectChainLink = ALL_OBJECTS_CHAINS[iRes];
                                        if( tObjectChainLink!=NULL)
                                        {
                                            delete tObjectChainLink;
                                            tObjectChainLink = NULL;
                                        }
                                    }
                                    ALL_OBJECTS_CHAINS.clear();

                                    tempObjCreationFunc->add_line("         default:");
                                    tempObjCreationFunc->add_line("         break;");
                                    tempObjCreationFunc->add_line("     }");
                                    tempObjCreationFunc->add_line("     if(newObjOut!=IS_NULL)");
                                    tempObjCreationFunc->add_line("     {");
                                    tempObjCreationFunc->add_line("         newObjOut.xPos = newX;");
                                    tempObjCreationFunc->add_line("         newObjOut.yPos = newY;");
                                    tempObjCreationFunc->add_line("         newObjOut.sceneLayerPos = objectLayerId;");
                                    tempObjCreationFunc->add_line("         GPR.add_object(newObjOut, false, objectLayerId);");
                                    tempObjCreationFunc->add_line("         return newObjOut;");
                                    tempObjCreationFunc->add_line("     }");
                                    tempObjCreationFunc->add_line("     return IS_NULL;");
                                    tempObjCreationFunc->add_line("};");

                                    tempObjCreationFunc->write_data_into_file(&indexJSCustomGameFILE,0,true,true);
                                    indexJSCustomGameFILE << "GPE.rsm.audioIsReadyForLoading = true; \n";

                                    indexJSCustomGameFILE << "var _scn_temp_layer = IS_NULL; \n\n";
                                    GPE_Report("Creating  scenes...");
                                    for( iRes = 0; iRes < (int)buildGameSceneOptions.size(); iRes++)
                                    {
                                        tempContainer = buildGameSceneOptions[iRes];
                                        if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                                        {
                                            tempScnRes = (gameSceneResource * ) tempContainer->get_held_resource();
                                            tempScnRes->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                        }
                                    }
                                    GPE_Report("Scenes created");
                                    if( projectSettingsObject->projectGameMacros!=NULL)
                                    {
                                        indexJSCustomGameFILE << "//Start of Project Macros \n";
                                        projectSettingsObject->projectGameMacros->write_short_data_into_file(&indexJSCustomGameFILE,0,true);
                                        indexJSCustomGameFILE << "//End of Project Macros \n";
                                    }
                                }

                                //Closes the custom gpe_app.js file
                                indexJSCustomGameFILE.close();
                                GPE_Report("Building CSS file..");
                                if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                                {
                                    std::string indexCustomCSSGameFileName = projectBuildDirectory+"/css/gpe_custom_style.css";
                                    std::ofstream indexCustomCSSGameFile ( indexCustomCSSGameFileName.c_str() );
                                    GPE_Main_Logs->log_build_line("Creating [gpe_custom_styles.css] file...");
                                    if (indexCustomCSSGameFile.is_open())
                                    {
                                        for( iRes = 0; iRes < (int)buildFontOptions.size(); iRes++)
                                        {
                                            tempContainer = buildFontOptions[iRes];
                                            if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                                            {
                                                tempFntRes = (fontResource * ) tempContainer->get_held_resource();
                                                tempFntRes->build_css3_file(&indexCustomCSSGameFile,0);
                                            }
                                        }
                                        indexCustomCSSGameFile << "\n";
                                        indexCustomCSSGameFile <<"body\n{\n   background-color: #" << projectSettingsObject->projectWebsiteBackgroundColor->get_hex_string() << ";\n   color: #" << projectSettingsObject->projectTextParagraphColor->get_hex_string()<< ";\n width: 100%;\n height: 100%;\n}\n";
                                        indexCustomCSSGameFile <<"canvas\n{\nborder-color: #" << projectSettingsObject->projectBorderColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, h12\n{\n  color: #" << projectSettingsObject->projectTextHeaderColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"p\n{\n  color: #" << projectSettingsObject->projectTextParagraphColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a,a:link\n{\n     color: #" << projectSettingsObject->projectTextLinkColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a:active\n{\n     color: #" << projectSettingsObject->projectTextLinkActiveColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a:hover\n{\n     color: #" << projectSettingsObject->projectTextLinkHoverColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a:visited\n{\n     color: #" << projectSettingsObject->projectTextLinkVisitedColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"\n\n";

                                        indexCustomCSSGameFile.close();
                                    }
                                    else
                                    {
                                        hadSaveErrors = true;
                                        GPE_Main_Logs->log_build_error("Project Build Error: Unable to create [gpe_custom_style.css]..");
                                    }
                                }
                                //Final check for build errors
                                if( !hadSaveErrors)
                                {
                                    GPE_Main_Logs->log_build_line("Game built with no detected errors.");
                                    if( runGameOnCompile && buildMetaTemplate==GPE_BUILD_HTML5)
                                    {
                                        GPE_Main_Logs->log_build_line("Opening... "+indexHTML5FileName+" .");
                                        GPE_OpenURL(indexHTML5FileName);
                                    }
                                }
                                else
                                {
                                    remove(indexJSCustomGameFileName.c_str());
                                }
                            }
                            else
                            {
                                hadSaveErrors = true;
                                GPE_Main_Logs->log_build_error("Unable to create [gpe_app.js]..");
                            }
                        }
                        else
                        {
                            hadSaveErrors = true;
                            display_user_alert("Project Build Error!","Unable to create game [index.html] file...");
                            GPE_Main_Logs->log_build_error("Unable to create game [index.html] file...");
                        }
                        if(RESC_PROJECT_FOLDER!=NULL)
                        {
                            //hadSaveErrors = RESC_PROJECT_FOLDER->write_data_into_projectfile(&indexHTML5FILE);
                        }
                    }
                    else
                    {
                        hadSaveErrors = true;
                    }
                }
                else
                {
                    hadSaveErrors = true;
                }
            }
            else
            {
                hadSaveErrors = true;
                GPE_Main_Logs->log_build_error("At least one game object must exist in project...[Build canceled]");
            }
        }
        else
        {
            hadSaveErrors = true;
            GPE_Main_Logs->log_build_error("At least one game scene must exist in project...[Build cancelede]");
        }
    }
    if( hadSaveErrors)
    {
        GPE_Main_Logs->log_build_error("Unable to build game!");
        display_user_alert("Project Build Error!","Unable to build game! Please see error log...");
    }
    return hadSaveErrors;
}

bool GPE_ProjectFolder::export_project_wiiu( bool inDebugMode)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_messages( "Exporting ["+projectName+"][wiiU]", "Please wait...", "DO NOT CLOSE" );
    }
    std::string projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/wiiu";
    projectBuildDirectory = GPE_MAIN_GUI->setup_build_folder(projectBuildDirectory,GPE_BUILD_WIIU,64,inDebugMode);
    return export_project_html5( projectBuildDirectory,GPE_BUILD_WIIU,false,inDebugMode);
}

bool GPE_ProjectFolder::export_project_windows(std::string projectBuildDirectory, int buildBits, bool runGameOnCompile, bool inDebugMode, int nativeBuildType )
{
    bool buildResult = false;
    if( GPE_Main_Logs!=NULL)
    {
        GPE_Main_Logs->clear_build_log();
        GPE_Main_Logs->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[GPE_BUILD_WINDOWS]+" Export] (Compiler: Pawbitious Compiler)---------------");

        if( nativeBuildType==Native_Electron)
        {
            GPE_Main_Logs->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[GPE_BUILD_WINDOWS]+" Export] (Compiler: Pawbitious Compiler)---------------");
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Exporting ["+projectName+"][Windows]", "Please wait...", "DO NOT CLOSE");
            }
            if( (int)projectBuildDirectory.size() < 3)
            {
                projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/windows";
            }
            std::string pBuildDirectory = GPE_MAIN_GUI->setup_build_folder(projectBuildDirectory,GPE_BUILD_WINDOWS,buildBits,inDebugMode,nativeBuildType);
            buildResult =  export_project_html5( pBuildDirectory,GPE_BUILD_WINDOWS,runGameOnCompile, inDebugMode );

            if( runGameOnCompile && GPE_FOUND_OS==GPE_IDE_WINDOWS)
            {
                run_project(projectBuildDirectory,GPE_IDE_WINDOWS,buildBits,inDebugMode);
                /*
                std::string appAddonFileName = "";
                if( buildBits==64)
                {
                    appAddonFileName+="_64";
                }
                if( inDebugMode)
                {
                    appAddonFileName+="_debug";
                }
                if( file_exists(pBuildDirectory+"/../../game"+appAddonFileName+".exe") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../game"+appAddonFileName+".exe");
                }
                else if( file_exists(pBuildDirectory+"/../../electron"+appAddonFileName+".exe") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../electron"+appAddonFileName+".exe");
                }
                */
            }
        }
        else if (nativeBuildType==Native_GamePencil)
        {
            GPE_Main_Logs->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[GPE_BUILD_WINDOWS]+" Export] (Compiler: Environment Compiler)---------------");
            GPE_Main_Logs->log_build_line("---Alpha Feature---");
            GPE_Main_Logs->log_build_line("Initializing CPP compiler...");


            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Generating ["+projectName+"][Environment]", "Please wait...", "DO NOT CLOSE");
            }
        }
        else
        {
            GPE_Main_Logs->open_build_log();
            GPE_Main_Logs->log_build_error("Project not built. Native Build Type NOT Selected");
        }
    }
    else
    {
        GPE_Report("Unable to build game. Internal Log error");
    }
    return buildResult;
}

bool GPE_ProjectFolder::export_project_osx(std::string projectBuildDirectory, int buildBits, bool runGameOnCompile, bool inDebugMode, int nativeBuildType)
{
    bool buildResult = false;
    if( GPE_Main_Logs!=NULL)
    {
        GPE_Main_Logs->clear_build_log();
        GPE_Main_Logs->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[GPE_BUILD_MAC]+" Export] (Compiler: Pawbitious Compiler)---------------");
        if( nativeBuildType!=Native_None)
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Exporting ["+projectName+"][OSX]", "Please wait...", "DO NOT CLOSE");
            }
            if( (int)projectBuildDirectory.size() < 3)
            {
                projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/osx";
            }
            std::string pBuildDirectory  = GPE_MAIN_GUI->setup_build_folder(projectBuildDirectory,GPE_BUILD_MAC,buildBits,inDebugMode, nativeBuildType);
            buildResult =  export_project_html5( pBuildDirectory,GPE_BUILD_MAC, runGameOnCompile, inDebugMode );
            if( runGameOnCompile && GPE_FOUND_OS==GPE_IDE_MAC)
            {
                run_project(projectBuildDirectory,GPE_IDE_MAC,buildBits,inDebugMode);

                /*
                std::string appAddonFileName = "";
                if( buildBits==64)
                {
                    appAddonFileName+="_64";
                }
                if( inDebugMode)
                {
                    appAddonFileName+="_debug";
                }
                if( file_exists(pBuildDirectory+"/../../game"+appAddonFileName+".app") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../game"+appAddonFileName+".app");
                }
                else if( file_exists(pBuildDirectory+"/../../Game"+appAddonFileName+".app") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../Game"+appAddonFileName+".app");
                }
                else if(file_exists(pBuildDirectory+"/../../Electron"+appAddonFileName+".app") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../Electron"+appAddonFileName+".app");
                }
                else if(file_exists(pBuildDirectory+"/../../electron"+appAddonFileName+".app") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../electron"+appAddonFileName+".app");
                }
                */
            }
        }
        else
        {
            GPE_Main_Logs->open_build_log();
            GPE_Main_Logs->log_build_error("Project not built. Native Build Type NOT Selected");
        }
    }
    else
    {
        GPE_Report("Unable to build game. Internal Log error");
    }
    return buildResult;
}

bool GPE_ProjectFolder::export_project_linux(std::string projectBuildDirectory, int buildBits, bool runGameOnCompile, bool inDebugMode, int nativeBuildType)
{
    bool buildResult = false;
    if( GPE_Main_Logs!=NULL)
    {
        GPE_Main_Logs->clear_build_log();
        GPE_Main_Logs->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[GPE_BUILD_LINUX]+" Export] (Compiler: Pawbitious Compiler)---------------");
        if( nativeBuildType!=Native_None)
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Exporting ["+projectName+"][Linux]", "Please wait...", "DO NOT CLOSE");
            }
            if( (int)projectBuildDirectory.size() < 3)
            {
                projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/linux";
            }
            std::string pBuildDirectory  = GPE_MAIN_GUI->setup_build_folder(projectBuildDirectory,GPE_BUILD_LINUX,buildBits,inDebugMode, nativeBuildType);
            bool buildResult =  export_project_html5( pBuildDirectory,GPE_BUILD_LINUX, runGameOnCompile, inDebugMode);

            if( runGameOnCompile && GPE_FOUND_OS==GPE_IDE_LINUX )
            {
                run_project(projectBuildDirectory,GPE_IDE_LINUX,buildBits,inDebugMode);
                /*
                std::string appAddonFileName = "";
                if( buildBits==64)
                {
                    appAddonFileName+="_64";
                }
                if( inDebugMode)
                {
                    appAddonFileName+="_debug";
                }
                if( file_exists(pBuildDirectory+"/../../game"+appAddonFileName) )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../game"+appAddonFileName);
                }
                else if( file_exists(pBuildDirectory+"/../../electron"+appAddonFileName) )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../electron"+appAddonFileName);
                }
                else if( file_exists(pBuildDirectory+"/../../Electron"+appAddonFileName) )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../Electron"+appAddonFileName);
                }
                */
            }
        }
        else
        {
            GPE_Main_Logs->open_build_log();
            GPE_Main_Logs->log_build_error("Project not built. Native Export Mode not selected");
        }
    }
    else
    {
        GPE_Report("Unable to build game. Internal Log error");
    }
    return buildResult;
}

std::string GPE_ProjectFolder::get_project_language()
{
    return projectLanguage;
}

int GPE_ProjectFolder::get_project_language_id()
{
    return myProjectLanguage;
}

std::string GPE_ProjectFolder::get_project_name()
{
    return projectName;
}

std::string GPE_ProjectFolder::get_project_directory()
{
    return projectDirectory;
}

std::string GPE_ProjectFolder::get_project_file_name()
{
    return projectFileName;
}

int GPE_ProjectFolder::get_resource_count()
{
    if( RESC_PROJECT_FOLDER!=NULL)
    {
        return RESC_PROJECT_FOLDER->get_resource_count();
    }
    return 0;
}

bool GPE_ProjectFolder::load_project_file(std::string projectFileIn )
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_messages( "Loading Project [" + projectName + "]", "Opening Project File","DO NOT CLOSE..." );
    }

    if( (int)projectFileIn.size() > 0)
    {
        std::ifstream newprofileFile( projectFileIn.c_str() );

        //Checks and/or makes all of the sub-directories for the folder.
        if( GPE_MAIN_GUI!=NULL)
        {
            GPE_MAIN_GUI->setup_project_directory(projectDirectory);
        }

        //If the level file could be loaded
        if( !newprofileFile.fail() )
        {
            //makes sure the file is open
            if (newprofileFile.is_open())
            {
                projectFileName = projectFileIn;
                set_current_gpe_project_from_name( CURRENT_PROJECT_NAME = projectFileIn );
                if( GPE_LOADER != NULL )
                {
                    GPE_LOADER->update_submessages( "Reading Project File","DO NOT CLOSE..." );
                }
                GPE_GeneralResourceContainer * currentResFolder = NULL;
                GPE_GeneralResourceContainer * tempResFolder = NULL;
                GPE_GeneralResourceContainer * containerFolderToEdit = NULL;
                GPE_GeneralResourceContainer * newContainer = NULL;
                animationResource * tempSprRes = NULL;
                textureResource * tempTexRes = NULL;
                tilesheetResource * tempTstRes = NULL;
                audioResource * tempAudRes = NULL;
                videoResource * tempVidRes = NULL;
                gamePathResource * tempPathRes = NULL;
                lightResource * tempLightRes = NULL;
                particleResource * tempEmitterRes = NULL;
                functionResource * tempFuncRes = NULL;
                classResource *    tempClassRes = NULL;
                gameObjectResource * tempObjRes = NULL;
                gameSceneResource * tempScnRes = NULL;
                //achievementResource * tempAchRes = NULL;
                fontResource * tempFntRes = NULL;
                std::string firstChar="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string newResName= "";
                std::string tempNewResName = "";
                int equalPos=-1;
                int endBracketPos = -1;
                int foundResGlobalId = -1;
                int tempStrToInt = -1;
                int iItr = 0;
                int iLang = 0;

                std::vector <animationResource *> projectGameSprites;
                std::vector <textureResource *> projectGameTextures;
                std::vector <tilesheetResource *> projectGameTilesheets;
                std::vector <audioResource * > projectGameAudio;
                std::vector <videoResource *> projectGameVideos;
                std::vector <lightResource *> projectLights2D;
                std::vector <particleResource *> projectEmitters;
                std::vector <gamePathResource *> projectGamePaths;
                std::vector <functionResource *> projectGameFunctions;
                std::vector <classResource *> projectGameClasses;
                std::vector <gameObjectResource *> projectGameObjects;
                std::vector <gameSceneResource *> projectScenes;
                std::vector <fontResource *> projectGameFonts;

                //makes sure the file is in good condition and the version is still unloaded
                GPE_Report("Loading in project file and processing version number...");
                while ( newprofileFile.good() )
                {
                    getline (newprofileFile,currLine); //gets the next line of the file
                    currLine = trim_left_inplace(currLine);
                    currLine = trim_right_inplace(currLine);

                    if(!currLine.empty() )
                    {
                        // skips comment lines
                        if( currLine[0]!= '#' && currLine[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLine.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLine.substr(0,equalPos);
                                valString = currLine.substr(equalPos+1,currLine.length());
                                //finds the version of the game level file
                                if (keyString== "Version")
                                {
                                    projectFilePreviousVersion = projectFileVersion = string_to_double(valString,1);

                                    if( !compare_doubles(projectFileVersion, GPE_VERSION_DOUBLE_NUMBER) && projectFileVersion < GPE_VERSION_DOUBLE_NUMBER )
                                    {
                                        if( GPE_Display_Basic_Prompt("Project Version Warning!","The following project version varies from the current version of this editor. Are you sure you will like to import this. Please note saving/loading may experience difficulties we recommend you back up these project files/folders before importing...")!=DISPLAY_QUERY_YES )
                                        {
                                            newprofileFile.close();
                                            GPE_Main_Logs->log_general_line("Project import canceled ( Older version )");
                                            return false;
                                        }
                                        else
                                        {
                                            if( GPE_MAIN_GUI->editorReleaseType!=RELEASE_TYPE_PRODUCTION )
                                            {
                                                if( GPE_Display_Basic_Prompt("WARNING!","You are using a NON-PRODUCTION version of the editor. Are you sure you will like to continue? Potential incompatibility issues may happen in later versions.")!=DISPLAY_QUERY_YES )
                                                {
                                                    newprofileFile.close();
                                                    GPE_Main_Logs->log_general_line("Project import canceled ( Alpha/Beta version )");
                                                    return false;
                                                }
                                            }
                                            GPE_Main_Logs->log_general_line("Attempting to open older project...");
                                        }
                                    }
                                    else if( GPE_MAIN_GUI->editorReleaseType!=RELEASE_TYPE_PRODUCTION )
                                    {
                                        if( GPE_Display_Basic_Prompt("WARNING!","You are using a NON-PRODUCTION version of the editor. Are you sure you will like to continue? Potential incompatibility issues may happen in later versions.")!=DISPLAY_QUERY_YES )
                                        {
                                            newprofileFile.close();
                                            GPE_Main_Logs->log_general_line("Project import canceled ( Alpha/Beta version )");
                                            return false;
                                        }
                                    }
                                }
                                else if( keyString=="Count" || keyString=="ResourcesCount")
                                {
                                    tempStrToInt = string_to_int(valString);
                                    if( tempStrToInt> 1000)
                                    {
                                        GLOBAL_REZ_ID_COUNT = tempStrToInt;
                                    }
                                }
                                else if(keyString=="ProjectName" || keyString=="Name")
                                {
                                    projectName = valString;
                                    RESC_PROJECT_FOLDER->set_name(projectName);
                                }
                                else if(keyString=="ProjectLanguage" || keyString=="ProgrammingLanguage" || keyString=="CodingLanguage")
                                {
                                    myProjectLanguage = GPE_MAIN_HIGHLIGHTER->get_language_id_from_name( valString);
                                    if( myProjectLanguage < 0 )
                                    {
                                        set_project_language_id( PROGRAM_LANGUAGE_CPP );
                                    }
                                }
                                else if( keyString == "ProjectIcon")
                                {
                                    projectIconName = valString;
                                }
                                else if(keyString=="[SuperFolder")
                                {
                                    endBracketPos=currLine.find_first_of("]");
                                    if(endBracketPos!=(int)std::string::npos)
                                    {
                                        valString = currLine.substr(equalPos+1,endBracketPos-equalPos-1);
                                        if( (int)valString.size()>0)
                                        {
                                            tempResFolder = RESC_PROJECT_FOLDER->find_resource_from_name(valString,false);
                                            if( tempResFolder!=NULL)
                                            {
                                                containerFolderToEdit = currentResFolder = tempResFolder;
                                                tempResFolder = NULL;
                                            }
                                        }
                                    }
                                }
                                else if(keyString=="[Folder")
                                {
                                    valString = currLine.substr(equalPos+1,endBracketPos-equalPos-1);
                                    if( (int)valString.size()>0 && containerFolderToEdit!=NULL)
                                    {
                                        endBracketPos=currLine.find_first_of("]");
                                        if(endBracketPos!=(int)std::string::npos)
                                        {
                                            valString = currLine.substr(equalPos+1,endBracketPos-equalPos-1);
                                            if( (int)valString.size()>0)
                                            {
                                                newResName = split_first_string(valString,',');
                                                foundResGlobalId = string_to_int(valString,-1);
                                                tempResFolder = containerFolderToEdit->find_resource_from_name(newResName,false);
                                                if( tempResFolder!=NULL)
                                                {
                                                    containerFolderToEdit = containerFolderToEdit->find_resource_from_name(newResName,false);
                                                }
                                                else
                                                {
                                                    containerFolderToEdit = create_blank_folder(containerFolderToEdit,newResName,foundResGlobalId);
                                                }
                                            }
                                        }
                                    }
                                }
                                else if(keyString=="Sprite" || keyString=="sprite" || keyString=="Animation" || keyString=="animation" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_sprite(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempSprRes = (animationResource * )newContainer->get_held_resource();
                                        if( tempSprRes!=NULL)
                                        {
                                            tempSprRes->resourcePostProcessed = false;
                                            projectGameSprites.push_back(tempSprRes);
                                        }
                                    }
                                }
                                else if( keyString=="Texture" || keyString=="texture" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_texture(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempTexRes = (textureResource * )newContainer->get_held_resource();
                                        if( tempTexRes!=NULL)
                                        {
                                            tempTexRes->resourcePostProcessed = false;
                                            projectGameTextures.push_back(tempTexRes);
                                        }
                                    }
                                }
                                else if(keyString=="Tilesheet" || keyString=="tilesheet")
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_tilesheet(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempTstRes = (tilesheetResource * )newContainer->get_held_resource();
                                        if( tempTstRes!=NULL)
                                        {
                                            tempTstRes->resourcePostProcessed = false;
                                            projectGameTilesheets.push_back(tempTstRes);
                                        }
                                    }
                                }
                                else if( keyString=="Audio" || keyString=="audio" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_audio(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempAudRes = (audioResource * )newContainer->get_held_resource();
                                        if( tempAudRes!=NULL)
                                        {
                                            tempAudRes->resourcePostProcessed = false;
                                            projectGameAudio.push_back( tempAudRes);
                                        }
                                    }
                                }
                                else if( keyString=="Video" || keyString=="video" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_video(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempVidRes = (videoResource * )newContainer->get_held_resource();
                                        if( tempVidRes!=NULL)
                                        {
                                            tempVidRes->resourcePostProcessed = false;
                                            projectGameVideos.push_back( tempVidRes);
                                        }
                                    }
                                }
                                else if( keyString=="Light2D" || keyString=="light2d" )
                                {

                                    tempNewResName = split_first_string(valString,',');
                                    GPE_Report("Adding 2d light["+tempNewResName+"]...");
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_light(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempLightRes = (lightResource * )newContainer->get_held_resource();
                                        if( tempLightRes!=NULL)
                                        {
                                            tempLightRes->resourcePostProcessed = false;
                                            projectLights2D.push_back( tempLightRes);
                                        }
                                    }
                                }
                                else if( keyString=="ParticleEmitter" || keyString=="emitter" )
                                {

                                    tempNewResName = split_first_string(valString,',');
                                    GPE_Report("Adding particle emitter ["+tempNewResName+"]...");
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_emitter(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempEmitterRes = (particleResource * )newContainer->get_held_resource();
                                        if( tempEmitterRes!=NULL)
                                        {
                                            tempEmitterRes->resourcePostProcessed = false;
                                            projectEmitters.push_back( tempEmitterRes);
                                        }
                                    }
                                }
                                else if( keyString=="Path" || keyString=="path" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_path(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempPathRes = (gamePathResource * )newContainer->get_held_resource();
                                        if( tempPathRes!=NULL)
                                        {
                                            tempPathRes->resourcePostProcessed = false;
                                            projectGamePaths.push_back( tempPathRes);
                                        }
                                    }
                                }
                                else if( keyString=="Function" || keyString=="function" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_function(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempFuncRes = (functionResource * )newContainer->get_held_resource();
                                        if( tempFuncRes!=NULL)
                                        {
                                            tempFuncRes->resourcePostProcessed = false;
                                            projectGameFunctions.push_back( tempFuncRes);
                                        }
                                    }
                                }
                                else if( keyString=="Class" || keyString=="class" )
                                {

                                    tempNewResName = split_first_string(valString,',');
                                    GPE_Report("Adding class ["+tempNewResName+"]...");
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_class(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempClassRes = (classResource * )newContainer->get_held_resource();
                                        if( tempClassRes!=NULL)
                                        {
                                            tempClassRes->resourcePostProcessed = false;
                                            projectGameClasses.push_back( tempClassRes);
                                        }
                                    }
                                }
                                else if( keyString=="Object" || keyString=="object"|| keyString=="GameObject"|| keyString=="Game-Object" || keyString=="Actor"|| keyString=="actor" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_gameobject(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempObjRes = (gameObjectResource * )newContainer->get_held_resource();
                                        if( tempObjRes!=NULL)
                                        {
                                            tempObjRes->resourcePostProcessed = false;
                                            projectGameObjects.push_back(tempObjRes);
                                        }
                                    }
                                }
                                else if( keyString=="Scene" || keyString=="scene"|| keyString=="GameScene"|| keyString=="Game-Scene"  )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_scene(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempScnRes = (gameSceneResource * )newContainer->get_held_resource();
                                        if( tempScnRes!=NULL)
                                        {
                                            tempScnRes->scnPostProcessed = false;
                                            projectScenes.push_back(tempScnRes);
                                        }
                                    }
                                }
                                else if(keyString=="Font" || keyString=="font")
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_font(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempFntRes = (fontResource * )newContainer->get_held_resource();
                                        if( tempFntRes!=NULL)
                                        {
                                            tempFntRes->resourcePostProcessed = false;
                                            projectGameFonts.push_back(tempFntRes);
                                        }
                                    }
                                }
                            }
                            else if(currLine=="[/Folder]")
                            {
                                if( containerFolderToEdit!=NULL)
                                {
                                    if( containerFolderToEdit->parentResource!=NULL)
                                    {
                                        containerFolderToEdit = containerFolderToEdit->parentResource;
                                    }
                                    else
                                    {
                                        containerFolderToEdit = NULL;
                                        tempResFolder = NULL;
                                    }
                                }
                                else
                                {
                                    tempResFolder = NULL;
                                }
                            }
                            else if(currLine=="[/SuperFolder]")
                            {
                                containerFolderToEdit = NULL;
                                tempResFolder = NULL;
                            }
                        }
                    }
                }
                newprofileFile.close();

                std::string projectLayerInfoFileName =projectDirectory+"/gpe_project/project_layer_info.gpf";
                std::ifstream projectLayerInfoFile (projectLayerInfoFileName.c_str() );
                int iLayerN = 0;
                GPE_Report("Version number["+double_to_string( projectFilePreviousVersion )+"] found and now processing project data...");
                while ( projectLayerInfoFile.good() )
                {
                    getline (projectLayerInfoFile,currLine); //gets the next line of the file
                    currLine = trim_left_inplace(currLine);
                    currLine = trim_right_inplace(currLine);

                    if(!currLine.empty() )
                    {
                        // skips comment lines
                        if( currLine[0]!= '#' && currLine[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLine.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLine.substr(0,equalPos);
                                valString = currLine.substr(equalPos+1,currLine.length());
                                for( iLayerN = 0; iLayerN < 32; iLayerN++)
                                {
                                    if( keyString == "ProjectLayerName["+int_to_string(iLayerN)+"]" )
                                    {
                                        projectLayerNames[iLayerN]  = valString;
                                    }
                                }
                            }
                        }
                    }
                }

                for( iItr = 0; iItr < (int)projectGameTextures.size(); iItr++)
                {
                    tempTexRes = projectGameTextures[iItr];
                    if( tempTexRes!=NULL)
                    {
                        tempTexRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameTilesheets.size(); iItr++)
                {
                    tempTstRes = projectGameTilesheets[iItr];
                    if( tempTstRes!=NULL)
                    {
                        tempTstRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameSprites.size(); iItr++)
                {
                    tempSprRes = projectGameSprites[iItr];
                    if( tempSprRes!=NULL)
                    {
                        tempSprRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameAudio.size(); iItr++)
                {
                    tempAudRes = projectGameAudio[iItr];
                    if( tempAudRes!=NULL)
                    {
                        tempAudRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameVideos.size(); iItr++)
                {
                    tempVidRes = projectGameVideos[iItr];
                    if( tempVidRes!=NULL)
                    {
                        tempVidRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGamePaths.size(); iItr++)
                {
                    tempPathRes = projectGamePaths[iItr];
                    if( tempPathRes!=NULL)
                    {
                        tempPathRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectLights2D.size(); iItr++)
                {
                    tempLightRes = projectLights2D[iItr];
                    if( tempLightRes!=NULL)
                    {
                        tempLightRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectEmitters.size(); iItr++)
                {
                    tempEmitterRes = projectEmitters[iItr];
                    if( tempEmitterRes!=NULL)
                    {
                        tempEmitterRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameFonts.size(); iItr++)
                {
                    tempFntRes = projectGameFonts[iItr];
                    if( tempFntRes!=NULL)
                    {
                        tempFntRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameFunctions.size(); iItr++)
                {
                    tempFuncRes = projectGameFunctions[iItr];
                    if( tempFuncRes!=NULL)
                    {
                        tempFuncRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameClasses.size(); iItr++)
                {
                    tempClassRes = projectGameClasses[iItr];
                    if( tempClassRes!=NULL)
                    {
                        tempClassRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameObjects.size(); iItr++)
                {
                    tempObjRes = projectGameObjects[iItr];
                    if( tempObjRes!=NULL)
                    {
                        tempObjRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectScenes.size(); iItr++)
                {
                    tempScnRes = projectScenes[iItr];
                    if( tempScnRes!=NULL)
                    {
                        tempScnRes->preprocess_self();
                    }
                }
                RESC_PROJECT_SETTINGS->set_project_parent_name(projectFileIn);
                RESC_PROJECT_SETTINGS->preprocess_container();
                GPE_Report("Project successfully preprocessed.");
                if( (int)projectScenes.size() > 0 )
                {
                    GPE_Main_TabManager->add_new_tab( projectScenes[0] );
                    input->reset_all_input();
                }
                time( &lastTimeBackedUp);
                return true;
            }
            else
            {
                display_user_alert("Project Open Error","Unable to open file");
            }
        }
        else
        {
            display_user_alert("Project Open Error","Unable to process file");
        }
    }
    else
    {
        display_user_alert("Project Open Error","Invalid File given");
    }
    return false;
}

bool GPE_ProjectFolder::check_obj_inlist(int objTypeIdIn)
{
    for( int i = 0; i < (int)currentObjParents.size(); i++)
    {
        if( currentObjParents[i]==objTypeIdIn)
        {
            return true;
        }
    }
    return false;
}

void GPE_ProjectFolder::swap_obj_lists()
{
    currentObjParents.clear();
    if( (int)nextObjParents.size() > 0)
    {
        finalObjParents.clear();
    }
    for( int i = 0; i < (int)nextObjParents.size(); i++)
    {
        currentObjParents.push_back( nextObjParents[i] );
        finalObjParents.push_back( nextObjParents[i] );
    }
    nextObjParents.clear();
}

void GPE_ProjectFolder::begin_obj_reverse_inheritence()
{
    currentObjParents.clear();
    for( int i = 0; i < (int)finalObjParents.size(); i++)
    {
        currentObjParents.push_back( finalObjParents[i] );
    }
    nextObjParents.clear();
}

void GPE_ProjectFolder::integrate_syntax()
{
    clear_project_functions();
    clear_project_keywords();
    set_current_gpe_project_from_name( projectFileName );
    if( RESC_PROJECT_FOLDER!=NULL)
    {
        RESC_PROJECT_FOLDER->integrate_into_syntax();
    }
    refresh_gui_syntax();
}

void GPE_ProjectFolder::refresh_gui_syntax()
{
    if( GPE_MAIN_HIGHLIGHTER!=NULL )
    {
        GPE_Compiler_Term * tempTerm = NULL;
        GPE_MAIN_HIGHLIGHTER->activeProjectFunctions.clear();
        int i = 0;
        for( i = 0; i < (int)projectFunctions.size(); i++ )
        {
            tempTerm = projectFunctions[i];
            if( tempTerm!=NULL)
            {
                GPE_MAIN_HIGHLIGHTER->activeProjectFunctions.push_back( tempTerm);
            }
        }
        GPE_MAIN_HIGHLIGHTER->activeProjectKeywords.clear();
        for( i = 0; i < (int)projectKeywords.size(); i++ )
        {
            tempTerm = projectKeywords[i];
            if( tempTerm!=NULL)
            {
                GPE_MAIN_HIGHLIGHTER->activeProjectKeywords.push_back( tempTerm );
            }
        }
    }
}

int GPE_ProjectFolder::increment_resouce_count()
{
    GLOBAL_REZ_ID_COUNT+=1;
    return GLOBAL_REZ_ID_COUNT;
}

bool GPE_ProjectFolder::run_project(std::string projectBuildDirectory, int buildMetaTemplate,int buildBits, bool inDebugMode )
{
    if( buildMetaTemplate == GPE_BUILD_HTML5)
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/html5";
        }
        if( file_exists(projectBuildDirectory+"/index.html")==true )
        {
            GPE_OpenURL(projectBuildDirectory+"/index.html");
            return true;
        }
        else
        {
            display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate==GPE_BUILD_WINDOWS && GPE_FOUND_OS==GPE_IDE_WINDOWS)
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/windows";
        }
        /*if( buildBits==64)
        {
            projectBuildDirectory+="_64";
        }
        else
        {
            projectBuildDirectory+="_32";
        }
        if( inDebugMode)
        {
            //projectBuildDirectory+="_debug";
        }*/
        if( file_exists(projectBuildDirectory+"/game.exe")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/game.exe");
            return true;
        }
        else if( file_exists(projectBuildDirectory+"/electron.exe")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/electron.exe");
            return true;
        }
        else
        {
            display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate==GPE_BUILD_MAC && GPE_FOUND_OS==GPE_IDE_MAC)
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/osx";
        }
        /*
        if( buildBits==64)
        {
            projectBuildDirectory+="_64";
        }
        else
        {
            projectBuildDirectory+="_32";
        }
        if( inDebugMode)
        {
            //projectBuildDirectory+="_debug";
        }
        */
        if( file_exists(projectBuildDirectory+"/game.app")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/game.app");
            return true;
        }
        else if( file_exists(projectBuildDirectory+"/electron.app")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/electron.app");
            return true;
        }
        else
        {
            display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate==GPE_BUILD_LINUX && GPE_FOUND_OS==GPE_IDE_LINUX)
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/linux";
        }
        if( buildBits==64)
        {
            projectBuildDirectory+="_64";
        }
        else
        {
            projectBuildDirectory+="_32";
        }
        if( inDebugMode)
        {
            //projectBuildDirectory+="_debug";
        }
        if( file_exists(projectBuildDirectory+"/game")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/game");
            return true;
        }
        else if( file_exists(projectBuildDirectory+"/electron")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/electron");
            return true;
        }
        else
        {
            display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate==GPE_BUILD_WIIU)
    {
        /*
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = fileToDir(projectFileName)+"/gpe_project/builds/wiiu";
        }
        if( file_exists(projectBuildDirectory+"/index.html")==true)
        {
            GPE_OpenURL(projectBuildDirectory+"/index.html");
        }
        else
        {
            display_user_alert("[Run Project Error]","Please first build this project");
        }
        */
        display_user_alert("[Run Project Error]","Please use the WiiU Developer Version!");
    }
    return false;
}

bool GPE_ProjectFolder::save_project()
{
    return save_project_as(projectFileName);
}

bool GPE_ProjectFolder::save_project_as(std::string projectFileNewName)
{
    bool hadSaveErrors = false;
    if( (int)projectFileNewName.size()>3)
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_messages( "Saving Project [" + projectName + "]", "Please Wait...","DO NOT CLOSE..." );
        }

        GPE_Report( "Saving Project [" + projectFileName + "]" );
        bool isSameFileName = false;
        bool saveAuthorized = false;
        if( !compare_doubles(projectFilePreviousVersion, GPE_VERSION_DOUBLE_NUMBER ) && projectFilePreviousVersion < GPE_VERSION_DOUBLE_NUMBER )
        {
            std::string versionComparePhrase = "The following project version varies from the current version of this editor. Are you sure you will like to save this. Please note saving/loading may experience difficulties we recommend you back up these project files/folders ELSEWHERE before saving...";
            if( GPE_Display_Basic_Prompt("Project Version Warning!",versionComparePhrase)==DISPLAY_QUERY_YES )
            {
                saveAuthorized = true;
            }
            else
            {
                hadSaveErrors = true;
                saveAuthorized= false;
            }
        }
        else
        {
            saveAuthorized = true;
        }

        if( saveAuthorized)
        {
            if( projectFileNewName.compare(projectFileName)==0)
            {
                isSameFileName = true;
            }
            else
            {
                isSameFileName = false;
            }
            if( !isSameFileName)
            {
                projectFileName = projectFileNewName;
                projectDirectory = get_path_from_file(projectFileName);
                std::string newProjectLocalFileName = get_local_from_global_file(projectFileName);
                if( (int)newProjectLocalFileName.size()>0 )
                {
                    projectDirectory = projectDirectory+"/"+projectFileName;
                    display_user_alert("Error:",projectDirectory.c_str() );

                    GPE_MAIN_GUI->setup_project_directory(projectDirectory);
                }
                else
                {
                    display_user_alert("Error!","Unable to save project("+projectFileName+")");
                }
                set_current_gpe_project_from_name( projectFileName );
                if(RESC_PROJECT_FOLDER!=NULL)
                {
                    RESC_PROJECT_FOLDER->set_project_parent_name(projectFileName);
                }
            }
            else
            {
                GPE_MAIN_GUI->setup_project_directory(projectDirectory);
            }

            std::ofstream myfile (projectFileName.c_str() );
            if (myfile.is_open())
            {
                myfile << "#    --------------------------------------------------  # \n";
                myfile << "#     \n";
                myfile << "#     \n";
                myfile << "#    Game Pencil Engine Project File \n";
                myfile << "#    Created automatically via the Game Pencil Engine Editor \n";
                myfile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
                myfile << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
                myfile << "#     \n";
                myfile << "#     \n";
                myfile << "#    --------------------------------------------------  # \n";
                myfile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
                myfile << "Name=" << projectName << "\n";

                myfile << "Count=" << GLOBAL_REZ_ID_COUNT << "\n";
                if( (int)projectLanguage.size()  > 0  )
                {
                    myfile << "ProjectLanguage=" << projectLanguage << "\n";
                }
                else
                {
                    myfile << "ProjectLanguage=CPP\n";
                }
                myfile << "ProjectIcon=" << projectIconName << "\n";
                if(RESC_PROJECT_FOLDER!=NULL)
                {
                    hadSaveErrors = RESC_PROJECT_FOLDER->write_data_into_projectfile(&myfile);
                }
                time( &lastTimeBackedUp);
                myfile.close();
            }
            else
            {
                GPE_Report("Unable to open to save ["+projectFileNewName+"].");
                hadSaveErrors = true;
            }
            std::string projectLayerInfoFileName =projectDirectory+"/gpe_project/project_layer_info.gpf";
            GPE_Report("Saving project layer info to "+projectLayerInfoFileName);
            std::ofstream projectLayerInfoFile (projectLayerInfoFileName.c_str() );
            if (projectLayerInfoFile.is_open() )
            {
                //write_header_on_file(&projectLayerInfoFile);
                for( int iLayerN = 0; iLayerN < 32; iLayerN++)
                {
                    projectLayerInfoFile << "ProjectLayerName["+int_to_string(iLayerN)+"]=" << projectLayerNames[iLayerN] << "\n";
                }
                projectLayerInfoFile.close();
                projectFilePreviousVersion = projectFileVersion = GPE_VERSION_DOUBLE_NUMBER;
            }
            else
            {
                GPE_Report("Unable to open to save ["+projectLayerInfoFileName+"].");
                hadSaveErrors = true;
            }
        }
        else
        {
            GPE_Main_Logs->log_general_comment("Save averted for Project["+projectFileName+"](not authorized by user choice).");
        }
    }
    else
    {
        GPE_Report("File named "+projectFileNewName+" is too short and does not fit the proper format for project save feature.");
    }
    integrate_syntax();
    return hadSaveErrors;
}

bool GPE_ProjectFolder::save_project_settings()
{
    if( RESC_PROJECT_SETTINGS!=NULL)
    {
        std::string projectSettingsFileName = projectStartDirectoryName+"gpe_project/project_settings.ini";
        std::ofstream myfile (projectSettingsFileName.c_str() );
        if (myfile.is_open() )
        {
            RESC_PROJECT_SETTINGS->write_data_into_projectfile(&myfile,0);
            myfile.close();
            return true;
        }
    }
    return false;
}

void GPE_ProjectFolder::set_project_language( std::string newProjectLanguage)
{
    if( (int)newProjectLanguage.size() > 0 )
    {
        GPE_Gui_Engine_Language * tLanguage  = NULL;
        bool languageChanged = false;
        int languageMax = GPE_MAIN_HIGHLIGHTER->get_language_count();
        for( int cLanguage = 0; cLanguage < languageMax; cLanguage++ )
        {
            tLanguage = GPE_MAIN_HIGHLIGHTER->get_language_object(cLanguage);
            //checks if the found language isnt NULL and is a coding language
            if( tLanguage!=NULL && tLanguage->isCodingLanguage)
            {
                //if its regular name or short name matches. We end loop and change our project language.
                if( newProjectLanguage == tLanguage->languageName || newProjectLanguage == tLanguage->languageShortName )
                {
                    GPE_Report("Changing project language to "+ tLanguage->languageName + "...");
                    myProjectLanguage = cLanguage;
                    projectLanguage = tLanguage->languageShortName;
                    languageChanged = true;
                    break;
                }
            }
        }
        if( !languageChanged )
        {
            GPE_Report("Unable to find language ["+newProjectLanguage+"]...");
        }
        return;
    }
    GPE_Report("You can not change project language to an empty name");
}

void GPE_ProjectFolder::set_project_language_id( int projectLanguageId )
{
    if( projectLanguageId >= 0 )
    {
        GPE_Gui_Engine_Language * tLanguage = GPE_MAIN_HIGHLIGHTER->get_language_object_from_id( projectLanguageId );
        //checks if the found lanuage isnt NULL and is a coding language
        if( tLanguage!=NULL && tLanguage->isCodingLanguage)
        {
            myProjectLanguage = projectLanguageId;
            projectLanguage = tLanguage->languageShortName;
        }
    }
}

bool set_current_gpe_project( GPE_ProjectFolder * newMainProject)
{
    bool projectChanged = false;
    if( newMainProject!=NULL)
    {
        CURRENT_PROJECT = newMainProject;
        if( CURRENT_PROJECT_NAME != newMainProject->get_project_file_name() )
        {
            CURRENT_PROJECT_NAME = newMainProject->get_project_file_name();
            //Integrate syntax for syntax highlighter and such...
            CURRENT_PROJECT->refresh_gui_syntax();
        }
    }
    else if( CURRENT_PROJECT!=NULL)
    {
        projectChanged = true;
    }
    return projectChanged;
}

bool set_current_gpe_project_from_name( std::string newMainProjectName )
{
    bool projectChanged = false;
    if( (int)newMainProjectName.size() > 0  )
    {
        if( CURRENT_PROJECT_NAME != newMainProjectName)
        {
            projectChanged = true;
        }
        CURRENT_PROJECT_NAME = newMainProjectName;
        CURRENT_PROJECT = GPE_MAIN_GUI->find_project_from_filename(CURRENT_PROJECT_NAME);
        if( CURRENT_PROJECT!=NULL && projectChanged)
        {
            //Integrate for syntax highlighter and such...
            GPE_MAIN_GUI->process_window_title();
            CURRENT_PROJECT->refresh_gui_syntax();
        }
    }
    else if( CURRENT_PROJECT!=NULL)
    {
        projectChanged = true;
    }
    return projectChanged;
}

GPE_Gui_Engine::GPE_Gui_Engine()
{
    includeNintendoWiiUExport = false;
    includeNintendoSwitchExport = false;
    includePlaystation4Export = false;
    includeXboxOneExport = false;
    editorReleaseType = RELEASE_TYPE_ALPHA;
    if( MAIN_RENDERER!=NULL)
    {
        MAIN_RENDERER->set_window_title("Game Pencil Engine");
        GPE_Report("Main renderer is not NULL...");
    }
    else
    {
        GPE_Report("Main renderer is NULL...");
    }
    GPE_DEFAULT_TEMPLATE = add_theme("default");
    GPE_DEFAULT_TEMPLATE->load_theme(APP_DIRECTORY_NAME+"themes/default.gpf");
    GPE_MAIN_THEME = GPE_DEFAULT_TEMPLATE;

    load_tips();
    fileOpenProjectDir = "";
    fileOpenProjectIconDir = "";
    fileSaveProjectDir = "";


    mainToolBar = NULL;
    mainResourceTree = NULL;

    //findTextStringBox->set_label("Find:");
    //goToLineStringBox->set_label("Go To Line:");
    //replaceTextStringBox->set_label("Replace:");

    searchResultProjectName = "";
    searchResultResourceName = "";
    searchResultResourceId = 0;

    showTipsAtStartUp = true;
    showStartPageAtStartUp = true;

    //loads the recent file list
    std::string recentFileListFileName = get_user_settings_folder()+"recent_projects.txt";
    std::ifstream recentFileListFile( recentFileListFileName.c_str() );
    if( !recentFileListFile.fail() )
    {
        std::string currLine = "";
        //makes sure the file is open
        if (recentFileListFile.is_open())
        {
            while ( recentFileListFile.good() )
            {
                getline (recentFileListFile,currLine); //gets the next line of the file
                currLine = trim_left_inplace(currLine);
                currLine = trim_right_inplace(currLine);
                add_to_recent_project_list(currLine,false);
            }
            recentFileListFile.close();
        }
    }
    eraserAnimation = guiRCM->sprite_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/eraser.png",1, true );
}

GPE_Gui_Engine::~GPE_Gui_Engine()
{
    GPE_ProjectFolder * tempProject = NULL;
    for( int i = (int)gpeProjects.size()-1; i >=0; i--)
    {
        tempProject = gpeProjects[i];
        if( tempProject!=NULL)
        {
            delete tempProject;
            tempProject= NULL;
        }
    }
    gpeProjects.clear();



    if( eraserAnimation!=NULL)
    {
        guiRCM->remove_animation( eraserAnimation );
        eraserAnimation = NULL;
    }
    if( mainToolBar!=NULL)
    {
        delete mainToolBar;
        mainToolBar = NULL;
    }

    if( mainResourceTree!=NULL)
    {
        delete mainResourceTree;
        mainResourceTree = NULL;
    }

    if( GPE_MAIN_THEME!=NULL)
    {
        delete GPE_MAIN_THEME;
        GPE_MAIN_THEME = NULL;
    }

    if( MAIN_CONTEXT_MENU!=NULL)
    {
        delete MAIN_CONTEXT_MENU;
        MAIN_CONTEXT_MENU = NULL;
    }
    if(GPE_Main_TabManager!=NULL)
    {
        delete GPE_Main_TabManager;
        GPE_Main_TabManager = NULL;
    }

    if(GPE_Main_Logs!=NULL)
    {
        delete GPE_Main_Logs;
        GPE_Main_Logs = NULL;
    }
    if( GPE_ANCHOR_GC!=NULL )
    {
        GPE_ANCHOR_GC->clear_list( false );
        delete GPE_ANCHOR_GC;
        GPE_ANCHOR_GC = NULL;
    }

    if(GPE_Cursor!=NULL)
    {
        SDL_FreeCursor(GPE_Cursor);
        GPE_Cursor = NULL;
    }
    if(GPE_LoadingCursor!=NULL)
    {
        SDL_FreeCursor(GPE_LoadingCursor);
        GPE_LoadingCursor = NULL;
    }
    if(GPE_HoverCursor!=NULL)
    {
        SDL_FreeCursor(GPE_HoverCursor);
        GPE_HoverCursor = NULL;
    }

    if( GPE_TEXTURE_COLOR_PICKER_GRADIENT!=NULL)
    {
        delete GPE_TEXTURE_COLOR_PICKER_GRADIENT;
        GPE_TEXTURE_COLOR_PICKER_GRADIENT = NULL;
    }
    if( GPE_SURFACE_COLOR_PICKER_GRADIENT!=NULL)
    {
        SDL_FreeSurface(GPE_SURFACE_COLOR_PICKER_GRADIENT);
        GPE_SURFACE_COLOR_PICKER_GRADIENT = NULL;
    }
    if( GPE_LOGO!=NULL)
    {
        delete GPE_LOGO;
        GPE_LOGO = NULL;
    }


}

GPE_Theme * GPE_Gui_Engine::add_theme(std::string themeName, bool customTheme )
{
    GPE_Theme * newTemplate = new GPE_Theme(themeName, customTheme);
    gpeThemes.push_back( newTemplate);
    return newTemplate;
}

void GPE_Gui_Engine::add_to_recent_project_list(std::string newProjectFileName, bool saveData)
{
    int cSize = (int)gpeRecentProjects.size();
    int i = 0;
    if( (int)newProjectFileName.size() > 0 )
    {
        cSize = (int)gpeRecentProjects.size();
        for(  i = cSize-1; i >=0; i--)
        {
            if( gpeRecentProjects[i]==newProjectFileName)
            {
                gpeRecentProjects.erase( gpeRecentProjects.begin()+i);
            }
        }
        cSize = (int)gpeRecentProjects.size();
        if( cSize >= 15)
        {
            int cDiff = cSize - 14;
            for(  i = cSize-1; i >=cDiff; i--)
            {
                gpeRecentProjects.erase( gpeRecentProjects.begin()+i);
            }
        }
        gpeRecentProjects.insert(gpeRecentProjects.begin(),newProjectFileName);
        GPE_Report("Added ["+newProjectFileName+"] to Recent Project List");
    }
    if( saveData)
    {
        cSize = (int)gpeRecentProjects.size();
        std::string recentFileListFileName = get_user_settings_folder()+"recent_projects.txt";
        std::ofstream recentFileListFile( recentFileListFileName.c_str() );
        if( !recentFileListFile.fail() )
        {
            //makes sure the file is open
            if (recentFileListFile.is_open())
            {
                for( i = cSize-1; i>=0; i--)
                {
                    recentFileListFile << gpeRecentProjects[i] <<  "\n";
                }
                if( CURRENT_PROJECT!=NULL)
                {
                    recentFileListFile << CURRENT_PROJECT->get_project_file_name() <<  "\n";
                }
                recentFileListFile.close();
            }
        }
    }
    update_recent_project_list(false);
}

void GPE_Gui_Engine::apply_logic()
{
    //always resetting the tooltip
    if( input->input_received() )
    {
        if( MAIN_OVERLAY!=NULL)
        {
            MAIN_OVERLAY->update_tooltip("");
        }
        update_popup_info();
    }
    process_window_title();

    if( mainToolBar!=NULL && MAIN_CONTEXT_MENU!=NULL)
    {
        //all other gui elements are dependent on the toolbar and popup menus being closed
        if( !MAIN_CONTEXT_MENU->is_open() )
        {
            //prevents the toolbar isnt activated when popup menu is open
            mainToolBar->process_toolbar();
        }

        if( mainToolBar->open_requested() )
        {
            GPE_DOCK->update_toolbar();
        }
        if( !mainToolBar->is_open() )
        {
            //prevents the popup menu from being processed while toolbar is open
            if( MAIN_CONTEXT_MENU->is_open() )
            {
                //POPUP_MENU_VALUE = MAIN_CONTEXT_MENU->process_menu_option();
                //if( input->check_mouse_released(kb_anykey) || WINDOW_WAS_JUST_RESIZED || input->released[ESC] || input->released[kb_enter])
                {
                    GPE_close_context_menu();
                }
            }
        }

        if( !mainToolBar->is_open() && !MAIN_CONTEXT_MENU->is_open() )
        {
            if( GPE_Main_Statusbar!=NULL)
            {
                GPE_Main_Statusbar->process_self();
            }

            if( GPE_DOCK!=NULL)
            {
                GPE_DOCK->setup_dock();
                if( mainResourceTree!=NULL && PANEL_RESOURCE_TREE!=NULL )
                {
                    PANEL_RESOURCE_TREE->clear_panel();
                    PANEL_RESOURCE_TREE->add_gui_element_fullsize(  mainResourceTree );
                }
                GPE_DOCK->process_self();
                if( PANEL_RESOURCE_TREE!=NULL)
                {
                    if( input->check_keyboard_down( kb_ctrl) || input->check_keyboard_only_released(kb_1) )
                    {
                        //GPE_Main_Logs->log_general_line( )
                    }
                }
            }
            else
            {
                GPE_Report( "GPE_DOCK not initialized!" );
            }
        }
    }
    if( mainToolBar!=NULL && mainToolBar->just_closed() )
    {
        process_overlay_message();
    }

    if( input->check_keyboard_down(kb_ctrl) )
    {
        if( input->check_keyboard_released(kb_b) )
        {
            GPE_Main_TabManager->add_new_tab(MAIN_ABOUT_PAGE);
        }
        else if( input->pressed[kb_f])
        {
            if( input->down[kb_shift] )
            {
                MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FINDALL;

            }
            else
            {
                MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FIND;
            }
            MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
            if( GPE_Main_Logs!=NULL)
            {
                GPE_Main_Logs->open_search_mode();
            }
        }
        else if( input->pressed[kb_g])
        {
            MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_GOTO;
            MAIN_SEARCH_CONTROLLER->goToLineStringBox->set_string("1");
            MAIN_SEARCH_CONTROLLER->goToLineStringBox->switch_inuse(true);
            if( GPE_Main_Logs!=NULL)
            {
                GPE_Main_Logs->open_search_mode();
            }
        }
        else if( input->pressed[kb_h])
        {
            MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACE;
            MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true );
            if( GPE_Main_Logs!=NULL)
            {
                GPE_Main_Logs->open_replace_mode();
            }
        }
        else if( input->check_keyboard_released(kb_n) )
        {
            launch_new_project();
        }
        else if( input->check_keyboard_released(kb_o) )
        {
            open_new_project();
        }
        else if( input->check_keyboard_released(kb_q) )
        {
            GPE_Action_Message=="Quit Editor";
            input->done = true;
        }
        else if( input->pressed[kb_r])
        {
            if( input->down[kb_shift] )
            {
                MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACEALL;

            }
            else
            {
                MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACE;

            }
            MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
            if( GPE_Main_Logs!=NULL)
            {
                GPE_Main_Logs->open_replace_mode();
            }
        }
        else if( input->check_keyboard_released(kb_s) )
        {
            if( input->check_keyboard_down(kb_shift) )
            {
                save_all_projects();
            }
            else
            {
                save_current_project();
            }
        }
        else if( input->check_keyboard_released(kb_u) )
        {
            GPE_Main_TabManager->add_new_tab(MAIN_EDITOR_SETTINGS);
        }
    }
    else if( input->check_keyboard_released(kb_f1) )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_HELP_PAGE);
    }
    else if( input->check_keyboard_released(kb_f2) )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_EDITOR_SETTINGS);
    }
    else if( input->check_keyboard_released(kb_f5) )
    {
        GPE_MAIN_GUI->export_current_project_html5( input->check_keyboard_down(kb_ctrl)==false );
    }
    if( input->check_keyboard_released(kb_f6) )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_START_PAGE);
    }
    else if( input->check_keyboard_released(kb_f8) && CURRENT_PROJECT!=NULL )
    {
        GPE_MAIN_GUI->rum_current_project("",GPE_BUILD_HTML5);
    }
    else if( input->check_keyboard_released(kb_f10) )
    {
        //gcanvas->get_font_cache_size(true);
    }
    else if( input->check_keyboard_released(kb_f11) )
    {
        if( !MAIN_RENDERER->is_fullscreen() )
        {
            MAIN_OVERLAY->update_temporary_message("Entering Full-Screen","Press [F11] to exit","",1);
        }
        else
        {
            MAIN_OVERLAY->update_temporary_message("Exiting Full-Screen","Press [F11] to toggle","",1);
        }
        MAIN_RENDERER->toggle_fullscreen();
    }
    else if( input->check_keyboard_released(kb_f12) )
    {
        GPE_MAIN_GUI->take_live_screenshor();
    }
    else if( input->check_keyboard_released(kb_esc) )
    {
        MAIN_SEARCH_CONTROLLER->close_finder();
    }

    int uInS = input->fileDroppedList.size();
    std::string tempInputtedFile = "";
    if( uInS > 0)
    {
        for( int iRFile = 0; iRFile < uInS; iRFile++)
        {
            tempInputtedFile = input->fileDroppedList.at( iRFile);
            if( file_exists(tempInputtedFile) )
            {
                if( get_file_ext(tempInputtedFile)=="gppf" )
                {
                    open_project(tempInputtedFile);
                }
                else
                {
                    display_user_alert("Error","Content dropped in window:"+tempInputtedFile+".");
                }
            }
            else
            {
                display_user_alert("Error","Content dropped in window:"+tempInputtedFile+" file not found.");
            }
        }
    }

    if( input->check_mouse_released( mb_anybutton ) )
    {
        RESOURCE_TO_DRAG = NULL;
        LAST_CLICKED_RESOURCE = NULL;
    }

    //If the user has Xed out the window
    if( MAIN_RENDERER->windowClosed || input->done || GPE_Action_Message=="Quit Editor" || GPE_Action_Message=="Quit" )
    {
        if( (int)gpeProjects.size() > 0 )
        {
            int returnedAction = -1;
            int projectAction = -1;
            GPE_ProjectFolder * tempProjFolder;
            for( int i= (int)gpeProjects.size()-1; i>=0; i--)
            {
                tempProjFolder = gpeProjects[i];
                if( tempProjFolder!=NULL)
                {
                    projectAction = GPE_Display_Basic_Prompt("[Warning!] "+tempProjFolder->get_project_name()+" project not saved","Will you like to save and close before exiting?",true);
                    if( projectAction==DISPLAY_QUERY_YES)
                    {
                        tempProjFolder->save_project();
                        remove_project( tempProjFolder->get_project_file_name() );
                    }
                    else if( projectAction==DISPLAY_QUERY_CANCEL)
                    {
                        returnedAction = 2;
                        input->done = true;
                        MAIN_RENDERER->windowClosed = false;
                    }
                    else
                    {
                        remove_project( tempProjFolder->get_project_file_name() );
                        input->done = true;
                    }
                }
            }
            if( returnedAction!=2)
            {
                //Quit the programs
                set_next_state( STATE_EXIT );
            }
        }
        else
        {
            //Quit the programs
            set_next_state( STATE_EXIT );
        }
    }
}


GPE_Toolbar * GPE_Gui_Engine::init_toolbar()
{
    if( mainToolBar!=NULL)
    {
        delete mainToolBar;
    }
    mainToolBar = new GPE_Toolbar();
    return mainToolBar;
}

int GPE_Gui_Engine::get_recent_project_list_size()
{
    return (int)gpeRecentProjects.size();
}

int GPE_Gui_Engine::get_tip_count()
{
    return (int)gpeTips.size();
}

int GPE_Gui_Engine::get_random_tip()
{
    return random(0, (int)gpeTips.size()-1);
}

int GPE_Gui_Engine::get_previous_tip(int tipId)
{
    if( tipId >0 && tipId < (int)gpeTips.size() )
    {
        return tipId-1;
    }
    else
    {
        return (int)gpeTips.size()-1;
    }
}

int GPE_Gui_Engine::get_next_tip(int tipId)
{
    if( tipId >=0 && tipId < (int)gpeTips.size()-1 )
    {
        return tipId+1;
    }
    else
    {
        return 0;
    }
}

std::string GPE_Gui_Engine::get_tip(int tipId)
{
    if( tipId >=0 && tipId < (int)gpeTips.size() )
    {
        return gpeTips.at(tipId);
    }
    else
    {
        tipId = 0;
        if( (int)gpeTips.size() > 0)
        {
            return gpeTips.at(tipId);
        }
    }
    return "";
}

std::string GPE_Gui_Engine::get_recent_project_name( int pId )
{
    if( pId >=0 && pId < (int)gpeRecentProjects.size() )
    {
        return gpeRecentProjects.at(pId);
    }
    return "";
}

GPE_ResourceTree * GPE_Gui_Engine::init_resourcebar()
{
    if( mainResourceTree!=NULL)
    {
        delete mainResourceTree;
        mainResourceTree = NULL;
    }
    mainResourceTree = new GPE_ResourceTree();

    //update the tabbar and resourcemenu
    return mainResourceTree;
}

GPE_Toolbar * GPE_Gui_Engine::init_toolbar(std::string bName,GPE_Rect bRect)
{
    if( mainToolBar!=NULL)
    {
        delete mainToolBar;
    }
    mainToolBar = new GPE_Toolbar();
    mainToolBar->set_name( bName );
    mainToolBar->set_coords( bRect.x, bRect.y);
    mainToolBar->set_width( bRect.w) ;
    mainToolBar->set_height( bRect.h );
    return mainToolBar;
}

GPE_ProjectFolder * GPE_Gui_Engine::find_project_from_name(std::string projectName)
{
    if( (int)projectName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        GPE_ProjectFolder * foundProject = NULL;
        int i = 0;
        for( i = 0; i < (int)gpeProjects.size(); i++ )
        {
            tPFolder = gpeProjects[i];
            if( tPFolder!=NULL)
            {
                if( tPFolder->get_project_name().compare(projectName)==0 )
                {
                    foundProject = tPFolder;
                    break;
                }
            }
        }
        if( foundProject==NULL)
        {
            for( i = 0; i < (int)gpeProjects.size(); i++ )
            {
                tPFolder = gpeProjects[i];
                if( tPFolder!=NULL)
                {
                    if( tPFolder->get_project_file_name().compare(projectName)==0 )
                    {
                        foundProject = tPFolder;
                        break;
                    }
                }
            }
        }
        return foundProject;
    }
    return NULL;
}

GPE_ProjectFolder *  GPE_Gui_Engine::find_project_from_filename(std::string projectFileName)
{
    if( (int)projectFileName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        GPE_ProjectFolder * foundProject = NULL;
        for( int i = 0; i < (int)gpeProjects.size(); i++ )
        {
            tPFolder = gpeProjects[i];
            if( tPFolder!=NULL)
            {
                if( tPFolder->get_project_file_name().compare(projectFileName)==0 )
                {
                    foundProject = tPFolder;
                }
            }
        }
        return foundProject;
    }
    return NULL;
}

void GPE_Gui_Engine::clean_current_project_build_folder(int buildMetaTemplate)
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
    if( fFolder!=NULL)
    {
        fFolder->clean_build_folder(buildMetaTemplate);
    }
}

void GPE_Gui_Engine::clear_recent_project_list()
{
    gpeRecentProjects.clear();
    std::string recentFileListFileName = get_user_settings_folder()+"recent_projects.txt";
    std::ofstream recentFileListFile( recentFileListFileName.c_str() );
    recentFileListFile.close();
    update_recent_project_list(false);
}

void GPE_Gui_Engine::close_project(std::string projectFileName )
{
    remove_project( projectFileName);
}

int GPE_Gui_Engine::find_project_id_from_name(std::string projectName, int ignoreId)
{
    if( (int)projectName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        int foundProject = -1;
        for( int i = 0; i < (int)gpeProjects.size(); i++ )
        {
            if( i!=ignoreId)
            {
                tPFolder = gpeProjects[i];
                if( tPFolder!=NULL)
                {
                    if( tPFolder->get_project_name().compare(projectName)==0 )
                    {
                        foundProject = i;
                        break;
                    }
                }
            }
        }
        return foundProject;
    }
    return -1;
}

int GPE_Gui_Engine::find_project_id_from_filename(std::string projectFileName, int ignoreId)
{
    if( (int)projectFileName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        int foundProject = -1;
        for( int i = 0; i < (int)gpeProjects.size(); i++ )
        {
            if( i!=ignoreId)
            {
                tPFolder = gpeProjects[i];
                if( tPFolder!=NULL)
                {
                    if( tPFolder->get_project_file_name().compare(projectFileName)==0 )
                    {
                        foundProject = i;
                    }
                }
            }
        }
        return foundProject;
    }
    return -1;
}

GPE_Theme * GPE_Gui_Engine::find_theme(std::string themeName )
{
    if( (int)themeName.size() > 0 )
    {
        for( int i = (int)gpeThemes.size()-1; i >=0; i-- )
        {
            if( gpeThemes[i]->themeName == themeName)
            {
                return gpeThemes[i];
            }
        }
    }
    return NULL;
}

GPE_Toolbar * GPE_Gui_Engine::get_main_toolbar()
{
    return mainToolBar;
}

void GPE_Gui_Engine::launch_new_project()
{
    bool exitOperation = false;
    bool manualCancel = false;
    bool createButtonPushed = false;

    std::string newProjectFileName = "";
    std::string newProjectLocalFileName = "";
    std::string newProjectName = "";
    std::string foundFileName = "";
    RESOURCE_TO_DRAG = NULL;
    if( GPE_MAIN_GUI!=NULL && MAIN_RENDERER!=NULL)
    {
        GPE_Report("Launching New Project");
        std::string popUpCaption = "Launch New Project";
        GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
        MAIN_OVERLAY->process_cursor();
        GPE_MAIN_GUI->reset_gui_info();
        MAIN_OVERLAY->take_frozen_screenshot( );

        int promptBoxWidth = DEFAULT_WINDOW_MIN_WIDTH;
        int promptBoxHeight = DEFAULT_WINDOW_MIN_HEIGHT;
        GPE_Rect elementBox;


        input->reset_all_input();
        int currentTipId = GPE_MAIN_GUI->get_random_tip();
        if( currentTipId < 0 || currentTipId > GPE_MAIN_GUI->get_tip_count() )
        {
            currentTipId = 0;
        }
        std::string currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);

        GPE_Label_Title * mainMenuLabel = new GPE_Label_Title("Create A Project?","Create A Project");
        GPE_Label_Text * projectLocationLabel = new GPE_Label_Text("Project Location:","Project Location:");
        GPE_Label_Text * projectNameLabel = new GPE_Label_Text("Project Name:","Project Name:");

        GPE_Label_Text * projectMainExportLabel = new GPE_Label_Text("Primary Export Target:","Primary Export Target:");
        GPE_Label_Text * projectMainLanguageLabel = new GPE_Label_Text("Programming Language:","Programming Language:");
        int biggerLabelSize = std::max( projectMainExportLabel->get_width(), projectMainLanguageLabel->get_width() );
        biggerLabelSize = std::max( biggerLabelSize, projectNameLabel->get_width() );
        projectMainExportLabel->set_width(biggerLabelSize);
        projectNameLabel->set_width(biggerLabelSize);
        projectMainLanguageLabel->set_width(biggerLabelSize);

        //Adds all available languages to drop down menu
        GPE_DropDown_Menu * newprojectLanguage = new GPE_DropDown_Menu( "Project Language",true);
        int addedLanguages = 0;
        if( GPE_MAIN_HIGHLIGHTER!=NULL )
        {
            //short operation so not "optimized"
            GPE_Gui_Engine_Language * tLanguage  = NULL;
            int languageCount = (int)GPE_MAIN_HIGHLIGHTER->get_language_count();

            for( int cLanguage = 0; cLanguage < languageCount; cLanguage++ )
            {
                tLanguage = GPE_MAIN_HIGHLIGHTER->get_language_object( cLanguage);
                if( tLanguage!=NULL && tLanguage->isCodingLanguage)
                {
                    newprojectLanguage->add_menu_option(tLanguage->languageName+" ("+tLanguage->languageShortName+")",tLanguage->languageShortName,cLanguage,true);
                    addedLanguages++;
                }
            }
        }
        //In the event something went wrong and we somehow didn't add JS and any other new coding language...
        if( addedLanguages == 0 )
        {
            newprojectLanguage->add_menu_option("JavaScript","JS", PROGRAM_LANGUAGE_JS,true);
            newprojectLanguage->add_menu_option("C++","CPP", PROGRAM_LANGUAGE_CPP,true);
        }

        GPE_DropDown_Menu * newprojectMainExport= new GPE_DropDown_Menu( "Main Export Target",true);
        newprojectMainExport->add_menu_option("HTML5","HTML5",GPE_BUILD_HTML5,true);
        newprojectMainExport->add_menu_option("WINDOWS","WINDOWS", GPE_BUILD_WINDOWS);
        newprojectMainExport->add_menu_option("MAC","MAC",GPE_BUILD_MAC);
        newprojectMainExport->add_menu_option("LINUX","LINUX",GPE_BUILD_LINUX);


        GPE_GuiElementList * popupMenuList = new GPE_GuiElementList();
        GPE_TextInputBasic * projectNameField = new GPE_TextInputBasic("","New Project");
        GPE_Label_Text * fileToCreateField = new GPE_Label_Text("No file selected","No file selected" );
        fileToCreateField->set_width( DEFAULT_WINDOW_MIN_WIDTH -32 );
        GPE_ToolLabelButton * closeButton = new GPE_ToolLabelButton( "Cancel","Cancel");
        GPE_ToolLabelButton * createButton = new GPE_ToolLabelButton( "Create","Create");
        GPE_CheckBoxBasic * matchProjectFileName = new GPE_CheckBoxBasic("Same as file name","Makes project name same as file name", true );
        GPE_Label_Error * projectCreateErrorLabel = new GPE_Label_Error("Please input all fields above","");
        GPE_ToolPushButton * fileFindButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/folder.png","Browse Projects...");
        fileToCreateField->set_width( DEFAULT_WINDOW_MIN_WIDTH -32 );
        MAIN_RENDERER->reset_viewpoint( );
        //MAIN_OVERLAY->render_frozen_screenshot( );
        while(exitOperation==false)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            //GPE_Report("Processing tip of the day");
            gpe->start_loop();

            if( SCREEN_WIDTH > DEFAULT_WINDOW_MIN_WIDTH*2 )
            {
                promptBoxWidth =DEFAULT_WINDOW_MIN_WIDTH*2;
            }
            else
            {
                promptBoxWidth = SCREEN_WIDTH;
            }

            if( SCREEN_HEIGHT > DEFAULT_WINDOW_MIN_HEIGHT*2 )
            {
                promptBoxHeight =DEFAULT_WINDOW_MIN_HEIGHT*2;
            }
            else
            {
                promptBoxHeight = SCREEN_HEIGHT;
            }

            elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2;
            elementBox.y = (SCREEN_HEIGHT-promptBoxHeight)/2;
            elementBox.w = promptBoxWidth;
            elementBox.h = promptBoxHeight;
            popupMenuList->set_coords(elementBox.x, elementBox.y+32);
            popupMenuList->set_width(elementBox.w);
            popupMenuList->set_height(elementBox.h-32);
            popupMenuList->barXMargin = GENERAL_GPE_PADDING;
            popupMenuList->barYMargin = GENERAL_GPE_PADDING;
            popupMenuList->barXPadding = GENERAL_GPE_PADDING*2;
            popupMenuList->barYPadding = GENERAL_GPE_PADDING*2;
            GPE_MAIN_GUI->reset_gui_info();
            popupMenuList->clear_list();
            popupMenuList->add_gui_element(mainMenuLabel,true);

            popupMenuList->add_gui_element(projectLocationLabel,false);
            popupMenuList->add_gui_element(fileFindButton, true);

            popupMenuList->add_gui_element(fileToCreateField,true);
            popupMenuList->add_gui_element(matchProjectFileName, true);

            popupMenuList->add_gui_element(projectNameLabel,false);
            popupMenuList->add_gui_element(projectNameField,true);

            popupMenuList->add_gui_element(projectMainExportLabel, false);
            popupMenuList->add_gui_element(newprojectMainExport, true);

            popupMenuList->add_gui_element(projectMainLanguageLabel, false);
            popupMenuList->add_gui_element(newprojectLanguage, true);

            popupMenuList->add_gui_element(projectCreateErrorLabel, true);
            popupMenuList->add_gui_element(createButton,false);
            popupMenuList->add_gui_element(closeButton,false);
            popupMenuList->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
            if( input->check_keyboard_released(kb_esc) || closeButton->is_clicked() )
            {
                exitOperation = true;
                manualCancel = true;
                newProjectFileName = "";
            }
            else if( createButton->is_clicked() )
            {
                newProjectName = projectNameField->get_string();
                //Checks if a file is actually here or is it still just a folder...
                newProjectLocalFileName = get_local_from_global_file( newProjectName );
                if( (int)newProjectLocalFileName.size() > 0 )
                {
                    if( is_alnum(newProjectName,true, true) )
                    {
                        /*
                        If the path exists, file name checks, is alpha numeric and such
                        We exit this while loop and skip to end of function ready to create
                        a new project.
                        */
                        if( path_exists( get_path_from_file(newProjectFileName)  ) )
                        {
                            manualCancel = false;
                            createButtonPushed = true;
                            exitOperation = true;
                        }
                        else if( (int)newProjectFileName.size() > 0 )
                        {
                            projectCreateErrorLabel->set_name("Empty path given...");
                        }
                        else
                        {
                            projectCreateErrorLabel->set_name("Path does not exist...");
                        }
                    }
                    else
                    {
                        projectCreateErrorLabel->set_name("Project Name must be alphanumeric...");
                    }
                }
                else
                {
                    projectCreateErrorLabel->set_name("Empty project name given...");
                }
            }
            else if( fileFindButton->is_clicked() )
            {
                foundFileName = GPE_GetSaveFileName("Create a New Project File","Game Pencil Projects",GPE_MAIN_GUI->fileOpenProjectDir);
                if( (int)foundFileName.size() > 0 )
                {
                    newProjectFileName = foundFileName;
                    fileToCreateField->set_name(foundFileName);
                    if( matchProjectFileName!=NULL && matchProjectFileName->is_clicked() )
                    {
                        projectNameField->set_string( get_file_noext( get_local_from_global_file(foundFileName) ) );
                    }
                }

            }

            //GPE_Report("Rendering tip of the day");
            MAIN_RENDERER->reset_viewpoint( );
            if( !WINDOW_WAS_JUST_RESIZED)
            {
                //if( input->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot( );
                }
                //Update screen
                gcanvas->render_rectangle( elementBox.x-4,elementBox.y-4,elementBox.x+elementBox.w+8,elementBox.y+elementBox.h+8,c_blgray,false,64);
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Color,false);

                gcanvas->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,false);
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Button_Box_Highlighted_Color,true);
                gfs->render_text( elementBox.x+elementBox.w/2,elementBox.y+GENERAL_GPE_PADDING,popUpCaption,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
                popupMenuList->render_self( &GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
                //GPE_MAIN_GUI-render_gui_info(MAIN_RENDERER, true);

                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
                MAIN_OVERLAY->process_cursor();
                GPE_MAIN_GUI->render_gui_info( true);
            }
            gpe->end_loop();
        }

        input->reset_all_input();
        MAIN_OVERLAY->render_frozen_screenshot();
        MAIN_RENDERER->update_renderer();

        //Creates project if authorized to do so....
        if( manualCancel==false && (int)newProjectFileName.size()> 0 )
        {
            GPE_ProjectFolder * tPFolder = find_project_from_filename(newProjectFileName);
            if(tPFolder==NULL)
            {
                std::string newProjectDir= get_path_from_file(newProjectFileName);
                std::string newProjectLocalFileName = get_local_from_global_file(newProjectFileName);
                if( (int)newProjectLocalFileName.size()>0 && (int)newProjectName.size() > 0 )
                {
                    std::string newProjectTitle = get_file_noext(newProjectLocalFileName);
                    if( (int)newProjectTitle.size() > 0)
                    {
                        newProjectFileName = newProjectDir+"/"+newProjectTitle+".gppf";
                        newProjectDir = newProjectDir+"/"+newProjectTitle;
                        setup_project_directory(newProjectDir);
                        GPE_Main_Logs->log_general_line("Creating new Project["+newProjectName+"] at ["+newProjectFileName+"] location.");
                        GPE_Main_Logs->log_general_line("Detected Language for new project: ["+newprojectLanguage->get_selected_tag() + "]." );
                        GPE_ProjectFolder * newProject = new GPE_ProjectFolder(newProjectName,newProjectDir,newProjectFileName, newprojectLanguage->get_selected_tag(), true );
                        display_user_alert("Project Opened",newProjectFileName.c_str() );
                        gpeProjects.push_back(newProject);
                        newProject->save_project();
                    }
                }
            }
        }

        //Cleans up all of the UI elements.
        if( popupMenuList!=NULL)
        {
            delete popupMenuList;
            popupMenuList = NULL;
        }

        if( mainMenuLabel!=NULL)
        {
            delete mainMenuLabel;
            mainMenuLabel = NULL;
        }
        if( newprojectMainExport!=NULL)
        {
            delete newprojectMainExport;
            newprojectMainExport = NULL;
        }
        if( projectMainExportLabel!=NULL)
        {
            delete projectMainExportLabel;
            projectMainExportLabel = NULL;
        }
        if( newprojectLanguage!=NULL)
        {
            delete newprojectLanguage;
            newprojectLanguage = NULL;
        }
        if( projectMainLanguageLabel!=NULL)
        {
            delete projectMainLanguageLabel;
            projectMainLanguageLabel = NULL;
        }
        if( projectLocationLabel!=NULL)
        {
            delete projectLocationLabel;
            projectLocationLabel = NULL;
        }
        if( fileToCreateField!=NULL)
        {
            delete fileToCreateField;
            fileToCreateField = NULL;
        }
        if( matchProjectFileName!=NULL)
        {
            delete matchProjectFileName;
            matchProjectFileName = NULL;
        }
        if( closeButton!=NULL)
        {
            delete closeButton;
            closeButton = NULL;
        }
        if( fileFindButton!=NULL)
        {
            delete fileFindButton;
            fileFindButton = NULL;
        }
        gpe->start_loop();
    }
}

void GPE_Gui_Engine::load_tips()
{
    gpeTips.clear();
    gpeTips.push_back("Save your projects often to avoid lost work or memory in the event of a malfunction or crash!");
    gpeTips.push_back("In the Scene Editor, by holding 'M' you are you able to place multiple-objects at once.");
    gpeTips.push_back("Use the [kb_ctrl] KEY plus mouse wheel to horizontally scroll areas.");
    gpeTips.push_back("Be sure to take a look at the documentation for learning to make the most of Game Pencil.");
    gpeTips.push_back("You can have multiple projects open at once.");
    gpeTips.push_back("You can indent / unindent blocks of text/code using the [TAB] and [kb_shift-TAB] keys respectively");
    gpeTips.push_back("Objects inherit variables and functions from their parents. Very helpful for larger projects.");
    gpeTips.push_back("Use the arrow keys in input fields to traverse through previous entries.");
    gpeTips.push_back("Use the shortcut [kb_ctrl]+[O] to open a project to edit.");

    gpeTips.push_back("You are able to remove the background color, grayscale and invert an image using the transform button in the Sprite, Texture and Tilesheet editors.");
    gpeTips.push_back("Use the project settings editor to edit vital meta-data for your projects.");
    gpeTips.push_back("Need help or resources for your game? Check out the new Asset Store at http://store.pawbyte.com .");
    gpeTips.push_back("You can create a duplicate of your selected text, by pressing [kb_ctrl]+[D] keys.");
    gpeTips.push_back("You are able to give your objects custom functions in the object editor.");
    gpeTips.push_back("Save your current project folder using [kb_ctrl]+[S] keys together.");
    gpeTips.push_back("Assets are copied from their source directory to their project folder.");
    gpeTips.push_back("You can close a tab by clicking it with the middle mouse button or simply clicking on the X-icon.");
    gpeTips.push_back("The Game Pencil Editor is made via the SDL Library and some keyboard shortcuts may not behave 'normally to you'. If you experience any inconveniences please email help@pawbyte.com .");
    gpeTips.push_back("Save your projects often! There may be a few un-found errors in the  Game Pencil Engine and we will hate for a bug to be the cause of the loss of hours of work!");
    gpeTips.push_back("You can create a duplicate of the line the caret is on, by pressing [kb_ctrl]+[D] keys.");
    gpeTips.push_back("Version 1.1 of the GPE allows you to code in pure JavaScript.");
    gpeTips.push_back("You can add macros in your project's settings code editor.");
    gpeTips.push_back("You can debug your program using your web browser's console log.");
    gpeTips.push_back("You can save all open projects at once using the toolbar or buttonbar.");
    gpeTips.push_back("Project source code is provided free of obfuscation.");
    gpeTips.push_back("Use the shortcut [kb_ctrl]+[S] to save the current project you're editing.");
    gpeTips.push_back("You can get the most up to date documentation via http://docs.pawbyte.com");
}

void GPE_Gui_Engine::load_settings()
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_messages( "Loading Gui Settings", "Please Wait...","..." );
    }

    std::string otherColContainerName = "";

    std::string newFileIn = get_user_settings_folder()+"gpe_ide_settings.txt";
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    GPE_Report("Loading settings - "+newFileIn);
    //If the level file could be loaded
    if( !gameResourceFileIn.fail() )
    {
        //makes sure the file is open
        if (gameResourceFileIn.is_open())
        {
            int equalPos = 0;
            std::string firstChar="";
            std::string keyString="";
            std::string valString="";
            std::string subValString="";
            std::string currLine="";
            std::string currLineToBeProcessed;
            std::string colorThemeName;
            double foundFileVersion = 0;
            while ( gameResourceFileIn.good() )
            {
                getline (gameResourceFileIn,currLine); //gets the next line of the file
                currLineToBeProcessed = trim_left_inplace(currLine);
                currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

                if( foundFileVersion <=0 )
                {
                    //Empty Line skipping is only allowed at the top of the file
                    if(!currLineToBeProcessed.empty() )
                    {
                        //Comment skipping is only allowed at the top of the file
                        if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( keyString=="Version")
                                {
                                    foundFileVersion = string_to_double(valString);
                                }
                            }
                        }
                    }
                }
                else if( foundFileVersion <= 2 )
                {
                    //Begin processing the file.
                    if(!currLineToBeProcessed.empty() )
                    {
                        equalPos=currLineToBeProcessed.find_first_of("=");
                        if(equalPos!=(int)std::string::npos)
                        {
                            //if the equalPos is present, then parse on through and carryon
                            keyString = currLineToBeProcessed.substr(0,equalPos);
                            valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());


                            if( keyString == "BottomLogsHeight")
                            {
                                if( GPE_Main_Logs!=NULL )
                                {
                                    GPE_Main_Logs->set_height( string_to_int( valString,32 ) );
                                    GPE_Report("Logs Height = "+valString+";");
                                }
                                else
                                {
                                    GPE_Report("main logs = NULL...");
                                }
                            }
                            else if( keyString=="TextAreaViewLineCount")
                            {
                                MAIN_GUI_SETTINGS->showTextEditorLineCount = string_to_bool(valString);
                            }
                            else if( keyString=="TextAreaViewSyntaxHighlighting")
                            {
                                MAIN_GUI_SETTINGS->showTextEditorSyntaxHightlight =string_to_bool(valString);
                            }

                            else if(keyString=="ShowStartPageAtStart")
                            {
                                showStartPageAtStartUp =string_to_bool(valString);
                            }
                            else if(keyString=="ShowTipOfDayAtStart")
                            {
                                showTipsAtStartUp = string_to_bool(valString);
                                MAIN_EDITOR_SETTINGS->showStatupTipsBox->set_clicked(showTipsAtStartUp);
                            }
                            else if( keyString=="ShowFPSCounter")
                            {
                                SHOW_FPS_COUNTER = string_to_bool( valString);
                            }
                            else if(keyString=="ColorTheme")
                            {
                                colorThemeName = valString;
                                if( file_exists(APP_DIRECTORY_NAME+"themes/"+colorThemeName) )
                                {

                                }
                            }
                        }
                    }
                }
                else
                {
                    GPE_Report("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                }
            }
        }
    }
}

void GPE_Gui_Engine::open_new_project()
{
    std::string newProjectFileName = GPE_GetOpenFileName("Load Your Project","Game Pencil Projects",fileOpenProjectDir);
    open_project( newProjectFileName);
}

void GPE_Gui_Engine::open_project(std::string newProjectFileName)
{
    if( (int)newProjectFileName.size()> 5 )
    {
        newProjectFileName = string_replace_all(newProjectFileName,"\\","/");
        std::string newProjectDir = get_file_noext(newProjectFileName);
        GPE_ProjectFolder * fFolder = find_project_from_filename( newProjectFileName );
        if( fFolder==NULL)
        {
            if( get_file_ext(newProjectFileName)=="gppf" )
            {
                if( file_exists(newProjectFileName) )
                {
                    if( MAIN_GUI_SETTINGS && MAIN_GUI_SETTINGS->useShortProjectNames )
                    {
                        GPE_Main_Logs->log_general_line("Attempting to load ["+ get_local_from_global_file( newProjectFileName )+"]...");
                    }
                    else
                    {
                        GPE_Main_Logs->log_general_line("Attempting to load ["+newProjectFileName+"]...");
                    }
                    std::string previousProjectInView = CURRENT_PROJECT_NAME;
                    std::string newProjectTitle = "Loaded Project";
                    GPE_ProjectFolder * newProject = new GPE_ProjectFolder(newProjectTitle,newProjectDir,newProjectFileName);
                    CURRENT_PROJECT_NAME = newProjectFileName;
                    gpeProjects.push_back(newProject);
                    GPE_Report("New Project added into editor.");
                    bool projectLoadedSuccessfuly = newProject->load_project_file(newProjectFileName);
                    if( projectLoadedSuccessfuly )
                    {
                        newProject->integrate_syntax();
                        set_current_gpe_project_from_name( newProjectFileName );
                        GPE_Report("Checking with project list: ["+newProject->get_project_name()+"].");
                        add_to_recent_project_list(newProjectFileName,true );
                        GPE_Report("Project added to GPE ["+newProject->get_project_name()+"]");
                        GPE_Main_Logs->log_general_line("Done.");
                        if( MAIN_GUI_SETTINGS && MAIN_GUI_SETTINGS->useShortProjectNames )
                        {
                            GPE_Main_Logs->log_general_line("Project ["+ get_local_from_global_file( newProjectFileName )+"] processed...");
                        }
                        else
                        {
                            GPE_Main_Logs->log_general_line("Project ["+newProjectFileName+"] processed...");
                        }
                    }
                    else
                    {
                        set_current_gpe_project_from_name( previousProjectInView );
                        remove_project(newProjectFileName);
                    }
                }
                else
                {
                    display_user_alert("Project Open Error","File does not exist.");
                    GPE_Main_Logs->log_debug_line("Project Open Error - File does not exist ("+newProjectFileName+")...");
                }
            }
            else
            {
                display_user_alert("Project Open Error","Invalid Project File Type given.");
            }
        }
        else
        {
            //display_user_alert("Project Open Error","This project is already open!");
            display_user_alert("Project Open Error","This project is already open!");
        }
    }
}


void GPE_Gui_Engine::prerender_gui( )
{
    mainResourceTree->prerender_self( );
    mainToolBar->prerender_self( );
    MAIN_CONTEXT_MENU->prerender_self( );
}


void GPE_Gui_Engine::process_overlay_message()
{
    if( (int)GPE_Action_Message.size()== 0 )
    {
        return;
    }
    if( GPE_Action_Message=="Add Folder")
    {
        GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
        if( fFolder!=NULL)
        {
            std::string nFolderName = get_string_from_popup("Name Your Folder","Please enter the new name","newFolder" );
            if( (int)nFolderName.size()>0)
            {
                fFolder->create_blank_folder(LAST_CLICKED_RESOURCE, nFolderName);
            }
        }
    }
    else if( GPE_Action_Message=="Rename Folder" && LAST_CLICKED_RESOURCE!=NULL)
    {
        std::string nFolderName = get_string_from_popup("Rename Your Folder","Please enter the new name",LAST_CLICKED_RESOURCE->get_name() );
        if( (int)nFolderName.size()>0)
        {
            LAST_CLICKED_RESOURCE->set_name( nFolderName);
        }
    }
    else if( GPE_Action_Message=="Rename Resource" && LAST_CLICKED_RESOURCE!=NULL )
    {
        if( LAST_CLICKED_RESOURCE->get_resource_type()!=RESOURCE_TYPE_PROJECT_SETTINGS)
        {
            std::string nResName = get_string_from_popup("Name Your Resource","Please enter the new name",LAST_CLICKED_RESOURCE->get_name() );
            if( (int)nResName.size()>0)
            {
                LAST_CLICKED_RESOURCE->set_name(nResName);
            }
        }
    }
    else if( GPE_Action_Message=="Clear Resource Folder" || GPE_Action_Message=="Clear Resource Folder Contents")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( GPE_Display_Basic_Prompt("Warning","Are you sure you will like to clear this folder's contents?")==DISPLAY_QUERY_YES)
            {
                /*Coming in Version 1.1.3...
                GPE_GeneralResourceContainer * parRes = LAST_CLICKED_RESOURCE->parentResource;
                parRes->delete_resource(LAST_CLICKED_RESOURCE);
                LAST_CLICKED_RESOURCE = NULL;
                RESOURCE_TO_DRAG = NULL;*/
            }
        }
    }
    else if( GPE_Action_Message=="Delete Folder" || GPE_Action_Message=="Remove Folder")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( GPE_Display_Basic_Prompt("Warning","Are you sure you will like to delete this folder?")==DISPLAY_QUERY_YES)
            {
                GPE_GeneralResourceContainer * parRes = LAST_CLICKED_RESOURCE->parentResource;
                parRes->delete_resource(LAST_CLICKED_RESOURCE);
                LAST_CLICKED_RESOURCE = NULL;
                RESOURCE_TO_DRAG = NULL;
            }
        }
    }
    else if( GPE_Action_Message=="Delete Resource" || GPE_Action_Message=="Remove Resource")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( LAST_CLICKED_RESOURCE->get_resource_type()!=RESOURCE_TYPE_PROJECT_SETTINGS)
            {
                if( GPE_Display_Basic_Prompt("Warning","Are you sure you will like to delete this resource?")==DISPLAY_QUERY_YES)
                {
                    GPE_GeneralResourceContainer * parRes = LAST_CLICKED_RESOURCE->parentResource;
                    if( parRes!=NULL)
                    {
                        parRes->delete_resource(LAST_CLICKED_RESOURCE);
                        LAST_CLICKED_RESOURCE = NULL;
                        RESOURCE_TO_DRAG = NULL;
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Find Resource..." || GPE_Action_Message=="Find Local Resource..." )
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->get_resource_type()!=RESOURCE_TYPE_PROJECT_SETTINGS )
        {
            std::string resourceNeedle = get_string_from_popup("Project Resource Search","Resource name:","");
            if( (int)resourceNeedle.size() > 0 )
            {
                GPE_GeneralResourceContainer * foundRes = LAST_CLICKED_RESOURCE->find_resource_from_name(resourceNeedle);
                if( foundRes!=NULL)
                {
                    generalGameResource * foundHeldRes = foundRes->get_held_resource();
                    if( GPE_Main_TabManager!=NULL && foundHeldRes!=NULL )
                    {
                        GPE_Main_TabManager->add_new_tab(foundHeldRes);
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Add Resource")
    {
        GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
        if( fFolder!=NULL)
        {
            int tempResType = LAST_CLICKED_RESOURCE->get_resource_type();
            GPE_GeneralResourceContainer * newResource = fFolder->create_blank_resource(tempResType,LAST_CLICKED_RESOURCE);
            if( newResource!=NULL)
            {
                GPE_Main_TabManager->add_new_tab(newResource->get_held_resource() );
                GPE_MAIN_GUI->mainResourceTree->selectedSubOption = newResource->get_global_id();
            }
        }
    }
    else if( GPE_Action_Message=="Duplicate Resource" || GPE_Action_Message=="Clone Resource")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( !LAST_CLICKED_RESOURCE->is_folder() && !LAST_CLICKED_RESOURCE->is_super_project_folder() && !LAST_CLICKED_RESOURCE->is_super_project_folder() )
            {
                int tempResType = LAST_CLICKED_RESOURCE->get_resource_type();

                if( tempResType!=RESOURCE_TYPE_TEXTURE && tempResType!=RESOURCE_TYPE_TILESHEET && tempResType!=RESOURCE_TYPE_SPRITE && tempResType!=RESOURCE_TYPE_AUDIO && tempResType!=RESOURCE_TYPE_VIDEO && tempResType!=RESOURCE_TYPE_PROJECT_SETTINGS )
                {
                    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
                    if( fFolder!=NULL)
                    {
                        GPE_GeneralResourceContainer * newResource = fFolder->create_blank_resource(LAST_CLICKED_RESOURCE->get_resource_type(),LAST_CLICKED_RESOURCE->parentResource,"copy_of_"+LAST_CLICKED_RESOURCE->get_name() );
                        if( newResource!=NULL)
                        {
                            LAST_CLICKED_RESOURCE->save_container(get_user_temp_folder()+"temp_duplicated_resource.gpf");
                            newResource->preprocess_container(get_user_temp_folder()+"temp_duplicated_resource.gpf");
                            newResource->detect_name_change(true);
                            GPE_Main_TabManager->add_new_tab(newResource->get_held_resource() );
                            GPE_MAIN_GUI->mainResourceTree->selectedSubOption = newResource->get_global_id();
                        }
                        else
                        {
                            GPE_Report("Unable to duplicate resource - new resource is NULL...");
                        }
                    }
                    else
                    {
                        GPE_Report("Unable to duplicate resource - Unable to find project for resource ");
                    }
                }
            }
            else
            {
                GPE_Report("Unable to duplicate resource - Last clicked resource is a folder...");
            }
        }
        else
        {
            GPE_Report("Unable to duplicate resource - Last Clicked Resource or is parent equals NULL.");
        }
    }
    else if( GPE_Action_Message=="Close Project")
    {
        GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
        if( fFolder!=NULL)
        {
            int projectSaveState = GPE_Display_Basic_Prompt("Project Warning","Will you like to save this project before closing(Recommended)?",true);

            if( projectSaveState==DISPLAY_QUERY_YES)
            {
                save_current_project();
            }
            if( projectSaveState!=DISPLAY_QUERY_CANCEL)
            {
                //Close the project
                std::string projectNameToDelete = fFolder->get_project_file_name();
                if( GPE_LOADER != NULL )
                {
                    GPE_LOADER->update_messages( "Closing Game Project",projectNameToDelete, "Please wait..." );
                }

                GPE_Main_TabManager->close_tabs_from_project( projectNameToDelete );

                remove_project(projectNameToDelete);
            }
        }
    }
    else if( GPE_Action_Message=="New Project")
    {
        launch_new_project();
    }
    else if( GPE_Action_Message=="Open Project")
    {
        open_new_project();
    }
    else if( GPE_Action_Message== "Open Project Directory" )
    {
        if( CURRENT_PROJECT!=NULL)
        {
            GPE_OpenProgram(CURRENT_PROJECT->get_project_directory() );
        }
    }
    else if( GPE_Action_Message=="Save Project")
    {
        save_current_project();
    }
    else if( GPE_Action_Message=="Save All Projects")
    {
        save_all_projects();
    }
    else if( GPE_Action_Message=="Build Project" || GPE_Action_Message=="Build GAME")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
        }
    }
    else if( GPE_Action_Message=="Clean [HTML5] Build Folder")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            CURRENT_PROJECT->clean_build_folder(GPE_BUILD_HTML5);
        }
    }
    else if( GPE_Action_Message=="Clean [WiiU] Build Folder")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            CURRENT_PROJECT->clean_build_folder(GPE_BUILD_WIIU);
        }
    }
    else if( GPE_Action_Message=="Clean [Linux] Build Folder")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            CURRENT_PROJECT->clean_build_folder(GPE_BUILD_LINUX);
        }
    }
    else if( GPE_Action_Message=="Clean [OSX] Build Folder")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            CURRENT_PROJECT->clean_build_folder(GPE_BUILD_MAC);
        }
    }
    else if( GPE_Action_Message=="Build HTML5 APP" || GPE_Action_Message=="Build HTML5 GAME")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("HTML5");
        }
    }
    else if( GPE_Action_Message=="Build HTML5 APP & RUN" || GPE_Action_Message=="Build HTML5 GAME & RUN")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            /*
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("HTML5");
            */
            GPE_MAIN_GUI->export_current_project_html5(true);
        }
    }
    else if(GPE_Action_Message=="RUN HTML5 GAME")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            rum_current_project("",GPE_BUILD_HTML5);
        }
    }
    else if( GPE_Action_Message=="Build Nintendo Wii U GAME" || GPE_Action_Message=="Build NINTENDO Wii U" || GPE_Action_Message=="Build Wii U" || GPE_Action_Message=="Build WiiU")
    {
        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("WiiU");
        }
    }
    else if( GPE_Action_Message=="Build Nintendo Switch GAME")
    {
        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("Switch");
        }
    }
    else if( GPE_Action_Message=="Build WINDOWS GAME")
    {
        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("Windows");
        }
    }
    else if( GPE_Action_Message=="Build OSX GAME")
    {
        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("OSX");
        }
    }
    else if( GPE_Action_Message=="Build LINUX GAME")
    {
        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("Linux");
        }
    }
    else if( GPE_Action_Message=="Clean [HTML5] Build Folder")
    {
        clean_current_project_build_folder(GPE_BUILD_HTML5);
    }
    else if( GPE_Action_Message=="Clean [WiiU] Build Folder")
    {
        clean_current_project_build_folder(GPE_BUILD_WIIU);
    }
    else if( GPE_Action_Message=="Clean [Windows] Build Folder")
    {
        clean_current_project_build_folder( GPE_BUILD_WINDOWS );
    }
    else if( GPE_Action_Message=="Clean [OSX] Build Folder")
    {
        clean_current_project_build_folder( GPE_BUILD_MAC );
    }
    else if( GPE_Action_Message=="Clean [Linux] Build Folder")
    {
        clean_current_project_build_folder( GPE_BUILD_LINUX );
    }
    else if( GPE_Action_Message=="Online Documentation"|| GPE_Action_Message=="Documentation")
    {
        GPE_OpenURL("http://docs.pawbyte.com/");
    }
    else if( GPE_Action_Message=="EULA" || GPE_Action_Message=="License")
    {
        GPE_OpenURL("http://gamepencil.pawbyte.com/eula/");
    }
    else if( GPE_Action_Message=="Check Updates"|| GPE_Action_Message=="Check Version")
    {
        GPE_OpenURL("http://gamepencil.pawbyte.com/get-game-pencil-engine/");
    }

    else if( GPE_Action_Message=="Community" || GPE_Action_Message=="Online Forums" || GPE_Action_Message=="Community" || GPE_Action_Message=="Forums")
    {
        GPE_OpenURL("http://community.pawbyte.com/");
    }
    else if( GPE_Action_Message=="Community Chat")
    {
        GPE_OpenURL("https://discord.gg/bua3rDW");
    }
    else if( GPE_Action_Message=="Toggle Fullscreen Mode")
    {
        if( !MAIN_RENDERER->is_fullscreen() )
        {
            MAIN_OVERLAY->update_temporary_message("Entering Full-Screen","Press [F11] to exit","",1);
        }
        MAIN_RENDERER->toggle_fullscreen();
    }
    else if( GPE_Action_Message=="Toggle Logs")
    {
        GPE_DOCK->add_to_panel("Logs", DOCK_BOTTOM_MIDDLE );
        GPE_Main_Logs->toggle_manager();
    }
    else if( GPE_Action_Message=="Reset Dock" )
    {
        GPE_DOCK->reset_dock();
        if( GPE_Main_Logs->is_enabled() ==false )
        {
            GPE_Main_Logs->toggle_manager();
        }
        if( GPE_Main_TabManager->requests_fullscreen() )
        {
            GPE_Main_TabManager->toggle_full_width();
        }
    }
    else if( GPE_Action_Message=="Clear Recent Projects List")
    {
        GPE_MAIN_GUI->clear_recent_project_list();
    }
    else if( GPE_Action_Message=="Toggle Line Count")
    {
        MAIN_GUI_SETTINGS->showTextEditorLineCount = !MAIN_GUI_SETTINGS->showTextEditorLineCount;
        save_settings();
    }
    else if(GPE_Action_Message=="Toggle Syntax Highlighting")
    {
        MAIN_GUI_SETTINGS->showTextEditorSyntaxHightlight = !MAIN_GUI_SETTINGS->showTextEditorSyntaxHightlight;
        save_settings();
    }
    else if( GPE_Action_Message=="Find...")
    {
        if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->open_search_mode();
        }
        MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FIND;
        MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
    }
    else if( GPE_Action_Message=="Find in files...")
    {
        if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->open_search_mode();
        }
        MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FINDALL;
        MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
    }
    else if( GPE_Action_Message=="Goto Line...")
    {
        MAIN_SEARCH_CONTROLLER->textSearchMode = SEARCH_MODE_GOTO;
        MAIN_SEARCH_CONTROLLER->goToLineStringBox->set_string("1");
        MAIN_SEARCH_CONTROLLER->goToLineStringBox->switch_inuse(true);
        if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->open_search_mode();
        }
    }
    else if( GPE_Action_Message=="Find Project Resource...")
    {
        if( CURRENT_PROJECT!=NULL&& CURRENT_PROJECT->RESC_PROJECT_FOLDER!=NULL)
        {
            std::string resourceNeedle = get_string_from_popup("Project Resource Search","Resource name:","");
            if( (int)resourceNeedle.size() > 0 )
            {
                GPE_GeneralResourceContainer * foundRes = CURRENT_PROJECT->RESC_PROJECT_FOLDER->find_resource_from_name(resourceNeedle);
                if( foundRes!=NULL)
                {
                    generalGameResource * foundHeldRes = foundRes->get_held_resource();
                    if( GPE_Main_TabManager!=NULL && foundHeldRes!=NULL)
                    {
                        GPE_Main_TabManager->add_new_tab(foundHeldRes);
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Replace...")
    {
        MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACE;
        MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true );
        if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->open_replace_mode();
        }
    }
    else if( GPE_Action_Message=="Use Default Color Theme")
    {
        GPE_MAIN_THEME = GPE_DEFAULT_TEMPLATE;
        prerender_gui();
        save_settings();
    }
    else if( GPE_Action_Message=="Start Page")
    {
        GPE_Main_TabManager->add_new_tab(MAIN_START_PAGE);
    }
    else if( GPE_Action_Message=="Tip of the Day")
    {
        GPE_Show_Tip_Of_Day();
    }
    else if( GPE_Action_Message=="Report Issue")
    {
        GPE_OpenURL("https://github.com/pawbyte/Game-Pencil-Engine/issues");
    }
    else if( GPE_Action_Message=="About" || GPE_Action_Message=="About..." || GPE_Action_Message=="About Game Pencil Engine" )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_ABOUT_PAGE);
    }
    else if( GPE_Action_Message=="Help" || GPE_Action_Message=="Help..." || GPE_Action_Message=="General Help"  )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_HELP_PAGE);
    }
    else if( GPE_Action_Message=="User Settings" || GPE_Action_Message=="User Settings..." || GPE_Action_Message=="Game Pencil User Settings" )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_EDITOR_SETTINGS);
    }
    else if( GPE_Action_Message=="Game Controller Tester" )
    {
        GPE_Main_TabManager->add_new_tab( MAIN_GameController_TESTER );
    }
    else if( GPE_Action_Message=="JS Compiler Settings" )
    {
        GPE_Main_TabManager->add_new_tab( GPE_JS_COMPILER_SETTINGS );
    }
    else if( GPE_Action_Message=="Extra Tools" )
    {
        GPE_Main_TabManager->add_new_tab( MAIN_EXTRA_TOOLS );
    }
    else if( GPE_Action_Message=="C++ Compiler Settings" )
    {
        GPE_Main_TabManager->add_new_tab( GPE_CPP_COMPILER_SETTINGS );
    }
    else if( string_starts(GPE_Action_Message, "Import ") )
    {
        //Import Resources ImportResources
        if( CURRENT_PROJECT!=NULL)
        {
            std::string remainingType = get_substring(GPE_Action_Message, 7);
            int tempResType = 0;
            bool foundResType = false;
            for( int iResType = 0; iResType < res_type_count; iResType++ )
            {
                if( remainingType == RESOURCE_TYPE_NAMES[iResType] )
                {
                    foundResType = true;
                    tempResType = iResType;
                    break;
                }
            }

            if( foundResType )
            {
                std::string fileToImportName = GPE_GetOpenFileName( GPE_Action_Message,".gpf", MAIN_GUI_SETTINGS->fileOpenImportFileDir );
                if( file_exists(fileToImportName) && get_file_ext(fileToImportName) =="gpf" )
                {
                    GPE_GeneralResourceContainer * newResource = CURRENT_PROJECT->create_blank_resource(tempResType,NULL, get_file_noext(get_local_from_global_file( fileToImportName) ) );
                    if( newResource!=NULL)
                    {
                        GPE_Main_TabManager->add_new_tab(newResource->get_held_resource() );
                        newResource->preprocess_container(fileToImportName);
                        GPE_MAIN_GUI->mainResourceTree->selectedSubOption = newResource->get_global_id();
                        newResource->detect_name_change(true);
                    }
                    else
                    {
                        display_user_alert("Import file error","Unable to create new "+RESOURCE_TYPE_NAMES[tempResType]+" resource.");
                    }
                }
                else
                {
                    display_user_alert("Import file error","Unable to import "+RESOURCE_TYPE_NAMES[tempResType]+" file ["+get_local_from_global_file(fileToImportName)+"]");
                }
            }
        }
    }
    else if( string_starts(GPE_Action_Message, "Toggle ") && string_ends(GPE_Action_Message, " Pane") )
    {
        GPE_Action_Message = string_replace_all(GPE_Action_Message,"Toggle ","");
        GPE_Action_Message = string_replace_all(GPE_Action_Message," Pane","");
        GPE_DOCK->toggle_default_pane( GPE_Action_Message );
    }
    else if( file_exists(GPE_Action_Message) )
    {
        if( get_file_ext(GPE_Action_Message)=="gppf")
        {
            open_project(GPE_Action_Message);
        }
    }
    else
    {
        GPE_Report( "Unknown Command: ["+GPE_Action_Message+"]" );
    }
}

void GPE_Gui_Engine::process_window_title()
{
    if( MAIN_RENDERER!=NULL )
    {
        std::string windowCurrentTitle = "";
        if( CURRENT_PROJECT!=NULL)
        {
            windowCurrentTitle = CURRENT_PROJECT->get_project_name();
        }
        if( GPE_VERSION_UPDATE_NUMBER==0)
        {
            windowCurrentTitle+= "Game Pencil Engine   "+double_to_string(GPE_VERSION_DOUBLE_NUMBER)+".0 -";
        }
        else
        {
            windowCurrentTitle+= "Game Pencil Engine   "+double_to_string(GPE_VERSION_DOUBLE_NUMBER)+" -";
        }

        if( editorReleaseType==RELEASE_TYPE_PRODUCTION)
        {
            windowCurrentTitle+=" [Production]";
        }
        else if( editorReleaseType==RELEASE_TYPE_RC)
        {
            windowCurrentTitle+=" [Release Candidate]";
        }
        else if( editorReleaseType== RELEASE_TYPE_BETA)
        {
            windowCurrentTitle+=" [Beta]";
        }
        else if( editorReleaseType== RELEASE_TYPE_ALPHA)
        {
            windowCurrentTitle+=" [Alpha]";
        }
        if( !MAIN_RENDERER->windowHasFocus)
        {
            windowCurrentTitle += " Out of Focus |";
        }
        if( !MAIN_RENDERER->windowHasMouse)
        {
            windowCurrentTitle += " Mouse Outside |";
        }
        MAIN_RENDERER->set_window_title(windowCurrentTitle);
    }
}

void GPE_Gui_Engine::render_foreground_engine(bool forceRedraw)
{
    if(GPE_IS_LOADING)
    {
        SDL_SetCursor(GPE_LoadingCursor);
    }
    else
    {
        //SDL_SetCursor(GPE_Cursor);
    }

    MAIN_RENDERER->reset_viewpoint();

    if( mainToolBar!=NULL && MAIN_CONTEXT_MENU!=NULL )
    {
        if( !mainToolBar->is_open() && !MAIN_CONTEXT_MENU->is_open() )
        {
            if( !forceRedraw )
            {
                //return;
            }
            if( GPE_DOCK!=NULL )
            {
                GPE_DOCK->render_self( NULL, NULL, forceRedraw );
            }

            render_gui_info(  forceRedraw);
            MAIN_OVERLAY->render_temporary_message();
            GPE_MAIN_HIGHLIGHTER->render_code_highlights(  forceRedraw);
            GPE_MAIN_HIGHLIGHTER->render_code_suggestions(  forceRedraw);
            if( RESOURCE_TO_DRAG!=NULL && forceRedraw )
            {
                RESOURCE_TO_DRAG->render_option(  input->mouse_x+32, input->mouse_y, -1,NULL, NULL,false, true);
                gfs->render_text_boxed( input->mouse_x+32,input->mouse_y,RESOURCE_TO_DRAG->get_name(),GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_MAIN_THEME->PopUp_Box_Color,FONT_CATEGORY_BAR,FA_LEFT,FA_TOP );
            }
            if( forceRedraw )
            {
                gfs->render_fps(FONT_TEXTINPUT, GPE_MAIN_THEME->Main_Box_Font_Color );
            }
        }
        mainToolBar->render_toolbar( NULL, forceRedraw);
        MAIN_CONTEXT_MENU->render_self( &GPE_DEFAULT_CAMERA);
    }
}

void GPE_Gui_Engine::render_gui_info(bool forceRedraw)
{
    if(  forceRedraw)
    {
        if( GPE_Main_Statusbar!=NULL)
        {
            GPE_Main_Statusbar->render_self( NULL,NULL,true);
        }
        MAIN_OVERLAY->render_tooltip(  input->mouse_x,input->mouse_y+32);
    }
}

void GPE_Gui_Engine::remove_project(std::string projectFileName )
{
    if( (int)projectFileName.size()>0 )
    {
        GPE_ProjectFolder * tPFolder = NULL;
        for( int i = (int)gpeProjects.size()-1; i>=0; i--)
        {
            tPFolder = gpeProjects[i];
            if( tPFolder!=NULL )
            {
                if( tPFolder->get_project_file_name().compare(projectFileName)==0 )
                {
                    if( GPE_LOADER != NULL )
                    {
                        GPE_LOADER->update_messages( "Closing Game Project ["+tPFolder->get_project_name()+"]", projectFileName,"PLEASE WAIT" );
                    }
                    GPE_DOCK->setup_dock();
                    GPE_Main_TabManager->close_tabs_from_project( projectFileName );
                    if(CURRENT_PROJECT!=NULL)
                    {
                        if( CURRENT_PROJECT->get_project_name()==tPFolder->get_project_name() )
                        {
                            CURRENT_PROJECT = NULL;
                            CURRENT_PROJECT_NAME = "";
                        }
                    }

                    if( GPE_MAIN_GUI!=NULL && GPE_MAIN_GUI->mainResourceTree!=NULL)
                    {
                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Removing from Projects Menu.." );
                        }

                        GPE_MAIN_GUI->mainResourceTree->remove_project_resources(projectFileName);

                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Removed from Projects Menu.." );
                        }
                    }

                    delete tPFolder;
                    tPFolder = NULL;
                    gpeProjects.erase(gpeProjects.begin()+i );

                    GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Clearing cache..." );
                    gfs->clear_font_cache();
                    GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Cache Cleared..." );
                }
            }
        }
    }
}

void GPE_Gui_Engine::reset_gui_info()
{
    //update_tooltip("");
    if( GPE_Main_Statusbar!=NULL)
    {
        GPE_Main_Statusbar->reset_statusbar();
        /*GPE_Main_Statusbar->set_coords(0,SCREEN_HEIGHT-32);
        GPE_Main_Statusbar->set_width(SCREEN_WIDTH);
        GPE_Main_Statusbar->set_height(32);*/
    }
}

void GPE_Gui_Engine::save_all_projects()
{
    GPE_ProjectFolder * tPFolder = NULL;
    for( int i = 0; i < (int)gpeProjects.size(); i++ )
    {
        tPFolder = gpeProjects[i];
        if( tPFolder!=NULL)
        {
            tPFolder->save_project();
        }
    }

}

void GPE_Gui_Engine::export_current_project_html5(bool runGameOnCompile)
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
    if( fFolder!=NULL)
    {
        fFolder->save_project();
        fFolder->export_project_html5("",GPE_BUILD_HTML5,runGameOnCompile);
    }
}

void GPE_Gui_Engine::export_current_project_wiiu()
{
    /*
    //Not used for public build
    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
    if( fFolder!=NULL)
    {
        fFolder->save_project();
        fFolder->export_project_wiiu();
    }
    */
}

void GPE_Gui_Engine::rum_current_project(std::string projectBuildDirectory, int buildMetaTemplate)
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
    if( fFolder!=NULL)
    {
        fFolder->run_project(projectBuildDirectory, buildMetaTemplate);
    }
}


void GPE_Gui_Engine::save_current_project()
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
    if( fFolder!=NULL)
    {
        fFolder->save_project();
    }
}

void GPE_Gui_Engine::save_settings()
{
    std::string newSaveDataFilename = get_user_settings_folder()+"gpe_ide_settings.txt";
    std::ofstream newSaveDataFile( newSaveDataFilename.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            //write_header_on_file(&newSaveDataFile);
            newSaveDataFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            if( GPE_Main_Logs!=NULL )
            {
                newSaveDataFile << "BottomLogsHeight=" << GPE_Main_Logs->get_height() << "\n";

            }
            newSaveDataFile << "ShowFPSCounter=" << SHOW_FPS_COUNTER << "\n";
            newSaveDataFile << "ShowStartPageAtStart=" << showStartPageAtStartUp << "\n";
            newSaveDataFile << "ShowTipOfDayAtStart=" << showTipsAtStartUp <<  "\n";
            newSaveDataFile << "TextAreaViewLineCount=" << MAIN_GUI_SETTINGS->showTextEditorLineCount << "\n";
            newSaveDataFile << "TextAreaViewSyntaxHighlighting=" << MAIN_GUI_SETTINGS->showTextEditorSyntaxHightlight << "\n";
            if( GPE_MAIN_THEME!=NULL)
            {
                newSaveDataFile << "ColorTheme=" << GPE_MAIN_THEME->themeLocalLocation << "\n";
            }
            newSaveDataFile.close();
        }
    }
}

void GPE_Gui_Engine::setup_project_directory(std::string newProjectDir)
{
    if( (int)newProjectDir.size() > 3)
    {
        create_directory(newProjectDir);
        create_directory(newProjectDir+"/gpe_project");
        create_directory(newProjectDir+"/gpe_project/builds");

        create_directory(newProjectDir+"/gpe_project/data");

        create_directory(newProjectDir+"/gpe_project/gpebackups");

        create_directory(newProjectDir+"/gpe_project/resources");
        create_directory(newProjectDir+"/gpe_project/resources/achievements");
        create_directory(newProjectDir+"/gpe_project/resources/audio");
        //Class Directory addition for 1.14
        create_directory(newProjectDir+"/gpe_project/resources/animations");
        create_directory(newProjectDir+"/gpe_project/resources/classes");
        create_directory(newProjectDir+"/gpe_project/resources/fonts");
        create_directory(newProjectDir+"/gpe_project/resources/functions");
        create_directory(newProjectDir+"/gpe_project/resources/lights");
        create_directory(newProjectDir+"/gpe_project/resources/objects");
        create_directory(newProjectDir+"/gpe_project/resources/paths");
        create_directory(newProjectDir+"/gpe_project/resources/particles");
        create_directory(newProjectDir+"/gpe_project/resources/scenes");
        create_directory(newProjectDir+"/gpe_project/resources/textures");
        create_directory(newProjectDir+"/gpe_project/resources/tilesheets");
        create_directory(newProjectDir+"/gpe_project/resources/videos");
    }
}

std::string GPE_Gui_Engine::setup_build_folder(std::string buildDirectory,int buildType, int buildBits, bool inDebugMode, int nativeBuildType)
{
    if( GPE_Main_Logs!=NULL)
    {
        if( (int)buildDirectory.size() > 3)
        {
            /*if( buildType!=GPE_BUILD_HTML5 && buildType!=GPE_BUILD_WIIU)
            {
                if( buildBits==64)
                {
                    buildDirectory+="_64";
                }
                else
                {
                    buildDirectory+="_32";
                }
            }
            if( inDebugMode)
            {
                //buildDirectory+="_debug";
            }
            */
            std::string newBuildDirectory = buildDirectory;

            create_directory(buildDirectory);
            if( path_exists(buildDirectory) )
            {
                if( ( buildType==GPE_BUILD_WINDOWS || buildType==GPE_BUILD_LINUX ) && nativeBuildType==Native_Electron)
                {
                    create_directory(buildDirectory+"/locales");
                    create_directory(buildDirectory+"/resources");
                    create_directory(buildDirectory+"/resources/app");
                    if( path_exists( buildDirectory+"/resources/app") )
                    {
                        newBuildDirectory = buildDirectory+"/resources/app";
                    }
                    else
                    {
                        GPE_Main_Logs->log_build_line("Unable to access ["+buildDirectory+"/resources/app] folder...");
                    }
                }
                else if( buildType== GPE_BUILD_MAC && nativeBuildType==Native_Electron)
                {
                    create_directory(buildDirectory+"/Electron.app");
                    create_directory(buildDirectory+"/Electron.app/Contents");
                    create_directory(buildDirectory+"/Electron.app/Contents/Resources");
                    if( path_exists( buildDirectory+"/Electron.app/Contents/Resources") )
                    {
                        newBuildDirectory = buildDirectory+"/Electron.app/Contents/Resources";
                    }
                    else
                    {
                        GPE_Main_Logs->log_build_line("Unable to access ["+buildDirectory+"/Electron.app/Contents/Resources] folder...");
                    }
                }

                create_directory(newBuildDirectory+"/css");
                create_directory(newBuildDirectory+"/gamedata");
                create_directory(newBuildDirectory+"/js");
                create_directory(newBuildDirectory+"/js/lib");

                create_directory(newBuildDirectory+"/res");

                create_directory(newBuildDirectory+"/resources");
                create_directory(newBuildDirectory+"/resources/animations");
                create_directory(newBuildDirectory+"/resources/audio");
                create_directory(newBuildDirectory+"/resources/fonts");
                create_directory(newBuildDirectory+"/resources/lights");
                create_directory(newBuildDirectory+"/resources/particles");
                create_directory(newBuildDirectory+"/resources/sprites");
                create_directory(newBuildDirectory+"/resources/textures");
                create_directory(newBuildDirectory+"/resources/tilesheets");
                create_directory(newBuildDirectory+"/resources/videos");

                if( nativeBuildType ==Native_Electron )
                {
                    if( buildType == GPE_BUILD_WINDOWS)
                    {
                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_messages( "Copying Electron-Windows Files", "Please wait","Do NOT CLOSE" );
                        }
                        if( buildBits==64)
                        {
                            GPE_Main_Logs->log_build_line("Copying Electron-Windows 64Bit Files...");
                            if( GPE_LOADER!=NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [game.exe and dlls] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/win64build_electron",buildDirectory);

                            if( GPE_LOADER!=NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [game.exe and dlls] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/win64build_electron/locales",buildDirectory+"/locales");


                            if( GPE_LOADER!=NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [resources] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/win64build_electron/resources",buildDirectory+"/resources", true);
                        }
                        else
                        {
                            GPE_Main_Logs->log_build_line("Copying Electron-Windows 32Bit Files...");
                            if( GPE_LOADER!=NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [game.exe and dlls] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/win32build_electron",buildDirectory);

                            if( GPE_LOADER!=NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [locales] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/win32build_electron/locales",buildDirectory+"/locales");

                            if( GPE_LOADER!=NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [resources] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/win32build_electron/resources",buildDirectory+"/resources", true);
                        }
                    }
                    else if( buildType == GPE_BUILD_LINUX)
                    {
                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_messages( "Copying Electron-Linux Files", "Please wait","Do NOT CLOSE" );
                        }

                        if( buildBits==64)
                        {
                            GPE_Main_Logs->log_build_line("Copying Electron-Linux 64bit Files...");
                            if( GPE_LOADER != NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [game and SO files] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/linux64build_electron",buildDirectory);

                            if( GPE_LOADER != NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [locales] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/linux64build_electron/locales",buildDirectory+"/locales");


                            if( GPE_LOADER != NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [resources] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/linux64build_electron/resources",buildDirectory+"/resources", true);
                        }
                        else
                        {
                            GPE_Main_Logs->log_build_line("Copying Electron-Linux 32Bit Files...");
                            if( GPE_LOADER != NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [game and SO files] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/linux32build_electron",buildDirectory);

                            if( GPE_LOADER != NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [locales] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/linux32build_electron/locales",buildDirectory+"/locales");

                            if( GPE_LOADER != NULL )
                            {
                                GPE_LOADER->update_submessages( "Copying [resources] folder", "Please Wait..." );
                            }
                            copy_folder(APP_DIRECTORY_NAME+"build_files/linux32build_electron/resources",buildDirectory+"/resources", true);
                        }
                    }
                    else if(buildType == GPE_BUILD_MAC)
                    {
                        GPE_Main_Logs->log_build_comment("Automatic Electron OSX building not supported yet...");
                    }
                }
                else
                {
                    if( buildType == GPE_BUILD_WINDOWS || buildType == GPE_BUILD_MAC || buildType == GPE_BUILD_LINUX)
                    {
                        GPE_Main_Logs->log_build_comment("Electron not copied, previous version if available will be used...");
                    }
                }
            }
            else
            {
                GPE_Main_Logs->log_build_error("Unable to create ["+buildDirectory+"] folder. Unable to access...");
            }
            return newBuildDirectory;
        }
        else
        {
            GPE_Main_Logs->log_build_error("Unable to create ["+buildDirectory+"] folder. Unallowed name.");
        }
    }
    return "";
}

void GPE_Gui_Engine::take_live_screenshor()
{

    if( MAIN_RENDERER!=NULL)
    {
        if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->autoSaveScreenshots!=NULL && MAIN_EDITOR_SETTINGS->autoSaveScreenshots->is_clicked() )
        {
            if( MAIN_EDITOR_SETTINGS->makeMetaScreenshots!=NULL && MAIN_EDITOR_SETTINGS->makeMetaScreenshots->is_clicked() )
            {
                if( (int)CURRENT_PROJECT_NAME.size() > 0 )
                {
                    std::time_t rawtime;
                    std::tm* timeinfo;
                    char timeBuffer [80];

                    std::time(&rawtime);
                    timeinfo = std::localtime(&rawtime);

                    std::strftime(timeBuffer,80,"%y%m%d_%H%M%S",timeinfo);
                    std::puts(timeBuffer);

                    bool foundBlank = false;
                    std::string screenshotNewLocation  = "";


                    screenshotNewLocation = APP_DIRECTORY_NAME+"screenshots/"+get_file_noext( get_local_from_global_file( CURRENT_PROJECT_NAME) )+"_"+timeBuffer+".png";
                    if( file_exists( screenshotNewLocation) ==false )
                    {
                        foundBlank = true;
                        MAIN_RENDERER->save_screenshot(screenshotNewLocation);
                    }
                    for(int i = 1; foundBlank == false && i <= 1000; i++)
                    {
                        screenshotNewLocation = APP_DIRECTORY_NAME+"screenshots/"+get_file_noext( get_local_from_global_file( CURRENT_PROJECT_NAME) )+"_"+timeBuffer+"_"+int_to_string(i)+".png";
                        if( file_exists( screenshotNewLocation) ==false )
                        {
                            foundBlank = true;
                            MAIN_RENDERER->save_screenshot(screenshotNewLocation);
                            break;
                        }
                    }
                    if( foundBlank==false)
                    {
                        appendToFile(APP_DIRECTORY_NAME+"screenshots/screenshot_log.txt","Unable to save screenshot to "+screenshotNewLocation);
                        GPE_Report("Unable to save screenshot to "+screenshotNewLocation);
                    }
                }
                else
                {
                    MAIN_RENDERER->save_screenshot();
                }
            }
            else
            {
                MAIN_RENDERER->save_screenshot();
            }
        }
    }
}

void GPE_Gui_Engine::update_recent_project_list(bool saveData)
{
    if( MAIN_TOOLBAR_RECENT_PROJECTS!=NULL)
    {
        MAIN_TOOLBAR_RECENT_PROJECTS->clear_menu();
        int iRPList = 0;
        int maxRPList = GPE_MAIN_GUI->get_recent_project_list_size();
        for( iRPList = 0; iRPList < maxRPList; iRPList++)
        {
            if( iRPList==maxRPList-1)
            {
                MAIN_TOOLBAR_RECENT_PROJECTS->add_menu_option(GPE_MAIN_GUI->get_recent_project_name(iRPList),-1,NULL,-1,NULL, true );
            }
            else
            {
                MAIN_TOOLBAR_RECENT_PROJECTS->add_menu_option(GPE_MAIN_GUI->get_recent_project_name(iRPList),-1,NULL,-1,NULL,false );
            }
        }
        MAIN_TOOLBAR_RECENT_PROJECTS->add_menu_option("Clear Recent Projects List");
    }
}


//



//


//
GPE_Editor_State::GPE_Editor_State()
{
    stateName = "GPE_Editor";
    if( GPE_VERSION_UPDATE_NUMBER==0)
    {
        GPE_Main_Logs->log_general_line("Starting Game Pencil Engine Version "+double_to_string(GPE_VERSION_DOUBLE_NUMBER)+".0...");
    }
    else
    {
        GPE_Main_Logs->log_general_line("Starting Game Pencil Engine Version "+double_to_string(GPE_VERSION_DOUBLE_NUMBER)+"...");
    }

    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    //Gets SDL Version
    SDL_compiled_version.major = 0;
    SDL_VERSION(&SDL_compiled_version);
    SDL_VersionText = "SDL Version: "+int_to_string(SDL_compiled_version.major)+"."+int_to_string(SDL_compiled_version.minor)+"."+int_to_string(SDL_compiled_version.patch);
    spriteDataLabelWidth =  0;
    int hh=0; // the 8th position is the widest
    GPE_DEFAULT_FONT->get_metrics(spriteDataLabels[sprHPixelPadding].c_str(),&spriteDataLabelWidth,&hh);
    tsDataLabelWidth = 0;
    GPE_DEFAULT_FONT->get_metrics(tsDataLabels[5].c_str(),&tsDataLabelWidth,&hh);
    spriteDataLabelWidth+=GENERAL_ICON_WIDTH_AND_PADDING; //add some extra padding for "just in case"

    mainToolBar = GPE_MAIN_GUI->init_toolbar();

    GPE_Animation * mainExportOptionsSprite = guiRCM->sprite_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/main_export_options_icons.png",12,true,0,0,false);
    mainButtonBar = new GPE_ToolIconButtonBar(24);
    mainButtonBar->set_coords( 0, mainToolBar->get_height() );
    mainButtonBar->widthAutoResizes = true;
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/file.png","New Project",1,true );
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/folder.png","Open Project",2,true );
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/save.png","Save Project",3,false );
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/book.png","Save All Projects",4,true );
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/cog.png","Build Project",5,false);
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png","Build & PLAY Project",6,false);

    GPE_PopUpMenu_Option * newOption = NULL;
    GPE_PopUpMenu_Option * newOptionLayer2 = NULL;
    GPE_PopUpMenu_Option * newOptionLayer3 = NULL;
    newOption = mainToolBar->add_menu_option("File");
    newOption->add_menu_option("New Project",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/file.png"),-1,NULL,false,true,false,kb_ctrl,kb_n );
    newOption->add_menu_option("Open Project",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/folder.png"),-1,NULL,false,true,false,kb_ctrl,kb_o );
    newOption->add_menu_option("Browse Projects",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/binoculars.png") );
    newOptionLayer2 = MAIN_TOOLBAR_RECENT_PROJECTS = newOption->add_menu_option("Recent Projects",-1,NULL,-1,NULL,true);
    GPE_MAIN_GUI->update_recent_project_list(false);
    //Adds the recent projects to the list

    //Adds the recent files to the list
    newOption->add_menu_option("Save Project",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/save.png"),-1,NULL,false,true,false,kb_ctrl,kb_s  );
    newOption->add_menu_option("Save All Projects",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/save.png"),-1,NULL,false,true,false,kb_ctrl,kb_s,kb_shift );
    /*
    newOption->add_menu_option("Export",-1,-1,NULL,false);
    newOption->add_menu_option("Import Resources");
    newOption->add_menu_option("Export Resources");
    newOption->add_menu_option("Properties",-1,5,NULL,true);
    */
    newOption->add_menu_option("Quit Editor",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/truck.png"),76,NULL,false,true,false,kb_ctrl,kb_q);


    /*
    newOption = mainToolBar->add_menu_option("Edit");
    newOption->add_menu_option("Undo",-1,-1,NULL,false);
    newOption->add_menu_option("Redo",-1,-1,NULL,false);
    newOption->add_menu_option("Cut",-1,-1,NULL,false);
    newOption->add_menu_option("Copy",-1,-1,NULL,false,false);
    newOption->add_menu_option("Paste",-1,-1,NULL,false);
    */

    newOption = mainToolBar->add_menu_option("View");
    //newOption->add_menu_option("Reset View",-1,-1,NULL,false);

    newOption->add_menu_option("Toggle Fullscreen Mode",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/crop.png"),-1,NULL,false,true,false, kb_f11);
    if( GPE_DOCK!=NULL)
    {
        newOption->add_option( GPE_DOCK->toolbarOptonsHolder );
    }

    newOptionLayer2 = newOption->add_menu_option("Text Area Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/cogs.png"),-1,NULL,false,false);
    newOptionLayer2->add_menu_option("Toggle Line Count",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    newOptionLayer2->add_menu_option("Toggle Syntax Highlighting",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    //newOptionLayer2->add_menu_option("",-1,-1,NULL,false);
    newOption->add_menu_option("Start Page",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/cube.png"),-1,NULL,false);
    newOption->add_menu_option("Tip of the Day",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/info.png"),-1,NULL,false);

    //newOption = mainToolBar->add_menu_option("Tools");
    //newOption = mainToolBar->add_menu_option("Resources");

    newOption = mainToolBar->add_menu_option("Search");
    newOption->add_menu_option("Find...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_f);
    newOption->add_menu_option("Find in files...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_f,kb_shift);
    newOption->add_menu_option("Replace...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_r);
    newOption->add_menu_option("Replace in files....",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_r,kb_shift);
    newOption->add_menu_option("Goto Line...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_g);
    newOption->add_menu_option("Find Project Resource...",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/search.png"),-1,NULL,false);

    newOption = mainToolBar->add_menu_option("Import");
    for( int tempResType = RESOURCE_TYPE_SPRITE; tempResType <= RESOURCE_TYPE_FONT; tempResType++ )
    {
        if( (int)RESOURCE_TYPE_NAMES[tempResType].size() > 2 )
            newOption->add_menu_option("Import "+RESOURCE_TYPE_NAMES[tempResType]);
    }

    newOption = mainToolBar->add_menu_option("Build");
    // HTML5/ Web platforms

    newOption->add_menu_option("Build HTML5 GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/html5.png"),-1,NULL,true,true,false,kb_ctrl,kb_f5);
    newOption->add_menu_option("RUN HTML5 GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play-circle.png"),-1,NULL,true,true,false,kb_f8);
    newOption->add_menu_option("Build HTML5 GAME & RUN",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png"),-1,NULL,true,true,false,kb_f5);
    //desktop / laptop platforms
    if( GPE_FOUND_OS==GPE_IDE_WINDOWS)
    {
        newOption->add_menu_option("Build WINDOWS GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build OSX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build LINUX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,NULL,true,true );
    }
    else if( GPE_FOUND_OS==GPE_IDE_MAC)
    {
        newOption->add_menu_option("Build OSX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build LINUX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,NULL,true,true );
        newOption->add_menu_option("Build WINDOWS GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,NULL,false,true );
    }
    else
    {
        newOption->add_menu_option("ExpBuildort LINUX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,NULL,true,true );
        newOption->add_menu_option("Build WINDOWS GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build OSX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,NULL,false,true );
    }
    /*
    newOption->add_menu_option("Compile Current Tab",-1);
    newOption->add_menu_option("OUYA- P2",-1,5,mainExportOptionsSprite,false,false);
    */

    //console platforms
    /*newOptionLayer2 =newOption->add_menu_option("Build Nintendo WiiU GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/nintendo_wiiu_logo.png"),-1,NULL,true,true);
    newOptionLayer2->renderWhite = true;
    newOptionLayer2 =newOption->add_menu_option("Build Nintendo Switch GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/nintendo_switch_logo.png"),-1,NULL,true,true);
    newOptionLayer2->renderWhite = true;*/
    /*
    newOption->add_menu_option("Build Xbox One GAME",-1,3,mainExportOptionsSprite,true,false);
    */

    /*
    //handheld consoles
    newOptionLayer2 = newOption->add_menu_option("Handheld Platforms (Coming Soon)",-1);
    newOptionLayer2->add_menu_option("GCW-Zero- P3",-1,8,mainExportOptionsSprite,false);
    newOptionLayer2->add_menu_option("Nintendo 3DS & New 3DS- P3",-1,9,mainExportOptionsSprite,false,false);
    newOptionLayer2->add_menu_option("Open Pandora- P3",-1,10,mainExportOptionsSprite,false,false);
    newOptionLayer2->add_menu_option("Playstation Vita- P3",-1,11,mainExportOptionsSprite,false,false);

    //mobile platforms
    newOptionLayer2 = newOption->add_menu_option("Mobile Platforms(Coming Soon)",-1);
    newOptionLayer2->add_menu_option("Android - P4",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Blackberry - P5",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("iPhones - P5",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Tizen Phones - P6",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Ubuntu Phones - P6",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Windows Phones - P4",-1,11,NULL,false,false);
    */

    newOptionLayer2 = newOption->add_menu_option("Clean Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [HTML5] Build Folder",-1);
    if( GPE_MAIN_GUI->includeNintendoWiiUExport )
        newOptionLayer2->add_menu_option("Clean [WiiU] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [Windows] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [Linux] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [OSX] Build Folder",-1);
    //newOptionLayer2->add_menu_option("Clean [HTML5] Build Folder",-1,-1,mainExportOptionsSprite,false);

    newOption = mainToolBar->add_menu_option("Tools");
    newOption->add_menu_option("Game Controller Tester",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/gamepad.png"),-1,NULL,false,true,false);
    newOption->add_menu_option("Extra Tools",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/briefcase.png"),-1,NULL,false,true,false);

    newOption = mainToolBar->add_menu_option("Settings");
    newOption->add_menu_option("User Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/cog.png"),-1, NULL,false,true,false,kb_f2);
    newOption->add_menu_option("JS Compiler Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/code.png"),-1,NULL,false,true,false);
    //C++ Settings
    newOption->add_menu_option("C++ Compiler Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/code.png"),-1,NULL,false,true,false);

    newOption = mainToolBar->add_menu_option("Help" );
    //newOption->add_menu_option("Online Documentation",-1,17,mainMenuSprite,false);
    //newOption->add_menu_option("Tips",-1,20,mainMenuSprite,false);
    //newOption->add_menu_option("Forums",-1);
    newOption->add_menu_option("General Help",-1,NULL,-1,NULL,false,true,false,kb_f1);


    newOption->add_menu_option("Online Documentation",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/book.png"),-1,NULL,false);
    newOption->add_menu_option("Online Forums",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/group.png"),-1,NULL,false);
    newOption->add_menu_option("Community Chat",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/comments.png"),-1,NULL,false);
    newOption->add_menu_option("Tip of the Day",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/infO.png"),-1,NULL,false);
    newOption->add_menu_option("Report Issue",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/bug.png"),-1,NULL,false);
    newOption->add_menu_option("Check Updates",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/question-circle.png"),-1,NULL,false);
    newOption->add_menu_option("License",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/file-text.png"),-1,NULL,false);
    newOption->add_menu_option("About Game Pencil Engine",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/pencil.png"),-1,NULL,false,true,false,kb_ctrl,kb_b);
    //newOption->add_menu_option("Licenses",-1,27,mainMenuSprite,false);
    GPE_Main_TabManager->set_coords(-1,mainButtonBar->get_y2pos()+GENERAL_GPE_PADDING );
    GPE_MAIN_GUI->init_resourcebar();
    //main resource bar elements

    country_language_image = new GPE_Texture();
    if(country_language_image!=NULL)
    {
        //country_language_image->load_new_texture( APP_DIRECTORY_NAME+"resources/gfx/flags/24/us.png",-1,true);
    }

    //Load the background
    background = NULL;

    //Render the intro message
    message = NULL;
    if( GPE_MAIN_GUI!=NULL)
    {
        GPE_MAIN_GUI->load_settings();
        if( GPE_MAIN_GUI->showStartPageAtStartUp)
        {
            GPE_Main_TabManager->add_new_tab(MAIN_START_PAGE);
            MAIN_START_PAGE->set_name("Start Page");
        }

        if( GPE_MAIN_GUI->showTipsAtStartUp)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            input->handle_input(true);
            process_input();
            input->reset_all_input();
            apply_logic();
            render();
            MAIN_RENDERER->update_renderer();
            if( GPE_MAIN_GUI->showTipsAtStartUp)
            {
                GPE_Show_Tip_Of_Day();
            }
        }


        /*
        GPE_ProjectFolder * newProj = new GPE_ProjectFolder("New Project","here/","newproject.tgif");
        GPE_MAIN_GUI->gpeProjects.push_back(newProj);
        GPE_ProjectFolder * newExampleProj = new GPE_ProjectFolder("Examples","here/","examples.tgif");
        GPE_MAIN_GUI->gpeProjects.push_back(newExampleProj);
        */
    }
}

GPE_Editor_State::~GPE_Editor_State()
{
    //Free the surfaces
    delete background;
    delete message;
    if( GPE_MAIN_GUI!=NULL)
    {
        delete GPE_MAIN_GUI;
        GPE_MAIN_GUI = NULL;
    }
}

void GPE_Editor_State::process_input()
{
    if( input->check_mouse_released( mb_anybutton ) )
    {
        GPE_MAIN_HIGHLIGHTER->clear_suggestions();
    }
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    //if( MAIN_RENDERER->window_changed() )
    {
        //updates toolbars and whatnots info
        SCREEN_WIDTH = MAIN_RENDERER->get_window_width();
        SCREEN_HEIGHT= MAIN_RENDERER->get_window_height();
        reset_camera();
        mainToolBar->set_width(SCREEN_WIDTH);
        //mainButtonBar->set_width(SCREEN_WIDTH);
        mainButtonBar->set_coords( 0,32 );
        if( MAIN_EDITOR_SETTINGS!=NULL)
        {
            if( MAIN_EDITOR_SETTINGS->ideButtonBarSize!=NULL)
            {
                int foundButtonSize =  MAIN_EDITOR_SETTINGS->ideButtonBarSize->get_selected_value();
                mainButtonBar->set_height( foundButtonSize );
                if( MAIN_EDITOR_SETTINGS->ideButtonBarAlignment !=NULL && foundButtonSize > 0)
                {
                    int foundButtonBarAlignment = MAIN_EDITOR_SETTINGS->ideButtonBarAlignment->get_selected_value();
                    if( foundButtonBarAlignment==1)
                    {
                        mainButtonBar->set_coords( (SCREEN_WIDTH-mainButtonBar->get_width() )/2, 32 );
                    }
                    else if( foundButtonBarAlignment==2)
                    {
                        mainButtonBar->set_coords( (SCREEN_WIDTH-mainButtonBar->get_width() ), 32 );
                    }
                    else
                    {
                        mainButtonBar->set_coords( 0, 32 );
                    }
                }
                else
                {
                    mainButtonBar->set_coords( 0, 24 );
                }
            }
        }

        GPE_Main_Statusbar->set_coords(0,SCREEN_HEIGHT-24 );
        GPE_Main_Statusbar->set_height( 24 );
        GPE_Main_Statusbar->set_width(SCREEN_WIDTH);

        if( GPE_DOCK!=NULL )
        {
            GPE_DOCK->set_coords( 0,mainButtonBar->get_y2pos() + GENERAL_GPE_PADDING);
            GPE_DOCK->set_width( SCREEN_WIDTH );
            GPE_DOCK->set_height( SCREEN_HEIGHT-GPE_DOCK->get_ypos() - GPE_Main_Statusbar->get_height() );
            GPE_DOCK->setup_dock();
        }
    }
}

void GPE_Editor_State::apply_logic()
{
    GPE_MAIN_GUI->apply_logic();

    if( !mainToolBar->is_open() && !MAIN_CONTEXT_MENU->is_open() )
    {
        if( mainButtonBar!=NULL )
        {
            mainButtonBar->process_self();
            if( mainButtonBar->selectedOption>= 0 )
            {
                switch( mainButtonBar->selectedOption)
                {
                case 1:
                    GPE_MAIN_GUI->launch_new_project();
                    break;
                case 2:
                    GPE_MAIN_GUI->open_new_project();
                    break;

                case 3:
                    GPE_MAIN_GUI->save_current_project();
                    break;

                case 4:
                    GPE_MAIN_GUI->save_all_projects();
                    break;

                case 5:
                    if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
                    {
                        GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
                        projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
                        tProjectProps->projectSettingsBar->set_selected_option("Platforms");
                    }
                    break;

                case 6:
                    if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
                    {
                        CURRENT_PROJECT->export_and_play_native();
                    }
                    break;
                default:

                    break;
                }
            }
        }
    }
    MAIN_OVERLAY->process_cursor();
    /*
    //language tooltip
    //place elsewhere once language support is added
    if( point_between(input->mouse_x,input->mouse_y,SCREEN_WIDTH-32,0,SCREEN_WIDTH,32) )
    {
        MAIN_OVERLAY->update_tooltip(input->mouse_x,input->mouse_y+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING,"Language Settings: American English! USA! ",FA_RIGHT);
    }
    */
}

void GPE_Editor_State::clean_up()
{

}

void GPE_Editor_State::render()
{
    bool forceRedraw = false;
    if( input->input_received() )
    {
        forceRedraw = true;
    }
    else if( MAIN_EDITOR_SETTINGS!=NULL)
    {
        if( MAIN_EDITOR_SETTINGS->forceFrameRedraw!=NULL && MAIN_EDITOR_SETTINGS->forceFrameRedraw->is_clicked() )
        {
            forceRedraw = true;
        }
    }
    /*else if( GPE_FOUND_OS==GPE_IDE_LINUX )
    {
        gcanvas->render_rectangle( 0,0,SCREEN_WIDTH,SCREEN_HEIGHT,c_black,false,255);
         forceRedraw = true;
    }*/

    if( forceRedraw )
    {
        gcanvas->render_rectangle( 0,0,SCREEN_WIDTH,SCREEN_HEIGHT,c_black,false,255);
        GPE_MAIN_THEME->render_background(NULL, NULL);

        if( mainButtonBar!=NULL  )
        {
            //gcanvas->render_rectangle( 0,0,SCREEN_WIDTH,GPE_Main_TabManager->get_ypos(),GPE_MAIN_THEME->Program_Header_Color,false);
            mainButtonBar->render_self( &GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA,forceRedraw);
        }
    }
    GPE_MAIN_GUI->render_foreground_engine(forceRedraw);
    //for( int i = 0; i < 100; i++)
    {
        gcanvas->render_triangle_color(input->mouse_x,input->mouse_y,input->mouse_x,input->mouse_y+256,input->mouse_x+256,input->mouse_y-360,c_green, 128, false );
    }
    //MAIN_RENDERER->update_renderer();
    /*
    //Pending language support
    if(country_language_image!=NULL)
    {
        render_texture(country_language_image,SCREEN_WIDTH-32,0,NULL,NULL);
    }
    */
    input->forceRedrawNeeded  = false;
}

int modify_folder_images(std::string folderLocation, int modifcationType)
{
    int imagesModifiedCount = -1;
    if( path_exists(folderLocation) && modifcationType >=0 && modifcationType < 3)
    {
        GPE_FileDirectory * foundFolder = new GPE_FileDirectory();
        foundFolder->open_directory_sorted(folderLocation);

        GPE_Color * colorToRemove = c_fuchsia->duplicate_color();
        bool continueWithAction = false;
        imagesModifiedCount = 0;
        if( foundFolder->get_count() > 0 )
        {
            if( modifcationType==0)
            {
                if( GPE_Change_Color_PopUp("Image Background Color To Remove",colorToRemove) )
                {
                    if( GPE_Display_Basic_Prompt("Are you sure you want to erase this Color from all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                    {
                        continueWithAction = true;
                    }
                }
            }
            else if( modifcationType==1 )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you want to invert your all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                {
                    continueWithAction = true;
                }
            }
            else if( modifcationType==2 )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you want to grayscale all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                {
                    continueWithAction = true;
                }
            }
            if( continueWithAction)
            {
                std::string newImageName = "";
                GPE_File * tempFile = NULL;
                SDL_Surface * oTempSurface = NULL;
                SDL_Surface *nTempSurface = NULL;
                for( int i = 0; i < foundFolder->get_count(); i++)
                {
                    tempFile = foundFolder->get_file(i);
                    if( tempFile!=NULL)
                    {
                        if( tempFile->get_type()=="bmp" || tempFile->get_type()=="png")
                        {
                            newImageName = folderLocation+"/"+tempFile->get_name();
                            oTempSurface = gpe_sdl->load_surface_image( newImageName );
                            if( oTempSurface->w > 0 && oTempSurface->h > 0)
                            {
                                nTempSurface = NULL;
                                if( oTempSurface!=NULL)
                                {
                                    GPE_Report("Modifying image at: "+newImageName+".");

                                    if( GPE_LOADER != NULL )
                                    {
                                        GPE_LOADER->update_messages( "Modifying Image", tempFile->get_name(),"Please wait..." );
                                    }

                                    if( modifcationType==0)
                                    {
                                        nTempSurface= gpe_sdl->surface_remove_color(oTempSurface,colorToRemove->get_sdl_color() );
                                    }
                                    else if( modifcationType==1 )
                                    {
                                        nTempSurface= gpe_sdl->surface_invert(oTempSurface);
                                    }
                                    else if( modifcationType==2 )
                                    {
                                        nTempSurface= gpe_sdl->surface_grayscale(oTempSurface);
                                    }
                                    if( nTempSurface!=NULL)
                                    {
                                        if( tempFile->get_type()=="bmp" || tempFile->get_type()=="png")
                                        {
                                            SDL_SaveBMP(nTempSurface,newImageName.c_str() );
                                        }
                                        else if( tempFile->get_type()=="png")
                                        {
                                            IMG_SavePNG(nTempSurface,newImageName.c_str() );
                                        }
                                        SDL_FreeSurface(nTempSurface);
                                        nTempSurface = NULL;
                                        imagesModifiedCount++;
                                    }
                                    SDL_FreeSurface(oTempSurface);
                                    oTempSurface = NULL;
                                }
                            }
                        }
                    }
                }
            }
        }
        foundFolder->close_directory();
        delete foundFolder;
        if( colorToRemove!=NULL)
        {
            delete colorToRemove;
            colorToRemove = NULL;
        }
    }
    return imagesModifiedCount;
}



bool GPE_Editor_Init( int argc, char* args[] )
{
    GPE_Report("Starting GPE Editor...");
    GPE_Editor_Init_Globals();
    GPE_Report( "Starting GUI..." );
    if( !PAW_GUI_START() )
    {
        GPE_Report("Unable to start Paw_GUI LIBRARY!\n");
        return false;
    }
    GPE_MAIN_GUI = new GPE_Gui_Engine();
    //If everything initialized fine
    GPE_Main_TabManager = new GPE_TabManager();

    MAIN_EDITOR_SETTINGS = new gamePencilEditorSettingsResource();
    MAIN_EDITOR_SETTINGS->set_global_rid(-1);
    MAIN_EDITOR_SETTINGS->set_name("User Settings");
    MAIN_EDITOR_SETTINGS->preprocess_self();

    MAIN_ABOUT_PAGE = new gamePencilAboutPageResource();
    MAIN_ABOUT_PAGE->set_global_rid(-2);
    MAIN_ABOUT_PAGE->set_name("About...");

    MAIN_HELP_PAGE = new gamePencilHelpPageResource();
    MAIN_HELP_PAGE->set_global_rid(-3);
    MAIN_HELP_PAGE->set_name("Help...");


    MAIN_START_PAGE = new gamePencilStartPageResource();
    MAIN_START_PAGE->set_global_rid(-4);
    MAIN_START_PAGE->set_name("Start Page");

    MAIN_EXTRA_TOOLS = new gamePencilExtraTools();
    MAIN_EXTRA_TOOLS->set_global_rid(-5);
    MAIN_EXTRA_TOOLS->set_name("Extra Tools");

    GPE_JS_COMPILER_SETTINGS = new gameJSCompilerSettingsResource();
    GPE_JS_COMPILER_SETTINGS->set_global_rid(-6);
    GPE_JS_COMPILER_SETTINGS->set_name("JS Compiler Settings");

    GPE_CPP_COMPILER_SETTINGS = new gameCPPCompilerSettingsResource();
    GPE_CPP_COMPILER_SETTINGS->set_global_rid(-7);
    GPE_CPP_COMPILER_SETTINGS->set_name("C++ Compiler Settings");

    MAIN_GameController_TESTER = new gamePencilgamecontrollerTesterResource();
    MAIN_GameController_TESTER->set_global_rid(-8);
    MAIN_GameController_TESTER->set_name("Game Controller Tester");

    //GPE_Main_TabManager->add_new_tab(MAIN_START_PAGE);

    GPE_Main_Statusbar = new GPE_StatusBar();
    GPE_Main_Statusbar->set_coords(0,0);
    GPE_Main_Statusbar->set_width(SCREEN_WIDTH);
    GPE_Main_Statusbar->set_height(24);

    GPE_Main_Logs = new GPE_LogManager();
    GPE_Main_Logs->set_coords(0,SCREEN_HEIGHT-64);
    GPE_Main_Logs->set_width(SCREEN_WIDTH);
    GPE_Main_Logs->set_height(32);

    GPE_MAIN_HIGHLIGHTER = new GPE_Syntax_Highlighter();
    GPE_Report("Adding Docking System...");
    GPE_DOCK = new gpeEditorDock();
    GPE_DOCK->add_default_panel("Editor",DOCK_TOP_LEFT, true );
    GPE_DOCK->add_default_panel("Inspector",DOCK_BOTTOM_LEFT, true );
    GPE_DOCK->add_default_panel("Resources",DOCK_TOP_RIGHT, true );
    GPE_DOCK->add_default_panel("Tilesheet",DOCK_BOTTOM_RIGHT, true );
    GPE_DOCK->add_default_panel("Grid Settings",DOCK_BOTTOM_RIGHT, true );
    GPE_DOCK->reset_dock();
    GPE_Report("Docking System added...");

    spm = new GPE_SceneEditorHelper();
    gpe->set_fps( FPS_CAP );

    GPE_NextProgramState = STATE_NULL;
    GPE_ProgramStateId = STATE_GPE_EDITOR;
    GPE_IS_LOADING = false;
    currentState = new GPE_Editor_State();

    //Opens projects based on args[0] being the application name.
    if( argc > 1)
    {
        for( int iArgc = 1; iArgc <argc; iArgc++ )
        {
            GPE_MAIN_GUI->open_project( args[iArgc] );
            GPE_Report( args[iArgc] );
        }
    }
    else if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->launchLastProjectBox!=NULL )
    {
        if( MAIN_EDITOR_SETTINGS->launchLastProjectBox->is_clicked() )
        {
            int iRPList = 0;
            int maxRPList = GPE_MAIN_GUI->get_recent_project_list_size();
            if( maxRPList > 0 )
            {
                if( file_exists(GPE_MAIN_GUI->get_recent_project_name(0) ) )
                {
                    GPE_MAIN_GUI->open_project( GPE_MAIN_GUI->get_recent_project_name(0) );
                }
            }
        }
    }

    gpe->set_fps( FPS_CAP );
    return true;
}

void GPE_Editor_Init_Globals()
{
    //Let's start off with the build names
    for(int bI = 0; bI < GPE_BUILD_OPTIONS; bI++)
    {
        GPE_BUILD_NAMES[bI] = "";
    }
    GPE_BUILD_NAMES[GPE_BUILD_HTML5] = "HTML5";
    GPE_BUILD_NAMES[GPE_BUILD_WINDOWS] = "WINDOWS";
    GPE_BUILD_NAMES[GPE_BUILD_MAC] = "OSX";
    GPE_BUILD_NAMES[GPE_BUILD_LINUX] = "LINUX";
    GPE_BUILD_NAMES[GPE_BUILD_WIIU] = "WIIU";
    GPE_BUILD_NAMES[GPE_BUILD_XBOXONE] = "XBOXONE";
    GPE_BUILD_NAMES[GPE_BUILD_PS4] = "PS4";
    GPE_BUILD_NAMES[GPE_BUILD_PSVITA] = "PSVITA";
    GPE_BUILD_NAMES[GPE_BUILD_ANDROID] = "ANDROID";
    GPE_BUILD_NAMES[GPE_BUILD_IOS] = "IOS";
    GPE_BUILD_NAMES[GPE_BUILD_WINDOWS_PHONE] = "WINDOWS PHONE";

    //Sprite Labels we will use
    spriteDataLabels[sprFrameCount] = "Frame Count";
    spriteDataLabels[sprFrameWidth] = "Frame Width";
    spriteDataLabels[sprFrameHeight] = "Frame Height";
    spriteDataLabels[sprHPixelOffset] = "Horizontal Pixel  Offset";
    spriteDataLabels[sprVPixelOffset] = "Vertical Pixel Offset";
    spriteDataLabels[sprHPixelPadding] = "Horizontal Frame Padding";
    spriteDataLabels[sprVPixelPadding] = "Vertictal Frame Padding";

    //Tilesheet labels we will use
    tsDataLabels[0] = "Width";
    tsDataLabels[1] = "Height";
    tsDataLabels[2] = "X-Offset";
    tsDataLabels[3] = "Y-Offset";
    tsDataLabels[4] = "H-Padding";
    tsDataLabels[5] = "Y-Padding";

    //Font Popup variables
    POPUP_FONT_SIZE_WIDTH = 12;
    POPUP_FONT_SIZE_HEIGHT = 12;
    if( FONT_POPUP!=NULL)
    {
        FONT_POPUP->get_metrics("A",&POPUP_FONT_SIZE_WIDTH,&POPUP_FONT_SIZE_HEIGHT);
    }
    //Object names
    DEFAULT_OBJECT_FUNCTIONS_NAMES[0] = "Constructor";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[1] = "Destructor";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[2] = "Start Frame";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[3] = "Main Logic";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[4] = "Sprite Done";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[5] = "End Frame";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[6] = "PRE-Render";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[7] = "Render";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[8] = "POST-Render";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[9] = "Render HUD";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[10] = "";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[11] = "";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[12] = "";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[13] = "";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[14] = "";
    DEFAULT_OBJECT_FUNCTIONS_NAMES[15] = "";

    DEFAULT_OBJECT_SPECIALS_NAMES[0] = "Scene Start";
    DEFAULT_OBJECT_SPECIALS_NAMES[1] = "Scene End";
    DEFAULT_OBJECT_SPECIALS_NAMES[2] = "Game Start";
    DEFAULT_OBJECT_SPECIALS_NAMES[3] = "Game Exit";
    DEFAULT_OBJECT_SPECIALS_NAMES[4] = "Player Died";
    DEFAULT_OBJECT_SPECIALS_NAMES[5] = "Game Over";

    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[0] = "Constructor";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[1] = "self_obliterate";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[2] = "start_frame_logic";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[3] = "perform_object_logic";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[4] = "animation_concluded";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[5] = "end_frame_logic";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[6] = "render_under";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[7] = "render_self";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[8] = "render_above";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[9] = "render_hud";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[10] = "";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[11] = "";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[12] = "";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[13] = "";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[14] = "";
    DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[15] = "";


    DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[0] = "scene_start";
    DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[1] = "scene_end";
    DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[2] = "game_start";
    DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[3] = "game_exit";
    DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[4] = "player_died";
    DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[5] = "game_over";

    //Scene globals
    DEFAULT_SCENE_SUBEDITOR_NAMES[0] = "Settings";
    DEFAULT_SCENE_SUBEDITOR_NAMES[1] = "Layers";
    DEFAULT_SCENE_SUBEDITOR_NAMES[2] = "Extras";
    DEFAULT_SCENE_SUBEDITOR_NAMES[3] = "Extras";
    DEFAULT_SCENE_SUBEDITOR_NAMES[3] = "Extras";

    //Audio type names
    SUPPORTED_AUDIO_EXT[aacFileName] = "AAC";
    SUPPORTED_AUDIO_EXT[mp3FileName] = "MP3";
    SUPPORTED_AUDIO_EXT[oggFileName] = "OGG";
    SUPPORTED_AUDIO_EXT[wavFileName] = "WAV";

    //Video type names
    SUPPORTED_VIDEO_EXT[0] = "MP4";
    SUPPORTED_VIDEO_EXT[1] = "WEBM";
    SUPPORTED_VIDEO_EXT[2] = "OGG";

    //Font type names
    SUPPORTED_FONT_EXT[FONT_EOT]  = "EOT";
    SUPPORTED_FONT_EXT[FONT_SVG]  = "SVG";
    SUPPORTED_FONT_EXT[FONT_OTF] = "OTF";
    SUPPORTED_FONT_EXT[FONT_TTF]  = "TTF";
    SUPPORTED_FONT_EXT[FONT_WOFF] = "WOFF";
    SUPPORTED_FONT_EXT[FONT_WOFF2] = "WOFF2";
}

bool GPE_Editor_Quit()
{
    GPE_Report("Closing all tabs....");
    if( GPE_Main_TabManager!=NULL)
    {
        GPE_Main_TabManager->close_all_tabs();
    }
    GPE_Report("Deleting Settings....");
    if( MAIN_EDITOR_SETTINGS!=NULL)
    {
        MAIN_EDITOR_SETTINGS->save_resource();
        delete MAIN_EDITOR_SETTINGS;
        MAIN_EDITOR_SETTINGS = NULL;
    }
    GPE_Report("Deleting about page....");
    if( MAIN_ABOUT_PAGE!=NULL)
    {
        delete MAIN_ABOUT_PAGE;
        MAIN_ABOUT_PAGE = NULL;
    }
    GPE_Report("Deleting help page....");
    if( MAIN_HELP_PAGE!=NULL)
    {
        delete MAIN_HELP_PAGE;
        MAIN_HELP_PAGE = NULL;
    }
    GPE_Report("Deleting start page....");
    if( MAIN_START_PAGE!=NULL)
    {
        delete MAIN_START_PAGE;
        MAIN_START_PAGE = NULL;
    }
    GPE_Report("Deleting extra tools....");
    if( MAIN_EXTRA_TOOLS!=NULL)
    {
        delete MAIN_EXTRA_TOOLS;
        MAIN_EXTRA_TOOLS = NULL;
    }
    GPE_Report("Deleting context menu....");
    if( MAIN_CONTEXT_MENU!=NULL)
    {
        delete MAIN_CONTEXT_MENU;
        MAIN_CONTEXT_MENU = NULL;
    }
    GPE_Report("Deleting toolbar recent projects....");
    if( MAIN_TOOLBAR_RECENT_PROJECTS!=NULL)
    {
        delete MAIN_TOOLBAR_RECENT_PROJECTS;
        MAIN_TOOLBAR_RECENT_PROJECTS = NULL;
    }

    GPE_Report("Deleting gui....");
    if( GPE_MAIN_GUI!=NULL)
    {
        GPE_MAIN_GUI->save_settings();
        delete GPE_MAIN_GUI;
        GPE_MAIN_GUI = NULL;
    }
}

void GPE_Show_Tip_Of_Day()
{
    if( GPE_MAIN_GUI!=NULL && MAIN_RENDERER!=NULL )
    {
        gpe->end_loop();
        RESOURCE_TO_DRAG = NULL;
        GPE_Report("Showing tip of the day");
        std::string popUpCaption = "Tip of the Day";
        GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
        MAIN_OVERLAY->process_cursor();
        GPE_MAIN_GUI->reset_gui_info();
        MAIN_OVERLAY->take_frozen_screenshot( );

        int promptBoxWidth = 544;
        int promptBoxHeight = 384;
        GPE_Rect elementBox;

        bool exitOperation = false;
        input->reset_all_input();
        int currentTipId = GPE_MAIN_GUI->get_random_tip();
        if( currentTipId < 0 || currentTipId > GPE_MAIN_GUI->get_tip_count() )
        {
            currentTipId = 0;
        }
        std::string currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);

        GPE_Label_Title * doYouKnowLabel = new GPE_Label_Title("Do you know?","Do you know?");
        GPE_GuiElementList * showTipList = new GPE_GuiElementList();
        showTipList->hideXScroll = true;
        showTipList->hideYScroll = true;
        GPE_ToolLabelButton * closeButton = new GPE_ToolLabelButton( "Close","");
        GPE_ToolLabelButton * nextTipButton = new GPE_ToolLabelButton( "Next Tip","");
        GPE_ToolLabelButton * previousTipButton = new GPE_ToolLabelButton( "Previous Tip","");
        GPE_CheckBoxBasic * showAtStartUpButton = new GPE_CheckBoxBasic("Show tips at startup","Unclick to not see this popup automatically on start");
        showAtStartUpButton->set_clicked( GPE_MAIN_GUI->showTipsAtStartUp);
        GPE_ToolLabelButton * randomTipButton = new GPE_ToolLabelButton( "Random Tip","");
        GPE_WrappedTextArea * tipParagraph = new GPE_WrappedTextArea();
        tipParagraph->set_string(currentTipString);
        tipParagraph->set_width(512-GENERAL_GPE_PADDING*2);
        tipParagraph->set_height(96);
        MAIN_RENDERER->reset_viewpoint();
        //MAIN_OVERLAY->render_frozen_screenshot( );
        while(exitOperation==false)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            //GPE_Report("Processing tip of the day");
            gpe->start_loop();

            elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2;
            elementBox.y = (SCREEN_HEIGHT-promptBoxHeight)/2;
            elementBox.w = promptBoxWidth;
            elementBox.h = promptBoxHeight;
            showTipList->set_coords(elementBox.x, elementBox.y+32);
            showTipList->set_width(elementBox.w);
            showTipList->set_height(elementBox.h-32);
            showTipList->barXMargin = GENERAL_GPE_PADDING;
            showTipList->barYMargin = GENERAL_GPE_PADDING;
            showTipList->barXPadding = GENERAL_GPE_PADDING*2;
            showTipList->barYPadding = GENERAL_GPE_PADDING*2;

            GPE_MAIN_GUI->reset_gui_info();
            showTipList->clear_list();
            showTipList->add_gui_element(doYouKnowLabel,true);
            showTipList->add_gui_element(tipParagraph,true);
            showTipList->add_gui_element(showAtStartUpButton, true);
            showTipList->add_gui_element(previousTipButton,false);
            showTipList->add_gui_element(nextTipButton,true);
            showTipList->add_gui_element(randomTipButton,false);
            showTipList->add_gui_element(closeButton,false);
            showTipList->process_self( NULL, NULL );

            if( input->check_keyboard_released(kb_esc) || closeButton->is_clicked() || WINDOW_WAS_JUST_RESIZED )
            {
                exitOperation = true;
            }
            else if( nextTipButton->is_clicked() )
            {
                currentTipId = GPE_MAIN_GUI->get_next_tip(currentTipId);
                currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }
            else if( previousTipButton->is_clicked() )
            {
                currentTipId = GPE_MAIN_GUI->get_previous_tip(currentTipId);
                currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }
            else if( randomTipButton->is_clicked() )
            {
                currentTipId = GPE_MAIN_GUI->get_random_tip();
                currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }

            //GPE_Report("Rendering tip of the day");
            MAIN_RENDERER->reset_viewpoint();
            if( !WINDOW_WAS_JUST_RESIZED)
            {
                //if( input->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot( );
                }
                //Update screen
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Main_Box_Color,false);

                gcanvas->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Color,false);
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,true);
                gfs->render_text( elementBox.x+elementBox.w/2,elementBox.y+GENERAL_GPE_PADDING,popUpCaption,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
                showTipList->render_self( NULL, NULL );
                //GPE_MAIN_GUI-render_gui_info(  true);

                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
                MAIN_OVERLAY->process_cursor();
                GPE_MAIN_GUI->render_gui_info(  true);
            }
            gpe->end_loop();
        }

        if( showTipList!=NULL)
        {
            delete showTipList;
            showTipList = NULL;
        }

        if( doYouKnowLabel!=NULL)
        {
            delete doYouKnowLabel;
            doYouKnowLabel = NULL;
        }
        if( closeButton!=NULL)
        {
            delete closeButton;
            closeButton = NULL;
        }
        if( nextTipButton!=NULL)
        {
            delete nextTipButton;
            nextTipButton = NULL;
        }
        if( previousTipButton!=NULL)
        {
            delete previousTipButton;
            previousTipButton = NULL;
        }
        if( randomTipButton!=NULL)
        {
            delete randomTipButton;
            randomTipButton = NULL;
        }
        if( showAtStartUpButton!=NULL)
        {
            GPE_MAIN_GUI->showTipsAtStartUp = showAtStartUpButton->is_clicked();
            GPE_MAIN_GUI->save_settings();
            delete showAtStartUpButton;
            showAtStartUpButton = NULL;
        }
        if( tipParagraph!=NULL)
        {
            delete tipParagraph;
            tipParagraph = NULL;
        }
        input->reset_all_input();
        MAIN_OVERLAY->render_frozen_screenshot( );
        MAIN_RENDERER->update_renderer();
    }
}
