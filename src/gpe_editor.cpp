/*
gpe_editor.cpp
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

#include "GPE/GPE.h"
#include "gpe_editor.h"
#include "gpe_editor_start_page.h"
#include "gpe_project_resources.h"
#include "gpe_editor_helppage.h"
#include "gpe_cpp_builder_settings.h"
#include "gpe_gamecontroller_tester.h"


GPE_ProjectFolder * CURRENT_PROJECT = NULL;

GPE_Gui_Engine * GPE_MAIN_GUI = NULL;

GPE_PopUpMenu_Option * MAIN_TOOLBAR_RECENT_PROJECTS = NULL;

//generalGameResource * SELECTED_GENERAL_RESOUCE = NULL;
GPE_Texture_Base * GPE_LOGO = NULL;
GPE_Texture_Base * GPE_TEXTURE_TRANSPARENT_BG = NULL;

std::string GPE_BUILD_NAMES[GPE_BUILD_OPTIONS];


int GPE_GetMessageBoxResult(const std::string  title, const std::string  prompt, int flagType)
{
    return 0;
}

bool set_current_gpe_project_from_name( std::string newMainProjectName )
{
    bool projectChanged = false;
    if( (int)newMainProjectName.size() > 0  )
    {
        if( CURRENT_PROJECT_NAME != newMainProjectName)
        {
            projectChanged = true;
        }
        CURRENT_PROJECT_NAME = newMainProjectName;
        CURRENT_PROJECT = GPE_MAIN_GUI->find_project_from_filename(CURRENT_PROJECT_NAME);
        if( CURRENT_PROJECT!=NULL && projectChanged)
        {
            //Integrate for syntax highlighter and such...
            GPE_MAIN_GUI->process_window_title();
            CURRENT_PROJECT->refresh_gui_syntax();
        }
    }
    else if( CURRENT_PROJECT!=NULL)
    {
        projectChanged = true;
    }
    return projectChanged;
}

GPE_ProjectFolder::GPE_ProjectFolder(std::string name, std::string directoryIn, std::string fileNameIn, std::string projectLanguageIn, bool createBlankScene )
{
    projectRSM = new GPE_DataManager(gpeph->get_default_render_package(), "fileNameIn");
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
            projectLayerNames[iLayerN] = "Custom Layer"+int_to_string(iLayerN-2);
        }
    for( iLayerN = 24; iLayerN < 32; iLayerN++)
    {
        projectLayerNames[iLayerN] = "Default Tile Layer"+int_to_string(iLayerN-24);
    }


    //Defaults to JS Project Language
    myProjectLanguage = PROGRAM_LANGUAGE_JS;

    //Changes Project Language if name is found...
    if( (int)projectLanguageIn.size() > 0 )
    {
        GPE_Gui_Engine_Language * tLanguage  = NULL;
        int maxLanguageCount = GPE_MAIN_HIGHLIGHTER->get_language_count();
        for( int cLanguage = 0; cLanguage < maxLanguageCount; cLanguage++ )
        {
            tLanguage = GPE_MAIN_HIGHLIGHTER->get_language_object(cLanguage);
            //checks if the found lanuage isnt NULL and is a coding language
            if( tLanguage!=NULL && tLanguage->isCodingLanguage)
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
    projectStartDirectoryName = projectDirectory = directoryIn;
    projectFileName = fileNameIn;
    RESC_PROJECT_FOLDER = NULL;
    RESC_animationS = NULL;
    RESC_TEXTURES = NULL;
    RESC_TILESHEETS = NULL;
    RESC_3DMODELS = NULL;
    RESC_AUDIO = NULL;
    RESC_VIDEOS = NULL;
    RESC_FUNCTIONS = NULL;
    RESC_OBJECTS = NULL;
    RESC_CLASSES = NULL;
    RESC_PATHS = NULL;
    RESC_SCENES = NULL;
    RESC_ACHIEVEMENTS = NULL;
    RESC_FONTS = NULL;
    RESC_SHADERS = NULL;
    for( int i = 0; i < RESOURCE_TYPE_MAX; i++)
    {
        RESC_ALL[i] = NULL;
        CREATED_RESOURCE_COUNT[i] = 0;
    }
    RESC_PROJECT_FOLDER = GPE_MAIN_GUI->mainResourceTree->add_project_folder(0,projectFileName,projectName);
    RESC_PROJECT_FOLDER->parentProjectDirectory = projectDirectory;
    RESC_PROJECT_FOLDER->projectParentFileName = projectFileName;
    RESC_PROJECT_FOLDER->open_folder();

    RESC_TEXTURES =  RESC_ALL[RESOURCE_TYPE_TEXTURE] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_TEXTURE,"Textures", increment_resouce_count(), restype_superfolder);
    RESC_TILESHEETS =  RESC_ALL[RESOURCE_TYPE_TILESHEET] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_TILESHEET,"Tilesheets", increment_resouce_count(), restype_superfolder);
    RESC_ANIMATIONS =  RESC_ALL[RESOURCE_TYPE_ANIMATION] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_ANIMATION,"Animations", increment_resouce_count(), restype_superfolder);
    RESC_AUDIO =  RESC_ALL[RESOURCE_TYPE_AUDIO] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_AUDIO,"Audio", increment_resouce_count(), restype_superfolder);
    RESC_VIDEOS =  RESC_ALL[RESOURCE_TYPE_VIDEO] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_VIDEO,"Videos", increment_resouce_count(), restype_superfolder);
    RESC_FUNCTIONS =  RESC_ALL[RESOURCE_TYPE_FUNCTION] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_FUNCTION,"Functions", increment_resouce_count(), restype_superfolder);
    RESC_PATHS =  RESC_ALL[RESOURCE_TYPE_PATH] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_PATH,"Paths", increment_resouce_count(), restype_superfolder);
    RESC_EMITTERS =  RESC_ALL[RESOURCE_TYPE_EMITTER] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_EMITTER,"Particles", increment_resouce_count(), restype_superfolder);
    RESC_LIGHTS =  RESC_ALL[RESOURCE_TYPE_LIGHT] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_LIGHT,"Lights", increment_resouce_count(), restype_superfolder);
    RESC_OBJECTS =  RESC_ALL[RESOURCE_TYPE_OBJECT] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_OBJECT,"Objects", increment_resouce_count(), restype_superfolder);
    RESC_CLASSES =  RESC_ALL[RESOURCE_TYPE_CLASS] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_CLASS,"Classes", increment_resouce_count(), restype_superfolder);
    RESC_SCENES =  RESC_ALL[RESOURCE_TYPE_SCENE] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_SCENE,"Scenes", increment_resouce_count(), restype_superfolder);
    //RESC_ACHIEVEMENTS =  RESC_ALL[RESOURCE_TYPE_ACHIEVEMENT] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_ACHIEVEMENT,"Achievements", increment_resouce_count(), restype_superfolder);
    RESC_FONTS =  RESC_ALL[RESOURCE_TYPE_FONT] =RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_FONT,"Fonts", increment_resouce_count(), restype_superfolder);
    RESC_DICTIONARIES =  RESC_ALL[RESOURCE_TYPE_DICTIONARY]  = NULL;//=RESC_PROJECT_FOLDER->add_newtype_folder(RESOURCE_TYPE_DICTIONARY,"Dictionaries", increment_resouce_count(), restype_superfolder);

    int projectPropertiesId = increment_resouce_count();
    RESC_PROJECT_SETTINGS = new GPE_GeneralResourceContainer(projectFileName, RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PROJECT_SETTINGS], RESOURCE_TYPE_PROJECT_SETTINGS,projectPropertiesId,false, projectPropertiesId,-1);
    RESC_PROJECT_SETTINGS->parentProjectDirectory = projectDirectory;

    RESC_PROJECT_FOLDER->add_resource_container(RESC_PROJECT_SETTINGS,true);
    projectPropertiesResource * projectProps = new projectPropertiesResource(RESC_PROJECT_FOLDER);
    projectProps->set_name( RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PROJECT_SETTINGS] );
    projectProps->set_parent_project( projectFileName );
    RESC_PROJECT_SETTINGS->set_held_resource(projectProps);
    GLOBAL_REZ_ID_COUNT = 1000;
    projectFilePreviousVersion = projectFileVersion = GPE_VERSION_FLOAT_NUMBER;
    if( createBlankScene )
    {
        create_blank_scene(RESC_SCENES,"start_scene");
    }
}

GPE_ProjectFolder::~GPE_ProjectFolder()
{
    GPE_Report("Deleting RESC_PROJECT_FOLDER" );
    if( RESC_PROJECT_FOLDER!=NULL )
    {
        delete RESC_PROJECT_FOLDER;
        RESC_PROJECT_FOLDER = NULL;
    }

    if( projectRSM!=NULL )
    {
        delete projectRSM;
        projectRSM = NULL;
    }
}

bool GPE_ProjectFolder::add_project_function(std::string nName, std::string nDescription,std::string  nParameters, std::string tFunctionReturnType,std::string nScope)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)projectFunctions.size()-1; i>=0; i--)
    {
        tTerm = projectFunctions[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    tTerm = new GPE_Compiler_Term(nName, nParameters, tFunctionReturnType, nDescription, CTERM_FUNCTION,nScope);
    projectFunctions.push_back( tTerm);
    return true;
}

bool GPE_ProjectFolder::add_project_keyword(std::string nName, std::string nDescription,int nType,std::string nScope)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)projectKeywords.size()-1; i>=0; i--)
    {
        tTerm = projectKeywords[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    tTerm = new GPE_Compiler_Term(nName, "", "", nDescription, nType,nScope);
    projectKeywords.push_back( tTerm);
    return true;
}

bool GPE_ProjectFolder::check_names_against_keywords( GPE_GeneralResourceContainer * resContainer )
{
    if( resContainer == NULL)
    {
        return true;
    }
    bool allChecksCleared = true;

    if( resContainer->is_super_project_folder()== false && resContainer->is_folder() == false )
    {
        if( is_alnum( resContainer->get_name(),false,true ) )
        {
            if( !GPE_MAIN_HIGHLIGHTER->is_compilable_word( resContainer->get_name() ) )
            {
                GPE_Main_Logs->log_build_error("Invalid Resource Name Given ["+resContainer->get_name()+"] conflicts with the GPE system variable. Please rename your resource!");
                allChecksCleared = false;
            }
        }
        else
        {
            allChecksCleared = false;
            GPE_Main_Logs->log_build_error("Invalid Resource Name Given ["+resContainer->get_name()+"].");
        }
    }
    else
    {
        GPE_Main_Logs->log_build_comment("Checking ["+resContainer->get_name()+"] names.");
    }

    GPE_GeneralResourceContainer * tempContainer = NULL;
    int resLimit = resContainer->get_size();
    for( int iRes = 0; iRes < resLimit; iRes++)
    {
        tempContainer = resContainer->get_resource_at( iRes );
        if( tempContainer!=NULL)
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
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)projectFunctions.size()-1; i>=0; i--)
    {
        tTerm = projectFunctions[i];
        if( tTerm!=NULL)
        {
            delete tTerm;
            tTerm = NULL;
        }
    }
    projectFunctions.clear();
}

void GPE_ProjectFolder::clear_project_keywords()
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)projectKeywords.size()-1; i>=0; i--)
    {
        tTerm = projectKeywords[i];
        if( tTerm!=NULL)
        {
            delete tTerm;
            tTerm = NULL;
        }
    }
    projectKeywords.clear();
}

bool GPE_ProjectFolder::export_and_play_native(bool launchProgram)
{
    if( RESC_PROJECT_SETTINGS!=NULL && RESC_PROJECT_SETTINGS->get_held_resource()!=NULL)
    {
        projectPropertiesResource * projectSettingsObject = (projectPropertiesResource*)RESC_PROJECT_SETTINGS->get_held_resource();
        return projectSettingsObject->export_and_play_native(launchProgram);
    }
    return false;
}

bool GPE_ProjectFolder::clean_build_folder( int buildMetaTemplate )
{
    std::string projectBuildDirectory = "";
    std::string folderDeletionName  = "";
    if( buildMetaTemplate == GPE_BUILD_HTML5)
    {
        projectBuildDirectory  = file_to_dir(projectFileName)+"/gpe_project/builds/html5";
        folderDeletionName = "HTML5";
    }
    else if( buildMetaTemplate == GPE_BUILD_WINDOWS)
    {
        projectBuildDirectory  = file_to_dir(projectFileName)+"/gpe_project/builds/windows";
        folderDeletionName = "WINDOWS";
    }
    else if( buildMetaTemplate == GPE_BUILD_LINUX)
    {
        projectBuildDirectory  = file_to_dir(projectFileName)+"/gpe_project/builds/linux";
        folderDeletionName = "LINUX";
    }
    else if( buildMetaTemplate == GPE_BUILD_MAC)
    {
        projectBuildDirectory  = file_to_dir(projectFileName)+"/gpe_project/builds/osx";
        folderDeletionName = "OSX";
    }
    if( (int)projectBuildDirectory.size() > 0 )
    {
        if( path_exists(projectBuildDirectory) )
        {
            if( GPE_Display_Basic_Prompt("[WARNING]Function deletion of "+folderDeletionName+" build folder?","Are you sure you will like to delete all the contents of this build directory? This action is irreversible!")==DISPLAY_QUERY_YES)
            {
                GPE_Main_Logs->log_build_line("---");
                GPE_Main_Logs->log_build_line("Cleaning Project ["+projectName+" build folder:");
                GPE_Main_Logs->log_build_line("["+projectBuildDirectory+"]...");
                int filesDeleted =  clean_folder(projectBuildDirectory);
                if( filesDeleted > 0)
                {
                    GPE_Main_Logs->log_build_line("Folder content files were deleted successfully. Estimated ["+int_to_string(filesDeleted)+"] files deleted...");
                }
                else
                {
                    GPE_Main_Logs->log_build_line("0 files were prepared for deleted.");
                }
                GPE_Main_Logs->log_build_line("---");
                return filesDeleted;
            }
        }
        else if( GPE_Main_Logs!=NULL )
        {
            GPE_Main_Logs->log_build_line("---");
            GPE_Main_Logs->log_build_line("Cleaning Project ["+projectName+" build folder:");
            GPE_Main_Logs->log_build_line("["+projectBuildDirectory+"] was not detected.");
            GPE_Main_Logs->log_build_line("Cleaning process aborted.");
        }
    }

    return false;
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_folder(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    if( folderContainer!=NULL )
    {
        if( newResId < 0)
        {
            newResId = increment_resouce_count();
        }
        if( (int)newName.size() <= 0)
        {
            newName = "New Folder";
        }
        GPE_GeneralResourceContainer *  newFolder= folderContainer->add_resource_folder(newName,newResId,-1);
        newFolder->projectParentFileName = projectFileName;
        //folderContainer->open_folder();
        return newFolder;
    }
    return NULL;
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_resource(int rNewType, GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    if( rNewType >= 0 && rNewType <= RESOURCE_TYPE_ACHIEVEMENT )
    {
        GPE_GeneralResourceContainer * RES_FOLDER_HOLDER = RESC_ALL[rNewType];
        if(RES_FOLDER_HOLDER!=NULL)
        {
            CREATED_RESOURCE_COUNT[rNewType]+=1;
            int resourceNumb = CREATED_RESOURCE_COUNT[rNewType];
            if( (int)newName.size() <= 0)
            {
                newName = "new"+RESOURCE_TYPE_NAMES[rNewType]+int_to_string(resourceNumb );
            }
            standardEditableGameResource * newProjectResource;
            switch( rNewType)
            {
            case RESOURCE_TYPE_AUDIO:
                newProjectResource = new audioResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_VIDEO:
                newProjectResource = new videoResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_FONT:
                newProjectResource = new fontResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_FUNCTION:
                newProjectResource = new functionResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_CLASS:
                newProjectResource = new classResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_LIGHT:
                newProjectResource = new lightResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_OBJECT:
                newProjectResource = new gameObjectResource(RESC_PROJECT_FOLDER);
                break;
            //Added since Version 1.12 [BEGIN]
            case RESOURCE_TYPE_PATH:
                newProjectResource = new gamePathResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_EMITTER:
                newProjectResource = new particleResource(RESC_PROJECT_FOLDER);
                break;
            //Added since Version 1.12 [END]
            case RESOURCE_TYPE_SCENE:
                newProjectResource = new gameSceneResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_ANIMATION:
                newProjectResource = new animationResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_TEXTURE:
                newProjectResource = new textureResource(RESC_PROJECT_FOLDER);
                break;
            case RESOURCE_TYPE_TILESHEET:
                newProjectResource = new tilesheetResource(RESC_PROJECT_FOLDER);
                break;
            default:
                newProjectResource = NULL;
                break;
            }

            if( folderContainer==NULL )
            {
                folderContainer = RES_FOLDER_HOLDER;
            }

            if( folderContainer!=NULL )
            {
                if( newProjectResource!=NULL)
                {
                    if( newResId < 0)
                    {
                        newProjectResource->set_global_rid( increment_resouce_count() );
                    }
                    else
                    {
                        newProjectResource->set_global_rid( newResId );
                    }
                    newProjectResource->set_name( newName);
                    newProjectResource->set_parent_name( projectFileName);
                    newProjectResource->parentProjectName = projectFileName;
                    newProjectResource->parentProjectDirectory = projectDirectory;
                    newProjectResource->resourceType = rNewType;
                    GPE_GeneralResourceContainer * newContainer = new GPE_GeneralResourceContainer(projectFileName,newName,rNewType,resourceNumb,false,newProjectResource->get_global_rid() );
                    newContainer->set_held_resource(newProjectResource);
                    folderContainer->add_resource_container(newContainer);
                    //folderContainer->open_folder();
                    newContainer->parentProjectDirectory = projectDirectory;
                    return newContainer;
                }
            }
        }
    }
    return NULL;
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_audio( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_AUDIO, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_video( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_VIDEO, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_emitter( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_EMITTER, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_light( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_LIGHT, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_path( GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId  )
{
    return create_blank_resource(RESOURCE_TYPE_PATH, folderContainer, newName, newResId);
}

//
GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_font(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_FONT, folderContainer, newName, newResId);
}
//

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_function(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_FUNCTION, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_class(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_CLASS, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_animation(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_ANIMATION, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_texture(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_TEXTURE, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_tilesheet(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_TILESHEET, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_gameobject(GPE_GeneralResourceContainer * folderContainer, std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_OBJECT, folderContainer, newName, newResId);
}

GPE_GeneralResourceContainer *  GPE_ProjectFolder::create_blank_scene(GPE_GeneralResourceContainer * folderContainer,std::string newName, int newResId )
{
    return create_blank_resource(RESOURCE_TYPE_SCENE, folderContainer, newName, newResId);
}

//Export C++
bool GPE_ProjectFolder::export_project_cpp(std::string projectBuildDirectory, std::string gpeBuilderName , int buildMetaTemplate, bool runGameOnCompile, bool inDebugMode)
{
    //Generate main file.
    GPE_MAIN_HIGHLIGHTER->begin_compiling();

    GPE_Main_Logs->log_build_line("Copying GPE source files..");
    int filesCopiedCount = copy_folder(APP_DIRECTORY_NAME+"build_files/cplusplus_source",projectDirectory+"/gpe_project/source", true, true );
    if(  filesCopiedCount > 0 )
    {
        GPE_Main_Logs->log_build_line("Successfully copied GPE Source("+int_to_string( filesCopiedCount )+") files!");
    }
    else if( filesCopiedCount == -1)
    {
        GPE_Main_Logs->log_build_error("Unable to copy GPE Source files: Source Folder not found!");
    }
    else if( filesCopiedCount == -2)
    {
        GPE_Main_Logs->log_build_error("Unable to copy GPE Source files: Target Folder not found!");
    }
    else
    {
        GPE_Main_Logs->log_build_error("Unable to copy GPE Source files...");
    }

    GPE_Main_Logs->log_build_line("Auto-generating C++ file [main.cpp]..");

    std::string projectSettingsFileName = projectDirectory+"/gpe_project/source/main.cpp";
    std::ofstream newMainFile (projectSettingsFileName.c_str() );
    if (newMainFile.is_open() )
    {
        if( GPE_CPP_BUILDER_SETTINGS->write_cpp_header_file( &newMainFile, "main.cpp") == false )
        {
            GPE_Main_Logs->log_build_error("Failed auto-generating C++ file [main.cpp]'s  header info+..");
        }
        //Add in the game spice here...
        newMainFile.close();
        GPE_Main_Logs->log_build_line("Successfully auto-generated [main.cpp]..");
    }
    else
    {
        GPE_Main_Logs->log_build_error("Failed auto-generating C++ file [main.cpp] AT +["+projectSettingsFileName+"]+..");
    }

    if( (int)gpeBuilderName.size() > 0 )
    {
        GPE_Main_Logs->log_build_line("--  Initiating Build System ["+ gpeBuilderName +"] --");
    }
    else
    {
        GPE_Main_Logs->log_build_error("Unable to detect BuildSystem. Exiting build now...");
        return false;
    }
    if( GPE_CPP_BUILDER_SETTINGS == NULL)
    {
        GPE_Main_Logs->log_build_error("Unable to detect GPE_CPP_BUILDER_SETTINGS. Exciting build now...");
        return NULL;
    }

    GPE_Main_Logs->log_build_line("Seeking Build System ["+ gpeBuilderName+"]...");
    gpeCPPBuildHolder * currentBuildSystem = GPE_CPP_BUILDER_SETTINGS->find_build_system( gpeBuilderName );

    if( currentBuildSystem == NULL)
    {
        GPE_Main_Logs->log_build_error("Unable to detect BuildSystem["+gpeBuilderName+"]. Exciting build now...");
        return false;
    }

    gpeCPPSubsytemHolder * tempSubSystem = NULL;
    int cBuildSubsystemcount = (int)currentBuildSystem->mySubsystems.size();
    GPE_Main_Logs->log_build_line("Build subsystems detected ["+int_to_string(cBuildSubsystemcount)+"]..");

    std::string buildScriptFile = "";
    std::string buildTemplateFile = "";
    std::string cppBuildDir = file_to_dir(projectFileName)+"/gpe_project";
    for( int i = 0; i < cBuildSubsystemcount; i++)
    {
        tempSubSystem = currentBuildSystem->mySubsystems[i];
        if( tempSubSystem !=NULL)
        {
            GPE_Main_Logs->log_build_line("--- Initiating Build Subsystem ["+ tempSubSystem->subsystemName +"] ---");

            if( tempSubSystem->systemProgramTemplateFile!=NULL && tempSubSystem->systemProgramTemplateFile!=NULL )
            {
                buildScriptFile = tempSubSystem->systemProgramTemplateFile->get_string();
                buildTemplateFile = tempSubSystem->systemProgramTemplateFile->get_string();
                if( process_build_script( buildScriptFile, buildScriptFile, cppBuildDir ) )
                {
                    GPE_Main_Logs->log_build_line("----["+buildTemplateFile+"] template made via ["+buildScriptFile+"] script. ----");
                }
                else
                {
                    GPE_Main_Logs->log_build_error("---- Unable to create ["+buildTemplateFile+"] template file via ["+buildScriptFile+"] script. ----");
                }
            }
            else
            {
                GPE_Main_Logs->log_build_error("---- Invalid build subsystem["+ tempSubSystem->subsystemName+"] detected at position: ["+int_to_string(i)+"]... ----");
            }
        }
        else
        {
            GPE_Main_Logs->log_build_error("--- Invalid build subsystem detected at position: ["+int_to_string(i)+"]... ---");
        }
    }

    //Now prepackages game instructions for custom game...
    //Grabs the list of resources and such.
    std::vector <GPE_GeneralResourceContainer *> buildTextureOptions;
    std::vector <GPE_GeneralResourceContainer *> buildTilesheetOptions;
    std::vector <GPE_GeneralResourceContainer *> buildanimationOptions;
    std::vector <GPE_GeneralResourceContainer *> buildAudioOptions;
    std::vector <GPE_GeneralResourceContainer *> buildVideoOptions;
    std::vector <GPE_GeneralResourceContainer *> buildPathsOptions;
    std::vector <GPE_GeneralResourceContainer *> buildFunctionsOptions;
    std::vector <GPE_GeneralResourceContainer *> buildFontOptions;
    //Added as of 1.14 [ BEGIN ]
    std::vector <GPE_GeneralResourceContainer *> buildClassesOptions;
    //Added as of 1.14 [ END ]
    std::vector <GPE_GeneralResourceContainer *> buildGameObjectOptions;
    std::vector <GPE_GeneralResourceContainer *> buildGameSceneOptions;
    projectPropertiesResource * projectSettingsObject = (projectPropertiesResource*)RESC_PROJECT_SETTINGS->get_held_resource();
    RESC_TEXTURES->grab_useable_resources(buildTextureOptions);
    RESC_TILESHEETS->grab_useable_resources(buildTilesheetOptions);
    RESC_animationS->grab_useable_resources(buildanimationOptions);
    RESC_AUDIO->grab_useable_resources(buildAudioOptions);
    RESC_VIDEOS->grab_useable_resources(buildVideoOptions);
    RESC_PATHS->grab_useable_resources(buildPathsOptions);
    RESC_FUNCTIONS->grab_useable_resources(buildFunctionsOptions);
    RESC_FONTS->grab_useable_resources(buildFontOptions);
    //Added as of 1.14 [ BEGIN ]
    RESC_CLASSES->grab_useable_resources(buildClassesOptions);
    //Added as of 1.14 [ END ]
    RESC_OBJECTS->grab_useable_resources(buildGameObjectOptions);
    RESC_SCENES->grab_useable_resources(buildGameSceneOptions);

    //For Mega Looking of resources:
    std::vector <GPE_GeneralResourceContainer *> buildGameBuildAllOptions;
    RESC_TEXTURES->grab_useable_resources(buildGameBuildAllOptions);
    RESC_TILESHEETS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_animationS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_AUDIO->grab_useable_resources(buildGameBuildAllOptions);
    RESC_VIDEOS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_PATHS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_FUNCTIONS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_FONTS->grab_useable_resources(buildGameBuildAllOptions);
    //Added as of 1.14 [ BEGIN ]
    RESC_CLASSES->grab_useable_resources(buildGameBuildAllOptions);
    //Added as of 1.14 [ END ]
    RESC_OBJECTS->grab_useable_resources(buildGameBuildAllOptions);
    RESC_SCENES->grab_useable_resources(buildGameBuildAllOptions);

    currentObjParents.clear();
    nextObjParents.clear();

    return true;
}

//Export HTML5
//Build HTML5
bool GPE_ProjectFolder::export_project_html5(std::string projectBuildDirectory,  std::string gpeBuilderName , int buildMetaTemplate, bool runGameOnCompile, bool inDebugMode)
{
    GPE_GeneralResourceContainer * firstObjectContainer = RESC_OBJECTS->get_usable_resource();
    GPE_GeneralResourceContainer * firstSceneContainer = RESC_SCENES->get_usable_resource();
    bool hadSaveErrors = false;
    int BUILD_SCREEN_WIDTH = 640;
    int BUILD_SCREEN_HEIGHT = 480;

    GPE_MAIN_HIGHLIGHTER->begin_compiling();
    if( GPE_Main_Logs!=NULL)
    {
        if( buildMetaTemplate!=GPE_BUILD_WINDOWS && buildMetaTemplate!=GPE_BUILD_MAC  && buildMetaTemplate!=GPE_BUILD_LINUX )
        {
            GPE_Main_Logs->clear_build_log();
            GPE_Main_Logs->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[buildMetaTemplate]+" Export] (Compiler: Pawbitious Compiler)---------------");
        }
        GPE_Main_Logs->log_build_line("Exporting JS Based Project...");

        if( firstSceneContainer!=NULL)
        {
            std::string projectFirstLevelName = firstSceneContainer->get_name();
            if( firstObjectContainer!=NULL && GPE_MAIN_HIGHLIGHTER!=NULL)
            {

                if( projectBuildDirectory.empty() )
                {
                    projectBuildDirectory = file_to_dir(projectFileName)+"/gpe_project/builds/html5";
                }
                if( buildMetaTemplate==GPE_BUILD_HTML5)
                {
                    projectBuildDirectory = file_to_dir(projectFileName)+"/gpe_project/builds/html5";
                    GPE_MAIN_GUI->setup_js_folder(projectBuildDirectory,GPE_BUILD_HTML5,0,inDebugMode );
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

                projectPropertiesResource * projectSettingsObject = NULL;

                if( RESC_PROJECT_SETTINGS!=NULL && RESC_PROJECT_SETTINGS->get_held_resource() !=NULL)
                {
                    projectSettingsObject  = (projectPropertiesResource*)RESC_PROJECT_SETTINGS->get_held_resource();
                }

                if( projectSettingsObject == NULL)
                {
                    hadSaveErrors = true;
                }

                if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                {
                    if( projectSettingsObject->projectScreenWidthField->get_held_int() > 0)
                    {
                        BUILD_SCREEN_WIDTH = std::max(160, projectSettingsObject->projectScreenWidthField->get_held_int() );
                    }

                    if( projectSettingsObject->projectScreenHeightField->get_held_int() > 0)
                    {
                        BUILD_SCREEN_HEIGHT = std::max(120,projectSettingsObject->projectScreenHeightField->get_held_int() );
                    }
                    //temp variables for held resources and containers

                    //for preloading of surface and such

                    appendToFile(get_user_settings_folder()+"resources_check.txt","Checking all resources..");
                    appendToFile(get_user_settings_folder()+"resources_check.txt","[Textures]");

                    bool hasTextureToUse = false;
                    int amountOfObjectsCompiled = 0;

                    if( include_local_files( RESC_PROJECT_FOLDER, projectBuildDirectory,buildMetaTemplate ) )
                    {

                    }

                    //Added as of 1.14 [ BEGIN ]

                    if( buildMetaTemplate >=0 && buildMetaTemplate < GPE_BUILD_OPTIONS)
                    {
                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_messages( GPE_BUILD_NAMES[buildMetaTemplate]+" Export", "Please Wait...", "DO NOT CLOSE" );
                        }
                    }
                    else
                    {
                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_messages( "Unknown Export", "Please Wait...", "DO NOT CLOSE" );
                        }
                    }
                    //displayMessageString = projectName;

                    //GPE Engine Files
                    std::string buildFileStyleCssFilleName = APP_DIRECTORY_NAME+"build_files/css/style.css";
                    std::string indexStyleCssFileName = projectBuildDirectory+"/css/style.css";
                    if( copy_file( buildFileStyleCssFilleName, indexStyleCssFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy [style.css] file");
                    }

                    std::string buildFileGPEPowerFilleName = "";
                    std::string indexFileGPEPowerFileName = "";
                    if( inDebugMode )
                    {
                        GPE_Main_Logs->log_build_line("Using DEBUG mode...");
                        buildFileGPEPowerFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/gpepower_strict_v1_2_2_debug.txt";
                        indexFileGPEPowerFileName =projectBuildDirectory+"/js/lib/gpepower_strict_v1_2_2_debug.js";
                    }
                    else
                    {
                        GPE_Main_Logs->log_build_line("Using RELEASE mode...");
                        buildFileGPEPowerFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/gpepower_strict_v1_2_2.txt";
                        indexFileGPEPowerFileName =projectBuildDirectory+"/js/lib/gpepower_strict_v1_2_2.js";
                    }

                    if( buildMetaTemplate ==GPE_BUILD_WINDOWS || buildMetaTemplate ==GPE_BUILD_MAC || buildMetaTemplate ==GPE_BUILD_LINUX )
                    {
                        std::string buildFileElectronAddonFile = APP_DIRECTORY_NAME+"build_files/js/lib/electron_addon.txt";
                        std::string indexFileElectronAddonFile = projectBuildDirectory+"/js/lib/electron_addon.js";
                        if( copy_file( buildFileElectronAddonFile, indexFileElectronAddonFile)==false )
                        {
                            hadSaveErrors = true;
                            GPE_Main_Logs->log_build_error("Unable to copy Electron Addon");
                        }
                    }

                    if( copy_file( buildFileGPEPowerFilleName, indexFileGPEPowerFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy GPE HTML5 engine");
                    }

                    std::string buildFileGPESuperArrayFileName = APP_DIRECTORY_NAME+"build_files/js/lib/gpe_superarray.txt";
                    std::string indexFileGPESuperArrayFileName = projectBuildDirectory+"/js/lib/gpe_superarray.js";
                    if( copy_file( buildFileGPESuperArrayFileName, indexFileGPESuperArrayFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy array library");

                    }
                    std::string buildFileGPESuperStringFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/gpe_superstring.txt";
                    std::string indexFileGPESuperStringFileName = projectBuildDirectory+"/js/lib/gpe_superstring.js";
                    if( copy_file( buildFileGPESuperStringFilleName, indexFileGPESuperStringFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to superstring library");
                    }

                    /*std::string buildFileScreenfullgFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/screenfull.txt";
                    std::string indexFileScreenfullFileName = projectBuildDirectory+"/js/lib/screenfull.js";
                    if( copy_file( buildFileScreenfullgFilleName, indexFileScreenfullFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to screenfull library");
                    }*/

                    std::string buildFileGPEFrameManagerFilleName = APP_DIRECTORY_NAME+"build_files/js/lib/frame_management.txt";
                    std::string indexFileGPEFrameManagerFileName = projectBuildDirectory+"/js/lib/frame_management.js";
                    if( copy_file( buildFileGPEFrameManagerFilleName, indexFileGPEFrameManagerFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy frame management library");
                    }
                    ///GPE Intro Files

                    std::string buildFileGPELogoStartFilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_logo_start.png";
                    std::string indexFileGPELogoStartFileName = projectBuildDirectory+"/res/gpe_logo_start.png";
                    if( copy_file( buildFileGPELogoStartFilleName, indexFileGPELogoStartFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copyGPE logo.");
                    }
                    std::string buildFileIntroAudioStartAACFilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_intro.aac";
                    std::string indexFileGPEIntroAudioStartAACFileName = projectBuildDirectory+"/res/gpe_intro.aac";
                    if( copy_file( buildFileIntroAudioStartAACFilleName, indexFileGPEIntroAudioStartAACFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy gpe_intro.aac");
                    }
                    std::string buildFileIntroAudioStartMP3FilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_intro.mp3";
                    std::string indexFileIntroAudioStartMP3FileName = projectBuildDirectory+"/res/gpe_intro.mp3";
                    if( copy_file( buildFileIntroAudioStartMP3FilleName, indexFileIntroAudioStartMP3FileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy gpe_intro.mp3");
                    }
                    std::string buildFileIntroAudioStartOOGFilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_intro.ogg";
                    std::string indexFileGPEIntroAudioStartOOGFileName = projectBuildDirectory+"/res/gpe_intro.ogg";
                    if( copy_file( buildFileIntroAudioStartOOGFilleName, indexFileGPEIntroAudioStartOOGFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy gpe_intro.ogg");
                    }
                    std::string buildFileIntroAudioStartWAVFilleName = APP_DIRECTORY_NAME+"build_files/res/gpe_intro.wav";
                    std::string indexFileGPEIntroAudioStartWAVFileName = projectBuildDirectory+"/res/gpe_intro.wav";
                    if( copy_file( buildFileIntroAudioStartWAVFilleName, indexFileGPEIntroAudioStartWAVFileName)==false )
                    {
                        hadSaveErrors = true;
                        GPE_Main_Logs->log_build_error("Unable to copy gpe_intro.wav");
                    }

                    if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                    {
                        std::string indexHTML5FileName = projectBuildDirectory+"/index.html";
                        GPE_Main_Logs->log_build_line("Creating [index.html] file.");
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
                            if( buildMetaTemplate ==GPE_BUILD_HTML5 && projectSettingsObject->projectHtmlHeaderCode!=NULL)
                            {
                                if( projectSettingsObject->headerCodeBeforeGPECheckbox!=NULL && projectSettingsObject->headerCodeBeforeGPECheckbox->is_clicked() )
                                {
                                    if( projectSettingsObject->projectHtmlHeaderCode->has_content() )
                                    {
                                        projectSettingsObject->projectHtmlHeaderCode->write_data_into_file(&indexHTML5FILE,2,true);
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
                            indexHTML5FILE << projectSettingsObject->projectGameShortTitleField->get_string() << "</title> \n";
                            //Added as of 1.14 [ END ]
                            indexHTML5FILE << "        <link rel='stylesheet' type='text/css'  href='css/style.css'> \n";
                            indexHTML5FILE << "        <link rel='stylesheet' type='text/css'  href='css/gpe_custom_style.css'> \n";

                            if( buildMetaTemplate ==GPE_BUILD_WINDOWS || buildMetaTemplate ==GPE_BUILD_MAC || buildMetaTemplate ==GPE_BUILD_LINUX )
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
                            indexHTML5FILE << "        <canvas id='gpeCanvas' width='auto' height='"<< BUILD_SCREEN_HEIGHT << "' oncontextmenu='return false;'></canvas> \n";
                            if( buildMetaTemplate ==GPE_BUILD_HTML5 && projectSettingsObject->checkBoxShowPublisherInfo!=NULL)
                            {
                                if(projectSettingsObject->checkBoxShowPublisherInfo->is_clicked() )
                                {
                                    if( projectSettingsObject->projectGameTitleField->has_content() )
                                    {
                                        indexHTML5FILE << "     <h2>" << projectSettingsObject->projectGameTitleField->get_string() << "</h2>\n";
                                    }

                                    if( projectSettingsObject->projectGameDescriptionField->has_content() )
                                    {
                                        indexHTML5FILE << "     <h3>" << projectSettingsObject->projectGameDescriptionField->get_string() << "</h3>\n";
                                    }

                                    if( projectSettingsObject->projectGameNotes!=NULL)
                                    {
                                        if( projectSettingsObject->projectGameNotes->has_content() )
                                        {
                                            indexHTML5FILE << "     <p>";
                                            projectSettingsObject->projectGameNotes->write_data_into_file(&indexHTML5FILE,2,true);
                                            indexHTML5FILE << "</p>\n";
                                        }
                                    }

                                    if( projectSettingsObject->projectGamePublisherField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p>Published by " << projectSettingsObject->projectGamePublisherField->get_string() << "</p>\n";
                                    }

                                    if( projectSettingsObject->projectGameDeveloperField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p>Developed by " << projectSettingsObject->projectGameDeveloperField->get_string() << "</p>\n";
                                    }

                                    if( projectSettingsObject->projectGameCopyrightField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p>Copyright " << projectSettingsObject->projectGameCopyrightField->get_string() << " "<<projectSettingsObject->projectGameDateField->get_string() <<"</p>\n";
                                    }

                                    if( projectSettingsObject->projectGameWebsiteField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p> <a ahref='" << projectSettingsObject->projectGameWebsiteField->get_string() << "'>For more information on game click here</a></p>\n";
                                    }

                                    if( projectSettingsObject->projectGameEmailField->has_content() )
                                    {
                                        indexHTML5FILE << "     <p> Email <a ahref='mailto:" << projectSettingsObject->projectGameEmailField->get_string() << "?Subject=Hello'>"<< projectSettingsObject->projectGameEmailField->get_string() << " for assistance</a></p>\n";
                                    }
                                }
                            }

                            if( buildMetaTemplate==GPE_BUILD_HTML5 && projectSettingsObject->warnOnCloseCheckbox->is_clicked() )
                            {
                                indexHTML5FILE << "<script type='text/javascript'>\n";
                                indexHTML5FILE << "window.onbeforeunload = function () {return 'Are you sure that you want to leave this page? Game Data may be lost';}\n";
                                indexHTML5FILE << "</script>\n";
                            }
                            indexHTML5FILE << "    </body>\n";
                            indexHTML5FILE << "</html> \n";

                            indexHTML5FILE.close();
                            std::string indexGameSettingsFileName = projectBuildDirectory+"/js/gpe_settings.js";
                            std::ofstream indexJSSettingsFILE ( indexGameSettingsFileName.c_str() );
                            if (indexJSSettingsFILE.is_open())
                            {
                                indexJSSettingsFILE << "/*     \n";
                                indexJSSettingsFILE << "#    --------------------------------------------------  #\n";
                                indexJSSettingsFILE << "#       \n";
                                indexJSSettingsFILE << "#       \n";
                                indexJSSettingsFILE << "#  Game Pencil Engine Game File \n";
                                indexJSSettingsFILE << "#  Created automatically via the Game Pencil Engine Editor \n";
                                indexJSSettingsFILE << "#  Warning: Manually editing this file may cause unexpected bugs and errors. \n";
                                indexJSSettingsFILE << "#  If you have any problems reading this file please report it to help@pawbyte.com . \n";
                                indexJSSettingsFILE << "#     \n";
                                indexJSSettingsFILE << "#      \n";
                                indexJSSettingsFILE << "#    --------------------------------------------------  # \n";
                                indexJSSettingsFILE << "*/     \n\n";
                                indexJSSettingsFILE << "var game = game || {};\n \n";
                                int fpsValue = projectSettingsObject->projectSettingsFPSRate->get_selected_value();
                                if( fpsValue > 0)
                                {
                                    indexJSSettingsFILE << "GPE_STANDARD_FPS ="+int_to_string(fpsValue)+";\n \n";
                                }
                                else
                                {
                                    indexJSSettingsFILE << "GPE_STANDARD_FPS = 60;\n \n";
                                    indexJSSettingsFILE << "GPE_FPS_UNCAPPED = true;\n \n";
                                }
                                indexJSSettingsFILE << "GPE_STANDARD_INTERVAL =  1000/GPE_STANDARD_FPS ; \n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_APP_LOGO_LOCATION = 'res/gpe_logo_start.png'; \n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_START_SCENE = '"+projectFirstLevelName+"'; \n";


                                indexJSSettingsFILE << "var GPE_SETTINGS_SCREEN_WIDTH = " << BUILD_SCREEN_WIDTH << ";\n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_SCREEN_HEIGHT = " << BUILD_SCREEN_HEIGHT << ";\n";

                                indexJSSettingsFILE << "var GPE_SETTINGS_PROG_NAME = '" << projectSettingsObject->projectGameTitleField->get_string() << "';\n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_VERSION_NUMBER = "<< projectSettingsObject->projectGameVersionField->get_string()<< ";\n";
                                //indexJSSettingsFILE << "var GPE_MAX_OBJECT_TYPES = "<< (int)buildGameObjectOptions.size() << ";\n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_SYSTEM_OS = 'Browser';\n";
                                indexJSSettingsFILE << "var GPE_GAME_PUBLISHER = '" << projectSettingsObject->projectGamePublisherField->get_string()<< "';\n";
                                indexJSSettingsFILE << "var GPE_GAME_DEVELOPER = '" << projectSettingsObject->projectGameDeveloperField->get_string() << "';\n";
                                //indexJSSettingsFILE << "var GPE_GAME_OBJECTS_COUNT = "+int_to_string( (int)buildGameObjectOptions.size() )+";\n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_IS_DEBUGGING = false;\n";
                                indexJSSettingsFILE << "var GPE_SETTINGS_SHOW_FPS = true;\n";

                                indexJSSettingsFILE << "var GPE_GAME_SCALING_SETTING = " << projectSettingsObject->projectScaleSettings->get_selected_value()<< ";\n";


                                indexJSSettingsFILE << "var GPE_PreloadImageResource = [];\n";
                                int preloadCount = 0;

                                int preloadAudioCount[SUPPORTED_AUDIO_FORMAT_COUNT];
                                preloadAudioCount[aacFileName] = 0;
                                preloadAudioCount[mp3FileName] = 0;
                                preloadAudioCount[oggFileName] = 0;
                                preloadAudioCount[wavFileName] = 0;

                                if( RESC_PROJECT_SETTINGS!=NULL )
                                {
                                    indexJSSettingsFILE << "var Game_LayerMatrix = [];\n";
                                    indexJSSettingsFILE << "var Game_SelfCollideLayerList = [];\n";
                                    projectPropertiesResource * tProjectProps = (projectPropertiesResource *)RESC_PROJECT_SETTINGS->get_held_resource();
                                    if( tProjectProps!=NULL)
                                    {
                                        int jLayerCol = 0;
                                        for( int iLayerRow = 0; iLayerRow < 32; iLayerRow++)
                                        {
                                            indexJSSettingsFILE << "Game_LayerMatrix.push( [] );\n";
                                            for(  jLayerCol = 0; jLayerCol < 32-iLayerRow; jLayerCol++)
                                            {
                                                if( iLayerRow==jLayerCol)
                                                {
                                                    if( tProjectProps->projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->is_clicked() )
                                                    {
                                                        indexJSSettingsFILE << "Game_SelfCollideLayerList.push(1);\n";
                                                    }
                                                    else
                                                    {
                                                        indexJSSettingsFILE << "Game_SelfCollideLayerList.push(0);\n";
                                                    }
                                                }
                                                else if( tProjectProps->projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->is_clicked() )
                                                {
                                                    indexJSSettingsFILE << "Game_LayerMatrix["+int_to_string(iLayerRow)+"].push("+int_to_string( jLayerCol )+");\n";
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        indexJSSettingsFILE << "//RESC_PROJECT_SETTINGS local not reachable...\n";
                                    }
                                }
                                else
                                {
                                    indexJSSettingsFILE << "//RESC_PROJECT_SETTINGS not reachable...\n";
                                }

                                indexJSSettingsFILE.close();
                            }
                            else
                            {
                                hadSaveErrors = true;
                                GPE_Main_Logs->log_build_error("Unable to create [gpe_settings.js]...[Build canceled]");
                            }

                            if( buildMetaTemplate==GPE_BUILD_WINDOWS || buildMetaTemplate==GPE_BUILD_MAC || buildMetaTemplate==GPE_BUILD_LINUX )
                            {
                                std::string indexJsonPackageFileName = projectBuildDirectory+"/package.json";
                                std::ofstream indexJsonPackageFile ( indexJsonPackageFileName.c_str() );
                                GPE_Main_Logs->log_build_line("Creating [package.json] file...");
                                if (indexJsonPackageFile.is_open())
                                {
                                    indexJsonPackageFile << "{\n";
                                    indexJsonPackageFile << "\"name\"             :   \"" << projectSettingsObject->projectGameShortTitleField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"productName\"      :   \"" << projectSettingsObject->projectGameTitleField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"description\"      :   \"" << projectSettingsObject->projectGameDescriptionField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"version\"          :   \"" << projectSettingsObject->projectGameVersionField->get_string() << "\",\n";
                                    indexJsonPackageFile << "\"main\"             :   \"main.js\"\n";
                                    indexJsonPackageFile << "}\n";
                                    indexJsonPackageFile.close();
                                }
                                else
                                {
                                    hadSaveErrors = true;
                                    GPE_Main_Logs->log_build_error("Unable to create [package.json]...[Build canceled]");
                                }
                            }

                            std::string indexJSCustomGameFileName = projectBuildDirectory+"/js/gpe_app.js";
                            std::ofstream indexJSCustomGameFILE ( indexJSCustomGameFileName.c_str() );
                            GPE_Main_Logs->log_build_line("Creating [gpe_app.js] file...");
                            if (indexJSCustomGameFILE.is_open())
                            {
                                indexJSCustomGameFILE << "/*     \n";
                                indexJSCustomGameFILE << "#    --------------------------------------------------  #\n";
                                indexJSCustomGameFILE << "#       \n";
                                indexJSCustomGameFILE << "#       \n";
                                indexJSCustomGameFILE << "#  Game Pencil Engine Game File \n";
                                indexJSCustomGameFILE << "#  Created automatically via the Game Pencil Engine Editor Version "<< int_to_string(GPE_VERSION_FLOAT_NUMBER) << "\n";
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

                                GPE_GeneralResourceContainer * tempContainer = NULL;
                                generalGameResource * tempHeldResource = NULL;
                                int maxResources = RESC_PROJECT_FOLDER->get_size();

                                for( iRes = 0; iRes < maxResources; iRes++)
                                {
                                    tempContainer = RESC_PROJECT_FOLDER->get_resource_at( iRes );
                                    if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL )
                                    {
                                        tempHeldResource = tempContainer->get_held_resource();
                                        tempHeldResource->build_intohtml5_file(&indexJSCustomGameFILE,0);
                                    }
                                }

                                indexJSCustomGameFILE << "\n";

                                //Closes the custom gpe_app.js file
                                indexJSCustomGameFILE.close();
                                GPE_Report("Building CSS file..");
                                if( !hadSaveErrors || GPE_JS_COMPILER_SETTINGS->stopCompileOnError->is_clicked()==false )
                                {
                                    std::string indexCustomCSSGameFileName = projectBuildDirectory+"/css/gpe_custom_style.css";
                                    std::ofstream indexCustomCSSGameFile ( indexCustomCSSGameFileName.c_str() );
                                    GPE_Main_Logs->log_build_line("Creating [gpe_custom_styles.css] file...");
                                    if (indexCustomCSSGameFile.is_open())
                                    {
                                        indexCustomCSSGameFile << "\n";
                                        indexCustomCSSGameFile <<"body\n{\n   background-color: #" << projectSettingsObject->projectWebsiteBackgroundColor->get_hex_string() << ";\n   color: #" << projectSettingsObject->projectTextParagraphColor->get_hex_string()<< ";\n width: 100%;\n height: 100%;\n}\n";
                                        indexCustomCSSGameFile <<"canvas\n{\nborder-color: #" << projectSettingsObject->projectBorderColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, h12\n{\n  color: #" << projectSettingsObject->projectTextHeaderColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"p\n{\n  color: #" << projectSettingsObject->projectTextParagraphColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a,a:link\n{\n     color: #" << projectSettingsObject->projectTextLinkColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a:active\n{\n     color: #" << projectSettingsObject->projectTextLinkActiveColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a:hover\n{\n     color: #" << projectSettingsObject->projectTextLinkHoverColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"a:visited\n{\n     color: #" << projectSettingsObject->projectTextLinkVisitedColor->get_hex_string() << ";\n}\n";
                                        indexCustomCSSGameFile <<"\n\n";

                                        indexCustomCSSGameFile.close();
                                    }
                                    else
                                    {
                                        hadSaveErrors = true;
                                        GPE_Main_Logs->log_build_error("Project Build Error: Unable to create [gpe_custom_style.css]..");
                                    }
                                }
                                //Final check for build errors
                                if( !hadSaveErrors)
                                {
                                    GPE_Main_Logs->log_build_line("Game built with no detected errors.");
                                    if( runGameOnCompile && buildMetaTemplate==GPE_BUILD_HTML5)
                                    {
                                        GPE_Main_Logs->log_build_line("Opening... "+indexHTML5FileName+" .");
                                        GPE_OpenURL(indexHTML5FileName);
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
                                GPE_Main_Logs->log_build_error("Unable to create [gpe_app.js]..");
                            }
                        }
                        else
                        {
                            hadSaveErrors = true;
                            display_user_alert("Project Build Error!","Unable to create game [index.html] file...");
                            GPE_Main_Logs->log_build_error("Unable to create game [index.html] file...");
                        }
                        if(RESC_PROJECT_FOLDER!=NULL)
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
                GPE_Main_Logs->log_build_error("At least one game object must exist in project...[Build canceled]");
            }
        }
        else
        {
            hadSaveErrors = true;
            GPE_Main_Logs->log_build_error("At least one game scene must exist in project...[Build cancelede]");
        }
    }
    if( hadSaveErrors)
    {
        GPE_Main_Logs->log_build_error("Unable to build game!");
        display_user_alert("Project Build Error!","Unable to build game! Please see error log...");
    }
    return hadSaveErrors;
}

bool GPE_ProjectFolder::export_project_windows(std::string projectBuildDirectory, std::string gpeBuilderName , int buildBits, bool runGameOnCompile, bool inDebugMode, int nativeBuildType )
{
    bool buildResult = false;
    if( GPE_Main_Logs!=NULL)
    {
        GPE_Main_Logs->clear_build_log();
        GPE_Main_Logs->log_build_line("-------------- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[GPE_BUILD_WINDOWS]+" Export] (Compiler: Pawbitious Builder)---------------");

        if( (int)projectBuildDirectory.size() < 3)
        {
            projectBuildDirectory = file_to_dir(projectFileName)+"/gpe_project/builds/windows";
        }
        std::string pBuildDirectory = "";
        //Check if we are using the C++ language and proceed as follows
        GPE_Main_Logs->log_build_line("- Creating Windows Output environment... -");

        if( myProjectLanguage == PROGRAM_LANGUAGE_CPP )
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Exporting ["+projectName+"][Windows]", "Please wait...", "DO NOT CLOSE");
            }

            pBuildDirectory = GPE_MAIN_GUI->setup_cpp_folder(projectBuildDirectory,GPE_BUILD_WINDOWS,buildBits,inDebugMode);

            buildResult = export_project_cpp( pBuildDirectory, gpeBuilderName,GPE_BUILD_WINDOWS,runGameOnCompile, inDebugMode );
        }
        else if( nativeBuildType!=Native_None)
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Exporting ["+projectName+"][Windows]", "Please wait...", "DO NOT CLOSE");
            }

            pBuildDirectory = GPE_MAIN_GUI->setup_js_folder(projectBuildDirectory,GPE_BUILD_WINDOWS,buildBits,inDebugMode,nativeBuildType);
            buildResult =  export_project_html5( pBuildDirectory, gpeBuilderName,GPE_BUILD_WINDOWS,runGameOnCompile, inDebugMode );

            if( runGameOnCompile && GPE_FOUND_OS==GPE_IDE_WINDOWS)
            {
                run_project(projectBuildDirectory,GPE_IDE_WINDOWS,buildBits,inDebugMode);
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
                if( file_exists(pBuildDirectory+"/../../game"+appAddonFileName+".exe") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../game"+appAddonFileName+".exe");
                }
                else if( file_exists(pBuildDirectory+"/../../electron"+appAddonFileName+".exe") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../electron"+appAddonFileName+".exe");
                }
                */
            }
        }
        else
        {
            GPE_Main_Logs->open_build_log();
            GPE_Main_Logs->log_build_error("Project not built. Native Build Type NOT Selected");
        }
    }
    else
    {
        GPE_Report("Unable to build game. Internal Log error");
    }
    return buildResult;
}

bool GPE_ProjectFolder::export_project_osx(std::string projectBuildDirectory,  std::string gpeBuilderName , int buildBits, bool runGameOnCompile, bool inDebugMode, int nativeBuildType)
{
    bool buildResult = false;
    if( GPE_Main_Logs!=NULL)
    {
        GPE_Main_Logs->clear_build_log();
        GPE_Main_Logs->log_build_line("- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[GPE_BUILD_MAC]+" Export] (Compiler: Pawbitious Builder)- ");

        if( (int)projectBuildDirectory.size() < 3)
        {
            projectBuildDirectory = file_to_dir(projectFileName)+"/gpe_project/builds/osx";
        }
        std::string pBuildDirectory = "";
        //Check if we are using the C++ language and proceed as follows
        GPE_Main_Logs->log_build_line("-- Creating OSX Output environment... --");

        if( myProjectLanguage == PROGRAM_LANGUAGE_CPP )
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Exporting ["+projectName+"][Windows]", "Please wait...", "DO NOT CLOSE");
            }


            pBuildDirectory = GPE_MAIN_GUI->setup_cpp_folder(projectBuildDirectory,GPE_BUILD_MAC,buildBits,inDebugMode );

            if( myProjectLanguage == PROGRAM_LANGUAGE_CPP )
            {
                buildResult = export_project_cpp( pBuildDirectory, gpeBuilderName, GPE_BUILD_MAC,runGameOnCompile, inDebugMode );
            }

            if( runGameOnCompile && GPE_FOUND_OS==GPE_IDE_MAC)
            {
                run_project(projectBuildDirectory,GPE_IDE_MAC,buildBits,inDebugMode);
            }
        }
        else if( nativeBuildType!=Native_None)
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Exporting ["+projectName+"][OSX]", "Please wait...", "DO NOT CLOSE");
            }

            pBuildDirectory  = GPE_MAIN_GUI->setup_js_folder(projectBuildDirectory,GPE_BUILD_MAC,buildBits,inDebugMode, nativeBuildType);
            buildResult =  export_project_html5( pBuildDirectory, gpeBuilderName, GPE_BUILD_MAC, runGameOnCompile, inDebugMode );
            if( runGameOnCompile && GPE_FOUND_OS==GPE_IDE_MAC)
            {
                run_project(projectBuildDirectory,GPE_IDE_MAC,buildBits,inDebugMode);

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
                if( file_exists(pBuildDirectory+"/../../game"+appAddonFileName+".app") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../game"+appAddonFileName+".app");
                }
                else if( file_exists(pBuildDirectory+"/../../Game"+appAddonFileName+".app") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../Game"+appAddonFileName+".app");
                }
                else if(file_exists(pBuildDirectory+"/../../Electron"+appAddonFileName+".app") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../Electron"+appAddonFileName+".app");
                }
                else if(file_exists(pBuildDirectory+"/../../electron"+appAddonFileName+".app") )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../electron"+appAddonFileName+".app");
                }
                */
            }
        }
        else
        {
            GPE_Main_Logs->open_build_log();
            GPE_Main_Logs->log_build_error("Project not built. Native Build Type NOT Selected");
        }
    }
    else
    {
        GPE_Report("Unable to build game. Internal Log error");
    }
    return buildResult;
}

