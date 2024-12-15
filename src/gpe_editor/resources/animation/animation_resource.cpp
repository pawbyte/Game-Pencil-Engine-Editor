/*
animation_resource.cpp
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

#include "animation_resource.h"
#include "../../editor/gpe_editor_settings.h"

std::string animation2d_LABELS[ANIMATION_DATA_FIELD_COUNT];

colision_area_resource::colision_area_resource()
{
    pawgui::widget_input_number *  current_field = nullptr;
    for( int i = 0; i < collision_box_fields;  i++ )
    {
        current_field = new pawgui::widget_input_number("",true, 0 );
        animation_collision_property[i] = current_field;
    }

    //Adds the default values for the collision box( Updated as of 1.6.0 LTS)
    animation_collision_property[0]->set_label("X-Pos");
    animation_collision_property[0]->set_string("0");
    animation_collision_property[0]->scale_width(0.5);

    animation_collision_property[1]->set_label("Y-Pos");
    animation_collision_property[1]->set_string("0");
    animation_collision_property[1]->scale_width(0.5);

    animation_collision_property[2]->set_label("Width");
    animation_collision_property[2]->set_string("0");
    animation_collision_property[2]->scale_width(0.5);


    animation_collision_property[3]->set_label("Height");
    animation_collision_property[3]->set_string("0");
    animation_collision_property[3]->scale_width(0.5);

    animation_collision_property[4]->set_label("Radius");
    animation_collision_property[4]->set_string("0");
}

colision_area_resource::~colision_area_resource()
{
    pawgui::widget_input_number *  current_field = nullptr;
    for( int i = collision_box_fields-1; i>=0;  i--)
    {
        current_field = animation_collision_property[i];
        if( current_field !=nullptr)
        {
            delete current_field;
            current_field = nullptr;
            animation_collision_property[i] = nullptr;
        }
    }
}

animationResource::animationResource(pawgui::widget_resource_container * pFolder)
{
    for( int i_quad = 0; i_quad < 4; i_quad ++ )
    {
        quad_points[i_quad].x = 0;
        quad_points[i_quad].y = 0;
    }
    quad_place_position = 0;

    autoAnimationPos = 0;
    animationTrackBar = new pawgui::widget_slide_xaxis();

    animatonPreviewRect = new gpe::shape_rect(0,0,0,0);
    animCameraRect.x = animRect.x = 0;
    animCameraRect.y = animRect.y = 0;
    animCameraRect.w = animRect.w = 0;
    animCameraRect.h = animRect.h = 0;
    areaIsScrollable = false;
    animationPreviewCam = new gpe::shape_rect();
    animationSpeed = 1;
    editorZoomValue = 1;
    minZoomValue = 0.0625;
    maxZoomValue = 16;

    projectParentFolder = pFolder;

    imageUsesColorKey = new pawgui::widget_checkbox("Image Transparent?","Image uses a color key?", false );
    imageColorKey = new pawgui::gpe_widget_color_picker("Image Color Key","The RGB value of image's color key",255,0,255 );

    animInEditor =  gpe::rph->get_default_render_package()->create_animation2d( "","" );
    animationSpeedLabel = new pawgui::widget_label_text ("Preview Speed","Preview Speed");
    labelanimationDimensions = new pawgui::widget_label_text ("animation Dimensions");
    previewZoomLevel = new pawgui::widget_checkbox("Preview Zoom Level","Preview Zoom Level", true);
    previewSubImageNumbers = new pawgui::widget_checkbox("Sub-images","Sub-images", true);

    labelFrameInfo = new pawgui::widget_label_text ("Frame Info");
    labelanimationMessage = new pawgui::widget_label_text ("animation Message");


    //animation Collision Area
    preloadCheckBox = new pawgui::widget_checkbox("Preload animation","Check to load animation at game open", true);
    showCollisionShapeCheckBox = new pawgui::widget_checkbox("Preview Collision Shape","", true);
    showAnimationCheckBox = new pawgui::widget_checkbox("Preview animation Animation","", true);


    animationAlignmentMenu = new pawgui::widget_dropdown_menu( "Animation Alignment",true);

    animationAlignmentMenu->add_menu_option("Top-Left","topleft",gpe::dir_top_left,true);
    animationAlignmentMenu->add_menu_option("Top-Center","topcenter",gpe::dir_top_center,false);
    animationAlignmentMenu->add_menu_option("Top-Right","topright",gpe::dir_top_right,false);

    animationAlignmentMenu->add_menu_option("Middle-Left","middleleft",gpe::dir_middle_left,false);
    animationAlignmentMenu->add_menu_option("Middle-Center","middlecenter",gpe::dir_middle_center,false);
    animationAlignmentMenu->add_menu_option("Middle-Right","middleright",gpe::dir_middle_right,false);

    animationAlignmentMenu->add_menu_option("Bottom-Left","bottomleft",gpe::dir_bottom_left,false);
    animationAlignmentMenu->add_menu_option("Bottom-Center","bottomcenter",gpe::dir_bottom_center,false);
    animationAlignmentMenu->add_menu_option("Bottom-Right","bottomright",gpe::dir_bottom_right,false);


    animationCollisionShapeMenu = new pawgui::widget_dropdown_menu( "Collision Shape",true);
    animationCollisionShapeMenu->add_menu_option("Rectangle","rectangle",0,true);
    animationCollisionShapeMenu->add_menu_option("Circle","circle",1,false);
    //animationCollisionShapeMenu->add_menu_option("Triangle [ Coming Soon]");
    //animationCollisionShapeMenu->add_menu_option("Diamond [ Coming Soon]");
    //animationCollisionShapeMenu->add_menu_option("Hexagon [ Coming Soon]");

    animationCenterCollision_button = new pawgui::widget_button_label( "Center Collision Shape","Centers the collision box");

    colision_area_resource * default_collision_box = new colision_area_resource();
    collision_areas.push_back(  default_collision_box );
    int i = 0;

    animationDataFields[i] = new pawgui::widget_input_number("");
    //pawgui::padding_default*2+animationDataLabelWidth,pawgui::padding_default+pawgui::default_line_height_padded*(i+1),64,pawgui::default_line_height,"0",false,INT_MIN,INT_MAX);
    animationDataFields[i]->set_string("0");
    animationDataFields[i]->set_label(animation2d_LABELS[i]);

    for( i =1; i < ANIMATION_DATA_FIELD_COUNT; i++)
    {
        animationDataFields[i] = new pawgui::widget_input_number("");
        //pawgui::padding_default*2+animationDataLabelWidth,pawgui::padding_default+pawgui::default_line_height_padded*(i+1),64,pawgui::default_line_height,"0",true,0,512);
        animationDataFields[i]->set_string("0");
        animationDataFields[i]->set_label(animation2d_LABELS[i]);
    }
    editResource_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/edit.png","Edit animation Data",-1);
    playPauseResource_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Stop",-1);
    subImagePreviewNumber = 0;
    subImageMiniAnimationNumber = 0;
    resourcePostProcessed = false;

    //Subimage
    previousSubImage_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-left.png","Previous");
    //previousSubImage_button->set_animation(mainMenuanimation, 36 );
    nextSubImage_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-right.png","Next");
    //nextSubImage_button->set_animation(mainMenuanimation, 38 );
    subImageNumberField = new pawgui::widget_input_number("");
    subImageNumberField->set_string("0");
    subImageNumberField->set_width(64);
    subImageEditorPreviewId = new pawgui::widget_input_number("");
    subImageEditorPreviewId->set_string("0");
    subImageEditorPreviewId->set_label("Editor Preview Frame");
    openExternalEditor_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens animation Image In External Editor");
    refreshResourceData_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded animation image");
    labelInfoMaxTextureSize = new pawgui::widget_label_text ("Max Image Size: 8192 X 8192px","Max Image Size: 8192 X 8192px");

    animXScroll = new pawgui::widget_scrollbar_xaxis();
    animYScroll = new pawgui::widget_scrollbar_yaxis();
}

animationResource::~animationResource()
{
    if( animatonPreviewRect!=nullptr )
    {
        delete animatonPreviewRect;
        animatonPreviewRect = nullptr;
    }
    if( animationTrackBar!=nullptr )
    {
        delete animationTrackBar;
        animationTrackBar = nullptr;
    }
    if( previewZoomLevel!=nullptr )
    {
        delete previewZoomLevel;
        previewZoomLevel = nullptr;
    }
    if( previewSubImageNumbers!=nullptr )
    {
        delete previewSubImageNumbers;
        previewSubImageNumbers = nullptr;
    }
    if( animationPreviewCam!=nullptr )
    {
        delete animationPreviewCam;
        animationPreviewCam = nullptr;
    }
    if( animationSpeedLabel!=nullptr )
    {
        delete animationSpeedLabel;
        animationSpeedLabel = nullptr;
    }
    if( labelInfoMaxTextureSize!=nullptr )
    {
        delete labelInfoMaxTextureSize;
        labelInfoMaxTextureSize = nullptr;
    }
    if( openExternalEditor_button!=nullptr )
    {
        delete openExternalEditor_button;
        openExternalEditor_button = nullptr;
    }
    if( refreshResourceData_button!=nullptr )
    {
        delete refreshResourceData_button;
        refreshResourceData_button = nullptr;
    }
    if( animInEditor!=nullptr )
    {
        delete animInEditor;
        animInEditor = nullptr;
    }
    pawgui::widget_input_number *  tempDataField = nullptr;
    for( int i =ANIMATION_DATA_FIELD_COUNT-1; i >=0; i--)
    {
        tempDataField = animationDataFields[i];
        if( tempDataField!=nullptr )
        {
            delete tempDataField;
            tempDataField = nullptr;
        }
        animationDataFields[i] = nullptr;
    }

    if( editResource_button!=nullptr)
    {
        delete editResource_button;
        editResource_button = nullptr;
    }
    if( playPauseResource_button!=nullptr)
    {
        delete playPauseResource_button;
        playPauseResource_button = nullptr;
    }

    if( preloadCheckBox!=nullptr)
    {
        delete preloadCheckBox;
        preloadCheckBox = nullptr;
    }
    if( showAnimationCheckBox!=nullptr)
    {
        delete showAnimationCheckBox;
        showAnimationCheckBox = nullptr;
    }
    if( showCollisionShapeCheckBox!=nullptr)
    {
        delete showCollisionShapeCheckBox;
        showCollisionShapeCheckBox = nullptr;
    }


    if( animationAlignmentMenu!=nullptr)
    {
        delete animationAlignmentMenu;
        animationAlignmentMenu = nullptr;
    }
    if( animationCollisionShapeMenu!=nullptr)
    {
        delete animationCollisionShapeMenu;
        animationCollisionShapeMenu = nullptr;
    }
    if( animationCenterCollision_button!=nullptr)
    {
        delete animationCenterCollision_button;
        animationCenterCollision_button = nullptr;
    }

    colision_area_resource * current_collison_area = nullptr;
    for( int iCollisionBoxes = (int)collision_areas.size() -1; iCollisionBoxes >=0; iCollisionBoxes-- )
    {
        current_collison_area = collision_areas[iCollisionBoxes];
        delete current_collison_area;
        current_collison_area = nullptr;
    }
    collision_areas.clear();

    if( previousSubImage_button!=nullptr)
    {
        delete previousSubImage_button;
        previousSubImage_button = nullptr;
    }

    if( nextSubImage_button!=nullptr)
    {
        delete nextSubImage_button;
        nextSubImage_button = nullptr;
    }

    if( subImageNumberField!=nullptr)
    {
        delete subImageNumberField;
        subImageNumberField = nullptr;
    }
    if( subImageEditorPreviewId!=nullptr)
    {
        delete subImageEditorPreviewId;
        subImageEditorPreviewId = nullptr;
    }

    if( labelanimationDimensions!=nullptr)
    {
        delete labelanimationDimensions;
        labelanimationDimensions = nullptr;
    }

    if( labelFrameInfo!=nullptr)
    {
        delete labelFrameInfo;
        labelFrameInfo = nullptr;
    }
    if( labelanimationMessage!=nullptr)
    {
        delete labelanimationMessage;
        labelanimationMessage = nullptr;
    }

}


bool animationResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
   if( fileTarget!=nullptr && fileTarget->is_open() )
   {
        std::string nestedTabsStr = pawgui::generate_tabs( leftTabAmount  );
        std::string nestedTabsStrObjFunc = pawgui::generate_tabs( leftTabAmount +1 );
        std::string cppAnimationName = get_name();

        if( is_build_ready() )
        {
            *fileTarget << nestedTabsStr << "settings->animationsInstructions->add_keypair( new  key_pair( " + stg_ex::int_to_string (exportBuildGlobalId )+ "," << cppAnimationName+"-"+ stg_ex::get_short_filename (animInEditor->get_file_name(),true ) << ", \"";
            *fileTarget << stg_ex::int_to_string ( (int)animInEditor->get_frame_count() )  +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_frame_xoffset() ) +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_frame_yoffset() ) +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_width() ) +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_height() ) +",";
            *fileTarget << stg_ex::int_to_string (animationCollisionShapeMenu->get_selected_id() ) +",\n";

            colision_area_resource * c_box_area = nullptr;
            int j_col_property = 0;
            //Loops through every stored frame and store it's collision frame data
            for( int i_col_box = 0; i_col_box < (int)collision_areas.size(); i_col_box++ )
            {
                c_box_area = collision_areas[i_col_box];
                *fileTarget << "CollisionInfo[" << stg_ex::int_to_string (i_col_box ) +"]=";
                for( j_col_property = 0; j_col_property <= collision_area_r; j_col_property++ )
                {
                    *fileTarget << stg_ex::int_to_string (c_box_area->animation_collision_property[j_col_property]->get_held_number() )+",";
                }
                *fileTarget << "\n";
            }
            return true;
        }
    }
    return false;
}

void animationResource::compile_cpp()
{

}

/*bool animationResource::get_mouse_coords( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    local_mouse_x = 0;
    local_mouse_y = 0;
    if( view_space==nullptr || cam== nullptr)
    {
        return false;
    }

    if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,view_space->x,view_space->y,view_space->x+view_space->w,view_space->y+view_space->h) == false )
    {
        return false;
    }


    local_mouse_x = ( gpe::input->mouse_position_x-cam->x) - view_space->x;
    local_mouse_y = ( gpe::input->mouse_position_y-cam->y) - view_space->y;
    return true;
}
*/

