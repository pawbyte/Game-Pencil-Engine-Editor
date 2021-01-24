/*
functionResource.cpp
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

#include "function_resource.h"

functionResource::functionResource(pawgui::widget_resource_container * pFolder)
{
    projectParentFolder = pFolder;

    /*cancelResourceButton->disable_self();
    confirmResourceButton->disable_self();
    loadResourceButton->disable_self();
    saveResourceButton->disable_self();
    */
    //textEditorButtonBar->set_width(256);
    functionReturnType = new pawgui::widget_input_text("void","");
    parametersField = new pawgui::widget_input_text("","parameter1, parameter2, etc");
    parametersField->set_label("Function Parameters");
    functionCode = new pawgui::widget_text_editor(false);
    renameBox->set_coords(pawgui::padding_default,-1 );
    saveButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png","Save Changes",-1,24);
    saveButton->set_height(parametersField->get_height() );
    saveButton->set_width(parametersField->get_height() );
    functionEditorList = new pawgui::widget_panel_list();
    //loadResourceButton->disable_self();
    //saveResourceButton->disable_self();
}

functionResource::~functionResource()
{
    if(functionEditorList!=NULL)
    {
        delete functionEditorList;
        functionEditorList = NULL;
    }

    if(functionCode!=NULL)
    {
        delete functionCode;
        functionCode = NULL;
    }

    if(functionReturnType!=NULL)
    {
        delete functionReturnType;
        functionReturnType = NULL;
    }

    if(parametersField!=NULL)
    {
        delete parametersField;
        parametersField = NULL;
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

bool functionResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    return true;
}

bool functionResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void functionResource::compile_cpp()
{

}

bool functionResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

void functionResource::integrate_into_syntax()
{
    if( current_project!=NULL)
    {
        std::string fReturnType = "unknown_void";
        if( functionReturnType!=NULL)
        {
            if( functionReturnType->get_string().size() > 0)
            {
                fReturnType = functionReturnType->get_string();
            }
        }
        std::string functionDescription = "User defined function...";
        if( functionCode!=NULL)
        {
            std::string firstLineOfCode = functionCode->get_line_string(0);
            if( (int)firstLineOfCode.size() > 2)
            {
                if( firstLineOfCode[0]=='/' && (firstLineOfCode.at(1)=='/' || firstLineOfCode.at(1)=='*' ) )
                {
                    functionDescription = stg_ex::get_substring(firstLineOfCode,2);
                }
            }
        }
        if( parametersField !=NULL )
        {
            current_project->add_project_function(resource_name,functionDescription,parametersField->get_string(),fReturnType,"User Defined Global Function");
        }
        else
        {
            current_project->add_project_function(resource_name,functionDescription,"",fReturnType,"User Defined Global Function");
        }
    }
}

bool functionResource::is_build_ready()
{
    recentErrorMessage ="";
    return true;
}

void functionResource::open_code( int lineNumb, int colNumb,std::string codeTitle)
{
    if( functionCode!=NULL && functionCode->has_content() )
    {
        functionCode->scroll_to_pos(lineNumb, colNumb);
    }
}

void functionResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed ==false  || sff_ex::file_exists(file_path) )
    {
        if( pawgui::main_loader_display != NULL )
        {
            pawgui::main_loader_display->update_submessages( "Processing Function", resource_name );
        }

        bool usingAltSaveSource = false;
        std::string newFileIn ="";
        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/source/";
        if( sff_ex::file_exists(file_path) )
        {
            newFileIn = file_path;
            soughtDir = stg_ex::get_path_from_file(newFileIn);
            usingAltSaveSource = true;
        }
        else
        {
            newFileIn = soughtDir + resource_name+".gpf";
        }


        std::string  functionCodeLoadLocation = soughtDir+ "function_" + resource_name + ".cps";

        if( current_project !=NULL )
        {
            if( current_project->get_project_language_id() ==
               pawgui::program_language_cpp )
            {
                functionCodeLoadLocation = soughtDir+  "function_" + resource_name + ".cpp";
            }
        }

        if( functionCode!=NULL)
        {
            functionCode->import_text(functionCodeLoadLocation);
            functionCode->activate_self();
            functionCode->init_save_history();
        }

        std::string otherColContainerName = "";

        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        gpe::error_log->report("Loading function - "+newFileIn);
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
                int fCursorX = 0;
                int fCursorY = 0;
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
                                else if( keyString=="Parameters")
                                {
                                    parametersField->set_string(valString);
                                }
                                if( keyString=="ReturnType")
                                {
                                    functionReturnType->set_string(valString);
                                }
                                else if( keyString=="Cursor")
                                {
                                    fCursorY = stg_ex::split_first_int(valString,',');
                                    fCursorX = stg_ex::string_to_int(valString,0);
                                    functionCode->set_ycursor(fCursorY);
                                    functionCode->set_xcursor(fCursorX);
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

void functionResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
}

void functionResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if(cam!=NULL && view_space!=NULL && saveButton!=NULL && functionEditorList!=NULL && parametersField!=NULL && functionCode!=NULL )
    {
        functionEditorList->set_coords( 0,0 );
        functionEditorList->set_width(view_space->w );
        functionEditorList->set_height(view_space->h);
        functionEditorList->clear_list();
        if( panel_main_area!=NULL )
        {
            functionEditorList->barXMargin = functionEditorList->barYMargin = 0;
            functionEditorList->barXPadding = functionEditorList->barYPadding = 0;
            functionCode->set_width(view_space->w );
            functionCode->set_height(view_space->h );

            //Process function parameter elements in the Editor Panel
            panel_main_area->clear_panel();
            panel_main_area->add_gui_element(renameBox,true);
            panel_main_area->add_gui_element(parametersField,true);
            panel_main_area->add_gui_element(confirmResourceButton,true);
            panel_main_area->add_gui_element(cancelResourceButton,true);
            panel_main_area->process_self();

            functionEditorList->add_gui_element_fullsize( functionCode );
            functionEditorList->process_self( view_space,cam );
            if( confirmResourceButton->is_clicked() )
            {
                save_resource();
            }
            else if( cancelResourceButton->is_clicked() )
            {
                if( pawgui::display_prompt_message("Are you sure you will like to reverse code changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
                {
                    resourcePostProcessed = false;
                    load_resource();
                }
            }
        }
        else
        {
            functionEditorList->barXMargin = functionEditorList->barYMargin = pawgui::padding_default;
            functionEditorList->barXPadding = functionEditorList->barYPadding = pawgui::padding_default;
            functionEditorList->set_coords( 0,0 );
            functionCode->set_width(view_space->w-pawgui::padding_default*4 );
            functionCode->set_height(view_space->h - pawgui::padding_default*4 - 32 );
            functionEditorList->add_gui_element(saveButton,false);
            functionEditorList->add_gui_element(renameBox,false);
            functionEditorList->add_gui_element(parametersField,true);
            functionEditorList->add_gui_element(functionCode,true);
            functionEditorList->process_self( view_space,cam );
            if( saveButton->is_clicked() )
            {
                save_resource();
            }
        }

        // parametersField->set_height(renameBox->get_height() );
    }
}

void functionResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if(cam!=NULL && view_space!=NULL && functionEditorList!=NULL )
    {
        functionEditorList->render_self( view_space, cam );
    }
}

void functionResource::save_resource(std::string file_path, int backupId)
{
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Processing Function", resource_name );
    }
    sff_ex::append_to_file(file_path,"blank");
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
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/source/";
        newFileOut = soughtDir + "function_" + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);

            if( functionCode!=NULL)
            {
                //    *fileTarget << functionCode->get_xcursor() << "," << functionCode->get_ycursor() << ",";
                std::string  functionCodeSaveLocation = soughtDir+resource_name+".cps";
                if( usingAltSaveSource)
                {
                    if( sff_ex::file_exists(functionCodeSaveLocation) )
                    {
                        /*
                        if( pawgui::display_prompt_message("[WARNING]Function File Already exists?","Are you sure you will like to overwrite your ["+resource_name+".cps] Function file? This action is irreversible!")==pawgui::display_query_yes)
                        {
                            functionCode->export_text(functionCodeSaveLocation );
                        }
                        */
                    }
                    else
                    {
                        functionCode->export_text(functionCodeSaveLocation );
                    }
                }
                else
                {
                    functionCode->export_text(functionCodeSaveLocation );
                }
                newSaveDataFile << "Cursor=" << functionCode->get_ycursor() << "," << functionCode->get_xcursor() << "\n";
            }
            else
            {
                //*fileTarget << "0,0,";
            }

            std::string paramStrSize = parametersField->get_string();
            if( paramStrSize.size() > 0)
            {
                newSaveDataFile << "Parameters=" << parametersField->get_string() << "\n";
            }
            else
            {
                //*fileTarget << " ||,";
            }
            newSaveDataFile << "ReturnType=" << functionReturnType->get_string() << "\n";
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

