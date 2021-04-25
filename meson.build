
project(
  'bp-nfoview',
  'cpp',
  default_options: ['cpp_std=c++14'],
  version: '0.2.8',
  meson_version: '>= 0.44.0'
)

# {{{ files
sources = [
  'src/AboutDialog.cpp',
  'src/BPNVMainWindow.cpp',
  'src/qcodepage437codec.cpp',
  'src/bp-nfoview.cpp'
]
moc_headers = [
  'src/AboutDialog.hpp',
  'src/BPNVMainWindow.hpp',
  #'src/qcodepage437codec.h',
]
qrc_files = [
  'src/bp-nfoview.qrc'
]

qt_modules = [
  'Core',
  'Gui',
  'Widgets',
]

qt_versions = [
  'qt5',
  'qt6',
]
# }}}

# {{{ config
configuration_inc = include_directories('.')
cdata = configuration_data()
cdata.set_quoted('MAIN_VERSION', meson.project_version())
cdata.set('BINDIR', get_option('bindir'))
# }}}

# {{{ dependencies
qt_ver = get_option('qt_version')

if qt_ver != 'auto'
  qt_versions = [qt_ver]
endif

foreach qt_version : qt_versions
  # compare to 0.44 so meson does not complain about warning(), despite the meson_version parameter in project()
  mver = meson.version()
  if qt_version == 'qt6' and mver.version_compare('< 0.57.0') and mver.version_compare('>= 0.44.0')
    warning('Meson version @0@ is too old to support Qt6!'.format(mver))
    continue
  endif

  qt = import(qt_version)
  qt_dep = dependency(qt_version, modules: qt_modules, required: false)
  cdata.set('HAS_' + qt_version.to_upper(), qt_dep.found())

  if qt_dep.found()
    qt_exact_version = qt_dep.version()
    if qt_exact_version.version_compare('>= 6.0.0') # and qt_exact_version.version_compare('< 6.x.y')
      message('For this version of Qt6 an additional module is needed: Core5Compat')
      qt_modules += 'Core5Compat'
      qt_dep = dependency(qt_version, modules: qt_modules, required: false)
      # update cdata to not define HAS_QT6, if Core5Compat not found
      cdata.set('HAS_' + qt_version.to_upper(), qt_dep.found())
    endif
    if qt_dep.found()
      break
    endif
  endif
endforeach

if not qt_dep.found()
  error('Any Qt of the following is required!', qt_versions)
endif
if qt_dep.version().version_compare('>=6.0.0')
  # qt6 requires c++17 or newer, bp-nfoviews code is c++14 or newer right now
  override_options = ['cpp_std=c++17']
else
  override_options = []
endif


mocs = qt.preprocess(
	moc_headers: moc_headers,
	qresources: qrc_files
)
# }}}


# {{{ executable
configure_file(
	output: 'config.hpp',
	configuration: cdata
)
executable(
	'bp-nfoview',
	sources, mocs,
	dependencies: qt_dep,
	include_directories: configuration_inc,
	install: true,
	override_options: override_options,
)
# }}}

# {{{ install data
#configure_file(
#  input:  'data/bp-nfoview.desktop.in',
#  output: 'data/bp-nfoview.desktop',
#  configuration: cdata,
#)
install_data(
	'data/bp-nfoview.desktop',
	install_dir: join_paths(get_option('datadir'), 'applications')
)
install_data(
	'data/bp-nfoview.png',
	install_dir: join_paths(get_option('datadir'), 'pixmaps')
)
# }}}
