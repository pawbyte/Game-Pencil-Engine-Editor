/*
gpe_dock_system.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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


#include "gpe_dock_system.h"



gpeEditorDock * GPE_DOCK = NULL;
gpeEditorDockPanel * PANEL_RESOURCE_TREE = NULL;
gpeEditorDockPanel * PANEL_GENERAL_EDITOR = NULL;
gpeEditorDockPanel * PANEL_INSPECTOR = NULL;
gpeEditorDockPanel * PANEL_META = NULL;

gpeEditorDockPanel::gpeEditorDockPanel()
{
    specialPanelElement = NULL;
    panelBar = new GPE_TabBar(true);
    panelRect = new GPE_Rect();
    elementBox.w = 0;
    wasProcessed = false;
    panelGuiList = new GPE_GuiElementList();
    panelGuiList->set_horizontal_align(FA_LEFT);
    panelGuiList->barXPadding = GENERAL_GPE_PADDING;
    panelGuiList->barXMargin = 0;
    containerRect = new GPE_Rect();
    dockSettingsButton = new GPE_ToolIconButton(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/bars.png","Panel Settings");
}

gpeEditorDockPanel::~gpeEditorDockPanel()
{
    if( panelBar!=NULL )
    {
        delete panelBar;
        panelBar = NULL;
    }

    if( dockSettingsButton!=NULL )
    {
        delete dockSettingsButton;
        dockSettingsButton = NULL;
    }

    if( panelRect!=NULL )
    {
        delete panelRect;
        panelRect = NULL;
    }

    if( containerRect!=NULL )
    {
        delete containerRect;
        containerRect = NULL;
    }

    if( panelGuiList!=NULL )
    {
        delete panelGuiList;
        panelGuiList = NULL;
    }
}

bool gpeEditorDockPanel::add_container( std::string name, bool openNew )
{
    if( (int)name.size() > 0 && specialPanelElement == NULL )
    {
        panelBar->add_new_tab( name, openNew );
        if( elementBox.w <= DOCK_COLUMN_MIN_WIDTH )
        {
            elementBox.w = DOCK_COLUMN_MIN_WIDTH;
        }
        return true;
    }
    return false;
}

void gpeEditorDockPanel::add_gui_element(GPE_GeneralGuiElement * element, bool isNLElement)
{
    if( panelGuiList!=NULL )
    {
        panelGuiList->add_gui_element( element, isNLElement);
    }
}

void gpeEditorDockPanel::add_gui_auto(GPE_GeneralGuiElement * element)
{
    if( panelGuiList!=NULL )
    {
        panelGuiList->add_gui_auto( element );
    }
}

void gpeEditorDockPanel::add_gui_element_fullsize(GPE_GeneralGuiElement * element )
{
    if( panelGuiList!=NULL && element!=NULL)
    {
        panelGuiList->add_gui_element_fullsize( element);
    }
}

void gpeEditorDockPanel::clear_panel()
{
    if( panelGuiList!=NULL )
    {
        panelGuiList->clear_list();
    }
}

bool gpeEditorDockPanel::container_in_view( std::string name )
{
    if( panelBar!=NULL )
    {
        if( panelBar->get_selected_name() == name )
        {
            return true;
        }
    }
    return false;
}

int gpeEditorDockPanel::get_panel_height()
{
    return panelRect->h;
}

std::string gpeEditorDockPanel::get_selected_container()
{
    if( panelBar!=NULL )
    {
        return panelBar->get_selected_name();
    }
    return "";
}

bool gpeEditorDockPanel::has_content()
{
    if( specialPanelElement !=NULL)
    {
        return true;
    }
    else if( panelBar!=NULL)
    {
        return ( panelBar->get_tab_count() > 0 );
    }
    return false;
}

void gpeEditorDockPanel::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam  )
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );
    if( panelBar!=NULL)
    {
        panelRect->x = elementBox.x;
        panelRect->y = elementBox.y + panelBar->get_height();
        panelRect->w = elementBox.w;
        panelRect->h = elementBox.h - panelBar->get_height();

        if( wasProcessed == false )
        {
            if( specialPanelElement!=NULL )
            {
                specialPanelElement->set_coords(elementBox.x, elementBox.y );
                specialPanelElement->set_width( elementBox.w );
                specialPanelElement->set_height( elementBox.h );
                specialPanelElement->process_self( NULL, cam );
            }
            else
            {
                //For normal tabs
                //Processes the gui element list
                if( panelGuiList!=NULL )
                {
                    panelGuiList->set_coords(0, 0);
                    panelGuiList->set_width( panelRect->w );
                    panelGuiList->set_height( panelRect->h );
                    panelGuiList->process_self( panelRect, cam  );
                }
                //Processes the settings button on normal panel
                if( dockSettingsButton!=NULL && panelBar->get_tab_count()!=0 )
                {
                    if( wasProcessed==false )
                    {
                        dockSettingsButton->set_clicked( false );
                    }
                    dockSettingsButton->set_coords( elementBox.w-panelBar->get_height(), 0 );
                    dockSettingsButton->set_width(  panelBar->get_height() );
                    dockSettingsButton->set_height(  panelBar->get_height() );
                    dockSettingsButton->process_self( &elementBox );
                }
            }
        }
        wasProcessed = true;
    }
}

bool gpeEditorDockPanel::remove_container( std::string name )
{
    if( (int)name.size() > 0 )
    {
        if( panelBar!=NULL )
        {
            if( panelBar->get_selected_name()==name )
            {
                panelGuiList->reset_self();
            }
            return panelBar->remove_tab( name );
        }
    }
    return false;
}

void gpeEditorDockPanel::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw   )
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );
    if( has_content() == false)
    {
        return;
    }
    MAIN_RENDERER->reset_viewpoint();
    if( specialPanelElement !=NULL )
    {
        specialPanelElement->render_self( NULL, NULL, forceRedraw );
    }
    else if( panelBar!=NULL )
    {
        MAIN_RENDERER->reset_viewpoint();
        //MAIN_RENDERER->set_viewpoint( panelRect );
        if( panelGuiList!=NULL )
        {
            panelGuiList->render_self( panelRect, NULL, forceRedraw );
        }
        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint( &elementBox);
        if( dockSettingsButton!=NULL && panelBar->get_tab_count()!=0 )
        {
            dockSettingsButton->render_self( &elementBox, NULL, forceRedraw );
        }
        panelBar->render_self( &elementBox, NULL, forceRedraw );
    }
    MAIN_RENDERER->reset_viewpoint();
    //gcanvas->render_rectangle( tPanel->get_xpos(), input->mouse_y-4, tPanel->get_x2pos(),input->mouse_y+4, GPE_MAIN_THEME->PopUp_Box_Border_Color, false );
}

void gpeEditorDockPanel::reset_panel()
{
    specialPanelElement = NULL;
    if( panelGuiList!=NULL )
    {
        panelGuiList->reset_self();
    }

    if( panelBar!=NULL )
    {
        panelBar->remove_all_tabs();
    }
}

void gpeEditorDockPanel::setup_panel(  bool addSpacing, bool clearList  )
{
    wasProcessed = false;
    if( specialPanelElement!=NULL )
    {
        specialPanelElement->set_coords(elementBox.x, elementBox.y );
        specialPanelElement->set_width( elementBox.w );
        specialPanelElement->set_height( elementBox.h );
    }
    else if( panelBar!=NULL)
    {
        panelBar->set_coords(0, 0);
        panelBar->set_width( elementBox.w -32);
        panelBar->process_self( &elementBox );

        panelRect->x = elementBox.x;
        panelRect->y = elementBox.y+panelBar->get_height();
        panelRect->w = elementBox.w;
        panelRect->h = elementBox.h - panelBar->get_height();

        if( panelGuiList!=NULL )
        {
            panelGuiList->set_horizontal_align( FA_LEFT );
            panelGuiList->set_coords( panelRect->x, panelRect->y );
            panelGuiList->set_width( panelRect->w );
            panelGuiList->set_height( panelRect->h );
            if( clearList )
            {
                panelGuiList->clear_list();
            }
            if( addSpacing )
            {
                panelGuiList->barXPadding = GENERAL_GPE_PADDING;
                panelGuiList->barYPadding = GENERAL_GPE_PADDING;
                panelGuiList->barXMargin = GENERAL_GPE_PADDING;
                panelGuiList->barYMargin = GENERAL_GPE_PADDING;
            }
        }
    }
}

gpeEditorDock::gpeEditorDock()
{
    toolbarOptonsHolder = new GPE_PopUpMenu_Option("Manage Dock Panels",-1,false, false, false );
    toolbarOptonsHolder->set_texture( guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/cogs.png") );
    toolbarOptonsHolder->add_menu_option("Toggle Logs",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    toolbarOptonsHolder->add_menu_option("Toggle Editor Pane",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    toolbarOptonsHolder->add_menu_option("Toggle ResourcesBar",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    toolbarOptonsHolder->add_menu_option("Toggle Inspector",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    toolbarOptonsHolder->add_menu_option("Toggle Meta",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);

    isHidden = false;
    columnBeingResizedId = -1;
    horiResizeStartX = 0;
    justResized = beingResized = horizontalResize = verticalResize = false;

    for( int iColumn = 0; iColumn < DOCK_MAX_COLUMN_COUNT; iColumn++ )
    {
        panelColumnWidth[iColumn] = DOCK_COLUMN_DEFAULT_WIDTH;
        panelWidthPercentages[ iColumn ] = 1.d / (double)DOCK_MAX_COLUMN_COUNT;
    }
    if( SCREEN_WIDTH > DOCK_COLUMN_DEFAULT_WIDTH * DOCK_MAX_COLUMN_COUNT)
    {
        panelColumnWidth[DOCK_COLUMN_DEFAULT_WIDTH * DOCK_MAX_COLUMN_COUNT] = SCREEN_WIDTH - ( DOCK_COLUMN_DEFAULT_WIDTH * (DOCK_MAX_COLUMN_COUNT-1) );
    }

    GPE_Report( "Attempt to create ["+int_to_string(DOCK_MAX_PANEL_COUNT)+"] panels...");
    for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
    {
        GPE_Report("Adding Pane["+ int_to_string(iPanel) +"]..");
        panelHeightPercentages[iPanel] = 0.d;
        defaultPanelNames[ iPanel ] = "";
        panels[ iPanel ] = new gpeEditorDockPanel();
    }

    defaultPanelNames[DOCK_TOP_LEFT] = "DOCK_TOP_LEFT";
    defaultPanelNames[DOCK_BOTTOM_LEFT] = "DOCK_BOTTOM_LEFT";

    defaultPanelNames[DOCK_TOP_LEFT_CENTER] = "DOCK_TOP_LEFT_CENTER";
    defaultPanelNames[DOCK_BOTTOM_LEFT_CENTER] = "DOCK_BOTTOM_LEFT_CENTER";

    defaultPanelNames[DOCK_TOP_MIDDLE] = "DOCK_TOP_MIDDLE";
    defaultPanelNames[DOCK_BOTTOM_MIDDLE] = "DOCK_BOTTOM_MIDDLE";

    defaultPanelNames[DOCK_TOP_RIGHT_CENTER] = "DOCK_TOP_RIGHT_CENTER";
    defaultPanelNames[DOCK_BOTTOM_RIGHT_CENTER] = "DOCK_BOTTOM_RIGHT_CENTER";

    defaultPanelNames[DOCK_TOP_RIGHT] = "DOCK_TOP_RIGHT";
    defaultPanelNames[DOCK_BOTTOM_RIGHT] = "DOCK_BOTTOM_RIGHT";

    panels[ DOCK_TOP_MIDDLE ]->specialPanelElement = GPE_Main_TabManager;
    panels[ DOCK_BOTTOM_MIDDLE ]->specialPanelElement = GPE_Main_Logs;
    GPE_Report("Resettings Panels for first time..");
    reset_dock();
}

gpeEditorDock::~gpeEditorDock()
{

}


void gpeEditorDock::add_default_panel( std::string name, int panelId, bool open )
{
    GPE_KeyPair * newPair = new GPE_KeyPair( panelId, name, name);
    defaultPanels.push_back( newPair);
    if( open)
    {
        add_to_panel( name, panelId, open);
    }
    update_toolbar();
}

void gpeEditorDock::add_to_panel( std::string name, int panelId, bool open )
{
    if( panelId >=0 && panelId < DOCK_MAX_PANEL_COUNT && (int)name.size() > 0 )
    {
        //Removes container from all other panels
        gpeEditorDockPanel *  currentPanel = NULL;
        gpeEditorDockPanel *  otherPanel = NULL;
        for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
        {
            currentPanel = panels[iPanel];
            if( currentPanel!=NULL )
            {
                currentPanel->remove_container( name );
            }
        }
        //Resizes the width to DOCK_COLUMN_MIN_WIDTH if its "unopen"
        int assignedColumn = get_column( panelId );
        if( assignedColumn < 0 )
        {
            return;
        }
        currentPanel = panels[panelId];
        if( panelId < DOCK_MAX_COLUMN_COUNT )
        {
            otherPanel = panels[panelId + DOCK_MAX_COLUMN_COUNT];
        }
        else
        {
           otherPanel = panels[ panelId - DOCK_MAX_COLUMN_COUNT];
        }
        if( currentPanel == NULL || otherPanel == NULL )
        {
            return;
        }
        //panels[assignedColumn + DOCK_MAX_COLUMN_COUNT];

        //If we are adding this panel to an empty column
        if( panelColumnWidth[assignedColumn] < DOCK_COLUMN_MIN_WIDTH )
        {
            panelColumnWidth[assignedColumn] = DOCK_COLUMN_MIN_WIDTH;
        }

        currentPanel->set_width( panelColumnWidth[assignedColumn] );
        otherPanel->set_width( panelColumnWidth[assignedColumn] );


        if( !currentPanel->has_content() && !otherPanel->has_content() )
        {
            if( panelId < DOCK_MAX_COLUMN_COUNT )
            {
                panelHeightPercentages[panelId ] = 1.0;
                panelHeightPercentages[panelId + DOCK_MAX_COLUMN_COUNT ] = 0.0d;
            }
            else
            {
                panelHeightPercentages[panelId ] = 1.0;
                panelHeightPercentages[panelId - DOCK_MAX_COLUMN_COUNT ] = 0.0d;
            }
        }
        else if( panelHeightPercentages[panelId ] <= 0.2 )
        {
            //If our panel is on the top
            if( panelId < DOCK_MAX_COLUMN_COUNT )
            {
                panelHeightPercentages[panelId ] = 0.2;
                panelHeightPercentages[panelId + DOCK_MAX_COLUMN_COUNT ] = 0.8;
            }
            else
            {
                //If our panel is on the bottom,
                panelHeightPercentages[panelId ] = 0.2;
                panelHeightPercentages[panelId - DOCK_MAX_COLUMN_COUNT ] = 0.8;
            }
        }
        //Makes the column visible if closed;
        currentPanel->add_container( name, open );

    }
}

void gpeEditorDock::clear_defaults()
{
    GPE_KeyPair * tempPair = NULL;
    for( int i = (int)defaultPanels.size()-1; i >=0; i--)
    {
        tempPair = defaultPanels[i];
        if( tempPair !=NULL)
        {
            delete tempPair;
            tempPair = NULL;
        }
    }
    defaultPanels.clear();
}

bool gpeEditorDock::container_in_view( std::string name )
{
    gpeEditorDockPanel *  tPanel = NULL;
    for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
    {
        tPanel = panels[iPanel];
        if( tPanel!=NULL && tPanel->container_in_view(name) )
        {
            return true;
        }
    }
    return false;
}

int gpeEditorDock::get_column(int panelId)
{
    if( panelId < DOCK_MAX_COLUMN_COUNT )
    {
        return panelId;
    }
    else if( panelId < DOCK_MAX_PANEL_COUNT )
    {
        return panelId - DOCK_MAX_COLUMN_COUNT;
    }
    return -1;
}

int gpeEditorDock::get_dock_left_width()
{
    if( panelColumnWidth[0] + panelColumnWidth[1] >= DOCK_COLUMN_MIN_WIDTH )
    {
        return (panelColumnWidth[0] + panelColumnWidth[1] + GENERAL_GPE_PADDING);
    }
    else
    {
        return 0;
    }
    /*
    if( panels[DOCK_TOP_LEFT]!=NULL && panels[DOCK_TOP_LEFT_CENTER]!=NULL )
    {
        return panels[DOCK_TOP_LEFT]->get_width()+panels[DOCK_TOP_LEFT_CENTER]->get_width();
    }
    else if( panels[DOCK_BOTTOM_LEFT]!=NULL && panels[DOCK_BOTTOM_LEFT_CENTER]!=NULL )
    {
        return panels[DOCK_BOTTOM_LEFT]->get_width()+panels[DOCK_BOTTOM_LEFT_CENTER]->get_width();
    }
    return 0;
    */
}

