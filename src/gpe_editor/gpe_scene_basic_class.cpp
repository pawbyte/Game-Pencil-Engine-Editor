/*
gpe_scene_basic_class.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_scene_basic_class.h"


GPE_SceneBasicClass::GPE_SceneBasicClass()
{
    dual_scaleClass = true;
    projectParentFolder = nullptr;
    parentSceneBranch = nullptr;
    isBeingMoved = false;
    layerParentId = -1;
    x_pos = y_pos = 0;
    width = height = 32;
    x_scale = y_scale = 1;
    angle =  0;
    branch_type_id = gpe::branch_type::BASIC_SCENE_ELEMENT;
    xPivot = width/2;
    yPivot = height/2;
    isLocked = false;

    branchGlobalId = new pawgui::widget_label_text ("Global Id:" + stg_ex::int_to_string(globalId),"Automatically assigned on create...");
    angleField = new pawgui::widget_input_number("",false,-720,720);
    angleField->set_name("Angle");
    angleField->set_label("Angle");
    angleField->set_number(angle);

    x_posField = new pawgui::widget_input_number("","");
    x_posField->set_label("X-Pos");
    x_posField->set_label("X-Start");
    x_posField->scale_width( 0.4 );

    y_posField = new pawgui::widget_input_number("","");
    y_posField->set_label("Y-Pos");
    y_posField->set_label("Y-Start");
    y_posField->scale_width( 0.4 );

    x_scaleField = new pawgui::widget_input_number("",false);
    x_scaleField->set_string("1.0");
    x_scaleField->set_label("X-scale");
    x_scaleField->scale_width( 0.4 );

    y_scaleField = new pawgui::widget_input_number("",false);
    y_scaleField->set_string("1.0");
    y_scaleField->set_label("Y-scale");
    y_scaleField->scale_width( 0.4 );

    branchNameField = new pawgui::widget_input_text("name","branch name");
    branchNameField->set_string("0");
    branchNameField->set_label("Name");

    branchTagField = new pawgui::widget_input_text("tag","branch tag");
    branchTagField->set_string("");
    branchTagField->set_label("Tag");

    branchColor = new pawgui::gpe_widget_color_picker();
    branchColor->set_name("Color");
    branchColor->set_label("Color");

    branchAlpha = new pawgui::widget_slide_xaxis(255, 0, 255);
    iconTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/tree.png") ;
}

GPE_SceneBasicClass::~GPE_SceneBasicClass()
{
    reset_components();
    if( x_posField!=nullptr )
    {
        delete x_posField;
        x_posField = nullptr;
    }

    if( y_posField!=nullptr )
    {
        delete y_posField;
        y_posField = nullptr;
    }
    if( angleField!=nullptr)
    {
        delete angleField;
        angleField = nullptr;
    }

    if( x_scaleField!=nullptr )
    {
        delete x_scaleField;
        x_scaleField = nullptr;
    }

    if( y_scaleField!=nullptr )
    {
        delete y_scaleField;
        y_scaleField = nullptr;
    }

    if( branchColor!=nullptr)
    {
        delete branchColor;
        branchColor = nullptr;
    }

    if( branchAlpha!=nullptr)
    {
        delete branchAlpha;
        branchAlpha = nullptr;
    }

    if( branchNameField!=nullptr )
    {
        delete branchNameField;
        branchNameField = nullptr;
    }

    if( branchTagField!=nullptr )
    {
        delete branchTagField;
        branchTagField = nullptr;
    }
}

void GPE_SceneBasicClass::add_basic_elements()
{
    if( panel_inspector!=nullptr)
    {
        if( branch_type_id != gpe::branch_type::LAYER )
        {
            panel_inspector->add_gui_element(branchNameField , true );
        }
        panel_inspector->add_gui_element( branchGlobalId , true );

        if( branch_type_id != gpe::branch_type::LAYER && branch_type_id!= gpe::branch_type::GROUP)
        {
            if( !isLocked )
            {
                panel_inspector->add_gui_element( x_posField , false );
                panel_inspector->add_gui_element( y_posField , true );
            }
            panel_inspector->add_gui_element( branchTagField, true );
            if( dual_scaleClass )
            {
                /*
                Temporarily disabled
                panel_inspector->add_gui_element( angleField , true );
                */
                panel_inspector->add_gui_element( x_scaleField , false );
                panel_inspector->add_gui_element( y_scaleField , true );
                panel_inspector->add_gui_element( branchColor  , true );
                panel_inspector->add_gui_element( branchAlpha  , true );
            }
            else
            {
                /*
                Temporarily disabled
                panel_inspector->add_gui_element( angleField , true );
                */
                panel_inspector->add_gui_element( x_scaleField , true );
                panel_inspector->add_gui_element( branchColor  , true );
                panel_inspector->add_gui_element( branchAlpha  , true );
            }
        }
    }
}

void GPE_SceneBasicClass::add_typed_elements()
{

}

bool GPE_SceneBasicClass::add_scene_branch( GPE_SceneBasicClass * branch, bool changeId, bool openBranch  )
{
    if( branch!=nullptr )
    {
        if( branch_type_id == gpe::branch_type::LAYER || branch_type_id == gpe::branch_type::GROUP )
        {
            add_branch( branch, changeId);
            branch->layerParentId = layerParentId;
            branch->parentSceneBranch = this;
            if( openBranch )
            {
                open_and_view();
            }
            branch->refresh_branch();
            return true;
        }
        else if( parentSceneBranch!=nullptr )
        {
            return parentSceneBranch->add_scene_branch( branch, changeId, openBranch );
        }
    }
    return false;
}

