/*
classresource.cpp
This file is part of:
GAME PENCIL ENGINE
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

#include "classresource.h"

classResource::classResource(GPE_ResourceContainer * pFolder)
{
    codeCategoryTabs = new GPE_TabBar();
    codeCategoryTabs->set_coords(16,16);
    codeCategoryTabs->add_new_tab("Source",true);
    codeCategoryTabs->add_new_tab("Header",false);
    classHeaderCode = NULL;
    classHeaderCode = new GPE_TextAreaInputBasic(false);
    classHeaderCode->set_placeholder("Header Code...");


    classSourceCode = new GPE_TextAreaInputBasic(false);
    classSourceCode->set_placeholder("Source Code...");

    projectParentFolder = pFolder;

    cancelResourceButton->disable_self();
    confirmResourceButton->disable_self();
    loadResourceButton->disable_self();
    saveResourceButton->disable_self();
    //textEditorButtonBar->set_width(256);
    renameBox->set_coords(GENERAL_GPE_PADDING,-1 );
    saveButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/save.png","Save Changes",-1,24);

    //saveResourceButton->disable_self();
}

classResource::~classResource()
{
    if(classHeaderCode!=NULL)
    {
        delete classHeaderCode;
        classHeaderCode = NULL;
    }
    if(classSourceCode!=NULL)
    {
        delete classSourceCode;
        classSourceCode = NULL;
    }
    if(codeCategoryTabs!=NULL)
    {
        delete codeCategoryTabs;
        codeCategoryTabs = NULL;
    }

    if(renameBox!=NULL)
    {
        delete renameBox;
        renameBox = NULL;
    }
    if( saveButton!=NULL)
    {
        delete saveButton;
        saveButton = NULL;
    }

}

bool classResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    return true;
}

void classResource::integrate_into_syntax()
{
    if( CURRENT_PROJECT!=NULL)
    {
        std::string fReturnType = "unknown_void";
        std::string functionDescription = "User defined function...";
        /*
        if( parametersField !=NULL )
        {
            CURRENT_PROJECT->add_project_function(resourceName,functionDescription,parametersField->get_string(),fReturnType,"User Defined Global Function");
        }
        */
    }
}

void classResource::open_code( int lineNumb, int colNumb,std::string codeTitle)
{
    if( classHeaderCode!=NULL && classHeaderCode->has_content() && codeTitle=="header" )
    {
        classHeaderCode->scroll_to_pos(lineNumb, colNumb);
    }
    else  if( classSourceCode!=NULL && classSourceCode->has_content() )
    {
        classSourceCode->scroll_to_pos(lineNumb, colNumb);
    }
}

void classResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        displayMessageTitle = "Processing Class";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();

        bool usingAltSaveSource = false;
        std::string newFileIn ="";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/classes/";
        if( file_exists(alternatePath) )
        {
            newFileIn = alternatePath;
            soughtDir = get_path_from_file(newFileIn);
            usingAltSaveSource = true;
        }
        else
        {
            newFileIn = soughtDir + resourceName+".gpf";
        }


        if( classSourceCode!=NULL)
        {
            std::string  classSrcCodeLoadLocation = "";
            //if( )
            classSrcCodeLoadLocation = soughtDir+resourceName+".js";

            classSourceCode->import_text(classSrcCodeLoadLocation);
            classSourceCode->activate_self();
            classSourceCode->init_save_history();
        }


        if( classHeaderCode!=NULL)
        {
            std::string classHeaderCodeLoadLocation = soughtDir+resourceName+".h";
            classHeaderCode->import_text(classHeaderCodeLoadLocation);
            classHeaderCode->activate_self();
            classHeaderCode->init_save_history();
        }

        std::string otherColContainerName = "";

        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        record_error("Loading class meta data - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
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
                int fCursorX = 0;
                int fCursorY = 0;
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
                                else if( keyString=="Header_Cursor" && classHeaderCode!=NULL)
                                {
                                    fCursorY = split_first_int(valString,',');
                                    fCursorX = string_to_int(valString,0);
                                    classHeaderCode->set_ycursor(fCursorY);
                                    classHeaderCode->set_xcursor(fCursorX);
                                }
                                else if( keyString=="Source_Cursor" & classSourceCode!=NULL)
                                {
                                    fCursorY = split_first_int(valString,',');
                                    fCursorX = string_to_int(valString,0);
                                    classSourceCode->set_ycursor(fCursorY);
                                    classSourceCode->set_xcursor(fCursorX);
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

void classResource::prerender_self(GPE_Renderer * cRender )
{
	standardEditableGameResource::prerender_self( cRender);
}

void classResource::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && codeCategoryTabs!=NULL &&  saveButton!=NULL && renameBox!=NULL && classSourceCode!=NULL )
    {
        int prevTab = codeCategoryTabs->tabInUse;

        classHeaderCode->set_width(viewedSpace->w-128 );
        classHeaderCode->set_height(viewedSpace->h-64 );

        classSourceCode->set_width(viewedSpace->w-128 );
        classSourceCode->set_height(viewedSpace->h-64);

        editorPaneList->set_coords(0,0);
        editorPaneList->set_width(viewedSpace->w);
        editorPaneList->set_height(viewedSpace->h);
        editorPaneList->barXMargin = GENERAL_GPE_PADDING;
        editorPaneList->barXPadding = 0;
        editorPaneList->barYMargin = GENERAL_GPE_PADDING;
        editorPaneList->barYPadding = 0;

        editorPaneList->clear_list();
        editorPaneList->add_gui_element(saveButton,false);
        editorPaneList->add_gui_element(renameBox,true);

        //CPP is the only language that does header files so...
        if( CURRENT_PROJECT->myProjectLanguage == PROJECT_LANGUAGE_CPP)
        {
            classHeaderCode->set_height(viewedSpace->h-128);
            classSourceCode->set_height(viewedSpace->h-64);
            if( codeCategoryTabs->get_selected_name()=="Header" && classHeaderCode!=NULL)
            {

                editorPaneList->add_gui_element(classHeaderCode,true);
            }
            else
            {
                editorPaneList->add_gui_element(classSourceCode,true);
            }
            editorPaneList->add_gui_element(codeCategoryTabs,true);
        }
        else
        {
            editorPaneList->add_gui_element(classSourceCode,true);
        }

        editorPaneList->process_self( viewedSpace,cam );
        if( saveButton->is_clicked() )
        {
            save_resource();
        }
    }
}

