/*
texture_resource.cpp
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

#include "texture_resource.h"
#include "gpe_editor_settings.h"

textureResource::textureResource(GPE_GeneralResourceContainer * pFolder)
{
    projectParentFolder = pFolder;
    editorMode = 0;
    textureInEditor = NULL;
    isPreloaded = true;
    preloadCheckBox = new GPE_CheckBoxBasic("Preload Texture","Check to load texture at game open", true);
    transformResourceButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magic.png","Transform the Image",-1);
    labelImageDimensions = new GPE_Label_Text("","");
    //labelTextureMessage = new GPE_Label_Text("","");
    openExternalEditorButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Texture Image In External Editor");
    refreshResourceDataButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded texture image");
    labelInfoMaxTextureSize = new GPE_Label_Text("Max Image Size: 4096 X 4096px","Max Image Size: 4096 X 4096px");
}

textureResource::~textureResource()
{
    if( labelInfoMaxTextureSize!=NULL)
    {
        delete labelInfoMaxTextureSize;
        labelInfoMaxTextureSize = NULL;
    }
    if( openExternalEditorButton!=NULL)
    {
        delete openExternalEditorButton;
        openExternalEditorButton = NULL;
    }
    if( refreshResourceDataButton!=NULL)
    {
        delete refreshResourceDataButton;
        refreshResourceDataButton = NULL;
    }

    if( preloadCheckBox!=NULL)
    {
        delete preloadCheckBox;
        preloadCheckBox = NULL;
    }
    if( transformResourceButton!=NULL)
    {
        delete transformResourceButton;
        transformResourceButton = NULL;
    }
    if( labelImageDimensions!=NULL)
    {
        delete labelImageDimensions;
        labelImageDimensions = NULL;
    }
}

bool textureResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string html5TSName = get_name();
        if( textureInEditor!=NULL)
        {
            *fileTarget << nestedTabsStr << "var " << html5TSName << " =  GPE.rsm.texture_add(";
            *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) +",";
            *fileTarget << "'resources/textures/"+stg_ex::get_short_filename (textureInEditor->get_filename(),true )+"'";
            *fileTarget << "); \n";
        }
        else
        {
            *fileTarget << nestedTabsStr << "var " << html5TSName << " = GPE.rsm.texture_add( -1 );\n";
        }
    }
    return false;
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
    if( textureInEditor!=NULL && textureInEditor->get_width() > 0 )
    {
        return textureInEditor;
    }
    return NULL;
}

bool textureResource::include_local_files( std::string pBuildDir , int buildType )
{
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt",get_name() +"...");

    if( ( textureInEditor!=NULL) && ( textureInEditor->get_width() > 0 ) )
    {
        return textureInEditor->copy_image_source(pBuildDir+"/resources/textures");
    }
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt","Does not contain texture...");
    return true;
}

bool textureResource::is_build_ready()
{
    recentErrorMessage = "";
    if( textureInEditor == NULL )
    {
         recentErrorMessage = "Texture=NULL";
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
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Texture",resource_name );
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
        if( !gameResourceFileIn.fail() )
        {
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
                                else if( keyString=="ImageLocation")
                                {
                                    if( valString!="NULL")
                                    {
                                        load_image( soughtDir+valString );
                                    }
                                }
                                else if( keyString=="Preload")
                                {
                                    isPreloaded = stg_ex::string_to_bool(valString);
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
    }
}

void textureResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
    if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self();
    }
}

void textureResource::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if(cam!=NULL && viewedSpace!=NULL && PANEL_GENERAL_EDITOR!=NULL)
    {
        /*
        if( textureInEditor!=NULL)
            {
                render_text( GENERAL_GPE_GUI_PADDING,GENERAL_GPE_GUI_PADDING*2+preloadCheckBox->get_ypos()+preloadCheckBox->get_height(),
                int_to_string(textureInEditor->get_width() )+" x "+ stg_ex::int_to_string(textureInEditor->get_height() )+"px",
                theme_main->main_box_font_color,font_default,gpe::fa_left,gpe::fa_top);
            }
            else
            {
                render_text( GENERAL_GPE_GUI_PADDING,GENERAL_GPE_GUI_PADDING*2+preloadCheckBox->get_ypos()+preloadCheckBox->get_height(),
                "Image not loaded",theme_main->main_box_font_color,font_default,gpe::fa_left,gpe::fa_top);
            }
        */
        PANEL_GENERAL_EDITOR->clear_panel();

        PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);
        PANEL_GENERAL_EDITOR->add_gui_element(refreshResourceDataButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element(loadResourceButton,false);
        //PANEL_GENERAL_EDITOR->add_gui_element(saveResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element(transformResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element( openExternalEditorButton,true);

        if( textureInEditor!=NULL)
        {
            labelImageDimensions->set_name("Image Size: "+ stg_ex::int_to_string(textureInEditor->get_width() )+" x "+ stg_ex::int_to_string(textureInEditor->get_height() )+"px");
        }
        else
        {
            labelImageDimensions->set_name("Image not loaded");
        }
        PANEL_GENERAL_EDITOR->add_gui_element( labelImageDimensions,true);
        PANEL_GENERAL_EDITOR->add_gui_element(labelInfoMaxTextureSize,true);

        PANEL_GENERAL_EDITOR->add_gui_element(preloadCheckBox,true);
        PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);
        PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
        //PANEL_GENERAL_EDITOR->set_maxed_out_width();
        //PANEL_GENERAL_EDITOR->process_self(NULL, NULL);
        if( editorMode==0)
        {
            if( loadResourceButton!=NULL && loadResourceButton->is_clicked() )
            {
                std::string new_file_name = GPE_GetOpenFileName("Load Your Texture and such...","Images",main_GUI_SETTINGS->fileOpenTextureDir);
                if( (int)new_file_name.size() > 3)
                {
                    int loadResult = load_image(new_file_name);
                    if( loadResult==false)
                    {
                        display_user_alert("Unable to load image","File type["+stg_ex::get_file_ext(new_file_name)+"] not supported when loading ["+new_file_name+"].");
                    }
                    else if( loadResult==-1)
                    {
                        display_user_alert("Unable to load image","File does not exist["+new_file_name+"].");
                    }
                }
            }
            else if( refreshResourceDataButton!=NULL && refreshResourceDataButton->is_clicked() )
            {
                if(  textureInEditor!=NULL )
                {
                    std::string currentFileToRefresh = stg_ex::get_short_filename (textureInEditor->get_filename(),true );
                    currentFileToRefresh = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/textures/"+currentFileToRefresh;
                    load_image(currentFileToRefresh);
                }
            }
            else if( confirmResourceButton->is_clicked() )
            {
                save_resource();
            }
            else if( cancelResourceButton->is_clicked() )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== DISPLAY_QUERY_YES )
                {
                    resourcePostProcessed = false;
                    load_resource();
                }
            }
            if(renameBox!=NULL)
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
            if( confirmResourceButton!=NULL)
            {
                //actual saving of the tilesheet onto the engine and files
                if( confirmResourceButton->is_clicked() )
                {
                    isPreloaded = preloadCheckBox->is_clicked();
                }
            }
            if( transformResourceButton!=NULL)
            {
                if( transformResourceButton->is_clicked() && textureInEditor!=NULL)
                {
                    if( textureInEditor->get_width() > 0 && textureInEditor->get_height() > 0)
                    {
                        GPE_open_context_menu(-1,-1,256);
                        main_CONTEXT_MENU->add_menu_option("Erase BG Color",0);
                        main_CONTEXT_MENU->add_menu_option("Invert Colors",1);
                        main_CONTEXT_MENU->add_menu_option("Make GrayScale",2);
                        main_CONTEXT_MENU->add_menu_option("Exit",10);
                        int menuSelection = GPE_Get_Context_Result();
                        if( menuSelection>=0 && menuSelection <=3)
                        {
                            std::string tempStr = textureInEditor->get_filename();
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
                                            gpe::error_log->report("Modifying image at: "+textureInEditor->get_filename()+".");
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
                                    remove( textureInEditor->get_filename().c_str() );
                                    std::string newImageName = stg_ex::get_file_noext(textureInEditor->get_filename())+".png";
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
            }

            if( openExternalEditorButton!=NULL && textureInEditor!=NULL)
            {
                if( openExternalEditorButton->is_clicked() )
                {
                    if( sff_ex::file_exists(textureInEditor->get_filename() ) )
                    {
                        if( main_EDITOR_SETTINGS!=NULL && main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=NULL)
                        {
                            gpe::external_open_program(main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),textureInEditor->get_filename(), true );
                        }
                        else
                        {
                             gpe::external_open_url(textureInEditor->get_filename());
                        }
                        sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit texture ["+textureInEditor->get_filename()+"]...");
                    }
                }
            }

        }
    }
}