bool GPE_ProjectFolder::export_project_linux(std::string projectBuildDirectory,  std::string gpeBuilderName , int buildBits, bool runGameOnCompile, bool inDebugMode, int nativeBuildType)
{
    bool buildResult = false;
    if( GPE_Main_Logs!=NULL)
    {
        GPE_Main_Logs->clear_build_log();
        GPE_Main_Logs->log_build_line("- Building: ["+projectName+"] ["+GPE_BUILD_NAMES[GPE_BUILD_MAC]+" Export] (Compiler: Pawbitious Builder) --");

        if( (int)projectBuildDirectory.size() < 3)
        {
            projectBuildDirectory = file_to_dir(projectFileName)+"/gpe_project/builds/linux";
        }
        std::string pBuildDirectory = "";
        //Check if we are using the C++ language and proceed as follows
        GPE_Main_Logs->log_build_line("-- Creating Linux Output environment... --");

        if( myProjectLanguage == PROGRAM_LANGUAGE_CPP )
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Exporting ["+projectName+"][Windows]", "Please wait...", "DO NOT CLOSE");
            }


            pBuildDirectory = GPE_MAIN_GUI->setup_cpp_folder(projectBuildDirectory,GPE_BUILD_LINUX,buildBits,inDebugMode );

            if( myProjectLanguage == PROGRAM_LANGUAGE_CPP )
            {
                buildResult = export_project_cpp( pBuildDirectory, gpeBuilderName, GPE_BUILD_LINUX,runGameOnCompile, inDebugMode );
            }

            if( runGameOnCompile && GPE_FOUND_OS==GPE_IDE_LINUX)
            {
                run_project(projectBuildDirectory,GPE_IDE_LINUX,buildBits,inDebugMode);
            }
        }
        else if( nativeBuildType!=Native_None)
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->update_messages( "Exporting ["+projectName+"][Linux]", "Please wait...", "DO NOT CLOSE");
            }

            pBuildDirectory  = GPE_MAIN_GUI->setup_js_folder(projectBuildDirectory,GPE_BUILD_LINUX,buildBits,inDebugMode, nativeBuildType);
            buildResult =  export_project_html5( pBuildDirectory, gpeBuilderName,GPE_BUILD_LINUX, runGameOnCompile, inDebugMode);

            if( runGameOnCompile && GPE_FOUND_OS==GPE_IDE_LINUX )
            {
                run_project(projectBuildDirectory,GPE_IDE_LINUX,buildBits,inDebugMode);
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
                if( file_exists(pBuildDirectory+"/../../game"+appAddonFileName) )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../game"+appAddonFileName);
                }
                else if( file_exists(pBuildDirectory+"/../../electron"+appAddonFileName) )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../electron"+appAddonFileName);
                }
                else if( file_exists(pBuildDirectory+"/../../Electron"+appAddonFileName) )
                {
                    GPE_OpenProgram(pBuildDirectory+"/../../Electron"+appAddonFileName);
                }
                */
            }
        }
        else
        {
            GPE_Main_Logs->open_build_log();
            GPE_Main_Logs->log_build_error("Project not built. Native Export Mode not selected");
        }
    }
    else
    {
        GPE_Report("Unable to build game. Internal Log error");
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
    if( RESC_PROJECT_FOLDER!=NULL)
    {
        return RESC_PROJECT_FOLDER->get_resource_count();
    }
    return 0;
}

