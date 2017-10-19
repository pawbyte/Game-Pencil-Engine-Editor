/*
spriteresource.cpp
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

#include "spriteresource.h"

spriteResource::spriteResource(GPE_ResourceContainer * pFolder)
{
    zoomLevel = 1;
    editorPaneList->set_horizontal_align(FA_LEFT);
    editorPaneList->barXPadding = GENERAL_GPE_PADDING;
    editorPaneList->barXMargin = 0;

    projectParentFolder = pFolder;
    editorMode = 0;
    spriteInEditor = new GPE_Sprite();

    labelSpriteDimensions = new GPE_Label_Text("Sprite Dimensions");
    labelFrameInfo = new GPE_Label_Text("Frame Info");
    labelSpriteMessage = new GPE_Label_Text("Sprite Message");
    //Sprite Collision Area
    preloadCheckBox = new GPE_CheckBoxBasic("Preload Sprite","Check to load sprite at game open",0,0,true);
    showCollisionShapeCheckBox = new GPE_CheckBoxBasic("Preview Collision Shape","",0,0,true);
    showAnimationCheckBox = new GPE_CheckBoxBasic("Preview Sprite Animation","",0,0,true);
    spriteCollisionShapeMenu = new GPE_DropDown_Menu(0,0,"Collision Shape",true);
    spriteCollisionShapeMenu->add_menu_option("Rectangle");
    spriteCollisionShapeMenu->add_menu_option("Circle");
    //spriteCollisionShapeMenu->add_menu_option("Triangle [ Coming Soon]");
    //spriteCollisionShapeMenu->add_menu_option("Diamond [ Coming Soon]");
    //spriteCollisionShapeMenu->add_menu_option("Hexagon [ Coming Soon]");

    spriteCenterCollisionButton = new GPE_ToolLabelButton(0,0,"Center Collision Shape","Centers the collision box");
    spriteCollisionRectX= new GPE_TextInputNumber("0",true,0);
    spriteCollisionRectX->set_label("X-Pos");
    spriteCollisionRectX->set_string("0");
    spriteCollisionRectY= new GPE_TextInputNumber("0",true,0);
    spriteCollisionRectY->set_label("Y-Pos");
    spriteCollisionRectY->set_string("0");
    spriteCollisionRectW= new GPE_TextInputNumber("0",true,0);
    spriteCollisionRectW->set_label("Width");
    spriteCollisionRectW->set_string("0");
    spriteCollisionRectH= new GPE_TextInputNumber("0",true,0);
    spriteCollisionRectH->set_label("Height");
    spriteCollisionRectH->set_string("0");

    spriteCollisionCircleX= new GPE_TextInputNumber("0",true,0);
    spriteCollisionCircleX->set_label("X-Pos");
    spriteCollisionCircleX->set_string("0");
    spriteCollisionCircleY= new GPE_TextInputNumber("0",true,0);
    spriteCollisionCircleY->set_label("Y-Pos");
    spriteCollisionCircleY->set_string("0");
    spriteCollisionCircleR= new GPE_TextInputNumber("0",true,0);
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
    if( saveResourceButton!=NULL)
    {
        editResourceButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/edit.png","Edit Resource","Edit Sprite Data",-1);
    }
    transformResourceButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/magic.png","Transform IMG","Transform the Image",-1);
    playPauseResourceButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png","Stop","Stops Animation",-1);
    playPauseResourceButton->set_width( transformResourceButton->get_width() );
    subImagePreviewNumber = 0;
    subImageMiniAnimationNumber = 0;
    resourcePostProcessed = false;

    //Subimage
    previousSubImageButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/arrow-left.png","Previous");
    //previousSubImageButton->set_sprite(mainMenuSprite, 36 );
    nextSubImageButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/arrow-right.png","Next");
    //nextSubImageButton->set_sprite(mainMenuSprite, 38 );
    subImageNumberField = new GPE_TextInputNumber("");
    subImageNumberField->set_string("0");
    subImageNumberField->set_width(64);
    subImageEditorPreviewId = new GPE_TextInputNumber("");
    subImageEditorPreviewId->set_string("0");
    subImageEditorPreviewId->set_label("Editor Preview Frame");
    openExternalEditorButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/rocket.png","Use External Editor","Opens Sprite Image In External Editor");
    refreshResourceDataButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/refresh.png","Refresh","Refreshes the loaded sprite image");
    labelInfoMaxTextureSize = new GPE_Label_Text("Max Image Size: 8192 X 8192px","Max Image Size: 8192 X 8192px");
}

spriteResource::~spriteResource()
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
    if( spriteInEditor!=NULL)
    {
        delete spriteInEditor;
        spriteInEditor = NULL;
    }
    GPE_TextInputNumber *  tempDataField = NULL;
    for( int i =SPRITE_DATA_FIELD_COUNT-1; i >=0; i--)
    {
        tempDataField = spriteDataFields[i];
        if( tempDataField!=NULL)
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
}

bool spriteResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        bool isBuildReady = true;
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string nestedTabsStrObjFunc = generate_tabs( leftTabAmount +1 );
        std::string html5SpShName = get_name();
        if( spriteInEditor!=NULL)
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
            if( spriteInEditor->width <=0 || spriteInEditor->height <=0)
            {
                 isBuildReady = false;
            }
            if( isBuildReady)
            {
                *fileTarget << nestedTabsStr << "var " << html5SpShName << " =  GPE.rsm.add_sprite(";
                *fileTarget << int_to_string (html5BuildGlobalId ) +",";
                *fileTarget << int_to_string ( (int)spriteInEditor->spriteImages.size() ) +",";
                *fileTarget << "'resources/sprites/"+getShortFileName (spriteInEditor->fileName,true )+"',";
                *fileTarget << int_to_string (spriteInEditor->xoffset ) +",";
                *fileTarget << int_to_string (spriteInEditor->yoffset ) +",";
                *fileTarget << int_to_string (spriteInEditor->width ) +",";
                *fileTarget << int_to_string (spriteInEditor->height ) +",";
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

int spriteResource::get_preview_frame()
{
    int previewFrameId = 0;
    if( subImageEditorPreviewId!=NULL && spriteInEditor!=NULL)
    {
        previewFrameId = subImageEditorPreviewId->get_held_number();
        if( (int)spriteInEditor->spriteImages.size() <= previewFrameId)
        {
            previewFrameId = spriteInEditor->spriteImages.size() - 1;
        }
        if( previewFrameId < 0)
        {
            previewFrameId = 0;
        }
        //subImageEditorPreviewId->set_string( int_to_string(previewFrameId) );
    }
    return previewFrameId;
}

void spriteResource::load_image(std::string newFileName, bool autoProcess)
{
    if( (int)newFileName.size() > 3 && newFileName!="NULL")
    {
        if( file_is_image(newFileName) )
        {
            GPE_Texture * tempNewTexture = new GPE_Texture();
            tempNewTexture->load_new_texture(MAIN_RENDERER, newFileName, -1, false);
            if( tempNewTexture->get_width() <1 || tempNewTexture->get_width()>8192 || tempNewTexture->get_height() <1 || tempNewTexture->get_height()>8192 )
            {
                display_user_alert("Unable to load image","Editor Error: Unable to load ["+newFileName+"] please check file and make sure it is between 1x1 and 8192x8192 pixels and is a valid image");
                delete tempNewTexture;
                tempNewTexture = NULL;
                record_error("[Sprite Error:] Unable to load image: " +newFileName);
                appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","[Sprite Error:] Unable to load image: " +newFileName);
            }
            else
            {
                if(spriteInEditor==NULL)
                {
                    spriteInEditor = new GPE_Sprite();
                    autoProcess = true;
                }
                if( spriteInEditor!=NULL)
                {
                    if(spriteInEditor->spriteTexture!=NULL)
                    {
                       delete spriteInEditor->spriteTexture;
                       spriteInEditor->spriteTexture = NULL;
                    }
                    spriteInEditor->spriteTexture = tempNewTexture;
                    spriteInEditor->fileName = newFileName;

                    spriteInEditor->spriteImages.clear();
                    GPE_Rect newBox;
                    newBox.w = spriteInEditor->spriteTexture->get_width();
                    newBox.h = spriteInEditor->spriteTexture->get_height();
                    spriteInEditor->spriteImages.push_back(newBox);
                    if( autoProcess )
                    {
                        for( int i =0; i < SPRITE_DATA_FIELD_COUNT; i++)
                        {
                            spriteDataFields[i]->set_number(0);
                        }
                        spriteDataFields[sprFrameWidth]->set_number( spriteInEditor->spriteTexture->get_width() );
                        spriteDataFields[sprFrameHeight]->set_number( spriteInEditor->spriteTexture->get_height() );

                        spriteCollisionCircleX->set_number(0);
                        spriteCollisionCircleY->set_number(0);
                        spriteCollisionCircleR->set_number(  std::min (spriteInEditor->spriteTexture->get_width(),spriteInEditor->spriteTexture->get_height() ) );

                        spriteCollisionRectX->set_number(0);
                        spriteCollisionRectY->set_number(0);
                        spriteCollisionRectW->set_number( spriteInEditor->spriteTexture->get_width() );
                        spriteCollisionRectH->set_number( spriteInEditor->spriteTexture->get_height() );
                    }
                    process_data_fields(GPE_VERSION_DOUBLE_NUMBER);
                    spriteInEditor->spriteTexture->copy_image_source( fileToDir(parentProjectName)+"/gpe_project/resources/sprites");
                }
            }
        }
        else
        {
            display_user_alert("Unable to load image","File type["+get_file_ext(newFileName)+"] not supported when loading ["+newFileName+"].");
        }
    }
}

void spriteResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed ==false || file_exists(alternatePath))
    {
        displayMessageTitle = "Processing Sprite";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();

        std::string otherColContainerName = "";

        std::string newFileIn = "";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/sprites/";
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

        //record_error("Loading sprite - "+newFileIn);
        //If the level file could be loaded
        double foundFileVersion = -1;
        if( !gameResourceFileIn.fail() )
        {
            //record_error("Procesing sprite file...");
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
                                else if( keyString=="ImageLocation")
                                {
                                    if( valString!="NULL")
                                    {
                                        load_image( soughtDir+valString );
                                    }
                                }
                                else if( keyString=="Preload")
                                {
                                    preloadCheckBox->set_clicked(is_bool(valString) );
                                }
                                else if( keyString=="EditorPreviewFrame")
                                {
                                    subImageEditorPreviewId->set_string( valString );
                                }
                                else if( keyString=="PreviewCollisionShape")
                                {
                                    if( showCollisionShapeCheckBox!=NULL)
                                    {
                                        showCollisionShapeCheckBox->set_clicked(is_bool(valString) );
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
                                    if( compare_doubles(foundFileVersion , GPE_VERSION_SIG_UPDATE1) || foundFileVersion > GPE_VERSION_SIG_UPDATE1 )
                                    {
                                        //record_error("Proper sprite system used ["+resourceName+"]");
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
                                        //record_error("Alternate sprite system used ["+resourceName+"]");
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
                        record_error("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
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

void spriteResource::process_data_fields(float versionToProcess)
{
    if( spriteInEditor!=NULL && spriteInEditor->spriteTexture!=NULL)
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
            record_error("Beginning to Process Sprite Data...");
            int fWidth = spriteDataFields[sprFrameWidth]->make_valid_number(0);
            int fHeight = spriteDataFields[sprFrameHeight]->make_valid_number(0);

            int fPixelOffsetH = spriteDataFields[sprHPixelOffset]->make_valid_number(0);
            spriteInEditor->xoffset = fPixelOffsetH;
            int fPixelOffsetV = spriteDataFields[sprVPixelOffset]->make_valid_number(0);
            spriteInEditor->yoffset = fPixelOffsetV;

            int hPadding = spriteDataFields[sprHPixelPadding]->make_valid_number(0);
            int vPadding = spriteDataFields[sprVPixelPadding]->make_valid_number(0);
            GPE_Rect tempRect;
            tempRect.x = fPixelOffsetH;
            tempRect.y = fPixelOffsetV;
            tempRect.w = fWidth;
            tempRect.h = fHeight;
            if( fWidth > 0 && fHeight > 0 && hPadding>=0 && vPadding >=0)
            {
                spriteInEditor->spriteImages.clear();
                int startX = fPixelOffsetH;
                int startY = fPixelOffsetV;

                int spritesAdded = 0;
                int maxSpriteFrames = spriteDataFields[sprFrameCount]->make_valid_number(-1);
                if( versionToProcess > 0 && ( compare_doubles(versionToProcess,GPE_VERSION_SIG_UPDATE1) || versionToProcess > GPE_VERSION_SIG_UPDATE1  ) && maxSpriteFrames > 0)
                {
                    for( i = startY; i < spriteInEditor->spriteTexture->get_height();  i += fHeight+vPadding)
                    {
                        for( j = startX; j < spriteInEditor->spriteTexture->get_width();  j += fWidth+hPadding)
                        {
                            if( spritesAdded <  maxSpriteFrames )
                            {
                                GPE_Rect newRect;
                                newRect.x = j;
                                newRect.y = i;
                                newRect.w = fWidth;
                                newRect.h = fHeight;
                                spriteInEditor->spriteImages.push_back( newRect );
                                spritesAdded++;
                            }
                        }
                    }
                }
                else
                {
                    for( i = startY; i < spriteInEditor->spriteTexture->get_height();  i += fHeight+vPadding)
                    {
                        for( j = startX; j < spriteInEditor->spriteTexture->get_width();  j += fWidth+hPadding)
                        {
                            GPE_Rect newRect;
                            newRect.x = j;
                            newRect.y = i;
                            newRect.w = fWidth;
                            newRect.h = fHeight;
                            spriteInEditor->spriteImages.push_back( newRect );
                            spritesAdded++;
                        }
                    }
                    spriteDataFields[sprFrameCount]->set_number(spritesAdded);
                }
                spriteInEditor->width = fWidth;
                spriteInEditor->height = fHeight;
            }
        }
    }
}

void spriteResource::prerender_self(GPE_Renderer * cRender )
{
	standardEditableGameResource::prerender_self( cRender);
	if( editResourceButton!=NULL)
    {
        editResourceButton->prerender_self( cRender);
    }
    if( transformResourceButton!=NULL)
    {
        transformResourceButton->prerender_self( cRender);
    }
    if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self( cRender);
    }
    if( showAnimationCheckBox!=NULL)
    {
        showAnimationCheckBox->prerender_self( cRender);
    }
    if( spriteCenterCollisionButton!=NULL)
    {
        spriteCenterCollisionButton->prerender_self( cRender);
    }
}

void spriteResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    int i;
    if( cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL )
    {
        int editorPaneW = 320;
        editorPaneList->set_coords(0,0);
        editorPaneList->set_width(editorPaneW);
        editorPaneList->set_height(viewedSpace->h);
        editorPaneList->barXPadding = GENERAL_GPE_PADDING/2;
        editorPaneList->barXMargin = 0;

        editorPaneList->clear_list();

        if( spriteInEditor!=NULL)
        {
            if( spriteInEditor->spriteTexture!=NULL)
            {
                labelSpriteDimensions->set_name("Image Dimensions:"+int_to_string( spriteInEditor->spriteTexture->get_width() )+" x "+int_to_string( spriteInEditor->spriteTexture->get_height() )+"px");
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
            editorPaneList->add_gui_element(renameBox,true);
            renameBox->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);
            editorPaneList->add_gui_element(refreshResourceDataButton,true);
            refreshResourceDataButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);
            editorPaneList->add_gui_element(loadResourceButton,true);
            editorPaneList->add_gui_element(labelInfoMaxTextureSize,true);
            loadResourceButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);
            //editorPaneList->add_gui_element(saveResourceButton,true);
            editorPaneList->add_gui_element(editResourceButton,true);
            editResourceButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3 );
            editorPaneList->add_gui_element(transformResourceButton,true);
            transformResourceButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);
            editorPaneList->add_gui_element( labelSpriteDimensions,true);

            labelFrameInfo->set_name("Sprite Size: "+int_to_string(spriteInEditor->width )+" x "+int_to_string(spriteInEditor->height )+"px");
            editorPaneList->add_gui_element( labelFrameInfo,true);

            editorPaneList->add_gui_element(preloadCheckBox,true);
            editorPaneList->add_gui_element(playPauseResourceButton,true);
            playPauseResourceButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);

            editorPaneList->add_gui_element( previousSubImageButton,false);
            editorPaneList->add_gui_element( subImageNumberField,false);
            editorPaneList->add_gui_element( nextSubImageButton,true);
            editorPaneList->add_gui_element( subImageEditorPreviewId,true);


            //Collision stuffs
            editorPaneList->add_gui_element( spriteCollisionShapeMenu,true);
            spriteCollisionShapeMenu->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);

            editorPaneList->add_gui_element( spriteCenterCollisionButton,true);
            spriteCenterCollisionButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);

            if( spriteCollisionShapeMenu->get_selected_id()==0 )
            {
                editorPaneList->add_gui_element( spriteCollisionRectX,false);
                editorPaneList->add_gui_element( spriteCollisionRectY,true);
                editorPaneList->add_gui_element( spriteCollisionRectW,false);
                editorPaneList->add_gui_element( spriteCollisionRectH,true);
            }
            else if( spriteCollisionShapeMenu->get_selected_id()==1 )
            {
                editorPaneList->add_gui_element( spriteCollisionCircleX,true);
                spriteCollisionCircleX->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);

                editorPaneList->add_gui_element( spriteCollisionCircleY,true);
                spriteCollisionCircleY->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);

                editorPaneList->add_gui_element( spriteCollisionCircleR,true);
                spriteCollisionCircleR->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);
            }

            editorPaneList->add_gui_element( showCollisionShapeCheckBox,true);

            editorPaneList->add_gui_element( openExternalEditorButton,true);
            openExternalEditorButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);

            editorPaneList->add_gui_element(confirmResourceButton,true);
            confirmResourceButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);

            editorPaneList->add_gui_element(cancelResourceButton,true);
            cancelResourceButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);
            //editorPaneList->set_maxed_out_width();
            editorPaneList->process_self(viewedSpace,cam);
            if( loadResourceButton!=NULL)
            {
                if( loadResourceButton->is_clicked() )
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
                        editorMode = 1;
                        userInput->reset_all_input();
                        process_self(viewedSpace,cam);
                    }
                }
            }

            if( saveResourceButton!=NULL)
            {
                saveResourceButton->process_self(viewedSpace,cam);
                if( saveResourceButton->is_clicked() )
                {
                    //display_user_alert( "File Saved", GPE_GetSaveFileName("Test and save your file","",GPE_MAIN_GUI->file).c_str());
                }
            }

            if( editResourceButton!=NULL)
            {
                if( editResourceButton->is_clicked() )
                {
                    editorMode = 1;
                }
            }

            if( transformResourceButton!=NULL)
            {
                if( transformResourceButton->is_clicked() && spriteInEditor!=NULL)
                {
                    if( spriteInEditor->width > 0 && spriteInEditor->height > 0)
                    {
                        GPE_open_context_menu();
                        MAIN_CONTEXT_MENU->set_width(256);
                        MAIN_CONTEXT_MENU->add_menu_option("Erase BG Color",0);
                        MAIN_CONTEXT_MENU->add_menu_option("Invert Colors",1);
                        MAIN_CONTEXT_MENU->add_menu_option("Make GrayScale",2);
                        MAIN_CONTEXT_MENU->add_menu_option("Exit",10);
                        int menuSelection = get_popupmenu_result();
                        if( menuSelection>=0 && menuSelection <=3)
                        {
                            SDL_Surface * oTempSurface = load_surface_image(spriteInEditor->fileName);
                            SDL_Surface *nTempSurface = NULL;
                            if( oTempSurface!=NULL)
                            {
                                if( menuSelection==0)
                                {
                                    GPE_Color * foundBGColor = GPE_Get_Color_PopUp("Image Background Color To Remove",c_fuchsia);
                                    if( foundBGColor!=NULL)
                                    {
                                        if( display_get_prompt("Are you sure you want to erase this Color from this image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                        {
                                            record_error("Modifying image at: "+spriteInEditor->fileName+".");
                                            nTempSurface=surface_remove_color(oTempSurface,foundBGColor->get_sdl_color() );
                                            delete foundBGColor;
                                            foundBGColor = NULL;
                                        }
                                    }

                                }
                                else if( menuSelection==1 )
                                {
                                    if( display_get_prompt("Are you sure you want to invert your image's colors?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                    {
                                        nTempSurface=surface_invert(oTempSurface);
                                    }
                                }
                                else if( menuSelection==2 )
                                {
                                    if( display_get_prompt("Are you sure you want to grayscale your image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                    {
                                        nTempSurface=surface_grayscale(oTempSurface);
                                    }
                                }
                                if( nTempSurface!=NULL)
                                {
                                    remove( spriteInEditor->fileName.c_str() );
                                    std::string newImageName = get_file_noext(spriteInEditor->fileName)+".png";
                                    IMG_SavePNG(nTempSurface,newImageName.c_str() );
                                    spriteInEditor->fileName = newImageName;
                                    SDL_Texture * tChangedTex = SDL_CreateTextureFromSurface(MAIN_RENDERER->get_renderer(),nTempSurface);
                                    if( tChangedTex!=NULL && spriteInEditor->spriteTexture!=NULL)
                                    {
                                        spriteInEditor->spriteTexture->change_texture(tChangedTex);
                                    }
                                    else
                                    {
                                        //spriteInEditor->spriteTexture = tChangedTex;
                                    }
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

            if( refreshResourceDataButton!=NULL  )
            {
                if( refreshResourceDataButton->is_clicked() )
                {
                    resourcePostProcessed = false;
                    preprocess_self();
                }
            }
            if(renameBox!=NULL)
            {
                if( renameBox->get_string()!=resourceName)
                {
                    isModified = true;
                }
            }

            if(isPreloaded!=preloadCheckBox->is_clicked() )
            {
                isModified = true;
            }

            if( confirmResourceButton!=NULL)
            {
                //actual saving of the sprite onto the engine and files
                if( confirmResourceButton->is_clicked() )
                {
                    isPreloaded = preloadCheckBox->is_clicked();
                }
            }

            if( spriteInEditor!=NULL)
            {
                if( playPauseResourceButton!=NULL)
                {
                    if( playPauseResourceButton->is_clicked() )
                    {
                        if( playPauseResourceButton->get_name()=="Play")
                        {
                            playPauseResourceButton->set_name("Stop");
                            playPauseResourceButton->change_texture( rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/stop.png") );
                        }
                        else
                        {
                            playPauseResourceButton->set_name("Play");
                            playPauseResourceButton->change_texture( rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png") );
                        }
                        playPauseResourceButton->set_width(transformResourceButton->get_width());
                    }
                }

                if( playPauseResourceButton->get_name()=="Stop" )
                {
                    subImageMiniAnimationNumber++;
                    if( FPS_RATIO > 4 )
                    {
                        if( subImageMiniAnimationNumber > 2*FPS_RATIO/4 )
                        {
                            subImagePreviewNumber++;
                            subImageNumberField->set_number( subImagePreviewNumber);
                            subImageMiniAnimationNumber= 0;
                        }
                    }
                    else if( subImageMiniAnimationNumber > 10*FPS_RATIO )
                    {
                        subImagePreviewNumber++;
                        subImageNumberField->set_number( subImagePreviewNumber);
                        subImageMiniAnimationNumber= 0;
                    }
                }

                if( previousSubImageButton!=NULL)
                {
                    if( previousSubImageButton->is_clicked() && subImagePreviewNumber > 0)
                    {
                        subImagePreviewNumber--;
                        subImageNumberField->set_number( subImagePreviewNumber);
                        playPauseResourceButton->set_name("Play");
                        playPauseResourceButton->change_texture( rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png") );
                        playPauseResourceButton->set_width(transformResourceButton->get_width());
                    }
                }

                if( subImageNumberField!=NULL)
                {
                    if( subImageNumberField->is_inuse() )
                    {
                        subImagePreviewNumber = subImageNumberField->get_held_number();
                        playPauseResourceButton->set_name("Play");
                        playPauseResourceButton->change_texture( rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png") );
                        playPauseResourceButton->set_width(transformResourceButton->get_width());
                    }
                }

                if( nextSubImageButton!=NULL)
                {
                    if( nextSubImageButton->is_clicked() )
                    {
                        subImagePreviewNumber++;
                        subImageNumberField->set_number( subImagePreviewNumber);
                        playPauseResourceButton->set_name("Play");
                        playPauseResourceButton->change_texture( rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png") );
                        playPauseResourceButton->set_width(transformResourceButton->get_width());
                    }
                }


                if( subImagePreviewNumber >= (int)spriteInEditor->spriteImages.size() )
                {
                    subImagePreviewNumber = 0;
                    subImageNumberField->set_number( subImagePreviewNumber);
                }
            }

            if(subImagePreviewNumber < 0 )
            {
                subImagePreviewNumber = 0;
                subImageNumberField->set_number( subImagePreviewNumber);
            }

            if( spriteCollisionShapeMenu!=NULL)
            {
                if( spriteCollisionShapeMenu->get_selected_id()==0 )
                {
                    if( spriteCenterCollisionButton->is_clicked() && spriteInEditor!=NULL)
                    {
                        spriteCollisionRectX->set_string( int_to_string( spriteInEditor->width/4) );
                        spriteCollisionRectY->set_string( int_to_string( spriteInEditor->height/4) );
                        spriteCollisionRectW->set_string( int_to_string( spriteInEditor->width/2) );
                        spriteCollisionRectH->set_string( int_to_string( spriteInEditor->height/2) );
                    }
                }
                else if( spriteCollisionShapeMenu->get_selected_id()==1 )
                {
                    if( spriteCenterCollisionButton->is_clicked() && spriteInEditor!=NULL)
                    {
                        int smallestCenter = std::min( spriteInEditor->width, spriteInEditor->height);
                        spriteCollisionCircleX->set_string( "0" );
                        spriteCollisionCircleY->set_string( "0");
                        spriteCollisionCircleR->set_string( int_to_string(smallestCenter/2) );
                    }
                }
            }
        }
        else if( editorMode==1)
        {
            editorPaneList->add_gui_element(labelSpriteDimensions,true);
            for(  i =0; i < SPRITE_DATA_FIELD_COUNT; i++)
            {
                if( spriteDataFields[i]!=NULL)
                {
                    editorPaneList->add_gui_element(spriteDataFields[i],true );
                    spriteDataFields[i]->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);
                }
            }
            editorPaneList->add_gui_element( openExternalEditorButton,true);
            openExternalEditorButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);

            editorPaneList->add_gui_element(confirmResourceButton,true );
            confirmResourceButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);

            editorPaneList->add_gui_element(cancelResourceButton,true );
            cancelResourceButton->set_width(editorPaneList->get_width()-GENERAL_GPE_PADDING*3);
            ///
            //editorPaneList->set_maxed_out_width();
            editorPaneList->process_self(viewedSpace,cam);
            if( spriteInEditor!=NULL)
            {
                process_data_fields(GPE_VERSION_DOUBLE_NUMBER);
            }

            if( confirmResourceButton!=NULL)
            {
                //actual saving of the sprite onto the engine and files
                if( confirmResourceButton->is_clicked() )
                {
                    for( int i =0; i < SPRITE_DATA_FIELD_COUNT; i++)
                    {
                        if( spriteDataFields[i]!=NULL)
                        {
                            spriteDataFields[i]->switch_inuse(false);
                        }
                    }
                    process_data_fields(GPE_VERSION_DOUBLE_NUMBER);
                    editorMode = 0;

                    spriteCollisionCircleX->set_number(0);
                    spriteCollisionCircleY->set_number(0);
                    spriteCollisionCircleR->set_number(  std::min (spriteDataFields[sprFrameWidth]->get_held_number(),spriteDataFields[sprFrameHeight]->get_held_number() ) );

                    spriteCollisionRectX->set_number(0);
                    spriteCollisionRectY->set_number(0);
                    spriteCollisionRectW->set_number( spriteDataFields[sprFrameWidth]->get_held_number() );
                    spriteCollisionRectH->set_number( spriteDataFields[sprFrameHeight]->get_held_number() );
                }
            }
            if( cancelResourceButton!=NULL)
            {
                if( cancelResourceButton->is_clicked() )
                {
                    editorMode = 0;
                    record_error("Reverting to Editor Mode 0");
                }
            }
        }

        if( openExternalEditorButton!=NULL && spriteInEditor!=NULL)
        {
            if( openExternalEditorButton->is_clicked() )
            {
                if( file_exists(spriteInEditor->fileName) )
                {
                    if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=NULL)
                    {
                        GPE_OpenProgram(MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),spriteInEditor->fileName, true );
                    }
                    else
                    {
                        GPE_OpenURL(spriteInEditor->fileName);
                    }
                    appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+spriteInEditor->fileName+"]...");
                }
            }
        }
        if( !editorPaneList->hasScrollControl && editorMode==0)
        {
            if( userInput->check_keyboard_down(kb_ctrl) )
            {
                if( userInput->check_keyboard_released(kb_minus) )
                {
                    zoomLevel*=0.5;
                }
                else if( userInput->check_keyboard_released(kb_plus) )
                {
                    zoomLevel*=2.f;
                }
            }
        }
        if( zoomLevel < ZOOM_LEVEL_MIN)
        {
            zoomLevel = ZOOM_LEVEL_MIN;
        }
        if( zoomLevel > ZOOM_LEVEL_MAX)
        {
            zoomLevel = ZOOM_LEVEL_MAX;
        }
    }
}

void spriteResource::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace,GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    bool spritePreviewIsRendered = false;
    if( cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL  )
    {
        if( forceRedraw || editorMode==0 )
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
        }
        GPE_Rect spritePreviewCam;
        spritePreviewCam.x = viewedSpace->x+editorPaneList->get_x2pos();
        spritePreviewCam.y = viewedSpace->y+GENERAL_GPE_PADDING;
        spritePreviewCam.w = viewedSpace->x+viewedSpace->w-spritePreviewCam.x - GENERAL_GPE_PADDING;
        spritePreviewCam.h = abs(viewedSpace->y+viewedSpace->h-GENERAL_GPE_PADDING- spritePreviewCam.y)-32;
        if( editorMode==0)
        {
            //renders the right side of the area, mainly preview of sprite
        }
        cRender->set_viewpoint(&spritePreviewCam );

        if( (forceRedraw || editorMode==0) && GPE_TEXTURE_TRANSPARENT_BG!=NULL)
        {
            for(int iPV= spritePreviewCam.x; iPV< spritePreviewCam.x+spritePreviewCam.w;iPV+=GPE_TEXTURE_TRANSPARENT_BG->get_width() )
            {
                for(int jPV= spritePreviewCam.y; jPV< spritePreviewCam.y+spritePreviewCam.h; jPV+=GPE_TEXTURE_TRANSPARENT_BG->get_height() )
                {
                    GPE_TEXTURE_TRANSPARENT_BG->render_tex(cRender,iPV-spritePreviewCam.x,jPV-spritePreviewCam.y,NULL,NULL);
                }
            }
        }
        //renders the frame boxes for the sprite
        if( editorMode==0)
        {
            if( spriteInEditor!=NULL)
            {
                if ( spriteInEditor->spriteTexture!=NULL)
                {
                    if( subImagePreviewNumber >=0 && subImagePreviewNumber < (int)spriteInEditor->spriteImages.size() )
                    {
                        //spriteInEditor->spriteTexture->render_tex_resized(cRender,0,0,, &spriteInEditor->spriteImages[subImagePreviewNumber],NULL);
                        spriteInEditor->render_resized(cRender,subImagePreviewNumber,0,0,zoomLevel*spriteInEditor->width,zoomLevel*spriteInEditor->height,NULL );
                        if( userInput->check_keyboard_down(kb_ctrl) && userInput->check_keyboard_released(kb_i) )
                        {
                            record_error("Rendering ["+int_to_string(subImagePreviewNumber)+"] frame...");
                        }
                    }
                    else
                    {
                        if( userInput->check_keyboard_down(kb_ctrl) && userInput->check_keyboard_released(kb_i) )
                        {
                            record_error("Rendering default frame...");
                        }
                        spriteInEditor->spriteTexture->render_tex_resized(cRender,0,0, zoomLevel*spriteInEditor->width,zoomLevel*spriteInEditor->height,&spriteInEditor->spriteImages[0],NULL);
                    }
                }
                if( showCollisionShapeCheckBox->is_clicked() )
                {
                    if( spriteCollisionShapeMenu->get_selected_id()==0 )
                    {
                        int sprCX = spriteCollisionRectX->get_held_number();
                        int sprCY = spriteCollisionRectY->get_held_number();
                        int sprCW = spriteCollisionRectW->get_held_number();
                        int sprCH = spriteCollisionRectH->get_held_number();
                        if( sprCX >= 0 && sprCY >=0 && sprCW > 0 && sprCH > 0)
                        {
                            render_rectangle(cRender,sprCX,sprCY,sprCX+sprCW,sprCY+sprCH,c_green,false,96);
                        }
                    }
                    else if( spriteCollisionShapeMenu->get_selected_id()==1 )
                    {
                        int sprCX = spriteCollisionCircleX->get_held_number();
                        int sprCY = spriteCollisionCircleY->get_held_number();
                        int sprCR = spriteCollisionCircleR->get_held_number();
                        if( sprCX >= 0 && sprCY >=0 && sprCR > 0)
                        {
                            render_circle_filled_rgba(cRender,sprCX+sprCR,sprCY+sprCR,sprCR,0,205,205,192);
                        }
                    }
                }
            }
            process_data_fields(GPE_VERSION_DOUBLE_NUMBER);
        }
        else if(forceRedraw &&  editorMode ==1)
        {
            //renders the sprite in edit
            if(spriteInEditor!=NULL)
            {
                if(spriteInEditor->spriteTexture!=NULL)
                {
                    spriteInEditor->spriteTexture->render_tex(cRender,0,0,NULL,NULL);

                    int i = 0;
                    int rectanglesRendered = 0;
                    int maxSpriteFrames = spriteDataFields[0]->get_held_number();
                    GPE_Rect * tempRect = NULL;
                    for( i = 0; i < (int)spriteInEditor->spriteImages.size(); i++)
                    {
                        if( maxSpriteFrames < 0 || ( rectanglesRendered < maxSpriteFrames) )
                        {
                            tempRect = &spriteInEditor->spriteImages[i];
                            render_rectangle(cRender,tempRect->x,tempRect->y,tempRect->x+tempRect->w,tempRect->y+tempRect->h,c_white,true);
                            rectanglesRendered++;
                        }
                    }
                }
            }
            process_data_fields(GPE_VERSION_DOUBLE_NUMBER);
        }
        cRender->reset_viewpoint();
        if(forceRedraw )
        {
            if( spritePreviewIsRendered)
            {
                render_new_text(cRender,spritePreviewCam.x,spritePreviewCam.y+spritePreviewCam.h,
                "Dimensions: "+int_to_string(spriteInEditor->spriteTexture->get_width() )+" x "
                                +int_to_string(spriteInEditor->spriteTexture->get_height() )+"px",
                                GPE_MAIN_TEMPLATE->Text_Box_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);

            }
        }
        render_horizontal_line_color(cRender,spritePreviewCam.y+spritePreviewCam.h+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING,
                               spritePreviewCam.x-GENERAL_GPE_PADDING,viewedSpace->x+viewedSpace->w,GPE_MAIN_TEMPLATE->Text_Box_Color );

        render_vertical_line_color(cRender,spritePreviewCam.x-GENERAL_GPE_PADDING,
                               viewedSpace->y,viewedSpace->y+viewedSpace->h,GPE_MAIN_TEMPLATE->Text_Box_Color );
        if( editorMode==0)
        {
            render_rect(cRender,&spritePreviewCam,GPE_MAIN_TEMPLATE->Button_Box_Color,true);
            render_new_text(cRender,spritePreviewCam.x,spritePreviewCam.y+spritePreviewCam.h+GENERAL_GPE_PADDING,
                "Zoom Level: "+double_to_string(zoomLevel*100 )+"%",
                                GPE_MAIN_TEMPLATE->Text_Box_Font_Color,DEFAULT_FONT,FA_LEFT,FA_TOP);
        }
        cRender->set_viewpoint( viewedSpace );
        editorPaneList->render_self(cRender,viewedSpace,cam, forceRedraw || editorMode==0);

    }
}

void spriteResource::save_resource(std::string alternatePath, int backupId)
{
    displayMessageTitle = "Saving Game Sprite";
    displayMessageSubtitle = resourceName;
    displayMessageString = "...";
    display_user_messaage();
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
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/sprites/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    Game Pencil Engine Project Game Sprite DataFile \n";
            newSaveDataFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newSaveDataFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newSaveDataFile << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            newSaveDataFile << "ResourceName=" << resourceName << "\n";

            if( spriteInEditor!=NULL)
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
                std::string resFileLocation = getShortFileName (spriteInEditor->fileName,true );
                newSaveDataFile << "ImageLocation="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                {
                    std::string resFileCopySrc = fileToDir(parentProjectName)+"/gpe_project/resources/sprites/"+resFileLocation;
                    std::string resFileCopyDest = soughtDir+resFileLocation;
                    if( file_exists(resFileCopyDest) )
                    {
                        if( display_get_prompt("[WARNING]Sprite Image File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] Sprite Image File? This action is irreversible!")==DISPLAY_QUERY_YES)
                        {
                            copy_file(resFileCopySrc,resFileCopyDest);
                        }
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

void spriteResource::update_box(int newX, int newY, int newW, int newH)
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

bool spriteResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        displayMessageTitle = "Saving Sprite Resource";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
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

