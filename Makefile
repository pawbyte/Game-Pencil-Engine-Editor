.POSIX:

########
# Vars #
########
CXXFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -I/usr/include/SDL2

.PHONY: build release debug



HEADERS = \
	src/animation_resource.h \
	src/AOSGUI/ambitious_gui_library.h \
	src/AOSGUI/gpe_syntax_handler.h \
	src/AOSGUI/pawi_gui_resource_dropdown.h \
	src/AOSGUI/paw_gui.h \
	src/AOSGUI/paw_gui_base.h \
	src/AOSGUI/paw_gui_buttons.h \
	src/AOSGUI/paw_gui_checkbox.h \
	src/AOSGUI/paw_gui_color_picker.h \
	src/AOSGUI/paw_gui_context.h \
	src/AOSGUI/paw_gui_dropdown.h \
	src/AOSGUI/paw_gui_file_popups.h \
	src/AOSGUI/paw_gui_labels.h \
	src/AOSGUI/paw_gui_overlay.h \
	src/AOSGUI/paw_gui_panel_list.h \
	src/AOSGUI/paw_gui_parsers.h \
	src/AOSGUI/paw_gui_popups.h \
	src/AOSGUI/paw_gui_radiobuttons.h \
	src/AOSGUI/paw_gui_resource_dropdown.h \
	src/AOSGUI/paw_gui_scrollbars.h \
	src/AOSGUI/paw_gui_search_controller.h \
	src/AOSGUI/paw_gui_selectbox.h \
	src/AOSGUI/paw_gui_sliders.h \
	src/AOSGUI/paw_gui_stree.h \
	src/AOSGUI/paw_gui_tab_basic.h \
	src/AOSGUI/paw_gui_tab_manager.h \
	src/AOSGUI/paw_gui_text_input.h \
	src/AOSGUI/paw_gui_themes.h \
	src/AOSGUI/text_editor.h \
	src/audio_resource.h \
	src/class_resource.h \
	src/dictionaryresource.h \
	src/fontResource.h \
	src/font_resource.h \
	src/function_resource.h \
	src/game_object_resource.h \
	src/game_scene_resource.h \
	src/gpe_basic_resource_page.h \
	src/gpe_cpp_compiler_settings.h \
	src/gpe_dock_system.h \
	src/gpe_editor.h \
	src/gpe_editor_aboutpage.h \
	src/gpe_editor_helppage.h \
	src/gpe_editor_settings.h \
	src/gpe_editor_start_page.h \
	src/GPE_Engine/GPE.h \
	src/GPE_Engine/GPE_Animation.h \
	src/GPE_Engine/GPE_Audio.h \
	src/GPE_Engine/GPE_Backgrounds.h \
	src/GPE_Engine/GPE_Basic_Object.h \
	src/GPE_Engine/GPE_Camera.h \
	src/GPE_Engine/GPE_CIncludes.h \
	src/GPE_Engine/GPE_Collisions.h \
	src/GPE_Engine/GPE_Color.h \
	src/GPE_Engine/GPE_Color_Manager.h \
	src/GPE_Engine/GPE_Color_Master.h \
	src/GPE_Engine/GPE_Constants.h \
	src/GPE_Engine/GPE_Engine.h \
	src/GPE_Engine/GPE_File_System.h \
	src/GPE_Engine/GPE_Fonts.h \
	src/GPE_Engine/GPE_Functions.h \
	src/GPE_Engine/GPE_Globals.h \
	src/GPE_Engine/GPE_Input.h \
	src/GPE_Engine/GPE_Layers.h \
	src/GPE_Engine/GPE_Lights.h \
	src/GPE_Engine/GPE_Particles.h \
	src/GPE_Engine/GPE_Paths.h \
	src/GPE_Engine/GPE_Program_State.h \
	src/GPE_Engine/GPE_Renderer.h \
	src/GPE_Engine/GPE_Resources_Controller.h \
	src/GPE_Engine/GPE_Scene.h \
	src/GPE_Engine/GPE_Shapes.h \
	src/GPE_Engine/GPE_Shared_Resources.h \
	src/GPE_Engine/GPE_Spatial_Partitions.h \
	src/GPE_Engine/GPE_State_Manager.h \
	src/GPE_Engine/GPE_String_Ex.h \
	src/GPE_Engine/GPE_Surface_Modifier.h \
	src/GPE_Engine/GPE_Texture.h \
	src/GPE_Engine/GPE_Tiles.h \
	src/GPE_Engine/GPE_Timer.h \
	src/GPE_Engine/GPE_Translator.h \
	src/gpe_extra_tools.h \
	src/gpe_gamecontroller_tester.h \
	src/gpe_js_compiler_settings.h \
	src/gpe_log_manager.h \
	src/gpe_project_resources.h \
	src/gpe_resource_tree.h \
	src/gpe_scene_animation_class.h \
	src/gpe_scene_background_class.h \
	src/gpe_scene_basic_class.h \
	src/gpe_scene_group_class.h \
	src/gpe_scene_helper_class.h \
	src/gpe_scene_layer_class.h \
	src/gpe_scene_light_class.h \
	src/gpe_scene_object_class.h \
	src/gpe_scene_particle_class.h \
	src/gpe_scene_text_class.h \
	src/gpe_scene_tilemap_class.h \
	src/light_resource.h \
	src/particle_resource.h \
	src/path_resource.h \
	src/project_properties.h \
	src/texture_resource.h \
	src/tilesheet_resource.h \
	src/video_resource.h \

