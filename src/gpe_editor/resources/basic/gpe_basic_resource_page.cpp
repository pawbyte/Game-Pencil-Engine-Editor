/*
gpe_basic_resource_page.cpp
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

#include "../../resources/basic/gpe_basic_resource_page.h"


gpe::texture_base * texture_transparent_bg = nullptr;
gpe::texture_base * texture_gpe_logo = nullptr;

GPE_ObjectComponent::GPE_ObjectComponent()
{
    component = nullptr;
    settingsGear = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cogs.png","settings",-1,16 );
}

GPE_ObjectComponent::~GPE_ObjectComponent()
{
    if( component!=nullptr )
    {
        delete component;
        component = nullptr;
    }

    if( settingsGear!=nullptr)
    {
        delete settingsGear;
        settingsGear = nullptr;
    }
}

void GPE_ObjectComponent::load_data(std::string datastring)
{
    if( component!=nullptr)
    {
        component->load_data(datastring);
    }
}

void GPE_ObjectComponent::remove_data(std::string datastring)
{
    if( component!=nullptr)
    {
        component->remove_data(datastring);
    }
}

void GPE_ObjectComponent::rename_component(std::string new_name)
{
    if( component!=nullptr)
    {
        component->set_name(new_name);
    }

    if( settingsGear!=nullptr)
    {
        settingsGear->set_name(new_name);
    }
}

pawgui::widget_basic * GPE_ObjectComponent::get_component()
{
    return component;
}

pawgui::widget_button_icon * GPE_ObjectComponent::get_gear()
{
    return settingsGear;
}

std::string GPE_ObjectComponent::get_data()
{
    if( component!=nullptr)
    {
        return component->get_data();
    }
    return "";
}

std::string GPE_ObjectComponent::get_name()
{
    if( component!=nullptr)
    {
        return component->get_name();
    }

    if( settingsGear!=nullptr)
    {
        return settingsGear->get_name();
    }
    return "";

}

std::string GPE_ObjectComponent::get_plain_string()
{
    if( component!=nullptr)
    {
        return component->get_plain_string();
    }
    return "";
}

std::string GPE_ObjectComponent::get_type()
{
    if( component!=nullptr)
    {
        return component->get_element_type();
    }

    return "";
}

void GPE_ObjectComponent::set_component(pawgui::widget_basic * newComponent)
{
    if( newComponent!=nullptr)
    {
        if( component!=nullptr)
        {
            delete component;
        }
        component = newComponent;
        component->hasLineBreak = false;
        if( settingsGear!=nullptr)
        {
            settingsGear->set_name( component->get_name() );
        }
    }
}

standardEditableGameResource::standardEditableGameResource(pawgui::widget_resource_container * ppFolder)
{
    local_mouse_x = -1;
    local_mouse_y = -1;
    zoom_level = 1.0;

    resource_name = "";
    globalResouceIdNumber = -1;
    common_buttonAlignment = 0;
    resourcePostProcessed = false;
    justOpenedThisFrame = false;
    parentProjectName = "";
    projectParentFolder = ppFolder;
    if( projectParentFolder!=nullptr)
    {
        parentProjectName = projectParentFolder->get_name();
    }
    renameBox = new pawgui::widget_input_text("","Name");
    renameBox->set_label("Name");

    loadResource_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png","Load Resource",-1);
    exportResource_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/plane.png","Export Resource TO File",-1);
    saveResource_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png","Save Resource to File",-1);

    confirmResource_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/check.png","Confirm Changes","Confirm and Save Changes");
    cancelResource_button =  new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/times.png","Cancel Changes","Cancel Changes and Revert to previous settings");

    int maxSize = std::max(confirmResource_button->get_width(),cancelResource_button->get_width() );
    //confirmResource_button->set_width(maxSize);
    //cancelResource_button->set_width(maxSize);
    //makes it so the main button options are the same width along with the rename string box too for design purposes
}


standardEditableGameResource::~standardEditableGameResource()
{
    if( confirmResource_button!=nullptr)
    {
        delete confirmResource_button;
        confirmResource_button = nullptr;
    }
    if( cancelResource_button!=nullptr)
    {
        delete cancelResource_button;
        cancelResource_button = nullptr;
    }
    if( loadResource_button!=nullptr)
    {
        delete loadResource_button;
        loadResource_button = nullptr;
    }
    if( exportResource_button!=nullptr)
    {
        delete exportResource_button;
        exportResource_button = nullptr;
    }
    if( saveResource_button!=nullptr)
    {
        delete saveResource_button;
        saveResource_button = nullptr;
    }
    if( renameBox!=nullptr)
    {
        delete renameBox;
        renameBox = nullptr;
    }
}

std::string standardEditableGameResource::get_current_name()
{
    if( renameBox!=nullptr)
    {
        if( stg_ex::is_alnum(renameBox->get_string(),false,true)==true)
        {
            return renameBox->get_string();
        }
        else if( main_editor_log!=nullptr && resourceType!=gpe::resource_type_project_settings && globalResouceIdNumber >=1000 )
        {
            main_editor_log->log_general_comment("Non-alphabetical resource ["+renameBox->get_string()+"] being renamed to "+resource_name+"." );
        }
    }
    return get_name();
}

pawgui::widget_basic * standardEditableGameResource::add_gui_component(std::string componentData)
{
    if( (int)componentData.size() > 0)
    {
        int colonPos = componentData.find(":");
        int equalPos = componentData.find("==|||==");

        if( colonPos!=(int)std::string::npos && equalPos!=(int)std::string::npos && (equalPos>colonPos) )
        {
            std::string newComponentType = stg_ex::split_first_string(componentData,":");
            std::string newComponentName = stg_ex::split_first_string(componentData,"==|||==");

            pawgui::widget_basic * newComponentField = nullptr;
            if( newComponentType=="checkbox")
            {
                newComponentField = new pawgui::widget_checkbox(newComponentName);
            }
            else if( newComponentType=="colorpicker")
            {
                pawgui::gpe_widget_color_picker * color_newField = new pawgui::gpe_widget_color_picker( newComponentName);
                color_newField->set_label(newComponentName);
                newComponentField = color_newField;
            }
            else if( newComponentType == "contentselector-int")
            {
                pawgui::widget_content_selector * newContentSelector = new pawgui::widget_content_selector( newComponentName, "");
                newContentSelector->selectorType = pawgui::content_selector_type::cs_type_int;
                newComponentField = newContentSelector;
            }
            else if( newComponentType == "contentselector-float")
            {
                pawgui::widget_content_selector * newContentSelector = new pawgui::widget_content_selector( newComponentName, "");
                newContentSelector->selectorType = pawgui::content_selector_type::cs_type_float;
                newComponentField = newContentSelector;
            }
            else if( newComponentType == "contentselector-string")
            {
                pawgui::widget_content_selector * newContentSelector = new pawgui::widget_content_selector( newComponentName, "");
                newContentSelector->selectorType = pawgui::content_selector_type::cs_type_string;
                newComponentField = newContentSelector;
            }
            else if( newComponentType == "contentselector-color")
            {
                pawgui::widget_content_selector * newContentSelector = new pawgui::widget_content_selector( newComponentName, "");
                newContentSelector->selectorType = pawgui::content_selector_type::cs_type_color;
                newComponentField = newContentSelector;
            }
            else if( newComponentType=="dropdown")
            {
                newComponentField = new pawgui::widget_dropdown_menu( newComponentName,false);
            }
            else if(newComponentType=="inputtext")
            {
                pawgui::widget_input_text * tempNewField = new pawgui::widget_input_text("");
                tempNewField->set_name(newComponentName);
                tempNewField->set_label(newComponentName);
                tempNewField->set_width(256);
                newComponentField = tempNewField;
            }
            else if(newComponentType=="inputnumber")
            {
                pawgui::widget_input_text * tempNewField = new pawgui::widget_input_number("");
                tempNewField->set_name(newComponentName);
                tempNewField->set_label(newComponentName);
                tempNewField->set_width(256);
                newComponentField = tempNewField;
            }
            else if(newComponentType=="radio")
            {
                pawgui::widget_radio_button_controller * tempRadio_button = new pawgui::widget_radio_button_controller();
                tempRadio_button->set_name(newComponentName);
                newComponentField = tempRadio_button;
            }
            else if(newComponentType=="labeltext")
            {
                newComponentField = new pawgui::widget_label_text (newComponentName,newComponentName);
            }
            else if(newComponentType=="texturl")
            {
                newComponentField = new pawgui::widget_text_url(newComponentName,newComponentName);
            }
            else if( newComponentType=="resourcedropdown")
            {
                std::string dropdownType = stg_ex::split_first_string(componentData,"=");
                newComponentField = new pawgui::widget_drop_down_resource_menu( newComponentName, projectParentFolder );
            }
            if( newComponentField!=nullptr)
            {
                newComponentField->load_data(componentData);
            }
            //error_log->report("Returning ["+newComponentType+"] gui component named ["+newComponentName+"] using ["+componentData+"] data.");
            return newComponentField;
        }
    }
    gpe::error_log->report("Returning nullptr gui component using ["+componentData+"] data.");
    return nullptr;
}


bool standardEditableGameResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount )
{
    return true;
}

void standardEditableGameResource::compile_cpp( )
{

}

bool standardEditableGameResource::get_mouse_coords( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    local_mouse_x = 0;
    local_mouse_y = 0;
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( view_space == nullptr || cam== nullptr )
    {
        return false;
    }

    if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,view_space->x,view_space->y,view_space->x+view_space->w,view_space->y+view_space->h) == false )
    {
        return false;
    }

    if ( gpe::point_between( gpe::input->mouse_position_x,gpe::input->mouse_position_y,
                            view_space->x-cam->x,view_space->y-cam->y,
                            view_space->x+view_space->w-cam->x,view_space->y+view_space->h-cam->y) == false )
    {
        return false;
    }


    local_mouse_x = ( gpe::input->mouse_position_x-cam->x)/zoom_level - view_space->x;
    local_mouse_y = ( gpe::input->mouse_position_y-cam->y)/zoom_level - view_space->y;

    if( pawgui::main_statusbar!=nullptr)
    {
        pawgui::main_statusbar->set_codestring( "Mouse( "+ stg_ex::int_to_string(local_mouse_x )+" , "+ stg_ex::int_to_string(local_mouse_y)+")"+
                                            "Camera( "+ stg_ex::float_to_string(cam->x )+" , "+ stg_ex::float_to_string(cam->y)+") Zoom:"+ stg_ex::float_to_string(zoom_level) );
    }
    return true;
}


bool standardEditableGameResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

void standardEditableGameResource::prerender_self( )
{
    if( loadResource_button!=nullptr)
    {
        loadResource_button->prerender_self( );
    }
    if( exportResource_button!=nullptr)
    {
        exportResource_button->prerender_self( );
    }
    if( saveResource_button!=nullptr)
    {
        saveResource_button->prerender_self( );
    }
    if( confirmResource_button!=nullptr)
    {
        confirmResource_button->prerender_self( );
    }
    if( cancelResource_button!=nullptr)
    {
        cancelResource_button->prerender_self( );
    }
}

void standardEditableGameResource::open_code( int lineNumb, int colNumb, std::string codeTitle )
{

}

void standardEditableGameResource::process_resource( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    isModified = false;
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    //Unclicks buttons
    if( loadResource_button!=nullptr )
        loadResource_button->set_clicked( false );
    if( saveResource_button!=nullptr )
        saveResource_button->set_clicked( false );
    if( exportResource_button!=nullptr )
        exportResource_button->set_clicked( false );
    if( confirmResource_button!=nullptr )
        confirmResource_button->set_clicked( false );
    if( cancelResource_button!=nullptr )
        cancelResource_button->set_clicked( false );

    process_self(view_space,cam);
    process_export();
    /*
    if( confirmResource_button!=nullptr)
    {
        if( common_buttonAlignment==1)
        {
            cancelResource_button->set_coords(view_space->w-cancelResource_button->get_width()-pawgui::padding_default,view_space->h-cancelResource_button->get_height()-pawgui::default_line_height);
            cancelResource_button->process_self(view_space,cam);
            if( confirmResource_button!=nullptr)
            {
                if( confirmResource_button->is_enabled() )
                {
                    confirmResource_button->set_coords(cancelResource_button->get_xpos()-confirmResource_button->get_width()-pawgui::padding_default,cancelResource_button->get_ypos());
                }
                else
                {
                    confirmResource_button->set_coords(cancelResource_button->get_xpos()-confirmResource_button->get_width()-pawgui::padding_default,cancelResource_button->get_ypos());
                }
                confirmResource_button->process_self(view_space,cam);
                if( confirmResource_button->is_clicked() && editorMode==0 )
                {
                    if( renameBox!=nullptr)
                    {
                        set_name( renameBox->get_string() );
                    }
                }
            }
        }
        else
        {
            cancelResource_button->set_coords(-1,view_space->h-cancelResource_button->get_height()-pawgui::default_line_height);
            cancelResource_button->process_self(view_space,cam);
            if( confirmResource_button!=nullptr)
            {
                if( cancelResource_button->is_enabled() )
                {
                    confirmResource_button->set_coords(-1,cancelResource_button->get_ypos()-confirmResource_button->get_height()-pawgui::default_line_height);
                }
                else
                {
                    confirmResource_button->set_coords(-1,view_space->h-confirmResource_button->get_height()-pawgui::default_line_height);
                }
                confirmResource_button->process_self(view_space,cam);
                if( confirmResource_button->is_clicked() && editorMode==0 )
                {
                    if( renameBox!=nullptr)
                    {
                        set_name( renameBox->get_string() );
                    }
                }
            }
        }
    }
    if( renameBox!=nullptr)
    {
        renameBox->process_self(view_space,cam);
        //set_name( renameBox->get_string() );
        if(resource_name!=renameBox->get_string() )
        {
            isModified = true;
        }
    }
    */
}

