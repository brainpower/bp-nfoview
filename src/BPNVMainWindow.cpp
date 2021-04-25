// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2010-2021  brainpower <brainpower@mailbox.org>


#include "config.hpp"

#include "BPNVMainWindow.hpp"
#include "qcodepage437codec.h"
#include "AboutDialog.hpp"

#include <QApplication>
#include <QCoreApplication>

// core
#include <QFile>
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


BPNVMainWindow::BPNVMainWindow() {
	setupUi();
}
BPNVMainWindow::BPNVMainWindow(QStringList list) {
	setupUi();

	if(!list.isEmpty()){
		loadFile(list[0]);
	}
}

BPNVMainWindow::~BPNVMainWindow() {
	//qDebug("MainWindow goes boom!");
}

bool BPNVMainWindow::loadFile(QString file) {
	bool isCP437 = true; // most nfo files are CP437, so assume input file is
	QFile nfofile(file);

	if(!nfofile.exists())
		return false;

	nfofile.open(QIODevice::ReadOnly | QIODevice::Text);
	rawFileData = std::make_shared<QByteArray>(nfofile.readAll());
	nfofile.close();

	// TODO: I should put a nicer encoding detection here...

	// this checks for UTF-8 BOM, if it's there, input is probably UTF-8.
	if( (*rawFileData)[0] == (char)0xEF && (*rawFileData)[1] == (char)0xBB && (*rawFileData)[2] == (char)0xBF ){
		isCP437 = false;
	}

	if(isCP437){
		actionCodecCP437->setChecked(true);
	} else {
		actionCodecUTF8->setChecked(true); // radio button of codec selector
	}
	updateTextBrowser(isCP437);

	// if statusbar is enabled, show a message.
	if(actionStatusBar->isChecked())
		statusbar->showMessage(QStringLiteral("'%1' loaded.").arg(file));

	return true;
}

