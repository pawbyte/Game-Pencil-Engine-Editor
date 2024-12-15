/*
gpe_scene_text_class.cpp
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


#include "gpe_scene_text_class.h"

GPE_SceneText::GPE_SceneText( pawgui::widget_resource_container *pFolder )
{
    branch_type_id = gpe::branch_type::STEXT;
    dual_scaleClass = false;
    angleField->scale_width( 0.4 );

    iconTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/font.png") ;
    projectParentFolder = pFolder;

    if( projectParentFolder!=nullptr)
    {
        fontInEditor  = new pawgui::widget_drop_down_resource_menu( "Font",projectParentFolder->find_resource_from_name( gpe::resource_type_names_plural[ gpe::resource_type_font]),-1,true);
        fontInEditor->set_width(192);
    }
    else
    {
        fontInEditor  = nullptr;
    }
    text = "";
    fontId = -1;

    if( x_scaleField!=nullptr)
    {
        x_scaleField->set_label("Text scale:");
    }

    fontHalign = gpe::fa_left;
    fontValign = gpe::fa_top;

    sceneTextHalign = new pawgui::widget_button_iconbar( 32,true);
    sceneTextHalign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-left.png","Align left", gpe::fa_left, false );
    sceneTextHalign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-center.png","Align center", gpe::fa_center, false );
    sceneTextHalign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-right.png","Align right", gpe::fa_right, false );


    sceneTextValign = new pawgui::widget_button_iconbar( 32,true);
    sceneTextValign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-left.png","Align top", gpe::fa_top, false );
    sceneTextValign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-center.png","Align middle", gpe::fa_middle, false );
    sceneTextValign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-right.png","Align bottom", gpe::fa_bottom, false );

    defaultTextElement = new pawgui::widget_input_text("","Custom text...");
    defaultTextElement->set_label("Default Text:");
    textSection = new pawgui::widget_input_text("","Text Section");
    textSection->set_label("Text Section(localization):");

    textKey = new pawgui::widget_input_text("","Text Key");
    textKey->set_label("Text Key(localization):");
}

GPE_SceneText::~GPE_SceneText()
{
    if( sceneTextHalign !=nullptr )
    {
        delete sceneTextHalign;
        sceneTextHalign = nullptr;
    }
    if( sceneTextValign !=nullptr )
    {
        delete sceneTextValign;
        sceneTextValign = nullptr;
    }
    if( defaultTextElement !=nullptr )
    {
        delete defaultTextElement;
        defaultTextElement = nullptr;
    }
    if( textSection !=nullptr )
    {
        delete textSection;
        textSection = nullptr;
    }
    if( textKey !=nullptr )
    {
        delete textKey;
        textKey = nullptr;
    }
}

void GPE_SceneText::add_typed_elements()
{
    if( panel_inspector!=nullptr )
    {
        panel_inspector->add_gui_element( fontInEditor, true );
        panel_inspector->add_gui_element( sceneTextHalign, true );
        panel_inspector->add_gui_element( sceneTextValign, true );
        panel_inspector->add_gui_element( defaultTextElement, true );
        panel_inspector->add_gui_element( textSection, true );
        panel_inspector->add_gui_element( textKey, true );
    }
}


void GPE_SceneText::calculate_size()
{

}

void GPE_SceneText::process_elements()
{
    GPE_SceneBasicClass::process_elements();
    if( sceneTextHalign!=nullptr )
    {
        fontHalign = sceneTextHalign->get_tab_id();
    }
    if( sceneTextValign!=nullptr )
    {
        fontValign = sceneTextValign->get_tab_id();
    }
}


void GPE_SceneText::render_branch( )
{
    //Avoid the time waste and don't continue if alpha is too low.
    if( branchAlpha!=nullptr && branchAlpha->get_value() < 5)
    {
        return;
    }

    if( spm == nullptr || defaultTextElement == nullptr || fontInEditor == nullptr )
    {
        return;
    }

    spm->tempRect->x = ceil( (x_pos*spm->zoomValue-spm->currentCamera->x*spm->zoomValue) );
    spm->tempRect->y = ceil( (y_pos*spm->zoomValue-spm->currentCamera->y*spm->zoomValue) );

    gpe::font_base * foundFont = nullptr;
    pawgui::widget_resource_container * fContainer = fontInEditor->get_selected_container();
    bool fontRendered = false;
    if( fContainer!=nullptr && fContainer->get_held_resource() )
    {
        fontResource * actualFontObject = (fontResource*) fContainer->get_held_resource();
        if( actualFontObject!=nullptr)
        {
            fontRendered = actualFontObject->render_held_font( spm->tempRect->x, spm->tempRect->y, defaultTextElement->get_string(), branchColor->get_color(), fontHalign, fontValign, angle, spm->zoomValue * x_scale, branchAlpha->get_value() );
        }
    }
    //Renders using default for preview
    if( !fontRendered && gpe::font_default!=nullptr )
    {
        gpe::font_default->render_text_special( spm->tempRect->x, spm->tempRect->y, "DEFAULT:  "+defaultTextElement->get_string(), branchColor->get_color(), fontHalign, fontValign, angle, spm->zoomValue * x_scale, branchAlpha->get_value() );
    }
}

bool GPE_SceneText::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=nullptr && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"   [GPE_GameText=";
        *fileTarget << fontId << ",";
        if( x_posField!=nullptr)
        {
            x_posField->make_valid_number(0);
            *fileTarget <<  x_posField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( y_posField!=nullptr)
        {
            y_posField->make_valid_number(0);
            *fileTarget << y_posField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( sceneTextValign!=nullptr)
        {
            *fileTarget << sceneTextValign->get_tab_id() << ",";
        }
        else
        {
            *fileTarget << "0,";
        }

        if( sceneTextValign!=nullptr)
        {
            *fileTarget << sceneTextValign->get_tab_id() << ",";
        }
        else
        {
            *fileTarget << "0,";
        }
        *fileTarget << widget_name+",,]";
        std::string customtTextTabsStr = pawgui::generate_tabs( nestedFoldersIn+1 );
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
    if( panel_inspector!=nullptr )
    {
        panel_inspector->add_gui_element( fontInEditor, true );
        panel_inspector->add_gui_element( sceneTextHalign, true );
        panel_inspector->add_gui_element( sceneTextValign, true );
        panel_inspector->add_gui_element( checkWrapText, true );
        panel_inspector->add_gui_element( customTextElement, true );
    }
}

GPE_SceneMultilineText::GPE_SceneMultilineText( pawgui::widget_resource_container *pFolder )
{
    dual_scaleClass = false;
    iconTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/paragraph.png") ;
    projectParentFolder = pFolder;

    if( projectParentFolder!=nullptr)
    {
        fontInEditor  = new pawgui::widget_drop_down_resource_menu( "Font",projectParentFolder->find_resource_from_name( gpe::resource_type_names_plural[ gpe::resource_type_font]),-1,true);
        fontInEditor->set_width(192);
    }
    else
    {
        fontInEditor  = nullptr;
    }

    branch_type_id = gpe::branch_type::TEXT;
    text = "";
    fontId = -1;

    fontHalign = gpe::fa_left;
    fontValign = gpe::fa_top;

    sceneTextHalign = new pawgui::widget_button_iconbar( 32,true);
    sceneTextHalign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-left.png","Align left", gpe::fa_left, false );
    sceneTextHalign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-center.png","Align center", gpe::fa_center, false );
    sceneTextHalign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-right.png","Align right", gpe::fa_right, false );

    sceneTextValign = new pawgui::widget_button_iconbar( 32,true);
    sceneTextValign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-left.png","Align top", gpe::fa_top, false );
    sceneTextValign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-center.png","Align middle", gpe::fa_middle, false );
    sceneTextValign->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/align-right.png","Align bottom", gpe::fa_bottom, false );

    customTextElement = new pawgui::widget_text_editor( false );
    customTextElement->show_buttonBar = false;
    customTextElement->isCodeEditor = false;

    textSection = new pawgui::widget_input_text("","Text Section");
    textSection->set_label("Text Section(localization):");

    textKey = new pawgui::widget_input_text("","Text Key");
    textKey->set_label("Text Key(localization):");
    checkWrapText = new pawgui::widget_checkbox("Automatically wrap text","", false );
}

GPE_SceneMultilineText::~GPE_SceneMultilineText()
{
    if( checkWrapText !=nullptr )
    {
        delete checkWrapText;
        checkWrapText = nullptr;
    }
    if( sceneTextHalign !=nullptr )
    {
        delete sceneTextHalign;
        sceneTextHalign = nullptr;
    }
    if( sceneTextValign !=nullptr )
    {
        delete sceneTextValign;
        sceneTextValign = nullptr;
    }
    if( customTextElement !=nullptr )
    {
        delete customTextElement;
        customTextElement = nullptr;
    }
    if( textSection !=nullptr )
    {
        delete textSection;
        textSection = nullptr;
    }
    if( textKey !=nullptr )
    {
        delete textKey;
        textKey = nullptr;
    }
}



void GPE_SceneMultilineText::calculate_size()
{

}

void GPE_SceneMultilineText::process_elements()
{
    GPE_SceneBasicClass::process_elements();
    if( sceneTextHalign!=nullptr )
    {
        fontHalign = sceneTextHalign->get_tab_id();
    }
    if( sceneTextValign!=nullptr )
    {
        fontValign = sceneTextValign->get_tab_id();
    }
}

void GPE_SceneMultilineText::render_branch( )
{

}

bool GPE_SceneMultilineText::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=nullptr && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"   [GPE_GameText=";
        *fileTarget << fontId << ",";
        if( x_posField!=nullptr)
        {
            x_posField->make_valid_number(0);
            *fileTarget <<  x_posField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( y_posField!=nullptr)
        {
            y_posField->make_valid_number(0);
            *fileTarget << y_posField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( sceneTextValign!=nullptr)
        {
            *fileTarget << sceneTextValign->get_tab_id() << ",";
        }
        else
        {
            *fileTarget << "0,";
        }

        if( sceneTextValign!=nullptr)
        {
            *fileTarget << sceneTextValign->get_tab_id() << ",";
        }
        else
        {
            *fileTarget << "0,";
        }

        if( checkWrapText!=nullptr)
        {
            *fileTarget << checkWrapText->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        *fileTarget << widget_name+",,]";
        std::string customtTextTabsStr = pawgui::generate_tabs( nestedFoldersIn+1 );
        *fileTarget << customtTextTabsStr+"[CUSTOM_TEXT]\n";
        *fileTarget << customtTextTabsStr+text+"\n";
        *fileTarget << customtTextTabsStr+"[/CUSTOM_TEXT]\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        *fileTarget << nestedTabsStr+"   [/GPE_GameText]\n";

        return true;
    }
    return false;
}
