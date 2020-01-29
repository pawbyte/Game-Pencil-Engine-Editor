/*
gpe_scene_text_class.cpp
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


#include "gpe_scene_text_class.h"

GPE_SceneText::GPE_SceneText( GPE_GeneralResourceContainer *pFolder )
{
    branchType = BRANCH_TYPE_STEXT;
    dualScaleClass = false;
    angleField->scale_width( 0.4 );

    iconTexture = guiRCM->texture_add( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/font.png") ;
    projectParentFolder = pFolder;

    if( projectParentFolder!=NULL)
    {
        fontInEditor  = new GPE_DropDown_Resouce_Menu( "Font",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_FONT]+"s"),-1,true);
        fontInEditor->set_width(192);
    }
    else
    {
        fontInEditor  = NULL;
    }
    text = "";
    fontId = -1;

    if( xScaleField!=NULL)
    {
        xScaleField->set_label("Text Scale:");
    }

    fontHalign = FA_LEFT;
    fontValign = FA_TOP;

    sceneTextHalign = new GPE_ToolIconButtonBar( 32,true);
    sceneTextHalign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-left.png","Align left", FA_LEFT, false );
    sceneTextHalign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-center.png","Align center", FA_CENTER, false );
    sceneTextHalign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-right.png","Align right", FA_RIGHT, false );


    sceneTextValign = new GPE_ToolIconButtonBar( 32,true);
    sceneTextValign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-left.png","Align top", FA_TOP, false );
    sceneTextValign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-center.png","Align middle", FA_MIDDLE, false );
    sceneTextValign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-right.png","Align bottom", FA_BOTTOM, false );

    defaultTextElement = new GPE_TextInputBasic("","Custom text...");
    defaultTextElement->set_label("Default Text:");
    textSection = new GPE_TextInputBasic("","Text Section");
    textSection->set_label("Text Section(localization):");

    textKey = new GPE_TextInputBasic("","Text Key");
    textKey->set_label("Text Key(localization):");
}

GPE_SceneText::~GPE_SceneText()
{
    if( sceneTextHalign !=NULL )
    {
        delete sceneTextHalign;
        sceneTextHalign = NULL;
    }
    if( sceneTextValign !=NULL )
    {
        delete sceneTextValign;
        sceneTextValign = NULL;
    }
    if( defaultTextElement !=NULL )
    {
        delete defaultTextElement;
        defaultTextElement = NULL;
    }
    if( textSection !=NULL )
    {
        delete textSection;
        textSection = NULL;
    }
    if( textKey !=NULL )
    {
        delete textKey;
        textKey = NULL;
    }
}

void GPE_SceneText::add_typed_elements()
{
    if( PANEL_INSPECTOR!=NULL )
    {
        PANEL_INSPECTOR->add_gui_element( fontInEditor, true );
        PANEL_INSPECTOR->add_gui_element( sceneTextHalign, true );
        PANEL_INSPECTOR->add_gui_element( sceneTextValign, true );
        PANEL_INSPECTOR->add_gui_element( defaultTextElement, true );
        PANEL_INSPECTOR->add_gui_element( textSection, true );
        PANEL_INSPECTOR->add_gui_element( textKey, true );
    }
}

bool GPE_SceneText::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  GPE_GeneralResourceContainer * localResTypeController )
{
    GPE_SceneBasicClass::build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController);
    return true;
}

void GPE_SceneText::calculate_size()
{

}

void GPE_SceneText::process_elements()
{
    GPE_SceneBasicClass::process_elements();
    if( sceneTextHalign!=NULL )
    {
        fontHalign = sceneTextHalign->get_tab_id();
    }
    if( sceneTextValign!=NULL )
    {
        fontValign = sceneTextValign->get_tab_id();
    }
}


void GPE_SceneText::render_branch( )
{
    //Avoid the time waste and don't continue if alpha is too low.
    if( branchAlpha!=NULL && branchAlpha->get_value() < 5)
    {
        return;
    }

    if( spm == NULL || defaultTextElement == NULL || fontInEditor == NULL )
    {
        return;
    }

    spm->tempRect->x = ceil( (xPos*spm->zoomValue-spm->currentCamera->x*spm->zoomValue) );
    spm->tempRect->y = ceil( (yPos*spm->zoomValue-spm->currentCamera->y*spm->zoomValue) );

    GPE_Font * foundFont = NULL;
    GPE_GeneralResourceContainer * fContainer = fontInEditor->get_selected_container();
    bool fontRendered = false;
    if( fContainer!=NULL && fContainer->get_held_resource() )
    {
        fontResource * actualFontObject = (fontResource*) fContainer->get_held_resource();
        if( actualFontObject!=NULL)
        {
            fontRendered = actualFontObject->render_held_font( spm->tempRect->x, spm->tempRect->y, defaultTextElement->get_string(), branchColor->get_color(), fontHalign, fontValign, angle, spm->zoomValue * xScale, branchAlpha->get_value() );
        }
    }
    //Renders using default for preview
    if( !fontRendered && GPE_DEFAULT_FONT!=NULL )
    {
        GPE_DEFAULT_FONT->render_text_special( spm->tempRect->x, spm->tempRect->y, "DEFAULT:  "+defaultTextElement->get_string(), branchColor->get_color(), fontHalign, fontValign, angle, spm->zoomValue * xScale, branchAlpha->get_value() );
    }
}

bool GPE_SceneText::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"   [GPE_GameText=";
        *fileTarget << fontId << ",";
        if( xPosField!=NULL)
        {
            xPosField->make_valid_number(0);
            *fileTarget <<  xPosField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( yPosField!=NULL)
        {
            yPosField->make_valid_number(0);
            *fileTarget << yPosField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( sceneTextValign!=NULL)
        {
            *fileTarget << sceneTextValign->get_tab_id() << ",";
        }
        else
        {
            *fileTarget << "0,";
        }

        if( sceneTextValign!=NULL)
        {
            *fileTarget << sceneTextValign->get_tab_id() << ",";
        }
        else
        {
            *fileTarget << "0,";
        }
        *fileTarget << opName+",,]";
        std::string customtTextTabsStr = generate_tabs( nestedFoldersIn+1 );
        *fileTarget << customtTextTabsStr+"[CUSTOM_TEXT]\n";
        *fileTarget << customtTextTabsStr+text+"\n";
        *fileTarget << customtTextTabsStr+"[/CUSTOM_TEXT]\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        *fileTarget << nestedTabsStr+"   [/GPE_GameText]\n";

        return true;
    }
    return false;
}

void GPE_SceneMultilineText::add_typed_elements()
{
    if( PANEL_INSPECTOR!=NULL )
    {
        PANEL_INSPECTOR->add_gui_element( fontInEditor, true );
        PANEL_INSPECTOR->add_gui_element( sceneTextHalign, true );
        PANEL_INSPECTOR->add_gui_element( sceneTextValign, true );
        PANEL_INSPECTOR->add_gui_element( checkWrapText, true );
        PANEL_INSPECTOR->add_gui_element( customTextElement, true );
    }
}

GPE_SceneMultilineText::GPE_SceneMultilineText( GPE_GeneralResourceContainer *pFolder )
{
    dualScaleClass = false;
    iconTexture = guiRCM->texture_add( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/paragraph.png") ;
    projectParentFolder = pFolder;

    if( projectParentFolder!=NULL)
    {
        fontInEditor  = new GPE_DropDown_Resouce_Menu( "Font",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_FONT]+"s"),-1,true);
        fontInEditor->set_width(192);
    }
    else
    {
        fontInEditor  = NULL;
    }

    branchType = BRANCH_TYPE_TEXT;
    text = "";
    fontId = -1;

    fontHalign = FA_LEFT;
    fontValign = FA_TOP;

    sceneTextHalign = new GPE_ToolIconButtonBar( 32,true);
    sceneTextHalign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-left.png","Align left", FA_LEFT, false );
    sceneTextHalign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-center.png","Align center", FA_CENTER, false );
    sceneTextHalign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-right.png","Align right", FA_RIGHT, false );

    sceneTextValign = new GPE_ToolIconButtonBar( 32,true);
    sceneTextValign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-left.png","Align top", FA_TOP, false );
    sceneTextValign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-center.png","Align middle", FA_MIDDLE, false );
    sceneTextValign->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/align-right.png","Align bottom", FA_BOTTOM, false );

    customTextElement = new GPE_TextAreaInputBasic( false );
    customTextElement->showButtonBar = false;
    customTextElement->isCodeEditor = false;

    textSection = new GPE_TextInputBasic("","Text Section");
    textSection->set_label("Text Section(localization):");

    textKey = new GPE_TextInputBasic("","Text Key");
    textKey->set_label("Text Key(localization):");
    checkWrapText = new GPE_CheckBoxBasic("Automatically wrap text","", false );
}

GPE_SceneMultilineText::~GPE_SceneMultilineText()
{
    if( checkWrapText !=NULL )
    {
        delete checkWrapText;
        checkWrapText = NULL;
    }
    if( sceneTextHalign !=NULL )
    {
        delete sceneTextHalign;
        sceneTextHalign = NULL;
    }
    if( sceneTextValign !=NULL )
    {
        delete sceneTextValign;
        sceneTextValign = NULL;
    }
    if( customTextElement !=NULL )
    {
        delete customTextElement;
        customTextElement = NULL;
    }
    if( textSection !=NULL )
    {
        delete textSection;
        textSection = NULL;
    }
    if( textKey !=NULL )
    {
        delete textKey;
        textKey = NULL;
    }
}


bool GPE_SceneMultilineText::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  GPE_GeneralResourceContainer * localResTypeController )
{
    GPE_SceneBasicClass::build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController);
    return true;
}

void GPE_SceneMultilineText::calculate_size()
{

}

void GPE_SceneMultilineText::process_elements()
{
    GPE_SceneBasicClass::process_elements();
    if( sceneTextHalign!=NULL )
    {
        fontHalign = sceneTextHalign->get_tab_id();
    }
    if( sceneTextValign!=NULL )
    {
        fontValign = sceneTextValign->get_tab_id();
    }
}

void GPE_SceneMultilineText::render_branch( )
{

}

bool GPE_SceneMultilineText::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"   [GPE_GameText=";
        *fileTarget << fontId << ",";
        if( xPosField!=NULL)
        {
            xPosField->make_valid_number(0);
            *fileTarget <<  xPosField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( yPosField!=NULL)
        {
            yPosField->make_valid_number(0);
            *fileTarget << yPosField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( sceneTextValign!=NULL)
        {
            *fileTarget << sceneTextValign->get_tab_id() << ",";
        }
        else
        {
            *fileTarget << "0,";
        }

        if( sceneTextValign!=NULL)
        {
            *fileTarget << sceneTextValign->get_tab_id() << ",";
        }
        else
        {
            *fileTarget << "0,";
        }

        if( checkWrapText!=NULL)
        {
            *fileTarget << checkWrapText->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        *fileTarget << opName+",,]";
        std::string customtTextTabsStr = generate_tabs( nestedFoldersIn+1 );
        *fileTarget << customtTextTabsStr+"[CUSTOM_TEXT]\n";
        *fileTarget << customtTextTabsStr+text+"\n";
        *fileTarget << customtTextTabsStr+"[/CUSTOM_TEXT]\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        *fileTarget << nestedTabsStr+"   [/GPE_GameText]\n";

        return true;
    }
    return false;
}
