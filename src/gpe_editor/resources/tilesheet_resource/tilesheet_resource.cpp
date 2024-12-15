/*
tilesheet_resource.cpp
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

#include "tilesheet_resource.h"
#include "../../editor/gpe_editor_settings.h"

std::string tsDataLabels[TILESHEET_DATA_FIELD_COUNT];
int tsDataLabelWidth;

tilesheetPreviewer::tilesheetPreviewer()
{
    areaIsScrollable = false;
    zoomValue = 1;
    minZoomValue = 0.125;
    maxZoomValue = 8;
    local_mouse_x = 0;
    local_mouse_y = 0;
    showGrid = true;
    tileSheetToPreview = nullptr;

    tsEditorViewRect.x = 128;
    tsEditorViewRect.y = 32;
    tsEditorViewRect.w = 640;
    tsEditorViewRect.h = 480;

    tsRect.x = 128;
    tsRect.y = 32;
    tsRect.w = 640;
    tsRect.h = 480;

    tsCameraRect.x = 0;
    tsCameraRect.y = 0;
    tsCameraRect.w = 640;
    tsCameraRect.h = 480;

    tsselectedArea.x = 0;
    tsselectedArea.y = 0;
    tsselectedArea.w = 0;
    tsselectedArea.h = 0;

    tileToPrevX1 = 0;
    tileToPrevX2 = 0;
    tileToPrevY1 = 0;
    tileToPrevY2 = 0;
    previewXScroll = new pawgui::widget_scrollbar_xaxis();
    previewYScroll = new pawgui::widget_scrollbar_yaxis();
}

tilesheetPreviewer::~tilesheetPreviewer()
{
    tilesIdsInPreview.clear();
    if( previewXScroll!=nullptr)
    {
        delete previewXScroll;
        previewXScroll = nullptr;
    }
    if( previewYScroll!=nullptr)
    {
        delete previewYScroll;
        previewYScroll = nullptr;
    }
}

/*
For now this will be essentially a copy/paste of the gameSceneResource object with varaible name changes.
If zooming is added to all gui elements this will go to the parent object with all elements containing zoom data
For now this works for its needed purpose
*/

bool tilesheetPreviewer::get_mouse_coords( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    local_mouse_x = 0;
    local_mouse_y = 0;
    if( view_space!=nullptr)
    {
        if( gpe::point_within( gpe::input->mouse_position_x,gpe::input->mouse_position_y,
                         tsEditorViewRect.x+view_space->x,
                         tsEditorViewRect.y+view_space->y,
                         tsEditorViewRect.x+view_space->x+tsEditorViewRect.w,
                         tsEditorViewRect.y+view_space->y+tsEditorViewRect.h ) )
        {
            local_mouse_x = (gpe::input->mouse_position_x-tsEditorViewRect.x-view_space->x )/zoomValue +tsCameraRect.x - cam->x;
            local_mouse_y = (gpe::input->mouse_position_y-tsEditorViewRect.y-view_space->y )/zoomValue +tsCameraRect.y - cam->y;
            return true;
        }
    }
    return false;
}

void tilesheetPreviewer::handle_scrolling()
{
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    if( areaIsScrollable )
    {
        if( gpe::input->check_kb_down(kb_ctrl) )
        {
            //Zoom Out
            if( gpe::input->mouse_scrolling_down && zoomValue >= minZoomValue*2 )
            {
                zoomValue-=minZoomValue;
            }
            else if( gpe::input->mouse_scrolling_up && zoomValue < maxZoomValue-minZoomValue)
            {
                //Zoom in
                zoomValue += minZoomValue;
            }

        }
        else if( gpe::input->kb_shift_pressed)
        {
            if( gpe::input->mouse_scrolling_up > 0 )
            {
                xScrollHappened = true;
                tsCameraRect.x-=(tsCameraRect.w/16);
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                xScrollHappened = true;
                tsCameraRect.x+=(tsCameraRect.w/16);
            }
        }
        else
        {
            if( gpe::input->mouse_scrolling_up )
            {
                yScrollHappened = true;
                tsCameraRect.y-=(tsCameraRect.h/4);
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                yScrollHappened = true;
                tsCameraRect.y+=(tsCameraRect.h/4);
            }
            else if( hasArrowkeyControl)
            {
                //arrow scrolling
                if( gpe::input->check_kb_down(kb_up) )
                {
                    yScrollHappened = true;
                    tsCameraRect.y-=(tsCameraRect.h/4);
                }
                else if( gpe::input->check_kb_down(kb_down) )
                {
                    yScrollHappened = true;
                    tsCameraRect.y+=(tsCameraRect.h/4);
                }
                if( gpe::input->check_kb_down(kb_left) )
                {
                    if( tsCameraRect.x > (tsCameraRect.w/4) )
                    {
                        xScrollHappened = true;
                        tsCameraRect.x-=(tsCameraRect.w/4);
                    }
                    else
                    {
                        tsCameraRect.x = 0;
                        xScrollHappened = true;
                    }
                }
                else if( gpe::input->check_kb_down(kb_right) )
                {
                    if( (tsCameraRect.x +tsCameraRect.w/4) < tsRect.w )
                    {
                        xScrollHappened = true;
                        tsCameraRect.x+=(tsCameraRect.w/4);
                    }
                }
            }
        }
    }

    //Prevents camera from going out of bounds
    if( tsCameraRect.x+tsCameraRect.w > tsRect.w )
    {
        xScrollHappened = true;
        tsCameraRect.x = tsRect.w-tsCameraRect.w;
    }

    if( tsCameraRect.y+tsCameraRect.h > tsRect.h )
    {
        yScrollHappened = true;
        tsCameraRect.y = tsRect.h-tsCameraRect.h;
    }

    if( tsCameraRect.x <= 0)
    {
        xScrollHappened = true;
        tsCameraRect.x = 0;
    }

    if( tsCameraRect.y <= 0)
    {
        yScrollHappened = true;
        tsCameraRect.y = 0;
    }

    if( xScrollHappened)
    {
        previewXScroll->contextRect.x = tsCameraRect.x;
        previewXScroll->process_self(nullptr,nullptr );
    }
    if( yScrollHappened)
    {
        previewYScroll->contextRect.y = tsCameraRect.y;
        previewYScroll->process_self(nullptr,nullptr );
    }
}

