/*
pawgui_general_resource_container.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_general_resource_container.h"
#include "pawgui_parsers.h"
#include "pawgui_popups.h"
#include "pawgui_tab_manager.h"

namespace pawgui
{
    widget_resource_container * resource_dragged = NULL;
    widget_resource_container * resource_last_clicked = NULL;
    bool resourcemenu_rightclicked = false;
    widget_resource_container * resource_being_renamed = NULL;

    int resource_last_clicked_type = -1;
    int resource_dragged_type = -1;
    int resourcetype_last_rendered_type = -1;

    std::string project_current_name = "";
    std::string seeked_project_name = "";


    widget_resource_container::widget_resource_container(std::string projFolderName, std::string new_name, int rType, int rId,bool folder, int globalIdVal,  int rezPropValue)
    {
        foundX2Pos = 0;
        resouceNameChanged = false;
        subContentsModified = false;
        parentResource = NULL;
        projectParentFileName = projectParentName = projFolderName;
        widget_name = new_name;
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
        element_box.h = resource_container_default_height;
        containeranimation = NULL;
        containerTexture = NULL;
        animationFrameNumber = 0;
        heldResource = NULL;
        subMenuIsOpen = false;
        int textW = 0;
        int textH = 0;

        if( (int)widget_name.size()>0 && font_toolbar!=NULL)
        {
            font_toolbar->get_metrics(widget_name,&textW, &textH);
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

    widget_resource_container::~widget_resource_container()
    {
        gpe::error_log->report("Removing Resource named ["+widget_name+"].");
        if( main_loader_display != NULL )
        {
            main_loader_display->update_submessages( "Removing Resource", widget_name );
        }
        if( heldResource!=NULL)
        {
            gpe::error_log->report("Removing Held-Resource named ["+widget_name+"].");
            if( main_tab_resource_bar!=NULL)
            {
                main_tab_resource_bar->close_resource_tab( projectParentName,heldResource->globalResouceIdNumber );
            }
            gpe::error_log->report("Held-Resource deleted ["+widget_name+"].");
            delete heldResource;
            heldResource = NULL;
        }
        gpe::error_log->report("Held-Resource's sub-options deleted ["+widget_name+"].");
        widget_resource_container * tempSubOp = NULL;
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
        gpe::error_log->report("Resource ["+widget_name+"] deleted.");

    }

    void widget_resource_container::add_resource_container( widget_resource_container * newResource, bool changeGlobalId, widget_resource_container * referenceObject)
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
                widget_resource_container * tempResource = NULL;
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
                else if( resource_dragged!=NULL)
                {
                    if( resource_dragged->matches(newResource) )
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

    widget_resource_container * widget_resource_container::add_resource_folder( std::string resourceTypeName,  int gResId,int rezPropValue)
    {
        int new_id = (int)subOptions.size()+1;
        widget_resource_container * newFolder = new widget_resource_container(projectParentFileName,resourceTypeName,resourceType,new_id,true, gResId, rezPropValue);
        newFolder->element_box.x = element_box.x;
        newFolder->element_box.y = element_box.y+new_id*resource_container_default_height;
        //element_box.h+=resource_container_default_height;
        newFolder->parentResource = this;
        subOptions.push_back(newFolder);
        if( subMenuIsOpen)
        {
            element_box.h += resource_container_default_height;
        }
        return newFolder;
    }

    widget_resource_container * widget_resource_container::add_newtype_folder( int rType,std::string resourceTypeName, int gResId, int rezPropValue)
    {
        int new_id = (int)subOptions.size()+1;
        widget_resource_container * newFolder = new widget_resource_container(projectParentFileName,resourceTypeName,rType,new_id,true, gResId,rezPropValue);
        newFolder->element_box.x = element_box.x;
        newFolder->element_box.y = element_box.y+new_id*resource_container_default_height;
        //element_box.h+=resource_container_default_height;
        newFolder->parentResource = this;
        subOptions.push_back(newFolder);
        element_box.h += resource_container_default_height;
        return newFolder;
    }

    void widget_resource_container::delete_resource(widget_resource_container * otherContainer)
    {
        remove_resource(otherContainer);
    }

    bool widget_resource_container::detect_name_change(bool autoChange)
    {
        if( heldResource!=NULL)
        {
            std::string fName = heldResource->get_current_name();
            if( fName!=widget_name)
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

    widget_resource_container * widget_resource_container::get_resource_at(int resourcePos, bool nestDown)
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

    widget_resource_container * widget_resource_container::find_resource_from_id(int resourceIdToFind, bool nestDown, bool includeSelf)
    {
        widget_resource_container * rFound = NULL;
        widget_resource_container * nResource = NULL;
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

    widget_resource_container * widget_resource_container::get_usable_resource()
    {
        widget_resource_container * rFound = NULL;
        widget_resource_container * nResource = NULL;
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

    widget_resource_container * widget_resource_container::find_resource_from_name(std::string rNameToFind, bool nestDown)
    {
        bool foundResult = false;
        widget_resource_container * fContainer = NULL;
        widget_resource_container * soughtContainer = NULL;
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

    widget_resource_container * widget_resource_container::find_resource_target(std::string rNameToFind, bool nestDown)
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

    std::string widget_resource_container::get_name()
    {
        return widget_name;
    }

    std::string widget_resource_container::get_project_name()
    {
        return projectParentName;
    }

    int widget_resource_container::get_global_id()
    {
        return globalResouceId;
    }

    general_resource * widget_resource_container::get_held_resource()
    {
        return heldResource;
    }

    int widget_resource_container::get_resource_count()
    {
        int returnNumb = 0;
        if( !isFolder &&  !isSuperFolder &&  !isSuperProjectFolder)
        {
            returnNumb = 1;
        }
        widget_resource_container * tempContainer = NULL;
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


    int widget_resource_container::get_resource_id()
    {
        return resourceId;
    }

    int widget_resource_container::get_resource_type()
    {
        return resourceType;
    }


    int widget_resource_container::get_resource_image_frame()
    {
        return animationFrameNumber;
    }

    gpe::animaton2d * widget_resource_container::get_resource_animation()
    {
        if( !isFolder && !isSuperProjectFolder && heldResource!=NULL )
        {
            return heldResource->get_resource_animation();
        }
        return NULL;
    }

    gpe::texture_base * widget_resource_container::get_resource_texture()
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

    int widget_resource_container::get_options_width()
    {
        int maxFoundWith = 0;
        widget_resource_container * tempOption = NULL;
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

    int widget_resource_container::get_size()
    {
        return (int)subOptions.size();
    }

    void widget_resource_container::grab_useable_resources(std::vector <widget_resource_container * > &rVector)
    {
        widget_resource_container * nResource = NULL;
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

    bool widget_resource_container::include_local_files( std::string pBuildDir , int buildType )
    {

    }

    void widget_resource_container::integrate_into_syntax()
    {
        if( heldResource!=NULL)
        {
            heldResource->integrate_into_syntax();
        }

        widget_resource_container * tSub = NULL;
        for( int i = 0; i < (int)subOptions.size(); i++)
        {
            tSub = subOptions[i];
            if( tSub!=NULL)
            {
                tSub->integrate_into_syntax();
            }
        }
    }

    bool widget_resource_container::is_folder()
    {

        return isFolder;
    }

    bool widget_resource_container::is_super_folder()
    {
        return isSuperFolder;
    }

    bool widget_resource_container::is_super_project_folder()
    {
        return isSuperProjectFolder;
    }

    bool widget_resource_container::is_child_of(widget_resource_container * otherContainer)
    {
        if( otherContainer!=NULL)
        {
            int i;
            widget_resource_container * tempSubOption = NULL;
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

    bool widget_resource_container::can_obtain(widget_resource_container * otherContainer)
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

    int widget_resource_container::matches(widget_resource_container * otherContainer)
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

    void widget_resource_container::open_folder()
    {
        subMenuIsOpen = true;
    }

    void widget_resource_container::preprocess_container(std::string file_path, int backupId)
    {
        if( heldResource!=NULL)
        {
            heldResource->load_resource(file_path);
        }
        widget_resource_container * cResource = NULL;
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

    void widget_resource_container::prerender_self( )
    {

        if( heldResource!=NULL)
        {
            heldResource->prerender_self( );
        }
        widget_resource_container * tSubOption= NULL;
        for( int i = 0; i < (int)subOptions.size(); i++)
        {
            tSubOption = subOptions[i];
            if( tSubOption!=NULL)
            {
                tSubOption->prerender_self( );
            }
        }
    }

    int widget_resource_container::process_container(int xPos, int yPos, int selectedId, gpe::shape_rect * view_space, gpe::shape_rect * cam, bool mouseInRange )
    {
        int returnAnswer = -1;
        int lxPos = xPos-cam->x+view_space->x;
        int lyPos = yPos-cam->y+view_space->y;
        int lx2Pos = lxPos+cam->w;
        int ly2Pos = lyPos+resource_container_default_height;
        bool folderOpened = false;
        subContentsModified = false;
        foundX2Pos = xPos+strTexWidth+resource_container_default_height*3/2;
        if( isFolder || isSuperFolder || isSuperProjectFolder )
        {
            widget_resource_container * cResource = NULL;
            int subYPos = yPos+resource_container_default_height;
            if( mouseInRange && gpe::input->check_mouse_pressed(0) )
            {
                if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos) )
                {
                    seeked_project_name = projectParentFileName;

                    if( !isSuperFolder && !isSuperProjectFolder)
                    {
                        resource_dragged = this;
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
                    if( resource_dragged!=NULL)
                    {
                        resource_last_clicked = this;
                        if( resource_dragged->matches(this)!=1)
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
                                    cResource->process_container(xPos+resource_container_default_height,subYPos,selectedId,view_space,cam,mouseInRange);
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

                    resource_last_clicked = this;
                    resource_last_clicked_type = resourceType;
                    resourcemenu_rightclicked = true;
                    resource_dragged = NULL;
                }
            }
            else if( mouseInRange && gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos) )
            {
                main_overlay_system->update_tooltip( widget_name );
            }

            if( subMenuIsOpen )
            {
                cResource = NULL;
                subYPos = yPos+resource_container_default_height;
                element_box.h = resource_container_default_height;
                int subAnswer = -1;
                for(int i = 0; i<(int)subOptions.size(); i+=1)
                {
                    cResource = subOptions[i];
                    if(cResource != NULL)
                    {
                        subAnswer = cResource->process_container(xPos,subYPos,selectedId,view_space,cam,mouseInRange);
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
                element_box.h = resource_container_default_height;
                foundX2Pos = xPos+strTexWidth+resource_container_default_height/2;
            }
        }
        else
        {
            element_box.h = resource_container_default_height;
            //set_basic_image_value();

            if( mouseInRange && gpe::input->check_mouse_button_clicked(0))
            {
                if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos ) )
                {
                    if( globalResouceId == selectedId && main_tab_resource_bar!=NULL )
                    {
                        main_tab_resource_bar->set_selected_gresource( heldResource );
                    }
                    returnAnswer = globalResouceId;
                    seeked_project_name = projectParentFileName;

                    resource_last_clicked = this;
                    resource_dragged = NULL;
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
                    resource_last_clicked = this;
                }
            }
            else if( mouseInRange && gpe::input->check_mouse_released(1) )
            {
                if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,view_space->x-cam->x,lyPos,lx2Pos,ly2Pos) )
                {
                    returnAnswer = globalResouceId;
                    seeked_project_name = projectParentFileName;
                    resource_last_clicked = this;
                    resourcemenu_rightclicked = true;
                }
            }
            else if( mouseInRange && gpe::input->check_mouse_pressed(0) )
            {
                if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,view_space->x-cam->x,lyPos,lx2Pos,ly2Pos) )
                {
                    resource_dragged = this;
                }
            }
            else if(mouseInRange && gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,lxPos,lyPos,lx2Pos,ly2Pos) )
            {
                main_overlay_system->update_tooltip( widget_name );
            }

            if(heldResource!=NULL)
            {
                std::string heldresource_name = heldResource->get_name();
                if( widget_name!=heldresource_name )
                {
                    if( heldresource_name.size()>0 && stg_ex::is_alnum(heldresource_name,false,true)  )
                    {
                        resouceNameChanged = true;
                        widget_name = heldresource_name;
                    }
                }
            }
        }
        return returnAnswer;
    }

    bool widget_resource_container::read_data_from_projectfile(std::ofstream * fileTarget)
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

    void widget_resource_container::render_contained_object( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {

    }

    void widget_resource_container::render_option( int xPos, int yPos, int selectedIdNumber, gpe::shape_rect * view_space, gpe::shape_rect * cam, bool renderSubOptions, bool renderAutomatically)
    {
        bool selfIsInView = false;

        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );
        int rendY = yPos;
        if( rendY>=cam->y && rendY <=cam->y+cam->h )
        {
            selfIsInView = true;
        }
        else if( rendY+resource_container_default_height >=cam->y && rendY+resource_container_default_height <=cam->y+cam->h  )
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
                    gpe::gfs->render_text( xPos-cam->x,yPos-cam->y,"-",pawgui::theme_main->main_box_font_color,font_resourcebar,gpe::fa_left,gpe::fa_top);
                }
                widget_resource_container * foundContainer = NULL;
                int subXPos = xPos+resource_container_default_height;
                int subYPos = yPos+resource_container_default_height;
                for( int i=0; i< subOptionsSize; i++ )
                {
                    foundContainer = subOptions[i];
                    if(foundContainer!=NULL)
                    {
                        foundContainer->render_option( subXPos,subYPos,selectedIdNumber,view_space,cam,renderSubOptions,renderAutomatically);
                        subYPos+=foundContainer->element_box.h;
                    }
                }
            }
            else if( (selfIsInView || renderAutomatically ) && (int)subOptions.size() > 0 )
            {
                gpe::gfs->render_text( xPos-cam->x,yPos-cam->y,"+",pawgui::theme_main->main_box_font_color,font_resourcebar,gpe::fa_left,gpe::fa_top);
            }
        }

        if( selfIsInView || renderAutomatically)
        {
            if(globalResouceId ==selectedIdNumber && projectParentName.compare(project_current_name)==0 )
            {
                gpe::gcanvas->render_rectangle( xPos-cam->x, yPos-cam->y, widget_box.w-cam->x,yPos+16-cam->y,pawgui::theme_main->button_box_highlight_color,false,64);
            }

            if( (isFolder || isSuperFolder || isSuperProjectFolder ) && containerTexture!=NULL )
            {
                if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
                {
                    containerTexture->render_tex_resized( xPos+default_icon_plusminus_size-cam->x,yPos-cam->y,resource_container_default_height,resource_container_default_height,NULL,pawgui::theme_main->folder_color_highlight);
                }
                else
                {
                    containerTexture->render_tex_resized( xPos+default_icon_plusminus_size-cam->x,yPos-cam->y,resource_container_default_height,resource_container_default_height,NULL,pawgui::theme_main->folder_color);
                }
            }
            else if( !isSuperProjectFolder && !isFolder && !isSuperFolder )
            {
                render_image( xPos,yPos,resource_container_default_height,resource_container_default_height,NULL,cam );
            }

            gpe::gfs->render_text( xPos+resource_container_default_height+default_icon_plusminus_size-cam->x,yPos-cam->y+resource_container_default_height/2,widget_name,pawgui::theme_main->main_box_font_color,font_resourcebar,gpe::fa_left,gpe::fa_center);
            resourcetype_last_rendered_type = resourceType;
        }
    }

    void widget_resource_container::render_image( int xPos, int yPos, int r_width, int r_height, gpe::shape_rect * view_space, gpe::shape_rect *cam, gpe::color * renderColor)
    {
        if( renderColor == NULL )
        {
            renderColor = gpe::c_white;
        }
        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);
        bool imageWasRendered = false;

        if( (isFolder || isSuperProjectFolder) && containerTexture!=NULL )
        {
            if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
            {
                containerTexture->render_tex_resized( xPos+default_icon_plusminus_size-cam->x,yPos-cam->y,r_width,r_height,NULL,pawgui::theme_main->folder_color_highlight);
            }
            else
            {
                containerTexture->render_tex_resized( xPos+default_icon_plusminus_size-cam->x,yPos-cam->y,r_width,r_height,NULL,pawgui::theme_main->folder_color);
            }
        }
        else if( !isSuperProjectFolder)
        {
            gpe::animaton2d * myAnim = get_resource_animation();
            if( myAnim !=NULL )
            {
                myAnim->render_resized( 0, xPos - cam->x, yPos - cam->y, r_width, r_height, NULL );
                imageWasRendered= true;
            }
            else
            {
                gpe::texture_base * myTexture = get_resource_texture();
                if( myTexture!=NULL)
                {
                    myTexture->render_tex_resized( xPos-cam->x, yPos-cam->y, r_width, r_height,NULL);
                    imageWasRendered= true;
                }
            }
        }

        if( imageWasRendered==false && containerTexture!=NULL )
        {
            gpe::color * imageColor  = pawgui::theme_main->main_box_font_color;

            if( r_width <=0 && r_height <= 0 )
            {
                r_width = r_height = resource_container_default_height;
            }

            if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
            {
                containerTexture->render_tex_resized(xPos-cam->x,yPos-cam->y,r_width,r_height,NULL,pawgui::theme_main->main_box_font_color);
            }
            else
            {
                containerTexture->render_tex_resized(  xPos-cam->x,yPos-cam->y,r_width,r_height,NULL,pawgui::theme_main->main_box_font_color);
            }
        }

    }

    void widget_resource_container::render_image_scaled( int xPos, int yPos, float xScale, float yScale, gpe::shape_rect * view_space, gpe::shape_rect *cam, gpe::color * renderColor)
    {
        if( renderColor == NULL )
        {
            renderColor = gpe::c_white;
        }
        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);
        bool imageWasRendered = false;

        if( (isFolder || isSuperProjectFolder) && containerTexture!=NULL )
        {
            if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
            {
                containerTexture->render_tex_scaled( xPos+default_icon_plusminus_size-cam->x,yPos-cam->y, xScale,yScale,NULL, pawgui::theme_main->folder_color_highlight);
            }
            else
            {
                containerTexture->render_tex_scaled( xPos+default_icon_plusminus_size-cam->x,yPos-cam->y,xScale,yScale,NULL, pawgui::theme_main->folder_color );
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
            gpe::color * imageColor  = pawgui::theme_main->main_box_font_color;

            if(projectParentName.compare(project_current_name)==0 && isSuperProjectFolder)
            {
                containerTexture->render_tex_scaled( xPos-cam->x,yPos-cam->y, xScale, yScale, NULL,pawgui::theme_main->main_box_font_color);
            }
            else
            {
                containerTexture->render_tex_scaled( xPos-cam->x,yPos-cam->y,xScale, yScale, NULL,pawgui::theme_main->main_box_font_color);
            }
        }
    }

    void widget_resource_container::remove_resource(widget_resource_container * otherContainer, bool deleteResource)
    {
        if( otherContainer!=NULL )
        {
            widget_resource_container * tContainer = NULL;
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
                        main_tab_resource_bar->close_resource_tab(projectParentName, resIdToDelete );
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

    void widget_resource_container::save_container(std::string file_path, int backupId)
    {
        if( heldResource!=NULL)
        {
            heldResource->save_resource(file_path, backupId);
        }
    }

    int widget_resource_container::search_for_string(std::string needle)
    {
        int foundStrings = 0;
        widget_resource_container * fContainer  = NULL;
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

    int widget_resource_container::search_and_replace_string(std::string needle, std::string newStr)
    {
        int foundStrings = 0;
        widget_resource_container * fContainer  = NULL;
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

    void widget_resource_container::set_basic_image_value()
    {
        if( isFolder)
        {
            containerTexture = rsm_gui->texture_add( "guiFolder", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png");
        }
        else
        {
            switch( resourceType)
            {
                case gpe::resource_type_animation:
                    containerTexture = rsm_gui->texture_add( "guiAnimationIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magnet.png");
                break;
                case gpe::resource_type_texture:
                    containerTexture = rsm_gui->texture_add( "guiTextureIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/image.png");
                break;
                case gpe::resource_type_tilesheet:
                    containerTexture = rsm_gui->texture_add( "guiTilesheetIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/th-large.png");
                break;
                case gpe::resource_type_audio:
                    containerTexture = rsm_gui->texture_add( "guiAudioIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/volume-up.png");
                break;
                case gpe::resource_type_video:
                    containerTexture = rsm_gui->texture_add( "guiVideoIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/film.png");
                break;
                case gpe::resource_type_object:
                    containerTexture = rsm_gui->texture_add( "guiObjectIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/automobile.png");
                break;
                case gpe::resource_type_function:
                    containerTexture = rsm_gui->texture_add( "guiFunctionIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/code.png");
                break;
                case gpe::resource_type_font:
                    containerTexture = rsm_gui->texture_add( "guiFontIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/font.png");
                break;
                case gpe::resource_type_scene:
                    containerTexture = rsm_gui->texture_add( "guiSceneIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/area-chart.png");
                break;
                default:
                    containerTexture = rsm_gui->texture_add( "guiDefaultIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/diamond.png");
                break;
            }
        }
    }

    void widget_resource_container::set_held_resource(general_resource * newGenResource)
    {
        heldResource = newGenResource;
    }

    void widget_resource_container::set_global_id(int new_id)
    {
        globalResouceId = new_id;
        if( heldResource!=NULL)
        {
            heldResource->set_global_rid(new_id);
        }
    }

    void widget_resource_container::set_container_gameid(int new_id)
    {
        exportBuildGlobalId = new_id;
        if( heldResource!=NULL)
        {
            heldResource->exportBuildGlobalId = new_id;
        }
    }

    void widget_resource_container::set_name(std::string new_name)
    {
        widget_name = new_name;
        int textW = 0;
        int textH = 0;

        if( (int)widget_name.size()>0 && font_toolbar!=NULL)
        {
            font_toolbar->get_metrics(widget_name,&textW, &textH);
        }
        strTexWidth = textW;
        strTexHeight = textH;
        if( heldResource!=NULL)
        {
            heldResource->set_name(new_name);
        }
    }

    void widget_resource_container::set_project_parent_name(std::string newParentName)
    {
        projectParentFileName = newParentName;
        if( heldResource!=NULL)
        {
            heldResource->parentProjectName = projectParentFileName;
        }
        int sSize = (int)subOptions.size();
        if( sSize > 0)
        {
            widget_resource_container * cResource = NULL;
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

    bool widget_resource_container::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
    {
        bool foundProblem = false;
        std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
        if( fileTarget!=NULL)
        {
            if( fileTarget->is_open() )
            {
                if( isSuperProjectFolder)
                {
                    *fileTarget << "[Project=" << widget_name<< "]\n";
                }
                else if(isSuperFolder)
                {
                    *fileTarget << nestedTabsStr << "[SuperFolder=" << widget_name<< "]\n";
                    if( main_loader_display != NULL )
                    {
                        main_loader_display->update_submessages( "Saving "+widget_name+" resources", "Please wait..." );
                    }
                }
                else if( isFolder)
                {
                    *fileTarget << nestedTabsStr << "[Folder=" << widget_name << "," << globalResouceId << "]\n";
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
                    widget_resource_container * cResource = NULL;
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
}
