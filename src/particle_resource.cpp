/*
particle_resource.cpp
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

#include "particle_resource.h"
#include "gpe_editor_settings.h"


particleResource::particleResource(GPE_GeneralResourceContainer * pFolder)
{
    //bottomPaneList = new GPE_GuiElementList();
    projectParentFolder = pFolder;
    myEmitter = new GPE_ParticleEmitter(0, 0,true, 500 );
    myEmitter->showBox = true;
    myEmitter->set_emission_rate( 60 );
    emitterTexture = NULL;
    //emitterTexture->load_new_texture(APP_DIRECTORY_NAME+"resources/gfx/sprites/c_snow.png",-1,true );
    myEmitter->change_texture( emitterTexture, blend_mode_blend );
    transformResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/magic.png","Transform the Image",-1);
    openExternalEditorButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Texture Image In External Editor");
    refreshResourceDataButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded texture image");
    clearTextureButton = new GPE_ToolLabelButton("Clear Image...","Clears texture and reverts to shape" );
    textureIsRotated = new GPE_CheckBoxBasic("Rotate Texture","The Particle texture rotates with it's direction", true );


    textureLabel = new GPE_Label_Title("Particle Texture","Enter below:" );
    texturePreviewImgLabel = new GPE_Label_Image( NULL, "Particle-Texture" );
    textureLocationField = new GPE_TextInputBasic("","Texture Location...");

    blendTypeMenu = new GPE_DropDown_Menu("Blend Mode", false );
    blendTypeMenu->add_menu_option("Add","add",blend_mode_add, false );
    blendTypeMenu->add_menu_option("Blend","blend",blend_mode_blend, true  );
    blendTypeMenu->add_menu_option("Mod","mod",blend_mode_mod, false  );
    blendTypeMenu->add_menu_option("None","none",blend_mode_none, false  );

    shapeMenu = new GPE_DropDown_Menu("Emitter Shape", false );
    shapeMenu->add_menu_option("Point", "point",part_shape_point, false );
    shapeMenu->add_menu_option("Square", "square",part_shape_square, true  );
    shapeMenu->add_menu_option("Rectangle", "rectangle",part_shape_rectangle, false  );
    shapeMenu->add_menu_option("Circle", "circle",part_shape_circle, false  );

    emitterBackupTextureMenu = new GPE_DropDown_Menu("Fallback Shape", false );
    emitterBackupTextureMenu->add_menu_option("Square", "square",part_shape_square, false  );
    emitterBackupTextureMenu->add_menu_option("Square Outline", "square_outline",part_shape_square_outline, false  );
    emitterBackupTextureMenu->add_menu_option("Rectangle", "rectangle",part_shape_rectangle, false  );
    emitterBackupTextureMenu->add_menu_option("Rectangle Outline", "rectangle_outline",part_shape_rectangle_outline, false  );
    emitterBackupTextureMenu->add_menu_option("Circle", "circle",part_shape_circle, true  );
    emitterBackupTextureMenu->add_menu_option("Circle Outline", "circle_outline",part_shape_circle_outline, false  );
    emitterBackupTextureMenu->add_menu_option("Line", "line",part_shape_line, false  );
    emitterBackupTextureMenu->add_menu_option("Line Capped", "line_capped",part_shape_linecapped, false  );


    showDebugInfo = new GPE_CheckBoxBasic("Show Debugger","Preview Particle Stats in Editor", true );
    emissionRateField = new GPE_TextInputNumber("particles per second", true, 0, PARTICLE_MAX_PER_EMITTER );
    emissionRateField->set_number( 60 );
    emissionRateField->set_label( "Emission Rate Per Second" );

    particleCountField = new GPE_TextInputNumber("Max Per Emitter", true, 0, PARTICLE_MAX_PER_EMITTER );
    particleCountField->set_number( 150 );

    lifeMinField = new GPE_TextInputNumber("Min Life (seconds)", true, 0, 500 );
    lifeMinField->set_number( 5 );
    lifeMaxField = new GPE_TextInputNumber("Max Life(seconds)", true, 0, 500 );
    lifeMaxField->set_number( 10 );

    colorsLabel = new GPE_Label_Title("Particle Colors","Particle Colors:" );
    sColorField = new GPE_Input_Field_Color("StartColor","DefaultColorValues");
    sColorField->set_color_from_rgb( c_purple );

    sVColorField = new GPE_Input_Field_Color("StartVarColor","DefaultVarColorValues");
    sVColorField->set_color_from_rgb( c_blgray );

    eColorField = new GPE_Input_Field_Color("EndColor","EndVarColorValues");
    eColorField->set_color_from_rgb( c_yellow );

    eVColorField = new GPE_Input_Field_Color("EndVarColor","EndVarColorValues");
    eVColorField->set_color_from_rgb( c_blgray );

    xStartPosField = new GPE_TextInputNumber("X-Start(pixels)" );
    yStartPosField = new GPE_TextInputNumber("Y-Start(pixels)" );

    xRandomField = new GPE_TextInputNumber("X-Random(pixels)" );
    xRandomField->set_number( 8 );

    yRandomField = new GPE_TextInputNumber("Y-Random(pixels)" );
    yRandomField->set_number( 8 );

    physicsLabel = new GPE_Label_Title("Particle Physics","Particle Physics:" );

    directionMin = new GPE_TextInputNumber("Min Direction(degrees)", true, -720, 720 );
    directionMin->set_number( -360 );

    directionMax = new GPE_TextInputNumber("Max Direction(degrees)", true, -720, 720 );
    directionMax->set_number( 360 );

    speedMin = new GPE_TextInputNumber("Min Speed(pixels per second)", true, -720, 720 );
    speedMin->set_number( 3 );

    speedMax = new GPE_TextInputNumber("Max Speed(pixels per second)", true, -720, 720 );
    speedMax->set_number( 7 );

    gravityXValue = new GPE_TextInputNumber("Gravity-X (pixels)", true, -720, 720 );

    gravityYValue = new GPE_TextInputNumber("Gravity-Y (pixels)", true, -720, 720 );

    shapeWField = new GPE_TextInputNumber("Shape W(R) (pixels)", true, 0, 128 );
    shapeWField->set_number( 16 );

    shapeHField = new GPE_TextInputNumber("Shape H (pixels)", true, 0, 128 );
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

}

bool particleResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void particleResource::compile_cpp()
{

}

bool particleResource::get_mouse_coords(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    /*areaMouseXPos = 0;
    areaMouseYPos = 0;*/
    if( viewedSpace!=NULL && cam!=NULL )
    {
        if( point_within(   input->mouse_x,input->mouse_y,
                            viewedSpace->x - cam->x,
                            viewedSpace->y - cam->y,
                            viewedSpace->x + viewedSpace->w - cam->x,
                            viewedSpace->y + viewedSpace->h - cam->y ) )
        {
            /*
            areaMouseXPos = (input->mouse_x-spritePreviewCam->x )/zoomValue +animCameraRect.x - cam->x;
            areaMouseYPos = (input->mouse_y-spritePreviewCam->y )/zoomValue +animCameraRect.y - cam->y;
            */
            return true;
        }
    }
    return false;
}