int gpeEditorDock::get_dock_right_width()
{
    if( panelColumnWidth[3] + panelColumnWidth[4] >= DOCK_COLUMN_MIN_WIDTH )
    {
        return (panelColumnWidth[3] + panelColumnWidth[4] + GENERAL_GPE_PADDING);
    }
    else
    {
        return 0;
    }
    /*
    if( panels[DOCK_TOP_RIGHT]!=NULL && panels[DOCK_TOP_RIGHT_CENTER]!=NULL )
    {
        return panels[DOCK_TOP_RIGHT]->get_width()+panels[DOCK_TOP_RIGHT_CENTER]->get_width();
    }
    else if( panels[DOCK_BOTTOM_RIGHT]!=NULL && panels[DOCK_BOTTOM_RIGHT_CENTER]!=NULL )
    {
        return panels[DOCK_BOTTOM_RIGHT]->get_width()+panels[DOCK_BOTTOM_RIGHT_CENTER]->get_width();
    }
    return 0;
    */
}

int gpeEditorDock::handle_resizing()
{
    //Starts off checking if the user wishes to end resize
    if( beingResized )
    {
        if( input->check_keyboard_released( kb_esc )  || input->windowEventHappendInFrame || input->check_mouse_released( mb_middle)  || input->check_mouse_released( mb_right) )
        {
            justResized = beingResized = horizontalResize = verticalResize = false;
            horiResizeStartX = 0;
            columnBeingResizedId = -1;
            return -1;
        }
        else if( point_between_rect( input->mouse_x, input->mouse_y, &elementBox ) )
        {
            //The above if makes sure we are actually in the dock, avoiding negative results
            //Checks if we
            bool resizeExited = false;
            if( horizontalResize )
            {
                GPE_change_cursor( SDL_SYSTEM_CURSOR_SIZEWE );

                //The code, below may appear unoptimized, but it rarely is executed since it only happens on ending of resize of dock...
                if( input->check_mouse_released( mb_left) )
                {
                    //Last check to make sure we are not out of bounds, we only resize from column 1 and to the right tho...
                    if( columnBeingResizedId > 0 && columnBeingResizedId < DOCK_MAX_COLUMN_COUNT )
                    {
                        gpeEditorDockPanel * foundTPanel = NULL;
                        gpeEditorDockPanel * foundBPanel = NULL;
                        int panelNewSize = 0;
                        int currentPanelX = 0;
                        int lastPanelWidth = 0;
                        int lastLeftPanel = -1;
                        int panelsUsedOnLeft = 0;
                        int iCol = 0;
                        //Let's add up the past columsn to see where our current colum should be
                        for( iCol = 0; iCol < columnBeingResizedId; iCol++)
                        {
                            if( panelColumnWidth[iCol] > 0 )
                            {
                                foundTPanel = panels[ iCol ];
                                foundBPanel = panels[ iCol + DOCK_MAX_COLUMN_COUNT ];

                                if( foundTPanel->has_content() || foundBPanel->has_content() )
                                {
                                    lastLeftPanel = iCol;
                                    lastPanelWidth = panelColumnWidth[iCol];
                                    currentPanelX+= lastPanelWidth + GENERAL_GPE_PADDING;
                                    panelsUsedOnLeft++;
                                }
                            }
                        }

                        //Resize to the left...
                        if( input->mouse_x < currentPanelX  &&  panelsUsedOnLeft > 0 && lastLeftPanel >=0  )
                        {
                            //We will move to the left
                            //If the mouse is to the right of the last panel to the left
                            if( input->mouse_x > currentPanelX - lastPanelWidth - GENERAL_GPE_PADDING  )
                            {
                                //Only going to interact with the panel on the immediate left
                                panelNewSize = panelColumnWidth[lastLeftPanel] - (currentPanelX - input->mouse_x );
                                if( panelNewSize >= DOCK_COLUMN_MIN_WIDTH )
                                {
                                    panelColumnWidth[ columnBeingResizedId ] += currentPanelX - input->mouse_x ;
                                    panelColumnWidth[ lastLeftPanel ] = panelNewSize;
                                }
                                else
                                {
                                    //We do not have enough space, so let's work with the minimums here.
                                    panelColumnWidth[columnBeingResizedId] += panelColumnWidth[lastLeftPanel] - DOCK_COLUMN_MIN_WIDTH;
                                    panelColumnWidth[ lastLeftPanel] = DOCK_COLUMN_MIN_WIDTH;
                                }
                            }
                            else
                            {
                                //A more complex resize operation not needed for 1.3.0 RC
                                /*
                                if( input->mouse_x < panelsUsedOnLeft * ( DOCK_COLUMN_MIN_WIDTH+GENERAL_GPE_PADDING)
                                {
                                    for( iCol = 0; iCol < columnBeingResizedId; iCol++)
                                    {
                                        if( panelColumnWidth[iCol] > 0 )
                                        {
                                            panelColumnWidth[iCol] = DOCK_COLUMN_MIN_WIDTH;
                                        }
                                    }
                                    panelColumnWidth[iCol] += currentPanelX - input->mouse_x;
                                }
                                */
                            }
                        }
                        //Move the panel to the right, where possible.
                        else if( panelsUsedOnLeft > 0 && lastLeftPanel >=0  )
                        {
                            //We will add to the panel
                            if( input->mouse_x <= currentPanelX + panelColumnWidth[columnBeingResizedId] )
                            {
                                //Panel resizing only is between two panels, so let's calculate the differences;
                                panelNewSize = panelColumnWidth[columnBeingResizedId] - (input->mouse_x - currentPanelX);
                                if( panelNewSize >= DOCK_COLUMN_MIN_WIDTH )
                                {
                                    panelColumnWidth[lastLeftPanel] += input->mouse_x - currentPanelX;
                                    panelColumnWidth[columnBeingResizedId] = panelNewSize;
                                }
                                else
                                {
                                    //We do not have enough space, so let's work with the minimums here.
                                    panelColumnWidth[lastLeftPanel] += panelColumnWidth[columnBeingResizedId] - DOCK_COLUMN_MIN_WIDTH;
                                    panelColumnWidth[columnBeingResizedId] = DOCK_COLUMN_MIN_WIDTH;
                                }
                            }
                            else
                            {
                                //A more complex resize operation not needed for 1.3.0 RC
                            }
                        }
                    }
                    resizeExited = true;
                    horiResizeStartX = 0;
                    columnBeingResizedId = -1;
                    beingResized = horizontalResize = verticalResize = false;
                }
            }
            else if( verticalResize )
            {
                GPE_change_cursor( SDL_SYSTEM_CURSOR_SIZENS );
                if( input->check_mouse_released( mb_left) )
                {
                    //Last check to make sure we are not out of bounds...
                    if( columnBeingResizedId >= 0 && columnBeingResizedId < DOCK_MAX_COLUMN_COUNT )
                    {
                        panelHeightPercentages[columnBeingResizedId] = ( (double) input->mouse_y - elementBox.y ) / ( elementBox.h - (double)GENERAL_GPE_PADDING );
                        panelHeightPercentages[columnBeingResizedId + DOCK_MAX_COLUMN_COUNT] = 1.0 - panelHeightPercentages[columnBeingResizedId];
                    }
                    resizeExited = true;
                    horiResizeStartX = 0;
                    columnBeingResizedId = -1;
                    beingResized = horizontalResize = verticalResize = false;
                }
            }
            else
            {
                resizeExited = true;
                horiResizeStartX = 0;
                columnBeingResizedId = -1;
                beingResized = horizontalResize = verticalResize = false;
            }
            if( resizeExited)
            {
                setup_dock();
                return 1;
            }
        }
        else
        {
            //Exits the resize if we get out of range
            justResized = beingResized = horizontalResize = verticalResize = false;
            horiResizeStartX = 0;
            columnBeingResizedId = -1;
            return -2;
        }
    }
    else
    {
        //If we started off not being resized we will check if user wants to resize
        gpeEditorDockPanel * tPanel = NULL;
        gpeEditorDockPanel * bPanel = NULL;

        int panelX = 0;
        for( int iColumn = 0; iColumn < DOCK_MAX_COLUMN_COUNT; iColumn++)
        {
            if( panelColumnWidth[iColumn] > 0 )
            {
                tPanel = panels[iColumn];
                bPanel = panels[iColumn + DOCK_MAX_COLUMN_COUNT];
                if( tPanel !=NULL && bPanel!=NULL )
                {
                    //Checks to see if user is attempting to resize panels vertically
                    if( tPanel->has_content() && bPanel->has_content() )
                    {
                        if( point_between(input->mouse_x, input->mouse_y,tPanel->get_xpos(),tPanel->get_y2pos(), tPanel->get_x2pos(),tPanel->get_y2pos()+GENERAL_GPE_PADDING ) )
                        {
                            GPE_change_cursor( SDL_SYSTEM_CURSOR_SIZENS );
                            if(input->check_mouse_pressed(0) )
                            {
                                columnBeingResizedId = iColumn;
                                verticalResize = beingResized = true;
                                horizontalResize = false;
                                horiResizeStartX = 0;
                                if( MAIN_OVERLAY!=NULL )
                                {
                                    MAIN_OVERLAY->take_frozen_screenshot();
                                }
                                return 0;
                            }
                        }
                    }

                    //Checks to see if user is attempting to resize panels horizontally
                    if( !horizontalResize && ( tPanel->has_content() ||  bPanel->has_content() ) )
                    {
                        //Unlike vertical check, we only require one panel to be visible(top or bottom)
                        if( point_between( input->mouse_x, input->mouse_y, panelX - GENERAL_GPE_PADDING,elementBox.y, panelX, elementBox.y2Pos ) )
                        {
                            GPE_change_cursor( SDL_SYSTEM_CURSOR_SIZEWE );
                            if(input->check_mouse_pressed(0) )
                            {
                                columnBeingResizedId = iColumn;
                                horizontalResize = beingResized = true;
                                verticalResize = false;
                                horiResizeStartX = input->mouse_x;
                                if( MAIN_OVERLAY!=NULL )
                                {
                                    MAIN_OVERLAY->take_frozen_screenshot();
                                }
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        //Both panels must be empty, so let's set the values to 0;
                        tPanel->set_width( 0 );
                        bPanel->set_width( 0 );
                        panelColumnWidth[iColumn] = 0;
                    }
                    //IF the above else has not kicked in, we add to panelX for next column
                    if( panelColumnWidth[iColumn] > 0 )
                    {
                        panelX += panelColumnWidth[iColumn] + GENERAL_GPE_PADDING;
                    }
                }
            }
        }
    }

	//
    if( beingResized)
    {
        return 0;
    }
    return -2;
}

void gpeEditorDock::hide_dock()
{
    PANEL_RESOURCE_TREE = NULL;
    PANEL_GENERAL_EDITOR = NULL;
    PANEL_INSPECTOR = NULL;
    for( int i = 0; i < DOCK_MAX_PANEL_COUNT; i++ )
    {
        if( panels[i] !=NULL )
        {
            panels[i]->clear_panel();
        }
    }
    isHidden = true;
}

bool gpeEditorDock::is_hidden()
{
    return isHidden;
}

void gpeEditorDock::process_dock( GPE_Rect * viewedSpace, GPE_Rect * cam  )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( panels[DOCK_TOP_MIDDLE]!=NULL)
    {
        panels[DOCK_TOP_MIDDLE]->process_self( viewedSpace, cam );
    }
    if( panels[DOCK_BOTTOM_MIDDLE]!=NULL)
    {
        panels[DOCK_BOTTOM_MIDDLE]->process_self( viewedSpace, cam );
    }
    //Process all of the remaining panels
    gpeEditorDockPanel *  tPanel = NULL;
    int tPanelButtonHitID = -1;
    int iPanel = 0;
    for( iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
    {
        tPanel = panels[iPanel];
        if( tPanel!=NULL )
        {
            tPanel->process_self(  viewedSpace, cam );
            if( tPanel->dockSettingsButton!=NULL && tPanel->dockSettingsButton->is_clicked() )
            {
                tPanelButtonHitID = iPanel;
                tPanel->dockSettingsButton->set_clicked( false );
                input->reset_all_input();
            }
        }
    }

    if( tPanelButtonHitID >=0 && tPanelButtonHitID < DOCK_MAX_PANEL_COUNT )
    {
        //Creates a context menu to allow user to move panel content over
        GPE_open_context_menu(input->mouse_x-32,input->mouse_y+32,128);
        for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++)
        {
            //Prevents us from adding to main editor/log or the same panel
            if( iPanel !=tPanelButtonHitID && iPanel != DOCK_TOP_MIDDLE && iPanel != DOCK_BOTTOM_MIDDLE )
            {
                MAIN_CONTEXT_MENU->add_menu_option("Move to "+defaultPanelNames[iPanel], iPanel,NULL,-1,NULL,true,true);
            }
        }
        int closePanelId = 128;
        MAIN_CONTEXT_MENU->add_menu_option("Close Content", closePanelId,NULL,-1,NULL,true,true);
        //tPanel->dockSettingsButton->set_clicked( false );
        int menuSelection = GPE_Get_Context_Result();
        if( menuSelection >= 0 && menuSelection < DOCK_MAX_PANEL_COUNT)
        {
            //Add content to new panel
            std::string selectedPanelName = panels[tPanelButtonHitID]->get_selected_container();
            add_to_panel(  selectedPanelName, menuSelection, true );
        }
        else if( menuSelection == closePanelId)
        {
            //Simply removes the panel otherwise
            tPanel->remove_container( panels[tPanelButtonHitID]->get_selected_container() );
            //Temporary note to see if this is being called
            GPE_Main_Logs->log_general_line("Removing panel ["+int_to_string(tPanelButtonHitID)+"]");
        }
        input->reset_all_input();
        setup_dock();
    }
}

void gpeEditorDock::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam  )
{
    //setup_dock();
    //If no sort of resizing occurred
    if( handle_resizing() < 0 )
    {
        process_dock();
    }
}

