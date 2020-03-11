/*
gpe_editor_project.h
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

#ifndef gpe_editor_project_h
#define gpe_editor_project_h

#include "../pawguipaw_gui.h"
#include "../pawguipaw_gui_resource_dropdown.h"
#include "../pawguipaw_gui_general_resource.h"

#include "gpe_editor_constants.h"
#include "gpe_editor_globals.h"
#include "gpe_log_manager.h"
#include "gpe_editor_project_helper.h"
#include "gpe_resource_tree.h"

class GPE_ProjectFolder
{
private:
    std::string projectName;
    std::string projectStartDirectoryName;
    std::string projectDirectory;
    std::string projectFileName;
    std::string projectLanguage;
    int myProjectLanguage;
    int GLOBAL_REZ_ID_COUNT;
    float projectFilePreviousVersion;
    float projectFileVersion;
public:
    gpe::asset_manager * projectRSM;
    std::string projectIconName;
    GPE_GeneralResourceContainer * RESC_project_FOLDER;
    GPE_GeneralResourceContainer * RESC_ANIMATIONS;
    GPE_GeneralResourceContainer * RESC_TEXTURES;
    GPE_GeneralResourceContainer * RESC_TILESHEETS;
    GPE_GeneralResourceContainer * RESC_3DMODELS;
    GPE_GeneralResourceContainer * RESC_AUDIO;
    GPE_GeneralResourceContainer * RESC_VIDEOS;
    GPE_GeneralResourceContainer * RESC_EMITTERS;
    GPE_GeneralResourceContainer * RESC_LIGHTS;
    GPE_GeneralResourceContainer * RESC_FUNCTIONS;
    GPE_GeneralResourceContainer * RESC_OBJECTS;
    GPE_GeneralResourceContainer * RESC_CLASSES;
    GPE_GeneralResourceContainer * RESC_RESOURCES;
    GPE_GeneralResourceContainer * RESC_PATHS;
    GPE_GeneralResourceContainer * RESC_DICTIONARIES;
    GPE_GeneralResourceContainer * RESC_SCENES;
    GPE_GeneralResourceContainer * RESC_ACHIEVEMENTS;
    GPE_GeneralResourceContainer * RESC_FONTS;
    GPE_GeneralResourceContainer * RESC_SHADERS;
    GPE_GeneralResourceContainer * RESC_project_SETTINGS;
    GPE_GeneralResourceContainer * RESC_ALL[ gpe::resource_type_max ];
    int CREATED_RESOURCE_COUNT[ gpe::resource_type_max ];

    //Used for code editor and hightlights of functions, resources and objects.
    std::vector <GPE_Compiler_Term *> projectFunctions;
    std::vector <GPE_Compiler_Term *> projectKeywords;

    //used for buld scripts
    std::vector <int >currentObjParents;
    std::vector <int> nextObjParents;
    std::vector <int> finalObjParents;

    time_t lastTimeBackedUp;
    std::string projectLayerNames[32];

    GPE_ProjectFolder(std::string name, std::string directoryIn, std::string new_file_name, std::string projectLanguageIn = "JS", bool createBlankScene = false);
    ~GPE_ProjectFolder();

    //Used for syntax highlights and such
    bool add_project_function(std::string nName, std::string nDescription = "", std::string nParameters = "", std::string tFunctionReturnType = "", std::string nScope="");
    bool add_project_keyword(std::string nName, std::string nDescription = "", int nType = -1, std::string nScope="");

    //Name checker
    bool check_names_against_keywords( GPE_GeneralResourceContainer * resContainer );

    void clear_project_functions();
    void clear_project_keywords();

    //Creating Resources and such
    bool clean_build_folder( int buildMetaTemplate = -1 );

    //creates sub folders all sexy like
    GPE_GeneralResourceContainer * create_blank_folder(GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_resource(int rNewType = -1,GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_audio( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_video( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_emitter( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_light( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_path( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_font( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_function( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_class( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_gameobject( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_scene( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_animation( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_texture( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_tilesheet( GPE_GeneralResourceContainer * folderContainer = NULL, std::string new_name="", int newResId = -1);

    //Export based functions
    bool export_and_play_native(bool launchProgram = true);
    bool export_project_cpp(std::string projectBuildDirectory = "", std::string gpeBuilderName = "", int buildMetaTemplate = -1, bool runGameOnCompile = false, bool inDebugMode = false);
    bool export_project_html5(std::string projectBuildDirectory = "", std::string gpeBuilderName = "", int buildMetaTemplate = -1, bool runGameOnCompile = false, bool inDebugMode = false);
    bool export_project_windows(std::string projectBuildDirectory = "",std::string gpeBuilderName = "", int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
    bool export_project_osx(std::string projectBuildDirectory = "", std::string gpeBuilderName = "", int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
    bool export_project_linux(std::string projectBuildDirectory = "", std::string gpeBuilderName = "", int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);

    //Getters
    std::string get_project_language();
    int get_project_language_id();
    std::string get_project_name();
    std::string get_project_directory();
    std::string get_project_file_name();
    int get_resource_count();

    //Load Project
    bool load_project_file(std::string projectFileIn);

    //For build scripts and such
    bool check_obj_inlist(int objTypeIdIn);
    void swap_obj_lists();
    void begin_obj_reverse_inheritence();

    //Includes for project resources
    bool include_local_files( GPE_GeneralResourceContainer * resContainer,std::string pBuildDir , int buildType );

    //used to give each resource a unique global id, like a boss
    int increment_resouce_count();
    void integrate_syntax();
    bool process_build_script(std::string scriptFName, std::string templateFName, std::string outputDir );
    void refresh_gui_syntax();
    bool run_project(std::string projectBuildDirectory = "", int buildMetaTemplate = -1,int buildBits = 32, bool inDebugMode = false);
    bool save_project();
    bool save_project_settings();
    bool save_project_as(std::string projectFilenew_name);
    void set_project_language( std::string newProjectLanguage);
    void set_project_language_id( int projectLanguageId );
    void start_state();
};

extern GPE_ProjectFolder * current_project;

#endif //gpe_editor_project_h
