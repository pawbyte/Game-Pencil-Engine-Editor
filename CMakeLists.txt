cmake_minimum_required(VERSION 3.10)

# set the project name
project(gpe-editor VERSION 1.4.1)

# Configure GPE Version build
configure_file(src/gpe_editor/gpe_editor_constants.h.in src/gpe_editor/gpe_editor_constants.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Cmake modules
INCLUDE(FindPkgConfig)

# Find dependencies
find_package(SDL2 REQUIRED)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS})
file(GLOB gpe_main_SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
file(GLOB gpe_editor_SRC CONFIGURE_DEPENDS "src/gpe_editor/*.h" "src/gpe_editor/*.cpp")
file(GLOB gpe_SRC CONFIGURE_DEPENDS "src/gpe/*.h" "src/gpe/*.cpp")
file(GLOB otherlibs_SRC CONFIGURE_DEPENDS "src/other_libs/*.h" "src/other_libs/*.cpp")
file(GLOB pawgui_SRC CONFIGURE_DEPENDS "src/pawgui/*.h" "src/pawgui/*.cpp")
file(GLOB sdllibs_SRC CONFIGURE_DEPENDS "src/sdl_libs/*.h" "src/sdl_libs/*.cpp")
if (WIN32)
  file(GLOB dlgmodule_SRC CONFIGURE_DEPENDS "src/other_libs/dlgmodule.h" "src/other_libs/dlgmodule/win32/win32.cpp")
endif (WIN32)
if (UNIX)
  file(GLOB dlgmodule_SRC CONFIGURE_DEPENDS "src/other_libs/dlgmodule.h" "src/other_libs/dlgmodule/unix/unix.cpp")
endif (UNIX)
if (APPLE)
  file(GLOB dlgmodule_SRC CONFIGURE_DEPENDS "src/other_libs/dlgmodule.h" "src/other_libs/dlgmodule/macos/macos.cpp")
endif (APPLE)
# add the executable
add_executable(gpe-editor ${dlgmodule_SRC} ${gpe_main_SRC} ${gpe_editor_SRC} ${otherlibs_SRC} ${pawgui_SRC} ${sdllibs_SRC} ${gpe_SRC} )

# Populate project-specific includes
target_include_directories(gpe-editor PRIVATE src/gpe)

# Link SDL2
target_link_libraries(gpe-editor ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2MIXER_LIBRARIES})