void tilesheetPreviewer::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    //sets viewedspace and cam to the default camera(entire screen if they are nullptr )
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    pawgui::widget_basic::process_self(view_space,cam);

    tsEditorViewRect.x = widget_box.x;
    tsEditorViewRect.y = widget_box.y;
    tsEditorViewRect.w = tsCameraRect.w =widget_box.w-16;
    tsEditorViewRect.h = tsCameraRect.h =widget_box.h-16;

    tsRect.x = 0;
    tsRect.y = 0;
    tsRect.w = tsEditorViewRect.w;
    tsRect.h = tsEditorViewRect.h;

    //checks if the element has arrow and/or scroll control
    if( isClicked && pawgui::resource_dragged==nullptr )
    {
        hasScrollControl = true;
        hasArrowkeyControl = true;
    }
    else if( clickedOutside)
    {
        hasScrollControl = false;
        hasArrowkeyControl = false;
    }

    //If the loaded tilesheet is not nullptr, continue with the logic below
    if(tileSheetToPreview!=nullptr && tileSheetToPreview->tsImage!=nullptr && cam!=nullptr && view_space!=nullptr )
    {
        tsEditorViewRect.x  = widget_box.x;
        tsEditorViewRect.y  = widget_box.y;
        tsEditorViewRect.w = (widget_box.w-previewYScroll->get_width() );
        tsEditorViewRect.h = (widget_box.h-previewXScroll->get_height() );

        tsCameraRect.w  = tsEditorViewRect.w / zoomValue;
        tsCameraRect.h  = tsEditorViewRect.h / zoomValue;

        tsRect.w = tileSheetToPreview->tsImage->get_width();
        tsRect.h = tileSheetToPreview->tsImage->get_height();

        if( tsCameraRect.w > tsRect.w)
        {
            tsCameraRect.w = tsRect.w;
        }

        if( tsCameraRect.h > tsRect.h)
        {
            tsCameraRect.h = tsRect.h;
        }
        //Handles scrolling
        //Horizontal scrolling
        previewXScroll->set_coords( tsEditorViewRect.x,tsEditorViewRect.y+tsEditorViewRect.h );
        previewXScroll->set_width( tsEditorViewRect.w );
        update_rectangle(&previewXScroll->fullRect,0,0,(float)tsRect.w, (float)tsRect.h );
        update_rectangle(&previewXScroll->contextRect,(float)tsCameraRect.x,(float)tsCameraRect.y, (float)tsCameraRect.w, (float)tsCameraRect.h );
        previewXScroll->process_self(view_space,cam );
        //if( previewXScroll->has_moved() )
        {
            tsCameraRect.x = (float)(previewXScroll->contextRect.x);
        }

        //Vertical Scrolling
        previewYScroll->set_coords( tsEditorViewRect.x+tsEditorViewRect.w,tsEditorViewRect.y );
        previewYScroll->set_height( tsEditorViewRect.h);
        update_rectangle(&previewYScroll->fullRect,0,0,(float)tsRect.w, (float)tsRect.h );
        update_rectangle(&previewYScroll->contextRect,(float)tsCameraRect.x,(float)tsCameraRect.y, (float)tsCameraRect.w, (float)tsCameraRect.h );
        //sceneYScroll->contextRect.h = sceneEditorView.h;
        previewYScroll->process_self(view_space,cam );
        //if( sceneYScroll->has_moved() )
        {
            tsCameraRect.y = float(previewYScroll->contextRect.y);
        }
        //yTileScroll->fullRect.w = tileSheetToPreview->tsImage->get_width()*zoomValue;

        if( get_mouse_coords(view_space, cam) )
        {
            areaIsScrollable = true;
        }
        else
        {
            areaIsScrollable = false;
        }
        //Handles selecting tiles within tilesheet previewer
        if( isHovered)
        {
            //Calculates the tile to select via the tilesheet
            if( previewXScroll->is_scrolling()==false && previewYScroll->is_scrolling()==false  && pawgui::resource_dragged==nullptr )
            {
                if( areaIsScrollable )
                {
                    pawgui::main_statusbar->set_codestring( "Mouse (X:"+ stg_ex::int_to_string(local_mouse_x)+",Y:"+ stg_ex::int_to_string( local_mouse_y)+")" );
                    gpe::shape_rect foundTSRect;
                    int iTSX = 0;

                    //If pressed finds the X/Y the mouse is in the tile-sheet by "found tile X/Y coordinae
                    //Only happens once on pressed...
                    if( gpe::input->check_mouse_pressed(0) )
                    {
                        tileToPrevX1 = local_mouse_x;
                        tileToPrevY1 = local_mouse_y;
                        for(  iTSX = 0; iTSX < (int)tileSheetToPreview->tsRects.size(); iTSX++)
                        {
                            foundTSRect = tileSheetToPreview->tsRects.at(iTSX);

                            if( gpe::point_between_rect(tileToPrevX1,tileToPrevY1,&foundTSRect) )
                            {
                                tileToPrevX1 = tileToPrevX2 = foundTSRect.x;
                                tileToPrevY1 = tileToPrevY2 = foundTSRect.y;
                                tsselectedArea.x = tileToPrevX1;
                                tsselectedArea.y = tileToPrevY1;
                                tsselectedArea.w = tileSheetToPreview->tsWidth;
                                tsselectedArea.h = tileSheetToPreview->tsHeight;
                                break;
                            }
                        }
                    }
                    else if( gpe::input->check_mouse_down( mb_left ) )
                    {
                        tileToPrevX2 = local_mouse_x;
                        tileToPrevY2 = local_mouse_y;

                        for(  iTSX = 0; iTSX < (int)tileSheetToPreview->tsRects.size(); iTSX++)
                        {
                            foundTSRect = tileSheetToPreview->tsRects.at(iTSX);

                            if( point_between_rect(tileToPrevX2,tileToPrevY2,&foundTSRect) )
                            {
                                tileToPrevX2 = foundTSRect.x;
                                tileToPrevY2 = foundTSRect.y;
                                break;
                            }
                        }


                        if( tileToPrevX1!=tileToPrevX2)
                        {
                            tsselectedArea.x = std::min(tileToPrevX1,tileToPrevX2);
                            tsselectedArea.w = abs(tileToPrevX1-tileToPrevX2)+tileSheetToPreview->tsWidth;
                        }
                        else
                        {
                            tsselectedArea.x = tileToPrevX1;
                            tsselectedArea.w = tileSheetToPreview->tsWidth;
                        }

                        if( tileToPrevY1!=tileToPrevY2)
                        {
                            tsselectedArea.y = std::min(tileToPrevY1,tileToPrevY2);
                            tsselectedArea.h = abs(tileToPrevY1-tileToPrevY2)+tileSheetToPreview->tsHeight;

                        }
                        else
                        {
                            tsselectedArea.y = tileToPrevY1;
                            tsselectedArea.h = tileSheetToPreview->tsHeight;
                        }

                        //Stores tile ids of what is currently selected
                        tilesIdsInPreview.clear();
                        bool yValueChanged = false;
                        tilesToPlacePerRow = 0;
                        int firstY = -1; //sets at -1 since no tilesheet has negative tile coordinates
                        for(  iTSX = 0; iTSX < (int)tileSheetToPreview->tsRects.size(); iTSX++)
                        {
                            foundTSRect = tileSheetToPreview->tsRects.at(iTSX);

                            if( check_collision_rects(tsselectedArea,foundTSRect) )
                            {
                                if( firstY < 0)
                                {
                                    firstY = foundTSRect.y;
                                }
                                else if( yValueChanged==false && firstY!=foundTSRect.y)
                                {
                                    yValueChanged = true;
                                    tilesToPlacePerRow = (int)tilesIdsInPreview.size();
                                }
                                tilesIdsInPreview.push_back(iTSX);
                            }
                        }
                        if( yValueChanged==false)
                        {
                            tilesToPlacePerRow = (int)tilesIdsInPreview.size();
                        }

                    }
                    else if( isRightClicked)
                    {
                        pawgui::context_menu_open(-1,-1,256);
                        if( showGrid)
                        {
                            pawgui::main_context_menu->add_menu_option("Show Grid",1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/check.png"),-1,nullptr,true,true,false);
                        }
                        else
                        {
                            pawgui::main_context_menu->add_menu_option("Show Grid",1,nullptr,-1,nullptr,true,true,false );
                        }
                        int menuSelection = pawgui::context_menu_process();
                        if( menuSelection==0 && pawgui::main_tab_resource_bar!=nullptr && tileSheetToPreview!=nullptr)
                        {
                            //pawgui::main_tab_resource_bar->add_new_tab(tileSheetToPreview);
                        }
                        else if( menuSelection==1)
                        {
                            showGrid = !showGrid;
                        }
                    }
                }
                else if( isHovered && gpe::input->check_mouse_released(  mb_anybutton ) )
                {
                    reset_preview(false);
                }
            }
            handle_scrolling();
        }
        else if( gpe::input->check_mouse_released( mb_right) || gpe::input->check_mouse_released( mb_middle ) )
        {
            reset_preview(false);
        }
        return;
    }
    //reset tile preview data as the above code did not return earlier
    reset_preview(true);
}

