/*
gpe_scene_light_class.cpp
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
#include "gpe_scene_light_class.h"

GPE_SceneBasicLight::GPE_SceneBasicLight(GPE_GeneralResourceContainer *pFolder)
{
    iconTexture = guiRCM->texture_add( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/lightbulb-o.png") ;

    branchType = BRANCH_TYPE_LIGHT;
    if( branchColor!=NULL)
    {
        branchColor->set_label("Light Color");
    }
    lightIsActive = new GPE_CheckBoxBasic("Is Active","Active When Scene Starts", true );

    projectParentFolder = pFolder;
    if( projectParentFolder!=NULL)
    {
        lightInEditor = new GPE_DropDown_Resouce_Menu( "Light Source",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_LIGHT]+"s"),-1,true);
        lightInEditor->set_width(192);
    }
    else
    {
        lightInEditor = NULL;
    }
}

GPE_SceneBasicLight::~GPE_SceneBasicLight()
{
    if( lightIsActive!=NULL )
    {
        delete lightIsActive;
        lightIsActive = NULL;
    }
}

void GPE_SceneBasicLight::add_typed_elements()
{
    if( PANEL_INSPECTOR!=NULL )
    {
        PANEL_INSPECTOR->add_gui_element( lightIsActive, true );
    }
}

bool GPE_SceneBasicLight::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  GPE_GeneralResourceContainer * localResTypeController )
{
    GPE_SceneBasicClass::build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController);
    return true;
}

void GPE_SceneBasicLight::process_elements()
{
    GPE_SceneBasicClass::process_elements();
}

void GPE_SceneBasicLight::render_branch()
{

}

bool GPE_SceneBasicLight::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"   GPE_AmbientLight=";
        if( xPosField!=NULL)
        {
            xPosField->make_valid_number(0);
            *fileTarget <<  xPosField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-0,";
        }
        if( yPosField!=NULL)
        {
            yPosField->make_valid_number(0);
            *fileTarget << yPosField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-0,";
        }
        if( branchColor!=NULL)
        {
            *fileTarget << branchColor->get_hex_string() << ",";
        }
        else
        {
            *fileTarget << "#FFFF00,";
        }
        if( branchAlpha!=NULL)
        {
            *fileTarget << int_to_string( branchAlpha->get_value() )<< ",";
        }
        else
        {
            *fileTarget << "255,";
        }
        if( lightIsActive!=NULL)
        {
            *fileTarget << lightIsActive->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "1,";
        }
        *fileTarget << opName+",,\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        return true;
    }
    return false;
}
