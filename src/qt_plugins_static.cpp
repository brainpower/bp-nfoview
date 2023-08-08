// SPDX-FileCopyrightText: 2021 brainpower <brainpower@mailbox.org>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "config.hpp"

#ifdef STATIC_BUILD
#include <QtCore/QtPlugin>

#ifdef HAS_QWEBPPLUGIN
Q_IMPORT_PLUGIN(QWebpPlugin)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
#ifdef HAS_QJPEGPLUGIN
Q_IMPORT_PLUGIN(QJpegPlugin)
#endif
#ifdef HAS_QGIFPLUGIN
Q_IMPORT_PLUGIN(QGifPlugin)
#endif
#endif // Qt 5.8.0

#ifdef Q_OS_WIN
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif defined Q_OS_MAC // Q_OS_WIN
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
Q_IMPORT_PLUGIN(QGenericEnginePlugin)
#elif defined Q_OS_UNIX // Q_OS_WIN | Q_OS_MAC
#ifdef HAS_QXCBINTEGRATIONPLUGIN
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QXcbEglIntegrationPlugin)
Q_IMPORT_PLUGIN(QXcbGlxIntegrationPlugin)
#endif
#ifdef HAS_QGENERICENGINEPLUGIN
Q_IMPORT_PLUGIN(QGenericEnginePlugin)
#endif
#ifdef HAS_QCOMPOSEPLATFORMINPUTCONTEXTPLUGIN
Q_IMPORT_PLUGIN(QComposePlatformInputContextPlugin)
#endif
#ifdef HAS_QSVGICONPLUGIN
Q_IMPORT_PLUGIN(QSvgIconPlugin)
#endif
#ifdef HAS_QIBUSPLATFORMINPUTCONTEXTPLUGIN
Q_IMPORT_PLUGIN(QIbusPlatformInputContextPlugin)
#endif
#ifdef HAS_QXDGDESKTOPPORTALTHEMEPLUGIN
Q_IMPORT_PLUGIN(QXdgDesktopPortalThemePlugin)
#endif
#ifdef HAS_QWAYLANDINTEGRATIONPLUGIN
Q_IMPORT_PLUGIN(QWaylandEglClientBufferPlugin)
Q_IMPORT_PLUGIN(QWaylandWlShellIntegrationPlugin)
Q_IMPORT_PLUGIN(QWaylandXdgShellIntegrationPlugin)
Q_IMPORT_PLUGIN(QWaylandBradientDecorationPlugin)
Q_IMPORT_PLUGIN(QWaylandIntegrationPlugin)
Q_IMPORT_PLUGIN(QWaylandEglPlatformIntegrationPlugin)
#endif // HAS_QWAYLAND
#endif // Q_OS_WIN | Q_OS_MAC | Q_OS_UNIX

#ifdef HAS_QT5CTPLATFORMTHEMEPLUGIN
Q_IMPORT_PLUGIN(Qt5CTPlatformThemePlugin)
Q_IMPORT_PLUGIN(Qt5CTStylePlugin)
#endif // HAS_QT5CTPLATFORMTHEMEPLUGIN
#endif // STATIC_BUILD