bool GPE_ProjectFolder::include_local_files( GPE_GeneralResourceContainer * resContainer,std::string pBuildDir , int buildType )
{
    if( resContainer == NULL)
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
        GPE_Main_Logs->log_build_comment("Including ["+resContainer->get_name()+"] files.");
    }

    GPE_GeneralResourceContainer * tempContainer = NULL;
    int resLimit = resContainer->get_size();
    for( int iRes = 0; iRes < resLimit; iRes++)
    {
        tempContainer = resContainer->get_resource_at( iRes );
        if( tempContainer!=NULL)
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
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_messages( "Loading Project [" + projectName + "]", "Opening Project File","DO NOT CLOSE..." );
    }

    if( (int)projectFileIn.size() > 0)
    {
        std::ifstream newprofileFile( projectFileIn.c_str() );

        //Checks and/or makes all of the sub-directories for the folder.
        if( GPE_MAIN_GUI!=NULL)
        {
            GPE_MAIN_GUI->setup_project_directory(projectDirectory);
        }

        //If the level file could be loaded
        if( !newprofileFile.fail() )
        {
            //makes sure the file is open
            if (newprofileFile.is_open())
            {
                projectFileName = projectFileIn;
                set_current_gpe_project_from_name( CURRENT_PROJECT_NAME = projectFileIn );
                if( GPE_LOADER != NULL )
                {
                    GPE_LOADER->update_submessages( "Reading Project File","DO NOT CLOSE..." );
                }
                GPE_GeneralResourceContainer * currentResFolder = NULL;
                GPE_GeneralResourceContainer * tempResFolder = NULL;
                GPE_GeneralResourceContainer * containerFolderToEdit = NULL;
                GPE_GeneralResourceContainer * newContainer = NULL;
                animationResource * tempSprRes = NULL;
                textureResource * tempTexRes = NULL;
                tilesheetResource * tempTstRes = NULL;
                audioResource * tempAudRes = NULL;
                videoResource * tempVidRes = NULL;
                gamePathResource * tempPathRes = NULL;
                lightResource * tempLightRes = NULL;
                particleResource * tempEmitterRes = NULL;
                functionResource * tempFuncRes = NULL;
                classResource *    tempClassRes = NULL;
                gameObjectResource * tempObjRes = NULL;
                gameSceneResource * tempScnRes = NULL;
                //achievementResource * tempAchRes = NULL;
                fontResource * tempFntRes = NULL;
                std::string firstChar="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string newResName= "";
                std::string tempNewResName = "";
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
                std::vector <lightResource *> projectLights2D;
                std::vector <particleResource *> projectEmitters;
                std::vector <gamePathResource *> projectGamePaths;
                std::vector <functionResource *> projectGameFunctions;
                std::vector <classResource *> projectGameClasses;
                std::vector <gameObjectResource *> projectGameObjects;
                std::vector <gameSceneResource *> projectScenes;
                std::vector <fontResource *> projectGameFonts;

                //makes sure the file is in good condition and the version is still unloaded
                GPE_Report("Loading in project file and processing version number...");
                while ( newprofileFile.good() )
                {
                    getline (newprofileFile,currLine); //gets the next line of the file
                    currLine = trim_left_inplace(currLine);
                    currLine = trim_right_inplace(currLine);

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
                                keyString = currLine.substr(0,equalPos);
                                keyString = string_lower( keyString );
                                valString = currLine.substr(equalPos+1,currLine.length());
                                //finds the version of the game level file
                                if (keyString== "version")
                                {
                                    projectFilePreviousVersion = projectFileVersion = string_to_float(valString,1);

                                    if( !gpemath::compare_floats(projectFileVersion, GPE_VERSION_FLOAT_NUMBER) && projectFileVersion < GPE_VERSION_FLOAT_NUMBER )
                                    {
                                        if( GPE_Display_Basic_Prompt("Project Version Warning!","The following project version varies from the current version of this editor. Are you sure you will like to import this. Please note saving/loading may experience difficulties we recommend you back up these project files/folders before importing...")!=DISPLAY_QUERY_YES )
                                        {
                                            newprofileFile.close();
                                            GPE_Main_Logs->log_general_line("Project import canceled ( Older version )");
                                            return false;
                                        }
                                        else
                                        {
                                            if( GPE_MAIN_GUI->editorReleaseType!=RELEASE_TYPE_PRODUCTION )
                                            {
                                                if( GPE_Display_Basic_Prompt("WARNING!","You are using a NON-PRODUCTION version of the editor. Are you sure you will like to continue? Potential incompatibility issues may happen in later versions.")!=DISPLAY_QUERY_YES )
                                                {
                                                    newprofileFile.close();
                                                    GPE_Main_Logs->log_general_line("Project import canceled ( Alpha/Beta version )");
                                                    return false;
                                                }
                                            }
                                            GPE_Main_Logs->log_general_line("Attempting to open older project...");
                                        }
                                    }
                                    else if( GPE_MAIN_GUI->editorReleaseType!=RELEASE_TYPE_PRODUCTION )
                                    {
                                        if( GPE_Display_Basic_Prompt("WARNING!","You are using a NON-PRODUCTION version of the editor. Are you sure you will like to continue? Potential incompatibility issues may happen in later versions.")!=DISPLAY_QUERY_YES )
                                        {
                                            newprofileFile.close();
                                            GPE_Main_Logs->log_general_line("Project import canceled ( Alpha/Beta version )");
                                            return false;
                                        }
                                    }
                                }
                                else if( keyString=="count" || keyString=="resourcescount")
                                {
                                    tempStrToInt = string_to_int(valString);
                                    if( tempStrToInt> 1000)
                                    {
                                        GLOBAL_REZ_ID_COUNT = tempStrToInt;
                                    }
                                }
                                else if(keyString=="projectname" || keyString=="name")
                                {
                                    projectName = valString;
                                    RESC_PROJECT_FOLDER->set_name(projectName);
                                }
                                else if(keyString=="projectlanguage" || keyString=="programmingLanguage" || keyString=="codinglanguage")
                                {
                                    myProjectLanguage = GPE_MAIN_HIGHLIGHTER->get_language_id_from_name( valString);
                                    if( myProjectLanguage < 0 )
                                    {
                                        set_project_language_id( PROGRAM_LANGUAGE_CPP );
                                    }
                                }
                                else if( keyString == "projecticon")
                                {
                                    projectIconName = valString;
                                }
                                else if(keyString == "[superfolder")
                                {
                                    endBracketPos=currLine.find_first_of("]");
                                    if(endBracketPos!=(int)std::string::npos)
                                    {
                                        valString = currLine.substr(equalPos+1,endBracketPos-equalPos-1);
                                        if( (int)valString.size()>0)
                                        {
                                            tempResFolder = RESC_PROJECT_FOLDER->find_resource_from_name(valString,false);
                                            if( tempResFolder!=NULL)
                                            {
                                                containerFolderToEdit = currentResFolder = tempResFolder;
                                                tempResFolder = NULL;
                                            }
                                        }
                                    }
                                }
                                else if(keyString == "[folder")
                                {
                                    valString = currLine.substr(equalPos+1,endBracketPos-equalPos-1);
                                    if( (int)valString.size()>0 && containerFolderToEdit!=NULL)
                                    {
                                        endBracketPos=currLine.find_first_of("]");
                                        if(endBracketPos!=(int)std::string::npos)
                                        {
                                            valString = currLine.substr(equalPos+1,endBracketPos-equalPos-1);
                                            if( (int)valString.size()>0)
                                            {
                                                newResName = split_first_string(valString,',');
                                                foundResGlobalId = string_to_int(valString,-1);
                                                tempResFolder = containerFolderToEdit->find_resource_from_name(newResName,false);
                                                if( tempResFolder!=NULL)
                                                {
                                                    containerFolderToEdit = containerFolderToEdit->find_resource_from_name(newResName,false);
                                                }
                                                else
                                                {
                                                    containerFolderToEdit = create_blank_folder(containerFolderToEdit,newResName,foundResGlobalId);
                                                }
                                            }
                                        }
                                    }
                                }
                                else if(keyString == "sprite" || keyString == "animation" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_animation(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempSprRes = (animationResource * )newContainer->get_held_resource();
                                        if( tempSprRes!=NULL)
                                        {
                                            tempSprRes->resourcePostProcessed = false;
                                            projectGameanimations.push_back(tempSprRes);
                                        }
                                    }
                                }
                                else if( keyString =="texture" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_texture(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempTexRes = (textureResource * )newContainer->get_held_resource();
                                        if( tempTexRes!=NULL)
                                        {
                                            tempTexRes->resourcePostProcessed = false;
                                            projectGameTextures.push_back(tempTexRes);
                                        }
                                    }
                                }
                                else if(keyString == "tilesheet")
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_tilesheet(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempTstRes = (tilesheetResource * )newContainer->get_held_resource();
                                        if( tempTstRes!=NULL)
                                        {
                                            tempTstRes->resourcePostProcessed = false;
                                            projectGameTilesheets.push_back(tempTstRes);
                                        }
                                    }
                                }
                                else if( keyString == "audio" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_audio(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempAudRes = (audioResource * )newContainer->get_held_resource();
                                        if( tempAudRes!=NULL)
                                        {
                                            tempAudRes->resourcePostProcessed = false;
                                            projectGameAudio.push_back( tempAudRes);
                                        }
                                    }
                                }
                                else if( keyString == "video" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_video(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempVidRes = (videoResource * )newContainer->get_held_resource();
                                        if( tempVidRes!=NULL)
                                        {
                                            tempVidRes->resourcePostProcessed = false;
                                            projectGameVideos.push_back( tempVidRes);
                                        }
                                    }
                                }
                                else if( keyString == "light2d" )
                                {

                                    tempNewResName = split_first_string(valString,',');
                                    GPE_Report("Adding 2d light["+tempNewResName+"]...");
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_light(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempLightRes = (lightResource * )newContainer->get_held_resource();
                                        if( tempLightRes!=NULL)
                                        {
                                            tempLightRes->resourcePostProcessed = false;
                                            projectLights2D.push_back( tempLightRes);
                                        }
                                    }
                                }
                                else if( keyString == "particleemitter" || keyString == "emitter" )
                                {

                                    tempNewResName = split_first_string(valString,',');
                                    GPE_Report("Adding particle emitter ["+tempNewResName+"]...");
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_emitter(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempEmitterRes = (particleResource * )newContainer->get_held_resource();
                                        if( tempEmitterRes!=NULL)
                                        {
                                            tempEmitterRes->resourcePostProcessed = false;
                                            projectEmitters.push_back( tempEmitterRes);
                                        }
                                    }
                                }
                                else if( keyString == "path" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_path(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempPathRes = (gamePathResource * )newContainer->get_held_resource();
                                        if( tempPathRes!=NULL)
                                        {
                                            tempPathRes->resourcePostProcessed = false;
                                            projectGamePaths.push_back( tempPathRes);
                                        }
                                    }
                                }
                                else if( keyString == "function" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_function(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempFuncRes = (functionResource * )newContainer->get_held_resource();
                                        if( tempFuncRes!=NULL)
                                        {
                                            tempFuncRes->resourcePostProcessed = false;
                                            projectGameFunctions.push_back( tempFuncRes);
                                        }
                                    }
                                }
                                else if( keyString == "class" )
                                {

                                    tempNewResName = split_first_string(valString,',');
                                    GPE_Report("Adding class ["+tempNewResName+"]...");
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_class(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempClassRes = (classResource * )newContainer->get_held_resource();
                                        if( tempClassRes!=NULL)
                                        {
                                            tempClassRes->resourcePostProcessed = false;
                                            projectGameClasses.push_back( tempClassRes);
                                        }
                                    }
                                }
                                else if( keyString == "object"|| keyString=="gameobject"|| keyString=="game-object" || keyString=="actor" )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_gameobject(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempObjRes = (gameObjectResource * )newContainer->get_held_resource();
                                        if( tempObjRes!=NULL)
                                        {
                                            tempObjRes->resourcePostProcessed = false;
                                            projectGameObjects.push_back(tempObjRes);
                                        }
                                    }
                                }
                                else if( keyString == "scene"|| keyString=="gamescene"|| keyString=="game-scene"  )
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_scene(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempScnRes = (gameSceneResource * )newContainer->get_held_resource();
                                        if( tempScnRes!=NULL)
                                        {
                                            tempScnRes->scnPostProcessed = false;
                                            projectScenes.push_back(tempScnRes);
                                        }
                                    }
                                }
                                else if(keyString == "font")
                                {
                                    tempNewResName = split_first_string(valString,',');
                                    foundResGlobalId = split_first_int(valString,',');
                                    newContainer = create_blank_font(containerFolderToEdit,tempNewResName,foundResGlobalId);
                                    if( newContainer->get_held_resource()!=NULL)
                                    {
                                        tempFntRes = (fontResource * )newContainer->get_held_resource();
                                        if( tempFntRes!=NULL)
                                        {
                                            tempFntRes->resourcePostProcessed = false;
                                            projectGameFonts.push_back(tempFntRes);
                                        }
                                    }
                                }
                            }
                            else if( string_lower( currLine ) == "[/folder]")
                            {
                                if( containerFolderToEdit!=NULL)
                                {
                                    if( containerFolderToEdit->parentResource!=NULL)
                                    {
                                        containerFolderToEdit = containerFolderToEdit->parentResource;
                                    }
                                    else
                                    {
                                        containerFolderToEdit = NULL;
                                        tempResFolder = NULL;
                                    }
                                }
                                else
                                {
                                    tempResFolder = NULL;
                                }
                            }
                            else if( string_lower( currLine ) =="[/superfolder]")
                            {
                                containerFolderToEdit = NULL;
                                tempResFolder = NULL;
                            }
                        }
                    }
                }
                newprofileFile.close();

                std::string projectLayerInfoFileName =projectDirectory+"/gpe_project/project_layer_info.gpf";
                std::ifstream projectLayerInfoFile (projectLayerInfoFileName.c_str() );
                int iLayerN = 0;
                GPE_Report("Version number["+float_to_string( projectFilePreviousVersion )+"] found and now processing project data...");
                while ( projectLayerInfoFile.good() )
                {
                    getline (projectLayerInfoFile,currLine); //gets the next line of the file
                    currLine = trim_left_inplace(currLine);
                    currLine = trim_right_inplace(currLine);

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
                                keyString = currLine.substr(0,equalPos);
                                valString = currLine.substr(equalPos+1,currLine.length());
                                for( iLayerN = 0; iLayerN < 32; iLayerN++)
                                {
                                    if( keyString == "ProjectLayerName["+int_to_string(iLayerN)+"]" )
                                    {
                                        projectLayerNames[iLayerN]  = valString;
                                    }
                                }
                            }
                        }
                    }
                }

                for( iItr = 0; iItr < (int)projectGameTextures.size(); iItr++)
                {
                    tempTexRes = projectGameTextures[iItr];
                    if( tempTexRes!=NULL)
                    {
                        tempTexRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameTilesheets.size(); iItr++)
                {
                    tempTstRes = projectGameTilesheets[iItr];
                    if( tempTstRes!=NULL)
                    {
                        tempTstRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameanimations.size(); iItr++)
                {
                    tempSprRes = projectGameanimations[iItr];
                    if( tempSprRes!=NULL)
                    {
                        tempSprRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameAudio.size(); iItr++)
                {
                    tempAudRes = projectGameAudio[iItr];
                    if( tempAudRes!=NULL)
                    {
                        tempAudRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectGameVideos.size(); iItr++)
                {
                    tempVidRes = projectGameVideos[iItr];
                    if( tempVidRes!=NULL)
                    {
                        tempVidRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGamePaths.size(); iItr++)
                {
                    tempPathRes = projectGamePaths[iItr];
                    if( tempPathRes!=NULL)
                    {
                        tempPathRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectLights2D.size(); iItr++)
                {
                    tempLightRes = projectLights2D[iItr];
                    if( tempLightRes!=NULL)
                    {
                        tempLightRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectEmitters.size(); iItr++)
                {
                    tempEmitterRes = projectEmitters[iItr];
                    if( tempEmitterRes!=NULL)
                    {
                        tempEmitterRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameFonts.size(); iItr++)
                {
                    tempFntRes = projectGameFonts[iItr];
                    if( tempFntRes!=NULL)
                    {
                        tempFntRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameFunctions.size(); iItr++)
                {
                    tempFuncRes = projectGameFunctions[iItr];
                    if( tempFuncRes!=NULL)
                    {
                        tempFuncRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameClasses.size(); iItr++)
                {
                    tempClassRes = projectGameClasses[iItr];
                    if( tempClassRes!=NULL)
                    {
                        tempClassRes->preprocess_self();
                    }
                }

                for( iItr = 0; iItr < (int)projectGameObjects.size(); iItr++)
                {
                    tempObjRes = projectGameObjects[iItr];
                    if( tempObjRes!=NULL)
                    {
                        tempObjRes->preprocess_self();
                    }
                }
                for( iItr = 0; iItr < (int)projectScenes.size(); iItr++)
                {
                    tempScnRes = projectScenes[iItr];
                    if( tempScnRes!=NULL)
                    {
                        tempScnRes->preprocess_self();
                    }
                }
                RESC_PROJECT_SETTINGS->set_project_parent_name(projectFileIn);
                RESC_PROJECT_SETTINGS->preprocess_container();
                GPE_Report("Project successfully preprocessed.");
                if( (int)projectScenes.size() > 0 )
                {
                    GPE_Main_TabManager->add_new_tab( projectScenes[0] );
                    input->reset_all_input();
                }
                time( &lastTimeBackedUp);
                return true;
            }
            else
            {
                display_user_alert("Project Open Error","Unable to open file");
            }
        }
        else
        {
            display_user_alert("Project Open Error","Unable to process file");
        }
    }
    else
    {
        display_user_alert("Project Open Error","Invalid File given");
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
    set_current_gpe_project_from_name( projectFileName );
    if( RESC_PROJECT_FOLDER!=NULL)
    {
        RESC_PROJECT_FOLDER->integrate_into_syntax();
    }
    refresh_gui_syntax();
}

bool GPE_ProjectFolder::process_build_script(std::string scriptFName, std::string templateFName, std::string outputDir )
{
    if( (int)templateFName.size()==0 )
    {
        GPE_Main_Logs->log_build_error("Build Script template file empty for ["+scriptFName+"] script!");
        return false;
    }

    if( file_exists( templateFName) ==0 )
    {
        GPE_Main_Logs->log_build_error("Build Script template ["+templateFName+"] file not found!");
        return false;
    }

    if( (int)outputDir.size()==0 )
    {
        outputDir = file_to_dir(projectFileName)+"/gpe_project";
    }
    //if the script doesnt exist we will do a straight copy of template file to outputdir
    GPE_Ini_File * scriptIniReader = new GPE_Ini_File();
    if( (int)scriptFName.size() > 0 )
    {
        if( file_exists( scriptFName) )
        {
            GPE_Main_Logs->log_build_line("Attempting to read build script ["+scriptFName+"]...");

            if( scriptIniReader->read_ini_file( scriptFName,2 ) )
            {
                GPE_Main_Logs->log_build_line("Successfully read build script ["+scriptFName+"].");
            }
            else
            {
                GPE_Main_Logs->log_build_error("Unable to read build script ["+scriptFName+"]...");
            }
        }
        else
        {
            GPE_Main_Logs->log_build_error("Unable to find build script ["+scriptFName+"]...");
        }
    }
    else
    {
        GPE_Main_Logs->log_build_error("No build script detected....");
    }

    std::string outputExtension = scriptIniReader->find_string_keypair("Main","outputextension");
    std::string scriptOutputName = scriptIniReader->find_string_keypair("Main","outputname");

    if( (int)outputExtension.size() == 0 )
    {
        outputExtension = get_file_ext( templateFName);
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

    std::ifstream templateInputFile( templateFName.c_str() );
    std::ofstream templateOutputFile( templateOutputFileName.c_str() );

    std::string currentLine = "";
    while ( templateInputFile.good() )
    {
        getline (templateInputFile,currentLine); //gets the next line of the file
        templateOutputFile << currentLine << "\n";
    }
    templateInputFile.close();
    templateOutputFile.close();

    if( scriptIniReader!=NULL )
    {
        delete scriptIniReader;
        scriptIniReader = NULL;
    }
    return true;
}

void GPE_ProjectFolder::refresh_gui_syntax()
{
    if( GPE_MAIN_HIGHLIGHTER!=NULL )
    {
        GPE_Compiler_Term * tempTerm = NULL;
        GPE_MAIN_HIGHLIGHTER->activeProjectFunctions.clear();
        int i = 0;
        for( i = 0; i < (int)projectFunctions.size(); i++ )
        {
            tempTerm = projectFunctions[i];
            if( tempTerm!=NULL)
            {
                GPE_MAIN_HIGHLIGHTER->activeProjectFunctions.push_back( tempTerm);
            }
        }
        GPE_MAIN_HIGHLIGHTER->activeProjectKeywords.clear();
        for( i = 0; i < (int)projectKeywords.size(); i++ )
        {
            tempTerm = projectKeywords[i];
            if( tempTerm!=NULL)
            {
                GPE_MAIN_HIGHLIGHTER->activeProjectKeywords.push_back( tempTerm );
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
    if( buildMetaTemplate == GPE_BUILD_HTML5)
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = file_to_dir(projectFileName)+"/gpe_project/builds/html5";
        }
        if( file_exists(projectBuildDirectory+"/index.html")==true )
        {
            GPE_OpenURL(projectBuildDirectory+"/index.html");
            return true;
        }
        else
        {
            display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate==GPE_BUILD_WINDOWS && GPE_FOUND_OS==GPE_IDE_WINDOWS)
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = file_to_dir(projectFileName)+"/gpe_project/builds/windows";
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
        if( file_exists(projectBuildDirectory+"/game.exe")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/game.exe");
            return true;
        }
        else if( file_exists(projectBuildDirectory+"/electron.exe")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/electron.exe");
            return true;
        }
        else
        {
            display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate==GPE_BUILD_MAC && GPE_FOUND_OS==GPE_IDE_MAC)
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = file_to_dir(projectFileName)+"/gpe_project/builds/osx";
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
        if( file_exists(projectBuildDirectory+"/game.app")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/game.app");
            return true;
        }
        else if( file_exists(projectBuildDirectory+"/electron.app")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/electron.app");
            return true;
        }
        else
        {
            display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    else if( buildMetaTemplate==GPE_BUILD_LINUX && GPE_FOUND_OS==GPE_IDE_LINUX)
    {
        if( projectBuildDirectory.empty() )
        {
            projectBuildDirectory = file_to_dir(projectFileName)+"/gpe_project/builds/linux";
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
        if( file_exists(projectBuildDirectory+"/game")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/game");
            return true;
        }
        else if( file_exists(projectBuildDirectory+"/electron")==true )
        {
            GPE_OpenProgram(projectBuildDirectory+"/electron");
            return true;
        }
        else
        {
            display_user_alert("[Run Project Error]","Please first build this project");
        }
    }
    return false;
}

bool GPE_ProjectFolder::save_project()
{
    return save_project_as(projectFileName);
}

bool GPE_ProjectFolder::save_project_as(std::string projectFileNewName)
{
    bool hadSaveErrors = false;
    if( (int)projectFileNewName.size()>3)
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_messages( "Saving Project [" + projectName + "]", "Please Wait...","DO NOT CLOSE..." );
        }

        GPE_Report( "Saving Project [" + projectFileName + "]" );
        bool isSameFileName = false;
        bool saveAuthorized = false;
        if( !gpemath::compare_floats(projectFilePreviousVersion, GPE_VERSION_FLOAT_NUMBER ) && projectFilePreviousVersion < GPE_VERSION_FLOAT_NUMBER )
        {
            std::string versionComparePhrase = "The following project version varies from the current version of this editor. Are you sure you will like to save this. Please note saving/loading may experience difficulties we recommend you back up these project files/folders ELSEWHERE before saving...";
            if( GPE_Display_Basic_Prompt("Project Version Warning!",versionComparePhrase)==DISPLAY_QUERY_YES )
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
            if( projectFileNewName.compare(projectFileName)==0)
            {
                isSameFileName = true;
            }
            else
            {
                isSameFileName = false;
            }
            if( !isSameFileName)
            {
                projectFileName = projectFileNewName;
                projectDirectory = get_path_from_file(projectFileName);
                std::string newProjectLocalFileName = get_local_from_global_file(projectFileName);
                if( (int)newProjectLocalFileName.size()>0 )
                {
                    projectDirectory = projectDirectory+"/"+projectFileName;
                    display_user_alert("Error:",projectDirectory.c_str() );

                    GPE_MAIN_GUI->setup_project_directory(projectDirectory);
                }
                else
                {
                    display_user_alert("Error!","Unable to save project("+projectFileName+")");
                }
                set_current_gpe_project_from_name( projectFileName );
                if(RESC_PROJECT_FOLDER!=NULL)
                {
                    RESC_PROJECT_FOLDER->set_project_parent_name(projectFileName);
                }
            }
            else
            {
                GPE_MAIN_GUI->setup_project_directory(projectDirectory);
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
                myfile << "Version=" << GPE_VERSION_FLOAT_NUMBER << "\n";
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
                if(RESC_PROJECT_FOLDER!=NULL)
                {
                    hadSaveErrors = RESC_PROJECT_FOLDER->write_data_into_projectfile(&myfile);
                }
                time( &lastTimeBackedUp);
                myfile.close();
            }
            else
            {
                GPE_Report("Unable to open to save ["+projectFileNewName+"].");
                hadSaveErrors = true;
            }
            std::string projectLayerInfoFileName =projectDirectory+"/gpe_project/project_layer_info.gpf";
            GPE_Report("Saving project layer info to "+projectLayerInfoFileName);
            std::ofstream projectLayerInfoFile (projectLayerInfoFileName.c_str() );
            if (projectLayerInfoFile.is_open() )
            {
                //write_header_on_file(&projectLayerInfoFile);
                for( int iLayerN = 0; iLayerN < 32; iLayerN++)
                {
                    projectLayerInfoFile << "ProjectLayerName["+int_to_string(iLayerN)+"]=" << projectLayerNames[iLayerN] << "\n";
                }
                projectLayerInfoFile.close();
                projectFilePreviousVersion = projectFileVersion = GPE_VERSION_FLOAT_NUMBER;
            }
            else
            {
                GPE_Report("Unable to open to save ["+projectLayerInfoFileName+"].");
                hadSaveErrors = true;
            }
        }
        else
        {
            GPE_Main_Logs->log_general_comment("Save averted for Project["+projectFileName+"](not authorized by user choice).");
        }
    }
    else
    {
        GPE_Report("File named "+projectFileNewName+" is too short and does not fit the proper format for project save feature.");
    }
    integrate_syntax();
    return hadSaveErrors;
}

bool GPE_ProjectFolder::save_project_settings()
{
    if( RESC_PROJECT_SETTINGS!=NULL)
    {
        std::string projectSettingsFileName = projectStartDirectoryName+"gpe_project/project_settings.ini";
        std::ofstream myfile (projectSettingsFileName.c_str() );
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
        GPE_Gui_Engine_Language * tLanguage  = NULL;
        bool languageChanged = false;
        int languageMax = GPE_MAIN_HIGHLIGHTER->get_language_count();
        for( int cLanguage = 0; cLanguage < languageMax; cLanguage++ )
        {
            tLanguage = GPE_MAIN_HIGHLIGHTER->get_language_object(cLanguage);
            //checks if the found language isnt NULL and is a coding language
            if( tLanguage!=NULL && tLanguage->isCodingLanguage)
            {
                //if its regular name or short name matches. We end loop and change our project language.
                if( newProjectLanguage == tLanguage->languageName || newProjectLanguage == tLanguage->languageShortName )
                {
                    GPE_Report("Changing project language to "+ tLanguage->languageName + "...");
                    myProjectLanguage = cLanguage;
                    projectLanguage = tLanguage->languageShortName;
                    languageChanged = true;
                    break;
                }
            }
        }
        if( !languageChanged )
        {
            GPE_Report("Unable to find language ["+newProjectLanguage+"]...");
        }
        return;
    }
    GPE_Report("You can not change project language to an empty name");
}

void GPE_ProjectFolder::set_project_language_id( int projectLanguageId )
{
    if( projectLanguageId >= 0 )
    {
        GPE_Gui_Engine_Language * tLanguage = GPE_MAIN_HIGHLIGHTER->get_language_object_from_id( projectLanguageId );
        //checks if the found lanuage isnt NULL and is a coding language
        if( tLanguage!=NULL && tLanguage->isCodingLanguage)
        {
            myProjectLanguage = projectLanguageId;
            projectLanguage = tLanguage->languageShortName;
        }
    }
}

bool set_current_gpe_project( GPE_ProjectFolder * newMainProject)
{
    bool projectChanged = false;
    if( newMainProject!=NULL)
    {
        CURRENT_PROJECT = newMainProject;
        if( CURRENT_PROJECT_NAME != newMainProject->get_project_file_name() )
        {
            CURRENT_PROJECT_NAME = newMainProject->get_project_file_name();
            //Integrate syntax for syntax highlighter and such...
            CURRENT_PROJECT->refresh_gui_syntax();
        }
    }
    else if( CURRENT_PROJECT!=NULL)
    {
        projectChanged = true;
    }
    return projectChanged;
}

GPE_Gui_Engine::GPE_Gui_Engine()
{
    includeNintendoSwitchExport = false;
    includePlaystation4Export = false;
    includeXboxOneExport = false;
    editorReleaseType = RELEASE_TYPE_ALPHA;
    if( GPE_MAIN_WINDOW!=NULL)
    {
        GPE_MAIN_WINDOW->set_window_title("Game Pencil Engine");
        GPE_Report("GPE_MAIN_WINDOW is not NULL...");
    }
    else
    {
        GPE_Report("GPE_MAIN_WINDOW is NULL...");
    }
    GPE_DEFAULT_TEMPLATE = add_theme("default");
    GPE_DEFAULT_TEMPLATE->load_theme(APP_DIRECTORY_NAME+"themes/default.gpf");
    GPE_MAIN_THEME = GPE_DEFAULT_TEMPLATE;

    load_tips();
    fileOpenProjectDir = "";
    fileOpenProjectIconDir = "";
    fileSaveProjectDir = "";


    mainToolBar = NULL;
    mainResourceTree = NULL;

    //findTextStringBox->set_label("Find:");
    //goToLineStringBox->set_label("Go To Line:");
    //replaceTextStringBox->set_label("Replace:");

    showTipsAtStartUp = true;
    showStartPageAtStartUp = true;

    //loads the recent file list
    std::string recentFileListFileName = get_user_settings_folder()+"recent_projects.txt";
    std::ifstream recentFileListFile( recentFileListFileName.c_str() );
    if( !recentFileListFile.fail() )
    {
        std::string currLine = "";
        //makes sure the file is open
        if (recentFileListFile.is_open())
        {
            while ( recentFileListFile.good() )
            {
                getline (recentFileListFile,currLine); //gets the next line of the file
                currLine = trim_left_inplace(currLine);
                currLine = trim_right_inplace(currLine);
                add_to_recent_project_list(currLine,false);
            }
            recentFileListFile.close();
        }
    }
    eraserAnimation = guiRCM->animation_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/eraser.png",1, true );
}

GPE_Gui_Engine::~GPE_Gui_Engine()
{
    GPE_ProjectFolder * tempProject = NULL;
    for( int i = (int)gpeProjects.size()-1; i >=0; i--)
    {
        tempProject = gpeProjects[i];
        if( tempProject!=NULL)
        {
            delete tempProject;
            tempProject= NULL;
        }
    }
    gpeProjects.clear();



    if( eraserAnimation!=NULL)
    {
        guiRCM->remove_animation( eraserAnimation );
        eraserAnimation = NULL;
    }
    if( mainToolBar!=NULL)
    {
        delete mainToolBar;
        mainToolBar = NULL;
    }

    if( mainResourceTree!=NULL)
    {
        delete mainResourceTree;
        mainResourceTree = NULL;
    }

    if( GPE_MAIN_THEME!=NULL)
    {
        delete GPE_MAIN_THEME;
        GPE_MAIN_THEME = NULL;
    }

    if( MAIN_CONTEXT_MENU!=NULL)
    {
        delete MAIN_CONTEXT_MENU;
        MAIN_CONTEXT_MENU = NULL;
    }
    if(GPE_Main_TabManager!=NULL)
    {
        delete GPE_Main_TabManager;
        GPE_Main_TabManager = NULL;
    }

    if(GPE_Main_Logs!=NULL)
    {
        delete GPE_Main_Logs;
        GPE_Main_Logs = NULL;
    }
    if( GPE_ANCHOR_GC!=NULL )
    {
        GPE_ANCHOR_GC->clear_list( false );
        delete GPE_ANCHOR_GC;
        GPE_ANCHOR_GC = NULL;
    }

    if( GPE_TEXTURE_COLOR_PICKER_GRADIENT!=NULL)
    {
        delete GPE_TEXTURE_COLOR_PICKER_GRADIENT;
        GPE_TEXTURE_COLOR_PICKER_GRADIENT = NULL;
    }
    if( GPE_SURFACE_COLOR_PICKER_GRADIENT!=NULL)
    {
        SDL_FreeSurface(GPE_SURFACE_COLOR_PICKER_GRADIENT);
        GPE_SURFACE_COLOR_PICKER_GRADIENT = NULL;
    }
    if( GPE_LOGO!=NULL)
    {
        delete GPE_LOGO;
        GPE_LOGO = NULL;
    }


}

GPE_Theme * GPE_Gui_Engine::add_theme(std::string themeName, bool customTheme )
{
    GPE_Theme * newTemplate = new GPE_Theme(themeName, customTheme);
    gpeThemes.push_back( newTemplate);
    return newTemplate;
}

void GPE_Gui_Engine::add_to_recent_project_list(std::string newProjectFileName, bool saveData)
{
    int cSize = (int)gpeRecentProjects.size();
    int i = 0;
    if( (int)newProjectFileName.size() > 0 )
    {
        cSize = (int)gpeRecentProjects.size();
        for(  i = cSize-1; i >=0; i--)
        {
            if( gpeRecentProjects[i]==newProjectFileName)
            {
                gpeRecentProjects.erase( gpeRecentProjects.begin()+i);
            }
        }
        cSize = (int)gpeRecentProjects.size();
        if( cSize >= 15)
        {
            int cDiff = cSize - 14;
            for(  i = cSize-1; i >=cDiff; i--)
            {
                gpeRecentProjects.erase( gpeRecentProjects.begin()+i);
            }
        }
        gpeRecentProjects.insert(gpeRecentProjects.begin(),newProjectFileName);
        GPE_Report("Added ["+newProjectFileName+"] to Recent Project List");
    }
    if( saveData)
    {
        cSize = (int)gpeRecentProjects.size();
        std::string recentFileListFileName = get_user_settings_folder()+"recent_projects.txt";
        std::ofstream recentFileListFile( recentFileListFileName.c_str() );
        if( !recentFileListFile.fail() )
        {
            //makes sure the file is open
            if (recentFileListFile.is_open())
            {
                for( i = cSize-1; i>=0; i--)
                {
                    recentFileListFile << gpeRecentProjects[i] <<  "\n";
                }
                if( CURRENT_PROJECT!=NULL)
                {
                    recentFileListFile << CURRENT_PROJECT->get_project_file_name() <<  "\n";
                }
                recentFileListFile.close();
            }
        }
    }
    update_recent_project_list(false);
}

void GPE_Gui_Engine::apply_logic()
{
    //always resetting the tooltip
    if( input->input_received() )
    {
        if( MAIN_OVERLAY!=NULL)
        {
            MAIN_OVERLAY->update_tooltip("");
        }
        update_popup_info();
    }
    process_window_title();

    if( mainToolBar!=NULL && MAIN_CONTEXT_MENU!=NULL)
    {
        //all other gui elements are dependent on the toolbar and popup menus being closed
        if( !MAIN_CONTEXT_MENU->is_open() )
        {
            //prevents the toolbar isnt activated when popup menu is open
            mainToolBar->process_toolbar();
        }

        if( mainToolBar->open_requested() )
        {
            GPE_DOCK->update_toolbar();
        }
        if( !mainToolBar->is_open() )
        {
            //prevents the popup menu from being processed while toolbar is open
            if( MAIN_CONTEXT_MENU->is_open() )
            {
                //POPUP_MENU_VALUE = MAIN_CONTEXT_MENU->process_menu_option();
                //if( input->check_mouse_released(kb_anykey) || GPE_MAIN_WINDOW->is_resized()  || input->released[ESC] || input->released[kb_enter])
                {
                    GPE_close_context_menu();
                }
            }
        }

        if( !mainToolBar->is_open() && !MAIN_CONTEXT_MENU->is_open() )
        {
            if( GPE_Main_Statusbar!=NULL)
            {
                GPE_Main_Statusbar->process_self();
            }

            if( GPE_DOCK!=NULL)
            {
                GPE_DOCK->process_self();
                if( PANEL_RESOURCE_TREE!=NULL)
                {
                    if( input->check_keyboard_down( kb_ctrl) || input->check_keyboard_only_released(kb_1) )
                    {
                        //GPE_Main_Logs->log_general_line( )
                    }
                }
            }
            else
            {
                GPE_Report( "GPE_DOCK not initialized!" );
            }
        }
    }
    if( mainToolBar!=NULL && mainToolBar->just_closed() )
    {
        process_overlay_message();
    }

    if( input->check_keyboard_down(kb_ctrl) )
    {
        if( input->check_keyboard_released(kb_b) )
        {
            GPE_Main_TabManager->add_new_tab(MAIN_ABOUT_PAGE);
        }
        else if( input->pressed[kb_f])
        {
            if( input->down[kb_shift] )
            {
                MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FINDALL;

            }
            else
            {
                MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FIND;
            }
            MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
            if( GPE_Main_Logs!=NULL)
            {
                GPE_Main_Logs->open_search_mode();
            }
        }
        else if( input->pressed[kb_g])
        {
            MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_GOTO;
            MAIN_SEARCH_CONTROLLER->goToLineStringBox->set_string("1");
            MAIN_SEARCH_CONTROLLER->goToLineStringBox->switch_inuse(true);
            if( GPE_Main_Logs!=NULL)
            {
                GPE_Main_Logs->open_search_mode();
            }
        }
        else if( input->pressed[kb_h])
        {
            MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACE;
            MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true );
            if( GPE_Main_Logs!=NULL)
            {
                GPE_Main_Logs->open_replace_mode();
            }
        }
        else if( input->check_keyboard_released(kb_n) )
        {
            launch_new_project();
        }
        else if( input->check_keyboard_released(kb_o) )
        {
            open_new_project();
        }
        else if( input->check_keyboard_released(kb_q) )
        {
            GPE_Action_Message=="Quit Editor";
            input->done = true;
        }
        else if( input->pressed[kb_r])
        {
            if( input->down[kb_shift] )
            {
                MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACEALL;

            }
            else
            {
                MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACE;

            }
            MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
            if( GPE_Main_Logs!=NULL)
            {
                GPE_Main_Logs->open_replace_mode();
            }
        }
        else if( input->check_keyboard_released(kb_s) )
        {
            if( input->check_keyboard_down(kb_shift) )
            {
                save_all_projects();
            }
            else
            {
                save_current_project();
            }
        }
        else if( input->check_keyboard_released(kb_u) )
        {
            GPE_Main_TabManager->add_new_tab(MAIN_EDITOR_SETTINGS);
        }
    }
    else if( input->check_keyboard_released(kb_f1) )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_HELP_PAGE);
    }
    else if( input->check_keyboard_released(kb_f2) )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_EDITOR_SETTINGS);
    }
    else if( input->check_keyboard_released(kb_f5) )
    {
        GPE_MAIN_GUI->export_current_project_html5( input->check_keyboard_down(kb_ctrl)==false );
    }
    if( input->check_keyboard_released(kb_f6) )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_START_PAGE);
    }
    else if( input->check_keyboard_released(kb_f8) && CURRENT_PROJECT!=NULL )
    {
        GPE_MAIN_GUI->rum_current_project("",GPE_BUILD_HTML5);
    }
    else if( input->check_keyboard_released(kb_f10) )
    {
        //gcanvas->get_font_cache_size(true);
    }
    else if( input->check_keyboard_released(kb_f11) )
    {
        if( !GPE_MAIN_WINDOW->is_fullscreen() )
        {
            MAIN_OVERLAY->update_temporary_message("Entering Full-Screen","Press [F11] to exit","",1);
        }
        else
        {
            MAIN_OVERLAY->update_temporary_message("Exiting Full-Screen","Press [F11] to toggle","",1);
        }
        GPE_MAIN_WINDOW->toggle_fullscreen();
    }
    else if( input->check_keyboard_released(kb_f12) )
    {
        GPE_MAIN_GUI->take_live_screenshor();
    }
    else if( input->check_keyboard_released(kb_esc) )
    {
        MAIN_SEARCH_CONTROLLER->close_finder();
    }

    int uInS = input->fileDroppedList.size();
    std::string tempInputtedFile = "";
    if( uInS > 0)
    {
        for( int iRFile = 0; iRFile < uInS; iRFile++)
        {
            tempInputtedFile = input->fileDroppedList.at( iRFile);
            if( file_exists(tempInputtedFile) )
            {
                if( get_file_ext(tempInputtedFile)=="gppf" )
                {
                    open_project(tempInputtedFile);
                }
                else
                {
                    display_user_alert("Error","Content dropped in window:"+tempInputtedFile+".");
                }
            }
            else
            {
                display_user_alert("Error","Content dropped in window:"+tempInputtedFile+" file not found.");
            }
        }
    }

    if( input->check_mouse_released( mb_anybutton ) )
    {
        RESOURCE_TO_DRAG = NULL;
        LAST_CLICKED_RESOURCE = NULL;
    }

    //If the user has Xed out the window
    if( GPE_MAIN_WINDOW->windowClosed || input->done || GPE_Action_Message=="Quit Editor" || GPE_Action_Message=="Quit" )
    {
        if( (int)gpeProjects.size() > 0 )
        {
            int returnedAction = -1;
            int projectAction = -1;
            GPE_ProjectFolder * tempProjFolder;
            for( int i= (int)gpeProjects.size()-1; i>=0; i--)
            {
                tempProjFolder = gpeProjects[i];
                if( tempProjFolder!=NULL)
                {
                    projectAction = GPE_Display_Basic_Prompt("[Warning!] "+tempProjFolder->get_project_name()+" project not saved","Will you like to save and close before exiting?",true);
                    if( projectAction==DISPLAY_QUERY_YES)
                    {
                        tempProjFolder->save_project();
                        remove_project( tempProjFolder->get_project_file_name() );
                    }
                    else if( projectAction==DISPLAY_QUERY_CANCEL)
                    {
                        returnedAction = 2;
                        input->done = true;
                        GPE_MAIN_WINDOW->windowClosed = false;
                    }
                    else
                    {
                        remove_project( tempProjFolder->get_project_file_name() );
                        input->done = true;
                    }
                }
            }
            if( returnedAction!=2)
            {
                //Quit the programs
                gpe->state_set( "exit" );
            }
        }
        else
        {
            //Quit the programs
            gpe->state_set( "exit" );
        }
    }
}


GPE_Toolbar * GPE_Gui_Engine::init_toolbar()
{
    if( mainToolBar!=NULL)
    {
        delete mainToolBar;
    }
    mainToolBar = new GPE_Toolbar();
    return mainToolBar;
}

int GPE_Gui_Engine::get_recent_project_list_size()
{
    return (int)gpeRecentProjects.size();
}

int GPE_Gui_Engine::get_tip_count()
{
    return (int)gpeTips.size();
}

int GPE_Gui_Engine::get_random_tip()
{
    return gpemath::random(0, (int)gpeTips.size()-1);
}

int GPE_Gui_Engine::get_previous_tip(int tipId)
{
    if( tipId >0 && tipId < (int)gpeTips.size() )
    {
        return tipId-1;
    }
    else
    {
        return (int)gpeTips.size()-1;
    }
}

int GPE_Gui_Engine::get_next_tip(int tipId)
{
    if( tipId >=0 && tipId < (int)gpeTips.size()-1 )
    {
        return tipId+1;
    }
    else
    {
        return 0;
    }
}

std::string GPE_Gui_Engine::get_tip(int tipId)
{
    if( tipId >=0 && tipId < (int)gpeTips.size() )
    {
        return gpeTips.at(tipId);
    }
    else
    {
        tipId = 0;
        if( (int)gpeTips.size() > 0)
        {
            return gpeTips.at(tipId);
        }
    }
    return "";
}

std::string GPE_Gui_Engine::get_recent_project_name( int pId )
{
    if( pId >=0 && pId < (int)gpeRecentProjects.size() )
    {
        return gpeRecentProjects.at(pId);
    }
    return "";
}

GPE_ResourceTree * GPE_Gui_Engine::init_resourcebar()
{
    if( mainResourceTree!=NULL)
    {
        delete mainResourceTree;
        mainResourceTree = NULL;
    }
    mainResourceTree = new GPE_ResourceTree();

    //update the tabbar and resourcemenu
    return mainResourceTree;
}

GPE_Toolbar * GPE_Gui_Engine::init_toolbar(std::string bName,GPE_Rect bRect)
{
    if( mainToolBar!=NULL)
    {
        delete mainToolBar;
    }
    mainToolBar = new GPE_Toolbar();
    mainToolBar->set_name( bName );
    mainToolBar->set_coords( bRect.x, bRect.y);
    mainToolBar->set_width( bRect.w) ;
    mainToolBar->set_height( bRect.h );
    return mainToolBar;
}

GPE_ProjectFolder * GPE_Gui_Engine::find_project_from_name(std::string projectName)
{
    if( (int)projectName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        GPE_ProjectFolder * foundProject = NULL;
        int i = 0;
        for( i = 0; i < (int)gpeProjects.size(); i++ )
        {
            tPFolder = gpeProjects[i];
            if( tPFolder!=NULL)
            {
                if( tPFolder->get_project_name().compare(projectName)==0 )
                {
                    foundProject = tPFolder;
                    break;
                }
            }
        }
        if( foundProject==NULL)
        {
            for( i = 0; i < (int)gpeProjects.size(); i++ )
            {
                tPFolder = gpeProjects[i];
                if( tPFolder!=NULL)
                {
                    if( tPFolder->get_project_file_name().compare(projectName)==0 )
                    {
                        foundProject = tPFolder;
                        break;
                    }
                }
            }
        }
        return foundProject;
    }
    return NULL;
}

GPE_ProjectFolder *  GPE_Gui_Engine::find_project_from_filename(std::string projectFileName)
{
    if( (int)projectFileName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        GPE_ProjectFolder * foundProject = NULL;
        for( int i = 0; i < (int)gpeProjects.size(); i++ )
        {
            tPFolder = gpeProjects[i];
            if( tPFolder!=NULL)
            {
                if( tPFolder->get_project_file_name().compare(projectFileName)==0 )
                {
                    foundProject = tPFolder;
                }
            }
        }
        return foundProject;
    }
    return NULL;
}

void GPE_Gui_Engine::clean_current_project_build_folder(int buildMetaTemplate)
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
    if( fFolder!=NULL)
    {
        fFolder->clean_build_folder(buildMetaTemplate);
    }
}

void GPE_Gui_Engine::clear_recent_project_list()
{
    gpeRecentProjects.clear();
    std::string recentFileListFileName = get_user_settings_folder()+"recent_projects.txt";
    std::ofstream recentFileListFile( recentFileListFileName.c_str() );
    recentFileListFile.close();
    update_recent_project_list(false);
}

void GPE_Gui_Engine::close_project(std::string projectFileName )
{
    remove_project( projectFileName);
}

int GPE_Gui_Engine::find_project_id_from_name(std::string projectName, int ignoreId)
{
    if( (int)projectName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        int foundProject = -1;
        for( int i = 0; i < (int)gpeProjects.size(); i++ )
        {
            if( i!=ignoreId)
            {
                tPFolder = gpeProjects[i];
                if( tPFolder!=NULL)
                {
                    if( tPFolder->get_project_name().compare(projectName)==0 )
                    {
                        foundProject = i;
                        break;
                    }
                }
            }
        }
        return foundProject;
    }
    return -1;
}

int GPE_Gui_Engine::find_project_id_from_filename(std::string projectFileName, int ignoreId)
{
    if( (int)projectFileName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        int foundProject = -1;
        for( int i = 0; i < (int)gpeProjects.size(); i++ )
        {
            if( i!=ignoreId)
            {
                tPFolder = gpeProjects[i];
                if( tPFolder!=NULL)
                {
                    if( tPFolder->get_project_file_name().compare(projectFileName)==0 )
                    {
                        foundProject = i;
                    }
                }
            }
        }
        return foundProject;
    }
    return -1;
}

GPE_Theme * GPE_Gui_Engine::find_theme(std::string themeName )
{
    if( (int)themeName.size() > 0 )
    {
        for( int i = (int)gpeThemes.size()-1; i >=0; i-- )
        {
            if( gpeThemes[i]->themeName == themeName)
            {
                return gpeThemes[i];
            }
        }
    }
    return NULL;
}

GPE_Toolbar * GPE_Gui_Engine::get_main_toolbar()
{
    return mainToolBar;
}

void GPE_Gui_Engine::launch_new_project()
{
    bool exitOperation = false;
    bool manualCancel = false;
    bool createButtonPushed = false;

    std::string newProjectFileName = "";
    std::string newProjectLocalFileName = "";
    std::string newProjectName = "";
    std::string foundFileName = "";
    RESOURCE_TO_DRAG = NULL;
    if( GPE_MAIN_GUI!=NULL && GPE_MAIN_RENDERER!=NULL)
    {
        GPE_Report("Launching New Project");
        std::string popUpCaption = "Launch New Project";
        gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );
        GPE_MAIN_GUI->reset_gui_info();
        MAIN_OVERLAY->take_frozen_screenshot( );

        int promptBoxWidth = GPE_ENGINE_SETTINGS->minWindowWidth;
        int promptBoxHeight = GPE_ENGINE_SETTINGS->minWindowHeight;
        GPE_Rect elementBox;


        input->reset_all_input();
        int currentTipId = GPE_MAIN_GUI->get_random_tip();
        if( currentTipId < 0 || currentTipId > GPE_MAIN_GUI->get_tip_count() )
        {
            currentTipId = 0;
        }
        std::string currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);

        GPE_Label_Title * mainMenuLabel = new GPE_Label_Title("Create A Project?","Create A Project");
        GPE_Label_Text * projectLocationLabel = new GPE_Label_Text("Project Location:","Project Location:");
        GPE_Label_Text * projectNameLabel = new GPE_Label_Text("Project Name:","Project Name:");

        GPE_Label_Text * projectMainExportLabel = new GPE_Label_Text("Primary Export Target:","Primary Export Target:");
        GPE_Label_Text * projectMainLanguageLabel = new GPE_Label_Text("Programming Language:","Programming Language:");
        int biggerLabelSize = std::max( projectMainExportLabel->get_width(), projectMainLanguageLabel->get_width() );
        biggerLabelSize = std::max( biggerLabelSize, projectNameLabel->get_width() );
        projectMainExportLabel->set_width(biggerLabelSize);
        projectNameLabel->set_width(biggerLabelSize);
        projectMainLanguageLabel->set_width(biggerLabelSize);

        //Adds all available languages to drop down menu
        GPE_DropDown_Menu * newprojectLanguage = new GPE_DropDown_Menu( "Project Language",true);
        int addedLanguages = 0;
        if( GPE_MAIN_HIGHLIGHTER!=NULL )
        {
            //short operation so not "optimized"
            GPE_Gui_Engine_Language * tLanguage  = NULL;
            int languageCount = (int)GPE_MAIN_HIGHLIGHTER->get_language_count();

            for( int cLanguage = 0; cLanguage < languageCount; cLanguage++ )
            {
                tLanguage = GPE_MAIN_HIGHLIGHTER->get_language_object( cLanguage);
                if( tLanguage!=NULL && tLanguage->isCodingLanguage)
                {
                    newprojectLanguage->add_menu_option(tLanguage->languageName+" ("+tLanguage->languageShortName+")",tLanguage->languageShortName,cLanguage,true);
                    addedLanguages++;
                }
            }
        }
        //In the event something went wrong and we somehow didn't add JS and any other new coding language...
        if( addedLanguages == 0 )
        {
            newprojectLanguage->add_menu_option("JavaScript","JS", PROGRAM_LANGUAGE_JS,true);
            newprojectLanguage->add_menu_option("C++","CPP", PROGRAM_LANGUAGE_CPP,true);
        }

        GPE_DropDown_Menu * newprojectMainExport= new GPE_DropDown_Menu( "Main Export Target",true);
        newprojectMainExport->add_menu_option("HTML5","HTML5",GPE_BUILD_HTML5,true);
        newprojectMainExport->add_menu_option("WINDOWS","WINDOWS", GPE_BUILD_WINDOWS);
        newprojectMainExport->add_menu_option("MAC","MAC",GPE_BUILD_MAC);
        newprojectMainExport->add_menu_option("LINUX","LINUX",GPE_BUILD_LINUX);


        GPE_GuiElementList * popupMenuList = new GPE_GuiElementList();
        GPE_TextInputBasic * projectNameField = new GPE_TextInputBasic("","New Project");
        GPE_Label_Text * fileToCreateField = new GPE_Label_Text("No file selected","No file selected" );
        fileToCreateField->set_width( GPE_ENGINE_SETTINGS->minWindowWidth -32 );
        GPE_ToolLabelButton * closeButton = new GPE_ToolLabelButton( "Cancel","Cancel");
        GPE_ToolLabelButton * createButton = new GPE_ToolLabelButton( "Create","Create");
        GPE_CheckBoxBasic * matchProjectFileName = new GPE_CheckBoxBasic("Same as file name","Makes project name same as file name", true );
        GPE_Label_Error * projectCreateErrorLabel = new GPE_Label_Error("Please input all fields above","");
        GPE_ToolPushButton * fileFindButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/folder.png","Browse Projects...");
        fileToCreateField->set_width( GPE_ENGINE_SETTINGS->minWindowWidth -32 );
        GPE_MAIN_RENDERER->reset_viewpoint( );
        //MAIN_OVERLAY->render_frozen_screenshot( );
        while(exitOperation==false)
        {
            gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );
            //GPE_Report("Processing tip of the day");
            gpe->start_loop();

            if( SCREEN_WIDTH > GPE_ENGINE_SETTINGS->defaultWindowWidth*2 )
            {
                promptBoxWidth = GPE_ENGINE_SETTINGS->defaultWindowWidth*2;
            }
            else
            {
                promptBoxWidth = SCREEN_WIDTH;
            }

            if( SCREEN_HEIGHT > GPE_ENGINE_SETTINGS->defaultWindowWidth*2 )
            {
                promptBoxHeight = GPE_ENGINE_SETTINGS->defaultWindowWidth*2;
            }
            else
            {
                promptBoxHeight = SCREEN_HEIGHT;
            }

            elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2;
            elementBox.y = (SCREEN_HEIGHT-promptBoxHeight)/2;
            elementBox.w = promptBoxWidth;
            elementBox.h = promptBoxHeight;
            popupMenuList->set_coords(elementBox.x, elementBox.y+32);
            popupMenuList->set_width(elementBox.w);
            popupMenuList->set_height(elementBox.h-32);
            popupMenuList->barXMargin = GENERAL_GPE_GUI_PADDING;
            popupMenuList->barYMargin = GENERAL_GPE_GUI_PADDING;
            popupMenuList->barXPadding = GENERAL_GPE_GUI_PADDING;
            popupMenuList->barYPadding = GENERAL_GPE_GUI_PADDING;
            GPE_MAIN_GUI->reset_gui_info();
            popupMenuList->clear_list();
            popupMenuList->add_gui_element(mainMenuLabel,true);

            popupMenuList->add_gui_element(projectLocationLabel,false);
            popupMenuList->add_gui_element(fileFindButton, true);

            popupMenuList->add_gui_element(fileToCreateField,true);
            popupMenuList->add_gui_element(matchProjectFileName, true);

            popupMenuList->add_gui_element(projectNameLabel,false);
            popupMenuList->add_gui_element(projectNameField,true);

            popupMenuList->add_gui_element(projectMainExportLabel, false);
            popupMenuList->add_gui_element(newprojectMainExport, true);

            popupMenuList->add_gui_element(projectMainLanguageLabel, false);
            popupMenuList->add_gui_element(newprojectLanguage, true);

            popupMenuList->add_gui_element(projectCreateErrorLabel, true);
            popupMenuList->add_gui_element(createButton,false);
            popupMenuList->add_gui_element(closeButton,false);
            popupMenuList->process_self();
            if( input->check_keyboard_released(kb_esc) || closeButton->is_clicked() )
            {
                exitOperation = true;
                manualCancel = true;
                newProjectFileName = "";
            }
            else if( createButton->is_clicked() )
            {
                newProjectName = projectNameField->get_string();
                //Checks if a file is actually here or is it still just a folder...
                newProjectLocalFileName = get_local_from_global_file( newProjectName );
                if( (int)newProjectLocalFileName.size() > 0 )
                {
                    if( is_alnum(newProjectName,true, true) )
                    {
                        /*
                        If the path exists, file name checks, is alpha numeric and such
                        We exit this while loop and skip to end of function ready to create
                        a new project.
                        */
                        if( path_exists( get_path_from_file(newProjectFileName)  ) )
                        {
                            manualCancel = false;
                            createButtonPushed = true;
                            exitOperation = true;
                        }
                        else if( (int)newProjectFileName.size() > 0 )
                        {
                            projectCreateErrorLabel->set_name("Empty path given...");
                        }
                        else
                        {
                            projectCreateErrorLabel->set_name("Path does not exist...");
                        }
                    }
                    else
                    {
                        projectCreateErrorLabel->set_name("Project Name must be alphanumeric...");
                    }
                }
                else
                {
                    projectCreateErrorLabel->set_name("Empty project name given...");
                }
            }
            else if( fileFindButton->is_clicked() )
            {
                foundFileName = GPE_GetSaveFileName("Create a New Project File","Game Pencil Projects",GPE_MAIN_GUI->fileOpenProjectDir);
                if( (int)foundFileName.size() > 0 )
                {
                    newProjectFileName = foundFileName;
                    fileToCreateField->set_name(foundFileName);
                    if( matchProjectFileName!=NULL && matchProjectFileName->is_clicked() )
                    {
                        projectNameField->set_string( get_file_noext( get_local_from_global_file(foundFileName) ) );
                    }
                }

            }

            //GPE_Report("Rendering tip of the day");
            GPE_MAIN_RENDERER->reset_viewpoint( );
            if( !GPE_MAIN_WINDOW->is_resized() )
            {
                //if( input->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot( );
                }
                //Update screen
                gcanvas->render_rectangle( elementBox.x-4,elementBox.y-4,elementBox.x+elementBox.w+8,elementBox.y+elementBox.h+8,c_blgray,false,64);
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Color,false);

                gcanvas->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,false);
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Button_Box_Highlighted_Color,true);
                gfs->render_text( elementBox.x+elementBox.w/2,elementBox.y+GENERAL_GPE_GUI_PADDING,popUpCaption,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
                popupMenuList->render_self( );
                //GPE_MAIN_GUI-render_gui_info(GPE_MAIN_RENDERER, true);

                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
                if( GPE_MAIN_GUI!= NULL )
                {
                    GPE_MAIN_GUI->render_gui_info();
                }
            }
            gpe->end_loop();
        }

        input->reset_all_input();

        //Creates project if authorized to do so....
        if( manualCancel==false && (int)newProjectFileName.size()> 0 )
        {
            GPE_ProjectFolder * tPFolder = find_project_from_filename(newProjectFileName);
            if(tPFolder==NULL)
            {
                std::string newProjectDir= get_path_from_file(newProjectFileName);
                std::string newProjectLocalFileName = get_local_from_global_file(newProjectFileName);
                if( (int)newProjectLocalFileName.size()>0 && (int)newProjectName.size() > 0 )
                {
                    std::string newProjectTitle = get_file_noext(newProjectLocalFileName);
                    if( (int)newProjectTitle.size() > 0)
                    {
                        newProjectFileName = newProjectDir+"/"+newProjectTitle+".gppf";
                        newProjectDir = newProjectDir+"/"+newProjectTitle;
                        setup_project_directory(newProjectDir);
                        GPE_Main_Logs->log_general_line("Creating new Project["+newProjectName+"] at ["+newProjectFileName+"] location.");
                        GPE_Main_Logs->log_general_line("Detected Language for new project: ["+newprojectLanguage->get_selected_tag() + "]." );
                        GPE_ProjectFolder * newProject = new GPE_ProjectFolder(newProjectName,newProjectDir,newProjectFileName, newprojectLanguage->get_selected_tag(), true );
                        display_user_alert("Project Opened",newProjectFileName.c_str() );
                        gpeProjects.push_back(newProject);
                        newProject->save_project();
                    }
                }
            }
        }

        //Cleans up all of the UI elements.
        if( popupMenuList!=NULL)
        {
            delete popupMenuList;
            popupMenuList = NULL;
        }

        if( mainMenuLabel!=NULL)
        {
            delete mainMenuLabel;
            mainMenuLabel = NULL;
        }
        if( newprojectMainExport!=NULL)
        {
            delete newprojectMainExport;
            newprojectMainExport = NULL;
        }
        if( projectMainExportLabel!=NULL)
        {
            delete projectMainExportLabel;
            projectMainExportLabel = NULL;
        }
        if( newprojectLanguage!=NULL)
        {
            delete newprojectLanguage;
            newprojectLanguage = NULL;
        }
        if( projectMainLanguageLabel!=NULL)
        {
            delete projectMainLanguageLabel;
            projectMainLanguageLabel = NULL;
        }
        if( projectLocationLabel!=NULL)
        {
            delete projectLocationLabel;
            projectLocationLabel = NULL;
        }
        if( fileToCreateField!=NULL)
        {
            delete fileToCreateField;
            fileToCreateField = NULL;
        }
        if( matchProjectFileName!=NULL)
        {
            delete matchProjectFileName;
            matchProjectFileName = NULL;
        }
        if( closeButton!=NULL)
        {
            delete closeButton;
            closeButton = NULL;
        }
        if( fileFindButton!=NULL)
        {
            delete fileFindButton;
            fileFindButton = NULL;
        }
        gpe->start_loop();
    }
}