void gpeEditorDock::remove_default_panel( std::string name )
{
    GPE_KeyPair * tempPair = NULL;
    for( int i = (int)defaultPanels.size()-1; i >=0; i--)
    {
        tempPair = defaultPanels[i];
        if( tempPair !=NULL)
        {
            if( tempPair->keyString == name || tempPair->keySubString == name )
            {
                delete tempPair;
                tempPair = NULL;
                defaultPanels.erase( defaultPanels.begin()+i );
            }
        }
    }
}

void gpeEditorDock::remove_panel( std::string name )
{
    gpeEditorDockPanel *  tPanel = NULL;
    for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
    {
        tPanel = panels[iPanel];
        if( tPanel!=NULL )
        {
            tPanel->remove_container( name );
        }
    }
}

void gpeEditorDock::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw   )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);

    if( isHidden )
    {
        //return;
    }

    if( beingResized && columnBeingResizedId >=0 )
    {
        MAIN_RENDERER->reset_viewpoint();
        if( forceRedraw )
        {
            MAIN_OVERLAY->render_frozen_screenshot( );
        }
        if( horizontalResize )
        {
            gcanvas->render_rectangle( input->mouse_x-4, get_ypos(),input->mouse_x+4, get_y2pos(), GPE_MAIN_THEME->PopUp_Box_Border_Color, false );
        }
        else if( verticalResize)
        {
            gcanvas->render_rectangle( get_xpos(),input->mouse_y+4, get_x2pos(), input->mouse_y-4, GPE_MAIN_THEME->PopUp_Box_Border_Color, false );
        }
    }
    else
    {
        MAIN_RENDERER->reset_viewpoint();
        gpeEditorDockPanel *  tPanel = NULL;
        for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
        {
            //if( iPanel != DOCK_TOP_MIDDLE && iPanel!= DOCK_BOTTOM_MIDDLE)
            {
                tPanel = panels[iPanel];
                if( tPanel!=NULL )
                {
                    tPanel->render_self( viewedSpace, cam, forceRedraw);
                }
            }
        }
    }
}

