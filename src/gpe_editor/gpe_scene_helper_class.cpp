/*
gpe_scene_helper_class.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/
#include "gpe_editor.h"
#include "gpe_scene_helper_class.h"

GPE_SceneEditorHelper * spm = NULL;

gameScenePopupCategories::gameScenePopupCategories( std::string cName )
{
    name = cName;
    categoryLabel = new pawgui::widget_label_title( name, name );
    categoryLabel->needsNewLine = true;
}

gameScenePopupCategories::~gameScenePopupCategories()
{
    pawgui::widget_button_card_vertical * tempButon = NULL;
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

pawgui::widget_button_card_vertical * gameScenePopupCategories::add_button(  std::string name, int id, std::string imgLocation, std::string parsedLines  )
{
    pawgui::widget_button_card_vertical * newCard = new pawgui::widget_button_card_vertical( imgLocation, parsedLines, name, id, 64 );
    newCard->showBackground = false;
    newCard->usingFlagIcon = true;
    elements.push_back( newCard );
    return newCard;
}

void gameScenePopupCategories::add_if_available( pawgui::widget_panel_list *  cList, std::string str )
{
    if( cList !=NULL )
    {
        int i = 0;
        if( (int)str.size() > 0 )
        {
            bool categoryAdded = false;
            if( stg_ex::string_contains( stg_ex::string_lower( categoryLabel->get_name() ), stg_ex::string_lower( str ) ) )
            {
                cList->add_gui_element( categoryLabel, true );
                categoryAdded = true;
            }
            for( i = 0; i < (int)elements.size(); i++ )
            {
                if( elements[i]!=NULL && stg_ex::string_contains( stg_ex::string_lower( elements[i]->get_name() ), stg_ex::string_lower (str ) ) )
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
    currentCamera = new gpe::shape_rect();
    tempRect = new gpe::shape_rect();
    cameraFloorXPos = cameraFloorYPos = 0;

    highlightRect = gpe::rph->get_new_texture();
    highlightRect->prerender_rectangle( gpe::renderer_main, 256, 256, gpe::c_blue );
    highlightRect->set_blend_mode( gpe::blend_mode_add );
    highlightRect->change_alpha( 255 );

    topList = new pawgui::widget_panel_list();
    middleList = new pawgui::widget_panel_list();
    bottomList = new pawgui::widget_panel_list();
    confirmButton = new pawgui::widget_button_label("Create","Creates new element");
    cancelButton = new pawgui::widget_button_label("Cancel","Cancels Operation");
    currentLabel = new pawgui::widget_label_text ( "","" );
    descriptionLabel = new pawgui::widget_label_paragraph( "","","" );
    nameField = new pawgui::widget_input_text("","Name...");
    searchField = new pawgui::widget_input_text("","Search...");

    eraserAnimation = pawgui::rsm_gui->animation_add("guiErase", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/eraser.png",1, true );

    gameScenePopupCategories * tCategory = add_category("Layer Element");
    tCategory->add_button("Layer", (int)gpe::branch_type::LAYER, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/map.png","Layer" );
    tCategory->add_button("Group", (int)gpe::branch_type::GROUP, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/object-group.png","Group" );
    tCategory->add_button("TileMap",(int)gpe::branch_type::TILEMAP, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/th.png","TileMap" );

    tCategory = add_category("Standard Element");
    tCategory->add_button("Animation",(int)gpe::branch_type::ANIMATION, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magnet.png","Animation" );
    tCategory->add_button("Background",(int)gpe::branch_type::BACKGROUND, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/image.png","Background" );
    tCategory->add_button("Object", (int)gpe::branch_type::OBJECT, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/automobile.png","Object" );
    tCategory->add_button("Multi-Line Text", (int)gpe::branch_type::TEXT, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/text-height.png","Multi-Line\nText" );
    tCategory->add_button("Single-Line Text", (int)gpe::branch_type::STEXT, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/text-width.png","Single-Line\nText" );

    tCategory = add_category("Effects");
    tCategory->add_button("Light", (int)gpe::branch_type::LIGHT, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/lightbulb-o.png","Light" );
    tCategory->add_button("Particle Emitter", (int)gpe::branch_type::PARTIClE_EMITTER, gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magic.png","Particle\n Emitter" );
    layerListsDropDown = new pawgui::widget_dropdown_menu("Available Layers", false);
}

GPE_SceneEditorHelper::~GPE_SceneEditorHelper()
{
    if( eraserAnimation!=NULL)
    {
        pawgui::rsm_gui->remove_animation( eraserAnimation->get_name() );
        eraserAnimation = NULL;
    }

    if( highlightRect!=NULL )
    {
        delete highlightRect;
        highlightRect = NULL;
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
    if( editor_gui_main!=NULL && gpe::renderer_main!=NULL )
    {
        gpe::game_runtime->end_loop();
        currentLabel->set_name("Select an option below for a new type in your scene");
        pawgui::resource_dragged = NULL;
        gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
        editor_gui_main->reset_gui_info();
        pawgui::main_overlay_system->take_frozen_screenshot( );

        int promptBoxWidth = gpe::screen_width *3/4;
        if( promptBoxWidth < 320 )
        {
            promptBoxWidth = 320;
        }
        int promptBoxHeight = gpe::screen_height * 7/8;
        if( promptBoxHeight < 240 )
        {
            promptBoxHeight = 240;
        }
        gpe::shape_rect widget_box;

        bool exitOperation = false;
        gpe::input->reset_all_input();

        gpe::renderer_main->reset_viewpoint();
        //pawgui::main_overlay_system->render_frozen_screenshot( );
        int selectedOptionId = (int)gpe::branch_type::BASIC_SCENE_ELEMENT;
        std::string selectedOptionStr = "";
        pawgui::widget_button_card_vertical * selectedButton = NULL;
        while(exitOperation==false)
        {
            gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
            //error_log->report("Processing tip of the day");
            gpe::game_runtime->start_loop();

            widget_box.x = (gpe::screen_width-promptBoxWidth)/2;
            widget_box.y = (gpe::screen_height-promptBoxHeight)/2;
            widget_box.w = promptBoxWidth;
            widget_box.h = promptBoxHeight;

            topList->set_height( 64 );
            bottomList->set_height( 128 );

            topList->set_coords(widget_box.x, widget_box.y +32 );
            topList->set_width(widget_box.w);

            topList->barXMargin = 0;
            topList->barYMargin = 0;
            topList->barXPadding = pawgui::padding_default;
            topList->barYPadding = pawgui::padding_default;
            topList->clear_list();
            topList->add_gui_element(currentLabel,true);
            topList->add_gui_element(searchField,true);
            topList->process_self( );

            middleList->set_coords(widget_box.x, topList->get_y2() );
            middleList->set_width(widget_box.w);
            middleList->set_height(widget_box.h - middleList->get_ypos() - bottomList->get_height());
            middleList->barXMargin = 0;
            middleList->barYMargin = 0;
            middleList->barXPadding = pawgui::padding_default;
            middleList->barYPadding = pawgui::padding_default;

            editor_gui_main->reset_gui_info();
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
                    selectedButton = (pawgui::widget_button_card_vertical * ) middleList->selectedElement;
                    selectedOptionId = selectedButton->get_id();
                }
            }


            bottomList->clear_list();
            bottomList->set_coords(middleList->get_xpos(), middleList->get_y2() );
            bottomList->set_width(widget_box.w);
            bottomList->barXMargin = 0;
            bottomList->barYMargin = 0;
            bottomList->barXPadding = pawgui::padding_default;
            bottomList->barYPadding = pawgui::padding_default;

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

            if( gpe::input->check_kb_released(kb_esc) || cancelButton->is_clicked() || gpe::window_controller_main->is_resized()  )
            {
                exitOperation = true;
                selectedOptionId = (int)gpe::branch_type::BASIC_SCENE_ELEMENT;
                chosenName = "";
            }
            else if( confirmButton->is_clicked() )
            {
                exitOperation = true;
                chosenName = nameField->get_string();
            }

            //error_log->report("Rendering tip of the day");
            gpe::renderer_main->reset_viewpoint();
            if( !gpe::window_controller_main->is_resized() )
            {
                //if( gpe::input->window_input_received )
                {
                    pawgui::main_overlay_system->render_frozen_screenshot( );
                }
                //Update screen
                gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->main_box_color,false);

                gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,pawgui::theme_main->popup_box_color,false);
                gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_highlight_color,true);
                gpe::gfs->render_text( widget_box.x+widget_box.w/2,widget_box.y+pawgui::padding_default,title,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top);
                topList->render_self( NULL, NULL );
                middleList->render_self( NULL, NULL );
                bottomList->render_self( NULL, NULL );
                //editor_gui_main-render_gui_info(  true);

                gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_border_color,true);
                if( editor_gui_main!= NULL )
                {
                    editor_gui_main->render_gui_info();
                }
            }
            gpe::game_runtime->end_loop();
        }
        gpe::input->reset_all_input();
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
