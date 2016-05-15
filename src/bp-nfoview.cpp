/**#############################################################################
#                                                                              #
# PROJECT = "bp-nfoview"                                                       #
# AUTHOR = "brainpower@mailbox.org"                                            #
# VERSION = "0.2.0"                                                            #
# LICENCE = "GPLv3"                                                            #
# DESC = 'A simple lightweight nfo-viewer written in C++ with Qt Interface'    #
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
# Copyright (c) 2010-2016  brainpower <brainpower@mailbox.org>                 #
#                                                                              #
#############################################################################**/


#include <QCommandLineParser>
#include <QApplication>

#include "bp-nfoview.h"
#include "BPNVMainWindow.hpp"


int main( int argc, char* argv[] ){
	QApplication app( argc, argv );

	app.setApplicationName("bp-nfoview");
	app.setApplicationDisplayName("brainpower's NfoViewer");
	app.setOrganizationName("brainpower");
	app.setOrganizationDomain("de.brainpower");
	app.setApplicationVersion(MAIN_VERSION);

	QApplication::setWindowIcon(QIcon(":/img/logo.png"));

	QCommandLineParser parser;
	parser.setApplicationDescription("A very simple viewer for .nfo files.");
	parser.addVersionOption();
	parser.addHelpOption();

	parser.addPositionalArgument(QStringLiteral("file"), QStringLiteral("File to open."), QStringLiteral("[file]"));

	parser.process(app);

	BPNVMainWindow *gui;
	if(parser.positionalArguments().isEmpty()){
		gui = new BPNVMainWindow;
	} else {
		gui = new BPNVMainWindow(parser.positionalArguments());
	}

	gui->show();
	return app.exec();
}
