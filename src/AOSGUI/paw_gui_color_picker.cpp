/*
paw_gui_color_picker.cpp
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

#include "paw_gui_text_input.h"
#include "paw_gui_buttons.h"
#include "paw_gui_color_picker.h"
#include "paw_gui_labels.h"

SDL_Surface * GPE_SURFACE_COLOR_PICKER_GRADIENT = NULL;
GPE_Texture_Base * GPE_TEXTURE_COLOR_PICKER_GRADIENT = NULL;

GPE_Input_Field_Color::GPE_Input_Field_Color( std::string name,std::string description, int r, int g, int b )
{
    opName = name;
    set_label( name );
    descriptionText = description;
    guiListTypeName = "colorpicker";
    guiListTypeId = 2;
    storedColor = new GPE_Color("customColor", r,g,b );
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 196;
    elementBox.h = 32;
    fieldElementBox.x = elementBox.x+128;
    fieldElementBox.y = elementBox.y;
    fieldElementBox.w = elementBox.w-128;
    fieldElementBox.h = elementBox.h;

    fontSizeH = 12;
    fontSizeW = 12;
    FONT_TEXTINPUT->get_metrics("A",&fontSizeW,&fontSizeH);
    if( fontSizeW<=0)
    {
        fontSizeW = 12;
    }
    if( fontSizeH<=0)
    {
        fontSizeH = 12;
    }
}

GPE_Input_Field_Color::~GPE_Input_Field_Color()
{
    if( storedColor!=NULL)
    {
        delete storedColor;
        storedColor = NULL;
    }
}

std::string GPE_Input_Field_Color::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+get_rgb_string()+",,,";
}

void GPE_Input_Field_Color::load_data(std::string dataString)
{
    std::string rgbColor = split_first_string(dataString,",,,");
    set_color_from_rgb(rgbColor);
}

GPE_Color * GPE_Input_Field_Color::get_color()
{
    if( storedColor!=NULL)
    {
        return storedColor;
    }
    return NULL;
}

GPE_Color * GPE_Input_Field_Color::get_duplicate_color()
{
    if( storedColor!=NULL)
    {
        return storedColor->duplicate_color();
    }
    return NULL;
}

std::string GPE_Input_Field_Color::get_hex_string()
{
    if( storedColor!=NULL)
    {
        return GPE_COLOR_SYSTEM->rgb_to_hex(storedColor->get_r(),storedColor->get_g(),storedColor->get_b() );
    }
    return "000";
}

std::string GPE_Input_Field_Color::get_rgb_string()
{
    if( storedColor!=NULL)
    {
        return int_to_string(storedColor->get_r() )+","+int_to_string(storedColor->get_g() )+","+int_to_string(storedColor->get_b() );
    }
    return "0,0,0";
}

std::string GPE_Input_Field_Color::get_plain_string()
{
    return "\""+get_hex_string()+"\"";

}

int GPE_Input_Field_Color::get_r()
{
    if( storedColor!=NULL)
    {
        return storedColor->get_r();
    }
    return 0;
}

int GPE_Input_Field_Color::get_g()
{
    if( storedColor!=NULL)
    {
        return storedColor->get_g();
    }
    return 0;
}

int GPE_Input_Field_Color::get_b()
{
    if( storedColor!=NULL)
    {
        return storedColor->get_b();
    }
    return 0;
}

void GPE_Input_Field_Color::set_label(std::string newLabel)
{
    if( (int)newLabel.size() > 0)
    {
        inputLabel = newLabel;
        showLabel = true;
    }
    else
    {
        inputLabel = "";
        showLabel = false;
    }
}

void GPE_Input_Field_Color::set_name(std::string newName)
{
    if( (int)newName.size() > 0)
    {
        if( opName == inputLabel )
        {
            set_label(newName);
        }
        opName = newName;
    }
}

void GPE_Input_Field_Color::set_string(std::string newString)
{

}

void GPE_Input_Field_Color::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    if( elementBox.w >= 256 )
    {
        fieldElementBox.x = elementBox.x+elementBox.w-128;
        fieldElementBox.y = elementBox.y;
        fieldElementBox.w = 128;
        fieldElementBox.h = elementBox.h;
    }
    else if( elementBox.w >= 64 )
    {
        fieldElementBox.x = elementBox.x+elementBox.w-32;
        fieldElementBox.y = elementBox.y;
        fieldElementBox.w = 32;
        fieldElementBox.h = elementBox.h;
    }
    else
    {
        fieldElementBox.x = elementBox.x+elementBox.w/2;
        fieldElementBox.y = elementBox.y;
        fieldElementBox.w = elementBox.w/2;
        fieldElementBox.h = elementBox.h;
    }
    if( showLabel)
    {
        set_label(inputLabel);
    }
    /*
    Old Protocol for Label Fields
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);

    New Protocol for labeled fields

    */
    //
    if( isEnabled)
    {
        isClicked = false;
        isHovered = false;
        clickedOutside = false;
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if(viewedSpace!=NULL && cam!=NULL)
        {
            if( point_between(input->mouse_x,input->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
            {
                if (point_between(input->mouse_x,input->mouse_y,fieldElementBox.x+viewedSpace->x-cam->x,fieldElementBox.y+viewedSpace->y-cam->y,fieldElementBox.x+fieldElementBox.w+viewedSpace->x-cam->x,fieldElementBox.y+fieldElementBox.h+viewedSpace->y-cam->y) )
                {
                    isHovered = true;
                    if( (int)descriptionText.size()>0 )
                    {
                        MAIN_OVERLAY->update_tooltip(descriptionText);
                    }
                    else
                    {
                        MAIN_OVERLAY->update_tooltip(opName);
                    }
                    if( input->check_mouse_released( mb_left))
                    {
                        isClicked = true;
                    }
                }
                else if( input->check_mouse_released( mb_anybutton ) )
                {
                    clickedOutside = true;
                }
            }
            else if( input->check_mouse_released( mb_anybutton ) )
            {
                clickedOutside = true;
            }
        }
    }

    //
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }
    if( isClicked)
    {
        std::string popUpString = "";
        if( (int)inputLabel.size() > 0)
        {
            popUpString = "Change "+inputLabel;
        }
        else
        {
            popUpString = "Select a Color";
        }
        GPE_Change_Color_PopUp(popUpString,storedColor);
    }
}

