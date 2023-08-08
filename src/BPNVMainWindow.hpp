// SPDX-FileCopyrightText: 2010 - 2023 brainpower <brainpower@mailbox.org>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef BP_NFOVIEW_BPNVMAINWINDOW_HPP
#define BP_NFOVIEW_BPNVMAINWINDOW_HPP

#include <memory>

#include <QMainWindow>
#include <QSize>
#include <QUrl>
#include <QMimeDatabase>

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
  bool loadFile(QUrl file);
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
  void onActionLineHeight();
  void onActionAbout();
  void onActionQuit();

  QSize sizeHint() const override;
  void closeEvent(QCloseEvent *event) override;

protected:
  void dragEnterEvent(QDragEnterEvent *e) override;
  void dropEvent(QDropEvent *e) override;

private:
  // FIXME: unique_ptr ?
  std::shared_ptr<QByteArray> rawFileData;
  bool isCP437 = true;

  void setupUi();

  QTextBrowser *textBrowser;
  QSettings *settings;


  QAction *actionOpen, *actionSaveImage, *actionQuit, *actionAbout, *actionStatusBar,
    *actionColor, *actionDefaultColor, *actionFont, *actionDefaultFont,
    *actionLineHeight, *actionCodecUTF8, *actionCodecCP437;
  QActionGroup *agCodec;

  QMenuBar *menubar;
  QMenu *menuFile, *menuView, *menuViewCodec, *menuHelp;
  QStatusBar *statusbar;

  QFont defaultFont;
  QFont currentFont;
  QPalette defaultPalette;
  QPalette currentPalette;

  QMimeDatabase mime_db;

private slots:
  void updateTextBrowser();
};

#endif  // BP_NFOVIEW_BPNVMAINWINDOW_HPP
