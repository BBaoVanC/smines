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

executable('smines', srcs, dependencies: [ncurses_dep])