void GPE_Gui_Engine::load_tips()
{
    gpeTips.clear();
    gpeTips.push_back("Save your projects often to avoid lost work or memory in the event of a malfunction or crash!");
    gpeTips.push_back("In the Scene Editor, by holding 'M' you are you able to place multiple-objects at once.");
    gpeTips.push_back("Use the [kb_ctrl] KEY plus mouse wheel to horizontally scroll areas.");
    gpeTips.push_back("Be sure to take a look at the documentation for learning to make the most of Game Pencil.");
    gpeTips.push_back("You can have multiple projects open at once.");
    gpeTips.push_back("You can indent / unindent blocks of text/code using the [TAB] and [kb_shift-TAB] keys respectively");
    gpeTips.push_back("Objects inherit variables and functions from their parents. Very helpful for larger projects.");
    gpeTips.push_back("Use the arrow keys in input fields to traverse through previous entries.");
    gpeTips.push_back("Use the shortcut [kb_ctrl]+[O] to open a project to edit.");

    gpeTips.push_back("You are able to remove the background color, grayscale and invert an image using the transform button in the animation, Texture and Tilesheet editors.");
    gpeTips.push_back("Use the project settings editor to edit vital meta-data for your projects.");
    gpeTips.push_back("Need help or resources for your game? Check out the new Asset Store at http://store.pawbyte.com .");
    gpeTips.push_back("You can create a duplicate of your selected text, by pressing [kb_ctrl]+[D] keys.");
    gpeTips.push_back("You are able to give your objects custom functions in the object editor.");
    gpeTips.push_back("Save your current project folder using [kb_ctrl]+[S] keys together.");
    gpeTips.push_back("Assets are copied from their source directory to their project folder.");
    gpeTips.push_back("You can close a tab by clicking it with the middle mouse button or simply clicking on the X-icon.");
    gpeTips.push_back("The Game Pencil Editor is made via the SDL Library and some keyboard shortcuts may not behave 'normally to you'. If you experience any inconveniences please email help@pawbyte.com .");
    gpeTips.push_back("Save your projects often! There may be a few un-found errors in the  Game Pencil Engine and we will hate for a bug to be the cause of the loss of hours of work!");
    gpeTips.push_back("You can create a duplicate of the line the caret is on, by pressing [kb_ctrl]+[D] keys.");
    gpeTips.push_back("Version 1.1 of the GPE allows you to code in pure JavaScript.");
    gpeTips.push_back("You can add macros in your project's settings code editor.");
    gpeTips.push_back("You can debug your program using your web browser's console log.");
    gpeTips.push_back("You can save all open projects at once using the toolbar or buttonbar.");
    gpeTips.push_back("Project source code is provided free of obfuscation.");
    gpeTips.push_back("Use the shortcut [kb_ctrl]+[S] to save the current project you're editing.");
    gpeTips.push_back("You can get the most up to date documentation via http://docs.pawbyte.com");
}

