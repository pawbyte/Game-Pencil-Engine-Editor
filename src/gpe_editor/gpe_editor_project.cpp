/*
gpe_editor_project.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_editor_project.h"
#include "gpe_project_resources.h"
#include "project_properties.h"

GPE_ProjectFolder * current_project = nullptr;

GPE_ProjectFolder::GPE_ProjectFolder(std::string name, std::string directory_string, std::string new_file_name, std::string projectLanguageIn, bool createBlankScene )
{
    projectRSM = new gpe::asset_manager(gpe::rph->get_default_render_package(), "new_file_name");
    int iLayerN = 0;
    for( iLayerN = 0; iLayerN < 32; iLayerN++)
    {
        projectLayerNames[iLayerN] = "";
    }

    projectLayerNames[0] = "Default Background Layer";
    projectLayerNames[1] = "Default Object Layer";
    for( iLayerN = 2; iLayerN < 24; iLayerN++)
        for( iLayerN = 2; iLayerN < 24; iLayerN++)
        {
            projectLayerNames[iLayerN] = "Custom Layer"+ stg_ex::int_to_string(iLayerN-2);
        }
    for( iLayerN = 24; iLayerN < 32; iLayerN++)
    {
        projectLayerNames[iLayerN] = "Default Tile Layer"+ stg_ex::int_to_string(iLayerN-24);
    }


    //Defaults to JS Project Language
    myProjectLanguage = pawgui::program_language_js;

    //Changes Project Language if name is found...
    if( (int)projectLanguageIn.size() > 0 )
    {
        pawgui::syntax_language * tLanguage  = nullptr;
        int maxLanguageCount = pawgui::main_syntax_highlighter->get_language_count();
        for( int cLanguage = 0; cLanguage < maxLanguageCount; cLanguage++ )
        {
            tLanguage = pawgui::main_syntax_highlighter->get_language_object(cLanguage);
            //checks if the found lanuage isnt nullptr and is a coding language
            if( tLanguage!=nullptr && tLanguage->isCodingLanguage)
            {
                //if its regular name or short name matches. We end loop and change our project language.
                if( projectLanguageIn == tLanguage->languageName || projectLanguageIn == tLanguage->languageShortName )
                {
                    myProjectLanguage = cLanguage;
                    break;
                }
            }
        }
    }
    time( &lastTimeBackedUp);
    GLOBAL_REZ_ID_COUNT = 0;
    projectName = name;
    projectStartDirectoryName = projectDirectory = directory_string;
    projectFileName = new_file_name;
    RESC_PROJECT_FOLDER = nullptr;
    RESC_ANIMATIONS = nullptr;
    RESC_TEXTURES = nullptr;
    RESC_TILESHEETS = nullptr;
    RESC_3DMODELS = nullptr;
    RESC_AUDIO = nullptr;
    RESC_VIDEOS = nullptr;
    RESC_FUNCTIONS = nullptr;
    RESC_ENTITIES = nullptr;
    RESC_CLASSES = nullptr;
    RESC_PATHS = nullptr;
    RESC_SCENES = nullptr;
    RESC_ACHIEVEMENTS = nullptr;
    RESC_FONTS = nullptr;
    RESC_SHADERS = nullptr;
    for( int i = 0; i < gpe::resource_type_max; i++)
    {
        RESC_ALL[i] = nullptr;
        CREATED_RESOURCE_COUNT[i] = 0;
    }
    RESC_PROJECT_FOLDER = new pawgui::widget_resource_container(projectFileName,projectName,pawgui::restype_projfolder, -1,true, increment_resouce_count(), pawgui::restype_projfolder);
    RESC_PROJECT_FOLDER->parentProjectDirectory = projectDirectory;
    RESC_PROJECT_FOLDER->projectParentFileName = projectFileName;
    RESC_PROJECT_FOLDER->open_folder();

    RESC_TEXTURES =  RESC_ALL[ gpe::resource_type_texture] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_texture,"Textures", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_TILESHEETS =  RESC_ALL[ gpe::resource_type_tilesheet] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_tilesheet,"Tilesheets", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_ANIMATIONS =  RESC_ALL[ gpe::resource_type_animation] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_animation,"Animations", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_AUDIO =  RESC_ALL[ gpe::resource_type_audio] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_audio,"Audio", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_VIDEOS =  RESC_ALL[ gpe::resource_type_video] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_video,"Videos", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_FUNCTIONS =  RESC_ALL[ gpe::resource_type_function] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_function,"Functions", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_PATHS =  RESC_ALL[ gpe::resource_type_path] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_path,"Paths", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_EMITTERS =  RESC_ALL[ gpe::resource_type_emitter] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_emitter,"Particles", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_LIGHTS =  RESC_ALL[ gpe::resource_type_light] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_light,"Lights", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_ENTITIES =  RESC_ALL[ gpe::resource_type_object] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_object,"Entities", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_CLASSES =  RESC_ALL[ gpe::resource_type_class] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_class,"Classes", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_SCENES =  RESC_ALL[ gpe::resource_type_scene] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_scene,"Scenes", increment_resouce_count(), pawgui::restype_superfolder);
    //RESC_ACHIEVEMENTS =  RESC_ALL[ gpe::resource_type_achievement] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_achievement,"Achievements", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_FONTS =  RESC_ALL[ gpe::resource_type_font] =RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_font,"Fonts", increment_resouce_count(), pawgui::restype_superfolder);
    RESC_DICTIONARIES =  RESC_ALL[ gpe::resource_type_dictionary]  = nullptr;//=RESC_PROJECT_FOLDER->add_newtype_folder( gpe::resource_type_dictionary,"Dictionaries", increment_resouce_count(), pawgui::restype_superfolder);

    int projectPropertiesId = increment_resouce_count();
    RESC_PROJECT_SETTINGS = new pawgui::widget_resource_container(projectFileName, gpe::resource_type_names[ gpe::resource_type_project_settings], gpe::resource_type_project_settings,projectPropertiesId,false, projectPropertiesId, -1);
    RESC_PROJECT_SETTINGS->parentProjectDirectory = projectDirectory;

    RESC_PROJECT_FOLDER->add_resource_container(RESC_PROJECT_SETTINGS,  false );
    projectPropertiesResource * projectProps = new projectPropertiesResource(RESC_PROJECT_FOLDER);
    projectProps->set_name( gpe::resource_type_names[ gpe::resource_type_project_settings] );
    projectProps->set_parent_project( projectFileName );
    RESC_PROJECT_SETTINGS->set_held_resource(projectProps);

    //DO NOT REMOVE THIS LINE, could create instability and incompability. IDs 0 through 999 are reserved for key folders/resource types see ids above ^^^
    GLOBAL_REZ_ID_COUNT = 1000; //After we create the project, all remaining resources ids will begin at 1000 and above

    projectFilePreviousVersion = projectFileVersion = gpe::version_number_total;
    if( createBlankScene )
    {
        create_blank_scene(RESC_SCENES,"start_scene");
    }
}

GPE_ProjectFolder::~GPE_ProjectFolder()
{
    gpe::error_log->report("Deleting RESC_PROJECT_FOLDER" );
    if( RESC_PROJECT_FOLDER!=nullptr )
    {
        delete RESC_PROJECT_FOLDER;
        RESC_PROJECT_FOLDER = nullptr;
    }

    if( projectRSM!=nullptr )
    {
        delete projectRSM;
        projectRSM = nullptr;
    }
}

bool GPE_ProjectFolder::add_project_function(std::string nName, std::string nDescription,std::string  nParameters, std::string tFunctionReturnType,std::string nScope)
{
    pawgui::syntax_compiler_term * tTerm = nullptr;
    for( int i = (int)projectFunctions.size()-1; i>=0; i--)
    {
        tTerm = projectFunctions[i];
        if( tTerm!=nullptr)
        {
            if( tTerm->termstring==nName)
            {
                return false;
            }
        }
    }
    tTerm = new pawgui::syntax_compiler_term(nName, nParameters, tFunctionReturnType, nDescription, pawgui::cterm_function,nScope);
    projectFunctions.push_back( tTerm);
    return true;
}

bool GPE_ProjectFolder::add_project_keyword(std::string nName, std::string nDescription,int nType,std::string nScope)
{
    pawgui::syntax_compiler_term * tTerm = nullptr;
    for( int i = (int)projectKeywords.size()-1; i>=0; i--)
    {
        tTerm = projectKeywords[i];
        if( tTerm!=nullptr)
        {
            if( tTerm->termstring==nName)
            {
                return false;
            }
        }
    }
    tTerm = new pawgui::syntax_compiler_term(nName, "", "", nDescription, nType,nScope);
    projectKeywords.push_back( tTerm);
    return true;
}

bool GPE_ProjectFolder::check_names_against_keywords( pawgui::widget_resource_container * resContainer )
{
    if( resContainer == nullptr)
    {
        return true;
    }
    bool allChecksCleared = true;

    if( resContainer->is_super_project_folder()== false && resContainer->is_folder() == false )
    {
        if( stg_ex::is_alnum( resContainer->get_name(),false,true ) )
        {
            if( !pawgui::main_syntax_highlighter->is_compilable_word( resContainer->get_name() ) )
            {
                main_editor_log->log_build_error("Invalid Resource Name Given ["+resContainer->get_name()+"] conflicts with the GPE system variable. Please rename your resource!");
                allChecksCleared = false;
            }
        }
        else
        {
            allChecksCleared = false;
            main_editor_log->log_build_error("Invalid Resource Name Given ["+resContainer->get_name()+"].");
        }
    }
    else
    {
        main_editor_log->log_build_comment("Checking ["+resContainer->get_name()+"] names.");
    }

    pawgui::widget_resource_container * tempContainer = nullptr;
    int resLimit = resContainer->get_size();
    for( int iRes = 0; iRes < resLimit; iRes++)
    {
        tempContainer = resContainer->get_resource_at( iRes );
        if( tempContainer!=nullptr)
        {
            if( check_names_against_keywords(tempContainer) == false )
            {
                allChecksCleared = false;
            }
        }
    }
    return allChecksCleared;
}

void GPE_ProjectFolder::clear_project_functions()
{
    pawgui::syntax_compiler_term * tTerm = nullptr;
    for( int i = (int)projectFunctions.size()-1; i>=0; i--)
    {
        tTerm = projectFunctions[i];
        if( tTerm!=nullptr)
        {
            delete tTerm;
            tTerm = nullptr;
        }
    }
    projectFunctions.clear();
}

void GPE_ProjectFolder::clear_project_keywords()
{
    pawgui::syntax_compiler_term * tTerm = nullptr;
    for( int i = (int)projectKeywords.size()-1; i>=0; i--)
    {
        tTerm = projectKeywords[i];
        if( tTerm!=nullptr)
        {
            delete tTerm;
            tTerm = nullptr;
        }
    }
    projectKeywords.clear();
}

bool GPE_ProjectFolder::export_and_play_native(bool launchProgram)
{
    if( RESC_PROJECT_SETTINGS!=nullptr && RESC_PROJECT_SETTINGS->get_held_resource()!=nullptr)
    {
        projectPropertiesResource * project_settingsObject = (projectPropertiesResource*)RESC_PROJECT_SETTINGS->get_held_resource();
        return project_settingsObject->export_and_play_native(launchProgram);
    }
    return false;
}

bool GPE_ProjectFolder::clean_build_folder( int buildMetaTemplate )
{
    std::string projectBuildDirectory = "";
    std::string folderDeletionName  = "";
    if( buildMetaTemplate == gpe::system_os_html5 )
    {
        projectBuildDirectory  = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/html5";
        folderDeletionName = "HTML5";
    }
    else if( buildMetaTemplate == gpe::system_os_windows)
    {
        projectBuildDirectory  = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/windows";
        folderDeletionName = "WINDOWS";
    }
    else if( buildMetaTemplate == gpe::system_os_linux)
    {
        projectBuildDirectory  = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/linux";
        folderDeletionName = "LINUX";
    }
    else if( buildMetaTemplate == gpe::system_os_mac)
    {
        projectBuildDirectory  = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/osx";
        folderDeletionName = "OSX";
    }
    if( (int)projectBuildDirectory.size() > 0 )
    {
        if( sff_ex::path_exists(projectBuildDirectory) )
        {
            if( pawgui::display_prompt_message("[WARNING]Function deletion of "+folderDeletionName+" build folder?","Are you sure you will like to delete all the contents of this build directory? This action is irreversible!")==pawgui::display_query_yes)
            {
                main_editor_log->log_build_line("---");
                main_editor_log->log_build_line("Cleaning Project ["+projectName+" build folder:");
                main_editor_log->log_build_line("["+projectBuildDirectory+"]...");
                int filesDeleted =  gpe::clean_folder(projectBuildDirectory);
                if( filesDeleted > 0)
                {
                    main_editor_log->log_build_line("Folder content files were deleted successfully. Estimated ["+ stg_ex::int_to_string(filesDeleted)+"] files deleted...");
                }
                else
                {
                    main_editor_log->log_build_line("0 files were prepared for deleted.");
                }
                main_editor_log->log_build_line("---");
                return filesDeleted;
            }
        }
        else if( main_editor_log!=nullptr )
        {
            main_editor_log->log_build_line("---");
            main_editor_log->log_build_line("Cleaning Project ["+projectName+" build folder:");
            main_editor_log->log_build_line("["+projectBuildDirectory+"] was not detected.");
            main_editor_log->log_build_line("Cleaning process aborted.");
        }
    }

    return false;
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_folder(pawgui::widget_resource_container * folderContainer, std::string new_name, int newResId )
{
    if( folderContainer!=nullptr )
    {
        if( newResId < 0)
        {
            newResId = increment_resouce_count();
        }
        if( (int)new_name.size() <= 0)
        {
            new_name = "New Folder";
        }
        pawgui::widget_resource_container *  newFolder= folderContainer->add_resource_folder(new_name,newResId,-1);
        newFolder->projectParentFileName = projectFileName;
        //folderContainer->open_folder();
        return newFolder;
    }
    return nullptr;
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_resource(int rNewType, pawgui::widget_resource_container * folderContainer, std::string new_name, int newResId )
{
    if( rNewType >= 0 && rNewType <= gpe::resource_type_achievement )
    {
        pawgui::widget_resource_container * RES_FOLDER_HOLDER = RESC_ALL[rNewType];
        if(RES_FOLDER_HOLDER!=nullptr)
        {
            CREATED_RESOURCE_COUNT[rNewType]+=1;
            int resourceNumb = CREATED_RESOURCE_COUNT[rNewType];
            if( (int)new_name.size() == 0)
            {
                new_name = "new"+gpe::resource_type_names[rNewType]+ stg_ex::int_to_string(resourceNumb );
            }
            standardEditableGameResource * newProjectResource;
            switch( rNewType)
            {
                case gpe::resource_type_audio:
                    newProjectResource = new audioResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_video:
                    newProjectResource = new videoResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_font:
                    newProjectResource = new fontResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_function:
                    newProjectResource = new functionResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_class:
                    newProjectResource = new classResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_light:
                    newProjectResource = new lightResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_object:
                    newProjectResource = new gameEntityResource(RESC_PROJECT_FOLDER);
                break;
                //Added since Version 1.12 [BEGIN]
                case gpe::resource_type_path:
                    newProjectResource = new gamePathResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_emitter:
                    newProjectResource = new particleResource(RESC_PROJECT_FOLDER);
                break;
                //Added since Version 1.12 [END]
                case gpe::resource_type_scene:
                    newProjectResource = new gameSceneResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_animation:
                    newProjectResource = new animationResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_texture:
                    newProjectResource = new textureResource(RESC_PROJECT_FOLDER);
                break;
                case gpe::resource_type_tilesheet:
                    newProjectResource = new tilesheetResource(RESC_PROJECT_FOLDER);
                break;
                default:
                    newProjectResource = nullptr;
                break;
            }

            if( folderContainer==nullptr )
            {
                folderContainer = RES_FOLDER_HOLDER;
            }

            if( folderContainer!=nullptr )
            {
                if( newProjectResource!=nullptr)
                {
                    if( newResId < 0)
                    {
                        newProjectResource->set_global_rid( increment_resouce_count() );
                    }
                    else
                    {
                        newProjectResource->set_global_rid( newResId );
                    }
                    newProjectResource->set_name( new_name);
                    newProjectResource->set_parent_name( projectFileName);
                    newProjectResource->parentProjectName = projectFileName;
                    newProjectResource->parentProjectDirectory = projectDirectory;
                    newProjectResource->resourceType = rNewType;
                    pawgui::widget_resource_container * newContainer = new pawgui::widget_resource_container(projectFileName,new_name,rNewType,resourceNumb,false,newProjectResource->get_global_rid() );
                    newContainer->set_held_resource(newProjectResource);
                    folderContainer->add_resource_container(newContainer);
                    //folderContainer->open_folder();
                    newContainer->parentProjectDirectory = projectDirectory;
                    return newContainer;
                }
            }
        }
    }
    return nullptr;
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_audio( pawgui::widget_resource_container * folderContainer,std::string new_name, int newResId  )
{
    return create_blank_resource( gpe::resource_type_audio, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_video( pawgui::widget_resource_container * folderContainer,std::string new_name, int newResId  )
{
    return create_blank_resource( gpe::resource_type_video, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_emitter( pawgui::widget_resource_container * folderContainer,std::string new_name, int newResId  )
{
    return create_blank_resource( gpe::resource_type_emitter, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_light( pawgui::widget_resource_container * folderContainer,std::string new_name, int newResId  )
{
    return create_blank_resource( gpe::resource_type_light, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_path( pawgui::widget_resource_container * folderContainer,std::string new_name, int newResId  )
{
    return create_blank_resource( gpe::resource_type_path, folderContainer, new_name, newResId);
}

//
pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_font(pawgui::widget_resource_container * folderContainer, std::string new_name, int newResId )
{
    return create_blank_resource( gpe::resource_type_font, folderContainer, new_name, newResId);
}
//

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_function(pawgui::widget_resource_container * folderContainer, std::string new_name, int newResId )
{
    return create_blank_resource( gpe::resource_type_function, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_class(pawgui::widget_resource_container * folderContainer, std::string new_name, int newResId )
{
    return create_blank_resource( gpe::resource_type_class, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_animation(pawgui::widget_resource_container * folderContainer, std::string new_name, int newResId )
{
    return create_blank_resource( gpe::resource_type_animation, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_texture(pawgui::widget_resource_container * folderContainer, std::string new_name, int newResId )
{
    return create_blank_resource( gpe::resource_type_texture, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_tilesheet(pawgui::widget_resource_container * folderContainer, std::string new_name, int newResId )
{
    return create_blank_resource( gpe::resource_type_tilesheet, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_game_enttity(pawgui::widget_resource_container * folderContainer, std::string new_name, int newResId )
{
    return create_blank_resource( gpe::resource_type_object, folderContainer, new_name, newResId);
}

pawgui::widget_resource_container *  GPE_ProjectFolder::create_blank_scene(pawgui::widget_resource_container * folderContainer,std::string new_name, int newResId )
{
    return create_blank_resource( gpe::resource_type_scene, folderContainer, new_name, newResId);
}

//Export C++
bool GPE_ProjectFolder::export_project_cpp(std::string projectBuildDirectory, std::string gpeBuilderName , int buildMetaTemplate, bool runGameOnCompile, bool inDebugMode)
{
    //Generate main file.
    pawgui::main_syntax_highlighter->begin_compiling();

    main_editor_log->log_build_line("Copying GPE source files..");
    int filesCopiedCount = gpe::copy_folder( gpe::app_directory_name+"build_files/cplusplus_source",projectDirectory+"/gpe_project/source", true, true );
    if(  filesCopiedCount > 0 )
    {
        main_editor_log->log_build_line("Successfully copied GPE Source("+ stg_ex::int_to_string( filesCopiedCount )+") files!");
    }
    else if( filesCopiedCount == -1)
    {
        main_editor_log->log_build_error("Unable to copy GPE Source files: Source Folder not found!");
    }
    else if( filesCopiedCount == -2)
    {
        main_editor_log->log_build_error("Unable to copy GPE Source files: Target Folder not found!");
    }
    else
    {
        main_editor_log->log_build_error("Unable to copy GPE Source files...");
    }

    main_editor_log->log_build_line("Auto-generating C++ file [main.cpp]..");

    std::string project_settingsFileName = projectDirectory+"/gpe_project/source/main.cpp";
    std::ofstream newMainFile (project_settingsFileName.c_str() );
    if (newMainFile.is_open() )
    {
        if( GPE_CPP_BUILDER_SETTINGS->write_cpp_header_file( &newMainFile, "main.cpp") )
        {

        }
        else
        {
            main_editor_log->log_build_error("Failed auto-generating C++ file [main.cpp]'s  header info+..");
        }
        //Add in the game spice here...
        newMainFile.close();
        main_editor_log->log_build_line("Successfully auto-generated [main.cpp]..");
    }
    else
    {
        main_editor_log->log_build_error("Failed auto-generating C++ file [main.cpp] AT +["+project_settingsFileName+"]+..");
    }

    if( (int)gpeBuilderName.size() > 0 )
    {
        main_editor_log->log_build_line("--  Initiating Build System ["+ gpeBuilderName +"] --");
    }
    else
    {
        main_editor_log->log_build_error("Unable to detect BuildSystem. Exiting build now...");
        return false;
    }
    if( GPE_CPP_BUILDER_SETTINGS == nullptr)
    {
        main_editor_log->log_build_error("Unable to detect GPE_CPP_BUILDER_SETTINGS. Exciting build now...");
        return false;
    }

    main_editor_log->log_build_line("Seeking Build System ["+ gpeBuilderName+"]...");
    gpeCPPBuildHolder * current_color_buildSystem = GPE_CPP_BUILDER_SETTINGS->find_build_system( gpeBuilderName );

    if( current_color_buildSystem == nullptr)
    {
        main_editor_log->log_build_error("Unable to detect BuildSystem["+gpeBuilderName+"]. Exciting build now...");
        return false;
    }

    gpeCPPSubsytemHolder * tempSubSystem = nullptr;
    int cBuildSubsystemcount = (int)current_color_buildSystem->mySubsystems.size();
    main_editor_log->log_build_line("Build subsystems detected ["+ stg_ex::int_to_string(cBuildSubsystemcount)+"]..");

    std::string buildScriptFile = "";
    std::string buildTemplateFile = "";
    std::string cppBuildDir = stg_ex::file_to_dir(projectFileName)+"/gpe_project";
    for( int i = 0; i < cBuildSubsystemcount; i++)
    {
        tempSubSystem = current_color_buildSystem->mySubsystems[i];
        if( tempSubSystem !=nullptr)
        {
            main_editor_log->log_build_line("--- Initiating Build Subsystem ["+ tempSubSystem->subsystemName +"] ---");

            if( tempSubSystem->systemProgramTemplateFile!=nullptr && tempSubSystem->systemProgramTemplateFile!=nullptr )
            {
                buildScriptFile = tempSubSystem->systemProgramTemplateFile->get_string();
                buildTemplateFile = tempSubSystem->systemProgramTemplateFile->get_string();
                if( process_build_script( buildScriptFile, buildScriptFile, cppBuildDir ) )
                {
                    main_editor_log->log_build_line("----["+buildTemplateFile+"] template made via ["+buildScriptFile+"] script. ----");
                }
                else
                {
                    main_editor_log->log_build_error("---- Unable to create ["+buildTemplateFile+"] template file via ["+buildScriptFile+"] script. ----");
                }
            }
            else
            {
                main_editor_log->log_build_error("---- Invalid build subsystem["+ tempSubSystem->subsystemName+"] detected at position: ["+ stg_ex::int_to_string(i)+"]... ----");
            }
        }
        else
        {
            main_editor_log->log_build_error("--- Invalid build subsystem detected at position: ["+ stg_ex::int_to_string(i)+"]... ---");
        }
    }

    //Now prepackages game instructions for custom game...
    //Grabs the list of resources and such.
    std::vector <pawgui::widget_resource_container *> buildTextureOptions;
    std::vector <pawgui::widget_resource_container *> buildTilesheetOptions;
    std::vector <pawgui::widget_resource_container *> buildanimationOptions;
    std::vector <pawgui::widget_resource_container *> buildAudioOptions;
    std::vector <pawgui::widget_resource_container *> buildVideoOptions;
    std::vector <pawgui::widget_resource_container *> buildPathsOptions;
    std::vector <pawgui::widget_resource_container *> buildFunctionsOptions;
    std::vector <pawgui::widget_resource_container *> buildFontOptions;
    //Added as of 1.14 [ BEGIN ]
    std::vector <pawgui::widget_resource_container *> buildClassesOptions;
    //Added as of 1.14 [ END ]
    std::vector <pawgui::widget_resource_container *> buildGameObjectOptions;
    std::vector <pawgui::widget_resource_container *> buildGameSceneOptions;
    projectPropertiesResource * project_settingsObject = (projectPropertiesResource*)RESC_PROJECT_SETTINGS->get_held_resource();
    RESC_TEXTURES->grab_useable_resources(buildTextureOptions);
    RESC_TILESHEETS->grab_useable_resources(buildTilesheetOptions);
    RESC_ANIMATIONS->grab_useable_resources(buildanimationOptions);
    RESC_AUDIO->grab_useable_resources(buildAudioOptions);
    RESC_VIDEOS->grab_useable_resources(buildVideoOptions);
    RESC_PATHS->grab_useable_resources(buildPathsOptions);
    RESC_FUNCTIONS->grab_useable_resources(buildFunctionsOptions);
    RESC_FONTS->grab_useable_resources(buildFontOptions);
    //Added as of 1.14 [ BEGIN ]
    RESC_CLASSES->grab_useable_resources(buildClassesOptions);
    //Added as of 1.14 [ END ]
    RESC_ENTITIES->grab_useable_resources(buildGameObjectOptions);
    RESC_SCENES->grab_useable_resources(buildGameSceneOptions);

    //For Mega Looking of resources:
    std::vector <pawgui::widget_resource_container *> buildGameBuildAllOptions;
    RESC_TEXTURES->grab_useable_resources(buildGameBuildAllOptions);
    RESC_TILESHEETS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_ANIMATIONS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_AUDIO->grab_useable_resources(buildGameBuildAllOptions);
    RESC_VIDEOS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_PATHS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_FUNCTIONS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_FONTS->grab_useable_resources(buildGameBuildAllOptions);
    //Added as of 1.14 [ BEGIN ]
    RESC_CLASSES->grab_useable_resources(buildGameBuildAllOptions);
    //Added as of 1.14 [ END ]
    RESC_ENTITIES->grab_useable_resources(buildGameBuildAllOptions);
    RESC_SCENES->grab_useable_resources(buildGameBuildAllOptions);

    currentObjParents.clear();
    nextObjParents.clear();

    return true;
}

//Export HTML5
//Build HTML5
bool GPE_ProjectFolder::export_project_html5(std::string projectBuildDirectory,  std::string gpeBuilderName , int buildMetaTemplate, bool runGameOnCompile, bool inDebugMode)
{
    pawgui::widget_resource_container * firstObjectContainer = RESC_ENTITIES->get_usable_resource();
    pawgui::widget_resource_container * firstSceneContainer = RESC_SCENES->get_usable_resource();
    bool hadSaveErrors = false;
    int build_screen_width = 640;
    int build_screen_height = 480;

    pawgui::main_syntax_highlighter->begin_compiling();
    if( main_editor_log!=nullptr)
    {
        if( buildMetaTemplate!= gpe::system_os_windows && buildMetaTemplate!= gpe::system_os_mac  && buildMetaTemplate!= gpe::system_os_linux )
        {
            main_editor_log->clear_build_log();
            main_editor_log->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[buildMetaTemplate]+" Export] (Compiler: Pawbitious Compiler)---------------");
        }
        main_editor_log->log_build_line("Exporting JS Based Project...");

        if( firstSceneContainer!=nullptr)
        {
            std::string projectFirstLevelName = firstSceneContainer->get_name();
            if( firstObjectContainer!=nullptr && pawgui::main_syntax_highlighter!=nullptr)
            {

                if( projectBuildDirectory.empty() )
                {
                    projectBuildDirectory = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/html5";
                }
                if( buildMetaTemplate== gpe::system_os_html5)
                {
                    projectBuildDirectory = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/html5";
                    setup_js_folder(projectBuildDirectory, gpe::system_os_html5,0,inDebugMode );
                }

                //Grabs the list of resources and such.
                currentObjParents.clear();
                nextObjParents.clear();
                //temp variables for looping through containers
                int iRes = 0;
                int paramLine = 0;
                int jFontType = 0;
                std::string copyFileDestination;
                int tempParentHTML5Id;
                int foundHtml5BuildId;


                //Checks if any of the resources names are banned/part of engine
                if( check_names_against_keywords( RESC_PROJECT_FOLDER) )
                {
                    hadSaveErrors = true;
                }

                projectPropertiesResource * project_settingsObject = nullptr;

                if( RESC_PROJECT_SETTINGS!=nullptr && RESC_PROJECT_SETTINGS->get_held_resource() !=nullptr)
                {
                    project_settingsObject  = (projectPropertiesResource*)RESC_PROJECT_SETTINGS->get_held_resource();
                }

                if( project_settingsObject == nullptr)
                {
                    hadSaveErrors = true;
                }

                if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                {
                    if( project_settingsObject->projectScreenWidthField->get_held_int() > 0)
                    {
                        build_screen_width = std::max(160, project_settingsObject->projectScreenWidthField->get_held_int() );
                    }

                    if( project_settingsObject->projectScreenHeightField->get_held_int() > 0)
                    {
                        build_screen_height = std::max(120,project_settingsObject->projectScreenHeightField->get_held_int() );
                    }
                    //temp variables for held resources and containers

                    //for preloading of surface and such

                    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt","Checking all resources..");
                    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt","[Textures]");

                    bool hasTextureToUse = false;
                    int amountOfObjectsCompiled = 0;

                    if( include_local_files( RESC_PROJECT_FOLDER, projectBuildDirectory,buildMetaTemplate ) )
                    {

                    }

                    //Added as of 1.14 [ BEGIN ]

                    if( buildMetaTemplate >=0 && buildMetaTemplate < gpe::system_os_max )
                    {
                        if( main_gpe_splash_page!=nullptr )
                        {
                            main_gpe_splash_page->update_messages( GPE_BUILD_NAMES[buildMetaTemplate]+" Export", "Please Wait...", "DO NOT CLOSE" );
                        }
                    }
                    else
                    {
                        if( main_gpe_splash_page!=nullptr )
                        {
                            main_gpe_splash_page->update_messages( "Unknown Export", "Please Wait...", "DO NOT CLOSE" );
                        }
                    }
                    //displayMessagestring = projectName;

                    //GPE Engine Files
                    std::string buildFileStyleCssFilleName = gpe::app_directory_name+"build_files/css/style.css";
                    std::string indexStyleCssFileName = projectBuildDirectory+"/css/style.css";
                    if( sff_ex::file_copy( buildFileStyleCssFilleName, indexStyleCssFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to copy [style.css] file");
                    }

                    std::string buildFileGPEPowerFilleName = "";
                    std::string indexFileGPEPowerFileName = "";
                    if( inDebugMode )
                    {
                        main_editor_log->log_build_line("Using DEBUG mode...");
                        buildFileGPEPowerFilleName = gpe::app_directory_name+"build_files/js/lib/gpepower_strict_v1_2_2_debug.txt";
                        indexFileGPEPowerFileName =projectBuildDirectory+"/js/lib/gpepower_strict_v1_2_2_debug.js";
                    }
                    else
                    {
                        main_editor_log->log_build_line("Using RELEASE mode...");
                        buildFileGPEPowerFilleName = gpe::app_directory_name+"build_files/js/lib/gpepower_strict_v1_2_2.txt";
                        indexFileGPEPowerFileName =projectBuildDirectory+"/js/lib/gpepower_strict_v1_2_2.js";
                    }

                    if( buildMetaTemplate == gpe::system_os_windows || buildMetaTemplate == gpe::system_os_mac || buildMetaTemplate == gpe::system_os_linux )
                    {
                        std::string buildFileElectronAddonFile = gpe::app_directory_name+"build_files/js/lib/electron_addon.txt";
                        std::string indexFileElectronAddonFile = projectBuildDirectory+"/js/lib/electron_addon.js";
                        if( sff_ex::file_copy( buildFileElectronAddonFile, indexFileElectronAddonFile)==false )
                        {
                            hadSaveErrors = true;
                            main_editor_log->log_build_error("Unable to copy Electron Addon");
                        }
                    }

                    if( sff_ex::file_copy( buildFileGPEPowerFilleName, indexFileGPEPowerFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to copy GPE HTML5 engine");
                    }

                    std::string buildFileGPESuperArrayFileName = gpe::app_directory_name+"build_files/js/lib/gpe_superarray.txt";
                    std::string indexFileGPESuperArrayFileName = projectBuildDirectory+"/js/lib/gpe_superarray.js";
                    if( sff_ex::file_copy( buildFileGPESuperArrayFileName, indexFileGPESuperArrayFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to copy array library");

                    }
                    std::string buildFileGPESuperstringFilleName = gpe::app_directory_name+"build_files/js/lib/gpe_superstring.txt";
                    std::string indexFileGPESuperstringFileName = projectBuildDirectory+"/js/lib/gpe_superstring.js";
                    if( sff_ex::file_copy( buildFileGPESuperstringFilleName, indexFileGPESuperstringFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to superstring library");
                    }

                    /*std::string buildFileScreenfullgFilleName = gpe::app_directory_name+"build_files/js/lib/screenfull.txt";
                    std::string indexFileScreenfullFileName = projectBuildDirectory+"/js/lib/screenfull.js";
                    if( sff_ex::file_copy( buildFileScreenfullgFilleName, indexFileScreenfullFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to screenfull library");
                    }*/

                    std::string buildFileGPEFrameManagerFilleName = gpe::app_directory_name+"build_files/js/lib/frame_management.txt";
                    std::string indexFileGPEFrameManagerFileName = projectBuildDirectory+"/js/lib/frame_management.js";
                    if( sff_ex::file_copy( buildFileGPEFrameManagerFilleName, indexFileGPEFrameManagerFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to copy frame management library");
                    }
                    ///GPE Intro Files

                    std::string buildFileGPELogoStartFilleName = gpe::app_directory_name+"build_files/res/gpe_logo_start.png";
                    std::string indexFileGPELogoStartFileName = projectBuildDirectory+"/res/gpe_logo_start.png";
                    if( sff_ex::file_copy( buildFileGPELogoStartFilleName, indexFileGPELogoStartFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to copyGPE logo.");
                    }
                    std::string buildFileIntroAudioStartAACFilleName = gpe::app_directory_name+"build_files/res/gpe_intro.aac";
                    std::string indexFileGPEIntroAudioStartAACFileName = projectBuildDirectory+"/res/gpe_intro.aac";
                    if( sff_ex::file_copy( buildFileIntroAudioStartAACFilleName, indexFileGPEIntroAudioStartAACFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to copy gpe_intro.aac");
                    }
                    std::string buildFileIntroAudioStartMP3FilleName = gpe::app_directory_name+"build_files/res/gpe_intro.mp3";
                    std::string indexFileIntroAudioStartMP3FileName = projectBuildDirectory+"/res/gpe_intro.mp3";
                    if( sff_ex::file_copy( buildFileIntroAudioStartMP3FilleName, indexFileIntroAudioStartMP3FileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to copy gpe_intro.mp3");
                    }
                    std::string buildFileIntroAudioStartOOGFilleName = gpe::app_directory_name+"build_files/res/gpe_intro.ogg";
                    std::string indexFileGPEIntroAudioStartOOGFileName = projectBuildDirectory+"/res/gpe_intro.ogg";
                    if( sff_ex::file_copy( buildFileIntroAudioStartOOGFilleName, indexFileGPEIntroAudioStartOOGFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to copy gpe_intro.ogg");
                    }
                    std::string buildFileIntroAudioStartWAVFilleName = gpe::app_directory_name+"build_files/res/gpe_intro.wav";
                    std::string indexFileGPEIntroAudioStartWAVFileName = projectBuildDirectory+"/res/gpe_intro.wav";
                    if( sff_ex::file_copy( buildFileIntroAudioStartWAVFilleName, indexFileGPEIntroAudioStartWAVFileName)==false )
                    {
                        hadSaveErrors = true;
                        main_editor_log->log_build_error("Unable to copy gpe_intro.wav");
                    }

                    if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                    {
                        std::string indexHTML5FileName = projectBuildDirectory+"/index.html";
                        main_editor_log->log_build_line("Creating [index.html] file.");
                        std::ofstream indexHTML5FILE ( indexHTML5FileName.c_str() );

                        if (indexHTML5FILE.is_open())
                        {
                            indexHTML5FILE << "<!DOCTYPE HTML> \n";
                            indexHTML5FILE << "<html style='margin: 0px; padding: 0px;'> \n";
                            indexHTML5FILE << "<!--  Game Pencil Engine Project File --> \n";
                            indexHTML5FILE << "<!--  Created automatically via the Game Pencil Engine Editor -->\n";
                            indexHTML5FILE << "<!--  Warning: Manually editing this file may cause unexpected bugs and errors. -->\n";
                            indexHTML5FILE << "<!--  If you have any problems reading this file please report it to help@pawbyte.com -->\n";
                            indexHTML5FILE << "    <head>\n";
                            /*
                            if( buildMetaTemplate ==system_os_html5 && project_settingsObject->projectHtmlHeaderCode!=nullptr)
                            {
                                if( project_settingsObject->headerCodeBeforeGPECheckbox!=nullptr && project_settingsObject->headerCodeBeforeGPECheckbox->is_clicked() )
                                {
                                    if( project_settingsObject->projectHtmlHeaderCode->has_content() )
                                    {
                                        project_settingsObject->projectHtmlHeaderCode->write_data_into_file(&indexHTML5FILE,2,true);
                                        indexHTML5FILE << "\n";
                                    }
                                }
                            }
                            */

                            indexHTML5FILE << "        <meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>\n";
                            indexHTML5FILE << "        <meta name='viewport' content='width=device-width, user-scalable=no, initial-scale=1, maximum-scale=1.0'> \n";
                            //Modified as of 1.14 [ BEGIN ]
                            indexHTML5FILE << "        <title>";
                            if( inDebugMode)
                            {
                                indexHTML5FILE << "[DEBUG MODE]";
                            }
                            indexHTML5FILE << project_settingsObject->projectGameShortTitleField->get_string() << "</title> \n";
                            //Added as of 1.14 [ END ]
                            indexHTML5FILE << "        <link rel='stylesheet' type='text/css'  href='css/style.css'> \n";
                            indexHTML5FILE << "        <link rel='stylesheet' type='text/css'  href='css/gpe_custom_style.css'> \n";

                            if( buildMetaTemplate == gpe::system_os_windows || buildMetaTemplate == gpe::system_os_mac || buildMetaTemplate == gpe::system_os_linux )
                            {
                                indexHTML5FILE << "        <script src='js/lib/electron_addon.js'></script> \n";
                            }
                            indexHTML5FILE << "        <script src='js/lib/frame_management.js'></script> \n";
                            //indexHTML5FILE << "        <script src='js/lib/screenfull.js'></script> \n";
                            indexHTML5FILE << "        <script src='js/lib/gpe_superarray.js'></script> \n";
                            indexHTML5FILE << "        <script src='js/gpe_settings.js'></script> \n";
                            //Modified as of 1.14 [ BEGIN ]
                            if( inDebugMode)
                            {
                                indexHTML5FILE << "        <script src='js/lib/gpepower_strict_v1_2_2_debug.js'></script>	 \n";
                            }
                            else
                            {
                                indexHTML5FILE << "        <script src='js/lib/gpepower_strict_v1_2_2.js'></script>	 \n";
                            }
                            //indexHTML5FILE << "        <script src='js/gpe_app.js'></script>	 \n";
                            indexHTML5FILE << "     </head>\n";
                            indexHTML5FILE << "     <body>\n";
                            indexHTML5FILE << "        <canvas id='gpeCanvas' width='auto' height='"<< build_screen_height << "' oncontextmenu='return false;'></canvas> \n";
                            if( buildMetaTemplate == gpe::system_os_html5 && project_settingsObject->checkBoxShowPublisherInfo!=nullptr)
                            {
                                if(project_settingsObject->checkBoxShowPublisherInfo->is_clicked() )
                                {
                                    if( project_settingsObject->projectGameTitleField->has_content() )
                                    {
                                        indexHTML5FILE << "     <h2>" << project_settingsObject->projectGameTitleField->get_string() << "</h2>\n";
                                    }

                                    if( project_settingsObject->projectGameDescriptionField->has_content() )
                                    {
                                        indexHTML5FILE << "     <h3>" << project_settingsObject->projectGameDescriptionField->get_string() << "</h3>\n";
                                    }

                                    if( project_settingsObject->projectGameNotes!=nullptr)
                                    {
                                        if( project_settingsObject->projectGameNotes->has_content() )
                                        {
                                            indexHTML5FILE << "     <p>";
                                            project_settingsObject->projectGameNotes->write_data_into_file(&indexHTML5FILE,2,true);
                                            indexHTML5FILE << "</p>\n";
                                        }
                                    }

                                    if( project_settingsObject->projectGamePublisherField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p>Published by " << project_settingsObject->projectGamePublisherField->get_string() << "</p>\n";
                                    }

                                    if( project_settingsObject->projectGameDeveloperField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p>Developed by " << project_settingsObject->projectGameDeveloperField->get_string() << "</p>\n";
                                    }

                                    if( project_settingsObject->projectGameCopyrightField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p>Copyright " << project_settingsObject->projectGameCopyrightField->get_string() << " "<<project_settingsObject->projectGameDateField->get_string() <<"</p>\n";
                                    }

                                    if( project_settingsObject->projectGameWebsiteField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p> <a ahref='" << project_settingsObject->projectGameWebsiteField->get_string() << "'>For more information on game click here</a></p>\n";
                                    }

                                    if( project_settingsObject->projectGameEmailField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p> Email <a ahref='mailto:" << project_settingsObject->projectGameEmailField->get_string() << "?Subject=Hello'>"<< project_settingsObject->projectGameEmailField->get_string() << " for assistance</a></p>\n";
                                    }
                                }
                            }

                            if( buildMetaTemplate== gpe::system_os_html5 && project_settingsObject->warnOnCloseCheckbox->is_clicked() )
                            {
                                indexHTML5FILE << "<script type='text/javascript'>\n";
                                indexHTML5FILE << "window.onbeforeunload = function () {return 'Are you sure that you want to leave this page? Game Data may be lost';}\n";
                                indexHTML5FILE << "</script>\n";
                            }
                            indexHTML5FILE << "    </body>\n";
                            indexHTML5FILE << "</html> \n";

                            indexHTML5FILE.close();
                            std::string indexGame_settingsFileName = projectBuildDirectory+"/js/gpe_settings.js";
                            std::ofstream indexJS_settingsFILE ( indexGame_settingsFileName.c_str() );
                            if (indexJS_settingsFILE.is_open())
                            {
                                indexJS_settingsFILE << "/*     \n";
                                indexJS_settingsFILE << "#    --------------------------------------------------  #\n";
                                indexJS_settingsFILE << "#       \n";
                                indexJS_settingsFILE << "#       \n";
                                indexJS_settingsFILE << "#  Game Pencil Engine Game File \n";
                                indexJS_settingsFILE << "#  Created automatically via the Game Pencil Engine Editor \n";
                                indexJS_settingsFILE << "#  Warning: Manually editing this file may cause unexpected bugs and errors. \n";
                                indexJS_settingsFILE << "#  If you have any problems reading this file please report it to help@pawbyte.com . \n";
                                indexJS_settingsFILE << "#     \n";
                                indexJS_settingsFILE << "#      \n";
                                indexJS_settingsFILE << "#    --------------------------------------------------  # \n";
                                indexJS_settingsFILE << "*/     \n\n";
                                indexJS_settingsFILE << "var game = game || {};\n \n";
                                int fps_value = project_settingsObject->project_settingsFPSRate->get_selected_value();
                                if( fps_value > 0)
                                {
                                    indexJS_settingsFILE << "GPE_STANDARD_FPS ="+ stg_ex::int_to_string(fps_value)+";\n \n";
                                }
                                else
                                {
                                    indexJS_settingsFILE << "GPE_STANDARD_FPS = 60;\n \n";
                                    indexJS_settingsFILE << "GPE_FPS_UNCAPPED = true;\n \n";
                                }
                                indexJS_settingsFILE << "GPE_STANDARD_INTERVAL =  1000/GPE_STANDARD_FPS ; \n";
                                indexJS_settingsFILE << "var GPE_SETTINGS_APP_LOGO_LOCATION = 'res/gpe_logo_start.png'; \n";
                                indexJS_settingsFILE << "var GPE_SETTINGS_START_SCENE = '"+projectFirstLevelName+"'; \n";


                                indexJS_settingsFILE << "var GPE_SETTINGS_screen_width = " << build_screen_width << ";\n";
                                indexJS_settingsFILE << "var GPE_SETTINGS_screen_height = " << build_screen_height << ";\n";

                                indexJS_settingsFILE << "var GPE_SETTINGS_PROG_NAME = '" << project_settingsObject->projectGameTitleField->get_string() << "';\n";
                                indexJS_settingsFILE << "var GPE_SETTINGS_VERSION_NUMBER = "<< project_settingsObject->projectGameVersionField->get_string()<< ";\n";
                                //indexJS_settingsFILE << "var object_max_types = "<< (int)buildGameObjectOptions.size() << ";\n";
                                indexJS_settingsFILE << "var GPE_SETTINGS_SYSTEM_OS = 'Browser';\n";
                                indexJS_settingsFILE << "var GPE_GAME_PUBLISHER = '" << project_settingsObject->projectGamePublisherField->get_string()<< "';\n";
                                indexJS_settingsFILE << "var GPE_GAME_DEVELOPER = '" << project_settingsObject->projectGameDeveloperField->get_string() << "';\n";
                                //indexJS_settingsFILE << "var GPE_game_objects_COUNT = "+ stg_ex::int_to_string( (int)buildGameObjectOptions.size() )+";\n";
                                indexJS_settingsFILE << "var GPE_SETTINGS_IS_DEBUGGING = false;\n";
                                indexJS_settingsFILE << "var GPE_SETTINGS_SHOW_FPS = true;\n";

                                indexJS_settingsFILE << "var GPE_GAME_SCALING_SETTING = " << project_settingsObject->project_scale_settings->get_selected_value()<< ";\n";


                                indexJS_settingsFILE << "var GPE_PreloadImageResource = [];\n";
                                int preloadCount = 0;

                                int sound_preload_count[ gpe::sound_format_max ];
                                for( int i_sound_format = 0; i_sound_format < gpe::sound_format_max ; i_sound_format++ )
                                {
                                    sound_preload_count[ i_sound_format ] = 0;
                                }

                                if( RESC_PROJECT_SETTINGS!=nullptr )
                                {
                                    indexJS_settingsFILE << "var Game_LayerMatrix = [];\n";
                                    indexJS_settingsFILE << "var Game_SelfCollideLayerList = [];\n";
                                    projectPropertiesResource * tProjectProps = (projectPropertiesResource *)RESC_PROJECT_SETTINGS->get_held_resource();
                                    if( tProjectProps!=nullptr)
                                    {
                                        int jLayerCol = 0;
                                        for( int iLayerRow = 0; iLayerRow < 32; iLayerRow++)
                                        {
                                            indexJS_settingsFILE << "Game_LayerMatrix.push( [] );\n";
                                            for(  jLayerCol = 0; jLayerCol < 32-iLayerRow; jLayerCol++)
                                            {
                                                if( iLayerRow==jLayerCol)
                                                {
                                                    if( tProjectProps->projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->is_clicked() )
                                                    {
                                                        indexJS_settingsFILE << "Game_SelfCollideLayerList.push(1);\n";
                                                    }
                                                    else
                                                    {
                                                        indexJS_settingsFILE << "Game_SelfCollideLayerList.push(0);\n";
                                                    }
                                                }
                                                else if( tProjectProps->projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->is_clicked() )
                                                {
                                                    indexJS_settingsFILE << "Game_LayerMatrix["+ stg_ex::int_to_string(iLayerRow)+"].push("+ stg_ex::int_to_string( jLayerCol )+");\n";
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        indexJS_settingsFILE << "//RESC_PROJECT_SETTINGS local not reachable...\n";
                                    }
                                }
                                else
                                {
                                    indexJS_settingsFILE << "//RESC_PROJECT_SETTINGS not reachable...\n";
                                }

                                indexJS_settingsFILE.close();
                            }
                            else
                            {
                                hadSaveErrors = true;
                                main_editor_log->log_build_error("Unable to create [gpe_settings.js]...[Build canceled]");
                            }

                            if( buildMetaTemplate== gpe::system_os_windows || buildMetaTemplate== gpe::system_os_mac || buildMetaTemplate== gpe::system_os_linux )
                            {
                                std::string indexJsonPackageFileName = projectBuildDirectory+"/package.json";
                                std::ofstream indexJsonPackageFile ( indexJsonPackageFileName.c_str() );
                                main_editor_log->log_build_line("Creating [package.json] file...");
                                if (indexJsonPackageFile.is_open())
                                {
                                    indexJsonPackageFile << "{\n";
                                    indexJsonPackageFile << "\"name\"             :   \"" << project_settingsObject->projectGameShortTitleField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"productName\"      :   \"" << project_settingsObject->projectGameTitleField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"description\"      :   \"" << project_settingsObject->projectGameDescriptionField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"version\"          :   \"" << project_settingsObject->projectGameVersionField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"main\"             :   \"main.js\"\n";
                                    indexJsonPackageFile << "}\n";
                                    indexJsonPackageFile.close();
                                }
                                else
                                {
                                    hadSaveErrors = true;
                                    main_editor_log->log_build_error("Unable to create [package.json]...[Build canceled]");
                                }
                            }

                            std::string indexJSCustomGameFileName = projectBuildDirectory+"/js/gpe_app.js";
                            std::ofstream indexJSCustomGameFILE ( indexJSCustomGameFileName.c_str() );
                            main_editor_log->log_build_line("Creating [gpe_app.js] file...");
                            if (indexJSCustomGameFILE.is_open())
                            {
                                indexJSCustomGameFILE << "/*     \n";
                                indexJSCustomGameFILE << "#    --------------------------------------------------  #\n";
                                indexJSCustomGameFILE << "#       \n";
                                indexJSCustomGameFILE << "#       \n";
                                indexJSCustomGameFILE << "#  Game Pencil Engine Game File \n";
                                indexJSCustomGameFILE << "#  Created automatically via the Game Pencil Engine Editor Version "<< stg_ex::int_to_string(gpe::version_number_total) << "\n";
                                indexJSCustomGameFILE << "#  Warning: Manually editing this file may cause unexpected bugs and errors. \n";
                                indexJSCustomGameFILE << "#  If you have any problems reading this file please report it to help@pawbyte.com . \n";
                                indexJSCustomGameFILE << "#     \n";
                                indexJSCustomGameFILE << "#      \n";
                                indexJSCustomGameFILE << "#    --------------------------------------------------  # \n";
                                indexJSCustomGameFILE << "*/     \n\n";

                                //Modified as of 1.14 [ BEGIN ]
                                indexJSCustomGameFILE << "'use strict';\n";
                                //Modified as of 1.14 [ END ]
                                currentObjParents.clear();
                                nextObjParents.clear();

                                pawgui::widget_resource_container * tempContainer = nullptr;
                                pawgui::general_resource * tempHeldResource = nullptr;
                                int maxResources = RESC_PROJECT_FOLDER->get_size();

                                for( iRes = 0; iRes < maxResources; iRes++)
                                {
                                    tempContainer = RESC_PROJECT_FOLDER->get_resource_at( iRes );
                                    if( tempContainer!=nullptr && tempContainer->get_held_resource()!=nullptr )
                                    {
                                        tempHeldResource = tempContainer->get_held_resource();
                                        tempHeldResource->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                    }
                                }

                                indexJSCustomGameFILE << "\n";

                                //Closes the custom gpe_app.js file
                                indexJSCustomGameFILE.close();
                                gpe::error_log->report("Building CSS file..");
                                if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                                {
                                    std::string indexCustomCSSGameFileName = projectBuildDirectory+"/css/gpe_custom_style.css";
                                    std::ofstream indexCustomCSSGameFile ( indexCustomCSSGameFileName.c_str() );
                                    main_editor_log->log_build_line("Creating [gpe_custom_styles.css] file...");
                                    if (indexCustomCSSGameFile.is_open())
                                    {
                                        indexCustomCSSGameFile << "\n";
                                        indexCustomCSSGameFile <<"body\n{\n   background-color: #" << project_settingsObject->projectWebsiteBackgroundColor->get_hex_string() << ";\n   color: #" << project_settingsObject->projectTextParagraphColor->get_hex_string()<< ";\n width: 100%;\n height: 100%;\n}\n";
                                        indexCustomCSSGameFile <<"canvas\n{\nborder-color: #" << project_settingsObject->projectBorderColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, h12\n{\n  color: #" << project_settingsObject->projectTextHeaderColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"p\n{\n  color: #" << project_settingsObject->projectTextParagraphColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a,a:link\n{\n     color: #" << project_settingsObject->projectTextLinkColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a:active\n{\n     color: #" << project_settingsObject->projectTextLinkActiveColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a:hover\n{\n     color: #" << project_settingsObject->projectTextLinkHoverColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a:visited\n{\n     color: #" << project_settingsObject->projectTextLinkVisitedColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"\n\n";

                                        indexCustomCSSGameFile.close();
                                    }
                                    else
                                    {
                                        hadSaveErrors = true;
                                        main_editor_log->log_build_error("Project Build Error: Unable to create [gpe_custom_style.css]..");
                                    }
                                }
                                //Final check for build errors
                                if( !hadSaveErrors)
                                {
                                    main_editor_log->log_build_line("Game built with no detected errors.");
                                    if( runGameOnCompile && buildMetaTemplate== gpe::system_os_html5)
                                    {
                                        main_editor_log->log_build_line("Opening... "+indexHTML5FileName+" .");
                                        gpe::external_open_url(indexHTML5FileName);
                                    }
                                }
                                else
                                {
                                    remove(indexJSCustomGameFileName.c_str());
                                }
                            }
                            else
                            {
                                hadSaveErrors = true;
                                main_editor_log->log_build_error("Unable to create [gpe_app.js]..");
                            }
                        }
                        else
                        {
                            hadSaveErrors = true;
                            pawgui::display_user_alert("Project Build Error!","Unable to create game [index.html] file...");
                            main_editor_log->log_build_error("Unable to create game [index.html] file...");
                        }
                        if(RESC_PROJECT_FOLDER!=nullptr)
                        {
                            //hadSaveErrors = RESC_PROJECT_FOLDER->write_data_into_projectfile(&indexHTML5FILE);
                        }
                    }
                    else
                    {
                        hadSaveErrors = true;
                    }
                }
                else
                {
                    hadSaveErrors = true;
                }
            }
            else
            {
                hadSaveErrors = true;
                main_editor_log->log_build_error("At least one game object must exist in project...[Build canceled]");
            }
        }
        else
        {
            hadSaveErrors = true;
            main_editor_log->log_build_error("At least one game scene must exist in project...[Build cancelede]");
        }
    }
    if( hadSaveErrors)
    {
        main_editor_log->log_build_error("Unable to build game!");
        pawgui::display_user_alert("Project Build Error!","Unable to build game! Please see error log...");
    }
    return hadSaveErrors;
}

bool GPE_ProjectFolder::export_project_windows(std::string projectBuildDirectory, std::string gpeBuilderName , int buildBits, bool runGameOnCompile, bool inDebugMode, int nativeBuildType )
{
    bool buildResult = false;
    if( main_editor_log!=nullptr)
    {
        main_editor_log->clear_build_log();
        main_editor_log->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[gpe::system_os_windows]+" Export] (Compiler: Pawbitious Builder)---------------");

        if( (int)projectBuildDirectory.size() < 3)
        {
            projectBuildDirectory = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/windows";
        }
        std::string pBuildDirectory = "";
        //Check if we are using the C++ language and proceed as follows
        main_editor_log->log_build_line("- Creating Windows Output environment... -");

        if( myProjectLanguage == pawgui::program_language_cpp )
        {
            if( main_gpe_splash_page != nullptr )
            {
                main_gpe_splash_page->update_messages( "Exporting ["+projectName+"][Windows]", "Please wait...", "DO NOT CLOSE");
            }

            pBuildDirectory = setup_cpp_folder(projectBuildDirectory,gpe::system_os_windows,buildBits,inDebugMode);

            buildResult = export_project_cpp( pBuildDirectory, gpeBuilderName,gpe::system_os_windows,runGameOnCompile, inDebugMode );
        }
        else if( nativeBuildType!=native_option_none)
        {
            if( main_gpe_splash_page != nullptr )
            {
                main_gpe_splash_page->update_messages( "Exporting ["+projectName+"][Windows]", "Please wait...", "DO NOT CLOSE");
            }

            pBuildDirectory = setup_js_folder(projectBuildDirectory,gpe::system_os_windows,buildBits,inDebugMode,nativeBuildType);
            buildResult =  export_project_html5( pBuildDirectory, gpeBuilderName,gpe::system_os_windows,runGameOnCompile, inDebugMode );

            if( runGameOnCompile && gpe::system_found_os== gpe::system_os_windows)
            {
                run_project(projectBuildDirectory, gpe::system_os_windows,buildBits,inDebugMode);
                /*
                std::string appAddonFileName = "";
                if( buildBits==64)
                {
                    appAddonFileName+="_64";
                }
                if( inDebugMode)
                {
                    appAddonFileName+="_debug";
                }
                if( sff_ex::file_exists(pBuildDirectory+"/../../game"+appAddonFileName+".exe") )
                {
                    gpe::external_open_program(pBuildDirectory+"/../../game"+appAddonFileName+".exe");
                }
                else if( sff_ex::file_exists(pBuildDirectory+"/../../electron"+appAddonFileName+".exe") )
                {
                    gpe::external_open_program(pBuildDirectory+"/../../electron"+appAddonFileName+".exe");
                }
                */
            }
        }
        else
        {
            main_editor_log->open_build_log();
            main_editor_log->log_build_error("Project not built. Native Build Type NOT selected");
        }
    }
    else
    {
        gpe::error_log->report("Unable to build game. Internal Log error");
    }
    return buildResult;
}

bool GPE_ProjectFolder::export_project_osx(std::string projectBuildDirectory,  std::string gpeBuilderName , int buildBits, bool runGameOnCompile, bool inDebugMode, int nativeBuildType)
{
    bool buildResult = false;
    if( main_editor_log!=nullptr)
    {
        main_editor_log->clear_build_log();
        main_editor_log->log_build_line("- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[ gpe::system_os_mac]+" Export] (Compiler: Pawbitious Builder)- ");

        if( (int)projectBuildDirectory.size() < 3)
        {
            projectBuildDirectory = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/osx";
        }
        std::string pBuildDirectory = "";
        //Check if we are using the C++ language and proceed as follows
        main_editor_log->log_build_line("-- Creating OSX Output environment... --");

        if( myProjectLanguage == pawgui::program_language_cpp )
        {
            if( main_gpe_splash_page != nullptr )
            {
                main_gpe_splash_page->update_messages( "Exporting ["+projectName+"][Windows]", "Please wait...", "DO NOT CLOSE");
            }


            pBuildDirectory = setup_cpp_folder(projectBuildDirectory, gpe::system_os_mac,buildBits,inDebugMode );

            if( myProjectLanguage == pawgui::program_language_cpp )
            {
                buildResult = export_project_cpp( pBuildDirectory, gpeBuilderName, gpe::system_os_mac,runGameOnCompile, inDebugMode );
            }

            if( runGameOnCompile && gpe::system_found_os== gpe::system_os_mac )
            {
                run_project(projectBuildDirectory, gpe::system_os_mac,buildBits,inDebugMode);
            }
        }
        else if( nativeBuildType!=native_option_none)
        {
            if( main_gpe_splash_page != nullptr )
            {
                main_gpe_splash_page->update_messages( "Exporting ["+projectName+"][OSX]", "Please wait...", "DO NOT CLOSE");
            }

            pBuildDirectory  = setup_js_folder(projectBuildDirectory, gpe::system_os_mac,buildBits,inDebugMode, nativeBuildType);
            buildResult =  export_project_html5( pBuildDirectory, gpeBuilderName, gpe::system_os_mac, runGameOnCompile, inDebugMode );
            if( runGameOnCompile && gpe::system_found_os== gpe::system_os_mac)
            {
                run_project(projectBuildDirectory, gpe::system_os_mac,buildBits,inDebugMode);

                /*
                std::string appAddonFileName = "";
                if( buildBits==64)
                {
                    appAddonFileName+="_64";
                }
                if( inDebugMode)
                {
                    appAddonFileName+="_debug";
                }
                if( sff_ex::file_exists(pBuildDirectory+"/../../game"+appAddonFileName+".app") )
                {
                    gpe::external_open_program(pBuildDirectory+"/../../game"+appAddonFileName+".app");
                }
                else if( sff_ex::file_exists(pBuildDirectory+"/../../Game"+appAddonFileName+".app") )
                {
                    gpe::external_open_program(pBuildDirectory+"/../../Game"+appAddonFileName+".app");
                }
                else if( sff_ex::file_exists(pBuildDirectory+"/../../Electron"+appAddonFileName+".app") )
                {
                    gpe::external_open_program(pBuildDirectory+"/../../Electron"+appAddonFileName+".app");
                }
                else if( sff_ex::file_exists(pBuildDirectory+"/../../electron"+appAddonFileName+".app") )
                {
                    gpe::external_open_program(pBuildDirectory+"/../../electron"+appAddonFileName+".app");
                }
                */
            }
        }
        else
        {
            main_editor_log->open_build_log();
            main_editor_log->log_build_error("Project not built. Native Build Type NOT selected");
        }
    }
    else
    {
        gpe::error_log->report("Unable to build game. Internal Log error");
    }
    return buildResult;
}

bool GPE_ProjectFolder::export_project_linux(std::string projectBuildDirectory,  std::string gpeBuilderName , int buildBits, bool runGameOnCompile, bool inDebugMode, int nativeBuildType)
{
    bool buildResult = false;
    if( main_editor_log!=nullptr)
    {
        main_editor_log->clear_build_log();
        main_editor_log->log_build_line("- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[ gpe::system_os_linux]+" Export] (Compiler: Pawbitious Builder) --");

        if( (int)projectBuildDirectory.size() < 3)
        {
            projectBuildDirectory = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/linux";
        }
        std::string pBuildDirectory = "";
        //Check if we are using the C++ language and proceed as follows
        main_editor_log->log_build_line("-- Creating Linux Output environment... --");

        if( myProjectLanguage == pawgui::program_language_cpp )
        {
            if( main_gpe_splash_page != nullptr )
            {
                main_gpe_splash_page->update_messages( "Exporting ["+projectName+"][Windows]", "Please wait...", "DO NOT CLOSE");
            }


            pBuildDirectory = setup_cpp_folder(projectBuildDirectory,gpe::system_os_linux,buildBits,inDebugMode );

            if( myProjectLanguage == pawgui::program_language_cpp )
            {
                buildResult = export_project_cpp( pBuildDirectory, gpeBuilderName, gpe::system_os_linux,runGameOnCompile, inDebugMode );
            }

            if( runGameOnCompile && gpe::system_found_os==gpe::system_os_linux )
            {
                run_project(projectBuildDirectory,gpe::system_os_linux,buildBits,inDebugMode);
            }
        }
        else if( nativeBuildType!=native_option_none)
        {
            if( main_gpe_splash_page != nullptr )
            {
                main_gpe_splash_page->update_messages( "Exporting ["+projectName+"][Linux]", "Please wait...", "DO NOT CLOSE");
            }

            pBuildDirectory  = setup_js_folder(projectBuildDirectory, gpe::system_os_linux,buildBits,inDebugMode, nativeBuildType);
            buildResult =  export_project_html5( pBuildDirectory, gpeBuilderName, gpe::system_os_linux, runGameOnCompile, inDebugMode);

            if( runGameOnCompile && gpe::system_found_os==gpe::system_os_linux )
            {
                run_project(projectBuildDirectory,gpe::system_os_linux,buildBits,inDebugMode);
                /*
                std::string appAddonFileName = "";
                if( buildBits==64)
                {
                    appAddonFileName+="_64";
                }
                if( inDebugMode)
                {
                    appAddonFileName+="_debug";
                }
                if( sff_ex::file_exists(pBuildDirectory+"/../../game"+appAddonFileName) )
                {
                    gpe::external_open_program(pBuildDirectory+"/../../game"+appAddonFileName);
                }
                else if( sff_ex::file_exists(pBuildDirectory+"/../../electron"+appAddonFileName) )
                {
                    gpe::external_open_program(pBuildDirectory+"/../../electron"+appAddonFileName);
                }
                else if( sff_ex::file_exists(pBuildDirectory+"/../../Electron"+appAddonFileName) )
                {
                    gpe::external_open_program(pBuildDirectory+"/../../Electron"+appAddonFileName);
                }
                */
            }
        }
        else
        {
            main_editor_log->open_build_log();
            main_editor_log->log_build_error("Project not built. Native Export Mode not selected");
        }
    }
    else
    {
        gpe::error_log->report("Unable to build game. Internal Log error");
    }
    return buildResult;
}

std::string GPE_ProjectFolder::get_project_language()
{
    return projectLanguage;
}

int GPE_ProjectFolder::get_project_language_id()
{
    return myProjectLanguage;
}

std::string GPE_ProjectFolder::get_project_name()
{
    return projectName;
}

std::string GPE_ProjectFolder::get_project_directory()
{
    return projectDirectory;
}

std::string GPE_ProjectFolder::get_project_file_name()
{
    return projectFileName;
}

int GPE_ProjectFolder::get_resource_count()
{
    if( RESC_PROJECT_FOLDER!=nullptr)
    {
        return RESC_PROJECT_FOLDER->get_resource_count();
    }
    return 0;
}

bool GPE_ProjectFolder::include_local_files( pawgui::widget_resource_container * resContainer,std::string pBuildDir , int buildType )
{
    if( resContainer == nullptr)
    {
        return true;
    }
    bool includesWithoutErrors  = true;

    if( resContainer->is_super_project_folder()== false && resContainer->is_folder() == false )
    {
        if(resContainer->include_local_files( pBuildDir, buildType) ==false )
        {
            includesWithoutErrors = true;
        }
    }
    else
    {
        main_editor_log->log_build_comment("Including ["+resContainer->get_name()+"] files.");
    }

    pawgui::widget_resource_container * tempContainer = nullptr;
    int resLimit = resContainer->get_size();
    for( int iRes = 0; iRes < resLimit; iRes++)
    {
        tempContainer = resContainer->get_resource_at( iRes );
        if( tempContainer!=nullptr)
        {
            if( include_local_files(tempContainer, pBuildDir, buildType ) == false )
            {
                includesWithoutErrors = false;
            }
        }
    }
    return includesWithoutErrors;
}

bool GPE_ProjectFolder::load_project_file(std::string projectFileIn )
{
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_messages( "Loading Project [" + projectName + "]", "Opening Project File","DO NOT CLOSE..." );
    }

    if( (int)projectFileIn.size() > 0)
    {
        std::ifstream newprofileFile( projectFileIn.c_str() );

        //Checks and/or makes all of the sub-directories for the folder.
        setup_project_directory(projectDirectory);

        //If the level file could be loaded
        if( !newprofileFile.fail() )
        {
            //makes sure the file is open
            if (newprofileFile.is_open())
            {
                projectFileName = projectFileIn;
                if( main_gpe_splash_page != nullptr )
                {
                    main_gpe_splash_page->update_submessages( "Reading Project File","DO NOT CLOSE..." );
                }
                pawgui::widget_resource_container * current_color_resFolder = nullptr;
                pawgui::widget_resource_container * tempResFolder = nullptr;
                pawgui::widget_resource_container * containerFolderToEdit = nullptr;
                pawgui::widget_resource_container * newContainer = nullptr;
                animationResource * tempSprRes = nullptr;
                textureResource * tempTexRes = nullptr;
                tilesheetResource * tempTstRes = nullptr;
                audioResource * tempAudRes = nullptr;
                videoResource * tempVidRes = nullptr;
                gamePathResource * tempPathRes = nullptr;
                lightResource * tempLightRes = nullptr;
                particleResource * tempEmitterRes = nullptr;
                functionResource * tempFuncRes = nullptr;
                classResource *    tempClassRes = nullptr;
                gameEntityResource * tempObjRes = nullptr;
                gameSceneResource * tempScnRes = nullptr;
                //achievementResource * tempAchRes = nullptr;
                fontResource * tempFntRes = nullptr;
                std::string firstChar="";
                std::string key_string="";
                std::string valstring="";
                std::string subValstring="";
                std::string currLine="";
                std::string newresource_name= "";
                std::string tempNewresource_name = "";
                int equalPos=-1;
                int endBracketPos = -1;
                int foundResGlobalId = -1;
                int tempStrToInt = -1;
                int iItr = 0;
                int iLang = 0;

                std::vector <animationResource *> projectGameanimations;
                std::vector <textureResource *> projectGameTextures;
                std::vector <tilesheetResource *> projectGameTilesheets;
                std::vector <audioResource * > projectGameAudio;
                std::vector <videoResource *> projectGameVideos;
                std::vector <lightResource *> projectLights2d;
                std::vector <particleResource *> projectEmitters;
                std::vector <gamePathResource *> projectGamePaths;
                std::vector <functionResource *> projectGameFunctions;
                std::vector <classResource *> projectGameClasses;
                std::vector <gameEntityResource *> projectGameEntities;
                std::vector <gameSceneResource *> projectScenes;
                std::vector <fontResource *> projectGameFonts;

                //makes sure the file is in good condition and the version is still unloaded
                gpe::error_log->report("Loading in project file and processing version number...");
                while ( newprofileFile.good() )
                {
                    getline (newprofileFile,currLine); //gets the next line of the file
                    currLine = stg_ex::trim_left_inplace(currLine);
                    currLine = stg_ex::trim_right_inplace(currLine);

                    if(!currLine.empty() )
                    {
                        // skips comment lines
                        if( currLine[0]!= '#' && currLine[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLine.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                key_string = currLine.substr(0,equalPos);
                                key_string= stg_ex::string_lower( key_string );
                                valstring = currLine.substr(equalPos+1,currLine.length());
                                //finds the version of the game level file
                                if (key_string== "version")
                                {
                                    projectFilePreviousVersion = projectFileVersion = stg_ex::string_to_float(valstring,1);

                                    if( !semath::compare_floats(projectFileVersion, gpe::version_number_total) && projectFileVersion < gpe::version_number_total )
                                    {
                                        if( pawgui::display_prompt_message("Project Version Warning!","The following project version varies from the current version of this editor. Are you sure you will like to import this. Please note saving/loading may experience difficulties we recommend you back up these project files/folders before importing...")!=pawgui::display_query_yes )
                                        {
                                            newprofileFile.close();
                                            main_editor_log->log_general_line("Project import canceled ( Older version )");
                                            return false;
                                        }
                                        else
                                        {
                                            if( release_current_mode!=relesae_type_production )
                                            {
                                                if( pawgui::display_prompt_message("WARNING!","You are using a NON-PRODUCTION version of the editor. Are you sure you will like to continue? Potential incompatibility issues may happen in later versions.")!=pawgui::display_query_yes )
                                                {
                                                    newprofileFile.close();
                                                    main_editor_log->log_general_line("Project import canceled ( Alpha/Beta version )");
                                                    return false;
                                                }
                                            }
                                            main_editor_log->log_general_line("Attempting to open older project...");
                                        }
                                    }
                                    else if( release_current_mode!=relesae_type_production )
                                    {
                                        if( pawgui::display_prompt_message("WARNING!","You are using a NON-PRODUCTION version of the editor. Are you sure you will like to continue? Potential incompatibility issues may happen in later versions.")!=pawgui::display_query_yes )
                                        {
                                            newprofileFile.close();
                                            main_editor_log->log_general_line("Project import canceled ( Alpha/Beta version )");
                                            return false;
                                        }
                                    }
                                }
                                else if( key_string=="count" || key_string=="resourcescount")
                                {
                                    tempStrToInt = stg_ex::string_to_int(valstring);
                                    if( tempStrToInt> 1000)
                                    {
                                        GLOBAL_REZ_ID_COUNT = tempStrToInt;
                                    }
                                }
                                else if(key_string=="projectname" || key_string=="name")
                                {
                                    projectName = valstring;
                                    RESC_PROJECT_FOLDER->set_name(projectName);
                                }
                                else if(key_string=="projectlanguage" || key_string=="programmingLanguage" || key_string=="codinglanguage")
                                {
                                    myProjectLanguage = pawgui::main_syntax_highlighter->get_language_id_from_name( valstring);
                                    if( myProjectLanguage < 0 )
                                    {
                                        set_project_language_id( pawgui::program_language_cpp );
                                    }
                                }
                                else if( key_string == "projecticon")
                                {
                                    projectIconName = valstring;
                                }
                                else if(key_string == "[superfolder")
                                {
                                    endBracketPos=currLine.find_first_of("]");
                                    if(endBracketPos!=(int)std::string::npos)
                                    {
                                        valstring = currLine.substr(equalPos+1,endBracketPos-equalPos-1);
                                        if( (int)valstring.size()>0)
                                        {
                                            tempResFolder = RESC_PROJECT_FOLDER->find_resource_from_name(valstring,false);
                                            if( tempResFolder!=nullptr)
                                            {
                                                containerFolderToEdit = current_color_resFolder = tempResFolder;
                                                tempResFolder = nullptr;
                                            }
                                        }
                                    }
                                }
                                else if(key_string == "[folder")
                                {
                                    valstring = currLine.substr(equalPos+1,endBracketPos-equalPos-1);
                                    if( (int)valstring.size()>0 && containerFolderToEdit!=nullptr)
                                    {
                                        endBracketPos=currLine.find_first_of("]");
                                        if(endBracketPos!=(int)std::string::npos)
                                        {
                                            valstring = currLine.substr(equalPos+1,endBracketPos-equalPos-1);
                                            if( (int)valstring.size()>0)
                                            {
                                                newresource_name = stg_ex::split_first_string(valstring,',');
                                                foundResGlobalId = stg_ex::string_to_int(valstring,-1);
                                                tempResFolder = containerFolderToEdit->find_resource_from_name(newresource_name,false);
                                                if( tempResFolder!=nullptr)
                                                {
                                                    containerFolderToEdit = containerFolderToEdit->find_resource_from_name(newresource_name,false);
                                                }
                                                else
                                                {
                                                    containerFolderToEdit = create_blank_folder(containerFolderToEdit,newresource_name,foundResGlobalId);
                                                }
                                            }
                                        }
                                    }
                                }
                                else if(key_string == "sprite" || key_string == "animation" )
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_animation(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempSprRes = (animationResource * )newContainer->get_held_resource();
                                        if( tempSprRes!=nullptr)
                                        {
                                            tempSprRes->resourcePostProcessed = false;
                                            projectGameanimations.push_back(tempSprRes);
                                        }
                                    }
                                }
                                else if( key_string =="texture" )
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_texture(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempTexRes = (textureResource * )newContainer->get_held_resource();
                                        if( tempTexRes!=nullptr)
                                        {
                                            tempTexRes->resourcePostProcessed = false;
                                            projectGameTextures.push_back(tempTexRes);
                                        }
                                    }
                                }
                                else if(key_string == "tilesheet")
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_tilesheet(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempTstRes = (tilesheetResource * )newContainer->get_held_resource();
                                        if( tempTstRes!=nullptr)
                                        {
                                            tempTstRes->resourcePostProcessed = false;
                                            projectGameTilesheets.push_back(tempTstRes);
                                        }
                                    }
                                }
                                else if( key_string == "audio" )
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_audio(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempAudRes = (audioResource * )newContainer->get_held_resource();
                                        if( tempAudRes!=nullptr)
                                        {
                                            tempAudRes->resourcePostProcessed = false;
                                            projectGameAudio.push_back( tempAudRes);
                                        }
                                    }
                                }
                                else if( key_string == "video" )
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_video(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempVidRes = (videoResource * )newContainer->get_held_resource();
                                        if( tempVidRes!=nullptr)
                                        {
                                            tempVidRes->resourcePostProcessed = false;
                                            projectGameVideos.push_back( tempVidRes);
                                        }
                                    }
                                }
                                else if( key_string == "light2d" )
                                {

                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    gpe::error_log->report("Adding 2d light["+tempNewresource_name+"]...");
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_light(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempLightRes = (lightResource * )newContainer->get_held_resource();
                                        if( tempLightRes!=nullptr)
                                        {
                                            tempLightRes->resourcePostProcessed = false;
                                            projectLights2d.push_back( tempLightRes);
                                        }
                                    }
                                }
                                else if( key_string == "particleemitter" || key_string == "emitter" )
                                {

                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    gpe::error_log->report("Adding particle emitter ["+tempNewresource_name+"]...");
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_emitter(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempEmitterRes = (particleResource * )newContainer->get_held_resource();
                                        if( tempEmitterRes!=nullptr)
                                        {
                                            tempEmitterRes->resourcePostProcessed = false;
                                            projectEmitters.push_back( tempEmitterRes);
                                        }
                                    }
                                }
                                else if( key_string == "path" )
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_path(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempPathRes = (gamePathResource * )newContainer->get_held_resource();
                                        if( tempPathRes!=nullptr)
                                        {
                                            tempPathRes->resourcePostProcessed = false;
                                            projectGamePaths.push_back( tempPathRes);
                                        }
                                    }
                                }
                                else if( key_string == "function" )
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_function(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempFuncRes = (functionResource * )newContainer->get_held_resource();
                                        if( tempFuncRes!=nullptr)
                                        {
                                            tempFuncRes->resourcePostProcessed = false;
                                            projectGameFunctions.push_back( tempFuncRes);
                                        }
                                    }
                                }
                                else if( key_string == "class" )
                                {

                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    gpe::error_log->report("Adding class ["+tempNewresource_name+"]...");
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_class(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempClassRes = (classResource * )newContainer->get_held_resource();
                                        if( tempClassRes!=nullptr)
                                        {
                                            tempClassRes->resourcePostProcessed = false;
                                            projectGameClasses.push_back( tempClassRes);
                                        }
                                    }
                                }
                                else if( key_string == "object"|| key_string=="gameobject"|| key_string=="game-object" || key_string=="actor" || key_string=="entity"  || key_string=="game-entity"  ||  key_string=="gameentity")
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_game_enttity(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempObjRes = (gameEntityResource * )newContainer->get_held_resource();
                                        if( tempObjRes!=nullptr)
                                        {
                                            tempObjRes->resourcePostProcessed = false;
                                            projectGameEntities.push_back(tempObjRes);
                                        }
                                    }
                                }
                                else if( key_string == "scene"|| key_string=="gamescene"|| key_string=="game-scene"  )
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_scene(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempScnRes = (gameSceneResource * )newContainer->get_held_resource();
                                        if( tempScnRes!=nullptr)
                                        {
                                            tempScnRes->scnPostProcessed = false;
                                            projectScenes.push_back(tempScnRes);
                                        }
                                    }
                                }
                                else if(key_string == "font")
                                {
                                    tempNewresource_name = stg_ex::split_first_string(valstring,',');
                                    foundResGlobalId = stg_ex::split_first_int(valstring,',');
                                    newContainer = create_blank_font(containerFolderToEdit,tempNewresource_name,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=nullptr)
                                    {
                                        tempFntRes = (fontResource * )newContainer->get_held_resource();
                                        if( tempFntRes!=nullptr)
                                        {
                                            tempFntRes->resourcePostProcessed = false;
                                            projectGameFonts.push_back(tempFntRes);
                                        }
                                    }
                                }
                            }
                            else if( stg_ex::string_lower( currLine ) == "[/folder]")
                            {
                                if( containerFolderToEdit!=nullptr)
                                {
                                    if( containerFolderToEdit->parentResource!=nullptr)
                                    {
                                        containerFolderToEdit = containerFolderToEdit->parentResource;
                                    }
                                    else
                                    {
                                        containerFolderToEdit = nullptr;
                                        tempResFolder = nullptr;
                                    }
                                }
                                else
                                {
                                    tempResFolder = nullptr;
                                }
                            }
                            else if( stg_ex::string_lower( currLine ) =="[/superfolder]")
                            {
                                containerFolderToEdit = nullptr;
                                tempResFolder = nullptr;
                            }
                        }
                    }
                }
                newprofileFile.close();

                std::string projectLayerInfoFileName =projectDirectory+"/gpe_project/project_layer_info.gpf";
                std::ifstream projectLayerInfoFile (projectLayerInfoFileName.c_str() );
                int iLayerN = 0;
                gpe::error_log->report("Version number["+ stg_ex::float_to_string( projectFilePreviousVersion )+"] found and now processing project data...");
                while ( projectLayerInfoFile.good() )
                {
                    getline (projectLayerInfoFile,currLine); //gets the next line of the file
                    currLine = stg_ex::trim_left_inplace(currLine);
                    currLine = stg_ex::trim_right_inplace(currLine);

                    if(!currLine.empty() )
                    {
                        // skips comment lines
                        if( currLine[0]!= '#' && currLine[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLine.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                key_string = currLine.substr(0,equalPos);
                                valstring = currLine.substr(equalPos+1,currLine.length());
                                for( iLayerN = 0; iLayerN < 32; iLayerN++)
                                {
                                    if( key_string == "ProjectLayerName["+ stg_ex::int_to_string(iLayerN)+"]" )
                                    {
                                        projectLayerNames[iLayerN]  = valstring;
                                    }
                                }
                            }
                        }
                    }
                }

                for( iItr = 0; iItr < (int)projectGameTextures.size(); iItr++)
                {
                    tempTexRes = projectGameTextures[iItr];
                    if( tempTexRes!=nullptr)
                    {
                        tempTexRes->load_resource();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameTilesheets.size(); iItr++)
                {
                    tempTstRes = projectGameTilesheets[iItr];
                    if( tempTstRes!=nullptr)
                    {
                        tempTstRes->load_resource();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameanimations.size(); iItr++)
                {
                    tempSprRes = projectGameanimations[iItr];
                    if( tempSprRes!=nullptr)
                    {
                        tempSprRes->load_resource();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameAudio.size(); iItr++)
                {
                    tempAudRes = projectGameAudio[iItr];
                    if( tempAudRes!=nullptr)
                    {
                        tempAudRes->load_resource();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameVideos.size(); iItr++)
                {
                    tempVidRes = projectGameVideos[iItr];
                    if( tempVidRes!=nullptr)
                    {
                        tempVidRes->load_resource();
                    }
                }

                for( iItr = 0; iItr < (int)projectGamePaths.size(); iItr++)
                {
                    tempPathRes = projectGamePaths[iItr];
                    if( tempPathRes!=nullptr)
                    {
                        tempPathRes->load_resource();
                    }
                }
                for( iItr = 0; iItr < (int)projectLights2d.size(); iItr++)
                {
                    tempLightRes = projectLights2d[iItr];
                    if( tempLightRes!=nullptr)
                    {
                        tempLightRes->load_resource();
                    }
                }
                for( iItr = 0; iItr < (int)projectEmitters.size(); iItr++)
                {
                    tempEmitterRes = projectEmitters[iItr];
                    if( tempEmitterRes!=nullptr)
                    {
                        tempEmitterRes->load_resource();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameFonts.size(); iItr++)
                {
                    tempFntRes = projectGameFonts[iItr];
                    if( tempFntRes!=nullptr)
                    {
                        tempFntRes->load_resource();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameFunctions.size(); iItr++)
                {
                    tempFuncRes = projectGameFunctions[iItr];
                    if( tempFuncRes!=nullptr)
                    {
                        tempFuncRes->load_resource();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameClasses.size(); iItr++)
                {
                    tempClassRes = projectGameClasses[iItr];
                    if( tempClassRes!=nullptr)
                    {
                        tempClassRes->load_resource();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameEntities.size(); iItr++)
                {
                    tempObjRes = projectGameEntities[iItr];
                    if( tempObjRes!=nullptr)
                    {
                        tempObjRes->load_resource();
                    }
                }
                for( iItr = 0; iItr < (int)projectScenes.size(); iItr++)
                {
                    tempScnRes = projectScenes[iItr];
                    if( tempScnRes!=nullptr)
                    {
                        tempScnRes->load_resource();
                    }
                }
                RESC_PROJECT_SETTINGS->set_project_parent_name(projectFileIn);
                RESC_PROJECT_SETTINGS->preprocess_container();
                gpe::error_log->report("Project successfully preprocessed.");
                if( (int)projectScenes.size() > 0 )
                {
                    pawgui::main_tab_resource_bar->add_new_tab( projectScenes[0] );
                    gpe::input->reset_all_input();
                }
                time( &lastTimeBackedUp);
                return true;
            }
            else
            {
                pawgui::display_user_alert("Project Open Error","Unable to open file");
            }
        }
        else
        {
            pawgui::display_user_alert("Project Open Error","Unable to process file");
        }
    }
    else
    {
        pawgui::display_user_alert("Project Open Error","Invalid File given");
    }
    return false;
}

bool GPE_ProjectFolder::check_obj_inlist(int objTypeIdIn)
{
    for( int i = 0; i < (int)currentObjParents.size(); i++)
    {
        if( currentObjParents[i]==objTypeIdIn)
        {
            return true;
        }
    }
    return false;
}

void GPE_ProjectFolder::swap_obj_lists()
{
    currentObjParents.clear();
    if( (int)nextObjParents.size() > 0)
    {
        finalObjParents.clear();
    }
    for( int i = 0; i < (int)nextObjParents.size(); i++)
    {
        currentObjParents.push_back( nextObjParents[i] );
        finalObjParents.push_back( nextObjParents[i] );
    }
    nextObjParents.clear();
}

void GPE_ProjectFolder::begin_obj_reverse_inheritence()
{
    currentObjParents.clear();
    for( int i = 0; i < (int)finalObjParents.size(); i++)
    {
        currentObjParents.push_back( finalObjParents[i] );
    }
    nextObjParents.clear();
}

void GPE_ProjectFolder::integrate_syntax()
{
    clear_project_functions();
    clear_project_keywords();
    if( RESC_PROJECT_FOLDER!=nullptr)
    {
        RESC_PROJECT_FOLDER->integrate_into_syntax();
    }
    refresh_gui_syntax();
}

bool GPE_ProjectFolder::process_build_script(std::string scriptFName, std::string templateFName, std::string outputDir )
{
    if( (int)templateFName.size() == 0 )
    {
        main_editor_log->log_build_error("Build Script template file empty for ["+scriptFName+"] script!");
        return false;
    }

    if( sff_ex::file_exists( templateFName) ==0 )
    {
        main_editor_log->log_build_error("Build Script template ["+templateFName+"] file not found!");
        return false;
    }

    if( (int)outputDir.size()==0 )
    {
        outputDir = stg_ex::file_to_dir(projectFileName)+"/gpe_project";
    }
    //if the script doesnt exist we will do a straight copy of template file to outputdir
    gpe::gpe_ini_file * scriptIniReader = new gpe::gpe_ini_file();
    if( (int)scriptFName.size() > 0 )
    {
        if( sff_ex::file_exists( scriptFName) )
        {
            main_editor_log->log_build_line("Attempting to read build script ["+scriptFName+"]...");

            if( scriptIniReader->read_ini_file( scriptFName,2 ) )
            {
                main_editor_log->log_build_line("Successfully read build script ["+scriptFName+"].");
            }
            else
            {
                main_editor_log->log_build_error("Unable to read build script ["+scriptFName+"]...");
            }
        }
        else
        {
            main_editor_log->log_build_error("Unable to find build script ["+scriptFName+"]...");
        }
    }
    else
    {
        main_editor_log->log_build_error("No build script detected....");
    }

    std::string outputExtension = scriptIniReader->find_string_keypair("Main","outputextension");
    std::string scriptOutputName = scriptIniReader->find_string_keypair("Main","outputname");

    if( (int)outputExtension.size() == 0 )
    {
        outputExtension = stg_ex::get_file_ext( templateFName);
    }

    std::string templateOutputFileName = "";

    if( (int)scriptOutputName.size()  > 0 )
    {
        templateOutputFileName =  outputDir + "/" + scriptOutputName+ "." + outputExtension;
    }
    else
    {
        templateOutputFileName =outputDir + "/" + projectName + "." + outputExtension;
    }

    std::ifstream template_inputFile( templateFName.c_str() );
    std::ofstream templateOutputFile( templateOutputFileName.c_str() );

    std::string currentLine = "";
    while ( template_inputFile.good() )
    {
        getline (template_inputFile,currentLine); //gets the next line of the file
        templateOutputFile << currentLine << "\n";
    }
    template_inputFile.close();
    templateOutputFile.close();

    if( scriptIniReader!=nullptr )
    {
        delete scriptIniReader;
        scriptIniReader = nullptr;
    }
    return true;
}

void GPE_ProjectFolder::refresh_gui_syntax()
{
    if( pawgui::main_syntax_highlighter!=nullptr )
    {
        pawgui::syntax_compiler_term * tempTerm = nullptr;
        pawgui::main_syntax_highlighter->activeProjectFunctions.clear();
        int i = 0;
        for( i = 0; i < (int)projectFunctions.size(); i++ )
        {
            tempTerm = projectFunctions[i];
            if( tempTerm!=nullptr)
            {
                pawgui::main_syntax_highlighter->activeProjectFunctions.push_back( tempTerm);
            }
        }
        pawgui::main_syntax_highlighter->activeProjectKeywords.clear();
        for( i = 0; i < (int)projectKeywords.size(); i++ )
        {
            tempTerm = projectKeywords[i];
            if( tempTerm!=nullptr)
            {
                pawgui::main_syntax_highlighter->activeProjectKeywords.push_back( tempTerm );
            }
        }
    }
}

int GPE_ProjectFolder::increment_resouce_count()
{
    GLOBAL_REZ_ID_COUNT+=1;
    return GLOBAL_REZ_ID_COUNT;
}

bool GPE_ProjectFolder::run_project(std::string projectBuildDirectory, int buildMetaTemplate,int buildBits, bool inDebugMode )
{
    if( buildMetaTemplate == gpe::system_os_html5)
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/html5";
        }
        if( sff_ex::file_exists(projectBuildDirectory+"/index.html")==true )
        {
            gpe::external_open_url(projectBuildDirectory+"/index.html");
            return true;
        }
        else
        {
            pawgui::display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate==gpe::system_os_windows && gpe::system_found_os== gpe::system_os_windows )
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/windows";
        }
        /*if( buildBits==64)
        {
            projectBuildDirectory+="_64";
        }
        else
        {
            projectBuildDirectory+="_32";
        }
        if( inDebugMode)
        {
            //projectBuildDirectory+="_debug";
        }*/
        if( sff_ex::file_exists(projectBuildDirectory+"/game.exe")==true )
        {
            gpe::external_open_program(projectBuildDirectory+"/game.exe");
            return true;
        }
        else if( sff_ex::file_exists(projectBuildDirectory+"/electron.exe")==true )
        {
            gpe::external_open_program(projectBuildDirectory+"/electron.exe");
            return true;
        }
        else
        {
            pawgui::display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate == gpe::system_os_mac && gpe::system_found_os== gpe::system_os_mac )
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/osx";
        }
        /*
        if( buildBits==64)
        {
            projectBuildDirectory+="_64";
        }
        else
        {
            projectBuildDirectory+="_32";
        }
        if( inDebugMode)
        {
            //projectBuildDirectory+="_debug";
        }
        */
        if( sff_ex::file_exists(projectBuildDirectory+"/game.app")==true )
        {
            gpe::external_open_program(projectBuildDirectory+"/game.app");
            return true;
        }
        else if( sff_ex::file_exists(projectBuildDirectory+"/electron.app")==true )
        {
            gpe::external_open_program(projectBuildDirectory+"/electron.app");
            return true;
        }
        else
        {
            pawgui::display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate== gpe::system_os_linux && gpe::system_found_os== gpe::system_os_linux )
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = stg_ex::file_to_dir(projectFileName)+"/gpe_project/builds/linux";
        }
        if( buildBits==64)
        {
            projectBuildDirectory+="_64";
        }
        else
        {
            projectBuildDirectory+="_32";
        }
        if( inDebugMode)
        {
            //projectBuildDirectory+="_debug";
        }
        if( sff_ex::file_exists(projectBuildDirectory+"/game")==true )
        {
            gpe::external_open_program(projectBuildDirectory+"/game");
            return true;
        }
        else if( sff_ex::file_exists(projectBuildDirectory+"/electron")==true )
        {
            gpe::external_open_program(projectBuildDirectory+"/electron");
            return true;
        }
        else
        {
            pawgui::display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    return false;
}

bool GPE_ProjectFolder::save_project()
{
    return save_project_as(projectFileName);
}

bool GPE_ProjectFolder::save_project_as(std::string projectFilenew_name)
{
    bool hadSaveErrors = false;
    if( (int)projectFilenew_name.size()>3)
    {
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->update_messages( "Saving Project [" + projectName + "]", "Please Wait...","DO NOT CLOSE..." );
        }

        gpe::error_log->report( "Saving Project [" + projectFileName + "]" );
        bool isSameFileName = false;
        bool saveAuthorized = false;
        if( !semath::compare_floats(projectFilePreviousVersion, gpe::version_number_total ) && projectFilePreviousVersion < gpe::version_number_total )
        {
            std::string versionComparePhrase = "The following project version varies from the current version of this editor. Are you sure you will like to save this. Please note saving/loading may experience difficulties we recommend you back up these project files/folders ELSEWHERE before saving...";
            if( pawgui::display_prompt_message("Project Version Warning!",versionComparePhrase)==pawgui::display_query_yes )
            {
                saveAuthorized = true;
            }
            else
            {
                hadSaveErrors = true;
                saveAuthorized= false;
            }
        }
        else
        {
            saveAuthorized = true;
        }

        if( saveAuthorized)
        {
            if( projectFilenew_name.compare(projectFileName)==0)
            {
                isSameFileName = true;
            }
            else
            {
                isSameFileName = false;
            }
            if( !isSameFileName)
            {
                projectFileName = projectFilenew_name;
                projectDirectory = stg_ex::get_path_from_file(projectFileName);
                std::string newProjectLocalFileName = stg_ex::get_local_from_global_file(projectFileName);
                if( (int)newProjectLocalFileName.size()>0 )
                {
                    projectDirectory = projectDirectory+"/"+projectFileName;
                    pawgui::display_user_alert("Error:",projectDirectory.c_str() );

                    setup_project_directory(projectDirectory);
                }
                else
                {
                    pawgui::display_user_alert("Error!","Unable to save project("+projectFileName+")");
                }
                if(RESC_PROJECT_FOLDER!=nullptr)
                {
                    RESC_PROJECT_FOLDER->set_project_parent_name(projectFileName);
                }
            }
            else
            {
                setup_project_directory(projectDirectory);
            }

            std::ofstream myfile (projectFileName.c_str() );
            if (myfile.is_open())
            {
                myfile << "#    --------------------------------------------------  # \n";
                myfile << "#     \n";
                myfile << "#     \n";
                myfile << "#    Game Pencil Engine Project File \n";
                myfile << "#    Created automatically via the Game Pencil Engine Editor \n";
                myfile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
                myfile << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
                myfile << "#     \n";
                myfile << "#     \n";
                myfile << "#    --------------------------------------------------  # \n";
                myfile << "Version=" << gpe::version_number_total << "\n";
                myfile << "Name=" << projectName << "\n";

                myfile << "Count=" << GLOBAL_REZ_ID_COUNT << "\n";
                if( (int)projectLanguage.size()  > 0  )
                {
                    myfile << "ProjectLanguage=" << projectLanguage << "\n";
                }
                else
                {
                    myfile << "ProjectLanguage=CPP\n";
                }
                myfile << "ProjectIcon=" << projectIconName << "\n";
                if(RESC_PROJECT_FOLDER!=nullptr)
                {
                    hadSaveErrors = RESC_PROJECT_FOLDER->write_data_into_projectfile(&myfile);
                }
                time( &lastTimeBackedUp);
                myfile.close();
            }
            else
            {
                gpe::error_log->report("Unable to open to save ["+projectFilenew_name+"].");
                hadSaveErrors = true;
            }
            std::string projectLayerInfoFileName =projectDirectory+"/gpe_project/project_layer_info.gpf";
            gpe::error_log->report("Saving project layer info to "+projectLayerInfoFileName);
            std::ofstream projectLayerInfoFile (projectLayerInfoFileName.c_str() );
            if (projectLayerInfoFile.is_open() )
            {
                //write_header_on_file(&projectLayerInfoFile);
                for( int iLayerN = 0; iLayerN < 32; iLayerN++)
                {
                    projectLayerInfoFile << "ProjectLayerName["+ stg_ex::int_to_string(iLayerN)+"]=" << projectLayerNames[iLayerN] << "\n";
                }
                projectLayerInfoFile.close();
                projectFilePreviousVersion = projectFileVersion = gpe::version_number_total;
            }
            else
            {
                gpe::error_log->report("Unable to open to save ["+projectLayerInfoFileName+"].");
                hadSaveErrors = true;
            }
        }
        else
        {
            main_editor_log->log_general_comment("Save averted for Project["+projectFileName+"](not authorized by user choice).");
        }
    }
    else
    {
        gpe::error_log->report("File named "+projectFilenew_name+" is too short and does not fit the proper format for project save feature.");
    }
    integrate_syntax();
    return hadSaveErrors;
}

