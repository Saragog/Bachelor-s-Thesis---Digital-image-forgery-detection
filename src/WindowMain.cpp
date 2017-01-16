/*
 * WindowMain.cpp
 *
 *  Created on: 17 sie 2016
 *      Author: Andrzej Dackiewicz
 *
 *  Comment:This file is a part of Digital forgery detection program that was
 *  		an engineering thesis of the author.
 *  		This file implements the main window of the program.
 *  		The WindowMain class can be considered the controller of this program.
 *  		It handles most of the events that take place during program run.
 */

// Headers
#include "WindowMain.h"
#include "WindowMenu.h"

WindowMain::WindowMain()
{
	// Creation of screens used in program
	menu = new WindowMenu(this);
	exivData = new WindowExivData(this);
	secureImage = new WindowSecureImage(this);
	secureCheck = new WindowSecureCheck(this);
	allExivData = new WindowAllExivData(this);

	// QStackedWidget makes it easier to store screens
	stackedWidget = new QStackedWidget();
	// Each screen has an id
	stackedWidget->addWidget(menu); // id 0
	stackedWidget->addWidget(exivData); // id 1
	stackedWidget->addWidget(secureImage); // id 2
	stackedWidget->addWidget(secureCheck); // id 3
	stackedWidget->addWidget(allExivData); // id 4

	// QComboBox makes it easy to switch screens when needed
	pageComboBox = new QComboBox();
	pageComboBox->addItem(tr("WidokMenu"));
	pageComboBox->addItem(tr("WidokExivData"));
	pageComboBox->addItem(tr("WidokSecureImage"));
	pageComboBox->addItem(tr("WidokSecureCheck"));
	pageComboBox->addItem(tr("WidokWszystkieInformacjeExif"));

	// Switching screens depending on the id of the screen we want to see now
	connect(pageComboBox, SIGNAL(activated(int)), stackedWidget, SLOT(setCurrentIndex(int)));

	this->setCentralWidget(stackedWidget);
	this->setGeometry(100, 100, 1256, 924);

	// Firstly we want to see main menu
	showMenu();

	// ExivOperations and DrawingOperations are classes that can be considered as a model of the program
	// They are where most calculations take place
	exivOperations = new ExivOperations(this);
	drawingOperations = new DrawingOperations();

	// MessageBox can be shown if there is a problem with the chosen file
	message.setGeometry(600, 450, 200, 200);
	message.setText("Nie ma takiego pliku !!!");
	message.setDefaultButton(QMessageBox::Ok);

	this->show();
}

WindowMain::~WindowMain()
{
	delete menu;
	delete exivData;
	delete secureImage;
	delete secureCheck;
	delete allExivData;
	delete stackedWidget;
	delete pageComboBox;
	delete exivOperations;
	delete drawingOperations;
}

// Function checkImageFile is used to check if file with a given path is an image file
bool WindowMain::checkImageFile(std::string fileName)
{
	if (exivOperations->isImageFile(fileName))
	{
		return true;
	}
	return false;
}

// Function showMenu is used to switch current screen to main menu screen (id 0)
// It handles back buttons from exif screen, secure image screen and image validation screen
void WindowMain::showMenu()
{
	stackedWidget->setCurrentIndex(0);
	return;
}

// Function showExivData is used to switch current screen to exiv data screen
// It handles show exif metadata button in main menu screen
void WindowMain::showExivData(QString fileName)
{
	// vector for exif data
	std::vector<std::string> data;

	if (checkImageFile(fileName.toLatin1().data())) // checks if a file is an image file
	{
		if (!exivOperations->readFromFile(fileName.toStdString())) // checks if there is exif metadata stored in file
		{
			// Message that no exif metadata was found
			message.setText("W pliku nie znaleziono informacji exif !!!");
			message.exec();
		}
		data = exivOperations->getBasicExifData();
		drawingOperations->setImage(fileName.toStdString()); // drawingOperations will operate on a copy of image stored in this file

		// calculations for creating grey and RGB histograms
		QImage histogramGrey = drawingOperations->getHistogramGrey();
		QImage histogramRGB = drawingOperations->getHistogramRGB();

		// preparing screen
		exivData->prepareWindow(data);
		exivData->setImage(fileName);
		exivData->setHistogramGrey(histogramGrey);
		exivData->setHistogramRGB(histogramRGB);
		// switching to exif data screen
		stackedWidget->setCurrentIndex(1);
	}
	else
	{	// no such file was found
		message.setText("Nie ma takiego pliku !!!");
		message.exec();
	}
	return;
}

