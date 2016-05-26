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

#ifndef BP_NFOVIEW_ABOUTDIALOG_HPP
#define BP_NFOVIEW_ABOUTDIALOG_HPP

#include <QDialog>

class QLabel;
class QDialogButtonBox;
class QVBoxLayout;


class AboutDialog : public QDialog {
	Q_OBJECT
public:

	explicit AboutDialog(QWidget *parent = nullptr);

public slots:

	QSize sizeHint() const override;

private:
	QDialogButtonBox *buttonBox;
	QLabel *label;
	QLabel *ilabel;
	QVBoxLayout *layout;
};


#endif //BP_NFOVIEW_ABOUTDIALOG_HPP