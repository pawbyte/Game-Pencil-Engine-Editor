/*
gpe_gamecontroller_tester.cpp
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
#include "gpe_gamecontroller_tester.h"

gamePencilgamecontrollerTesterResource * MAIN_CONTROLLER_TESTER = NULL;


gamePencilgamecontrollerTesterResource::gamePencilgamecontrollerTesterResource()
{
    playerRadiusMin = 32;
    playerRadiusMax = 1024;
    for( int iController = 0; iController < gc_max_devices; iController++)
    {
        playerDeadZone[iController] = 0.25;
        playerRadius[iController] = 64;
        playerX[iController] = playerRadius[iController];
        playerY[iController] = playerRadius[iController];
        playerTestColor[iController]  = new GPE_Input_Field_Color("Player Color","Change to your favorite color",228,32,32 );
        playerFontColor[iController]  = new GPE_Input_Field_Color("Player Font Color","Change to your favorite color",228,228,228 );
        playerDeadZoneField[iController]  = new GPE_TextInputNumber("Joysticks DeadZone(From 0.0 to 1.0)",false,0, 1);
        playerDeadZoneField[iController] ->set_number( playerDeadZone[iController] );
        playerSizeRadius[iController]  = new GPE_TextInputNumber("Radius(in pixels)",true,playerRadiusMin, playerRadiusMax);
        playerSizeRadius[iController] ->set_number( playerRadius[iController] );

    }
    resourceFileName = "";
    resourceName = "gamecontroller Tester";
    resourceType = -8;
    subResouceId = -8;
    controllerInView = 0;
    detectControllersButton = new GPE_ToolPushButton("","Detect Controllers","Click to rediscover all controllers");
    vibrateControllerButton = new GPE_ToolPushButton("","Vibrate Controller","Click to vibrate current controller");
    previousControllerButton = new GPE_ToolIconButton(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrow-left.png","Previous Controller");
    nextControllerButton = new GPE_ToolIconButton(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrow-right.png","Next Controller");
    conrollerIdField  = new GPE_TextInputNumber("",true,0, gc_max_devices );
    conrollerIdField->set_string("0");
    conrollerIdField->set_width(64);

    controllerViewTitleLabel = new GPE_Label_Title("Current Controller:","Current Controller:");
    controllerNameLabel = new GPE_Label_Text("...","XYZ Controller....");
    requireInputToConnect = new GPE_CheckBoxBasic("Input required for connection","Delay controller connection state til a button is pressed(Useful for adapters)", input->requireControllerInputToConnect );
    showControllerInfo = new GPE_CheckBoxBasic("Show Debugger","Check to see controller info overlay",true );
    playerStats = new GPE_Label_Title("Player Stats","Player Stats");

}

gamePencilgamecontrollerTesterResource::~gamePencilgamecontrollerTesterResource()
{
    if( detectControllersButton != NULL)
    {
        delete detectControllersButton;
        detectControllersButton = NULL;
    }

    if( vibrateControllerButton != NULL)
    {
        delete vibrateControllerButton;
        vibrateControllerButton = NULL;
    }

    if( previousControllerButton != NULL)
    {
        delete previousControllerButton;
        previousControllerButton = NULL;
    }

    if( nextControllerButton != NULL)
    {
        delete nextControllerButton;
        nextControllerButton = NULL;
    }

    if( controllerViewTitleLabel != NULL)
    {
        delete controllerViewTitleLabel;
        controllerViewTitleLabel = NULL;
    }
    if( controllerNameLabel != NULL)
    {
        delete controllerNameLabel;
        controllerNameLabel = NULL;
    }
}

bool gamePencilgamecontrollerTesterResource::include_local_files( std::string pBuildDir , int buildType )
{

}

void gamePencilgamecontrollerTesterResource::prerender_self( )
{

}

void gamePencilgamecontrollerTesterResource::preprocess_self(std::string alternatePath)
{

}

void gamePencilgamecontrollerTesterResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera( viewedSpace);
    cam = GPE_find_camera( cam );

    controllerNameLabel->set_name( input->gamecontroller_detected_name( controllerInView ) );
    if( PANEL_GENERAL_EDITOR!=NULL )
    {
        PANEL_GENERAL_EDITOR->clear_panel();

        PANEL_GENERAL_EDITOR->add_gui_element(showControllerInfo,true);
        PANEL_GENERAL_EDITOR->add_gui_element(detectControllersButton,true);
        PANEL_GENERAL_EDITOR->add_gui_element(requireInputToConnect,true);
        PANEL_GENERAL_EDITOR->add_gui_element(controllerViewTitleLabel,true);
        PANEL_GENERAL_EDITOR->add_gui_element(controllerNameLabel,true);
        PANEL_GENERAL_EDITOR->add_gui_element(previousControllerButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element(conrollerIdField,false);
        PANEL_GENERAL_EDITOR->add_gui_element(nextControllerButton,true);
        PANEL_GENERAL_EDITOR->add_gui_element(playerStats,true);
        PANEL_GENERAL_EDITOR->add_gui_element(playerTestColor[controllerInView],true);
        PANEL_GENERAL_EDITOR->add_gui_element(playerFontColor[controllerInView],true);
        PANEL_GENERAL_EDITOR->add_gui_element(playerSizeRadius[controllerInView],true);
        PANEL_GENERAL_EDITOR->add_gui_element(playerDeadZoneField[controllerInView],true);
        PANEL_GENERAL_EDITOR->process_self();

        input->requireControllerInputToConnect = requireInputToConnect->is_clicked();
        if( detectControllersButton->is_clicked() )
        {
            input->detect_gamecontrollers( );
        }
        if( previousControllerButton->is_clicked() )
        {
            controllerInView--;
            if( controllerInView < 0 )
            {
                controllerInView = 0;
            }
            conrollerIdField->set_number( controllerInView );
        }
        else if( nextControllerButton->is_clicked() )
        {
            controllerInView++;
            if( controllerInView >= gc_max_devices )
            {
                controllerInView = gc_max_devices - 1;
            }
            conrollerIdField->set_number( controllerInView );
        }
        controllerInView = conrollerIdField->get_held_int();
    }
    playerRadiusMax = std::min( viewedSpace->w , viewedSpace->h )/2;



    for( int iController = 0; iController < gc_max_devices; iController++)
    {
        playerRadius[iController] = std::max( playerRadiusMin, playerSizeRadius[iController]->get_held_int() );
        playerRadius[iController] = std::min( playerRadiusMax, playerRadius[iController] );

        playerDeadZone[iController] = playerDeadZoneField[iController]->get_held_number() ;
        if(playerDeadZone[iController] > 1.f )
        {
            playerDeadZone[iController] = 1.f;
        }

        if(playerDeadZone[iController] < 0.f )
        {
            playerDeadZone[iController] = 0.f;
        }

        if( input->gamecontroller_get_axis_value( iController, 0) < -playerDeadZone[iController] )
        {
            playerX[iController] -= 2;
        }
        else if( input->gamecontroller_get_axis_value( iController, 0) > playerDeadZone[iController]  )
        {
            playerX[iController] += 2;
        }

        if( input->gamecontroller_get_axis_value( iController, 1) < -playerDeadZone[iController] )
        {
            playerY[iController] -= 2;
        }
        else if( input->gamecontroller_get_axis_value( iController, 1) > playerDeadZone[iController] )
        {
            playerY[iController] += 2;
        }

        if( playerX[iController] > viewedSpace->w - playerRadius[iController])
        {
            playerX[iController] = viewedSpace->w - playerRadius[iController];
        }

        if( playerY[iController] > viewedSpace->h - playerRadius[iController])
        {
            playerY[iController] = viewedSpace->h - playerRadius[iController];
        }

        if( playerX[iController] < playerRadius[iController])
        {
            playerX[iController] = playerRadius[iController];
        }
        if( playerY[iController] < playerRadius[iController])
        {
            playerY[iController] = playerRadius[iController];
        }
    }
}

void gamePencilgamecontrollerTesterResource::render_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{

    viewedSpace = GPE_find_camera( viewedSpace);
    cam = GPE_find_camera( cam );

    gcanvas->render_rectangle( 0,0,viewedSpace->w, viewedSpace->h, GPE_MAIN_THEME->Program_Color, false, 255 );
    for( int iController = gc_max_devices - 1; iController >=0 ; iController-- )
    {
        if( input->gamecontroller_connected( iController ) )
        {
            gcanvas->render_circle_color( playerX[iController], playerY[iController], playerRadius[iController],playerTestColor[iController]->get_color(), 255, false );
            gfs->render_text( playerX[ iController], playerY[iController], +"[ Player "+int_to_string(iController)+" ]",playerFontColor[iController]->get_color(), GPE_DEFAULT_FONT, FA_CENTER, FA_MIDDLE, 255);
        }
    }

    if( showControllerInfo!=NULL && showControllerInfo->is_clicked() )
    {
        std::string controllerOutputString = "Controller ["+int_to_string( controllerInView)+"]";

        if( input->gamecontroller_connected( controllerInView ) )
        {
            controllerOutputString+= " is connected";
            gfs->render_text( 32, 32, controllerOutputString,c_lime,GPE_DEFAULT_FONT, FA_LEFT, FA_BOTTOM, 255);

            controllerOutputString = "Buttons: ";
            for(int iCButton = 0; iCButton < gc_button_count; iCButton++)
            {
                controllerOutputString+= "["+int_to_string( input->check_gamecontroller_down( controllerInView, iCButton) ) +"], ";
            }
            gfs->render_text( 32, 64, controllerOutputString,GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT, FA_LEFT, FA_BOTTOM, 255);

            controllerOutputString = "Axis: ";
            for(int iCAxis = 0; iCAxis < gc_axes_count; iCAxis++)
            {
                controllerOutputString+= "["+float_to_string( input->gamecontroller_get_axis_value( controllerInView, iCAxis) ) +"], ";
            }
            gfs->render_text( 32, 96, controllerOutputString,GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT, FA_LEFT, FA_BOTTOM, 255);

        }
        else
        {
            controllerOutputString+= " is NOT connected";
            gfs->render_text( 32, 32, controllerOutputString,c_red,GPE_DEFAULT_FONT, FA_LEFT, FA_BOTTOM, 255);

        }
    }


    if( input->gamecontroller_count() > 0 )
    {
        gfs->render_text( viewedSpace->w/2, viewedSpace->h-64, "Viewing Controller ["+int_to_string(controllerInView+1)+"] out of ["+int_to_string(input->gamecontroller_count() )+"]",GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT, FA_CENTER, FA_BOTTOM, 255);
        gfs->render_text( viewedSpace->w/2, viewedSpace->h-32, input->game_controllers[controllerInView]->get_name(),GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT, FA_CENTER, FA_BOTTOM, 255);
    }
    else
    {
        gfs->render_text( viewedSpace->w/2, viewedSpace->h-64, "No game controllers detected!",GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT, FA_CENTER, FA_BOTTOM, 255);
        gfs->render_text( viewedSpace->w/2, viewedSpace->h-32, "Experimental gamecontroller Tester...",GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT, FA_CENTER, FA_BOTTOM, 255);

    }
}

void gamePencilgamecontrollerTesterResource::save_resource(std::string alternatePath, int backupId )
{

}

bool gamePencilgamecontrollerTesterResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{

}

