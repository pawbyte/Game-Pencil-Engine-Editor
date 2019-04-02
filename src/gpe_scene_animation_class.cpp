/*
gpe_scene_animation_class.cpp
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

#include "gpe_scene_animation_class.h"

GPE_SceneAnimation::GPE_SceneAnimation( GPE_GeneralResourceContainer *pFolder  )
{
    iconTexture = guiRCM->texture_add( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/magnet.png") ;

    projectParentFolder = pFolder;

    if( projectParentFolder!=NULL)
    {
        animationInEditor = new GPE_DropDown_Resouce_Menu( "Animation",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ANIMATION]+"s"),-1,true);
        animationInEditor->set_width(192);
    }
    else
    {
        animationInEditor = NULL;
    }
    branchType = BRANCH_TYPE_ANIMATION;
    xPosField = new GPE_TextInputNumber("",true, 0 );
    xPosField->set_label("X-Pos");
    yPosField = new GPE_TextInputNumber("",true, 0 );
    yPosField->set_label("Y-Pos");

    startPositionField = new GPE_TextInputNumber("",false, 0 );
    startPositionField->set_label("Animation Start Frame");


    animationId = -1;
    animRes = NULL;
    animSpeed = 1;
    xScale = 1;
    yScale = 1;
    frameStartPosition = 0;
    framePosition = 0;

    animationSpeedField = new GPE_TextInputNumber("",false, -7000);
    animationSpeedField->set_label("Animation Speed");
    animationSpeedField->set_number( animSpeed );
    useCustomSpeed = new GPE_CheckBoxBasic("Uuse Custom Speed","Diffrentiate from the default speed of animation resource", false );
}

GPE_SceneAnimation::~GPE_SceneAnimation()
{
    if( startPositionField!=NULL)
    {
        delete startPositionField;
        startPositionField = NULL;
    }
    if( xPosField!=NULL)
    {
        delete xPosField;
        xPosField = NULL;
    }

    if( yPosField!=NULL)
    {
        delete yPosField;
        yPosField = NULL;
    }
    if( useCustomSpeed!=NULL )
    {
        delete useCustomSpeed;
        useCustomSpeed = NULL;
    }
    if( animationSpeedField!=NULL)
    {
        delete animationSpeedField;
        animationSpeedField = NULL;
    }
    if( xScaleField!=NULL)
    {
        delete xScaleField;
        xScaleField = NULL;
    }
    if( yScaleField!=NULL)
    {
        delete yScaleField;
        yScaleField = NULL;
    }
}

bool GPE_SceneAnimation::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  GPE_GeneralResourceContainer * localResTypeController )
{
    GPE_SceneBasicClass::build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController);
    return true;
}

void GPE_SceneAnimation::add_typed_elements()
{
    if( PANEL_INSPECTOR!=NULL )
    {
        PANEL_INSPECTOR->add_gui_element( animationInEditor , true );
        PANEL_INSPECTOR->add_gui_element( startPositionField ,true );
        PANEL_INSPECTOR->add_gui_element( animationSpeedField ,true );
        PANEL_INSPECTOR->add_gui_element( useCustomSpeed ,true );
    }
}

void GPE_SceneAnimation::calculate_size()
{

}

void GPE_SceneAnimation::process_elements()
{
    GPE_SceneBasicClass::process_elements();
}

void GPE_SceneAnimation::render_branch()
{

}

bool GPE_SceneAnimation::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{

}