void tilesheetPreviewer::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( tileSheetToPreview!=nullptr && cam!=nullptr && view_space!=nullptr )
    {
        if(tileSheetToPreview->tsImage!=nullptr)
        {
            tsImageCameraRect.x = tsCameraRect.x;
            tsImageCameraRect.y = tsCameraRect.y;
            tsImageCameraRect.w = tsCameraRect.w;
            tsImageCameraRect.h = tsCameraRect.h;
            //Renders the tilesheet in its current preview scaling
            tileSheetToPreview->tsImage->render_tex_scaled( (widget_box.x-cam->x),(widget_box.y-cam->y),  zoomValue, zoomValue, &tsImageCameraRect );
            //Renders the grid of the tilesheet's  boxes
            if( showGrid)
            {
                gpe::shape_rect foundTSRect;
                int iTSX = 0;
                int bx1 = 0;
                int bx2 = 0;
                int by1 = 0;
                int by2 = 0;
                for(  iTSX = 0; iTSX < (int)tileSheetToPreview->tsRects.size(); iTSX++)
                {
                    foundTSRect = tileSheetToPreview->tsRects.at(iTSX);
                    bx1 = foundTSRect.x*zoomValue - tsCameraRect.x*zoomValue;
                    bx1 =  semath::bound_number(bx1,0, widget_box.w);

                    by1 = foundTSRect.y*zoomValue - tsCameraRect.y*zoomValue;
                    by1 =  semath::bound_number(by1,0, widget_box.h);

                    bx2 = (foundTSRect.x+foundTSRect.w)*zoomValue - tsCameraRect.x*zoomValue;
                    bx2 =  semath::bound_number(bx2,0, widget_box.w);

                    by2 = (foundTSRect.y+foundTSRect.h)*zoomValue - tsCameraRect.y*zoomValue;
                    by2 =  semath::bound_number(by2,0, widget_box.h);

                    gpe::gcanvas->render_rectangle( widget_box.x+bx1-cam->x, widget_box.y+by1-cam->y, widget_box.x+bx2-cam->x, widget_box.y+by2-cam->y, pawgui::theme_main->text_box_font_color,true,255);
                }
            }

            if( tsselectedArea.x >=0 && tsselectedArea.y >=0 )
            {
                float renderTSSelectX1 = tsselectedArea.x*zoomValue - tsCameraRect.x*zoomValue;
                renderTSSelectX1 =  semath::bound_number(renderTSSelectX1,0, widget_box.w);

                float renderTSSelectY1 = tsselectedArea.y*zoomValue - tsCameraRect.y*zoomValue;
                renderTSSelectY1 =  semath::bound_number(renderTSSelectY1,0, widget_box.h);

                float renderTSSelectX2 = (tsselectedArea.x+tsselectedArea.w)*zoomValue - tsCameraRect.x*zoomValue;
                renderTSSelectX2 =  semath::bound_number(renderTSSelectX2,0, widget_box.w);

                float renderTSSelectY2 = (tsselectedArea.y+tsselectedArea.h)*zoomValue - tsCameraRect.y*zoomValue;
                renderTSSelectY2 =  semath::bound_number(renderTSSelectY2,0, widget_box.h);

                gpe::gcanvas->render_rectangle( widget_box.x+renderTSSelectX1-cam->x,widget_box.y+renderTSSelectY1-cam->y,widget_box.x+renderTSSelectX2-cam->x,widget_box.y+renderTSSelectY2-cam->y,pawgui::theme_main->button_box_highlight_color,false,128);

            }
            gpe::gfs->render_text_boxed( widget_box.x+widget_box.w-32,widget_box.y+widget_box.h-cam->y-32,
                              "Zoom Level: "+ stg_ex::float_to_string(zoomValue*100 )+"%",
                              pawgui::theme_main->text_box_font_color, gpe::c_black, gpe::font_default,gpe::fa_right,gpe::fa_bottom);
            if( previewXScroll!=nullptr)
            {
                previewXScroll->render_self( view_space,cam);
            }
            if( previewYScroll!=nullptr)
            {
                previewYScroll->render_self( view_space,cam);
            }
        }
    }
}