// Function showSecureImage is used to switch current screen to screen of securing image
// It handles secure image button in main menu screen
void WindowMain::showSecureImage(QString fileName)
{
	if (checkImageFile(fileName.toLatin1().data())) // checks if a file is an image file
	{
		// setting model for actual file
		exivOperations->readFromFile(fileName.toStdString());
		secureImage->setImage(fileName);
		drawingOperations->setImage(fileName.toStdString());

		// checks if image is big enough to secure it (at least 97 x 97 pixels)
		if (!drawingOperations->isLargeEnough())
		{
			// image is too small - message is shown
			message.setText("Ten obraz jest za maly aby stworzyc na nim zabezpieczenia !!!");
			message.exec();
		}
		else
		{
			// switching to secure image screen
			stackedWidget->setCurrentIndex(2);
		}
	}
	else
	{
		// message shown in case there is no such file
		message.setText("Nie ma takiego pliku !!!");
		message.exec();
	}
	return;
}

// Function showSecureCheck is used to switch current screen to screen showing results of image validation
// It handles check security button in main menu screen
void WindowMain::showSecureCheck(QString fileName)
{
	std::vector<std::pair<bool, std::string> > exifRaport, imageRaport, concatRaport;
	std::vector<int> savedGreyTones;
	QImage histogramComparison;
	QImage checkedImage;

	if (checkImageFile(fileName.toLatin1().data())) 	// checks if file is an image file
	{
		// setting model to current file
		secureCheck->setImage(fileName);
		drawingOperations->setImage(fileName.toStdString());

		if (!drawingOperations->isLargeEnough()) // checks if image is large enough for validation
		{
			// message when image is too small
			message.setText("Ten obraz jest za maly aby sprawdzic dla niego zabezpieczenia !!!");
			message.exec();
			return;
		}
		else if (exivOperations->readFromFile(fileName.toStdString())) // checks if there is exif metadata in file
		{
			exivOperations->checkExifSecurity();
			savedGreyTones = exivOperations->getSavedGreyTones();
		}
		else
		{
			exivOperations->checkExifSecurity();
			// message when no exif was found in file
			message.setText("W pliku nie znaleziono informacji exif !!!");
			message.exec();
		}

		drawingOperations->checkImageSecurity(savedGreyTones);

		// preparation of validation raport
		exifRaport = exivOperations->getRaportExif();
		imageRaport = drawingOperations->getRaportImage();

		concatRaport = exifRaport;
		for (unsigned int x = 0; x < imageRaport.size(); x++)
		{
			concatRaport.push_back(imageRaport.at(x));
		}

		// getting hisgotram and result of image validation
		histogramComparison = drawingOperations->getHistogramComparison();
		checkedImage = drawingOperations->getCheckedImage();

		// preparation of screen
		secureCheck->setRaport(concatRaport);
		secureCheck->setHistogramC(histogramComparison);
		secureCheck->setCheckedImage(checkedImage);

		// switching actual screen to secure check screen
		stackedWidget->setCurrentIndex(3);
	}
	else
	{
		// message that file was not found
		message.setText("Nie ma takiego pliku !!!");
		message.exec();
	}
	return;
}

// Function showExivDataAgain handles back button in all exif screen
// Switches AllExif screen back to ExifData screen
void WindowMain::showExivDataAgain()
{
	stackedWidget->setCurrentIndex(1);
	return;
}

// Function showAllExivData is used to switch current screen to AllExivData screen
// It handles show all exif data button in exif data screen
void WindowMain::showAllExivData()
{
	std::vector<std::string> data;

	// preparation of screen
	data = exivOperations->getAllExifData();
	allExivData->prepareWindow(data);
	// switching actual screen to all exiv metadata screen
	stackedWidget->setCurrentIndex(4);
}

// Function saveImage is used to create a secured image in given location
// path is the location of new secured file
void WindowMain::saveImage(QString path)
{
	if (drawingOperations->saveImage(path)) // secures image and saves it in given location
	{
		// writes exif metadata to an existing secured file
		exivOperations->saveExifIntoFile(path.toStdString());
		showMenu(); // switches actual screen to main menu screen
	}
	else
	{
		// message if failed to secure image
		message.setText("Nie udalo sie zapisac pliku !!!");
		message.exec();
	}
	return;
}

// Function adjustToNewAccDif handles check for set acceptable difference button
void WindowMain::adjustToNewAccDif(int newAccDif)
{
	QImage checkedImage;
	// get new result of image validation
	checkedImage = drawingOperations->checkForNewAccDif(newAccDif);
	// adjust screen to new result image
	secureCheck->setCheckedImage(checkedImage);
	return;
}

// Function getHistGTones returns numerousness of each of 153 groups of shades of grey
int* WindowMain::getHistGTones() const
{
	return drawingOperations->getHistGTones();
}

// Function getActualSize returns size of image
std::pair<int, int> WindowMain::getActualSize() const
{
	return drawingOperations->getActualSize();
}