void gpeEditorDock::reset_dock()
{
    for( int iColumn = 0; iColumn < DOCK_MAX_COLUMN_COUNT; iColumn++ )
    {
        panelColumnWidth[iColumn] = DOCK_COLUMN_DEFAULT_WIDTH;
        panelWidthPercentages[iColumn] = 1.d/ (double)DOCK_MAX_COLUMN_COUNT;
    }
    for( int iPanels = 0; iPanels < DOCK_MAX_PANEL_COUNT; iPanels++ )
    {
        panelHeightPercentages[iPanels] = 1.d/ (double)DOCK_MAX_ROW_COUNT;
    }
    //Calculates the middle width
    if( SCREEN_WIDTH > DOCK_COLUMN_DEFAULT_WIDTH * DOCK_MAX_COLUMN_COUNT)
    {
        panelColumnWidth[DOCK_COLUMN_DEFAULT_WIDTH * DOCK_MAX_COLUMN_COUNT] = SCREEN_WIDTH - ( DOCK_COLUMN_DEFAULT_WIDTH * (DOCK_MAX_COLUMN_COUNT-1) );
    }
    gpeEditorDockPanel *  tPanel = NULL;
    for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
    {
        tPanel = panels[iPanel];
        if( tPanel!=NULL )
        {
            tPanel->reset_panel();
        }
    }
    add_to_panel( "Resources", DOCK_TOP_LEFT,  true );
    add_to_panel( "Editor", DOCK_TOP_LEFT_CENTER, true );
    add_to_panel( "Inspector", DOCK_BOTTOM_RIGHT, true );
    //add_to_panel( "Meta", DOCK_BOTTOM_RIGHT, true );
    add_to_panel("Main", DOCK_TOP_MIDDLE, true );
    add_to_panel("Logs", DOCK_BOTTOM_MIDDLE, true );

    GPE_KeyPair * tempPair;
    for( int iDefaultPane = 0; iDefaultPane < (int)defaultPanels.size(); iDefaultPane++)
    {
        tempPair = defaultPanels.at(iDefaultPane);
        if( tempPair !=NULL)
        {
            add_to_panel( tempPair->keyString, tempPair->keyValue, true );
        }
        /*delete tempPair;
        tempPair = NULL; */
    }
    panels[ DOCK_TOP_MIDDLE ]->specialPanelElement = GPE_Main_TabManager;
    panels[ DOCK_BOTTOM_MIDDLE ]->specialPanelElement = GPE_Main_Logs;
    //defaultPanels.clear();
}

