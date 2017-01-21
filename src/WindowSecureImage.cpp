/*
 * W.cpp
 *
 *  Created on: 26 sie 2016
 *      Author: andrzej
 */

#include "WindowSecureImage.h"
#include <QFileDialog>

WindowSecureImage::WindowSecureImage(QWidget* main)
{
	mainWindow = main;
	codec = QTextCodec::codecForName("UTF-8");
	this->setWindowTitle("JPGAnalizer - Zabezpieczanie Obrazu");

	label = new QLabel("Zabezpieczanie obrazu JPG", this);
	textFont = label->font();
	textFont.setPointSize(14);
	textFont.setBold(true);
	label->setAlignment(Qt::AlignCenter);
	label->setFont(textFont);
	label->setGeometry(500, 50, 300, 50);

	editableLine = new QLineEdit(codec->toUnicode("Ścieżka do zapisu"), this);
	editableLine->setGeometry(100, 300, 550, 20);

	backButton = new QPushButton("Powrot", this);
	backButton->setFont(textFont);
	backButton->setGeometry(150, 600, 350, 120);

	chooseWhereToSaveButton = new QPushButton(codec->toUnicode("Wskaż ścieżkę do zapisu"), this);
	chooseWhereToSaveButton->setFont(textFont);
	chooseWhereToSaveButton->setGeometry(700, 260, 250, 80);

	saveButton = new QPushButton("Zapisz", this);
	saveButton->setFont(textFont);
	saveButton->setGeometry(700, 600, 350, 120);

	mapper = new QSignalMapper(this);

	connect(backButton, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
	connect(chooseWhereToSaveButton, SIGNAL(clicked()), this, SLOT(chooseWhereToSave()));

	// Narazie bez argumentow na stale w jakimstam pliku bede zapisywal w okreslonym miejscu

	connect(saveButton, SIGNAL(clicked()), this, SLOT(setMapping()));
	connect(saveButton, SIGNAL(clicked()), mapper, SLOT(map()));
	mapper->setMapping(saveButton, editableLine->text());
	connect(mapper, SIGNAL(mapped(QString)), mainWindow, SLOT(saveImage(QString)));
}

WindowSecureImage::~WindowSecureImage()
{
	delete label;
	delete editableLine;
	delete backButton;
	delete chooseWhereToSaveButton;
	delete saveButton;

	delete mapper;
}

void WindowSecureImage::setImage(QString name)
{
	fileName = name.toStdString();

	return;
}

void WindowSecureImage::chooseWhereToSave()
{
	QStringList fileName;

	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilter(tr("Images (*.jpg)"));
	dialog.setViewMode(QFileDialog::Detail);

	if (dialog.exec()) fileName = dialog.selectedFiles();

	if (fileName.count() == 1)
	{
		editableLine->setText(fileName.at(0));
	}
	return;
}

void WindowSecureImage::setMapping()
{
	mapper->setMapping(saveButton, editableLine->text());
	return;
}
