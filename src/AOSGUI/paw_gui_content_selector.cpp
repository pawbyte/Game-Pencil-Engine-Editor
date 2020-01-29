/*
paw_gui_content_selector.cpp
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
#include "paw_gui_content_selector.h"

GPE_Content_Selector * GPE_Content_Selector_Current = NULL;
int GPE_Content_Selector_State = -1;

GPE_Content_Selector::GPE_Content_Selector(std::string name,std::string description)
{
    opName = name;
    set_label( name );
    descriptionText = description;
    guiListTypeName = "contentselector";
    guiListTypeId = 2;
    storedInt = -1;
    storeddfloat = -1;
    storedString = "";
    storedColor = new GPE_Color("customColor", 128,128, 128 );

    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 196;
    elementBox.h = 32;
    fieldElementBox.x = elementBox.x+128;
    fieldElementBox.y = elementBox.y;
    fieldElementBox.w = elementBox.w-128;
    fieldElementBox.h = elementBox.h;


}

GPE_Content_Selector::~GPE_Content_Selector()
{

}

std::string GPE_Content_Selector::get_data()
{

}

void GPE_Content_Selector::load_data(std::string dataString)
{

}

std::string GPE_Content_Selector::get_plain_string()
{

}

void GPE_Content_Selector::set_label(std::string newLabel)
{

}

void GPE_Content_Selector::set_name(std::string newName)
{

}

void GPE_Content_Selector::set_string(std::string newString)
{

}

void GPE_Content_Selector::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    fieldElementBox.x = elementBox.x+elementBox.w-32;
    fieldElementBox.y = elementBox.y;
    fieldElementBox.w = 32;
    fieldElementBox.h = elementBox.h;

    if( elementBox.w > 512)
    {
        fieldElementBox.x = elementBox.x+elementBox.w-128;
        fieldElementBox.w = 128;
    }

    if( elementBox.w > 384)
    {
        fieldElementBox.x = elementBox.x+elementBox.w-96;
        fieldElementBox.w = 96;
    }
    else if( elementBox.w > 256)
    {
        fieldElementBox.x = elementBox.x+elementBox.w-64;
        fieldElementBox.w = 64;
    }
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);

    if( isEnabled)
    {

        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
    }

    //
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }
    if( isClicked)
    {
        //GPE_Change_Color_PopUp(popUpString,storedColor);
        if( GPE_Content_Selector_State == -1)
        {
            GPE_Content_Selector_State = 0;
            GPE_Content_Selector_Current = this;
            gpe->cursor_change( "hand" );
        }
        else
        {
            GPE_Content_Selector_State = -1;
            GPE_Content_Selector_Current = NULL;
            gpe->cursor_change( "arrow");
        }
        isClicked = false;
    }
    else if( GPE_Content_Selector_State == 0)
    {
        gpe->cursor_change( "hand" );
    }
}

void GPE_Content_Selector::render_self( GPE_Rect * viewedSpace , GPE_Rect * cam  )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( isEnabled && cam!=NULL)
    {
        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y, GPE_MAIN_THEME->Main_Box_Highlighted_Color, false );
            if( (int)opName.size() > 0)
            {
                gfs->render_text_resized( elementBox.x-cam->x+elementBox.h+GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2-cam->y,opName+":",GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE );
            }
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y, GPE_MAIN_THEME->Main_Box_Color, false );

            if( (int)opName.size() > 0 )
            {
                gfs->render_text_resized( elementBox.x-cam->x+elementBox.h+GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2-cam->y,opName+":",GPE_MAIN_THEME->Main_Box_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE );
            }
        }

        if( GPE_EYE_DROPPER!=NULL)
        {
            GPE_EYE_DROPPER->render_tex_resized( elementBox.x-cam->x + elementBox.h/4, elementBox.y-cam->y + elementBox.h/4, elementBox.h/2, elementBox.h/2,  NULL, GPE_MAIN_THEME->Main_Box_Font_Color );
        }
        if( selectorType == GPE_CS_TYPE_INT )
        {
            gfs->render_text_resized( elementBox.x-cam->x+elementBox.w,elementBox.y+elementBox.h/2-cam->y,int_to_string(storedInt),GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,FONT_TEXTINPUT,FA_RIGHT,FA_MIDDLE );

        }
        else if( selectorType == GPE_CS_TYPE_float )
        {
            gfs->render_text_resized( elementBox.x-cam->x+elementBox.w,elementBox.y+elementBox.h/2-cam->y,float_to_string(storeddfloat),GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,FONT_TEXTINPUT,FA_RIGHT,FA_MIDDLE );
        }
        else if( selectorType == GPE_CS_TYPE_COLOR )
        {
            gcanvas->render_rectangle( fieldElementBox.x-cam->x,fieldElementBox.y-cam->y,fieldElementBox.x+fieldElementBox.w-cam->x,fieldElementBox.y+fieldElementBox.h-cam->y,storedColor,false);
            //gcanvas->render_vertical_line_color( elementBox.x-cam->x,elementBox.y-cam->y, elementBox.y+elementBox.h-cam->y, GPE_MAIN_THEME->Main_Border_Color );
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y, GPE_MAIN_THEME->Main_Border_Color, true );
        }
        else if(  selectorType== GPE_CS_TYPE_STRING)
        {
            if( (int)storedString.size() > 0  )
            {
                gfs->render_text_resized( elementBox.x-cam->x+elementBox.w,elementBox.y+elementBox.h/2-cam->y,storedString,GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,FONT_TEXTINPUT,FA_RIGHT,FA_MIDDLE );
            }
            else
            {
                gfs->render_text_resized( elementBox.x-cam->x+elementBox.w,elementBox.y+elementBox.h/2-cam->y,"undefined",GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,FONT_TEXTINPUT,FA_RIGHT,FA_MIDDLE );
            }
        }
        else
        {
            gfs->render_text_resized( elementBox.x-cam->x+elementBox.w,elementBox.y+elementBox.h/2-cam->y,"[invalid]",GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,FONT_TEXTINPUT,FA_RIGHT,FA_MIDDLE );
        }
    }
}

void GPE_Content_Selector::set_color_from_rgb(GPE_Color *newColor)
{

}

void GPE_Content_Selector::set_color_from_rgb(std::string newColorStr)
{

}

void GPE_Content_Selector::set_color_from_hex(std::string newColorStr)
{

}

void GPE_Content_Selector::set_r(int r)
{

}

void GPE_Content_Selector::set_g(int g)
{

}

void GPE_Content_Selector::set_b(int b)
{

}

void GPE_Content_Selector::set_rgb(int r, int g, int b)
{

}
