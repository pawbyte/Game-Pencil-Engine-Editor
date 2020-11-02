/*
fontResource.cpp
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

#include "font_resource.h"
#include "gpe_editor_settings.h"

//

std::string SUPPORTED_FONT_EXT[FONT_FILE_TYPES];

fontResource::fontResource(pawgui::widget_resource_container * pFolder)
{
    projectParentFolder = pFolder;
    fontInEditor = NULL;
    resourceFileName = " ";
    for( int i = 0; i < FONT_FILE_TYPES; i++)
    {
        storedFontFileNames[i] = "";
    }
    resource_name = " ";
    fontId = -1;
    fontType = -1;
    fontWidth = 32;
    fontHeight = 32;
    fontSize = 12;
    fontSizeField = new pawgui::widget_input_number("Range: 8 to 512",true,2,256);
    fontPreviewTextField = new pawgui::widget_input_text("ABCD012456789","Previewed Text");
    fontPreviewTextField->set_label("Preview Text");
    fontSizeField->set_string("12");
    fontSizeField->set_label("Font Size");
    if( saveResourceButton!=NULL)
    {
        fontSizeField->set_coords(-1,saveResourceButton->get_ypos()+saveResourceButton->get_height()+pawgui::padding_default+48);
    }
    fontTypeButtonController = new pawgui::widget_radio_button_controller("Font Type", true,1);
    fontTypeButtonController->add_opton("Normal Font");
    fontTypeButtonController->add_opton("MonoSpace Font");
    loadResourceButton->set_name("Load Custom Font");
    saveResourceButton->set_width( loadResourceButton->get_width() );
    fontFamilyName = "";

    openExternalEditorButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Use External Editor" );
}

fontResource::~fontResource()
{
    if( fontTypeButtonController!=NULL)
    {
        delete fontTypeButtonController;
        fontTypeButtonController = NULL;
    }
    if( fontPreviewTextField!=NULL)
    {
        delete fontPreviewTextField;
        fontPreviewTextField = NULL;
    }
    if( fontSizeField!=NULL)
    {
        delete fontSizeField;
        fontSizeField = NULL;
    }
    if( fontInEditor!=NULL)
    {
        gpe::gfs->close_font(fontInEditor);
        fontInEditor= NULL;
    }
    if( openExternalEditorButton!=NULL)
    {
        delete openExternalEditorButton;
        openExternalEditorButton= NULL;
    }
}

bool fontResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( leftTabAmount  );

        *fileTarget << nestedTabsStr << "var " << resource_name << " =  GPE.rsm.add_font(";
        *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) +",'";
        if( (int)fontFamilyName.size() > 4)
        {
            *fileTarget << fontFamilyName+"',";
        }
        else
        {
            *fileTarget << "Arial',";
        }
        for( int i = 0; i < FONT_FILE_TYPES; i++)
        {
            //*fileTarget  << "'resources/animations/"+stg_ex::get_short_filename (animInEditor->fileName,true )+"',";
            if( (int)storedFontFileNames[i].size() > 4)
            {
                *fileTarget << "'resources/fonts/"+ stg_ex::get_short_filename( storedFontFileNames[i],true) <<  "',";
            }
            else
            {
                *fileTarget << "'',";
            }
        }
        if( fontInEditor==NULL )
        {
            fontSize = fontSizeField->get_held_number();
        }
        if( fontSize <=8)
        {
            fontSize = 8;
        }
        if( fontSize >=256)
        {
            fontSize = 256;
        }
        *fileTarget << stg_ex::int_to_string (fontSize ) +",";

        *fileTarget << stg_ex::int_to_string (fontWidth ) +",";
        *fileTarget << stg_ex::int_to_string (fontHeight ) +",";
        if( fontTypeButtonController!=NULL)
        {
            *fileTarget << stg_ex::int_to_string (fontTypeButtonController->get_selected_id() );
        }
        else
        {
            *fileTarget << "0";
        }

        *fileTarget << ");\n";
        return true;
    }
    return false;

}

bool fontResource::build_css3_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( leftTabAmount  );

        bool foundHeldFont = false;
        for( int i = 0; i < FONT_FILE_TYPES; i++)
        {
            if( (int)storedFontFileNames[i].size() > 0)
            {
                foundHeldFont = true;
                break;
            }
        }
        if( foundHeldFont)
        {
            *fileTarget << nestedTabsStr << "@font-face{\n";
            *fileTarget << nestedTabsStr << "font-family: '"+fontFamilyName+"';\n";
            if( (int)storedFontFileNames[FONT_EOT].size() > 0)
            {
                *fileTarget << nestedTabsStr << "src: url('../resources/fonts/" << stg_ex::get_short_filename(storedFontFileNames[FONT_EOT],true)   << "'),\n";
                *fileTarget << nestedTabsStr << "src: url('../resources/fonts/" << stg_ex::get_short_filename(storedFontFileNames[FONT_EOT],true ) << "?#iefix') format('embedded-opentype'),\n,";
            }
            *fileTarget << nestedTabsStr << "src: local('☺')";
            if( (int)storedFontFileNames[FONT_WOFF].size() > 0)
            {
                *fileTarget << nestedTabsStr << ",\n" << "url('../resources/fonts/" << stg_ex::get_short_filename(storedFontFileNames[FONT_WOFF],true)   << "') format('woff')";
            }
            if( (int)storedFontFileNames[FONT_TTF].size() > 0)
            {
                *fileTarget << nestedTabsStr << ",\n" << "url('../resources/fonts/" << stg_ex::get_short_filename(storedFontFileNames[FONT_TTF],true)   << "') format('truetype')";
            }
            if( (int)storedFontFileNames[FONT_SVG].size() > 0)
            {
                *fileTarget << nestedTabsStr << "\n" << "url('../resources/fonts/" << stg_ex::get_short_filename(storedFontFileNames[FONT_SVG],true)   << "#" << fontFamilyName << "') format('svg')";
            }
            *fileTarget << nestedTabsStr << ";\n" << "font-weight: normal;\n";
            *fileTarget << nestedTabsStr << "font-style: normal;\n";
            *fileTarget << nestedTabsStr << "}\n";
        }
    }
    return false;
}

bool fontResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void fontResource::compile_cpp()
{

}

bool fontResource::include_local_files( std::string pBuildDir , int buildType )
{
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt " , get_name() +"...");

    bool fontNotCopied = false;
    std::string copyFileDestination;
    for( int jFontType = 0; jFontType < FONT_FILE_TYPES; jFontType++)
    {
        if( (int)storedFontFileNames[jFontType].size() > 3 )
        {
            copyFileDestination = pBuildDir+"/resources/fonts/"+ stg_ex::get_short_filename(storedFontFileNames[jFontType],true);
            if( sff_ex::file_copy( storedFontFileNames[jFontType],copyFileDestination)==false )
            {
                sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt","Unable to copy ["+storedFontFileNames[jFontType]+"] to ["+copyFileDestination+"]...");
                return fontNotCopied;
            }
        }
    }

    return !fontNotCopied;
}

bool fontResource::is_build_ready()
{
    recentErrorMessage ="";
    return true;
}

void fontResource::load_font(std::string new_file_name, int newFontSize )
{
    if( (int)new_file_name.size() > 0)
    {
        if( newFontSize < 0 && fontSizeField->get_held_number() > 0 )
        {
            newFontSize = fontSizeField->get_held_number();
        }
        else if( newFontSize < 0 )
        {
            newFontSize = 8;
        }
        else if(newFontSize > 512)
        {
            newFontSize = 512;
        }
        fontSizeField->set_string( stg_ex::int_to_string(newFontSize) );
        fontSize = newFontSize;

        //Saves the font where possible...
        if( stg_ex::get_file_ext(new_file_name)=="eot" || stg_ex::get_file_ext(new_file_name)=="EOT" )
        {
            std::string copyDestinationStr = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ stg_ex::get_short_filename(new_file_name,true);
            storedFontFileNames[FONT_EOT] = copyDestinationStr;
            sff_ex::file_copy(new_file_name.c_str(),copyDestinationStr );
        }
        else if( stg_ex::get_file_ext(new_file_name)=="svg" || stg_ex::get_file_ext(new_file_name)=="SVG" )
        {
            std::string copyDestinationStr = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ stg_ex::get_short_filename(new_file_name,true);
            storedFontFileNames[FONT_SVG] = copyDestinationStr;
            sff_ex::file_copy(new_file_name.c_str(),copyDestinationStr );
        }
        else if( stg_ex::get_file_ext(new_file_name)=="otf" || stg_ex::get_file_ext(new_file_name)=="OTF" )
        {
            if( fontInEditor!=NULL)
            {
                gpe::gfs->close_font(fontInEditor);
                fontInEditor = NULL;
            }
            fontInEditorFileName = stg_ex::get_short_filename(new_file_name,true);
            std::string copyDestinationStr = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ fontInEditorFileName;
            sff_ex::file_copy(new_file_name.c_str(),copyDestinationStr );

            fontInEditor = gpe::gfs->open_font( copyDestinationStr.c_str(),newFontSize,false,"Custom Font");
            fontFamilyName =fontInEditor->get_family_name();
            if( fontInEditor==NULL)
            {
                pawgui::display_user_alert("Font Resource Editor","Oh dear! Houston has a problem reading this .ttf font"+new_file_name+"/"+ stg_ex::int_to_string(newFontSize)+"...");
            }
            else
            {
                fontInEditorFileName = stg_ex::get_short_filename(new_file_name,true);
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;
                std::string copyDestinationStr = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ stg_ex::get_short_filename(new_file_name,true);
                storedFontFileNames[FONT_OTF] = copyDestinationStr;
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;
            }
        }
        else if( stg_ex::get_file_ext(new_file_name)=="ttf" || stg_ex::get_file_ext(new_file_name)=="TTF" )
        {
            if( fontInEditor!=NULL)
            {
                gpe::gfs->close_font(fontInEditor);
                fontInEditor = NULL;
            }
            fontInEditorFileName = stg_ex::get_short_filename(new_file_name,true);
            std::string copyDestinationStr = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ fontInEditorFileName;
            sff_ex::file_copy(new_file_name.c_str(),copyDestinationStr );

            fontInEditor = gpe::gfs->open_font(copyDestinationStr.c_str(),newFontSize,false,"Custom Font");
            fontFamilyName =fontInEditor->get_family_name();
            if( fontInEditor==NULL)
            {
                pawgui::display_user_alert("Font Resource Editor","Oh dear! Houston has a problem reading this .ttf font"+new_file_name+"/"+ stg_ex::int_to_string(newFontSize)+"...");
            }
            else
            {
                fontInEditorFileName = stg_ex::get_short_filename(new_file_name,true);
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;
                std::string copyDestinationStr = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ stg_ex::get_short_filename(new_file_name,true);
                storedFontFileNames[FONT_TTF] = copyDestinationStr;
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;

            }
        }
        else if( stg_ex::get_file_ext(new_file_name)=="woff" || stg_ex::get_file_ext(new_file_name)=="WOFF" )
        {
            std::string copyDestinationStr = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ stg_ex::get_short_filename(new_file_name,true);
            storedFontFileNames[FONT_WOFF] = copyDestinationStr;
            sff_ex::file_copy(new_file_name.c_str(),copyDestinationStr );
        }
        else if( stg_ex::get_file_ext(new_file_name)=="woff2" || stg_ex::get_file_ext(new_file_name)=="WOFF2" )
        {
            std::string copyDestinationStr = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ stg_ex::get_short_filename(new_file_name,true);
            storedFontFileNames[FONT_WOFF2] = copyDestinationStr;
            sff_ex::file_copy(new_file_name.c_str(),copyDestinationStr );
        }
        else
        {
            pawgui::display_user_alert("Font Resource Editor","Invalid Font File Type given. Please use .ttf files");
        }
    }
}

void fontResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed ==false || sff_ex::file_exists(file_path) )
    {
        if( pawgui::main_loader_display != NULL )
        {
            pawgui::main_loader_display->update_submessages( "Processing Font", resource_name );
        }

        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/";
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

        gpe::error_log->report("Loading font - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
        {
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string section="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                float foundFileVersion = 0;
                std::string fFontFile = "";
                int tFontSize = 12;
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed  = stg_ex::trim_left_inplace(currLine);
                    currLineToBeProcessed  = stg_ex::trim_right_inplace(currLineToBeProcessed);

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
                                else if( keyString=="FontSize")
                                {
                                    fontSizeField->set_string(valString);
                                    tFontSize = fontSizeField->get_held_number();
                                }
                                else if( keyString=="FontType")
                                {
                                    fontTypeButtonController->set_selection( stg_ex::string_to_int(valString,0));
                                }
                                if( keyString=="PreviewText")
                                {
                                    fontPreviewTextField->set_string(valString);
                                }
                                else
                                {
                                    for( int i = 0; i < FONT_FILE_TYPES; i++)
                                    {
                                        if( keyString==SUPPORTED_FONT_EXT[i]+"_File")
                                        {
                                            load_font( soughtDir+valString,tFontSize );
                                        }
                                    }
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

void fontResource::prerender_self( )
{
    standardEditableGameResource::prerender_self( );
    if( fontTypeButtonController!=NULL)
    {
        fontTypeButtonController->prerender_self( );
    }
}

void fontResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if(cam!=NULL && view_space!=NULL && panel_main_area!=NULL && fontSizeField!=NULL)
    {
        panel_main_area->clear_panel();
        panel_main_area->add_gui_element(renameBox,true);
        panel_main_area->add_gui_element(loadResourceButton,false);
        panel_main_area->add_gui_element(exportResourceButton,false);
        panel_main_area->add_gui_element(openExternalEditorButton,true);

        panel_main_area->add_gui_element(fontSizeField,true);
        panel_main_area->add_gui_element(fontPreviewTextField,true);
        panel_main_area->add_gui_element(fontTypeButtonController,true);
        panel_main_area->add_gui_element(confirmResourceButton,true);
        panel_main_area->add_gui_element(cancelResourceButton,true);
        int prevFontSize = fontSizeField->get_held_number();


        panel_main_area->process_self(NULL, NULL);
        if( openExternalEditorButton!=NULL &&  openExternalEditorButton->is_clicked() )
        {
            bool hasFileToOpen = false;
            int ii = 0;
            for( ii = 0; ii < FONT_FILE_TYPES; ii++)
            {
                if( (int)storedFontFileNames[ii].size() > 0)
                {
                    hasFileToOpen = true;
                    break;
                }
            }
            if( hasFileToOpen )
            {
                pawgui::context_menu_open(-1,-1,400);
                for( ii = 0; ii < FONT_FILE_TYPES; ii++)
                {
                    if( (int)storedFontFileNames[ii].size() > 0)
                    {
                        pawgui::main_context_menu->add_menu_option("Edit "+storedFontFileNames[ii],ii,NULL,-1,NULL,true,true);
                    }
                }
                pawgui::main_context_menu->set_width( pawgui::main_context_menu->subOptionWidthSpace );
                int menuSelection = pawgui::context_menu_process();
                if( menuSelection >=0 && menuSelection < FONT_FILE_TYPES)
                {
                    std::string fileToEdit = storedFontFileNames[menuSelection];

                    if( main_editor_settings!=NULL && main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]!=NULL)
                    {
                        gpe::external_open_program(main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]->get_string(),fileToEdit, true );
                    }
                    else
                    {
                        gpe::external_open_url(fileToEdit);
                    }
                    /*
                    fileToEdit = "\"C:/Program Files (x86)/Audacity/audacity.exe\" \""+fileToEdit+"\"";
                    external_open_url(fileToEdit);*/
                    sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+fileToEdit+"]...");
                }
            }
        }

        if( fontSizeField->is_valid() )
        {
            if( fontSizeField->get_held_number()!=prevFontSize && fontSizeField->get_held_number() >= 8 )
            {
                load_font( storedFontFileNames[FONT_TTF], fontSizeField->get_held_number() );
            }
        }
        if( loadResourceButton->is_clicked() )
        {
            std::string newFontFile = pawgui::get_filename_open_from_popup("Load In Custom Font...","",pawgui::main_settings->fileOpenFontDir );
            if( (int)newFontFile.size() > 3)
            {
                load_font( newFontFile, fontSizeField->get_held_number() );
            }
        }
        else if( confirmResourceButton->is_clicked() )
        {
            save_resource();
        }
        else if( cancelResourceButton->is_clicked() )
        {
            if( pawgui::display_prompt_message("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
            {
                resourcePostProcessed = false;
                load_resource();
            }
        }
    }
}

