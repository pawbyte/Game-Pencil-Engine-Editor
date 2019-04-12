#include "gpe_extra_tools.h"

gamePencilExtraTools * MAIN_EXTRA_TOOLS = NULL;
gamePencilExtraTools::gamePencilExtraTools()
{
    isFullScreenResource = true;
    resourceFileName = "";
    resourceName = "JS Compiler Settings";
    resourceName = "Extra Tools";
    parentProjectName = "";
    parentProjectDirectory = "";

    sideAreaPanel = new GPE_SelectBoxBasic("Mode");
    sideAreaPanel->set_width(160);
    sideAreaPanel->set_option_height(64);
    //sideAreaPanel->add_option("General",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/wrench.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Image Editing",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/image.png"),NULL,2, false, false);

    sidePanelRect = new GPE_Rect();

    editorPageList = new GPE_GuiElementList();
    editorPageList->barXPadding = GENERAL_GPE_PADDING;
    editorPageList->barXMargin = 0;

    sectionTitleExtras = new GPE_Label_Title("Extras","Extras");
    transformImageDirectoryButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/magic.png","Transform IMG Folder","Transforms all Images in a given folder",-1);
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

void gamePencilExtraTools::prerender_self( )
{

}

void gamePencilExtraTools::preprocess_self(std::string alternatePath)
{

}

void gamePencilExtraTools::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
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
        editorPageList->barXPadding = GENERAL_GPE_PADDING;
        editorPageList->barXMargin = GENERAL_GPE_PADDING;
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
                MAIN_CONTEXT_MENU->add_menu_option("Erase BG Color On All Images",0);
                MAIN_CONTEXT_MENU->add_menu_option("Invert Colors On All Images",1);
                MAIN_CONTEXT_MENU->add_menu_option("Make GrayScale On All Images",2);
                MAIN_CONTEXT_MENU->add_menu_option("Exit Menu",10);
                int menuSelection = GPE_Get_Context_Result();
                if( menuSelection>=0 && menuSelection <=3)
                {
                    std::string lastDirectory = "";
                    std::string directoryToMod = GPE_GetDirectoryName("Transform All Images in this directory", lastDirectory,false);
                    int imagesModified = modify_folder_images(directoryToMod,menuSelection);
                }
            }
        }
    }
}

void gamePencilExtraTools::render_self(GPE_Rect * viewedSpace,GPE_Rect * cam, bool forceRedraw)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        if( forceRedraw && sideAreaPanel!=NULL && PANEL_GENERAL_EDITOR==NULL )
        {
            sideAreaPanel->render_self( viewedSpace,cam, forceRedraw);
        }
        if( editorPageList!=NULL )
        {
            editorPageList->render_self( viewedSpace,cam, forceRedraw);
        }
    }
}

void gamePencilExtraTools::save_resource(std::string alternatePath , int backupId )
{

}

bool gamePencilExtraTools::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{

}
