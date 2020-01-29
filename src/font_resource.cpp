/*
fontResource.cpp
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

#include "font_resource.h"
#include "gpe_editor_settings.h"

//

std::string SUPPORTED_FONT_EXT[FONT_FILE_TYPES];

fontResource::fontResource(GPE_GeneralResourceContainer * pFolder)
{
    projectParentFolder = pFolder;
    fontInEditor = NULL;
    resourceFileName = " ";
    for( int i = 0; i < FONT_FILE_TYPES; i++)
    {
        storedFontFileNames[i] = "";
    }
    resourceName = " ";
    fontId = -1;
    fontType = -1;
    fontWidth = 32;
    fontHeight = 32;
    fontSize = 12;
    fontSizeField = new GPE_TextInputNumber("Range: 8 to 512",true,2,256);
    fontPreviewTextField = new GPE_TextInputBasic("ABCD012456789","Previewed Text");
    fontPreviewTextField->set_label("Preview Text");
    fontSizeField->set_string("12");
    fontSizeField->set_label("Font Size");
    if( saveResourceButton!=NULL)
    {
        fontSizeField->set_coords(-1,saveResourceButton->get_ypos()+saveResourceButton->get_height()+GENERAL_GPE_GUI_PADDING+48);
    }
    fontTypeButtonController = new GPE_RadioButtonControllerBasic("Font Type", true,1);
    fontTypeButtonController->add_opton("Normal Font");
    fontTypeButtonController->add_opton("MonoSpace Font");
    loadResourceButton->set_name("Load Custom Font");
    saveResourceButton->set_width( loadResourceButton->get_width() );
    fontFamilyName = "";

    openExternalEditorButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/rocket.png","Use External Editor" );
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
        gfs->close_font(fontInEditor);
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
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );

        *fileTarget << nestedTabsStr << "var " << resourceName << " =  GPE.rsm.add_font(";
        *fileTarget << int_to_string (exportBuildGlobalId ) +",'";
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
            //*fileTarget  << "'resources/animations/"+get_short_filename (animInEditor->fileName,true )+"',";
            if( (int)storedFontFileNames[i].size() > 4)
            {
                *fileTarget << "'resources/fonts/"+get_short_filename( storedFontFileNames[i],true) <<  "',";
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
        *fileTarget << int_to_string (fontSize ) +",";

        *fileTarget << int_to_string (fontWidth ) +",";
        *fileTarget << int_to_string (fontHeight ) +",";
        if( fontTypeButtonController!=NULL)
        {
            *fileTarget << int_to_string (fontTypeButtonController->get_selected_id() );
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
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );

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
                *fileTarget << nestedTabsStr << "src: url('../resources/fonts/" << get_short_filename(storedFontFileNames[FONT_EOT],true)   << "'),\n";
                *fileTarget << nestedTabsStr << "src: url('../resources/fonts/" << get_short_filename(storedFontFileNames[FONT_EOT],true ) << "?#iefix') format('embedded-opentype'),\n,";
            }
            *fileTarget << nestedTabsStr << "src: local('☺')";
            if( (int)storedFontFileNames[FONT_WOFF].size() > 0)
            {
                *fileTarget << nestedTabsStr << ",\n" << "url('../resources/fonts/" << get_short_filename(storedFontFileNames[FONT_WOFF],true)   << "') format('woff')";
            }
            if( (int)storedFontFileNames[FONT_TTF].size() > 0)
            {
                *fileTarget << nestedTabsStr << ",\n" << "url('../resources/fonts/" << get_short_filename(storedFontFileNames[FONT_TTF],true)   << "') format('truetype')";
            }
            if( (int)storedFontFileNames[FONT_SVG].size() > 0)
            {
                *fileTarget << nestedTabsStr << "\n" << "url('../resources/fonts/" << get_short_filename(storedFontFileNames[FONT_SVG],true)   << "#" << fontFamilyName << "') format('svg')";
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
    appendToFile(get_user_settings_folder()+"resources_check.txt",get_name() +"...");

    bool fontNotCopied = false;
    std::string copyFileDestination;
    for( int jFontType = 0; jFontType < FONT_FILE_TYPES; jFontType++)
    {
        if( (int)storedFontFileNames[jFontType].size() > 3 )
        {
            copyFileDestination = pBuildDir+"/resources/fonts/"+get_short_filename(storedFontFileNames[jFontType],true);
            if( copy_file( storedFontFileNames[jFontType],copyFileDestination)==false )
            {
                appendToFile(get_user_settings_folder()+"resources_check.txt","Unable to copy ["+storedFontFileNames[jFontType]+"] to ["+copyFileDestination+"]...");
                return fontNotCopied;
            }
        }
    }

    return !fontNotCopied;
}

void fontResource::load_font(std::string newFileName, int newFontSize )
{
    if( (int)newFileName.size() > 0)
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
        fontSizeField->set_string( int_to_string(newFontSize) );
        fontSize = newFontSize;

        //Saves the font where possible...
        if( get_file_ext(newFileName)=="eot" || get_file_ext(newFileName)=="EOT" )
        {
            std::string copyDestinationStr = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ get_short_filename(newFileName,true);
            storedFontFileNames[FONT_EOT] = copyDestinationStr;
            copy_file(newFileName.c_str(),copyDestinationStr );
        }
        else if( get_file_ext(newFileName)=="svg" || get_file_ext(newFileName)=="SVG" )
        {
            std::string copyDestinationStr = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ get_short_filename(newFileName,true);
            storedFontFileNames[FONT_SVG] = copyDestinationStr;
            copy_file(newFileName.c_str(),copyDestinationStr );
        }
        else if( get_file_ext(newFileName)=="otf" || get_file_ext(newFileName)=="OTF" )
        {
            if( fontInEditor!=NULL)
            {
                gfs->close_font(fontInEditor);
                fontInEditor = NULL;
            }
            fontInEditorFileName = get_short_filename(newFileName,true);
            std::string copyDestinationStr = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ fontInEditorFileName;
            copy_file(newFileName.c_str(),copyDestinationStr );

            fontInEditor = gfs->open_font(copyDestinationStr.c_str(),newFontSize,false,"Custom Font");
            fontFamilyName =fontInEditor->get_family_name();
            if( fontInEditor==NULL)
            {
                display_user_alert("Font Resource Editor","Oh dear! Houston has a problem reading this .ttf font"+newFileName+"/"+int_to_string(newFontSize)+"...");
            }
            else
            {
                fontInEditorFileName = get_short_filename(newFileName,true);
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;
                std::string copyDestinationStr = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ get_short_filename(newFileName,true);
                storedFontFileNames[FONT_OTF] = copyDestinationStr;
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;

            }
        }
        else if( get_file_ext(newFileName)=="ttf" || get_file_ext(newFileName)=="TTF" )
        {
            if( fontInEditor!=NULL)
            {
                gfs->close_font(fontInEditor);
                fontInEditor = NULL;
            }
            fontInEditorFileName = get_short_filename(newFileName,true);
            std::string copyDestinationStr = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ fontInEditorFileName;
            copy_file(newFileName.c_str(),copyDestinationStr );

            fontInEditor = gfs->open_font(copyDestinationStr.c_str(),newFontSize,false,"Custom Font");
            fontFamilyName =fontInEditor->get_family_name();
            if( fontInEditor==NULL)
            {
                display_user_alert("Font Resource Editor","Oh dear! Houston has a problem reading this .ttf font"+newFileName+"/"+int_to_string(newFontSize)+"...");
            }
            else
            {
                fontInEditorFileName = get_short_filename(newFileName,true);
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;
                std::string copyDestinationStr = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ get_short_filename(newFileName,true);
                storedFontFileNames[FONT_TTF] = copyDestinationStr;
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;

            }
        }
        else if( get_file_ext(newFileName)=="woff" || get_file_ext(newFileName)=="WOFF" )
        {
            std::string copyDestinationStr = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ get_short_filename(newFileName,true);
            storedFontFileNames[FONT_WOFF] = copyDestinationStr;
            copy_file(newFileName.c_str(),copyDestinationStr );
        }
        else if( get_file_ext(newFileName)=="woff2" || get_file_ext(newFileName)=="WOFF2" )
        {
            std::string copyDestinationStr = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+ get_short_filename(newFileName,true);
            storedFontFileNames[FONT_WOFF2] = copyDestinationStr;
            copy_file(newFileName.c_str(),copyDestinationStr );
        }
        else
        {
            display_user_alert("Font Resource Editor","Invalid Font File Type given. Please use .ttf files");
        }
    }
}

void fontResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed ==false || file_exists(alternatePath) )
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Font", resourceName );
        }

        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/";
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

        GPE_Report("Loading font - "+newFileIn);
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
                    currLineToBeProcessed  = trim_left_inplace(currLine);
                    currLineToBeProcessed  = trim_right_inplace(currLineToBeProcessed);

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
                                        foundFileVersion = string_to_float(valString);
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
                                    fontTypeButtonController->set_selection(string_to_int(valString,0));
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
                        GPE_Report("Invalid FoundFileVersion ="+float_to_string(foundFileVersion)+".");
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

void fontResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && PANEL_GENERAL_EDITOR!=NULL && fontSizeField!=NULL)
    {
        PANEL_GENERAL_EDITOR->clear_panel();
        PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);
        PANEL_GENERAL_EDITOR->add_gui_element(loadResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element(exportResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element(openExternalEditorButton,true);

        PANEL_GENERAL_EDITOR->add_gui_element(fontSizeField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(fontPreviewTextField,true);
        PANEL_GENERAL_EDITOR->add_gui_element(fontTypeButtonController,true);
        PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);
        PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
        int prevFontSize = fontSizeField->get_held_number();


        PANEL_GENERAL_EDITOR->process_self(NULL, NULL);
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
                GPE_open_context_menu(-1,-1,400);
                for( ii = 0; ii < FONT_FILE_TYPES; ii++)
                {
                    if( (int)storedFontFileNames[ii].size() > 0)
                    {
                        MAIN_CONTEXT_MENU->add_menu_option("Edit "+storedFontFileNames[ii],ii,NULL,-1,NULL,true,true);
                    }
                }
                MAIN_CONTEXT_MENU->set_width( MAIN_CONTEXT_MENU->subOptionWidthSpace );
                int menuSelection = GPE_Get_Context_Result();
                if( menuSelection >=0 && menuSelection < FONT_FILE_TYPES)
                {
                    std::string fileToEdit = storedFontFileNames[menuSelection];

                    if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]!=NULL)
                    {
                        GPE_OpenProgram(MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]->get_string(),fileToEdit, true );
                    }
                    else
                    {
                        GPE_OpenURL(fileToEdit);
                    }
                    /*
                    fileToEdit = "\"C:/Program Files (x86)/Audacity/audacity.exe\" \""+fileToEdit+"\"";
                    GPE_OpenURL(fileToEdit);*/
                    appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+fileToEdit+"]...");
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
            std::string newFontFile = GPE_GetOpenFileName("Load In Custom Font...","",MAIN_GUI_SETTINGS->fileOpenFontDir );
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
            if( GPE_Display_Basic_Prompt("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== DISPLAY_QUERY_YES )
            {
                resourcePostProcessed = false;
                preprocess_self();
            }
        }
    }
}

