/*
gpe_scene_group_class.h
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

#ifndef GPE_SCENE_LAYER_CLASS_H
#define GPE_SCENE_LAYER_CLASS_H

#include "gpe_scene_basic_class.h"
#include "gpe_scene_light_class.h"

class sceneLayer: public GPE_SceneBasicClass
{
public:
    pawgui::widget_resource_container * projectParentFolder;
    int tileAmountX, tileAmountY;
    //int layer_id;
    int layer_alpha;
    int projectLayerId;
    int prevTileAmountX, prevTileAmountY;
    //Lights [ BEGIN ]
    std::vector< GPE_SceneBasicLight * > layerLights;
    //Lights [ END ]
    sceneLayer( int foundProjectLayerId, pawgui::widget_resource_container *pFolder = nullptr);
    ~sceneLayer();
    void add_typed_elements();
    void calculate_size();
    void process_elements();
    void render_branch();
    void set_background_name( int bbId );
    bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif //GPE_SCENE_LAYER_CLASS_H