void GPE_Gui_Engine::load_settings()
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_messages( "Loading Gui Settings", "Please Wait...","..." );
    }

    std::string otherColContainerName = "";

    std::string newFileIn = get_user_settings_folder()+"gpe_ide_settings.txt";
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    GPE_Report("Loading settings - "+newFileIn);
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
            std::string colorThemeName;
            float foundFileVersion = 0;
            while ( gameResourceFileIn.good() )
            {
                getline (gameResourceFileIn,currLine); //gets the next line of the file
                currLineToBeProcessed = trim_left_inplace(currLine);
                currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

                if( foundFileVersion <=0 )
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
                else if( foundFileVersion <= 2 )
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


                            if( keyString == "BottomLogsHeight")
                            {
                                if( GPE_Main_Logs!=NULL )
                                {
                                    GPE_Main_Logs->set_height( string_to_int( valString,32 ) );
                                    GPE_Report("Logs Height = "+valString+";");
                                }
                                else
                                {
                                    GPE_Report("main logs = NULL...");
                                }
                            }
                            else if( keyString=="TextAreaViewLineCount")
                            {
                                MAIN_GUI_SETTINGS->showTextEditorLineCount = string_to_bool(valString);
                            }
                            else if( keyString=="TextAreaViewSyntaxHighlighting")
                            {
                                MAIN_GUI_SETTINGS->showTextEditorSyntaxHightlight =string_to_bool(valString);
                            }

                            else if(keyString=="ShowStartPageAtStart")
                            {
                                showStartPageAtStartUp =string_to_bool(valString);
                            }
                            else if(keyString=="ShowTipOfDayAtStart")
                            {
                                showTipsAtStartUp = string_to_bool(valString);
                                MAIN_EDITOR_SETTINGS->showStatupTipsBox->set_clicked(showTipsAtStartUp);
                            }
                            else if( keyString=="ShowFPSCounter")
                            {
                                SHOW_FPS_COUNTER = string_to_bool( valString);
                            }
                            else if(keyString=="ColorTheme")
                            {
                                colorThemeName = valString;
                                if( file_exists(APP_DIRECTORY_NAME+"themes/"+colorThemeName) )
                                {

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

void GPE_Gui_Engine::open_new_project()
{
    std::string newProjectFileName = GPE_GetOpenFileName("Load Your Project","Game Pencil Projects",fileOpenProjectDir);
    open_project( newProjectFileName);
}

void GPE_Gui_Engine::open_project(std::string newProjectFileName)
{
    if( (int)newProjectFileName.size()> 5 )
    {
        newProjectFileName = string_replace_all(newProjectFileName,"\\","/");
        std::string newProjectDir = get_file_noext(newProjectFileName);
        GPE_ProjectFolder * fFolder = find_project_from_filename( newProjectFileName );
        if( fFolder==NULL)
        {
            if( get_file_ext(newProjectFileName)=="gppf" )
            {
                if( file_exists(newProjectFileName) )
                {
                    if( MAIN_GUI_SETTINGS && MAIN_GUI_SETTINGS->useShortProjectNames )
                    {
                        GPE_Main_Logs->log_general_line("Attempting to load ["+ get_local_from_global_file( newProjectFileName )+"]...");
                    }
                    else
                    {
                        GPE_Main_Logs->log_general_line("Attempting to load ["+newProjectFileName+"]...");
                    }
                    std::string previousProjectInView = CURRENT_PROJECT_NAME;
                    std::string newProjectTitle = "Loaded Project";
                    GPE_ProjectFolder * newProject = new GPE_ProjectFolder(newProjectTitle,newProjectDir,newProjectFileName);
                    CURRENT_PROJECT_NAME = newProjectFileName;
                    gpeProjects.push_back(newProject);
                    GPE_Report("New Project added into editor.");
                    bool projectLoadedSuccessfuly = newProject->load_project_file(newProjectFileName);
                    if( projectLoadedSuccessfuly )
                    {
                        newProject->integrate_syntax();
                        set_current_gpe_project_from_name( newProjectFileName );
                        GPE_Report("Checking with project list: ["+newProject->get_project_name()+"].");
                        add_to_recent_project_list(newProjectFileName,true );
                        GPE_Report("Project added to GPE ["+newProject->get_project_name()+"]");
                        GPE_Main_Logs->log_general_line("Done.");
                        if( MAIN_GUI_SETTINGS && MAIN_GUI_SETTINGS->useShortProjectNames )
                        {
                            GPE_Main_Logs->log_general_line("Project ["+ get_local_from_global_file( newProjectFileName )+"] processed...");
                        }
                        else
                        {
                            GPE_Main_Logs->log_general_line("Project ["+newProjectFileName+"] processed...");
                        }
                    }
                    else
                    {
                        set_current_gpe_project_from_name( previousProjectInView );
                        remove_project(newProjectFileName);
                    }
                }
                else
                {
                    display_user_alert("Project Open Error","File does not exist.");
                    GPE_Main_Logs->log_debug_line("Project Open Error - File does not exist ("+newProjectFileName+")...");
                }
            }
            else
            {
                display_user_alert("Project Open Error","Invalid Project File Type given.");
            }
        }
        else
        {
            //display_user_alert("Project Open Error","This project is already open!");
            display_user_alert("Project Open Error","This project is already open!");
        }
    }
}


void GPE_Gui_Engine::prerender_gui( )
{
    mainResourceTree->prerender_self( );
    mainToolBar->prerender_self( );
    MAIN_CONTEXT_MENU->prerender_self( );
}


void GPE_Gui_Engine::process_overlay_message()
{
    if( (int)GPE_Action_Message.size()== 0 )
    {
        return;
    }
    if( GPE_Action_Message=="Add Folder")
    {
        GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
        if( fFolder!=NULL)
        {
            std::string nFolderName = get_string_from_popup("Name Your Folder","Please enter the new name","newFolder" );
            if( (int)nFolderName.size()>0)
            {
                fFolder->create_blank_folder(LAST_CLICKED_RESOURCE, nFolderName);
            }
        }
    }
    else if( GPE_Action_Message=="Rename Folder" && LAST_CLICKED_RESOURCE!=NULL)
    {
        std::string nFolderName = get_string_from_popup("Rename Your Folder","Please enter the new name",LAST_CLICKED_RESOURCE->get_name() );
        if( (int)nFolderName.size()>0)
        {
            LAST_CLICKED_RESOURCE->set_name( nFolderName);
        }
    }
    else if( GPE_Action_Message=="Rename Resource" && LAST_CLICKED_RESOURCE!=NULL )
    {
        if( LAST_CLICKED_RESOURCE->get_resource_type()!=RESOURCE_TYPE_PROJECT_SETTINGS)
        {
            std::string nResName = get_string_from_popup("Name Your Resource","Please enter the new name",LAST_CLICKED_RESOURCE->get_name() );
            if( (int)nResName.size()>0)
            {
                LAST_CLICKED_RESOURCE->set_name(nResName);
            }
        }
    }
    else if( GPE_Action_Message=="Clear Resource Folder" || GPE_Action_Message=="Clear Resource Folder Contents")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( GPE_Display_Basic_Prompt("Warning","Are you sure you will like to clear this folder's contents?")==DISPLAY_QUERY_YES)
            {
                /*Coming in Version 1.1.3...
                GPE_GeneralResourceContainer * parRes = LAST_CLICKED_RESOURCE->parentResource;
                parRes->delete_resource(LAST_CLICKED_RESOURCE);
                LAST_CLICKED_RESOURCE = NULL;
                RESOURCE_TO_DRAG = NULL;*/
            }
        }
    }
    else if( GPE_Action_Message=="Delete Folder" || GPE_Action_Message=="Remove Folder")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( GPE_Display_Basic_Prompt("Warning","Are you sure you will like to delete this folder?")==DISPLAY_QUERY_YES)
            {
                GPE_GeneralResourceContainer * parRes = LAST_CLICKED_RESOURCE->parentResource;
                parRes->delete_resource(LAST_CLICKED_RESOURCE);
                LAST_CLICKED_RESOURCE = NULL;
                RESOURCE_TO_DRAG = NULL;
            }
        }
    }
    else if( GPE_Action_Message=="Delete Resource" || GPE_Action_Message=="Remove Resource")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( LAST_CLICKED_RESOURCE->get_resource_type()!=RESOURCE_TYPE_PROJECT_SETTINGS)
            {
                if( GPE_Display_Basic_Prompt("Warning","Are you sure you will like to delete this resource?")==DISPLAY_QUERY_YES)
                {
                    GPE_GeneralResourceContainer * parRes = LAST_CLICKED_RESOURCE->parentResource;
                    if( parRes!=NULL)
                    {
                        parRes->delete_resource(LAST_CLICKED_RESOURCE);
                        LAST_CLICKED_RESOURCE = NULL;
                        RESOURCE_TO_DRAG = NULL;
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Find Resource..." || GPE_Action_Message=="Find Local Resource..." )
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->get_resource_type()!=RESOURCE_TYPE_PROJECT_SETTINGS )
        {
            std::string resourceNeedle = get_string_from_popup("Project Resource Search","Resource name:","");
            if( (int)resourceNeedle.size() > 0 )
            {
                GPE_GeneralResourceContainer * foundRes = LAST_CLICKED_RESOURCE->find_resource_from_name(resourceNeedle);
                if( foundRes!=NULL)
                {
                    generalGameResource * foundHeldRes = foundRes->get_held_resource();
                    if( GPE_Main_TabManager!=NULL && foundHeldRes!=NULL )
                    {
                        GPE_Main_TabManager->add_new_tab(foundHeldRes);
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Add Resource")
    {
        GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
        if( fFolder!=NULL)
        {
            int tempResType = LAST_CLICKED_RESOURCE->get_resource_type();
            if( tempResType > RESOURCE_TYPE_PROJECT_SETTINGS && tempResType < RESOURCE_TYPE_MAX )
            {
                std::string newResName = get_string_from_popup("Please name your new "+RESOURCE_TYPE_NAMES[tempResType]+".","Please use an alphanumeric value","" );
                if( (int)newResName.size() > 0 && is_alnum( newResName) )
                {
                    GPE_GeneralResourceContainer * newResource = fFolder->create_blank_resource(tempResType,LAST_CLICKED_RESOURCE, newResName );
                    if( newResource!=NULL)
                    {
                        GPE_Main_TabManager->add_new_tab(newResource->get_held_resource() );
                        GPE_MAIN_GUI->mainResourceTree->selectedSubOption = newResource->get_global_id();
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Duplicate Resource" || GPE_Action_Message=="Clone Resource")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( !LAST_CLICKED_RESOURCE->is_folder() && !LAST_CLICKED_RESOURCE->is_super_project_folder() && !LAST_CLICKED_RESOURCE->is_super_project_folder() )
            {
                int tempResType = LAST_CLICKED_RESOURCE->get_resource_type();

                if( tempResType!=RESOURCE_TYPE_TEXTURE && tempResType!=RESOURCE_TYPE_TILESHEET && tempResType!=RESOURCE_TYPE_ANIMATION && tempResType!=RESOURCE_TYPE_AUDIO && tempResType!=RESOURCE_TYPE_VIDEO && tempResType!=RESOURCE_TYPE_PROJECT_SETTINGS )
                {
                    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
                    if( fFolder!=NULL)
                    {
                        std::string newResName = get_string_from_popup("Please name your new copy of " + LAST_CLICKED_RESOURCE->get_name() + " ["+RESOURCE_TYPE_NAMES[tempResType]+"].","Please use an alphanumeric value","" );
                        if( (int)newResName.size() > 0 && is_alnum( newResName) )
                        {
                            GPE_GeneralResourceContainer * newResource = fFolder->create_blank_resource( tempResType,LAST_CLICKED_RESOURCE->parentResource,newResName );
                            if( newResource!=NULL)
                            {
                                LAST_CLICKED_RESOURCE->save_container(get_user_temp_folder()+"temp_duplicated_resource.gpf");
                                newResource->preprocess_container(get_user_temp_folder()+"temp_duplicated_resource.gpf");
                                newResource->set_name( newResName );
                                newResource->detect_name_change(true);
                                GPE_Main_TabManager->add_new_tab(newResource->get_held_resource() );
                                GPE_MAIN_GUI->mainResourceTree->selectedSubOption = newResource->get_global_id();
                            }
                            else
                            {
                                GPE_Report("Unable to duplicate resource - new resource is NULL...");
                            }
                        }
                    }
                    else
                    {
                        GPE_Report("Unable to duplicate resource - Unable to find project for resource ");
                    }
                }
            }
            else
            {
                GPE_Report("Unable to duplicate resource - Last clicked resource is a folder...");
            }
        }
        else
        {
            GPE_Report("Unable to duplicate resource - Last Clicked Resource or is parent equals NULL.");
        }
    }
    else if( GPE_Action_Message=="Close Project")
    {
        GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
        if( fFolder!=NULL)
        {
            int projectSaveState = GPE_Display_Basic_Prompt("Project Warning","Will you like to save this project before closing(Recommended)?",true);

            if( projectSaveState==DISPLAY_QUERY_YES)
            {
                save_current_project();
            }
            if( projectSaveState!=DISPLAY_QUERY_CANCEL)
            {
                //Close the project
                std::string projectNameToDelete = fFolder->get_project_file_name();
                if( GPE_LOADER != NULL )
                {
                    GPE_LOADER->update_messages( "Closing Game Project",projectNameToDelete, "Please wait..." );
                }

                GPE_Main_TabManager->close_tabs_from_project( projectNameToDelete );

                remove_project(projectNameToDelete);
            }
        }
    }
    else if( GPE_Action_Message=="New Project")
    {
        launch_new_project();
    }
    else if( GPE_Action_Message=="Open Project")
    {
        open_new_project();
    }
    else if( GPE_Action_Message== "Open Project Directory" )
    {
        if( CURRENT_PROJECT!=NULL)
        {
            GPE_OpenProgram(CURRENT_PROJECT->get_project_directory() );
        }
    }
    else if( GPE_Action_Message=="Save Project")
    {
        save_current_project();
    }
    else if( GPE_Action_Message=="Save All Projects")
    {
        save_all_projects();
    }
    else if( GPE_Action_Message=="Build Project" || GPE_Action_Message=="Build GAME")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
        }
        else if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Clean [HTML5] Build Folder")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            CURRENT_PROJECT->clean_build_folder(GPE_BUILD_HTML5);
        }
    }
    else if( GPE_Action_Message=="Clean [Linux] Build Folder")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            CURRENT_PROJECT->clean_build_folder(GPE_BUILD_LINUX);
        }
    }
    else if( GPE_Action_Message=="Clean [OSX] Build Folder")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            CURRENT_PROJECT->clean_build_folder(GPE_BUILD_MAC);
        }
    }
    else if( GPE_Action_Message=="Build HTML5 APP" || GPE_Action_Message=="Build HTML5 GAME")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("HTML5");
        }
        else if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build HTML5 APP & RUN" || GPE_Action_Message=="Build HTML5 GAME & RUN")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            /*
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("HTML5");
            */
            GPE_MAIN_GUI->export_current_project_html5(true);
        }
        else if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if(GPE_Action_Message=="RUN HTML5 GAME")
    {
        if( CURRENT_PROJECT!=NULL)
        {
            rum_current_project("",GPE_BUILD_HTML5);
        }
        else if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build Nintendo Switch GAME")
    {
        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("Switch");
        }
        else if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build WINDOWS GAME")
    {
        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("Windows");
        }
        else if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build OSX GAME")
    {
        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("OSX");
        }
        else if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build LINUX GAME")
    {
        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("Linux");
        }
        else if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Clean [HTML5] Build Folder")
    {
        clean_current_project_build_folder(GPE_BUILD_HTML5);
    }
    else if( GPE_Action_Message=="Clean [Windows] Build Folder")
    {
        clean_current_project_build_folder( GPE_BUILD_WINDOWS );
    }
    else if( GPE_Action_Message=="Clean [OSX] Build Folder")
    {
        clean_current_project_build_folder( GPE_BUILD_MAC );
    }
    else if( GPE_Action_Message=="Clean [Linux] Build Folder")
    {
        clean_current_project_build_folder( GPE_BUILD_LINUX );
    }
    else if( GPE_Action_Message=="Online Documentation"|| GPE_Action_Message=="Documentation")
    {
        GPE_OpenURL("http://docs.pawbyte.com/");
    }
    else if( GPE_Action_Message=="EULA" || GPE_Action_Message=="License")
    {
        GPE_OpenURL("http://gamepencil.pawbyte.com/eula/");
    }
    else if( GPE_Action_Message=="Check Updates"|| GPE_Action_Message=="Check Version")
    {
        GPE_OpenURL("http://gamepencil.pawbyte.com/get-game-pencil-engine/");
    }

    else if( GPE_Action_Message=="Community" || GPE_Action_Message=="Online Forums" || GPE_Action_Message=="Community" || GPE_Action_Message=="Forums")
    {
        GPE_OpenURL("http://community.pawbyte.com/");
    }
    else if( GPE_Action_Message=="Community Chat")
    {
        GPE_OpenURL("https://discord.gg/bua3rDW");
    }
    else if( GPE_Action_Message=="Toggle Fullscreen Mode")
    {
        if( !GPE_MAIN_WINDOW->is_fullscreen() )
        {
            MAIN_OVERLAY->update_temporary_message("Entering Full-Screen","Press [F11] to exit","",1);
        }
        GPE_MAIN_WINDOW->toggle_fullscreen();
    }
    else if( GPE_Action_Message=="Toggle Logs")
    {
        GPE_DOCK->add_to_panel("Logs", DOCK_BOTTOM_MIDDLE );
        GPE_Main_Logs->toggle_manager();
    }
    else if( GPE_Action_Message=="Reset Dock" )
    {
        GPE_DOCK->reset_dock();
        if( GPE_Main_Logs->is_enabled() ==false )
        {
            GPE_Main_Logs->toggle_manager();
        }
        if( GPE_Main_TabManager->requests_fullscreen() )
        {
            GPE_Main_TabManager->toggle_full_width();
        }
    }
    else if( GPE_Action_Message=="Clear Recent Projects List")
    {
        GPE_MAIN_GUI->clear_recent_project_list();
    }
    else if( GPE_Action_Message=="Toggle Line Count")
    {
        MAIN_GUI_SETTINGS->showTextEditorLineCount = !MAIN_GUI_SETTINGS->showTextEditorLineCount;
        save_settings();
    }
    else if(GPE_Action_Message=="Toggle Syntax Highlighting")
    {
        MAIN_GUI_SETTINGS->showTextEditorSyntaxHightlight = !MAIN_GUI_SETTINGS->showTextEditorSyntaxHightlight;
        save_settings();
    }
    else if( GPE_Action_Message=="Find...")
    {
        if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->open_search_mode();
        }
        MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FIND;
        MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
    }
    else if( GPE_Action_Message=="Find in files...")
    {
        if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->open_search_mode();
        }
        MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FINDALL;
        MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
    }
    else if( GPE_Action_Message=="Goto Line...")
    {
        MAIN_SEARCH_CONTROLLER->textSearchMode = SEARCH_MODE_GOTO;
        MAIN_SEARCH_CONTROLLER->goToLineStringBox->set_string("1");
        MAIN_SEARCH_CONTROLLER->goToLineStringBox->switch_inuse(true);
        if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->open_search_mode();
        }
    }
    else if( GPE_Action_Message=="Find Project Resource...")
    {
        if( CURRENT_PROJECT!=NULL&& CURRENT_PROJECT->RESC_PROJECT_FOLDER!=NULL)
        {
            std::string resourceNeedle = get_string_from_popup("Project Resource Search","Resource name:","");
            if( (int)resourceNeedle.size() > 0 )
            {
                GPE_GeneralResourceContainer * foundRes = CURRENT_PROJECT->RESC_PROJECT_FOLDER->find_resource_from_name(resourceNeedle);
                if( foundRes!=NULL)
                {
                    generalGameResource * foundHeldRes = foundRes->get_held_resource();
                    if( GPE_Main_TabManager!=NULL && foundHeldRes!=NULL)
                    {
                        GPE_Main_TabManager->add_new_tab(foundHeldRes);
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Replace...")
    {
        MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACE;
        MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true );
        if( GPE_Main_Logs!=NULL)
        {
            GPE_Main_Logs->open_replace_mode();
        }
    }
    else if( GPE_Action_Message=="Use Default Color Theme")
    {
        GPE_MAIN_THEME = GPE_DEFAULT_TEMPLATE;
        prerender_gui();
        save_settings();
    }
    else if( GPE_Action_Message=="Start Page")
    {
        GPE_Main_TabManager->add_new_tab(MAIN_START_PAGE);
    }
    else if( GPE_Action_Message=="Tip of the Day")
    {
        GPE_Show_Tip_Of_Day();
    }
    else if( GPE_Action_Message=="Report Issue")
    {
        GPE_OpenURL("https://github.com/pawbyte/Game-Pencil-Engine/issues");
    }
    else if( GPE_Action_Message=="About" || GPE_Action_Message=="About..." || GPE_Action_Message=="About Game Pencil Engine" )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_ABOUT_PAGE);
    }
    else if( GPE_Action_Message=="Help" || GPE_Action_Message=="Help..." || GPE_Action_Message=="General Help"  )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_HELP_PAGE);
    }
    else if( GPE_Action_Message=="User Settings" || GPE_Action_Message=="User Settings..." || GPE_Action_Message=="Game Pencil User Settings" )
    {
        GPE_Main_TabManager->add_new_tab(MAIN_EDITOR_SETTINGS);
    }
    else if( GPE_Action_Message=="Game Controller Tester" )
    {
        GPE_Main_TabManager->add_new_tab( MAIN_CONTROLLER_TESTER );
    }
    else if( GPE_Action_Message=="JS Compiler Settings" )
    {
        GPE_Main_TabManager->add_new_tab( GPE_JS_COMPILER_SETTINGS );
    }
    else if( GPE_Action_Message=="Extra Tools" )
    {
        GPE_Main_TabManager->add_new_tab( MAIN_EXTRA_TOOLS );
    }
    else if( GPE_Action_Message=="C++ Builder Settings" )
    {
        GPE_Main_TabManager->add_new_tab( GPE_CPP_BUILDER_SETTINGS );
    }
    else if( string_starts(GPE_Action_Message, "Import ") )
    {
        //Import Resources ImportResources
        if( CURRENT_PROJECT!=NULL)
        {
            std::string remainingType = get_substring(GPE_Action_Message, 7);
            int tempResType = 0;
            bool foundResType = false;
            for( int iResType = 0; iResType < RESOURCE_TYPE_MAX; iResType++ )
            {
                if( remainingType == RESOURCE_TYPE_NAMES[iResType] )
                {
                    foundResType = true;
                    tempResType = iResType;
                    break;
                }
            }

            if( foundResType )
            {
                std::string fileToImportName = GPE_GetOpenFileName( GPE_Action_Message,".gpf", MAIN_GUI_SETTINGS->fileOpenImportFileDir );
                if( file_exists(fileToImportName) && get_file_ext(fileToImportName) =="gpf" )
                {
                    GPE_GeneralResourceContainer * newResource = CURRENT_PROJECT->create_blank_resource(tempResType,NULL, get_file_noext(get_local_from_global_file( fileToImportName) ) );
                    if( newResource!=NULL)
                    {
                        GPE_Main_TabManager->add_new_tab(newResource->get_held_resource() );
                        newResource->preprocess_container(fileToImportName);
                        GPE_MAIN_GUI->mainResourceTree->selectedSubOption = newResource->get_global_id();
                        newResource->detect_name_change(true);
                    }
                    else
                    {
                        display_user_alert("Import file error","Unable to create new "+RESOURCE_TYPE_NAMES[tempResType]+" resource.");
                    }
                }
                else
                {
                    display_user_alert("Import file error","Unable to import "+RESOURCE_TYPE_NAMES[tempResType]+" file ["+get_local_from_global_file(fileToImportName)+"]");
                }
            }
        }
    }
    else if( string_starts(GPE_Action_Message, "Toggle ") && string_ends(GPE_Action_Message, " Pane") )
    {
        GPE_Action_Message = string_replace_all(GPE_Action_Message,"Toggle ","");
        GPE_Action_Message = string_replace_all(GPE_Action_Message," Pane","");
        GPE_DOCK->toggle_default_pane( GPE_Action_Message );
    }
    else if( file_exists(GPE_Action_Message) )
    {
        if( get_file_ext(GPE_Action_Message)=="gppf")
        {
            open_project(GPE_Action_Message);
        }
    }
    else
    {
        GPE_Report( "Unknown Command: ["+GPE_Action_Message+"]" );
    }
}