void classResource::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL)
    {
        if( forceRedraw)
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
        }
        if( editorPaneList!=NULL)
        {
            editorPaneList->render_self(cRender, viewedSpace, cam, forceRedraw);
        }
    }
}

void classResource::save_resource(std::string alternatePath, int backupId)
{
    appendToFile(alternatePath,"blank");
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
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/classes/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    Game Pencil Engine Project Game Class DataFile \n";
            newSaveDataFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newSaveDataFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newSaveDataFile << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            newSaveDataFile << "ResourceName=" << resourceName << "\n";

            if( classSourceCode!=NULL)
            {
                displayMessageString = int_to_string( classSourceCode->get_line_count() )+" lines of code.";
            //    *fileTarget << classSourceCode->get_xcursor() << "," << classSourceCode->get_ycursor() << ",";
                //std::string headerCodeSaveLocation = soughtDir+resourceName+".h"; // CPP headers
                std::string sourceCodeSaveLocation = soughtDir+resourceName+".js";
                if( usingAltSaveSource)
                {
                    if( file_exists(sourceCodeSaveLocation) )
                    {
                        if( display_get_prompt("[WARNING]Class File Already exists?","Are you sure you will like to overwrite your ["+resourceName+".js] Class file? This action is irreversible!")==DISPLAY_QUERY_YES)
                        {
                            classSourceCode->export_text(sourceCodeSaveLocation );
                        }
                    }
                    else
                    {
                        classSourceCode->export_text(sourceCodeSaveLocation );
                    }
                }
                else
                {
                    classSourceCode->export_text(sourceCodeSaveLocation );
                }
                newSaveDataFile << "Cursor=" << classSourceCode->get_ycursor() << "," << classSourceCode->get_xcursor() << "\n";
            }
            else
            {
                //*fileTarget << "0,0,";
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

int classResource::search_for_string(std::string needle)
{
    int foundStrings = 0;
    GPE_Main_Logs->log_general_comment("Searching ["+resourceName+"] class..");
    if( classSourceCode!=NULL && GPE_MAIN_GUI!=NULL && classSourceCode->has_content() )
    {
        GPE_MAIN_GUI->searchResultProjectName = parentProjectName;
        GPE_MAIN_GUI->searchResultResourceId = globalResouceIdNumber;
        GPE_MAIN_GUI->searchResultResourceName = resourceName;
        foundStrings=classSourceCode->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,true);
    }
    return foundStrings;
}

int classResource::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundStrings = 0;
    if( classSourceCode!=NULL && MAIN_SEARCH_CONTROLLER!=NULL && classSourceCode->has_content() )
    {
        foundStrings=classSourceCode->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() );
        displayMessageTitle = "Replacing Substring";
        displayMessageSubtitle = needle;

        if( foundStrings > 0)
        {
            displayMessageString = "Replaced "+int_to_string( classSourceCode->replace_all_found(needle, newStr ) )+" copies.";;
            MAIN_SEARCH_CONTROLLER->showFindAllResults = true;
        }
        else
        {
            displayMessageString ="No matches found";
            MAIN_SEARCH_CONTROLLER->showFindAllResults = false;
        }
        display_user_messaage();
        MAIN_OVERLAY->update_temporary_message(displayMessageTitle,displayMessageSubtitle,displayMessageString,1);
    }
    return foundStrings;
}

bool classResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    displayMessageTitle = "Saving Class";
    displayMessageSubtitle = resourceName;
    displayMessageString = "...";
    display_user_messaage();
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Class=" << resourceName << "," << get_global_rid() << ",\n";
            save_resource();
            return true;
        }
    }
    return false;
}
