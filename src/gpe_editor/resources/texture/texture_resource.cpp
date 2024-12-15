/*
texture_resource.cpp
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

#include "texture_resource.h"
#include "../../editor/gpe_editor_settings.h"

textureResource::textureResource(pawgui::widget_resource_container * pFolder)
{
    for( int i_quad = 0; i_quad < 4; i_quad ++ )
    {
        quad_points[i_quad].x = 0;
        quad_points[i_quad].y = 0;
    }
    quad_place_position = 0;
    polygon_point_count = 3;
    projectParentFolder = pFolder;
    editorMode = 0;
    textureInEditor = nullptr;
    isPreloaded = true;
    preview_mode_checkbox = new pawgui::widget_dropdown_menu("Texture Preview Mode", false );


    preview_mode_checkbox->add_menu_option("Full Texture", "Full Texture", gpe_tex_preview_full, true );
    preview_mode_checkbox->add_menu_option("Texture Circle", "Texture Circle", gpe_tex_preview_circle, false );
    preview_mode_checkbox->add_menu_option("Texture Quad", "Texture Quad", gpe_tex_preview_quad, false );
    preview_mode_checkbox->add_menu_option("Texture Polygon", "Texture Polygon", gpe_tex_preview_polygon, false );

    preview_mode_follows_mouse = new pawgui::widget_checkbox("Preview Mode Follows Mouse","Preview Mode Follows Mouse", true);
    preloadCheckBox = new pawgui::widget_checkbox("Preload Texture","Check to load texture at game open", true);
    textureUsesPixels = new pawgui::widget_checkbox("Pixel Data Accessible","Useful for mode7/raycasting and other effects", false );
    labelImageDimensions = new pawgui::widget_label_text ("","");
    //labelTextureMessage = new pawgui::widget_label_text ("","");
    openExternalEditor_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Texture Image In External Editor");
    refreshResourceData_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded texture image");
    labelInfoMaxTextureSize = new pawgui::widget_label_text ("Max Image Size: 4096 X 4096px","Max Image Size: 4096 X 4096px");
    imageUsesColorKey = new pawgui::widget_checkbox("Image Transparent?","Image uses a color key?", false );
    imageColorKey = new pawgui::gpe_widget_color_picker("Image Color Key","The RGB value of image's color key",255,0,255 );
}

textureResource::~textureResource()
{
    if( labelInfoMaxTextureSize!=nullptr)
    {
        delete labelInfoMaxTextureSize;
        labelInfoMaxTextureSize = nullptr;
    }
    if( openExternalEditor_button!=nullptr)
    {
        delete openExternalEditor_button;
        openExternalEditor_button = nullptr;
    }
    if( refreshResourceData_button!=nullptr)
    {
        delete refreshResourceData_button;
        refreshResourceData_button = nullptr;
    }

    if( preview_mode_checkbox != nullptr )
    {
        delete preview_mode_checkbox;
        preview_mode_checkbox = nullptr;
    }

    if( preview_mode_follows_mouse != nullptr )
    {
        delete preview_mode_follows_mouse;
        preview_mode_follows_mouse = nullptr;
    }

    if( preloadCheckBox!=nullptr)
    {
        delete preloadCheckBox;
        preloadCheckBox = nullptr;
    }

    if( textureUsesPixels!=nullptr)
    {
        delete textureUsesPixels;
        textureUsesPixels = nullptr;
    }

    if( labelImageDimensions!=nullptr)
    {
        delete labelImageDimensions;
        labelImageDimensions = nullptr;
    }
}

bool textureResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void textureResource::compile_cpp()
{

}

gpe::texture_base * textureResource::get_resource_texture()
{
    if( textureInEditor!=nullptr && textureInEditor->get_width() > 0 )
    {
        return textureInEditor;
    }
    return nullptr;
}

bool textureResource::include_local_files( std::string pBuildDir , int buildType )
{
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt",get_name() +"...");

    if( ( textureInEditor!=nullptr) && ( textureInEditor->get_width() > 0 ) )
    {
        return textureInEditor->copy_image_source(pBuildDir+"/resources/textures");
    }
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt","Does not contain texture...");
    return true;
}

bool textureResource::is_build_ready()
{
    recentErrorMessage = "";
    if( textureInEditor == nullptr )
    {
         recentErrorMessage = "Texture=nullptr";
         return false;
    }
    bool isReady = true;
    if( textureInEditor->get_width() <= 0 )
    {
        recentErrorMessage = "Texture-Width=0";
        isReady = false;
    }
    if( textureInEditor->get_height() <=0 )
    {
        recentErrorMessage += "Texture-Height=0";
        isReady = false;

    }
    return isReady;
}

int textureResource::load_image(std::string new_file_name)
{
    if( sff_ex::file_exists(new_file_name) )
    {
        if( stg_ex::file_is_image(new_file_name) )
        {
            textureInEditor = gpe::rsm->texture_add_filename( new_file_name );
            textureInEditor->load_new_texture( new_file_name, -1,  imageUsesColorKey->is_clicked (), false,imageColorKey->get_r(),imageColorKey->get_g(),imageColorKey->get_b() );
            textureInEditor->copy_image_source( stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/textures");
            return true;
        }
        else
        {
            return false;
        }
    }
    return -1;
}

void textureResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed ==false  || sff_ex::file_exists(file_path) )
    {
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->update_submessages( "Processing Texture",resource_name );
        }

        std::string otherColContainerName = "";

        std::string newFileIn ="";

        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/textures/";
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

        //gpe::error_log->report("Loading Texture - "+newFileIn);
        //If the level file could be loaded
        std::string found_tx_string = "";
        if( !gameResourceFileIn.fail() )
        {
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
                float foundFileVersion = 0;
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
                                else if( key_string=="TransparentImage")
                                {
                                    imageUsesColorKey->set_checked( stg_ex::string_to_bool(valstring) );
                                }
                                else if( key_string=="ColorKeyValue")
                                {
                                    imageColorKey->set_color_from_rgb( valstring );
                                }
                                else if( key_string=="ImageLocation")
                                {
                                    if( valstring!="nullptr")
                                    {
                                        found_tx_string = soughtDir+valstring;
                                    }
                                }
                                else if( key_string=="Preload")
                                {
                                    isPreloaded = stg_ex::string_to_bool(valstring);
                                    preloadCheckBox->set_checked(isPreloaded );
                                }
                            }
                        }
                    }
                    else
                    {
                        gpe::error_log->report("Invalid FoundFileVersion ="+ stg_ex::float_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
        if( (int)found_tx_string.size() > 0  )
        {
            if( load_image(found_tx_string ) == 1 && textureInEditor != nullptr )
            {
                labelImageDimensions->set_name("Image Size: "+ stg_ex::int_to_string(textureInEditor->get_width() )+" x "+ stg_ex::int_to_string(textureInEditor->get_height() )+"px" );
            }
        }
    }
}

void textureResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
    if( preloadCheckBox!=nullptr)
    {
        preloadCheckBox->prerender_self();
    }
}

void textureResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if(cam!=nullptr && view_space!=nullptr && panel_main_editor!=nullptr )
    {
        /*
        if( textureInEditor!=nullptr)
            {
                render_text( pawgui::padding_default,pawgui::padding_default*2+preloadCheckBox->get_ypos()+preloadCheckBox->get_height(),
                int_to_string(textureInEditor->get_width() )+" x "+ stg_ex::int_to_string(textureInEditor->get_height() )+"px",
                pawgui::theme_main->main_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top);
            }
            else
            {
                render_text( pawgui::padding_default,pawgui::padding_default*2+preloadCheckBox->get_ypos()+preloadCheckBox->get_height(),
                "Image not loaded",pawgui::theme_main->main_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top);
            }
        */

        if( get_mouse_coords( view_space, cam ) )
        {
            if(preview_mode_checkbox->get_selected_value() == gpe_tex_preview_quad )
            {
                if( gpe::input->check_mouse_released( mb_left) )
                {
                    //The fun sprite/animation test zone
                    if( quad_place_position < 0 )
                    {
                        quad_place_position = 0;
                    }
                    else if ( quad_place_position > 3 )
                    {
                        quad_place_position = 0;
                    }

                    quad_points[quad_place_position].x = local_mouse_x;
                    quad_points[quad_place_position].y = local_mouse_y;

                    quad_place_position++;

                    if( quad_place_position < 0 )
                    {
                        quad_place_position = 0;
                    }
                    else if ( quad_place_position > 3 )
                    {
                        quad_place_position = 0;
                    }
                }
            }
            else if(preview_mode_checkbox->get_selected_value() == gpe_tex_preview_polygon )
            {
                if( gpe::input->check_mouse_released( mb_left) )
                {
                    polygon_point_count++;
                }
                else if( gpe::input->check_mouse_released( mb_right) )
                {
                    if( polygon_point_count >3 )
                    {
                        polygon_point_count--;
                    }
                }
            }
        }

        panel_main_editor->clear_panel();

        panel_main_editor->add_gui_element(renameBox,true);
        panel_main_editor->add_gui_element(refreshResourceData_button,false);
        panel_main_editor->add_gui_element(loadResource_button,false);
        //panel_main_editor->add_gui_element(saveResource_button,false);
        panel_main_editor->add_gui_element( openExternalEditor_button,true);

        if( textureInEditor!=nullptr)
        {
            labelImageDimensions->set_name("Image Size: "+ stg_ex::int_to_string(textureInEditor->get_width() )+" x "+ stg_ex::int_to_string(textureInEditor->get_height() )+"px");
        }
        else
        {
            labelImageDimensions->set_name("Image not loaded");
        }
        panel_main_editor->add_gui_element( textureUsesPixels,true);
        panel_main_editor->add_gui_element( labelImageDimensions,true);
        panel_main_editor->add_gui_element(labelInfoMaxTextureSize,true);

        panel_main_editor->add_gui_element(imageUsesColorKey,true);
        panel_main_editor->add_gui_element(imageColorKey,true);

        panel_main_editor->add_gui_element(preview_mode_checkbox,true);
        panel_main_editor->add_gui_element(preview_mode_follows_mouse,true);

        panel_main_editor->add_gui_element(preloadCheckBox,true);
        panel_main_editor->add_gui_element(confirmResource_button,true);
        panel_main_editor->add_gui_element(cancelResource_button,true);
        //panel_main_editor->set_maxed_out_width();
        panel_main_editor->process_self(nullptr, nullptr);



        if( loadResource_button!=nullptr && loadResource_button->is_clicked() )
        {
            std::string new_file_name = pawgui::get_filename_open_from_popup("Load Your Texture and such...","Images",pawgui::main_settings->fileOpenTextureDir);
            if( (int)new_file_name.size() > 3)
            {
                int loadResult = load_image(new_file_name);
                if( loadResult==false)
                {
                    pawgui::display_user_alert("Unable to load image","File type["+stg_ex::get_file_ext(new_file_name)+"] not supported when loading ["+new_file_name+"].");
                }
                else if( loadResult==-1)
                {
                    pawgui::display_user_alert("Unable to load image","File does not exist["+new_file_name+"].");
                }
            }
            std::string currentFileToRefresh = stg_ex::get_short_filename (textureInEditor->get_filename(),true );
        }
        else if( refreshResourceData_button!=nullptr && refreshResourceData_button->is_clicked() )
        {
            if(  textureInEditor!=nullptr )
            {
                std::string currentFileToRefresh = stg_ex::get_short_filename (textureInEditor->get_filename(),true );
                currentFileToRefresh = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/textures/"+currentFileToRefresh;
                load_image(  currentFileToRefresh );
            }
        }
        else if( confirmResource_button->is_clicked() )
        {
            save_resource();
        }
        else if( cancelResource_button->is_clicked() )
        {
            if( pawgui::display_prompt_message("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
            {
                resourcePostProcessed = false;
                load_resource();
            }
        }
        if(renameBox!=nullptr)
        {
            if( renameBox->get_string()!=resource_name)
            {
                isModified = true;
            }
        }

        if(isPreloaded!=preloadCheckBox->is_clicked() )
        {
            isModified = true;
        }
        if( confirmResource_button!=nullptr)
        {
            //actual saving of the tilesheet onto the engine and files
            if( confirmResource_button->is_clicked() )
            {
                isPreloaded = preloadCheckBox->is_clicked();
            }
        }

        if( openExternalEditor_button!=nullptr && textureInEditor!=nullptr)
        {
            if( openExternalEditor_button->is_clicked() )
            {
                if( sff_ex::file_exists(textureInEditor->get_filename() ) )
                {
                    std::string external_editor_program = "";
                    if( main_editor_settings!=nullptr && main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=nullptr)
                    {
                        external_editor_program = main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string();
                    }

                    if( (int)external_editor_program.size() > 0 )
                    {
                        gpe::external_open_program( external_editor_program, textureInEditor->get_filename(), true );
                    }
                    else
                    {
                         gpe::external_open_program(textureInEditor->get_filename());
                    }
                    sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit texture ["+textureInEditor->get_filename()+"]...");
                }
            }
        }
    }
}

void textureResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);

    if( gpe::gcanvas->is_render_mode_supported( gpe::gcanvas->get_render_mode() ) !=1 )
    {
        //We draw the message that our current render can not draw the preview in this editor and then exit the render function :-(
        gpe::gfs->render_text( view_space->w/2,view_space->h/2,
                          "Render Mode ["+gpe::gcanvas->get_render_mode_name( gpe::gcanvas->get_render_mode())+"] is not supported by "+gpe::gcanvas->get_artist_name()+ " artist backend",
                              pawgui::theme_main->program_color_header, gpe::font_default, gpe::fa_center, gpe::fa_middle );
        return;
    }

    if(cam!=nullptr && view_space!=nullptr)
    {
        //renders the right side of the area, mainly preview of tilesheet
        //renderer_main->set_viewpoint( &texturePreviewCam );
        if( texture_transparent_bg!=nullptr && pawgui::theme_main->theme_texture_bg==nullptr )
        {
            for(int iPV= 0; iPV< view_space->w; iPV+=texture_transparent_bg->get_width() )
            {
                for(int jPV= 0; jPV< view_space->h; jPV+=texture_transparent_bg->get_height() )
                {
                    texture_transparent_bg->render_tex( iPV,jPV,nullptr);
                }
            }
        }
        //IMG_SavePNG()
        //renders the tilesheet in edit
        if( textureInEditor!=nullptr)
        {
            int preview_angle = 90;


            if( preview_mode_follows_mouse->is_clicked() )
            {
                preview_angle = semath::point_direction( view_space->w/2,view_space->h/2, local_mouse_x, local_mouse_y ) * semath::math_degrees_multiplier;
            }

            if(preview_mode_checkbox->get_selected_value() == gpe_tex_preview_circle )
            {
                gpe::gcanvas->render_circle_filled( view_space->w/2, view_space->h/2, view_space->w/4 );
                textureInEditor->render_tex_circle( view_space->w/2, view_space->h/2, view_space->w/4, 256, nullptr, 255, nullptr );
            }
            else if(preview_mode_checkbox->get_selected_value() == gpe_tex_preview_quad )
            {
                if( textureInEditor->render_tex_quad( quad_points[0], quad_points[1], quad_points[2], quad_points[3], gpe::c_white, 255 ) == false )
                {
                    gpe::error_log->report("Failed to draw quad of texture...");
                }

                for( int i_quad_pos = 0; i_quad_pos < 4; i_quad_pos++ )
                {
                    gpe::gfs->render_text_boxed( quad_points[i_quad_pos].x,quad_points[i_quad_pos].y,
                            stg_ex::float_to_string(i_quad_pos ),
                                gpe::c_white, gpe::c_black, gpe::font_default, gpe::fa_center, gpe::fa_middle );
                }
            }
            else if(preview_mode_checkbox->get_selected_value() == gpe_tex_preview_polygon )
            {
                textureInEditor->render_tex_polygon( view_space->w/2, view_space->h/2, std::min( view_space->w/4, view_space->h/4) ,polygon_point_count, nullptr, 255, preview_angle );
            }
            else
            {
                if( textureInEditor->get_width() <=view_space->w && textureInEditor->get_height() <=view_space->h )
                {
                    textureInEditor->render_tex( 0, 0,nullptr);
                }
                else
                {
                    float neededTexture_scale= (float)std::min( (float)view_space->w/ (float)textureInEditor->get_width(),  (float)view_space->h / (float)textureInEditor->get_height() );
                    textureInEditor->render_tex_scaled( 0, 0,neededTexture_scale,neededTexture_scale,nullptr);
                }
            }
        }
    }
}

