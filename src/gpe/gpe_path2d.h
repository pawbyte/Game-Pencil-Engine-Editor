/*
gpe_path2d.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef gpe_path2d_h
#define gpe_path2d_h

#include "gpe_common_includes.h"
#include "gpe_branch.h"

namespace gpe
{
    class game_path_point2d
    {
        public:
            float x_pos;
            float y_pos;
            float pointSpeed;
            game_path_point2d(int point_x, int point_y, float speed = 1);
            ~game_path_point2d();
    };

    class game_path2d
    {
        private:
            int pathId;
            bool isClosedPath;
            std::vector< game_path_point2d * > pathPoints;
            std::vector< game_path_point2d * > pathOriginalPoints;
            int pathLength;
            int originalPathLength;
        public:
            game_path2d(int pId, bool pIsClosed);
            ~game_path2d();
    };
}

#endif //gpe_path2d_h
