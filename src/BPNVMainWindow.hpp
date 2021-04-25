// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2010-2021  brainpower <brainpower@mailbox.org>


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
	void  closeEvent(QCloseEvent *event) override;

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
