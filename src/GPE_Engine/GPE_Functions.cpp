/*
functions.cpp
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

#include "GPE_Functions.h"
#include "GPE_Globals.h"
#include "GPE_Texture.h"
#include "GPE_Collisions.h"
#include "GPE_Constants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <limits>
//#include <direct.h>

bool compare_doubles(double a, double b)
{
    return fabs(a - b) < 0.00001;
}

double bound_number(double numbIn, double xMin,double xMax)
{
    if( numbIn < xMin)
    {
        numbIn = xMin;
    }
    else if( numbIn > xMax)
    {
        numbIn = xMax;
    }
    return numbIn;
}

std::string get_file_ext(std::string fileLocationIn)
{
    if( (int)fileLocationIn.size() > 0)
    {
        int dotPos = fileLocationIn.find_first_of(".");
        if(dotPos != (int)std::string::npos)
        {
            return fileLocationIn.substr( dotPos+ 1);
        }
    }
    return "null";
}

bool file_is_audio(std::string fileLocationIn)
{
    std::string foundExt = get_file_ext(fileLocationIn);
    if( foundExt=="aac" || foundExt=="mp3"  || foundExt=="ogg"  || foundExt=="wav" )
    {
        return true;
    }
    return false;
}

bool file_is_font(std::string fileLocationIn)
{
    std::string foundExt = get_file_ext(fileLocationIn);
    if( foundExt=="eot" || foundExt=="svg"  || foundExt=="ttf"  || foundExt=="woff" || foundExt=="woff2" )
    {
        return true;
    }
    return false;
}

bool file_is_image(std::string fileLocationIn)
{
    std::string foundExt = get_file_ext(fileLocationIn);
    if( foundExt=="bmp" || foundExt=="gif"  ||foundExt=="jpg"  || foundExt=="jpeg"  || foundExt=="png" )
    {
        return true;
    }
    return false;
}


bool file_is_video(std::string fileLocationIn)
{
    std::string foundExt = get_file_ext(fileLocationIn);
    if( foundExt=="mp4" || foundExt=="ogg" || foundExt=="webm" )
    {
        return true;
    }
    return false;
}

std::string get_file_noext(std::string fileLocationIn)
{
    if( fileLocationIn.size() > 0)
    {
        int dotPos = fileLocationIn.find_first_of(".");
        if(dotPos != (int)std::string::npos)
        {
            return fileLocationIn.substr( 0,dotPos);
        }
        else
        {
            return fileLocationIn;
        }
    }
    return "null";
}

double point_direction(double x1,double y1,double x2,double y2)
{
    if(x1==x2&&y1==y2)
    {
        return 0;
    }
    return atan2(y2-y1,x2-x1);
}

double point_distance(double x1, double y1, double x2, double y2)
{
    if(x1==x2&&y1==y2)
    {
        return 0;
    }
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int random(int maxIn)
{
    return ( rand()  % (maxIn + 1));
}

int random(int low,int high)
{
    return(rand()  % (high-low + 1) + low);
}

int sign(double valIn)
{
    if(valIn==0)
    {
        return 0;
    }
    else if(valIn<0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

//Operating System Functions
int GPE_Gain_OS()
{
    int returnOS = GPE_IDE_WINDOWS;
    #ifdef _WIN32
        returnOS = GPE_IDE_WINDOWS;
    #elif __APPLE__
        returnOS = GPE_IDE_MAC;
    #else
        returnOS = GPE_IDE_LINUX;
    #endif

    return returnOS;
}
//File Functions
void appendToFile(std::string fileName, std::string stringIn,std::string stringIn2,std::string stringIn3,std::string stringIn4, std::string stringIn5, std::string stringIn6, std::string stringIn7, std::string stringIn8, std::string stringIn9)
{
      std::ofstream filestr(fileName.c_str(), std::ios::out | std::ios::app);
      filestr << "ERROR: "<< stringIn << stringIn2 << stringIn3 << stringIn4 << stringIn5 << stringIn6 << stringIn7 << stringIn8 << stringIn9;
      filestr << " \n";
      filestr.close();
}


void update_rect(GPE_Rect * rectIn, int nx, int ny, int nw, int nh)
{
    if( rectIn!=NULL)
    {
        rectIn->x = nx;
        rectIn->y = ny;
        rectIn->w = nw;
        rectIn->h = nh;
    }
}

bool copy_file(std::string sourceFile, std::string desinationFile)
{
    if( sourceFile!=desinationFile)
    {
        if(file_exists(sourceFile) )
        {
            #ifdef _WIN32
                CopyFile(sourceFile.c_str(), desinationFile.c_str(), FALSE );
            #else
            std::ifstream srce( sourceFile.c_str(), std::ios::binary ) ;
            std::ofstream dest( desinationFile.c_str(), std::ios::binary ) ;
            dest << srce.rdbuf() ;
            #endif
            return true;
        }
    }
    return false;
}

int create_directory( std::string newPathToCreate)
{
    if( (int)newPathToCreate.size() > 2)
    {
        if( path_exists(newPathToCreate) )
        {
            return 0;
        }
        #ifdef _WIN32
            std::string winNewDir = "mkdir \"" + newPathToCreate + "\"";
            if (system(winNewDir.c_str()) == 0)
            {
                return true;
            }
            else
            {
                record_error("Unable to save directory named: "+newPathToCreate+".");
            }
        #else
            int foundError = -1;
            foundError = mkdir(newPathToCreate.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if( foundError==0)
            {
                return true;
            }
        #endif
    }
    return -1;
}

bool file_exists(std::string fileNameIn)
{
    if( (int)fileNameIn.size() > 0 )
    {
        /*
            Code derieved from http://stackoverflow.com/questions/146924/how-can-i-tell-if-a-given-path-is-a-directory-or-a-file-c-c
            http://stackoverflow.com/a/146938
            Mk12 - http://stackoverflow.com/users/148195/mk12
        */
        struct stat s;
        if( stat(fileNameIn.c_str(),&s) == 0 )
        {
            if( s.st_mode & S_IFREG )
            {
                //it's a file
                return true;
            }
        }
    }
    return false;
}

