// SPDX-FileCopyrightText: 2010 - 2013 brainpower <brainpower@mailbox.org>
//
// SPDX-License-Identifier: GPL-2.0-or-later

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
