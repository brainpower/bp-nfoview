// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2010-2023  brainpower <brainpower@mailbox.org>

#include "config.hpp"

#include "BPNVMainWindow.hpp"

#include <QCommandLineParser>
#include <QApplication>


int main( int argc, char* argv[] ){
	QApplication app( argc, argv );
	QCommandLineParser parser;

	app.setApplicationName(QStringLiteral("bp-nfoview"));
	app.setApplicationDisplayName(QStringLiteral("brainpower's NfoViewer"));
	app.setOrganizationName(QStringLiteral("brainpower"));
	app.setOrganizationDomain(QStringLiteral("de.brainpower"));
	app.setApplicationVersion(QStringLiteral(MAIN_VERSION));

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