std::string get_path_from_file(std::string fullFilePath)
{
    if( fullFilePath.size()>0)
    {
        size_t found;
        found=fullFilePath.find_last_of("/\\");
        if( found != std::string::npos)
        {
            std::string rString = fullFilePath.substr(0,found);
            if( path_exists(rString) )
            {
                return rString+"\\";
            }
            else
            {
                return rString+"\\";
            }
        }
    }
    return fullFilePath;
}

std::string get_local_from_global_file(std::string fullFilePath)
{
    if( fullFilePath.size()>0)
    {
        size_t found;
        found=fullFilePath.find_last_of("/\\");
        if( found != std::string::npos)
        {
            std::string dString = fullFilePath.substr(0,found);
            std::string rString = fullFilePath.substr(found+1);
            //if( path_exists(dString) )
            {
                return rString;
            }
        }
    }
    return fullFilePath;
}

bool path_exists(std::string pathNameIn)
{
    int pNameSize = (int)pathNameIn.size();
    if( pNameSize > 0 )
    {

        if( pathNameIn[ pNameSize-1] =='/')
        {
            pathNameIn = get_substring(pathNameIn,0,pNameSize-1);
        }
        else if( pathNameIn[ pNameSize-1] =='\\')
        {
            pathNameIn = get_substring(pathNameIn,0,pNameSize-1);
        }
        /*
            Code derieved from http://stackoverflow.com/questions/146924/how-can-i-tell-if-a-given-path-is-a-directory-or-a-file-c-c
            http://stackoverflow.com/a/146938
            Mk12 - http://stackoverflow.com/users/148195/mk12
        */
        struct stat s;
        if( stat(pathNameIn.c_str(),&s) == 0 )
        {
            if( s.st_mode & S_IFDIR )
            {
                //it's a directory
                return true;
            }
        }
    }
    return false;
}

void HEXtoRGB(std::string hexCode,int &r, int &g, int &b)
{
    if( (int)hexCode.size() >= 3)
    {
        if(hexCode[0] == '#')
        {
            hexCode.erase(0,1);
        }
        if( (int)hexCode.size() == 3)
        {
            hexCode = hexCode.substr(0,1)+hexCode.substr(0,1)+hexCode.substr(1,1)+hexCode.substr(1,1)+hexCode.substr(2,1)+hexCode.substr(2,1);
        }

        if( (int)hexCode.size()==6)
        {
            int rrrrr =0;
            int ggggg =0;
            int bbbbb =0;
            std::istringstream(hexCode.substr(0,2)) >> std::hex >> rrrrr;
            std::istringstream(hexCode.substr(2,2)) >> std::hex >> ggggg;
            std::istringstream(hexCode.substr(4,2)) >> std::hex >> bbbbb;
            r = rrrrr;
            g = ggggg;
            b = bbbbb;
        }
        else
        {
            //invalid hex given
            r = 0;
            g = 0;
            b = 0;
        }
    }
}

std::string RGBtoHEX(int r, int g, int b)
{
    char buf[256];
    sprintf(buf, "%.2X%.2X%.2X", r, g, b);
    return std::string(buf);
}

