/*
class_resource.cpp
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

#include "class_resource.h"

classResource::classResource(GPE_GeneralResourceContainer * pFolder)
{
    codeCategoryTabs = new GPE_TabBar();
    codeCategoryTabs->set_coords(16,16);
    codeCategoryTabs->add_new_tab("Header",false);
    codeCategoryTabs->add_new_tab("Source",true);
    classHeaderCode = NULL;
    classHeaderCode = new GPE_TextAreaInputBasic(false);
    classHeaderCode->set_placeholder("Header Code...");


    classSourceCode = new GPE_TextAreaInputBasic(false);
    classSourceCode->set_placeholder("Source Code...");

    projectParentFolder = pFolder;

    /*cancelResourceButton->disable_self();
    confirmResourceButton->disable_self();
    loadResourceButton->disable_self();
    saveResourceButton->disable_self();*/
    //textEditorButtonBar->set_width(256);
    renameBox->set_coords(GENERAL_GPE_PADDING,-1 );
    saveButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/save.png","Save Changes",-1,24);
    classEditorList = new GPE_GuiElementList();
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

    if( classEditorList!=NULL)
    {
        delete classEditorList;
        classEditorList = NULL;
    }

}

bool classResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    return true;
}

bool classResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void classResource::compile_cpp()
{

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
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Class", resourceName );
        }

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

        GPE_Report("Loading class meta data - "+newFileIn);
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
                double foundFileVersion = 0;
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
                        GPE_Report("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
    }
}

void classResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
}

void classResource::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    gpeEditorDockPanel * fEditorPanel = GPE_DOCK->find_panel("Editor");
    if( classEditorList!=NULL && cam!=NULL && viewedSpace!=NULL && codeCategoryTabs!=NULL &&  saveButton!=NULL && renameBox!=NULL && classSourceCode!=NULL )
    {
        int prevTab = codeCategoryTabs->tabInUse;

        classEditorList->set_coords( 0, 0 );
        classEditorList->set_width(viewedSpace->w );
        classEditorList->set_height(viewedSpace->h);
        classEditorList->clear_list();

        classHeaderCode->set_width(viewedSpace->w );
        classHeaderCode->set_height(viewedSpace->h-64 );

        classSourceCode->set_width(viewedSpace->w );
        classSourceCode->set_height(viewedSpace->h-64);

        classEditorList->set_coords( 0, 0 );
        classEditorList->set_width( viewedSpace->w );
        classEditorList->set_height( viewedSpace->h);
        classEditorList->clear_list();

        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);
            PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);
            PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
            PANEL_GENERAL_EDITOR->process_self();
        }
        else
        {
            classEditorList->add_gui_element(saveButton,false);
            classEditorList->add_gui_element(renameBox,true);
        }
        //CPP is the only language that does header files so...
        if( CURRENT_PROJECT->get_project_language_id() == PROGRAM_LANGUAGE_CPP)
        {
            if( codeCategoryTabs->get_selected_name()=="Header" && classHeaderCode!=NULL)
            {
                classEditorList->add_gui_element(classHeaderCode,true);
            }
            else
            {
                classEditorList->add_gui_element(classSourceCode,true);
            }
            classEditorList->add_gui_element(codeCategoryTabs,true);
        }
        else
        {
            classEditorList->add_gui_element(classSourceCode,true);
        }

        classEditorList->process_self( viewedSpace,cam );
        if( saveButton->is_clicked() )
        {
            save_resource();
        }
        else if( PANEL_GENERAL_EDITOR!=NULL )
        {
            if( confirmResourceButton->is_clicked() )
            {
                save_resource();
            }
        }
    }
}

void classResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL)
    {
        classEditorList->render_self( viewedSpace,cam, forceRedraw );
    }
}

void classResource::save_resource(std::string alternatePath, int backupId)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Class", resourceName );
    }
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
            write_header_on_file(&newSaveDataFile);

            if( classSourceCode!=NULL)
            {
                //    *fileTarget << classSourceCode->get_xcursor() << "," << classSourceCode->get_ycursor() << ",";
                //std::string headerCodeSaveLocation = soughtDir+resourceName+".h"; // CPP headers
                std::string sourceCodeSaveLocation = soughtDir+resourceName+".js";
                if( usingAltSaveSource)
                {
                    if( file_exists(sourceCodeSaveLocation) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]Class File Already exists?","Are you sure you will like to overwrite your ["+resourceName+".js] Class file? This action is irreversible!")==DISPLAY_QUERY_YES)
                        {
                            classSourceCode->export_text(sourceCodeSaveLocation );
                        }*/
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
        foundStrings=classSourceCode->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked(),true);
    }
    return foundStrings;
}

int classResource::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundStrings = 0;
    if( classSourceCode!=NULL && MAIN_SEARCH_CONTROLLER!=NULL && classSourceCode->has_content() )
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_messages( "Replacing Substring", needle, "with ["+newStr+"]" );
        }

        foundStrings = classSourceCode->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() );
        if( foundStrings > 0)
        {
            int replaceCount = classSourceCode->replace_all_found(needle, newStr );
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Replaced", needle, int_to_string(replaceCount)+" times");
            }
            MAIN_SEARCH_CONTROLLER->showFindAllResults = true;
        }
        else
        {
            GPE_LOADER->update_messages( "Replaced", needle, "No matches found");
            MAIN_SEARCH_CONTROLLER->showFindAllResults = false;
        }
        //MAIN_OVERLAY->update_temporary_message(displayMessageTitle,displayMessageSubtitle,displayMessageString,1);
    }
    return foundStrings;
}

bool classResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
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
