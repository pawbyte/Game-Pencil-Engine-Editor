#include "gpe_extra_tools.h"

gamePencilExtraTools * main_EXTRA_TOOLS = NULL;

atlas_data_viewer::atlas_data_viewer( gpe::asset_manager * manager )
{
    if( manager != NULL )
    {
        atlas_title = new GPE_Label_Title( manager->get_name(), manager->get_name() );
        atlas_text_animation = new GPE_Label_Text("Animations count("+stg_ex::int_to_string( manager->get_size_animations() )+")","Last added animation: "+manager->get_last_animation_name() );
        atlas_text_music = new GPE_Label_Text( "Music count("+ stg_ex::int_to_string( manager->get_size_music() )+")","Last added audio: "+manager->get_last_music_name()  );
        atlas_text_sounds = new GPE_Label_Text( "Sounds count("+ stg_ex::int_to_string( manager->get_size_sounds() )+")","Last added audio: "+manager->get_last_sound_name()  );
        atlas_text_textures = new GPE_Label_Text( "Textures count("+ stg_ex::int_to_string( manager->get_size_textures() )+")","Last added texture: "+manager->get_last_texture_name()  );
        atlas_text_tilesheets = new GPE_Label_Text( "Tilesheets count("+ stg_ex::int_to_string( manager->get_size_tilesheets() )+")","Last added tilesheet: "+manager->get_last_tilesheet_name()  );
    }
    else
    {
        atlas_title = NULL;
        atlas_text_animation = NULL;
        atlas_text_music = NULL;
        atlas_text_sounds = NULL;
        atlas_text_textures = NULL;
        atlas_text_tilesheets = NULL;
    }
}

atlas_data_viewer::~atlas_data_viewer()
{
    if( atlas_title != NULL )
    {
        delete atlas_title;
        atlas_title = NULL;
    }

    if( atlas_text_animation != NULL )
    {
        delete atlas_text_animation;
        atlas_text_animation = NULL;
    }
    if( atlas_text_music != NULL )
    {
        delete atlas_text_music;
        atlas_text_music = NULL;
    }
    if( atlas_text_sounds != NULL )
    {
        delete atlas_text_sounds;
        atlas_text_sounds = NULL;
    }

    if( atlas_text_textures != NULL )
    {
        delete atlas_text_textures;
        atlas_text_textures = NULL;
    }

    if( atlas_text_tilesheets != NULL )
    {
        delete atlas_text_tilesheets;
        atlas_text_tilesheets = NULL;
    }

}

void atlas_data_viewer::add_to_list( GPE_GuiElementList * element_list )
{
    if( element_list == NULL )
    {
        return;
    }
    element_list->add_gui_element( atlas_title, true );
    element_list->add_gui_element( atlas_text_animation, true );
    element_list->add_gui_element( atlas_text_music, true );
    element_list->add_gui_element( atlas_text_sounds, true );
    element_list->add_gui_element( atlas_text_textures, true );
    element_list->add_gui_element( atlas_text_tilesheets, true );
}

gamePencilExtraTools::gamePencilExtraTools()
{
    isFullScreenResource = true;
    resourceFileName = "";
    resource_name = "JS Compiler Settings";
    resource_name = "Extra Tools";
    parentProjectName = "";
    parentProjectDirectory = "";

    sideAreaPanel = new GPE_SelectBoxBasic("Mode");
    sideAreaPanel->set_width(160);
    sideAreaPanel->set_option_height(64);
    //sideAreaPanel->add_option("General",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/wrench.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Image Editing",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/image.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Data Info",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/question-circle.png"),NULL,2, false, false);

    sidePanelRect = new gpe::shape_rect();

    editorPageList = new GPE_GuiElementList();
    editorPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
    editorPageList->panelAlignType = GPE_PANEL_ALIGN_FULL_LEFT;
    editorPageList->barXMargin = 0;

    sectionTitleExtras = new GPE_Label_Title("Extras","Extras");
    transformImageDirectoryButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magic.png","Transform IMG Folder","Transforms all Images in a given folder",-1);
    refreshAtlasRefreshButton = new GPE_ToolLabelButton("Refresh Atlas Info","Hit to refresh the atlas info..." );
    refresh_atlas_info();
}

gamePencilExtraTools::~gamePencilExtraTools()
{
    if( sectionTitleExtras!=NULL )
    {
        delete sectionTitleExtras;
        sectionTitleExtras = NULL;
    }

    if( transformImageDirectoryButton!=NULL )
    {
        delete transformImageDirectoryButton;
        transformImageDirectoryButton = NULL;
    }
}

