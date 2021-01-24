/*
pawgui_general_resource_container.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#ifndef PAWGUI_GRC_H
#define PAWGUI_GRC_H

#include "pawgui_base.h"
#include "pawgui_general_resource.h"

namespace pawgui
{
    const int resource_container_default_height = 14;
    const int restype_regular = -1;
    const int restype_superfolder = 0;
    const int restype_projfolder = 1;

    class widget_resource_container: public widget_basic
    {
        protected:
            int globalResouceId;
            int resourceId;
            int resourceType;
            bool isFolder, isSuperFolder, isSuperProjectFolder;
            bool subMenuIsOpen;
            gpe::animaton2d * containeranimation;
            gpe::texture_base * containerTexture;
            int animationFrameNumber;
            general_resource * heldResource;
            std::vector <widget_resource_container *> subOptions;
            bool resouceNameChanged;
            bool subContentsModified;
            int strTexWidth, strTexHeight;
        public:
            int foundX2Pos;
            int exportBuildGlobalId;
            std::string parentProjectDirectory;
            std::string projectParentFileName;
            std::string projectParentName;
            gpe::shape_rect element_box;
            widget_resource_container * parentResource;
            //widget_resource_container(std::string projFolderName, int  rezPropValue = -1);
            widget_resource_container(std::string projFolderName = "", std::string new_name =  "", int rType = -1, int rId = -1,bool folder = true, int globalIdVal = -1,int  rezPropValue = -1);
            virtual ~widget_resource_container();
            void add_resource_container( widget_resource_container * newResource, bool changeGlobalId = false, widget_resource_container * referenceObject = NULL );
            widget_resource_container * add_resource_folder( std::string resourceTypeName, int gResId = -1, int rezPropValue = -1);
            widget_resource_container * add_newtype_folder( int rType,std::string resourceTypeName, int gResId = -1,int rezPropValue = -1);
            void delete_resource(widget_resource_container * otherContainer);
            bool detect_name_change(bool autoChange = true);
            int get_global_id();
            general_resource * get_held_resource();
            std::string get_name();
            int get_resource_id();
            int get_resource_type();
            widget_resource_container * find_resource_from_id(int resourceIdToFind, bool nestDown = true, bool includeSelf = true);
            widget_resource_container * find_resource_from_name(std::string rNameToFind, bool nestDown = true);
            widget_resource_container * find_resource_target(std::string rNameToFind, bool nestDown = true);
            widget_resource_container * get_usable_resource();
            void grab_useable_resources(std::vector <widget_resource_container * > &rVector);
            widget_resource_container * get_resource_at(int resourcePos, bool nestDown = false);
            std::string get_project_name();
            int get_resource_count();
            int get_resource_image_frame();
            virtual gpe::animaton2d * get_resource_animation();
            virtual gpe::texture_base * get_resource_texture();
            int get_size();
            int get_options_width();
            bool include_local_files( std::string pBuildDir , int buildType );
            void integrate_into_syntax();
            bool is_folder();
            bool is_super_folder();
            bool is_super_project_folder();
            bool is_child_of(widget_resource_container * otherContainer);
            bool can_obtain(widget_resource_container * otherContainer);
            int matches(widget_resource_container * otherContainer);
            void open_folder();
            void preprocess_container(std::string file_path = "", int backupId = -1);
            void prerender_self( );
            int process_container(int xPos, int yPos, int selectedId = -1, gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL, bool mouseInRange = false);
            bool read_data_from_projectfile(std::ofstream * fileTarget);
            void remove_resource(widget_resource_container * otherContainer, bool deleteResource = true);
            //Render options
            void render_contained_object( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
            void render_option( int xPos, int yPos, int selectedIdNumber=-1, gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL, bool renderSubOptions = true, bool renderAutomatically = false);
            void render_image( int xPos, int yPos, int r_width = -1, int r_height = -1, gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL, gpe::color * renderColor = NULL);
            void render_image_scaled( int xPos, int yPos, float xScale = 1, float yScale = -1, gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL, gpe::color * renderColor = NULL);

            void save_container(std::string file_path = "", int backupId = -1);
            int search_for_string(std::string needle);
            int search_and_replace_string(std::string needle, std::string newStr = "");
            void set_basic_image_value();
            void set_global_id(int new_id);
            void set_container_gameid(int new_id);
            void set_held_resource(general_resource * newGenResource);
            void set_name(std::string new_name);
            void set_project_parent_name(std::string newParentName);
            bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
    };

    extern widget_resource_container * resource_dragged;
    extern widget_resource_container * resource_last_clicked;
    extern bool resourcemenu_rightclicked;
    extern widget_resource_container * resource_being_renamed;
    extern int resource_last_clicked_type;
    extern int resource_dragged_type;

    extern std::string project_current_name;
    extern std::string seeked_project_name;
    extern int resourcetype_last_rendered_type;
}
#endif // PAWGUI_GRC_H