void textureResource::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if(cam!=NULL && viewedSpace!=NULL)
    {
        //renders the right side of the area, mainly preview of tilesheet
        //renderer_main->set_viewpoint( &texturePreviewCam );
        if( GPE_TEXTURE_TRANSPARENT_BG!=NULL && theme_main->theme_texture_bg==NULL )
        {
            for(int iPV= 0; iPV< viewedSpace->w; iPV+=GPE_TEXTURE_TRANSPARENT_BG->get_width() )
            {
                for(int jPV= 0; jPV< viewedSpace->h; jPV+=GPE_TEXTURE_TRANSPARENT_BG->get_height() )
                {
                    GPE_TEXTURE_TRANSPARENT_BG->render_tex( iPV,jPV,NULL);
                }
            }
        }
        //IMG_SavePNG()
        //renders the tilesheet in edit
        if( textureInEditor!=NULL)
        {
            if( textureInEditor->get_width() <=viewedSpace->w && textureInEditor->get_height() <=viewedSpace->h )
            {
                textureInEditor->render_tex( 0, 0,NULL);
            }
            else
            {
                float neededTextureScale= (float)std::min( (float)viewedSpace->w/ (float)textureInEditor->get_width(),  (float)viewedSpace->h / (float)textureInEditor->get_height() );
                textureInEditor->render_tex_scaled( 0, 0,neededTextureScale,neededTextureScale,NULL);
            }
        }
    }
}

void textureResource::save_resource(std::string file_path, int backupId)
{
    isModified = false;
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Texture",resource_name );
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
        //GPE_main_Logs->log_general_error("Path ["+soughtDir+"] does not exist");
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

            if( textureInEditor!=NULL)
            {
                std::string resFileLocation = stg_ex::get_short_filename (textureInEditor->get_filename(),true );
                newSaveDataFile << "ImageLocation="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                {
                    std::string resFileCopySrc = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/textures/"+resFileLocation;
                    std::string resFileCopyDest = soughtDir+resFileLocation;
                    if( sff_ex::file_exists(resFileCopyDest) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]Texture Image Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] texture file? This action is irreversible!")==DISPLAY_QUERY_YES)
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
            if( preloadCheckBox!=NULL)
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
            GPE_main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}


void textureResource::update_box(int newX, int newY, int newW, int newH)
{
    if( newX!=-1)
    {
        encapBox.x = newX;
    }
    if(newY!=-1)
    {
        encapBox.y = newY;
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
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Texture=" << resource_name << "," << get_global_rid() << ",";
            *fileTarget << "\n";
            return true;
        }
    }
    return false;
}

