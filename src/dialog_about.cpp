/**#############################################################################
#                                                                              #
NAME = "bp-nfoview"                                                            #
AUTHOR = "brainpower@gulli.com"                                                #
VERSION = "0.1.0"                                                              #
LICENCE = "GPL v2"                                                             #
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
# Copyright (c) 2010  brainpower@gulli.com                                     #
#                                                                              #
#############################################################################**/
#include "dialog_about.h"

Ui::Dialog_About::Dialog_About(){
	setObjectName("Dialog_About");
	setWindowTitle("About");
	resize(380, 312);
	setMaximumSize(380, 312);
	setMinimumSize(380, 312);
	QIcon aboutIcon(":/img/bp-nfoview.png");
	setWindowIcon(aboutIcon);

	ilabel = new QLabel(this);
	ilabel->setGeometry(5,5,370,64);
	ilabel->setPixmap(QPixmap(":/img/bp-nfoview.png").scaled(64,64,Qt::KeepAspectRatio));
	ilabel->setAlignment(Qt::AlignCenter);

	buttonBox = new QDialogButtonBox(this);
	buttonBox->setGeometry(QRect(5, 280, 370, 32));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Close);
	buttonBox->setCenterButtons(true);
	buttonBox->setObjectName("buttonBox");

	label = new QLabel(this);
	label->setGeometry(QRect(5, 64, 370, 220));
	label->setAlignment(Qt::AlignCenter);
	label->setWordWrap(true);
	label->setOpenExternalLinks(true);
	label->setObjectName("label");

	QString aboutText("<span style=\"font-size:16pt;\"><b>bp-nfoview ");
	aboutText += MAIN_VERSION;
	aboutText += "</b></span><br>A simple lightweight nfo-viewer written in C++ using Qt4 for its interface<br>";

	aboutText += "<br>\nits also using:\n";
	aboutText += "<br>Codepage437Codec ";
	aboutText += QCODEPAGE437CODEC_VERSION;

	aboutText += "<br>AboutDialog ";
	aboutText += DIALOG_ABOUT_VERSION;

	aboutText += "<br><br>Copyright (c) 2011 &lt;brainpower@gulli.com&gt; <br>licensed under GPL<br> See <a href=\"http://www.gnu.org/licenses/gpl.html\">http://www.gnu.org/licenses/</a> for more info";
	label->setText(aboutText);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	QMetaObject::connectSlotsByName(this);
}