int animationResource::get_preview_frame()
{
    autoAnimationPos = 0;
    if( subImageEditorPreviewId!=nullptr && animInEditor!=nullptr)
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

gpe::animation2d * animationResource::get_resource_animation()
{
    if( animInEditor!=nullptr )
    {
        return animInEditor;
    }
    return nullptr;
}

void animationResource::handle_scrolling()
{
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    if( areaIsScrollable )
    {
        if( gpe::input->check_kb_down(kb_ctrl) )
        {
            //Zoom Out
            if( gpe::input->mouse_scrolling_down && editorZoomValue >= minZoomValue*2 )
            {
                editorZoomValue-=minZoomValue;
            }
            else if( gpe::input->mouse_scrolling_up && editorZoomValue < maxZoomValue-minZoomValue)
            {
                //Zoom in
                editorZoomValue += minZoomValue;
            }
        }
        else if( gpe::input->kb_shift_pressed)
        {
            if( gpe::input->mouse_scrolling_up > 0 )
            {
                xScrollHappened = true;
                animCameraRect.x-=(animCameraRect.w/16);
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                xScrollHappened = true;
                animCameraRect.x+=(animCameraRect.w/16);
            }
        }
        else
        {
            if( gpe::input->mouse_scrolling_up )
            {
                yScrollHappened = true;
                animCameraRect.y-=(animCameraRect.h/4);
            }
            else if( gpe::input->mouse_scrolling_down)
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
        animXScroll->process_self(nullptr,nullptr );
    }
    if( yScrollHappened)
    {
        animYScroll->contextRect.y = animCameraRect.y;
        animYScroll->process_self(nullptr,nullptr );
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
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt ",  get_name() +"...");
    if( animInEditor!=nullptr && animInEditor->has_texture() )
    {
        if( animInEditor->get_width() > 0 )
        {
            save_resource();
            return animInEditor->copy_image_source(pBuildDir+"/resources/animations");
        }
    }
    return true;
}

bool animationResource::is_build_ready()
{
    recentErrorMessage = "";
    bool isBuildReady = true;
    if( animInEditor==nullptr )
    {
        recentErrorMessage = "Animation is blank!";
        return false;
    }

    for( int i = 0; i < ANIMATION_DATA_FIELD_COUNT; i++)
    {
        if( animationDataFields[i]!=nullptr)
        {
            if( animationDataFields[i]->get_held_number() < 0)
            {
                animationDataFields[i]->set_string("0");
            }
        }
        else
        {
            recentErrorMessage += "animationDataFields["+ stg_ex::int_to_string(i)+"]=nullptr;";
            isBuildReady = false;
        }
    }

    if( animInEditor->get_width() <=0 )
    {
        recentErrorMessage +="AnimationWidth=0;";
        isBuildReady = false;
    }
    if( animInEditor->get_height() <=0 )
    {
        recentErrorMessage +="AnimationHeight=0;";
        isBuildReady = false;
    }
    return isBuildReady;
}

void animationResource::load_image(std::string new_file_name, bool autoProcess)
{
    if( (int)new_file_name.size() > 3 && new_file_name!="nullptr")
    {
        if( stg_ex::file_is_image(new_file_name) && animInEditor!=nullptr )
        {
            animInEditor->load_image( new_file_name, true);
            if( animInEditor->get_texture_width() <1 || animInEditor->get_texture_width()>8192 || animInEditor->get_texture_height() <1 || animInEditor->get_texture_height()>8192 )
            {
                gpe::error_log->report("[Animation Error:] Unable to load image: " +new_file_name);
                sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","[Animation Error:] Unable to load image: " +new_file_name);
            }
            else if( autoProcess )
            {
                int j = 0;
                for( int i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
                {
                    animationDataFields[i]->set_number(0);
                }
                animationDataFields[ gpe::anim_frame_width ]->set_number( animInEditor->get_texture_width() );
                animationDataFields[ gpe::anim_frame_height ]->set_number( animInEditor->get_texture_height() );

                int radMin = std::min ( animInEditor->get_texture_width(),animInEditor->get_texture_height() )/2;

                colision_area_resource * c_box = nullptr;

                for( j = 0; j < (int)animInEditor->get_frame_count(); j ++ )
                {
                    c_box = new colision_area_resource();
                    collision_areas.push_back( c_box);
                }
                for( j = 0; j < (int)collision_areas.size(); j++)
                {
                    c_box = collision_areas[j];
                    if( c_box !=nullptr )
                    {
                        if( animationCollisionShapeMenu->get_selected_value()  == 0 )
                        {
                            c_box->animation_collision_property[collision_area_x]->set_number(0);
                            c_box->animation_collision_property[collision_area_y]->set_number(0);

                        }
                        else
                        {
                            c_box->animation_collision_property[collision_area_x]->set_number( animInEditor->get_texture_width()/2);
                            c_box->animation_collision_property[collision_area_y]->set_number( animInEditor->get_texture_height()/2 );
                        }

                        c_box->animation_collision_property[collision_area_r]->set_number(  radMin );
                        c_box->animation_collision_property[collision_area_w]->set_number( animInEditor->get_texture_width() );
                        c_box->animation_collision_property[collision_area_h]->set_number( animInEditor->get_texture_height() );
                    }
                }
            }
        }
        else
        {
            pawgui::display_user_alert("Unable to load image","File type["+  stg_ex::get_file_ext(new_file_name)+"] not supported when loading ["+new_file_name+"].");
        }
    }
}

void animationResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed && sff_ex::file_exists(file_path))
    {
        return;
    }
    gpe::error_log->report("[animationResource] loading" + file_path + "file...");
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Loading Animation", resource_name );
    }

    std::string otherColContainerName = "";

    std::string newFileIn = "";
    std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/animations/";
    if( sff_ex::file_exists(file_path) )
    {
        newFileIn = file_path;
        soughtDir = stg_ex::get_path_from_file(newFileIn);
    }
    else
    {

        newFileIn = soughtDir + resource_name+".gpf";
        //Fallsback to previous folder for older versions
        if( sff_ex::file_exists(newFileIn) == false )
        {
            soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/animations/";
            newFileIn = soughtDir + resource_name+".gpf";
        }
    }
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    //error_log->report("Loading animation - "+newFileIn);
    //If the level file could be loaded
    float found_file_version = -1;
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
    std::string key_string="";
    std::string valstring="";
    std::string subValstring="";
    std::string currLine="";
    std::string currLineToBeProcessed;
    std::string sprDataStr;

    int i = 0, j = 0, k = 0;

    bool one_collision_version = true;

    int temp_col_x = 0, temp_col_y = 0, temp_col_w = 0, temp_col_h = 0, temp_col_r = 0, temp_col_shape = 0 ;
    while ( gameResourceFileIn.good() )
    {
        getline (gameResourceFileIn,currLine); //gets the next line of the file
        currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
        currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

        if( found_file_version <=0)
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
                            found_file_version = stg_ex::string_to_float(valstring);

                            if( found_file_version < GPE_ANIM_CBOX_VERSION )
                            {
                                one_collision_version = true;
                            }
                            else
                            {
                                one_collision_version = false;
                            }
                        }
                    }
                }
            }
        }
        else if( found_file_version <= 2)
        {
            //Begin processing the file.
            if(!currLineToBeProcessed.empty() )
            {
                equalPos=currLineToBeProcessed.find_first_of("=");
                if(equalPos!=(int)std::string::npos)
                {
                    //if the equalPos is present, then parse on through and carryon
                    key_string = currLineToBeProcessed.substr(0,equalPos);
                    key_string= stg_ex::string_lower( key_string );
                    valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                    if( key_string=="resourcename")
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
                    else if( key_string=="imagelocation")
                    {
                        if( valstring!="nullptr")
                        {
                            load_image( soughtDir+valstring );
                        }
                    }
                    else if( key_string=="preload")
                    {
                        preloadCheckBox->set_checked( stg_ex::string_to_bool(valstring) );
                    }
                    else if( key_string=="editorpreviewFrame")
                    {
                        subImageEditorPreviewId->set_string( valstring );
                    }
                    else if( key_string=="previewcollisionshape")
                    {
                        if( showCollisionShapeCheckBox!=nullptr)
                        {
                            showCollisionShapeCheckBox->set_checked( stg_ex::string_to_bool(valstring) );
                        }
                    }
                    else if( key_string=="collisionshape" )
                    {
                        temp_col_shape = stg_ex::string_to_int(valstring);
                        if( temp_col_shape < 0 || temp_col_shape > 1 )
                        {
                            temp_col_shape = 0;
                        }
                        animationCollisionShapeMenu->set_option_value( temp_col_shape );
                    }
                    else if( key_string == "circlecollisionregion")
                    {
                        //We will attempt to process older formats from 1.6x or earlier

                        temp_col_x = stg_ex::split_first_int(valstring,',');
                        temp_col_y = stg_ex::split_first_int(valstring,',');
                        temp_col_r = stg_ex::string_to_int(valstring,0);
                        temp_col_w = temp_col_h = temp_col_r;

                        for( i = 0; i < animInEditor->get_frame_count(); i ++)
                        {
                            animInEditor->edit_collision_circle(i, temp_col_x, temp_col_y, temp_col_r);
                        }
                    }
                    else if( key_string=="rectanglecollisionbox")
                    {
                        //We will attempt to process older formats from 1.6x or earlier
                        temp_col_x = stg_ex::split_first_int(valstring,',');
                        temp_col_y = stg_ex::split_first_int(valstring,',');
                        temp_col_w = stg_ex::split_first_int(valstring,',');
                        temp_col_h = stg_ex::string_to_int(valstring,0);
                        temp_col_w = std::min( temp_col_w, temp_col_h);
                        for( i = 0; i < animInEditor->get_frame_count(); i ++)
                        {
                            animInEditor->edit_collision_box(i, temp_col_x, temp_col_y, temp_col_w, temp_col_h );
                        }
                    }
                    else if( key_string=="subimageinview")
                    {
                        subImagePreviewNumber = stg_ex::string_to_int(valstring);
                        subImageNumberField->set_string( stg_ex::int_to_string(subImagePreviewNumber) );
                    }
                    else
                    {
                        if( semath::compare_floats(found_file_version, GPE_ANIM_FCOUNT_VERSION) || found_file_version > GPE_ANIM_FCOUNT_VERSION )
                        {
                            //error_log->report("Proper animation system used ["+resource_name+"]");
                            for( i = 0; i <ANIMATION_DATA_FIELD_COUNT ; i++)
                            {
                                sprDataStr = "animationdata["+ stg_ex::int_to_string(i)+"]";
                                if( key_string==sprDataStr)
                                {
                                    animationDataFields[i]->set_string( valstring );
                                }
                                else
                                {
                                    sprDataStr = "spritedata["+ stg_ex::int_to_string(i)+"]";
                                    if( key_string==sprDataStr)
                                    {
                                        animationDataFields[i]->set_string( valstring );

                                    }
                                }
                            }
                        }
                        else
                        {
                            //error_log->report("Alternate animation system used ["+resource_name+"]");
                            animationDataFields[0]->set_string( "-1");
                            for( i = 0; i <ANIMATION_DATA_FIELD_COUNT-1 ; i++)
                            {
                                sprDataStr = "animationdata["+ stg_ex::int_to_string(i)+"]";
                                if( key_string==sprDataStr)
                                {
                                    animationDataFields[i+1]->set_string( valstring);
                                }
                                else
                                {
                                    sprDataStr = "spritedata["+ stg_ex::int_to_string(i)+"]";
                                    if( key_string==sprDataStr)
                                    {
                                        animationDataFields[i+1]->set_string( valstring);
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
            gpe::error_log->report("Invalid found_file_version ="+ stg_ex::float_to_string(found_file_version)+".");
        }
    }


    if( found_file_version > 0 )
    {
        resourcePostProcessed= true;
    }

    process_data_fields(found_file_version);
    gpe::error_log->report("[animationResource] loaded" + file_path + "successfully...");

}

void animationResource::process_collision_box(int frame_id)
{
    if( animInEditor== nullptr || frame_id < 0 || frame_id >= animInEditor->get_frame_count() )
    {
        return;
    }
    if( frame_id >= (int)collision_areas.size() )
    {
        return;
    }
    colision_area_resource * t_col_box = collision_areas[frame_id];
    if( animationCollisionShapeMenu->get_selected_value()==1)
    {
        int colX = t_col_box->animation_collision_property[collision_area_x]->get_held_number();
        int colY = t_col_box->animation_collision_property[collision_area_y]->get_held_number();
        int colR = t_col_box->animation_collision_property[collision_area_r]->get_held_number();
        animInEditor->edit_collision_circle(frame_id,colX, colY, colR );
    }
    else
    {
        int colX = t_col_box->animation_collision_property[collision_area_x]->get_held_number();
        int colY = t_col_box->animation_collision_property[collision_area_y]->get_held_number();
        int colW = t_col_box->animation_collision_property[collision_area_w]->get_held_number();
        int colH = t_col_box->animation_collision_property[collision_area_h]->get_held_number();
        animInEditor->edit_collision_box(frame_id,colX, colY, colW,  colH );
    }
}

void animationResource::process_data_fields(float versionToProcess)
{
    if( animInEditor!=nullptr )
    {
        bool animationIsReadyForEditing = true;

        int i = 0;
        int j = 0;
        while( i < ANIMATION_DATA_FIELD_COUNT )
        {
            if( animationDataFields[i]!=nullptr)
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
            //error_log->report("Beginning to Process animation Data...");
            int fWidth = animationDataFields[ gpe::anim_frame_width]->make_valid_number(0);
            int fHeight = animationDataFields[ gpe::anim_frame_height]->make_valid_number(0);

            int fPixelOffsetH = animationDataFields[ gpe::anim_pixel_offset_hori]->make_valid_number(0);
            int fPixelOffsetV = animationDataFields[ gpe::anim_pixel_offset_vert]->make_valid_number(0);

            int hPadding = animationDataFields[ gpe::anim_pixel_padding_hori]->make_valid_number(0);

            int vPadding = animationDataFields[ gpe::anim_pixel_padding_vert]->make_valid_number(0);

            if( fWidth > 0 && fHeight > 0 && hPadding>=0 && vPadding >=0)
            {
                int startX = fPixelOffsetH;
                int startY = fPixelOffsetV;
                int maxanimationFrames = animationDataFields[ gpe::anim_frame_count]->make_valid_number(-1);

                if( versionToProcess > 0 && ( semath::compare_floats(versionToProcess,GPE_ANIM_FCOUNT_VERSION) || versionToProcess > GPE_ANIM_FCOUNT_VERSION  ) && maxanimationFrames > 0)
                {
                    animInEditor->setup_animation( maxanimationFrames,fWidth, fHeight, fPixelOffsetH, fPixelOffsetV, hPadding, vPadding, animInEditor->get_alignment() );
                }
                else
                {
                    animInEditor->setup_fullimg_animation(fWidth, fHeight, fPixelOffsetH, fPixelOffsetV, hPadding, vPadding, animInEditor->get_alignment() );
                    animationDataFields[ gpe::anim_frame_count]->set_number( animInEditor->get_frame_count() );
                }
            }
        }
        process_collision_box(0);

        colision_area_resource * c_box = nullptr;
        if( semath::compare_floats(versionToProcess, GPE_ANIM_CBOX_VERSION) || versionToProcess < GPE_ANIM_CBOX_VERSION )
        {

            for( int i_box = 1; i_box < (int)animInEditor->frame_data.size(); i_box++)
            {
                animInEditor->frame_data[i_box]->collision_box->x = animInEditor->frame_data[0]->collision_box->x;
                animInEditor->frame_data[i_box]->collision_box->y = animInEditor->frame_data[0]->collision_box->y;
                animInEditor->frame_data[i_box]->collision_box->w = animInEditor->frame_data[0]->collision_box->w;
                animInEditor->frame_data[i_box]->collision_box->h = animInEditor->frame_data[0]->collision_box->h;


                c_box = new colision_area_resource();
                c_box->animation_collision_property[collision_area_x]->set_number(animInEditor->frame_data[0]->collision_box->x );
                c_box->animation_collision_property[collision_area_y]->set_number(animInEditor->frame_data[0]->collision_box->y );
                c_box->animation_collision_property[collision_area_w]->set_number(animInEditor->frame_data[0]->collision_box->w);
                c_box->animation_collision_property[collision_area_h]->set_number(animInEditor->frame_data[0]->collision_box->h );
                c_box->animation_collision_property[collision_area_r]->set_number(animInEditor->frame_data[0]->collision_radius );
                collision_areas.push_back( c_box);
            }
        }

    }
}

void animationResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
    if( editResource_button!=nullptr)
    {
        editResource_button->prerender_self();
    }

    if( preloadCheckBox!=nullptr)
    {
        preloadCheckBox->prerender_self();
    }
    if( showAnimationCheckBox!=nullptr)
    {
        showAnimationCheckBox->prerender_self();
    }
    if( animationCenterCollision_button!=nullptr)
    {
        animationCenterCollision_button->prerender_self();
    }
}

void animationResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    int i;

    if( cam == nullptr|| view_space == nullptr )
    {
        return;
    }
    if( openExternalEditor_button!=nullptr )
        openExternalEditor_button->set_clicked( false );

    bool editorHasControl = false;

    animRect.x = 0;
    animRect.y = 0;
    animRect.w = 0;
    animRect.h = 0;

    if( animInEditor!=nullptr && animInEditor->has_texture() )
    {
        animRect.w = animInEditor->get_texture_width();
        animRect.h = animInEditor->get_texture_height();
    }

    colision_area_resource *  col_area = nullptr;
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
    animationPreviewCam->w = view_space->w;
    animationPreviewCam->h = view_space->h;

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
    animXScroll->set_coords( 0,view_space->h - animXScroll->get_height() );
    animXScroll->set_width( view_space->w);
    update_rectangle(&animXScroll->fullRect,0,0,(float)animRect.w, (float)animRect.h );
    update_rectangle(&animXScroll->contextRect,(float)animCameraRect.x,(float)animCameraRect.y, (float)animCameraRect.w, (float)animCameraRect.h );
    animXScroll->process_self(view_space,cam );
    //if( animXScroll->has_moved() )
    {
        animCameraRect.x = (float)(animXScroll->contextRect.x);
    }

    //Vertical Scrolling
    animYScroll->set_coords( view_space->w -animYScroll->get_width(),0);
    animYScroll->set_height( view_space->h );
    update_rectangle(&animYScroll->fullRect,0,0,(float)animRect.w, (float)animRect.h );
    update_rectangle(&animYScroll->contextRect,(float)animCameraRect.x,(float)animCameraRect.y, (float)animCameraRect.w, (float)animCameraRect.h );
    //animYScroll->contextRect.h = sceneEditorView.h;
    animYScroll->process_self(view_space,cam );
    //if( animYScroll->has_moved() )
    {
        animCameraRect.y = float(animYScroll->contextRect.y);
    }

    if( animInEditor!=nullptr)
    {
        if( animInEditor->has_texture() )
        {
            labelanimationDimensions->set_name("Image Dimensions:"+ stg_ex::int_to_string( animInEditor->get_texture_width() )+" x "+ stg_ex::int_to_string( animInEditor->get_texture_height() )+"px");
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


    if( panel_main_editor==nullptr )
    {
        return;
    }

    //Click a point in editor zone and then add it to the quad
    if( get_mouse_coords( view_space, cam ) )
    {
        if( gpe::input->check_mouse_released( mb_left) )
        {
            //The fun sprite/animation test zone
            if( quad_place_position < 0 )
            {
                quad_place_position = 0;
            }
            else if ( quad_place_position > 3 )
            {
                quad_place_position = 0;
            }

            quad_points[quad_place_position].x = local_mouse_x;
            quad_points[quad_place_position].y = local_mouse_y;

            quad_place_position++;

            if( quad_place_position < 0 )
            {
                quad_place_position = 0;
            }
            else if ( quad_place_position > 3 )
            {
                quad_place_position = 0;
            }
        }
    }

    pawgui::widget_dock_panel * collisionAndPreviewDataPanel = panel_main_editor;
    if( panel_inspector != nullptr)
    {
        panel_inspector->clear_panel();
        collisionAndPreviewDataPanel = panel_inspector;
    }

    if( panel_main_editor!=nullptr )
    {
        editorHasControl = panel_main_editor->hasScrollControl || collisionAndPreviewDataPanel->hasScrollControl;
        panel_main_editor->add_gui_element( renameBox,true);
        panel_main_editor->add_gui_element( loadResource_button,false );
        panel_main_editor->add_gui_element( refreshResourceData_button,false );
        panel_main_editor->add_gui_element( editResource_button,false);
        panel_main_editor->add_gui_element( openExternalEditor_button,true);
        panel_main_editor->add_gui_element( previewSubImageNumbers,true);

        panel_main_editor->add_gui_element(labelInfoMaxTextureSize,true);
        panel_main_editor->add_gui_element( labelanimationDimensions,true);


        panel_main_editor->add_gui_element( animationAlignmentMenu,true);

        panel_main_editor->add_gui_element(imageUsesColorKey,true);
        panel_main_editor->add_gui_element(imageColorKey,true);
        panel_main_editor->add_gui_element(imageColorKey,true);

        for(  i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
        {
            if( animationDataFields[i]!=nullptr)
            {
                panel_main_editor->add_gui_element(animationDataFields[i],true );
                        //animationDataFields[i]->set_width(panel_main_editor->get_width()-pawgui::padding_default*3);
            }
        }
    }
    else
    {
        editorHasControl = collisionAndPreviewDataPanel->hasScrollControl;
    }
    //Collision and animation preview stuff
    collisionAndPreviewDataPanel->add_gui_element( previousSubImage_button,false);
    collisionAndPreviewDataPanel->add_gui_element( subImageNumberField,false);
    collisionAndPreviewDataPanel->add_gui_element( nextSubImage_button,false);
    collisionAndPreviewDataPanel->add_gui_element(playPauseResource_button,true);
    collisionAndPreviewDataPanel->add_gui_element( subImageEditorPreviewId,true);

    labelFrameInfo->set_name("Animation Size: "+ stg_ex::int_to_string(animInEditor->get_width() )+" x "+ stg_ex::int_to_string(animInEditor->get_height() )+"px");
    collisionAndPreviewDataPanel->add_gui_element( labelFrameInfo,true);
    collisionAndPreviewDataPanel->add_gui_element( animationCollisionShapeMenu,true);
    collisionAndPreviewDataPanel->add_gui_element( animationCenterCollision_button,true);

    //Rectangle collision box

    if( subImagePreviewNumber >= (int)collision_areas.size() )
    {
        subImageEditorPreviewId = 0;
    }
    if( subImagePreviewNumber >=0 && subImagePreviewNumber < (int)collision_areas.size() )
    {
        col_area = collision_areas[subImagePreviewNumber];
        if( animationCollisionShapeMenu->get_selected_value()==0 )
        {
            collisionAndPreviewDataPanel->add_gui_element( col_area->animation_collision_property[collision_area_x],false);
            collisionAndPreviewDataPanel->add_gui_element( col_area->animation_collision_property[collision_area_y],true);
            collisionAndPreviewDataPanel->add_gui_element( col_area->animation_collision_property[collision_area_w],false);
            collisionAndPreviewDataPanel->add_gui_element( col_area->animation_collision_property[collision_area_h],true);
        }
        else
        {
            //Circle collision box
            col_area->animation_collision_property[collision_area_x]->set_width( (collisionAndPreviewDataPanel->get_width()-pawgui::padding_default*3 )/3 );
            col_area->animation_collision_property[collision_area_y]->set_width( col_area->animation_collision_property[collision_area_x]->get_width( ) );
            col_area->animation_collision_property[collision_area_h]->set_width(col_area->animation_collision_property[collision_area_x]->get_width( ) );

            collisionAndPreviewDataPanel->add_gui_element( col_area->animation_collision_property[collision_area_x],false);
            collisionAndPreviewDataPanel->add_gui_element( col_area->animation_collision_property[collision_area_y],false);
            collisionAndPreviewDataPanel->add_gui_element( col_area->animation_collision_property[collision_area_r],true);
        }
    }
    collisionAndPreviewDataPanel->add_gui_element( showCollisionShapeCheckBox,true);

    if( panel_inspector != nullptr)
    {
        panel_inspector->process_self( nullptr, nullptr );
    }

    if( panel_main_editor!=nullptr )
    {
        panel_main_editor->add_gui_element(preloadCheckBox,true);
        panel_main_editor->add_gui_element(confirmResource_button,true);

        panel_main_editor->add_gui_element(cancelResource_button,true);
        //panel_main_editor->set_maxed_out_width();
        panel_main_editor->process_self( nullptr, nullptr );

        if( loadResource_button!=nullptr && loadResource_button->is_clicked() )
        {
            std::string new_file_name = pawgui::get_filename_open_from_popup("Load Your animation Image","Image",pawgui::main_settings->fileOpenanimationDir);
            if( (int)new_file_name.size() > 3 && sff_ex::file_exists(new_file_name) )
            {
                bool autoProcessData = false;
                if( animationDataFields[0]!=nullptr)
                {
                    if( animationDataFields[0]->get_held_number()==0)
                    {
                        autoProcessData = true;
                    }
                }
                load_image(new_file_name, autoProcessData);
                gpe::input->reset_all_input();
                return;
            }
        }
        else if( confirmResource_button->is_clicked() )
        {
            gpe::input->reset_all_input();
            for( int i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
            {
                if( animationDataFields[i]!=nullptr)
                {
                    animationDataFields[i]->switch_inuse(false);
                }
            }

            int radMin = std::min (animationDataFields[ gpe::anim_frame_width]->get_held_number(), animationDataFields[ gpe::anim_frame_height]->get_held_number() )/2;

            col_area = nullptr;
            for( int i_col_frame = 0; i_col_frame < (int)collision_areas.size(); i_col_frame++)
            {
                col_area = collision_areas[i_col_frame];
                if( animationCollisionShapeMenu->get_selected_value()  == 0 )
                {
                    col_area->animation_collision_property[collision_area_x]->set_number(0);
                    col_area->animation_collision_property[collision_area_y]->set_number(0);
                    col_area->animation_collision_property[collision_area_w]->set_number( animationDataFields[ gpe::anim_frame_width]->get_held_number() );
                    col_area->animation_collision_property[collision_area_h]->set_number( animationDataFields[ gpe::anim_frame_height]->get_held_number() );
                }
                else
                {
                    col_area->animation_collision_property[collision_area_x]->set_number( animationDataFields[ gpe::anim_frame_width]->get_held_number()/2);
                    col_area->animation_collision_property[collision_area_r]->set_number( animationDataFields[ gpe::anim_frame_height]->get_held_number()/2 );
                    col_area->animation_collision_property[collision_area_y]->set_number(  radMin );
                }
            }



            requestedMode = 0;
            gpe::input->reset_all_input();
            save_resource();
            isPreloaded = preloadCheckBox->is_clicked();
        }
        else if( cancelResource_button->is_clicked() )
        {
            if( pawgui::display_prompt_message("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
            {
                resourcePostProcessed = false;
                gpe::input->reset_all_input();
                requestedMode = 0;
                gpe::error_log->report("Reverting to Editor Mode 0");
                revert_data_fields();
                process_data_fields( gpe::version_number_total);
                load_resource();
            }
        }

        if( editResource_button!=nullptr && editResource_button->is_clicked() )
        {
            editResource_button->set_clicked( false );
            gpe::error_log->report("Going to Editor Mode 1");
            gpe::input->reset_all_input();
            process_self(view_space,cam);
            return;
        }

        if( openExternalEditor_button!=nullptr && animInEditor!=nullptr)
        {
            if( openExternalEditor_button->is_clicked() )
            {
                if( sff_ex::file_exists(animInEditor->get_file_name() ) )
                {
                    std::string external_editor_program = "";

                    if( main_editor_settings!=nullptr && main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=nullptr)
                    {
                        external_editor_program = main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string();
                    }

                    if( (int)external_editor_program.size() > 0 )
                    {
                        gpe::external_open_program( external_editor_program, animInEditor->get_file_name(), true  );
                    }
                    else
                    {
                        gpe::external_open_program( animInEditor->get_file_name() );
                    }

                    sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+animInEditor->get_file_name()+"]...");
                }
            }
        }

        if( refreshResourceData_button!=nullptr  && refreshResourceData_button->is_clicked() )
        {
            resourcePostProcessed = false;
            load_resource();
        }

        //actual saving of the animation onto the engine and files
        if( playPauseResource_button!=nullptr &&  playPauseResource_button->is_clicked() )
        {
            if( animInEditor!=nullptr)
            {
                if( playPauseResource_button->get_name()=="Play")
                {
                    playPauseResource_button->set_name("Stop");
                    playPauseResource_button->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/stop.png" );
                }
                else
                {
                    playPauseResource_button->set_name("Play");
                    playPauseResource_button->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
                }
            }
        }
    }

    if( animationCollisionShapeMenu!=nullptr)
    {
        if( animationCenterCollision_button->is_clicked() && animInEditor!=nullptr)
        {
            int col_box_numb = 0;
            colision_area_resource *  temp_bol_box = nullptr;
            if( animationCollisionShapeMenu->get_selected_value()==0 )
            {
                for( col_box_numb = 0; col_box_numb < (int)collision_areas.size(); col_box_numb++ )
                {
                    temp_bol_box= collision_areas[col_box_numb];
                    temp_bol_box->animation_collision_property[collision_area_x]->set_string( stg_ex::int_to_string( 0 ) );
                    temp_bol_box->animation_collision_property[collision_area_y]->set_string( stg_ex::int_to_string( 0 ) );
                    temp_bol_box->animation_collision_property[collision_area_w]->set_string( stg_ex::int_to_string( animInEditor->get_width() ) );
                    temp_bol_box->animation_collision_property[collision_area_h]->set_string( stg_ex::int_to_string( animInEditor->get_height() ) );
                }
            }
            else if( animationCollisionShapeMenu->get_selected_value()==1 )
            {
                int smallestCenter = std::min( animInEditor->get_width(), animInEditor->get_height() )/2;
                for( col_box_numb = 0; col_box_numb < (int)collision_areas.size(); col_box_numb++ )
                {
                    temp_bol_box= collision_areas[col_box_numb];
                    temp_bol_box->animation_collision_property[collision_area_x]->set_string( stg_ex::int_to_string( animInEditor->get_width()/2 ) );
                    temp_bol_box->animation_collision_property[collision_area_y]->set_string( stg_ex::int_to_string( animInEditor->get_height()/2 ) );
                    temp_bol_box->animation_collision_property[collision_area_r]->set_string( stg_ex::int_to_string(smallestCenter ) );
                }
            }
        }
    }

    if( animationAlignmentMenu != nullptr )
    {
        if( animationAlignmentMenu->just_activated() )
        {
            int selected_align_value = animationAlignmentMenu->get_selected_value();
            gpe::error_log->report("[Animation DEBUG:] animationAlignmentMenu just activated");

            if( selected_align_value >=0 && selected_align_value <= gpe::dir_bottom_right )
            {
                gpe::error_log->report("[Animation DEBUG:] animationAlignmentMenu's selected_align_value = " + stg_ex::int_to_string(selected_align_value) );
                if( animInEditor!=nullptr )
                {
                    gpe::error_log->report("[Animation DEBUG:] Changing animInEditor's alignment to " + stg_ex::int_to_string(selected_align_value) );
                    animInEditor->set_alignment( selected_align_value );
                }
            }
        }
    }
    else
    {
        gpe::error_log->report("[Animation DEBUG:] animationAlignmentMenu = nullptr");
    }

    //Animates animation
    if( playPauseResource_button->get_name()=="Stop" )
    {
        animationSpeed = gpe::fps_cap/4.f;
        //animationSpeed = std::max( 1, animationSpeed );
        subImageMiniAnimationNumber += ( animationSpeed * gpe::time_keeper->get_delta_performance() )/ 100.f ;
        //error_log->report("Animating animation by "+ stg_ex::float_to_string(animationSpeed)+"..."+ stg_ex::int_to_string(random(0,100) )+"...");

        if( subImageMiniAnimationNumber >= animationSpeed )
        {
            subImagePreviewNumber++;
            subImageNumberField->set_number( subImagePreviewNumber);
            subImageMiniAnimationNumber = 0;
        }
    }

    if( previousSubImage_button!=nullptr && previousSubImage_button->is_clicked() && subImagePreviewNumber > 0)
    {
        subImagePreviewNumber--;
        subImageNumberField->set_number( subImagePreviewNumber);
        playPauseResource_button->set_name("Play");
        playPauseResource_button->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
    }

    if( subImageNumberField!=nullptr && subImageNumberField->is_inuse() )
    {
        subImagePreviewNumber = subImageNumberField->get_held_number();
        playPauseResource_button->set_name("Play");
        playPauseResource_button->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
    }

    if( nextSubImage_button!=nullptr &&  nextSubImage_button->is_clicked() )
    {
        subImagePreviewNumber++;
        subImageNumberField->set_number( subImagePreviewNumber);
        playPauseResource_button->set_name("Play");
        playPauseResource_button->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
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

    if( panel_main_editor!=nullptr)
    {
        editorHasControl = panel_main_editor->hasScrollControl || collisionAndPreviewDataPanel->hasScrollControl;
    }
    else
    {
        editorHasControl = collisionAndPreviewDataPanel->hasScrollControl;
    }
    //actual saving of the animation onto the engine and files
    if( get_mouse_coords(view_space, cam) )
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
        if( gpe::input->check_mouse_released( mb_right) )
        {
            /*
            pawgui::context_menu_open( gpe::input->mouse_position_x, gpe::input->mouse_position_y );
                pawgui::main_context_menu->add_menu_option("Change Box Color",0);
                pawgui::main_context_menu->add_menu_option("Change Number Color",1);
                pawgui::main_context_menu->add_menu_option("Return to Mode 0",2);
                int menuSelection = pawgui::context_menu_process();

                if( menuSelection == 2)
                {
                    gpe::input->reset_all_input();
                    requestedMode = 0;
                    error_log->report("Reverting to Editor Mode 0");
                    gpe::input->reset_all_input();
                    process_self(view_space,cam);
                    return;
                }
                */
        }
    }

    //if( !editorHasControl )
    if( areaIsScrollable )
    {
        if( gpe::input->check_kb_down(kb_ctrl) )
        {
            if( gpe::input->check_kb_released(kb_minus) || gpe::input->mouse_scrolling_down )
            {
                editorZoomValue -= minZoomValue;
            }
            else if( gpe::input->check_kb_released(kb_plus) || gpe::input->mouse_scrolling_up )
            {
                editorZoomValue += minZoomValue;
            }
        }
    }
}

void animationResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    bool animationPreviewIsRendered = false;
    if( cam==nullptr || view_space==nullptr )
    {
        return;
    }

    if( texture_transparent_bg!=nullptr )
    {
        int iPV = 0, jPV = 0;
        for( iPV = 0; iPV< view_space->w; iPV+=texture_transparent_bg->get_width() )
        {
            for( jPV= 0; jPV< view_space->h; jPV+=texture_transparent_bg->get_height() )
            {
                texture_transparent_bg->render_tex( iPV,jPV,nullptr);
            }
        }
    }
    else
    {
        gpe::gcanvas->render_rectangle( 0,0,view_space->w, view_space->h, pawgui::theme_main->program_color, false, 255 );
    }

    if( gpe::gcanvas->is_render_mode_supported( gpe::gcanvas->get_render_mode() ) !=1 )
    {
        //We draw the message that our current render can not draw the preview in this editor and then exit the render function :-(
        gpe::gfs->render_text( view_space->w/2,view_space->h/2,
                          "Render Mode ["+gpe::gcanvas->get_render_mode_name( gpe::gcanvas->get_render_mode())+"] is not supported by "+gpe::gcanvas->get_artist_name()+ " artist backend",
                              pawgui::theme_main->program_color_header, gpe::font_default, gpe::fa_center, gpe::fa_middle );
        return;
    }

    render_axis_2d( animationPreviewCam->x+animationPreviewCam->w/2, animationPreviewCam->y+animationPreviewCam->h/2 , true );
    if( animInEditor!=nullptr &&animInEditor->has_texture() )
    {
        if( subImagePreviewNumber < 0 || subImagePreviewNumber >= (int)animInEditor->get_frame_count() )
        {
            subImagePreviewNumber = 0;
        }

        animInEditor->render_scaled( subImagePreviewNumber, animationPreviewCam->x+animationPreviewCam->w/2,
        animationPreviewCam->y+animationPreviewCam->h/2, editorZoomValue,editorZoomValue,nullptr );


        if( showCollisionShapeCheckBox->is_clicked() && animInEditor!=nullptr )
        {
            if( animationCollisionShapeMenu->get_selected_value()==0 )
            {
                int sprCX = animationPreviewCam->x+animationPreviewCam->w/2;
                sprCX+=collision_areas[subImagePreviewNumber]->animation_collision_property[collision_area_x]->get_held_number() * editorZoomValue;
                int sprCY = animationPreviewCam->y+animationPreviewCam->h/2;
                sprCY+=collision_areas[subImagePreviewNumber]->animation_collision_property[collision_area_y]->get_held_number() * editorZoomValue;
                int sprCW = collision_areas[subImagePreviewNumber]->animation_collision_property[collision_area_w]->get_held_number() * editorZoomValue;
                int sprCH = collision_areas[subImagePreviewNumber]->animation_collision_property[collision_area_h]->get_held_number() * editorZoomValue;
                sprCX-= sprCW/2;
                sprCY-= sprCH/2;

                if( sprCX >= 0 && sprCY >=0 && sprCW > 0 && sprCH > 0)
                {
                    gpe::gcanvas->render_rectangle( sprCX,sprCY,sprCX+sprCW,sprCY+sprCH, gpe::c_green,false,96);
                }
            }
            else if( animationCollisionShapeMenu->get_selected_value()==1 )
            {
                int sprCX = animationPreviewCam->x+( animationPreviewCam->w )/2;// - (animInEditor->get_width()*editor0ZoomValue)/2;
                sprCX+=collision_areas[subImagePreviewNumber]->animation_collision_property[collision_area_x]->get_held_number()/2 * editorZoomValue;

                int sprCY = animationPreviewCam->y+( animationPreviewCam->h )/2;// - (animInEditor->get_height()*editor0ZoomValue)/2;
                sprCY+=collision_areas[subImagePreviewNumber]->animation_collision_property[collision_area_y]->get_held_number() * editorZoomValue;

                int sprCR = collision_areas[subImagePreviewNumber]->animation_collision_property[collision_area_r]->get_held_number()/2 * editorZoomValue;
                sprCX-=sprCR;
                sprCY-=sprCR;
                if( sprCX >= 0 && sprCY >=0 && sprCR > 0)
                {
                    gpe::gcanvas->render_circle_filled_color( sprCX,sprCY,sprCR, gpe::c_lime, 128 );
                }
            }
        }

        process_data_fields( gpe::version_number_total);

        //renders the animation in edit
        //renders the frame boxes for the animation
        animInEditor->render_piece_resized( subImagePreviewNumber, animationPreviewCam->x, animationPreviewCam->y, animCameraRect.w* editorZoomValue, animCameraRect.h* editorZoomValue, &animCameraRect );

        if( animInEditor->render_quad( subImagePreviewNumber, quad_points[0], quad_points[1], quad_points[2], quad_points[3], gpe::c_white, 255 ) == false )
        {
            gpe::error_log->report("Failed to draw quad of sprite...");
        }

        for( int i_quad_pos = 0; i_quad_pos < 4; i_quad_pos++ )
        {
            gpe::gfs->render_text_boxed( quad_points[i_quad_pos].x,quad_points[i_quad_pos].y,
                    stg_ex::float_to_string(i_quad_pos ),
                        gpe::c_white, gpe::c_black, gpe::font_default, gpe::fa_center, gpe::fa_middle );
        }

        int iAnimFrame = 0, jAnimRow = 0;
        int rectanglesRendered = 0;
        int maxAnimationFrames = animationDataFields[ gpe::anim_frame_count]->get_held_number();
        animatonPreviewRect->x = animationDataFields[ gpe::anim_pixel_offset_hori]->get_held_number();
        animatonPreviewRect->y = animationDataFields[ gpe::anim_pixel_offset_hori]->get_held_number();
        animatonPreviewRect->w = animationDataFields[ gpe::anim_frame_width]->get_held_number();
        animatonPreviewRect->h = animationDataFields[ gpe::anim_frame_height]->get_held_number();

        int incrementWidth = animatonPreviewRect->w  + animationDataFields[ gpe::anim_pixel_padding_hori]->get_held_number();
        int incrementHeight = animatonPreviewRect->h + animationDataFields[ gpe::anim_pixel_padding_vert]->get_held_number();
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
                bx2 = bx1 + ( animatonPreviewRect->w * editorZoomValue );
                //bx1 = bound_number(bx1,0, widget_box.w);

                by1 = (animationPreviewCam->y+animatonPreviewRect->y - animCameraRect.y)*editorZoomValue;
                //by1 = bound_number(by1,0, widget_box.h);

                bx2 = ( animationPreviewCam->x + animatonPreviewRect->x + animatonPreviewRect->w - animCameraRect.x ) * editorZoomValue;
                //bx2 = bound_number(bx2,0, widget_box.w);

                by2 = ( animationPreviewCam->y + animatonPreviewRect->y + animatonPreviewRect->h - animCameraRect.y ) * editorZoomValue;
                //by2 = bound_number(by2,0, widget_box.h);
                gpe::gcanvas->render_rectangle( bx1-cam->x, by1-cam->y, bx2-cam->x, by2-cam->y, pawgui::theme_main->main_box_highlight_color,true,255);
                rectanglesRendered++;

                if( previewSubImageNumbers->is_clicked() && editorZoomValue > 0.40 )
                {
                    gpe::gfs->render_text_boxed( bx1-cam->x+pawgui::padding_default, by2-4-cam->y, stg_ex::int_to_string(iAnimFrame),
                        pawgui::theme_main->main_box_highlight_color,  pawgui::theme_main->main_box_font_highlight_color, gpe::font_default, gpe::fa_left, gpe::fa_bottom );

                }

                animationPreviewCam->x+= incrementWidth;
                if( animationPreviewCam->x >= animInEditor->get_texture_width() )
                {
                    animatonPreviewRect->y += incrementHeight;
                    animationPreviewCam->x = 0;
                    bx1 = 0;
                }
            }
        }
    }

    if( previewZoomLevel!=nullptr && previewZoomLevel->is_clicked() )
    {
        gpe::gfs->render_text_boxed( view_space->w-32,view_space->h-32,
                          "Zoom Level: "+ stg_ex::float_to_string(editorZoomValue*100 )+"%",
                          pawgui::theme_main->text_box_font_color, gpe::c_black, gpe::font_default, gpe::fa_right, gpe::fa_bottom);
    }

    if( animXScroll!=nullptr)
    {
        animXScroll->render_self( view_space,cam);
    }
    if( animYScroll!=nullptr)
    {
        animYScroll->render_self( view_space,cam);
    }
}


void animationResource::revert_data_fields()
{
    for( int i = 0; i < ANIMATION_DATA_FIELD_COUNT; i++ )
    {
        if( animationDataFields[i]!=nullptr)
        {
            animationDataFields[i]->switch_inuse( false );
        }
        else
        {
            gpe::error_log->report("animation Detected to have nullptr field at ["+ stg_ex::int_to_string(i)+"]");
            return;
        }
    }
    if( animInEditor!=nullptr )
    {
        animationDataFields[ gpe::anim_frame_count]->set_number( animInEditor->get_frame_count() );
        animationDataFields[ gpe::anim_frame_width]->set_number( animInEditor->get_width() );
        animationDataFields[ gpe::anim_frame_height]->set_number( animInEditor->get_height() );
        animationDataFields[ gpe::anim_pixel_offset_hori]->set_number( animInEditor->get_frame_xoffset() );
        animationDataFields[ gpe::anim_pixel_offset_vert]->set_number( animInEditor->get_frame_yoffset() );
        animationDataFields[ gpe::anim_pixel_padding_hori]->set_number( animInEditor->get_hpadding() );
        animationDataFields[ gpe::anim_pixel_padding_vert]->set_number( animInEditor->get_vpadding() );
    }
}

void animationResource::save_resource(std::string file_path, int backupId)
{
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Processing Animation", resource_name );
    }
    std::string newSaveDataFilename = "";
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
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/animations/";
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

            if( animInEditor!=nullptr)
            {
                for( int i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
                {
                    newSaveDataFile << "animationData["<< stg_ex::int_to_string(i)<< "]=";

                    if( animationDataFields[i]!=nullptr)
                    {
                        newSaveDataFile << stg_ex::int_to_string( animationDataFields[i]->get_held_number() )<<"\n";
                    }
                    else
                    {
                        newSaveDataFile << "0\n";
                    }
                }
                newSaveDataFile << "SubimageInView=" << stg_ex::int_to_string( subImagePreviewNumber) << "\n";
                if( preloadCheckBox!=nullptr)
                {
                    newSaveDataFile << "Preload="+ stg_ex::int_to_string(preloadCheckBox->is_clicked() )+"\n";
                }
                if( subImageEditorPreviewId!=nullptr)
                {
                    newSaveDataFile << "EditorPreviewFrame="+ stg_ex::int_to_string(subImageEditorPreviewId->get_held_number() )+"\n";
                }
                if( showCollisionShapeCheckBox!=nullptr)
                {
                    newSaveDataFile << "PreviewCollisionShape="+ stg_ex::int_to_string(showCollisionShapeCheckBox->is_clicked() )+"\n";
                }
                newSaveDataFile << "CollisionShape="<< stg_ex::int_to_string (animationCollisionShapeMenu->get_selected_id() ) +"\n";

                colision_area_resource * c_box_area = nullptr;
                int j_col_property = 0;
                //Loops through every stored frame and store it's collision frame data
                for( int i_col_box = 0; i_col_box < (int)collision_areas.size(); i_col_box++ )
                {
                    c_box_area = collision_areas[i_col_box];
                    newSaveDataFile << "CollisionInfo[" << stg_ex::int_to_string (i_col_box ) +"]=";
                    for( j_col_property = 0; j_col_property <= collision_area_r; j_col_property++ )
                    {
                        newSaveDataFile << stg_ex::int_to_string (c_box_area->animation_collision_property[j_col_property]->get_held_number() )+",";
                    }
                    newSaveDataFile << "\n";
                }
                std::string resFileCopySrc = animInEditor->get_file_name();
                std::string resfile_location = stg_ex::get_short_filename (resFileCopySrc,true );
                newSaveDataFile << "ImageLocation="+resfile_location+"\n";
                if( (int)resfile_location.size() > 0 )
                {
                    std::string resFileCopyDest = soughtDir+resfile_location;
                    if( sff_ex::file_exists(resFileCopyDest) )
                    {
                        /*
                        if( pawgui::display_prompt_message("[WARNING]animation Image File Already exists?","Are you sure you will like to overwrite your ["+resfile_location+"] animation Image File? This action is irreversible!")==pawgui::display_query_yes)
                        {
                            file_copy(resFileCopySrc,resFileCopyDest);
                        }
                        */
                    }
                    else
                    {
                        sff_ex::file_copy(resFileCopySrc,resFileCopyDest);
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
            main_editor_log->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        main_editor_log->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void animationResource::update_box(int x_new, int y_new, int newW, int newH)
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

bool animationResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "animation=" << resource_name << "," << get_global_rid() << ",";
            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}