bool fontResource::render_held_font( int xPos, int yPos, std::string textureText, GPE_Color * textColor,int hAlign,int vAlign, float renderAngle, float renderScale, int renderAlpha  )
{
    if( fontInEditor!=NULL )
    {
        return fontInEditor->render_text_special( xPos, yPos, textureText, textColor, hAlign, vAlign, renderAngle, renderScale, renderAlpha );
    }
    return false;
}

void fontResource::render_self(GPE_Rect * viewedSpace,GPE_Rect *cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && viewedSpace!=NULL  )
    {
        if(fontTypeButtonController!=NULL)
        {
            gcanvas->render_vertical_line_color( 0,0,viewedSpace->h,GPE_MAIN_THEME->Program_Header_Color );
        }
        if( fontInEditor!=NULL)
        {
            gfs->render_text( GENERAL_GPE_GUI_PADDING,GENERAL_GPE_GUI_PADDING,"Font Preview: "+get_short_filename(storedFontFileNames[FONT_TTF],true)+" | "+fontFamilyName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_LABEL,FA_LEFT,FA_TOP);
            gfs->render_text( GENERAL_GPE_GUI_PADDING,GENERAL_GPE_GUI_PADDING+32,fontPreviewTextField->get_string(),GPE_MAIN_THEME->Main_Box_Font_Color,fontInEditor,FA_LEFT,FA_TOP);
        }
        else
        {
            gfs->render_text( GENERAL_GPE_GUI_PADDING,GENERAL_GPE_GUI_PADDING,"Please Select A Font",GPE_MAIN_THEME->Main_Box_Font_Color,FONT_LABEL,FA_LEFT,FA_TOP);
        }
        for( int i = FONT_FILE_TYPES-1; i >=0; i--)
        {
            if( storedFontFileNames[i].size()> 3)
            {
                gfs->render_text( viewedSpace->w-GENERAL_GPE_GUI_PADDING*2,viewedSpace->h-GENERAL_GPE_GUI_PADDING-GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_FONT_EXT[i]+" is used",GPE_MAIN_THEME->Main_Suggestion_Font_Color,GPE_DEFAULT_FONT,FA_RIGHT,FA_BOTTOM);
            }
            else
            {
                gfs->render_text( viewedSpace->w-GENERAL_GPE_GUI_PADDING*2,viewedSpace->h-GENERAL_GPE_GUI_PADDING-GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_FONT_EXT[i]+" not used",GPE_MAIN_THEME->Main_Error_Font_Color,GPE_DEFAULT_FONT,FA_RIGHT,FA_BOTTOM);
            }
        }
    }
}

void fontResource::save_resource(std::string alternatePath, int backupId)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Font", resourceName );
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
        soughtDir = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/";
        newFileOut = soughtDir + resourceName+".gpf";
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
                    resFileCopySrc = file_to_dir(parentProjectName)+"/gpe_project/resources/fonts/"+resFileLocation;
                    resFileCopyDest = soughtDir+resFileLocation;
                    if( file_exists(resFileCopyDest) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]Font File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] font file? This action is irreversible!")==DISPLAY_QUERY_YES)
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
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Font Successfully Saved!", resourceName );
        }
        return;
    }

    GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Font Unable to  Save!", resourceName );
    }
}

bool fontResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Font=" << resourceName << "," << get_global_rid() << ",";

            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}

