// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2010-2023  brainpower <brainpower@mailbox.org>


#include "config.hpp"

#include "AboutDialog.hpp"
#include "BPNVMainWindow.hpp"
#include "qcodepage437codec.h"

#include <QApplication>
#include <QCoreApplication>

// core
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFile>
#include <QFileInfo>
#include <QMimeData>
#include <QRegularExpression>
#include <QSettings>
#include <QTextCodec>

// gui
#include <QPainter>

// widgets
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextBrowser>

// dialogs
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>


BPNVMainWindow::BPNVMainWindow() { setupUi(); }
BPNVMainWindow::BPNVMainWindow(QStringList list) {
  setupUi();

  if (!list.isEmpty()) { loadFile(list[0]); }
}

BPNVMainWindow::~BPNVMainWindow() {
  // qDebug("MainWindow goes boom!");
}

bool
BPNVMainWindow::loadFile(QUrl file) {
  if (!file.isLocalFile()) return false;
  return loadFile(file.toLocalFile());
}

bool
BPNVMainWindow::loadFile(QString file) {
  isCP437 = true;  // most nfo files are CP437, so assume input file is
  QFile nfofile(file);

  if (!nfofile.exists()) return false;
  auto mtype = mime_db.mimeTypeForFile(file);
  if (!mtype.inherits("text/plain")) {
    // do not try to open non-text / binary files
    return false;
  }
  // TODO: these checks should trigger QMessageBoxes or something

  nfofile.open(QIODevice::ReadOnly | QIODevice::Text);
  rawFileData = std::make_shared<QByteArray>(nfofile.readAll());
  nfofile.close();

  // TODO: I should put a nicer encoding detection here...

  // this checks for UTF-8 BOM, if it's there, input is probably UTF-8.
  if (
    rawFileData->length() > 2 && (*rawFileData)[0] == (char)0xEF
    && (*rawFileData)[1] == (char)0xBB && (*rawFileData)[2] == (char)0xBF) {
    isCP437 = false;
  }

  if (isCP437) { actionCodecCP437->setChecked(true); }
  else {
    actionCodecUTF8->setChecked(true);  // radio button of codec selector
  }
  updateTextBrowser();

  // if statusbar is enabled, show a message.
  if (actionStatusBar->isChecked())
    statusbar->showMessage(QStringLiteral("'%1' loaded.").arg(file));

  return true;
}

