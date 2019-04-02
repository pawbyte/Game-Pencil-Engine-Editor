/*
animation_resource.cpp
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

#include "animation_resource.h"
#include "gpe_editor_settings.h"


std::string spriteDataLabels[SPRITE_DATA_FIELD_COUNT];
int spriteDataLabelWidth;

animationResource::animationResource(GPE_GeneralResourceContainer * pFolder)
{
    autoAnimationPos = 0;
    animationModeToolBar = new GPE_TabBar();
    animationModeToolBar->add_new_tab("Animation & Collision",true );
    animationModeToolBar->add_new_tab("Image Sheet" );
    animationModeToolBar->open_tab( 0 );
    animationTrackBar = new GPE_Slider_XAxis();
    animCameraRect.x = animRect.x = 0;
    animCameraRect.y = animRect.y = 0;
    animCameraRect.w = animRect.w = 0;
    animCameraRect.h = animRect.h = 0;
    areaIsScrollable = false;
    spritePreviewCam = new GPE_Rect();
    animationSpeed = 1;
    editor0ZoomValue = 1;
    editor1ZoomValue = 1;
    minZoomValue = 0.0625;
    maxZoomValue = 16;

    projectParentFolder = pFolder;
    requestedMode = editorMode = 0;
    animInEditor = new GPE_Animation();
    topPanel = new GPE_GuiElementList();
    animationSpeedSlider = new GPE_Slider_XAxis(100,0,800);
    animationSpeedLabel = new GPE_Label_Text("Preview Speed","Preview Speed");
    labelSpriteDimensions = new GPE_Label_Text("Sprite Dimensions");
    previewZoomLevel = new GPE_CheckBoxBasic("Preview Zoom Level","Preview Zoom Level", true);
    previewSubImageNumbers = new GPE_CheckBoxBasic("Sub-images","Sub-images", true);

    labelFrameInfo = new GPE_Label_Text("Frame Info");
    labelSpriteMessage = new GPE_Label_Text("Sprite Message");
    //Sprite Collision Area
    preloadCheckBox = new GPE_CheckBoxBasic("Preload Sprite","Check to load sprite at game open", true);
    showCollisionShapeCheckBox = new GPE_CheckBoxBasic("Preview Collision Shape","", true);
    showAnimationCheckBox = new GPE_CheckBoxBasic("Preview Sprite Animation","", true);
    spriteCollisionShapeMenu = new GPE_DropDown_Menu( "Collision Shape",true);
    spriteCollisionShapeMenu->add_menu_option("Rectangle","rectangle",0,true);
    spriteCollisionShapeMenu->add_menu_option("Circle","circle",0,false);
    //spriteCollisionShapeMenu->add_menu_option("Triangle [ Coming Soon]");
    //spriteCollisionShapeMenu->add_menu_option("Diamond [ Coming Soon]");
    //spriteCollisionShapeMenu->add_menu_option("Hexagon [ Coming Soon]");

    spriteCenterCollisionButton = new GPE_ToolLabelButton( "Center Collision Shape","Centers the collision box");
    spriteCollisionRectX = new GPE_TextInputNumber("0",true,0);
    spriteCollisionRectX->set_label("X-Pos");
    spriteCollisionRectX->set_string("0");
    spriteCollisionRectX->scale_width(0.5);

    spriteCollisionRectY = new GPE_TextInputNumber("0",true,0);
    spriteCollisionRectY->set_label("Y-Pos");
    spriteCollisionRectY->set_string("0");
    spriteCollisionRectY->scale_width(0.5);

    spriteCollisionRectW = new GPE_TextInputNumber("0",true,0);
    spriteCollisionRectW->set_label("Width");
    spriteCollisionRectW->set_string("0");
    spriteCollisionRectW->scale_width(0.5);

    spriteCollisionRectH = new GPE_TextInputNumber("0",true,0);
    spriteCollisionRectH->set_label("Height");
    spriteCollisionRectH->set_string("0");
    spriteCollisionRectH->scale_width(0.5);

    spriteCollisionCircleX = new GPE_TextInputNumber("0",true,0);
    spriteCollisionCircleX->set_label("X-Pos");
    spriteCollisionCircleX->set_string("0");

    spriteCollisionCircleY = new GPE_TextInputNumber("0",true,0);
    spriteCollisionCircleY->set_label("Y-Pos");
    spriteCollisionCircleY->set_string("0");

    spriteCollisionCircleR = new GPE_TextInputNumber("0",true,0);
    spriteCollisionCircleR->set_label("Radius");
    spriteCollisionCircleR->set_string("0");
    int i = 0;

    spriteDataFields[i] = new GPE_TextInputNumber("");
    //GENERAL_GPE_PADDING*2+spriteDataLabelWidth,GENERAL_GPE_PADDING+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING*(i+1),64,GPE_AVERAGE_LINE_HEIGHT,"0",false,INT_MIN,INT_MAX);
    spriteDataFields[i]->set_string("0");
    spriteDataFields[i]->set_label(spriteDataLabels[i]);

    for( i =1; i < SPRITE_DATA_FIELD_COUNT; i++)
    {
        spriteDataFields[i] = new GPE_TextInputNumber("");
        //GENERAL_GPE_PADDING*2+spriteDataLabelWidth,GENERAL_GPE_PADDING+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING*(i+1),64,GPE_AVERAGE_LINE_HEIGHT,"0",true,0,512);
        spriteDataFields[i]->set_string("0");
        spriteDataFields[i]->set_label(spriteDataLabels[i]);
    }
    editResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/edit.png","Edit Sprite Data",-1);
    transformResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/magic.png","Transform the Image",-1);
    playPauseResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png","Stop",-1);
    subImagePreviewNumber = 0;
    subImageMiniAnimationNumber = 0;
    resourcePostProcessed = false;

    //Subimage
    previousSubImageButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrow-left.png","Previous");
    //previousSubImageButton->set_sprite(mainMenuSprite, 36 );
    nextSubImageButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrow-right.png","Next");
    //nextSubImageButton->set_sprite(mainMenuSprite, 38 );
    subImageNumberField = new GPE_TextInputNumber("");
    subImageNumberField->set_string("0");
    subImageNumberField->set_width(64);
    subImageEditorPreviewId = new GPE_TextInputNumber("");
    subImageEditorPreviewId->set_string("0");
    subImageEditorPreviewId->set_label("Editor Preview Frame");
    openExternalEditorButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Sprite Image In External Editor");
    refreshResourceDataButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded sprite image");
    labelInfoMaxTextureSize = new GPE_Label_Text("Max Image Size: 8192 X 8192px","Max Image Size: 8192 X 8192px");

    animXScroll = new GPE_ScrollBar_XAxis();
    animYScroll = new GPE_ScrollBar_YAxis();
}

animationResource::~animationResource()
{
    if( animationModeToolBar!=NULL )
    {
        delete animationModeToolBar;
        animationModeToolBar = NULL;
    }
    if( animationTrackBar!=NULL )
    {
        delete animationTrackBar;
        animationTrackBar = NULL;
    }
    if( topPanel!=NULL )
    {
        topPanel->clear_list();
        delete topPanel;
        topPanel = NULL;
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
    if( spritePreviewCam!=NULL )
    {
        delete spritePreviewCam;
        spritePreviewCam = NULL;
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
    for( int i =SPRITE_DATA_FIELD_COUNT-1; i >=0; i--)
    {
        tempDataField = spriteDataFields[i];
        if( tempDataField!=NULL )
        {
            delete tempDataField;
        }
        spriteDataFields[i] = NULL;
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
    if( spriteCollisionShapeMenu!=NULL)
    {
        delete spriteCollisionShapeMenu;
        spriteCollisionShapeMenu = NULL;
    }
    if( spriteCenterCollisionButton!=NULL)
    {
        delete spriteCenterCollisionButton;
        spriteCenterCollisionButton = NULL;
    }
    if( spriteCollisionRectX!=NULL)
    {
        delete spriteCollisionRectX;
        spriteCollisionRectX = NULL;
    }
    if( spriteCollisionRectY!=NULL)
    {
        delete spriteCollisionRectY;
        spriteCollisionRectY = NULL;
    }
    if( spriteCollisionRectW!=NULL)
    {
        delete spriteCollisionRectW;
        spriteCollisionRectW = NULL;
    }
    if( spriteCollisionRectH!=NULL)
    {
        delete spriteCollisionRectH;
        spriteCollisionRectH = NULL;
    }
    if( spriteCollisionCircleX!=NULL)
    {
        delete spriteCollisionCircleX;
        spriteCollisionCircleX = NULL;
    }
    if( spriteCollisionCircleY!=NULL)
    {
        delete spriteCollisionCircleY;
        spriteCollisionCircleY = NULL;
    }
    if( spriteCollisionCircleR!=NULL)
    {
        delete spriteCollisionCircleR;
        spriteCollisionCircleR = NULL;
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

    if( labelSpriteDimensions!=NULL)
    {
        delete labelSpriteDimensions;
        labelSpriteDimensions = NULL;
    }

    if( labelFrameInfo!=NULL)
    {
        delete labelFrameInfo;
        labelFrameInfo = NULL;
    }
    if( labelSpriteMessage!=NULL)
    {
        delete labelSpriteMessage;
        labelSpriteMessage = NULL;
    }
    if( animationSpeedSlider!=NULL)
    {
        delete animationSpeedSlider;
        animationSpeedSlider = NULL;
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
            for( int i = 0; i < SPRITE_DATA_FIELD_COUNT; i++)
            {
                if( spriteDataFields[i]!=NULL)
                {
                    if( spriteDataFields[i]->get_held_number() < 0)
                    {
                        spriteDataFields[i]->set_string("0");
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
                *fileTarget << nestedTabsStr << "var " << html5SpShName << " =  GPE.rsm.add_sprite(";
                *fileTarget << int_to_string (exportBuildGlobalId ) +",";
                *fileTarget << int_to_string ( (int)animInEditor->get_frame_count() )  +",";
                *fileTarget << "'resources/animations/"+getShortFileName (animInEditor->get_file_name(),true )+"',";
                *fileTarget << int_to_string (animInEditor->get_frame_xoffset() ) +",";
                *fileTarget << int_to_string (animInEditor->get_frame_yoffset() ) +",";
                *fileTarget << int_to_string (animInEditor->get_width() ) +",";
                *fileTarget << int_to_string (animInEditor->get_height() ) +",";
                *fileTarget << int_to_string (spriteCollisionShapeMenu->get_selected_id() ) +",";
                if( spriteCollisionShapeMenu->get_selected_id()==1)
                {
                    *fileTarget << int_to_string (spriteCollisionCircleX->get_held_number() ) +",";
                    *fileTarget << int_to_string (spriteCollisionCircleY->get_held_number() ) +",";
                    *fileTarget << int_to_string (spriteCollisionCircleR->get_held_number() );

                }
                else
                {

                    *fileTarget << int_to_string (spriteCollisionRectX->get_held_number() ) +",";
                    *fileTarget << int_to_string (spriteCollisionRectY->get_held_number() ) +",";
                    *fileTarget << int_to_string (spriteCollisionRectW->get_held_number() ) +",";
                    *fileTarget << int_to_string (spriteCollisionRectH->get_held_number() );
                }
                *fileTarget << ");\n";
                return true;
            }
        }
        *fileTarget << nestedTabsStr << "var " << html5SpShName << " = GPE.rsm.add_sprite_empty();\n";
        return true;
    }
    return false;
}

bool animationResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
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
                         spritePreviewCam->x,
                         spritePreviewCam->y,
                         spritePreviewCam->x+spritePreviewCam->w,
                         spritePreviewCam->y+spritePreviewCam->h ) )
        {
            /*
            areaMouseXPos = (input->mouse_x-spritePreviewCam->x )/zoomValue +animCameraRect.x - cam->x;
            areaMouseYPos = (input->mouse_y-spritePreviewCam->y )/zoomValue +animCameraRect.y - cam->y;
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

void animationResource::handle_scrolling()
{
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    if( areaIsScrollable )
    {
        if( input->check_keyboard_down(kb_ctrl) )
        {
            //Zoom Out
            if( input->mouseScrollingDown && editor1ZoomValue >= minZoomValue*2 )
            {
                editor1ZoomValue-=minZoomValue;
            }
            else if( input->mouseScrollingUp && editor1ZoomValue < maxZoomValue-minZoomValue)
            {
                //Zoom in
                editor1ZoomValue += minZoomValue;
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
            else if( topPanel->hasArrowkeyControl==false )
            {
                //arrow scrolling
                if( input->check_keyboard_down(kb_up) )
                {
                    yScrollHappened = true;
                    animCameraRect.y-=(animCameraRect.h/4);
                }
                else if( input->check_keyboard_down(kb_down) )
                {
                    yScrollHappened = true;
                    animCameraRect.y+=(animCameraRect.h/4);
                }
                if( input->check_keyboard_down(kb_left) )
                {
                    if( animCameraRect.x > (animCameraRect.w/4) )
                    {
                        xScrollHappened = true;
                        animCameraRect.x-=(animCameraRect.w/4);
                    }
                    else
                    {
                        animCameraRect.x = 0;
                        xScrollHappened = true;
                    }
                }
                else if( input->check_keyboard_down(kb_right) )
                {
                    if( (animCameraRect.x +animCameraRect.w/4) < animRect.w )
                    {
                        xScrollHappened = true;
                        animCameraRect.x+=(animCameraRect.w/4);
                    }
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
}

void animationResource::load_image(std::string newFileName, bool autoProcess)
{
    if( (int)newFileName.size() > 3 && newFileName!="NULL")
    {
        if( file_is_image(newFileName) && animInEditor!=NULL )
        {
            animInEditor->load_image( newFileName, true);
            if( animInEditor->get_texture_width() <1 || animInEditor->get_texture_width()>8192 || animInEditor->get_texture_height() <1 || animInEditor->get_texture_height()>8192 )
            {
                GPE_Report("[Animation Error:] Unable to load image: " +newFileName);
                appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","[Animation Error:] Unable to load image: " +newFileName);
            }
            else if( autoProcess )
            {
                for( int i =0; i < SPRITE_DATA_FIELD_COUNT; i++)
                {
                    spriteDataFields[i]->set_number(0);
                }
                spriteDataFields[sprFrameWidth]->set_number( animInEditor->get_texture_width() );
                spriteDataFields[sprFrameHeight]->set_number( animInEditor->get_texture_height() );

                int radMin = std::min ( animInEditor->get_texture_width(),animInEditor->get_texture_height() )/2;
                spriteCollisionCircleX->set_number( animInEditor->get_texture_width()/2);
                spriteCollisionCircleY->set_number( animInEditor->get_texture_height()/2 );
                spriteCollisionCircleR->set_number(  radMin );

                spriteCollisionRectX->set_number(0);
                spriteCollisionRectY->set_number(0);
                spriteCollisionRectW->set_number( animInEditor->get_texture_width() );
                spriteCollisionRectH->set_number( animInEditor->get_texture_height() );
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
    if( resourcePostProcessed ==false || file_exists(alternatePath))
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Loading Animation", resourceName );
        }

        std::string otherColContainerName = "";

        std::string newFileIn = "";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/animations/";
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
                soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/sprites/";
                newFileIn = soughtDir + resourceName+".gpf";
            }
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        //GPE_Report("Loading sprite - "+newFileIn);
        //If the level file could be loaded
        double foundFileVersion = -1;
        if( !gameResourceFileIn.fail() )
        {
            //GPE_Report("Procesing sprite file...");
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
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
                                        foundFileVersion = string_to_double(valString);
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
                                }
                                else if( keyString=="Preload")
                                {
                                    preloadCheckBox->set_clicked(string_to_bool(valString) );
                                }
                                else if( keyString=="EditorPreviewFrame")
                                {
                                    subImageEditorPreviewId->set_string( valString );
                                }
                                else if( keyString=="PreviewCollisionShape")
                                {
                                    if( showCollisionShapeCheckBox!=NULL)
                                    {
                                        showCollisionShapeCheckBox->set_clicked(string_to_bool(valString) );
                                    }
                                }
                                else if( keyString=="CollisionShape" )
                                {
                                    spriteCollisionShapeMenu->set_selection( string_to_int(valString) );
                                }
                                else if( keyString=="CircleCollisionRegion")
                                {
                                    spriteCollisionCircleX->set_number(split_first_int(valString,',') );
                                    spriteCollisionCircleY->set_number(split_first_int(valString,',') );
                                    spriteCollisionCircleR->set_number( string_to_int(valString,0) );
                                }
                                else if( keyString=="RectangleCollisionBox")
                                {
                                    spriteCollisionRectX->set_number(split_first_int(valString,',') );
                                    spriteCollisionRectY->set_number(split_first_int(valString,',') );
                                    spriteCollisionRectW->set_number(split_first_int(valString,',') );
                                    spriteCollisionRectH->set_number( string_to_int(valString,0) );
                                }
                                else if( keyString=="SubimageInView")
                                {
                                    subImagePreviewNumber = string_to_int(valString);
                                    subImageNumberField->set_string( int_to_string(subImagePreviewNumber) );
                                }
                                else
                                {
                                    if( compare_doubles(foundFileVersion, GPE_ANIM_FCOUNT_VERSION) || foundFileVersion > GPE_ANIM_FCOUNT_VERSION )
                                    {
                                        //GPE_Report("Proper sprite system used ["+resourceName+"]");
                                        for( i = 0; i <SPRITE_DATA_FIELD_COUNT ; i++)
                                        {
                                            sprDataStr = "SpriteData["+int_to_string(i)+"]";
                                            if( keyString==sprDataStr)
                                            {
                                                spriteDataFields[i]->set_string( valString );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        //GPE_Report("Alternate sprite system used ["+resourceName+"]");
                                        spriteDataFields[0]->set_string( "-1");
                                        for( i = 0; i <SPRITE_DATA_FIELD_COUNT-1 ; i++)
                                        {
                                            sprDataStr = "SpriteData["+int_to_string(i)+"]";
                                            if( keyString==sprDataStr)
                                            {
                                                spriteDataFields[i+1]->set_string( valString);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        GPE_Report("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }
                if( foundFileVersion > 0 )
                {
                    resourcePostProcessed= true;
                }
            }
        }
        process_data_fields(foundFileVersion);
    }
}

void animationResource::process_collision_box()
{
    if( animInEditor== NULL)
    {
        return;
    }
    if( spriteCollisionShapeMenu->get_selected_name()=="Circle")
    {
        int colX = spriteCollisionCircleX->get_held_number();
        int colY = spriteCollisionCircleY->get_held_number();
        int colR = spriteCollisionCircleR->get_held_number();
        animInEditor->edit_collision_circle(colX, colY, colR );
    }
    else
    {
        int colX = spriteCollisionRectX->get_held_number();
        int colW = spriteCollisionRectW->get_held_number();
        int colY = spriteCollisionRectY->get_held_number();
        int colH = spriteCollisionRectH->get_held_number();
        animInEditor->edit_collision_box(colX, colY, colW,  colH );
    }
}

void animationResource::process_data_fields(double versionToProcess)
{
    if( animInEditor!=NULL )
    {
        bool spriteIsReadyForEditing = true;

        int i = 0;
        int j = 0;
        while( i < SPRITE_DATA_FIELD_COUNT )
        {
            if( spriteDataFields[i]!=NULL)
            {
                if( spriteDataFields[i]->is_inuse()==true)
                {
                    spriteIsReadyForEditing = false;
                }
            }
            else
            {
                spriteIsReadyForEditing = false;
            }
            i++;
        }
        if( spriteIsReadyForEditing)
        {
            //GPE_Report("Beginning to Process Sprite Data...");
            int fWidth = spriteDataFields[sprFrameWidth]->make_valid_number(0);
            int fHeight = spriteDataFields[sprFrameHeight]->make_valid_number(0);

            int fPixelOffsetH = spriteDataFields[sprHPixelOffset]->make_valid_number(0);
            int fPixelOffsetV = spriteDataFields[sprVPixelOffset]->make_valid_number(0);

            int hPadding = spriteDataFields[sprHPixelPadding]->make_valid_number(0);

            int vPadding = spriteDataFields[sprVPixelPadding]->make_valid_number(0);

            if( fWidth > 0 && fHeight > 0 && hPadding>=0 && vPadding >=0)
            {
                int startX = fPixelOffsetH;
                int startY = fPixelOffsetV;
                int maxSpriteFrames = spriteDataFields[sprFrameCount]->make_valid_number(-1);

                if( versionToProcess > 0 && ( compare_doubles(versionToProcess,GPE_ANIM_FCOUNT_VERSION) || versionToProcess > GPE_ANIM_FCOUNT_VERSION  ) && maxSpriteFrames > 0)
                {
                    animInEditor->setup_animation( maxSpriteFrames,fWidth, fHeight, fPixelOffsetH, fPixelOffsetV, hPadding, vPadding);
                }
                else
                {
                    animInEditor->setup_fullimg_animation(fWidth, fHeight, fPixelOffsetH, fPixelOffsetV, hPadding, vPadding);
                    spriteDataFields[sprFrameCount]->set_number( animInEditor->get_frame_count() );
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
    if( spriteCenterCollisionButton!=NULL)
    {
        spriteCenterCollisionButton->prerender_self();
    }
}

void animationResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    int i;
    if( cam!=NULL && viewedSpace!=NULL )
    {
        if( openExternalEditorButton!=NULL )
            openExternalEditorButton->set_clicked( false );

        bool editorHasControl = false;

        //top pane

        if( editorMode != requestedMode )
        {
            editorMode = requestedMode;
            animationModeToolBar->open_tab( editorMode );
        }

        topPanel->set_coords(0,0 );
        topPanel->set_width( viewedSpace->w );
        topPanel->set_height( 64 );
        topPanel->barXPadding = 0;
        topPanel->barXMargin = GENERAL_GPE_PADDING;

        topPanel->barYPadding = GENERAL_GPE_PADDING;
        topPanel->barYMargin = 0;

        topPanel->clear_list();

        spritePreviewCam->x = 0;
        spritePreviewCam->y = topPanel->get_y2pos();
        spritePreviewCam->w = viewedSpace->w;
        spritePreviewCam->h = viewedSpace->h - spritePreviewCam->y;

        animRect.x = 0;
        animRect.y = 0;
        animRect.w = 0;
        animRect.h = 0;
        if( animInEditor!=NULL && animInEditor->has_texture() )
        {
            animRect.w = animInEditor->get_texture_width();
            animRect.h = animInEditor->get_texture_height();
        }
        animCameraRect.w = spritePreviewCam->w;
        animCameraRect.h = spritePreviewCam->h;


        if( editorMode == 0)
        {
            //topPanel->add_gui_element(animationSpeedLabel,false);
            topPanel->add_gui_element(animationSpeedSlider,false);
            topPanel->add_gui_element(previewZoomLevel,true);
            topPanel->add_gui_element(animationModeToolBar,true);
            //topPanel->add_gui_element(animationSpeedLabel,false);
            animCameraRect.w = spritePreviewCam->w / editor0ZoomValue;
            animCameraRect.h = spritePreviewCam->h / editor0ZoomValue;
        }
        else if( editorMode ==1 )
        {
            topPanel->add_gui_element(previewZoomLevel,false);
            topPanel->add_gui_element(previewSubImageNumbers,true);
            topPanel->add_gui_element(animationModeToolBar,true);
            //Handles scrolling
            animCameraRect.w = spritePreviewCam->w / editor1ZoomValue;
            animCameraRect.h = spritePreviewCam->h / editor1ZoomValue;
        }
        topPanel->process_self(viewedSpace,cam);
        if( editorMode!= animationModeToolBar->get_selected_tab() )
        {
            //Swaps to new mode.
            requestedMode = animationModeToolBar->get_selected_tab();
        }

        if( animCameraRect.w > animRect.w)
        {
            animCameraRect.w = animRect.w;
        }

        if( animCameraRect.h > animRect.h)
        {
            animCameraRect.h = animRect.h;
        }
        if( editorMode ==1 )
        {
            //Horizontal scrolling
            animXScroll->set_coords( spritePreviewCam->x,spritePreviewCam->y+spritePreviewCam->h - animXScroll->get_height() );
            animXScroll->set_width( spritePreviewCam->w);
            update_rectangle(&animXScroll->fullRect,0,0,(double)animRect.w, (double)animRect.h );
            update_rectangle(&animXScroll->contextRect,(double)animCameraRect.x,(double)animCameraRect.y, (double)animCameraRect.w, (double)animCameraRect.h );
            animXScroll->process_self(viewedSpace,cam );
            //if( animXScroll->has_moved() )
            {
                animCameraRect.x = (double)(animXScroll->contextRect.x);
            }

            //Vertical Scrolling
            animYScroll->set_coords( spritePreviewCam->x+spritePreviewCam->w -animXScroll->get_height(),spritePreviewCam->y );
            animYScroll->set_height( spritePreviewCam->h );
            update_rectangle(&animYScroll->fullRect,0,0,(double)animRect.w, (double)animRect.h );
            update_rectangle(&animYScroll->contextRect,(double)animCameraRect.x,(double)animCameraRect.y, (double)animCameraRect.w, (double)animCameraRect.h );
            //animYScroll->contextRect.h = sceneEditorView.h;
            animYScroll->process_self(viewedSpace,cam );
            //if( animYScroll->has_moved() )
            {
                animCameraRect.y = double(animYScroll->contextRect.y);
            }
        }

        if( animInEditor!=NULL)
        {
            if( animInEditor->has_texture() )
            {
                labelSpriteDimensions->set_name("Image Dimensions:"+int_to_string( animInEditor->get_texture_width() )+" x "+int_to_string( animInEditor->get_texture_height() )+"px");
            }
            else
            {
                labelSpriteDimensions->set_name("Spritesheet size unknown");
            }
        }
        else
        {
            labelSpriteDimensions->set_name("Spritesheet size unknown");
        }

        if( editorMode==0)
        {
            if( PANEL_GENERAL_EDITOR!=NULL )
            {
                editorHasControl = PANEL_GENERAL_EDITOR->hasScrollControl;
                PANEL_GENERAL_EDITOR->clear_panel();

                PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);
                PANEL_GENERAL_EDITOR->add_gui_element(loadResourceButton,false );
                PANEL_GENERAL_EDITOR->add_gui_element(refreshResourceDataButton,false );
                PANEL_GENERAL_EDITOR->add_gui_element(editResourceButton,false);
                PANEL_GENERAL_EDITOR->add_gui_element(transformResourceButton,false);
                PANEL_GENERAL_EDITOR->add_gui_element( openExternalEditorButton,true);

                //PANEL_GENERAL_EDITOR->add_gui_element(labelInfoMaxTextureSize,true);


                PANEL_GENERAL_EDITOR->add_gui_element( previousSubImageButton,false);
                PANEL_GENERAL_EDITOR->add_gui_element( subImageNumberField,false);
                PANEL_GENERAL_EDITOR->add_gui_element( nextSubImageButton,false);
                PANEL_GENERAL_EDITOR->add_gui_element(playPauseResourceButton,true);
                PANEL_GENERAL_EDITOR->add_gui_element( subImageEditorPreviewId,true);

                PANEL_GENERAL_EDITOR->add_gui_element( labelSpriteDimensions,true);

                labelFrameInfo->set_name("Animation Size: "+int_to_string(animInEditor->get_width() )+" x "+int_to_string(animInEditor->get_height() )+"px");
                PANEL_GENERAL_EDITOR->add_gui_element( labelFrameInfo,true);
                //Collision stuffs
                PANEL_GENERAL_EDITOR->add_gui_element( spriteCollisionShapeMenu,true);

                PANEL_GENERAL_EDITOR->add_gui_element( spriteCenterCollisionButton,true);

                if( spriteCollisionShapeMenu->get_selected_id()==0 )
                {
                    PANEL_GENERAL_EDITOR->add_gui_element( spriteCollisionRectX,false);
                    PANEL_GENERAL_EDITOR->add_gui_element( spriteCollisionRectY,true);
                    PANEL_GENERAL_EDITOR->add_gui_element( spriteCollisionRectW,false);
                    PANEL_GENERAL_EDITOR->add_gui_element( spriteCollisionRectH,true);
                }
                else if( spriteCollisionShapeMenu->get_selected_id()==1 )
                {
                    //Resize to allow 3 in 1 one.
                    spriteCollisionCircleX->set_width( (PANEL_GENERAL_EDITOR->get_width()-GENERAL_GPE_PADDING*3 )/3 );
                    spriteCollisionCircleY->set_width( spriteCollisionCircleX->get_width( ) );
                    spriteCollisionCircleR->set_width(spriteCollisionCircleX->get_width( ) );



                    PANEL_GENERAL_EDITOR->add_gui_element( spriteCollisionCircleX,false);
                    PANEL_GENERAL_EDITOR->add_gui_element( spriteCollisionCircleY,false);
                    PANEL_GENERAL_EDITOR->add_gui_element( spriteCollisionCircleR,true);
                    PANEL_GENERAL_EDITOR->add_gui_element(preloadCheckBox,true);
                }

                PANEL_GENERAL_EDITOR->add_gui_element( showCollisionShapeCheckBox,true);

                PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);

                PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
                //PANEL_GENERAL_EDITOR->set_maxed_out_width();
                PANEL_GENERAL_EDITOR->process_self( NULL, NULL );

                if( loadResourceButton!=NULL && loadResourceButton->is_clicked() )
                {
                    std::string newFileName = GPE_GetOpenFileName("Load Your Sprite Image","Image",MAIN_GUI_SETTINGS->fileOpenSpriteDir);
                    if( (int)newFileName.size() > 3 && file_exists(newFileName) )
                    {
                        bool autoProcessData = false;
                        if( spriteDataFields[0]!=NULL)
                        {
                            if( spriteDataFields[0]->get_held_number()==0)
                            {
                                autoProcessData = true;
                            }
                        }
                        load_image(newFileName, autoProcessData);
                        requestedMode = 1;
                        input->reset_all_input();
                        return;
                    }
                }

                else if( confirmResourceButton->is_clicked() )
                {
                    save_resource();
                }
                else if( cancelResourceButton->is_clicked() )
                {
                    if( GPE_Display_Basic_Prompt("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== DISPLAY_QUERY_YES )
                    {
                        resourcePostProcessed = false;
                        preprocess_self();
                    }
                }

                if( editResourceButton!=NULL && editResourceButton->is_clicked() )
                {
                    editResourceButton->set_clicked( false );
                    requestedMode = 1;
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
                            SDL_Surface * oTempSurface = gpe_sdl->load_surface_image(animInEditor->get_file_name() );
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
                                            nTempSurface = gpe_sdl->surface_remove_color(oTempSurface,foundBGColor->get_sdl_color() );
                                        }
                                    }
                                    delete foundBGColor;
                                    foundBGColor = NULL;

                                }
                                else if( menuSelection==1 )
                                {
                                    if( GPE_Display_Basic_Prompt("Are you sure you want to invert your image's colors?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                    {
                                        nTempSurface= gpe_sdl->surface_invert(oTempSurface);
                                    }
                                }
                                else if( menuSelection==2 )
                                {
                                    if( GPE_Display_Basic_Prompt("Are you sure you want to grayscale your image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                    {
                                        nTempSurface= gpe_sdl->surface_grayscale(oTempSurface);
                                    }
                                }
                                if( nTempSurface!=NULL)
                                {
                                    delete_file( animInEditor->get_file_name() );
                                    std::string newImageName = get_file_noext(animInEditor->get_file_name() )+".png";
                                    IMG_SavePNG(nTempSurface,newImageName.c_str() );
                                    SDL_FreeSurface(nTempSurface);
                                    nTempSurface = NULL;
                                    animInEditor->load_image(  newImageName, true );
                                }
                                SDL_FreeSurface(oTempSurface);
                                oTempSurface = NULL;
                            }
                        }
                    }

                }

                if( refreshResourceDataButton!=NULL  && refreshResourceDataButton->is_clicked() )
                {
                    resourcePostProcessed = false;
                    preprocess_self();
                }


                //actual saving of the sprite onto the engine and files
                if( confirmResourceButton!=NULL && confirmResourceButton->is_clicked() )
                {
                    isPreloaded = preloadCheckBox->is_clicked();
                }

                if( animInEditor!=NULL)
                {
                    if( playPauseResourceButton!=NULL &&  playPauseResourceButton->is_clicked() )
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

                if( spriteCollisionShapeMenu!=NULL)
                {
                    if( spriteCollisionShapeMenu->get_selected_id()==0 )
                    {
                        if( spriteCenterCollisionButton->is_clicked() && animInEditor!=NULL)
                        {
                            spriteCollisionRectX->set_string( int_to_string( 0 ) );
                            spriteCollisionRectY->set_string( int_to_string( 0 ) );
                            spriteCollisionRectW->set_string( int_to_string( animInEditor->get_width() ) );
                            spriteCollisionRectH->set_string( int_to_string( animInEditor->get_height() ) );
                        }
                    }
                    else if( spriteCollisionShapeMenu->get_selected_id()==1 )
                    {
                        if( spriteCenterCollisionButton->is_clicked() && animInEditor!=NULL)
                        {
                            int smallestCenter = std::min( animInEditor->get_width(), animInEditor->get_height() )/2;
                            spriteCollisionCircleX->set_string( int_to_string( animInEditor->get_width()/2 ) );
                            spriteCollisionCircleY->set_string( int_to_string( animInEditor->get_height()/2 ) );
                            spriteCollisionCircleR->set_string( int_to_string(smallestCenter ) );
                        }
                    }
                }
            }

            //Animates Sprite
            if( playPauseResourceButton->get_name()=="Stop" )
            {
                animationSpeed = abs(  ( (double)animationSpeedSlider->get_value() )/ 100.d );
                //animationSpeed = std::max( 1, animationSpeed );
                subImageMiniAnimationNumber+=animationSpeed;
                //GPE_Report("Animating sprite by "+double_to_string(animationSpeed)+"..."+int_to_string(random(0,100) )+"...");

                if( FPS_RATIO > 4 )
                {
                    if( subImageMiniAnimationNumber > 2*FPS_RATIO/4 )
                    {
                        subImagePreviewNumber++;
                        subImageNumberField->set_number( subImagePreviewNumber);
                        subImageMiniAnimationNumber = 0;
                    }
                }
                else if( subImageMiniAnimationNumber > 10*FPS_RATIO )
                {
                    subImagePreviewNumber++;
                    subImageNumberField->set_number( subImagePreviewNumber);
                    subImageMiniAnimationNumber= 0;
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
        }
        else if( editorMode==1)
        {
            if( PANEL_GENERAL_EDITOR !=NULL )
            {
                editorHasControl = PANEL_GENERAL_EDITOR->hasScrollControl;
                PANEL_GENERAL_EDITOR->clear_panel();

                PANEL_GENERAL_EDITOR->add_gui_element(labelSpriteDimensions,true);
                for(  i =0; i < SPRITE_DATA_FIELD_COUNT; i++)
                {
                    if( spriteDataFields[i]!=NULL)
                    {
                        PANEL_GENERAL_EDITOR->add_gui_element(spriteDataFields[i],true );
                        //spriteDataFields[i]->set_width(PANEL_GENERAL_EDITOR->get_width()-GENERAL_GPE_PADDING*3);
                    }
                }
                PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true );

                PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true );
                ///
                //PANEL_GENERAL_EDITOR->set_maxed_out_width();
                PANEL_GENERAL_EDITOR->process_self(NULL, NULL);
            }

            if( animInEditor!=NULL)
            {
                //process_data_fields(GPE_VERSION_DOUBLE_NUMBER);
            }

            //actual saving of the sprite onto the engine and files
            if( confirmResourceButton!=NULL && confirmResourceButton->is_clicked() )
            {
                input->reset_all_input();
                for( int i =0; i < SPRITE_DATA_FIELD_COUNT; i++)
                {
                    if( spriteDataFields[i]!=NULL)
                    {
                        spriteDataFields[i]->switch_inuse(false);
                    }
                }

                int radMin = std::min (spriteDataFields[sprFrameWidth]->get_held_number(), spriteDataFields[sprFrameHeight]->get_held_number() )/2;
                spriteCollisionCircleX->set_number( spriteDataFields[sprFrameWidth]->get_held_number()/2);
                spriteCollisionCircleY->set_number( spriteDataFields[sprFrameHeight]->get_held_number()/2 );
                spriteCollisionCircleR->set_number(  radMin );

                spriteCollisionRectX->set_number(0);
                spriteCollisionRectY->set_number(0);
                spriteCollisionRectW->set_number( spriteDataFields[sprFrameWidth]->get_held_number() );
                spriteCollisionRectH->set_number( spriteDataFields[sprFrameHeight]->get_held_number() );
                requestedMode = 0;
                input->reset_all_input();
            }
            else if( cancelResourceButton!=NULL &&  cancelResourceButton->is_clicked() )
            {
                input->reset_all_input();
                requestedMode = 0;
                GPE_Report("Reverting to Editor Mode 0");
                revert_data_fields();
                process_data_fields(GPE_VERSION_DOUBLE_NUMBER);
            }

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
        }

        if( openExternalEditorButton!=NULL && animInEditor!=NULL)
        {
            if( openExternalEditorButton->is_clicked() && editorMode == 0 )
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

        if( !editorHasControl && editorMode==0)
        {
            if( input->check_keyboard_down(kb_ctrl) )
            {
                if( input->check_keyboard_released(kb_minus) || input->mouseScrollingDown )
                {
                    editor0ZoomValue -= minZoomValue;
                }
                else if( input->check_keyboard_released(kb_plus) || input->mouseScrollingUp )
                {
                    editor0ZoomValue += minZoomValue;
                }
            }
        }
        else if( !editorHasControl  && editorMode==1 )
        {
            if( input->check_keyboard_down(kb_ctrl) )
            {
                if( input->check_keyboard_released(kb_minus) || input->mouseScrollingDown )
                {
                    editor1ZoomValue -= minZoomValue;
                }
                else if( input->check_keyboard_released(kb_plus) || input->mouseScrollingUp )
                {
                    editor1ZoomValue += minZoomValue;
                }
            }
        }

        if( editor0ZoomValue < minZoomValue)
        {
            editor0ZoomValue = minZoomValue;
        }
        if( editor0ZoomValue > maxZoomValue)
        {
            editor0ZoomValue = maxZoomValue;
        }

        if( editor1ZoomValue < minZoomValue)
        {
            editor1ZoomValue = minZoomValue;
        }
        if( editor1ZoomValue > maxZoomValue)
        {
            editor1ZoomValue = maxZoomValue;
        }
    }
}

void animationResource::render_self(GPE_Rect * viewedSpace,GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    bool spritePreviewIsRendered = false;
    if( cam!=NULL && viewedSpace!=NULL )
    {
        if( editorMode==0)
        {
            //renders the right side of the area, mainly preview of sprite
        }

        if( editorMode==0  && GPE_TEXTURE_TRANSPARENT_BG!=NULL && GPE_MAIN_THEME->themeBgTexture==NULL )
        {
            for(int iPV= spritePreviewCam->x; iPV< spritePreviewCam->x+spritePreviewCam->w; iPV+=GPE_TEXTURE_TRANSPARENT_BG->get_width() )
            {
                for(int jPV= spritePreviewCam->y; jPV< spritePreviewCam->y+spritePreviewCam->h; jPV+=GPE_TEXTURE_TRANSPARENT_BG->get_height() )
                {
                    GPE_TEXTURE_TRANSPARENT_BG->render_tex( iPV,jPV,NULL);
                }
            }
        }
        else
        {
            gcanvas->render_rectangle( spritePreviewCam->x, spritePreviewCam->y, spritePreviewCam->x + spritePreviewCam->w , spritePreviewCam->y+spritePreviewCam->h, GPE_MAIN_THEME->Program_Color, false, 255 );
        }

        if( editorMode==0)
        {
            if( animInEditor!=NULL)
            {
                if ( animInEditor->has_texture() )
                {
                    if( subImagePreviewNumber >=0 && subImagePreviewNumber < (int)animInEditor->get_frame_count() )
                    {
                        animInEditor->render_scaled( subImagePreviewNumber,
                                                      spritePreviewCam->x+spritePreviewCam->w/2,
                                                      spritePreviewCam->y+spritePreviewCam->h/2,
                                                     editor0ZoomValue,editor0ZoomValue,NULL );

                        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_i) )
                        {
                            GPE_Report("Rendering ["+int_to_string(subImagePreviewNumber)+"] frame...");
                        }
                    }
                    else
                    {
                        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_i) )
                        {
                            GPE_Report("Rendering default frame...");
                        }
                        animInEditor->render_scaled( 0,
                                                      spritePreviewCam->x+spritePreviewCam->w/2,
                                                      spritePreviewCam->y+spritePreviewCam->h/2,
                                                      editor0ZoomValue,editor0ZoomValue,NULL );
                    }
                }

                if( showCollisionShapeCheckBox->is_clicked() && animInEditor!=NULL )
                {
                    if( spriteCollisionShapeMenu->get_selected_id()==0 )
                    {
                        int sprCX = spritePreviewCam->x+spritePreviewCam->w/2;
                        sprCX-=spriteCollisionRectX->get_held_number() * editor0ZoomValue;
                        int sprCY = spritePreviewCam->y+spritePreviewCam->h/2;
                        sprCY-=spriteCollisionRectY->get_held_number() * editor0ZoomValue;
                        int sprCW = spriteCollisionRectW->get_held_number() * editor0ZoomValue;
                        int sprCH = spriteCollisionRectH->get_held_number() * editor0ZoomValue;
                        sprCX-= sprCW/2;
                        sprCY-= sprCH/2;

                        //sprCX-=sprCW/2;
                        //sprCY-=sprCH/2;
                        if( sprCX >= 0 && sprCY >=0 && sprCW > 0 && sprCH > 0)
                        {
                            gcanvas->render_rectangle( sprCX,sprCY,sprCX+sprCW,sprCY+sprCH,c_green,false,96);
                        }
                    }
                    else if( spriteCollisionShapeMenu->get_selected_id()==1 )
                    {
                        int sprCX = spritePreviewCam->x+( spritePreviewCam->w )/2 - (animInEditor->get_width()*editor0ZoomValue)/2- (  spriteCollisionCircleX->get_held_number()*editor0ZoomValue );
                        int sprCY = spritePreviewCam->y+( spritePreviewCam->h )/2 - (animInEditor->get_height()*editor0ZoomValue)/2- (  spriteCollisionCircleY->get_held_number()*editor0ZoomValue );
                        int sprCR = spriteCollisionCircleR->get_held_number() * editor0ZoomValue;
                        sprCX+=sprCR/2;
                        sprCY+=sprCR/2;
                        if( sprCX >= 0 && sprCY >=0 && sprCR > 0)
                        {
                            gcanvas->render_circle_color( sprCX,sprCY,sprCR,c_gray,192);
                        }
                    }
                }
            }
            process_data_fields(GPE_VERSION_DOUBLE_NUMBER);
            if( previewZoomLevel!=NULL && previewZoomLevel->is_clicked() )
            {
                gfs->render_text_boxed( viewedSpace->w-32,viewedSpace->h-32,
                                  "Zoom Level: "+double_to_string(editor0ZoomValue*100 )+"%",
                                  GPE_MAIN_THEME->Text_Box_Font_Color,c_black, GPE_DEFAULT_FONT,FA_RIGHT,FA_BOTTOM);
            }
        }
        else
        {
            //renders the sprite in edit
            if( animInEditor!=NULL && animInEditor->has_texture() )
            {
                //renders the frame boxes for the sprite
                animInEditor->render_piece_resized( spritePreviewCam->x, spritePreviewCam->y, animCameraRect.w* editor1ZoomValue, animCameraRect.h* editor1ZoomValue, &animCameraRect );

                /*
                int i = 0;
                int rectanglesRendered = 0;
                int maxSpriteFrames = spriteDataFields[0]->get_held_number();
                GPE_Rect * tempRect = NULL;

                int iTSX = 0;
                int bx1 = 0;
                int bx2 = 0;
                int by1 = 0;
                int by2 = 0;
                for( i = (int)animInEditor->get_frame_count()-1; i>=0 i--)
                {
                    if( maxSpriteFrames < 0 || ( rectanglesRendered < maxSpriteFrames) )
                    {
                        tempRect = &animInEditor->animFrames[i];
                        bx1 = spritePreviewCam->x+tempRect->x*editor1ZoomValue - animCameraRect.x*editor1ZoomValue;
                        //bx1 = bound_number(bx1,0, elementBox.w);

                        by1 = spritePreviewCam->y+tempRect->y*editor1ZoomValue - animCameraRect.y*editor1ZoomValue;
                        //by1 = bound_number(by1,0, elementBox.h);

                        bx2 = spritePreviewCam->x+(tempRect->x+tempRect->w)*editor1ZoomValue - animCameraRect.x*editor1ZoomValue;
                        //bx2 = bound_number(bx2,0, elementBox.w);

                        by2 = spritePreviewCam->y+(tempRect->y+tempRect->h)*editor1ZoomValue - animCameraRect.y*editor1ZoomValue;
                        //by2 = bound_number(by2,0, elementBox.h);
                        gcanvas->render_rectangle( bx1-cam->x, by1-cam->y, bx2-cam->x, by2-cam->y, GPE_MAIN_THEME->Text_Box_Font_Color,true,255);
                        rectanglesRendered++;
                        if( previewSubImageNumbers->is_clicked() && editor1ZoomValue > 0.40 )
                        {
                            gfs->render_text( bx1-cam->x+GENERAL_GPE_PADDING, by2-4-cam->y, int_to_string(i), GPE_MAIN_THEME->Main_Box_Font_Highlight_Color, GPE_DEFAULT_FONT, FA_LEFT, FA_BOTTOM );
                        }
                    }
                }
                */
            }
            if( previewZoomLevel!=NULL && previewZoomLevel->is_clicked() )
            {
                gfs->render_text_boxed( viewedSpace->w-32,viewedSpace->h-32,
                                  "Zoom Level: "+double_to_string(editor1ZoomValue*100 )+"%",
                                  GPE_MAIN_THEME->Text_Box_Font_Color,c_black, GPE_DEFAULT_FONT,FA_RIGHT,FA_BOTTOM);
            }
        }



        /*if( spritePreviewIsRendered)
        {
            gfs->render_text( spritePreviewCam->x,0,
                            "Dimensions: "+int_to_string(animInEditor->animationTexture->get_width() )+" x "
                            +int_to_string(animInEditor->animationTexture->get_height() )+"px",
                            GPE_MAIN_THEME->Text_Box_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP);

        }*/

        if( editorMode ==1  )
        {
            if( animXScroll!=NULL)
            {
                animXScroll->render_self( viewedSpace,cam);
            }
            if( animYScroll!=NULL)
            {
                animYScroll->render_self( viewedSpace,cam);
            }
        }
        //Only used on 1st editor mode for now...
        //if( editorMode == 0)
        topPanel->render_self( viewedSpace,cam, forceRedraw );
    }

}


