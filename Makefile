.POSIX:

########
# Vars #
########
CXXFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

.PHONY: build release debug

HEADERS = \
	src/project_properties.h \
	src/AOSGUI/ambitious_gui_library.h \
	src/AOSGUI/gpe_syntax_handler.h \
	src/AOSGUI/paw_gui.h \
	src/AOSGUI/text_editor.h \
	src/gameobjectresource.h \
	src/functionResource.h \
	src/gpe_editor_aboutpage.h \
	src/dictionaryresource.h \
	src/project_browser.h \
	src/fontResource.h \
	src/gpe_editor_helppage.h \
	src/GPE_Engine/GPE_Program_State.h \
	src/GPE_Engine/GPE_Shared_Resources.h \
	src/GPE_Engine/GPE_Renderer.h \
	src/GPE_Engine/GPE_Color.h \
	src/GPE_Engine/GPE_File_System.h \
	src/GPE_Engine/GPE_Globals.h \
	src/GPE_Engine/GPE_CIncludes.h \
	src/GPE_Engine/GPE_Input.h \
	src/GPE_Engine/GPE.h \
	src/GPE_Engine/GPE_Functions.h \
	src/GPE_Engine/GPE_Color_Manager.h \
	src/GPE_Engine/GPE_Timer.h \
	src/GPE_Engine/GPE_Collisions.h \
	src/GPE_Engine/GPE_Resources_Controller.h \
	src/GPE_Engine/GPE_Sprite.h \
	src/GPE_Engine/GPE_String_Ex.h \
	src/GPE_Engine/GPE_Engine.h \
	src/GPE_Engine/GPE_State_Manager.h \
	src/GPE_Engine/GPE_Color_Master.h \
	src/GPE_Engine/GPE_Constants.h \
	src/GPE_Engine/GPE_Texture.h \
	src/spriteresource.h \
	src/gpe_project_resources.h \
	src/gpe_editor_start_page.h \
	src/pathresource.h \
	src/gpe_editor.h \


OBJECTS = \
	src/AOSGUI/text_editor.o \
	src/AOSGUI/ambitious_gui_library.o \
	src/AOSGUI/gpe_syntax_handler.o \
	src/AOSGUI/paw_gui.o \
	src/gpe_editor_start_page.o \
	src/fontResource.o \
	src/gpe_editor.o \
	src/gameobjectresource.o \
	src/functionResource.o \
	src/pathresource.o \
	src/tilesheet_resource.o \
	src/gpe_editor_aboutpage.o \
	src/dictionaryresource.o \
	src/project_browser.o \
	src/gpe_editor_settings.o \
	src/GPE_Engine/GPE_Globals.o \
	src/GPE_Engine/GPE_String_Ex.o \
	src/GPE_Engine/GPE_Sprite.o \
	src/GPE_Engine/GPE_Functions.o \
	src/GPE_Engine/GPE_Input.o \
	src/GPE_Engine/GPE_State_Manager.o \
	src/GPE_Engine/GPE_Resources_Controller.o \
	src/GPE_Engine/GPE_Program_State.o \
	src/GPE_Engine/GPE_Color.o \
	src/GPE_Engine/GPE_File_System.o \
	src/GPE_Engine/GPE_Texture.o \
	src/GPE_Engine/GPE_Shared_Resources.o \
	src/GPE_Engine/GPE_Collisions.o \
	src/GPE_Engine/GPE_Color_Manager.o \
	src/GPE_Engine/GPE_Engine.o \
	src/GPE_Engine/GPE_Timer.o \
	src/GPE_Engine/GPE_Renderer.o \
	src/GPE_Engine/GPE_Color_Master.o \
	src/video_resource.o \
	src/gpe_project_resources.o \
	src/gamesceneresource.o \
	src/giant_popup_menus.o \
	src/audio_resource.o \
	src/project_properties.o \
	src/spriteresource.o \
	src/gpe_editor_helppage.o \
	src/main.o \
	src/textureresource.o \

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

clean:
	rm $(OBJECTS)
	rm $(BINARIES)