bool GPE_ProjectFolder::save_project_settings()
{
    if( RESC_PROJECT_SETTINGS!=nullptr)
    {
        std::string project_settingsFileName = projectStartDirectoryName+"gpe_project/project_settings.ini";
        std::ofstream myfile (project_settingsFileName.c_str() );
        if (myfile.is_open() )
        {
            RESC_PROJECT_SETTINGS->write_data_into_projectfile(&myfile,0);
            myfile.close();
            return true;
        }
    }
    return false;
}

void GPE_ProjectFolder::set_project_language( std::string newProjectLanguage)
{
    if( (int)newProjectLanguage.size() > 0 )
    {
        pawgui::syntax_language * tLanguage  = nullptr;
        bool languageChanged = false;
        int languageMax = pawgui::main_syntax_highlighter->get_language_count();
        for( int cLanguage = 0; cLanguage < languageMax; cLanguage++ )
        {
            tLanguage = pawgui::main_syntax_highlighter->get_language_object(cLanguage);
            //checks if the found language isnt nullptr and is a coding language
            if( tLanguage!=nullptr && tLanguage->isCodingLanguage)
            {
                //if its regular name or short name matches. We end loop and change our project language.
                if( newProjectLanguage == tLanguage->languageName || newProjectLanguage == tLanguage->languageShortName )
                {
                    gpe::error_log->report("Changing project language to "+ tLanguage->languageName + "...");
                    myProjectLanguage = cLanguage;
                    projectLanguage = tLanguage->languageShortName;
                    languageChanged = true;
                    break;
                }
            }
        }
        if( !languageChanged )
        {
            gpe::error_log->report("Unable to find language ["+newProjectLanguage+"]...");
        }
        return;
    }
    gpe::error_log->report("You can not change project language to an empty name");
}

void GPE_ProjectFolder::set_project_language_id( int projectLanguageId )
{
    if( projectLanguageId >= 0 )
    {
        pawgui::syntax_language * tLanguage = pawgui::main_syntax_highlighter->get_language_object_from_id( projectLanguageId );
        //checks if the found lanuage isnt nullptr and is a coding language
        if( tLanguage!=nullptr && tLanguage->isCodingLanguage)
        {
            myProjectLanguage = projectLanguageId;
            projectLanguage = tLanguage->languageShortName;
        }
    }
}

