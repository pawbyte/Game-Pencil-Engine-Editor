/*
light_resource.cpp
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

#include "light_resource.h"


lightResource::lightResource(pawgui::widget_resource_container * pFolder)
{
    tempAngleExtra = 0;
    autorotateAngle = new pawgui::widget_checkbox("Test Rotate","Rotate Light", true );
    //bottomPaneList = new pawgui::widget_panel_list();
    projectParentFolder = pFolder;
    myAmbientLight = new gpe::light_basic_2d();
    myDirectionLight = new gpe::light_direction2d();
    myPointLight = new gpe::light_point2d();

    //emitterTexture->load_new_texture( app_directory_name+"resources/gfx/animations/c_snow.png",-1,true );
    browseTexture_button = new pawgui::widget_button_label("Browse...","Browse for texture image" );
    clearTexture_button = new pawgui::widget_button_label("Clear Image...","Clears texture and reverts to shape" );

    textureLabel = new pawgui::widget_label_text ("Direction Light Texture","Enter below:" );
    texturePreviewImgLabel = new pawgui::widget_label_image(  "Direction Light","" );
    textureLocationField = new pawgui::widget_input_text("","Texture Location...");

    lightType = new pawgui::widget_radio_button_controller("Light Type", true );
    lightType->add_menu_option("Ambient Light","ambient_light", gpe::light_type_ambient2d, false );
    lightType->add_menu_option("Direction Light","direction_light", gpe::light_type_direction2d, false );
    lightType->add_menu_option("Point Light","point", gpe::light_type_point2d, true );

    light_colorField = new pawgui::gpe_widget_color_picker("Light Color","Light Color");
    light_colorField->set_color_from_rgb( gpe::c_yellow );

    light_radiusField = new pawgui::widget_input_number("Radius(pixels)", true, 0, 1024 );
    light_radiusField->set_number( 128 );

    lightSecondRadiusField = new pawgui::widget_input_number("Secondary Radius(pixels)", true, 0, 1024 );
    lightSecondRadiusField->set_number( 128 );

    lightThirdRadiusField = new pawgui::widget_input_number("Third Radius(pixels)", true, 0, 1024 );
    lightThirdRadiusField->set_number( 0 );

    light_lengthField = new pawgui::widget_input_number("Length (pixels)", true, 0, 1024 );
    light_lengthField->set_number( 256 );

    light_widthField = new pawgui::widget_input_number("Width (pixels)", true, 0, 1024 );
    light_widthField->set_number( 64 );

    light_intensityField = new pawgui::widget_input_number("Intensity (pixels)", true, 0, 255 );
    light_intensityField->set_number( 228 );

    lightSecondIntensityField = new pawgui::widget_input_number("Secondary Light Intensity (pixels)", true, 0, 255 );
    lightSecondIntensityField->set_number( 192 );

    lightThirdIntensityField = new pawgui::widget_input_number("Secondary Light Intensity (pixels)", true, 0, 255 );
    lightThirdIntensityField->set_number( 160 );

    light_flicker_timeField = new pawgui::widget_input_number("Light Flicker Timer (milliseconds)", true, 0, 9000 );
    light_flicker_timeField->set_number( 255 );

    lightFlickerAmountField = new pawgui::widget_input_number("Flicker Random Amount (pixels)", true, 0, 1024 );
    lightFlickerAmountField->set_number( 2 );

    lightUseFlicker = new pawgui::widget_checkbox("Flicker Light","Check to flicker the light in intervals", false );

    light_directionField = new pawgui::widget_input_number("Direction (degrees)", false, -360, 360 );
    light_directionField->set_number( 270 );

    directionLightTexture = nullptr;
}

lightResource::~lightResource()
{
    if( myAmbientLight!=nullptr )
    {
        delete myAmbientLight;
        myAmbientLight = nullptr;
    }
    if( myPointLight!=nullptr )
    {
        delete myPointLight;
        myPointLight = nullptr;
    }
    if( myDirectionLight!=nullptr )
    {
        delete myDirectionLight;
        myDirectionLight = nullptr;
    }
}


bool lightResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void lightResource::compile_cpp()
{

}

bool lightResource::is_build_ready()
{
    recentErrorMessage ="";
    return true;
}


void lightResource::handle_scrolling()
{

}

bool lightResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}


void lightResource::load_image(std::string new_file_name, bool autoProcess )
{
    if( new_file_name.size() <= 4)
    {
        return;
    }
    textureLocationField->set_string( new_file_name );
    if( directionLightTexture==nullptr )
    {
        directionLightTexture = gpe::rph->get_new_texture();
    }
    directionLightTexture->load_new_texture( new_file_name );
    directionLightTexture->set_blend_mode( gpe::blend_mode_add );
    myDirectionLight->light_texture = directionLightTexture;
    texturePreviewImgLabel->change_texture( directionLightTexture );
    texturePreviewImgLabel->set_width( 64 );
    texturePreviewImgLabel->set_height( 64 );
    if( autoProcess)
    {
        sff_ex::file_copy( new_file_name, stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/lights/"+ stg_ex::get_local_from_global_file(new_file_name) );
    }
}

void lightResource::load_resource(std::string file_path )
{
    if( resourcePostProcessed ==false || sff_ex::file_exists(file_path))
    {
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->update_submessages( "Loading Light", resource_name );
        }

        std::string otherColContainerName = "";

        std::string newFileIn = "";
        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/lights/";
        if( sff_ex::file_exists(file_path) )
        {
            newFileIn = file_path;
            soughtDir = stg_ex::get_path_from_file(newFileIn);
        }
        else
        {
            newFileIn = soughtDir + resource_name+".gpf";
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        //error_log->report("Loading animation - "+newFileIn);
        //If the level file could be loaded
        float foundFileVersion = -1;
        if( !gameResourceFileIn.fail() )
        {
            //error_log->report("Procesing animation file...");
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string key_string="";
                std::string valstring="";
                std::string subValstring="";
                std::string currLine="";
                std::string currLineToBeProcessed;

                int i = 0;
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                    currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

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
                                    key_string = currLineToBeProcessed.substr(0,equalPos);
                                    valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( key_string=="Version")
                                    {
                                        foundFileVersion = stg_ex::string_to_float(valstring);
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
                                key_string = currLineToBeProcessed.substr(0,equalPos);
                                valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                                if( key_string=="ResourceName")
                                {
                                    renameBox->set_string(valstring);
                                }
                                else if( key_string=="DirectionLightTexture")
                                {
                                    load_image( soughtDir+valstring);
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

void lightResource::process_data_fields(float versionToProcess )
{

}

void lightResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find( view_space);
    cam = gpe::camera_find( cam );

    if( panel_main_editor!=nullptr )
    {
        panel_main_editor->clear_panel();

        panel_main_editor->add_gui_element(renameBox,true);

        panel_main_editor->add_gui_element( lightType,true);
        panel_main_editor->add_gui_element(light_colorField,true);
        panel_main_editor->add_gui_element( light_intensityField,true);

        if( lightType->get_selected_value() == gpe::light_type_direction2d )
        {
            panel_main_editor->add_gui_element(light_lengthField,true);
            panel_main_editor->add_gui_element(light_widthField,true);
            panel_main_editor->add_gui_element(light_directionField,true);
            panel_main_editor->add_gui_element(autorotateAngle,true);

            panel_main_editor->add_gui_element(textureLabel,true);
            panel_main_editor->add_gui_element(textureLocationField,true);
            panel_main_editor->add_gui_element(browseTexture_button,true);
            panel_main_editor->add_gui_element(clearTexture_button,true);
            panel_main_editor->add_gui_element(texturePreviewImgLabel,true);
        }
        else if( lightType->get_selected_value() == gpe::light_type_point2d )
        {
            panel_main_editor->add_gui_element(light_radiusField,true);
            panel_main_editor->add_gui_element(lightSecondRadiusField,true);
            panel_main_editor->add_gui_element(lightSecondIntensityField,true);
            panel_main_editor->add_gui_element(lightThirdRadiusField,true);
            panel_main_editor->add_gui_element(lightThirdIntensityField,true);
            panel_main_editor->add_gui_element(lightUseFlicker,true);
            panel_main_editor->add_gui_element(light_flicker_timeField,true);
            panel_main_editor->add_gui_element(lightFlickerAmountField,true);
        }
        else
        {

        }

        panel_main_editor->add_gui_element(confirmResource_button,true);
        panel_main_editor->add_gui_element(cancelResource_button,true);
        panel_main_editor->process_self();

        if( lightType->get_selected_value() == gpe::light_type_direction2d )
        {
            if( browseTexture_button!=nullptr && browseTexture_button->is_clicked() )
            {
                std::string newTextureLocation = pawgui::get_filename_open_from_popup("Choose Light Texture","Images",pawgui::main_settings->fileOpenTextureDir);
                if( sff_ex::file_exists( newTextureLocation) && stg_ex::file_is_image(newTextureLocation) )
                {
                    load_image( newTextureLocation, true);
                }
            }
            else if( clearTexture_button!=nullptr && clearTexture_button->is_clicked() )
            {
                if( directionLightTexture!=nullptr )
                {
                    delete directionLightTexture;
                    directionLightTexture = nullptr;
                }
                textureLocationField->set_string( "" );
                myDirectionLight->light_texture = nullptr;
                texturePreviewImgLabel->change_texture( nullptr );
                texturePreviewImgLabel->set_width( 16 );
                texturePreviewImgLabel->set_height( 16 );
            }
        }

        if( confirmResource_button->is_clicked() )
        {
            save_resource();
        }
        else if( cancelResource_button->is_clicked() )
        {
            if( pawgui::display_prompt_message("Are you sure you will like to reverse light changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
            {
                resourcePostProcessed = false;
                load_resource();
            }
        }
    }

    tempAngleExtra+= gpe::time_keeper->get_delta_ticks() / 25.f;

    while( tempAngleExtra < -360.f  )
    {
        tempAngleExtra += 360.f ;
    }
    while( tempAngleExtra >= 360.f )
    {
        tempAngleExtra -= 360.f;
    }

    if( myAmbientLight!=nullptr )
    {
        myAmbientLight->light_color->change_and_verify_rgba( light_colorField->get_r(), light_colorField->get_g(), light_colorField->get_b(), 255 );
        myAmbientLight->light_intensity = light_intensityField->get_held_int();
    }

    if( myDirectionLight!=nullptr )
    {
        myDirectionLight->light_color->change_and_verify_rgba( light_colorField->get_r(), light_colorField->get_g(), light_colorField->get_b(), 255 );
        myDirectionLight->light_intensity = light_intensityField->get_held_int();
        if( autorotateAngle->is_clicked() )
        {
            myDirectionLight->set_direction( tempAngleExtra );
        }
        else
        {
            myDirectionLight->set_direction( light_directionField->get_held_int() );
        }
        myDirectionLight->light_width = light_widthField->get_held_int();
        myDirectionLight->light_length = light_lengthField->get_held_int();
    }

    if( myPointLight!=nullptr )
    {
        myPointLight->light_color->change_and_verify_rgba( light_colorField->get_r(), light_colorField->get_g(), light_colorField->get_b(), 255 );
        myPointLight->light_intensity = light_intensityField->get_held_int();
        myPointLight->light_radius = light_radiusField->get_held_int();
        myPointLight->light_radius_second = lightSecondRadiusField->get_held_int();
        myPointLight->light_radius_second_intensity = lightSecondIntensityField->get_held_int();

        if( lightUseFlicker!=nullptr && lightUseFlicker->is_clicked() )
        {
            myPointLight->setup_flicker( light_flicker_timeField->get_held_int(), lightFlickerAmountField->get_held_int() );
            myPointLight->update_light( gpe::time_keeper->get_delta_ticks() );

        }
        else
        {
            myPointLight->disable_flicker();
        }
    }
}

void lightResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find( view_space);
    cam = gpe::camera_find( cam );
    gpe::gcanvas->render_rectangle( 0,0,view_space->w, view_space->h, pawgui::theme_main->program_color, false, 255 );

    if( gpe::gcanvas->is_render_mode_supported( gpe::gcanvas->get_render_mode() ) !=1 )
    {
        //We draw the message that our current render can not draw the preview in this editor and then exit the render function :-(
        gpe::gfs->render_text( view_space->w/2,view_space->h/2,
                          "Render Mode ["+gpe::gcanvas->get_render_mode_name( gpe::gcanvas->get_render_mode())+"] is not supported by "+gpe::gcanvas->get_artist_name()+ " artist backend",
                              pawgui::theme_main->program_color_header, gpe::font_default, gpe::fa_center, gpe::fa_middle );
        return;
    }

    texture_gpe_logo->render_align( view_space->w/2, view_space->h/2,gpe::fa_center, gpe::fa_middle, nullptr, nullptr, 255 );
    gpe::gcanvas->resize_ligting_overlay( view_space->w, view_space->h );
    gpe::gcanvas->switch_ligting_overlay( true );
    gpe::renderer_main->reset_viewpoint( );
    gpe::gcanvas->render_rectangle(0,0,view_space->w, view_space->h, gpe::c_black, false, 255 );
    //gpe::gcanvas->set_artist_blend_mode( blend_mode_add );
    bool mouseInView = get_mouse_coords( view_space, cam );
    if( lightType->get_selected_value() == gpe::light_type_direction2d )
    {
        if(  mouseInView && !tempAngleExtra )
        {
            myDirectionLight->render_light_at( local_mouse_x, local_mouse_y, 1 );
        }
        else
        {
            myDirectionLight->render_light_at( view_space->w/2, view_space->h/2, 1 );
        }
    }
    else if( lightType->get_selected_value() == gpe::light_type_point2d )
    {
        if(  mouseInView )
        {
            myPointLight->render_light_at( local_mouse_x, local_mouse_y, 1 );

        }
        else
        {
            myPointLight->render_light_at( view_space->w/2, view_space->h/2, 1 );
        }
    }
    else if( myAmbientLight!=nullptr )
    {
        myAmbientLight->render_light( 1, view_space );
    }

    //gpe::gcanvas->render_circle_filled_color( editorView.w/2, editorView.h/2, c_white, 255 );
    gpe::gcanvas->switch_ligting_overlay( false );
    gpe::renderer_main->reset_viewpoint( );
    gpe::renderer_main->set_viewpoint( view_space );
    gpe::gcanvas->render_ligting_overlay( 0,0 );
}

void lightResource::revert_data_fields()
{

}

void lightResource::save_resource(std::string file_path, int backupId )
{
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Saving Light", resource_name );
    }

    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = stg_ex::get_path_from_file(file_path);
    if( sff_ex::path_exists(soughtDir) )
    {
        newFileOut = file_path;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/lights/";
        newFileOut = soughtDir + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the particle file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);

            if( lightType!=nullptr )
            {
                newSaveDataFile << "LightType=" + stg_ex::int_to_string(lightType->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "LightType="+ stg_ex::int_to_string( gpe::light_type_point2d )+"\n";
            }
            if( light_colorField!=nullptr )
            {
                newSaveDataFile << "LightColor=" + light_colorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "LightColor=#FFF\n";
            }
            if( light_intensityField!=nullptr )
            {
                newSaveDataFile << "LightIntensity[0]=" + stg_ex::int_to_string( light_intensityField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "LightIntensity[0]=255\n";
            }
            if( lightSecondIntensityField!=nullptr )
            {
                newSaveDataFile << "LightIntensity[1]=" + stg_ex::int_to_string( lightSecondIntensityField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "LightIntensity[1]=192\n";
            }
            if( lightThirdIntensityField!=nullptr )
            {
                newSaveDataFile << "LightIntensity[2]=" + stg_ex::int_to_string( lightThirdIntensityField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "LightIntensity[2]=128\n";
            }
            if( light_radiusField!=nullptr )
            {
                newSaveDataFile << "PointLightRadius[0]=" + stg_ex::int_to_string( light_radiusField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "PointLightRadius[0]=128\n";
            }
            if( lightSecondRadiusField!=nullptr )
            {
                newSaveDataFile << "PointLightRadius[1]=" + stg_ex::int_to_string( lightSecondRadiusField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "PointLightRadius[1]=128\n";
            }

            if( lightThirdRadiusField!=nullptr )
            {
                newSaveDataFile << "PointLightRadius[2]=" + stg_ex::int_to_string( lightThirdRadiusField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "PointLightRadius[2]=128\n";
            }

            if( light_lengthField!=nullptr )
            {
                newSaveDataFile << "DirectionLightLength=" + stg_ex::int_to_string( light_lengthField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "DirectionLightLength=128\n";
            }
            if( light_widthField!=nullptr )
            {
                newSaveDataFile << "DirectionLightWidth=" + stg_ex::int_to_string( light_widthField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "DirectionLightWidth=64\n";
            }
            if( light_directionField!=nullptr )
            {
                newSaveDataFile << "DirectionLightAngle=" + stg_ex::int_to_string( light_directionField->get_held_int() ) +"\n";
            }
            else
            {
                newSaveDataFile << "DirectionLightAngle=270\n";
            }
            if( textureLocationField!=nullptr )
            {
                newSaveDataFile << "DirectionLightTexture=" + stg_ex::get_local_from_global_file( textureLocationField->get_string() )+"\n";
            }
            if( lightUseFlicker!=nullptr )
            {
                newSaveDataFile << "LightFlicker=" + stg_ex::int_to_string( lightUseFlicker->is_clicked() )+"\n";
            }
            if( light_flicker_timeField!=nullptr )
            {
                newSaveDataFile << "LightFlickerTime=" + stg_ex::int_to_string( light_flicker_timeField->get_held_int() )+"\n";
            }
            if( lightFlickerAmountField!=nullptr )
            {
                newSaveDataFile << "LightFlickerAmount=" + stg_ex::int_to_string( lightFlickerAmountField->get_held_int() )+"\n";
            }
            if( autorotateAngle!=nullptr )
            {
                newSaveDataFile << "RotateInEditor=" + stg_ex::int_to_string( autorotateAngle->is_clicked() )+"\n";
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
            main_editor_log->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        main_editor_log->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void lightResource::update_box(int x_new, int y_new, int newW, int newH )
{

}

bool lightResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn  )
{
    //resource_nameLabel->set_name(renameBox->get_string() );
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            load_resource();
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Light2d=" << resource_name << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}


