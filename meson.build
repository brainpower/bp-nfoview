
project('bp-nfoview', 'cpp', default_options: ['cpp_std=c++14'], version: '0.2.7')

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
# }}}

# {{{ config
configuration_inc = include_directories('.')
cdata = configuration_data()
cdata.set_quoted('MAIN_VERSION', meson.project_version())
# }}}

# {{{ dependencies
qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Widgets', 'Gui'])

if qt5_dep.found()
	cdata.set('BP_QT5', true)
endif

mocs = qt5.preprocess(
	moc_headers: moc_headers,
	qresources: qrc_files
)
# }}}


# {{{ executable
configure_file(
	output: 'config.h',
	configuration: cdata
)
executable(
	'bp-nfoview',
	sources, mocs,
	dependencies: qt5_dep,
	include_directories: configuration_inc,
	install: true
)
# }}}

# {{{ install data
install_data(
	'bp-nfoview.desktop',
	install_dir: join_paths(get_option('datadir'), 'applications')
)
install_data(
	'img/bp-nfoview.png',
	install_dir: join_paths(get_option('datadir'), 'pixmaps')
)
# }}}
