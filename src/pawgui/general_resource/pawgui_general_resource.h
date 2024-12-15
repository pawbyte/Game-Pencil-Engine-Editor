/*
pawgui_general_resource.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/

#ifndef PAWGUI_GENERAL_RESOURCE_H
#define PAWGUI_GENERAL_RESOURCE_H

#include <string>
#include "../gpe/gpe_font_base.h"
#include "../gpe/gpe_shapes.h"
#include "../gpe/gpe_renderer_base.h"
#include "../gpe/gpe_resources_controller.h"

namespace pawgui
{
    class general_resource
    {
        protected:
            gpe::shape_rect encapBox;
            bool isModified;
            int editorMode;
            int requestedMode;
        public:
            std::string recentErrorMessage;
            bool isFullScreenResource;
            int globalResouceIdNumber;
            std::string resourceFileName;
            std::string resource_name;
            std::string parentProjectName;
            std::string parentProjectDirectory;
            int resourceType;
            int subResouceId;
            int exportBuildGlobalId;
            bool justOpenedThisFrame;
            general_resource();
            virtual ~general_resource() = 0;
            virtual bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
            int get_global_rid();
            virtual std::string get_current_name();
            std::string get_name();
            virtual gpe::animation2d * get_resource_animation();
            virtual gpe::texture_base * get_resource_texture();
            virtual bool is_build_ready();
            virtual bool is_modified();
            bool matches(general_resource * otherResource);
            virtual bool include_local_files( std::string pBuildDir , int buildType );
            virtual void integrate_into_syntax();
            virtual void open_code(int lineNumb, int colNumb, std::string codeTitle = "");
            virtual void load_resource(std::string file_path = "");
            virtual void prerender_self( );
            virtual void process_resource( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            virtual void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_resource( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            virtual void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            virtual void save_resource(std::string file_path = "", int backupId = -1);
            virtual void set_global_rid(int newRId);
            virtual void set_name(std::string new_name);
            virtual void set_parent_project(std::string new_name);
            virtual void update_box(int x_new=-1, int y_new=-1, int newW=-1, int newH=-1);
            virtual void update_string(std::string new_name);
            virtual bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0 );
            virtual int search_for_string(std::string needle);
            virtual int search_and_replace_string(std::string needle, std::string newStr = "");
            bool write_header_on_file(std::ofstream * fileTarget);
    };
}
#endif // PAWGUI_GENERAL_RESOURCE_H