GPE_ObjectComponent * GPE_SceneBasicClass::add_variable(pawgui::widget_basic * newVariable)
{
    if( newVariable!=nullptr)
    {
        std::string newVarName = newVariable->get_name();
        if( (int)newVarName.size() > 0)
        {
            bool foundVal = false;
            GPE_ObjectComponent * tField = nullptr;
            for( int i = 0; i < (int)objCustomValPairs.size(); i++)
            {
                tField = objCustomValPairs[i];
                if( tField!=nullptr)
                {
                    if( tField->get_name()==newVarName)
                    {
                        foundVal = true;
                        return nullptr;
                    }
                }
            }
            if( foundVal==false)
            {
                GPE_ObjectComponent * newComponent = new GPE_ObjectComponent();
                newComponent->set_component(newVariable);
                objCustomValPairs.push_back(newComponent);
                return newComponent;
            }
        }
    }
    return nullptr;
}

void GPE_SceneBasicClass::calculate_size()
{

}

void GPE_SceneBasicClass::process_elements()
{
    x_pos = x_posField->get_held_number();
    y_pos = y_posField->get_held_number();
    x_scale = x_scaleField->get_held_number();
    y_scale = y_scaleField->get_held_number();
    angle = angleField->get_held_number();
}

void GPE_SceneBasicClass::refresh_branch()
{
    if( branchGlobalId!=nullptr)
    {
        branchGlobalId->set_name( "Global Id:" + stg_ex::int_to_string(globalId) );
    }
}

void GPE_SceneBasicClass::reset_components()
{
    GPE_ObjectComponent * tField = nullptr;
    for( int i = (int)objCustomValPairs.size()-1; i >=0; i--)
    {
        tField = objCustomValPairs[i];
        if( tField!=nullptr )
        {
            delete tField;
            tField = nullptr;
        }
    }
    objCustomValPairs.clear();
}

void GPE_SceneBasicClass::render_branch()
{
    int subElCount  = (int)sub_elements.size();
    pawgui::widget_branch * current_branch = nullptr;
    for(  int myItr = 0; myItr < subElCount; myItr++ )
    {
        current_branch = sub_elements[myItr];
        if( current_branch!=nullptr)
        {
            current_branch->render_branch();
        }
    }
}

float GPE_SceneBasicClass::rotx( float rx, float ry )
{
    return x_pos + semath::lengthdir_x(rx, angle) * x_scale - semath::lengthdir_x(ry, angle - 90) * y_scale;
}

float GPE_SceneBasicClass::roty( float rx, float ry )
{
    return y_pos + semath::lengthdir_y(rx, angle) * x_scale - semath::lengthdir_y(ry, angle - 90) * y_scale;
}

bool GPE_SceneBasicClass::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    int componentCount = (int) objCustomValPairs.size();
    if( componentCount > 0)
    {
        if( fileTarget!=nullptr && fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            std::string componentNestedTabsStr = pawgui::generate_tabs( nestedFoldersIn +1 );
            *fileTarget << nestedTabsStr+"[GPECustomFields]\n";
            GPE_ObjectComponent * tObjCustomProperty = nullptr;
            for( int iComponent = 0; iComponent < componentCount ; iComponent++ )
            {
                tObjCustomProperty = objCustomValPairs[iComponent];
                if( tObjCustomProperty!=nullptr)
                {
                    *fileTarget << componentNestedTabsStr+"[component]"+tObjCustomProperty->get_data()+"[/component]\n";
                }
            }
            *fileTarget <<nestedTabsStr+"[/GPECustomFields]\n";
            return true;
        }
    }
    return false;
}

void GPE_SceneBasicClass::set_angle( float new_angle )
{
    angle = new_angle;
    if( angle < -720 )
    {
        angle = 0;
    }
    else if( angle> 720 )
    {
        angle = 720;
    }
    angleField->set_number( angle );
}

void GPE_SceneBasicClass::set_name( std::string new_name )
{
    pawgui::widget_branch::set_name( new_name);
    if( branchNameField!=nullptr )
    {
        branchNameField->set_string( new_name );
    }
}

void GPE_SceneBasicClass::set_tag( std::string tag_new )
{
    if( branchTagField!=nullptr )
    {
        branchTagField->set_string( tag_new );
    }
}

void GPE_SceneBasicClass::set_position( float x, float y)
{
    x = round(x);
    y = round(y);
    x_pos = x;
    y_pos = y;
    x_posField->set_number( x_pos );
    y_posField->set_number( y_pos );
}

bool GPE_SceneBasicClass::under_mouse(float mx, float my )
{
    if( branch_type_id > gpe::branch_type::GROUP )
    {
        float relativeMouseX = mx - x_pos;
        float relativeMouseY = my - y_pos;
        int lmx = ( semath::lengthdir_x( relativeMouseX, -angle) - semath::lengthdir_x( relativeMouseY, -(angle - 90) ) ) / abs( x_scale );
        int lmy = ( semath::lengthdir_y( relativeMouseX, -angle)  - semath::lengthdir_y( relativeMouseY, -(angle - 90) ) ) / abs( y_scale );
        if (lmx >= -xPivot && lmx <= width-xPivot)
        {
            if(  lmy >= -yPivot && lmy <= height-yPivot)
            {
                return true;
            }
        }
    }
    /*
    else if( point_within( mx, my, x_pos, y_pos, x_pos+width, y_pos+height) )
    {
        return true;
    }
    */
    return false;
}
