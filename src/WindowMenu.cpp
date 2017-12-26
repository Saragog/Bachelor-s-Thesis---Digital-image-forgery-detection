/*
 * MainWindow.cpp
 *
 *  Created on: 17 sie 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Comment:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Implementuje funkcje klasy WindowMenu
 */

#include "WindowMenu.h"
#include <fstream>
#include <QFileDialog>

// konstruktor klasy WindowMenu
WindowMenu::WindowMenu(WindowMain* main)
{
	mainWindow = main;
	mapperExivInfo = new QSignalMapper(this);
	mapperSecureImage = new QSignalMapper(this);
	mapperSecureCheck = new QSignalMapper(this);
	mapperTextEdited = new QSignalMapper (this);

	codec = QTextCodec::codecForName("UTF-8");

	// Wstępne ustalenie wyglądu widoku

    this->setGeometry(100, 100, 500, 400);
    this->setWindowTitle("JPGAnalizer - Menu");
    this->setObjectName("Okno_Menu");

    label = new QLabel(codec->toUnicode("Menu Główne"), this);
    textFont = label->font();
    textFont.setPointSize(14);
    textFont.setBold(true);
    label->setGeometry(450, 40, 350, 50);
    label->setAlignment(Qt::AlignCenter);
    label->setFont(textFont);

    editableLine = new QLineEdit(codec->toUnicode("Ścieżka do badanego obrazu"), this);
    editableLine->setGeometry(30, 780, 550, 20);

    buttonChooseImage = new QPushButton("Wybierz obraz", this);
    buttonChooseImage->setGeometry(600, 760, 200, 60);
    buttonChooseImage->setFont(textFont);

    buttonExivInfo = new QPushButton(codec->toUnicode("Pokaż informacje Exiv"), this);
    buttonExivInfo->setGeometry(870, 200, 350, 120);
    buttonExivInfo->setFont(textFont);

    buttonSecureImage = new QPushButton("Zabezpiecz obraz", this);
    buttonSecureImage->setGeometry(870, 400, 350, 120);
    buttonSecureImage->setFont(textFont);

    buttonSecureCheck = new QPushButton(codec->toUnicode("Sprawdź zabezpieczenia obrazu"), this);
    buttonSecureCheck->setGeometry(870, 600, 350, 120);
    buttonSecureCheck->setFont(textFont);

    QImageReader reader(QString::fromAscii("./ProgramImages/No-image-found.jpg"));
    defaultImage = reader.read();

    imageLabel = new QLabel(this);
    imageLabel->setGeometry(20, 120, 806, 606);

    setDefaultImage();

    // połączenie zmiany tekstu z walidacją obrazu

    connect(editableLine, SIGNAL(textEdited(QString)), this, SLOT(setMapperTextEdited()));
    connect(editableLine, SIGNAL(textEdited(QString)), mapperTextEdited, SLOT(map()));

    // połączenie wciśnięcia przycisku wyboru pliku z funkcją wyboru pliku

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

// Destruktor klasy WindowMenu
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

// ________________________________________________________________________
// Funkcje mapujące umożliwiają wykorzystanie parametrów w obsłudze zdarzeń
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
//____________________________________________________________________________

// Funkcja chooseFile powoduje pokazanie się okna wyboru pliku
// Po wyborze pliku aktualizowany jest obraz i ścieżka pliku
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
	//	std::cout << fileNames.at(0).toStdString() << std::endl;

		editableLine->setText(fileNames.at(0));
		validateImage(fileNames.at(0));
	}
	return;
}

// Funkcja validateImage aktualizuje pokazywany obraz
// Na wejście otrzymuje ścieżkę do pliku z obrazem
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

// Funkcja setDefaultImage ustala pokazywany obraz na domyślny
void WindowMenu::setDefaultImage()
{
    QPixmap pixmap;
    pixmap.convertFromImage(defaultImage);
    pixmap = pixmap.scaled(800, 600);
    imageLabel->setPixmap(pixmap);
    return;
}