void GPE_Input_Field_Color::render_self( GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( isEnabled && cam!=NULL)
    {
        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y, GPE_MAIN_THEME->Main_Box_Highlighted_Color, false );
            if( showLabel && (int)inputLabel.size() > 0)
            {
                gfs->render_text_resized( elementBox.x-cam->x+GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2-cam->y,inputLabel,GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE );
            }
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y, GPE_MAIN_THEME->Main_Box_Color, false );

            if( showLabel && (int)inputLabel.size() > 0)
            {
                gfs->render_text_resized( elementBox.x-cam->x+GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2-cam->y,inputLabel,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE );
            }
        }
        gcanvas->render_rectangle( fieldElementBox.x-cam->x,fieldElementBox.y-cam->y,fieldElementBox.x+fieldElementBox.w-cam->x,fieldElementBox.y+fieldElementBox.h-cam->y,storedColor,false);
        //gcanvas->render_vertical_line_color( elementBox.x-cam->x,elementBox.y-cam->y, elementBox.y+elementBox.h-cam->y, GPE_MAIN_THEME->Main_Border_Color );
        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y, GPE_MAIN_THEME->Main_Border_Color, true );
    }
}

void GPE_Input_Field_Color::set_color_from_rgb(GPE_Color *newColor)
{
    if( storedColor==NULL)
    {
        storedColor = new GPE_Color("custom",0,0,0);
    }
    if( storedColor!=NULL)
    {
        if( newColor!=NULL)
        {
            storedColor->change_rgba(newColor->get_r(),newColor->get_g(),newColor->get_b() );
        }
    }
}

void GPE_Input_Field_Color::set_color_from_rgb(std::string newColorStr)
{
    int r = split_first_int(newColorStr,',');
    int g = split_first_int(newColorStr,',');
    int b = string_to_int(newColorStr,0);

    if( r < 0 || r > 255)
    {
        r = 0;
    }
    if( g < 0 || g > 255)
    {
        g = 0;
    }
    if( b < 0 || g > 255)
    {
        b = 0;
    }
    if( storedColor==NULL)
    {
        storedColor = new GPE_Color( "custom",0,0,0 );
    }
    if( storedColor!=NULL)
    {
        storedColor->change_rgba(r,g,b);
    }
}

