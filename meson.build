# SPDX-FileCopyrightText: 2018 - 2023 brainpower <brainpower@mailbox.org>
#
# SPDX-License-Identifier: MIT

project(
  'bp-nfoview',
  'cpp',
  default_options: ['cpp_std=c++14'],
  version: '0.4.0',
  license: 'GPL-2.0-or-later',
  meson_version: '>= 0.53.0'
)
static_build = get_option('static')

# {{{ files
sources = [
  'src/AboutDialog.cpp',
  'src/BPNVMainWindow.cpp',
  'src/qcodepage437codec.cpp',
  'src/bp-nfoview.cpp'
]
if static_build
  sources += 'src/qt_plugins_static.cpp'
endif

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

qt_versions = {
  'qt5': 'Qt5',
  'qt6': 'Qt6',
}

# this is needed for static builds
qt_plugins = {
  'QWebpPlugin': {
    'required': false,
    'section': 'imageformats',
    'libname': 'qwebp',
    'libs': ['libwebp', 'libwebpdemux', 'libwebpmux'],
    'qlibs': [],
  },
  'QJpegPlugin': {
    'required': false,
    'section': 'imageformats',
    'libname': 'qjpeg',
    'libs': ['libjpeg'],
    'qlibs': [],
  },
  'QGifPlugin': {
    'required': false,
    'section': 'imageformats',
    'libname': 'qgif',
    'libs': [],
    'qlibs': [],
  },
  #'QSvgIconPlugin': {'section': 'iconengines', 'libname': 'qsvgicon', 'qlibs': ['svg']},
  'QWaylandIntegrationPlugin': {
    'platforms': ['linux'], # must be one of host_machine.system() return values
    'required': false,
    'section': 'platforms',
    'libname': 'qwayland',
    'libs': [], # TODO
    'qlibs': [],
  },
  'QWindowsIntegrationPlugin': {
    'platforms': ['windows'], # must be one of host_machine.system() return values
    'required': true,
    'section': 'platforms',
    'libname': 'qwindows',
    'libs': ['wtsapi32'],
    'qlibs': [
      'WindowsUIAutomationSupport',
      'FontDatabaseSupport',
      'EventDispatcherSupport',
      'ThemeSupport',
    ],
  },
}
# }}}

# {{{ config
configuration_inc = include_directories('.')
cdata = configuration_data()
cdata.set_quoted('MAIN_CPYRGT_YEAR', '2010 - 2023')
cdata.set_quoted('MAIN_VERSION', meson.project_version())
#cdata.set('BINDIR', join_paths(get_option('prefix'), get_option('bindir'), '')) # the '' produces a trailing /, needed here
cdata.set('STATIC_BUILD', static_build)
# }}}

# {{{ dependencies
qt_ver = get_option('qt_version')

if qt_ver != 'auto'
  qt_versions = {qt_ver: qt_versions[qt_ver]}
endif

foreach qt_version, qt_verprefix : qt_versions
  # compare to 0.44 so meson does not complain about warning(), despite the meson_version parameter in project()
  mver = meson.version()
  if qt_version == 'qt6' and mver.version_compare('< 0.57.0') and mver.version_compare('>= 0.47.0')
    warning('Meson version @0@ is too old to support Qt6!'.format(mver))
    continue
  endif

  qt = import(qt_version)
  qt_dep = dependency(
    qt_version,
    modules: qt_modules,
    required: false,
    static: static_build,
    include_type: 'system' # so we dont get compiler warnings from inside QT headers
  )
  cdata.set('HAS_' + qt_version.to_upper(), qt_dep.found())

  if qt_dep.found()
    qt_exact_version = qt_dep.version()
    if qt_exact_version.version_compare('>= 6.0.0') # and qt_exact_version.version_compare('< 6.x.y')
      message('For this version of Qt6 an additional module is needed: Core5Compat')
      qt_modules += 'Core5Compat'
      qt_dep = dependency(
        qt_version,
        modules: qt_modules,
        required: false,
        static: static_build,
        include_type: 'system' # so we dont get compiler warnings from inside QT headers
      )
      # update cdata to not define HAS_QT6, if Core5Compat not found
      cdata.set('HAS_' + qt_version.to_upper(), qt_dep.found())
    endif
    if qt_dep.found()
      qt_found = qt_version
      qt_libprefix = qt_verprefix
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

