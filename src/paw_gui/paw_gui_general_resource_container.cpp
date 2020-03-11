/*
paw_gui_general_resource_container.cpp
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

#include "paw_gui_general_resource_container.h"
#include "paw_gui_parsers.h"
#include "paw_gui_popups.h"
#include "paw_gui_tab_manager.h"

GPE_GeneralResourceContainer * RESOURCE_TO_DRAG = NULL;
GPE_GeneralResourceContainer * LAST_CLICKED_RESOURCE = NULL;
bool RESOURCEMENU_WAS_RIGHTCLICKED = false;
GPE_GeneralResourceContainer * RESOURCE_BEINGRENAMED = NULL;

int LAST_CLICKED_RESTYPE = -1;
int DRAGGED_RESTYPE = -1;
int lastResTypeRendered = -1;

std::string project_current_name = "";
std::string seeked_project_name = "";


GPE_GeneralResourceContainer::GPE_GeneralResourceContainer(std::string projFolderName, std::string new_name, int rType, int rId,bool folder, int globalIdVal,  int rezPropValue)
{
    foundX2Pos = 0;
    resouceNameChanged = false;
    subContentsModified = false;
    parentResource = NULL;
    projectParentFileName = projectParentName = projFolderName;
    opName = new_name;
    resourceType = rType;
    resourceId = rId;
    isFolder = folder;
    exportBuildGlobalId = -1;
    globalResouceId = -1;
    if(globalIdVal>=0)
    {
        globalResouceId = globalIdVal;
    }
    element_box.x = 0;
    element_box.y = 0;
    element_box.w = 0;
    element_box.h = RESOURCE_AREA_HEIGHT;
    containeranimation = NULL;
    containerTexture = NULL;
    animationFrameNumber = 0;
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
    gpe::error_log->report("Removing Resource named ["+opName+"].");
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Removing Resource", opName );
    }
    if( heldResource!=NULL)
    {
        gpe::error_log->report("Removing Held-Resource named ["+opName+"].");
        if( GPE_main_TabManager!=NULL)
        {
            GPE_main_TabManager->close_resource_tab( projectParentName,heldResource->globalResouceIdNumber );
        }
        gpe::error_log->report("Held-Resource deleted ["+opName+"].");
        delete heldResource;
        heldResource = NULL;
    }
    gpe::error_log->report("Held-Resource's sub-options deleted ["+opName+"].");
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

    if( containeranimation!=NULL)
    {
        //delete containeranimation;
        //containeranimation = NULL;
    }
    if( containerTexture!=NULL)
    {
        /*delete containerTexture;
        containerTexture = NULL;*/
    }
    subOptions.clear();
    gpe::error_log->report("Resource ["+opName+"] deleted.");

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
            element_box.h += newResource->element_box.h;
        }
        if( changeGlobalId)
        {
            int new_id = (int)subOptions.size()+1;
            newResource->set_global_id(new_id);
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
            if( newResource->get_resource_type()== gpe::resource_type_project_settings )
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
    int new_id = (int)subOptions.size()+1;
    GPE_GeneralResourceContainer * newFolder = new GPE_GeneralResourceContainer(projectParentFileName,resourceTypeName,resourceType,new_id,true, gResId, rezPropValue);
    newFolder->element_box.x = element_box.x;
    newFolder->element_box.y = element_box.y+new_id*RESOURCE_AREA_HEIGHT;
    //element_box.h+=RESOURCE_AREA_HEIGHT;
    newFolder->parentResource = this;
    subOptions.push_back(newFolder);
    if( subMenuIsOpen)
    {
        element_box.h += RESOURCE_AREA_HEIGHT;
    }
    return newFolder;
}

