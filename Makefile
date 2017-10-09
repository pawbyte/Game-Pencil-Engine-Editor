.POSIX:

########
# Vars #
########
CXXFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

.PHONY: build release debug

HEADERS = \
	src/input_manager.h \
	src/pathresource.h \
	src/program_state.h \
	src/project_properties.h \
	src/resources_controller.h \
	src/shared_resources.h \
	src/sprite.h \
	src/spriteresource.h \
	src/stateManager.h \
	src/string_ex.h \
	src/timer.h \
	src/GPE_Globals.h \
	src/GPE_Renderer.h \
	src/GPE_Texture.h \
	src/collision_checks.h \
	src/color_master.h \
	src/constants.h \
	src/dictionaryresource.h \
	src/fontResource.h \
	src/functionResource.h \
	src/functions.h \
	src/gameobjectresource.h \
	src/gpe_editor.h \
	src/gpe_editor_helppage.h \
	src/GPE_CIncludes.h \
	src/GPE_Color.h \
	src/GPE_Color_Manager.h \
	src/GPE_Engine.h

OBJECTS = \
	src/input_manager.o \
	src/main.o \
	src/pathresource.o \
	src/program_state.o \
	src/project_browser.o \
	src/project_properties.o \
	src/resources_controller.o \
	src/shared_resources.o \
	src/sprite.o \
	src/spriteresource.o \
	src/stateManager.o \
	src/string_ex.o \
	src/textureresource.o \
	src/tilesheet_resource.o \
	src/timer.o \
	src/video_resource.o \
	src/GPE_Globals.o \
	src/GPE_Renderer.o \
	src/GPE_Texture.o \
	src/audio_resource.o \
	src/collision_checks.o \
	src/color_master.o \
	src/dictionaryresource.o \
	src/fontResource.o \
	src/functionResource.o \
	src/functions.o \
	src/gameobjectresource.o \
	src/gamesceneresource.o \
	src/gpe_editor.o \
	src/gpe_editor_aboutpage.o \
	src/gpe_editor_helppage.o \
	src/gpe_editor_settings.o \
	src/GPE_Color.o \
	src/GPE_Color_Manager.o \
	src/GPE_Engine.o

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
