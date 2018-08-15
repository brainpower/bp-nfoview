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

#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "AboutDialog.hpp"
#include "config.h"

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
     "Copyright (c) 2010-2018 &lt;brainpower@mailbox.org&gt; <br>"
     "licensed under GPLv2<br>"
     "See <a href=\"http://www.gnu.org/licenses/gpl.html\">http://www.gnu.org/licenses/</a> for more info.<br>"
	));

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}

QSize AboutDialog::sizeHint() const {
	return QSize(400,200).expandedTo(minimumSizeHint());
}

