/*
gpe_scene_layer_class.cpp
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

#include "gpe_scene_layer_class.h"

sceneLayer::sceneLayer(int foundProjectLayerId,pawgui::widget_resource_container *pFolder )
{
    branch_type_id = gpe::branch_type::LAYER;
    iconTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/map.png") ;
    tileAmountX=0;
    tileAmountY =0;
    //layer_id=-1;
    layer_alpha=0;
    layerParentId = projectLayerId = foundProjectLayerId;
    prevTileAmountX=0;
    prevTileAmountY=0;
    projectParentFolder = pFolder;
}

sceneLayer::~sceneLayer()
{

}

void sceneLayer::add_typed_elements()
{

}


void sceneLayer::calculate_size()
{

}

void sceneLayer::process_elements()
{
    GPE_SceneBasicClass::process_elements();
}

void sceneLayer::render_branch()
{
    pawgui::widget_branch::render_branch();
}

bool sceneLayer::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn  )
{
    if( fileTarget!=nullptr && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"[scene_layerContainer="+ stg_ex::int_to_string( projectLayerId )+",]" << "\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        pawgui::widget_branch * current_branch = nullptr;
        for( int i = 0; i < (int)sub_elements.size(); i++)
        {
            current_branch = sub_elements[i];
            if( current_branch!=nullptr )
            {
                current_branch->save_branch_data( fileTarget, nestedFoldersIn+1 );
            }
        }
        *fileTarget << "\n"+nestedTabsStr+"[/scene_layerContainer] \n";
        return true;
    }
    return false;
}
