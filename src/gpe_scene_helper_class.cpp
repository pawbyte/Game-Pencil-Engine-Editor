/*
gpe_scene_helper_class.cpp
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
#include "gpe_scene_helper_class.h"

GPE_SceneEditorHelper * spm = NULL;

gameScenePopupCategories::gameScenePopupCategories( std::string cName )
{
    name = cName;
    categoryLabel = new GPE_Label_Title( name, name );
    categoryLabel->needsNewLine = true;
}

gameScenePopupCategories::~gameScenePopupCategories()
{
    GPE_VerticalCardButton * tempButon = NULL;
    for( int i = (int)elements.size()-1; i >=0; i-- )
    {
        tempButon = elements[i];
        if( tempButon!=NULL)
        {
            delete tempButon;
            tempButon = NULL;
        }
    }
    elements.clear();
    if( categoryLabel!=NULL)
    {
        delete categoryLabel;
        categoryLabel = NULL;
    }
}

GPE_VerticalCardButton * gameScenePopupCategories::add_button(  std::string name, int id, std::string imgLocation, std::string parsedLines  )
{
    GPE_VerticalCardButton * newCard = new GPE_VerticalCardButton( imgLocation, parsedLines, name, id, 64 );
    newCard->showBackground = false;
    newCard->usingFlagIcon = true;
    elements.push_back( newCard );
    return newCard;
}

void gameScenePopupCategories::add_if_available( GPE_GuiElementList *  cList, std::string str )
{
    if( cList !=NULL )
    {
        int i = 0;
        if( (int)str.size() > 0 )
        {
            bool categoryAdded = false;
            if( string_contains( string_lower( categoryLabel->get_name() ), string_lower( str ) ) )
            {
                cList->add_gui_element( categoryLabel, true );
                categoryAdded = true;
            }
            for( i = 0; i < (int)elements.size(); i++ )
            {
                if( elements[i]!=NULL && string_contains( string_lower( elements[i]->get_name() ), string_lower (str ) ) )
                {
                    if( !categoryAdded )
                    {
                        cList->add_gui_element( categoryLabel, true );
                        categoryAdded = true;
                    }
                    cList->add_gui_auto( elements[i] );
                }
            }
        }
        else
        {
            cList->add_gui_element( categoryLabel, true );
            for( i = 0; i < (int)elements.size(); i++ )
            {
                cList->add_gui_auto( elements[i] );
            }
        }
    }
}

GPE_SceneEditorHelper::GPE_SceneEditorHelper()
{
    editMode = SCENE_MODE_PLACE;
    mouseInScene = false;
    mouseXPos = 0;
    mouseYPos = 0;
    sWidth = 0;
    sHeight = 0;
    cSceneAnimtList = NULL;
    cSceneObjList = NULL;
    cSceneTexList = NULL;
    cSceneTstList = NULL;

    zoomValue = 1.0;
    currentCamera = new GPE_Rect();
    tempRect = new GPE_Rect();
    cameraFloorXPos = cameraFloorYPos = 0;
    lightCircleTexture = gpeph->get_new_texture();
    lightCircleTexture->prerender_circle( GPE_MAIN_RENDERER,256, c_white, 255 );
    lightCircleTexture->set_blend_mode( blend_mode_add);
    lightCircleTexture->change_alpha( 255 );

    highlightRect = gpeph->get_new_texture();
    highlightRect->prerender_rectangle( GPE_MAIN_RENDERER, 256, 256, c_blue );
    highlightRect->set_blend_mode( blend_mode_add );
    highlightRect->change_alpha( 255 );

    topList = new GPE_GuiElementList();
    middleList = new GPE_GuiElementList();
    bottomList = new GPE_GuiElementList();
    confirmButton = new GPE_ToolLabelButton("Create","Creates new element");
    cancelButton = new GPE_ToolLabelButton("Cancel","Cancels Operation");
    currentLabel = new GPE_Label_Text( "","" );
    descriptionLabel = new GPE_Label_Paragraph( "","","" );
    nameField = new GPE_TextInputBasic("","Name...");
    searchField = new GPE_TextInputBasic("","Search...");

    gameScenePopupCategories * tCategory = add_category("Layer Element");
    tCategory->add_button("Layer",BRANCH_TYPE_LAYER,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/map.png","Layer" );
    tCategory->add_button("Group",BRANCH_TYPE_GROUP,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/object-group.png","Group" );
    tCategory->add_button("TileMap",BRANCH_TYPE_TILEMAP,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/th.png","TileMap" );

    tCategory = add_category("Standard Element");
    tCategory->add_button("Animation",BRANCH_TYPE_ANIMATION,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/magnet.png","Animation" );
    tCategory->add_button("Background",BRANCH_TYPE_BACKGROUND,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/image.png","Background" );
    tCategory->add_button("Object",BRANCH_TYPE_OBJECT,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/automobile.png","Object" );
    tCategory->add_button("Multi-Line Text",BRANCH_TYPE_TEXT,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/text-height.png","Multi-Line\nText" );
    tCategory->add_button("Single-Line Text",BRANCH_TYPE_STEXT,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/text-width.png","Single-Line\nText" );

    tCategory = add_category("Effects");
    tCategory->add_button("Light",BRANCH_TYPE_LIGHT,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/lightbulb-o.png","Light" );
    tCategory->add_button("Particle Emitter",BRANCH_TYPE_PARTIClE_EMITTER,APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/magic.png","Particle\n Emitter" );
    layerListsDropDown = new GPE_DropDown_Menu("Available Layers", false);
}

GPE_SceneEditorHelper::~GPE_SceneEditorHelper()
{
    if( highlightRect!=NULL )
    {
        delete highlightRect;
        highlightRect = NULL;
    }

    if( lightCircleTexture!=NULL )
    {
        delete lightCircleTexture;
        lightCircleTexture = NULL;
    }

    if( topList!=NULL )
    {
        topList->clear_list();
        delete topList;
        topList = NULL;
    }

    if( middleList!=NULL )
    {
        middleList->clear_list();
        delete middleList;
        middleList = NULL;
    }

    if( bottomList!=NULL )
    {
        bottomList->clear_list();
        delete bottomList;
        bottomList = NULL;
    }
    if( highlightRect!=NULL )
    {
        delete highlightRect;
        highlightRect = NULL;
    }
}

gameScenePopupCategories *  GPE_SceneEditorHelper::add_category( std::string name )
{
    gameScenePopupCategories * newCategory = new gameScenePopupCategories( name );
    popupCategories.push_back( newCategory );
    return newCategory;
}

int GPE_SceneEditorHelper::get_new_resource(std::string title )
{
    reset_meta();
    if( GPE_MAIN_GUI!=NULL && GPE_MAIN_RENDERER!=NULL )
    {
        gpe->end_loop();
        currentLabel->set_name("Select an option below for a new type in your scene");
        RESOURCE_TO_DRAG = NULL;
        gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );
        GPE_MAIN_GUI->reset_gui_info();
        MAIN_OVERLAY->take_frozen_screenshot( );

        int promptBoxWidth = SCREEN_WIDTH *3/4;
        if( promptBoxWidth < 320 )
        {
            promptBoxWidth = 320;
        }
        int promptBoxHeight = SCREEN_HEIGHT * 7/8;
        if( promptBoxHeight < 240 )
        {
            promptBoxHeight = 240;
        }
        GPE_Rect elementBox;

        bool exitOperation = false;
        input->reset_all_input();

        GPE_MAIN_RENDERER->reset_viewpoint();
        //MAIN_OVERLAY->render_frozen_screenshot( );
        int selectedOptionId = -1;
        std::string selectedOptionStr = "";
        GPE_VerticalCardButton * selectedButton = NULL;
        while(exitOperation==false)
        {
            gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );
            //GPE_Report("Processing tip of the day");
            gpe->start_loop();

            elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2;
            elementBox.y = (SCREEN_HEIGHT-promptBoxHeight)/2;
            elementBox.w = promptBoxWidth;
            elementBox.h = promptBoxHeight;

            topList->set_height( 64 );
            bottomList->set_height( 128 );

            topList->set_coords(elementBox.x, elementBox.y +32 );
            topList->set_width(elementBox.w);

            topList->barXMargin = 0;
            topList->barYMargin = 0;
            topList->barXPadding = GENERAL_GPE_GUI_PADDING;
            topList->barYPadding = GENERAL_GPE_GUI_PADDING;
            topList->clear_list();
            topList->add_gui_element(currentLabel,true);
            topList->add_gui_element(searchField,true);
            topList->process_self( );

            middleList->set_coords(elementBox.x, topList->get_y2pos() );
            middleList->set_width(elementBox.w);
            middleList->set_height(elementBox.h - middleList->get_ypos() - bottomList->get_height());
            middleList->barXMargin = GENERAL_GPE_GUI_PADDING;
            middleList->barYMargin = GENERAL_GPE_GUI_PADDING;
            middleList->barXPadding = GENERAL_GPE_GUI_PADDING;
            middleList->barYPadding = GENERAL_GPE_GUI_PADDING;

            GPE_MAIN_GUI->reset_gui_info();
            middleList->clear_list();
            for( int i = 0; i < (int)popupCategories.size(); i++ )
            {
                if( popupCategories[i]!=NULL )
                {
                    popupCategories[i]->add_if_available( middleList, searchField->get_string() );
                }
            }

            middleList->process_self( NULL, NULL );
            if( middleList->selectedElement !=NULL && middleList->selectedElement->get_element_type()=="verticalButton" )
            {
                selectedOptionStr = middleList->selectedElement->descriptionText;
                if( descriptionText!= selectedOptionStr)
                {
                    descriptionText = selectedOptionStr;
                    descriptionLabel->update_text( descriptionText );
                    selectedButton = (GPE_VerticalCardButton * ) middleList->selectedElement;
                    selectedOptionId = selectedButton->get_id();
                }
            }


            bottomList->clear_list();
            bottomList->set_coords(middleList->get_xpos(), middleList->get_y2pos() );
            bottomList->set_width(elementBox.w);
            bottomList->barXMargin = 0;
            bottomList->barYMargin = 0;
            bottomList->barXPadding = GENERAL_GPE_GUI_PADDING;
            bottomList->barYPadding = GENERAL_GPE_GUI_PADDING;

            bottomList->clear_list();
            bottomList->add_gui_element(descriptionLabel,true);
            if( descriptionLabel->get_paragraph() == "Layer")
            {
                bottomList->add_gui_element(layerListsDropDown,true);
            }
            else
            {
                bottomList->add_gui_element(nameField, true );
            }
            bottomList->add_gui_auto(confirmButton );
            bottomList->add_gui_auto(cancelButton );
            bottomList->process_self( NULL, NULL );

            if( input->check_keyboard_released(kb_esc) || cancelButton->is_clicked() || GPE_MAIN_WINDOW->is_resized()  )
            {
                exitOperation = true;
                selectedOptionId = -1;
                chosenName = "";
            }
            else if( confirmButton->is_clicked() )
            {
                exitOperation = true;
                chosenName = nameField->get_string();
            }

            //GPE_Report("Rendering tip of the day");
            GPE_MAIN_RENDERER->reset_viewpoint();
            if( !GPE_MAIN_WINDOW->is_resized() )
            {
                //if( input->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot( );
                }
                //Update screen
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Main_Box_Color,false);

                gcanvas->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Color,false);
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,true);
                gfs->render_text( elementBox.x+elementBox.w/2,elementBox.y+GENERAL_GPE_GUI_PADDING,title,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
                topList->render_self( NULL, NULL );
                middleList->render_self( NULL, NULL );
                bottomList->render_self( NULL, NULL );
                //GPE_MAIN_GUI-render_gui_info(  true);

                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
                if( GPE_MAIN_GUI!= NULL )
                {
                    GPE_MAIN_GUI->render_gui_info();
                }
            }
            gpe->end_loop();
        }
        input->reset_all_input();
        return selectedOptionId;
    }
    return -1;
}

void GPE_SceneEditorHelper::reset_meta()
{
    boxIsMoving = false;
    boxWasResized = false;
    boxBeingResized = false;
    bottomList->reset_self();
    middleList->reset_self();
    topList->reset_self();
    currentLabel->set_name("");
    descriptionText = "";
    chosenName = "";
    currentLabel->descriptionText = "";
    nameField->set_string( "" );
    searchField->set_string( "" );
    descriptionLabel->update_text("");
}