void gpeEditorDock::setup_dock()
{
    panels[ DOCK_TOP_MIDDLE ]->specialPanelElement = GPE_Main_TabManager;
    panels[ DOCK_BOTTOM_MIDDLE ]->specialPanelElement = GPE_Main_Logs;
    gpeEditorDockPanel *  tPanel = NULL;
    gpeEditorDockPanel *  bPanel = NULL;

    //Manages the heights of the panels
    int iPanelCol = 0;
    int panelColumnCount = 1;
    for( iPanelCol = 0; iPanelCol < DOCK_MAX_COLUMN_COUNT; iPanelCol++ )
    {
        tPanel = panels[iPanelCol];
        bPanel = panels[iPanelCol + DOCK_MAX_COLUMN_COUNT ];
        if( tPanel!=NULL ) //&& iPanelCol != DOCK_TOP_MIDDLE )
        {
            //Resets the panel's settings button
            if( tPanel->dockSettingsButton==NULL )
            {
                tPanel->dockSettingsButton->set_clicked( false );
            }
            //Checks to make sure heights are not out of the ordinary
            if( panelHeightPercentages[iPanelCol] < 0.d )
            {
                panelHeightPercentages[iPanelCol] = 0.d;
                panelHeightPercentages[iPanelCol + DOCK_MAX_COLUMN_COUNT  ] = 1.0;
            }
            else if(  panelHeightPercentages[iPanelCol]  > 1.d)
            {
                panelHeightPercentages[iPanelCol] = 1.0;
                panelHeightPercentages[iPanelCol + DOCK_MAX_COLUMN_COUNT  ] = 0.d;
            }

            if( panelHeightPercentages[iPanelCol  + DOCK_MAX_COLUMN_COUNT  ] < 0.0 )
            {
                panelHeightPercentages[iPanelCol] = 1.0;
                panelHeightPercentages[iPanelCol + DOCK_MAX_COLUMN_COUNT  ] = 0.0;
            }
            else if(  panelHeightPercentages[iPanelCol  + DOCK_MAX_COLUMN_COUNT ]  > 1.0)
            {
                panelHeightPercentages[iPanelCol] = 0.0;
                panelHeightPercentages[iPanelCol  + DOCK_MAX_COLUMN_COUNT ] = 1.0;
            }

            //Resets the panel's settings button
            if( bPanel!=NULL )
            {
                if( bPanel->dockSettingsButton==NULL )
                {
                    bPanel->dockSettingsButton->set_clicked( false );
                }
                //Cuts Column from view if both panels has no content
                if( !tPanel->has_content() && !bPanel->has_content() )
                {
                    panelColumnWidth[iPanelCol] = 0;
                    panelHeightPercentages[iPanelCol + DOCK_MAX_COLUMN_COUNT ] = 0.d;
                    panelHeightPercentages[ iPanelCol ] = 0.d;
                    //panelColumnWidth[iPanelCol + DOCK_MAX_COLUMN_COUNT] = 0;
                }
                else
                {
                    if( panelColumnWidth[iPanelCol] < DOCK_COLUMN_MIN_WIDTH )
                    {
                         panelColumnWidth[iPanelCol] = DOCK_COLUMN_MIN_WIDTH;
                    }
                    if( tPanel->has_content() && !bPanel->has_content())
                    {
                        //cuts bottom panel from view
                        panelHeightPercentages[iPanelCol] = 1;
                        panelHeightPercentages[iPanelCol + DOCK_MAX_COLUMN_COUNT ] = 0;
                        tPanel->set_height( elementBox.h );
                    }
                    else if( bPanel->has_content() && !tPanel->has_content())
                    {
                        //cuts top panel from view
                        panelHeightPercentages[iPanelCol] = 0;
                        panelHeightPercentages[iPanelCol + DOCK_MAX_COLUMN_COUNT ] = 1;
                        bPanel->set_height( elementBox.h );
                    }
                    tPanel->set_height( (elementBox.h-GENERAL_GPE_PADDING) * panelHeightPercentages[iPanelCol] );
                    bPanel->set_height( (elementBox.h-GENERAL_GPE_PADDING) * ( panelHeightPercentages[iPanelCol + DOCK_MAX_COLUMN_COUNT] ) );
                    panelColumnCount++;
                }
            }
            else
            {
                //This should not occur, since we should always have col*row panels
                //But here is the fuzzy code in case this rare case happens(Maybe low memory or something..
                //Defaults to top panel being 100% in height
                panelHeightPercentages[iPanelCol] = 1.0;
                panelHeightPercentages[iPanelCol + DOCK_MAX_COLUMN_COUNT ] = 0;
                if( tPanel->has_content() )
                {
                    if( panelColumnWidth[iPanelCol] < DOCK_COLUMN_MIN_WIDTH )
                    {
                         panelColumnWidth[iPanelCol] = DOCK_COLUMN_MIN_WIDTH;
                    }
                    panelColumnCount++;
                }
                tPanel->set_height( elementBox.h );
            }
        }
        else
        {
            //Would be weird if this happens, since we should have all panels as non-nulls
            //panelColumnWidth[iPanelCol] = 0;
        }
    }

    //Horizontally Resizing panels;
    gpeEditorDockPanel * currPanel;
    bool panelBeingHovered = false;

    //Let's set the position of the  panels
    int panelX = 0;
    int panelY = 0;
    int cPanel = 0;


    for( iPanelCol = 0; iPanelCol < DOCK_MAX_COLUMN_COUNT ; iPanelCol++)
    {
        for( int jPanelRow = 0; jPanelRow < DOCK_MAX_ROW_COUNT ; jPanelRow++)
        {
            cPanel = iPanelCol + jPanelRow*DOCK_MAX_COLUMN_COUNT;
            tPanel =panels[cPanel];
            if( tPanel!=NULL && tPanel->has_content() )
            {
                tPanel->set_coords(panelX, elementBox.y + panelY );
                tPanel->set_width( panelColumnWidth[ iPanelCol ] );

                //Basically only add to PanelY if its not the bottom one
                panelY+= panels[ cPanel]->get_height() + GENERAL_GPE_PADDING;
                tPanel->setup_panel( !beingResized, true );
            }
        }
        if( panelColumnWidth[iPanelCol] > 0 )
        {
            if( panelColumnCount > 1 )
            {
                panelX+= panelColumnWidth[ iPanelCol ] + GENERAL_GPE_PADDING;
            }
            else
            {
                panelX = SCREEN_WIDTH;
            }
        }
        panelY = 0;
    }

    //Readjust middle column if more space is available
    if( panelX < SCREEN_WIDTH)
    {
        panelColumnWidth[ DOCK_TOP_MIDDLE] += SCREEN_WIDTH - panelX;
        /*setup_dock(); //Performs recursion to set the new true coords
        return; */
    }
    else if( panelX > SCREEN_WIDTH && panelColumnCount > 0 )
    {
        if( panelColumnCount == 1 )
        {
            for( iPanelCol = 0; iPanelCol < DOCK_MAX_COLUMN_COUNT ; iPanelCol++)
            {
                panelColumnWidth[iPanelCol] = 0;
            }
            panelColumnWidth[DOCK_TOP_MIDDLE] = SCREEN_WIDTH;
        }
        else
        {
            int adjustedColumnWidth = SCREEN_WIDTH / panelColumnCount - (GENERAL_GPE_PADDING*panelColumnCount) - GENERAL_GPE_PADDING;
            for( iPanelCol = 0; iPanelCol < DOCK_MAX_COLUMN_COUNT ; iPanelCol++)
            {
                panelColumnWidth[iPanelCol] = adjustedColumnWidth;
            }
        }
        //setup_dock();
        //return;
    }
    PANEL_RESOURCE_TREE = find_panel("Resources");
    PANEL_GENERAL_EDITOR = find_panel("Editor");
    PANEL_INSPECTOR = find_panel("Inspector");
    PANEL_META = find_panel("Meta");
}

