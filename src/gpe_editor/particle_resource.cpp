/*
particle_resource.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "particle_resource.h"
#include "gpe_editor_settings.h"


particleResource::particleResource(GPE_GeneralResourceContainer * pFolder)
{
    //bottomPaneList = new GPE_GuiElementList();
    projectParentFolder = pFolder;
    myEmitter = new gpe::particle_emitter(0, 0,true, 500 );
    myEmitter->showBox = true;
    myEmitter->set_emission_rate( 60 );
    emitterTexture = NULL;
    //emitterTexture->load_new_texture( renderer_main, gpe::app_directory_name+"resources/gfx/animations/c_snow.png",-1,true );
    myEmitter->change_texture( emitterTexture, gpe::blend_mode_blend );
    transformResourceButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magic.png","Transform the Image",-1);
    openExternalEditorButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Texture Image In External Editor");
    refreshResourceDataButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded texture image");
    clearTextureButton = new GPE_ToolLabelButton("Clear Image...","Clears texture and reverts to shape" );
    textureIsRotated = new GPE_CheckBoxBasic("Rotate Texture","The Particle texture rotates with it's direction", true );


    textureLabel = new GPE_Label_Title("Particle Texture","Enter below:" );
    texturePreviewImgLabel = new GPE_Label_Image( "","" );
    textureLocationField = new gpe_text_widget_string("","Texture Location...");

    blendTypeMenu = new GPE_DropDown_Menu("Blend Mode", false );
    blendTypeMenu->add_menu_option("Add","add", gpe::blend_mode_add, false );
    blendTypeMenu->add_menu_option("Blend","blend", gpe::blend_mode_blend, true  );
    blendTypeMenu->add_menu_option("Mod","mod", gpe::blend_mode_mod, false  );
    blendTypeMenu->add_menu_option("None","none",gpe::blend_mode_none, false  );

    shapeMenu = new GPE_DropDown_Menu("Emitter Shape", false );
    shapeMenu->add_menu_option("Point", "point", gpe::part_shape_point, false );
    shapeMenu->add_menu_option("Square", "square",gpe::part_shape_square, true  );
    shapeMenu->add_menu_option("Rectangle", "rectangle",gpe::part_shape_rectangle, false  );
    shapeMenu->add_menu_option("Circle", "circle", gpe::part_shape_circle, false  );

    emitterBackupTextureMenu = new GPE_DropDown_Menu("Fallback Shape", false );
    emitterBackupTextureMenu->add_menu_option("Square", "square",gpe::part_shape_square, false  );
    emitterBackupTextureMenu->add_menu_option("Square Outline", "square_outline",gpe::part_shape_square_outline, false  );
    emitterBackupTextureMenu->add_menu_option("Rectangle", "rectangle",gpe::part_shape_rectangle, false  );
    emitterBackupTextureMenu->add_menu_option("Rectangle Outline", "rectangle_outline", gpe::part_shape_rectangle_outline, false  );
    emitterBackupTextureMenu->add_menu_option("Circle", "circle",gpe::part_shape_circle, true  );
    emitterBackupTextureMenu->add_menu_option("Circle Outline", "circle_outline", gpe::part_shape_circle_outline, false  );
    emitterBackupTextureMenu->add_menu_option("Line", "line",gpe::part_shape_line, false  );
    emitterBackupTextureMenu->add_menu_option("Line Capped", "line_capped",gpe::part_shape_linecapped, false  );


    showDebugInfo = new GPE_CheckBoxBasic("Show Debugger","Preview Particle Stats in Editor", true );
    emissionRateField = new gpe_text_widget_number("particles per second", true, 0, gpe::PARTICLE_MAX_PER_EMITTER );
    emissionRateField->set_number( 60 );
    emissionRateField->set_label( "Emission Rate Per Second" );

    particleCountField = new gpe_text_widget_number("Max Per Emitter", true, 0, gpe::PARTICLE_MAX_PER_EMITTER );
    particleCountField->set_number( 150 );

    lifeMinField = new gpe_text_widget_number("Min Life (seconds)", true, 0, 500 );
    lifeMinField->set_number( 5 );
    lifeMaxField = new gpe_text_widget_number("Max Life(seconds)", true, 0, 500 );
    lifeMaxField->set_number( 10 );

    colorsLabel = new GPE_Label_Title("Particle Colors","Particle Colors:" );
    sColorField = new gpe_widget_color_picker("StartColor","DefaultColorValues");
    sColorField->set_color_from_rgb( gpe::c_purple );

    sVColorField = new gpe_widget_color_picker("StartVarColor","DefaultVarColorValues");
    sVColorField->set_color_from_rgb( gpe::c_blgray );

    eColorField = new gpe_widget_color_picker("EndColor","EndVarColorValues");
    eColorField->set_color_from_rgb( gpe::c_yellow );

    eVColorField = new gpe_widget_color_picker("EndVarColor","EndVarColorValues");
    eVColorField->set_color_from_rgb( gpe::c_blgray );

    xStartPosField = new gpe_text_widget_number("X-Start(pixels)" );
    yStartPosField = new gpe_text_widget_number("Y-Start(pixels)" );

    xRandomField = new gpe_text_widget_number("X-Random(pixels)" );
    xRandomField->set_number( 8 );

    yRandomField = new gpe_text_widget_number("Y-Random(pixels)" );
    yRandomField->set_number( 8 );

    physicsLabel = new GPE_Label_Title("Particle Physics","Particle Physics:" );

    directionMin = new gpe_text_widget_number("Min Direction(degrees)", true, -720, 720 );
    directionMin->set_number( -360 );

    directionMax = new gpe_text_widget_number("Max Direction(degrees)", true, -720, 720 );
    directionMax->set_number( 360 );

    speedMin = new gpe_text_widget_number("Min Speed(pixels per second)", true, -720, 720 );
    speedMin->set_number( 3 );

    speedMax = new gpe_text_widget_number("Max Speed(pixels per second)", true, -720, 720 );
    speedMax->set_number( 7 );

    gravityXValue = new gpe_text_widget_number("Gravity-X (pixels)", true, -720, 720 );

    gravityYValue = new gpe_text_widget_number("Gravity-Y (pixels)", true, -720, 720 );

    shapeWField = new gpe_text_widget_number("Shape W(R) (pixels)", true, 0, 128 );
    shapeWField->set_number( 16 );

    shapeHField = new gpe_text_widget_number("Shape H (pixels)", true, 0, 128 );
    shapeHField->set_number( 16 );
}

particleResource::~particleResource()
{
    if( myEmitter!=NULL )
    {
        delete myEmitter;
        myEmitter = NULL;
    }
}

bool particleResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount )
{
    return true;
}

bool particleResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void particleResource::compile_cpp()
{

}

bool particleResource::get_mouse_coords( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    /*areaMouseXPos = 0;
    areaMouseYPos = 0;*/
    if( viewedSpace!=NULL && cam!=NULL )
    {
        if( gpe::point_within(   gpe::input->mouse_position_x,gpe::input->mouse_position_y,
                            viewedSpace->x - cam->x,
                            viewedSpace->y - cam->y,
                            viewedSpace->x + viewedSpace->w - cam->x,
                            viewedSpace->y + viewedSpace->h - cam->y ) )
        {
            /*
            areaMouseXPos = (gpe::input->mouse_position_x-animationPreviewCam->x )/zoomValue +animCameraRect.x - cam->x;
            areaMouseYPos = (gpe::input->mouse_position_y-animationPreviewCam->y )/zoomValue +animCameraRect.y - cam->y;
            */
            return true;
        }
    }
    return false;
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
        if( emitterTexture!=NULL && emitterTexture->get_width() > 512 || emitterTexture->get_height() > 512 )
        {
            if( GPE_main_Logs!=NULL )
            {
                GPE_main_Logs->log_general_error("Particle Emitter is too large. Texture must be 512x512px or smaller." );
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
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Loading Particle Emitter", resource_name );
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
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
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
                                    keyString = currLineToBeProcessed.substr(0,equalPos);
                                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( keyString=="Version")
                                    {
                                        foundFileVersion = stg_ex::string_to_float(valString);
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
                                else  if( keyString=="MaxParticles")
                                {
                                    particleCountField->set_string(valString);
                                }
                                else  if( keyString=="EmissionRate")
                                {
                                    emissionRateField->set_string(valString);
                                }
                                else  if( keyString=="LifeMin")
                                {
                                    lifeMinField->set_string(valString);
                                }
                                else  if( keyString=="LifeMax")
                                {
                                    lifeMaxField->set_string(valString);
                                }
                                else  if( keyString=="StartColor")
                                {
                                    sColorField->set_color_from_hex(valString);
                                }
                                else  if( keyString=="StartColorVar")
                                {
                                    sVColorField->set_color_from_hex(valString);
                                }
                                else  if( keyString=="EndColor")
                                {
                                    eColorField->set_color_from_hex(valString);
                                }
                                else  if( keyString=="EndColorVar")
                                {
                                    eVColorField->set_color_from_hex(valString);
                                }
                                else  if( keyString=="XStart")
                                {
                                    xStartPosField->set_string(valString);
                                }
                                else  if( keyString=="YStart")
                                {
                                    yStartPosField->set_string(valString);
                                }
                                else  if( keyString=="XRandom")
                                {
                                    xRandomField->set_string(valString);
                                }
                                else  if( keyString=="YRandom")
                                {
                                    yRandomField->set_string(valString);
                                }
                                else  if( keyString=="DirectionMin")
                                {
                                    directionMin->set_string(valString);
                                }
                                else  if( keyString=="DirectionMax")
                                {
                                    directionMax->set_string(valString);
                                }
                                else  if( keyString=="SpeedMin")
                                {
                                    speedMin->set_string(valString);
                                }
                                else  if( keyString=="SpeedMax")
                                {
                                    speedMax->set_string(valString);
                                }
                                else  if( keyString=="GravityXValue")
                                {
                                     gravityXValue->set_string(valString);
                                }
                                else  if( keyString=="GravityYValue")
                                {
                                    gravityYValue->set_string(valString);
                                }
                                else  if( keyString=="BlendType")
                                {
                                    blendTypeMenu->set_option_value( stg_ex::string_to_int(valString) );
                                }
                                else  if( keyString=="EmitterShape")
                                {
                                    shapeMenu->set_option_value( stg_ex::string_to_int(valString) );
                                }
                                else  if( keyString=="ShowDebugInfo")
                                {
                                    showDebugInfo->set_clicked( stg_ex::string_to_bool(valString) );
                                }
                                else  if( keyString=="Texture")
                                {
                                    load_image( soughtDir+valString);
                                }
                                else  if( keyString=="FallbackShape")
                                {
                                    emitterBackupTextureMenu->set_option_value( stg_ex::string_to_bool(valString) );
                                }
                                else  if( keyString=="ShapeW" || keyString=="ShapeR" )
                                {
                                     shapeWField->set_string(valString);
                                }
                                else  if( keyString=="ShapeH")
                                {
                                    shapeHField->set_string(valString);
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

void particleResource::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find( viewedSpace);
    cam = gpe::camera_find( cam );

    if( PANEL_GENERAL_EDITOR!=NULL )
    {
        int previousBlendMode = blendTypeMenu->get_selected_value();
        myEmitter->set_fallback_shape( emitterBackupTextureMenu->get_selected_value() );
        PANEL_GENERAL_EDITOR->clear_panel();

        PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);
        PANEL_GENERAL_EDITOR->add_gui_element(refreshResourceDataButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element(loadResourceButton,false);
        //PANEL_GENERAL_EDITOR->add_gui_element(saveResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element(transformResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element( openExternalEditorButton,true);

        PANEL_GENERAL_EDITOR->add_gui_element(textureLabel,true);
        PANEL_GENERAL_EDITOR->add_gui_element(textureLocationField,true);

        PANEL_GENERAL_EDITOR->add_gui_element(textureIsRotated,true);
        PANEL_GENERAL_EDITOR->add_gui_element(clearTextureButton,true);
        PANEL_GENERAL_EDITOR->add_gui_element(texturePreviewImgLabel,true);

        PANEL_GENERAL_EDITOR->add_gui_element(emitterBackupTextureMenu,true);
        PANEL_GENERAL_EDITOR->add_gui_element(shapeWField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(shapeHField,true);
        PANEL_GENERAL_EDITOR->add_gui_element( shapeMenu,true);
        PANEL_GENERAL_EDITOR->add_gui_element(particleCountField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(emissionRateField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(lifeMinField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(lifeMaxField,true);

        PANEL_GENERAL_EDITOR->add_gui_element(xStartPosField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(yStartPosField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(xRandomField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(yRandomField,true);


        PANEL_GENERAL_EDITOR->add_gui_element( blendTypeMenu,true);
        PANEL_GENERAL_EDITOR->add_gui_element(colorsLabel,true);
        PANEL_GENERAL_EDITOR->add_gui_element(sColorField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(sVColorField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(eColorField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(eVColorField,true);


        PANEL_GENERAL_EDITOR->add_gui_element(physicsLabel,true);
        PANEL_GENERAL_EDITOR->add_gui_element(directionMin,true);
        PANEL_GENERAL_EDITOR->add_gui_element(directionMax,true);

        PANEL_GENERAL_EDITOR->add_gui_element(speedMin,true);
        PANEL_GENERAL_EDITOR->add_gui_element(speedMax,true);

        PANEL_GENERAL_EDITOR->add_gui_element(gravityXValue,true);
        PANEL_GENERAL_EDITOR->add_gui_element(gravityYValue,true);

        PANEL_GENERAL_EDITOR->add_gui_element(showDebugInfo,true);
        PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);
        PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
        PANEL_GENERAL_EDITOR->process_self();

        if( loadResourceButton!=NULL && loadResourceButton->is_clicked() )
        {
            std::string newTextureLocation = GPE_GetOpenFileName("Choose Particle Texture","Images",main_GUI_SETTINGS->fileOpenTextureDir);
            if( sff_ex::file_exists( newTextureLocation) && stg_ex::file_is_image(newTextureLocation) )
            {
                load_image( newTextureLocation);
            }
        }
        else if( refreshResourceDataButton!=NULL && refreshResourceDataButton->is_clicked() )
        {
            if(  emitterTexture!=NULL )
            {
                std::string currentFileToRefresh = stg_ex::get_short_filename (emitterTexture->get_filename(),true );
                currentFileToRefresh = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/textures/"+currentFileToRefresh;
                load_image(currentFileToRefresh);
            }
        }
        else if( clearTextureButton!=NULL && clearTextureButton->is_clicked() )
        {
            emitterTexture = NULL;
            textureLocationField->set_string( "" );
            myEmitter->change_texture( NULL, blendTypeMenu->get_selected_value() );
            myEmitter->set_fallback_shape( emitterBackupTextureMenu->get_selected_value() );
            texturePreviewImgLabel->change_texture( NULL );
            texturePreviewImgLabel->set_width( 16 );
            texturePreviewImgLabel->set_height( 16 );
        }

        else if( transformResourceButton!=NULL && transformResourceButton->is_clicked())
        {
            if (emitterTexture!=NULL && emitterTexture->get_width() > 0 && emitterTexture->get_height() > 0)
            {
                GPE_open_context_menu(-1,-1,256);
                main_CONTEXT_MENU->add_menu_option("Erase BG Color",0);
                main_CONTEXT_MENU->add_menu_option("Invert Colors",1);
                main_CONTEXT_MENU->add_menu_option("Make GrayScale",2);
                main_CONTEXT_MENU->add_menu_option("Exit",10);
                int menuSelection = GPE_Get_Context_Result();
                if( menuSelection>=0 && menuSelection <=3)
                {
                    std::string tempStr = emitterTexture->get_filename();
                    SDL_Surface * oTempSurface = sdl_surface_ex::load_surface_image( tempStr.c_str());
                    SDL_Surface *nTempSurface = NULL;
                    if( oTempSurface!=NULL)
                    {
                        if( menuSelection==0)
                        {
                            gpe::color * foundBGColor = gpe::c_fuchsia->duplicate_color();
                            if( GPE_Change_color_PopUp("Image Background Color To Remove, ",foundBGColor) )
                            {
                                if( GPE_Display_Basic_Prompt("Are you sure you want to erase this Color from this image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                {
                                    gpe::error_log->report("Modifying image at: "+emitterTexture->get_filename()+".");
                                    nTempSurface= sdl_surface_ex::surface_remove_color_rgba(oTempSurface, foundBGColor->get_r(), foundBGColor->get_r(), foundBGColor->get_b() );

                                    delete foundBGColor;
                                    foundBGColor = NULL;
                                }
                            }

                        }
                        else if( menuSelection==1 )
                        {
                            if( GPE_Display_Basic_Prompt("Are you sure you want to invert your image's colors?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                            {
                                nTempSurface= sdl_surface_ex::surface_invert(oTempSurface);
                            }
                        }
                        else if( menuSelection==2 )
                        {
                            if( GPE_Display_Basic_Prompt("Are you sure you want to grayscale your image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                            {
                                nTempSurface= sdl_surface_ex::surface_grayscale(oTempSurface);
                            }
                        }
                        if( nTempSurface!=NULL)
                        {
                            remove( emitterTexture->get_filename().c_str() );
                            std::string newImageName = stg_ex::get_file_noext( emitterTexture->get_filename())+".png";
                            IMG_SavePNG(nTempSurface,newImageName.c_str() );
                            load_image(newImageName);
                            SDL_FreeSurface(nTempSurface);
                            nTempSurface = NULL;
                        }
                        SDL_FreeSurface(oTempSurface);
                        oTempSurface = NULL;
                    }
                }
            }
        }
        else if( openExternalEditorButton!=NULL && openExternalEditorButton->is_clicked() )
        {
            if(  emitterTexture!=NULL )
            {
                if( sff_ex::file_exists(emitterTexture->get_filename() ) )
                {
                    if( main_EDITOR_SETTINGS!=NULL && main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=NULL)
                    {
                        gpe::external_open_program(main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),emitterTexture->get_filename(), true );
                    }
                    else
                    {
                        gpe::external_open_url(emitterTexture->get_filename());
                    }
                    sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit texture ["+emitterTexture->get_filename()+"]...");
                }
            }
        }
        else if( confirmResourceButton->is_clicked() )
        {
            save_resource();
        }
        else if( cancelResourceButton->is_clicked() )
        {
            if( GPE_Display_Basic_Prompt("Are you sure you will like to reverse particle changes?","This will load in data from save-file!", true )== DISPLAY_QUERY_YES )
            {
                resourcePostProcessed = false;
                load_resource();
            }
        }

        if( previousBlendMode != blendTypeMenu->get_selected_value() )
        {
            if( emitterTexture!=NULL )
            {
                emitterTexture->set_blend_mode( blendTypeMenu->get_selected_value() );
            }
            myEmitter->set_blend_mode(  blendTypeMenu->get_selected_value() );
        }
    }

    if( myEmitter!=NULL )
    {
        myEmitter->emitterXPos = viewedSpace->w/2;
        myEmitter->emitterYPos = viewedSpace->h/2;

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
        if( textureIsRotated!=NULL )
        {
            myEmitter->rotateTexture = textureIsRotated->is_clicked();
        }
        myEmitter->process_emitter(  );
    }
}

void particleResource::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find( viewedSpace);
    cam = gpe::camera_find( cam );
    gpe::gcanvas->render_rectangle( 0,0,viewedSpace->w, viewedSpace->h, theme_main->program_color, false, 255 );
    //if( myEmitter!=NULL )
    {
        myEmitter->render( );
    }
    //gpe::gfs->render_text( viewedSpace->w/2, viewedSpace->h-32, "C++ Feature. Currently not supported in HTML5 runtime",theme_main->main_box_font_color,font_default, gpe::fa_center, gpe::fa_bottom, 255);
}

void particleResource::revert_data_fields()
{

}

void particleResource::save_resource(std::string file_path, int backupId )
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Particle", resource_name );
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
            if( particleCountField!=NULL )
            {
                newSaveDataFile << "MaxParticles=" + stg_ex::int_to_string(particleCountField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "MaxParticles=100\n";
            }
            if( emissionRateField!=NULL )
            {
                newSaveDataFile << "EmissionRate=" + stg_ex::int_to_string(emissionRateField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "EmissionRate=100\n";
            }

            if( lifeMinField!=NULL )
            {
                newSaveDataFile << "LifeMin=" + stg_ex::float_to_string( lifeMinField->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "LifeMin=100\n";
            }

            if( lifeMaxField!=NULL )
            {
                newSaveDataFile << "LifeMax=" + stg_ex::float_to_string(lifeMaxField->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "LifeMax=100\n";
            }

            if( sColorField!=NULL )
            {
                newSaveDataFile << "StartColor=" + sColorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "StartColor=#FFF\n";
            }

            if( sVColorField!=NULL )
            {
                newSaveDataFile << "StartColorVar=" + sVColorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "StartColorVar=#000\n";
            }

            if( eColorField!=NULL )
            {
                newSaveDataFile << "EndColor=" + eColorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "EndColor=#333\n";
            }

            if( eVColorField!=NULL )
            {
                newSaveDataFile << "EndColorVar=" + eVColorField->get_hex_string() +"\n";
            }
            else
            {
                newSaveDataFile << "EndColorVar=#000\n";
            }

            if( xStartPosField!=NULL )
            {
                newSaveDataFile << "XStart=" + stg_ex::int_to_string(xStartPosField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "XStart=0\n";
            }
            if( yStartPosField!=NULL )
            {
                newSaveDataFile << "YStart=" + stg_ex::int_to_string(yStartPosField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "YStart=0\n";
            }

            if( xRandomField!=NULL )
            {
                newSaveDataFile << "XRandom=" + stg_ex::int_to_string(xRandomField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "XRandom=32\n";
            }

            if( yRandomField!=NULL )
            {
                newSaveDataFile << "YRandom=" + stg_ex::int_to_string(yRandomField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "YRandom=32\n";
            }

            if( directionMin!=NULL )
            {
                newSaveDataFile << "DirectionMin=" + stg_ex::float_to_string(directionMin->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "DirectionMin=0\n";
            }

            if( directionMax!=NULL )
            {
                newSaveDataFile << "DirectionMax=" + stg_ex::float_to_string(directionMax->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "DirectionMax=360\n";
            }

            if( speedMin!=NULL )
            {
                newSaveDataFile << "SpeedMin=" + stg_ex::int_to_string(speedMin->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "SpeedMin=3\n";
            }

            if( speedMax!=NULL )
            {
                newSaveDataFile << "SpeedMax=" + stg_ex::int_to_string(speedMax->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "SpeedMax=5\n";
            }

            if( gravityXValue!=NULL )
            {
                newSaveDataFile << "GravityXValue=" + stg_ex::float_to_string(gravityXValue->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "GravityXValue=0\n";
            }


            if( gravityYValue!=NULL )
            {
                newSaveDataFile << "GravityYValue=" + stg_ex::float_to_string(gravityYValue->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "GravityYValue=0\n";
            }

            if( blendTypeMenu!=NULL )
            {
                newSaveDataFile << "BlendType=" + stg_ex::int_to_string(blendTypeMenu->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "BlendType=0\n";
            }
            if( shapeMenu!=NULL )
            {
                newSaveDataFile << "EmitterShape=" + stg_ex::int_to_string(shapeMenu->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "EmitterShape=0\n";
            }
            if( showDebugInfo!=NULL )
            {
                newSaveDataFile << "ShowDebugInfo=" + stg_ex::int_to_string(showDebugInfo->is_clicked() )+"\n";
            }
            else
            {
                newSaveDataFile << "ShowDebugInfo=0\n";
            }

            if( textureLocationField!=NULL )
            {
                newSaveDataFile << "Texture=" + stg_ex::get_local_from_global_file( textureLocationField->get_string() )+"\n";
            }

            if( emitterBackupTextureMenu!=NULL )
            {
                newSaveDataFile << "FallbackShape=" + stg_ex::int_to_string( emitterBackupTextureMenu->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "FallbackShape=0\n";
            }
            if( shapeWField!=NULL )
            {
                newSaveDataFile << "ShapeW=" + stg_ex::int_to_string( shapeWField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "ShapeW=16\n";
            }
            if( shapeHField!=NULL )
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
            GPE_main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void particleResource::update_box(int newX, int newY, int newW, int newH )
{

}

bool particleResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn  )
{
    //resource_nameLabel->set_name(renameBox->get_string() );
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            load_resource();
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "ParticleEmitter=" << resource_name << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}
