/*
gpe_editor.h
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

#ifndef GPE_EDITOR_H
#define GPE_EDITOR_H

#include "GPE_Engine/GPE_State_Manager.h"
#include "AOSGUI/paw_gui.h"
#include "AOSGUI/paw_gui_resource_dropdown.h"
#include "gpe_log_manager.h"
#include "gpe_resource_tree.h"

typedef void (*CallbackType)(void);

typedef enum
{
    gpv_int,
    gpv_double,
    gpv_float,
    gpv_char,
    gpv_string,
    gpv_array,
    gpv_vector,
    gpv_gameobject
} gpvTypes;

class gpv
{
private:
    int actualDataType;
    int heldInt;
    double helddouble;
    double heldDouble;
    char heldChar;
    std::string heldString;
    std::vector < gpv * > heldArray;
    std::vector < gpv * > heldVector;
    //GPE_GameObject * heldObject;
public:
    gpv();
    ~gpv();
    gpv & operator=(const gpv &other);
    gpv & operator=(int other);
    gpv & operator=(std::string other);
    gpv & operator=(float other);
    gpv & operator=(double other);
    gpv & operator=(char other);
};



extern std::string GPE_BUILD_NAMES[GPE_BUILD_OPTIONS];

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
    double projectFilePreviousVersion;
    double projectFileVersion;
public:
    GPE_DataManager * projectRSM;
    std::string projectIconName;
    GPE_GeneralResourceContainer * RESC_PROJECT_FOLDER;
    GPE_GeneralResourceContainer * RESC_ANIMATIONS;
    GPE_GeneralResourceContainer * RESC_SPRITES;
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
    GPE_GeneralResourceContainer * RESC_PROJECT_SETTINGS;
    GPE_GeneralResourceContainer * RESC_ALL[res_type_count];
    int CREATED_RESOURCE_COUNT[res_type_count];

    //Used for code editor and hightlights of functions, resources and objects.
    std::vector <GPE_Compiler_Term *> projectFunctions;
    std::vector <GPE_Compiler_Term *> projectKeywords;

    //used for buld scripts
    std::vector <int >currentObjParents;
    std::vector <int> nextObjParents;
    std::vector <int> finalObjParents;

    time_t lastTimeBackedUp;
    std::string projectLayerNames[32];

    GPE_ProjectFolder(std::string name, std::string directoryIn, std::string fileNameIn, std::string projectLanguageIn = "JS", bool createBlankScene = false);
    ~GPE_ProjectFolder();

    //Used for syntax highlights and such
    bool add_project_function(std::string nName, std::string nDescription = "", std::string nParameters = "", std::string tFunctionReturnType = "", std::string nScope="");
    bool add_project_keyword(std::string nName, std::string nDescription = "", int nType = -1, std::string nScope="");
    void clear_project_functions();
    void clear_project_keywords();

    //Creating Resources and such
    bool clean_build_folder( int buildMetaTemplate = -1 );
    //creates sub folders all sexy like
    GPE_GeneralResourceContainer * create_blank_folder(GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_resource(int rNewType = -1,GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_audio( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_video( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_emitter( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_light( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_path( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_font( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_function( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_class( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_gameobject( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_scene( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_sprite( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_texture( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    GPE_GeneralResourceContainer * create_blank_tilesheet( GPE_GeneralResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
    bool export_and_play_native(bool launchProgram = true);
    bool export_project_cpp(std::string projectBuildDirectory = "", int buildMetaTemplate = -1, bool runGameOnCompile = false, bool inDebugMode = false);
    bool export_project_html5(std::string projectBuildDirectory = "", int buildMetaTemplate = -1, bool runGameOnCompile = false, bool inDebugMode = false);
    bool export_project_wiiu( bool inDebugMode = false);
    bool export_project_windows(std::string projectBuildDirectory = "",int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
    bool export_project_osx(std::string projectBuildDirectory = "",int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
    bool export_project_linux(std::string projectBuildDirectory = "",int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
    std::string get_project_language();
    int get_project_language_id();
    std::string get_project_name();
    std::string get_project_directory();
    std::string get_project_file_name();
    int get_resource_count();
    bool load_project_file(std::string projectFileIn);

    //For build scripts and such
    bool check_obj_inlist(int objTypeIdIn);
    void swap_obj_lists();
    void begin_obj_reverse_inheritence();
    //used to give each resource a unique global id, like a boss
    int increment_resouce_count();
    void integrate_syntax();
    void refresh_gui_syntax();
    bool run_project(std::string projectBuildDirectory = "", int buildMetaTemplate = -1,int buildBits = 32, bool inDebugMode = false);
    bool save_project();
    bool save_project_settings();
    bool save_project_as(std::string projectFileNewName);
    void set_project_language( std::string newProjectLanguage);
    void set_project_language_id( int projectLanguageId );
};

extern GPE_ProjectFolder * CURRENT_PROJECT;

bool set_current_gpe_project( GPE_ProjectFolder * newMainProject = NULL);
bool set_current_gpe_project_from_name( std::string newMainProjectName );

extern GPE_PopUpMenu_Option * MAIN_TOOLBAR_RECENT_PROJECTS;

const int RELEASE_TYPE_ALPHA = 0;
const int RELEASE_TYPE_BETA = 1;
const int RELEASE_TYPE_RC = 2;
const int RELEASE_TYPE_PRODUCTION = 3;

class GPE_Gui_Engine
{
private:
    std::vector<std::string> gpeRecentProjects;
    std::vector<std::string> gpeTips;
public:
    std::vector< GPE_Theme *> gpeThemes;
    bool includeNintendoWiiUExport;
    bool includeNintendoSwitchExport;
    bool includePlaystation4Export;
    bool includeXboxOneExport;
    int editorReleaseType;
    bool showStartPageAtStartUp;
    bool showTipsAtStartUp;
    std::string fileOpenProjectDir;
    std::string fileOpenProjectIconDir;
    std::string fileSaveProjectDir;

    //For Find/Replace Box of TextArea
    std::string searchResultProjectName;
    std::string searchResultResourceName;
    int searchResultResourceId;

    std::vector<GPE_ProjectFolder * > gpeProjects;
    GPE_Toolbar * mainToolBar;
    GPE_ResourceTree * mainResourceTree;
    GPE_Animation * eraserAnimation;
    GPE_Gui_Engine();
    ~GPE_Gui_Engine();
    GPE_Theme * add_theme(std::string themeName, bool customTheme = false );
    void add_to_recent_project_list(std::string newProjectFileName, bool saveData = false);
    void apply_logic();
    void clean_current_project_build_folder(int buildMetaTemplate);
    void clear_recent_project_list();
    void close_project(std::string projectFileName );
    GPE_ProjectFolder * find_project_from_name(std::string projectName);
    GPE_ProjectFolder * find_project_from_filename(std::string projectFileName);
    int find_project_id_from_name(std::string projectName, int ignoreId = -1);
    int find_project_id_from_filename(std::string projectFileName, int ignoreId = -1);
    GPE_Theme * find_theme(std::string themeName );
    int get_recent_project_list_size();
    int get_tip_count();
    int get_random_tip();
    int get_previous_tip(int tipId);
    int get_next_tip(int tipId);
    std::string get_recent_project_name(int pId);
    std::string get_tip(int tipId);

    GPE_ResourceTree  * init_resourcebar();
    GPE_Toolbar * init_toolbar();
    GPE_Toolbar * init_toolbar(std::string bName, GPE_Rect bRect);
    void launch_new_project();
    void load_tips();
    void load_settings();
    GPE_Toolbar * get_main_toolbar();
    GPE_ToolIconButtonBar * get_main_buttonbar();
    void open_project(std::string projName);
    void open_new_project();
    void prerender_gui( );
    void process_overlay_message();
    void process_window_title();

    void render_foreground_engine( bool forceRedraw = true);
    void render_gui_info( bool forceRedraw = true);
    void remove_project(std::string projectFileName );
    void reset_gui_info();
    void reset_settings();
    void export_current_project_html5(bool runGameOnCompile = false);
    void export_current_project_wiiu();
    void rum_current_project(std::string projectBuildDirectory, int buildMetaTemplate);

    void save_all_projects();
    void save_current_project();
    void save_settings();
    void set_main_toolbar(GPE_Toolbar * newToolbar);
    void setup_project_directory(std::string newProjectDir);
    std::string setup_build_folder(std::string buildDirectory, int buildType = GPE_BUILD_WINDOWS, int buildBits = 32, bool inDebugMode = false, int nativeBuildType = Native_None );
    void take_live_screenshor();
    void update_recent_project_list(bool saveData);
};

extern GPE_Gui_Engine * GPE_MAIN_GUI;


int modify_folder_images(std::string folderLocation, int modifcationType);

void GPE_Show_Tip_Of_Day();
bool GPE_Editor_Init( int argc, char* args[]);
void GPE_Editor_Init_Globals();
bool GPE_Editor_Quit();
#endif
