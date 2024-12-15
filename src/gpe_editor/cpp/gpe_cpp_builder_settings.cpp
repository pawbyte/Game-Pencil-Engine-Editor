/*
gpe_cpp_builder_settings.cpp
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

#include "gpe_cpp_builder_settings.h"

gameCPPBuilder_settingsResource * GPE_CPP_BUILDER_SETTINGS = nullptr;

gpeCPPSubsytemHolder::gpeCPPSubsytemHolder( std::string gpeSubSystemName )
{
    subsystemName = gpeSubSystemName;
    systemNameField = new pawgui::widget_input_text(gpeSubSystemName,"ie: visualstudio,codeblocks,cmake");
    systemNameLabel = new pawgui::widget_label_text ("Build Subsystem Name:","subsystem used within project build phase");
    removeSystem_button = new pawgui::widget_button_label("Remove build subsystem","deletes build subsystem");

    systemProgramScriptLabel = new pawgui::widget_label_text ("Subsystem GPE Script:","Used to auto-generate file");
    mainProgramFileScript = new pawgui::widget_input_text("","Subsystem GPE file script");
    mainProgramFileScript_button = new pawgui::widget_button_label("Browse...","Search for gpe substem build script...");

    systemProgramTemplateFileLabel = new pawgui::widget_label_text ("Subsystem GPE File Template:","Template file copied in auto-generation");
    systemProgramTemplateFile = new pawgui::widget_input_text("","Subsystem GPE template file");
    systemProgramTemplateFile_button = new pawgui::widget_button_label("Browse...","Search for gpe substem template file...");
}

gpeCPPSubsytemHolder::~gpeCPPSubsytemHolder()
{

}

bool gpeCPPSubsytemHolder::add_to_list(pawgui::widget_panel_list * listIn)
{
    if( listIn ==nullptr)
    return false;
    listIn->add_gui_element( systemNameLabel , false );
    listIn->add_gui_element( systemNameField , true );

    listIn->add_gui_element( removeSystem_button , true );


    listIn->add_gui_element( systemProgramScriptLabel , true );
    listIn->add_gui_element( mainProgramFileScript_button , false );
    listIn->add_gui_element( mainProgramFileScript , true );

    listIn->add_gui_element( systemProgramTemplateFileLabel , true );
    listIn->add_gui_element( systemProgramTemplateFile_button , false );
    listIn->add_gui_element( systemProgramTemplateFile , true );

    return true;
}

gpeCPPBuildHolder::gpeCPPBuildHolder(std::string buildSystemName)
{
    subSystemToRemove = -1; //keep negative to avoid deletions
    builderName = buildSystemName;
    builderNameField = new pawgui::widget_input_text(buildSystemName,"ie: Visual Studio, CodeBlocks, DevC++, Premake5...");
    builderNameLabel = new pawgui::widget_label_text ("Builder System Name:");
    addSubSystem_button = new pawgui::widget_button_label("Add subsystem","Add a new subsystem for C++ builder");
    buildSystemOS = new pawgui::widget_dropdown_menu("Build Platform", false );
}

gpeCPPBuildHolder::~gpeCPPBuildHolder()
{
    if( builderNameField!=nullptr)
    {
        delete builderNameField;
        builderNameField = nullptr;
    }

    if( builderNameLabel!=nullptr)
    {
        delete builderNameLabel;
        builderNameLabel = nullptr;
    }
    if( buildSystemOS!=nullptr)
    {
        delete buildSystemOS;
        buildSystemOS = nullptr;
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
    if( listIn ==nullptr)
    return false;
    listIn->add_gui_element( builderNameLabel, true);
    listIn->add_gui_element( builderNameField, true);
    listIn->add_gui_element( buildSystemOS, true);
    listIn->add_gui_element( addSubSystem_button, true);

    int subSystemCount = (int)mySubsystems.size();
    if( subSystemCount > 0 )
    {
        gpeCPPSubsytemHolder * tempSubSystem = nullptr;
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
        if( find_sub_system( newSysName) !=nullptr )
        {
            if( main_editor_log!=nullptr)
            {
                main_editor_log->log_general_error("Subsystem["+newSysName+"] name already used for ["+builderName+"] build system.");
            }
            return nullptr;
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
        gpeCPPSubsytemHolder * tempSys = nullptr;
        for( int i = (int)mySubsystems.size()-1; i >=0; i-- )
        {
            tempSys = mySubsystems[i];
            if(tempSys !=nullptr)
            {
                delete tempSys;
                tempSys = nullptr;
            }
        }
        mySubsystems.clear();
        return true; //systems deleted
    }
    return false;
}

gpeCPPSubsytemHolder * gpeCPPBuildHolder::find_sub_system( std::string sysName )
{
    gpeCPPSubsytemHolder * tempSys = nullptr;
    for( int i = (int)mySubsystems.size()-1; i >=0; i-- )
    {
        tempSys = mySubsystems[i];
        if( tempSys!=nullptr )
        {
            if( tempSys->subsystemName == sysName )
            {
                return tempSys;
            }
        }
    }
    return nullptr;
}

void gpeCPPBuildHolder::process_self()
{
    if( addSubSystem_button->is_clicked() )
    {
        std::string newSubSystemName = pawgui::get_string_from_popup("Name your new subsystem","","");
        if( (int)newSubSystemName.size() > 0 )
        {
            add_subsystem( newSubSystemName );
        }
        else if( main_editor_log!=nullptr)
        {
            main_editor_log->log_general_error("["+builderName+"] build system add_subsystem exited, (Name not entered).");
        }
    }

    //Process the subsystems
    int subSystemCount = (int)mySubsystems.size();
    if( subSystemCount > 0 )
    {
        gpeCPPSubsytemHolder * tempSubSystem = nullptr;
        for( int i = subSystemCount -1; i >=0; i-- )
        {
            tempSubSystem = mySubsystems[i];
            if( tempSubSystem!=nullptr )
            {
                if( tempSubSystem->removeSystem_button!=nullptr && tempSubSystem->removeSystem_button->is_clicked()  )
                {
                    if( pawgui::display_prompt_message("Are you sure you want to delete Subsystem["+tempSubSystem->subsystemName+"]?","Scripts/Templates won't be deleted",true) == pawgui::display_query_yes )
                    {
                        subSystemToRemove = i;
                        return; // only one thing can be clicked so just exit like a boss :-)
                    }
                    else
                    {
                        if( main_editor_log!=nullptr )
                        {
                            main_editor_log->log_build_comment("Subsystem removal process averted by user.");
                        }
                        subSystemToRemove = -1;
                    }
                }
                else if( tempSubSystem->mainProgramFileScript_button!=nullptr && tempSubSystem->mainProgramFileScript_button->is_clicked()  )
                {
                    std::string newGPEScriptFilename = pawgui::get_filename_open_from_popup("Browse for a GPE Build Script File...","",GPE_CPP_BUILDER_SETTINGS->fileOpenDefaultScriptDir);
                    if( (int)newGPEScriptFilename.size() > 0  && tempSubSystem->mainProgramFileScript!=nullptr )
                    {
                        tempSubSystem->mainProgramFileScript->set_string( newGPEScriptFilename );
                    }
                }
                else if( tempSubSystem->systemProgramTemplateFile_button!=nullptr && tempSubSystem->systemProgramTemplateFile_button->is_clicked()  )
                {
                    std::string newGPETemplateFilename = pawgui::get_filename_open_from_popup("Browse for a GPE Build Template File...","",GPE_CPP_BUILDER_SETTINGS->fileOpenDefaultTemplateDir);
                    if( (int)newGPETemplateFilename.size() > 0  && tempSubSystem->systemProgramTemplateFile!=nullptr )
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

        if( tempSubSystem!=nullptr)
        {
            mySubsystems.erase( mySubsystems.begin()+ subsystemPos );
            delete tempSubSystem;
            tempSubSystem = nullptr;
            return true;
        }
    }
    return false;
}

gameCPPBuilder_settingsResource::gameCPPBuilder_settingsResource(pawgui::widget_resource_container * pFolder)
{
    fileOpenDefaultScriptDir = "";
    fileOpenDefaultTemplateDir = "";
    resourceFileName = "";
    resource_name = "C++ Builder Settings";//C++ _settings
    parentProjectName = "";
    parentProjectDirectory = "";

    cppbuildSystemsLabel = new pawgui::widget_label_title("Build Systems","Your editor's list of build systems");

    cppBuildSystemsDropDown = new pawgui::widget_dropdown_menu("Build Systems...", false );
    addCPPBuildSystem_button = new pawgui::widget_button_label("Add System","Add a new C++ build system" );
    sideAreaPanel = new pawgui::widget_selectbox("Mode");
    sideAreaPanel->set_width(160);
    sideAreaPanel->set_option_height(64);
    sideAreaPanel->add_option("Builder Settings",-1,pawgui::rsm_gui->texture_add( "wrench", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/wrench.png"),nullptr,2, false, false);

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

gameCPPBuilder_settingsResource::~gameCPPBuilder_settingsResource()
{

}

gpeCPPBuildHolder * gameCPPBuilder_settingsResource::add_cpp_build_system( std::string newSysName )
{
    //Checks if subsystem already exists with name.
    if( (int)cppBuildSystems.size() > 0  && (int)newSysName.size() > 0 )
    {
        if( find_build_system( newSysName) !=nullptr )
        {
            if( main_editor_log!=nullptr)
            {
                main_editor_log->log_general_error("CPP Build System["+newSysName+"] name already exists.");
            }
            return nullptr;
        }
    }
    gpeCPPBuildHolder * newCPPBuildSystem = new gpeCPPBuildHolder( newSysName );
    cppBuildSystems.push_back(newCPPBuildSystem);
    cppBuildSystemsDropDown->add_menu_option(newSysName,newSysName, -1 ,true );
    return newCPPBuildSystem;
}

void gameCPPBuilder_settingsResource::clear_build_systems()
{
    if( editorPageList!=nullptr)
    {
        editorPageList->clear_list();
    }
    if( cppBuildSystemsDropDown!=nullptr)
    {
        cppBuildSystemsDropDown->clear_dropdown();
    }
    gpeCPPBuildHolder * tempSys = nullptr;
    if( (int)cppBuildSystems.size() > 0 )
    {
        gpeCPPBuildHolder * tempSys = nullptr;
        for( int i = (int)cppBuildSystems.size()-1; i >=0; i-- )
        {
            tempSys = cppBuildSystems[i];
            if(tempSys !=nullptr)
            {
                delete tempSys;
                tempSys = nullptr;
            }
        }
        cppBuildSystems.clear();
    }
}

gpeCPPBuildHolder * gameCPPBuilder_settingsResource::find_build_system( std::string sysName )
{
    if( (int)sysName.size() <=0 )
    {
        return nullptr;
    }

    gpeCPPBuildHolder * tempSys = nullptr;
    for( int i = (int)cppBuildSystems.size()-1; i >=0; i-- )
    {
        tempSys = cppBuildSystems[i];
        if( tempSys!=nullptr && tempSys->builderName == sysName )
        {
            return tempSys;
        }
    }
    return nullptr;
}

bool gameCPPBuilder_settingsResource::include_local_files( std::string pBuildDir , int buildType )
{
    return false; ////WIPNOTFUNCTIONALYET
}

void gameCPPBuilder_settingsResource::prerender_self( )
{

}

void gameCPPBuilder_settingsResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed )
    {
        return;
    }
    resourcePostProcessed = true;
    if( main_editor_log!=nullptr )
    {
        main_editor_log->log_general_line("Loading CPP Builder _settings...");
    }
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Processing C++ Builder","Loading systems" );
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
    std::string key_string="";
    std::string valstring="";
    std::string subValstring="";
    std::string currLine="";
    std::string currLineToBeProcessed;
    std::string tsDataStr = "";
    float foundFileVersion = 0;

    gpeCPPBuildHolder *cHolder = nullptr;
    gpeCPPSubsytemHolder * cSubsystemHolder = nullptr;
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

                    if( key_string=="BuildSystem")
                    {
                        cHolder = add_cpp_build_system( valstring);
                        cSubsystemHolder = nullptr;
                    }
                    else if( key_string=="BuildSubsystem")
                    {
                        if( cHolder!=nullptr)
                        {
                            cSubsystemHolder = cHolder->add_subsystem( valstring);
                        }
                    }
                    else if( key_string=="BuildSubSystemScript")
                    {
                        if( cSubsystemHolder!=nullptr && cSubsystemHolder->mainProgramFileScript!=nullptr )
                        {
                            cSubsystemHolder->mainProgramFileScript->set_string( valstring);
                        }
                    }
                    else if( key_string=="BuildSubSystemTemplate")
                    {
                        if( cSubsystemHolder!=nullptr && cSubsystemHolder->systemProgramTemplateFile!=nullptr )
                        {
                            cSubsystemHolder->systemProgramTemplateFile->set_string( valstring);
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
    if( main_editor_log!=nullptr )
    {
        main_editor_log->log_general_line("CPP Builder _settings loaded ("+ stg_ex::int_to_string( (int)cppBuildSystems.size() )+") subsystems...");
    }
}

void gameCPPBuilder_settingsResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    if( cam!=nullptr && editorPageList!=nullptr && sideAreaPanel!=nullptr && view_space!=nullptr)
    {
        int prevTab = sideAreaPanel->get_selection();
        if( panel_main_editor!=nullptr )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w;
            subViewedSpace.h = view_space->h;
            panel_main_editor->add_gui_element_fullsize( sideAreaPanel );
            panel_main_editor->process_self();
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
        gpeCPPBuildHolder *cTempHolder = nullptr;

        editorPageList->add_gui_element(cppbuildSystemsLabel,true );

        int i = 0;
        if( sideAreaPanel->get_selected_name() == "Builder Settings")
        {
            cTempHolder = find_build_system ( cppBuildSystemsDropDown->get_selected_tag() );

            editorPageList->add_gui_element(addCPPBuildSystem_button,false );
            editorPageList->add_gui_element(cppBuildSystemsDropDown,true);

            if( cTempHolder !=nullptr )
            {
                cTempHolder->add_to_list( editorPageList );
            }
            editorPageList->add_gui_element(confirmResource_button,false );
            editorPageList->add_gui_element(cancelResource_button,true );
            editorPageList->process_self( view_space,cam);

            if( cTempHolder!=nullptr)
            {
                cTempHolder->process_self();
            }
            if( addCPPBuildSystem_button!=nullptr && addCPPBuildSystem_button->is_clicked() )
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
            editorPageList->add_gui_element(confirmResource_button,false);
            editorPageList->add_gui_element(cancelResource_button,true);
            editorPageList->process_self( view_space,cam);
        }

        if( confirmResource_button->is_clicked() )
        {
            save_resource();
        }

        if( cancelResource_button->is_clicked() )
        {
            load_resource();
        }
    }
}

void gameCPPBuilder_settingsResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    if( cam!=nullptr && view_space!=nullptr)
    {
        if( sideAreaPanel!=nullptr && panel_main_editor==nullptr )
        {
            sideAreaPanel->render_self( view_space,cam);
        }
        if( editorPageList!=nullptr )
        {
            editorPageList->render_self( view_space,cam);
        }
    }
}

void gameCPPBuilder_settingsResource::save_resource(std::string file_path, int backupId )
{
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Saving C++ Settings", "Builder Systems" );
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
    gpeCPPBuildHolder *cHolder = nullptr;
    gpeCPPSubsytemHolder * cSubsystemHolder = nullptr;
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
        if( cHolder !=nullptr )
        {
            if( cHolder->builderNameField!=nullptr )
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
                if( cSubsystemHolder !=nullptr )
                {
                    if( cSubsystemHolder->systemNameField!=nullptr )
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

                    if( cSubsystemHolder->mainProgramFileScript!=nullptr)
                    {
                        newSaveDataFile << "BuildSubSystemScript=" << cSubsystemHolder->mainProgramFileScript->get_string() << "\n";

                    }
                    if( cSubsystemHolder->systemProgramTemplateFile!=nullptr)
                    {
                        newSaveDataFile << "BuildSubSystemTemplate=" << cSubsystemHolder->systemProgramTemplateFile->get_string() << "\n";
                    }
                }
            }
        }
    }
    newSaveDataFile.close();
}

bool gameCPPBuilder_settingsResource::write_cpp_header_file(std::ofstream * fileTarget,std::string f_name)
{
    if( fileTarget==nullptr)
    {
        return false;
    }
    if( !fileTarget->is_open() )
    {
        return false;
    }
    *fileTarget << "/* \n";
    *fileTarget << f_name+" \n";
    *fileTarget << "This file is auto-generated by: \n";
    *fileTarget << "GAME PENCIL ENGINE's Editor \n";
    *fileTarget << "Version "+ stg_ex::float_to_string( gpe::version_number_total)+" \n";
    *fileTarget << "https://www.pawbyte.com/gamepencilengine \n";
    *fileTarget << "Copyright (c) 2014-2024 Nathan Hurde, Chase Lee. \n";
    *fileTarget << "\n";
    *fileTarget << "Copyright (c) 2014-2024 PawByte. \n";
    *fileTarget << "Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page ) \n";
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
    return true;
}

bool gameCPPBuilder_settingsResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{
    return true; //WIPNOTFUNCTIONALYET
}
