project('game_pencil_engine', 'cpp')

inc = include_directories(['src', 'src/AOSGUI', 'src/GPE_Engine'])
sources = files()
subdir('src')

executable('game_pencil_engine', sources, 
  include_directories : inc,
  dependencies : dependency('sdl', version : '>=2.0.0')
  )
