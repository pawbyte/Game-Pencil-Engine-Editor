project('GamePencil', 'cpp',
    version: '1.0',
    default_options: ['cpp_std=c++11', 'warning_level=3'])

# Dependencies
sdl2_dep = dependency('sdl2', version: '>=2.0.0', required: true)
sdl2_image_dep = dependency('sdl2_image', version: '>=2.0.0', required: true)
sdl2_mixer_dep = dependency('sdl2_mixer', version: '>=2.0.0', required: true)
sdl2_ttf_dep = dependency('sdl2_ttf', version: '>=2.0.0', required: true)
x11_dep = dependency('x11', required: host_machine.system() != 'windows')

# Include directories
inc = include_directories('.')

# Get all source files
find_program = find_program('find')
cpp_files = run_command(find_program, meson.source_root(), '-name', '*.cpp').stdout().strip().split()

# Exclude platform-specific files
filtered_cpp_files = []
foreach f : cpp_files
    if host_machine.system() == 'darwin' and not f.contains('win32')
        filtered_cpp_files += f
    elif host_machine.system() == 'windows' and not f.contains('macos') and not f.contains('unix')
        filtered_cpp_files += f
    elif host_machine.system() == 'linux' and not f.contains('win32') and not f.contains('macos')
        filtered_cpp_files += f
    endif
endforeach

cpp_files = filtered_cpp_files

# Executable
executable(
    'GamePencil',
    cpp_files,
    include_directories: inc,
    dependencies: [sdl2_dep, sdl2_image_dep, sdl2_mixer_dep, sdl2_ttf_dep, x11_dep]
)