void standardEditableGameResource::process_export()
{
    bool exportCalled = false;
    if( exportResource_button!=nullptr && exportResource_button->is_clicked() )
    {
        exportCalled = true;
    }
    else if( gpe::input->check_kb_down(kb_ctrl) && gpe::input->check_kb_pressed(kb_e) )
    {
        exportCalled = true;
        gpe::input->reset_all_input();
    }

    if( exportCalled )
    {
        if( resourceType >=0 && resourceType < gpe::resource_type_max )
        {
            std::string fileToExportName = pawgui::get_filename_save_from_popup( "Export "+gpe::resource_type_names[resourceType]+" to file",".gpf", pawgui::main_settings->fileSaveExportFileDir );
            if( (int)fileToExportName.size() > 0)
            {
                if(  stg_ex::get_file_ext(fileToExportName) !="gpf" )
                {
                    fileToExportName = stg_ex::get_file_noext(fileToExportName)+".gpf";
                }
                main_editor_log->log_general_line("Exporting "+gpe::resource_type_names[resourceType]+" resource to "+fileToExportName+".");
                save_resource(fileToExportName);
            }
            else
            {
                main_editor_log->log_general_comment("Export of ["+resource_name+"] canceled.");
            }
        }
        exportResource_button->set_clicked( false );
    }
}

