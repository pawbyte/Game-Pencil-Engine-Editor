/*
texture_resource.cpp
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

#include "texture_resource.h"
#include "gpe_editor_settings.h"

textureResource::textureResource(GPE_GeneralResourceContainer * pFolder)
{
    projectParentFolder = pFolder;
    editorMode = 0;
    textureInEditor = NULL;
    isPreloaded = true;
    preloadCheckBox = new GPE_CheckBoxBasic("Preload Texture","Check to load texture at game open", true);
    transformResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/magic.png","Transform the Image",-1);
    labelImageDimensions = new GPE_Label_Text("","");
    //labelTextureMessage = new GPE_Label_Text("","");
    openExternalEditorButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Texture Image In External Editor");
    refreshResourceDataButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded texture image");
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
    if( textureInEditor!=NULL)
    {
        delete textureInEditor;
        textureInEditor = NULL;
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
            *fileTarget << nestedTabsStr << "var " << html5TSName << " =  GPE.rsm.add_texture(";
            *fileTarget << int_to_string (exportBuildGlobalId ) +",";
            *fileTarget << "'resources/textures/"+getShortFileName (textureInEditor->get_filename(),true )+"'";
            *fileTarget << "); \n";
        }
        else
        {
            *fileTarget << nestedTabsStr << "var " << html5TSName << " = GPE.rsm.add_texture( -1 );\n";
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

int textureResource::load_image(std::string newFileName)
{
    if( file_exists(newFileName) )
    {
        if( file_is_image(newFileName) )
        {
            if(textureInEditor!=NULL)
            {
                delete textureInEditor;
            }
            textureInEditor = new GPE_Texture();
            textureInEditor->load_new_texture( newFileName,-1,false);
            if( textureInEditor->get_width() <1 || textureInEditor->get_width()>4096 || textureInEditor->get_height() <1 || textureInEditor->get_height()>4096)
            {
                display_user_alert("Unable to load image","Editor Error: Unable to load ["+newFileName+"] please check file and make sure it is between 1x1 and 4096x4096 pixels and is a valid image");
                if( textureInEditor!=NULL)
                {
                    delete textureInEditor;
                    textureInEditor = NULL;
                }
            }
            else
            {
                textureInEditor->copy_image_source( fileToDir(parentProjectName)+"/gpe_project/resources/textures");
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    return -1;
}

void textureResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Texture",resourceName );
        }

        std::string otherColContainerName = "";

        std::string newFileIn ="";

        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/textures/";
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

        //GPE_Report("Loading Texture - "+newFileIn);
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
                double foundFileVersion = 0;
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
                                else if( keyString=="ImageLocation")
                                {
                                    if( valString!="NULL")
                                    {
                                        load_image( soughtDir+valString );
                                    }
                                }
                                else if( keyString=="Preload")
                                {
                                    isPreloaded = string_to_bool(valString);
                                    preloadCheckBox->set_clicked(isPreloaded );
                                }
                            }
                        }
                    }
                    else
                    {
                        GPE_Report("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
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

void textureResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && PANEL_GENERAL_EDITOR!=NULL)
    {
        /*
        if( textureInEditor!=NULL)
            {
                render_text( GENERAL_GPE_PADDING,GENERAL_GPE_PADDING*2+preloadCheckBox->get_ypos()+preloadCheckBox->get_height(),
                int_to_string(textureInEditor->get_width() )+" x "+int_to_string(textureInEditor->get_height() )+"px",
                GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP);
            }
            else
            {
                render_text( GENERAL_GPE_PADDING,GENERAL_GPE_PADDING*2+preloadCheckBox->get_ypos()+preloadCheckBox->get_height(),
                "Image not loaded",GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP);
            }
        */

        PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);
        PANEL_GENERAL_EDITOR->add_gui_element(refreshResourceDataButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element(loadResourceButton,false);
        //PANEL_GENERAL_EDITOR->add_gui_element(saveResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element(transformResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element( openExternalEditorButton,true);

        if( textureInEditor!=NULL)
        {
            labelImageDimensions->set_name("Image Size: "+int_to_string(textureInEditor->get_width() )+" x "+int_to_string(textureInEditor->get_height() )+"px");
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
        PANEL_GENERAL_EDITOR->process_self(NULL, NULL);
        if( editorMode==0)
        {
            if( loadResourceButton!=NULL && loadResourceButton->is_clicked() )
            {
                std::string newFileName = GPE_GetOpenFileName("Load Your Texture and such...","Images",MAIN_GUI_SETTINGS->fileOpenTextureDir);
                if( (int)newFileName.size() > 3)
                {
                    int loadResult = load_image(newFileName);
                    if( loadResult==false)
                    {
                        display_user_alert("Unable to load image","File type["+get_file_ext(newFileName)+"] not supported when loading ["+newFileName+"].");
                    }
                    else if( loadResult==-1)
                    {
                        display_user_alert("Unable to load image","File does not exist["+newFileName+"].");
                    }
                }
            }
            else if( refreshResourceDataButton!=NULL && refreshResourceDataButton->is_clicked() )
            {
                if(  textureInEditor!=NULL )
                {
                    std::string currentFileToRefresh = getShortFileName (textureInEditor->get_filename(),true );
                    currentFileToRefresh = fileToDir(parentProjectName)+"/gpe_project/resources/textures/"+currentFileToRefresh;
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
                    preprocess_self();
                }
            }
            if(renameBox!=NULL)
            {
                if( renameBox->get_string()!=resourceName)
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
                        MAIN_CONTEXT_MENU->add_menu_option("Erase BG Color",0);
                        MAIN_CONTEXT_MENU->add_menu_option("Invert Colors",1);
                        MAIN_CONTEXT_MENU->add_menu_option("Make GrayScale",2);
                        MAIN_CONTEXT_MENU->add_menu_option("Exit",10);
                        int menuSelection = GPE_Get_Context_Result();
                        if( menuSelection>=0 && menuSelection <=3)
                        {

                            SDL_Surface * oTempSurface = gpe_sdl->load_surface_image(textureInEditor->get_filename() );
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
                                            GPE_Report("Modifying image at: "+textureInEditor->get_filename()+".");
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
                                    remove( textureInEditor->get_filename().c_str() );
                                    std::string newImageName = get_file_noext(textureInEditor->get_filename())+".png";
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
                    if( file_exists(textureInEditor->get_filename() ) )
                    {
                        if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=NULL)
                        {
                            GPE_OpenProgram(MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),textureInEditor->get_filename(), true );
                        }
                        else
                        {
                            GPE_OpenURL(textureInEditor->get_filename());
                        }
                        appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit texture ["+textureInEditor->get_filename()+"]...");
                    }
                }
            }

        }
    }
}