static_deps = []
if static_build
  pkgc = find_program('pkgconfig')
  rc = run_command(pkgc, '--variable=prefix', qt_libprefix + 'Core')
  if rc.returncode() != 0
    qmake = find_program('qmake', 'qmake-' + qt_found)
    rc = run_command(qmake, '-query', 'QT_INSTALL_PREFIX')
    if rc.returncode() != 0
      error('failed to get qt prefix')
    endif
  endif
  plgdir = join_paths(rc.stdout().strip(), 'plugins')
  libdir = join_paths(rc.stdout().strip(), 'lib')
  message('Qt plugin dir: '+plgdir)
  message('Qt lib dir: '+libdir)

  cxx = meson.get_compiler('cpp')
  code = '''#include <QtCore/QtPlugin>
  Q_IMPORT_PLUGIN(@0@);
  int main(int,char**) {}'''

  foreach plugin, data : qt_plugins
    if 'platforms' in data and host_machine.system() not in data['platforms']
      continue # skip plugins not intended for this platform
    endif
    searchdir = join_paths(plgdir, data['section'])
    if mver.version_compare('>= 0.51.0')
      lib = cxx.find_library(data['libname'], dirs: searchdir, required: data['required'], static: true)
    else
      lib = find_library(data['libname'], dirs: searchdir, required: data['required'], static: true)
    endif
    if lib.found()
      allfound = true
      xdeps = [lib,]
      foreach xlibname : data['libs']
        xlib = dependency(xlibname, required: false, static: true)
        if xlib.found()
          xdeps += xlib
        else
          if mver.version_compare('>= 0.51.0')
            xlib = cxx.find_library(xlibname, dirs: libdir, required: data['required'], static: true)
          else
            xlib = find_library(xlibname, dirs: libdir, required: data['required'], static: true)
          endif
          if xlib.found()
            xdeps += xlib
          else
            allfound = false
          endif
        endif
      endforeach
      foreach qlibname : data['qlibs']
        if qt_dep.version().version_compare('>=6.0.0')
          # does not need these wit h Qt6
        else
          if mver.version_compare('>= 0.51.0')
            xdeps += cxx.find_library(qt_libprefix + qlibname, dirs: libdir, required: data['required'], static: true)
          else
            xdeps += find_library(qt_libprefix + qlibname, dirs: libdir, required: data['required'], static: true)
          endif
        endif
      endforeach
      if allfound
        res = cxx.links(code.format(plugin), dependencies: [qt_dep, xdeps], name: plugin)
        cdata.set('HAS_' + plugin.to_upper(), true)
        static_deps += xdeps
      endif
    endif
  endforeach
#elif static_build
#  error('static build needs meson >= 0.51 to be able to bundle qt plugins')
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
if static_build
  executable(
    'bp-nfoview',
    sources, mocs,
    dependencies: static_deps + [qt_dep],
    include_directories: configuration_inc,
    install: true,
    override_options: override_options,
    # ensure static linkage, so we dont silently produce a dynamicly linked binary
    link_args: '-Wl,--static',
  )
else
  executable(
    'bp-nfoview',
    sources, mocs,
    dependencies: static_deps + [qt_dep],
    include_directories: configuration_inc,
    install: true,
    override_options: override_options,
  )
endif

# }}}

# {{{ install data
# configure_file(
#   input:  'data/bp-nfoview.desktop.in',
#   output: 'bp-nfoview.desktop',
#   configuration: cdata,
# )
install_data(
  #join_paths(meson.build_root(), 'bp-nfoview.desktop'),
	'data/bp-nfoview.desktop',
	install_dir: join_paths(get_option('datadir'), 'applications')
)
install_data(
	'data/bp-nfoview.png',
	install_dir: join_paths(get_option('datadir'), 'pixmaps')
)
# }}}

if get_option('tests')
  dfv = find_program('desktop-file-validate', required: false)
  if dfv.found()
    test('validate desktop file', dfv, args: files(join_paths('data', 'bp-nfoview.desktop')))
  endif
endif