void tilesheetPreviewer::render_selection( int x_pos, int y_pos, gpe::shape_rect * view_space, gpe::shape_rect * cam, float scale_size, gpe::color * fColor)
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( tileSheetToPreview!=nullptr && cam!=nullptr && view_space!=nullptr && scale_size!=0 )
    {
        if(tileSheetToPreview->tsImage!=nullptr)
        {
            if( tsselectedArea.x >=0 && tsselectedArea.y >=0 && tsselectedArea.w  >0 && tsselectedArea.h > 0 )
            {
                if( tsselectedArea.x+tsselectedArea.w <= tileSheetToPreview->tsImage->get_width() && tsselectedArea.y+tsselectedArea.h <= tileSheetToPreview->tsImage->get_height()  )
                {
                    tileSheetToPreview->tsImage->render_tex_scaled( x_pos-cam->x,y_pos-cam->y,scale_size, scale_size,&tsselectedArea,fColor );
                }
            }
        }
    }
}

void tilesheetPreviewer::reset_preview(bool moveCamera)
{
    tilesIdsInPreview.clear();
    if( moveCamera)
    {
        tsCameraRect.x = 0;
        tsCameraRect.y = 0;
        tsCameraRect.w = 0;
        tsCameraRect.h = 0;
    }
    tsselectedArea.x = 0;
    tsselectedArea.y = 0;
    tsselectedArea.w = 0;
    tsselectedArea.h = 0;
    tileToPrevX1 = tileToPrevY1 = tileToPrevX2 = tileToPrevY2 = 0;
}

tilesheetResource::tilesheetResource(pawgui::widget_resource_container * pFolder)
{
    projectParentFolder = pFolder;
    editorMode = 0;
    tilesheetInEditor = nullptr;
    tsPreviewer = new tilesheetPreviewer();
    preloadCheckBox = new pawgui::widget_checkbox("Preload tilesheet","Check to load tilesheet at game open", true);
    isPreloaded = true;
    if(saveResource_button!=nullptr)
    {
        //saveResource_button->disable_self();
    }
    int i = 0;
    for( i =0; i < TILESHEET_DATA_FIELD_COUNT; i++)
    {
        tsDataFields[i] = new pawgui::widget_input_number("0",true,0,2048);
        tsDataFields[i] ->set_label(tsDataLabels[i]);
    }
    tilesheetDimensionsStr = new pawgui::widget_label_text ("Image Size: 0x0px");

    imageUsesColorKey = new pawgui::widget_checkbox("Image Transparent?","Image uses a color key?", false );
    imageColorKey = new pawgui::gpe_widget_color_picker("Image Color Key","The RGB value of image's color key",255,0,255 );

    openExternalEditor_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Tilesheet Image In External Editor");
    refreshResourceData_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png", "Refreshes the loaded tilesheet image");
    labelInfoMaxTextureSize = new pawgui::widget_label_text ("Max Image Size: 4096 X 4096px","Max Image Size: 4096 X 4096px");
}