void gpeEditorDock::toggle_default_pane( std::string name )
{
    GPE_Report("Trying to toggle ["+name+"] pane...");
    GPE_KeyPair * tempPair = NULL;
    for( int i = 0; i < (int)defaultPanels.size(); i++)
    {
        tempPair = defaultPanels[i];
        if( tempPair !=NULL)
        {
            if( tempPair->keyString == name || tempPair->keySubString == name )
            {
                toggle_panel( tempPair->keyString, tempPair->keyValue, true );
            }
        }
    }
}

void gpeEditorDock::toggle_panel(  std::string name, int panelId,  bool open  )
{
    if( (int)name.size() == 0)
    {
        return;
    }
    if( find_panel( name) != NULL )
    {
        remove_panel( name );
    }
    else
    {
        add_to_panel( name, panelId, open );
    }
}

void gpeEditorDock::unhide_dock()
{
    isHidden = false;
    PANEL_RESOURCE_TREE = find_panel("Resources");
    PANEL_GENERAL_EDITOR = find_panel("Editor");
    PANEL_INSPECTOR = find_panel("Inspector");
}

void gpeEditorDock::update_toolbar()
{
    if( toolbarOptonsHolder==NULL )
    {
        return;
    }
    toolbarOptonsHolder->clear_menu();
    if( GPE_Main_Logs!=NULL && GPE_Main_Logs->is_enabled() )
    {
        toolbarOptonsHolder->add_menu_option("Toggle Logs",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/check-square.png"),-1,NULL,false);
    }
    else
    {
        toolbarOptonsHolder->add_menu_option("Toggle Logs",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    }
    GPE_KeyPair * tempPair = NULL;
    for( int i = 0; i < (int)defaultPanels.size(); i++)
    {
        tempPair = defaultPanels[i];
        if( tempPair !=NULL)
        {
            if( find_panel(tempPair->keyString)!=NULL )
            {
                toolbarOptonsHolder->add_menu_option("Toggle "+tempPair->keyString+" Pane",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/check-square.png"),-1,NULL,false);
            }
            else
            {
                toolbarOptonsHolder->add_menu_option("Toggle "+tempPair->keyString+" Pane",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
            }
        }
    }
    toolbarOptonsHolder->add_menu_option("Reset Dock",-1,NULL,-1,NULL,false);
}

gpeEditorDockPanel * gpeEditorDock::find_panel( std::string name )
{
    gpeEditorDockPanel *  tPanel = NULL;
    for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
    {
        tPanel = panels[iPanel];
        if( tPanel!=NULL && tPanel->container_in_view(name) )
        {
            return tPanel;
        }
    }
    return NULL;
}

