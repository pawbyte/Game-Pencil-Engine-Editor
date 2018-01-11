/*
pathresource.cpp
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

#include "pathresource.h"

GPE_PathPoint::GPE_PathPoint(int pointX, int pointY, float speed)
{
    xPos = pointX;
    yPos = pointY;
    pointSpeed = speed;
}

GPE_PathPoint::~GPE_PathPoint()
{

}

gamePathResource::gamePathResource(GPE_ResourceContainer * pFolder )
{
    projectParentFolder = pFolder;

    if( projectParentFolder!=NULL)
    {
        sceneToPreview = new GPE_DropDown_Resouce_Menu(0,0,"Previewed Scene",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE]+"s"),-1,true );
        sceneToPreview->set_width(128);
    }
    else
    {
        sceneToPreview = NULL;
    }
    sceneZoomLevel = new GPE_DropDown_Menu(0,0,"",true);
    sceneZoomLevel->add_menu_option("10%","10",10,false);
    sceneZoomLevel->add_menu_option("25%","25",25,false);
    sceneZoomLevel->add_menu_option("33%","33",33,false);
    sceneZoomLevel->add_menu_option("50%","50",50,false);
    sceneZoomLevel->add_menu_option("67%","67",67,false);
    sceneZoomLevel->add_menu_option("75%","75",75,false);
    sceneZoomLevel->add_menu_option("80%","80",80,false);
    sceneZoomLevel->add_menu_option("90%","90",90,false);
    sceneZoomLevel->add_menu_option("100%","100",100,true);
    sceneZoomLevel->add_menu_option("110%","110",110,false);
    sceneZoomLevel->add_menu_option("125%","125",125,false);
    sceneZoomLevel->add_menu_option("150%","150",150,false);
    sceneZoomLevel->add_menu_option("175%","175",175,false);
    sceneZoomLevel->add_menu_option("200%","200",200,false);
    sceneZoomLevel->add_menu_option("250%","250",250,false);
    sceneZoomLevel->add_menu_option("300%","300",300,false);
    sceneZoomLevel->add_menu_option("400%","400",400,false);
    sceneZoomLevel->add_menu_option("500%","500",500,false);
    sceneZoomLevel->set_width(96);

    pathOptions = new GPE_SelectBoxBasic(0,0,"Path Points");
    pathOptions->set_height(160);
    pathOptions->limit_height(160);

    pointSettingsButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/cogs.png","Path Settings",-1,32);
    pointRemoveButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/remove.png", "Remove Point",-1,32 );
    pointMoveUpButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/arrow-up.png", "Move Point Up",-1,32);
    pointMoveDownButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/arrow-down.png","Move Point Down",-1,32 );

    pathOpenType = new GPE_RadioButtonControllerBasic("Path Type",0,0);
    pathOpenType->add_menu_option("Open Path","open",0,true);
    pathOpenType->add_menu_option("Closed Path","closed",1,false);

    pathShapeType = new GPE_RadioButtonControllerBasic("Path Shape",0,0);
    pathShapeType->add_menu_option("Lines","lines",0,true);
    pathShapeType->add_menu_option("Curves","curves",1,false);

    sceneZoomAmount = 1;
    editorMode = 0;
    sceneEditorView.x = 128;
    sceneEditorView.y = 32;
    sceneEditorView.w = 640;
    sceneEditorView.h = 480;

    scenePreviewRect.x = 0;
    scenePreviewRect.y = 0;
    scenePreviewRect.w = 640;
    scenePreviewRect.h = 480;

    pathLineColor = c_blue->duplicate_color();
    pathPointColor = c_orange->duplicate_color();
    sceneXScroll = new GPE_ScrollBar_XAxis();
    update_rectangle(&sceneXScroll->contextRect,0,0,640,480);
    sceneYScroll = new GPE_ScrollBar_YAxis();
    update_rectangle(&sceneYScroll->contextRect,0,0,640,480);
    sceneAreaScrollable = false;
    sceneMouseXPos = 0;
    sceneMouseYPos = 0;

    pointPos = 0;
    selectedPointPos = 0;
    selectedPathPoint = NULL;
    currentPointX = new GPE_TextInputNumber("PointX",false,0);
    currentPointY = new GPE_TextInputNumber("PointY",false,0);
    currentPointSpeed = new GPE_TextInputNumber("PointSpeed",false,0);
}

gamePathResource::~gamePathResource()
{
    if( sceneZoomLevel!=NULL)
    {
        delete sceneZoomLevel;
        sceneZoomLevel = NULL;
    }
    if( pathOptions!=NULL)
    {
        delete pathOptions;
        pathOptions = NULL;
    }
    if( pointSettingsButtton!=NULL)
    {
        delete pointSettingsButtton;
        pointSettingsButtton = NULL;
    }

    if( pointRemoveButton!=NULL)
    {
        delete pointRemoveButton;
        pointRemoveButton = NULL;
    }
    if( pointMoveUpButtton!=NULL)
    {
        delete pointMoveUpButtton;
        pointMoveUpButtton = NULL;
    }
    if( pointMoveDownButton!=NULL)
    {
        delete pointMoveDownButton;
        pointMoveDownButton = NULL;
    }

    if( sceneToPreview!=NULL)
    {
        delete sceneToPreview;
        sceneToPreview = NULL;
    }

    if( sceneXScroll!=NULL)
    {
        delete sceneXScroll;
        sceneXScroll = NULL;
    }
    if( sceneYScroll!=NULL)
    {
        delete sceneYScroll;
        sceneYScroll = NULL;
    }
    clear_points();
}

GPE_PathPoint * gamePathResource::add_point( int pointX, int pointY, float pointSpeed )
{
    GPE_PathPoint * newPoint = new GPE_PathPoint(pointX, pointY, pointSpeed);
    if( pointPos >=0 && pointPos < (int)pathPoints.size() )
    {
        pathPoints.insert( pathPoints.begin()+pointPos+1,newPoint );
        if( pathOptions!=NULL)
        {
            pathOptions->insert_option(pointPos+1,"X:"+int_to_string(pointX)+" Y:"+int_to_string(pointY)+" Spd:"+float_to_string(pointSpeed),NULL,NULL,-1, true );
        }
        pointPos++;
    }
    else
    {
        pathPoints.push_back( newPoint );
        if( pathOptions!=NULL)
        {
            pathOptions->add_option("X:"+int_to_string(pointX)+"Y:"+int_to_string(pointY)+"Spd:"+float_to_string(pointSpeed),pointPos+1,NULL,NULL,-1,true );
        }
        pointPos = (int)pathPoints.size();
    }
}

bool gamePathResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string html5PathName = "_path_" + get_name();

        *fileTarget << nestedTabsStr << "var " + html5PathName + " =  GPE.add_path(";
        if( pathOpenType!=NULL && pathOpenType->get_selected_tag()=="closed" )
        {
            *fileTarget << int_to_string (html5BuildGlobalId ) +",true); \n";
        }
        else
        {
            *fileTarget << int_to_string (html5BuildGlobalId ) +",false); \n";
        }
        GPE_PathPoint * tempPoint = NULL;
        int pointCount = (int)pathPoints.size();
        for( int pointI = 0; pointI < pointCount; pointI++)
        {
            tempPoint = pathPoints[pointI];
            if( tempPoint!=NULL )
            {
                *fileTarget << nestedTabsStr+html5PathName+".add_original_point( ";
                *fileTarget << float_to_string(tempPoint->xPos)+",";
                *fileTarget << float_to_string(tempPoint->yPos)+",";
                *fileTarget << float_to_string(tempPoint->pointSpeed)+");\n";
            }
        }
        return true;
    }
    return false;
}

void gamePathResource::clear_points()
{
    selectedPointPos = -1;
    selectedPathPoint = NULL;
    GPE_PathPoint * tempPoint = NULL;
    for( int i = (int)pathPoints.size()-1; i >=0; i--)
    {
        tempPoint = pathPoints[i];
        if( tempPoint!=NULL)
        {
            delete tempPoint;
            tempPoint = NULL;
        }
    }
    pathPoints.clear();
    if( pathOptions!=NULL)
    {
        pathOptions->clear_list();
    }
}

bool gamePathResource::export_and_play_native( bool launchProgram )
{
    return false;
}

bool gamePathResource::get_mouse_coords(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    sceneMouseXPos = 0;
    sceneMouseYPos = 0;
    if( viewedSpace!=NULL)
    {
        if( point_within(userInput->mouse_x,userInput->mouse_y,
                         sceneEditorView.x+viewedSpace->x,
                         sceneEditorView.y+viewedSpace->y,
                         sceneEditorView.x+viewedSpace->x+sceneEditorView.w,
                         sceneEditorView.y+viewedSpace->y+sceneEditorView.h ) )
        {
            sceneMouseXPos = (userInput->mouse_x-sceneEditorView.x-viewedSpace->x)/sceneZoomAmount +scenePreviewRect.x;
            sceneMouseYPos = (userInput->mouse_y-sceneEditorView.y-viewedSpace->y)/sceneZoomAmount +scenePreviewRect.y;
            MAIN_OVERLAY->update_tooltip( "Mouse( "+int_to_string(sceneMouseXPos )+" , "+int_to_string(sceneMouseYPos)+")" );
            return true;
        }
    }
    return false;
}

void gamePathResource::handle_scrolling()
{
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    if( sceneAreaScrollable && editorPaneList->hasScrollControl==false )
    {
        if( userInput->check_keyboard_down(kb_ctrl) && sceneZoomLevel!=NULL )
        {
            //Zoom In
            if( userInput->mouseScrollingDown > 0 )
            {
                sceneZoomLevel->set_selection(sceneZoomLevel->get_selected_id()-1 );
            }
            else if( userInput->mouseScrollingUp)
            {
                //zoom out
                sceneZoomLevel->set_selection(sceneZoomLevel->get_selected_id()+1 );
            }

        }
        else if( userInput->shiftKeyIsPressed)
        {
            if( userInput->mouseScrollingUp > 0 )
            {
                xScrollHappened = true;
                scenePreviewRect.x-=(scenePreviewRect.w/16)*sceneZoomAmount;
            }
            else if( userInput->mouseScrollingDown)
            {
                //zoom out
                xScrollHappened = true;
                scenePreviewRect.x+=(scenePreviewRect.w/16)*sceneZoomAmount;
            }
        }
        else
        {
            if( userInput->mouseScrollingUp )
            {
                yScrollHappened = true;
                scenePreviewRect.y-=(scenePreviewRect.h/16)*sceneZoomAmount;
            }
            else if( userInput->mouseScrollingDown)
            {
                yScrollHappened = true;
                scenePreviewRect.y+=(scenePreviewRect.h/16)*sceneZoomAmount;
            }
            else if( editorPaneList->isInUse==false )
            {
                //arrow scrolling
                if( userInput->check_keyboard_down(kb_up) )
                {
                    yScrollHappened = true;
                    scenePreviewRect.y-=(scenePreviewRect.h/32)*sceneZoomAmount;
                }
                else if( userInput->check_keyboard_down(kb_down) )
                {
                    yScrollHappened = true;
                    scenePreviewRect.y+=(scenePreviewRect.h/32)*sceneZoomAmount;
                }
                if( userInput->check_keyboard_down(kb_left) )
                {
                    if( scenePreviewRect.x > (scenePreviewRect.w/32)*sceneZoomAmount )
                    {
                        xScrollHappened = true;
                        scenePreviewRect.x-=(scenePreviewRect.w/32)*sceneZoomAmount;
                    }
                    else
                    {
                        scenePreviewRect.x = 0;
                        xScrollHappened = true;
                    }
                }
                else if( userInput->check_keyboard_down(kb_right) )
                {
                    if( (scenePreviewRect.x +scenePreviewRect.w/32)*sceneZoomAmount < sceneRect.w*sceneZoomAmount )
                    {
                        xScrollHappened = true;
                        scenePreviewRect.x+=(scenePreviewRect.w/32)*sceneZoomAmount;
                    }
                }
            }
        }
    }



    if( scenePreviewRect.x+scenePreviewRect.w/sceneZoomAmount > sceneRect.w )
    {
        xScrollHappened = true;
        scenePreviewRect.x = sceneRect.w-scenePreviewRect.w/sceneZoomAmount;
    }

    if( scenePreviewRect.y+scenePreviewRect.h/sceneZoomAmount > sceneRect.h )
    {
        yScrollHappened = true;
        scenePreviewRect.y = sceneRect.h-scenePreviewRect.h/sceneZoomAmount;
    }

    if( scenePreviewRect.x <= 0)
    {
        xScrollHappened = true;
        scenePreviewRect.x = 0;
    }

    if( scenePreviewRect.y <= 0)
    {
        yScrollHappened = true;
        scenePreviewRect.y = 0;
    }

    if( xScrollHappened)
    {
        sceneXScroll->contextRect.x = scenePreviewRect.x;
        sceneXScroll->process_self(NULL,NULL,true);
    }
    if( yScrollHappened)
    {
        sceneXScroll->contextRect.y = scenePreviewRect.y;
        sceneYScroll->process_self(NULL,NULL,true);
    }
}

void gamePathResource::integrate_into_syntax()
{

}

void gamePathResource::open_code(int lineNumb, int colNumb, std::string codeTitle )
{

}

void gamePathResource::prerender_self(GPE_Renderer * cRender)
{

}

void gamePathResource::preprocess_self(std::string alternatePath )
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        displayMessageTitle = "Processing Dictionary";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();
        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/paths/";
        if( file_exists(alternatePath) )
        {
            newFileIn = alternatePath;
            soughtDir = get_path_from_file(newFileIn);
        }
        else
        {
            newFileIn = soughtDir + resourceName+".gpf";
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        record_error("Loading Path - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
        {
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string section="";
                std::string cur_layer="";
                std::string data_format="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                float foundFileVersion = 0;
                int foundX = 0;
                int foundY = 0;
                int foundSpeed = 0;
                int foundWaiver = 0;
                int foundR = 0, foundG = 0, foundB = 0;
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
                                equalPos = currLineToBeProcessed.find_first_of("=");
                                if(equalPos!=(int)std::string::npos)
                                {
                                    //if the equalPos is present, then parse on through and carryon
                                    keyString = currLineToBeProcessed.substr(0,equalPos);
                                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( keyString=="Version")
                                    {
                                        foundFileVersion = string_to_double(valString);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion < 2)
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
                                else if( keyString=="Point" || keyString=="point" )
                                {
                                    foundX = split_first_int(valString,',');
                                    foundY = split_first_int(valString,',');
                                    foundSpeed = split_first_int(valString,',');
                                    foundWaiver = split_first_int(valString,',');
                                    add_point( foundX, foundY, foundSpeed );
                                }
                                else if ( keyString == "pathType")
                                {
                                    pathOpenType->set_from_tag( split_first_string(valString, ",") );
                                }
                                else if ( keyString == "LineColor"|| keyString=="PathLineColor")
                                {
                                    foundR = split_first_int(valString, ',');
                                    foundG = split_first_int(valString, ',');
                                    foundB = split_first_int(valString, ',');
                                    if( pathLineColor!=NULL)
                                    {
                                        pathLineColor->change_rgba(foundR,foundG, foundB);
                                    }
                                }

                                else if ( keyString == "PointColor"|| keyString=="PathPointColor")
                                {
                                    foundR = split_first_int(valString, ',');
                                    foundG = split_first_int(valString, ',');
                                    foundB = split_first_int(valString, ',');
                                    if( pathPointColor!=NULL)
                                    {
                                        pathPointColor->change_rgba(foundR,foundG, foundB);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        record_error("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
    }
}

void gamePathResource::process_self(GPE_Rect * viewedSpace ,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( projectParentFolder!=NULL)
    {
        editorPane.x = 0;
        editorPane.y = 0;
        editorPane.w = 256;
        editorPane.h = viewedSpace->h;

        sceneEditorView.x = editorPane.x+editorPane.w;
        sceneEditorView.y = 0;
        sceneEditorView.w = scenePreviewRect.w = (int)(viewedSpace->w-sceneEditorView.x-sceneYScroll->get_box_width() )-16; //xcroll = 16px height
        sceneEditorView.h = scenePreviewRect.h = viewedSpace->h-64-sceneEditorView.y; //Comment pane = 32, yscroll = 16 height

        sceneRect.x =0;
        sceneRect.y =0;
        sceneRect.w =8192;
        sceneRect.h =8192;
        GPE_ResourceContainer * sceneTypeContainer =  sceneToPreview->get_selected_container();
        if( sceneTypeContainer!=NULL && sceneTypeContainer->get_held_resource()!=NULL )
        {
            generalGameResource * sceneGenObject = sceneTypeContainer->get_held_resource();
            if( sceneGenObject!=NULL)
            {
                gameSceneResource * foundSceneObject = (gameSceneResource *)sceneGenObject;
                if( foundSceneObject!=NULL)
                {
                    sceneRect.w = foundSceneObject->sceneRect.w;
                    sceneRect.h = foundSceneObject->sceneRect.h;
                }
            }
        }

        editorCommentPane.x = sceneEditorView.x;
        editorCommentPane.y = sceneEditorView.y+sceneEditorView.h+16;
        editorCommentPane.w = viewedSpace->w-sceneEditorView.x;
        editorCommentPane.h = 32;
        if(cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL)
        {

            editorPaneList->set_coords(0,0);
            editorPaneList->set_width(256);
            editorPaneList->set_height(viewedSpace->h);
            editorPaneList->barXPadding = GENERAL_GPE_PADDING;
            editorPaneList->barYPadding = GENERAL_GPE_PADDING;
            editorPaneList->barXMargin  = GENERAL_GPE_PADDING;
            editorPaneList->barYMargin  = 0;
            editorPaneList->clear_list();
            editorPaneList->add_gui_element(renameBox,true);
            editorPaneList->add_gui_element(pathOptions,true);
            if( selectedPathPoint!=NULL)
            {
                editorPaneList->add_gui_element(currentPointX,true);
                editorPaneList->add_gui_element(currentPointY,true);
                editorPaneList->add_gui_element(currentPointSpeed,true);
            }
            editorPaneList->add_gui_element(pointSettingsButtton,false);
            editorPaneList->add_gui_element(pointRemoveButton,false);
            editorPaneList->add_gui_element(pointMoveUpButtton,false);
            editorPaneList->add_gui_element(pointMoveDownButton,true);
            editorPaneList->add_gui_element(pathOpenType,true);
            //editorPaneList->add_gui_element(pathShapeType,true);


            editorPaneList->add_gui_element(exportResourceButton,true);
            editorPaneList->add_gui_element(loadResourceButton,true);
            editorPaneList->add_gui_element(confirmResourceButton,true);
            editorPaneList->add_gui_element(cancelResourceButton,true);
            editorPaneList->process_self( viewedSpace, cam);

            if( pathOptions!=NULL)
            {
                pointPos = pathOptions->get_selection();
                if( selectedPointPos !=pointPos )
                {
                    selectedPointPos = -1;
                    selectedPathPoint = NULL;
                }
            }
            if( pointSettingsButtton!=NULL && pointSettingsButtton->is_clicked() )
            {
                GPE_open_context_menu();
                MAIN_CONTEXT_MENU->set_width(256);

                MAIN_CONTEXT_MENU->add_menu_option("Change Points Color",1,NULL,-1,NULL,false,true);
                MAIN_CONTEXT_MENU->add_menu_option("Change Lines Color",2,NULL,-1,NULL,false,true);
                MAIN_CONTEXT_MENU->add_menu_option("Clear Point List",3,NULL,-1,NULL,false,true);
                int menuSelection = get_popupmenu_result();
                if( menuSelection ==1)
                {
                    GPE_Color * foundColor = GPE_Get_Color_PopUp("Change Points Color",pathPointColor);
                    if( foundColor!=NULL )
                    {
                        delete pathPointColor;
                        pathPointColor = foundColor;
                    }
                }
                else if( menuSelection ==2)
                {
                    GPE_Color * foundColor = GPE_Get_Color_PopUp("Change Lines Color",pathLineColor);
                    if( foundColor!=NULL )
                    {
                        delete pathLineColor;
                        pathLineColor = foundColor;
                    }
                }
                else if( menuSelection ==3)
                {
                    if( (int)pathPoints.size() > 0 )
                    {
                        if( (int)pathPoints.size() <=5)
                        {
                            clear_points();
                        }
                        else
                        {
                            if( display_get_prompt("Path Clear","Are you sure you will like to clear this path?") ==DISPLAY_QUERY_YES )
                            {
                                clear_points();
                            }
                        }
                    }
                }
            }
            else if( pointRemoveButton!=NULL && pointRemoveButton->is_clicked() )
            {
                remove_point( pointPos );
                selectedPointPos = -1;
                selectedPathPoint = NULL;
            }
            if( sceneToPreview!=NULL)
            {
                sceneToPreview->set_coords(editorCommentPane.x+GENERAL_GPE_PADDING,editorCommentPane.y+GENERAL_GPE_PADDING );
                sceneToPreview->process_self(viewedSpace,cam);
            }

            if( sceneZoomLevel!=NULL)
            {
                if( sceneToPreview!=NULL)
                {
                    sceneZoomLevel->set_coords(sceneToPreview->get_x2pos()+GENERAL_GPE_PADDING, editorCommentPane.y+GENERAL_GPE_PADDING );
                }
                else
                {
                    sceneZoomLevel->set_coords(editorCommentPane.x+GENERAL_GPE_PADDING, editorCommentPane.y+GENERAL_GPE_PADDING);
                }
                sceneZoomLevel->process_self(viewedSpace,cam);
                sceneZoomAmount = sceneZoomLevel->get_selected_value();
                if( sceneZoomAmount < 0)
                {
                    sceneZoomAmount = 1;
                    sceneZoomLevel->set_value(100);
                }
                sceneZoomAmount/=100;
            }
            else
            {
                sceneZoomAmount = 1;
            }
        }


        //if( editorPaneList->hasScrollControl==false && layerPaneList->hasScrollControl==false )
        {
            //Horizontal scrolling
            update_rectangle(&sceneXScroll->barBox,sceneEditorView.x,sceneEditorView.y+sceneEditorView.h,sceneEditorView.w,16);
            update_rectangle(&sceneXScroll->fullRect,0,0,(double)sceneRect.w, (double)sceneRect.h );
            update_rectangle(&sceneXScroll->contextRect,(double)scenePreviewRect.x,(double)scenePreviewRect.y, (double)scenePreviewRect.w/sceneZoomAmount, (double)scenePreviewRect.h/sceneZoomAmount );
            sceneXScroll->process_self(viewedSpace,cam, true );
            //if( sceneXScroll->has_moved() )
            {
                scenePreviewRect.x = (double)(sceneXScroll->contextRect.x);
            }

            //Vertical Scrolling
            update_rectangle(&sceneYScroll->barBox,sceneEditorView.x+sceneEditorView.w,sceneEditorView.y,16,sceneEditorView.h);
            update_rectangle(&sceneYScroll->fullRect,0,0,(double)sceneRect.w, (double)sceneRect.h );
            update_rectangle(&sceneYScroll->contextRect,(double)scenePreviewRect.x,(double)scenePreviewRect.y, (double)scenePreviewRect.w/sceneZoomAmount, (double)scenePreviewRect.h/sceneZoomAmount );
            //sceneYScroll->contextRect.h = sceneEditorView.h;
            sceneYScroll->process_self(viewedSpace,cam, true );
            //if( sceneYScroll->has_moved() )
            {
                scenePreviewRect.y = double(sceneYScroll->contextRect.y);
            }
        }

        if( userInput->input_received() )
        {
            get_mouse_coords(viewedSpace,cam);
        }
        //if( userInput->check_mouse_pressed(0) ||  userInput->check_mouse_pressed(1) || userInput->check_mouse_pressed(2) )
        {
            if( get_mouse_coords(viewedSpace,cam) )
            {
                sceneAreaScrollable = true;
                editorPaneList->hasScrollControl = false;
            }
            else
            {
                sceneAreaScrollable = false;
            }
        }

        int pointSize = std::max( 1, (int)( 7*sceneZoomAmount) );
        GPE_PathPoint * tempPoint = NULL;
        bool foundPoint  = false;
        int i = 0;
        if( userInput->check_mouse_down(0) )
        {
            if( get_mouse_coords(viewedSpace,cam) )
            {
                if( sceneMouseXPos-scenePreviewRect.x < 0 )
                {
                    scenePreviewRect.x = 0;
                }

                if( sceneMouseYPos-scenePreviewRect.y < 0 )
                {
                    scenePreviewRect.y = 0;
                }
            }
            /*if(sceneYScroll->is_held() )
            {
                sceneEditorViewRect.x+=16;
            }
            if(sceneXScroll->is_held() )
            {
                //sceneEditorViewRect.y+=16;
            }*/
        }
        else if( userInput->check_mouse_released(0) )
        {
            //Adds, selects or moves a point
            if( get_mouse_coords(viewedSpace,cam) )
            {
                if( selectedPathPoint!=NULL)
                {
                    selectedPathPoint->xPos = sceneMouseXPos;
                    selectedPathPoint->yPos = sceneMouseYPos;
                    if( currentPointX !=NULL)
                    {
                        currentPointX->set_number( sceneMouseXPos );
                    }
                    if( currentPointY !=NULL)
                    {
                        currentPointY->set_number( sceneMouseYPos );
                    }
                    if( pathOptions!=NULL)
                    {
                        pathOptions->rename_option( selectedPointPos, "X:"+int_to_string(selectedPathPoint->xPos)+" Y:"+int_to_string(selectedPathPoint->yPos)+" Spd:"+float_to_string(selectedPathPoint->pointSpeed) );
                    }
                }
                else
                {
                    for( i = (int)pathPoints.size()-1; i >=0 && !foundPoint; i--)
                    {
                        tempPoint = pathPoints[i];
                        if( tempPoint!=NULL)
                        {
                            if( point_between( (int)sceneMouseXPos,(int)sceneMouseYPos, tempPoint->xPos -pointSize ,tempPoint->yPos - pointSize,tempPoint->xPos +pointSize,tempPoint->yPos +pointSize) )
                            {
                                foundPoint = true;
                                selectedPathPoint = tempPoint;
                                if( currentPointX !=NULL)
                                {
                                    currentPointX->set_number( selectedPathPoint->xPos );
                                }
                                if( currentPointY !=NULL)
                                {
                                    currentPointY->set_number( selectedPathPoint->yPos );
                                }
                                if( currentPointSpeed !=NULL)
                                {
                                    currentPointSpeed->set_number( selectedPathPoint->pointSpeed );
                                }
                                pointPos = i;
                                selectedPointPos = i;
                                if( pathOptions!=NULL)
                                {
                                    pathOptions->set_selection( i);
                                }
                            }
                        }
                    }

                    if( selectedPathPoint == NULL)
                    {
                        GPE_PathPoint * newPoint = add_point(sceneMouseXPos, sceneMouseYPos, 1);
                    }
                }
            }
        }
        else if( userInput->check_mouse_released(1) )
        {
            //removes points
            if( get_mouse_coords(viewedSpace,cam) )
            {
                for( i = (int)pathPoints.size()-1; i >=0 && !foundPoint; i--)
                {
                    tempPoint = pathPoints[i];
                    if( tempPoint!=NULL)
                    {
                        if( point_between( (int)sceneMouseXPos,(int)sceneMouseYPos, tempPoint->xPos -pointSize ,tempPoint->yPos - pointSize,tempPoint->xPos +pointSize,tempPoint->yPos +pointSize) )
                        {
                            foundPoint = true;
                            remove_point( i );
                        }
                    }
                }
            }
            selectedPathPoint = NULL;
            selectedPointPos = -1;
        }
        else if( selectedPathPoint!=NULL)
        {
            bool foundPointChange = false;
            float foundNumber = 0;
            if( currentPointX!=NULL && currentPointX->is_valid() )
            {
                foundNumber = currentPointX->get_held_number();
                if( foundNumber >=0 && foundNumber!=selectedPathPoint->xPos)
                {
                    selectedPathPoint->xPos = foundNumber;
                    foundPointChange = true;
                }
            }

            if( currentPointX!=NULL && currentPointX->is_valid() )
            {
                foundNumber = currentPointY->get_held_number();
                if( foundNumber >=0 && foundNumber!=selectedPathPoint->yPos)
                {
                    selectedPathPoint->yPos = foundNumber;
                    foundPointChange = true;
                }
            }

            if( pathOptions!=NULL && foundPointChange)
            {
                pathOptions->rename_option( selectedPointPos, "X:"+int_to_string(selectedPathPoint->xPos)+" Y:"+int_to_string(selectedPathPoint->yPos)+" Spd:"+float_to_string( selectedPathPoint->pointSpeed) );
            }
        }
        handle_scrolling();
    }
}

