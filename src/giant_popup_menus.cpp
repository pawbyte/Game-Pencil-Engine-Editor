/*
giant_popup_menus.cpp
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

#include "gpe_editor.h"

GPE_Color * GPE_Get_Color_PopUp(std::string popUpCaption ,GPE_Color * currColor)
{
    RESOURCE_TO_DRAG = NULL;
    if( (int)popUpCaption.size() ==0)
    {
        popUpCaption = "Select a Color";
    }
    MAIN_OVERLAY->take_frozen_screenshot(MAIN_RENDERER);

    int promptBoxWidth = 500;
    int promptBoxHeight = 400;
    GPE_Rect barBox;
    GPE_Rect colorShadeBox;
    colorShadeBox.w = 32;
    colorShadeBox.h = 200;

    bool exitOperation = false;
    bool operationCancelled = false;
    userInput->reset_all_input();
    GPE_Color * returnedColor = NULL, *colorShadeTempColor = NULL;
    colorShadeTempColor = new GPE_Color();
    int rV = 0,  gV = 0,  bV = 0;
    Uint8 rU8 = 0, gU8 = 0, bU8 = 0;
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

    if(currColor!=NULL)
    {
        returnedColor = new GPE_Color(currColor->get_r(),currColor->get_g(),currColor->get_b() );
        newColorRValue->set_string( int_to_string( currColor->get_r() ) );
        newColorGValue->set_string( int_to_string( currColor->get_g() ) );
        newColorBValue->set_string( int_to_string( currColor->get_b() ) );
        newColorHexValue->set_string( RGBtoHEX(currColor->get_r(),currColor->get_g(),currColor->get_b() ) ); //Convert RGB to Hex here
    }
    else
    {
        returnedColor = new GPE_Color(0,0,0);
        newColorHexValue->set_string("#000000");
    }
    GPE_ToolLabelButton * yesButton = new GPE_ToolLabelButton(0,16,"Okay","Confirm Color Change");
    GPE_ToolLabelButton * cancelButton = new GPE_ToolLabelButton(0,16,"Cancel","Cancel Color Request");

    MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
    //currentState->render();
    MAIN_RENDERER->update_renderer();
    while(exitOperation==false)
    {
        //Start the frame timer
        capTimer->start();
        //gets user input
        userInput->handle(true,true);
        GPE_MAIN_GUI->reset_gui_info();

        barBox.x = (SCREEN_WIDTH-promptBoxWidth)/2;
        barBox.y = (SCREEN_HEIGHT-promptBoxHeight)/2;
        barBox.w = promptBoxWidth;
        barBox.h = promptBoxHeight;

        colorShadeBox.x = barBox.x+GENERAL_GPE_PADDING*2+200;
        colorShadeBox.y = barBox.y+GENERAL_GPE_PADDING+32;

        GPE_MAIN_GUI->reset_gui_info();
        yesButton->set_coords( barBox.x+GENERAL_GPE_PADDING, barBox.y+barBox.h-32);
        cancelButton->set_coords( yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_PADDING,yesButton->get_ypos() );

        newColorRValue->set_coords(colorShadeBox.x+colorShadeBox.w+GENERAL_GPE_PADDING,barBox.y+GENERAL_GPE_PADDING+48);
        newColorRValue->set_width(128);

        newColorGValue->set_coords(colorShadeBox.x+colorShadeBox.w+GENERAL_GPE_PADDING,newColorRValue->get_ypos()+newColorRValue->get_height()+GENERAL_GPE_PADDING);
        newColorGValue->set_width(128);

        newColorBValue->set_coords(colorShadeBox.x+colorShadeBox.w+GENERAL_GPE_PADDING,newColorGValue->get_ypos()+newColorGValue->get_height()+GENERAL_GPE_PADDING);
        newColorBValue->set_width(128);

        newColorHexValue->set_coords(colorShadeBox.x+colorShadeBox.w+GENERAL_GPE_PADDING,newColorBValue->get_ypos()+newColorBValue->get_height()+GENERAL_GPE_PADDING);
        newColorHexValue->set_width(128);

        yesButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        cancelButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        newColorRValue->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        newColorGValue->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        newColorBValue->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        newColorHexValue->process_self(&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
        if( newColorRValue->is_inuse() || newColorGValue->is_inuse() || newColorBValue->is_inuse() )
        {
            returnedColor->change_rgba(newColorRValue->get_held_number(),newColorGValue->get_held_number(),newColorBValue->get_held_number(),255 );
            newColorHexValue->set_string( RGBtoHEX(returnedColor->get_r(),returnedColor->get_g(),returnedColor->get_b() ) ); //Convert RGB to Hex here
        }
        else if( newColorHexValue->is_inuse() )
        {
            //convert hex to rgb is possibre
            HEXtoRGB(newColorHexValue->get_string(),rV,gV,bV);
            newColorRValue->set_number(rV);
            newColorGValue->set_number(gV);
            newColorBValue->set_number(bV);
            returnedColor->change_rgba(newColorRValue->get_held_number(),newColorGValue->get_held_number(),newColorBValue->get_held_number(),255 );
            //newColorHexValue->set_string( RGBtoHEX(currColor->get_r(),currColor->get_g(),currColor->get_b() ) ); //Convert RGB to Hex here
        }
        if( userInput->check_keyboard_released(kb_esc) || cancelButton->is_clicked() || WINDOW_WAS_JUST_RESIZED )
        {
            exitOperation = true;
            operationCancelled = true;
        }
        else if( userInput->check_keyboard_released(kb_enter) || yesButton->is_clicked() )
        {
            exitOperation = true;
        }
        else if( userInput->check_mouse_down(0) )
        {
            if( point_within(userInput->mouse_x,userInput->mouse_y,
                             barBox.x+GENERAL_GPE_PADDING,barBox.y+032+GENERAL_GPE_PADDING,
                             barBox.x+200+GENERAL_GPE_PADDING,barBox.y+232+GENERAL_GPE_PADDING) )
            {
                //Gets "clicked color"
                foundColorPickerX = userInput->mouse_x-barBox.x-GENERAL_GPE_PADDING;
                foundColorPickerY = userInput->mouse_y-barBox.y-32-GENERAL_GPE_PADDING;
                foundPixelColor = get_pixel32(GPE_SURFACE_COLOR_PICKER_GRADIENT,foundColorPickerX , foundColorPickerY );

                SDL_GetRGB(foundPixelColor,GPE_SURFACE_COLOR_PICKER_GRADIENT->format,&rU8,&gU8,&bU8);
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

                newColorRValue->set_number( merge_channel(rV ,(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                newColorGValue->set_number( merge_channel(gV ,(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                newColorBValue->set_number( merge_channel(bV ,(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                returnedColor->change_rgba(newColorRValue->get_held_number(),newColorGValue->get_held_number(),newColorBValue->get_held_number(),255 );
                newColorHexValue->set_string( RGBtoHEX(returnedColor->get_r(),returnedColor->get_g(),returnedColor->get_b() ) ); //Convert RGB to Hex here
            }

        }
        else if( userInput->check_mouse_released(0) )
        {
            if( point_within_rect(userInput->mouse_x,userInput->mouse_y,&colorShadeBox) )
            {
                selectedColorShade = userInput->mouse_y - colorShadeBox.y;

                colorShadeDivision = (float)selectedColorShade/(float)colorShadeBox.h;

                newColorRValue->set_number( merge_channel(returnedColor->get_r() ,(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                newColorGValue->set_number( merge_channel(returnedColor->get_g() ,(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                newColorBValue->set_number( merge_channel(returnedColor->get_b() ,(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                returnedColor->change_rgba(newColorRValue->get_held_number(),newColorGValue->get_held_number(),newColorBValue->get_held_number(),255 );
                newColorHexValue->set_string( RGBtoHEX(returnedColor->get_r(),returnedColor->get_g(),returnedColor->get_b() ) ); //Convert RGB to Hex here

            }
        }

        calculate_avg_fps();
        if( !WINDOW_WAS_JUST_RESIZED)
        {
            MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);

            //Update screen
            render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false);
            render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
            if( GPE_TEXTURE_COLOR_PICKER_GRADIENT!=NULL)
            {
                render_texture_resized(MAIN_RENDERER,GPE_TEXTURE_COLOR_PICKER_GRADIENT,barBox.x+GENERAL_GPE_PADDING,barBox.y+32+GENERAL_GPE_PADDING,200,200);
            }
            int preiewColorYPos = yesButton->get_ypos()-GENERAL_GPE_PADDING-64;

            render_rect(MAIN_RENDERER,&colorShadeBox,GPE_MAIN_TEMPLATE->Program_Color,false);
            render_rect(MAIN_RENDERER,&colorShadeBox,GPE_MAIN_TEMPLATE->Main_Border_Color,true);

            for( colorPickerShaderI = 0; colorPickerShaderI <= colorShadeBox.h; colorPickerShaderI++)
            {
                colorShadeDivision = (float)colorPickerShaderI/(float)colorShadeBox.h;
                colorShadeTempColor->change_r( merge_channel(returnedColor->get_r() ,(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_g( merge_channel(returnedColor->get_g() ,(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_b( merge_channel(returnedColor->get_b() ,(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                render_horizontal_line_color(MAIN_RENDERER,colorShadeBox.y+colorPickerShaderI,colorShadeBox.x,colorShadeBox.x+colorShadeBox.w,colorShadeTempColor);
            }
            render_horizontal_line_color(MAIN_RENDERER,colorShadeBox.y+selectedColorShade,colorShadeBox.x-4,colorShadeBox.x+colorShadeBox.w+4,GPE_MAIN_TEMPLATE->Main_Border_Color );


            if( currColor!=NULL)
            {
                preiewColorYPos = yesButton->get_ypos()-GENERAL_GPE_PADDING-40;
                render_new_text(MAIN_RENDERER,barBox.x+GENERAL_GPE_PADDING,preiewColorYPos,"Older Color:",GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
                render_rectangle(MAIN_RENDERER,barBox.x+128,preiewColorYPos,barBox.x+256,preiewColorYPos+32,currColor,false);

                preiewColorYPos = yesButton->get_ypos()-GENERAL_GPE_PADDING-80;
                render_new_text(MAIN_RENDERER,barBox.x+GENERAL_GPE_PADDING,preiewColorYPos,"New Color:",GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
                render_rectangle(MAIN_RENDERER,barBox.x+128,preiewColorYPos,barBox.x+256,preiewColorYPos+32,returnedColor,false);
            }
            else
            {
                preiewColorYPos = yesButton->get_ypos()-GENERAL_GPE_PADDING-40;
                render_new_text(MAIN_RENDERER,barBox.x+GENERAL_GPE_PADDING,preiewColorYPos,"New Color:",GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
                render_rectangle(MAIN_RENDERER,barBox.x+128,preiewColorYPos,barBox.x+256,preiewColorYPos+32,returnedColor,false);
            }
            render_rectangle(MAIN_RENDERER,barBox.x,barBox.y,barBox.x+barBox.w,barBox.y+32,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color,false);
            render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
            render_new_text(MAIN_RENDERER,barBox.x+barBox.w/2,barBox.y+GENERAL_GPE_PADDING,popUpCaption,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP);
            yesButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
            cancelButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
            newColorRValue->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
            newColorGValue->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
            newColorBValue->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
            newColorHexValue->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
            //GPE_MAIN_GUI->render_tooltip(MAIN_RENDERER);
            MAIN_OVERLAY->process_cursor();
            GPE_MAIN_GUI->render_gui_info(MAIN_RENDERER, true);
            MAIN_RENDERER->update_renderer();
        }
        cap_fps();
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
    if( operationCancelled)
    {
        if( returnedColor!=NULL)
        {
            delete returnedColor;
            returnedColor = NULL;
            return NULL;
        }
    }
    else
    {
        return returnedColor;
    }
    return NULL;
}

void GPE_Show_Tip_Of_Day()
{
    RESOURCE_TO_DRAG = NULL;
    if( GPE_MAIN_GUI!=NULL && MAIN_RENDERER!=NULL)
    {
        record_error("Showing tip of the day");
        std::string popUpCaption = "Tip of the Day";
        GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
        MAIN_OVERLAY->process_cursor();
        GPE_MAIN_GUI->reset_gui_info();
        MAIN_OVERLAY->take_frozen_screenshot(MAIN_RENDERER);

        int promptBoxWidth = 544;
        int promptBoxHeight = 384;
        GPE_Rect barBox;

        bool exitOperation = false;
        userInput->reset_all_input();
        int currentTipId = GPE_MAIN_GUI->get_random_tip();
        if( currentTipId < 0 || currentTipId > GPE_MAIN_GUI->get_tip_count() )
        {
            currentTipId = 0;
        }
        std::string currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);

        GPE_Label_Title * doYouKnowLabel = new GPE_Label_Title("Do you know?","Do you know?");
        GPE_GuiElementList * showTipList = new GPE_GuiElementList();
        showTipList->hideXScroll = true;
        showTipList->hideYScroll = true;
        GPE_ToolLabelButton * closeButton = new GPE_ToolLabelButton(0,16,"Close","");
        GPE_ToolLabelButton * nextTipButton = new GPE_ToolLabelButton(0,16,"Next Tip","");
        GPE_ToolLabelButton * previousTipButton = new GPE_ToolLabelButton(0,16,"Previous Tip","");
        GPE_CheckBoxBasic * showAtStartUpButton = new GPE_CheckBoxBasic("Show tips at startup","Unclick to not see this popup automatically on start");
        showAtStartUpButton->set_clicked( GPE_MAIN_GUI->showTipsAtStartUp);
        GPE_ToolLabelButton * randomTipButton = new GPE_ToolLabelButton(0,16,"Random Tip","");
        GPE_WrappedTextArea * tipParagraph = new GPE_WrappedTextArea();
        tipParagraph->set_string(currentTipString);
        tipParagraph->set_width(512-GENERAL_GPE_PADDING*2);
        tipParagraph->set_height(96);
        SDL_RenderSetViewport(MAIN_RENDERER->get_renderer() ,NULL);
        MAIN_RENDERER->reset_viewpoint();
        //MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
        while(exitOperation==false)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            //record_error("Processing tip of the day");
            //Start the frame timer
            capTimer->start();
            //gets user input
            userInput->handle(true,true);

            barBox.x = (SCREEN_WIDTH-promptBoxWidth)/2;
            barBox.y = (SCREEN_HEIGHT-promptBoxHeight)/2;
            barBox.w = promptBoxWidth;
            barBox.h = promptBoxHeight;
            showTipList->set_coords(barBox.x, barBox.y+32);
            showTipList->set_width(barBox.w);
            showTipList->set_height(barBox.h-32);
            showTipList->barXMargin = GENERAL_GPE_PADDING;
            showTipList->barYMargin = GENERAL_GPE_PADDING;
            showTipList->barXPadding = GENERAL_GPE_PADDING*2;
            showTipList->barYPadding = GENERAL_GPE_PADDING*2;

            GPE_MAIN_GUI->reset_gui_info();
            showTipList->clear_list();
            showTipList->add_gui_element(doYouKnowLabel,true);
            showTipList->add_gui_element(tipParagraph,true);
            showTipList->add_gui_element(showAtStartUpButton, true);
            showTipList->add_gui_element(previousTipButton,false);
            showTipList->add_gui_element(nextTipButton,true);
            showTipList->add_gui_element(randomTipButton,false);
            showTipList->add_gui_element(closeButton,false);
            showTipList->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
            if( userInput->check_keyboard_released(kb_esc) || closeButton->is_clicked() || WINDOW_WAS_JUST_RESIZED )
            {
                exitOperation = true;
            }
            else if( nextTipButton->is_clicked() )
            {
                currentTipId = GPE_MAIN_GUI->get_next_tip(currentTipId);
                currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }
            else if( previousTipButton->is_clicked() )
            {
                currentTipId = GPE_MAIN_GUI->get_previous_tip(currentTipId);
                currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }
            else if( randomTipButton->is_clicked() )
            {
                currentTipId = GPE_MAIN_GUI->get_random_tip();
                currentTipString = GPE_MAIN_GUI->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }

            calculate_avg_fps();
            //record_error("Rendering tip of the day");
            MAIN_RENDERER->reset_viewpoint();
            if( !WINDOW_WAS_JUST_RESIZED)
            {
                //if( userInput->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
                }
                //Update screen
                render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Color,false);

                render_rectangle(MAIN_RENDERER,barBox.x,barBox.y,barBox.x+barBox.w,barBox.y+32,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color,false);
                render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color,true);
                render_new_text(MAIN_RENDERER,barBox.x+barBox.w/2,barBox.y+GENERAL_GPE_PADDING,popUpCaption,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color,DEFAULT_FONT,FA_CENTER,FA_TOP);
                showTipList->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
                //GPE_MAIN_GUI-render_gui_info(MAIN_RENDERER, true);

                render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
                MAIN_OVERLAY->process_cursor();
                GPE_MAIN_GUI->render_gui_info(MAIN_RENDERER, true);
                MAIN_RENDERER->update_renderer();
            }
            cap_fps();
        }
        if( showTipList!=NULL)
        {
            delete showTipList;
            showTipList = NULL;
        }

        if( doYouKnowLabel!=NULL)
        {
            delete doYouKnowLabel;
            doYouKnowLabel = NULL;
        }
        if( closeButton!=NULL)
        {
            delete closeButton;
            closeButton = NULL;
        }
        if( nextTipButton!=NULL)
        {
            delete nextTipButton;
            nextTipButton = NULL;
        }
        if( previousTipButton!=NULL)
        {
            delete previousTipButton;
            previousTipButton = NULL;
        }
        if( randomTipButton!=NULL)
        {
            delete randomTipButton;
            randomTipButton = NULL;
        }
        if( showAtStartUpButton!=NULL)
        {
            GPE_MAIN_GUI->showTipsAtStartUp = showAtStartUpButton->is_clicked();
            GPE_MAIN_GUI->save_settings();
            delete showAtStartUpButton;
            showAtStartUpButton = NULL;
        }
        if( tipParagraph!=NULL)
        {
            delete tipParagraph;
            tipParagraph = NULL;
        }
    }
    userInput->reset_all_input();
    MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
    MAIN_RENDERER->update_renderer();
}

std::string GPE_GetPlainFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory,bool isSaving, bool isDirectorySearch )
{
    RESOURCE_TO_DRAG = NULL;
    GPE_File * currentFile = NULL;
    GPE_FileDirectory * currentDirectory = new GPE_FileDirectory();

    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    record_error("[Opening file] Plainfile Function... using <"+previousDirectory+"> as previous.");
    MAIN_OVERLAY->take_frozen_screenshot();
    std::string returnFile = "";
    std::string currentDirectoryInView = "";
    std::string nextDirectoryToView = "";
    std::string soughtReturnFilename = "";
    std::string userHomePath = "";
    std::string  osFileFilterString = "All types(*.*)\0*.*\0";
    int filesAndDirectoryPassedFilterCount = 16;
    int maxCharsToView = 16;
    bool fileMatchesFilter = false;
    bool mouseIsInBrowserBox = false;
    float imagePreviewScaleSize = 1;
    char* homeDir;
    std::string iDirLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    homeDir = getenv("%UserProfile%");
    if( homeDir!=NULL)
    {
        userHomePath = currentDirectoryInView = homeDir;
    }
    else
    {
        homeDir = getenv("home");
        if( homeDir!=NULL)
        {
            userHomePath = currentDirectoryInView = homeDir;
        }
        else
        {
            homeDir = getenv("HOME");
            if( homeDir!=NULL)
            {
                userHomePath = currentDirectoryInView = homeDir;
            }
            else
            {
                homeDir = getenv("homepath");
                if( homeDir!=NULL)
                {
                    userHomePath = currentDirectoryInView = homeDir;
                }
                else
                {
                    userHomePath = currentDirectoryInView = APP_DIRECTORY_NAME;
                }
            }
        }
    }

    for( int iLetter = 0; iLetter < (int)iDirLetters.size(); iLetter++)
    {
        if( path_exists(iDirLetters.substr(iLetter,1)+":"+userHomePath) )
        {
            userHomePath = iDirLetters.substr(iLetter,1)+":"+userHomePath;
            break;
        }
        else
        {
            record_error(iDirLetters.substr(iLetter,1)+":"+userHomePath+" is not a directory.");
        }
    }
    userHomePath = string_replace_all(userHomePath,"\\","/");
    currentDirectoryInView = userHomePath;
    if( path_exists(previousDirectory) )
    {
        currentDirectoryInView = previousDirectory.c_str();
    }
    if( (int)currentDirectoryInView.size()>0 && currentDirectory!=NULL)
	{
	    MAIN_OVERLAY->take_frozen_screenshot(MAIN_RENDERER);
	    int fontSizeH = 12;
        int fontSizeW = 12;
        FONT_TEXTINPUT->get_metrics("A",&fontSizeW,&fontSizeH);
        if( fontSizeW<=0)
        {
            fontSizeW = 12;
        }
	    GPE_Texture * previewedImageTexture = new GPE_Texture();
        GPE_GuiElementList * shortCutGList = new GPE_GuiElementList();

        std::string shorterCurrentDirectoryInView = currentDirectoryInView;

        std::vector<std::string > fileFilterTypes;
        if( currentDirectoryInView.size() > 32)
        {
            shorterCurrentDirectoryInView = currentDirectoryInView.substr(0,32);
        }
        userInput->reset_all_input();

        int iFile = 0;
        int filesProcessedCount = 0;
        int iDirectory = 0;
        bool enterMyComputerMode = false;
        bool exitOperation = false;
        if( currentDirectory->open_directory_sorted(currentDirectoryInView )==-1)
        {
            exitOperation = true;
        }
        int TEXTBOX_FONT_SIZE_WIDTH = 12;
        int TEXTBOX_FONT_SIZE_HEIGHT = 12;

        int DRIVER_FONT_SIZE_WIDTH = 24;
        int DRIVER_FONT_SIZE_HEIGHT = 24;
        if( FONT_TEXTINPUT!=NULL)
        {
             FONT_TEXTINPUT->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
        }
        if( FONT_CATEGORY_BAR!=NULL)
        {
             FONT_CATEGORY_BAR->get_metrics("A",&DRIVER_FONT_SIZE_WIDTH,&DRIVER_FONT_SIZE_HEIGHT);
        }
        std::string returnVal = "";
        std::string fileToClick = "";
        std::string currentFileExt = "";
        std::string fileRenderName[3];
        fileRenderName[0] = "";
        fileRenderName[1] = "";
        fileRenderName[2] = "";
        std::vector <std::string> mountedDriversList;
        GPE_DropDown_Menu * fileTypeDropDown = new GPE_DropDown_Menu(0,0,"All types(*.*)",false);
        osFileFilterString = parse_file_types(allowedFileTypes,fileFilterTypes);
        std::string tempFileType = "";
        for( int fDT = 0; fDT < (int)fileFilterTypes.size(); fDT++)
        {
            tempFileType  = fileFilterTypes.at(fDT);
            fileTypeDropDown->add_menu_option(tempFileType,tempFileType );
        }
        fileFilterTypes.clear();

        GPE_ToolLabelButton * yesButton = NULL;
        if( isSaving)
        {
            yesButton= new GPE_ToolLabelButton(0,32,"Save","");
        }
        else
        {
            yesButton = new GPE_ToolLabelButton(0,32,"Open","");
        }

        GPE_Sprite * mainFilesSprite = NULL;
        GPE_Sprite * mainMenuSprite = NULL;


        GPE_ToolIconButtonBar * fileBrowserModeBar = new GPE_ToolIconButtonBar(0,0,32,true);
        fileBrowserModeBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/buttons/table.png","Thumbnailed View",1,false);
        fileBrowserModeBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/buttons/list-alt.png","Tiled View",1,false);
        fileBrowserModeBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/buttons/list.png","Detailed View",1,false);
        int pastFileBrowserMode = 0;
        GPE_ToolPushButton * mycomputerButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/archive.png","Computer","My Computer");
        GPE_ToolPushButton * homeButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/home.png","Home","View Home Disk Drives");
        GPE_ToolPushButton * desktopButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/desktop.png","Desktop","Opens the Desktop folder");
        GPE_ToolPushButton * documentsButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/suitcase.png","Documents","Opens the Documents folder");
        GPE_ToolPushButton * downloadsButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/download.png","Downloads","Opens the Downloads folder");
        GPE_ToolPushButton * picturesButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/image.png","Pictures","Opens the Pictures folder");
        GPE_ToolPushButton * musicButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/music.png","Music","Opens the Music folder");
        GPE_ToolPushButton * videoButtons = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/film.png","Videos","Opens the Videos folder");

        GPE_ToolPushButton * backButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/backward.png","Up Dir...","Traverses up one directory");
        GPE_ToolPushButton * appDirectoryButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/pencil.png","GPE Folder","Opens the drive to GPE IDE");
        GPE_ToolPushButton * examplesDirectoryButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/leanpub.png","Examples","Examples of Key and simple features");
        //GPE_ToolPushButton * tutorialsDirectoryButton = new GPE_ToolPushButton(0,32,APP_DIRECTORY_NAME+"resources/gfx/buttons/institution.png","Tutorials","Tutorials on using GPE");

        GPE_ToolLabelButton * cancelButton = new GPE_ToolLabelButton(0,32,"Cancel","");
        GPE_TextInputBasic * newStringBox = new GPE_TextInputBasic("");
        GPE_TextInputBasic * directoryViewBox = new GPE_TextInputBasic("Directory");
        directoryViewBox->set_string("Home");
        GPE_ToolIconButton * refreshButton = new GPE_ToolIconButton(32,32,"resources/gfx/buttons/refresh.png","Refresh");

        GPE_ScrollBar_YAxis * fileDirYScroll = new GPE_ScrollBar_YAxis();
        int maxFilesInColumn = 0;
        int maxFilesInRows = 0;
        int maxContentInView = 0;
        int iFileXPos = 0;
        int iDirectoryXPos = 0;
        int fileSelectedNumber = -1;
        int fileHoverNumber = -1;
        int fileSubImageToDraw = 0;
        int colTextWidth = 128;
        int fileRowHeight = 128;
        int colTextCharacters = colTextWidth/fontSizeW;
        if( isDirectorySearch)
        {
            newStringBox->set_label("Directory Name:");
        }
        else
        {
            newStringBox->set_label("File Name:");
        }
        newStringBox->switch_inuse(true);
        int buttonsWidth = yesButton->get_width()+cancelButton->get_width()+GENERAL_GPE_PADDING*2;

        GPE_Rect barBox;
        barBox.x = 0;
        barBox.y = 0;
        barBox.w = 512;
        barBox.h = 416;

        GPE_Rect fileBrowserBox;
        fileBrowserBox.x = 160;
        fileBrowserBox.y = 64;
        fileBrowserBox.w = 512-32;
        fileBrowserBox.h = barBox.h -384;

        GPE_Rect filePreviewBox;
        filePreviewBox.x = SCREEN_WIDTH-(barBox.x+barBox.w);
        filePreviewBox.y = 64;
        filePreviewBox.w = SCREEN_WIDTH-filePreviewBox.x;
        filePreviewBox.h = barBox.h -384;

        GPE_Rect fileShortcutTab;
        fileShortcutTab.x = 0;
        fileShortcutTab.y = 64;
        fileShortcutTab.w = 144;
        fileShortcutTab.h = barBox.h -384;

        GPE_Rect currFileRect;
        currFileRect.x = 0;
        currFileRect.y = 0;
        currFileRect.w = GENERAL_ICON_WIDTH;

        int iContentCol = 0;
        int jContentRow = 0;

        MAIN_RENDERER->reset_viewpoint();
        MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
        record_error("Attempting to open file selection menu...");
        //The lovely file selector loop
        nextDirectoryToView = currentDirectoryInView;
        #ifdef _WIN32
        #include "windows.h"
        if( !isDirectorySearch)
        {
            const int BUFSIZE = 1024;
            char buffer[BUFSIZE] = {0};
            OPENFILENAME ofns = {0};
            ofns.lStructSize = sizeof( ofns );
            ofns.lpstrFile = buffer;
            ofns.nMaxFile = BUFSIZE;
            ofns.lpstrInitialDir = currentDirectoryInView.c_str();
            ofns.lpstrTitle = prompt.c_str();
            ofns.lpstrFilter =  osFileFilterString.c_str();

            ofns.nFilterIndex = 1;
            ofns.Flags =  OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
            bool emptyFileReturned = false;
            if( isSaving)
            {
                if ( !GetSaveFileName( & ofns ) )
                {
                    emptyFileReturned = true;
                }
            }
            else if ( !GetOpenFileName( & ofns ) )
            {
                emptyFileReturned = true;
            }
            returnFile = buffer;
            if( emptyFileReturned)
            {
                returnFile = "";
            }
            exitOperation = true;
        }
        #endif

        GPE_Texture * currentFIleTexture = NULL;
        GPE_Texture * textRepFolder = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/folder-open.png");
        GPE_Texture * textRepHarddrive = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/hdd-o.png");
        GPE_Texture * textRepAudio = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/file-audio-o.png");
        GPE_Texture * textRepCode = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/file-code-o.png");
        GPE_Texture * textRepExe = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/paper-plane-o.png");
        GPE_Texture * textRepFont = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/font.png");
        GPE_Texture * textRepGPE = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/pencil-square.png");
        GPE_Texture * textRepImage = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/file-image-o.png");
        GPE_Texture * textRepOther = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/file.png");
        GPE_Texture * textRepPDF = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/file-pdf-o.png");
        GPE_Texture * textRepText = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/file-text-o.png");
        GPE_Texture * textRepVideo = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/file-video-o.png");
        GPE_Texture * textRepZip = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/file-zip-o.png");
        GPE_Color * fileIconColor = NULL;
        while(exitOperation==false)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            MAIN_RENDERER->reset_viewpoint();
            if( !point_within(userInput->mouse_x,userInput->mouse_y,
                              fileBrowserBox.x,fileBrowserBox.y,
                              fileBrowserBox.x+fileBrowserBox.w,
                              fileBrowserBox.y+fileBrowserBox.h) )
            {
                fileHoverNumber = -1;
            }

            ///
            if( (int)nextDirectoryToView.size() > 0 && path_exists(nextDirectoryToView) )
            {
                record_error("Attempting to open ["+nextDirectoryToView+"] directory...");
                if( path_exists(nextDirectoryToView)==false)
                {
                    nextDirectoryToView+="\\";
                }
                if( path_exists(nextDirectoryToView) )
                {
                    record_error("Opening... ["+nextDirectoryToView+"] directory...");

                    currentDirectoryInView = nextDirectoryToView;
                    directoryViewBox->set_string(currentDirectoryInView);
                    shorterCurrentDirectoryInView = currentDirectoryInView;
                    if( currentDirectoryInView.size() > 32)
                    {
                        shorterCurrentDirectoryInView = currentDirectoryInView.substr(0,32);
                    }
                    nextDirectoryToView = "";
                    fileHoverNumber = -1;
                    fileSelectedNumber = -1;
                    enterMyComputerMode= false;
                    iFile = 0;
                    iDirectory = 0;
                    iFileXPos = 0;
                    iDirectoryXPos = 0;
                    fileDirYScroll->fullRect.x = fileDirYScroll->contextRect.x = 0;
                    fileDirYScroll->fullRect.y = fileDirYScroll->contextRect.y = 0;
                    fileDirYScroll->fullRect.w = fileDirYScroll->contextRect.w = 0;
                    fileDirYScroll->fullRect.h = fileDirYScroll->contextRect.h = 16;
                    fileDirYScroll->process_self();
                    previewedImageTexture->change_texture(NULL);

                    parse_file_types(fileTypeDropDown->get_selected_name(),fileFilterTypes);
                    //Processes files to view
                    filesAndDirectoryPassedFilterCount = 0;

                    if( currentDirectory->open_directory_sorted( currentDirectoryInView ) ==-1)
                    {
                        exitOperation = true;
                    }
                    else
                    {
                        currentDirectory->filter_directory(isDirectorySearch, fileFilterTypes );
                        filesAndDirectoryPassedFilterCount = currentDirectory->get_count();
                        /*
                        for(iFile = 0; iFile < currentDirectory->get_count(); iFile++)
                        {
                            currentFile = currentDirectory->get_file(iFile);
                            if( currentFile!=NULL)
                            {
                                fileToClick = currentFile->get_name();
                                if( currentFile->is_directory() )
                                {
                                    filesAndDirectoryPassedFilterCount++;
                                }
                                else if( !isDirectorySearch &&  file_passes_filter(currentFile->get_type(),fileFilterTypes) )
                                {
                                    filesAndDirectoryPassedFilterCount++;
                                }
                            }
                        }
                        */
                        record_error("<"+currentDirectoryInView+"> has ["+int_to_string(filesAndDirectoryPassedFilterCount)+"/"+int_to_string(currentDirectory->get_count() )+"] amount of files/directories...");
                        fileDirYScroll->fullRect.h = fileDirYScroll->contextRect.h =  filesAndDirectoryPassedFilterCount;
                    }
                }
                else
                {
                    nextDirectoryToView = "";
                }
            }

            ///
            //Start the frame timer
            capTimer->start();
            //gets user input
            GPE_MAIN_GUI->reset_gui_info();
            userInput->handle(true,true);

            barBox.x = 0;
            barBox.y = 0;
            if( SCREEN_WIDTH > 1024)
            {
                barBox.w = SCREEN_WIDTH*2/3;
            }
            else
            {
                barBox.w = SCREEN_WIDTH-128;
            }

            //if( SCREEN_HEIGHT >= 600)
            {
                barBox.h = SCREEN_HEIGHT*4/5;
            }/*
            else
            {
                barBox.h = SCREEN_HEIGHT;
            }*/

            fileBrowserBox.w = barBox.w-16-fileBrowserBox.x;
            fileBrowserBox.h = barBox.h - 192;

            filePreviewBox.x = barBox.x+barBox.w;
            filePreviewBox.y = fileBrowserBox.y;
            filePreviewBox.w = SCREEN_WIDTH-filePreviewBox.x-(GENERAL_GPE_PADDING*2);
            filePreviewBox.h = fileBrowserBox.h;

            fileShortcutTab.h = barBox.h - fileShortcutTab.y;
            fileBrowserModeBar->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
            if( fileBrowserModeBar->is_clicked() || pastFileBrowserMode!=fileBrowserModeBar->get_tab_pos() )
            {
                iDirectoryXPos = iFileXPos = 0;
                pastFileBrowserMode=fileBrowserModeBar->get_tab_pos();
            }

            if( enterMyComputerMode ==true)
            {
                maxFilesInRows = 1;
                colTextWidth = fileBrowserBox.w-GENERAL_GPE_PADDING-GENERAL_THUMBNAIL_SIZE;
            }
            else
            {
                if( fileBrowserBox.w >TEXTBOX_FONT_SIZE_WIDTH*60 )
                {
                    maxFilesInRows = fileBrowserBox.w / (TEXTBOX_FONT_SIZE_WIDTH*60);
                    colTextWidth = (fileBrowserBox.w-maxFilesInColumn*(GENERAL_GPE_PADDING+GENERAL_ICON_WIDTH) )/maxFilesInRows;
                }
                else
                {
                    maxFilesInRows = 1;
                    colTextWidth = fileBrowserBox.w-GENERAL_GPE_PADDING;
                }
            }

            if( enterMyComputerMode ==true)
            {
                maxFilesInColumn = fileBrowserBox.h / (GENERAL_THUMBNAIL_SIZE+GENERAL_GPE_PADDING);
                maxFilesInRows = 1;
                currFileRect.w =colTextWidth = fileBrowserBox.w-32;
                currFileRect.h = GENERAL_THUMBNAIL_SIZE;
            }
            else
            {
                maxCharsToView = (fileBrowserBox.w/TEXTBOX_FONT_SIZE_WIDTH)-20;
                fileRowHeight = GENERAL_ICON_WIDTH;
                if( fileBrowserModeBar->get_tab_pos()==0 )
                {
                    currFileRect.w = colTextWidth = (GENERAL_THUMBNAIL_SIZE+GENERAL_GPE_PADDING*4);
                    maxFilesInRows = fileBrowserBox.w/currFileRect.w;
                    currFileRect.h = fileRowHeight = GENERAL_THUMBNAIL_SIZE+TEXTBOX_FONT_SIZE_HEIGHT*3+GENERAL_GPE_PADDING;
                    maxFilesInColumn = std::ceil( (float)fileBrowserBox.h/(float)currFileRect.h );
                    maxContentInView = (maxFilesInColumn)*maxFilesInRows;
                }
                else if( fileBrowserModeBar->get_tab_pos()==1 )
                {
                    currFileRect.w = colTextWidth = 40*TEXTBOX_FONT_SIZE_WIDTH+GENERAL_ICON_WIDTH;
                    maxFilesInRows = fileBrowserBox.w/currFileRect.w;
                    currFileRect.h = GENERAL_ICON_WIDTH+GENERAL_GPE_PADDING;
                    maxFilesInColumn = fileBrowserBox.h/currFileRect.h;
                    maxContentInView = (maxFilesInColumn)*maxFilesInRows;
                }
                else if( fileBrowserModeBar->get_tab_pos()==2 )
                {
                    currFileRect.w =colTextWidth = fileBrowserBox.w-32;
                    currFileRect.h = GENERAL_ICON_WIDTH+GENERAL_GPE_PADDING;
                    maxFilesInRows = 1;
                    maxFilesInColumn = fileBrowserBox.h/currFileRect.h;
                    maxContentInView = (maxFilesInColumn)*maxFilesInRows;
                }
            }


            colTextCharacters = colTextWidth/fontSizeW;
            shorterCurrentDirectoryInView = currentDirectoryInView;

            if( (int)currentDirectoryInView.size() > maxCharsToView)
            {
                shorterCurrentDirectoryInView = "/..."+currentDirectoryInView.substr(currentDirectoryInView.size()-maxCharsToView);
            }

            refreshButton->set_coords(GENERAL_GPE_PADDING,barBox.y+32);
            newStringBox->set_coords( fileBrowserBox.x+GENERAL_GPE_PADDING,fileBrowserBox.y+fileBrowserBox.h+GENERAL_GPE_PADDING);
            newStringBox->set_width(fileBrowserBox.w-128 );

            fileTypeDropDown->set_coords( fileBrowserBox.x+GENERAL_GPE_PADDING,newStringBox->get_y2pos()+GENERAL_GPE_PADDING);
            fileTypeDropDown->set_width(fileBrowserBox.w-128 );


            directoryViewBox->set_coords( barBox.x+GENERAL_GPE_PADDING+32,barBox.y+32);
            directoryViewBox->set_width(barBox.w - 192);

            fileBrowserModeBar->set_coords(directoryViewBox->get_x2pos(),directoryViewBox->get_ypos() );
            fileBrowserModeBar->set_width(128);

            backButton->set_width(128);
            mycomputerButton->set_width(128);
            homeButton->set_width(128);
            desktopButton->set_width(128);
            documentsButton->set_width(128);
            downloadsButton->set_width(128);
            picturesButton->set_width(128);
            musicButton->set_width(128);
            videoButtons->set_width(128);
            appDirectoryButton->set_width(128);
            examplesDirectoryButton->set_width(128);
            //tutorialsDirectoryButton->set_width(128);

            yesButton->set_coords(  barBox.x+barBox.w-buttonsWidth,barBox.h-48);
            cancelButton->set_coords( barBox.x+yesButton->get_x2pos()+GENERAL_GPE_PADDING,barBox.h-48);

            if( shortCutGList!=NULL)
            {
                shortCutGList->clear_list();
                shortCutGList->barXMargin = GENERAL_GPE_PADDING;
                shortCutGList->barYMargin = 0;
                shortCutGList->barXPadding = 0;
                shortCutGList->barYPadding = GENERAL_GPE_PADDING;

                shortCutGList->set_coords(fileShortcutTab.x,fileShortcutTab.y);
                shortCutGList->set_width(fileShortcutTab.w);
                shortCutGList->set_height(fileShortcutTab.h);
                /*
                Commmented out temporarily in 1.13
                //if( GPE_FOUND_OS==GPE_IDE_LINUX)
                {
                    if( MAIN_EDITOR_SETTINGS!=NULL)
                    {
                        shortCutGList->add_gui_element(MAIN_EDITOR_SETTINGS->showHiddenFilesInBrowser,true);
                    }
                }*/
                shortCutGList->add_gui_element(backButton,true);
                shortCutGList->add_gui_element(mycomputerButton,true);
                shortCutGList->add_gui_element(homeButton,true);
                shortCutGList->add_gui_element(desktopButton,true);
                shortCutGList->add_gui_element(documentsButton,true);
                shortCutGList->add_gui_element(downloadsButton,true);
                shortCutGList->add_gui_element(picturesButton,true);
                shortCutGList->add_gui_element(musicButton,true);
                shortCutGList->add_gui_element(videoButtons,true);
                shortCutGList->add_gui_element(appDirectoryButton,true);
                shortCutGList->add_gui_element(examplesDirectoryButton,true);
                //shortCutGList->add_gui_element(tutorialsDirectoryButton,true);
                //shortCutGList->add_gui_element(musicButton);
                shortCutGList->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
            }

            refreshButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
            yesButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);

            cancelButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);

            newStringBox->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
            if( !isDirectorySearch)
            {
                fileTypeDropDown->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
            }
            if( fileTypeDropDown->just_activated() )
            {
                nextDirectoryToView = currentDirectoryInView;
            }
            directoryViewBox->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);

            if( directoryViewBox->is_inuse() ==false)
            {
                if( directoryViewBox->get_string() !=currentDirectoryInView )
                {
                    if( path_exists(directoryViewBox->get_string() ) )
                    {
                        nextDirectoryToView  = directoryViewBox->get_string();
                    }
                }
            }

            if( backButton->is_clicked() )
            {
                nextDirectoryToView = get_path_from_file(currentDirectoryInView);
                if( nextDirectoryToView.size()<=1 )
                {
                    nextDirectoryToView = "";
                }
            }

            if( newStringBox->is_inuse()==false && directoryViewBox->is_inuse()==false )
            {
                if( userInput->check_keyboard_released(kb_backspace) )
                {
                    nextDirectoryToView = get_path_from_file(currentDirectoryInView);
                    if( nextDirectoryToView.size()<=1 )
                    {
                        nextDirectoryToView = "";
                    }
                }
            }
            if( homeButton->is_clicked() )
            {
                nextDirectoryToView = userHomePath;
            }

            if( desktopButton->is_clicked() )
            {
                nextDirectoryToView = userHomePath+"/Desktop";
                if( path_exists( nextDirectoryToView)==false)
                {
                    nextDirectoryToView = userHomePath+"/desktop";
                    if( path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/MyDesktop";
                        if( path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/My Desktop";
                        }
                        else
                        {
                            nextDirectoryToView = "";
                        }
                    }
                }
            }

            if( downloadsButton->is_clicked() )
            {
                nextDirectoryToView = userHomePath+"/Downloads";
                if( path_exists( nextDirectoryToView)==false)
                {
                    nextDirectoryToView = userHomePath+"/downloads";
                    if( path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/MyDownloads";
                        if( path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/My Downloads";
                        }
                        else
                        {
                            nextDirectoryToView = "";
                        }
                    }
                }
            }

            if( picturesButton->is_clicked() )
            {
                nextDirectoryToView = userHomePath+"/Pictures";
                if( path_exists( nextDirectoryToView)==false)
                {
                    nextDirectoryToView = userHomePath+"/pictures";
                    if( path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/MyPictures";
                        if( path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/My Pictures";
                        }
                        else
                        {
                            nextDirectoryToView = "";
                        }
                    }
                }
            }

            if( musicButton->is_clicked() )
            {
                nextDirectoryToView = userHomePath+"/Music";
                if( path_exists( nextDirectoryToView)==false)
                {
                    nextDirectoryToView = userHomePath+"/music";
                    if( path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/MyMusic";
                        if( path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/My Music";
                        }
                        else
                        {
                            nextDirectoryToView = "";
                        }
                    }
                }
            }

            if( videoButtons->is_clicked() )
            {
                nextDirectoryToView = userHomePath+"/Videos";
                if( path_exists( nextDirectoryToView)==false)
                {
                    nextDirectoryToView = userHomePath+"/videos";
                    if( path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/MyVideos";
                        if( path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/My Videos";
                        }
                        else
                        {
                            nextDirectoryToView = "";
                        }
                    }
                }
            }

            if( enterMyComputerMode==false && refreshButton->is_clicked() )
            {
                nextDirectoryToView = currentDirectoryInView;
            }

            if( mycomputerButton->is_clicked() || ( enterMyComputerMode==true && refreshButton->is_clicked()) )
            {
                mountedDriversList.clear();
                nextDirectoryToView = "/proc/mounts";
                if( file_exists( nextDirectoryToView) && GPE_FOUND_OS==GPE_IDE_LINUX )
                {
                    enterMyComputerMode = true;
                }
                else if( GPE_FOUND_OS==GPE_IDE_WINDOWS)
                {
                    enterMyComputerMode = true;
                    #ifdef _WIN32

                    DWORD dwSize = MAX_PATH;
                    char szLogicalDrives[MAX_PATH] = {0};
                    DWORD dwResult = GetLogicalDriveStrings(dwSize,szLogicalDrives);

                    if (dwResult > 0 && dwResult <= MAX_PATH)
                    {
                        char* szSingleDrive = szLogicalDrives;
                        while(*szSingleDrive)
                        {
                            mountedDriversList.push_back(szSingleDrive);

                              // get the next drive
                            szSingleDrive += strlen(szSingleDrive) + 1;
                        }
                    }
                    #endif
                }
                nextDirectoryToView = "";
                directoryViewBox->set_string("My Computer");
            }

            if( documentsButton->is_clicked() )
            {
                nextDirectoryToView = userHomePath+"/Documents";
                if( path_exists( nextDirectoryToView)==false)
                {
                    nextDirectoryToView = userHomePath+"/documents";
                    if( path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/MyDocuments";
                        if( path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/My Documents";
                        }
                    }
                }
            }

            if( appDirectoryButton->is_clicked() )
            {
                nextDirectoryToView = get_path_from_file(APP_DIRECTORY_NAME);
            }

            if( examplesDirectoryButton->is_clicked() )
            {
                nextDirectoryToView = APP_DIRECTORY_NAME+"examples";
            }

            /*if( tutorialsDirectoryButton->is_clicked() )
            {
                nextDirectoryToView = APP_DIRECTORY_NAME+"tutorials";
            }*/

            if( userInput->check_keyboard_released(kb_esc) || cancelButton->is_clicked() )
            {
                exitOperation = true;
                returnVal = "";
            }
            else if( userInput->check_keyboard_released(kb_enter) || yesButton->is_clicked() )
            {
                if( newStringBox!=NULL)
                {
                    soughtReturnFilename = currentDirectoryInView+"/"+newStringBox->get_string();
                    if( isDirectorySearch )
                    {
                        if( path_exists(soughtReturnFilename) )
                        {
                            exitOperation = true;
                            returnFile = soughtReturnFilename;
                        }
                        else
                        {
                            soughtReturnFilename = currentDirectoryInView;
                            if( path_exists(soughtReturnFilename) )
                            {
                                exitOperation = true;
                                returnFile = soughtReturnFilename;
                            }
                        }
                    }
                    else if( isSaving && (int)newStringBox->get_string().size()>0 )
                    {
                        if( file_exists(soughtReturnFilename) )
                        {
                            if( display_get_prompt("File already exists","Do you wish to overwrite this file?")==DISPLAY_QUERY_YES )
                            {
                                exitOperation = true;
                                returnFile = soughtReturnFilename;
                            }
                        }
                        else
                        {
                            exitOperation = true;
                            returnFile = soughtReturnFilename;
                        }
                    }
                    else if( file_exists(soughtReturnFilename) )
                    {
                        exitOperation = true;
                        returnFile = soughtReturnFilename;
                    }
                }
            }

            if( enterMyComputerMode)
            {
                if( (int)mountedDriversList.size() > maxContentInView)
                {
                    fileDirYScroll->barBox.x = fileBrowserBox.x+fileBrowserBox.w-16;
                    fileDirYScroll->barBox.y = fileBrowserBox.y;
                    fileDirYScroll->barBox.w = 16;
                    fileDirYScroll->barBox.h = fileBrowserBox.h;

                    fileDirYScroll->fullRect.x = 0;
                    fileDirYScroll->fullRect.y = 0;
                    fileDirYScroll->fullRect.w = 0;
                    fileDirYScroll->fullRect.h = (int)mountedDriversList.size();
                    //if( directoryViewBox->is_inuse()==false && newStringBox->is_inuse()==false && shortCutGList->hasScrollControl==false)
                    //{
                    if( point_between_rect(userInput->mouse_x,userInput->mouse_y,&fileBrowserBox ) )
                    {
                        if( userInput->mouseScrollingUp || userInput->check_keyboard_down(kb_page_up))
                        {
                            iDirectoryXPos-=maxContentInView/2;
                        }
                        else if( userInput->mouseScrollingDown || userInput->check_keyboard_down(kb_page_down) )
                        {
                            iDirectoryXPos+=maxContentInView/2;
                        }
                    }
                    fileDirYScroll->contextRect.x = 0;
                    fileDirYScroll->contextRect.y = iDirectoryXPos;
                    fileDirYScroll->contextRect.w = 0;
                    fileDirYScroll->contextRect.h = maxContentInView;

                    fileDirYScroll->process_self();
                    if( fileDirYScroll->has_moved() )
                    {
                        iDirectoryXPos = ceil( ( (float)mountedDriversList.size() ) * ( (float)fileDirYScroll->scrollYPos/(float)fileDirYScroll->barBox.h ) );
                    }
                }
                else
                {
                    fileDirYScroll->contextRect.x =  fileDirYScroll->fullRect.x = 0;
                    fileDirYScroll->contextRect.y =  fileDirYScroll->fullRect.y = 0;
                    fileDirYScroll->contextRect.w =  fileDirYScroll->fullRect.w = 32;
                    fileDirYScroll->contextRect.h =  fileDirYScroll->fullRect.h = (int)mountedDriversList.size();
                    fileDirYScroll->process_self();
                }
            }
            else
            {
                if( filesAndDirectoryPassedFilterCount > maxContentInView)
                {
                    fileDirYScroll->barBox.x = fileBrowserBox.x+fileBrowserBox.w-16;
                    fileDirYScroll->barBox.y = fileBrowserBox.y;
                    fileDirYScroll->barBox.w = 16;
                    fileDirYScroll->barBox.h = fileBrowserBox.h;

                    fileDirYScroll->fullRect.x = 0;
                    fileDirYScroll->fullRect.y = 0;
                    fileDirYScroll->fullRect.w = 0;
                    fileDirYScroll->fullRect.h = filesAndDirectoryPassedFilterCount;

                    /*if( maxContentInView >8)
                    {

                    }
                    scrollAmount = */
                    //if( directoryViewBox->is_inuse()==false && newStringBox->is_inuse()==false && shortCutGList->hasScrollControl==false)
                    if( point_between_rect(userInput->mouse_x,userInput->mouse_y,&fileBrowserBox ) )
                    {

                        if( userInput->mouseScrollingUp)
                        {
                            iFileXPos-=maxFilesInRows;
                        }
                        else if( userInput->mouseScrollingDown)
                        {
                            iFileXPos+=maxFilesInRows;
                        }
                    }
                    if( directoryViewBox->is_inuse()==false && newStringBox->is_inuse()==false && shortCutGList->hasScrollControl==false )
                    {
                        if( userInput->check_keyboard_down(kb_up) )
                        {
                            iFileXPos-=maxFilesInRows;
                        }
                        else if( userInput->check_keyboard_down(kb_down) )
                        {
                            iFileXPos+=maxFilesInRows;
                        }
                    }

                    fileDirYScroll->contextRect.x = 0;
                    fileDirYScroll->contextRect.y = iFileXPos;
                    fileDirYScroll->contextRect.w = 0;
                    fileDirYScroll->contextRect.h = maxContentInView;
                    fileDirYScroll->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);

                    if( fileDirYScroll->has_moved() )
                    {
                        iFileXPos = ceil( ( (float)filesAndDirectoryPassedFilterCount ) * ( (float)fileDirYScroll->scrollYPos/(float)fileDirYScroll->barBox.h ) );
                    }
                }
                else
                {
                    fileDirYScroll->contextRect.x =  fileDirYScroll->fullRect.x = 0;
                    fileDirYScroll->contextRect.y =  fileDirYScroll->fullRect.y = 0;
                    fileDirYScroll->contextRect.w =  fileDirYScroll->fullRect.w = 32;
                    fileDirYScroll->contextRect.h =  fileDirYScroll->fullRect.h = filesAndDirectoryPassedFilterCount;
                    fileDirYScroll->process_self();
                }
            }


            calculate_avg_fps();
            if( !WINDOW_WAS_JUST_RESIZED)
            {
                //if( userInput->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot(MAIN_RENDERER);
                    //currentState->render();
                    //MAIN_RENDERER->update_renderer();
                }
                //Update screen
                render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->Program_Header_Color,false);

                render_rectangle(MAIN_RENDERER,barBox.x,barBox.y,barBox.x+barBox.w,barBox.y+32,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Color,false);

                render_new_text(MAIN_RENDERER,barBox.x+barBox.w/2,barBox.y+16,prompt,GPE_MAIN_TEMPLATE->PopUp_Box_Highlight_Font_Color,DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
                if( enterMyComputerMode)
                {
                    //render_new_text(MAIN_RENDERER,barBox.x+32,barBox.y+48,"My Computer",GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_MIDDLE);
                }
                else
                {
                    //render_new_text(MAIN_RENDERER,barBox.x+32,barBox.y+48,"Viewing "+shorterCurrentDirectoryInView,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_MIDDLE);
                }

                render_rect(MAIN_RENDERER,&fileShortcutTab,GPE_MAIN_TEMPLATE->Text_Box_Color,false);
                render_rect(MAIN_RENDERER,&fileShortcutTab,GPE_MAIN_TEMPLATE->Text_Box_Highlight_Color,true);

                render_rect(MAIN_RENDERER,&fileBrowserBox,GPE_MAIN_TEMPLATE->Text_Box_Color,false);
                render_rect(MAIN_RENDERER,&fileBrowserBox,GPE_MAIN_TEMPLATE->Text_Box_Highlight_Color,true);

                iContentCol = 0;
                jContentRow = 0;
                MAIN_RENDERER->set_viewpoint( &fileBrowserBox);
                mouseIsInBrowserBox = point_between_rect(userInput->mouse_x,userInput->mouse_y,&fileBrowserBox);

                if( enterMyComputerMode==true)
                {
                    //display my computer directory
                    for (iDirectory = iDirectoryXPos; iDirectory < (int)mountedDriversList.size() && iDirectory < iDirectoryXPos+maxContentInView; iDirectory++)
                    {
                        currFileRect.x = fileBrowserBox.x+GENERAL_GPE_PADDING;
                        currFileRect.y = fileBrowserBox.y+jContentRow*(GENERAL_THUMBNAIL_SIZE+GENERAL_GPE_PADDING);

                        if( mouseIsInBrowserBox && point_between_rect(userInput->mouse_x,userInput->mouse_y,&currFileRect) )
                        {
                            fileHoverNumber = iDirectory;
                            if( userInput->check_mouse_doubleclicked(0) )
                            {
                                nextDirectoryToView = mountedDriversList[iDirectory];
                                fileHoverNumber = -1;
                                fileSelectedNumber = -1;
                            }
                            else if( userInput->check_mouse_released(0) )
                            {
                                fileSelectedNumber = iDirectory;
                                fileHoverNumber = -1;
                            }
                        }

                        currFileRect.x = GENERAL_GPE_PADDING;
                        currFileRect.y = jContentRow*(GENERAL_THUMBNAIL_SIZE+GENERAL_GPE_PADDING);

                        if( fileSelectedNumber ==iDirectory)
                        {
                            render_rect(MAIN_RENDERER,&currFileRect,GPE_MAIN_TEMPLATE->Main_Box_Font_Highlight_Color,false);
                        }
                        else if( fileHoverNumber ==iDirectory)
                        {
                            render_rect(MAIN_RENDERER,&currFileRect,GPE_MAIN_TEMPLATE->Main_Border_Color,false);
                        }
                        render_texture_resized(MAIN_RENDERER,textRepHarddrive,currFileRect.x,currFileRect.y,GENERAL_THUMBNAIL_SIZE,GENERAL_THUMBNAIL_SIZE,NULL,NULL,FA_LEFT, FA_TOP, GPE_MAIN_TEMPLATE->Main_Folder_Color );
                        render_new_text(MAIN_RENDERER,currFileRect.x+GENERAL_THUMBNAIL_SIZE+GENERAL_GPE_PADDING,currFileRect.y+GENERAL_THUMBNAIL_SIZE/2,mountedDriversList[iDirectory],GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_CATEGORY_BAR,FA_LEFT,FA_MIDDLE);

                        jContentRow += 1;
                    }
                }
                else
                {
                    filesProcessedCount = 0;
                    for(iFile = iFileXPos;filesProcessedCount < maxContentInView && iFile < currentDirectory->get_count(); iFile++)
                    {
                        currentFile = currentDirectory->get_file(iFile);
                        if( currentFile!=NULL)
                        {
                            fileToClick = currentFile->get_name();
                            if( currentFile->is_directory() )
                            {
                                fileMatchesFilter = true;
                            }
                            else if( !isDirectorySearch )
                            {
                                currentFileExt = currentFile->get_type();
                                fileMatchesFilter = file_passes_filter(currentFileExt,fileFilterTypes);
                            }
                            else
                            {
                                fileMatchesFilter = false;
                            }
                            if( fileMatchesFilter)
                            {
                                currFileRect.x = fileBrowserBox.x+iContentCol*(currFileRect.w);
                                currFileRect.y = fileBrowserBox.y+jContentRow*(currFileRect.h);
                                if( mouseIsInBrowserBox && point_between_rect(userInput->mouse_x,userInput->mouse_y,&currFileRect) )
                                {
                                    fileHoverNumber = iFile;
                                    if( userInput->check_mouse_doubleclicked(0) )
                                    {
                                        if( currentFile->is_directory() )
                                        {
                                            nextDirectoryToView = currentDirectoryInView+"/"+fileToClick;
                                        }
                                        else
                                        {
                                            soughtReturnFilename = currentDirectoryInView+"/"+fileToClick;
                                            fileMatchesFilter = file_passes_filter(get_file_ext(fileToClick),fileFilterTypes);
                                            if( fileMatchesFilter)
                                            {
                                                if( isSaving)
                                                {
                                                    if( file_exists(soughtReturnFilename) )
                                                    {
                                                        if( display_get_prompt("File already exists","Do you wish to overwrite this file?")==DISPLAY_QUERY_YES )
                                                        {
                                                            exitOperation = true;
                                                            returnFile = soughtReturnFilename;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        exitOperation = true;
                                                        returnFile = soughtReturnFilename;
                                                    }
                                                }
                                                else
                                                {
                                                    if( file_exists(soughtReturnFilename) )
                                                    {
                                                        exitOperation = true;
                                                        returnFile = soughtReturnFilename;
                                                    }
                                                }
                                            }
                                        }
                                        fileHoverNumber = -1;
                                        fileSelectedNumber = -1;
                                    }
                                    else if( userInput->check_mouse_released(0) )
                                    {
                                        fileSelectedNumber = iFile;
                                        fileHoverNumber = -1;
                                        if( !currentFile->is_directory() )
                                        {
                                            newStringBox->set_string(fileToClick);
                                            if( previewedImageTexture!=NULL )
                                            {
                                                if( fileMatchesFilter)
                                                {
                                                    if( currentFileExt=="bmp" || currentFileExt=="gif" || currentFileExt=="jpg" || currentFileExt=="jpeg"  || currentFileExt=="png" )
                                                    {
                                                        previewedImageTexture->load_new_texture(MAIN_RENDERER,currentDirectoryInView+"/"+fileToClick);
                                                        if( previewedImageTexture!=NULL)
                                                        {
                                                            if( previewedImageTexture->get_width() < 1 || previewedImageTexture->get_height() < 1 || previewedImageTexture->get_width() > 4096 || previewedImageTexture->get_height() > 4096)
                                                            {
                                                                previewedImageTexture->change_texture(NULL);
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        previewedImageTexture->change_texture(NULL);
                                                    }
                                                }
                                            }
                                            else if(previewedImageTexture!=NULL)
                                            {
                                                previewedImageTexture->change_texture(NULL);
                                            }
                                        }
                                        else if( isDirectorySearch)
                                        {
                                            newStringBox->set_string(fileToClick);
                                        }
                                    }
                                }
                                fileRenderName[0] = "";
                                fileRenderName[1] = "";
                                fileRenderName[2] = "";
                                //Renders file
                                currFileRect.x = iContentCol*(currFileRect.w);
                                currFileRect.y = jContentRow*(currFileRect.h);
                                if( fileSelectedNumber ==iFile)
                                {
                                    render_rect(MAIN_RENDERER,&currFileRect,GPE_MAIN_TEMPLATE->Button_Box_Selected_Color,false);
                                }
                                else if( fileHoverNumber ==iFile)
                                {
                                    render_rect(MAIN_RENDERER,&currFileRect,GPE_MAIN_TEMPLATE->Button_Box_Highlighted_Color,false);
                                }
                                currentFIleTexture = NULL;
                                if( currentFile->is_directory() )
                                {
                                    fileSubImageToDraw = 1;
                                    //mainFilesSprite
                                    if( fileSelectedNumber ==iFile)
                                    {
                                        fileIconColor = GPE_MAIN_TEMPLATE->Main_Folder_Highlighted_Color;
                                    }
                                    else
                                    {
                                        fileIconColor = GPE_MAIN_TEMPLATE->Main_Folder_Color;
                                    }
                                    currentFIleTexture = textRepFolder;
                                }
                                else
                                {
                                    fileToClick = currentFile->get_name();
                                    currentFileExt =currentFile->get_type();
                                    fileMatchesFilter = file_passes_filter(currentFileExt,fileFilterTypes);

                                    fileSubImageToDraw = 0;

                                    if( currentFileExt=="txt")
                                    {
                                        currentFIleTexture = textRepText;
                                    }
                                    else if( currentFileExt=="eot" || currentFileExt=="ttf" || currentFileExt=="woff")
                                    {
                                        currentFIleTexture = textRepFont;
                                    }
                                    else if( currentFileExt=="bmp" || currentFileExt=="gif" || currentFileExt=="jpg" || currentFileExt=="jpeg"  || currentFileExt=="png" )
                                    {
                                        currentFIleTexture = textRepImage;
                                    }
                                    else if( currentFileExt=="aac" || currentFileExt=="mp3" || currentFileExt=="ogg")
                                    {
                                        currentFIleTexture = textRepAudio;
                                    }
                                    else if( currentFileExt=="avi" || currentFileExt=="mp4" || currentFileExt=="webm")
                                    {
                                        currentFIleTexture = textRepVideo;
                                    }
                                    else if( currentFileExt=="c" || currentFileExt=="css" || currentFileExt=="h" || currentFileExt=="html" || currentFileExt=="js"  || currentFileExt=="php")
                                    {
                                        currentFIleTexture = textRepCode;
                                    }
                                    else if( currentFileExt=="gppf")
                                    {
                                        currentFIleTexture = textRepGPE;
                                    }
                                    else if( currentFileExt=="exe")
                                    {
                                        currentFIleTexture = textRepExe;
                                    }
                                    else
                                    {
                                        currentFIleTexture = textRepOther;
                                    }

                                    if( fileSelectedNumber ==iFile)
                                    {
                                       fileIconColor = GPE_MAIN_TEMPLATE->Main_Box_Font_Highlight_Color;
                                    }
                                    else
                                    {
                                       fileIconColor  = GPE_MAIN_TEMPLATE->Main_Box_Font_Color;
                                    }
                                }

                                fileRenderName[0] = "";
                                fileRenderName[1] = "";
                                fileRenderName[2] = "";
                                if( (int)fileToClick.size() >=colTextCharacters)
                                {
                                    fileRenderName[0] = get_substring(fileToClick,0,colTextCharacters);
                                    fileRenderName[1] = get_substring(fileToClick,colTextCharacters,colTextCharacters);

                                    if( (int)fileToClick.size() >colTextCharacters*3)
                                    {
                                        fileRenderName[2] = get_substring(fileToClick,colTextCharacters*2,colTextCharacters-3)+"...";
                                    }
                                    else
                                    {
                                        fileRenderName[2] = get_substring(fileToClick,colTextCharacters*2,colTextCharacters);
                                    }
                                    fileToClick = fileRenderName[2];
                                }
                                else
                                {
                                    fileRenderName[0] = fileToClick;
                                }
                                //Renders Thumbnails
                                if( fileBrowserModeBar->get_tab_pos()==0 )
                                {
                                    if( currentFile->fileThumbnail!=NULL )
                                    {
                                        render_texture_resized(MAIN_RENDERER,currentFile->fileThumbnail,
                                                               currFileRect.x+(currFileRect.w-GENERAL_THUMBNAIL_SIZE)/2,currFileRect.y,GENERAL_THUMBNAIL_SIZE,GENERAL_THUMBNAIL_SIZE);
                                    }
                                    else if( currentFIleTexture!=NULL)
                                    {
                                        render_texture_resized(MAIN_RENDERER,currentFIleTexture,currFileRect.x+(currFileRect.w-GENERAL_THUMBNAIL_SIZE)/2,currFileRect.y,GENERAL_THUMBNAIL_SIZE,GENERAL_THUMBNAIL_SIZE,NULL,NULL,FA_LEFT,FA_TOP,fileIconColor );
                                    }
                                    render_new_text(MAIN_RENDERER,currFileRect.x+currFileRect.w/2,currFileRect.y+GENERAL_THUMBNAIL_SIZE,fileRenderName[0],GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_TEXTINPUT,FA_CENTER,FA_TOP);
                                    render_new_text(MAIN_RENDERER,currFileRect.x+currFileRect.w/2,currFileRect.y+GENERAL_THUMBNAIL_SIZE+TEXTBOX_FONT_SIZE_HEIGHT,fileRenderName[1],GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_TEXTINPUT,FA_CENTER,FA_TOP);
                                    render_new_text(MAIN_RENDERER,currFileRect.x+currFileRect.w/2,currFileRect.y+GENERAL_THUMBNAIL_SIZE+TEXTBOX_FONT_SIZE_HEIGHT*2,fileRenderName[2],GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_TEXTINPUT,FA_CENTER,FA_TOP);
                                }
                                else
                                {
                                    //Renders listed icon
                                    if( currentFIleTexture!=NULL)
                                    {
                                        render_texture_resized(MAIN_RENDERER,currentFIleTexture,
                                                               currFileRect.x,currFileRect.y+(currFileRect.h-GENERAL_ICON_WIDTH)/2,
                                                               GENERAL_ICON_WIDTH,GENERAL_ICON_WIDTH,NULL,NULL,FA_LEFT,FA_TOP,fileIconColor );
                                    }
                                    render_new_text(MAIN_RENDERER,currFileRect.x+GENERAL_ICON_WIDTH,currFileRect.y+currFileRect.h/2,fileRenderName[0],GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE );
                                }

                                filesProcessedCount++;
                                iContentCol++;
                                if( iContentCol >= maxFilesInRows || fileBrowserModeBar->get_tab_pos()==2 )
                                {
                                    iContentCol = 0;
                                    jContentRow += 1;
                                }
                            }
                        }
                        //tinydir_next(&dir);
                    }
                }
                MAIN_RENDERER->reset_viewpoint();
                if( filesAndDirectoryPassedFilterCount > maxContentInView)
                {
                    fileDirYScroll->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
                }
                /*
                appDirectoryButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
                backButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
                desktopButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
                documentsButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
                downloadsButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
                homeButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
                musicButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
                mycomputerButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);*/
                shortCutGList->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);

                refreshButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);

                yesButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
                cancelButton->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
                directoryViewBox->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA,true);
                fileBrowserModeBar->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA,true);
                newStringBox->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA,true);
                if( !isDirectorySearch)
                {
                    fileTypeDropDown->render_self(MAIN_RENDERER,&GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA,true);
                }
                if( previewedImageTexture!=NULL && enterMyComputerMode==false && filePreviewBox.w!=0 && filePreviewBox.h!=0)
                {
                    if( filePreviewBox.w > previewedImageTexture->get_width()&& filePreviewBox.h> previewedImageTexture->get_height() )
                    {
                        previewedImageTexture->render_tex(MAIN_RENDERER,filePreviewBox.x,filePreviewBox.y);
                    }
                    else
                    {
                        imagePreviewScaleSize = std::max((float)previewedImageTexture->get_width()/ (float)(filePreviewBox.w), (float)previewedImageTexture->get_height()/(float)(filePreviewBox.h) );
                        if( imagePreviewScaleSize > 1)
                        {
                            previewedImageTexture->render_tex_resized(MAIN_RENDERER,filePreviewBox.x,filePreviewBox.y,(float)previewedImageTexture->get_width()/imagePreviewScaleSize,(float)previewedImageTexture->get_height()/imagePreviewScaleSize);
                        }
                        else
                        {
                            previewedImageTexture->render_tex_resized(MAIN_RENDERER,filePreviewBox.x,filePreviewBox.y,previewedImageTexture->get_width()*imagePreviewScaleSize,previewedImageTexture->get_height()*imagePreviewScaleSize);
                        }
                    }
                }
                render_rect(MAIN_RENDERER,&barBox,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
                MAIN_OVERLAY->process_cursor();
                GPE_MAIN_GUI->render_gui_info(MAIN_RENDERER, true);
                MAIN_RENDERER->update_renderer();
            }
        }

        //tinydir_close(&dir);
        if( currentDirectory!=NULL)
        {
            delete currentDirectory;
            currentDirectory = NULL;
        }

        if( refreshButton!=NULL)
        {
            delete refreshButton;
            refreshButton = NULL;
        }
        if( appDirectoryButton!=NULL)
        {
            delete appDirectoryButton;
            appDirectoryButton = NULL;
        }
        if( examplesDirectoryButton!=NULL)
        {
            delete examplesDirectoryButton;
            examplesDirectoryButton = NULL;
        }
        /*if( tutorialsDirectoryButton!=NULL)
        {
            delete tutorialsDirectoryButton;
            tutorialsDirectoryButton = NULL;
        }*/
        if( backButton!=NULL)
        {
            delete backButton;
            backButton = NULL;
        }
        if( desktopButton!=NULL)
        {
            delete desktopButton;
            desktopButton = NULL;
        }
        if( documentsButton!=NULL)
        {
            delete documentsButton;
            documentsButton = NULL;
        }
        if( downloadsButton!=NULL)
        {
            delete downloadsButton;
            downloadsButton = NULL;
        }
        if( picturesButton!=NULL)
        {
            delete picturesButton;
            picturesButton = NULL;
        }
         if( musicButton!=NULL)
        {
            delete musicButton;
            musicButton = NULL;
        }
        if( homeButton!=NULL)
        {
            delete homeButton;
            homeButton = NULL;
        }
        if( mycomputerButton!=NULL)
        {
            delete mycomputerButton;
            mycomputerButton = NULL;
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
        if( newStringBox!=NULL)
        {
            delete newStringBox;
            newStringBox = NULL;
        }
        if( fileTypeDropDown!=NULL)
        {
            delete fileTypeDropDown;
            fileTypeDropDown = NULL;
        }
        if( directoryViewBox!=NULL)
        {
            delete directoryViewBox;
            directoryViewBox = NULL;
        }
        if( fileBrowserModeBar!=NULL)
        {
            delete fileBrowserModeBar;
            fileBrowserModeBar = NULL;
        }
        if( fileDirYScroll!=NULL)
        {
            delete fileDirYScroll;
            fileDirYScroll = NULL;
        }
        if( previewedImageTexture!=NULL)
        {
            delete previewedImageTexture;
            previewedImageTexture = NULL;
        }
        if( shortCutGList!=NULL)
        {
            delete shortCutGList;
            shortCutGList = NULL;
        }
        userInput->reset_all_input();
        previousDirectory = currentDirectoryInView;
        return returnFile;
    }
    userInput->reset_all_input();
	return returnFile;
}

std::string  GPE_GetOpenFileName( const std::string & prompt, std::string allowedFileTypes , std::string &previousDirectory)
{
    return GPE_GetPlainFileName( prompt, allowedFileTypes,previousDirectory,false);
}

std::string GPE_GetSaveFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory )
{
    return GPE_GetPlainFileName( prompt, allowedFileTypes,previousDirectory,true);
}

std::string  GPE_GetDirectoryName( const std::string & prompt, std::string &previousDirectory, bool isSaving )
{
    return GPE_GetPlainFileName(prompt,"",previousDirectory,isSaving, true);
}

