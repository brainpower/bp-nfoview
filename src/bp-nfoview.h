/**#############################################################################
#                                                                              #
NAME = "bp-nfoview"                                                            #
AUTHOR = "brainpower@gulli.com"                                                #
VERSION = "0.1.2"                                                              #
LICENCE = "GPL-3"                                                              #
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
# Copyright (c) 2010-2013  brainpower@gulli.com                                #
#                                                                              #
#############################################################################**/

#ifndef BP_NFOVIEW_H
#define BP_NFOVIEW_H

#ifndef MAIN_VERSION
#define MAIN_VERSION "0.2.0"
#endif // VERSION

// ## Qt4 includes:
#include <QFile>
#include <QSettings>
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QTextBrowser>
#include <QWidget>
#include <QKeySequence>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QImage>
// ## own headers:
#include "qcodepage437codec.h"
#include "dialog_about.h"

QT_BEGIN_NAMESPACE
namespace Ui {

class Ui_MainWindow: public QMainWindow{
	Q_OBJECT
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
		QAction *actionCodecUTF8, *actionCodecCP437;
		QActionGroup *agCodec;
		QWidget *centralwidget;
		QTextBrowser *textEdit;
		QMenuBar *menubar;
		QMenu *menuFile;
		QMenu *menuView;
		QMenu *menuViewCodec;
		QMenu *menuHelp;
		QStatusBar *statusbar;
		QByteArray *raw;
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
		void switchCodecUTF8Action();
		void switchCodecCP437Action();
		void loadFile(QString);
		void saveImageAction();
}; // Ui_MainWindow

} // namespace Ui

QT_END_NAMESPACE

#endif // BP_NFOVIEW_H
