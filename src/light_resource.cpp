/*
light_resource.cpp
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

#include "light_resource.h"


lightResource::lightResource(GPE_GeneralResourceContainer * pFolder)
{
    tempAngleExtra = 0;
    autorotateAngle = new GPE_CheckBoxBasic("Test Rotate","Rotate Light", true );
    //bottomPaneList = new GPE_GuiElementList();
    projectParentFolder = pFolder;
    myAmbientLight = new GPE_BasicLight();
    myDirectionLight = new GPE_Directionight();
    myPointLight = new GPE_PointLight();

    //emitterTexture->load_new_texture(APP_DIRECTORY_NAME+"resources/gfx/sprites/c_snow.png",-1,true );
    browseTextureButton = new GPE_ToolLabelButton("Browse...","Browse for texture image" );
    clearTextureButton = new GPE_ToolLabelButton("Clear Image...","Clears texture and reverts to shape" );

    textureLabel = new GPE_Label_Text("Direction Light Texture","Enter below:" );
    texturePreviewImgLabel = new GPE_Label_Image( NULL, "Direction Light" );
    textureLocationField = new GPE_TextInputBasic("","Texture Location...");

    lightType = new GPE_RadioButtonControllerBasic("Light Type", true );
    lightType->add_menu_option("Ambient Light","ambient_light", gpe_light_type_ambient, false );
    lightType->add_menu_option("Direction Light","direction_light", gpe_light_type_direction, false );
    lightType->add_menu_option("Point Light","point", gpe_light_type_point, true );

    lightColorField = new GPE_Input_Field_Color("Light Color","Light Color");
    lightColorField->set_color_from_rgb( c_yellow );

    lightRadiusField = new GPE_TextInputNumber("Radius(pixels)", true, 0, 1024 );
    lightRadiusField->set_number( 128 );

    lightSecondRadiusField = new GPE_TextInputNumber("Secondary Radius(pixels)", true, 0, 1024 );
    lightSecondRadiusField->set_number( 128 );

    lightThirdRadiusField = new GPE_TextInputNumber("Third Radius(pixels)", true, 0, 1024 );
    lightThirdRadiusField->set_number( 0 );

    lightLengthField = new GPE_TextInputNumber("Length (pixels)", true, 0, 1024 );
    lightLengthField->set_number( 256 );

    lightWidthField = new GPE_TextInputNumber("Width (pixels)", true, 0, 1024 );
    lightWidthField->set_number( 64 );

    lightIntensityField = new GPE_TextInputNumber("Intensity (pixels)", true, 0, 255 );
    lightIntensityField->set_number( 228 );

    lightSecondIntensityField = new GPE_TextInputNumber("Secondary Light Intensity (pixels)", true, 0, 255 );
    lightSecondIntensityField->set_number( 192 );

    lightThirdIntensityField = new GPE_TextInputNumber("Secondary Light Intensity (pixels)", true, 0, 255 );
    lightThirdIntensityField->set_number( 160 );

    lightFlickerTimeField = new GPE_TextInputNumber("Light Flicker Timer (milliseconds)", true, 0, 9000 );
    lightFlickerTimeField->set_number( 255 );

    lightFlickerAmountField = new GPE_TextInputNumber("Flicker Random Amount (pixels)", true, 0, 1024 );
    lightFlickerAmountField->set_number( 2 );

    lightUseFlicker = new GPE_CheckBoxBasic("Flicker Light","Check to flicker the light in intervals", false );

    lightDirectionField = new GPE_TextInputNumber("Direction (degrees)", false, -360, 360 );
    lightDirectionField->set_number( 270 );

    directionLightTexture = NULL;
}

lightResource::~lightResource()
{
    if( myAmbientLight!=NULL )
    {
        delete myAmbientLight;
        myAmbientLight = NULL;
    }
    if( myPointLight!=NULL )
    {
        delete myPointLight;
        myPointLight = NULL;
    }
    if( myDirectionLight!=NULL )
    {
        delete myDirectionLight;
        myDirectionLight = NULL;
    }
}

bool lightResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount )
{

}

bool lightResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void lightResource::compile_cpp()
{

}

bool lightResource::get_mouse_coords(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    areaMouseXPos = 0;
    areaMouseYPos = 0;
    if( viewedSpace!=NULL && cam!=NULL )
    {
        if( point_within(   input->mouse_x,input->mouse_y,
                            viewedSpace->x - cam->x,
                            viewedSpace->y - cam->y,
                            viewedSpace->x + viewedSpace->w - cam->x,
                            viewedSpace->y + viewedSpace->h - cam->y ) )
        {

            areaMouseXPos = (input->mouse_x-viewedSpace->x ) + cam->x;
            areaMouseYPos = (input->mouse_y-viewedSpace->y ) + cam->y;

            return true;
        }
    }
    return false;
}

void lightResource::handle_scrolling()
{

}

void lightResource::load_image(std::string newFileName, bool autoProcess )
{
    if( newFileName.size() <= 4)
    {
        return;
    }
    textureLocationField->set_string( newFileName );
    if( directionLightTexture==NULL )
    {
        directionLightTexture = new GPE_Texture();
    }
    directionLightTexture->load_new_texture( newFileName );
    directionLightTexture->set_blend_mode( blend_mode_add );
    myDirectionLight->lightTexture = directionLightTexture;
    texturePreviewImgLabel->set_image( directionLightTexture );
    texturePreviewImgLabel->set_width( 64 );
    texturePreviewImgLabel->set_height( 64 );
    if( autoProcess)
    {
        copy_file( newFileName, fileToDir(parentProjectName)+"/gpe_project/resources/lights/"+ get_local_from_global_file(newFileName) );
    }
}

void lightResource::preprocess_self(std::string alternatePath )
{
    if( resourcePostProcessed ==false || file_exists(alternatePath))
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Loading Light", resourceName );
        }

        std::string otherColContainerName = "";

        std::string newFileIn = "";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/lights/";
        if( file_exists(alternatePath) )
        {
            newFileIn = alternatePath;
            soughtDir = get_path_from_file(newFileIn);
        }
        else
        {
            newFileIn = soughtDir + resourceName+".gpf";
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        //GPE_Report("Loading sprite - "+newFileIn);
        //If the level file could be loaded
        double foundFileVersion = -1;
        if( !gameResourceFileIn.fail() )
        {
            //GPE_Report("Procesing sprite file...");
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string currLineToBeProcessed;

                int i = 0;
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = trim_left_inplace(currLine);
                    currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

                    if( foundFileVersion <=0)
                    {
                        //Empty Line skipping is only allowed at the top of the file
                        if(!currLineToBeProcessed.empty() )
                        {
                            //Comment skipping is only allowed at the top of the file
                            if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                            {
                                //searches for an equal character and parses through the variable
                                equalPos=currLineToBeProcessed.find_first_of("=");
                                if(equalPos!=(int)std::string::npos)
                                {
                                    //if the equalPos is present, then parse on through and carryon
                                    keyString = currLineToBeProcessed.substr(0,equalPos);
                                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( keyString=="Version")
                                    {
                                        foundFileVersion = string_to_double(valString);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion <= 2)
                    {
                        //Begin processing the file.
                        if(!currLineToBeProcessed.empty() )
                        {
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                                if( keyString=="ResourceName")
                                {
                                    renameBox->set_string(valString);
                                }
                                else if( keyString=="DirectionLightTexture")
                                {
                                    load_image( soughtDir+valString);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void lightResource::prerender_self( )
{

}

void lightResource::process_data_fields(double versionToProcess )
{

}

void lightResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera( viewedSpace);
    cam = GPE_find_camera( cam );

    if( PANEL_GENERAL_EDITOR!=NULL )
    {
        PANEL_GENERAL_EDITOR->clear_panel();

        PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);

        PANEL_GENERAL_EDITOR->add_gui_element( lightType,true);
        PANEL_GENERAL_EDITOR->add_gui_element(lightColorField,true);
        PANEL_GENERAL_EDITOR->add_gui_element( lightIntensityField,true);

        if( lightType->get_selected_value() == gpe_light_type_direction )
        {
            PANEL_GENERAL_EDITOR->add_gui_element(lightLengthField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(lightWidthField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(lightDirectionField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(autorotateAngle,true);

            PANEL_GENERAL_EDITOR->add_gui_element(textureLabel,true);
            PANEL_GENERAL_EDITOR->add_gui_element(textureLocationField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(browseTextureButton,true);
            PANEL_GENERAL_EDITOR->add_gui_element(clearTextureButton,true);
            PANEL_GENERAL_EDITOR->add_gui_element(texturePreviewImgLabel,true);
        }
        else if( lightType->get_selected_value() == gpe_light_type_point )
        {
            PANEL_GENERAL_EDITOR->add_gui_element(lightRadiusField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(lightSecondRadiusField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(lightSecondIntensityField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(lightThirdRadiusField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(lightThirdIntensityField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(lightUseFlicker,true);
            PANEL_GENERAL_EDITOR->add_gui_element(lightFlickerTimeField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(lightFlickerAmountField,true);
        }
        else
        {

        }

        PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);
        PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
        PANEL_GENERAL_EDITOR->process_self();

        if( lightType->get_selected_value() == gpe_light_type_direction )
        {
            if( browseTextureButton!=NULL && browseTextureButton->is_clicked() )
            {
                std::string newTextureLocation = GPE_GetOpenFileName("Choose Light Texture","Images",MAIN_GUI_SETTINGS->fileOpenTextureDir);
                if( file_exists( newTextureLocation) && file_is_image(newTextureLocation) )
                {
                    load_image( newTextureLocation, true);
                }
            }
            else if( clearTextureButton!=NULL && clearTextureButton->is_clicked() )
            {
                if( directionLightTexture!=NULL )
                {
                    delete directionLightTexture;
                    directionLightTexture = NULL;
                }
                textureLocationField->set_string( "" );
                myDirectionLight->lightTexture = NULL;
                texturePreviewImgLabel->set_image( NULL );
                texturePreviewImgLabel->set_width( 16 );
                texturePreviewImgLabel->set_height( 16 );
            }
        }

        if( confirmResourceButton->is_clicked() )
        {
            save_resource();
        }
        else if( cancelResourceButton->is_clicked() )
        {
            if( GPE_Display_Basic_Prompt("Are you sure you will like to reverse light changes?","This will load in data from save-file!", true )== DISPLAY_QUERY_YES )
            {
                resourcePostProcessed = false;
                preprocess_self();
            }
        }
    }

    tempAngleExtra+=1;

    while( tempAngleExtra < -360.d  )
    {
        tempAngleExtra += 360.d ;
    }
    while( tempAngleExtra >= 360.d )
    {
        tempAngleExtra -= 360.d;
    }

    if( myAmbientLight!=NULL )
    {
        myAmbientLight->lightColor->change_and_verify_rgba( lightColorField->get_r(), lightColorField->get_g(), lightColorField->get_b(), 255 );
        myAmbientLight->lightIntensity = lightIntensityField->get_held_int();
    }

    if( myDirectionLight!=NULL )
    {
        myDirectionLight->lightColor->change_and_verify_rgba( lightColorField->get_r(), lightColorField->get_g(), lightColorField->get_b(), 255 );
        myDirectionLight->lightIntensity = lightIntensityField->get_held_int();
        if( autorotateAngle->is_clicked() )
        {
            myDirectionLight->set_direction( tempAngleExtra );
        }
        else
        {
            myDirectionLight->set_direction( lightDirectionField->get_held_int() );
        }
        myDirectionLight->lightWidth = lightWidthField->get_held_int();
        myDirectionLight->lightLength = lightLengthField->get_held_int();
    }

    if( myPointLight!=NULL )
    {
        myPointLight->lightColor->change_and_verify_rgba( lightColorField->get_r(), lightColorField->get_g(), lightColorField->get_b(), 255 );
        myPointLight->lightIntensity = lightIntensityField->get_held_int();
        myPointLight->lightRadius = lightRadiusField->get_held_int();
        myPointLight->secondRadius = lightSecondRadiusField->get_held_int();
        myPointLight->secondRadiusIntensity = lightSecondIntensityField->get_held_int();

        if( lightUseFlicker!=NULL && lightUseFlicker->is_clicked() )
        {
            myPointLight->setup_flicker( lightFlickerTimeField->get_held_int(), lightFlickerAmountField->get_held_int() );
            myPointLight->update_light( GPE->get_delta_time() );

        }
        else
        {
            myPointLight->disable_flicker();
        }
    }
}

void lightResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera( viewedSpace);
    cam = GPE_find_camera( cam );
    gcanvas->render_rectangle( 0,0,viewedSpace->w, viewedSpace->h, GPE_MAIN_THEME->Program_Color, false, 255 );
    //if( forceRedraw )
    {
        GPE_LOGO->render_align( viewedSpace->w/2, viewedSpace->h/2,FA_CENTER, FA_MIDDLE, NULL, NULL, 255 );
        gcanvas->resize_ligting_overlay( viewedSpace->w, viewedSpace->h );
        gcanvas->switch_ligting_overlay( true );
        CURRENT_RENDERER->reset_viewpoint( );
        gcanvas->render_rectangle(0,0,viewedSpace->w, viewedSpace->h, c_black, false, 255 );
        //gcanvas->set_artist_blend_mode( blend_mode_add );
        bool mouseInView = get_mouse_coords( viewedSpace, cam );
        if( lightType->get_selected_value() == gpe_light_type_direction )
        {
            if(  mouseInView && !tempAngleExtra )
            {
                myDirectionLight->render_light_at( areaMouseXPos, areaMouseYPos, 1 );
            }
            else
            {
                myDirectionLight->render_light_at( viewedSpace->w/2, viewedSpace->h/2, 1 );
            }
        }
        else if( lightType->get_selected_value() == gpe_light_type_point )
        {
            if(  mouseInView )
            {
                myPointLight->render_light_at( areaMouseXPos, areaMouseYPos, 1 );

            }
            else
            {
                myPointLight->render_light_at( viewedSpace->w/2, viewedSpace->h/2, 1 );
            }
        }
        else if( myAmbientLight!=NULL )
        {
            myAmbientLight->render_light( 1, viewedSpace );
        }

        //gcanvas->render_circle_color( editorView.w/2, editorView.h/2, c_white, 255 );
        gcanvas->switch_ligting_overlay( false );
        CURRENT_RENDERER->reset_viewpoint( );
        CURRENT_RENDERER->set_viewpoint( viewedSpace );
        gcanvas->render_ligting_overlay( 0,0 );
        gfs->render_text( viewedSpace->w/2, viewedSpace->h-32, "C++ Feature. Currently not supported in HTML5 runtime",GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT, FA_CENTER, FA_BOTTOM, 255);

    }
}

void lightResource::revert_data_fields()
{

}

void lightResource::save_resource(std::string alternatePath, int backupId )
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Light", resourceName );
    }

    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = get_path_from_file(alternatePath);
    if( path_exists(soughtDir) )
    {
        newFileOut = alternatePath;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/lights/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the particle file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);

            if( lightType!=NULL )
            {
                newSaveDataFile << "LightType=" + int_to_string(lightType->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "LightType="+int_to_string(gpe_light_type_point)+"\n";
            }
            if( lightColorField!=NULL )
            {
                newSaveDataFile << "LightColor=" + lightColorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "LightColor=#FFF\n";
            }
            if( lightIntensityField!=NULL )
            {
                newSaveDataFile << "LightIntensity[0]=" +int_to_string( lightIntensityField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "LightIntensity[0]=255\n";
            }
            if( lightSecondIntensityField!=NULL )
            {
                newSaveDataFile << "LightIntensity[1]=" +int_to_string( lightSecondIntensityField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "LightIntensity[1]=192\n";
            }
            if( lightThirdIntensityField!=NULL )
            {
                newSaveDataFile << "LightIntensity[2]=" +int_to_string( lightThirdIntensityField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "LightIntensity[2]=128\n";
            }
            if( lightRadiusField!=NULL )
            {
                newSaveDataFile << "PointLightRadius[0]=" +int_to_string( lightRadiusField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "PointLightRadius[0]=128\n";
            }
            if( lightSecondRadiusField!=NULL )
            {
                newSaveDataFile << "PointLightRadius[1]=" +int_to_string( lightSecondRadiusField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "PointLightRadius[1]=128\n";
            }

            if( lightThirdRadiusField!=NULL )
            {
                newSaveDataFile << "PointLightRadius[2]=" +int_to_string( lightThirdRadiusField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "PointLightRadius[2]=128\n";
            }

            if( lightLengthField!=NULL )
            {
                newSaveDataFile << "DirectionLightLength=" +int_to_string( lightLengthField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "DirectionLightLength=128\n";
            }
            if( lightWidthField!=NULL )
            {
                newSaveDataFile << "DirectionLightWidth=" +int_to_string( lightWidthField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "DirectionLightWidth=64\n";
            }
            if( lightDirectionField!=NULL )
            {
                newSaveDataFile << "DirectionLightAngle=" +int_to_string( lightDirectionField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "DirectionLightAngle=270\n";
            }
            if( textureLocationField!=NULL )
            {
                newSaveDataFile << "DirectionLightTexture=" + get_local_from_global_file( textureLocationField->get_string() )+"\n";
            }
            if( lightUseFlicker!=NULL )
            {
                newSaveDataFile << "LightFlicker=" + int_to_string( lightUseFlicker->is_clicked() )+"\n";
            }
            if( lightFlickerTimeField!=NULL )
            {
                newSaveDataFile << "LightFlickerTime=" + int_to_string( lightFlickerTimeField->get_held_int() )+"\n";
            }
            if( lightFlickerAmountField!=NULL )
            {
                newSaveDataFile << "LightFlickerAmount=" + int_to_string( lightFlickerAmountField->get_held_int() )+"\n";
            }
            if( autorotateAngle!=NULL )
            {
                newSaveDataFile << "RotateInEditor=" + int_to_string( autorotateAngle->is_clicked() )+"\n";
            }
            //newSaveDataFile << "AudioGroup="+audioGroupName->get_string()+"\n";

            newSaveDataFile.close();
            if( !usingAltSaveSource )
            {
                isModified = false;
            }
        }
        else
        {
            GPE_Main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void lightResource::update_box(int newX, int newY, int newW, int newH )
{

}

bool lightResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn  )
{
    //resourceNameLabel->set_name(renameBox->get_string() );
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            preprocess_self();
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Light2D=" << resourceName << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}


