/*
ambitious_gui_library.h
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

#ifndef AMBITIUOS_GUI_LIBRARY_H
#define AMBITIUOS_GUI_LIBRARY_H

#include "paw_gui_base.h"
#include "paw_gui_scrollbars.h"
#include "paw_gui_sliders.h"
#include "paw_gui_panel_list.h"
#include "paw_gui_popups.h"
#include "paw_gui_tab_basic.h"

//Once I remove the other files that require parsers remove the include for parsers
#include "paw_gui_parsers.h"


extern GPE_Texture * GPE_LOGO;
extern GPE_Texture * GPE_TEXTURE_TRANSPARENT_BG;


class GPE_StatusBar: public GPE_GeneralGuiElement
{
private:
    std::string codeEditorStatusBarString;
    std::string insertModeString;
public:
    GPE_StatusBar();
    ~GPE_StatusBar();
    void process_self(GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL);
    void render_self( GPE_Rect * viewedSpace=NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void reset_statusbar();
    void set_codestring(std::string newCodeString);
    void set_insertmode(std::string newInsertMode);
};

extern GPE_StatusBar * GPE_Main_Statusbar;

class GPE_TextURL: public GPE_GeneralGuiElement
{
public:
    bool wasClicked;
    std::string webUrl;
    std::string webId;
    GPE_TextURL(std::string nameIn="", std::string description="", std::string urlIn="" );
    ~GPE_TextURL();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void set_name(std::string nameIn);
};



const int restype_regular = -1;
const int restype_superfolder = 0;
const int restype_projfolder = 1;

class GPE_GeneralResourceContainer: public GPE_GeneralGuiElement
{
protected:
    int globalResouceId;
    int resourceId;
    int resourceType;
    bool isFolder, isSuperFolder, isSuperProjectFolder;
    bool subMenuIsOpen;
    GPE_Animation * containerSprite;
    GPE_Texture * containerTexture;
    int spriteFrameNumber;
    generalGameResource * heldResource;
    std::vector <GPE_GeneralResourceContainer *> subOptions;
    bool resouceNameChanged;
    bool subContentsModified;
    int strTexWidth, strTexHeight;
public:
    int foundX2Pos;
    int exportBuildGlobalId;
    std::string parentProjectDirectory;
    std::string projectParentFileName;
    std::string projectParentName;
    GPE_Rect optionBox;
    GPE_GeneralResourceContainer * parentResource;
    //GPE_GeneralResourceContainer(std::string projFolderName, int  rezPropValue = -1);
    GPE_GeneralResourceContainer(std::string projFolderName = "", std::string newName =  "", int rType = -1, int rId = -1,bool folder = true, int globalIdVal = -1,int  rezPropValue = -1);
    ~GPE_GeneralResourceContainer();
    void add_resource_container( GPE_GeneralResourceContainer * newResource, bool changeGlobalId = false, GPE_GeneralResourceContainer * referenceObject = NULL );
    GPE_GeneralResourceContainer * add_resource_folder( std::string resourceTypeName, int gResId = -1, int rezPropValue = -1);
    GPE_GeneralResourceContainer * add_newtype_folder( int rType,std::string resourceTypeName, int gResId = -1,int rezPropValue = -1);
    void delete_resource(GPE_GeneralResourceContainer * otherContainer);
    bool detect_name_change(bool autoChange = true);
    int get_global_id();
    generalGameResource * get_held_resource();
    std::string get_name();
    int get_resource_id();
    int get_resource_type();
    GPE_GeneralResourceContainer * find_resource_from_id(int resourceIdToFind, bool nestDown = true, bool includeSelf = true);
    GPE_GeneralResourceContainer * find_resource_from_name(std::string rNameToFind, bool nestDown = true);
    GPE_GeneralResourceContainer * find_resource_target(std::string rNameToFind, bool nestDown = true);
    GPE_GeneralResourceContainer * get_usable_resource();
    void grab_useable_resources(std::vector <GPE_GeneralResourceContainer * > &rVector);
    GPE_GeneralResourceContainer * get_resource_at(int resourcePos, bool nestDown = false);
    std::string get_project_name();
    int get_resource_count();
    int get_resource_image_frame();
    GPE_Animation * get_resource_sprite();
    GPE_Texture * get_resource_texture();
    int get_size();
    int get_options_width();
    void integrate_into_syntax();
    bool is_folder();
    bool is_super_folder();
    bool is_super_project_folder();
    bool is_child_of(GPE_GeneralResourceContainer * otherContainer);
    bool can_obtain(GPE_GeneralResourceContainer * otherContainer);
    int matches(GPE_GeneralResourceContainer * otherContainer);
    void open_folder();
    void preprocess_container(std::string alternatePath = "", int backupId = -1);
    void prerender_self( );
    int process_container(int xPos, int yPos, int selectedId = -1, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool mouseInRange = false);
    bool read_data_from_projectfile(std::ofstream * fileTarget);
    void remove_resource(GPE_GeneralResourceContainer * otherContainer, bool deleteResource = true);
    //Render options
    void render_contained_object(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_option( int xPos, int yPos, int selectedIdNumber=-1,GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool renderSubOptions = true, bool renderAutomatically = false);
    void render_image( int xPos, int yPos, int rWidth = -1, int rHeight = -1,GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, GPE_Color * renderColor = NULL);
    void render_image_scaled( int xPos, int yPos, double xScale = 1, double yScale = -1,GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, GPE_Color * renderColor = NULL);

    void save_container(std::string alternatePath = "", int backupId = -1);
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    void set_basic_image_value();
    void set_global_id(int newId);
    void set_container_gameid(int newId);
    void set_held_resource(generalGameResource * newGenResource);
    void set_name(std::string newName);
    void set_project_parent_name(std::string newParentName);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern GPE_GeneralResourceContainer * RESOURCE_TO_DRAG;
extern GPE_GeneralResourceContainer * LAST_CLICKED_RESOURCE;
extern bool RESOURCEMENU_WAS_RIGHTCLICKED;
extern GPE_GeneralResourceContainer * RESOURCE_BEINGRENAMED;
extern int LAST_CLICKED_RESTYPE;
extern int DRAGGED_RESTYPE;


#endif