void BPNVMainWindow::updateTextBrowser(bool isCP437){
	//qDebug("updateTextBrowser()");
	if(rawFileData) {
    //qDebug("updateTextBrowser(): if rawFileData");

    QRegularExpression linkRE(QStringLiteral(R"x(([^"])((http|https)://([^()"'<>\s]*))([^"]))x" ));
		QString text;

		if (isCP437) {
			if(!QTextCodec::availableCodecs().contains("IBM437")) {
				// if codec is not available on this system or Qt build,
				// use the included implementation
				// Qt4 doesn't have it, Qt5 sometimes (linux), sometimes not (OS X)

				// Qt will take ownership of this object and delete it
				// See: http://doc.qt.io/qt-5/qtextcodec.html#QTextCodec
				new QCodePage437Codec();
				qDebug("new 437 instance created");
			}
			auto *codec = QTextCodec::codecForName("IBM437");
			if(codec){
				text = codec->toUnicode(*rawFileData);
			} else {
				qCritical("Could not load codec 'CP437' for unknown reason. This is a bug!");
				return;
			}
		} else {
			text = QString::fromUtf8(*rawFileData);
		}

		textBrowser->setHtml( QStringLiteral(
				"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
		    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
				"body, p, li { white-space: pre-wrap; }\n"  // dont collapse whitespace!
				"</style></head><body>%1</body></html>"
		).arg(
				text
						.replace("\n", "<br>\n")                            // add HTML linebreaks
						.replace(linkRE, R"(\1<a href="\2">\2</a>\5)")   // add a-Tags around urls
				// TODO this is pretty dump url detection, maybe there are better ways to do this?
		));

	}
}

void BPNVMainWindow::saveAsImage(QString filepath) {
	QString str = textBrowser->toPlainText();
	QFontMetrics fm(currentFont);
	QSize textSize = fm.size(0,str);
	QImage pixmap(textSize, QImage::Format_RGB32);
	QPalette pal(textBrowser->palette());
	QBrush bpen(pal.color(QPalette::Base));
	QPen fpen(pal.color(QPalette::Text));
	auto *pa=new QPainter(&pixmap);

	//pa->begin(&pixmap);
	pa->setFont(currentFont);
	pa->setBrush(bpen);
	pa->drawRect(0,0,textSize.width(),textSize.height());
	pa->setPen(fpen);
	//pa->setBackground(bpen);
	//pa->setBackgroundMode(Qt::OpaqueMode);
	pa->drawText(0,fm.height(),textSize.width(),textSize.height(), Qt::AlignLeft | Qt::TextIncludeTrailingSpaces,str);
	//pa->drawLine(0,140,320,140);
	pa->end();
	pixmap.save(filepath);
	delete pa;
}

void BPNVMainWindow::closeEvent(QCloseEvent *event) {
  qDebug("BPNVMainWindow::closeEvent triggered, saving state...");
	settings->setValue("geometry", saveGeometry());
	settings->setValue("windowState", saveState());
	settings->setValue("statusbar", actionStatusBar->isChecked());
	QMainWindow::closeEvent(event);
}

QSize BPNVMainWindow::sizeHint() const {
	return QSize(settings->value(QStringLiteral("width"),  1024).toInt(),
	             settings->value(QStringLiteral("height"), 1280).toInt())
			.expandedTo(minimumSizeHint());
}

void BPNVMainWindow::onActionStatusBar(bool checked ) {
	if( checked ){
		statusbar->height();
		statusbar->show();
	} else {
		statusbar->hide();
	}
}

void BPNVMainWindow::onActionOpen() {
	auto recent = settings->value(QStringLiteral("GUI/recentDir"), QStringLiteral("")).toString();
	auto file = QFileDialog::getOpenFileName(
			this,
			QStringLiteral("Open .nfo file..."), // title
			recent,                              // starting point in FS hierarchy
			QStringLiteral("nfo Files (*.nfo)")  // filter
	);
	if(!file.isEmpty()){
		settings->setValue(QStringLiteral("GUI/recentDir"), QFileInfo(file).absoluteDir().absolutePath() );
		loadFile(file);
	}
}

void BPNVMainWindow::onActionSaveImage() {
	auto recent = settings->value(QStringLiteral("GUI/recentDir"), QStringLiteral("")).toString();
	auto file = QFileDialog::getSaveFileName(
			this,
			QStringLiteral("Save to..."),
			recent,
			QStringLiteral("Images (*.jpg *.jpeg *.png *.tiff *.bmp *.xpm *.ppm *.xbm)")
	);
	if(!file.isEmpty()){
		settings->setValue(QStringLiteral("GUI/recentDir"), QFileInfo(file).absoluteDir().absolutePath() );
		saveAsImage(file);
	}
}


void BPNVMainWindow::onActionColor() {
	auto p = textBrowser->palette();

	auto bg = QColorDialog::getColor(
			p.color(QPalette::Base),
	    this,
	    QStringLiteral("Choose Background Color")
	);
	auto fg = QColorDialog::getColor(
			p.color(QPalette::WindowText),
			this,
			QStringLiteral("Choose Text Color")
	);

	p.setColor(QPalette::Base,       bg);
	p.setColor(QPalette::WindowText, fg);
	p.setColor(QPalette::Text,       fg);

	textBrowser->setPalette(p);
	textBrowser->setAutoFillBackground(true);
	textBrowser->repaint();

	settings->setValue(QStringLiteral("GUI/colors"), p);
}


void BPNVMainWindow::onActionDefaultColor() {
	textBrowser->setPalette(defaultPalette);
	textBrowser->repaint();

	settings->remove(QStringLiteral("GUI/colors"));
}


void BPNVMainWindow::onActionFont() {
	bool ok = false;
	auto font = QFontDialog::getFont(
			&ok,
	    textBrowser->font(),
	    this,
	    QStringLiteral("Select Font - A fixed size font is recommended")
	);
	qDebug("onActionFont: valid font???");
	if(ok) {
		qDebug("onActionFont: valid font");
		textBrowser->setFont(font);
		textBrowser->repaint();

		currentFont = font;
		settings->setValue("GUI/font", font);
	}
}


void BPNVMainWindow::onActionDefaultFont() {
	textBrowser->setFont(defaultFont);

	settings->remove(QStringLiteral("GUI/font"));
}


void BPNVMainWindow::onActionSwitchToUTF8() {
	qDebug("onActionSwitchToUTF8()");
	updateTextBrowser(false);
}


void BPNVMainWindow::onActionSwitchToCP437() {
	qDebug("onActionSwitchToCP437()");
	updateTextBrowser(true);
}


void BPNVMainWindow::onActionAbout() {
	AboutDialog ad(this);
	ad.exec();
}

void BPNVMainWindow::onActionQuit() {
  QApplication::closeAllWindows();
}

void BPNVMainWindow::setupUi() {
	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
	                         QStringLiteral("brainpower"), QStringLiteral("bp-nfoview"), this);
	settings->setDefaultFormat(QSettings::IniFormat);

	// Widgets
	textBrowser = new QTextBrowser(this);


	// Actions

	// Menu -> File
	actionOpen         = new QAction(this);
	actionSaveImage    = new QAction(this);
	actionQuit         = new QAction(this);
	// Menu -> View
	actionStatusBar    = new QAction(this);
	actionColor        = new QAction(this);
	actionDefaultColor = new QAction(this);
	actionFont         = new QAction(this);
	actionDefaultFont  = new QAction(this);
	// Menu -> View -> Codec
	agCodec            = new QActionGroup(this);
	actionCodecUTF8    = new QAction(this);
	actionCodecCP437   = new QAction(this);
	// Menu -> Help
	actionAbout        = new QAction(this);

	// Menu
	menubar       = new QMenuBar(this);
	menuFile      = new QMenu(menubar);
	menuView      = new QMenu(menubar);
	menuViewCodec = new QMenu(menuView);
	menuHelp      = new QMenu(menubar);

	// StatusBar
	statusbar = new QStatusBar(this);

	// Actions
	actionOpen->setIcon(QIcon::fromTheme("document-open"));
	actionQuit->setIcon(QIcon::fromTheme("application-exit"));
	actionSaveImage->setIcon(QIcon::fromTheme("document-save-as"));
	actionFont->setIcon(QIcon::fromTheme("fonts-package"));
	actionDefaultFont->setIcon(QIcon::fromTheme("fonts-package"));
	actionColor->setIcon(QIcon::fromTheme("fill-color"));
	actionDefaultColor->setIcon(QIcon::fromTheme("fill-color"));
	actionAbout->setIcon(QIcon::fromTheme("help-about"));

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

	currentPalette = settings->value(QStringLiteral("GUI/colors"), defaultPalette).value<QPalette>();
	currentFont    = settings->value(QStringLiteral("GUI/font"),   defaultFont   ).value<QFont>();

	// TextBrowser
	textBrowser->setAcceptDrops(false); // TODO: maybe add function to open a dropped file
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
	menuView->addAction(menuViewCodec->menuAction());
	// Menu -> View -> Codec
	menuViewCodec->addAction(actionCodecCP437);
	menuViewCodec->addAction(actionCodecUTF8);
	// Menu -> Help
	menuHelp->addAction(actionAbout);


	menuFile->setTitle(QStringLiteral(          "File"                             ));
	actionOpen->setText(QStringLiteral(           "Open..."                        ));
	actionSaveImage->setText(QStringLiteral(      "Save as Image..."               ));
	actionQuit->setText(QStringLiteral(           "Quit"                           ));

	menuView->setTitle(QStringLiteral(          "View"                             ));
	actionStatusBar->setText(QStringLiteral(      "Show Statusbar"                 ));
	actionColor->setText(QStringLiteral(          "Select custom BG/FG color..."   ));
	actionDefaultColor->setText(QStringLiteral(   "Restore default color"          ));
	actionFont->setText(QStringLiteral(           "Select custom Font..."          ));
	actionDefaultFont->setText(QStringLiteral(    "Restore default font"           ));

	menuViewCodec->setTitle(QStringLiteral(     "Encoding"                         ));
	actionCodecCP437->setText(QStringLiteral(     "CP 437"                         ));
	actionCodecUTF8->setText(QStringLiteral(      "UTF-8"                          ));

	menuHelp->setTitle(QStringLiteral(          "Help"                             ));
	actionAbout->setText(QStringLiteral(          "About..."                       ));


	setMenuBar(menubar);
	setStatusBar(statusbar);
	setCentralWidget(textBrowser);

	//statusbar->setBackgroundRole(QPalette::Highlight);
	//statusbar->setAutoFillBackground(true);

	// connects
	connect(actionOpen,         SIGNAL(triggered()),   this, SLOT(onActionOpen())          );
	connect(actionSaveImage,    SIGNAL(triggered()),   this, SLOT(onActionSaveImage())     );
	connect(actionQuit,         SIGNAL(triggered()),   this, SLOT(onActionQuit())          );

	connect(actionStatusBar,    SIGNAL(toggled(bool)), this, SLOT(onActionStatusBar(bool)) );
	connect(actionColor,        SIGNAL(triggered()),   this, SLOT(onActionColor())         );
	connect(actionDefaultColor, SIGNAL(triggered()),   this, SLOT(onActionDefaultColor())  );
	connect(actionFont,         SIGNAL(triggered()),   this, SLOT(onActionFont())          );
	connect(actionDefaultFont,  SIGNAL(triggered()),   this, SLOT(onActionDefaultFont())   );

	connect(actionCodecUTF8,    SIGNAL(triggered()),   this, SLOT(onActionSwitchToUTF8())  );
	connect(actionCodecCP437,   SIGNAL(triggered()),   this, SLOT(onActionSwitchToCP437()) );

	connect(actionAbout,        SIGNAL(triggered()),   this, SLOT(onActionAbout())         );

	// restore window state and geometry
	restoreGeometry(settings->value("geometry").toByteArray());
	restoreState(settings->value("windowState").toByteArray());

	// restor StatusBar state
	bool statusBarEnabled = settings->value(QStringLiteral("statusbar"), true).toBool();
	actionStatusBar->setChecked(statusBarEnabled);
	onActionStatusBar(statusBarEnabled);
}