int functionResource::search_for_string(std::string needle)
{
    int foundStrings = 0;
    main_editor_log->log_general_comment("Searching ["+resource_name+"] function..");
    if( functionCode!=NULL && pawgui::main_anchor_controller!=NULL && functionCode->has_content() )
    {
        pawgui::main_anchor_controller->searchResultProjectName = parentProjectName;
        pawgui::main_anchor_controller->searchResultResourceId = globalResouceIdNumber;
        pawgui::main_anchor_controller->searchResultResourceName = resource_name;
        foundStrings=functionCode->find_all_strings(needle,pawgui::main_search_controller->findMatchCase->is_clicked(),true, "function");
    }
    return foundStrings;
}

int functionResource::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundStrings = 0;
    if( functionCode!=NULL && pawgui::main_search_controller!=NULL && functionCode->has_content() )
    {
        foundStrings=functionCode->find_all_strings(needle,pawgui::main_search_controller->findMatchCase->is_clicked() );
        if( pawgui::main_loader_display != NULL )
        {
            pawgui::main_loader_display->update_messages( "Replacing Substring", needle, "with ["+newStr+"]" );
        }

        if( foundStrings > 0)
        {
            int replaceCount =  functionCode->replace_all_found(needle, newStr );
            pawgui::main_loader_display->update_messages( "Replaced Substring", needle, stg_ex::int_to_string(replaceCount) +" times" );
            pawgui::main_search_controller->showFindAllResults = true;
        }
        else
        {
            pawgui::main_loader_display->update_messages( "Replacing Substring", needle, "No matches found");
            pawgui::main_search_controller->showFindAllResults = false;
        }
        //pawgui::main_overlay_system->update_temporary_message(displayMessageTitle,displayMessageSubtitle,displayMessageString,1);
    }
    return foundStrings;
}

bool functionResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Saving Function", resource_name );
    }
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Function=" << resource_name << "," << get_global_rid() << ",\n";
            save_resource();
            return true;
        }
    }
    return false;
}