void
BPNVMainWindow::updateTextBrowser() {
  // qDebug("updateTextBrowser()");
  if (rawFileData) {
    // qDebug("updateTextBrowser(): if rawFileData");

    QRegularExpression linkRE(
      QStringLiteral(R"x(([^"])((http|https)://([^()"'<>\s]*))([^"]))x"));
    QString text;

    if (isCP437) {
      if (!QTextCodec::availableCodecs().contains("IBM437")) {
        // if codec is not available on this system or Qt build,
        // use the included implementation
        // Qt4 doesn't have it, Qt5 sometimes (linux), sometimes not (OS X)

        // Qt will take ownership of this object and delete it
        // See: http://doc.qt.io/qt-5/qtextcodec.html#QTextCodec
        new QCodePage437Codec();
        qDebug("new 437 codec instance created");
      }
      auto *codec = QTextCodec::codecForName("IBM437");
      if (codec) { text = codec->toUnicode(*rawFileData); }
      else {
        qCritical("Could not load codec 'CP437' for unknown reason. This is a bug!");
        return;
      }
    }
    else { text = QString::fromUtf8(*rawFileData); }

    double lineHeight = settings->value("lineHeight", 1.0).toDouble();

    textBrowser->setHtml(
      QStringLiteral(
        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" "
        "\"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
        "<html><head><meta name=\"qrichtext\" content=\"1\" /><style "
        "type=\"text/css\">\n"
        "body, p, li, a { white-space: pre-wrap; line-height: %2; }\n"  // dont collapse
                                                                        // whitespace!
        "</style></head><body>%1</body></html>")
        .arg(text
               .toHtmlEscaped()  // avoid problems with <, >, & and "
               .replace(
                 QStringLiteral("\n"), QStringLiteral("<br>\n"))  // add HTML linebreaks
               .replace(
                 linkRE, QStringLiteral(
                           R"(\1<a href="\2">\2</a>\5)"))  // add a-Tags around urls
             // TODO this is pretty dump url detection, maybe there are better ways to
             // do this?
             )
        .arg(lineHeight));
  }
}

void
BPNVMainWindow::saveAsImage(QString filepath) {
  QString str = textBrowser->toPlainText();
  QFontMetrics fm(currentFont);
  QSize textSize = fm.size(0, str);
  QImage pixmap(textSize, QImage::Format_RGB32);
  QPalette pal(textBrowser->palette());
  QBrush bpen(pal.color(QPalette::Base));
  QPen fpen(pal.color(QPalette::Text));
  auto *pa = new QPainter(&pixmap);

  // pa->begin(&pixmap);
  pa->setFont(currentFont);
  pa->setBrush(bpen);
  pa->drawRect(0, 0, textSize.width(), textSize.height());
  pa->setPen(fpen);
  // pa->setBackground(bpen);
  // pa->setBackgroundMode(Qt::OpaqueMode);
  pa->drawText(
    0, fm.height(), textSize.width(), textSize.height(),
    Qt::AlignLeft | Qt::TextIncludeTrailingSpaces, str);
  // pa->drawLine(0,140,320,140);
  pa->end();
  pixmap.save(filepath);
  delete pa;
}

void
BPNVMainWindow::closeEvent(QCloseEvent *event) {
  settings->setValue(QStringLiteral("geometry"), saveGeometry());
  settings->setValue(QStringLiteral("windowState"), saveState());
  settings->setValue(QStringLiteral("statusbar"), actionStatusBar->isChecked());
  QMainWindow::closeEvent(event);
}

QSize
BPNVMainWindow::sizeHint() const {
  return QSize(
           settings->value(QStringLiteral("width"), 1024).toInt(),
           settings->value(QStringLiteral("height"), 1280).toInt())
    .expandedTo(minimumSizeHint());
}

void
BPNVMainWindow::onActionStatusBar(bool checked) {
  if (checked) {
    statusbar->height();
    statusbar->show();
  }
  else { statusbar->hide(); }
}

void
BPNVMainWindow::onActionOpen() {
  auto recent =
    settings->value(QStringLiteral("GUI/recentDir"), QStringLiteral("")).toString();
  auto file = QFileDialog::getOpenFileName(
    this,
    QStringLiteral("Open .nfo file..."),  // title
    recent,                               // starting point in FS hierarchy
    QStringLiteral("nfo Files (*.nfo)")   // filter
  );
  if (!file.isEmpty()) {
    settings->setValue(
      QStringLiteral("GUI/recentDir"), QFileInfo(file).absoluteDir().absolutePath());
    loadFile(file);
  }
}

void
BPNVMainWindow::onActionSaveImage() {
  auto recent =
    settings->value(QStringLiteral("GUI/recentDir"), QStringLiteral("")).toString();
  auto file = QFileDialog::getSaveFileName(
    this, QStringLiteral("Save to..."), recent,
    QStringLiteral("Images (*.jpg *.jpeg *.png *.tiff *.bmp *.xpm *.ppm *.xbm)"));
  if (!file.isEmpty()) {
    settings->setValue(
      QStringLiteral("GUI/recentDir"), QFileInfo(file).absoluteDir().absolutePath());
    saveAsImage(file);
  }
}


void
BPNVMainWindow::onActionColor() {
  auto p = textBrowser->palette();

  auto bg = QColorDialog::getColor(
    p.color(QPalette::Base), this, QStringLiteral("Choose Background Color"));
  auto fg = QColorDialog::getColor(
    p.color(QPalette::WindowText), this, QStringLiteral("Choose Text Color"));

  p.setColor(QPalette::Base, bg);
  p.setColor(QPalette::WindowText, fg);
  p.setColor(QPalette::Text, fg);

  textBrowser->setPalette(p);
  textBrowser->setAutoFillBackground(true);
  textBrowser->repaint();

  settings->setValue(QStringLiteral("GUI/colors"), p);
}


void
BPNVMainWindow::onActionDefaultColor() {
  textBrowser->setPalette(defaultPalette);
  textBrowser->repaint();

  settings->remove(QStringLiteral("GUI/colors"));
}


void
BPNVMainWindow::onActionFont() {
  bool ok   = false;
  auto font = QFontDialog::getFont(
    &ok, textBrowser->font(), this, QStringLiteral("Select Font"),
    QFontDialog::MonospacedFonts);
  if (ok) {
    textBrowser->setFont(font);
    textBrowser->repaint();

    currentFont = font;
    settings->setValue(QStringLiteral("GUI/font"), font);
  }
}


void
BPNVMainWindow::onActionDefaultFont() {
  textBrowser->setFont(defaultFont);
  settings->remove(QStringLiteral("GUI/font"));
}


void
BPNVMainWindow::onActionSwitchToUTF8() {
  isCP437 = false;
  updateTextBrowser();
}


void
BPNVMainWindow::onActionSwitchToCP437() {
  isCP437 = true;
  updateTextBrowser();
}


void
BPNVMainWindow::onActionLineHeight() {
  bool ok;
  const double old = settings->value(QStringLiteral("lineHeight"), 1.0).toDouble();
  const double lineHeight = QInputDialog::getDouble(
    this,
    QStringLiteral("Set line height..."),  // title
    QStringLiteral("Line Height:"),        // label
    old,                                   // default value
    0.0,                                   // minimum
    2.0,                                   // maximum
    2,                                     // decimals
    &ok, Qt::WindowFlags()
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
           ,
    0.01  // stepping
#endif
  );
  settings->setValue(QStringLiteral("lineHeight"), lineHeight);
  updateTextBrowser();
}


void
BPNVMainWindow::onActionAbout() {
  AboutDialog ad(this);
  ad.exec();
}

void
BPNVMainWindow::onActionQuit() {
  QApplication::closeAllWindows();
}

void
BPNVMainWindow::dragEnterEvent(QDragEnterEvent *e) {
  auto data = e->mimeData();

  if (data->hasFormat("text/uri-list")) {
    e->acceptProposedAction();
    return;
  }
  if (data->hasFormat("application/vnd.portal.filetransfer")) {
    // got dropped some file, this should be ok, too
    e->acceptProposedAction();
    return;
  }

  qDebug() << "== MW::dragEnterEvent(): unknown drag format, what are you trying to "
              "drop here?";
  qDebug() << e;
  qDebug() << data;
  qDebug() << data->formats();
}

void
BPNVMainWindow::dropEvent(QDropEvent *e) {
  if (e->mimeData()->hasUrls()) {
    // this is always true for "text/uri-list". Others are unknown.
    auto urls = e->mimeData()->urls();
    if (!urls.empty()) {
      if (loadFile(urls.first())) {
        e->acceptProposedAction();
        return;
      }
      else {
        // file loading was not successful. return without accepting drop
        return;
      }
    }
  }

  qDebug() << "BUG: drop event does not have any urls. No idea how to handle this.\nIf "
              "you see this message, file a bug report with data below and information "
              "about what you were trying to do.";
  qDebug() << " event:" << e;
  qDebug() << " data:" << e->mimeData();
}


void
BPNVMainWindow::setupUi() {
  settings = new QSettings(
    QSettings::IniFormat, QSettings::UserScope, QStringLiteral("brainpower"),
    QStringLiteral("bp-nfoview"), this);
  settings->setDefaultFormat(QSettings::IniFormat);

  // Widgets
  textBrowser = new QTextBrowser(this);


  // Actions

  // Menu -> File
  actionOpen      = new QAction(this);
  actionSaveImage = new QAction(this);
  actionQuit      = new QAction(this);
  // Menu -> View
  actionStatusBar    = new QAction(this);
  actionColor        = new QAction(this);
  actionDefaultColor = new QAction(this);
  actionFont         = new QAction(this);
  actionDefaultFont  = new QAction(this);
  actionLineHeight   = new QAction(this);
  // Menu -> View -> Codec
  agCodec          = new QActionGroup(this);
  actionCodecUTF8  = new QAction(this);
  actionCodecCP437 = new QAction(this);
  // Menu -> Help
  actionAbout = new QAction(this);

  // Menu
  menubar       = new QMenuBar(this);
  menuFile      = new QMenu(menubar);
  menuView      = new QMenu(menubar);
  menuViewCodec = new QMenu(menuView);
  menuHelp      = new QMenu(menubar);

  // StatusBar
  statusbar = new QStatusBar(this);

  // Actions
  actionOpen->setIcon(QIcon::fromTheme(QStringLiteral("document-open")));
  actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit")));
  actionSaveImage->setIcon(QIcon::fromTheme(QStringLiteral("document-save-as")));
  actionFont->setIcon(QIcon::fromTheme(QStringLiteral("fonts-package")));
  actionDefaultFont->setIcon(QIcon::fromTheme(QStringLiteral("fonts-package")));
  actionColor->setIcon(QIcon::fromTheme(QStringLiteral("fill-color")));
  actionDefaultColor->setIcon(QIcon::fromTheme(QStringLiteral("fill-color")));
  actionAbout->setIcon(QIcon::fromTheme(QStringLiteral("help-about")));

  actionOpen->setShortcut(QKeySequence(QKeySequence::Open));
  actionQuit->setShortcut(QKeySequence(QKeySequence::Quit));

  actionStatusBar->setCheckable(true);
  actionCodecUTF8->setCheckable(true);
  actionCodecCP437->setCheckable(true);

  actionCodecUTF8->setActionGroup(agCodec);
  actionCodecCP437->setActionGroup(agCodec);

  actionCodecCP437->setChecked(true);

  // default Font
  defaultFont.setFamily(QStringLiteral("DejaVu Sans Mono"));
  defaultFont.setPointSize(10);
  defaultFont.setStyleHint(QFont::Monospace);

  currentPalette =
    settings->value(QStringLiteral("GUI/colors"), defaultPalette).value<QPalette>();
  currentFont = settings->value(QStringLiteral("GUI/font"), defaultFont).value<QFont>();

  // TextBrowser
  textBrowser->setAcceptDrops(false);
  textBrowser->setFrameShape(QFrame::NoFrame);
  textBrowser->setFrameShadow(QFrame::Plain);
  textBrowser->setUndoRedoEnabled(false);
  textBrowser->setReadOnly(true);
  textBrowser->setLineWrapMode(QTextEdit::NoWrap);
  textBrowser->setOpenExternalLinks(true);

  textBrowser->setPalette(currentPalette);
  textBrowser->setFont(currentFont);

  // Menus
  menubar->addAction(menuFile->menuAction());
  menubar->addAction(menuView->menuAction());
  menubar->addAction(menuHelp->menuAction());
  // Menu -> File
  menuFile->addAction(actionOpen);
  menuFile->addAction(actionSaveImage);
  menuFile->addAction(actionQuit);
  // Menu -> View
  menuView->addAction(actionStatusBar);
  menuView->addAction(actionColor);
  menuView->addAction(actionDefaultColor);
  menuView->addAction(actionFont);
  menuView->addAction(actionDefaultFont);
  menuView->addAction(actionLineHeight);
  menuView->addAction(menuViewCodec->menuAction());
  // Menu -> View -> Codec
  menuViewCodec->addAction(actionCodecCP437);
  menuViewCodec->addAction(actionCodecUTF8);
  // Menu -> Help
  menuHelp->addAction(actionAbout);


  menuFile->setTitle(QStringLiteral("File"));
  actionOpen->setText(QStringLiteral("Open..."));
  actionSaveImage->setText(QStringLiteral("Save as Image..."));
  actionQuit->setText(QStringLiteral("Quit"));

  menuView->setTitle(QStringLiteral("View"));
  actionStatusBar->setText(QStringLiteral("Show Statusbar"));
  actionColor->setText(QStringLiteral("Select custom BG/FG color..."));
  actionDefaultColor->setText(QStringLiteral("Restore default color"));
  actionFont->setText(QStringLiteral("Select custom Font..."));
  actionDefaultFont->setText(QStringLiteral("Restore default font"));
  actionLineHeight->setText(QStringLiteral("Select line height..."));

  menuViewCodec->setTitle(QStringLiteral("Encoding"));
  actionCodecCP437->setText(QStringLiteral("CP 437"));
  actionCodecUTF8->setText(QStringLiteral("UTF-8"));

  menuHelp->setTitle(QStringLiteral("Help"));
  actionAbout->setText(QStringLiteral("About..."));

  setAcceptDrops(true);
  setMenuBar(menubar);
  setStatusBar(statusbar);
  setCentralWidget(textBrowser);

  // statusbar->setBackgroundRole(QPalette::Highlight);
  // statusbar->setAutoFillBackground(true);

  // connects
  connect(actionOpen, SIGNAL(triggered()), this, SLOT(onActionOpen()));
  connect(actionSaveImage, SIGNAL(triggered()), this, SLOT(onActionSaveImage()));
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(onActionQuit()));

  connect(actionStatusBar, SIGNAL(toggled(bool)), this, SLOT(onActionStatusBar(bool)));
  connect(actionColor, SIGNAL(triggered()), this, SLOT(onActionColor()));
  connect(actionDefaultColor, SIGNAL(triggered()), this, SLOT(onActionDefaultColor()));
  connect(actionFont, SIGNAL(triggered()), this, SLOT(onActionFont()));
  connect(actionDefaultFont, SIGNAL(triggered()), this, SLOT(onActionDefaultFont()));
  connect(actionLineHeight, SIGNAL(triggered()), this, SLOT(onActionLineHeight()));

  connect(actionCodecUTF8, SIGNAL(triggered()), this, SLOT(onActionSwitchToUTF8()));
  connect(actionCodecCP437, SIGNAL(triggered()), this, SLOT(onActionSwitchToCP437()));

  connect(actionAbout, SIGNAL(triggered()), this, SLOT(onActionAbout()));

  // restore window state and geometry
  restoreGeometry(settings->value(QStringLiteral("geometry")).toByteArray());
  restoreState(settings->value(QStringLiteral("windowState")).toByteArray());

  // restor StatusBar state
  bool statusBarEnabled = settings->value(QStringLiteral("statusbar"), true).toBool();
  actionStatusBar->setChecked(statusBarEnabled);
  onActionStatusBar(statusBarEnabled);
}
