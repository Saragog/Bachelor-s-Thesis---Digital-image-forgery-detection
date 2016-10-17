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

	label = new QLabel("Secure Image", this);
	label->setGeometry(100, 100, 200, 100);

	editableLine = new QLineEdit("Edit Me", this);
	editableLine->setGeometry(50, 600, 400, 20);

	backButton = new QPushButton("Powrot", this);
	backButton->setGeometry(50, 700, 100, 100);

	chooseWhereToSaveButton = new QPushButton("Choose where to save", this);
	chooseWhereToSaveButton->setGeometry(500, 600, 200, 100);

	saveButton = new QPushButton("Zapisz", this);
	saveButton->setGeometry(800, 700, 100, 100);

	palette.setColor(QPalette::Background, Qt::cyan);
	this->setAutoFillBackground(true);
	this->setPalette(palette);

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
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilter(tr("Images (*.jpg)"));

	dialog.setViewMode(QFileDialog::Detail);

	QStringList fileName;

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
