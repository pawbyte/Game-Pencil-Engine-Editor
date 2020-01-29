/*
animation_resource.cpp
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

#include "animation_resource.h"
#include "gpe_editor_settings.h"

std::string GPE_ANIMATION_LABELS[ANIMATION_DATA_FIELD_COUNT];

animationResource::animationResource(GPE_GeneralResourceContainer * pFolder)
{
    autoAnimationPos = 0;
    animationTrackBar = new GPE_Slider_XAxis();

    animatonPreviewRect = new GPE_Rect(0,0,0,0);
    animCameraRect.x = animRect.x = 0;
    animCameraRect.y = animRect.y = 0;
    animCameraRect.w = animRect.w = 0;
    animCameraRect.h = animRect.h = 0;
    areaIsScrollable = false;
    animationPreviewCam = new GPE_Rect();
    animationSpeed = 1;
    editorZoomValue = 1;
    minZoomValue = 0.0625;
    maxZoomValue = 16;

    projectParentFolder = pFolder;
    animInEditor = new GPE_Animation( gpeph->get_default_render_package() );
    animationSpeedLabel = new GPE_Label_Text("Preview Speed","Preview Speed");
    labelanimationDimensions = new GPE_Label_Text("animation Dimensions");
    previewZoomLevel = new GPE_CheckBoxBasic("Preview Zoom Level","Preview Zoom Level", true);
    previewSubImageNumbers = new GPE_CheckBoxBasic("Sub-images","Sub-images", true);

    labelFrameInfo = new GPE_Label_Text("Frame Info");
    labelanimationMessage = new GPE_Label_Text("animation Message");
    //animation Collision Area
    preloadCheckBox = new GPE_CheckBoxBasic("Preload animation","Check to load animation at game open", true);
    showCollisionShapeCheckBox = new GPE_CheckBoxBasic("Preview Collision Shape","", true);
    showAnimationCheckBox = new GPE_CheckBoxBasic("Preview animation Animation","", true);
    animationCollisionShapeMenu = new GPE_DropDown_Menu( "Collision Shape",true);
    animationCollisionShapeMenu->add_menu_option("Rectangle","rectangle",0,true);
    animationCollisionShapeMenu->add_menu_option("Circle","circle",1,false);
    //animationCollisionShapeMenu->add_menu_option("Triangle [ Coming Soon]");
    //animationCollisionShapeMenu->add_menu_option("Diamond [ Coming Soon]");
    //animationCollisionShapeMenu->add_menu_option("Hexagon [ Coming Soon]");

    animationCenterCollisionButton = new GPE_ToolLabelButton( "Center Collision Shape","Centers the collision box");
    animationCollisionRectX = new GPE_TextInputNumber("0",true,0);
    animationCollisionRectX->set_label("X-Pos");
    animationCollisionRectX->set_string("0");
    animationCollisionRectX->scale_width(0.5);

    animationCollisionRectY = new GPE_TextInputNumber("0",true,0);
    animationCollisionRectY->set_label("Y-Pos");
    animationCollisionRectY->set_string("0");
    animationCollisionRectY->scale_width(0.5);

    animationCollisionRectW = new GPE_TextInputNumber("0",true,0);
    animationCollisionRectW->set_label("Width");
    animationCollisionRectW->set_string("0");
    animationCollisionRectW->scale_width(0.5);

    animationCollisionRectH = new GPE_TextInputNumber("0",true,0);
    animationCollisionRectH->set_label("Height");
    animationCollisionRectH->set_string("0");
    animationCollisionRectH->scale_width(0.5);

    animationCollisionCircleX = new GPE_TextInputNumber("0",true,0);
    animationCollisionCircleX->set_label("X-Pos");
    animationCollisionCircleX->set_string("0");

    animationCollisionCircleY = new GPE_TextInputNumber("0",true,0);
    animationCollisionCircleY->set_label("Y-Pos");
    animationCollisionCircleY->set_string("0");

    animationCollisionCircleR = new GPE_TextInputNumber("0",true,0);
    animationCollisionCircleR->set_label("Radius");
    animationCollisionCircleR->set_string("0");
    int i = 0;

    animationDataFields[i] = new GPE_TextInputNumber("");
    //GENERAL_GPE_GUI_PADDING*2+animationDataLabelWidth,GENERAL_GPE_GUI_PADDING+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING*(i+1),64,GPE_AVERAGE_LINE_HEIGHT,"0",false,INT_MIN,INT_MAX);
    animationDataFields[i]->set_string("0");
    animationDataFields[i]->set_label(GPE_ANIMATION_LABELS[i]);

    for( i =1; i < ANIMATION_DATA_FIELD_COUNT; i++)
    {
        animationDataFields[i] = new GPE_TextInputNumber("");
        //GENERAL_GPE_GUI_PADDING*2+animationDataLabelWidth,GENERAL_GPE_GUI_PADDING+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING*(i+1),64,GPE_AVERAGE_LINE_HEIGHT,"0",true,0,512);
        animationDataFields[i]->set_string("0");
        animationDataFields[i]->set_label(GPE_ANIMATION_LABELS[i]);
    }
    editResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/edit.png","Edit animation Data",-1);
    transformResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/magic.png","Transform the Image",-1);
    playPauseResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png","Stop",-1);
    subImagePreviewNumber = 0;
    subImageMiniAnimationNumber = 0;
    resourcePostProcessed = false;

    //Subimage
    previousSubImageButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrow-left.png","Previous");
    //previousSubImageButton->set_animation(mainMenuanimation, 36 );
    nextSubImageButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrow-right.png","Next");
    //nextSubImageButton->set_animation(mainMenuanimation, 38 );
    subImageNumberField = new GPE_TextInputNumber("");
    subImageNumberField->set_string("0");
    subImageNumberField->set_width(64);
    subImageEditorPreviewId = new GPE_TextInputNumber("");
    subImageEditorPreviewId->set_string("0");
    subImageEditorPreviewId->set_label("Editor Preview Frame");
    openExternalEditorButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens animation Image In External Editor");
    refreshResourceDataButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded animation image");
    labelInfoMaxTextureSize = new GPE_Label_Text("Max Image Size: 8192 X 8192px","Max Image Size: 8192 X 8192px");

    animXScroll = new GPE_ScrollBar_XAxis();
    animYScroll = new GPE_ScrollBar_YAxis();
}

animationResource::~animationResource()
{
    if( animatonPreviewRect!=NULL )
    {
        delete animatonPreviewRect;
        animatonPreviewRect = NULL;
    }
    if( animationTrackBar!=NULL )
    {
        delete animationTrackBar;
        animationTrackBar = NULL;
    }
    if( previewZoomLevel!=NULL )
    {
        delete previewZoomLevel;
        previewZoomLevel = NULL;
    }
    if( previewSubImageNumbers!=NULL )
    {
        delete previewSubImageNumbers;
        previewSubImageNumbers = NULL;
    }
    if( animationPreviewCam!=NULL )
    {
        delete animationPreviewCam;
        animationPreviewCam = NULL;
    }
    if( animationSpeedLabel!=NULL )
    {
        delete animationSpeedLabel;
        animationSpeedLabel = NULL;
    }
    if( labelInfoMaxTextureSize!=NULL )
    {
        delete labelInfoMaxTextureSize;
        labelInfoMaxTextureSize = NULL;
    }
    if( openExternalEditorButton!=NULL )
    {
        delete openExternalEditorButton;
        openExternalEditorButton = NULL;
    }
    if( refreshResourceDataButton!=NULL )
    {
        delete refreshResourceDataButton;
        refreshResourceDataButton = NULL;
    }
    if( animInEditor!=NULL )
    {
        delete animInEditor;
        animInEditor = NULL;
    }
    GPE_TextInputNumber *  tempDataField = NULL;
    for( int i =ANIMATION_DATA_FIELD_COUNT-1; i >=0; i--)
    {
        tempDataField = animationDataFields[i];
        if( tempDataField!=NULL )
        {
            delete tempDataField;
            tempDataField = NULL;
        }
        animationDataFields[i] = NULL;
    }

    if( editResourceButton!=NULL)
    {
        delete editResourceButton;
        editResourceButton = NULL;
    }
    if( playPauseResourceButton!=NULL)
    {
        delete playPauseResourceButton;
        playPauseResourceButton = NULL;
    }
    if( transformResourceButton!=NULL)
    {
        delete transformResourceButton;
        transformResourceButton = NULL;
    }
    if( preloadCheckBox!=NULL)
    {
        delete preloadCheckBox;
        preloadCheckBox = NULL;
    }
    if( showAnimationCheckBox!=NULL)
    {
        delete showAnimationCheckBox;
        showAnimationCheckBox = NULL;
    }
    if( showCollisionShapeCheckBox!=NULL)
    {
        delete showCollisionShapeCheckBox;
        showCollisionShapeCheckBox = NULL;
    }
    if( animationCollisionShapeMenu!=NULL)
    {
        delete animationCollisionShapeMenu;
        animationCollisionShapeMenu = NULL;
    }
    if( animationCenterCollisionButton!=NULL)
    {
        delete animationCenterCollisionButton;
        animationCenterCollisionButton = NULL;
    }
    if( animationCollisionRectX!=NULL)
    {
        delete animationCollisionRectX;
        animationCollisionRectX = NULL;
    }
    if( animationCollisionRectY!=NULL)
    {
        delete animationCollisionRectY;
        animationCollisionRectY = NULL;
    }
    if( animationCollisionRectW!=NULL)
    {
        delete animationCollisionRectW;
        animationCollisionRectW = NULL;
    }
    if( animationCollisionRectH!=NULL)
    {
        delete animationCollisionRectH;
        animationCollisionRectH = NULL;
    }
    if( animationCollisionCircleX!=NULL)
    {
        delete animationCollisionCircleX;
        animationCollisionCircleX = NULL;
    }
    if( animationCollisionCircleY!=NULL)
    {
        delete animationCollisionCircleY;
        animationCollisionCircleY = NULL;
    }
    if( animationCollisionCircleR!=NULL)
    {
        delete animationCollisionCircleR;
        animationCollisionCircleR = NULL;
    }

    if( previousSubImageButton!=NULL)
    {
        delete previousSubImageButton;
        previousSubImageButton = NULL;
    }

    if( nextSubImageButton!=NULL)
    {
        delete nextSubImageButton;
        nextSubImageButton = NULL;
    }

    if( subImageNumberField!=NULL)
    {
        delete subImageNumberField;
        subImageNumberField = NULL;
    }
    if( subImageEditorPreviewId!=NULL)
    {
        delete subImageEditorPreviewId;
        subImageEditorPreviewId = NULL;
    }

    if( labelanimationDimensions!=NULL)
    {
        delete labelanimationDimensions;
        labelanimationDimensions = NULL;
    }

    if( labelFrameInfo!=NULL)
    {
        delete labelFrameInfo;
        labelFrameInfo = NULL;
    }
    if( labelanimationMessage!=NULL)
    {
        delete labelanimationMessage;
        labelanimationMessage = NULL;
    }

}

bool animationResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        bool isBuildReady = true;
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string nestedTabsStrObjFunc = generate_tabs( leftTabAmount +1 );
        std::string html5SpShName = get_name();
        if( animInEditor!=NULL)
        {
            for( int i = 0; i < ANIMATION_DATA_FIELD_COUNT; i++)
            {
                if( animationDataFields[i]!=NULL)
                {
                    if( animationDataFields[i]->get_held_number() < 0)
                    {
                        animationDataFields[i]->set_string("0");
                    }
                }
                else
                {
                    isBuildReady = false;
                    break;
                }
            }
            if( animInEditor->get_width() <=0 || animInEditor->get_height() <=0)
            {
                isBuildReady = false;
            }
            if( isBuildReady)
            {
                *fileTarget << nestedTabsStr << "var " << html5SpShName << " =  GPE.rsm.add_animation(";
                *fileTarget << int_to_string (exportBuildGlobalId ) +",";
                *fileTarget << int_to_string ( (int)animInEditor->get_frame_count() )  +",";
                *fileTarget << "'resources/animations/"+get_short_filename (animInEditor->get_file_name(),true )+"',";
                *fileTarget << int_to_string (animInEditor->get_frame_xoffset() ) +",";
                *fileTarget << int_to_string (animInEditor->get_frame_yoffset() ) +",";
                *fileTarget << int_to_string (animInEditor->get_width() ) +",";
                *fileTarget << int_to_string (animInEditor->get_height() ) +",";
                *fileTarget << int_to_string (animationCollisionShapeMenu->get_selected_id() ) +",";
                if( animationCollisionShapeMenu->get_selected_value()==1)
                {
                    *fileTarget << int_to_string (animationCollisionCircleX->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionCircleY->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionCircleR->get_held_number() );

                }
                else
                {

                    *fileTarget << int_to_string (animationCollisionRectX->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionRectY->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionRectW->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionRectH->get_held_number() );
                }
                *fileTarget << ");\n";
                return true;
            }
        }
        *fileTarget << nestedTabsStr << "var " << html5SpShName << " = GPE.rsm.add_animation_empty();\n";
        return true;
    }
    return false;
}

bool animationResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
   if( fileTarget!=NULL && fileTarget->is_open() )
   {
        bool isBuildReady = true;
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string nestedTabsStrObjFunc = generate_tabs( leftTabAmount +1 );
        std::string html5SpShName = get_name();
        if( animInEditor!=NULL)
        {
            for( int i = 0; i < ANIMATION_DATA_FIELD_COUNT; i++)
            {
                if( animationDataFields[i]!=NULL)
                {
                    if( animationDataFields[i]->get_held_number() < 0)
                    {
                        animationDataFields[i]->set_string("0");
                    }
                }
                else
                {
                    isBuildReady = false;
                    break;
                }
            }
            if( animInEditor->get_width() <=0 || animInEditor->get_height() <=0)
            {
                isBuildReady = false;
            }
            if( isBuildReady)
            {
                *fileTarget << nestedTabsStr << "var " << html5SpShName << " =  GPE.rsm.add_animation(";
                *fileTarget << int_to_string (exportBuildGlobalId ) +",";
                *fileTarget << int_to_string ( (int)animInEditor->get_frame_count() )  +",";
                *fileTarget << "'resources/animations/"+get_short_filename (animInEditor->get_file_name(),true )+"',";
                *fileTarget << int_to_string (animInEditor->get_frame_xoffset() ) +",";
                *fileTarget << int_to_string (animInEditor->get_frame_yoffset() ) +",";
                *fileTarget << int_to_string (animInEditor->get_width() ) +",";
                *fileTarget << int_to_string (animInEditor->get_height() ) +",";
                *fileTarget << int_to_string (animationCollisionShapeMenu->get_selected_id() ) +",";
                if( animationCollisionShapeMenu->get_selected_value()==1)
                {
                    *fileTarget << int_to_string (animationCollisionCircleX->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionCircleY->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionCircleR->get_held_number() );

                }
                else
                {

                    *fileTarget << int_to_string (animationCollisionRectX->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionRectY->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionRectW->get_held_number() ) +",";
                    *fileTarget << int_to_string (animationCollisionRectH->get_held_number() );
                }
                *fileTarget << ");\n";
                return true;
            }
        }
        *fileTarget << nestedTabsStr << "var " << html5SpShName << " = GPE.rsm.add_animation_empty();\n";
        return true;
    }
    return false;
}

void animationResource::compile_cpp()
{

}

bool animationResource::get_mouse_coords(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    /*areaMouseXPos = 0;
    areaMouseYPos = 0;*/
    if( viewedSpace!=NULL)
    {
        if( point_within(input->mouse_x,input->mouse_y,
                         animationPreviewCam->x,
                         animationPreviewCam->y,
                         animationPreviewCam->x+animationPreviewCam->w,
                         animationPreviewCam->y+animationPreviewCam->h ) )
        {
            /*
            areaMouseXPos = (input->mouse_x-animationPreviewCam->x )/zoomValue +animCameraRect.x - cam->x;
            areaMouseYPos = (input->mouse_y-animationPreviewCam->y )/zoomValue +animCameraRect.y - cam->y;
            */
            return true;
        }
    }
    return false;
}

