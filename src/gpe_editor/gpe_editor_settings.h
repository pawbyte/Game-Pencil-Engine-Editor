/*
gpe_editor_settings.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#ifndef GPE_EDITOR_SETTINGS_RESOURCES_H
#define GPE_EDITOR_SETTINGS_RESOURCES_H

#include "gpe_basic_resource_page.h"
#include "gpe_editor.h"

const int GPE_EXTERNAL_EDITOR_MAX = 4;
const int GPE_EXTERNAL_EDITOR_IMG = 0;
const int GPE_EXTERNAL_EDITOR_AUD = 1;
const int GPE_EXTERNAL_EDITOR_VID = 2;
const int GPE_EXTERNAL_EDITOR_FONT = 3;

class theme_holder
{
public:
    std::string theme_name;
    std::string templateFileName;
    std::string templateBgFileName;
    pawgui::widget_input_text * nameInEditor;
    bool non_default_theme;
    std::vector < pawgui::gpe_widget_color_picker * > colorsInEditor;
    pawgui::widget_label_image * themeIconImg;
    pawgui::widget_input_text * theme_bg_location;
    theme_holder( std::string tName, std::string tFileName, bool isCustomTheme = false  );
    ~theme_holder();
    void clear_colors();
    bool copy_theme( pawgui::gui_theme * systemTheme, bool copyToSelf = true );
    bool load_background( std::string bg_textureLocation, bool allOrNothing = false);
};

class gamePencilEditorSettingsResource: public pawgui::general_resource
{
public:
    pawgui::widget_button_push * confirmResourceButton;
    pawgui::widget_button_push * cancelResourceButton;
    pawgui::widget_selectbox * sideAreaPanel;
    gpe::shape_rect * sidePanelRect;
    pawgui::widget_checkbox * showShortProjectNames;
    pawgui::widget_checkbox * renderSceneBGColor;
    pawgui::widget_selectbox * subEditorSelectBox;


    //General Editor Settings
    pawgui::widget_checkbox * userInvertMouseScroll;

    pawgui::widget_button_push * openEditorFolder;
    pawgui::widget_button_push * openLocalSettingsFolder;
    pawgui::widget_checkbox * autoSaveScreenshots;
    pawgui::widget_checkbox * makeMetaScreenshots;

    //Coding language related settings
    pawgui::widget_label_title * CodingLanguageTitle;
    pawgui::widget_label_text  * CodingLanguageLabel;
    pawgui::widget_label_paragraph * CodingLanguageParagraph;
    pawgui::widget_text_url * CodingLanguageURL;
    pawgui::widget_selectbox * codingLanguageSelector;
    //FPS Related Items
    pawgui::widget_label_title * editorGuiSettingsLabel;
    pawgui::widget_label_title * ideSettingsFPSRateLabel;
    pawgui::widget_dropdown_menu * ideSettingsFPSRate;
    pawgui::widget_label_text  * ideFPSRatioLabel;
    pawgui::widget_label_text  *    ideButtonBarSizeLabel;
    pawgui::widget_checkbox * showFPSOnEditor;

    pawgui::widget_dropdown_menu * ideButtonBarSize;
    pawgui::widget_radio_button_controller * ideButtonBarAlignment;

    //External Editor Section
    pawgui::widget_label_title * externalEditorsTitle;
    pawgui::widget_input_text * pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_MAX];
    pawgui::widget_button_icon * pencilExternalEditorsLoadButton[GPE_EXTERNAL_EDITOR_MAX];

    pawgui::widget_input_number * editorTextAreaDelayTime;
    pawgui::widget_input_number * editorTextInputDelayTime;
    pawgui::widget_input_number * editorCursorBlinkDelayTime;
    pawgui::widget_input_number * editorNormalDelayTime;
    pawgui::widget_input_number * editorScrollSpeed;

    pawgui::widget_label_title * editorStatupTipsLabel;
    pawgui::widget_checkbox * showStatupTipsBox;
    pawgui::widget_checkbox * launchLastProjectBox;
    pawgui::widget_dropdown_menu * projectAutoSaveRate;
    pawgui::widget_label_text  * projectAutoSaveRateLabel;

    //Tab Section
    pawgui::widget_label_title * tabSectionLabel;
    pawgui::widget_checkbox * mouseAutoFindTabs;
    pawgui::widget_input_number * tabSpaceSize;

    //Themes Section
    pawgui::widget_label_title * themesLabel;
    pawgui::widget_dropdown_menu * themePicker;
    pawgui::widget_button_icon * themeLoadButton;
    pawgui::widget_button_icon * themeAddButton;
    pawgui::widget_button_icon * themeRemoveButton;
    pawgui::widget_button_label * themeSetDefaultButton;
    pawgui::widget_text_url * itchLinkForThemes;
    std::vector< theme_holder * > themeHolders;
    int defaultTemplateEndPos;
    int currentThemeInEdit;

    pawgui::widget_label_text  * themeBgLabel;
    pawgui::widget_button_label * themeBgBrowseButton;
    //Advanced Section
    pawgui::widget_label_title * advancedAreaLabel;
    pawgui::widget_checkbox * showHiddenFilesInBrowser;
    pawgui::widget_checkbox * forceFrameRedraw;
    pawgui::widget_button_push * clearCacheButton;

    //Editor Related variables
    gpe::shape_rect subViewedSpace;
    pawgui::widget_panel_list * editorPageList;
    std::string projectFolderListLocation;
    gamePencilEditorSettingsResource();
    ~gamePencilEditorSettingsResource();
    bool include_local_files( std::string pBuildDir , int buildType );

    void load_themes_from_folder( std::string themeFolder );
    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gamePencilEditorSettingsResource * main_editor_settings;
#endif // GPE_EDITOR_SETTINGS_RESOURCES_H
