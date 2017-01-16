/*
 * MainWindow.cpp
 *
 *  Created on: 17 sie 2016
 *      Author: Andrzej Dackiewicz
 *
 *  Comment:This file is a part of Digital forgery detection program that was
 *  		an engineering thesis of the author.
 *  		This file implements functionality of the main menu screen
 */

// libraries and headers
#include "WindowMenu.h"
#include <fstream>

WindowMenu::WindowMenu(WindowMain* main)
{
	mainWindow = main;
	mapperExivInfo = new QSignalMapper(this);
	mapperSecureImage = new QSignalMapper(this);
	mapperSecureCheck = new QSignalMapper(this);
	mapperTextEdited = new QSignalMapper (this);

    this->setGeometry(100, 100, 500, 400);
    this->setWindowTitle("JPGAnalizer - Menu");
    this->setObjectName("Okno_Menu");

    label = new QLabel("MENU GLOWNE", this);
    textFont = label->font();
    textFont.setPointSize(14);
    textFont.setBold(true);
    label->setGeometry(450, 40, 350, 50);
    label->setAlignment(Qt::AlignCenter);
    label->setFont(textFont);

    editableLine = new QLineEdit("Sciezka do badanego obrazu", this);
    editableLine->setGeometry(30, 780, 550, 20);

    buttonChooseImage = new QPushButton("Wybierz obraz", this);
    buttonChooseImage->setGeometry(600, 760, 200, 60);
    buttonChooseImage->setFont(textFont);

    buttonExivInfo = new QPushButton("Pokaz informacje Exiv", this);
    buttonExivInfo->setGeometry(870, 200, 350, 120);
    buttonExivInfo->setFont(textFont);

    buttonSecureImage = new QPushButton("Zabezpiecz obraz", this);
    buttonSecureImage->setGeometry(870, 400, 350, 120);
    buttonSecureImage->setFont(textFont);

    buttonSecureCheck = new QPushButton("Sprawdz zabezpieczenia obrazu", this);
    buttonSecureCheck->setGeometry(870, 600, 350, 120);
    buttonSecureCheck->setFont(textFont);

    QImageReader reader(QString::fromAscii("./ProgramImages/No-image-found.jpg"));
    defaultImage = reader.read();

    imageLabel = new QLabel(this);
    imageLabel->setGeometry(20, 120, 806, 606);

    setDefaultImage();

    // connecting editing text to validating image ...

    connect(editableLine, SIGNAL(textEdited(QString)), this, SLOT(setMapperTextEdited()));
    connect(editableLine, SIGNAL(textEdited(QString)), mapperTextEdited, SLOT(map()));

    // connecting chooseImage button with appropriate slot

    connect(buttonChooseImage, SIGNAL(clicked()), this, SLOT(chooseFile()));

    connect(buttonExivInfo, SIGNAL(clicked()), this, SLOT(setMapperExivInfo()));
    connect(buttonSecureImage, SIGNAL(clicked()), this, SLOT(setMapperSecureImage()));
    connect(buttonSecureCheck, SIGNAL(clicked()), this, SLOT(setMapperSecureCheck()));

    connect(buttonExivInfo, SIGNAL(clicked()), mapperExivInfo, SLOT(map()));
    connect(buttonSecureImage, SIGNAL(clicked()), mapperSecureImage, SLOT(map()));
    connect(buttonSecureCheck, SIGNAL(clicked()), mapperSecureCheck, SLOT(map()));

    mapperExivInfo->setMapping(buttonExivInfo, editableLine->text());
    mapperSecureImage->setMapping(buttonSecureImage, editableLine->text());
    mapperSecureCheck->setMapping(buttonSecureCheck, editableLine->text());
    mapperTextEdited->setMapping(editableLine, editableLine->text());

    connect(mapperExivInfo, SIGNAL(mapped(QString)), mainWindow, SLOT(showExivData(QString)));
    connect(mapperSecureImage, SIGNAL(mapped(QString)), mainWindow, SLOT(showSecureImage(QString)));
    connect(mapperSecureCheck, SIGNAL(mapped(QString)), mainWindow, SLOT(showSecureCheck(QString)));
    connect(mapperTextEdited, SIGNAL(mapped(QString)), this, SLOT(validateImage(QString)));
}

WindowMenu::~WindowMenu()
{
	delete label;
	delete buttonExivInfo;
	delete buttonSecureImage;
	delete buttonSecureCheck;
	delete buttonChooseImage;

	delete mapperExivInfo;
	delete mapperSecureImage;
	delete mapperSecureCheck;
}

void WindowMenu::setMapperExivInfo()
{
    mapperExivInfo->setMapping(buttonExivInfo, editableLine->text());

    return;
}

void WindowMenu::setMapperSecureImage()
{
	mapperSecureImage->setMapping(buttonSecureImage, editableLine->text());
	return;
}

void WindowMenu::setMapperSecureCheck()
{
	mapperSecureCheck->setMapping(buttonSecureCheck, editableLine->text());
	return;
}

void WindowMenu::setMapperTextEdited()
{
	mapperTextEdited->setMapping(editableLine, editableLine->text());
	return;
}

void WindowMenu::chooseFile()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(tr("Images (*.jpg *.JPG)"));

	dialog.setViewMode(QFileDialog::Detail);

	QStringList fileNames;
	if (dialog.exec())
		fileNames = dialog.selectedFiles();

	if (fileNames.count() == 1)
	{
		std::cout << fileNames.at(0).toStdString() << std::endl;

		editableLine->setText(fileNames.at(0));
		validateImage(fileNames.at(0));
	}
	return;
}

void WindowMenu::validateImage(QString path)
{
	if (mainWindow->checkImageFile(path.toStdString()))
	{
		QPixmap pixmap(path);
		pixmap = pixmap.scaled(800, 600);
		imageLabel->setPixmap(pixmap);
	}
	else
	{
		setDefaultImage();
	}

	return;
}

void WindowMenu::setDefaultImage()
{
    QPixmap pixmap;
    pixmap.convertFromImage(defaultImage);
    pixmap = pixmap.scaled(800, 600);
    imageLabel->setPixmap(pixmap);
    return;
}
