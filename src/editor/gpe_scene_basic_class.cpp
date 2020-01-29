/*
gpe_scene_basic_class.cpp
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

#include "gpe_scene_basic_class.h"


GPE_SceneBasicClass::GPE_SceneBasicClass()
{
    dualScaleClass = true;
    projectParentFolder = NULL;
    parentSceneBranch = NULL;
    isBeingMoved = false;
    layerParentId = -1;
    xPos = yPos = 0;
    width = height = 32;
    xScale = yScale = 1;
    angle =  0;
    branchType = BRANCH_TYPE_BASIC_SCENE_ELEMENT;
    xPivot = width/2;
    yPivot = height/2;
    isLocked = false;

    branchGlobalId = new GPE_Label_Text("Global Id:" + int_to_string(globalId),"Automatically assigned on create...");
    angleField = new GPE_TextInputNumber("",false,-720,720);
    angleField->set_name("Angle");
    angleField->set_label("Angle");
    angleField->set_number(angle);

    xPosField = new GPE_TextInputNumber("","");
    xPosField->set_label("X-Pos");
    xPosField->set_label("X-Start");
    xPosField->scale_width( 0.4 );

    yPosField = new GPE_TextInputNumber("","");
    yPosField->set_label("Y-Pos");
    yPosField->set_label("Y-Start");
    yPosField->scale_width( 0.4 );

    xScaleField = new GPE_TextInputNumber("",false);
    xScaleField->set_string("1.0");
    xScaleField->set_label("X-Scale");
    xScaleField->scale_width( 0.4 );

    yScaleField = new GPE_TextInputNumber("",false);
    yScaleField->set_string("1.0");
    yScaleField->set_label("Y-Scale");
    yScaleField->scale_width( 0.4 );

    branchNameField = new GPE_TextInputBasic("name","branch name");
    branchNameField->set_string("0");
    branchNameField->set_label("Name");

    branchColor = new GPE_Input_Field_Color();
    branchColor->set_name("Color");
    branchColor->set_label("Color");

    branchAlpha = new GPE_Slider_XAxis(255, 0, 255);
    iconTexture = guiRCM->texture_add( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/tree.png") ;
}

GPE_SceneBasicClass::~GPE_SceneBasicClass()
{
    reset_components();
    if( xPosField!=NULL )
    {
        delete xPosField;
        xPosField = NULL;
    }

    if( yPosField!=NULL )
    {
        delete yPosField;
        yPosField = NULL;
    }
    if( angleField!=NULL)
    {
        delete angleField;
        angleField = NULL;
    }

    if( xScaleField!=NULL )
    {
        delete xScaleField;
        xScaleField = NULL;
    }

    if( yScaleField!=NULL )
    {
        delete yScaleField;
        yScaleField = NULL;
    }

    if( branchColor!=NULL)
    {
        delete branchColor;
        branchColor = NULL;
    }

    if( branchAlpha!=NULL)
    {
        delete branchAlpha;
        branchAlpha = NULL;
    }

    if( branchNameField!=NULL )
    {
        delete branchNameField;
        branchNameField = NULL;
    }
}

void GPE_SceneBasicClass::add_basic_elements()
{
    if( PANEL_INSPECTOR!=NULL)
    {
        if( branchType != BRANCH_TYPE_LAYER )
        {
            PANEL_INSPECTOR->add_gui_element(branchNameField , true );
        }
        PANEL_INSPECTOR->add_gui_element( branchGlobalId , true );

        if( branchType != BRANCH_TYPE_LAYER && branchType!= BRANCH_TYPE_GROUP)
        {
            if( !isLocked )
            {
                PANEL_INSPECTOR->add_gui_element( xPosField , false );
                PANEL_INSPECTOR->add_gui_element( yPosField , true );
            }
            if( dualScaleClass )
            {
                /*
                Temporarily disabled
                PANEL_INSPECTOR->add_gui_element( angleField , true );
                */
                PANEL_INSPECTOR->add_gui_element( xScaleField , false );
                PANEL_INSPECTOR->add_gui_element( yScaleField , true );
                PANEL_INSPECTOR->add_gui_element( branchColor  , true );
                PANEL_INSPECTOR->add_gui_element( branchAlpha  , true );
            }
            else
            {
                /*
                Temporarily disabled
                PANEL_INSPECTOR->add_gui_element( angleField , true );
                */
                PANEL_INSPECTOR->add_gui_element( xScaleField , true );
                PANEL_INSPECTOR->add_gui_element( branchColor  , true );
                PANEL_INSPECTOR->add_gui_element( branchAlpha  , true );
            }
        }
    }
}