void GPE_Input_Field_Color::set_color_from_hex(std::string newColorStr)
{
    int r = 0, g = 0, b = 0;
    GPE_COLOR_SYSTEM->hex_to_rgb( newColorStr,r, g, b );
    if( storedColor==NULL)
    {
        storedColor = new GPE_Color( "custom",0,0,0 );
    }
    if( storedColor!=NULL)
    {
        storedColor->change_rgba(r,g,b);
    }
}

void GPE_Input_Field_Color::set_r(int r)
{
    if( r < 0 || r > 255)
    {
        r = 0;
    }
    if( storedColor!=NULL)
    {
        storedColor->change_r(r);
    }
}

void GPE_Input_Field_Color::set_g(int g)
{
    if( g < 0 || g > 255)
    {
        g = 0;
    }
    if( storedColor!=NULL)
    {
        storedColor->change_g(g);
    }
}

void GPE_Input_Field_Color::set_b(int b)
{
    if( b < 0 || b > 255)
    {
        b = 0;
    }
    if( storedColor!=NULL)
    {
        storedColor->change_b(b);
    }
}

void GPE_Input_Field_Color::set_rgb(int r, int g, int b)
{
    if( r < 0 || r > 255)
    {
        r = 0;
    }

    if( g < 0 || g > 255)
    {
        g = 0;
    }

    if( b < 0 || b > 255)
    {
        b = 0;
    }
    if( storedColor!=NULL)
    {
        storedColor->change_rgba(r,g,b,255);
    }
}