void particleResource::handle_scrolling()
{

}

void particleResource::load_image(std::string newFileName, bool autoProcess )
{
    if( newFileName.size() > 4 && file_exists( newFileName ) )
    {
        textureLocationField->set_string( newFileName );
        if( emitterTexture==NULL )
        {
            emitterTexture = new GPE_Texture();
        }
        emitterTexture->load_new_texture( newFileName );
        if( emitterTexture->get_width() > 512 || emitterTexture->get_height() > 512 )
        {
            emitterTexture->prerender_circle(128, c_white, 255 );
            if( GPE_Main_Logs!=NULL )
            {
                GPE_Main_Logs->log_general_error("Particle Emitter is too large. Texture must be 512x512px or smaller." );
            }
            else
            {
                GPE_Report("Particle Emitter is too large. Texture must be 512x512px or smaller." );
            }
        }

        emitterTexture->set_blend_mode( blendTypeMenu->get_selected_value() );
        myEmitter->change_texture( emitterTexture, blendTypeMenu->get_selected_value() );
        texturePreviewImgLabel->load_label_image( newFileName );
        texturePreviewImgLabel->set_width( 64 );
        texturePreviewImgLabel->set_height( 64 );
        if( autoProcess )
        {
            copy_file( newFileName, fileToDir(parentProjectName)+"/gpe_project/resources/particles/"+ get_local_from_global_file(newFileName) );
        }
    }
    else
    {
        textureLocationField->set_string( "" );
    }
}