void GPE_SceneBasicClass::add_typed_elements()
{

}

bool GPE_SceneBasicClass::add_scene_branch( GPE_SceneBasicClass * branch, bool changeId, bool openBranch  )
{
    if( branch!=NULL )
    {
        if( branchType == BRANCH_TYPE_LAYER || branchType == BRANCH_TYPE_GROUP )
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
        else if( parentSceneBranch!=NULL )
        {
            return parentSceneBranch->add_scene_branch( branch, changeId, openBranch );
        }
    }
    return false;
}

GPE_ObjectComponent * GPE_SceneBasicClass::add_variable(GPE_GeneralGuiElement * newVariable)
{
    if( newVariable!=NULL)
    {
        std::string newVarName = newVariable->get_name();
        if( (int)newVarName.size() > 0)
        {
            bool foundVal = false;
            GPE_ObjectComponent * tField = NULL;
            for( int i = 0; i < (int)objCustomValPairs.size(); i++)
            {
                tField = objCustomValPairs[i];
                if( tField!=NULL)
                {
                    if( tField->get_name()==newVarName)
                    {
                        foundVal = true;
                        return NULL;
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
    return NULL;
}

bool GPE_SceneBasicClass::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  GPE_GeneralResourceContainer * localResTypeController )
{
    //Loops thru all of his sub-elements
    std::string nestedTabsStr = generate_tabs( leftTabAmount );
    *fileTarget << nestedTabsStr << "newBranch.init_basic(";
    int_to_string(xPos)+","+int_to_string(yPos)+","+
    int_to_string(angle)+","+
    int_to_string(xScale)+","+int_to_string(yScale)+");\n";
    int componentsSize = (int)objCustomValPairs.size();
    if( componentsSize > 0 )
    {
        GPE_ObjectComponent * objCustVariableField = NULL;
        std::string customObjLabel;
        std::string customObjValue;
        for( int compItr = 0; compItr < componentsSize -1; compItr++)
        {
            objCustVariableField = objCustomValPairs.at(compItr);
            if( objCustVariableField !=NULL)
            {
                customObjLabel = objCustVariableField->get_name();
                customObjValue = objCustVariableField->get_plain_string();

                if( (int)customObjLabel.size() > 0)
                {
                    if( is_alnum(customObjLabel,false,true)==true)
                    {
                        if( (int)customObjValue.size()==0)
                        {
                            customObjValue = "0";
                        }
                        *fileTarget << nestedTabsStr << "newBranch.add_component( {"+customObjLabel+", "+customObjValue+");\n";
                    }
                }
            }
        }
    }
    int subElementsSize = (int)subElements.size();
    if( subElementsSize > 0 )
    {
        GPE_SpecialMenu_Branch * tBranch = NULL;
        GPE_SceneBasicClass * tbasicBranch = NULL;
        for( int iSubEl = 0; iSubEl < subElementsSize; iSubEl++)
        {
            tBranch = subElements.at(iSubEl);
            if( tBranch!=NULL && tBranch->get_type() >= BRANCH_TYPE_BASIC_SCENE_ELEMENT )
            {
                tbasicBranch = (GPE_SceneBasicClass * )tBranch;
                tbasicBranch->build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController );
            }
        }
    }
    return true;
}

void GPE_SceneBasicClass::calculate_size()
{

}

void GPE_SceneBasicClass::process_elements()
{
    xPos = xPosField->get_held_number();
    yPos = yPosField->get_held_number();
    xScale = xScaleField->get_held_number();
    yScale = yScaleField->get_held_number();
    angle = angleField->get_held_number();
}

void GPE_SceneBasicClass::refresh_branch()
{
    if( branchGlobalId!=NULL)
    {
        branchGlobalId->set_name( "Global Id:" + int_to_string(globalId) );
    }
}

void GPE_SceneBasicClass::reset_components()
{
    GPE_ObjectComponent * tField = NULL;
    for( int i = (int)objCustomValPairs.size()-1; i >=0; i--)
    {
        tField = objCustomValPairs[i];
        if( tField!=NULL )
        {
            delete tField;
            tField = NULL;
        }
    }
    objCustomValPairs.clear();
}

void GPE_SceneBasicClass::render_branch()
{
    int subElCount  = (int)subElements.size();
    GPE_SpecialMenu_Branch * cBranch = NULL;
    for(  int myItr = 0; myItr < subElCount; myItr++ )
    {
        cBranch = subElements[myItr];
        if( cBranch!=NULL)
        {
            cBranch->render_branch();
        }
    }
}

float GPE_SceneBasicClass::rotx( float rx, float ry )
{
    return xPos + gpemath::lengthdir_x(rx, angle) * xScale - gpemath::lengthdir_x(ry, angle - 90) * yScale;
}

float GPE_SceneBasicClass::roty( float rx, float ry )
{
    return yPos + gpemath::lengthdir_y(rx, angle) * xScale - gpemath::lengthdir_y(ry, angle - 90) * yScale;
}

bool GPE_SceneBasicClass::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    int componentCount = (int) objCustomValPairs.size();
    if( componentCount > 0)
    {
        if( fileTarget!=NULL && fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            std::string componentNestedTabsStr = generate_tabs( nestedFoldersIn +1 );
            *fileTarget << nestedTabsStr+"[GPECustomFields]\n";
            GPE_ObjectComponent * tObjCustomProperty = NULL;
            for( int iComponent = 0; iComponent < componentCount ; iComponent++ )
            {
                tObjCustomProperty = objCustomValPairs[iComponent];
                if( tObjCustomProperty!=NULL)
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

void GPE_SceneBasicClass::set_angle( float newAngle )
{
    angle = newAngle;
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

void GPE_SceneBasicClass::set_name( std::string newName )
{
    GPE_SpecialMenu_Branch::set_name( newName);
    if( branchNameField!=NULL )
    {
        branchNameField->set_string( newName );
    }
}

void GPE_SceneBasicClass::set_position( float x, float y)
{
    x = round(x);
    y = round(y);
    xPos = x;
    yPos = y;
    xPosField->set_number( xPos );
    yPosField->set_number( yPos );
}

bool GPE_SceneBasicClass::under_mouse(float mx, float my )
{
    if( branchType > BRANCH_TYPE_GROUP )
    {
        float relativeMouseX = mx - xPos;
        float relativeMouseY = my - yPos;
        int lmx = ( gpemath::lengthdir_x( relativeMouseX, -angle) - gpemath::lengthdir_x( relativeMouseY, -(angle - 90) ) ) / abs( xScale );
        int lmy = ( gpemath::lengthdir_y( relativeMouseX, -angle)  - gpemath::lengthdir_y( relativeMouseY, -(angle - 90) ) ) / abs( yScale );
        if (lmx >= -xPivot && lmx <= width-xPivot)
        {
            if(  lmy >= -yPivot && lmy <= height-yPivot)
            {
                return true;
            }
        }
    }
    /*
    else if( point_within( mx, my, xPos, yPos, xPos+width, yPos+height) )
    {
        return true;
    }
    */
    return false;
}