void textureResource::save_resource(std::string file_path, int backupId)
{
    isModified = false;
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Saving Texture",resource_name );
    }
    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = stg_ex::get_path_from_file(file_path);
    if(  sff_ex::path_exists(soughtDir) )
    {
        newFileOut = file_path;
        usingAltSaveSource= true;
    }
    else
    {
        //main_editor_log->log_general_error("Path ["+soughtDir+"] does not exist");
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/textures/";
        newFileOut = soughtDir + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {

        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);

            if(imageUsesColorKey!=nullptr)
            {
                newSaveDataFile << "TransparentImage=" << imageUsesColorKey->is_clicked() << "\n";
            }
            else
            {
                newSaveDataFile << "TransparentImage=1\n";
            }

            if(imageColorKey!=nullptr)
            {
                newSaveDataFile << "ColorKeyValue=" << imageColorKey->get_rgb_string() << ",\n";
            }
            else
            {
                newSaveDataFile << "ColorKeyValue=255,0,255,\n";
            }

            if( textureInEditor!=nullptr)
            {
                std::string resfile_location = stg_ex::get_short_filename (textureInEditor->get_filename(),true );
                newSaveDataFile << "ImageLocation="+resfile_location+"\n";
                if( (int)resfile_location.size() > 0 && usingAltSaveSource )
                {
                    std::string resFileCopySrc = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/textures/"+resfile_location;
                    std::string resFileCopyDest = soughtDir+resfile_location;
                    if( sff_ex::file_exists(resFileCopyDest) )
                    {
                        /*
                        if( pawgui::display_prompt_message("[WARNING]Texture Image Already exists?","Are you sure you will like to overwrite your ["+resfile_location+"] texture file? This action is irreversible!")==pawgui::display_query_yes)
                        {
                            file_copy(resFileCopySrc,resFileCopyDest);
                        }
                        */
                    }
                    else
                    {
                         sff_ex::file_copy(resFileCopySrc,resFileCopyDest);
                    }
                }
            }
            if( preloadCheckBox!=nullptr)
            {
                newSaveDataFile << "Preload="+ stg_ex::int_to_string(preloadCheckBox->is_clicked() )+"\n";
            }
            newSaveDataFile.close();
            if( !usingAltSaveSource)
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


void textureResource::update_box(int x_new, int y_new, int newW, int newH)
{
    if( x_new!=-1)
    {
        encapBox.x = x_new;
    }
    if(y_new!=-1)
    {
        encapBox.y = y_new;
    }

    if( newW!=-1)
    {
        encapBox.w = newW;
    }
    if(newH!=-1)
    {
        encapBox.h = newH;
    }
}

bool textureResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Texture=" << resource_name << "," << get_global_rid() << ",";
            *fileTarget << "\n";
            return true;
        }
    }
    return false;
}