void GPE_Gui_Engine::process_window_title()
{
    if( GPE_MAIN_RENDERER!=NULL )
    {
        std::string windowCurrentTitle = "";
        if( CURRENT_PROJECT!=NULL)
        {
            windowCurrentTitle = CURRENT_PROJECT->get_project_name();
        }
        if( GPE_VERSION_UPDATE_NUMBER==0)
        {
            windowCurrentTitle+= "Game Pencil Engine   "+float_to_string(GPE_VERSION_FLOAT_NUMBER)+".0 -";
        }
        else
        {
            windowCurrentTitle+= "Game Pencil Engine   "+float_to_string(GPE_VERSION_FLOAT_NUMBER)+" -";
        }

        if( editorReleaseType==RELEASE_TYPE_PRODUCTION)
        {
            windowCurrentTitle+=" [Production]";
        }
        else if( editorReleaseType==RELEASE_TYPE_RC)
        {
            windowCurrentTitle+=" [Release Candidate]";
        }
        else if( editorReleaseType== RELEASE_TYPE_BETA)
        {
            windowCurrentTitle+=" [Beta]";
        }
        else if( editorReleaseType== RELEASE_TYPE_ALPHA)
        {
            windowCurrentTitle+=" [Alpha]";
        }
        if( !GPE_MAIN_WINDOW->windowHasFocus)
        {
            windowCurrentTitle += " Out of Focus |";
        }
        if( !GPE_MAIN_WINDOW->windowHasMouse)
        {
            windowCurrentTitle += " Mouse Outside |";
        }
        GPE_MAIN_WINDOW->set_window_title(windowCurrentTitle);
    }
}