void animationResource::revert_data_fields()
{
    for( int i = 0; i < SPRITE_DATA_FIELD_COUNT; i++ )
    {
        if( spriteDataFields[i]!=NULL)
        {
            spriteDataFields[i]->switch_inuse( false );
        }
        else
        {
            GPE_Report("Sprite Detected to have NULL field at ["+int_to_string(i)+"]");
            return;
        }
    }
    if( animInEditor!=NULL )
    {
        spriteDataFields[sprFrameCount]->set_number( animInEditor->get_frame_count() );
        spriteDataFields[sprFrameWidth]->set_number( animInEditor->get_width() );
        spriteDataFields[sprFrameHeight]->set_number( animInEditor->get_height() );
        spriteDataFields[sprHPixelOffset]->set_number( animInEditor->get_frame_xoffset() );
        spriteDataFields[sprVPixelOffset]->set_number( animInEditor->get_frame_yoffset() );
        spriteDataFields[sprHPixelPadding]->set_number( animInEditor->get_hpadding() );
        spriteDataFields[sprVPixelPadding]->set_number( animInEditor->get_vpadding() );
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
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/animations/";
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
                for( int i =0; i < SPRITE_DATA_FIELD_COUNT; i++)
                {
                    newSaveDataFile << "SpriteData["<< int_to_string(i)<< "]=";

                    if( spriteDataFields[i]!=NULL)
                    {
                        newSaveDataFile << int_to_string( spriteDataFields[i]->get_held_number() )<<"\n";
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
                newSaveDataFile << "CollisionShape="<< int_to_string (spriteCollisionShapeMenu->get_selected_id() ) +"\n";

                newSaveDataFile << "CircleCollisionRegion=" << int_to_string (spriteCollisionCircleX->get_held_number() ) +",";
                newSaveDataFile << int_to_string (spriteCollisionCircleY->get_held_number() ) +",";
                newSaveDataFile << int_to_string (spriteCollisionCircleR->get_held_number() )+"\n";
                //Rectangle info

                newSaveDataFile << "RectangleCollisionBox=" << int_to_string (spriteCollisionRectX->get_held_number() ) +",";
                newSaveDataFile << int_to_string (spriteCollisionRectY->get_held_number() ) +",";
                newSaveDataFile << int_to_string (spriteCollisionRectW->get_held_number() ) +",";
                newSaveDataFile << int_to_string (spriteCollisionRectH->get_held_number() )+"\n";
                std::string resFileCopySrc = animInEditor->get_file_name();
                std::string resFileLocation = getShortFileName (resFileCopySrc,true );
                newSaveDataFile << "ImageLocation="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 )
                {
                    std::string resFileCopyDest = soughtDir+resFileLocation;
                    if( file_exists(resFileCopyDest) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]Sprite Image File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] Sprite Image File? This action is irreversible!")==DISPLAY_QUERY_YES)
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
            *fileTarget << nestedTabsStr << "Sprite=" << resourceName << "," << get_global_rid() << ",";
            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}