void textureResource::render_self(GPE_Rect * viewedSpace,GPE_Rect *cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && forceRedraw )
    {
        //renders the right side of the area, mainly preview of tilesheet
        //MAIN_RENDERER->set_viewpoint( &texturePreviewCam );
        if( GPE_TEXTURE_TRANSPARENT_BG!=NULL && GPE_MAIN_THEME->themeBgTexture==NULL )
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
                double neededTextureScale= (double)std::min( (double)viewedSpace->w/ (double)textureInEditor->get_width(),  (double)viewedSpace->h / (double)textureInEditor->get_height() );
                textureInEditor->render_tex_scaled( 0, 0,neededTextureScale,neededTextureScale,NULL);
            }
        }
    }
}

void textureResource::save_resource(std::string alternatePath, int backupId)
{
    isModified = false;
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Texture",resourceName );
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
        //GPE_Main_Logs->log_general_error("Path ["+soughtDir+"] does not exist");
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/textures/";
        newFileOut = soughtDir + resourceName+".gpf";
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
                std::string resFileLocation = getShortFileName (textureInEditor->get_filename(),true );
                newSaveDataFile << "ImageLocation="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                {
                    std::string resFileCopySrc = fileToDir(parentProjectName)+"/gpe_project/resources/textures/"+resFileLocation;
                    std::string resFileCopyDest = soughtDir+resFileLocation;
                    if( file_exists(resFileCopyDest) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]Texture Image Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] texture file? This action is irreversible!")==DISPLAY_QUERY_YES)
                        {
                            copy_file(resFileCopySrc,resFileCopyDest);
                        }
                        */
                    }
                    else
                    {
                        copy_file(resFileCopySrc,resFileCopyDest);
                    }
                }
            }
            if( preloadCheckBox!=NULL)
            {
                newSaveDataFile << "Preload="+int_to_string(preloadCheckBox->is_clicked() )+"\n";
            }
            newSaveDataFile.close();
            if( !usingAltSaveSource)
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
            *fileTarget << nestedTabsStr << "Texture=" << resourceName << "," << get_global_rid() << ",";
            *fileTarget << "\n";
            return true;
        }
    }
    return false;
}