int animationResource::get_preview_frame()
{
    autoAnimationPos = 0;
    if( subImageEditorPreviewId!=NULL && animInEditor!=NULL)
    {
        autoAnimationPos = subImageEditorPreviewId->get_held_number();
        if( (int)animInEditor->get_frame_count() <= autoAnimationPos )
        {
            autoAnimationPos = animInEditor->get_frame_count() - 1;
        }
        if( autoAnimationPos < 0)
        {
            autoAnimationPos = 0;
        }
        //subImageEditorPreviewId->set_string( int_to_string(previewFrameId) );
    }
    return autoAnimationPos;
}

GPE_Animation * animationResource::get_resource_animation()
{
    if( animInEditor!=NULL )
    {
        return animInEditor;
    }
    return NULL;
}

void animationResource::handle_scrolling()
{
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    if( areaIsScrollable )
    {
        if( input->check_keyboard_down(kb_ctrl) )
        {
            //Zoom Out
            if( input->mouseScrollingDown && editorZoomValue >= minZoomValue*2 )
            {
                editorZoomValue-=minZoomValue;
            }
            else if( input->mouseScrollingUp && editorZoomValue < maxZoomValue-minZoomValue)
            {
                //Zoom in
                editorZoomValue += minZoomValue;
            }
        }
        else if( input->shiftKeyIsPressed)
        {
            if( input->mouseScrollingUp > 0 )
            {
                xScrollHappened = true;
                animCameraRect.x-=(animCameraRect.w/16);
            }
            else if( input->mouseScrollingDown)
            {
                xScrollHappened = true;
                animCameraRect.x+=(animCameraRect.w/16);
            }
        }
        else
        {
            if( input->mouseScrollingUp )
            {
                yScrollHappened = true;
                animCameraRect.y-=(animCameraRect.h/4);
            }
            else if( input->mouseScrollingDown)
            {
                if( animCameraRect.y <animRect.h- animCameraRect.h/4 )
                {
                    yScrollHappened = true;
                    animCameraRect.y+=(animCameraRect.h/4);
                }
            }
        }
    }

    //Prevents camera from going out of bounds
    if( animCameraRect.x+animCameraRect.w > animRect.w )
    {
        xScrollHappened = true;
        animCameraRect.x = animRect.w-animCameraRect.w;
    }

    if( animCameraRect.y+animCameraRect.h > animRect.h )
    {
        yScrollHappened = true;
        animCameraRect.y = animRect.h-animCameraRect.h;
    }

    if( animCameraRect.x <= 0)
    {
        xScrollHappened = true;
        animCameraRect.x = 0;
    }

    if( animCameraRect.y <= 0)
    {
        yScrollHappened = true;
        animCameraRect.y = 0;
    }

    if( xScrollHappened)
    {
        animXScroll->contextRect.x = animCameraRect.x;
        animXScroll->process_self(NULL,NULL );
    }
    if( yScrollHappened)
    {
        animYScroll->contextRect.y = animCameraRect.y;
        animYScroll->process_self(NULL,NULL );
    }

    if( minZoomValue == 0)
    {
        minZoomValue = 0.0625;
    }

    if( editorZoomValue == 0 )
    {
        editorZoomValue  = minZoomValue;
    }
}

