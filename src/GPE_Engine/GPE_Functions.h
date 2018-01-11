/*
GPE_Functions.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "GPE_CIncludes.h"
#include "GPE_Renderer.h"

//The headers
bool compare_doubles(double a,double b);
bool copy_file(std::string sourceFile, std::string desinationFile);
int create_directory( std::string newPathToCreate);
double bound_number(double numbIn,double xMin =0, double xMax = 100);

std::string get_file_ext(std::string fileLocationIn);
std::string get_file_ext_last_dot(std::string fileLocationIn);
bool file_is_audio(std::string fileLocationIn);
bool file_is_font(std::string fileLocationIn);
bool file_is_image(std::string fileLocationIn);
bool file_is_video(std::string fileLocationIn);

std::string get_file_noext(std::string fileLocationIn);
//match functions
double point_direction(double x1,double y1,double x2,double y2);
double point_distance(double x1, double y1, double x2, double y2);
//random function
int random(int maxIn);
int random(int low,int high);
int sign(double valIn);

void update_rect(GPE_Rect * rectIn, int nx, int ny, int nw, int nh);

std::string get_path_from_file(std::string fullFilePath);
std::string get_local_from_global_file(std::string fullFilePath);

bool file_exists(std::string fileNameIn);
bool path_exists(std::string pathNameIn);

void HEXtoRGB(std::string hexCode,int &r, int &g, int &b);
std::string RGBtoHEX(int r, int g, int b);

//Surface Functions
SDL_Surface * load_surface_image( std::string filename );

Uint32 get_pixel32( SDL_Surface *surface, int x, int y );
void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );

SDL_Surface * surface_grayscale( SDL_Surface *surface);
SDL_Surface * surface_invert( SDL_Surface *surface);
SDL_Surface * surface_merge_color( SDL_Surface *surface, SDL_Color colorMerge, float amount );
SDL_Surface * surface_remove_color( SDL_Surface *surface, SDL_Color colorToRemove );
SDL_Surface * surface_flip( SDL_Surface *surface, int flags );

#endif
