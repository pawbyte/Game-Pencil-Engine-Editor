/*
gpe_dock_system.cpp
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
    panelGuiList->panelAlignType = GPE_PANEL_ALIGN_FULL_LEFT;
    panelGuiList->barXPadding = GENERAL_GPE_GUI_PADDING;
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
    if( (int)name.size() > 0  )
    {
        panelBar->add_new_tab( name, openNew );
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
    wasProcessed = false;
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
    setup_panel( false, false );
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
                specialPanelElement->set_coords( elementBox.x, elementBox.y);
                specialPanelElement->set_width( elementBox.w );
                specialPanelElement->set_height( elementBox.h );
                specialPanelElement->process_self( viewedSpace, cam );
            }
            else
            {
                //For normal tabs
                //Processes the gui element list
                if( panelGuiList!=NULL )
                {
                    panelGuiList->set_coords( 0,0 );
                    panelGuiList->set_width( panelRect->w );
                    panelGuiList->set_height( panelRect->h );
                    panelGuiList->barXMargin = GENERAL_GPE_GUI_PADDING;
                    panelGuiList->barYMargin = GENERAL_GPE_GUI_PADDING;
                    panelGuiList->barXPadding = GENERAL_GPE_GUI_PADDING;
                    panelGuiList->barYPadding = GENERAL_GPE_GUI_PADDING;
                    panelGuiList->process_self( panelRect, cam  );
                }

                if( panelBar!=NULL )
                {
                    panelBar->process_self( viewedSpace, cam);
                }
                if( dockSettingsButton!=NULL )
                {
                    dockSettingsButton->process_self( viewedSpace, cam);
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

void gpeEditorDockPanel::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam   )
{
    if( has_content() == false)
    {
        return;
    }

    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );
    GPE_MAIN_RENDERER->reset_viewpoint();
    GPE_MAIN_RENDERER->set_viewpoint( viewedSpace);

    if( specialPanelElement!=NULL )
    {
        specialPanelElement->render_self( NULL,NULL );
    }
    else
    {
        if( panelBar!=NULL  )
        {
            panelBar->render_self( NULL, NULL );
        }
        if( dockSettingsButton!=NULL )
        {
            dockSettingsButton->render_self( NULL, NULL );
        }

        GPE_MAIN_RENDERER->reset_viewpoint();
        GPE_MAIN_RENDERER->set_viewpoint( panelRect );
        if( panelGuiList!=NULL )
        {
            panelGuiList->render_self( panelRect, cam );
        }
        GPE_MAIN_RENDERER->reset_viewpoint();

    }
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

bool gpeEditorDockPanel::save_panel_data_to_file(std::ofstream * fileTarget)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        *fileTarget << "#    Created automatically via the Game Pencil Engine Editor \n";
    }
}

void gpeEditorDockPanel::setup_panel(  bool addSpacing, bool clearList  )
{
    panelRect->x = elementBox.x;
    panelRect->y = elementBox.y+panelBar->get_height();
    panelRect->w = elementBox.w;
    panelRect->h = elementBox.h - panelBar->get_height();

    if( specialPanelElement!=NULL )
    {
        specialPanelElement->set_coords(elementBox.x, elementBox.y );
        specialPanelElement->set_width( elementBox.w );
        specialPanelElement->set_height( elementBox.h );
    }
    else if( panelGuiList!=NULL )
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
            panelGuiList->barXPadding = GENERAL_GPE_GUI_PADDING;
            panelGuiList->barYPadding = GENERAL_GPE_GUI_PADDING;
            panelGuiList->barXMargin = GENERAL_GPE_GUI_PADDING;
            panelGuiList->barYMargin = GENERAL_GPE_GUI_PADDING;
        }
    }

    if( panelBar!=NULL)
    {
        panelBar->set_width( elementBox.w - panelBar->get_height() - GENERAL_GPE_GUI_PADDING );
        panelBar->set_coords( elementBox.x, elementBox.y );

        if( dockSettingsButton!=NULL )
        {
            if( wasProcessed==false )
            {
                dockSettingsButton->set_clicked( false );
            }
            dockSettingsButton->set_coords( panelBar->get_x2pos() + GENERAL_GPE_GUI_PADDING, panelBar->get_ypos() );
            dockSettingsButton->set_width(  panelBar->get_height() );
            dockSettingsButton->set_height(  panelBar->get_height() );
        }


    }
    else if( dockSettingsButton!=NULL )
    {
        if( wasProcessed==false )
        {
            dockSettingsButton->set_clicked( false );
        }
        dockSettingsButton->set_coords(elementBox.w - 32 - GENERAL_GPE_GUI_PADDING, 0 );
        dockSettingsButton->set_width(  32 );
        dockSettingsButton->set_height(  32 );
    }
}

gpeEditorDock::gpeEditorDock()
{
    dockHorizontalPadding = 16;
    dockVerticalPadding = 16;
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
    resizePanelX1 = 0; resizePanelX2 = 0;
    justResized = beingResized = horizontalResize = verticalResize = false;


    GPE_Report( "Attempt to create ["+int_to_string(DOCK_MAX_PANEL_COUNT)+"] panels...");
    for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
    {
        GPE_Report("Adding Pane["+ int_to_string(iPanel) +"]..");
        panelHeightPercentages[iPanel] = 0.f;
        defaultPanelNames[ iPanel ] = "";
        panels[ iPanel ] = new gpeEditorDockPanel();
    }
    reset_dock();


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
}

gpeEditorDock::~gpeEditorDock()
{
    PANEL_RESOURCE_TREE = NULL;
    PANEL_GENERAL_EDITOR = NULL;
    PANEL_INSPECTOR = NULL;
    PANEL_META = NULL;

    clear_defaults();

    gpeEditorDockPanel * tempPanel = NULL;

    for( int i =DOCK_MAX_PANEL_COUNT -1;  i >=0; i--)
    {
        tempPanel = panels[i];
        if(tempPanel != NULL )
        {
            delete tempPanel;
            tempPanel = NULL;
        }
        panels[i] = NULL;
    }
}


void gpeEditorDock::add_default_panel( std::string name, int panelId, bool open, bool saveToSettings )
{
    GPE_KeyPair * newPair = new GPE_KeyPair( panelId, name, name, panelId );
    defaultPanels.push_back( newPair);
    if( open)
    {
        add_to_panel( name, panelId, open, saveToSettings );
    }
    update_toolbar();
}

void gpeEditorDock::add_to_panel( std::string name, int panelId, bool open, bool saveToSettings )
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
        //Resizes the width to dockMinimumColumnPercentage * dockWidthMinusColumnPadding if its "unopen"
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
        if( panelWidthPercentages[assignedColumn] < dockMinimumColumnPercentage )
        {
            panelWidthPercentages[assignedColumn] = dockMinimumColumnPercentage;
        }

        currentPanel->add_container( name, open );

        if( !currentPanel->has_content() && !otherPanel->has_content() )
        {
            if( panelId < DOCK_MAX_COLUMN_COUNT )
            {
                panelHeightPercentages[panelId ] = 100.f;
                panelHeightPercentages[panelId + DOCK_MAX_COLUMN_COUNT ] = 0.0d;
            }
            else
            {
                panelHeightPercentages[panelId ] = 100.f;
                panelHeightPercentages[panelId - DOCK_MAX_COLUMN_COUNT ] = 0.0d;
            }
        }
        else if( panelHeightPercentages[panelId ] <= 20.f )
        {
            //If our panel is on the top
            if( panelId < DOCK_MAX_COLUMN_COUNT )
            {
                panelHeightPercentages[panelId ] = 20.f;
                panelHeightPercentages[panelId + DOCK_MAX_COLUMN_COUNT ] = 80.f;
            }
            else
            {
                //If our panel is on the bottom,
                panelHeightPercentages[panelId ] = 20.f;
                panelHeightPercentages[panelId - DOCK_MAX_COLUMN_COUNT ] = 80.f;
            }
        }

        setup_dock();
        //Makes the column visible if closed;
        if( saveToSettings )
        {
            save_dock_setings( "dock_settings.gpf" );
        }
    }
}

void gpeEditorDock::clear_all_panels()
{
    gpeEditorDockPanel * tempPanel = NULL;
    for( int i = 0; i < DOCK_MAX_PANEL_COUNT; i++)
    {
        tempPanel = panels[i];
        if( tempPanel!=NULL)
        {
            tempPanel->clear_panel();
        }
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
    if( panelColumnWidth[0] + panelColumnWidth[1] >= dockMinimumColumnPercentage * dockWidthMinusColumnPadding )
    {
        return (panelColumnWidth[0] + panelColumnWidth[1] + GENERAL_GPE_GUI_PADDING);
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
    if( panelColumnWidth[3] + panelColumnWidth[4] >= dockMinimumColumnPercentage * dockWidthMinusColumnPadding )
    {
        return (panelColumnWidth[3] + panelColumnWidth[4] + GENERAL_GPE_GUI_PADDING);
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
            resizePanelX1 = resizePanelX2 = 0;
            return -1;
        }
        else if( point_between_rect( input->mouse_x, input->mouse_y, &elementBox ) )
        {
            //The above if makes sure we are actually in the dock, avoiding negative results
            //Checks if we
            bool resizeExited = false;
            if( horizontalResize )
            {
                gpe->cursor_change( "sizewe" );

                //The code, below may appear unoptimized, but it rarely is executed since it only happens on ending of resize of dock...
                if( input->check_mouse_released( mb_left) )
                {
                    //Last check to make sure we are not out of bounds, we only resize from column 1 and to the right tho...
                    if( columnBeingResizedId > 0 && columnBeingResizedId < DOCK_MAX_COLUMN_COUNT )
                    {
                        gpeEditorDockPanel * foundTPanel = NULL;
                        gpeEditorDockPanel * foundBPanel = NULL;
                        float panelResizeDifference = 0;
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
                                    currentPanelX+= lastPanelWidth + GENERAL_GPE_GUI_PADDING;
                                    panelsUsedOnLeft++;
                                }
                            }
                        }

                        //Resize to the left...
                        if( input->mouse_x < currentPanelX  &&  panelsUsedOnLeft > 0 && lastLeftPanel >=0  )
                        {
                            //We will move to the left
                            //If the mouse is to the right of the last panel to the left
                            if( input->mouse_x > currentPanelX - lastPanelWidth - GENERAL_GPE_GUI_PADDING  )
                            {
                                //Only going to interact with the panel on the immediate left
                                panelResizeDifference = (currentPanelX - input->mouse_x - GENERAL_GPE_GUI_PADDING );

                                panelResizeDifference = (float)panelResizeDifference / (  dockWidthMinusColumnPadding / 100.000000d);
                                if( panelResizeDifference >= 0.0001 )
                                {
                                    panelWidthPercentages[ columnBeingResizedId ] +=  panelResizeDifference;
                                    panelWidthPercentages[ lastLeftPanel ] -= panelResizeDifference;
                                }
                            }
                            else
                            {
                                //A more complex resize operation not needed for 1.3.0 RC

                            }
                        }
                        //Move the panel to the right, where possible.
                        else if( panelsUsedOnLeft > 0 && lastLeftPanel >=0  )
                        {
                            //We will add to the panel
                            if( input->mouse_x > currentPanelX )
                            {
                                panelResizeDifference = ( input->mouse_x - currentPanelX );

                                //Panel resizing only is between two panels, so let's calculate the differences;
                                panelResizeDifference = (float)panelResizeDifference / (  dockWidthMinusColumnPadding / 100.00000d);
                                if( panelResizeDifference >= 0.0001 )
                                {
                                    panelWidthPercentages[ columnBeingResizedId ] -=  panelResizeDifference;
                                    panelWidthPercentages[ lastLeftPanel ] += panelResizeDifference;
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
                gpe->cursor_change( "sizens" );
                if( input->check_mouse_released( mb_left) )
                {
                    //Last check to make sure we are not out of bounds...
                    if( columnBeingResizedId >= 0 && columnBeingResizedId < DOCK_MAX_COLUMN_COUNT )
                    {
                        panelHeightPercentages[columnBeingResizedId] = ( (float) input->mouse_y - elementBox.y ) / ( elementBox.h - (float)GENERAL_GPE_GUI_PADDING ) * 100.f;
                        panelHeightPercentages[columnBeingResizedId + DOCK_MAX_COLUMN_COUNT] = 100.f - panelHeightPercentages[columnBeingResizedId];
                    }
                    resizeExited = true;
                    horiResizeStartX = 0;
                    columnBeingResizedId = -1;
                    beingResized = horizontalResize = verticalResize = false;
                    resizePanelX1 = 0;
                    resizePanelX2 = 0;
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
                save_dock_setings( "dock_settings.gpf" );
                return 1;
            }
        }
        else
        {
            //Exits the resize if we get out of range
            justResized = beingResized = horizontalResize = verticalResize = false;
            resizePanelX1 = resizePanelX2 = 0;
            columnBeingResizedId = -1;
            horiResizeStartX = 0;
            return -2;
        }
    }
    else
    {
        //If we started off not being resized we will check if user wants to resize
        gpeEditorDockPanel * tPanel = NULL;
        gpeEditorDockPanel * bPanel = NULL;

        int panelX = 0;
        resizePanelX1 = resizePanelX2 = 0;
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
                        //NS resize
                        if( point_between(input->mouse_x, input->mouse_y,tPanel->get_xpos(),tPanel->get_y2pos(), tPanel->get_x2pos(),tPanel->get_y2pos()+dockVerticalPadding ) )
                        {
                            gpe->cursor_change( "sizens" );
                            if(input->check_mouse_pressed(0) )
                            {
                                resizePanelX1 = tPanel->get_xpos();
                                resizePanelX2 = tPanel->get_x2pos();
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
                        if( point_between( input->mouse_x, input->mouse_y, panelX - dockHorizontalPadding,elementBox.y, panelX, elementBox.y2Pos ) )
                        {
                            gpe->cursor_change( "sizewe" );
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
                        panelX += panelColumnWidth[iColumn] + dockHorizontalPadding;
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

bool gpeEditorDock::load_dock_setings( std::string filename )
{
    //showStatupTipsBox->set_clicked( GPE_MAIN_GUI->showTipsAtStartUp );
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Loading Editor Settings", "Please wait..." );
    }

    std::string otherColContainerName = "";

    std::string newFileIn = get_user_settings_folder()+filename;
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    GPE_Report("Loading Local Dock settings - "+newFileIn);
    //If the level file could be loaded

    int iLoop = 0;

    //If the file failed to read we return false
    if( gameResourceFileIn.fail() )
    {
        GPE_Report("Failed to open dock settings!");
        return false;
    }
    //makes sure the file is open
    if ( !gameResourceFileIn.is_open())
    {
        GPE_Report("Failed to read dock settings!");
        return false;
    }

    int equalPos = 0;
    std::string firstChar="";
    std::string keyString="";
    std::string valString="";
    std::string subValString="";
    std::string currLine="";
    std::string currLineToBeProcessed;
    std::string colorThemeName;
    float foundFileVersion = 0;
    int foundDelayTime = 0;
    int foundFPSValue = FPS_CAP;
    while ( gameResourceFileIn.good() )
    {
        getline (gameResourceFileIn,currLine); //gets the next line of the file
        currLineToBeProcessed = trim_left_inplace(currLine);
        currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

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
                            foundFileVersion = string_to_float(valString);
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

                    //GPE_Report("Detected Key:     "+keyString);
                    if( keyString == "DockHidden")
                    {
                        isHidden = string_to_bool( valString);
                    }
                    else if( string_starts(keyString, "PanelWidthPercent[" ) )
                    {
                        for( int iLoop = 0; iLoop < DOCK_MAX_COLUMN_COUNT; iLoop++)
                        {
                            if( keyString == "PanelWidthPercent["+int_to_string(iLoop)+"]" )
                            {
                                panelWidthPercentages[ iLoop ] = string_to_float( valString);
                            }
                        }
                    }
                    else if( string_starts(keyString, "PanelHeightPercent[" ) )
                    {
                        for( int iLoop = 0; iLoop < DOCK_MAX_PANEL_COUNT; iLoop++)
                        {
                            if( keyString == "PanelHeightPercent["+int_to_string(iLoop)+"]" )
                            {
                                panelHeightPercentages[ iLoop ] = string_to_float( valString);
                            }
                        }
                    }
                }
            }
        }
    }

    GPE_Report("Loaded in dock settings..");
    GPE_Report("Setting up new dock..");
    return true;
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

    if( GPE_MAIN_GUI->mainResourceTree!=NULL && PANEL_RESOURCE_TREE!=NULL )
    {
        PANEL_RESOURCE_TREE->clear_panel();
        PANEL_RESOURCE_TREE->add_gui_element_fullsize(  GPE_MAIN_GUI->mainResourceTree );
    }
    //Process all of the remaining panels
    gpeEditorDockPanel *  tPanel = NULL;
    gpeEditorDockPanel *  bPanel = NULL;
    int tPanelButtonHitID = -1;
    int iPanel = 0;
    for( iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
    {
        if( iPanel!= DOCK_TOP_MIDDLE && iPanel!=DOCK_BOTTOM_MIDDLE)
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
        if( menuSelection >= 0 && menuSelection < DOCK_MAX_PANEL_COUNT )
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
    }
}

void gpeEditorDock::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam  )
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );


    setup_dock();
    //If no sort of resizing occurred
    if( handle_resizing() < 0 )
    {
        process_dock( viewedSpace, cam );
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

void gpeEditorDock::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam   )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);

    if( isHidden )
    {
        return;
    }

    if( beingResized && columnBeingResizedId >=0 )
    {
        GPE_MAIN_RENDERER->reset_viewpoint();
        //if( forceRedraw )
        {
            MAIN_OVERLAY->render_frozen_screenshot( );
        }
        if( horizontalResize )
        {
            gcanvas->render_rectangle( input->mouse_x-4, get_ypos(),input->mouse_x+4, get_y2pos(), GPE_MAIN_THEME->PopUp_Box_Border_Color, false );
        }
        else if( verticalResize)
        {
            gcanvas->render_rectangle( resizePanelX1,input->mouse_y+4, resizePanelX2, input->mouse_y-4, GPE_MAIN_THEME->PopUp_Box_Border_Color, false );
        }
    }
    else
    {
        GPE_MAIN_RENDERER->reset_viewpoint();
        gpeEditorDockPanel *  tPanel = NULL;
        for( int iPanel = 0; iPanel < DOCK_MAX_PANEL_COUNT; iPanel++ )
        {
            tPanel = panels[iPanel];
            if( tPanel!=NULL && tPanel->has_content() )
            {
                tPanel->render_self( viewedSpace, cam);
            }
        }

        if( GPE_DEFAULT_FONT == NULL )
        {
            return;
        }
    }
}

void gpeEditorDock::reset_dock()
{
    currentColumnCount = 0;
    dockWidthMinusColumnPadding = SCREEN_WIDTH;
    dockMinimumColumnPercentage = 5;
    for( int iColumn = 0; iColumn < DOCK_MAX_COLUMN_COUNT; iColumn++ )
    {
        panelWidthPercentages[ iColumn ] = 100.f / (float)DOCK_MAX_COLUMN_COUNT;
        panelColumnWidth[iColumn] = dockMinimumColumnPercentage * panelWidthPercentages[ iColumn ];
    }

    for( int iPanels = 0; iPanels < DOCK_MAX_PANEL_COUNT; iPanels++ )
    {
        panelHeightPercentages[iPanels] = 100.f/ (float)DOCK_MAX_ROW_COUNT;
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

bool gpeEditorDock::save_dock_setings( std::string filename )
{
    if( (int)filename.size() < 5 )
    {
        filename = "dock_settings.gpf";
    }
    std::string newSaveDataFilename = get_user_settings_folder()+filename;
    std::ofstream newSaveDataFile( newSaveDataFilename.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open() )
        {
            newSaveDataFile << "Version=" << GPE_VERSION_FLOAT_NUMBER << "\n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "DockHidden=" << isHidden << "\n";
            int i = 0;
            for ( i = 0; i < DOCK_MAX_COLUMN_COUNT; i++)
            {
                newSaveDataFile << "PanelWidth["+ int_to_string(i)+ "]=" << panelColumnWidth[i] << "\n";
                newSaveDataFile << "PanelWidthPercent["+ int_to_string(i)+ "]=" << panelWidthPercentages[i] << "\n";
            }
            for ( i = 0; i < DOCK_MAX_PANEL_COUNT; i++)
            {
                newSaveDataFile << "PanelHeightPercent["+ int_to_string(i)+ "]=" << panelHeightPercentages[i] << "\n";
            }
            newSaveDataFile << "\n";
        }
    }
}

void gpeEditorDock::setup_dock()
{
    panels[ DOCK_TOP_MIDDLE ]->specialPanelElement = GPE_Main_TabManager;
    panels[ DOCK_BOTTOM_MIDDLE ]->specialPanelElement = GPE_Main_Logs;

    dockWidthMinusColumnPadding = elementBox.w;
    currentColumnCount = 0;
    //checks the column max widths
    gpeEditorDockPanel * tPanel = NULL;
    gpeEditorDockPanel * bPanel = NULL;
    bool columnFound = false;
    float calculatedPercentages = 0;
    int iCol = 0;
    for(  iCol = 0; iCol < DOCK_MAX_COLUMN_COUNT; iCol++)
    {
        tPanel = panels[ iCol ];
        bPanel = panels[ iCol + DOCK_MAX_COLUMN_COUNT ];

        if( tPanel!=NULL && tPanel->has_content() )
        {
            columnFound = true;
        }

        if( bPanel != NULL &&  bPanel->has_content() )
        {
            columnFound = true;
        }
        if( columnFound )
        {
            currentColumnCount++;
            //panelWidthPercentages[iCol]  =  panelWidthPercentages[iCol]  );
            if( panelWidthPercentages[iCol] < dockMinimumColumnPercentage )
            {
                panelWidthPercentages[iCol] = dockMinimumColumnPercentage;
            }
            calculatedPercentages += panelWidthPercentages[iCol];

        }
        else
        {
            panelWidthPercentages[ iCol ] = 0.f;
            panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT ] = 0.f;
            panelHeightPercentages[ iCol ] = 0.f;
        }
        columnFound = false;
    }

    dockWidthMinusColumnPadding -=  (currentColumnCount -1) * (float)dockHorizontalPadding;

    float percentageDifference = 0;
    if( calculatedPercentages < 100.f )
    {
        percentageDifference = 100.f - calculatedPercentages;
        panelWidthPercentages[DOCK_TOP_MIDDLE] += percentageDifference;
        return;
    }
    else if( calculatedPercentages > 100.f )
    {
        percentageDifference =  calculatedPercentages -100.f;
        panelWidthPercentages[DOCK_TOP_MIDDLE] -= percentageDifference;
        return;
    }
    //Manages the heights of the panels
    for( iCol = 0; iCol < DOCK_MAX_COLUMN_COUNT; iCol++ )
    {
        tPanel = panels[iCol];
        bPanel = panels[iCol + DOCK_MAX_COLUMN_COUNT ];

        tPanel->clear_panel();
        bPanel->clear_panel();

        //Resets the panel's settings button
        if( tPanel->dockSettingsButton!=NULL )
        {
            tPanel->dockSettingsButton->set_clicked( false );
        }

        if( bPanel->dockSettingsButton!=NULL )
        {
            bPanel->dockSettingsButton->set_clicked( false );
        }

        panelColumnWidth[iCol] =  floor( dockWidthMinusColumnPadding * panelWidthPercentages[iCol]/100.f );

        //Checks to make sure heights are not out of the ordinary
        panelHeightPercentages[iCol] =  floor( panelHeightPercentages[iCol] );
        panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT ] =  floor( panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT ] );
        if( panelHeightPercentages[iCol] < 0.f )
        {
            panelHeightPercentages[iCol] = 0.f;
            panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT  ] = 100.f;
        }
        else if(  panelHeightPercentages[iCol]  > 100.f)
        {
            panelHeightPercentages[iCol] = 100.f;
            panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT  ] = 0.f;
        }

        if( panelHeightPercentages[iCol  + DOCK_MAX_COLUMN_COUNT  ] < 0.f )
        {
            panelHeightPercentages[iCol] = 100.f;
            panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT  ] = 0.f;
        }
        else if(  panelHeightPercentages[iCol  + DOCK_MAX_COLUMN_COUNT ]  > 100.f)
        {
            panelHeightPercentages[iCol] = 0.f;
            panelHeightPercentages[iCol  + DOCK_MAX_COLUMN_COUNT ] = 100.f;
        }


        if( tPanel->has_content() && bPanel->has_content())
        {
            tPanel->set_width( panelColumnWidth[ iCol ] );
            bPanel->set_width( panelColumnWidth[ iCol ] );

        }
        //If the botton doesnt have any content
        else if( tPanel->has_content() && !bPanel->has_content())
        {
            //cuts bottom panel from view
            panelHeightPercentages[iCol] = 100.f;
            panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT ] = 0.f;
            tPanel->set_width( panelColumnWidth[ iCol ] );
            tPanel->set_height( elementBox.h );

        }
        else if( bPanel->has_content() && !tPanel->has_content())
        {
            //cuts top panel from view
            bPanel->set_width( panelColumnWidth[ iCol ] );

            panelHeightPercentages[iCol] = 0.f;
            panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT ] = 100.f;
            bPanel->set_height( elementBox.h );
        }
        else
        {
            //This should not occur, since we should always have col*row panels
            //But here is the fuzzy code in case this rare case happens(Maybe low memory or something..
            //Defaults to top panel being 100% in height
            panelHeightPercentages[iCol] = 100.f;
            panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT ] = 0.f;
            if( tPanel->has_content() )
            {
                tPanel->set_width( panelColumnWidth[ iCol ] );
                tPanel->set_height( elementBox.h );
            }
        }

        tPanel->set_height( (elementBox.h-dockVerticalPadding) * panelHeightPercentages[iCol]/100.f );
        bPanel->set_height( (elementBox.h-dockVerticalPadding) * ( panelHeightPercentages[iCol + DOCK_MAX_COLUMN_COUNT] )/100.f );
    }

    //Horizontally Resizing panels;
    bool panelBeingHovered = false;

    //Let's set the position of the  panels
    int panelX = 0;
    int panelY = 0;
    int cPanel = 0;

    //Places the panels on the docks
    for( iCol = 0; iCol < DOCK_MAX_COLUMN_COUNT ; iCol++)
    {
        for( int jPanelRow = 0; jPanelRow < DOCK_MAX_ROW_COUNT ; jPanelRow++)
        {
            cPanel = iCol + jPanelRow*DOCK_MAX_COLUMN_COUNT;
            tPanel = panels[cPanel];
            if( tPanel!=NULL && tPanel->has_content() )
            {
                tPanel->set_coords(panelX, elementBox.y + panelY );

                //Basically only add to PanelY if its not the bottom one
                panelY+= panels[ cPanel]->get_height() + dockVerticalPadding;
            }
        }
        panelY = 0;
        if( panelColumnWidth[iCol] > 0.f )
        {
            panelX+= panelColumnWidth[ iCol ] + dockHorizontalPadding;
        }
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
                GPE_Report("Panel["+float_to_string(tempPair->keyValue)+"] found to toggle...");
                toggle_panel( tempPair->keyString, tempPair->keyValue, true );
            }
        }
    }
}

void gpeEditorDock::toggle_panel(  std::string name, int panelId,  bool open  )
{
    if( (int)name.size() == 0 )
    {
        return;
    }
    if( find_panel( name) != NULL )
    {
        GPE_Report("Panel already found, so removing...");
        remove_panel( name );
    }
    else
    {
        GPE_Report("Adding toggled panel to dock...");
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

