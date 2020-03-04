/*
project_properties.h
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

#ifndef GPE_project_PROOPERTIES_H
#define GPE_project_PROOPERTIES_H

#include "gpe_basic_resource_page.h"
#include "gpe_cpp_builder_settings.h"

class projectPropertiesResource: public standardEditableGameResource
{
public:
    GPE_SelectBoxBasic * projectSettingsBar;

    //Icon data and such
    std::string iconFileName;
    GPE_Label_Text * projectIconLabel;
    GPE_Label_Image * projectIconImg;
    GPE_ToolLabelButton * browseIconButton;
    //Meta information to be built into the project
    GPE_TextAreaInputBasic * projectGameMacros;
    GPE_TextAreaInputBasic * projectGameNotes;
    GPE_CheckBoxBasic * showMetaData;
    GPE_Label_Text * projectLanguageLabel;
    GPE_DropDown_Menu * projectLanguage;
    //Export Settings Tab
    GPE_SelectBoxBasic * exportSettingsBar;
    //Export Settings [ HTML5]
    GPE_CheckBoxBasic * obfuscateCodeCheckbox;
    GPE_CheckBoxBasic * minifyCodeCheckbox;
    GPE_CheckBoxBasic * warnOnCloseCheckbox;

    GPE_ToolPushButton * buildToSourceButton; //used to create projects to raw CPP files / dependencies
    GPE_DropDown_Menu  * buildPlatformdropdown; //used to select which "build script we use for building thru source"
    GPE_TextURL * buildPlatformRefreshLabel;
    GPE_ToolPushButton * exportPushButton;
    GPE_ToolPushButton * exportAndPlayPushButton;
    GPE_ToolPushButton * cleanBuildFolderPushButton;
    GPE_ToolPushButton * cleanBuildAllFolderPushButton;
    GPE_ToolPushButton * playProgramPushButton;

    //[Export Settings Desktop]
    GPE_TabBar * desktopSettingsTabBar;
    GPE_Label_Title * exportApplicationLabel;
    GPE_Label_Title * exportWindowsLabel;
    GPE_Label_Title * exportOSXLabel;
    GPE_Label_Title * exportLinuxLabel;
    GPE_CheckBoxBasic * win64BitCheckbox;
    GPE_CheckBoxBasic * osx64BitCheckbox;
    GPE_CheckBoxBasic * linux64BitCheckbox;

    GPE_CheckBoxBasic * html5DebugModeCheckbox;
    GPE_CheckBoxBasic * osxDebugModeCheckbox;
    GPE_CheckBoxBasic * winDebugModeCheckbox;
    GPE_CheckBoxBasic * linuxDebugModeCheckbox;

    GPE_RadioButtonControllerBasic * html5nativeBuildTypeOptions;
    GPE_RadioButtonControllerBasic * windowsNativeBuildTypeOptions;
    GPE_RadioButtonControllerBasic * osxNativeBuildTypeOptions;
    GPE_RadioButtonControllerBasic * linuxNativeBuildTypeOptions;
    GPE_RadioButtonControllerBasic * switchNativeBuildTypeOptions;
    GPE_Label_Text * electronCompileNote;
    GPE_TextURL * electronCompileUrl;

    GPE_Label_Paragraph * switchComingSoonNote;
    GPE_Label_Paragraph * nSwitchSideNote;
    GPE_Label_Paragraph * nSwitchThirdNote;
    GPE_TextURL * nSwitchContactUrl;

    GPE_Label_Paragraph * ps4ComingSoonNote;
    GPE_Label_Paragraph * ps4SideNote;
    GPE_TextURL * ps4ContactUrl;

    GPE_Label_Paragraph * xboxOneComingSoonNote;
    GPE_Label_Paragraph * xboxOneSideNote;
    GPE_TextURL * xboxOneContactUrl;

    //Other Platform label notes
    GPE_Label_Text * otherPlatformLabel;
    GPE_Label_Paragraph * otherPlatformLabelParagraph;
    GPE_TextURL * otherPlatformLabelURL;


    //Settings Tab
    gpe_text_widget_number * projectScreenWidthField;
    gpe_text_widget_number * projectScreenHeightField;
    gpe_text_widget_number * projectGameVersionField;
    gpe_text_widget_string * projectGameSubVersionField;
    GPE_RadioButtonControllerBasic * projectScaleSettings;
    GPE_Label_Text * projectSettingsFPSRateLabel;
    GPE_DropDown_Menu * projectSettingsFPSRate;
    //Publisher Setting
    GPE_Label_Title * sectionTitlePublisher;
    GPE_Label_Title * sectionTitleGameSettings;
    gpe_text_widget_string * projectGameTitleField;
    gpe_text_widget_string * projectGameShortTitleField;
    gpe_text_widget_string * projectGameDateField;
    gpe_text_widget_string * projectGameDescriptionField;
    gpe_text_widget_string * projectGamePublisherField;
    gpe_text_widget_string * projectGameDeveloperField;
    gpe_text_widget_string * projectGameCopyrightField;
    gpe_text_widget_string * projectGameWebsiteField;
    gpe_text_widget_string * projectGameEmailField;
    GPE_CheckBoxBasic * checkBoxShowPublisherInfo;

    //Colors Tab
    GPE_Label_Title * sectionWebColorsTitle;
    gpe_widget_color_picker * projectBorderColor;
    gpe_widget_color_picker * projectGameBackgroundColor;
    gpe_widget_color_picker * projectWebsiteBackgroundColor;
    gpe_widget_color_picker * projectTextHeaderColor;
    gpe_widget_color_picker * projectTextParagraphColor;
    gpe_widget_color_picker * projectTextLinkColor;
    gpe_widget_color_picker * projectTextLinkActiveColor;
    gpe_widget_color_picker * projectTextLinkHoverColor;
    gpe_widget_color_picker * projectTextLinkVisitedColor;
    gpe::shape_rect subViewedSpace;

    GPE_GuiElementList * projectSettingsList;
    GPE_GuiElementList * collisionLayerMatrixList;
    GPE_CheckBoxBasic * projectCollisionLayerMatrix[1024];
    GPE_Label_90Degree * projectLayerNamesColumnTitles[32];
    GPE_Label_Text * projectLayerNamesRowTitles[32];
    GPE_Label_Text * projectLayerEmptyTitle;
    GPE_ToolLabelButton * selectAllCollisionBoxes;
    GPE_ToolLabelButton * unselectAllCollisionBoxes;

    projectPropertiesResource(GPE_GeneralResourceContainer * pFolder = NULL);
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
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void refresh_build_to_source_targets();
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void save_resource(std::string file_path = "", int backupId = -1);
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    void update_project_layers();
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
