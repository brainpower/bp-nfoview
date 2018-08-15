/**#############################################################################
#                                                                              #
# PROJECT = "bp-nfoview"                                                       #
# LICENCE = "GPL"                                                              #
#                                                                              #
# Copyright (c) 2010-2018  brainpower <brainpower@mailbox.org>                 #
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


#include <QCommandLineParser>
#include <QApplication>

#include "config.h"
#include "BPNVMainWindow.hpp"


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