bool fontResource::render_held_font( int xPos, int yPos, std::string textureText, gpe::color * textColor,int hAlign,int vAlign, float renderAngle, float renderScale, int renderAlpha  )
{
    if( fontInEditor!=NULL )
    {
        return fontInEditor->render_text_special( xPos, yPos, textureText, textColor, hAlign, vAlign, renderAngle, renderScale, renderAlpha );
    }
    return false;
}

void fontResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( cam!=NULL && view_space!=NULL  )
    {
        if(fontTypeButtonController!=NULL)
        {
            gpe::gcanvas->render_vertical_line_color( 0,0,view_space->h,pawgui::theme_main->program_color_header );
        }
        if( fontInEditor!=NULL)
        {
            gpe::gfs->render_text( pawgui::padding_default,pawgui::padding_default,"Font Preview: "+stg_ex::get_short_filename(storedFontFileNames[FONT_TTF],true)+" | "+fontFamilyName,pawgui::theme_main->main_box_font_color,pawgui::FONT_LABEL,gpe::fa_left, gpe::fa_top);
            gpe::gfs->render_text( pawgui::padding_default,pawgui::padding_default+32,fontPreviewTextField->get_string(),pawgui::theme_main->main_box_font_color,fontInEditor,gpe::fa_left,gpe::fa_top);
        }
        else
        {
            gpe::gfs->render_text( pawgui::padding_default,pawgui::padding_default,"Please Select A Font",pawgui::theme_main->main_box_font_color,pawgui::FONT_LABEL,gpe::fa_left,gpe::fa_top);
        }
        for( int i = FONT_FILE_TYPES-1; i >=0; i--)
        {
            if( storedFontFileNames[i].size()> 3)
            {
                gpe::gfs->render_text( view_space->w-pawgui::padding_default*2,view_space->h-pawgui::padding_default-pawgui::default_line_height*i,SUPPORTED_FONT_EXT[i]+" is used",pawgui::theme_main->main_suggestion_font_color, gpe::font_default, gpe::fa_right, gpe::fa_bottom);
            }
            else
            {
                gpe::gfs->render_text( view_space->w-pawgui::padding_default*2,view_space->h-pawgui::padding_default-pawgui::default_line_height*i,SUPPORTED_FONT_EXT[i]+" not used",pawgui::theme_main->main_error_font_color, gpe::font_default,gpe::fa_right,gpe::fa_bottom);
            }
        }
    }
}

