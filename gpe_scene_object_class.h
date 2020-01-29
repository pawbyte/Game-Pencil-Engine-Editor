/*
gpe_scene_object_class.h
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

#ifndef GPE_SCENE_OBJECT_CLASS_H
#define GPE_SCENE_OBJECT_CLASS_H

#include "gpe_scene_basic_class.h"
#include "gpe_scene_helper_class.h"
#include "animation_resource.h"
#include "game_object_resource.h"

class GPE_SceneGameObject: public GPE_SceneBasicClass
{
public:
    int customObjId;
    int objTypeId;
    std::string objTypeName;
    gameObjectResource * objBeingPlaced;
    GPE_SceneGameObject( std::string nName );
    ~GPE_SceneGameObject();

    void add_typed_elements();
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  GPE_GeneralResourceContainer * localResTypeController );
    void calculate_size();
    void render_branch();
    void process_elements();
    bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

#endif //GPE_SCENE_OBJECT_CLASS_H
