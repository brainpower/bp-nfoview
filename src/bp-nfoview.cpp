// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2010-2021  brainpower <brainpower@mailbox.org>

#include "config.hpp"

#include "BPNVMainWindow.hpp"

#include <QCommandLineParser>
#include <QApplication>


int main( int argc, char* argv[] ){
	QApplication app( argc, argv );
	QCommandLineParser parser;

	app.setApplicationName("bp-nfoview");
	app.setApplicationDisplayName("brainpower's NfoViewer");
	app.setOrganizationName("brainpower");
	app.setOrganizationDomain("de.brainpower");
	app.setApplicationVersion(MAIN_VERSION);

	QApplication::setWindowIcon(QIcon(":/img/logo.png"));

	parser.setApplicationDescription("A very simple viewer for .nfo files.");
	parser.addVersionOption();
	parser.addHelpOption();

	parser.addPositionalArgument(QStringLiteral("file"), QStringLiteral("File to open."), QStringLiteral("[file]"));

	parser.process(app);

	BPNVMainWindow gui(parser.positionalArguments());

	gui.show();
	return app.exec();
}
