/**#############################################################################
#                                                                              #
NAME = "bp-nfoview"                                                            #
AUTHOR = "brainpower@gulli.com"                                                #
VERSION = "0.1.1"                                                              #
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
# Copyright (c) 2010-2012  brainpower@gulli.com                                #
#                                                                              #
#############################################################################**/
#include "bp-nfoview.h"

void Ui::Ui_MainWindow::setupUi(){
	if (this->objectName().isEmpty())
		this->setObjectName(QString::fromUtf8("MainWindow"));
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,"brainpower", "bp-nfoview");
	settings.setDefaultFormat(QSettings::IniFormat);
	resize(settings.value("general/width", 680).toInt(),settings.value("general/height", 720).toInt());
	QIcon mainIcon(":/img/logo.png");
	setWindowIcon(mainIcon);
	actionOpen = new QAction(this);
	actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
	actionOpen->setShortcut(QKeySequence(QKeySequence::Open));
	actionSaveImage = new QAction(this);
	actionQuit = new QAction(this);
	actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
	actionQuit->setShortcut(QKeySequence(QKeySequence::Quit));
	actionAbout = new QAction(this);
	actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
	actionSB = new QAction(this);
	actionSB->setObjectName(QString::fromUtf8("actionSB"));
	actionSB->setCheckable(true);
	actionColor = new QAction(this);
	actionDefaultColor = new QAction(this);
	actionFont = new QAction(this);
	actionDefaultFont = new QAction(this);
	centralwidget = new QWidget(this);
	centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
	textEdit = new QTextEdit(centralwidget);
	textEdit->setObjectName(QString::fromUtf8("textEdit"));
	textEdit->setGeometry(QRect(0, 0, 671, 641));
	//sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
	//textEdit->setSizePolicy(sizePolicy);
	//QFont font = defaultFont;
	defaultFont.setFamily(QString::fromUtf8("DejaVu Sans Mono"));
	defaultFont.setPointSize(10);
	defaultFont.setStyleHint(QFont::Monospace);
	cFont = settings.value("GUI/font", defaultFont).value<QFont>();
	textEdit->setFont(cFont);
	textEdit->setAcceptDrops(false);
	textEdit->setFrameShape(QFrame::NoFrame);
	textEdit->setFrameShadow(QFrame::Plain);
	textEdit->setUndoRedoEnabled(false);
	textEdit->setReadOnly(true);
	textEdit->setLineWrapMode(QTextEdit::NoWrap);
	textEdit->setPalette(settings.value("GUI/colors", defaultPalette).value<QPalette>());
	//defaultPalette = textEdit->palette();
	setCentralWidget(centralwidget);
	menubar = new QMenuBar(this);
	menubar->setObjectName(QString::fromUtf8("menubar"));
	menubar->setGeometry(QRect(0, 0, 682, 28));
	menuFile = new QMenu(menubar);
	menuFile->setObjectName(QString::fromUtf8("menuFile"));
	menuView = new QMenu(menubar);
	menuHelp = new QMenu(menubar);
	menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
	setMenuBar(menubar);
	statusbar = new QStatusBar(this);
	statusbar->setObjectName(QString::fromUtf8("statusbar"));
	//statusbar->setBackgroundRole(QPalette::Highlight);
	//statusbar->setAutoFillBackground(true);
	setStatusBar(statusbar);
	if(settings.value("general/sb", 23) == 23){
		actionSB->setChecked(true);
		sboffset = 23;
	} else{
		sboffset = 0;
		SBaction(false);
	}
	menubar->addAction(menuFile->menuAction());
	menubar->addAction(menuView->menuAction());
	menubar->addAction(menuHelp->menuAction());
	menuFile->addAction(actionOpen);
	menuFile->addAction(actionSaveImage);
	menuFile->addAction(actionQuit);
	menuView->addAction(actionSB);
	menuView->addAction(actionColor);
	menuView->addAction(actionDefaultColor);
	menuView->addAction(actionFont);
	menuView->addAction(actionDefaultFont);
	menuHelp->addAction(actionAbout);
	retranslateUi();
	updateGeometries();
	connect(actionSB, SIGNAL(toggled(bool)),this, SLOT(SBaction(bool)));
	connect(actionQuit, SIGNAL(triggered()),qApp, SLOT(quit()));
	connect(actionOpen, SIGNAL(triggered()),this, SLOT(OpenAction()));
	connect(actionAbout, SIGNAL(triggered()),this, SLOT(AboutAction()));
	connect(actionColor, SIGNAL(triggered()),this, SLOT(ColorAction()));
	connect(actionDefaultColor, SIGNAL(triggered()),this, SLOT(DefaultColorAction()));
	connect(actionFont, SIGNAL(triggered()),this, SLOT(FontAction()));
	connect(actionDefaultFont, SIGNAL(triggered()),this, SLOT(DefaultFontAction()));
	connect(actionSaveImage, SIGNAL(triggered()),this,SLOT(saveImageAction()));
	QMetaObject::connectSlotsByName(this);
} // setupUi

