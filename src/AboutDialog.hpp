// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2010-2023  brainpower <brainpower@mailbox.org>

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
