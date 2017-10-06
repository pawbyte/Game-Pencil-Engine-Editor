/*
fontResource.cpp
This file is part of:
GAME PENCI ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "fontResource.h"

//
fontResource::fontResource(GPE_ResourceContainer * pFolder)
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
        fontSizeField->set_coords(-1,saveResourceButton->get_ypos()+saveResourceButton->get_height()+GENERAL_GPE_PADDING+48);
    }
    fontTypeButtonController = new GPE_RadioButtonControllerBasic("Font Type",0,256,true,1);
    fontTypeButtonController->add_opton("Normal Font");
    fontTypeButtonController->add_opton("MonoSpace Font");
    loadResourceButton->set_name("Load Custom Font");
    saveResourceButton->set_width( loadResourceButton->get_width() );
    fontFamilyName = "";

    openExternalEditorButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/rocket.png","Use External Editor","Opens Audio In External Editor");
    openExternalEditorButton->set_width(loadResourceButton->get_width() );
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
        GPE_CloseFont(fontInEditor);
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
        *fileTarget << int_to_string (html5BuildGlobalId ) +",'";
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
            //*fileTarget  << "'resources/sprites/"+getShortFileName (spriteInEditor->fileName,true )+"',";
            if( (int)storedFontFileNames[i].size() > 4)
            {
                *fileTarget << "'resources/fonts/"+getShortFileName( storedFontFileNames[i],true) <<  "',";
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
                *fileTarget << nestedTabsStr << "src: url('../resources/fonts/" << getShortFileName(storedFontFileNames[FONT_EOT],true)   << "'),\n";
                *fileTarget << nestedTabsStr << "src: url('../resources/fonts/" << getShortFileName(storedFontFileNames[FONT_EOT],true ) << "?#iefix') format('embedded-opentype'),\n,";
            }
            *fileTarget << nestedTabsStr << "src: local('☺')";
            if( (int)storedFontFileNames[FONT_WOFF].size() > 0)
            {
                 *fileTarget << nestedTabsStr << ",\n" << "url('../resources/fonts/" << getShortFileName(storedFontFileNames[FONT_WOFF],true)   << "') format('woff')";
            }
            if( (int)storedFontFileNames[FONT_TTF].size() > 0)
            {
                *fileTarget << nestedTabsStr << ",\n" << "url('../resources/fonts/" << getShortFileName(storedFontFileNames[FONT_TTF],true)   << "') format('truetype')";
            }
            if( (int)storedFontFileNames[FONT_SVG].size() > 0)
            {
                *fileTarget << nestedTabsStr << "\n" << "url('../resources/fonts/" << getShortFileName(storedFontFileNames[FONT_SVG],true)   << "#" << fontFamilyName << "') format('svg')";
            }
            *fileTarget << nestedTabsStr << ";\n" << "font-weight: normal;\n";
            *fileTarget << nestedTabsStr << "font-style: normal;\n";
            *fileTarget << nestedTabsStr << "}\n";
        }
    }
    return false;
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
            std::string copyDestinationStr = fileToDir(parentProjectName)+"/gpe_project/resources/fonts/"+ getShortFileName(newFileName,true);
            storedFontFileNames[FONT_EOT] = copyDestinationStr;
            copy_file(newFileName.c_str(),copyDestinationStr );
        }
        else if( get_file_ext(newFileName)=="svg" || get_file_ext(newFileName)=="SVG" )
        {
            std::string copyDestinationStr = fileToDir(parentProjectName)+"/gpe_project/resources/fonts/"+ getShortFileName(newFileName,true);
            storedFontFileNames[FONT_SVG] = copyDestinationStr;
            copy_file(newFileName.c_str(),copyDestinationStr );
        }
        else if( get_file_ext(newFileName)=="ttf" || get_file_ext(newFileName)=="TTF" )
        {
            if( fontInEditor!=NULL)
            {
                GPE_CloseFont(fontInEditor);
                fontInEditor = NULL;
            }
            fontInEditorFileName = getShortFileName(newFileName,true);
            std::string copyDestinationStr = fileToDir(parentProjectName)+"/gpe_project/resources/fonts/"+ fontInEditorFileName;
            copy_file(newFileName.c_str(),copyDestinationStr );

            fontInEditor = GPE_OpenFont(copyDestinationStr.c_str(),newFontSize,false,"Custom Font");
            fontFamilyName =fontInEditor->get_family_name();
            if( fontInEditor==NULL)
            {
                display_user_alert("Font Resource Editor","Oh dear! Houston has a problem reading this .ttf font"+newFileName+"/"+int_to_string(newFontSize)+"...");
            }
            else
            {
                fontInEditorFileName = getShortFileName(newFileName,true);
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;
                std::string copyDestinationStr = fileToDir(parentProjectName)+"/gpe_project/resources/fonts/"+ getShortFileName(newFileName,true);
                storedFontFileNames[FONT_TTF] = copyDestinationStr;
                fontInEditor->get_metrics( "AgyW", &fontWidth, &fontHeight);
                fontWidth/=4;

            }
        }
        else if( get_file_ext(newFileName)=="woff" || get_file_ext(newFileName)=="WOFF" )
        {
            std::string copyDestinationStr = fileToDir(parentProjectName)+"/gpe_project/resources/fonts/"+ getShortFileName(newFileName,true);
            storedFontFileNames[FONT_WOFF] = copyDestinationStr;
            copy_file(newFileName.c_str(),copyDestinationStr );
        }
        else if( get_file_ext(newFileName)=="woff2" || get_file_ext(newFileName)=="WOFF2" )
        {
            std::string copyDestinationStr = fileToDir(parentProjectName)+"/gpe_project/resources/fonts/"+ getShortFileName(newFileName,true);
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
        displayMessageTitle = "Processing Font";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();

        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/fonts/";
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

        record_error("Loading font - "+newFileIn);
        //If the level file could be loaded
        if( gameResourceFileIn != NULL )
        {
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string section="";
                std::string cur_layer="";
                std::string data_format="";
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
                                        foundFileVersion = string_to_double(valString);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion < 2)
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
                        record_error("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
    }
}

void fontResource::prerender_self(GPE_Renderer * cRender)
{
    standardEditableGameResource::prerender_self(cRender);
    if( fontTypeButtonController!=NULL)
    {
        fontTypeButtonController->prerender_self(cRender);
    }
}

void fontResource::process_self(SDL_Rect *viewedSpace,SDL_Rect *cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL && fontSizeField!=NULL)
    {
        editorPaneList->set_coords(0,0);
        editorPaneList->set_width(256);
        editorPaneList->set_height(viewedSpace->h);
        editorPaneList->barXPadding = GENERAL_GPE_PADDING;
        editorPaneList->barXMargin  = GENERAL_GPE_PADDING;

        editorPaneList->clear_list();
        editorPaneList->add_gui_element(exportResourceButton,true);
        editorPaneList->add_gui_element(renameBox,true);
        editorPaneList->add_gui_element(loadResourceButton,true);

        editorPaneList->add_gui_element(fontSizeField,true);
        editorPaneList->add_gui_element(fontPreviewTextField,true);
        editorPaneList->add_gui_element(openExternalEditorButton,true);
        editorPaneList->add_gui_element(fontTypeButtonController,true);
        editorPaneList->add_gui_element(confirmResourceButton,true);
        editorPaneList->add_gui_element(cancelResourceButton,true);
        int prevFontSize = fontSizeField->get_held_number();


        editorPaneList->process_self(viewedSpace,cam);
        if( openExternalEditorButton!=NULL)
        {
            if( openExternalEditorButton->is_clicked() )
            {
                bool hasFileToOpen = false;
                int ii = 0;
                for( ii = 0; ii < SUPPORTED_AUDIO_FORMAT_COUNT; ii++)
                {
                    if( (int)storedFontFileNames[ii].size() > 0)
                    {
                        hasFileToOpen = true;
                        break;
                    }
                }
                if( hasFileToOpen )
                {
                    GPE_open_context_menu();
                    MAIN_CONTEXT_MENU->set_width( 400 );
                    for( ii = 0; ii < FONT_FILE_TYPES; ii++)
                    {
                        if( (int)storedFontFileNames[ii].size() > 0)
                        {
                            MAIN_CONTEXT_MENU->add_menu_option("Edit "+storedFontFileNames[ii],ii,NULL,-1,NULL,true,true);
                        }
                    }
                    MAIN_CONTEXT_MENU->set_width( MAIN_CONTEXT_MENU->subOptionWidthSpace );
                    int menuSelection = get_popupmenu_result();
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
            std::string newFontFile = GPE_GetOpenFileName("Load In Custom Font...","",GPE_MAIN_GUI->fileOpenFontDir );
            if( (int)newFontFile.size() > 3)
            {
                load_font( newFontFile, fontSizeField->get_held_number() );
            }
        }
        if( confirmResourceButton->is_clicked() )
        {
            save_resource();
        }
    }
}

void fontResource::render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace,SDL_Rect *cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL)
    {
        int fontPreviewXPos = 256;
        if( forceRedraw)
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
        }

        editorPaneList->render_self(cRender,viewedSpace,cam,forceRedraw);
        if( forceRedraw)
        {
            if(fontTypeButtonController!=NULL)
            {
                render_vertical_line_color(cRender,fontPreviewXPos,0,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Header_Color );
            }
            if( fontInEditor!=NULL)
            {
                render_new_text(cRender,fontPreviewXPos+GENERAL_GPE_PADDING,GENERAL_GPE_PADDING,"Font Preview: "+getShortFileName(storedFontFileNames[FONT_TTF],true)+" | "+fontFamilyName ,GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_CATEGORY_BAR,FA_LEFT,FA_TOP);
                render_new_text(cRender,fontPreviewXPos+GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+32,fontPreviewTextField->get_string(),GPE_MAIN_TEMPLATE->Main_Box_Font_Color,fontInEditor,FA_LEFT,FA_TOP);
            }
            else
            {
                render_new_text(cRender,fontPreviewXPos+GENERAL_GPE_PADDING,GENERAL_GPE_PADDING,"Please Select A Font",GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_CATEGORY_BAR,FA_LEFT,FA_TOP);
            }
            for( int i = FONT_FILE_TYPES-1; i >=0; i--)
            {
                if( storedFontFileNames[i].size()> 3)
                {
                    render_new_text(cRender,viewedSpace->w-GENERAL_GPE_PADDING*2,viewedSpace->h-GENERAL_GPE_PADDING-GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_FONT_EXT[i]+" is used",GPE_MAIN_TEMPLATE->Main_Suggestion_Font_Color,DEFAULT_FONT,FA_RIGHT,FA_BOTTOM);
                }
                else
                {
                    render_new_text(cRender,viewedSpace->w-GENERAL_GPE_PADDING*2,viewedSpace->h-GENERAL_GPE_PADDING-GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_FONT_EXT[i]+" not used",GPE_MAIN_TEMPLATE->Main_Error_Font_Color,DEFAULT_FONT,FA_RIGHT,FA_BOTTOM);
                }
            }
        }
    }
}

void fontResource::save_resource(std::string alternatePath, int backupId)
{
    displayMessageTitle = "Saving Game Font";
    displayMessageSubtitle = resourceName;
    displayMessageString = "...";
    display_user_messaage();

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
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/fonts/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the font file could be saved
    if( newSaveDataFile != NULL )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    Game Pencil Engine Project Font DataFile \n";
            newSaveDataFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newSaveDataFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newSaveDataFile << "#    If you have any problems reading this file please report it to debug@pawbyte.com . \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            newSaveDataFile << "#     \n";
            if( fontSizeField!=NULL)
            {
                newSaveDataFile << "FontSize=" << std::max(8,fontSizeField->get_held_number() )<< "\n";
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
                        resFileCopySrc = fileToDir(parentProjectName)+"/gpe_project/resources/fonts/"+resFileLocation;
                        resFileCopyDest = soughtDir+resFileLocation;
                        if( file_exists(resFileCopyDest) )
                        {
                            if( display_get_prompt("[WARNING]Font File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] font file? This action is irreversible!")==DISPLAY_QUERY_YES)
                            {
                                copy_file(resFileCopySrc,resFileCopyDest);
                            }
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
            displayMessageTitle = "Font Successfully Saved!";
            displayMessageSubtitle = resourceName;
            displayMessageString = "...";
            display_user_messaage();
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

