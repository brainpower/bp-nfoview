/**#############################################################################
#                                                                              #
# PROJECT = "bp-nfoview"                                                       #
# LICENCE = "GPL"                                                              #
#                                                                              #
# Copyright (c) 2010-2013  brainpower <brainpower@mailbox.org>                 #
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
#ifndef BP_NFOVIEW_DIALOG_CHARBOX_H
#define BP_NFOVIEW_DIALOG_CHARBOX_H

#ifndef DIALOG_CHARBOX_VERSION
#define DIALOG_CHARBOX_VERSION "0.1.0"
#endif // VERSION

#include <QTableWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {

class Dialog_Charbox: public QDialog{
	Q_OBJECT
public:
	//QTable
	Dialog_Charbox();

}; // Dialog_About
} // namespace Ui
QT_END_NAMESPACE

#endif //BP_NFOVIEW_DIALOG_CHARBOX_H