void GPE_Gui_Engine::render_foreground_engine()
{
    if(GPE_IS_LOADING)
    {
        gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_WAIT ) );
    }

    GPE_MAIN_RENDERER->reset_viewpoint();

    if( mainToolBar!=NULL && MAIN_CONTEXT_MENU!=NULL )
    {
        if( !mainToolBar->is_open() && !MAIN_CONTEXT_MENU->is_open() )
        {
            if( GPE_DOCK!=NULL )
            {
                GPE_DOCK->render_self( NULL, NULL );
            }

            render_gui_info();
            MAIN_OVERLAY->render_temporary_message();
            GPE_MAIN_HIGHLIGHTER->render_code_highlights();
            GPE_MAIN_HIGHLIGHTER->render_code_suggestions();
            if( RESOURCE_TO_DRAG!=NULL)
            {
                RESOURCE_TO_DRAG->render_option(  input->mouse_x+32, input->mouse_y, -1,NULL, NULL,false, true);
                gfs->render_text_boxed( input->mouse_x+32,input->mouse_y,RESOURCE_TO_DRAG->get_name(),GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_MAIN_THEME->PopUp_Box_Color,FONT_TEXTINPUT,FA_LEFT,FA_TOP );
            }

        }
        mainToolBar->render_toolbar( NULL);
        MAIN_CONTEXT_MENU->render_self( );
        ////if( forceRedraw )
        {
            gfs->render_fps(FONT_TEXTINPUT, GPE_MAIN_THEME->Main_Box_Font_Color, SCREEN_WIDTH, 32 );
        }
    }
}

void GPE_Gui_Engine::render_gui_info()
{
    if( GPE_Main_Statusbar!=NULL)
    {
        GPE_Main_Statusbar->render_self( NULL, NULL);
    }
    MAIN_OVERLAY->render_tooltip(  input->mouse_x,input->mouse_y+32);
}

void GPE_Gui_Engine::remove_project(std::string projectFileName )
{
    if( (int)projectFileName.size()>0 )
    {
        GPE_ProjectFolder * tPFolder = NULL;
        for( int i = (int)gpeProjects.size()-1; i>=0; i--)
        {
            tPFolder = gpeProjects[i];
            if( tPFolder!=NULL )
            {
                if( tPFolder->get_project_file_name().compare(projectFileName)==0 )
                {
                    if( GPE_LOADER != NULL )
                    {
                        GPE_LOADER->update_messages( "Closing Game Project ["+tPFolder->get_project_name()+"]", projectFileName,"PLEASE WAIT" );
                    }
                    GPE_Main_TabManager->close_tabs_from_project( projectFileName );
                    if(CURRENT_PROJECT!=NULL)
                    {
                        if( CURRENT_PROJECT->get_project_name()==tPFolder->get_project_name() )
                        {
                            CURRENT_PROJECT = NULL;
                            CURRENT_PROJECT_NAME = "";
                        }
                    }

                    if( GPE_MAIN_GUI!=NULL && GPE_MAIN_GUI->mainResourceTree!=NULL)
                    {
                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Removing from Projects Menu.." );
                        }

                        GPE_MAIN_GUI->mainResourceTree->remove_project_resources(projectFileName);
                        if( GPE_DOCK != NULL)
                        {
                            GPE_DOCK->clear_all_panels();
                        }

                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Removed from Projects Menu.." );
                        }
                    }

                    delete tPFolder;
                    tPFolder = NULL;
                    gpeProjects.erase(gpeProjects.begin()+i );

                    GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Clearing cache..." );
                    gfs->clear_font_cache();
                    GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Cache Cleared..." );
                }
            }
        }
    }
}

void GPE_Gui_Engine::reset_gui_info()
{
    //update_tooltip("");
    if( GPE_Main_Statusbar!=NULL)
    {
        GPE_Main_Statusbar->reset_statusbar();
        /*GPE_Main_Statusbar->set_coords(0,SCREEN_HEIGHT-32);
        GPE_Main_Statusbar->set_width(SCREEN_WIDTH);
        GPE_Main_Statusbar->set_height(32);*/
    }
}

void GPE_Gui_Engine::save_all_projects()
{
    GPE_ProjectFolder * tPFolder = NULL;
    for( int i = 0; i < (int)gpeProjects.size(); i++ )
    {
        tPFolder = gpeProjects[i];
        if( tPFolder!=NULL)
        {
            tPFolder->save_project();
        }
    }

}

void GPE_Gui_Engine::export_current_project_html5(bool runGameOnCompile)
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
    if( fFolder!=NULL)
    {
        fFolder->save_project();
        fFolder->export_project_html5("","",GPE_BUILD_HTML5,runGameOnCompile);
    }
}

void GPE_Gui_Engine::rum_current_project(std::string projectBuildDirectory, int buildMetaTemplate)
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
    if( fFolder!=NULL)
    {
        fFolder->run_project(projectBuildDirectory, buildMetaTemplate);
    }
}


void GPE_Gui_Engine::save_current_project()
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(CURRENT_PROJECT_NAME);
    if( fFolder!=NULL)
    {
        fFolder->save_project();
    }
}

void GPE_Gui_Engine::save_settings()
{
    std::string newSaveDataFilename = get_user_settings_folder()+"gpe_ide_settings.txt";
    std::ofstream newSaveDataFile( newSaveDataFilename.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            //write_header_on_file(&newSaveDataFile);
            newSaveDataFile << "Version=" << GPE_VERSION_FLOAT_NUMBER << "\n";
            if( GPE_Main_Logs!=NULL )
            {
                newSaveDataFile << "BottomLogsHeight=" << GPE_Main_Logs->get_height() << "\n";

            }
            newSaveDataFile << "ShowFPSCounter=" << SHOW_FPS_COUNTER << "\n";
            newSaveDataFile << "ShowStartPageAtStart=" << showStartPageAtStartUp << "\n";
            newSaveDataFile << "ShowTipOfDayAtStart=" << showTipsAtStartUp <<  "\n";

            if( MAIN_GUI_SETTINGS!=NULL )
            {
                newSaveDataFile << "TextAreaViewLineCount=" << MAIN_GUI_SETTINGS->showTextEditorLineCount << "\n";
            }
            if( MAIN_GUI_SETTINGS!=NULL )
            {
                newSaveDataFile << "TextAreaViewSyntaxHighlighting=" << MAIN_GUI_SETTINGS->showTextEditorSyntaxHightlight << "\n";
            }
            if( GPE_MAIN_THEME!=NULL)
            {
                newSaveDataFile << "ColorTheme=" << GPE_MAIN_THEME->themeLocalLocation << "\n";
            }
            newSaveDataFile.close();
        }
    }
}

void GPE_Gui_Engine::setup_project_directory(std::string newProjectDir)
{
    if( (int)newProjectDir.size() > 3)
    {
        create_directory(newProjectDir);
        create_directory(newProjectDir+"/gpe_project");
        create_directory(newProjectDir+"/gpe_project/builds");

        create_directory(newProjectDir+"/gpe_project/data");

        create_directory(newProjectDir+"/gpe_project/gpebackups");

        create_directory(newProjectDir+"/gpe_project/resources");
        create_directory(newProjectDir+"/gpe_project/resources/achievements");
        create_directory(newProjectDir+"/gpe_project/resources/audio");
        //Class Directory addition for 1.14
        create_directory(newProjectDir+"/gpe_project/resources/animations");
        create_directory(newProjectDir+"/gpe_project/resources/classes");
        create_directory(newProjectDir+"/gpe_project/resources/fonts");
        create_directory(newProjectDir+"/gpe_project/resources/functions");
        create_directory(newProjectDir+"/gpe_project/resources/lights");
        create_directory(newProjectDir+"/gpe_project/resources/objects");
        create_directory(newProjectDir+"/gpe_project/resources/paths");
        create_directory(newProjectDir+"/gpe_project/resources/particles");
        create_directory(newProjectDir+"/gpe_project/resources/scenes");
        create_directory(newProjectDir+"/gpe_project/resources/textures");
        create_directory(newProjectDir+"/gpe_project/resources/tilesheets");
        create_directory(newProjectDir+"/gpe_project/resources/videos");
        create_directory(newProjectDir+"/gpe_project/source");
        create_directory(newProjectDir+"/gpe_project/source/GPE");
        create_directory(newProjectDir+"/gpe_project/source/AOSGUI");
    }
}

std::string GPE_Gui_Engine::setup_cpp_folder(std::string buildDirectory,int buildType, int buildBits, bool inDebugMode)
{
    if( GPE_Main_Logs!=NULL)
    {
        if( (int)buildDirectory.size() < 3)
        {
            GPE_Main_Logs->log_build_error("Unable to create ["+buildDirectory+"] folder. Unallowed name.");
            return "";
        }
        std::string newBuildDirectory = buildDirectory;

        create_directory(buildDirectory);
        if( path_exists(buildDirectory) )
        {
            if( ( buildType==GPE_BUILD_WINDOWS || buildType==GPE_BUILD_LINUX ) )
            {
                create_directory(buildDirectory+"/gamedata");
                create_directory(buildDirectory+"/resources");
                if( path_exists( buildDirectory+"/resources") )
                {
                    newBuildDirectory = buildDirectory+"/resources";
                    create_directory(newBuildDirectory+"/animations");
                    create_directory(newBuildDirectory+"/audio");
                    create_directory(newBuildDirectory+"/fonts");
                    create_directory(newBuildDirectory+"/lights");
                    create_directory(newBuildDirectory+"/particles");
                    create_directory(newBuildDirectory+"/animations");
                    create_directory(newBuildDirectory+"/textures");
                    create_directory(newBuildDirectory+"/tilesheets");
                    create_directory(newBuildDirectory+"/videos");
                }
                else
                {
                    GPE_Main_Logs->log_build_line("Unable to access ["+buildDirectory+"/resources] folder...");
                }
            }
        }
        else
        {
            GPE_Main_Logs->log_build_error("Unable to create ["+buildDirectory+"] folder. Unable to access...");
        }
        return newBuildDirectory;
    }
    return "";
}