void Ui::Ui_MainWindow::retranslateUi(){
	setWindowTitle(QString("bp-nfoview ")+QString(MAIN_VERSION));
	actionOpen->setText("Open...");
	actionSaveImage->setText("Save as Image...");
	actionQuit->setText("Quit");
	actionSB->setText("Show Statusbar");
	actionAbout->setText("About...");
	actionColor->setText("Select custom BG/FG color...");
	actionDefaultColor->setText("Restore default color");
	actionFont->setText("Select custom Font...");
	actionDefaultFont->setText("Restore default font");
	menuFile->setTitle("File");
	menuView->setTitle("View");
	menuHelp->setTitle("Help");
} // retranslateUi

void Ui::Ui_MainWindow::updateGeometries(){
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,"brainpower", "bp-nfoview");
	textEdit->setGeometry(QRect(0, 0, width(), height()-sboffset));
	settings.setValue("general/height",height());
	settings.setValue("general/width",width());
	settings.setValue("general/sb", sboffset);
	//settings.sync();
} // updateGeometries

void Ui::Ui_MainWindow::resizeEvent(QResizeEvent*){
	updateGeometries();
} // resizeEvent

void Ui::Ui_MainWindow::SBaction(bool checked){
	if(checked){
		statusbar = new QStatusBar(this);
		setStatusBar(statusbar);
		sboffset = 23;
		updateGeometries();
	} else{
		setStatusBar(0);
		sboffset = 0;
		updateGeometries();
	}
}

void Ui::Ui_MainWindow::OpenAction(){
	//QSettings settings(QSettings::IniFormat, QSettings::UserScope,"brainpower", "bp-nfoview");
	loadFile(QFileDialog::getOpenFileName(this,"Open nfo-file...", "", "nfo Files (*.nfo)"));
}

void Ui::Ui_MainWindow::AboutAction(){
	Ui::Dialog_About *aboutDialog = new Ui::Dialog_About(this);
	aboutDialog->show();
}

void Ui::Ui_MainWindow::ColorAction(){
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,"brainpower", "bp-nfoview");
	QPalette p = textEdit->palette();
	QColor bg = QColorDialog::getColor(p.color(QPalette::Base), this,"Choose Background Color");
	QColor fg = QColorDialog::getColor(p.color(QPalette::WindowText), this,"Choose Text Color");
	p.setColor(QPalette::Base, bg);
	p.setColor(QPalette::WindowText, fg);
	p.setColor(QPalette::Text, fg);
	textEdit->setPalette(p);
	settings.setValue("GUI/colors", p);
	textEdit->setAutoFillBackground(true);
	textEdit->repaint();
}

void Ui::Ui_MainWindow::DefaultColorAction(){
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,"brainpower", "bp-nfoview");
	textEdit->setPalette(defaultPalette);
	settings.remove("GUI/colors");
	textEdit->repaint();
}

void Ui::Ui_MainWindow::FontAction(){
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,"brainpower", "bp-nfoview");
	QFont font = QFontDialog::getFont(0, textEdit->font(), this, "Select Font - fixed size font recommended");
	textEdit->setFont(font);
	settings.setValue("GUI/font",font);
}

void Ui::Ui_MainWindow::DefaultFontAction(){
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,"brainpower", "bp-nfoview");
	textEdit->setFont(defaultFont);
	settings.remove("GUI/font");
}

void Ui::Ui_MainWindow::loadFile(QString path){
	//QSettings settings(QSettings::IniFormat, QSettings::UserScope,"brainpower", "bp-nfoview");
	QFile nfofile(path);
	if(nfofile.exists()){
		nfofile.open(QIODevice::ReadOnly | QIODevice::Text);
		QString nline;
		QCodePage437Codec *codec = new QCodePage437Codec();
		QByteArray line;
		while(!nfofile.atEnd()){
			line = nfofile.readLine();
			nline.append(codec->toUnicode(line));
		}
		textEdit->setText(nline);
		if(sboffset!=0)	statusbar->showMessage(QString("'")+path+QString("' loaded."));
		nfofile.close();
		delete codec;
		codec = 0;
	}
}

void Ui::Ui_MainWindow::saveImageAction(){
	saveAsImage(QFileDialog::getSaveFileName(this,"Save to...","","Images (*.jpg *.jpeg *.png *.tiff *.bmp *.xpm *.ppm *.xbm)"));
}

void Ui::Ui_MainWindow::saveAsImage(QString path){
	QString str = textEdit->toPlainText();
	QFontMetrics fm(cFont);
	QSize textSize = fm.size(0,str);
	QImage pixmap(textSize, QImage::Format_RGB32);
	QPalette pal(textEdit->palette());
	QBrush bpen(pal.color(QPalette::Base));
	QPen fpen(pal.color(QPalette::Text));
	QPainter *pa=new QPainter(&pixmap);
	//pa->begin(&pixmap);
	pa->setFont(cFont);
	pa->setBrush(bpen);
	pa->drawRect(0,0,textSize.width(),textSize.height());
	pa->setPen(fpen);
	//pa->setBackground(bpen);
	//pa->setBackgroundMode(Qt::OpaqueMode);
	pa->drawText(0,fm.height(),textSize.width(),textSize.height(), Qt::AlignLeft | Qt::TextIncludeTrailingSpaces,str);
	//pa->drawLine(0,140,320,140);
	pa->end();
	pixmap.save(path);

}

int main( int argc, char* argv[] ){
    QApplication app( argc, argv );
    Ui::Ui_MainWindow gui;
		gui.setupUi();
		if(argc==2){
			gui.loadFile(argv[1]);
		}
    gui.show();
    return app.exec();
}
