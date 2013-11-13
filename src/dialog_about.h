/**#############################################################################
#                                                                              #
NAME = "bp-nfoview"                                                            #
AUTHOR = "brainpower@gulli.com"                                                #
VERSION = "0.1.2"                                                              #
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
# Copyright (c) 2010-2013 brainpower@gulli.com                                 #
#                                                                              #
#############################################################################**/
#ifndef BP_NFOVIEW_DIALOG_ABOUT_H
#define BP_NFOVIEW_DIALOG_ABOUT_H

#ifndef DIALOG_ABOUT_VERSION
#define DIALOG_ABOUT_VERSION "0.1.0"
#endif // VERSION

#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include "bp-nfoview.h"

QT_BEGIN_NAMESPACE
namespace Ui {

class Dialog_About: public QDialog{ Q_OBJECT
public:
	QDialogButtonBox *buttonBox;
	QLabel *label;
	QLabel *ilabel;
	Dialog_About(QWidget *parent =0);

}; // Dialog_About
} // namespace Ui
QT_END_NAMESPACE
#endif // BP_NFOVIEW_DIALOG_ABOUT_H

