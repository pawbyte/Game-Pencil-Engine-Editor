/*
gpe_editor.h
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

#ifndef GPE_EDITOR_H
#define GPE_EDITOR_H

#include "AOSGUI/paw_gui.h"

const int GPE_MAINICON_SPRCOUNT = 74;

const int SPRITE_DATA_FIELD_COUNT = 7;
extern std::string spriteDataLabels[SPRITE_DATA_FIELD_COUNT];
extern int spriteDataLabelWidth;

const int TILESHEET_DATA_FIELD_COUNT = 6;
extern std::string tsDataLabels[TILESHEET_DATA_FIELD_COUNT];
extern int tsDataLabelWidth;

typedef void (*CallbackType)(void);


void GPE_Show_Tip_Of_Day();

typedef enum
{
    gpv_int,
    gpv_float,
    gpv_double,
    gpv_char,
    gpv_string,
    gpv_array,
    gpv_vector,
    gpv_gameobject
}gpvTypes;

class gpv
{
    private:
        int actualDataType;
        int heldInt;
        float heldFloat;
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
        gpv & operator=(double other);
        gpv & operator=(float other);
        gpv & operator=(char other);
};

extern generalGameResource * SELECTED_GENERAL_RESOUCE;
void GPE_set_selected_gresource(generalGameResource * newResource = NULL);
generalGameResource * GPE_get_selected_gresource();

const int res_type_count = 20;
extern std::string RESOURCE_TYPE_NAMES[res_type_count];
extern std::string GPE_BUILD_NAMES[GPE_BUILD_OPTIONS];


const int PROJECT_LANGUAGE_JS = 0;
const int PROJECT_LANGUAGE_CS = 1;
const int PROJECT_LANGUAGE_CPP = 2;
const int PROJECT_LANGUAGE_TST = 3;
const int PROJECT_LANGUAGE_LUA = 4;
const int PROJECT_LANGUAGE_MAX = 5;

extern std::string PROJECT_LANGUAGE_NAMES[PROJECT_LANGUAGE_MAX];

class GPE_ProjectFolder
{
    private:
        std::string projectName;
        std::string projectStartDirectoryName;
        std::string projectDirectory;
        std::string projectFileName;
        std::string projectLanguage;
        int GLOBAL_REZ_ID_COUNT;
        double projectFilePreviousVersion;
        double projectFileVersion;
    public:
        std::string projectIconName;
        int myProjectLanguage;
        GPE_ResourceContainer * RESC_PROJECT_FOLDER;
        GPE_ResourceContainer * RESC_SPRITES;
        GPE_ResourceContainer * RESC_TEXTURES;
        GPE_ResourceContainer * RESC_TILESHEETS;
        GPE_ResourceContainer * RESC_3DMODELS;
        GPE_ResourceContainer * RESC_AUDIO;
        GPE_ResourceContainer * RESC_VIDEOS;
        GPE_ResourceContainer * RESC_FUNCTIONS;
        GPE_ResourceContainer * RESC_OBJECTS;
        GPE_ResourceContainer * RESC_CLASSES;
        GPE_ResourceContainer * RESC_RESOURCES;
        GPE_ResourceContainer * RESC_PATHS;
        GPE_ResourceContainer * RESC_DICTIONARIES;
        GPE_ResourceContainer * RESC_SCENES;
        GPE_ResourceContainer * RESC_ACHIEVEMENTS;
        GPE_ResourceContainer * RESC_FONTS;
        GPE_ResourceContainer * RESC_SHADERS;
        GPE_ResourceContainer * RESC_PROJECT_SETTINGS;
        GPE_ResourceContainer * RESC_ALL[res_type_count];
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
        GPE_ResourceContainer * create_blank_folder(GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_resource(int rNewType = -1,GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_audio( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_video( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_path( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_font( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_function( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_class( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_gameobject( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_scene( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_sprite( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_texture( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        GPE_ResourceContainer * create_blank_tilesheet( GPE_ResourceContainer * folderContainer = NULL, std::string newName="", int newResId = -1);
        bool export_and_play_native(bool launchProgram = true);
        bool export_project_html5(std::string projectBuildDirectory = "", int buildMetaTemplate = -1, bool runGameOnCompile = false, bool inDebugMode = false);
        bool export_project_wiiu( bool inDebugMode = false);
        bool export_project_windows(std::string projectBuildDirectory = "",int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
        bool export_project_osx(std::string projectBuildDirectory = "",int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
        bool export_project_linux(std::string projectBuildDirectory = "",int buildBits = 32, bool runGameOnCompile = false, bool inDebugMode = false, int nativeBuildType = true);
        std::string get_project_language();
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

};

extern GPE_ProjectFolder * CURRENT_PROJECT;

bool set_current_gpe_project( GPE_ProjectFolder * newMainProject = NULL);
bool set_current_gpe_project_from_name( std::string newMainProjectName );
extern int lastResTypeRendered;

class GPE_DropDown_Resouce_Menu: public GPE_GeneralGuiElement
{
    private:
        GPE_ResourceContainer * selectedResource;
        GPE_ResourceContainer * containerTree;
        int maxHeightToRender;
        GPE_PopUpMenu_Option * subOptions;
        std::string dropdownName;
        std::string displayedResult;
        GPE_Sprite * selectedSprite;
        GPE_Texture *selectedImage;
        int opId;
        bool isSelectable;
        int selectedId;
        bool isOpen;
        bool justActivated;
    public:
        GPE_DropDown_Resouce_Menu(int xPos, int yPos, std::string name, GPE_ResourceContainer * cTree = NULL,int id = -1,bool selectable = true);
        ~GPE_DropDown_Resouce_Menu();
        std::string get_data();
        void load_data(std::string dataString);
        std::string get_menu_option(int layerNumb, int atNumb);
        GPE_ResourceContainer * get_container_at(int layerNumb, int atNumb);
        GPE_ResourceContainer * get_selected_container();
        std::string get_plain_string();
        int get_selected_id();
        std::string get_selected_name();
        int get_menu_size();
        bool just_activated();
        //std::string get_name();
        void process_self(GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL);
        void add_folder_contents(GPE_ResourceContainer * fFolder = NULL, GPE_PopUpMenu_Option * fOptionFolder = NULL);
        void render_self(GPE_Renderer * cRender=NULL,GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL, bool forceRedraw = true);
        void set_name(std::string newName);
        void set_selected_target(std::string newName);
        void set_selection(int newId);
};

class GPE_ResourceManagementBar: public GPE_GeneralGuiElement
{
    private:
        std::vector <GPE_ResourceContainer *> subOptions;
        int seekedX2Pos;

        bool isVisible;
        int lastWidth;
        int barTitleWidth;
        int barTitleHeight;
    public:
        bool justResized;
        bool showYScroll;
        bool beingResized;
        bool menuResized;
        bool resourcebarMoved;
        GPE_Rect entireBox, cameraBox, menuBox;
        GPE_ScrollBar_XAxis * xScroll;
        GPE_ScrollBar_YAxis * yScroll;
        int barXPadding, barYPadding;
        bool subMenuIsOpen;
        int selectedSubOption;
        GPE_Texture * opTexture;
        GPE_ResourceManagementBar();
        ~GPE_ResourceManagementBar();
        GPE_ResourceContainer * add_resource_folder(int resourceType, std::string projFolderName,std::string resourceTypeName);
        GPE_ResourceContainer * add_project_folder(int resourceType, std::string projFolderName,std::string resourceTypeName);
        void delete_project_resources(std::string projectFileName );
        bool is_visible();
        void prerender_self(GPE_Renderer * cRender);
        void process_managementbar( );
        void render_resourcebar(GPE_Renderer * cRender, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void remove_project_resources(std::string projectFileName );
        void set_height(int newHeight);
        void toggle_self();
};

extern GPE_TabManager * GPE_Main_TabManager;


extern GPE_PopUpMenu_Option * MAIN_TOOLBAR_RECENT_PROJECTS;

const int RELEASE_TYPE_RC = 0;
const int RELEASE_TYPE_BETA = 1;
const int RELEASE_TYPE_RELEASE = 2;

class GPE_Gui_Engine
{
    private:
        std::vector<std::string> gpeRecentProjects;
        std::vector<std::string> gpeTips;
    public:
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
        GPE_ResourceManagementBar * mainResourceBar;
        GPE_Gui_Engine();
        ~GPE_Gui_Engine();
        void add_to_recent_project_list(std::string newProjectFileName, bool saveData = false);
        void apply_logic();
        void clean_current_project_build_folder(int buildMetaTemplate);
        void clear_recent_project_list();
        void close_project(std::string projectFileName );
        GPE_ProjectFolder * find_project_from_name(std::string projectName);
        GPE_ProjectFolder * find_project_from_filename(std::string projectFileName);
        int find_project_id_from_name(std::string projectName, int ignoreId = -1);
        int find_project_id_from_filename(std::string projectFileName, int ignoreId = -1);
        int get_recent_project_list_size();
        int get_tip_count();
        int get_random_tip();
        int get_previous_tip(int tipId);
        int get_next_tip(int tipId);
        std::string get_recent_project_name(int pId);
        std::string get_tip(int tipId);

        GPE_ResourceManagementBar  * init_resourcebar();
        GPE_Toolbar * init_toolbar();
        GPE_Toolbar * init_toolbar(std::string bName, GPE_Rect bRect);
        void launch_new_project();
        void load_tips();
        void load_settings();
        GPE_Toolbar * get_main_toolbar();
        GPE_ToolIconButtonBar * get_main_buttonbar();
        void open_project(std::string projName);
        void open_new_project();
        void prerender_gui(GPE_Renderer * cRender);
        void process_overlay_message();
        void process_window_title();

        void render_foreground_engine(GPE_Renderer * renderTarget = NULL, bool forceRedraw = true);
        void render_gui_info(GPE_Renderer * renderTarget = NULL, bool forceRedraw = true);
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
        void take_live_screenshor(GPE_Renderer * renderTarget = NULL);
        void update_recent_project_list(bool saveData);
};

extern GPE_Gui_Engine * GPE_MAIN_GUI;


int modify_folder_images(std::string folderLocation, int modifcationType);
int clean_folder(std::string folderName);
bool copy_folder(std::string folderName, std::string targetFolder, bool copySubFolders = false);
std::string get_user_settings_folder();
std::string get_user_temp_folder();
std::string get_user_screenshot_folder();

#endif