Uint32 get_pixel32( SDL_Surface *surface, int x, int y )
{
    if( surface!=NULL)
    {
        if( x>=0 && x < surface->w && y>=0 && y < surface->h)
        {
            Uint32 *ptr = (Uint32*)surface->pixels;
            //record_error( double_to_string(surface->pitch) );
            int lineoffset = y * (surface->pitch/4 );
            return ptr[lineoffset + x ];
        }
    }
    return 0;
}

void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    if( surface!=NULL)
    {
        if( x < surface->w && y < surface->h)
        {
            //Convert the pixels to 32 bit
            Uint32 *pixels = (Uint32 *)surface->pixels;

            //Set the pixel
            pixels[ ( y * surface->w ) + x ] = pixel;
        }
    }
}

SDL_Surface *load_surface_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        //loadedImage = SDL_DisplayFormatAlpha( loadedImage );

        //If the surface was optimized
        //Color key surface
        //SDL_SetColorKey( loadedImage, SDL_TRUE, SDL_MapRGBA( loadedImage->format, 255, 0, 255,255 ) );

        //Return the optimized surface
        SDL_Surface* formatedImage = SDL_ConvertSurfaceFormat(loadedImage,SDL_PIXELFORMAT_RGBA8888,0);
        if( formatedImage!=NULL)
        {
            SDL_FreeSurface(loadedImage);
            return formatedImage;
        }
        return loadedImage;
    }
    return NULL;
}

SDL_Surface * surface_grayscale( SDL_Surface *surface)
{
    if( surface!=NULL)
    {
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *tempSurface = NULL;
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        tempSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        coloredSurface = SDL_ConvertSurfaceFormat(tempSurface,SDL_PIXELFORMAT_RGBA8888,0);
        if( coloredSurface!=NULL)
        {
            SDL_FreeSurface(tempSurface);
            //coloredSurface = SDL_CreateRGBSurface(0, width, height, 32,SDL_rmask, SDL_gmask, SDL_bmask, SDL_amask);
            //If the surface must be locked
            if( SDL_MUSTLOCK( surface ) )
            {
                //Lock the surface
                SDL_LockSurface( surface );
            }
            Uint8 rr=0, bb=0, gg=0, aa =0;
            //Go through columns
            for( int x = 0; x < coloredSurface->w; x++)
            {
                //Go through rows
                for( int y = 0;y < coloredSurface->h; y++)
                {
                    //Get pixel
                    Uint32 pixel = get_pixel32( surface, x, y );
                    SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);
                    //if the color is a shade of white/black
                    Uint8 grayedColor=(rr+gg+bb)/3;
                    pixel = SDL_MapRGBA(surface->format,grayedColor,grayedColor,grayedColor,aa);
                    put_pixel32( coloredSurface, x, y, pixel );
                }
            }

            //Unlock surface
            if( SDL_MUSTLOCK( surface ) )
            {
                SDL_UnlockSurface( surface );
            }

            //Return coloredSurface surface
            return coloredSurface;
        }
        else if( tempSurface!=NULL)
        {
            SDL_FreeSurface(tempSurface);
            tempSurface = NULL;
        }
    }
    return NULL;
}

SDL_Surface * surface_invert( SDL_Surface *surface)
{
    if( surface!=NULL)
    {
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *tempSurface = NULL;
        SDL_Surface *coloredSurface = NULL;

        tempSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        coloredSurface = SDL_ConvertSurfaceFormat(tempSurface,SDL_PIXELFORMAT_RGBA8888,0);
        if( coloredSurface!=NULL)
        {
            SDL_FreeSurface(tempSurface);
            //If the surface must be locked
            if( SDL_MUSTLOCK( surface ) )
            {
                //Lock the surface
                SDL_LockSurface( surface );
            }
            Uint8 rr=0, bb=0, gg=0, aa = 0;
            //Go through columns
            for( int x = 0; x < coloredSurface->w; x++)
            {
                //Go through rows
                for( int y = 0;y < coloredSurface->h; y++)
                {
                    //Get pixel
                    Uint32 pixel = get_pixel32( surface, x, y );
                    SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);
                    pixel = SDL_MapRGBA(surface->format,255-rr,255-gg,255-bb,aa);
                    put_pixel32( coloredSurface, x, y, pixel );
                }
            }

            //Unlock surface
            if( SDL_MUSTLOCK( surface ) )
            {
                SDL_UnlockSurface( surface );
            }

            //Return coloredSurface surface
            return coloredSurface;
        }
        else if( tempSurface!=NULL)
        {
            SDL_FreeSurface(tempSurface);
            tempSurface = NULL;
        }
    }
    return NULL;
}