bool animationResource::include_local_files( std::string pBuildDir , int buildType )
{
    appendToFile(get_user_settings_folder()+"resources_check.txt",get_name() +"...");
    if( animInEditor!=NULL && animInEditor->has_texture() )
    {
        if( animInEditor->get_width() > 0 )
        {
            save_resource();
            return animInEditor->copy_image_source(pBuildDir+"/resources/animations");
        }
    }
    return true;
}

void animationResource::load_image(std::string newFileName, bool autoProcess)
{
    if( (int)newFileName.size() > 3 && newFileName!="NULL")
    {
        if( file_is_image(newFileName) && animInEditor!=NULL )
        {
            animInEditor->load_image( gpeph->get_default_render_package(), newFileName, true);
            if( animInEditor->get_texture_width() <1 || animInEditor->get_texture_width()>8192 || animInEditor->get_texture_height() <1 || animInEditor->get_texture_height()>8192 )
            {
                GPE_Report("[Animation Error:] Unable to load image: " +newFileName);
                appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","[Animation Error:] Unable to load image: " +newFileName);
            }
            else if( autoProcess )
            {
                for( int i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
                {
                    animationDataFields[i]->set_number(0);
                }
                animationDataFields[sprFrameWidth]->set_number( animInEditor->get_texture_width() );
                animationDataFields[sprFrameHeight]->set_number( animInEditor->get_texture_height() );

                int radMin = std::min ( animInEditor->get_texture_width(),animInEditor->get_texture_height() )/2;
                animationCollisionCircleX->set_number( animInEditor->get_texture_width()/2);
                animationCollisionCircleY->set_number( animInEditor->get_texture_height()/2 );
                animationCollisionCircleR->set_number(  radMin );

                animationCollisionRectX->set_number(0);
                animationCollisionRectY->set_number(0);
                animationCollisionRectW->set_number( animInEditor->get_texture_width() );
                animationCollisionRectH->set_number( animInEditor->get_texture_height() );
            }
        }
        else
        {
            display_user_alert("Unable to load image","File type["+get_file_ext(newFileName)+"] not supported when loading ["+newFileName+"].");
        }
    }
}

void animationResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed && file_exists(alternatePath))
    {
        return;
    }
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Loading Animation", resourceName );
    }

    std::string otherColContainerName = "";

    std::string newFileIn = "";
    std::string soughtDir = file_to_dir(parentProjectName)+"/gpe_project/resources/animations/";
    if( file_exists(alternatePath) )
    {
        newFileIn = alternatePath;
        soughtDir = get_path_from_file(newFileIn);
    }
    else
    {

        newFileIn = soughtDir + resourceName+".gpf";
        //Fallsback to previous folder for older versions
        if( file_exists(newFileIn) == false )
        {
            soughtDir = file_to_dir(parentProjectName)+"/gpe_project/resources/animations/";
            newFileIn = soughtDir + resourceName+".gpf";
        }
    }
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    //GPE_Report("Loading animation - "+newFileIn);
    //If the level file could be loaded
    float foundFileVersion = -1;
    if( gameResourceFileIn.fail() )
    {
        gameResourceFileIn.close();
        return;
    }
    //makes sure the file is open
    if ( !gameResourceFileIn.is_open())
    {
        gameResourceFileIn.close();
        return;
    }

    int equalPos = 0;
    std::string firstChar="";
    std::string section="";
    std::string keyString="";
    std::string valString="";
    std::string subValString="";
    std::string currLine="";
    std::string currLineToBeProcessed;
    std::string sprDataStr;

    int i = 0;
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
                    keyString = string_lower( keyString );
                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                    if( keyString=="resourcename")
                    {
                        renameBox->set_string(valString);
                    }
                    else if( keyString=="imagelocation")
                    {
                        if( valString!="NULL")
                        {
                            load_image( soughtDir+valString );
                        }
                    }
                    else if( keyString=="preload")
                    {
                        preloadCheckBox->set_clicked(string_to_bool(valString) );
                    }
                    else if( keyString=="editorpreviewFrame")
                    {
                        subImageEditorPreviewId->set_string( valString );
                    }
                    else if( keyString=="previewcollisionshape")
                    {
                        if( showCollisionShapeCheckBox!=NULL)
                        {
                            showCollisionShapeCheckBox->set_clicked(string_to_bool(valString) );
                        }
                    }
                    else if( keyString=="collisionshape" )
                    {
                        animationCollisionShapeMenu->set_option_value( string_to_int(valString) );
                    }
                    else if( keyString=="circlecollisionregion")
                    {
                        animationCollisionCircleX->set_number(split_first_int(valString,',') );
                        animationCollisionCircleY->set_number(split_first_int(valString,',') );
                        animationCollisionCircleR->set_number( string_to_int(valString,0) );
                    }
                    else if( keyString=="rectanglecollisionbox")
                    {
                        animationCollisionRectX->set_number(split_first_int(valString,',') );
                        animationCollisionRectY->set_number(split_first_int(valString,',') );
                        animationCollisionRectW->set_number(split_first_int(valString,',') );
                        animationCollisionRectH->set_number( string_to_int(valString,0) );
                    }
                    else if( keyString=="subimageinview")
                    {
                        subImagePreviewNumber = string_to_int(valString);
                        subImageNumberField->set_string( int_to_string(subImagePreviewNumber) );
                    }
                    else
                    {
                        if( gpemath::compare_floats(foundFileVersion, GPE_ANIM_FCOUNT_VERSION) || foundFileVersion > GPE_ANIM_FCOUNT_VERSION )
                        {
                            //GPE_Report("Proper animation system used ["+resourceName+"]");
                            for( i = 0; i <ANIMATION_DATA_FIELD_COUNT ; i++)
                            {
                                sprDataStr = "animationdata["+int_to_string(i)+"]";
                                if( keyString==sprDataStr)
                                {
                                    animationDataFields[i]->set_string( valString );
                                }
                                else
                                {
                                    sprDataStr = "spritedata["+int_to_string(i)+"]";
                                    if( keyString==sprDataStr)
                                    {
                                        animationDataFields[i]->set_string( valString );

                                    }
                                }
                            }
                        }
                        else
                        {
                            //GPE_Report("Alternate animation system used ["+resourceName+"]");
                            animationDataFields[0]->set_string( "-1");
                            for( i = 0; i <ANIMATION_DATA_FIELD_COUNT-1 ; i++)
                            {
                                sprDataStr = "animationdata["+int_to_string(i)+"]";
                                if( keyString==sprDataStr)
                                {
                                    animationDataFields[i+1]->set_string( valString);
                                }
                                else
                                {
                                    sprDataStr = "spritedata["+int_to_string(i)+"]";
                                    if( keyString==sprDataStr)
                                    {
                                        animationDataFields[i+1]->set_string( valString);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            GPE_Report("Invalid FoundFileVersion ="+float_to_string(foundFileVersion)+".");
        }
    }
    if( foundFileVersion > 0 )
    {
        resourcePostProcessed= true;
    }
    process_data_fields(foundFileVersion);
}

void animationResource::process_collision_box()
{
    if( animInEditor== NULL)
    {
        return;
    }
    if( animationCollisionShapeMenu->get_selected_name()=="Circle")
    {
        int colX = animationCollisionCircleX->get_held_number();
        int colY = animationCollisionCircleY->get_held_number();
        int colR = animationCollisionCircleR->get_held_number();
        animInEditor->edit_collision_circle(colX, colY, colR );
    }
    else
    {
        int colX = animationCollisionRectX->get_held_number();
        int colW = animationCollisionRectW->get_held_number();
        int colY = animationCollisionRectY->get_held_number();
        int colH = animationCollisionRectH->get_held_number();
        animInEditor->edit_collision_box(colX, colY, colW,  colH );
    }
}

void animationResource::process_data_fields(float versionToProcess)
{
    if( animInEditor!=NULL )
    {
        bool animationIsReadyForEditing = true;

        int i = 0;
        int j = 0;
        while( i < ANIMATION_DATA_FIELD_COUNT )
        {
            if( animationDataFields[i]!=NULL)
            {
                if( animationDataFields[i]->is_inuse()==true)
                {
                    animationIsReadyForEditing = false;
                }
            }
            else
            {
                animationIsReadyForEditing = false;
            }
            i++;
        }
        if( animationIsReadyForEditing)
        {
            //GPE_Report("Beginning to Process animation Data...");
            int fWidth = animationDataFields[sprFrameWidth]->make_valid_number(0);
            int fHeight = animationDataFields[sprFrameHeight]->make_valid_number(0);

            int fPixelOffsetH = animationDataFields[sprHPixelOffset]->make_valid_number(0);
            int fPixelOffsetV = animationDataFields[sprVPixelOffset]->make_valid_number(0);

            int hPadding = animationDataFields[sprHPixelPadding]->make_valid_number(0);

            int vPadding = animationDataFields[sprVPixelPadding]->make_valid_number(0);

            if( fWidth > 0 && fHeight > 0 && hPadding>=0 && vPadding >=0)
            {
                int startX = fPixelOffsetH;
                int startY = fPixelOffsetV;
                int maxanimationFrames = animationDataFields[sprFrameCount]->make_valid_number(-1);

                if( versionToProcess > 0 && ( gpemath::compare_floats(versionToProcess,GPE_ANIM_FCOUNT_VERSION) || versionToProcess > GPE_ANIM_FCOUNT_VERSION  ) && maxanimationFrames > 0)
                {
                    animInEditor->setup_animation( maxanimationFrames,fWidth, fHeight, fPixelOffsetH, fPixelOffsetV, hPadding, vPadding);
                }
                else
                {
                    animInEditor->setup_fullimg_animation(fWidth, fHeight, fPixelOffsetH, fPixelOffsetV, hPadding, vPadding);
                    animationDataFields[sprFrameCount]->set_number( animInEditor->get_frame_count() );
                }
            }
        }
        process_collision_box();
    }
}

void animationResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
    if( editResourceButton!=NULL)
    {
        editResourceButton->prerender_self();
    }
    if( transformResourceButton!=NULL)
    {
        transformResourceButton->prerender_self();
    }
    if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self();
    }
    if( showAnimationCheckBox!=NULL)
    {
        showAnimationCheckBox->prerender_self();
    }
    if( animationCenterCollisionButton!=NULL)
    {
        animationCenterCollisionButton->prerender_self();
    }
}

void animationResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    int i;
    if( cam==NULL && viewedSpace == NULL )
    {
        return;
    }
    if( openExternalEditorButton!=NULL )
        openExternalEditorButton->set_clicked( false );

    bool editorHasControl = false;

    animRect.x = 0;
    animRect.y = 0;
    animRect.w = 0;
    animRect.h = 0;

    if( animInEditor!=NULL && animInEditor->has_texture() )
    {
        animRect.w = animInEditor->get_texture_width();
        animRect.h = animInEditor->get_texture_height();
    }

    if( minZoomValue == 0)
    {
       minZoomValue = 0.0625;
    }
    if( editorZoomValue == 0)
    {
        editorZoomValue = minZoomValue;
    }
    animationPreviewCam->x = 0;
    animationPreviewCam->y = 0;
    animationPreviewCam->w = viewedSpace->w;
    animationPreviewCam->h = viewedSpace->h;

    animCameraRect.w = animationPreviewCam->w / editorZoomValue;
    animCameraRect.h = animationPreviewCam->h / editorZoomValue;

    if( animCameraRect.w > animRect.w)
    {
        animCameraRect.w = animRect.w;
    }

    if( animCameraRect.h > animRect.h)
    {
        animCameraRect.h = animRect.h;
    }

    //Horizontal scrolling
    animXScroll->set_coords( 0,viewedSpace->h - animXScroll->get_height() );
    animXScroll->set_width( viewedSpace->w);
    update_rectangle(&animXScroll->fullRect,0,0,(float)animRect.w, (float)animRect.h );
    update_rectangle(&animXScroll->contextRect,(float)animCameraRect.x,(float)animCameraRect.y, (float)animCameraRect.w, (float)animCameraRect.h );
    animXScroll->process_self(viewedSpace,cam );
    //if( animXScroll->has_moved() )
    {
        animCameraRect.x = (float)(animXScroll->contextRect.x);
    }

    //Vertical Scrolling
    animYScroll->set_coords( viewedSpace->w -animYScroll->get_width(),0);
    animYScroll->set_height( viewedSpace->h );
    update_rectangle(&animYScroll->fullRect,0,0,(float)animRect.w, (float)animRect.h );
    update_rectangle(&animYScroll->contextRect,(float)animCameraRect.x,(float)animCameraRect.y, (float)animCameraRect.w, (float)animCameraRect.h );
    //animYScroll->contextRect.h = sceneEditorView.h;
    animYScroll->process_self(viewedSpace,cam );
    //if( animYScroll->has_moved() )
    {
        animCameraRect.y = float(animYScroll->contextRect.y);
    }

    if( animInEditor!=NULL)
    {
        if( animInEditor->has_texture() )
        {
            labelanimationDimensions->set_name("Image Dimensions:"+int_to_string( animInEditor->get_texture_width() )+" x "+int_to_string( animInEditor->get_texture_height() )+"px");
        }
        else
        {
            labelanimationDimensions->set_name("animationsheet size unknown");
        }
    }
    else
    {
        labelanimationDimensions->set_name("animationsheet size unknown");
    }

    if( editorZoomValue == 0 )
    {
        minZoomValue = 0.0625;
    }
    if( editorZoomValue == 0 )
    {
        editorZoomValue  = minZoomValue;
    }
    if( editorZoomValue < minZoomValue)
    {
        editorZoomValue = minZoomValue;
    }
    if( editorZoomValue > maxZoomValue)
    {
        editorZoomValue = maxZoomValue;
    }


    if( PANEL_GENERAL_EDITOR==NULL && PANEL_INSPECTOR == NULL)
    {
        return;
    }

    gpeEditorDockPanel * collisionAndPreviewDataPanel = PANEL_GENERAL_EDITOR;
    if( PANEL_INSPECTOR != NULL)
    {
        PANEL_INSPECTOR->clear_panel();
        collisionAndPreviewDataPanel = PANEL_INSPECTOR;
    }

    if( PANEL_GENERAL_EDITOR!=NULL )
    {
        editorHasControl = PANEL_GENERAL_EDITOR->hasScrollControl || collisionAndPreviewDataPanel->hasScrollControl;
        PANEL_GENERAL_EDITOR->add_gui_element( renameBox,true);
        PANEL_GENERAL_EDITOR->add_gui_element( loadResourceButton,false );
        PANEL_GENERAL_EDITOR->add_gui_element( refreshResourceDataButton,false );
        PANEL_GENERAL_EDITOR->add_gui_element( editResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element( transformResourceButton,false);
        PANEL_GENERAL_EDITOR->add_gui_element( openExternalEditorButton,true);
        PANEL_GENERAL_EDITOR->add_gui_element( previewSubImageNumbers,true);

        PANEL_GENERAL_EDITOR->add_gui_element(labelInfoMaxTextureSize,true);
        PANEL_GENERAL_EDITOR->add_gui_element( labelanimationDimensions,true);
        for(  i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
        {
            if( animationDataFields[i]!=NULL)
            {
                PANEL_GENERAL_EDITOR->add_gui_element(animationDataFields[i],true );
                        //animationDataFields[i]->set_width(PANEL_GENERAL_EDITOR->get_width()-GENERAL_GPE_GUI_PADDING*3);
            }
        }
    }
    else
    {
        editorHasControl = collisionAndPreviewDataPanel->hasScrollControl;
    }
    //Collision and animation preview stuff
    collisionAndPreviewDataPanel->add_gui_element( previousSubImageButton,false);
    collisionAndPreviewDataPanel->add_gui_element( subImageNumberField,false);
    collisionAndPreviewDataPanel->add_gui_element( nextSubImageButton,false);
    collisionAndPreviewDataPanel->add_gui_element(playPauseResourceButton,true);
    collisionAndPreviewDataPanel->add_gui_element( subImageEditorPreviewId,true);

    labelFrameInfo->set_name("Animation Size: "+int_to_string(animInEditor->get_width() )+" x "+int_to_string(animInEditor->get_height() )+"px");
    collisionAndPreviewDataPanel->add_gui_element( labelFrameInfo,true);
    collisionAndPreviewDataPanel->add_gui_element( animationCollisionShapeMenu,true);
    collisionAndPreviewDataPanel->add_gui_element( animationCenterCollisionButton,true);

    //Rectangle collision box
    if( animationCollisionShapeMenu->get_selected_value()==0 )
    {
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionRectX,false);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionRectY,true);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionRectW,false);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionRectH,true);
    }
    else
    {
        //Circle collision box
        animationCollisionCircleX->set_width( (collisionAndPreviewDataPanel->get_width()-GENERAL_GPE_GUI_PADDING*3 )/3 );
        animationCollisionCircleY->set_width( animationCollisionCircleX->get_width( ) );
        animationCollisionCircleR->set_width(animationCollisionCircleX->get_width( ) );

        collisionAndPreviewDataPanel->add_gui_element( animationCollisionCircleX,false);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionCircleY,false);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionCircleR,true);
        collisionAndPreviewDataPanel->add_gui_element( showCollisionShapeCheckBox,true);
    }

    if( PANEL_INSPECTOR != NULL)
    {
        PANEL_INSPECTOR->process_self( NULL, NULL );
    }

    if( PANEL_GENERAL_EDITOR!=NULL )
    {
        PANEL_GENERAL_EDITOR->add_gui_element(preloadCheckBox,true);
        PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);

        PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
        //PANEL_GENERAL_EDITOR->set_maxed_out_width();
        PANEL_GENERAL_EDITOR->process_self( NULL, NULL );

        if( loadResourceButton!=NULL && loadResourceButton->is_clicked() )
        {
            std::string newFileName = GPE_GetOpenFileName("Load Your animation Image","Image",MAIN_GUI_SETTINGS->fileOpenanimationDir);
            if( (int)newFileName.size() > 3 && file_exists(newFileName) )
            {
                bool autoProcessData = false;
                if( animationDataFields[0]!=NULL)
                {
                    if( animationDataFields[0]->get_held_number()==0)
                    {
                        autoProcessData = true;
                    }
                }
                load_image(newFileName, autoProcessData);
                input->reset_all_input();
                return;
            }
        }
        else if( confirmResourceButton->is_clicked() )
        {
            input->reset_all_input();
            for( int i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
            {
                if( animationDataFields[i]!=NULL)
                {
                    animationDataFields[i]->switch_inuse(false);
                }
            }

            int radMin = std::min (animationDataFields[sprFrameWidth]->get_held_number(), animationDataFields[sprFrameHeight]->get_held_number() )/2;
            animationCollisionCircleX->set_number( animationDataFields[sprFrameWidth]->get_held_number()/2);
            animationCollisionCircleY->set_number( animationDataFields[sprFrameHeight]->get_held_number()/2 );
            animationCollisionCircleR->set_number(  radMin );

            animationCollisionRectX->set_number(0);
            animationCollisionRectY->set_number(0);
            animationCollisionRectW->set_number( animationDataFields[sprFrameWidth]->get_held_number() );
            animationCollisionRectH->set_number( animationDataFields[sprFrameHeight]->get_held_number() );
            requestedMode = 0;
            input->reset_all_input();
            save_resource();
            isPreloaded = preloadCheckBox->is_clicked();
        }
        else if( cancelResourceButton->is_clicked() )
        {
            if( GPE_Display_Basic_Prompt("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== DISPLAY_QUERY_YES )
            {
                resourcePostProcessed = false;
                input->reset_all_input();
                requestedMode = 0;
                GPE_Report("Reverting to Editor Mode 0");
                revert_data_fields();
                process_data_fields(GPE_VERSION_FLOAT_NUMBER);
                preprocess_self();
            }
        }

        if( editResourceButton!=NULL && editResourceButton->is_clicked() )
        {
            editResourceButton->set_clicked( false );
            GPE_Report("Going to Editor Mode 1");
            input->reset_all_input();
            process_self(viewedSpace,cam);
            return;
        }

        if( transformResourceButton!=NULL &&  transformResourceButton->is_clicked() && animInEditor!=NULL)
        {
            if( animInEditor->get_width() > 0 && animInEditor->get_height() > 0)
            {
                GPE_open_context_menu(-1,-1,256 );
                MAIN_CONTEXT_MENU->add_menu_option("Erase BG Color",0);
                MAIN_CONTEXT_MENU->add_menu_option("Invert Colors",1);
                MAIN_CONTEXT_MENU->add_menu_option("Make GrayScale",2);
                MAIN_CONTEXT_MENU->add_menu_option("Exit",10);
                int menuSelection = GPE_Get_Context_Result();
                if( menuSelection>=0 && menuSelection <=3)
                {
                    std::string tempStr = animInEditor->get_file_name();
                    SDL_Surface * oTempSurface = SDL_SurfaceEx::load_surface_image(  tempStr.c_str() );
                    SDL_Surface *nTempSurface = NULL;
                    if( oTempSurface!=NULL)
                    {
                        if( menuSelection==0)
                        {
                            GPE_Color * foundBGColor = c_fuchsia->duplicate_color();
                            if( GPE_Change_Color_PopUp("Image Background Color To Remove",foundBGColor) )
                            {
                                if( GPE_Display_Basic_Prompt("Are you sure you want to erase this Color from this image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                {
                                    GPE_Report("Modifying image at: "+animInEditor->get_file_name() +".");
                                    nTempSurface = SDL_SurfaceEx::surface_remove_color(oTempSurface,foundBGColor->get_sdl_color() );
                                }
                            }
                            delete foundBGColor;
                            foundBGColor = NULL;

                        }
                        else if( menuSelection==1 )
                        {
                            if( GPE_Display_Basic_Prompt("Are you sure you want to invert your image's colors?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                            {
                                nTempSurface= SDL_SurfaceEx::surface_invert(oTempSurface);
                            }
                        }
                        else if( menuSelection==2 )
                        {
                            if( GPE_Display_Basic_Prompt("Are you sure you want to grayscale your image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                            {
                                nTempSurface= SDL_SurfaceEx::surface_grayscale(oTempSurface);
                            }
                        }
                        if( nTempSurface!=NULL)
                        {
                            delete_file( animInEditor->get_file_name() );
                            std::string newImageName = get_file_noext(animInEditor->get_file_name() )+".png";
                            IMG_SavePNG(nTempSurface,newImageName.c_str() );
                            SDL_FreeSurface(nTempSurface);
                            nTempSurface = NULL;
                            animInEditor->load_image(  gpeph->get_default_render_package(), newImageName, true );
                        }
                        SDL_FreeSurface(oTempSurface);
                        oTempSurface = NULL;
                    }
                }
            }
        }

        if( openExternalEditorButton!=NULL && animInEditor!=NULL)
        {
            if( openExternalEditorButton->is_clicked() )
            {
                if( file_exists(animInEditor->get_file_name() ) )
                {
                    if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=NULL)
                    {
                        GPE_OpenProgram(MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),animInEditor->get_file_name(), true );
                    }
                    else
                    {
                        GPE_OpenURL(animInEditor->get_file_name() );
                    }
                    appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+animInEditor->get_file_name()+"]...");
                }
            }
        }

        if( refreshResourceDataButton!=NULL  && refreshResourceDataButton->is_clicked() )
        {
            resourcePostProcessed = false;
            preprocess_self();
        }

        //actual saving of the animation onto the engine and files
        if( playPauseResourceButton!=NULL &&  playPauseResourceButton->is_clicked() )
        {
            if( animInEditor!=NULL)
            {
                if( playPauseResourceButton->get_name()=="Play")
                {
                    playPauseResourceButton->set_name("Stop");
                    playPauseResourceButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/stop.png" );
                }
                else
                {
                    playPauseResourceButton->set_name("Play");
                    playPauseResourceButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png" );
                }
            }
        }
    }

    if( animationCollisionShapeMenu!=NULL)
    {
        if( animationCollisionShapeMenu->get_selected_value()==0 )
        {
            if( animationCenterCollisionButton->is_clicked() && animInEditor!=NULL)
            {
                animationCollisionRectX->set_string( int_to_string( 0 ) );
                animationCollisionRectY->set_string( int_to_string( 0 ) );
                animationCollisionRectW->set_string( int_to_string( animInEditor->get_width() ) );
                animationCollisionRectH->set_string( int_to_string( animInEditor->get_height() ) );
            }
        }
        else if( animationCollisionShapeMenu->get_selected_value()==1 )
        {
            if( animationCenterCollisionButton->is_clicked() && animInEditor!=NULL)
            {
                int smallestCenter = std::min( animInEditor->get_width(), animInEditor->get_height() )/2;
                animationCollisionCircleX->set_string( int_to_string( animInEditor->get_width()/2 ) );
                animationCollisionCircleY->set_string( int_to_string( animInEditor->get_height()/2 ) );
                animationCollisionCircleR->set_string( int_to_string(smallestCenter ) );
            }
        }

        //Animates animation
        if( playPauseResourceButton->get_name()=="Stop" )
        {
            animationSpeed = FPS_CAP/4.f;
            //animationSpeed = std::max( 1, animationSpeed );
            subImageMiniAnimationNumber += ( animationSpeed * gpe->get_delta_performance() )/ 100.f ;
            //GPE_Report("Animating animation by "+float_to_string(animationSpeed)+"..."+int_to_string(random(0,100) )+"...");

            if( subImageMiniAnimationNumber >= animationSpeed )
            {
                subImagePreviewNumber++;
                subImageNumberField->set_number( subImagePreviewNumber);
                subImageMiniAnimationNumber = 0;
            }
        }

        if( previousSubImageButton!=NULL && previousSubImageButton->is_clicked() && subImagePreviewNumber > 0)
        {
            subImagePreviewNumber--;
            subImageNumberField->set_number( subImagePreviewNumber);
            playPauseResourceButton->set_name("Play");
            playPauseResourceButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png" );
        }

        if( subImageNumberField!=NULL && subImageNumberField->is_inuse() )
        {
            subImagePreviewNumber = subImageNumberField->get_held_number();
            playPauseResourceButton->set_name("Play");
            playPauseResourceButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png" );
        }

        if( nextSubImageButton!=NULL &&  nextSubImageButton->is_clicked() )
        {
            subImagePreviewNumber++;
            subImageNumberField->set_number( subImagePreviewNumber);
            playPauseResourceButton->set_name("Play");
            playPauseResourceButton->change_texture( guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png") );
        }

        if( subImagePreviewNumber >= (int)animInEditor->get_frame_count() )
        {
            subImagePreviewNumber = 0;
            subImageNumberField->set_number( subImagePreviewNumber);
        }

        if(subImagePreviewNumber < 0 )
        {
            subImagePreviewNumber = 0;
            subImageNumberField->set_number( subImagePreviewNumber);
        }

        if( PANEL_GENERAL_EDITOR!=NULL)
        {
            editorHasControl = PANEL_GENERAL_EDITOR->hasScrollControl || collisionAndPreviewDataPanel->hasScrollControl;
        }
        else
        {
            editorHasControl = collisionAndPreviewDataPanel->hasScrollControl;
        }
        //actual saving of the animation onto the engine and files
        if( get_mouse_coords(viewedSpace, cam) )
        {
            areaIsScrollable = true;
        }
        else
        {
            areaIsScrollable = false;
        }
        handle_scrolling();

        if( areaIsScrollable )
        {
            if( input->check_mouse_released( mb_right) )
            {
                /*
                GPE_open_context_menu( input->mouse_x, input->mouse_y );
                    MAIN_CONTEXT_MENU->add_menu_option("Change Box Color",0);
                    MAIN_CONTEXT_MENU->add_menu_option("Change Number Color",1);
                    MAIN_CONTEXT_MENU->add_menu_option("Return to Mode 0",2);
                    int menuSelection = GPE_Get_Context_Result();

                    if( menuSelection == 2)
                    {
                        input->reset_all_input();
                        requestedMode = 0;
                        GPE_Report("Reverting to Editor Mode 0");
                        input->reset_all_input();
                        process_self(viewedSpace,cam);
                        return;
                    }
                    */
            }
        }

        if( !editorHasControl )
        {
            if( input->check_keyboard_down(kb_ctrl) )
            {
                if( input->check_keyboard_released(kb_minus) || input->mouseScrollingDown )
                {
                    editorZoomValue -= minZoomValue;
                }
                else if( input->check_keyboard_released(kb_plus) || input->mouseScrollingUp )
                {
                    editorZoomValue += minZoomValue;
                }
            }
        }
    }
}

void animationResource::render_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    bool animationPreviewIsRendered = false;
    if( cam==NULL || viewedSpace==NULL )
    {
        return;
    }

    if( GPE_TEXTURE_TRANSPARENT_BG!=NULL )
    {
        for(int iPV = 0; iPV< viewedSpace->w; iPV+=GPE_TEXTURE_TRANSPARENT_BG->get_width() )
        {
            for(int jPV= 0; jPV< viewedSpace->h; jPV+=GPE_TEXTURE_TRANSPARENT_BG->get_height() )
            {
                GPE_TEXTURE_TRANSPARENT_BG->render_tex( iPV,jPV,NULL);
            }
        }
    }
    else
    {
        gcanvas->render_rect( viewedSpace,GPE_MAIN_THEME->Program_Color, false, 255 );
    }

    if( animInEditor!=NULL &&animInEditor->has_texture() )
    {
        if( subImagePreviewNumber >=0 && subImagePreviewNumber < (int)animInEditor->get_frame_count() )
        {
            animInEditor->render_scaled( subImagePreviewNumber, animationPreviewCam->x+animationPreviewCam->w/2,
            animationPreviewCam->y+animationPreviewCam->h/2, editorZoomValue,editorZoomValue,NULL );
        }
        else
        {
            animInEditor->render_scaled( 0,animationPreviewCam->x+animationPreviewCam->w/2,
            animationPreviewCam->y+animationPreviewCam->h/2, editorZoomValue,editorZoomValue,NULL );
        }

        if( showCollisionShapeCheckBox->is_clicked() && animInEditor!=NULL )
        {
            if( animationCollisionShapeMenu->get_selected_value()==0 )
            {
                int sprCX = animationPreviewCam->x+animationPreviewCam->w/2;
                sprCX+=animationCollisionRectX->get_held_number() * editorZoomValue;
                int sprCY = animationPreviewCam->y+animationPreviewCam->h/2;
                sprCY+=animationCollisionRectY->get_held_number() * editorZoomValue;
                int sprCW = animationCollisionRectW->get_held_number() * editorZoomValue;
                int sprCH = animationCollisionRectH->get_held_number() * editorZoomValue;
                sprCX-= sprCW/2;
                sprCY-= sprCH/2;

                if( sprCX >= 0 && sprCY >=0 && sprCW > 0 && sprCH > 0)
                {
                    gcanvas->render_rectangle( sprCX,sprCY,sprCX+sprCW,sprCY+sprCH,c_green,false,96);
                }
            }
            else if( animationCollisionShapeMenu->get_selected_value()==1 )
            {
                int sprCX = animationPreviewCam->x+( animationPreviewCam->w )/2;// - (animInEditor->get_width()*editor0ZoomValue)/2;
                sprCX+=animationCollisionCircleX->get_held_number() * editorZoomValue;

                int sprCY = animationPreviewCam->y+( animationPreviewCam->h )/2;// - (animInEditor->get_height()*editor0ZoomValue)/2;
                sprCY+=animationCollisionCircleY->get_held_number() * editorZoomValue;

                int sprCR = animationCollisionCircleR->get_held_number() * editorZoomValue;
                sprCX-=sprCR;
                sprCY-=sprCR;
                if( sprCX >= 0 && sprCY >=0 && sprCR > 0)
                {
                    gcanvas->render_circle_color( sprCX,sprCY,sprCR,c_lime,192);
                }
            }
        }

        process_data_fields(GPE_VERSION_FLOAT_NUMBER);

        //renders the animation in edit
        //renders the frame boxes for the animation
        animInEditor->render_piece_resized( animationPreviewCam->x, animationPreviewCam->y, animCameraRect.w* editorZoomValue, animCameraRect.h* editorZoomValue, &animCameraRect );


        int iAnimFrame = 0, jAnimRow = 0;
        int rectanglesRendered = 0;
        int maxAnimationFrames = animationDataFields[sprFrameCount]->get_held_number();
        animatonPreviewRect->x = animationDataFields[sprHPixelOffset]->get_held_number();
        animatonPreviewRect->y = animationDataFields[sprHPixelOffset]->get_held_number();
        animatonPreviewRect->w = animationDataFields[sprFrameWidth]->get_held_number();
        animatonPreviewRect->h = animationDataFields[sprFrameHeight]->get_held_number();

        int incrementWidth = animatonPreviewRect->w  + animationDataFields[sprHPixelPadding]->get_held_number();
        int incrementHeight = animatonPreviewRect->h + animationDataFields[sprVPixelPadding]->get_held_number();
        int iTSX = 0;
        int bx1 = 0;
        int bx2 = 0;
        int by1 = 0;
        int by2 = 0;

        //Make sure we aren't gonna be stuck in a forever loop
        if( incrementWidth > 0 && incrementHeight > 0)
        {
            for( iAnimFrame = 0; iAnimFrame < maxAnimationFrames; iAnimFrame++)
            {
                bx1 = (animationPreviewCam->x+animatonPreviewRect->x - animCameraRect.x ) *editorZoomValue;
                //bx1 = bound_number(bx1,0, elementBox.w);

                by1 = (animationPreviewCam->y+animatonPreviewRect->y - animCameraRect.y)*editorZoomValue;
                //by1 = bound_number(by1,0, elementBox.h);

                bx2 = ( animationPreviewCam->x + animatonPreviewRect->x + animatonPreviewRect->w - animCameraRect.x ) * editorZoomValue;
                //bx2 = bound_number(bx2,0, elementBox.w);

                by2 = ( animationPreviewCam->y + animatonPreviewRect->y + animatonPreviewRect->h - animCameraRect.y ) * editorZoomValue;
                //by2 = bound_number(by2,0, elementBox.h);
                gcanvas->render_rectangle( bx1-cam->x, by1-cam->y, bx2-cam->x, by2-cam->y, c_black,true,255);
                rectanglesRendered++;

                if( previewSubImageNumbers->is_clicked() && editorZoomValue > 0.40 )
                {
                    gfs->render_text( bx1-cam->x+GENERAL_GPE_GUI_PADDING, by2-4-cam->y, int_to_string(iAnimFrame), GPE_MAIN_THEME->Main_Box_Font_Highlight_Color, GPE_DEFAULT_FONT, FA_LEFT, FA_BOTTOM );
                }

                animationPreviewCam->x+= incrementWidth;
                if( animationPreviewCam->x >= animInEditor->get_texture_width() )
                {
                    animatonPreviewRect->y += incrementHeight;
                }
            }
        }
    }

    if( previewZoomLevel!=NULL && previewZoomLevel->is_clicked() )
    {
        gfs->render_text_boxed( viewedSpace->w-32,viewedSpace->h-32,
                          "Zoom Level: "+float_to_string(editorZoomValue*100 )+"%",
                          GPE_MAIN_THEME->Text_Box_Font_Color,c_black, GPE_DEFAULT_FONT,FA_RIGHT,FA_BOTTOM);
    }

    if( animXScroll!=NULL)
    {
        animXScroll->render_self( viewedSpace,cam);
    }
    if( animYScroll!=NULL)
    {
        animYScroll->render_self( viewedSpace,cam);
    }
}


void animationResource::revert_data_fields()
{
    for( int i = 0; i < ANIMATION_DATA_FIELD_COUNT; i++ )
    {
        if( animationDataFields[i]!=NULL)
        {
            animationDataFields[i]->switch_inuse( false );
        }
        else
        {
            GPE_Report("animation Detected to have NULL field at ["+int_to_string(i)+"]");
            return;
        }
    }
    if( animInEditor!=NULL )
    {
        animationDataFields[sprFrameCount]->set_number( animInEditor->get_frame_count() );
        animationDataFields[sprFrameWidth]->set_number( animInEditor->get_width() );
        animationDataFields[sprFrameHeight]->set_number( animInEditor->get_height() );
        animationDataFields[sprHPixelOffset]->set_number( animInEditor->get_frame_xoffset() );
        animationDataFields[sprVPixelOffset]->set_number( animInEditor->get_frame_yoffset() );
        animationDataFields[sprHPixelPadding]->set_number( animInEditor->get_hpadding() );
        animationDataFields[sprVPixelPadding]->set_number( animInEditor->get_vpadding() );
    }
}

void animationResource::save_resource(std::string alternatePath, int backupId)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Processing Animation", resourceName );
    }
    std::string newSaveDataFilename = "";
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
        soughtDir = file_to_dir(parentProjectName)+"/gpe_project/resources/animations/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);

            if( animInEditor!=NULL)
            {
                for( int i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
                {
                    newSaveDataFile << "animationData["<< int_to_string(i)<< "]=";

                    if( animationDataFields[i]!=NULL)
                    {
                        newSaveDataFile << int_to_string( animationDataFields[i]->get_held_number() )<<"\n";
                    }
                    else
                    {
                        newSaveDataFile << "0\n";
                    }
                }
                newSaveDataFile << "SubimageInView=" << int_to_string( subImagePreviewNumber) << "\n";
                if( preloadCheckBox!=NULL)
                {
                    newSaveDataFile << "Preload="+int_to_string(preloadCheckBox->is_clicked() )+"\n";
                }
                if( subImageEditorPreviewId!=NULL)
                {
                    newSaveDataFile << "EditorPreviewFrame="+int_to_string(subImageEditorPreviewId->get_held_number() )+"\n";
                }
                if( showCollisionShapeCheckBox!=NULL)
                {
                    newSaveDataFile << "PreviewCollisionShape="+int_to_string(showCollisionShapeCheckBox->is_clicked() )+"\n";
                }
                newSaveDataFile << "CollisionShape="<< int_to_string (animationCollisionShapeMenu->get_selected_id() ) +"\n";

                newSaveDataFile << "CircleCollisionRegion=" << int_to_string (animationCollisionCircleX->get_held_number() ) +",";
                newSaveDataFile << int_to_string (animationCollisionCircleY->get_held_number() ) +",";
                newSaveDataFile << int_to_string (animationCollisionCircleR->get_held_number() )+"\n";
                //Rectangle info

                newSaveDataFile << "RectangleCollisionBox=" << int_to_string (animationCollisionRectX->get_held_number() ) +",";
                newSaveDataFile << int_to_string (animationCollisionRectY->get_held_number() ) +",";
                newSaveDataFile << int_to_string (animationCollisionRectW->get_held_number() ) +",";
                newSaveDataFile << int_to_string (animationCollisionRectH->get_held_number() )+"\n";
                std::string resFileCopySrc = animInEditor->get_file_name();
                std::string resFileLocation = get_short_filename (resFileCopySrc,true );
                newSaveDataFile << "ImageLocation="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 )
                {
                    std::string resFileCopyDest = soughtDir+resFileLocation;
                    if( file_exists(resFileCopyDest) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]animation Image File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] animation Image File? This action is irreversible!")==DISPLAY_QUERY_YES)
                        {
                            copy_file(resFileCopySrc,resFileCopyDest);
                        }
                        */
                    }
                    else
                    {
                        copy_file(resFileCopySrc,resFileCopyDest);
                    }
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
            GPE_Main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void animationResource::update_box(int newX, int newY, int newW, int newH)
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

bool animationResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "animation=" << resourceName << "," << get_global_rid() << ",";
            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}

