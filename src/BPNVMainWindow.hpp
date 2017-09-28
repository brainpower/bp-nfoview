/**#############################################################################
#                                                                              #
# PROJECT = "bp-nfoview"                                                       #
# LICENCE = "GPL"                                                              #
#                                                                              #
# Copyright (c) 2010-2017  brainpower <brainpower@mailbox.org>                 #
#                                                                              #
# This file is part of bp-nfoview.                                             #
#                                                                              #
# bp-nfoview is free software: you can redistribute it and/or modify           #
# it under the terms of the GNU General Public License as published by         #
# the Free Software Foundation, either version 2 of the License, or            #
# (at your option) any later version.                                          #
#                                                                              #
# bp-nfoview is distributed in the hope that it will be useful,                #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                         #
# See the GNU General Public License for more details.                         #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with bp-nfoview.  If not, see <http://www.gnu.org/licenses/>.          #
#                                                                              #
#############################################################################**/

#ifndef BP_NFOVIEW_BPNVMAINWINDOW_HPP
#define BP_NFOVIEW_BPNVMAINWINDOW_HPP

#include <memory>

#include <QMainWindow>
#include <QSize>

class QTextBrowser;
class QSettings;
class QSize;
class QAction;
class QActionGroup;
class QMenu;
class QMenuBar;
class QStatusBar;

class BPNVMainWindow : public QMainWindow {
	Q_OBJECT

public:
	BPNVMainWindow();
	explicit BPNVMainWindow(QStringList list);
	~BPNVMainWindow() override;

public slots:
	bool loadFile(QString file);
	void saveAsImage(QString qString);

	void onActionStatusBar(bool);
	void onActionOpen();
	void onActionSaveImage();
	void onActionColor();
	void onActionDefaultColor();
	void onActionFont();
	void onActionDefaultFont();
	void onActionSwitchToUTF8();
	void onActionSwitchToCP437();
	void onActionAbout();
	void onActionQuit();

	QSize sizeHint() const override;

private:
	std::shared_ptr<QByteArray> rawFileData;

	void setupUi();

	QTextBrowser *textBrowser;
	QSettings *settings;



	QAction *actionOpen,
			    *actionSaveImage,
	        *actionQuit,
	        *actionAbout,
	        *actionStatusBar,
	        *actionColor, *actionDefaultColor,
	        *actionFont,  *actionDefaultFont,
	        *actionCodecUTF8, *actionCodecCP437;
	QActionGroup *agCodec;

	QMenuBar *menubar;
	QMenu *menuFile,
	      *menuView,
	      *menuViewCodec,
	      *menuHelp;
	QStatusBar *statusbar;

	QFont defaultFont;
	QFont currentFont;
	QPalette defaultPalette;
	QPalette currentPalette;




private slots:
	void updateTextBrowser(bool isCP437);
};

#endif //BP_NFOVIEW_BPNVMAINWINDOW_HPP
