/**#############################################################################
#                                                                              #
NAME = "bp-nfoview"                                                            #
AUTHOR = "brainpower@gulli.com"                                                #
VERSION = "0.1.0"                                                              #
LICENCE = "GPL v2"                                                             #
DESC = 'A simple lightweight nfo-viewer written in C++ with Qt4 Interface'     #
#                                                                              #
# This program comes with ABSOLUTELY NO WARRANTY                               #
#                                                                              #
# This program is free software; you can redistribute it and/or modify it      #
# under the terms of the GNU General Public License as published by the        #
# Free Software Foundation; either version 2 of the License, or (at your       #
# option) any later version.                                                   #
#                                                                              #
# This program is distributed in the hope that it will be useful, but          #
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY   #
# or FITNESS FOR A PARTICULAR PURPOSE.                                         #
# See the GNU General Public License for more details.                         #
# You should have received a copy of the GNU General Public License along with #
# this program; if not, see <http://www.gnu.org/licenses/>.                    #
#                                                                              #
# Copyright (c) 2010  brainpower@gulli.com                                     #
#                                                                              #
#############################################################################**/
/********************************************************************************
** original Form generated from reading UI file 'bp-nfoview.ui'
**
** Created: Sat May 21 17:38:08 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BP_NFOVIEW_H
#define BP_NFOVIEW_H

#ifndef MAIN_VERSION
#define MAIN_VERSION "0.1.0"
#endif // VERSION

// ## Qt4 includes:
//#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QSettings>

#include <QtGui/QAction>
#include <QtGui/QApplication>
//#include <QtGui/QButtonGroup>
//#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include <QtGui/QKeySequence>
#include <QtGui/QFileDialog>
#include <QtGui/QColorDialog>
#include <QtGui/QFontDialog>
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
// ## own headers:
#include "qcodepage437codec.h"
#include "dialog_about.h"

QT_BEGIN_NAMESPACE
namespace Ui {

class Ui_MainWindow: public QMainWindow{	Q_OBJECT
public:
		// QObjects
		QAction *actionOpen;
		QAction *actionSaveImage;
		QAction *actionQuit;
		QAction *actionAbout;
		QAction *actionSB;
		QAction *actionColor;
		QAction *actionDefaultColor;
		QAction *actionFont;
		QAction *actionDefaultFont;
		QWidget *centralwidget;
		QTextEdit *textEdit;
		QMenuBar *menubar;
		QMenu *menuFile;
		QMenu *menuView;
		QMenu *menuHelp;
		QStatusBar *statusbar;
		QPalette defaultPalette;
		QFont defaultFont;
		QFont cFont;
		// attributes
		short sboffset;
		// methods
		void setupUi();
		void retranslateUi();
    void updateGeometries();
    void resizeEvent(QResizeEvent*);
    void saveAsImage(QString);

public slots:
		void SBaction(bool);
		void OpenAction();
		void AboutAction();
		void ColorAction();
		void DefaultColorAction();
		void FontAction();
		void DefaultFontAction();
		void loadFile(QString);
		void saveImageAction();
}; // Ui_MainWindow

} // namespace Ui

QT_END_NAMESPACE

#endif // BP_NFOVIEW_H