tilesheetResource::~tilesheetResource()
{
    if( labelInfoMaxTextureSize!=nullptr)
    {
        delete labelInfoMaxTextureSize;
        labelInfoMaxTextureSize = nullptr;
    }
    if( openExternalEditor_button!=nullptr)
    {
        delete openExternalEditor_button;
        openExternalEditor_button = nullptr;
    }
    if( refreshResourceData_button!=nullptr)
    {
        delete refreshResourceData_button;
        refreshResourceData_button = nullptr;
    }

    if( preloadCheckBox!=nullptr)
    {
        delete preloadCheckBox;
        preloadCheckBox = nullptr;
    }
    if( tilesheetDimensionsStr!=nullptr)
    {
        delete tilesheetDimensionsStr;
        tilesheetDimensionsStr = nullptr;
    }
    for( int i =TILESHEET_DATA_FIELD_COUNT-1; i >=0; i--)
    {
        if( tsDataFields[i]!=nullptr)
        {
            delete tsDataFields[i];
            tsDataFields[i] = nullptr;
        }
    }

    if( tsPreviewer!=nullptr)
    {
        delete tsPreviewer;
        tsPreviewer = nullptr;
    }
}

bool tilesheetResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void tilesheetResource::compile_cpp()
{

}

gpe::texture_base * tilesheetResource::get_resource_texture()
{
    if( tilesheetInEditor!=nullptr && tilesheetInEditor->tsImage!=nullptr)
    {
        return tilesheetInEditor->tsImage;
    }
    return nullptr;
}

bool tilesheetResource::include_local_files( std::string pBuildDir , int buildType )
{
    if( tilesheetInEditor!=nullptr && tilesheetInEditor->tsImage!=nullptr )
    {
        if( tilesheetInEditor->tsImage->get_width() >   0)
        {
            return tilesheetInEditor->tsImage->copy_image_source(pBuildDir+"/resources/tilesheets");
        }
    }
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt","Tilesheet Does not contain texture image...");
    return true;
}

bool tilesheetResource::is_build_ready()
{
    recentErrorMessage = "";
    if( tilesheetInEditor == nullptr)
    {
        recentErrorMessage = "tilesheetInEditor=nullptr;";
        return false;
    }
    bool isReady = true;
    for( int i = 0; i  < TILESHEET_DATA_FIELD_COUNT; i++)
    {
        if( tsDataFields[i]== nullptr )
        {
            recentErrorMessage += "tsDataFields["+ stg_ex::int_to_string(i)+"] = nullptr;";
            isReady = false;
        }
    }

    if( (int)tilesheetInEditor->file_nameLocation.size() == 0 )
    {
        recentErrorMessage += "file_nameLocation=EMPTY;";
        isReady = false;
    }
    if( tilesheetInEditor->tsWidth <=0 )
    {
        recentErrorMessage += "tilesheetInEditor->tsWidth=0;";
        isReady = false;
    }
    if( tilesheetInEditor->tsHeight <=0 )
    {
        recentErrorMessage += "tilesheetInEditor->tsHeight=0;";
        isReady = false;
    }
    return isReady;
}

