/*
 * MainWindow.cpp
 *
 *  Created on: 17 sie 2016
 *      Author: andrzej
 */
#include <iostream>
#include "WindowMenu.h"
#include <fstream>

WindowMenu::WindowMenu(WindowMain* main)
{
	mainWindow = main;

	palette.setColor(QPalette::Background, Qt::cyan);	// TODO potem mozna zamienic to na rysunek background !!!
														// podobno mozna jakies style ustalic i wtedy porzadniej bedzie
														// ale o tym trzeba poczytac !!!
	this->setAutoFillBackground(true);
	this->setPalette(palette);

	mapperExivInfo = new QSignalMapper(this);
	mapperSecureImage = new QSignalMapper(this);
	mapperSecureCheck = new QSignalMapper(this);
	mapperTextEdited = new QSignalMapper (this);

    this->setGeometry(100, 100, 500, 400);
    this->setWindowTitle("JPGAnalizer");

    label = new QLabel("Informacje Exiv i diagram", this);
    label->setGeometry(650, 100, 200, 100);

    editableLine = new QLineEdit("Edit Me", this);
    editableLine->setGeometry(50, 600, 400, 20);

    buttonChooseImage = new QPushButton("Choose Image", this);
    buttonChooseImage->setGeometry(450, 580, 100, 60);

    buttonExivInfo = new QPushButton("Pokaz informacje Exiv", this);
    buttonExivInfo->setGeometry(900, 200, 250, 80);

    buttonSecureImage = new QPushButton("Zabezpiecz obraz", this);
    buttonSecureImage->setGeometry(900, 350, 250, 80);

    buttonSecureCheck = new QPushButton("Sprawdz zabezpieczenia obrazu", this);
    buttonSecureCheck->setGeometry(900, 500, 250, 80);

    QImageReader reader(QString::fromAscii("./ProgramImages/No-image-found.jpg"));
    defaultImage = reader.read();

    imageLabel = new QLabel(this);
    imageLabel->setGeometry(10, 10, 600, 450);

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
	dialog.setNameFilter(tr("Images (*.jpg)"));

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
		pixmap = pixmap.scaled(600, 450);
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
    pixmap = pixmap.scaled(600, 450);
    imageLabel->setPixmap(pixmap);
    return;
}
