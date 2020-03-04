/*
tilesheet_resource.cpp
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

#include "tilesheet_resource.h"
#include "gpe_editor_settings.h"

std::string tsDataLabels[TILESHEET_DATA_FIELD_COUNT];
int tsDataLabelWidth;

tilesheetPreviewer::tilesheetPreviewer()
{
    areaIsScrollable = false;
    zoomValue = 1;
    minZoomValue = 0.125;
    maxZoomValue = 8;
    areaMouseXPos = 0;
    areaMouseYPos = 0;
    showGrid = true;
    tileSheetToPreview = NULL;

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
    previewXScroll = new GPE_ScrollBar_XAxis();
    previewYScroll = new GPE_ScrollBar_YAxis();
}

tilesheetPreviewer::~tilesheetPreviewer()
{
    tilesIdsInPreview.clear();
    if( previewXScroll!=NULL)
    {
        delete previewXScroll;
        previewXScroll = NULL;
    }
    if( previewYScroll!=NULL)
    {
        delete previewYScroll;
        previewYScroll = NULL;
    }
}

/*
For now this will be essentially a copy/paste of the gameSceneResource object with varaible name changes.
If zooming is added to all gui elements this will go to the parent object with all elements containing zoom data
For now this works for its needed purpose
*/

bool tilesheetPreviewer::get_mouse_coords( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    areaMouseXPos = 0;
    areaMouseYPos = 0;
    if( viewedSpace!=NULL)
    {
        if( gpe::point_within( gpe::input->mouse_position_x,gpe::input->mouse_position_y,
                         tsEditorViewRect.x+viewedSpace->x,
                         tsEditorViewRect.y+viewedSpace->y,
                         tsEditorViewRect.x+viewedSpace->x+tsEditorViewRect.w,
                         tsEditorViewRect.y+viewedSpace->y+tsEditorViewRect.h ) )
        {
            areaMouseXPos = (gpe::input->mouse_position_x-tsEditorViewRect.x-viewedSpace->x )/zoomValue +tsCameraRect.x - cam->x;
            areaMouseYPos = (gpe::input->mouse_position_y-tsEditorViewRect.y-viewedSpace->y )/zoomValue +tsCameraRect.y - cam->y;
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
        previewXScroll->process_self(NULL,NULL );
    }
    if( yScrollHappened)
    {
        previewYScroll->contextRect.y = tsCameraRect.y;
        previewYScroll->process_self(NULL,NULL );
    }
}

void tilesheetPreviewer::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    //sets viewedspace and cam to the default camera(entire screen if they are NULL )
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);

    tsEditorViewRect.x = widget_box.x;
    tsEditorViewRect.y = widget_box.y;
    tsEditorViewRect.w = tsCameraRect.w =widget_box.w-16;
    tsEditorViewRect.h = tsCameraRect.h =widget_box.h-16;

    tsRect.x = 0;
    tsRect.y = 0;
    tsRect.w = tsEditorViewRect.w;
    tsRect.h = tsEditorViewRect.h;

    //checks if the element has arrow and/or scroll control
    if( isClicked && RESOURCE_TO_DRAG==NULL )
    {
        hasScrollControl = true;
        hasArrowkeyControl = true;
    }
    else if( clickedOutside)
    {
        hasScrollControl = false;
        hasArrowkeyControl = false;
    }

    //If the loaded tilesheet is not NULL, continue with the logic below
    if(tileSheetToPreview!=NULL && tileSheetToPreview->tsImage!=NULL && cam!=NULL && viewedSpace!=NULL )
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
        previewXScroll->process_self(viewedSpace,cam );
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
        previewYScroll->process_self(viewedSpace,cam );
        //if( sceneYScroll->has_moved() )
        {
            tsCameraRect.y = float(previewYScroll->contextRect.y);
        }
        //yTileScroll->fullRect.w = tileSheetToPreview->tsImage->get_width()*zoomValue;

        if( get_mouse_coords(viewedSpace, cam) )
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
            if( previewXScroll->is_scrolling()==false && previewYScroll->is_scrolling()==false  && RESOURCE_TO_DRAG==NULL )
            {
                if( areaIsScrollable )
                {
                    GPE_main_Statusbar->set_codestring( "Mouse (X:"+ stg_ex::int_to_string(areaMouseXPos)+",Y:"+ stg_ex::int_to_string( areaMouseYPos)+")" );
                    gpe::shape_rect foundTSRect;
                    int iTSX = 0;

                    //If pressed finds the X/Y the mouse is in the tile-sheet by "found tile X/Y coordinae
                    //Only happens once on pressed...
                    if( gpe::input->check_mouse_pressed(0) )
                    {
                        tileToPrevX1 = areaMouseXPos;
                        tileToPrevY1 = areaMouseYPos;
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
                        tileToPrevX2 = areaMouseXPos;
                        tileToPrevY2 = areaMouseYPos;

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
                        GPE_open_context_menu(-1,-1,256);
                        if( showGrid)
                        {
                            main_CONTEXT_MENU->add_menu_option("Show Grid",1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/check.png"),-1,NULL,true,true,false);
                        }
                        else
                        {
                            main_CONTEXT_MENU->add_menu_option("Show Grid",1,NULL,-1,NULL,true,true,false );
                        }
                        int menuSelection = GPE_Get_Context_Result();
                        if( menuSelection==0 && GPE_main_TabManager!=NULL && tileSheetToPreview!=NULL)
                        {
                            //GPE_main_TabManager->add_new_tab(tileSheetToPreview);
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

void tilesheetPreviewer::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( tileSheetToPreview!=NULL && cam!=NULL && viewedSpace!=NULL )
    {
        if(tileSheetToPreview->tsImage!=NULL)
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

                    gpe::gcanvas->render_rectangle( widget_box.x+bx1-cam->x, widget_box.y+by1-cam->y, widget_box.x+bx2-cam->x, widget_box.y+by2-cam->y, theme_main->text_box_font_color,true,255);
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

                gpe::gcanvas->render_rectangle( widget_box.x+renderTSSelectX1-cam->x,widget_box.y+renderTSSelectY1-cam->y,widget_box.x+renderTSSelectX2-cam->x,widget_box.y+renderTSSelectY2-cam->y,theme_main->button_box_highlight_color,false,128);

            }
            gpe::gfs->render_text_boxed( widget_box.x+widget_box.w-32,widget_box.y+widget_box.h-cam->y-32,
                              "Zoom Level: "+ stg_ex::float_to_string(zoomValue*100 )+"%",
                              theme_main->text_box_font_color, gpe::c_black, font_default,gpe::fa_right,gpe::fa_bottom);
            if( previewXScroll!=NULL)
            {
                previewXScroll->render_self( viewedSpace,cam);
            }
            if( previewYScroll!=NULL)
            {
                previewYScroll->render_self( viewedSpace,cam);
            }
        }
    }
}

void tilesheetPreviewer::render_selection( int xPos, int yPos, gpe::shape_rect * viewedSpace, gpe::shape_rect * cam, float scaleSize, gpe::color * fColor)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( tileSheetToPreview!=NULL && cam!=NULL && viewedSpace!=NULL && scaleSize!=0 )
    {
        if(tileSheetToPreview->tsImage!=NULL)
        {
            if( tsselectedArea.x >=0 && tsselectedArea.y >=0 && tsselectedArea.w  >0 && tsselectedArea.h > 0 )
            {
                if( tsselectedArea.x+tsselectedArea.w <= tileSheetToPreview->tsImage->get_width() && tsselectedArea.y+tsselectedArea.h <= tileSheetToPreview->tsImage->get_height()  )
                {
                    tileSheetToPreview->tsImage->render_tex_scaled( xPos-cam->x,yPos-cam->y,scaleSize, scaleSize,&tsselectedArea,fColor );
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

tilesheetResource::tilesheetResource(GPE_GeneralResourceContainer * pFolder)
{
    projectParentFolder = pFolder;
    editorMode = 0;
    tilesheetInEditor = NULL;
    tsPreviewer = new tilesheetPreviewer();
    preloadCheckBox = new GPE_CheckBoxBasic("Preload tilesheet","Check to load tilesheet at game open", true);
    isPreloaded = true;
    if(saveResourceButton!=NULL)
    {
        //saveResourceButton->disable_self();
    }
    int i = 0;
    for( i =0; i < TILESHEET_DATA_FIELD_COUNT; i++)
    {
        tsDataFields[i] = new gpe_text_widget_number("0",true,0,2048);
        tsDataFields[i] ->set_label(tsDataLabels[i]);
    }
    transformResourceButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magic.png","Transform the Image",-1);
    tilesheetDimensionsStr = new GPE_Label_Text("Image Size: 0x0px");

    openExternalEditorButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Tilesheet Image In External Editor");
    refreshResourceDataButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png", "Refreshes the loaded tilesheet image");
    labelInfoMaxTextureSize = new GPE_Label_Text("Max Image Size: 4096 X 4096px","Max Image Size: 4096 X 4096px");
}

tilesheetResource::~tilesheetResource()
{
    if( labelInfoMaxTextureSize!=NULL)
    {
        delete labelInfoMaxTextureSize;
        labelInfoMaxTextureSize = NULL;
    }
    if( openExternalEditorButton!=NULL)
    {
        delete openExternalEditorButton;
        openExternalEditorButton = NULL;
    }
    if( refreshResourceDataButton!=NULL)
    {
        delete refreshResourceDataButton;
        refreshResourceDataButton = NULL;
    }

    if( preloadCheckBox!=NULL)
    {
        delete preloadCheckBox;
        preloadCheckBox = NULL;
    }
    if( tilesheetDimensionsStr!=NULL)
    {
        delete tilesheetDimensionsStr;
        tilesheetDimensionsStr = NULL;
    }
    for( int i =TILESHEET_DATA_FIELD_COUNT-1; i >=0; i--)
    {
        if( tsDataFields[i]!=NULL)
        {
            delete tsDataFields[i];
            tsDataFields[i] = NULL;
        }
    }
    if( transformResourceButton!=NULL)
    {
        delete transformResourceButton;
        transformResourceButton = NULL;
    }
    if( tsPreviewer!=NULL)
    {
        delete tsPreviewer;
        tsPreviewer = NULL;
    }
}

bool tilesheetResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string html5TSName = get_name();
        if( tilesheetInEditor!=NULL)
        {
            *fileTarget << nestedTabsStr << "var " << html5TSName << " =  GPE.rsm.add_tilesheet(";
            *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) +",";
            *fileTarget << "'resources/tilesheets/"+stg_ex::get_short_filename (tilesheetInEditor->fileNameLocation,true ) +"',";
            *fileTarget << stg_ex::int_to_string (tilesheetInEditor->tsWidth ) +",";
            *fileTarget << stg_ex::int_to_string (tilesheetInEditor->tsHeight ) +",";
            *fileTarget << stg_ex::int_to_string (tilesheetInEditor->tsXOff ) +",";
            *fileTarget << stg_ex::int_to_string (tilesheetInEditor->tsYOff ) +",";
            *fileTarget << stg_ex::int_to_string (tilesheetInEditor->tsXPadding ) +",";
            *fileTarget << stg_ex::int_to_string (tilesheetInEditor->tsYPadding );
            *fileTarget << "); \n";
        }
        else
        {
            *fileTarget << nestedTabsStr << "var " << html5TSName << " = GPE.rsm.add_tilesheet( -1 );\n";
        }
    }
    return false;
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
    if( tilesheetInEditor!=NULL && tilesheetInEditor->tsImage!=NULL)
    {
        return tilesheetInEditor->tsImage;
    }
    return NULL;
}

bool tilesheetResource::include_local_files( std::string pBuildDir , int buildType )
{
    if( tilesheetInEditor!=NULL && tilesheetInEditor->tsImage!=NULL )
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
    if( tilesheetInEditor == NULL)
    {
        recentErrorMessage = "tilesheetInEditor=NULL;";
        return false;
    }
    bool isReady = true;
    for( int i = 0; i  < TILESHEET_DATA_FIELD_COUNT; i++)
    {
        if( tsDataFields[i]== NULL )
        {
            recentErrorMessage += "tsDataFields["+ stg_ex::int_to_string(i)+"] = NULL;";
            isReady = false;
        }
    }

    if( (int)tilesheetInEditor->fileNameLocation.size() == 0 )
    {
        recentErrorMessage += "fileNameLocation=EMPTY;";
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

void tilesheetResource::load_image(std::string new_file_name)
{
    if( stg_ex::file_is_image(new_file_name) )
    {
        gpe::texture_base * tempTexture =  gpe::rph->get_new_texture();
        tempTexture->load_new_texture(  gpe::renderer_main,new_file_name, -1, true);
        if( tempTexture->get_width()>0 )
        {
            if(tilesheetInEditor==NULL)
            {
                tilesheetInEditor = new gpe::tilesheet();
            }
            if(tilesheetInEditor->tsImage!=NULL)
            {
                delete tilesheetInEditor->tsImage;
                tilesheetInEditor->tsImage = NULL;
            }
            if( tilesheetInEditor!=NULL)
            {
                tilesheetInEditor->tsImage = tempTexture;
                if(tilesheetInEditor->tsImage!=NULL)
                {
                    tilesheetInEditor->tsImage->load_new_texture( gpe::renderer_main, new_file_name, -1, true );
                    if( tilesheetInEditor->tsImage->get_width()>0 )
                    {
                        if( tilesheetInEditor->tsImage->get_width() <1 || tilesheetInEditor->tsImage->get_width()>4096 || tilesheetInEditor->tsImage->get_height() <1 || tilesheetInEditor->tsImage->get_height()>4096)
                        {
                            display_user_alert("Unable to load image","Editor Error: Unable to load ["+new_file_name+"] please check file and make sure it is between 1x1 and 4096x4096 pixels and is a valid image");
                            if( tilesheetInEditor->tsImage!=NULL)
                            {
                                delete tilesheetInEditor->tsImage;
                                tilesheetInEditor->tsImage = NULL;
                            }
                        }
                        else
                        {
                            //successfully added new image.
                            tilesheetInEditor->tsRects.clear();
                            tilesheetInEditor->fileNameLocation = new_file_name;
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
                        }
                    }
                }
            }
        }
        else
        {
            display_user_alert("Tilesheet Edit Log","Unable to load image" );
        }
    }
    else
    {
        display_user_alert("Unable to load image","File type["+stg_ex::get_file_ext(new_file_name)+"] not supported when loading ["+new_file_name+"].");
    }
}


void tilesheetResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed ==false  || sff_ex::file_exists(file_path) )
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Tilesheet",resource_name );
        }

        std::string otherColContainerName = "";

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
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
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
                                    keyString = currLineToBeProcessed.substr(0,equalPos);
                                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( keyString=="Version")
                                    {
                                        foundFileVersion = stg_ex::string_to_float(valString);
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
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( keyString=="ResourceName")
                                {
                                    renameBox->set_string(valString);
                                }
                                else if( keyString=="ImageLocation")
                                {
                                    if( valString!="NULL")
                                    {
                                        load_image( soughtDir+valString );
                                    }
                                    tilesheetDimensionsStr->set_name("Image Size: "+ stg_ex::int_to_string(tilesheetInEditor->tsImage->get_width() )+" x "+ stg_ex::int_to_string(tilesheetInEditor->tsImage->get_height() )+"px" );
                                }
                                else if( keyString=="Preload")
                                {
                                    preloadCheckBox->set_checked( stg_ex::string_to_bool(valString) );
                                }
                                else
                                {
                                    for( i = 0; i <TILESHEET_DATA_FIELD_COUNT ; i++)
                                    {
                                        tsDataStr = "TilesheetData["+ stg_ex::int_to_string(i)+"]";
                                        if( keyString==tsDataStr)
                                        {
                                            tsDataFields[i]->set_string(valString);
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
    }
    process_data_fields();
}

void tilesheetResource::process_data_fields()
{
    ///constantly updates the tilesheet boxes
    bool allInputsAreValid = true;
    if( tilesheetInEditor!=NULL)
    {
        for(  int i =0; i < TILESHEET_DATA_FIELD_COUNT; i++)
        {
            if( tsDataFields[i]!=NULL)
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
    if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self( );
    }
}

void tilesheetResource::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    int i;
    bool editorHasControl = false;
    if( cam!=NULL && viewedSpace!=NULL )
    {
        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            PANEL_GENERAL_EDITOR->clear_panel();
            PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);
            PANEL_GENERAL_EDITOR->add_gui_element(refreshResourceDataButton,false );
            PANEL_GENERAL_EDITOR->add_gui_element(loadResourceButton,false );
            PANEL_GENERAL_EDITOR->add_gui_element(transformResourceButton,false );
            PANEL_GENERAL_EDITOR->add_gui_element( openExternalEditorButton,true);

            //PANEL_GENERAL_EDITOR->add_gui_element(saveResourceButton,true);
            PANEL_GENERAL_EDITOR->add_gui_element(tilesheetDimensionsStr,true);
            PANEL_GENERAL_EDITOR->add_gui_element(labelInfoMaxTextureSize,true);

            //processes the tile data fields
            for(  i =0; i < TILESHEET_DATA_FIELD_COUNT; i++)
            {
                if( tsDataFields[i]!=NULL)
                {
                    PANEL_GENERAL_EDITOR->add_gui_element(tsDataFields[i],true);
                }
            }
            PANEL_GENERAL_EDITOR->add_gui_element(preloadCheckBox,true);
            PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);

            PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
            //PANEL_GENERAL_EDITOR->set_maxed_out_width();
            editorHasControl = tsPreviewer->hasScrollControl;
            if( !editorHasControl )
            {
                PANEL_GENERAL_EDITOR->process_self(NULL, NULL);
            }
            else
            {
                PANEL_GENERAL_EDITOR->hasScrollControl = false;
                PANEL_GENERAL_EDITOR->hasArrowkeyControl = false;
            }

            if( confirmResourceButton->is_clicked() )
            {
                save_resource();
                isPreloaded = preloadCheckBox->is_clicked();
            }
            else if( cancelResourceButton->is_clicked() )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== DISPLAY_QUERY_YES )
                {
                    resourcePostProcessed = false;
                    load_resource();
                }
            }
            else if( loadResourceButton!=NULL && loadResourceButton->is_clicked() )
            {
                std::string new_file_name = GPE_GetOpenFileName("Load Your tilesheet Image","Image",main_GUI_SETTINGS->fileOpenTilesheetDir);
                if( (int)new_file_name.size() > 3)
                {
                    load_image(new_file_name);
                }
            }
            else if( refreshResourceDataButton!=NULL && refreshResourceDataButton->is_clicked() )
            {
                if( tilesheetInEditor!=NULL  )
                {
                    resourcePostProcessed = false;
                    load_resource();
                }
            }
            else if( transformResourceButton!=NULL && transformResourceButton->is_clicked() )
            {
                if( tilesheetInEditor!=NULL && tilesheetInEditor->tsImage!=NULL)
                {
                    if( tilesheetInEditor->tsImage->get_width() > 0 && tilesheetInEditor->tsImage->get_height() > 0)
                    {
                        GPE_open_context_menu(-1,-1,256);
                        main_CONTEXT_MENU->add_menu_option("Erase BG Color",0);
                        main_CONTEXT_MENU->add_menu_option("Invert Colors",1);
                        main_CONTEXT_MENU->add_menu_option("Make GrayScale",2);
                        main_CONTEXT_MENU->add_menu_option("Exit",10);
                        int menuSelection = GPE_Get_Context_Result();
                        if( menuSelection>=0 && menuSelection <=3)
                        {
                            std::string tempStr = tilesheetInEditor->tsImage->get_filename();
                            SDL_Surface * oTempSurface = sdl_surface_ex::load_surface_image( tempStr.c_str() );
                            SDL_Surface *nTempSurface = NULL;
                            if( oTempSurface!=NULL)
                            {
                                if( menuSelection==0)
                                {
                                    gpe::color * foundBGColor = gpe::c_fuchsia->duplicate_color();
                                    if( GPE_Change_color_PopUp("Image Background Color To Remove",foundBGColor) )
                                    {
                                        if( GPE_Display_Basic_Prompt("Are you sure you want to erase this Color from this image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                        {
                                            gpe::error_log->report("Modifying image at: "+tilesheetInEditor->tsImage->get_filename()+".");
                                            nTempSurface= sdl_surface_ex::surface_remove_color_rgba(oTempSurface, foundBGColor->get_r(), foundBGColor->get_r(), foundBGColor->get_b() );

                                        }
                                    }
                                    delete foundBGColor;
                                    foundBGColor = NULL;
                                }
                                else if( menuSelection==1 )
                                {
                                    if( GPE_Display_Basic_Prompt("Are you sure you want to invert your image's colors?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                    {
                                        nTempSurface = sdl_surface_ex::surface_invert(oTempSurface);
                                    }
                                }
                                else if( menuSelection==2 )
                                {
                                    if( GPE_Display_Basic_Prompt("Are you sure you want to grayscale your image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                    {
                                        nTempSurface= sdl_surface_ex::surface_grayscale(oTempSurface);
                                    }
                                }
                                if( nTempSurface!=NULL)
                                {
                                    remove( tilesheetInEditor->fileNameLocation.c_str() );
                                    std::string newImageName = stg_ex::get_file_noext(tilesheetInEditor->tsImage->get_filename())+".png";
                                    IMG_SavePNG(nTempSurface,newImageName.c_str() );
                                    tilesheetInEditor->fileNameLocation = newImageName;
                                    tilesheetInEditor->tsImage = gpe::rsm->texture_add_filename(  newImageName);
                                    SDL_FreeSurface(nTempSurface);
                                    nTempSurface = NULL;
                                }
                                SDL_FreeSurface(oTempSurface);
                                oTempSurface = NULL;
                            }
                        }
                    }
                }
            }
            else if( openExternalEditorButton!=NULL && openExternalEditorButton->is_clicked() )
            {
                if( tilesheetInEditor!=NULL )
                {
                    if( sff_ex::file_exists(tilesheetInEditor->fileNameLocation ) )
                    {
                        if( main_EDITOR_SETTINGS!=NULL && main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=NULL)
                        {
                            gpe::external_open_program(main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),tilesheetInEditor->fileNameLocation, true  );
                        }
                        else
                        {
                            gpe::external_open_url(tilesheetInEditor->fileNameLocation );
                        }
                        sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+tilesheetInEditor->fileNameLocation+"]...");
                    }
                }
            }
        }

        if(renameBox!=NULL)
        {
            if( renameBox->get_string()!=resource_name)
            {
                isModified = true;
            }
        }
        process_data_fields();
        if( tsPreviewer!=NULL && tilesheetInEditor!=NULL)
        {
            tsPreviewer->set_coords(0,0);
            tsPreviewer->set_width(viewedSpace->w );
            tsPreviewer->set_height(viewedSpace->h );
            tsPreviewer->tileSheetToPreview = tilesheetInEditor;
            if( !editorHasControl )
            {
                tsPreviewer->process_self(viewedSpace,cam);
            }
            else
            {
                tsPreviewer->hasScrollControl = false;
                tsPreviewer->hasArrowkeyControl = false;
            }
        }


    }
}

void tilesheetResource::render_self( gpe::shape_rect *viewedSpace, gpe::shape_rect *cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    bool texturePreviewIsRendered = false;
    if( cam!=NULL && viewedSpace!=NULL )
    {
        if( tsPreviewer!=NULL)
        {
            tsPreviewer->render_self( viewedSpace,cam);
        }
            //gpe::gcanvas->render_rect( &tilesheetPreviewCam,theme_main->button_box_color,true);
        gpe::renderer_main->reset_viewpoint();
        gpe::renderer_main->set_viewpoint( viewedSpace);
    }
}

void tilesheetResource::save_resource(std::string file_path, int backupId)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Tilesheet",resource_name );
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

            if( tilesheetInEditor!=NULL)
            {
                std::string resFileLocation = stg_ex::get_short_filename(tilesheetInEditor->fileNameLocation,true);
                newSaveDataFile << "ImageLocation="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                {
                    std::string resFileCopySrc = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/tilesheets/"+resFileLocation;
                    std::string resFileCopyDest = soughtDir+resFileLocation;
                    if( sff_ex::file_exists(resFileCopyDest) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]Tilesheet Image Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] Tilesheet File? This action is irreversible!")==DISPLAY_QUERY_YES)
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
            if(preloadCheckBox!=NULL)
            {
                newSaveDataFile << "Preload=" << preloadCheckBox->is_clicked() << "\n";
            }
            else
            {
                newSaveDataFile << "Preload=1\n";
            }
            for( int i = 0; i < TILESHEET_DATA_FIELD_COUNT; i++)
            {
                newSaveDataFile << "TilesheetData[" << stg_ex::int_to_string(i)<< "]=";

                if( tsDataFields[i]!=NULL)
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
            GPE_main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void tilesheetResource::update_box(int newX, int newY, int newW, int newH)
{
    if( newX!=-1)
    {
        encapBox.x = newX;
    }
    if(newY!=-1)
    {
        encapBox.y = newY;
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
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Tilesheet=" << resource_name << "," << get_global_rid() << ",";

            *fileTarget << "\n";
            return true;
        }
    }
    //save_resource();
    return false;
}