std::string GPE_Gui_Engine::setup_js_folder(std::string buildDirectory,int buildType, int buildBits, bool inDebugMode, int nativeBuildType)
{
    if( GPE_Main_Logs!=NULL)
    {
        if( (int)buildDirectory.size() < 3)
        {
            GPE_Main_Logs->log_build_error("Unable to create ["+buildDirectory+"] folder. Unallowed name.");
            return "";
        }

        /*if( buildType!=GPE_BUILD_HTML5 )
        {
            if( buildBits==64)
            {
                buildDirectory+="_64";
            }
            else
            {
                buildDirectory+="_32";
            }
        }
        if( inDebugMode)
        {
            //buildDirectory+="_debug";
        }
        */
        std::string newBuildDirectory = buildDirectory;

        create_directory(buildDirectory);
        if( path_exists(buildDirectory) )
        {
            if( ( buildType==GPE_BUILD_WINDOWS || buildType==GPE_BUILD_LINUX ) && nativeBuildType==Native_Electron)
            {
                create_directory(buildDirectory+"/locales");
                create_directory(buildDirectory+"/resources");
                create_directory(buildDirectory+"/resources/app");
                if( path_exists( buildDirectory+"/resources/app") )
                {
                    newBuildDirectory = buildDirectory+"/resources/app";
                }
                else
                {
                    GPE_Main_Logs->log_build_line("Unable to access ["+buildDirectory+"/resources/app] folder...");
                }
            }
            else if( buildType== GPE_BUILD_MAC && nativeBuildType==Native_Electron)
            {
                create_directory(buildDirectory+"/Electron.app");
                create_directory(buildDirectory+"/Electron.app/Contents");
                create_directory(buildDirectory+"/Electron.app/Contents/Resources");
                if( path_exists( buildDirectory+"/Electron.app/Contents/Resources") )
                {
                    newBuildDirectory = buildDirectory+"/Electron.app/Contents/Resources";
                }
                else
                {
                    GPE_Main_Logs->log_build_line("Unable to access ["+buildDirectory+"/Electron.app/Contents/Resources] folder...");
                }
            }

            create_directory(newBuildDirectory+"/css");
            create_directory(newBuildDirectory+"/gamedata");
            create_directory(newBuildDirectory+"/js");
            create_directory(newBuildDirectory+"/js/lib");

            create_directory(newBuildDirectory+"/res");

            create_directory(newBuildDirectory+"/resources");
            create_directory(newBuildDirectory+"/resources/animations");
            create_directory(newBuildDirectory+"/resources/audio");
            create_directory(newBuildDirectory+"/resources/fonts");
            create_directory(newBuildDirectory+"/resources/lights");
            create_directory(newBuildDirectory+"/resources/particles");
            create_directory(newBuildDirectory+"/resources/animations");
            create_directory(newBuildDirectory+"/resources/textures");
            create_directory(newBuildDirectory+"/resources/tilesheets");
            create_directory(newBuildDirectory+"/resources/videos");

            if( nativeBuildType ==Native_Electron )
            {
                if( buildType == GPE_BUILD_WINDOWS)
                {
                    if( GPE_LOADER != NULL )
                    {
                        GPE_LOADER->update_messages( "Copying Electron-Windows Files", "Please wait","Do NOT CLOSE" );
                    }
                    if( buildBits==64)
                    {
                        GPE_Main_Logs->log_build_line("Copying Electron-Windows 64Bit Files...");
                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [game.exe and dlls] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/win64build_electron",buildDirectory);

                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [game.exe and dlls] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/win64build_electron/locales",buildDirectory+"/locales");


                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [resources] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/win64build_electron/resources",buildDirectory+"/resources", true);
                    }
                    else
                    {
                        GPE_Main_Logs->log_build_line("Copying Electron-Windows 32Bit Files...");
                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [game.exe and dlls] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/win32build_electron",buildDirectory);

                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [locales] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/win32build_electron/locales",buildDirectory+"/locales");

                        if( GPE_LOADER!=NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [resources] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/win32build_electron/resources",buildDirectory+"/resources", true);
                    }
                }
                else if( buildType == GPE_BUILD_LINUX)
                {
                    if( GPE_LOADER != NULL )
                    {
                        GPE_LOADER->update_messages( "Copying Electron-Linux Files", "Please wait","Do NOT CLOSE" );
                    }

                    if( buildBits==64)
                    {
                        GPE_Main_Logs->log_build_line("Copying Electron-Linux 64bit Files...");
                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [game and SO files] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/linux64build_electron",buildDirectory);

                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [locales] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/linux64build_electron/locales",buildDirectory+"/locales");


                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [resources] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/linux64build_electron/resources",buildDirectory+"/resources", true);
                    }
                    else
                    {
                        GPE_Main_Logs->log_build_line("Copying Electron-Linux 32Bit Files...");
                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [game and SO files] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/linux32build_electron",buildDirectory);

                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [locales] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/linux32build_electron/locales",buildDirectory+"/locales");

                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_submessages( "Copying [resources] folder", "Please Wait..." );
                        }
                        copy_folder(APP_DIRECTORY_NAME+"build_files/linux32build_electron/resources",buildDirectory+"/resources", true);
                    }
                }
                else if(buildType == GPE_BUILD_MAC)
                {
                    GPE_Main_Logs->log_build_comment("Automatic Electron OSX building not supported yet...");
                }
            }
            else
            {
                if( buildType == GPE_BUILD_WINDOWS || buildType == GPE_BUILD_MAC || buildType == GPE_BUILD_LINUX)
                {
                    GPE_Main_Logs->log_build_comment("Electron not copied, previous version if available will be used...");
                }
            }
        }
        else
        {
            GPE_Main_Logs->log_build_error("Unable to create ["+buildDirectory+"] folder. Unable to access...");
        }
        return newBuildDirectory;
    }
    return "";
}

void GPE_Gui_Engine::take_live_screenshor()
{

    if( GPE_MAIN_RENDERER!=NULL)
    {
        if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->autoSaveScreenshots!=NULL && MAIN_EDITOR_SETTINGS->autoSaveScreenshots->is_clicked() )
        {
            if( MAIN_EDITOR_SETTINGS->makeMetaScreenshots!=NULL && MAIN_EDITOR_SETTINGS->makeMetaScreenshots->is_clicked() )
            {
                if( (int)CURRENT_PROJECT_NAME.size() > 0 )
                {
                    std::time_t rawtime;
                    std::tm* timeinfo;
                    char timeBuffer [80];

                    std::time(&rawtime);
                    timeinfo = std::localtime(&rawtime);

                    std::strftime(timeBuffer,80,"%y%m%d_%H%M%S",timeinfo);
                    std::puts(timeBuffer);

                    bool foundBlank = false;
                    std::string screenshotNewLocation  = "";


                    screenshotNewLocation = APP_DIRECTORY_NAME+"screenshots/"+get_file_noext( get_local_from_global_file( CURRENT_PROJECT_NAME) )+"_"+timeBuffer+".png";
                    if( file_exists( screenshotNewLocation) ==false )
                    {
                        foundBlank = true;
                        GPE_MAIN_RENDERER->save_screenshot(screenshotNewLocation);
                    }
                    for(int i = 1; foundBlank == false && i <= 1000; i++)
                    {
                        screenshotNewLocation = APP_DIRECTORY_NAME+"screenshots/"+get_file_noext( get_local_from_global_file( CURRENT_PROJECT_NAME) )+"_"+timeBuffer+"_"+int_to_string(i)+".png";
                        if( file_exists( screenshotNewLocation) ==false )
                        {
                            foundBlank = true;
                            GPE_MAIN_RENDERER->save_screenshot(screenshotNewLocation);
                            break;
                        }
                    }
                    if( foundBlank==false)
                    {
                        appendToFile(APP_DIRECTORY_NAME+"screenshots/screenshot_log.txt","Unable to save screenshot to "+screenshotNewLocation);
                        GPE_Report("Unable to save screenshot to "+screenshotNewLocation);
                    }
                }
                else
                {
                    GPE_MAIN_RENDERER->save_screenshot();
                }
            }
            else
            {
                GPE_MAIN_RENDERER->save_screenshot();
            }
        }
    }
}

void GPE_Gui_Engine::update_recent_project_list(bool saveData)
{
    if( MAIN_TOOLBAR_RECENT_PROJECTS!=NULL)
    {
        MAIN_TOOLBAR_RECENT_PROJECTS->clear_menu();
        int iRPList = 0;
        int maxRPList = GPE_MAIN_GUI->get_recent_project_list_size();
        for( iRPList = 0; iRPList < maxRPList; iRPList++)
        {
            if( iRPList==maxRPList-1)
            {
                MAIN_TOOLBAR_RECENT_PROJECTS->add_menu_option(GPE_MAIN_GUI->get_recent_project_name(iRPList),-1,NULL,-1,NULL, true );
            }
            else
            {
                MAIN_TOOLBAR_RECENT_PROJECTS->add_menu_option(GPE_MAIN_GUI->get_recent_project_name(iRPList),-1,NULL,-1,NULL,false );
            }
        }
        MAIN_TOOLBAR_RECENT_PROJECTS->add_menu_option("Clear Recent Projects List");
    }
}

int modify_folder_images(std::string folderLocation, int modifcationType)
{
    int imagesModifiedCount = -1;
    if( path_exists(folderLocation) && modifcationType >=0 && modifcationType < 3)
    {
        GPE_FileDirectory * foundFolder = new GPE_FileDirectory();
        foundFolder->open_directory_sorted(folderLocation);

        GPE_Color * colorToRemove = c_fuchsia->duplicate_color();
        bool continueWithAction = false;
        imagesModifiedCount = 0;
        if( foundFolder->get_count() > 0 )
        {
            if( modifcationType==0)
            {
                if( GPE_Change_Color_PopUp("Image Background Color To Remove",colorToRemove) )
                {
                    if( GPE_Display_Basic_Prompt("Are you sure you want to erase this Color from all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                    {
                        continueWithAction = true;
                    }
                }
            }
            else if( modifcationType==1 )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you want to invert your all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                {
                    continueWithAction = true;
                }
            }
            else if( modifcationType==2 )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you want to grayscale all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                {
                    continueWithAction = true;
                }
            }
            if( continueWithAction)
            {
                std::string newImageName = "";
                GPE_File * tempFile = NULL;
                SDL_Surface * oTempSurface = NULL;
                SDL_Surface *nTempSurface = NULL;
                for( int i = 0; i < foundFolder->get_count(); i++)
                {
                    tempFile = foundFolder->get_file(i);
                    if( tempFile!=NULL)
                    {
                        if( tempFile->get_type()=="bmp" || tempFile->get_type()=="png")
                        {
                            newImageName = folderLocation+"/"+tempFile->get_name();
                            oTempSurface = SDL_SurfaceEx::load_surface_image( newImageName.c_str() );
                            if( oTempSurface->w > 0 && oTempSurface->h > 0)
                            {
                                nTempSurface = NULL;
                                if( oTempSurface!=NULL)
                                {
                                    GPE_Report("Modifying image at: "+newImageName+".");

                                    if( GPE_LOADER != NULL )
                                    {
                                        GPE_LOADER->update_messages( "Modifying Image", tempFile->get_name(),"Please wait..." );
                                    }

                                    if( modifcationType==0)
                                    {
                                        nTempSurface= SDL_SurfaceEx::surface_remove_color(oTempSurface,colorToRemove->get_sdl_color() );
                                    }
                                    else if( modifcationType==1 )
                                    {
                                        nTempSurface= SDL_SurfaceEx::surface_invert(oTempSurface);
                                    }
                                    else if( modifcationType==2 )
                                    {
                                        nTempSurface= SDL_SurfaceEx::surface_grayscale(oTempSurface);
                                    }
                                    if( nTempSurface!=NULL)
                                    {
                                        if( tempFile->get_type()=="bmp" || tempFile->get_type()=="png")
                                        {
                                            SDL_SaveBMP(nTempSurface,newImageName.c_str() );
                                        }
                                        else if( tempFile->get_type()=="png")
                                        {
                                            IMG_SavePNG(nTempSurface,newImageName.c_str() );
                                        }
                                        SDL_FreeSurface(nTempSurface);
                                        nTempSurface = NULL;
                                        imagesModifiedCount++;
                                    }
                                    SDL_FreeSurface(oTempSurface);
                                    oTempSurface = NULL;
                                }
                            }
                        }
                    }
                }
            }
        }
        foundFolder->close_directory();
        delete foundFolder;
        if( colorToRemove!=NULL)
        {
            delete colorToRemove;
            colorToRemove = NULL;
        }
    }
    return imagesModifiedCount;
}




void GPE_Editor_Init_Globals()
{
    //Let's start off with the build names
    for(int bI = 0; bI < GPE_BUILD_OPTIONS; bI++)
    {
        GPE_BUILD_NAMES[bI] = "";
    }
    GPE_BUILD_NAMES[GPE_BUILD_HTML5] = "HTML5";
    GPE_BUILD_NAMES[GPE_BUILD_WINDOWS] = "WINDOWS";
    GPE_BUILD_NAMES[GPE_BUILD_MAC] = "OSX";
    GPE_BUILD_NAMES[GPE_BUILD_LINUX] = "LINUX";
    GPE_BUILD_NAMES[GPE_BUILD_XBOXONE] = "XBOXONE";
    GPE_BUILD_NAMES[GPE_BUILD_PS4] = "PS4";
    GPE_BUILD_NAMES[GPE_BUILD_ANDROID] = "ANDROID";
    GPE_BUILD_NAMES[GPE_BUILD_IOS] = "IOS";

    //animation Labels we will use
    GPE_ANIMATION_LABELS[sprFrameCount] = "Frame Count";
    GPE_ANIMATION_LABELS[sprFrameWidth] = "Frame Width";
    GPE_ANIMATION_LABELS[sprFrameHeight] = "Frame Height";
    GPE_ANIMATION_LABELS[sprHPixelOffset] = "Horizontal Pixel  Offset";
    GPE_ANIMATION_LABELS[sprVPixelOffset] = "Vertical Pixel Offset";
    GPE_ANIMATION_LABELS[sprHPixelPadding] = "Horizontal Frame Padding";
    GPE_ANIMATION_LABELS[sprVPixelPadding] = "Vertictal Frame Padding";

    //Tilesheet labels we will use
    tsDataLabels[0] = "Width";
    tsDataLabels[1] = "Height";
    tsDataLabels[2] = "X-Offset";
    tsDataLabels[3] = "Y-Offset";
    tsDataLabels[4] = "H-Padding";
    tsDataLabels[5] = "Y-Padding";

    //Font Popup variables
    POPUP_FONT_SIZE_WIDTH = 12;
    POPUP_FONT_SIZE_HEIGHT = 12;
    if( FONT_POPUP!=NULL)
    {
        FONT_POPUP->get_metrics("A",&POPUP_FONT_SIZE_WIDTH,&POPUP_FONT_SIZE_HEIGHT);
    }

    //Scene globals
    DEFAULT_SCENE_SUBEDITOR_NAMES[0] = "Settings";
    DEFAULT_SCENE_SUBEDITOR_NAMES[1] = "Layers";
    DEFAULT_SCENE_SUBEDITOR_NAMES[2] = "Extras";
    DEFAULT_SCENE_SUBEDITOR_NAMES[3] = "Extras";
    DEFAULT_SCENE_SUBEDITOR_NAMES[3] = "Extras";

    //Audio type names
    SUPPORTED_AUDIO_EXT[aacFileName] = "AAC";
    SUPPORTED_AUDIO_EXT[mp3FileName] = "MP3";
    SUPPORTED_AUDIO_EXT[oggFileName] = "OGG";
    SUPPORTED_AUDIO_EXT[wavFileName] = "WAV";

    //Video type names
    SUPPORTED_VIDEO_EXT[0] = "MP4";
    SUPPORTED_VIDEO_EXT[1] = "WEBM";
    SUPPORTED_VIDEO_EXT[2] = "OGG";

    //Font type names
    SUPPORTED_FONT_EXT[FONT_EOT]  = "EOT";
    SUPPORTED_FONT_EXT[FONT_SVG]  = "SVG";
    SUPPORTED_FONT_EXT[FONT_OTF] = "OTF";
    SUPPORTED_FONT_EXT[FONT_TTF]  = "TTF";
    SUPPORTED_FONT_EXT[FONT_WOFF] = "WOFF";
    SUPPORTED_FONT_EXT[FONT_WOFF2] = "WOFF2";
}

void GPE_Show_Tip_Of_Day()
{
    if( GPE_MAIN_GUI!=NULL && GPE_MAIN_RENDERER!=NULL )
    {
        gpe->end_loop();
        RESOURCE_TO_DRAG = NULL;
        GPE_Report("Showing tip of the day");
        std::string popUpCaption = "Tip of the Day";
        gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );
        GPE_MAIN_GUI->reset_gui_info();
        MAIN_OVERLAY->take_frozen_screenshot( );

        int promptBoxWidth = 544;
        int promptBoxHeight = 384;
        GPE_Rect elementBox;

        bool exitOperation = false;
        input->reset_all_input();
        int currentTipId = GPE_MAIN_GUI->get_random_tip();
        if( currentTipId < 0 || currentTipId > GPE_MAIN_GUI->get_tip_count() )
        {
            currentTipId = 0;
        }
        std::string currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);

        GPE_Label_Title * doYouKnowLabel = new GPE_Label_Title("Do you know?","Do you know?");
        GPE_GuiElementList * showTipList = new GPE_GuiElementList();
        showTipList->hideXScroll = true;
        showTipList->hideYScroll = true;
        GPE_ToolLabelButton * closeButton = new GPE_ToolLabelButton( "Close","");
        GPE_ToolLabelButton * nextTipButton = new GPE_ToolLabelButton( "Next Tip","");
        GPE_ToolLabelButton * previousTipButton = new GPE_ToolLabelButton( "Previous Tip","");
        GPE_CheckBoxBasic * showAtStartUpButton = new GPE_CheckBoxBasic("Show tips at startup","Unclick to not see this popup automatically on start");
        showAtStartUpButton->set_clicked( GPE_MAIN_GUI->showTipsAtStartUp);
        GPE_ToolLabelButton * randomTipButton = new GPE_ToolLabelButton( "Random Tip","");
        GPE_WrappedTextArea * tipParagraph = new GPE_WrappedTextArea();
        tipParagraph->set_string(currentTipString);
        tipParagraph->set_width(512-GENERAL_GPE_GUI_PADDING*2);
        tipParagraph->set_height(96);
        GPE_MAIN_RENDERER->reset_viewpoint();
        //MAIN_OVERLAY->render_frozen_screenshot( );
        while(exitOperation==false)
        {
            gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );
            //GPE_Report("Processing tip of the day");
            gpe->start_loop();

            elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2;
            elementBox.y = (SCREEN_HEIGHT-promptBoxHeight)/2;
            elementBox.w = promptBoxWidth;
            elementBox.h = promptBoxHeight;
            showTipList->set_coords(elementBox.x, elementBox.y+32);
            showTipList->set_width(elementBox.w);
            showTipList->set_height(elementBox.h-32);
            showTipList->barXMargin = GENERAL_GPE_GUI_PADDING;
            showTipList->barYMargin = GENERAL_GPE_GUI_PADDING;
            showTipList->barXPadding = GENERAL_GPE_GUI_PADDING;
            showTipList->barYPadding = GENERAL_GPE_GUI_PADDING;

            GPE_MAIN_GUI->reset_gui_info();
            showTipList->clear_list();
            showTipList->add_gui_element(doYouKnowLabel,true);
            showTipList->add_gui_element(tipParagraph,true);
            showTipList->add_gui_element(showAtStartUpButton, true);
            showTipList->add_gui_element(previousTipButton,false);
            showTipList->add_gui_element(nextTipButton,true);
            showTipList->add_gui_element(randomTipButton,false);
            showTipList->add_gui_element(closeButton,false);
            showTipList->process_self( NULL, NULL );

            if( input->check_keyboard_released(kb_esc) || closeButton->is_clicked() || GPE_MAIN_WINDOW->is_resized()  )
            {
                exitOperation = true;
            }
            else if( nextTipButton->is_clicked() )
            {
                currentTipId = GPE_MAIN_GUI->get_next_tip(currentTipId);
                currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }
            else if( previousTipButton->is_clicked() )
            {
                currentTipId = GPE_MAIN_GUI->get_previous_tip(currentTipId);
                currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }
            else if( randomTipButton->is_clicked() )
            {
                currentTipId = GPE_MAIN_GUI->get_random_tip();
                currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }

            //GPE_Report("Rendering tip of the day");
            GPE_MAIN_RENDERER->reset_viewpoint();
            if( !GPE_MAIN_WINDOW->is_resized() )
            {
                //if( input->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot( );
                }
                //Update screen
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Main_Box_Color,false);

                gcanvas->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Color,false);
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,true);
                gfs->render_text( elementBox.x+elementBox.w/2,elementBox.y+GENERAL_GPE_GUI_PADDING,popUpCaption,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
                showTipList->render_self( NULL, NULL );
                //GPE_MAIN_GUI-render_gui_info(  true);

                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
                if( GPE_MAIN_GUI!= NULL )
                {
                    GPE_MAIN_GUI->render_gui_info( );
                }
            }
            gpe->end_loop();
        }

        if( showTipList!=NULL)
        {
            delete showTipList;
            showTipList = NULL;
        }

        if( doYouKnowLabel!=NULL)
        {
            delete doYouKnowLabel;
            doYouKnowLabel = NULL;
        }
        if( closeButton!=NULL)
        {
            delete closeButton;
            closeButton = NULL;
        }
        if( nextTipButton!=NULL)
        {
            delete nextTipButton;
            nextTipButton = NULL;
        }
        if( previousTipButton!=NULL)
        {
            delete previousTipButton;
            previousTipButton = NULL;
        }
        if( randomTipButton!=NULL)
        {
            delete randomTipButton;
            randomTipButton = NULL;
        }
        if( showAtStartUpButton!=NULL)
        {
            GPE_MAIN_GUI->showTipsAtStartUp = showAtStartUpButton->is_clicked();
            GPE_MAIN_GUI->save_settings();
            delete showAtStartUpButton;
            showAtStartUpButton = NULL;
        }
        if( tipParagraph!=NULL)
        {
            delete tipParagraph;
            tipParagraph = NULL;
        }
        input->reset_all_input();
    }
}
