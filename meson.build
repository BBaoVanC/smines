project(
  'smines',
  'c',
  version: '0.1.0',
)

ncurses_dep = dependency('ncurses')

srcs = [
  'draw.c',
  'help.c',
  'helper.c',
  'minefield.c',
  'undo.c',
  'window.c',
  'smines.c',
]

conf_data = configuration_data()
configure_file(input: 'config.def.h', output: 'config.h', configuration: conf_data)

executable('smines', srcs, dependencies: [ncurses_dep], install: true)
