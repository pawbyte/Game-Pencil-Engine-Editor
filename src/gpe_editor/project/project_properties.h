/*
project_properties.h
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

#ifndef GPE_project_PROOPERTIES_H
#define GPE_project_PROOPERTIES_H

#include "../resources/basic/gpe_basic_resource_page.h"
#include "../cpp/gpe_cpp_builder_settings.h"

class projectPropertiesResource: public standardEditableGameResource
{
public:

    //Project Requirements [ BEGIN ]
    pawgui::widget_dropdown_menu * renderer_backend_requirements;
    pawgui::widget_dropdown_menu * audio_backend_requirements;
    pawgui::widget_dropdown_menu * rmode_backend_requirements;
    //Project Requirements [ END ]
    pawgui::widget_selectbox * project_settingsBar;

    //Icon data and such
    std::string iconFileName;
    pawgui::widget_label_text  * projectIconLabel;
    pawgui::widget_label_image * projectIconImg;
    pawgui::widget_button_label * browseIcon_button;
    //Meta information to be built into the project
    pawgui::widget_text_editor * projectGameMacros;
    pawgui::widget_text_editor * projectGameNotes;
    pawgui::widget_checkbox * showMetaData;
    pawgui::widget_label_text  * projectLanguageLabel;
    pawgui::widget_dropdown_menu * projectLanguage;
    //Export _settings Tab
    pawgui::widget_selectbox * export_settingsBar;
    //Export _settings [ HTML5]
    pawgui::widget_checkbox * obfuscateCodeCheckbox;
    pawgui::widget_checkbox * minifyCodeCheckbox;
    pawgui::widget_checkbox * warnOnCloseCheckbox;

    pawgui::widget_button_push * buildToSource_button; //used to create projects to raw CPP files / dependencies
    pawgui::widget_dropdown_menu  * buildPlatformdropdown; //used to select which "build script we use for building thru source"
    pawgui::widget_text_url * buildPlatformRefreshLabel;
    pawgui::widget_button_push * exportPush_button;
    pawgui::widget_button_push * exportAndPlayPush_button;
    pawgui::widget_button_push * cleanBuildFolderPush_button;
    pawgui::widget_button_push * cleanBuildAllFolderPush_button;
    pawgui::widget_button_push * playProgramPush_button;

    //[Export _settings Desktop]
    pawgui::widget_tabbar * desktop_settingsTabBar;
    pawgui::widget_label_title * exportApplicationLabel;
    pawgui::widget_label_title * exportWindowsLabel;
    pawgui::widget_label_title * exportOSXLabel;
    pawgui::widget_label_title * exportLinuxLabel;
    pawgui::widget_checkbox * win64BitCheckbox;
    pawgui::widget_checkbox * osx64BitCheckbox;
    pawgui::widget_checkbox * linux64BitCheckbox;

    pawgui::widget_checkbox * html5DebugModeCheckbox;
    pawgui::widget_checkbox * osxDebugModeCheckbox;
    pawgui::widget_checkbox * winDebugModeCheckbox;
    pawgui::widget_checkbox * linuxDebugModeCheckbox;

    pawgui::widget_radio_button_controller * html5nativeBuildTypeOptions;
    pawgui::widget_radio_button_controller * windowsNativeBuildTypeOptions;
    pawgui::widget_radio_button_controller * osxNativeBuildTypeOptions;
    pawgui::widget_radio_button_controller * linuxNativeBuildTypeOptions;
    pawgui::widget_radio_button_controller * switchNativeBuildTypeOptions;
    pawgui::widget_label_text  * electronCompileNote;
    pawgui::widget_text_url * electronCompileUrl;

    pawgui::widget_label_paragraph * switchComingSoonNote;
    pawgui::widget_label_paragraph * nSwitchSideNote;
    pawgui::widget_label_paragraph * nSwitchThirdNote;
    pawgui::widget_text_url * nSwitchContactUrl;

    pawgui::widget_label_paragraph * ps4ComingSoonNote;
    pawgui::widget_label_paragraph * ps4SideNote;
    pawgui::widget_text_url * ps4ContactUrl;

    pawgui::widget_label_paragraph * xboxOneComingSoonNote;
    pawgui::widget_label_paragraph * xboxOneSideNote;
    pawgui::widget_text_url * xboxOneContactUrl;

    //Other Platform label notes
    pawgui::widget_label_text  * otherPlatformLabel;
    pawgui::widget_label_paragraph * otherPlatformLabelParagraph;
    pawgui::widget_text_url * otherPlatformLabelURL;


    //_settings Tab
    pawgui::widget_input_number * projectScreenWidthField;
    pawgui::widget_input_number * projectScreenHeightField;
    pawgui::widget_input_number * projectGameVersionField;
    pawgui::widget_input_text * projectGameSubVersionField;
    pawgui::widget_radio_button_controller * project_scale_settings;
    pawgui::widget_label_text  * project_settingsFPSRateLabel;
    pawgui::widget_dropdown_menu * project_settingsFPSRate;
    pawgui::widget_dropdown_menu * project_default_animation2d_alignment;

    //Publisher Setting
    pawgui::widget_label_title * sectionTitlePublisher;
    pawgui::widget_label_title * sectionTitleGame_settings;
    pawgui::widget_input_text * projectGameTitleField;
    pawgui::widget_input_text * projectGameShortTitleField;
    pawgui::widget_input_text * projectGameDateField;
    pawgui::widget_input_text * projectGameDescriptionField;
    pawgui::widget_input_text * projectGamePublisherField;
    pawgui::widget_input_text * projectGameDeveloperField;
    pawgui::widget_input_text * projectGameCopyrightField;
    pawgui::widget_input_text * projectGameWebsiteField;
    pawgui::widget_input_text * projectGameEmailField;
    pawgui::widget_checkbox * checkBoxShowPublisherInfo;

    //Colors Tab
    pawgui::widget_label_title * sectionWebColorsTitle;
    pawgui::gpe_widget_color_picker * projectBorderColor;
    pawgui::gpe_widget_color_picker * projectGameBackgroundColor;
    pawgui::gpe_widget_color_picker * projectWebsiteBackgroundColor;
    pawgui::gpe_widget_color_picker * projectTextHeaderColor;
    pawgui::gpe_widget_color_picker * projectTextParagraphColor;
    pawgui::gpe_widget_color_picker * projectTextLinkColor;
    pawgui::gpe_widget_color_picker * projectTextLinkActiveColor;
    pawgui::gpe_widget_color_picker * projectTextLinkHoverColor;
    pawgui::gpe_widget_color_picker * projectTextLinkVisitedColor;
    gpe::shape_rect subViewedSpace;

    pawgui::widget_panel_list * project_settingsList;
    pawgui::widget_panel_list * collisionLayerMatrixList;
    pawgui::widget_checkbox * projectCollisionLayerMatrix[1024];
    pawgui::widget_label_rotated_90 * projectLayerNamesColumnTitles[32];
    pawgui::widget_label_text  * projectLayerNamesRowTitles[32];
    pawgui::widget_label_text  * projectLayerEmptyTitle;
    pawgui::widget_button_label * selectAllCollisionBoxes;
    pawgui::widget_button_label * unselectAllCollisionBoxes;

    projectPropertiesResource(pawgui::widget_resource_container * pFolder = nullptr);
    ~projectPropertiesResource();
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool export_and_play_native( bool launchProgram = true);
    void integrate_into_syntax();
    bool include_local_files( std::string pBuildDir , int buildType );
    bool is_build_ready();
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void refresh_build_to_source_targets();
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void save_resource(std::string file_path = "", int backupId = -1);
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    void update_project_layers();
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
