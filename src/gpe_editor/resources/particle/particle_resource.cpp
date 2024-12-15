/*
particle_resource.cpp
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

#include "particle_resource.h"
#include "../../editor/gpe_editor_settings.h"


particleResource::particleResource(pawgui::widget_resource_container * pFolder)
{
    //bottomPaneList = new pawgui::widget_panel_list();
    projectParentFolder = pFolder;
    myEmitter = new gpe::particle_emitter(0, 0,0,0 );
    myEmitter->showBox = true;
    myEmitter->set_emission_rate( 60 );
    emitterTexture = nullptr;
    //emitterTexture->load_new_texture(  gpe::app_directory_name+"resources/gfx/animations/c_snow.png",-1,true );
    myEmitter->change_texture( emitterTexture, gpe::blend_mode_blend );
    openExternalEditor_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Texture Image In External Editor");
    refreshResourceData_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded texture image");
    clearTexture_button = new pawgui::widget_button_label("Clear Image...","Clears texture and reverts to shape" );
    textureIsRotated = new pawgui::widget_checkbox("Rotate Texture","The Particle texture rotates with it's direction", true );


    textureLabel = new pawgui::widget_label_title("Particle Texture","Enter below:" );
    texturePreviewImgLabel = new pawgui::widget_label_image( "","" );
    textureLocationField = new pawgui::widget_input_text("","Texture Location...");

    blendTypeMenu = new pawgui::widget_dropdown_menu("Blend Mode", false );
    blendTypeMenu->add_menu_option("Add","add", gpe::blend_mode_add, false );
    blendTypeMenu->add_menu_option("Blend","blend", gpe::blend_mode_blend, true  );
    blendTypeMenu->add_menu_option("Mod","mod", gpe::blend_mode_mod, false  );
    blendTypeMenu->add_menu_option("Mul","mul", gpe::blend_mode_mul, false  );
    blendTypeMenu->add_menu_option("None","none",gpe::blend_mode_none, false  );

    shapeMenu = new pawgui::widget_dropdown_menu("Emitter Shape", false );
    shapeMenu->add_menu_option("Point", "point", gpe::part_shape_point, false );
    shapeMenu->add_menu_option("Square", "square",gpe::part_shape_square, true  );
    shapeMenu->add_menu_option("Rectangle", "rectangle",gpe::part_shape_rectangle, false  );
    shapeMenu->add_menu_option("Circle", "circle", gpe::part_shape_circle, false  );

    emitterBackupTextureMenu = new pawgui::widget_dropdown_menu("Fallback Shape", false );
    emitterBackupTextureMenu->add_menu_option("Square", "square",gpe::part_shape_square, false  );
    emitterBackupTextureMenu->add_menu_option("Square Outline", "square_outline",gpe::part_shape_square_outline, false  );
    emitterBackupTextureMenu->add_menu_option("Rectangle", "rectangle",gpe::part_shape_rectangle, false  );
    emitterBackupTextureMenu->add_menu_option("Rectangle Outline", "rectangle_outline", gpe::part_shape_rectangle_outline, false  );
    emitterBackupTextureMenu->add_menu_option("Circle", "circle",gpe::part_shape_circle, true  );
    emitterBackupTextureMenu->add_menu_option("Circle Outline", "circle_outline", gpe::part_shape_circle_outline, false  );
    emitterBackupTextureMenu->add_menu_option("Line", "line",gpe::part_shape_line, false  );
    emitterBackupTextureMenu->add_menu_option("Line Capped", "line_capped",gpe::part_shape_linecapped, false  );


    showDebugInfo = new pawgui::widget_checkbox("Show Debugger","Preview Particle Stats in Editor", true );
    emissionRateField = new pawgui::widget_input_number("particles per second", true, 0, gpe::partical_max_per_emitter );
    emissionRateField->set_number( 60 );
    emissionRateField->set_label( "Emission Rate Per Second" );

    emissionDisabled = new pawgui::widget_checkbox("Emission Disabled","Emission is disabled", false );
    emissionPaused = new pawgui::widget_checkbox("Emission Paused","Emission is paused", false );
    emissionDrawReversed = new pawgui::widget_checkbox("Reverse render order","Reverse render order", false );

    particleCountField = new pawgui::widget_input_number("Max Per Emitter", true, 0, gpe::partical_max_per_emitter );
    particleCountField->set_number( 150 );

    lifeMinField = new pawgui::widget_input_number("Min Life (seconds)", true, 0, 500 );
    lifeMinField->set_number( 5 );
    lifeMaxField = new pawgui::widget_input_number("Max Life(seconds)", true, 0, 500 );
    lifeMaxField->set_number( 10 );

    colorsLabel = new pawgui::widget_label_title("Particle Colors","Particle Colors:" );
    sColorField = new pawgui::gpe_widget_color_picker("StartColor","DefaultColorValues");
    sColorField->set_color_from_rgb( gpe::c_purple );

    sVColorField = new pawgui::gpe_widget_color_picker("StartVarColor","DefaultVarColorValues");
    sVColorField->set_color_from_rgb( gpe::c_blgray );

    eColorField = new pawgui::gpe_widget_color_picker("EndColor","EndVarColorValues");
    eColorField->set_color_from_rgb( gpe::c_yellow );

    eVColorField = new pawgui::gpe_widget_color_picker("EndVarColor","EndVarColorValues");
    eVColorField->set_color_from_rgb( gpe::c_blgray );

    xStartPosField = new pawgui::widget_input_number("X-Start(pixels)" );
    yStartPosField = new pawgui::widget_input_number("Y-Start(pixels)" );

    xRandomField = new pawgui::widget_input_number("X-Random(pixels)" );
    xRandomField->set_number( 8 );

    yRandomField = new pawgui::widget_input_number("Y-Random(pixels)" );
    yRandomField->set_number( 8 );

    physicsLabel = new pawgui::widget_label_title("Particle Physics","Particle Physics:" );

    directionMin = new pawgui::widget_input_number("Min Direction(degrees)", true, -720, 720 );
    directionMin->set_number( -360 );

    directionMax = new pawgui::widget_input_number("Max Direction(degrees)", true, -720, 720 );
    directionMax->set_number( 360 );

    speedMin = new pawgui::widget_input_number("Min Speed(pixels per second)", true, -720, 720 );
    speedMin->set_number( 3 );

    speedMax = new pawgui::widget_input_number("Max Speed(pixels per second)", true, -720, 720 );
    speedMax->set_number( 7 );

    gravityXValue = new pawgui::widget_input_number("Gravity-X (pixels)", true, -720, 720 );

    gravityYValue = new pawgui::widget_input_number("Gravity-Y (pixels)", true, -720, 720 );

    shapeWField = new pawgui::widget_input_number("Shape W(R) (pixels)", true, 0, 128 );
    shapeWField->set_number( 16 );

    shapeHField = new pawgui::widget_input_number("Shape H (pixels)", true, 0, 128 );
    shapeHField->set_number( 16 );
}

particleResource::~particleResource()
{
    if( myEmitter!=nullptr )
    {
        delete myEmitter;
        myEmitter = nullptr;
    }
}



bool particleResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void particleResource::compile_cpp()
{

}

void particleResource::handle_scrolling()
{

}

bool particleResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

bool particleResource::is_build_ready()
{
    recentErrorMessage = "";
    if( particleCountField->get_held_int() <= 0)
    {
        recentErrorMessage = "ParticleCount <= 0;";
        return false;
    }
    if( emissionRateField->get_held_int() <= 0)
    {
        recentErrorMessage = "EmissionRate <= 0;";
        return false;
    }
    return true;
}

void particleResource::load_image(std::string new_file_name, bool autoProcess )
{
    if( new_file_name.size() > 4 && sff_ex::file_exists( new_file_name ) )
    {
        textureLocationField->set_string( new_file_name );
        emitterTexture = gpe::rsm->texture_add_filename( new_file_name );
        if( emitterTexture!=nullptr && ( emitterTexture->get_width() > 512 || emitterTexture->get_height() > 512 ) )
        {
            if( main_editor_log!=nullptr )
            {
                main_editor_log->log_general_error("Particle Emitter is too large. Texture must be 512x512px or smaller." );
            }
            else
            {
                gpe::error_log->report("Particle Emitter is too large. Texture must be 512x512px or smaller." );
            }
        }

        emitterTexture->set_blend_mode( blendTypeMenu->get_selected_value() );
        myEmitter->change_texture( emitterTexture, blendTypeMenu->get_selected_value() );
        texturePreviewImgLabel->load_label_image( new_file_name );
        texturePreviewImgLabel->set_width( 64 );
        texturePreviewImgLabel->set_height( 64 );
        if( autoProcess )
        {
            sff_ex::file_copy( new_file_name, stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/particles/"+ stg_ex::get_local_from_global_file(new_file_name) );
        }
    }
    else
    {
        textureLocationField->set_string( "" );
    }
}

void particleResource::load_resource(std::string file_path )
{
    if( resourcePostProcessed ==false || sff_ex::file_exists(file_path))
    {
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->update_submessages( "Loading Particle Emitter", resource_name );
        }

        std::string otherColContainerName = "";

        std::string newFileIn = "";
        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/particles/";
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

        //gpe::error_log->report("Loading animation - "+newFileIn);
        //If the level file could be loaded
        float foundFileVersion = -1;
        if( !gameResourceFileIn.fail() )
        {
            //gpe::error_log->report("Procesing animation file...");
            //makes sure the file is open
            if (gameResourceFileIn.is_open() )
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
                                else  if( key_string=="MaxParticles")
                                {
                                    particleCountField->set_string(valstring);
                                }
                                else  if( key_string=="EmissionRate")
                                {
                                    emissionRateField->set_string(valstring);
                                }
                                else  if( key_string=="LifeMin")
                                {
                                    lifeMinField->set_string(valstring);
                                }
                                else  if( key_string=="LifeMax")
                                {
                                    lifeMaxField->set_string(valstring);
                                }
                                else  if( key_string=="StartColor")
                                {
                                    sColorField->set_color_from_hex(valstring);
                                }
                                else  if( key_string=="StartColorVar")
                                {
                                    sVColorField->set_color_from_hex(valstring);
                                }
                                else  if( key_string=="EndColor")
                                {
                                    eColorField->set_color_from_hex(valstring);
                                }
                                else  if( key_string=="EndColorVar")
                                {
                                    eVColorField->set_color_from_hex(valstring);
                                }
                                else  if( key_string=="XStart")
                                {
                                    xStartPosField->set_string(valstring);
                                }
                                else  if( key_string=="YStart")
                                {
                                    yStartPosField->set_string(valstring);
                                }
                                else  if( key_string=="XRandom")
                                {
                                    xRandomField->set_string(valstring);
                                }
                                else  if( key_string=="YRandom")
                                {
                                    yRandomField->set_string(valstring);
                                }
                                else  if( key_string=="DirectionMin")
                                {
                                    directionMin->set_string(valstring);
                                }
                                else  if( key_string=="DirectionMax")
                                {
                                    directionMax->set_string(valstring);
                                }
                                else  if( key_string=="SpeedMin")
                                {
                                    speedMin->set_string(valstring);
                                }
                                else  if( key_string=="SpeedMax")
                                {
                                    speedMax->set_string(valstring);
                                }
                                else  if( key_string=="GravityXValue")
                                {
                                     gravityXValue->set_string(valstring);
                                }
                                else  if( key_string=="GravityYValue")
                                {
                                    gravityYValue->set_string(valstring);
                                }
                                else  if( key_string=="BlendType")
                                {
                                    blendTypeMenu->set_option_value( stg_ex::string_to_int(valstring) );
                                }
                                else  if( key_string=="EmitterShape")
                                {
                                    shapeMenu->set_option_value( stg_ex::string_to_int(valstring) );
                                }
                                else  if( key_string=="ShowDebugInfo")
                                {
                                    showDebugInfo->set_clicked( stg_ex::string_to_bool(valstring) );
                                }
                                else  if( key_string=="Texture")
                                {
                                    load_image( soughtDir+valstring);
                                }
                                else  if( key_string=="FallbackShape")
                                {
                                    emitterBackupTextureMenu->set_option_value( stg_ex::string_to_bool(valstring) );
                                }
                                else  if( key_string=="ShapeW" || key_string=="ShapeR" )
                                {
                                     shapeWField->set_string(valstring);
                                }
                                else  if( key_string=="ShapeH")
                                {
                                    shapeHField->set_string(valstring);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void particleResource::prerender_self( )
{

}

void particleResource::process_data_fields(float versionToProcess )
{

}

void particleResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find( view_space);
    cam = gpe::camera_find( cam );

    if( panel_main_editor!=nullptr )
    {
        int previousBlendMode = blendTypeMenu->get_selected_value();
        myEmitter->set_fallback_shape( emitterBackupTextureMenu->get_selected_value() );
        panel_main_editor->clear_panel();

        panel_main_editor->add_gui_element(renameBox,true);
        panel_main_editor->add_gui_element(refreshResourceData_button,false);
        panel_main_editor->add_gui_element(loadResource_button,false);
        //panel_main_editor->add_gui_element(saveResource_button,false);
        panel_main_editor->add_gui_element( openExternalEditor_button,true);

        panel_main_editor->add_gui_element(textureLabel,true);
        panel_main_editor->add_gui_element(textureLocationField,true);

        panel_main_editor->add_gui_element(textureIsRotated,true);
        panel_main_editor->add_gui_element(clearTexture_button,true);
        panel_main_editor->add_gui_element(texturePreviewImgLabel,true);

        panel_main_editor->add_gui_element(emitterBackupTextureMenu,true);
        panel_main_editor->add_gui_element(shapeWField,true);
        panel_main_editor->add_gui_element(shapeHField,true);
        panel_main_editor->add_gui_element( shapeMenu,true);
        panel_main_editor->add_gui_element(particleCountField,true);
        panel_main_editor->add_gui_element(emissionRateField,true);
        panel_main_editor->add_gui_element(lifeMinField,true);
        panel_main_editor->add_gui_element(lifeMaxField,true);

        panel_main_editor->add_gui_element(xStartPosField,true);
        panel_main_editor->add_gui_element(yStartPosField,true);
        panel_main_editor->add_gui_element(xRandomField,true);
        panel_main_editor->add_gui_element(yRandomField,true);


        panel_main_editor->add_gui_element( emissionDisabled,true);
        panel_main_editor->add_gui_element( emissionPaused,true);
        panel_main_editor->add_gui_element( emissionDrawReversed,true);

        panel_main_editor->add_gui_element( blendTypeMenu,true);
        panel_main_editor->add_gui_element(colorsLabel,true);
        panel_main_editor->add_gui_element(sColorField,true);
        panel_main_editor->add_gui_element(sVColorField,true);
        panel_main_editor->add_gui_element(eColorField,true);
        panel_main_editor->add_gui_element(eVColorField,true);


        panel_main_editor->add_gui_element(physicsLabel,true);
        panel_main_editor->add_gui_element(directionMin,true);
        panel_main_editor->add_gui_element(directionMax,true);

        panel_main_editor->add_gui_element(speedMin,true);
        panel_main_editor->add_gui_element(speedMax,true);

        panel_main_editor->add_gui_element(gravityXValue,true);
        panel_main_editor->add_gui_element(gravityYValue,true);

        panel_main_editor->add_gui_element(showDebugInfo,true);
        panel_main_editor->add_gui_element(confirmResource_button,true);
        panel_main_editor->add_gui_element(cancelResource_button,true);
        panel_main_editor->process_self();

        if( loadResource_button!=nullptr && loadResource_button->is_clicked() )
        {
            std::string newTextureLocation = pawgui::get_filename_open_from_popup("Choose Particle Texture","Images",pawgui::main_settings->fileOpenTextureDir);
            if( sff_ex::file_exists( newTextureLocation) && stg_ex::file_is_image(newTextureLocation) )
            {
                load_image( newTextureLocation);
            }
        }
        else if( refreshResourceData_button!=nullptr && refreshResourceData_button->is_clicked() )
        {
            if(  emitterTexture!=nullptr )
            {
                std::string currentFileToRefresh = stg_ex::get_short_filename (emitterTexture->get_filename(),true );
                currentFileToRefresh = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/textures/"+currentFileToRefresh;
                load_image(currentFileToRefresh);
            }
        }
        else if( clearTexture_button!=nullptr && clearTexture_button->is_clicked() )
        {
            emitterTexture = nullptr;
            textureLocationField->set_string( "" );
            myEmitter->change_texture( nullptr, blendTypeMenu->get_selected_value() );
            myEmitter->set_fallback_shape( emitterBackupTextureMenu->get_selected_value() );
            texturePreviewImgLabel->change_texture( nullptr );
            texturePreviewImgLabel->set_width( 16 );
            texturePreviewImgLabel->set_height( 16 );
        }
        else if( openExternalEditor_button!=nullptr && openExternalEditor_button->is_clicked() )
        {
            if(  emitterTexture!=nullptr )
            {
                if( sff_ex::file_exists(emitterTexture->get_filename() ) )
                {
                    if( main_editor_settings!=nullptr && main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=nullptr)
                    {
                        gpe::external_open_program(main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),emitterTexture->get_filename(), true );
                    }
                    else
                    {
                        gpe::external_open_url(emitterTexture->get_filename());
                    }
                    sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit texture ["+emitterTexture->get_filename()+"]...");
                }
            }
        }
        else if( confirmResource_button->is_clicked() )
        {
            save_resource();
        }
        else if( cancelResource_button->is_clicked() )
        {
            if( pawgui::display_prompt_message("Are you sure you will like to reverse particle changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
            {
                resourcePostProcessed = false;
                load_resource();
            }
        }

        if( previousBlendMode != blendTypeMenu->get_selected_value() )
        {
            if( emitterTexture!=nullptr )
            {
                emitterTexture->set_blend_mode( blendTypeMenu->get_selected_value() );
            }
            myEmitter->set_blend_mode(  blendTypeMenu->get_selected_value() );
        }
    }

    if( myEmitter!=nullptr )
    {
        if( get_mouse_coords( view_space, cam ) )
        {
            if( gpe::input->check_mouse_pressed( mb_left ) )
            {
                myEmitter->emitterXPos = local_mouse_x;
                myEmitter->emitterYPos = local_mouse_y;
            }
        }
        if( myEmitter->emitterXPos > view_space->w || myEmitter->emitterXPos < 0 )
        {
            myEmitter->emitterXPos = view_space->w/2;
        }
        if( myEmitter->emitterYPos > view_space->h || myEmitter->emitterYPos < 0 )
        {
            myEmitter->emitterYPos = view_space->h/2;
        }

        myEmitter->lifeMin = lifeMinField->get_held_number();
        myEmitter->lifeMax = lifeMaxField->get_held_number();
        myEmitter->showBox = showDebugInfo->is_clicked();

        myEmitter->startColor->change_and_verify_rgba( sColorField->get_r(), sColorField->get_g(), sColorField->get_b(), 255 );
        myEmitter->startColorVar->change_and_verify_rgba( sVColorField->get_r(), sVColorField->get_g(), sVColorField->get_b(), 255 );

        myEmitter->endColor->change_and_verify_rgba( eColorField->get_r(), eColorField->get_g(), eColorField->get_b(), 0 );
        myEmitter->endColorVar->change_and_verify_rgba( eVColorField->get_r(), eVColorField->get_g(), eVColorField->get_b(), 0 );

        myEmitter->angleMin = directionMin->get_held_number();
        myEmitter->angleMax = directionMax->get_held_number();

        myEmitter->speedMin = speedMin->get_held_number();
        myEmitter->speedMax = speedMax->get_held_number();

        myEmitter->shapeW = shapeWField->get_held_number();
        myEmitter->shapeH = shapeHField->get_held_number();

        myEmitter->xRandom = xRandomField->get_held_number();
        myEmitter->yRandom = yRandomField->get_held_number();

        myEmitter->emission_disabled = emissionDisabled->is_clicked();
        myEmitter->emission_paused = emissionPaused->is_clicked();
        myEmitter->reverse_draw = emissionPaused->is_clicked();

        if(emitterBackupTextureMenu->just_activated() )
        {
            myEmitter->set_fallback_shape( emitterBackupTextureMenu->get_selected_value() );
        }
        if( emissionRateField->is_inuse() == false )
        {
            myEmitter->set_emission_rate( emissionRateField->get_held_number() );
        }
        if( particleCountField->is_inuse() == false )
        {
            myEmitter->set_max_particles( particleCountField->get_held_number(), true );
        }

        if( gpe::input->check_kb_down( kb_space ) )
        {
            myEmitter->burst_emit();
        }
        if( textureIsRotated!=nullptr )
        {
            myEmitter->rotateTexture = textureIsRotated->is_clicked();
        }
        myEmitter->update( gpe::time_keeper->get_delta_ticks() );
    }
}

void particleResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
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

    //if( myEmitter!=nullptr )
    {
        myEmitter->render( );
    }
    //gpe::gfs->render_text( view_space->w/2, view_space->h-32, "C++ Feature. Currently not supported in HTML5 runtime",pawgui::theme_main->main_box_font_color,gpe::font_default, gpe::fa_center, gpe::fa_bottom, 255);
}

void particleResource::revert_data_fields()
{

}

void particleResource::save_resource(std::string file_path, int backupId )
{
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Saving Particle", resource_name );
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
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/particles/";
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
            if( particleCountField!=nullptr )
            {
                newSaveDataFile << "MaxParticles=" + stg_ex::int_to_string(particleCountField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "MaxParticles=100\n";
            }
            if( emissionRateField!=nullptr )
            {
                newSaveDataFile << "EmissionRate=" + stg_ex::int_to_string(emissionRateField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "EmissionRate=100\n";
            }

            if( lifeMinField!=nullptr )
            {
                newSaveDataFile << "LifeMin=" + stg_ex::float_to_string( lifeMinField->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "LifeMin=100\n";
            }

            if( lifeMaxField!=nullptr )
            {
                newSaveDataFile << "LifeMax=" + stg_ex::float_to_string(lifeMaxField->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "LifeMax=100\n";
            }

            if( sColorField!=nullptr )
            {
                newSaveDataFile << "StartColor=" + sColorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "StartColor=#FFF\n";
            }

            if( sVColorField!=nullptr )
            {
                newSaveDataFile << "StartColorVar=" + sVColorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "StartColorVar=#000\n";
            }

            if( eColorField!=nullptr )
            {
                newSaveDataFile << "EndColor=" + eColorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "EndColor=#333\n";
            }

            if( eVColorField!=nullptr )
            {
                newSaveDataFile << "EndColorVar=" + eVColorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "EndColorVar=#000\n";
            }

            if( xStartPosField!=nullptr )
            {
                newSaveDataFile << "XStart=" + stg_ex::int_to_string(xStartPosField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "XStart=0\n";
            }
            if( yStartPosField!=nullptr )
            {
                newSaveDataFile << "YStart=" + stg_ex::int_to_string(yStartPosField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "YStart=0\n";
            }

            if( xRandomField!=nullptr )
            {
                newSaveDataFile << "XRandom=" + stg_ex::int_to_string(xRandomField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "XRandom=32\n";
            }

            if( yRandomField!=nullptr )
            {
                newSaveDataFile << "YRandom=" + stg_ex::int_to_string(yRandomField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "YRandom=32\n";
            }

            if( directionMin!=nullptr )
            {
                newSaveDataFile << "DirectionMin=" + stg_ex::float_to_string(directionMin->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "DirectionMin=0\n";
            }

            if( directionMax!=nullptr )
            {
                newSaveDataFile << "DirectionMax=" + stg_ex::float_to_string(directionMax->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "DirectionMax=360\n";
            }

            if( speedMin!=nullptr )
            {
                newSaveDataFile << "SpeedMin=" + stg_ex::int_to_string(speedMin->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "SpeedMin=3\n";
            }

            if( speedMax!=nullptr )
            {
                newSaveDataFile << "SpeedMax=" + stg_ex::int_to_string(speedMax->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "SpeedMax=5\n";
            }

            if( gravityXValue!=nullptr )
            {
                newSaveDataFile << "GravityXValue=" + stg_ex::float_to_string(gravityXValue->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "GravityXValue=0\n";
            }


            if( gravityYValue!=nullptr )
            {
                newSaveDataFile << "GravityYValue=" + stg_ex::float_to_string(gravityYValue->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "GravityYValue=0\n";
            }

            if( blendTypeMenu!=nullptr )
            {
                newSaveDataFile << "BlendType=" + stg_ex::int_to_string(blendTypeMenu->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "BlendType=0\n";
            }
            if( shapeMenu!=nullptr )
            {
                newSaveDataFile << "EmitterShape=" + stg_ex::int_to_string(shapeMenu->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "EmitterShape=0\n";
            }
            if( showDebugInfo!=nullptr )
            {
                newSaveDataFile << "ShowDebugInfo=" + stg_ex::int_to_string(showDebugInfo->is_clicked() )+"\n";
            }
            else
            {
                newSaveDataFile << "ShowDebugInfo=0\n";
            }

            if( textureLocationField!=nullptr )
            {
                newSaveDataFile << "Texture=" + stg_ex::get_local_from_global_file( textureLocationField->get_string() )+"\n";
            }

            if( emitterBackupTextureMenu!=nullptr )
            {
                newSaveDataFile << "FallbackShape=" + stg_ex::int_to_string( emitterBackupTextureMenu->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "FallbackShape=0\n";
            }
            if( shapeWField!=nullptr )
            {
                newSaveDataFile << "ShapeW=" + stg_ex::int_to_string( shapeWField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "ShapeW=16\n";
            }
            if( shapeHField!=nullptr )
            {
                newSaveDataFile << "ShapeH=" + stg_ex::int_to_string( shapeHField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "ShapeH=16\n";
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

void particleResource::update_box(int x_new, int y_new, int newW, int newH )
{

}

bool particleResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn  )
{
    //resource_nameLabel->set_name(renameBox->get_string() );
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            load_resource();
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "ParticleEmitter=" << resource_name << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}