SDL_Surface * surface_recolor( SDL_Surface *surface, SDL_Color colorMerge, float amount )
{
    if( surface!=NULL)
    {
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *tempSurface = NULL;
        SDL_Surface *coloredSurface = NULL;

        //If the image is color keyed
        tempSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        coloredSurface = SDL_ConvertSurfaceFormat(tempSurface,SDL_PIXELFORMAT_RGBA8888,0);
        if( coloredSurface!=NULL)
        {
            SDL_FreeSurface(tempSurface);
            //If the surface must be locked
            if( SDL_MUSTLOCK( surface ) )
            {
                //Lock the surface
                SDL_LockSurface( surface );
            }
            Uint8 rr=0, bb=0, gg=0, aa = 0;
            //Go through columns
            for( int x = 0; x < coloredSurface->w; x++)
            {
                //Go through rows
                for( int y = 0;y < coloredSurface->h; y++)
                {
                    //Get pixel
                    Uint32 pixel = get_pixel32( surface, x, y );
                    SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);

                    rr=merge_channel(rr,colorMerge.r,amount);
                    gg=merge_channel(gg,colorMerge.g,amount);
                    bb=merge_channel(bb,colorMerge.b,amount);
                    pixel = SDL_MapRGBA(surface->format,rr,gg,bb, aa);
                    put_pixel32( coloredSurface, x, y, pixel );
                }
            }

            //Unlock surface
            if( SDL_MUSTLOCK( surface ) )
            {
                SDL_UnlockSurface( surface );
            }

            //Return coloredSurface surface
            return coloredSurface;
        }
        else if( tempSurface!=NULL)
        {
            SDL_FreeSurface(tempSurface);
            tempSurface = NULL;
        }
    }
    return NULL;
}

SDL_Surface * surface_remove_color( SDL_Surface *surface, SDL_Color colorToRemove )
{
    record_error("Removing Color.... rgb("+int_to_string( colorToRemove.r)+","+int_to_string( colorToRemove.g)+","+int_to_string( colorToRemove.b)+").");
    if( surface!=NULL)
    {
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;

        coloredSurface = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGBA8888,0);
        if( coloredSurface!=NULL)
        {
            //If the surface must be locked
            if( SDL_MUSTLOCK( coloredSurface ) )
            {
                //Lock the surface
                SDL_LockSurface( coloredSurface );
            }
            Uint8 rr=0, bb=0, gg=0, aa = 0;
            int y = 0;
            Uint32 pixel;
            //Go through columns
            for( int x = 0; x < coloredSurface->w; x++)
            {
                //Go through rows
                for(  y = 0;y < coloredSurface->h; y++)
                {
                    //Get pixel
                    pixel = get_pixel32( coloredSurface, x, y );
                    SDL_GetRGBA(pixel,coloredSurface->format,&rr,&gg,&bb, &aa);
                    //if the color is shade of white/gray
                    if(rr==colorToRemove.r&&gg==colorToRemove.g&&bb==colorToRemove.b)
                    {
                        pixel = SDL_MapRGBA(coloredSurface->format,255,0,255,0);
                    }
                    put_pixel32( coloredSurface, x, y, pixel );
                }
            }

            //Unlock surface
            if( SDL_MUSTLOCK( coloredSurface ) )
            {
                SDL_UnlockSurface( coloredSurface );
            }

            //Return coloredSurface surface
            record_error("Returning colored surface");
            return coloredSurface;
        }
    }
    record_error("Returning Null surface");
    return NULL;
}

SDL_Surface * surface_flip( SDL_Surface *surface, int flags )
{
    if( surface!=NULL)
    {
        //Pointer to the soon to be flipped surface
        SDL_Surface *flipped = NULL;

        flipped = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGBA8888,0);
        if( flipped!=NULL)
        {
            //If the surface must be locked
            if( SDL_MUSTLOCK( flipped ) )
            {
                //Lock the surface
                SDL_LockSurface( flipped );
            }

            //Go through columns
            for( int x = 0, rx = flipped->w - 1; x < flipped->w; x++, rx-- )
            {
                //Go through rows
                for( int y = 0, ry = flipped->h - 1; y < flipped->h; y++, ry-- )
                {
                    //Get pixel
                    Uint32 pixel = get_pixel32( surface, x, y );

                    //Copy pixel
                    if( ( flags & FLIP_VERTICAL ) && ( flags & FLIP_HORIZONTAL ) )
                    {
                        put_pixel32( flipped, rx, ry, pixel );
                    }
                    else if( flags & FLIP_HORIZONTAL )
                    {
                        put_pixel32( flipped, rx, y, pixel );
                    }
                    else if( flags & FLIP_VERTICAL )
                    {
                        put_pixel32( flipped, x, ry, pixel );
                    }
                }
            }

            //Unlock surface
            if( SDL_MUSTLOCK( flipped ) )
            {
                SDL_UnlockSurface( flipped );
            }

            //Return flipped surface
            return flipped;
        }
    }
    return NULL;
}