bool gamePencilExtraTools::include_local_files( std::string pBuildDir , int buildType )
{

}

void gamePencilExtraTools::load_resource(std::string file_path)
{

}

int gamePencilExtraTools::modify_folder_images(std::string folderLocation, int modifcationType)
{
    int imagesModifiedCount = -1;
    if( sff_ex::path_exists(folderLocation) && modifcationType >=0 && modifcationType < 3)
    {
        gpe::GPE_FileDirectory * foundFolder = new gpe::GPE_FileDirectory();
        foundFolder->open_directory_sorted(folderLocation);

        gpe::color * colorToRemove = gpe::c_fuchsia->duplicate_color();
        bool continueWithAction = false;
        imagesModifiedCount = 0;
        if( foundFolder->get_count() > 0 )
        {
            if( modifcationType==0)
            {
                if( GPE_Change_color_PopUp("Image Background Color To Remove",colorToRemove) )
                {
                    if( GPE_Display_Basic_Prompt("Are you sure you want to erase this Color from all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                    {
                        continueWithAction = true;
                    }
                }
            }
            else if( modifcationType==1 )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you want to invert your all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                {
                    continueWithAction = true;
                }
            }
            else if( modifcationType==2 )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you want to grayscale all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                {
                    continueWithAction = true;
                }
            }
            if( continueWithAction)
            {
                std::string newImageName = "";
                gpe::GPE_File * tempFile = NULL;
                SDL_Surface * oTempSurface = NULL;
                SDL_Surface *nTempSurface = NULL;
                for( int i = 0; i < foundFolder->get_count(); i++)
                {
                    tempFile = foundFolder->get_file(i);
                    if( tempFile!=NULL)
                    {
                        if( tempFile->get_type()=="bmp" || tempFile->get_type()=="png")
                        {
                            newImageName = folderLocation+"/"+tempFile->get_name();
                            oTempSurface = sdl_surface_ex::load_surface_image( newImageName.c_str() );
                            if( oTempSurface->w > 0 && oTempSurface->h > 0)
                            {
                                nTempSurface = NULL;
                                if( oTempSurface!=NULL)
                                {
                                    gpe::error_log->report("Modifying image at: "+newImageName+".");

                                    if( GPE_LOADER != NULL )
                                    {
                                        GPE_LOADER->update_messages( "Modifying Image", tempFile->get_name(),"Please wait..." );
                                    }

                                    if( modifcationType==0)
                                    {
                                        if( GPE_Change_color_PopUp("Image Background Color To Remove",colorToRemove) )
                                        {
                                            if( GPE_Display_Basic_Prompt("Are you sure you want to erase this Color from this image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                            {
                                                nTempSurface= sdl_surface_ex::surface_remove_color_rgba(oTempSurface, colorToRemove->get_r(), colorToRemove->get_r(), colorToRemove->get_b() );

                                            }
                                        }
                                    }
                                    else if( modifcationType==1 )
                                    {
                                        nTempSurface= sdl_surface_ex::surface_invert(oTempSurface);
                                    }
                                    else if( modifcationType==2 )
                                    {
                                        nTempSurface= sdl_surface_ex::surface_grayscale(oTempSurface);
                                    }
                                    if( nTempSurface!=NULL)
                                    {
                                        if( tempFile->get_type()=="bmp" || tempFile->get_type()=="png")
                                        {
                                            SDL_SaveBMP(nTempSurface,newImageName.c_str() );
                                        }
                                        else if( tempFile->get_type()=="png")
                                        {
                                            IMG_SavePNG(nTempSurface,newImageName.c_str() );
                                        }
                                        SDL_FreeSurface(nTempSurface);
                                        nTempSurface = NULL;
                                        imagesModifiedCount++;
                                    }
                                    SDL_FreeSurface(oTempSurface);
                                    oTempSurface = NULL;
                                }
                            }
                        }
                    }
                }
            }
        }
        foundFolder->close_directory();
        delete foundFolder;
        if( colorToRemove!=NULL)
        {
            delete colorToRemove;
            colorToRemove = NULL;
        }
    }
    return imagesModifiedCount;
}

void gamePencilExtraTools::prerender_self( )
{

}