bool tilesheetResource::load_image(std::string new_file_name)
{
    if( stg_ex::file_is_image(new_file_name) == false )
    {
        pawgui::display_user_alert("Unable to load image","File type["+stg_ex::get_file_ext(new_file_name)+"] not supported when loading ["+new_file_name+"] TS_ER_LOAD_01.");
        return false;
    }

    gpe::texture_base * tempTexture =  gpe::rph->get_new_texture();
    tempTexture->load_new_texture( new_file_name, -1, imageUsesColorKey->is_clicked (), false,imageColorKey->get_r(),imageColorKey->get_g(),imageColorKey->get_b() );
    if( tempTexture->get_width() == 0 || tempTexture->get_height() == 0 )
    {
        pawgui::display_user_alert("Tilesheet Edit Log","Unable to load image named ("+ new_file_name + ") Please check file location in project. TS_ER_LOAD_02" );
        delete tempTexture;
        tempTexture = NULL;
        return false;
    }

    if(tilesheetInEditor==nullptr)
    {
        tilesheetInEditor = new gpe::tilesheet();
    }

    if(tilesheetInEditor->tsImage!=nullptr)
    {
        delete tilesheetInEditor->tsImage;
        tilesheetInEditor->tsImage = nullptr;
    }

    if( tilesheetInEditor==nullptr)
    {
        pawgui::display_user_alert("Tilesheet Edit Log","Resource Error: ("+ resource_name + ") Problem loading Tilesheet TS_ER_LOAD_03." );
        delete tempTexture;
        tempTexture = NULL;
        return false;
    }

    tilesheetInEditor->tsImage = tempTexture;
    if(tilesheetInEditor->tsImage == nullptr)
    {
        pawgui::display_user_alert("Tilesheet Edit Log","Resource Error: ("+ resource_name + ") Problem loading Tilesheet Texture TS_ER_LOAD_04." );
        return false;
    }

    tilesheetInEditor->tsImage->load_new_texture(  new_file_name, -1,  imageUsesColorKey->is_clicked (), false,imageColorKey->get_r(),imageColorKey->get_g(),imageColorKey->get_b() );
    if( tilesheetInEditor->tsImage->get_width() == 0 || tilesheetInEditor->tsImage->get_height() == 0 )
    {
        pawgui::display_user_alert("Tilesheet Edit Log","Resource Error: ("+ resource_name + ") Problem loading Tilesheet Texture TS_ER_LOAD_05." );
        return false;
    }

    if( tilesheetInEditor->tsImage->get_width() <1 || tilesheetInEditor->tsImage->get_width()>16384 || tilesheetInEditor->tsImage->get_height() <1 || tilesheetInEditor->tsImage->get_height()>16384 )
    {
        pawgui::display_user_alert("Unable to load image","Editor Error: Unable to load ["+new_file_name+"] please check file and make sure it is between 1x1 and 4096x4096 pixels and is a valid image TS_ER_LOAD_06");
        if( tilesheetInEditor->tsImage!=nullptr)
        {
            delete tilesheetInEditor->tsImage;
            tilesheetInEditor->tsImage = nullptr;
        }
        return false;
    }


    //successfully added new image.
    tilesheetInEditor->tsRects.clear();
    tilesheetInEditor->file_nameLocation = new_file_name;
    gpe::shape_rect newBox;
    newBox.w = tilesheetInEditor->tsImage->get_width();
    newBox.h = tilesheetInEditor->tsImage->get_height();
    tilesheetInEditor->tsRects.push_back(newBox);
    tsDataFields[0]->set_string( stg_ex::int_to_string(tempTexture->get_width()) );
    tsDataFields[1]->set_string( stg_ex::int_to_string(tempTexture->get_height()) );
    tsDataFields[2]->set_string("0");
    tsDataFields[3]->set_string("0");
    tsDataFields[4]->set_string("0");
    tsDataFields[5]->set_string("0");
    tilesheetInEditor->tsImage->copy_image_source( stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/tilesheets");
    process_data_fields();
    tilesheetDimensionsStr->set_name("Image Size: "+ stg_ex::int_to_string(tilesheetInEditor->tsImage->get_width() )+" x "+ stg_ex::int_to_string(tilesheetInEditor->tsImage->get_height() )+"px" );

    //Lastly let's copy the tilesheet image to our local directory
    std::string resfile_location = stg_ex::get_short_filename( new_file_name, true);
    if( (int)new_file_name.size() > 0 )
    {
        std::string resFileCopyDest = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/tilesheets/"+resfile_location;

        //We won't overrride an existing file in the directory
        if( sff_ex::file_exists(resFileCopyDest) == false )
        {
            sff_ex::file_copy(new_file_name,resFileCopyDest, false );
        }
    }

    return true;
}


void tilesheetResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed ==false  || sff_ex::file_exists(file_path) )
    {
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->update_submessages( "Processing Tilesheet",resource_name );
        }

        std::string otherColContainerName = "";
        std::string found_ts_string = "";

        std::string newFileIn ="";
        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/tilesheets/";
        if( sff_ex::file_exists(file_path) )
        {
            newFileIn = file_path;
            soughtDir = stg_ex::get_path_from_file(newFileIn);
        }
        else
        {
            newFileIn = soughtDir + resource_name+".gpf";
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        //gpe::error_log->report("Loading Tilesheet - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
        {
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string key_string="";
                std::string valstring="";
                std::string subValstring="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                std::string tsDataStr = "";
                float foundFileVersion = 0;
                int i = 0;
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                    currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

                    if( foundFileVersion <=0)
                    {
                        //Empty Line skipping is only allowed at the top of the file
                        if(!currLineToBeProcessed.empty() )
                        {
                            //Comment skipping is only allowed at the top of the file
                            if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                            {
                                //searches for an equal character and parses through the variable
                                equalPos=currLineToBeProcessed.find_first_of("=");
                                if(equalPos!=(int)std::string::npos)
                                {
                                    //if the equalPos is present, then parse on through and carryon
                                    key_string = currLineToBeProcessed.substr(0,equalPos);
                                    valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( key_string=="Version")
                                    {
                                        foundFileVersion = stg_ex::string_to_float(valstring);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion <= 2)
                    {
                        //Begin processing the file.
                        if(!currLineToBeProcessed.empty() )
                        {
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                key_string = currLineToBeProcessed.substr(0,equalPos);
                                valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( key_string=="ResourceName")
                                {
                                    renameBox->set_string(valstring);
                                }
                                else if( key_string=="TransparentImage")
                                {
                                    imageUsesColorKey->set_checked( stg_ex::string_to_bool(valstring) );
                                }
                                else if( key_string=="ColorKeyValue")
                                {
                                    imageColorKey->set_color_from_rgb( valstring );
                                }
                                else if( key_string=="ImageLocation")
                                {
                                    if( valstring!="nullptr")
                                    {
                                        found_ts_string = soughtDir+valstring ;

                                    }
                                }
                                else if( key_string=="Preload")
                                {
                                    preloadCheckBox->set_checked( stg_ex::string_to_bool(valstring) );
                                }
                                else
                                {
                                    for( i = 0; i <TILESHEET_DATA_FIELD_COUNT ; i++)
                                    {
                                        tsDataStr = "TilesheetData["+ stg_ex::int_to_string(i)+"]";
                                        if( key_string==tsDataStr)
                                        {
                                            tsDataFields[i]->set_string(valstring);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        gpe::error_log->report("Invalid FoundFileVersion ="+ stg_ex::float_to_string(foundFileVersion)+".");
                    }
                }
            }
        }

        if( (int)found_ts_string.size() > 0  )
        {
            if( load_image(found_ts_string ) )
            {
                tilesheetDimensionsStr->set_name("Image Size: "+ stg_ex::int_to_string(tilesheetInEditor->tsImage->get_width() )+" x "+ stg_ex::int_to_string(tilesheetInEditor->tsImage->get_height() )+"px" );
            }
        }
    }


    process_data_fields();
}

void tilesheetResource::process_data_fields()
{
    ///constantly updates the tilesheet boxes
    bool allInputsAreValid = true;
    if( tilesheetInEditor!=nullptr)
    {
        for(  int i =0; i < TILESHEET_DATA_FIELD_COUNT; i++)
        {
            if( tsDataFields[i]!=nullptr)
            {
                if( !tsDataFields[i]->is_valid() )
                {
                    allInputsAreValid = false;
                }
            }
            else
            {
                allInputsAreValid = false;
            }
        }
        if( allInputsAreValid)
        {
            tilesheetInEditor->tsIsTransparent = imageUsesColorKey->is_clicked();
            tilesheetInEditor->tsCkeyR = imageColorKey->get_r();
            tilesheetInEditor->tsCkeyG = imageColorKey->get_g();
            tilesheetInEditor->tsCkeyB = imageColorKey->get_b();
            tilesheetInEditor->tsWidth = tsDataFields[0]->get_held_number();
            tilesheetInEditor->tsHeight = tsDataFields[1]->get_held_number();
            tilesheetInEditor->tsXOff = tsDataFields[2]->get_held_number();
            tilesheetInEditor->tsYOff = tsDataFields[3]->get_held_number();
            tilesheetInEditor->tsXPadding = tsDataFields[4]->get_held_number();
            tilesheetInEditor->tsYPadding = tsDataFields[5]->get_held_number();
            tilesheetInEditor->organize_tilesheet();
        }
    }
}

void tilesheetResource::prerender_self( )
{
    standardEditableGameResource::prerender_self( );
    if( preloadCheckBox!=nullptr)
    {
        preloadCheckBox->prerender_self( );
    }
}

void tilesheetResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    int i;
    bool editorHasControl = false;
    if( cam!=nullptr && view_space!=nullptr )
    {
        if( panel_main_editor!=nullptr )
        {
            panel_main_editor->clear_panel();
            panel_main_editor->add_gui_element(renameBox,true);
            panel_main_editor->add_gui_element(refreshResourceData_button,false );
            panel_main_editor->add_gui_element(loadResource_button,false );
            panel_main_editor->add_gui_element( openExternalEditor_button,true);

            //panel_main_editor->add_gui_element(saveResource_button,true);
            panel_main_editor->add_gui_element(tilesheetDimensionsStr,true);
            panel_main_editor->add_gui_element(labelInfoMaxTextureSize,true);

            panel_main_editor->add_gui_element(imageUsesColorKey,true);
            panel_main_editor->add_gui_element(imageColorKey,true);

            //processes the tile data fields
            for(  i =0; i < TILESHEET_DATA_FIELD_COUNT; i++)
            {
                if( tsDataFields[i]!=nullptr)
                {
                    panel_main_editor->add_gui_element(tsDataFields[i],true);
                }
            }
            panel_main_editor->add_gui_element(preloadCheckBox,true);
            panel_main_editor->add_gui_element(confirmResource_button,true);

            panel_main_editor->add_gui_element(cancelResource_button,true);
            //panel_main_editor->set_maxed_out_width();
            editorHasControl = tsPreviewer->hasScrollControl;
            if( !editorHasControl )
            {
                panel_main_editor->process_self(nullptr, nullptr);
            }
            else
            {
                panel_main_editor->hasScrollControl = false;
                panel_main_editor->hasArrowkeyControl = false;
            }

            if( confirmResource_button->is_clicked() )
            {
                save_resource();
                isPreloaded = preloadCheckBox->is_clicked();
            }
            else if( cancelResource_button->is_clicked() )
            {
                if( pawgui::display_prompt_message("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
                {
                    resourcePostProcessed = false;
                    load_resource();
                }
            }
            else if( loadResource_button!=nullptr && loadResource_button->is_clicked() )
            {
                std::string new_file_name = pawgui::get_filename_open_from_popup("Load Your tilesheet Image","Image",pawgui::main_settings->fileOpenTilesheetDir);
                if( (int)new_file_name.size() > 3)
                {
                    load_image(new_file_name);
                }
            }
            else if( refreshResourceData_button!=nullptr && refreshResourceData_button->is_clicked() )
            {
                if( tilesheetInEditor!=nullptr  )
                {
                    resourcePostProcessed = false;
                    load_resource();
                }
            }
            else if( openExternalEditor_button!=nullptr && openExternalEditor_button->is_clicked() )
            {
                if( tilesheetInEditor!=nullptr )
                {
                    if( sff_ex::file_exists(tilesheetInEditor->file_nameLocation ) )
                    {
                        std::string external_editor_program = "";

                        if( main_editor_settings!=nullptr && main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=nullptr)
                        {
                            external_editor_program = main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string();
                        }

                        if( (int)external_editor_program.size() > 0 )
                        {
                            gpe::external_open_program( external_editor_program,tilesheetInEditor->file_nameLocation, true  );
                        }
                        else
                        {
                            gpe::external_open_program(tilesheetInEditor->file_nameLocation );
                        }

                        sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+tilesheetInEditor->file_nameLocation+"]...");
                    }
                }
            }
        }

        if(renameBox!=nullptr)
        {
            if( renameBox->get_string()!=resource_name)
            {
                isModified = true;
            }
        }
        process_data_fields();
        if( tsPreviewer!=nullptr && tilesheetInEditor!=nullptr)
        {
            tsPreviewer->set_coords(0,0);
            tsPreviewer->set_width(view_space->w );
            tsPreviewer->set_height(view_space->h );
            tsPreviewer->tileSheetToPreview = tilesheetInEditor;
            if( !editorHasControl )
            {
                tsPreviewer->process_self(view_space,cam);
            }
            else
            {
                tsPreviewer->hasScrollControl = false;
                tsPreviewer->hasArrowkeyControl = false;
            }
        }


    }
}

void tilesheetResource::render_self( gpe::shape_rect *view_space, gpe::shape_rect *cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    bool texturePreviewIsRendered = false;
    if( cam!=nullptr && view_space!=nullptr )
    {

        if( gpe::gcanvas->is_render_mode_supported( gpe::gcanvas->get_render_mode() ) !=1 )
        {
            //We draw the message that our current render can not draw the preview in this editor and then exit the render function :-(
            gpe::gfs->render_text( view_space->w/2,view_space->h/2,
                          "Render Mode ["+gpe::gcanvas->get_render_mode_name( gpe::gcanvas->get_render_mode())+"] is not supported by "+gpe::gcanvas->get_artist_name()+ " artist backend",
                              pawgui::theme_main->program_color_header, gpe::font_default, gpe::fa_center, gpe::fa_middle );
            return;
        }

        gpe::gcanvas->render_rect( view_space, pawgui::theme_main->program_color, false, 255 );


        if( tsPreviewer!=nullptr)
        {
            tsPreviewer->render_self( view_space,cam);
        }
            //gpe::gcanvas->render_rect( &tilesheetPreviewCam,pawgui::theme_main->button_box_color,true);
        gpe::renderer_main->reset_viewpoint();
        gpe::renderer_main->set_viewpoint( view_space);
    }
}

void tilesheetResource::save_resource(std::string file_path, int backupId)
{
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Saving Tilesheet",resource_name );
    }
    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = stg_ex::get_path_from_file(file_path);
    if( sff_ex::path_exists(soughtDir) )
    {
        newFileOut = file_path;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/tilesheets/";
        newFileOut = soughtDir + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);


            if(preloadCheckBox!=nullptr)
            {
                newSaveDataFile << "Preload=" << preloadCheckBox->is_clicked() << "\n";
            }
            else
            {
                newSaveDataFile << "Preload=1\n";
            }
            if(imageUsesColorKey!=nullptr)
            {
                newSaveDataFile << "TransparentImage=" << imageUsesColorKey->is_clicked() << "\n";
            }
            else
            {
                newSaveDataFile << "TransparentImage=1\n";
            }

            if(imageColorKey!=nullptr)
            {
                newSaveDataFile << "ColorKeyValue=" << imageColorKey->get_rgb_string() << ",\n";
            }
            else
            {
                newSaveDataFile << "ColorKeyValue=255,0,255,\n";
            }

            if( tilesheetInEditor!=nullptr)
            {
                std::string resfile_location = stg_ex::get_short_filename(tilesheetInEditor->file_nameLocation,true);
                newSaveDataFile << "ImageLocation="+resfile_location+"\n";
                if( (int)resfile_location.size() > 0 && usingAltSaveSource )
                {
                    std::string resFileCopySrc = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/tilesheets/"+resfile_location;
                    std::string resFileCopyDest = soughtDir+resfile_location;
                    if( sff_ex::file_exists(resFileCopyDest) )
                    {
                        /*
                        if( pawgui::display_prompt_message("[WARNING]Tilesheet Image Already exists?","Are you sure you will like to overwrite your ["+resfile_location+"] Tilesheet File? This action is irreversible!")==pawgui::display_query_yes)
                        {
                            sff_ex::file_copy(resFileCopySrc,resFileCopyDest);
                        }
                        */
                    }
                    else
                    {
                        sff_ex::file_copy(resFileCopySrc,resFileCopyDest);
                    }
                }
            }

            for( int i = 0; i < TILESHEET_DATA_FIELD_COUNT; i++)
            {
                newSaveDataFile << "TilesheetData[" << stg_ex::int_to_string(i)<< "]=";

                if( tsDataFields[i]!=nullptr)
                {
                    newSaveDataFile << stg_ex::int_to_string( tsDataFields[i]->get_held_number() )<<"\n";
                }
                else
                {
                    newSaveDataFile << "0\n";
                }
            }
            newSaveDataFile.close();
            if( !usingAltSaveSource)
            {
                isModified = false;
            }
        }
        else
        {
            main_editor_log->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        main_editor_log->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void tilesheetResource::update_box(int x_new, int y_new, int newW, int newH)
{
    if( x_new!=-1)
    {
        encapBox.x = x_new;
    }
    if(y_new!=-1)
    {
        encapBox.y = y_new;
    }

    if( newW!=-1)
    {
        encapBox.w = newW;
    }
    if(newH!=-1)
    {
        encapBox.h = newH;
    }
}

bool tilesheetResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Tilesheet=" << resource_name << "," << get_global_rid() << ",";

            *fileTarget << "\n";
            return true;
        }
    }
    //save_resource();
    return false;
}