void standardEditableGameResource::integrate_into_syntax()
{
    if( current_project!=nullptr)
    {
        if( resourceType >=0 && resourceType < gpe::resource_type_max)
        {
            current_project->add_project_keyword(resource_name,"Project "+gpe::resource_type_names[resourceType]+" Resource",-1,"");
        }
    }
}

bool standardEditableGameResource::is_build_ready()
{
    recentErrorMessage = "";
    return true;
}

void standardEditableGameResource::load_resource(std::string file_path)
{

}

void standardEditableGameResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{

}

void standardEditableGameResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam  )
{

}
void standardEditableGameResource::render_axis_2d(int x_pos, int y_pos, bool render_sphere,  gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    if( render_sphere )
    {
        gpe::gcanvas->render_circle_filled_color(x_pos, y_pos, 64, gpe::c_gray, 128);
        gpe::gcanvas->render_circle_outline_color(x_pos, y_pos, 64, gpe::c_white, 255);
    }
    gpe::gcanvas->render_line_width_color(x_pos, y_pos, x_pos + 96, y_pos, 8, gpe::c_red, 255);
    gpe::gfs->render_text( x_pos + 96, y_pos, "X:", gpe::c_green, gpe::font_default, gpe::fa_left, gpe::fa_middle, 255 );
    gpe::gcanvas->render_line_width_color(x_pos, y_pos, x_pos , y_pos+ 96,8, gpe::c_blue, 255);
    gpe::gfs->render_text( x_pos, y_pos+96, "Y:", gpe::c_green, gpe::font_default, gpe::fa_center, gpe::fa_top, 255 );


}

void standardEditableGameResource::render_axis_3d(int x_pos, int y_pos, bool render_sphere, gpe::shape_rect * view_space , gpe::shape_rect * cam )
{

}

void standardEditableGameResource::seek_parent_name()
{
    if( projectParentFolder!=nullptr)
    {
        parentProjectName = projectParentFolder->get_name();
    }
}

void standardEditableGameResource::set_name(std::string new_name)
{
    resource_name = new_name;
    if(renameBox!=nullptr)
    {
        renameBox->set_string(new_name);
    }
}

void standardEditableGameResource::set_parent_name(std::string new_name)
{
    parentProjectName = new_name;
}

int standardEditableGameResource::search_for_string(std::string needle)
{
    return 0;
}

int standardEditableGameResource::search_and_replace_string(std::string needle, std::string newStr )
{
    return 0;
}