OBJECTS = \
	src/animation_resource.o \
	src/AOSGUI/ambitious_gui_library.o \
	src/AOSGUI/gpe_syntax_handler.o \
	src/AOSGUI/pawi_gui_resource_dropdown.o \
	src/AOSGUI/paw_gui.o \
	src/AOSGUI/paw_gui_base.o \
	src/AOSGUI/paw_gui_buttons.o \
	src/AOSGUI/paw_gui_checkbox.o \
	src/AOSGUI/paw_gui_color_picker.o \
	src/AOSGUI/paw_gui_context.o \
	src/AOSGUI/paw_gui_dropdown.o \
	src/AOSGUI/paw_gui_file_popups.o \
	src/AOSGUI/paw_gui_labels.o \
	src/AOSGUI/paw_gui_overlay.o \
	src/AOSGUI/paw_gui_panel_list.o \
	src/AOSGUI/paw_gui_parsers.o \
	src/AOSGUI/paw_gui_popups.o \
	src/AOSGUI/paw_gui_radiobuttons.o \
	src/AOSGUI/paw_gui_resource_dropdown.o \
	src/AOSGUI/paw_gui_scrollbars.o \
	src/AOSGUI/paw_gui_search_controller.o \
	src/AOSGUI/paw_gui_selectbox.o \
	src/AOSGUI/paw_gui_sliders.o \
	src/AOSGUI/paw_gui_stree.o \
	src/AOSGUI/paw_gui_tab_basic.o \
	src/AOSGUI/paw_gui_tab_manager.o \
	src/AOSGUI/paw_gui_text_input.o \
	src/AOSGUI/paw_gui_themes.o \
	src/AOSGUI/text_editor.o \
	src/audio_resource.o \
	src/class_resource.o \
	src/dictionaryresource.o \
	src/fontResource.o \
	src/font_resource.o \
	src/function_resource.o \
	src/game_object_resource.o \
	src/game_scene_resource.o \
	src/gpe_basic_resource_page.o \
	src/gpe_cpp_compiler_settings.o \
	src/gpe_dock_system.o \
	src/gpe_editor.o \
	src/gpe_editor_aboutpage.o \
	src/gpe_editor_helppage.o \
	src/gpe_editor_settings.o \
	src/gpe_editor_start_page.o \
	src/GPE_Engine/GPE.o \
	src/GPE_Engine/GPE_Animation.o \
	src/GPE_Engine/GPE_Audio.o \
	src/GPE_Engine/GPE_Backgrounds.o \
	src/GPE_Engine/GPE_Basic_Object.o \
	src/GPE_Engine/GPE_Camera.o \
	src/GPE_Engine/GPE_CIncludes.o \
	src/GPE_Engine/GPE_Collisions.o \
	src/GPE_Engine/GPE_Color.o \
	src/GPE_Engine/GPE_Color_Manager.o \
	src/GPE_Engine/GPE_Color_Master.o \
	src/GPE_Engine/GPE_Constants.o \
	src/GPE_Engine/GPE_Engine.o \
	src/GPE_Engine/GPE_File_System.o \
	src/GPE_Engine/GPE_Fonts.o \
	src/GPE_Engine/GPE_Functions.o \
	src/GPE_Engine/GPE_Globals.o \
	src/GPE_Engine/GPE_Input.o \
	src/GPE_Engine/GPE_Layers.o \
	src/GPE_Engine/GPE_Lights.o \
	src/GPE_Engine/GPE_Particles.o \
	src/GPE_Engine/GPE_Paths.o \
	src/GPE_Engine/GPE_Program_State.o \
	src/GPE_Engine/GPE_Renderer.o \
	src/GPE_Engine/GPE_Resources_Controller.o \
	src/GPE_Engine/GPE_Scene.o \
	src/GPE_Engine/GPE_Shapes.o \
	src/GPE_Engine/GPE_Shared_Resources.o \
	src/GPE_Engine/GPE_Spatial_Partitions.o \
	src/GPE_Engine/GPE_State_Manager.o \
	src/GPE_Engine/GPE_String_Ex.o \
	src/GPE_Engine/GPE_Surface_Modifier.o \
	src/GPE_Engine/GPE_Texture.o \
	src/GPE_Engine/GPE_Tiles.o \
	src/GPE_Engine/GPE_Timer.o \
	src/GPE_Engine/GPE_Translator.o \
	src/gpe_extra_tools.o \
	src/gpe_gamecontroller_tester.o \
	src/gpe_js_compiler_settings.o \
	src/gpe_log_manager.o \
	src/gpe_project_resources.o \
	src/gpe_resource_tree.o \
	src/gpe_scene_animation_class.o \
	src/gpe_scene_background_class.o \
	src/gpe_scene_basic_class.o \
	src/gpe_scene_group_class.o \
	src/gpe_scene_helper_class.o \
	src/gpe_scene_layer_class.o \
	src/gpe_scene_light_class.o \
	src/gpe_scene_object_class.o \
	src/gpe_scene_particle_class.o \
	src/gpe_scene_text_class.o \
	src/gpe_scene_tilemap_class.o \
	src/light_resource.o \
	src/particle_resource.o \
	src/path_resource.o \
	src/project_properties.o \
	src/texture_resource.o \
	src/tilesheet_resource.o \
	src/video_resource.o \


BINARIES = Game_Pencil_Engine_IDE

#########
# Rules #
#########

# This is the first rule. It is therefore default
build: release

Game_Pencil_Engine_IDE: $(OBJECTS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

# Since I don't know what sources go to what headers, if a header changes
# assume everything needs to be rebuilt.
$(OBJECTS): $(HEADERS)

release: CXXFLAGS += -O2 -Wfatal-errors -Wextra
release: $(BINARIES)

debug: CXXFLAGS += -g
debug: $(BINARIES)

syntax: CXXFLAGS += --syntax-only
syntax: $(BINARIES)

clean:
	rm $(OBJECTS)
	rm $(BINARIES)