void gamePencilExtraTools::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam)
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    if( cam!=NULL && editorPageList!=NULL && viewedSpace!=NULL)
    {
        int prevTab = sideAreaPanel->get_selection();
        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = viewedSpace->w;
            subViewedSpace.h = viewedSpace->h;
            PANEL_GENERAL_EDITOR->add_gui_element_fullsize( sideAreaPanel );
            PANEL_GENERAL_EDITOR->process_self();
        }
        else
        {
            sideAreaPanel->set_coords(0, 0 );
            sideAreaPanel->set_width( 192 );
            sideAreaPanel->set_height( viewedSpace->h );
            sideAreaPanel->process_self( viewedSpace, cam );
            subViewedSpace.x = sideAreaPanel->get_x2pos();
            subViewedSpace.y = 0;
            subViewedSpace.w = viewedSpace->w - sideAreaPanel->get_width();
            subViewedSpace.h = viewedSpace->h;
        }

        if( prevTab!=sideAreaPanel->get_selection() )
        {
            editorPageList->reset_self();
        }
        editorPageList->clear_list();
        editorPageList->set_coords( subViewedSpace.x, subViewedSpace.y );
        editorPageList->set_width(subViewedSpace.w);
        editorPageList->set_height(subViewedSpace.h );
        editorPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
        editorPageList->barXMargin = GENERAL_GPE_GUI_PADDING;
        int i = 0;
        if( sideAreaPanel->get_selected_name()=="General")
        {

        }
        else if( sideAreaPanel->get_selected_name()=="Image Editing")
        {
            editorPageList->add_gui_element( transformImageDirectoryButton, true );
            editorPageList->process_self( viewedSpace,cam);

            if( transformImageDirectoryButton->is_clicked()  )
            {
                GPE_open_context_menu(-1,-1,256);
                main_CONTEXT_MENU->add_menu_option("Erase BG Color On All Images",0);
                main_CONTEXT_MENU->add_menu_option("Invert Colors On All Images",1);
                main_CONTEXT_MENU->add_menu_option("Make GrayScale On All Images",2);
                main_CONTEXT_MENU->add_menu_option("Exit Menu",10);
                int menuSelection = GPE_Get_Context_Result();
                if( menuSelection>=0 && menuSelection <=3)
                {
                    std::string lastDirectory = "";
                    std::string directoryToMod = GPE_GetDirectoryName("Transform All Images in this directory", lastDirectory,false);
                    int imagesModified = modify_folder_images(directoryToMod,menuSelection);
                }
            }
        }
        else if( sideAreaPanel->get_selected_name() == "Data Info" )
        {
            atlas_data_viewer * current_atlas = NULL;
            for( int iAtlas = 0; iAtlas < (int)atlas_list.size(); iAtlas++ )
            {
                current_atlas = atlas_list[iAtlas];
                if( current_atlas!=NULL )
                {
                    current_atlas->add_to_list( editorPageList );
                }
            }
            editorPageList->add_gui_element( refreshAtlasRefreshButton, true );
            editorPageList->process_self( viewedSpace,cam);

            if( refreshAtlasRefreshButton!=NULL && refreshAtlasRefreshButton->is_clicked() )
            {
                refresh_atlas_info();
            }
        }
    }
}

void gamePencilExtraTools::refresh_atlas_info()
{
    if( editorPageList != NULL )
    {
        editorPageList->clear_list();
    }

    int iAtlasCount = (int)atlas_list.size();
    atlas_data_viewer * current_atlas = NULL;
    for( int iAtlas = iAtlasCount -1; iAtlas >=0; iAtlas-- )
    {
        current_atlas = atlas_list[iAtlas ];
        if( current_atlas != NULL )
        {
            delete current_atlas;
            current_atlas = NULL;
        }
    }
    atlas_list.clear();
    current_atlas = new atlas_data_viewer( gpe::rsm );
    atlas_list.push_back( current_atlas );

    current_atlas = new atlas_data_viewer( paw_gui_rsm );
    atlas_list.push_back( current_atlas );
}

void gamePencilExtraTools::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam)
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        if( sideAreaPanel!=NULL && PANEL_GENERAL_EDITOR==NULL )
        {
            sideAreaPanel->render_self( viewedSpace,cam);
        }
        if( editorPageList!=NULL )
        {
            editorPageList->render_self( viewedSpace,cam);
        }
    }
}

void gamePencilExtraTools::save_resource(std::string file_path , int backupId )
{

}

bool gamePencilExtraTools::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{

}