GPE_GeneralResourceContainer * GPE_GeneralResourceContainer::add_newtype_folder( int rType,std::string resourceTypeName, int gResId, int rezPropValue)
{
    int new_id = (int)subOptions.size()+1;
    GPE_GeneralResourceContainer * newFolder = new GPE_GeneralResourceContainer(projectParentFileName,resourceTypeName,rType,new_id,true, gResId,rezPropValue);
    newFolder->element_box.x = element_box.x;
    newFolder->element_box.y = element_box.y+new_id*RESOURCE_AREA_HEIGHT;
    //element_box.h+=RESOURCE_AREA_HEIGHT;
    newFolder->parentResource = this;
    subOptions.push_back(newFolder);
    element_box.h += RESOURCE_AREA_HEIGHT;
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
    int foundId = stg_ex::string_to_int(rNameToFind,-1);
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
    return animationFrameNumber;
}

gpe::animaton2d * GPE_GeneralResourceContainer::get_resource_animation()
{
    if( !isFolder && !isSuperProjectFolder && heldResource!=NULL )
    {
        return heldResource->get_resource_animation();
    }
    return NULL;
}

gpe::texture_base * GPE_GeneralResourceContainer::get_resource_texture()
{
    if( !isFolder && !isSuperProjectFolder )
    {
        gpe::texture_base * rTexture = NULL;
        if( heldResource!=NULL )
        {
            rTexture = heldResource->get_resource_texture();
            if( rTexture!=NULL )
            {
                return rTexture;
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

bool GPE_GeneralResourceContainer::include_local_files( std::string pBuildDir , int buildType )
{

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
                    gpe::error_log->report("Can not move["+otherContainer->get_name()+"] into ["+get_name()+"] folder.");
                }
            }
            else
            {
                gpe::error_log->report("Can not move["+otherContainer->get_name()+"] into ["+get_name()+"] folder[Different Folder Type].");
            }
        }
        else
        {
            gpe::error_log->report("Can not move super folder["+otherContainer->get_name()+".Illegal editor operation.");
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

void GPE_GeneralResourceContainer::preprocess_container(std::string file_path, int backupId)
{
    if( heldResource!=NULL)
    {
        heldResource->load_resource(file_path);
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

int GPE_GeneralResourceContainer::process_container(int xPos, int yPos, int selectedId, gpe::shape_rect * viewedSpace, gpe::shape_rect * cam, bool mouseInRange )
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
        if( mouseInRange && gpe::input->check_mouse_pressed(0) )
        {
            if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos) )
            {
                seeked_project_name = projectParentFileName;

                if( !isSuperFolder && !isSuperProjectFolder)
                {
                    RESOURCE_TO_DRAG = this;
                }
            }
        }
        else if(mouseInRange && gpe::input->check_mouse_released( mb_left))
        {
            if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos) )
            {
                returnAnswer = globalResouceId;
                seeked_project_name = projectParentFileName;

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
                                subYPos += cResource->element_box.h;
                                element_box.h += cResource->element_box.h;
                            }
                        }*/
                    }
                }
            }
        }
        else if( mouseInRange && gpe::input->check_mouse_released(1) )
        {
            if(gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos) )
            {
                returnAnswer = globalResouceId;
                seeked_project_name = projectParentFileName;

                LAST_CLICKED_RESOURCE = this;
                LAST_CLICKED_RESTYPE = resourceType;
                RESOURCEMENU_WAS_RIGHTCLICKED = true;
                RESOURCE_TO_DRAG = NULL;
            }
        }
        else if( mouseInRange && gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos) )
        {
            main_OVERLAY->update_tooltip( opName );
        }

        if( subMenuIsOpen )
        {
            cResource = NULL;
            subYPos = yPos+RESOURCE_AREA_HEIGHT;
            element_box.h = RESOURCE_AREA_HEIGHT;
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
                    element_box.h+= cResource->element_box.h;
                    subYPos += cResource->element_box.h;
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
            element_box.h = RESOURCE_AREA_HEIGHT;
            foundX2Pos = xPos+strTexWidth+RESOURCE_AREA_HEIGHT/2;
        }
    }
    else
    {
        element_box.h = RESOURCE_AREA_HEIGHT;
        //set_basic_image_value();

        if( mouseInRange && gpe::input->check_mouse_button_clicked(0))
        {
            if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos ) )
            {
                if( globalResouceId == selectedId && GPE_main_TabManager!=NULL )
                {
                    GPE_main_TabManager->set_selected_gresource( heldResource );
                }
                returnAnswer = globalResouceId;
                seeked_project_name = projectParentFileName;

                LAST_CLICKED_RESOURCE = this;
                RESOURCE_TO_DRAG = NULL;
            }
        }
        else if( mouseInRange && gpe::input->check_mouse_released( mb_left))
        {
            if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos ) )
            {
                if(globalResouceId == selectedId)
                {
                    //GPE_set_selected_gresource(heldResource);
                }
                returnAnswer = globalResouceId;
                seeked_project_name = projectParentFileName;
                LAST_CLICKED_RESOURCE = this;
            }
        }
        else if( mouseInRange && gpe::input->check_mouse_released(1) )
        {
            if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,viewedSpace->x-cam->x,lyPos,lx2Pos,ly2Pos) )
            {
                returnAnswer = globalResouceId;
                seeked_project_name = projectParentFileName;
                LAST_CLICKED_RESOURCE = this;
                RESOURCEMENU_WAS_RIGHTCLICKED = true;
            }
        }
        else if( mouseInRange && gpe::input->check_mouse_pressed(0) )
        {
            if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,viewedSpace->x-cam->x,lyPos,lx2Pos,ly2Pos) )
            {
                RESOURCE_TO_DRAG = this;
            }
        }
        else if(mouseInRange && gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos) )
        {
            main_OVERLAY->update_tooltip( opName );
        }

        if(heldResource!=NULL)
        {
            std::string heldresource_name = heldResource->get_name();
            if( opName!=heldresource_name )
            {
                if( heldresource_name.size()>0 && stg_ex::is_alnum(heldresource_name,false,true)  )
                {
                    resouceNameChanged = true;
                    opName = heldresource_name;
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

void GPE_GeneralResourceContainer::render_contained_object( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{

}

void GPE_GeneralResourceContainer::render_option( int xPos, int yPos, int selectedIdNumber, gpe::shape_rect * viewedSpace, gpe::shape_rect * cam, bool renderSubOptions, bool renderAutomatically)
{
    bool selfIsInView = false;

    viewedSpace = gpe::camera_find( viewedSpace );
    cam = gpe::camera_find( cam );
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
            int subOptionsSize = (int)subOptions.size();
            if( (selfIsInView || renderAutomatically) && subOptionsSize > 0 )
            {
                gpe::gfs->render_text( xPos-cam->x,yPos-cam->y,"-",theme_main->main_box_font_color,FONT_RESOURCEBAR,gpe::fa_left,gpe::fa_top);
            }
            GPE_GeneralResourceContainer * foundContainer = NULL;
            int subXPos = xPos+RESOURCE_AREA_HEIGHT;
            int subYPos = yPos+RESOURCE_AREA_HEIGHT;
            for( int i=0; i< subOptionsSize; i++ )
            {
                foundContainer = subOptions[i];
                if(foundContainer!=NULL)
                {
                    foundContainer->render_option( subXPos,subYPos,selectedIdNumber,viewedSpace,cam,renderSubOptions,renderAutomatically);
                    subYPos+=foundContainer->element_box.h;
                }
            }
        }
        else if( (selfIsInView || renderAutomatically ) && (int)subOptions.size() > 0 )
        {
            gpe::gfs->render_text( xPos-cam->x,yPos-cam->y,"+",theme_main->main_box_font_color,FONT_RESOURCEBAR,gpe::fa_left,gpe::fa_top);
        }
    }

    if( selfIsInView || renderAutomatically)
    {
        if(globalResouceId ==selectedIdNumber && projectParentName.compare(project_current_name)==0 )
        {
            //gpe::gcanvas->render_rectangle( xPos-cam->x,yPos-cam->y,xPos+RESOURCE_AREA_HEIGHT*2+GENERAL_NEAR_ICON_YPADDING+strTexWidth-cam->x,yPos+16-cam->y,theme_main->button_box_highlight_color,false,64);
            gpe::gcanvas->render_rectangle( xPos-cam->x, yPos-cam->y, widget_box.w-cam->x,yPos+16-cam->y,theme_main->button_box_highlight_color,false,64);
        }

        if( (isFolder || isSuperFolder || isSuperProjectFolder ) && containerTexture!=NULL )
        {
            if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
            {
                containerTexture->render_tex_resized( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,RESOURCE_AREA_HEIGHT,RESOURCE_AREA_HEIGHT,NULL,theme_main->folder_color_highlight);
            }
            else
            {
                containerTexture->render_tex_resized( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,RESOURCE_AREA_HEIGHT,RESOURCE_AREA_HEIGHT,NULL,theme_main->folder_color);
            }
        }
        else if( !isSuperProjectFolder && !isFolder && !isSuperFolder )
        {
            render_image( xPos,yPos,RESOURCE_AREA_HEIGHT,RESOURCE_AREA_HEIGHT,NULL,cam );
        }

        gpe::gfs->render_text( xPos+RESOURCE_AREA_HEIGHT+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y+RESOURCE_AREA_HEIGHT/2,opName,theme_main->main_box_font_color,FONT_RESOURCEBAR,gpe::fa_left,gpe::fa_center);
        lastResTypeRendered = resourceType;
    }
}

void GPE_GeneralResourceContainer::render_image( int xPos, int yPos, int rWidth, int rHeight, gpe::shape_rect * viewedSpace, gpe::shape_rect *cam, gpe::color * renderColor)
{
    if( renderColor == NULL )
    {
        renderColor = gpe::c_white;
    }
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    bool imageWasRendered = false;

    if( (isFolder || isSuperProjectFolder) && containerTexture!=NULL )
    {
        if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
        {
            containerTexture->render_tex_resized( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,rWidth,rHeight,NULL,theme_main->folder_color_highlight);
        }
        else
        {
            containerTexture->render_tex_resized( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,rWidth,rHeight,NULL,theme_main->folder_color);
        }
    }
    else if( !isSuperProjectFolder)
    {
        gpe::animaton2d * myAnim = get_resource_animation();
        if( myAnim !=NULL )
        {
            myAnim->render_resized( 0, xPos - cam->x, yPos - cam->y, rWidth, rHeight, NULL );
            imageWasRendered= true;
        }
        else
        {
            gpe::texture_base * myTexture = get_resource_texture();
            if( myTexture!=NULL)
            {
                myTexture->render_tex_resized( xPos-cam->x, yPos-cam->y, rWidth, rHeight,NULL);
                imageWasRendered= true;
            }
        }
    }

    if( imageWasRendered==false && containerTexture!=NULL )
    {
        gpe::color * imageColor  = theme_main->main_box_font_color;

        if( rWidth <=0 && rHeight <= 0 )
        {
            rWidth = rHeight = RESOURCE_AREA_HEIGHT;
        }

        if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
        {
            containerTexture->render_tex_resized(xPos-cam->x,yPos-cam->y,rWidth,rHeight,NULL,theme_main->main_box_font_color);
        }
        else
        {
            containerTexture->render_tex_resized(  xPos-cam->x,yPos-cam->y,rWidth,rHeight,NULL,theme_main->main_box_font_color);
        }
    }

}

void GPE_GeneralResourceContainer::render_image_scaled( int xPos, int yPos, float xScale, float yScale, gpe::shape_rect * viewedSpace, gpe::shape_rect *cam, gpe::color * renderColor)
{
    if( renderColor == NULL )
    {
        renderColor = gpe::c_white;
    }
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    bool imageWasRendered = false;

    if( (isFolder || isSuperProjectFolder) && containerTexture!=NULL )
    {
        if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
        {
            containerTexture->render_tex_scaled( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y, xScale,yScale,NULL, theme_main->folder_color_highlight);
        }
        else
        {
            containerTexture->render_tex_scaled( xPos+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,yPos-cam->y,xScale,yScale,NULL, theme_main->folder_color );
        }
    }
    else if( !isSuperProjectFolder)
    {
        gpe::animaton2d * myAnim = get_resource_animation();
        if( myAnim !=NULL )
        {
            myAnim->render_scaled(0, xPos, yPos, xScale, yScale,cam );
            imageWasRendered= true;
        }
        else
        {
            gpe::texture_base * myTexture = get_resource_texture();
            if( myTexture!=NULL)
            {
                myTexture->render_tex_scaled( xPos-cam->x, yPos-cam->y, xScale, yScale,NULL );
                imageWasRendered= true;
            }
        }
    }

    if( imageWasRendered==false && containerTexture!=NULL )
    {
        gpe::color * imageColor  = theme_main->main_box_font_color;

        if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
        {
            containerTexture->render_tex_scaled( xPos-cam->x,yPos-cam->y, xScale, yScale, NULL,theme_main->main_box_font_color);
        }
        else
        {
            containerTexture->render_tex_scaled( xPos-cam->x,yPos-cam->y,xScale, yScale, NULL,theme_main->main_box_font_color);
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
                element_box.h-=otherContainer->element_box.h;
                //otherContainer->parentResource = NULL;
                if( deleteResource )
                {
                    GPE_main_TabManager->close_resource_tab(projectParentName, resIdToDelete );
                    delete tContainer;
                    tContainer = NULL;
                    otherContainer = NULL;
                    //gpe::error_log->report("Rez deletedish...");
                }
                subOptions.erase( subOptions.begin()+i );
                //gpe::error_log->report("Rez deleted...");
            }
        }
    }
}

void GPE_GeneralResourceContainer::save_container(std::string file_path, int backupId)
{
    if( heldResource!=NULL)
    {
        heldResource->save_resource(file_path, backupId);
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
        containerTexture = paw_gui_rsm->texture_add( "guiFolder", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png");
    }
    else
    {
        switch( resourceType)
        {
            case gpe::resource_type_animation:
                containerTexture = paw_gui_rsm->texture_add( "guiAnimationIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magnet.png");
            break;
            case gpe::resource_type_texture:
                containerTexture = paw_gui_rsm->texture_add( "guiTextureIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/image.png");
            break;
            case gpe::resource_type_tilesheet:
                containerTexture = paw_gui_rsm->texture_add( "guiTilesheetIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/th-large.png");
            break;
            case gpe::resource_type_audio:
                containerTexture = paw_gui_rsm->texture_add( "guiAudioIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/volume-up.png");
            break;
            case gpe::resource_type_video:
                containerTexture = paw_gui_rsm->texture_add( "guiVideoIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/film.png");
            break;
            case gpe::resource_type_object:
                containerTexture = paw_gui_rsm->texture_add( "guiObjectIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/automobile.png");
            break;
            case gpe::resource_type_function:
                containerTexture = paw_gui_rsm->texture_add( "guiFunctionIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/code.png");
            break;
            case gpe::resource_type_font:
                containerTexture = paw_gui_rsm->texture_add( "guiFontIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/font.png");
            break;
            case gpe::resource_type_scene:
                containerTexture = paw_gui_rsm->texture_add( "guiSceneIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/area-chart.png");
            break;
            default:
                containerTexture = paw_gui_rsm->texture_add( "guiDefaultIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/diamond.png");
            break;
        }
    }
}

void GPE_GeneralResourceContainer::set_held_resource(generalGameResource * newGenResource)
{
    heldResource = newGenResource;
}

void GPE_GeneralResourceContainer::set_global_id(int new_id)
{
    globalResouceId = new_id;
    if( heldResource!=NULL)
    {
        heldResource->set_global_rid(new_id);
    }
}

void GPE_GeneralResourceContainer::set_container_gameid(int new_id)
{
    exportBuildGlobalId = new_id;
    if( heldResource!=NULL)
    {
        heldResource->exportBuildGlobalId = new_id;
    }
}

void GPE_GeneralResourceContainer::set_name(std::string new_name)
{
    opName = new_name;
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
        heldResource->set_name(new_name);
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