void particleResource::preprocess_self(std::string alternatePath )
{
    if( resourcePostProcessed ==false || file_exists(alternatePath))
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Loading Particle Emitter", resourceName );
        }

        std::string otherColContainerName = "";

        std::string newFileIn = "";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/particles/";
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
                                    blendTypeMenu->set_value( string_to_int(valString) );
                                }
                                else  if( keyString=="EmitterShape")
                                {
                                    shapeMenu->set_value( string_to_int(valString) );
                                }
                                else  if( keyString=="ShowDebugInfo")
                                {
                                    showDebugInfo->set_clicked( string_to_bool(valString) );
                                }
                                else  if( keyString=="Texture")
                                {
                                    load_image( soughtDir+valString);
                                }
                                else  if( keyString=="FallbackShape")
                                {
                                    emitterBackupTextureMenu->set_value( string_to_bool(valString) );
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

void particleResource::process_data_fields(double versionToProcess )
{

}

void particleResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera( viewedSpace);
    cam = GPE_find_camera( cam );

    if( PANEL_GENERAL_EDITOR!=NULL )
    {
        int previousBlendMode = blendTypeMenu->get_selected_value();
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
            std::string newTextureLocation = GPE_GetOpenFileName("Choose Particle Texture","Images",MAIN_GUI_SETTINGS->fileOpenTextureDir);
            if( file_exists( newTextureLocation) && file_is_image(newTextureLocation) )
            {
                load_image( newTextureLocation);
            }
        }
        else if( refreshResourceDataButton!=NULL && refreshResourceDataButton->is_clicked() )
        {
            if(  emitterTexture!=NULL )
            {
                std::string currentFileToRefresh = getShortFileName (emitterTexture->get_filename(),true );
                currentFileToRefresh = fileToDir(parentProjectName)+"/gpe_project/resources/textures/"+currentFileToRefresh;
                load_image(currentFileToRefresh);
            }
        }
        else if( clearTextureButton!=NULL && clearTextureButton->is_clicked() )
        {
            if( emitterTexture!=NULL )
            {
                delete emitterTexture;
                emitterTexture = NULL;
            }
            textureLocationField->set_string( "" );
            myEmitter->change_texture( NULL, blendTypeMenu->get_selected_value() );
            myEmitter->set_fallback_shape( emitterBackupTextureMenu->get_selected_value() );
            texturePreviewImgLabel->set_image( NULL );
            texturePreviewImgLabel->set_width( 16 );
            texturePreviewImgLabel->set_height( 16 );
        }

        else if( transformResourceButton!=NULL && transformResourceButton->is_clicked())
        {
            if (emitterTexture!=NULL && emitterTexture->get_width() > 0 && emitterTexture->get_height() > 0)
            {
                GPE_open_context_menu(-1,-1,256);
                MAIN_CONTEXT_MENU->add_menu_option("Erase BG Color",0);
                MAIN_CONTEXT_MENU->add_menu_option("Invert Colors",1);
                MAIN_CONTEXT_MENU->add_menu_option("Make GrayScale",2);
                MAIN_CONTEXT_MENU->add_menu_option("Exit",10);
                int menuSelection = GPE_Get_Context_Result();
                if( menuSelection>=0 && menuSelection <=3)
                {
                    SDL_Surface * oTempSurface = gpe_sdl->load_surface_image(emitterTexture->get_filename() );
                    SDL_Surface *nTempSurface = NULL;
                    if( oTempSurface!=NULL)
                    {
                        if( menuSelection==0)
                        {
                            GPE_Color * foundBGColor = c_fuchsia->duplicate_color();
                            if( GPE_Change_Color_PopUp("Image Background Color To Remove, ",foundBGColor) )
                            {
                                if( GPE_Display_Basic_Prompt("Are you sure you want to erase this Color from this image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                {
                                    GPE_Report("Modifying image at: "+emitterTexture->get_filename()+".");
                                    nTempSurface = gpe_sdl->surface_remove_color(oTempSurface,foundBGColor->get_sdl_color() );
                                    delete foundBGColor;
                                    foundBGColor = NULL;
                                }
                            }

                        }
                        else if( menuSelection==1 )
                        {
                            if( GPE_Display_Basic_Prompt("Are you sure you want to invert your image's colors?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                            {
                                nTempSurface= gpe_sdl->surface_invert(oTempSurface);
                            }
                        }
                        else if( menuSelection==2 )
                        {
                            if( GPE_Display_Basic_Prompt("Are you sure you want to grayscale your image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                            {
                                nTempSurface= gpe_sdl->surface_grayscale(oTempSurface);
                            }
                        }
                        if( nTempSurface!=NULL)
                        {
                            remove( emitterTexture->get_filename().c_str() );
                            std::string newImageName = get_file_noext(emitterTexture->get_filename())+".png";
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
                if( file_exists(emitterTexture->get_filename() ) )
                {
                    if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=NULL)
                    {
                        GPE_OpenProgram(MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),emitterTexture->get_filename(), true );
                    }
                    else
                    {
                        GPE_OpenURL(emitterTexture->get_filename());
                    }
                    appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit texture ["+emitterTexture->get_filename()+"]...");
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
                preprocess_self();
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

        myEmitter->xRandom = xRandomField->get_held_int();
        myEmitter->yRandom = yRandomField->get_held_int();

        if(emitterBackupTextureMenu->just_activated() )
        {
            myEmitter->set_fallback_shape( emitterBackupTextureMenu->get_selected_value() );
        }
        if( emissionRateField->is_inuse() == false )
        {
            myEmitter->set_emission_rate( emissionRateField->get_held_int() );
        }
        if( particleCountField->is_inuse() == false )
        {
            myEmitter->set_max_particles( particleCountField->get_held_int(), true );
        }

        if( input->check_keyboard_down( kb_space ) )
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

void particleResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera( viewedSpace);
    cam = GPE_find_camera( cam );
    gcanvas->render_rectangle( 0,0,viewedSpace->w, viewedSpace->h, GPE_MAIN_THEME->Program_Color, false, 255 );
    //if( forceRedraw && myEmitter!=NULL )
    {
        myEmitter->render_emitter( NULL );
    }
    gfs->render_text( viewedSpace->w/2, viewedSpace->h-32, "C++ Feature. Currently not supported in HTML5 runtime",GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT, FA_CENTER, FA_BOTTOM, 255);
}

void particleResource::revert_data_fields()
{

}

void particleResource::save_resource(std::string alternatePath, int backupId )
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Particle", resourceName );
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
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/particles/";
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
            if( particleCountField!=NULL )
            {
                newSaveDataFile << "MaxParticles=" + int_to_string(particleCountField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "MaxParticles=100\n";
            }
            if( emissionRateField!=NULL )
            {
                newSaveDataFile << "EmissionRate=" + int_to_string(emissionRateField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "EmissionRate=100\n";
            }

            if( lifeMinField!=NULL )
            {
                newSaveDataFile << "LifeMin=" + double_to_string( lifeMinField->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "LifeMin=100\n";
            }

            if( lifeMaxField!=NULL )
            {
                newSaveDataFile << "LifeMax=" + double_to_string(lifeMaxField->get_held_number() )+"\n";
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
                newSaveDataFile << "XStart=" + int_to_string(xStartPosField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "XStart=0\n";
            }
            if( yStartPosField!=NULL )
            {
                newSaveDataFile << "YStart=" + int_to_string(yStartPosField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "YStart=0\n";
            }

            if( xRandomField!=NULL )
            {
                newSaveDataFile << "XRandom=" + int_to_string(xRandomField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "XRandom=32\n";
            }

            if( yRandomField!=NULL )
            {
                newSaveDataFile << "YRandom=" + int_to_string(yRandomField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "YRandom=32\n";
            }

            if( directionMin!=NULL )
            {
                newSaveDataFile << "DirectionMin=" + double_to_string(directionMin->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "DirectionMin=0\n";
            }

            if( directionMax!=NULL )
            {
                newSaveDataFile << "DirectionMax=" + double_to_string(directionMax->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "DirectionMax=360\n";
            }

            if( speedMin!=NULL )
            {
                newSaveDataFile << "SpeedMin=" + int_to_string(speedMin->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "SpeedMin=3\n";
            }

            if( speedMax!=NULL )
            {
                newSaveDataFile << "SpeedMax=" + int_to_string(speedMax->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "SpeedMax=5\n";
            }

            if( gravityXValue!=NULL )
            {
                newSaveDataFile << "GravityXValue=" + double_to_string(gravityXValue->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "GravityXValue=0\n";
            }


            if( gravityYValue!=NULL )
            {
                newSaveDataFile << "GravityYValue=" + double_to_string(gravityYValue->get_held_number() )+"\n";
            }
            else
            {
                newSaveDataFile << "GravityYValue=0\n";
            }

            if( blendTypeMenu!=NULL )
            {
                newSaveDataFile << "BlendType=" + int_to_string(blendTypeMenu->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "BlendType=0\n";
            }
            if( shapeMenu!=NULL )
            {
                newSaveDataFile << "EmitterShape=" + int_to_string(shapeMenu->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "EmitterShape=0\n";
            }
            if( showDebugInfo!=NULL )
            {
                newSaveDataFile << "ShowDebugInfo=" + int_to_string(showDebugInfo->is_clicked() )+"\n";
            }
            else
            {
                newSaveDataFile << "ShowDebugInfo=0\n";
            }

            if( textureLocationField!=NULL )
            {
                newSaveDataFile << "Texture=" + get_local_from_global_file( textureLocationField->get_string() )+"\n";
            }

            if( emitterBackupTextureMenu!=NULL )
            {
                newSaveDataFile << "FallbackShape=" + int_to_string( emitterBackupTextureMenu->get_selected_value() )+"\n";
            }
            else
            {
                newSaveDataFile << "FallbackShape=0\n";
            }
            if( shapeWField!=NULL )
            {
                newSaveDataFile << "ShapeW=" + int_to_string( shapeWField->get_held_int() )+"\n";
            }
            else
            {
                newSaveDataFile << "ShapeW=16\n";
            }
            if( shapeHField!=NULL )
            {
                newSaveDataFile << "ShapeH=" + int_to_string( shapeHField->get_held_int() )+"\n";
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
            GPE_Main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void particleResource::update_box(int newX, int newY, int newW, int newH )
{

}

bool particleResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn  )
{
    //resourceNameLabel->set_name(renameBox->get_string() );
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            preprocess_self();
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "ParticleEmitter=" << resourceName << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}
