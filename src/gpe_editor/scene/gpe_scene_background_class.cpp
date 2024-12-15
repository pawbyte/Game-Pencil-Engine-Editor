/*
gpe_scene_background_class.cpp
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


#include "gpe_scene_background_class.h"

GPE_SceneBackground::GPE_SceneBackground(pawgui::widget_resource_container *pFolder)
{
    branch_type_id = gpe::branch_type::BACKGROUND;
    iconTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/image.png") ;

    textureId = -1;
    texRes = nullptr;
    inFront = false;
    tileHori = false;
    tileVert = false;
    strech_bg = false;
    bg_xspeed = 0;
    bg_yspeed =0;
    projectParentFolder = pFolder;

    if( projectParentFolder!=nullptr)
    {
        backgroundInEditor = new pawgui::widget_drop_down_resource_menu( "Background Texture",projectParentFolder->find_resource_from_name( gpe::resource_type_names_plural[ gpe::resource_type_texture]),-1,true);
        backgroundInEditor->set_width(192);
    }
    else
    {
        backgroundInEditor = nullptr;
    }

    bgHorSpeedField = new pawgui::widget_input_number("","");
    bgHorSpeedField->set_string("0");
    bgHorSpeedField->set_label("H-Speed");
    bgHorSpeedField->scale_width( 0.4 );

    bgVertSpeedField = new pawgui::widget_input_number("","");
    bgVertSpeedField->set_string("0");
    bgVertSpeedField->set_label("V-Speed");
    bgVertSpeedField->scale_width( 0.4 );

    checkTileHori = new pawgui::widget_checkbox("Tile-Hori","Repeat Tile Horizontally to fill the scene" );
    checkTileVert = new pawgui::widget_checkbox("Tile-Vert","Repeat Tile Vertically to fill the scene" );
    checkStretch = new pawgui::widget_checkbox("Stretch BG","Stretch across the entire scene" );

}

GPE_SceneBackground::~GPE_SceneBackground()
{

}

void GPE_SceneBackground::add_typed_elements()
{
    if( panel_inspector!=nullptr )
    {
        panel_inspector->add_gui_element( backgroundInEditor, true );
        panel_inspector->add_gui_element( bgHorSpeedField, false );
        panel_inspector->add_gui_element( bgVertSpeedField, true );

        panel_inspector->add_gui_element( checkTileHori, true );
        panel_inspector->add_gui_element( checkTileVert, true );
        panel_inspector->add_gui_element( checkStretch, true );
    }
}

void GPE_SceneBackground::calculate_size()
{

}

void GPE_SceneBackground::process_elements()
{
    GPE_SceneBasicClass::process_elements();
    textureId = backgroundInEditor->get_selected_id();
    if(backgroundInEditor->get_selected_id() > 0)
    {
        bg_xspeed = bgHorSpeedField->get_held_number();
        bg_yspeed = bgVertSpeedField->get_held_number();
        tileHori =checkTileHori->is_clicked();
        tileVert =checkTileVert->is_clicked();
        strech_bg =checkStretch->is_clicked();
    }
}

void GPE_SceneBackground::render_branch()
{
    if( spm->cSceneTexList == nullptr )
    {
        return;
    }
    pawgui::widget_resource_container * texTypeContainer = spm->cSceneTexList->find_resource_from_id(textureId );
    if( texTypeContainer!=nullptr)
    {
        spm->tempRect->x = floor( (x_pos*spm->zoomValue)- spm->cameraFloorXPos);
        spm->tempRect->y = floor( (y_pos*spm->zoomValue)- spm->cameraFloorYPos );
        textureResource * texRes = (textureResource*) texTypeContainer->get_held_resource();

        int bgXItr = 0, bgYItr= 0;
        if( texRes!=nullptr && texRes->textureInEditor!=nullptr )
        {
            if( texRes->textureInEditor->get_width() >0 && texRes->textureInEditor->get_height() >0 )
            {
                spm->tempRect->x = floor( (x_pos* spm->zoomValue)- spm->cameraFloorXPos );
                spm->tempRect->y = floor( (y_pos* spm->zoomValue)- spm->cameraFloorYPos );
                if( strech_bg)
                {
                    texRes->textureInEditor->render_tex_resized( 0,0,spm->currentCamera->w,spm->currentCamera->h,nullptr, branchColor->get_color(), branchAlpha->get_value()  );
                }
                else if(tileHori  )
                {
                    for( bgXItr = spm->tempRect->x; bgXItr <=( spm->sWidth)*spm->zoomValue; bgXItr+=texRes->textureInEditor->get_width()* spm->zoomValue )
                    {
                        if( tileVert)
                        {
                            for(bgYItr = spm->tempRect->y; bgYItr <= ( spm->sHeight )* spm->zoomValue; bgYItr+=texRes->textureInEditor->get_height()* spm->zoomValue )
                            {
                                texRes->textureInEditor->render_tex_scaled( floor(bgXItr),floor(bgYItr), spm->zoomValue, spm->zoomValue, nullptr, branchColor->get_color(), branchAlpha->get_value() );
                            }
                        }
                        else
                        {
                            texRes->textureInEditor->render_tex_scaled( floor( bgXItr ),floor(spm->tempRect->y), spm->zoomValue, spm->zoomValue, nullptr, branchColor->get_color(), branchAlpha->get_value() );
                        }
                    }
                }
                else if(tileVert)
                {
                    for(bgYItr = spm->tempRect->y; bgYItr <=( spm->sHeight)*spm->zoomValue; bgYItr+=texRes->textureInEditor->get_height()* spm->zoomValue )
                    {
                        texRes->textureInEditor->render_tex_scaled( floor(spm->tempRect->x),floor( bgYItr ),spm->zoomValue ,spm->zoomValue,nullptr, branchColor->get_color(), branchAlpha->get_value()  );
                    }
                }
                else
                {
                    texRes->textureInEditor->render_tex_scaled( floor(spm->tempRect->x),floor(spm->tempRect->y), spm->zoomValue , spm->zoomValue ,nullptr, branchColor->get_color(), branchAlpha->get_value()  );
                }
            }
        }
    }
    pawgui::widget_branch::render_branch();
}

bool GPE_SceneBackground::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn  )
{
    if( fileTarget!=nullptr && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"   background=";
        if( backgroundInEditor!=nullptr)
        {
            *fileTarget << backgroundInEditor->get_selected_name() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
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
        if( bgHorSpeedField!=nullptr)
        {
            bgHorSpeedField->make_valid_number(0);
            *fileTarget << bgHorSpeedField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( bgVertSpeedField!=nullptr)
        {
            bgVertSpeedField->make_valid_number(0);
            *fileTarget << bgVertSpeedField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( checkTileHori!=nullptr)
        {
            *fileTarget << checkTileHori->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( checkTileVert!=nullptr)
        {
            *fileTarget << checkTileVert->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( checkStretch!=nullptr)
        {
            *fileTarget << checkStretch->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        *fileTarget << widget_name+",";
        *fileTarget << "\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        return true;
    }
    return false;
}