bool GPE_Change_Color_PopUp(std::string popUpCaption,GPE_Color * currColor)
{
    if( currColor == NULL)
    {
        return false;
    }
    gpe->end_loop();
    //RESOURCE_TO_DRAG = NULL;
    if( (int)popUpCaption.size() ==0)
    {
        popUpCaption = "Select a Color";
    }
    MAIN_OVERLAY->take_frozen_screenshot( );

    int promptBoxWidth = 500;
    int promptBoxHeight = 400;
    GPE_Rect elementBox;
    GPE_Rect colorShadeBox;
    colorShadeBox.w = 32;
    colorShadeBox.h = 200;

    bool exitOperation = false;
    bool operationCancelled = false;
    input->reset_all_input();
    GPE_Color * alteredColor = NULL, *colorShadeTempColor = NULL;
    colorShadeTempColor = new GPE_Color( "custom",0,0,0 );
    int rV = 0,  gV = 0,  bV = 0;
    Uint8 rU8 = 0, gU8 = 0, bU8 = 0, uU8 = 0;
    Uint32 foundPixelColor = 0;
    int foundColorPickerX = 0, foundColorPickerY = 0;
    int colorPickerShaderI = 0;
    //float colorShadeDifference = 0;
    float colorShadeDivision = 0;
    int selectedColorShade = colorShadeBox.h/2;
    GPE_TextInputNumber * newColorRValue = new GPE_TextInputNumber("",true,0,255);
    newColorRValue->set_label("Red");
    GPE_TextInputNumber * newColorGValue = new GPE_TextInputNumber("",true,0,255);
    newColorGValue->set_label("Green");
    GPE_TextInputNumber * newColorBValue = new GPE_TextInputNumber("",true,0,255);
    newColorBValue->set_label("Blue");

    GPE_TextInputNumber * newColorHValue = new GPE_TextInputNumber("",true,0,240);
    newColorHValue->set_label("Hue");
    GPE_TextInputNumber * newColorSValue = new GPE_TextInputNumber("",true,0,240);
    newColorSValue->set_label("Sat");
    GPE_TextInputNumber * newColorLValue = new GPE_TextInputNumber("",true,0,240);
    newColorLValue->set_label("Lum");


    GPE_TextInputBasic * newColorHexValue = new GPE_TextInputBasic("#FFFFFF");
    newColorHexValue->set_label("Hex: #");

    alteredColor = new GPE_Color("CurrentColor",currColor->get_r(),currColor->get_g(),currColor->get_b() );
    newColorRValue->set_string( int_to_string( currColor->get_r() ) );
    newColorGValue->set_string( int_to_string( currColor->get_g() ) );
    newColorBValue->set_string( int_to_string( currColor->get_b() ) );
    newColorHexValue->set_string( GPE_COLOR_SYSTEM->rgb_to_hex( currColor->get_r(),currColor->get_g(),currColor->get_b() ) ); //Convert RGB to Hex here

    GPE_ToolLabelButton * yesButton = new GPE_ToolLabelButton( "Okay","Confirm Color Change");
    GPE_ToolLabelButton * cancelButton = new GPE_ToolLabelButton( "Cancel","Cancel Color Request");

    while(exitOperation==false)
    {
        gpe->start_loop();

        elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2;
        elementBox.y = (SCREEN_HEIGHT-promptBoxHeight)/2;
        elementBox.w = promptBoxWidth;
        elementBox.h = promptBoxHeight;

        colorShadeBox.x = elementBox.x+GENERAL_GPE_GUI_PADDING*2+GPE_TEXTURE_COLOR_PICKER_GRADIENT->get_width();
        colorShadeBox.y = elementBox.y+GENERAL_GPE_GUI_PADDING+32;

        //GPE_MAIN_GUI->reset_gui_info();
        yesButton->set_coords( elementBox.x+GENERAL_GPE_GUI_PADDING, elementBox.y+elementBox.h-32);
        cancelButton->set_coords( yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_GUI_PADDING,yesButton->get_ypos() );

        newColorRValue->set_coords(colorShadeBox.x+colorShadeBox.w+GENERAL_GPE_GUI_PADDING,elementBox.y+GENERAL_GPE_GUI_PADDING+48);
        newColorRValue->set_width(128);

        newColorGValue->set_coords(colorShadeBox.x+colorShadeBox.w+GENERAL_GPE_GUI_PADDING,newColorRValue->get_ypos()+newColorRValue->get_height()+GENERAL_GPE_GUI_PADDING);
        newColorGValue->set_width(128);

        newColorBValue->set_coords(colorShadeBox.x+colorShadeBox.w+GENERAL_GPE_GUI_PADDING,newColorGValue->get_ypos()+newColorGValue->get_height()+GENERAL_GPE_GUI_PADDING);
        newColorBValue->set_width(128);

        newColorHexValue->set_coords(colorShadeBox.x+colorShadeBox.w+GENERAL_GPE_GUI_PADDING,newColorBValue->get_ypos()+newColorBValue->get_height()+GENERAL_GPE_GUI_PADDING);
        newColorHexValue->set_width(128);

        yesButton->process_self();
        cancelButton->process_self();
        newColorRValue->process_self();
        newColorGValue->process_self();
        newColorBValue->process_self();
        newColorHexValue->process_self();
        if( newColorRValue->is_inuse() || newColorGValue->is_inuse() || newColorBValue->is_inuse() )
        {
            alteredColor->change_rgba(newColorRValue->get_held_number(),newColorGValue->get_held_number(),newColorBValue->get_held_number(),255 );
            newColorHexValue->set_string( GPE_COLOR_SYSTEM->rgb_to_hex(alteredColor->get_r(),alteredColor->get_g(),alteredColor->get_b() ) ); //Convert RGB to Hex here
        }
        else if( newColorHexValue->is_inuse() )
        {
            //convert hex to rgb is possibre
            GPE_COLOR_SYSTEM->hex_to_rgb(newColorHexValue->get_string(),rV,gV,bV);
            newColorRValue->set_number(rV);
            newColorGValue->set_number(gV);
            newColorBValue->set_number(bV);
            alteredColor->change_rgba(newColorRValue->get_held_number(),newColorGValue->get_held_number(),newColorBValue->get_held_number(),255 );
            //newColorHexValue->set_string( RGBtoHEX(currColor->get_r(),currColor->get_g(),currColor->get_b() ) ); //Convert RGB to Hex here
        }
        if( input->check_keyboard_released(kb_esc) || cancelButton->is_clicked() || GPE_MAIN_WINDOW->is_resized() )
        {
            exitOperation = true;
            operationCancelled = true;
        }
        else if( input->check_keyboard_released(kb_enter) || yesButton->is_clicked() )
        {
            exitOperation = true;
        }
        else if( input->check_mouse_down( mb_left ) )
        {
            if( point_within(input->mouse_x,input->mouse_y,
                             elementBox.x+GENERAL_GPE_GUI_PADDING,elementBox.y+032+GENERAL_GPE_GUI_PADDING,
                             elementBox.x+GPE_SURFACE_COLOR_PICKER_GRADIENT->w+GENERAL_GPE_GUI_PADDING,elementBox.y+GPE_SURFACE_COLOR_PICKER_GRADIENT->h+32+GENERAL_GPE_GUI_PADDING) )
            {
                //Gets "clicked color"
                foundColorPickerX = input->mouse_x-elementBox.x-GENERAL_GPE_GUI_PADDING;
                foundColorPickerY = input->mouse_y-elementBox.y-32-GENERAL_GPE_GUI_PADDING;
                foundPixelColor = SDL_SurfaceEx::get_pixel32(GPE_SURFACE_COLOR_PICKER_GRADIENT,foundColorPickerX, foundColorPickerY );

                SDL_GetRGBA(foundPixelColor,GPE_SURFACE_COLOR_PICKER_GRADIENT->format,&rU8,&gU8,&bU8, &uU8 );
                //sets new color to picked color
                rV = (int)rU8;
                gV = (int)gU8;
                bV = (int)bU8;
                //rV = foundPixelColor & 0xff;
                //gV =(foundPixelColor >> 8) & 0xff;
                //bV =(foundPixelColor >> 16) & 0xff;
                /*
                newColorRValue->set_number(rV);
                newColorGValue->set_number(gV);
                newColorBValue->set_number(bV);*/

                colorShadeDivision = (float)selectedColorShade/(float)colorShadeBox.h;

                newColorRValue->set_number( rV );
                newColorGValue->set_number( gV);
                newColorBValue->set_number( bV );
                alteredColor->change_rgba(newColorRValue->get_held_number(),newColorGValue->get_held_number(),newColorBValue->get_held_number(),255 );
                newColorHexValue->set_string( GPE_COLOR_SYSTEM->rgb_to_hex( alteredColor->get_r(),alteredColor->get_g(),alteredColor->get_b() ) ); //Convert RGB to Hex here
            }

        }
        else if( input->check_mouse_released( mb_left))
        {
            if( point_within_rect(input->mouse_x,input->mouse_y,&colorShadeBox) )
            {
                selectedColorShade = input->mouse_y - colorShadeBox.y;

                colorShadeDivision = (float)selectedColorShade/(float)colorShadeBox.h;

                newColorRValue->set_number( GPE_COLOR_SYSTEM->merge_channel(alteredColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                newColorGValue->set_number( GPE_COLOR_SYSTEM->merge_channel(alteredColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                newColorBValue->set_number( GPE_COLOR_SYSTEM->merge_channel(alteredColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                alteredColor->change_rgba(newColorRValue->get_held_number(),newColorGValue->get_held_number(),newColorBValue->get_held_number(),255 );
                newColorHexValue->set_string( GPE_COLOR_SYSTEM->rgb_to_hex(alteredColor->get_r(),alteredColor->get_g(),alteredColor->get_b() ) ); //Convert RGB to Hex here

            }
        }
        GPE_MAIN_RENDERER->reset_viewpoint();
        if( !GPE_MAIN_WINDOW->is_resized() )
        {
            MAIN_OVERLAY->render_frozen_screenshot( );

            //Update screen
            gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Color,false);
            gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
            if( GPE_TEXTURE_COLOR_PICKER_GRADIENT!=NULL)
            {
                GPE_TEXTURE_COLOR_PICKER_GRADIENT->render_tex( elementBox.x+GENERAL_GPE_GUI_PADDING,elementBox.y+32+GENERAL_GPE_GUI_PADDING );
            }
            int preiewColorYPos = yesButton->get_ypos()-GENERAL_GPE_GUI_PADDING-64;

            gcanvas->render_rect( &colorShadeBox,GPE_MAIN_THEME->Program_Color,false);
            gcanvas->render_rect( &colorShadeBox,GPE_MAIN_THEME->Main_Border_Color,true);

            for( colorPickerShaderI = 0; colorPickerShaderI <= colorShadeBox.h; colorPickerShaderI++)
            {
                colorShadeDivision = (float)colorPickerShaderI/(float)colorShadeBox.h;
                colorShadeTempColor->change_r( GPE_COLOR_SYSTEM->merge_channel(alteredColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_g( GPE_COLOR_SYSTEM->merge_channel(alteredColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_b( GPE_COLOR_SYSTEM->merge_channel(alteredColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                gcanvas->render_horizontal_line_color( colorShadeBox.y+colorPickerShaderI,colorShadeBox.x,colorShadeBox.x+colorShadeBox.w,colorShadeTempColor);
            }
            gcanvas->render_horizontal_line_color( colorShadeBox.y+selectedColorShade,colorShadeBox.x-4,colorShadeBox.x+colorShadeBox.w+4,GPE_MAIN_THEME->Main_Border_Color );


            if( currColor!=NULL)
            {
                preiewColorYPos = yesButton->get_ypos()-GENERAL_GPE_GUI_PADDING-40;
                gfs->render_text( elementBox.x+GENERAL_GPE_GUI_PADDING,preiewColorYPos,"Older Color:",GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP);
                gcanvas->render_rectangle( elementBox.x+128,preiewColorYPos,elementBox.x+256,preiewColorYPos+32,currColor,false);

                preiewColorYPos = yesButton->get_ypos()-GENERAL_GPE_GUI_PADDING-80;
                gfs->render_text( elementBox.x+GENERAL_GPE_GUI_PADDING,preiewColorYPos,"New Color:",GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP);
                gcanvas->render_rectangle( elementBox.x+128,preiewColorYPos,elementBox.x+256,preiewColorYPos+32,alteredColor,false);
            }
            else
            {
                preiewColorYPos = yesButton->get_ypos()-GENERAL_GPE_GUI_PADDING-40;
                gfs->render_text( elementBox.x+GENERAL_GPE_GUI_PADDING,preiewColorYPos,"New Color:",GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP);
                gcanvas->render_rectangle( elementBox.x+128,preiewColorYPos,elementBox.x+256,preiewColorYPos+32,alteredColor,false);
            }
            gcanvas->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,false);
            gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
            gfs->render_text( elementBox.x+elementBox.w/2,elementBox.y+GENERAL_GPE_GUI_PADDING,popUpCaption,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
            yesButton->render_self( );
            cancelButton->render_self( );
            newColorRValue->render_self( );
            newColorGValue->render_self( );
            newColorBValue->render_self( );
            newColorHexValue->render_self( );
        }
        gpe->end_loop();
    }

    if( yesButton!=NULL)
    {
        delete yesButton;
        yesButton = NULL;
    }
    if( cancelButton!=NULL)
    {
        delete cancelButton;
        cancelButton = NULL;
    }
    if( newColorRValue!=NULL)
    {
        delete newColorRValue;
        newColorRValue = NULL;
    }
    if( newColorGValue!=NULL)
    {
        delete newColorGValue;
        newColorGValue = NULL;
    }
    if( newColorBValue!=NULL)
    {
        delete newColorBValue;
        newColorBValue = NULL;
    }
    if( newColorHexValue!=NULL)
    {
        delete newColorHexValue;
        newColorHexValue = NULL;
    }
    if( colorShadeTempColor!=NULL)
    {
        delete colorShadeTempColor;
        colorShadeTempColor = NULL;
    }

    if( !operationCancelled)
    {
        if( alteredColor!=NULL)
        {
            currColor->change_rgba( alteredColor->get_r(),alteredColor->get_g(),alteredColor->get_b() , 255 );
            delete alteredColor;
            alteredColor = NULL;
            input->reset_all_input();
            gpe->start_loop();
            return true;
        }
    }

    if( alteredColor!=NULL)
    {
        delete alteredColor;
        alteredColor = NULL;
    }
    input->reset_all_input();
    gpe->start_loop();
    return false;
}

