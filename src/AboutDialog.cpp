// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2010-2021  brainpower <brainpower@mailbox.org>

#include "config.hpp"

#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "AboutDialog.hpp"


AboutDialog::AboutDialog(QWidget *parent)
		: QDialog(parent) {
	setWindowTitle(QStringLiteral("About"));

	layout    = new QVBoxLayout(this);
	ilabel    = new QLabel(this);
	label     = new QLabel(this);
	buttonBox = new QDialogButtonBox(this);

	layout->addWidget(ilabel);
	layout->addWidget(label, 1);
	layout->addWidget(buttonBox);

	label->setAlignment(Qt::AlignCenter);
	label->setWordWrap(true);
	label->setOpenExternalLinks(true);

	ilabel->setPixmap(QPixmap(":/img/bp-nfoview.png").scaled(64,64,Qt::KeepAspectRatio));
	ilabel->setAlignment(Qt::AlignCenter);

	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Close);
	buttonBox->setCenterButtons(true);

	label->setText(QStringLiteral(
     "<br>"
		 "<span style=\"font-size:16pt;\"><b>bp-nfoview " MAIN_VERSION "</b></span><br>"
     "A simple lightweight nfo-viewer written in C++ using Qt for its interface<br>"
     "<br>"
     "<br>"
     "Copyright (c) " MAIN_CPYRGT_YEAR " &lt;brainpower@mailbox.org&gt; <br>"
     "licensed under GPLv2<br>"
     "See <a href=\"http://www.gnu.org/licenses/gpl.html\">http://www.gnu.org/licenses/</a> for more info.<br>"
	));

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}

QSize AboutDialog::sizeHint() const {
	return QSize(400,200).expandedTo(minimumSizeHint());
}

