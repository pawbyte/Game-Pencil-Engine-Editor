/*
gpe_scene_object_class.cpp
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

#include "gpe_scene_object_class.h"

GPE_SceneGameObject::GPE_SceneGameObject( std::string nName)
{
    isLocked = false;
    branchType = gpe::branch_type::OBJECT;
    iconTexture = paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/automobile.png") ;

    objTypeName = "";
    isBeingMoved = false;
    objTypeId = -1;
    customObjId = -1;
    objBeingPlaced = NULL;

    width = 32;
    height = 32;
    xPivot = width/2;
    yPivot = height/2;

    branchNameField->set_name(nName );
    branchNameField->set_label( nName );
}

GPE_SceneGameObject::~GPE_SceneGameObject()
{
}

void GPE_SceneGameObject::add_typed_elements()
{
    if( PANEL_INSPECTOR!=NULL )
    {
        //PANEL_INSPECTOR->add_gui_element( objBeingPlaced, true );
    }
}

bool GPE_SceneGameObject::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  GPE_GeneralResourceContainer * localResTypeController )
{

    GPE_GeneralResourceContainer * allObjsFolder = localResTypeController->find_resource_from_name( gpe::resource_type_names[ gpe::resource_type_object]+"s");
    GPE_GeneralResourceContainer * fObjToPlace = NULL;

    fObjToPlace = allObjsFolder->find_resource_from_id( objTypeId);
    std::string nestedTabsStr = generate_tabs( leftTabAmount  );
    if( fObjToPlace!=NULL)
    {
        *fileTarget << nestedTabsStr << "newBranch = _scn_temp_layer.scnStartObjects.push( {objId:" <<stg_ex::int_to_string(fObjToPlace->exportBuildGlobalId) << "} ); \n";
    }
    else
    {
        *fileTarget << nestedTabsStr << "newBranch = _scn_temp_layer.scnStartObjects.push( {objId: -1} ); \n";
    }
    GPE_SceneBasicClass::build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController);
    return true;
}

void GPE_SceneGameObject::calculate_size()
{

}

void GPE_SceneGameObject::process_elements()
{
    GPE_SceneBasicClass::process_elements();
}

void GPE_SceneGameObject::render_branch()
{
    xPivot = 16;
    yPivot = 16;
    width = 32;
    height = 32;
    spm->tempRect->x = ceil( (xPos*spm->zoomValue-spm->currentCamera->x*spm->zoomValue) );
    spm->tempRect->y = ceil( (yPos*spm->zoomValue-spm->currentCamera->y*spm->zoomValue) );
    if( objTypeId > 0 && spm->cSceneObjList!=NULL )
    {
        //renders the object's animation
        GPE_GeneralResourceContainer * objTypeContainer = spm->cSceneObjList->find_resource_from_id(objTypeId);
        if( objTypeContainer!=NULL && objTypeContainer->get_held_resource()!=NULL )
        {
            bool objectanimationRender = false;
            gameObjectResource*tempGameObj = (gameObjectResource*) objTypeContainer->get_held_resource();
            if( tempGameObj!=NULL)
            {
                //fangle = angleField->get_held_number();
                //if( fangle!=0 && fangle!=360 && fangle!=720)
                {
                    GPE_GeneralResourceContainer * sprTypeContainer = tempGameObj->animationField->get_selected_container();
                    if( sprTypeContainer!=NULL)
                    {
                        animationResource*animRes = (animationResource*) sprTypeContainer->get_held_resource();
                        if( animRes!=NULL && animRes->animInEditor!=NULL)
                        {
                            xPivot = animRes->animInEditor->colBox->get_center();
                            yPivot = animRes->animInEditor->colBox->get_middle();
                            width = animRes->animInEditor->get_width();
                            height = animRes->animInEditor->get_height();
                            //animRes->animInEditor->render_rotated( animRes->get_preview_frame(),spm->tempRect->x,spm->tempRect->y, angle, xScale*spm->zoomValue,yScale*spm->zoomValue, NULL );
                            animRes->animInEditor->render_special( animRes->get_preview_frame(),spm->tempRect->x,spm->tempRect->y, xScale*spm->zoomValue,yScale*spm->zoomValue,angle, branchColor->get_color(),branchAlpha->get_value(), NULL );
                            //gpe::gfs->render_text( spm->tempRect->x, spm->tempRect->y-48, "XOff:"+ stg_ex::int_to_string(xOffset)+",YOff:"+ stg_ex::int_to_string(yOffset),c_red,font_default,gpe::fa_center,gpe::fa_bottom, 255 );
                            //gpe::gfs->render_text( spm->tempRect->x, spm->tempRect->y-16, "W:"+ stg_ex::int_to_string(width)+",H:"+ stg_ex::int_to_string(height),c_red,font_default,gpe::fa_center,gpe::fa_bottom, 255 );
                            objectanimationRender = true;
                        }
                    }
                }
            }
            else
            {
                xPos = 16;
                yPos = 16;
            }
            if( !objectanimationRender )
            {
                //objTypeContainer->render_image( spm->tempRect->x,spm->tempRect->y,ceil( (float)spm->tempRect->w*xScale*spm->zoomValue),ceil( (float)spm->tempRect->h*yScale*spm->zoomValue ),viewedSpace,cam);
            }
        }
        else
        {
            gpe::gcanvas->render_rectangle( spm->tempRect->x,spm->tempRect->y,spm->tempRect->x+spm->tempRect->w*spm->zoomValue,spm->tempRect->y+spm->tempRect->h*spm->zoomValue, gpe::c_maroon,false);
            gpe::gfs->render_text(  spm->tempRect->x, spm->tempRect->y,"Unknown Game Id["+ stg_ex::int_to_string( objTypeId )+"]", gpe::c_red,font_default,gpe::fa_left,gpe::fa_top);
        }
    }
    else
    {
        gpe::gcanvas->render_rectangle( spm->tempRect->x,spm->tempRect->y,spm->tempRect->x+spm->tempRect->w*spm->zoomValue,spm->tempRect->y+spm->tempRect->h*spm->zoomValue,gpe::c_maroon,false);
        gpe::gfs->render_text(  spm->tempRect->x, spm->tempRect->y,"Unknown Game Object", gpe::c_red,font_default,gpe::fa_left,gpe::fa_top);
    }
    GPE_SpecialMenu_Branch::render_branch();
}

bool GPE_SceneGameObject::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"GPE_SceneObject=";
        if( (int)objTypeName.size() > 0)
        {
            *fileTarget << objTypeName << ",";
        }
        else
        {
            *fileTarget <<  objTypeId << ",";
        }
        *fileTarget << xPos << "," <<yPos<< "," ;
        *fileTarget <<  angle << "," << xScale << "," << yScale << "," ;
        if( branchColor!=NULL)
        {
            *fileTarget << branchColor->get_hex_string() +",";
        }
        else
        {
            *fileTarget <<  "#FFF";
        }
        if( (int)branchNameField->get_string().size() >0)
        {
            *fileTarget << branchNameField->get_string() << ",,,";
        }
        else
        {
            *fileTarget << "unnamed,,,";
        }
        *fileTarget << "\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        return true;
    }
    return false;
}