bool gamePathResource::remove_point( int pointId )
{
    if( pointId >=0 && pointId < (int)pathPoints.size() )
    {
        GPE_PathPoint * tempPoint = pathPoints[pointId];
        if( tempPoint!=NULL )
        {
            delete tempPoint;
            tempPoint = NULL;
        }
        pathPoints.erase(pathPoints.begin()+ pointId );
        if( pathOptions!=NULL )
        {
            pathOptions->remove_option( pointId );
        }

        if( pointPos >=pointId)
        {
            pointId-=1;
        }
        pointPos--;
        if( pointPos < 0)
        {
            pointPos = 0;
        }

        selectedPathPoint = NULL;
        selectedPointPos = -1;
        return true;

    }
    return false;
}

void gamePathResource::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect * cam , bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && viewedSpace!=NULL )
    {
        if( forceRedraw)
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
        }

        GPE_ResourceContainer * sceneTypeContainer =  sceneToPreview->get_selected_container();
        if( sceneTypeContainer!=NULL && sceneTypeContainer->get_held_resource()!=NULL )
        {
            generalGameResource * sceneGenObject = sceneTypeContainer->get_held_resource();
            if( sceneGenObject!=NULL)
            {
                gameSceneResource * foundSceneObject = (gameSceneResource *)sceneGenObject;
                if( foundSceneObject!=NULL)
                {
                    foundSceneObject->render_scene_layers(cRender,viewedSpace, cam, &sceneEditorView, &scenePreviewRect,sceneZoomAmount,false, false, forceRedraw);
                }
            }
        }


        if( forceRedraw )
        {
            GPE_PathPoint * tempPoint = NULL;
            int tempXPoint = 0, tempYPoint = 0;
            int tempX2Point = 0, tempY2Point = 0;
            int pointI = 0;
            int pointCount = (int)pathPoints.size();
            for( pointI = 0; pointI < pointCount; pointI++)
            {
                tempPoint = pathPoints[pointI];
                if( tempPoint!=NULL )
                {
                    tempXPoint = floor( tempPoint->xPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                    tempYPoint = floor( tempPoint->yPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                    if( pointI >=1)
                    {
                        render_line(cRender, tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathLineColor, 255);
                    }
                    tempX2Point = tempXPoint;
                    tempY2Point = tempYPoint;
                }
            }

            if( pathOpenType!=NULL && pointCount >2)
            {
                tempPoint = pathPoints[0];
                if( tempPoint!=NULL && pathOpenType->get_selected_tag()=="closed" )
                {
                    tempXPoint = floor( tempPoint->xPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                    tempYPoint = floor( tempPoint->yPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                    render_line(cRender, tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathLineColor, 255);
                }
            }

            for( pointI = 0; pointI < (int)pathPoints.size(); pointI++)
            {
                tempPoint = pathPoints[pointI];
                if( tempPoint!=NULL )
                {
                    tempXPoint = floor( tempPoint->xPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                    tempYPoint = floor( tempPoint->yPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                    if( selectedPointPos==pointI)
                    {
                        render_circle_filled_rgba(cRender,tempXPoint,tempYPoint, std::max( 1, (int)( 7*sceneZoomAmount) ),pathLineColor->get_r(),pathLineColor->get_g(),pathLineColor->get_b(), 255 );
                    }
                    else
                    {
                        render_circle_filled_rgba(cRender,tempXPoint,tempYPoint, std::max( 1, (int)( 7*sceneZoomAmount) ),pathPointColor->get_r(),pathPointColor->get_g(),pathPointColor->get_b(), 255 );
                    }
                    //render_rectangle(cRender,tempXPoint, tempYPoint, tempXPoint+4, tempYPoint+4,pathPointColor, false, 255);
                    //record_error("Rendering point ["+int_to_string(pointI)+"] at ("+int_to_string(tempXPoint)+","+int_to_string(tempYPoint)+")");
                }
            }

            if( selectedPathPoint!=NULL)
            {
                tempXPoint = floor( sceneMouseXPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                tempYPoint = floor( sceneMouseYPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                render_circle_filled_rgba(cRender,tempXPoint,tempYPoint, std::max( 1, (int)( 7*sceneZoomAmount) ),pathLineColor->get_r(),pathLineColor->get_g(),pathLineColor->get_b(), 128 );

                if( selectedPointPos >=0 && selectedPointPos < (int)pathPoints.size() && pointCount >=2 )
                {
                    if( selectedPointPos == 0)
                    {
                        tempPoint = pathPoints[1];
                        if( tempPoint!=NULL )
                        {
                            tempX2Point = floor( tempPoint->xPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                            tempY2Point = floor( tempPoint->yPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                            render_line(cRender, tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                        }

                        if( pathOpenType!=NULL && pathOpenType->get_selected_tag()=="closed"  && pointCount >=3 )
                        {
                            tempPoint = pathPoints[ pointCount - 1];
                            if( tempPoint!=NULL)
                            {
                                tempX2Point = floor( tempPoint->xPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                                tempY2Point = floor( tempPoint->yPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                                render_line(cRender, tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                            }
                        }
                    }
                    else if( selectedPointPos == pointCount-1 )
                    {
                        if( pointCount >2 )
                        {
                            tempPoint = pathPoints[ selectedPointPos -1];
                            if( tempPoint!=NULL )
                            {
                                tempX2Point = floor( tempPoint->xPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                                tempY2Point = floor( tempPoint->yPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                                render_line(cRender, tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                            }

                            if( pathOpenType!=NULL && pathOpenType->get_selected_tag()=="closed"  && pointCount >=3 )
                            {
                                tempPoint = pathPoints[ 0];
                                if( tempPoint!=NULL)
                                {
                                    tempX2Point = floor( tempPoint->xPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                                    tempY2Point = floor( tempPoint->yPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                                    render_line(cRender, tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                                }
                            }
                        }
                    }
                    else
                    {
                        tempPoint = pathPoints[ selectedPointPos-1];
                        if( tempPoint!=NULL)
                        {
                            tempX2Point = floor( tempPoint->xPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                            tempY2Point = floor( tempPoint->yPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                            render_line(cRender, tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                        }

                        tempPoint = pathPoints[ selectedPointPos+1];
                        if( tempPoint!=NULL)
                        {
                            tempX2Point = floor( tempPoint->xPos*sceneZoomAmount+sceneEditorView.x-scenePreviewRect.x*sceneZoomAmount );
                            tempY2Point = floor( tempPoint->yPos*sceneZoomAmount+sceneEditorView.y-scenePreviewRect.y*sceneZoomAmount );
                            render_line(cRender, tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                        }
                    }
                }
            }
            //"Editor View:("+int_to_string(sceneEditorViewRect.x)+","+int_to_string(sceneEditorViewRect.y)+","+int_to_string(sceneEditorViewRect.w)+","+int_to_string(sceneEditorViewRect.h)+")",

            render_rect(cRender,&editorCommentPane,GPE_MAIN_TEMPLATE->Program_Color,false);
            render_rectangle(cRender,sceneYScroll->barBox.x+sceneYScroll->barBox.w,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
            render_rectangle(cRender,sceneXScroll->barBox.x,sceneXScroll->barBox.y,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
            if( sceneXScroll!=NULL)
            {
                sceneXScroll->render_self(cRender,viewedSpace,cam);
            }
            if( sceneYScroll!=NULL)
            {
                sceneYScroll->render_self(cRender,viewedSpace,cam);
            }
        }

        render_rect(cRender,&editorPane,GPE_MAIN_TEMPLATE->Program_Color,false);
        if( editorPaneList!=NULL )
        {
            editorPaneList->render_self( cRender,viewedSpace, cam, forceRedraw );
        }
        render_rectangle(cRender, sceneEditorView.x,0,viewedSpace->w,sceneEditorView.y,GPE_MAIN_TEMPLATE->Program_Color,false);
        if( sceneToPreview!=NULL )
        {
            sceneToPreview->render_self( cRender,viewedSpace, cam, forceRedraw );
        }

        if( sceneZoomLevel!=NULL )
        {
            sceneZoomLevel->render_self( cRender,viewedSpace, cam, forceRedraw );
        }
    }
}

void gamePathResource::save_resource(std::string alternatePath, int backupId )
{
    displayMessageTitle = "Saving Game Path";
    displayMessageSubtitle = resourceName;
    displayMessageString = "...";
    display_user_messaage();

    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = get_path_from_file(alternatePath);
    if( path_exists(soughtDir) )
    {
        newFileOut = alternatePath;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/paths/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the font file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    Game Pencil Engine Project Path DataFile \n";
            newSaveDataFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newSaveDataFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newSaveDataFile << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            newSaveDataFile << "#     \n";
            if( sceneToPreview!=NULL)
            {
                newSaveDataFile << "RelevantScene=" << sceneToPreview->get_selected_name() << "\n";
            }
            else
            {
                newSaveDataFile << "RelevantScene=-1\n";
            }

            if( pathOpenType!=NULL)
            {
                newSaveDataFile << "pathType="+pathOpenType->get_selected_tag()+",\n";
            }

            if( pathLineColor!=NULL)
            {
                newSaveDataFile << "LineColor="+int_to_string( (int)pathLineColor->get_r() )+","+int_to_string( (int)pathLineColor->get_g() )+","+int_to_string( (int)pathLineColor->get_b() )+",\n";
            }

            if( pathPointColor!=NULL)
            {
                newSaveDataFile << "PointColor="+int_to_string( (int)pathPointColor->get_r() )+","+int_to_string( (int)pathPointColor->get_g() )+","+int_to_string( (int)pathPointColor->get_b() )+",\n";
            }
            std::string resFileLocation = "";
            std::string resFileCopySrc;
            std::string resFileCopyDest;
            GPE_PathPoint * tempPoint = NULL;
            for( int i = 0; i < (int)pathPoints.size(); i++)
            {
                tempPoint = pathPoints[i];
                if( tempPoint!=NULL )
                {
                    newSaveDataFile << "Point="+int_to_string( tempPoint->xPos )+","+int_to_string( tempPoint->yPos )+","+ float_to_string( tempPoint->pointSpeed )+",\n";
                }
            }

            newSaveDataFile.close();
            if( !usingAltSaveSource)
            {
                isModified = false;
            }
            displayMessageTitle = "Path Successfully Saved!";
            displayMessageSubtitle = resourceName;
            displayMessageString = "...";
            display_user_messaage();
        }
        else
        {
            GPE_Main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}


int gamePathResource::search_for_string(std::string needle)
{
    return 0;
}

int gamePathResource::search_and_replace_string(std::string needle, std::string newStr )
{
    return 0;
}

void gamePathResource::update_project_layers()
{

}

bool gamePathResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Path=" << resourceName << "," << get_global_rid() << ",";

            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}