void fontResource::save_resource(std::string file_path, int backupId)
{
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Saving Font", resource_name );
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
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/";
        newFileOut = soughtDir + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the font file could be saved
    if( !newSaveDataFile.fail() && newSaveDataFile.is_open() )
    {
        write_header_on_file(&newSaveDataFile);
        if( fontSizeField!=NULL)
        {
            newSaveDataFile << "FontSize=" << std::max(8,(int)fontSizeField->get_held_number() )<< "\n";
        }
        else
        {
            newSaveDataFile << "FontSize=12\n";
        }

        std::string resFileLocation = "";
        std::string resFileCopySrc;
        std::string resFileCopyDest;
        for( int i = 0; i < FONT_FILE_TYPES; i++)
        {
            if( (int)storedFontFileNames[i].size() > 3)
            {
                resFileLocation = storedFontFileNames[i];
                newSaveDataFile << SUPPORTED_FONT_EXT[i]+"_File=" << resFileLocation <<"\n";
                if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                {
                    resFileCopySrc = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+resFileLocation;
                    resFileCopyDest = soughtDir+resFileLocation;
                    if( sff_ex::file_exists(resFileCopyDest) )
                    {
                        /*
                        if( pawgui::display_prompt_message("[WARNING]Font File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] font file? This action is irreversible!")==pawgui::display_query_yes)
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
        }

        if( fontTypeButtonController!=NULL)
        {
            newSaveDataFile << "FontType=" <<fontTypeButtonController->get_selected_id() << "\n";
        }
        else
        {
            newSaveDataFile << "FontType=0\n";
        }
        if(fontPreviewTextField!=NULL)
        {
            newSaveDataFile << "PreviewText=" <<fontPreviewTextField->get_string() << "\n";
        }
        newSaveDataFile.close();
        if( !usingAltSaveSource)
        {
            isModified = false;
        }
        if( pawgui::main_loader_display != NULL )
        {
            pawgui::main_loader_display->update_submessages( "Font Successfully Saved!", resource_name );
        }
        return;
    }

    main_editor_log->log_general_error("Unable to save file ["+newFileOut+"]");
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Font Unable to  Save!", resource_name );
    }
}

bool fontResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Font=" << resource_name << "," << get_global_rid() << ",";

            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}

