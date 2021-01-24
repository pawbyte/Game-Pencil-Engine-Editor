/*
gpe_cpp_builder_settings.cpp
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

#include "gpe_cpp_builder_settings.h"

gameCPPBuilderSettingsResource * GPE_CPP_BUILDER_SETTINGS = NULL;

gpeCPPSubsytemHolder::gpeCPPSubsytemHolder( std::string gpeSubSystemName )
{
    subsystemName = gpeSubSystemName;
    systemNameField = new pawgui::widget_input_text(gpeSubSystemName,"ie: visualstudio,codeblocks,cmake");
    systemNameLabel = new pawgui::widget_label_text ("Build Subsystem Name:","subsystem used within project build phase");
    removeSystemButton = new pawgui::widget_button_label("Remove build subsystem","deletes build subsystem");

    systemProgramScriptLabel = new pawgui::widget_label_text ("Subsystem GPE Script:","Used to auto-generate file");
    mainProgramFileScript = new pawgui::widget_input_text("","Subsystem GPE file script");
    mainProgramFileScriptButton = new pawgui::widget_button_label("Browse...","Search for gpe substem build script...");

    systemProgramTemplateFileLabel = new pawgui::widget_label_text ("Subsystem GPE File Template:","Template file copied in auto-generation");
    systemProgramTemplateFile = new pawgui::widget_input_text("","Subsystem GPE template file");
    systemProgramTemplateFileButton = new pawgui::widget_button_label("Browse...","Search for gpe substem template file...");
}

gpeCPPSubsytemHolder::~gpeCPPSubsytemHolder()
{

}

bool gpeCPPSubsytemHolder::add_to_list(pawgui::widget_panel_list * listIn)
{
    if( listIn ==NULL)
    return false;
    listIn->add_gui_element( systemNameLabel , false );
    listIn->add_gui_element( systemNameField , true );

    listIn->add_gui_element( removeSystemButton , true );


    listIn->add_gui_element( systemProgramScriptLabel , true );
    listIn->add_gui_element( mainProgramFileScriptButton , false );
    listIn->add_gui_element( mainProgramFileScript , true );

    listIn->add_gui_element( systemProgramTemplateFileLabel , true );
    listIn->add_gui_element( systemProgramTemplateFileButton , false );
    listIn->add_gui_element( systemProgramTemplateFile , true );

    return true;
}

gpeCPPBuildHolder::gpeCPPBuildHolder(std::string buildSystemName)
{
    subSystemToRemove = -1; //keep negative to avoid deletions
    builderName = buildSystemName;
    builderNameField = new pawgui::widget_input_text(buildSystemName,"ie: Visual Studio, CodeBlocks, DevC++, Premake5...");
    builderNameLabel = new pawgui::widget_label_text ("Builder System Name:");
    addSubSystemButton = new pawgui::widget_button_label("Add subsystem","Add a new subsystem for C++ builder");
    buildSystemOS = new pawgui::widget_dropdown_menu("Build Platform", false );
}

gpeCPPBuildHolder::~gpeCPPBuildHolder()
{
    if( builderNameField!=NULL)
    {
        delete builderNameField;
        builderNameField = NULL;
    }

    if( builderNameLabel!=NULL)
    {
        delete builderNameLabel;
        builderNameLabel = NULL;
    }
    if( buildSystemOS!=NULL)
    {
        delete buildSystemOS;
        buildSystemOS = NULL;
    }

    clear_subsystems();
}


bool gpeCPPBuildHolder::add_to_list(pawgui::widget_panel_list * listIn)
{
    if( subSystemToRemove >= 0 )
    {
        //removes subsystem from pending remove request before adding to list
        remove_subsystem_indexed( subSystemToRemove, true );
    }
    if( listIn ==NULL)
    return false;
    listIn->add_gui_element( builderNameLabel, true);
    listIn->add_gui_element( builderNameField, true);
    listIn->add_gui_element( buildSystemOS, true);
    listIn->add_gui_element( addSubSystemButton, true);

    int subSystemCount = (int)mySubsystems.size();
    if( subSystemCount > 0 )
    {
        gpeCPPSubsytemHolder * tempSubSystem = NULL;
        for( int i = 0; i < subSystemCount; i++ )
        {
            tempSubSystem = mySubsystems[i];
            tempSubSystem->add_to_list( listIn);
        }
    }

    return true;
}

gpeCPPSubsytemHolder * gpeCPPBuildHolder::add_subsystem( std::string newSysName )
{
    //Checks if subsystem already exists with name.
    if( (int)mySubsystems.size() > 0  && (int)newSysName.size() > 0 )
    {
        if( find_sub_system( newSysName) !=NULL )
        {
            if( main_editor_log!=NULL)
            {
                main_editor_log->log_general_error("Subsystem["+newSysName+"] name already used for ["+builderName+"] build system.");
            }
            return NULL;
        }
    }
    gpeCPPSubsytemHolder * tempSys = new gpeCPPSubsytemHolder(newSysName);
    mySubsystems.push_back( tempSys );
    return tempSys;
}

bool gpeCPPBuildHolder::clear_subsystems()
{
    if( (int)mySubsystems.size() > 0 )
    {
        gpeCPPSubsytemHolder * tempSys = NULL;
        for( int i = (int)mySubsystems.size()-1; i >=0; i-- )
        {
            tempSys = mySubsystems[i];
            if(tempSys !=NULL)
            {
                delete tempSys;
                tempSys = NULL;
            }
        }
        mySubsystems.clear();
        return true; //systems deleted
    }
    return false;
}

gpeCPPSubsytemHolder * gpeCPPBuildHolder::find_sub_system( std::string sysName )
{
    gpeCPPSubsytemHolder * tempSys = NULL;
    for( int i = (int)mySubsystems.size()-1; i >=0; i-- )
    {
        tempSys = mySubsystems[i];
        if( tempSys!=NULL )
        {
            if( tempSys->subsystemName == sysName )
            {
                return tempSys;
            }
        }
    }
    return NULL;
}

void gpeCPPBuildHolder::process_self()
{
    if( addSubSystemButton->is_clicked() )
    {
        std::string newSubSystemName = pawgui::get_string_from_popup("Name your new subsystem","","");
        if( (int)newSubSystemName.size() > 0 )
        {
            add_subsystem( newSubSystemName );
        }
        else if( main_editor_log!=NULL)
        {
            main_editor_log->log_general_error("["+builderName+"] build system add_subsystem exited, (Name not entered).");
        }
    }

    //Process the subsystems
    int subSystemCount = (int)mySubsystems.size();
    if( subSystemCount > 0 )
    {
        gpeCPPSubsytemHolder * tempSubSystem = NULL;
        for( int i = subSystemCount -1; i >=0; i-- )
        {
            tempSubSystem = mySubsystems[i];
            if( tempSubSystem!=NULL )
            {
                if( tempSubSystem->removeSystemButton!=NULL && tempSubSystem->removeSystemButton->is_clicked()  )
                {
                    if( pawgui::display_prompt_message("Are you sure you want to delete Subsystem["+tempSubSystem->subsystemName+"]?","Scripts/Templates won't be deleted",true) == pawgui::display_query_yes )
                    {
                        subSystemToRemove = i;
                        return; // only one thing can be clicked so just exit like a boss :-)
                    }
                    else
                    {
                        if( main_editor_log!=NULL )
                        {
                            main_editor_log->log_build_comment("Subsystem removal process averted by user.");
                        }
                        subSystemToRemove = -1;
                    }
                }
                else if( tempSubSystem->mainProgramFileScriptButton!=NULL && tempSubSystem->mainProgramFileScriptButton->is_clicked()  )
                {
                    std::string newGPEScriptFilename = pawgui::get_filename_open_from_popup("Browse for a GPE Build Script File...","",GPE_CPP_BUILDER_SETTINGS->fileOpenDefaultScriptDir);
                    if( (int)newGPEScriptFilename.size() > 0  && tempSubSystem->mainProgramFileScript!=NULL )
                    {
                        tempSubSystem->mainProgramFileScript->set_string( newGPEScriptFilename );
                    }
                }
                else if( tempSubSystem->systemProgramTemplateFileButton!=NULL && tempSubSystem->systemProgramTemplateFileButton->is_clicked()  )
                {
                    std::string newGPETemplateFilename = pawgui::get_filename_open_from_popup("Browse for a GPE Build Template File...","",GPE_CPP_BUILDER_SETTINGS->fileOpenDefaultTemplateDir);
                    if( (int)newGPETemplateFilename.size() > 0  && tempSubSystem->systemProgramTemplateFile!=NULL )
                    {
                        tempSubSystem->systemProgramTemplateFile->set_string( newGPETemplateFilename );
                    }
                }
            }
        }
    }
}

bool gpeCPPBuildHolder::remove_subsystem_indexed(int subsystemPos, bool resetIdToRemove)
{
    if( resetIdToRemove)
    {
        subSystemToRemove = -1;
    }
    int subSystemCount = (int)mySubsystems.size();
    if( subsystemPos >=0 && subsystemPos < subSystemCount )
    {
        gpeCPPSubsytemHolder * tempSubSystem = mySubsystems[subsystemPos];

        if( tempSubSystem!=NULL)
        {
            mySubsystems.erase( mySubsystems.begin()+ subsystemPos );
            delete tempSubSystem;
            tempSubSystem = NULL;
            return true;
        }
    }
    return false;
}

gameCPPBuilderSettingsResource::gameCPPBuilderSettingsResource(pawgui::widget_resource_container * pFolder)
{
    fileOpenDefaultScriptDir = "";
    fileOpenDefaultTemplateDir = "";
    resourceFileName = "";
    resource_name = "C++ Builder Settings";//C++ Settings
    parentProjectName = "";
    parentProjectDirectory = "";

    cppbuildSystemsLabel = new pawgui::widget_label_title("Build Systems","Your editor's list of build systems");

    cppBuildSystemsDropDown = new pawgui::widget_dropdown_menu("Build Systems...", false );
    addCPPBuildSystemButton = new pawgui::widget_button_label("Add System","Add a new C++ build system" );
    sideAreaPanel = new pawgui::widget_selectbox("Mode");
    sideAreaPanel->set_width(160);
    sideAreaPanel->set_option_height(64);
    sideAreaPanel->add_option("Builder Settings",-1,pawgui::rsm_gui->texture_add( "wrench", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/wrench.png"),NULL,2, false, false);

    sidePanelRect = new gpe::shape_rect();

    editorPageList = new pawgui::widget_panel_list();
    editorPageList->barXPadding = pawgui::padding_default;
    editorPageList->panelAlignType = pawgui::panel_align_right;
    editorPageList->barXMargin = 0;

    subViewedSpace.x = 0;
    subViewedSpace.y = 0;
    subViewedSpace.w = gpe::screen_width;
    subViewedSpace.h = gpe::screen_height;

}

gameCPPBuilderSettingsResource::~gameCPPBuilderSettingsResource()
{

}

gpeCPPBuildHolder * gameCPPBuilderSettingsResource::add_cpp_build_system( std::string newSysName )
{
    //Checks if subsystem already exists with name.
    if( (int)cppBuildSystems.size() > 0  && (int)newSysName.size() > 0 )
    {
        if( find_build_system( newSysName) !=NULL )
        {
            if( main_editor_log!=NULL)
            {
                main_editor_log->log_general_error("CPP Build System["+newSysName+"] name already exists.");
            }
            return NULL;
        }
    }
    gpeCPPBuildHolder * newCPPBuildSystem = new gpeCPPBuildHolder( newSysName );
    cppBuildSystems.push_back(newCPPBuildSystem);
    cppBuildSystemsDropDown->add_menu_option(newSysName,newSysName, -1 ,true );
    return newCPPBuildSystem;
}

void gameCPPBuilderSettingsResource::clear_build_systems()
{
    if( editorPageList!=NULL)
    {
        editorPageList->clear_list();
    }
    if( cppBuildSystemsDropDown!=NULL)
    {
        cppBuildSystemsDropDown->clear_dropdown();
    }
    gpeCPPBuildHolder * tempSys = NULL;
    if( (int)cppBuildSystems.size() > 0 )
    {
        gpeCPPBuildHolder * tempSys = NULL;
        for( int i = (int)cppBuildSystems.size()-1; i >=0; i-- )
        {
            tempSys = cppBuildSystems[i];
            if(tempSys !=NULL)
            {
                delete tempSys;
                tempSys = NULL;
            }
        }
        cppBuildSystems.clear();
    }
}

gpeCPPBuildHolder * gameCPPBuilderSettingsResource::find_build_system( std::string sysName )
{
    if( (int)sysName.size() <=0 )
    {
        return NULL;
    }

    gpeCPPBuildHolder * tempSys = NULL;
    for( int i = (int)cppBuildSystems.size()-1; i >=0; i-- )
    {
        tempSys = cppBuildSystems[i];
        if( tempSys!=NULL && tempSys->builderName == sysName )
        {
            return tempSys;
        }
    }
    return NULL;
}

bool gameCPPBuilderSettingsResource::include_local_files( std::string pBuildDir , int buildType )
{

}

void gameCPPBuilderSettingsResource::prerender_self( )
{

}

void gameCPPBuilderSettingsResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed )
    {
        return;
    }
    resourcePostProcessed = true;
    if( main_editor_log!=NULL )
    {
        main_editor_log->log_general_line("Loading CPP Builder Settings...");
    }
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Processing C++ Builder","Loading systems" );
    }

    std::string otherColContainerName = "";

    std::string newFileIn ="";
    std::string soughtDir =  gpe::get_user_settings_folder();
    if( sff_ex::file_exists(file_path) )
    {
        newFileIn = file_path;
        soughtDir = stg_ex::get_path_from_file(newFileIn);
    }
    else
    {
        newFileIn = soughtDir +"cpp_builder_settings.gpf";
    }
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    //error_log->report("Loading Tilesheet - "+newFileIn);
    if( gameResourceFileIn.fail() )
    {
        return; //failed to open
    }
    //makes sure the file is open
    if ( !gameResourceFileIn.is_open())
    {
        return; //failed to open
    }

    clear_build_systems();

    int equalPos = 0;
    std::string firstChar="";
    std::string keyString="";
    std::string valString="";
    std::string subValString="";
    std::string currLine="";
    std::string currLineToBeProcessed;
    std::string tsDataStr = "";
    float foundFileVersion = 0;

    gpeCPPBuildHolder *cHolder = NULL;
    gpeCPPSubsytemHolder * cSubsystemHolder = NULL;
    int iSys = 0;
    int iSysMax = (int)cppBuildSystems.size();
    int jSubsys = 0;
    int jSubsysMax = 0;
    int i = 0;
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

                    if( keyString=="BuildSystem")
                    {
                        cHolder = add_cpp_build_system( valString);
                        cSubsystemHolder = NULL;
                    }
                    else if( keyString=="BuildSubsystem")
                    {
                        if( cHolder!=NULL)
                        {
                            cSubsystemHolder = cHolder->add_subsystem( valString);
                        }
                    }
                    else if( keyString=="BuildSubSystemScript")
                    {
                        if( cSubsystemHolder!=NULL && cSubsystemHolder->mainProgramFileScript!=NULL )
                        {
                            cSubsystemHolder->mainProgramFileScript->set_string( valString);
                        }
                    }
                    else if( keyString=="BuildSubSystemTemplate")
                    {
                        if( cSubsystemHolder!=NULL && cSubsystemHolder->systemProgramTemplateFile!=NULL )
                        {
                            cSubsystemHolder->systemProgramTemplateFile->set_string( valString);
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
    gameResourceFileIn.close();
    resourcePostProcessed = true;
    if( main_editor_log!=NULL )
    {
        main_editor_log->log_general_line("CPP Builder Settings loaded ("+ stg_ex::int_to_string( (int)cppBuildSystems.size() )+") subsystems...");
    }
}

void gameCPPBuilderSettingsResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    if( cam!=NULL && editorPageList!=NULL && sideAreaPanel!=NULL && view_space!=NULL)
    {
        int prevTab = sideAreaPanel->get_selection();
        if( panel_main_area!=NULL )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w;
            subViewedSpace.h = view_space->h;
            panel_main_area->add_gui_element_fullsize( sideAreaPanel );
            panel_main_area->process_self();
        }
        else
        {
            sideAreaPanel->set_coords(  0,0  );
            sideAreaPanel->set_height( view_space->h );
            sideAreaPanel->process_self( view_space, cam );
            subViewedSpace.x = sideAreaPanel->get_x2();
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w - sideAreaPanel->get_width();
            subViewedSpace.h = view_space->h;
        }

        if( prevTab!=sideAreaPanel->get_selection() )
        {
            editorPageList->reset_self();
        }

        editorPageList->clear_list();
        editorPageList->set_coords( subViewedSpace.x, subViewedSpace.y );
        editorPageList->set_width(subViewedSpace.w);
        editorPageList->set_height(subViewedSpace.h );
        editorPageList->barXPadding = pawgui::padding_default;
        editorPageList->barXMargin = pawgui::padding_default;
        gpeCPPBuildHolder *cTempHolder = NULL;

        editorPageList->add_gui_element(cppbuildSystemsLabel,true );

        int i = 0;
        if( sideAreaPanel->get_selected_name() == "Builder Settings")
        {
            cTempHolder = find_build_system ( cppBuildSystemsDropDown->get_selected_tag() );

            editorPageList->add_gui_element(addCPPBuildSystemButton,false );
            editorPageList->add_gui_element(cppBuildSystemsDropDown,true);

            if( cTempHolder !=NULL )
            {
                cTempHolder->add_to_list( editorPageList );
            }
            editorPageList->add_gui_element(confirmResourceButton,false );
            editorPageList->add_gui_element(cancelResourceButton,true );
            editorPageList->process_self( view_space,cam);

            if( cTempHolder!=NULL)
            {
                cTempHolder->process_self();
            }
            if( addCPPBuildSystemButton!=NULL && addCPPBuildSystemButton->is_clicked() )
            {
                std::string newCPPBuildSystemName = pawgui::get_string_from_popup("Name your new C++ build system","","");
                if( (int)newCPPBuildSystemName.size() > 0 )
                {
                    add_cpp_build_system( newCPPBuildSystemName );
                }
            }
        }
        else
        {
            editorPageList->add_gui_element(confirmResourceButton,false);
            editorPageList->add_gui_element(cancelResourceButton,true);
            editorPageList->process_self( view_space,cam);
        }

        if( confirmResourceButton->is_clicked() )
        {
            save_resource();
        }

        if( cancelResourceButton->is_clicked() )
        {
            load_resource();
        }
    }
}

void gameCPPBuilderSettingsResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    if( cam!=NULL && view_space!=NULL)
    {
        if( sideAreaPanel!=NULL && panel_main_area==NULL )
        {
            sideAreaPanel->render_self( view_space,cam);
        }
        if( editorPageList!=NULL )
        {
            editorPageList->render_self( view_space,cam);
        }
    }
}

void gameCPPBuilderSettingsResource::save_resource(std::string file_path, int backupId )
{
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Saving C++ Settings", "Builder Systems" );
    }
    std::string newSaveDataFilename = "";
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
        soughtDir =  gpe::get_user_settings_folder();
        newFileOut = soughtDir + "cpp_builder_settings.gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( newSaveDataFile.fail() )
    {
        newSaveDataFile.close();
        main_editor_log->log_general_error("Failed to begin saving to file ["+newFileOut+"]");

        return;
    }
    //makes sure the file is open
    if( !newSaveDataFile.is_open() )
    {
        main_editor_log->log_general_error("Unable to save to file ["+newFileOut+"]");
        return;
    }

    write_header_on_file(&newSaveDataFile);
    gpeCPPBuildHolder *cHolder = NULL;
    gpeCPPSubsytemHolder * cSubsystemHolder = NULL;
    int iSys = 0;
    int iSysMax = (int)cppBuildSystems.size();
    int jSubsys = 0;
    int jSubsysMax = 0;
    std::string cSystemName = "";
    std::string cSubsystemName = "";
    bool validSystemName = false;
    bool validSubSystemName = false;
    for( iSys = 0; iSys < iSysMax; iSys++)
    {
        cHolder = cppBuildSystems[iSys];
        if( cHolder !=NULL )
        {
            if( cHolder->builderNameField!=NULL )
            {
                cSystemName = cHolder->builderNameField->get_string();
                if( (int)cSystemName.size() > 0 )
                {
                    newSaveDataFile << "BuildSystem=" << cSystemName << "\n";
                    validSystemName = true;
                }
            }
            if( !validSystemName )
            {
                newSaveDataFile << "BuildSystem=" << cHolder->builderName << "\n";
            }
            validSystemName = false;

            jSubsysMax = (int)cHolder->mySubsystems.size();
            for( jSubsys = 0; jSubsys < jSubsysMax; jSubsys++)
            {
                cSubsystemHolder = cHolder->mySubsystems[ jSubsys];
                if( cSubsystemHolder !=NULL )
                {
                    if( cSubsystemHolder->systemNameField!=NULL )
                    {
                        cSubsystemName = cSubsystemHolder->systemNameField->get_string();
                        if( (int)cSubsystemName.size() > 0 )
                        {
                            newSaveDataFile << "BuildSubsystem=" << cSubsystemName << "\n";
                            validSubSystemName = true;
                        }
                    }
                    if( !validSubSystemName )
                    {
                        newSaveDataFile << "BuildSubsystem=" << cSubsystemHolder->subsystemName << "\n";
                    }
                    validSubSystemName = false;

                    if( cSubsystemHolder->mainProgramFileScript!=NULL)
                    {
                        newSaveDataFile << "BuildSubSystemScript=" << cSubsystemHolder->mainProgramFileScript->get_string() << "\n";

                    }
                    if( cSubsystemHolder->systemProgramTemplateFile!=NULL)
                    {
                        newSaveDataFile << "BuildSubSystemTemplate=" << cSubsystemHolder->systemProgramTemplateFile->get_string() << "\n";
                    }
                }
            }
        }
    }
    newSaveDataFile.close();
}

bool gameCPPBuilderSettingsResource::write_cpp_header_file(std::ofstream * fileTarget,std::string fName)
{
    if( fileTarget==NULL)
    {
        return false;
    }
    if( !fileTarget->is_open() )
    {
        return false;
    }
    *fileTarget << "/* \n";
    *fileTarget << fName+" \n";
    *fileTarget << "This file is auto-generated by: \n";
    *fileTarget << "GAME PENCIL ENGINE's Editor \n";
    *fileTarget << "Version "+ stg_ex::float_to_string( gpe::version_number_total)+" \n";
    *fileTarget << "https://www.pawbyte.com/gamepencilengine \n";
    *fileTarget << "Copyright (c) 2014-2020 Nathan Hurde, Chase Lee. \n";
    *fileTarget << "\n";
    *fileTarget << "Copyright (c) 2014-2020 PawByte. \n";
    *fileTarget << "Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page ) \n";
    *fileTarget << "\n";
    *fileTarget << "Permission is hereby granted, free of charge, to any person obtaining a copy \n";
    *fileTarget << "of this software and associated documentation files (the “Software”), to deal \n";
    *fileTarget << "in the Software without restriction, including without limitation the rights \n";
    *fileTarget << "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell \n";
    *fileTarget << "copies of the Software, and to permit persons to whom the Software is \n";
    *fileTarget << "furnished to do so, subject to the following conditions: \n";

    *fileTarget << "The above copyright notice and this permission notice shall be included in all \n";
    *fileTarget << "copies or substantial portions of the Software. \n";
    *fileTarget << "\n";
    *fileTarget << "THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR \n";
    *fileTarget << "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \n";
    *fileTarget << "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE \n";
    *fileTarget << "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER \n";
    *fileTarget << "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, \n";
    *fileTarget << "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE \n";
    *fileTarget << "SOFTWARE. \n";

    *fileTarget << "-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine> \n";
    *fileTarget << "\n";
    *fileTarget << "\n";
    *fileTarget << "*/ \n";
}

bool gameCPPBuilderSettingsResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{

}
