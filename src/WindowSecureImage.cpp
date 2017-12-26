/*
 * W.cpp
 *
 *  Created on: 26 sie 2016
 *      Author: Andrzej Dackiewicz
 *
 *  Komentarz: Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Implementuje funkcje klasy WindowSecureImage pokazującej okno zabezpieczania obrazu.
 */

#include "WindowSecureImage.h"
#include <QFileDialog>

// Konstruktor klasy WindowSecureImage
WindowSecureImage::WindowSecureImage(QWidget* main)
{
	mainWindow = main;
	codec = QTextCodec::codecForName("UTF-8");
	// domyślny wyglad okna
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
	// przycisk powrotu
	backButton = new QPushButton(codec->toUnicode("Powrót"), this);
	backButton->setFont(textFont);
	backButton->setGeometry(150, 600, 350, 120);
	// przycisk wyboru miejsca zapisu
	chooseWhereToSaveButton = new QPushButton(codec->toUnicode("Wskaż ścieżkę do zapisu"), this);
	chooseWhereToSaveButton->setFont(textFont);
	chooseWhereToSaveButton->setGeometry(700, 260, 250, 80);
	// przycisk zapisu
	saveButton = new QPushButton("Zapisz", this);
	saveButton->setFont(textFont);
	saveButton->setGeometry(700, 600, 350, 120);

	mapper = new QSignalMapper(this);
	connect(backButton, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
	connect(chooseWhereToSaveButton, SIGNAL(clicked()), this, SLOT(chooseWhereToSave()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(setMapping()));
	connect(saveButton, SIGNAL(clicked()), mapper, SLOT(map()));
	mapper->setMapping(saveButton, editableLine->text());
	connect(mapper, SIGNAL(mapped(QString)), mainWindow, SLOT(saveImage(QString)));
}

// Destruktor klasy WindowSecureImage
WindowSecureImage::~WindowSecureImage()
{
	delete label;
	delete editableLine;
	delete backButton;
	delete chooseWhereToSaveButton;
	delete saveButton;
	delete mapper;
}

// Funkcja setImage czyta zawartość pola ścieżki do zapisu
void WindowSecureImage::setImage(QString name)
{
	fileName = name.toStdString();
	return;
}

// Funkcja chooseWhereToSave powoduje pojawienie się okna wyboru miejsca do zapisu
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

// Funkcja setMapping pozwala wykorzystać parametr ścieżki pliku do zapisu zabezpieczonego obrazu
void WindowSecureImage::setMapping()
{
	mapper->setMapping(saveButton, editableLine->text());
	return;
}
