/*
gpe_scene_background_class.cpp
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


#include "gpe_scene_background_class.h"

GPE_SceneBackground::GPE_SceneBackground(GPE_GeneralResourceContainer *pFolder)
{
    branchType = BRANCH_TYPE_BACKGROUND;
    iconTexture = guiRCM->texture_add( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/image.png") ;

    textureId = -1;
    texRes = NULL;
    inFront = false;
    tileHori = false;
    tileVert = false;
    stretchBG = false;
    bgXSpeed = 0;
    bgYSpeed =0;
    projectParentFolder = pFolder;

    if( projectParentFolder!=NULL)
    {
        backgroundInEditor = new GPE_DropDown_Resouce_Menu( "Background Texture",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE]+"s"),-1,true);
        backgroundInEditor->set_width(192);
    }
    else
    {
        backgroundInEditor = NULL;
    }

    bgHorSpeedField = new GPE_TextInputNumber("","");
    bgHorSpeedField->set_string("0");
    bgHorSpeedField->set_label("H-Speed");
    bgHorSpeedField->scale_width( 0.4 );

    bgVertSpeedField = new GPE_TextInputNumber("","");
    bgVertSpeedField->set_string("0");
    bgVertSpeedField->set_label("V-Speed");
    bgVertSpeedField->scale_width( 0.4 );

    checkTileHori = new GPE_CheckBoxBasic("Tile-Hori","Repeat Tile Horizontally to fill the scene" );
    checkTileVert = new GPE_CheckBoxBasic("Tile-Vert","Repeat Tile Vertically to fill the scene" );
    checkStretch = new GPE_CheckBoxBasic("Stretch BG","Stretch across the entire scene" );

}

GPE_SceneBackground::~GPE_SceneBackground()
{

}

void GPE_SceneBackground::add_typed_elements()
{
    if( PANEL_INSPECTOR!=NULL )
    {
        PANEL_INSPECTOR->add_gui_element( backgroundInEditor, true );
        PANEL_INSPECTOR->add_gui_element( bgHorSpeedField, false );
        PANEL_INSPECTOR->add_gui_element( bgVertSpeedField, true );

        PANEL_INSPECTOR->add_gui_element( checkTileHori, true );
        PANEL_INSPECTOR->add_gui_element( checkTileVert, true );
        PANEL_INSPECTOR->add_gui_element( checkStretch, true );
    }
}

bool GPE_SceneBackground::build_intohtml5_file( std::ofstream * fileTarget, int leftTabAmount,  GPE_GeneralResourceContainer * localResTypeController )
{
    GPE_GeneralResourceContainer * allTexturesFolder = localResTypeController->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE]+"s");
    if( allTexturesFolder == NULL)
    {
        GPE_SceneBasicClass::build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController);
        return false;
    }
    std::string nestedTabsStr = generate_tabs( leftTabAmount );
     GPE_GeneralResourceContainer * tempBgResource = allTexturesFolder->find_resource_from_id( textureId);
    if( tempBgResource!=NULL)
    {
        *fileTarget << nestedTabsStr << "newBranch = _scn_temp_layer.scnStartBackgrounds.push( {bgTexId:" <<int_to_string(tempBgResource->exportBuildGlobalId) << ",";

        *fileTarget << "bgXPos: " << int_to_string( xPos) << ",";
        *fileTarget << "bgYPos: " << int_to_string( yPos) << ",";
        *fileTarget << "bgXSpeed: " << int_to_string(bgXSpeed) << ",";
        *fileTarget << "bgYSpeed: " << int_to_string(bgYSpeed) << ",";
        *fileTarget << "bgTileHori: " << int_to_string(tileHori) << ",";
        *fileTarget << "bgTileVert: " << int_to_string(tileVert) << ",";
        *fileTarget << "bgStartStretch: " << int_to_string(stretchBG);
        *fileTarget << "} ); \n";
    }
    else
    {
        *fileTarget << nestedTabsStr << "newBranch = _scn_temp_layer.scnStartBackground = -1; \n";
    }
    GPE_SceneBasicClass::build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController);
    return true;
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
        bgXSpeed = bgHorSpeedField->get_held_number();
        bgYSpeed = bgVertSpeedField->get_held_number();
        tileHori =checkTileHori->is_clicked();
        tileVert =checkTileVert->is_clicked();
        stretchBG =checkStretch->is_clicked();
    }
}

void GPE_SceneBackground::render_branch()
{
    if( spm->cSceneTexList == NULL )
    {
        return;
    }
    GPE_GeneralResourceContainer * texTypeContainer = spm->cSceneTexList->find_resource_from_id(textureId );
    if( texTypeContainer!=NULL)
    {
        spm->tempRect->x = floor( (xPos*spm->zoomValue)- spm->cameraFloorXPos);
        spm->tempRect->y = floor( (yPos*spm->zoomValue)- spm->cameraFloorYPos );
        textureResource * texRes = (textureResource*) texTypeContainer->get_held_resource();

        int bgXItr = 0, bgYItr= 0;
        if( texRes!=NULL && texRes->textureInEditor!=NULL )
        {
            if( texRes->textureInEditor->get_width() >0 && texRes->textureInEditor->get_height() >0 )
            {
                spm->tempRect->x = floor( (xPos* spm->zoomValue)- spm->cameraFloorXPos );
                spm->tempRect->y = floor( (yPos* spm->zoomValue)- spm->cameraFloorYPos );
                if( stretchBG)
                {
                    texRes->textureInEditor->render_tex_resized( 0,0,spm->currentCamera->w,spm->currentCamera->h,NULL, branchColor->get_color(), branchAlpha->get_value()  );
                }
                else if(tileHori  )
                {
                    for( bgXItr = spm->tempRect->x; bgXItr <=( spm->sWidth)*spm->zoomValue; bgXItr+=texRes->textureInEditor->get_width()* spm->zoomValue )
                    {
                        if( tileVert)
                        {
                            for(bgYItr = spm->tempRect->y; bgYItr <= ( spm->sHeight )* spm->zoomValue; bgYItr+=texRes->textureInEditor->get_height()* spm->zoomValue )
                            {
                                texRes->textureInEditor->render_tex_scaled( floor(bgXItr),floor(bgYItr), spm->zoomValue, spm->zoomValue, NULL, branchColor->get_color(), branchAlpha->get_value() );
                            }
                        }
                        else
                        {
                            texRes->textureInEditor->render_tex_scaled( floor( bgXItr ),floor(spm->tempRect->y), spm->zoomValue, spm->zoomValue, NULL, branchColor->get_color(), branchAlpha->get_value() );
                        }
                    }
                }
                else if(tileVert)
                {
                    for(bgYItr = spm->tempRect->y; bgYItr <=( spm->sHeight)*spm->zoomValue; bgYItr+=texRes->textureInEditor->get_height()* spm->zoomValue )
                    {
                        texRes->textureInEditor->render_tex_scaled( floor(spm->tempRect->x),floor( bgYItr ),spm->zoomValue ,spm->zoomValue,NULL, branchColor->get_color(), branchAlpha->get_value()  );
                    }
                }
                else
                {
                    texRes->textureInEditor->render_tex_scaled( floor(spm->tempRect->x),floor(spm->tempRect->y), spm->zoomValue , spm->zoomValue ,NULL, branchColor->get_color(), branchAlpha->get_value()  );
                }
            }
        }
    }
    GPE_SpecialMenu_Branch::render_branch();
}

bool GPE_SceneBackground::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn  )
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"   GPE_Background=";
        if( backgroundInEditor!=NULL)
        {
            *fileTarget << backgroundInEditor->get_selected_name() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
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
        if( bgHorSpeedField!=NULL)
        {
            bgHorSpeedField->make_valid_number(0);
            *fileTarget << bgHorSpeedField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( bgVertSpeedField!=NULL)
        {
            bgVertSpeedField->make_valid_number(0);
            *fileTarget << bgVertSpeedField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( checkTileHori!=NULL)
        {
            *fileTarget << checkTileHori->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( checkTileVert!=NULL)
        {
            *fileTarget << checkTileVert->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        if( checkStretch!=NULL)
        {
            *fileTarget << checkStretch->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "-1,";
        }
        *fileTarget << opName+",";
        *fileTarget << "\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        return true;
    }
    return false;
}
