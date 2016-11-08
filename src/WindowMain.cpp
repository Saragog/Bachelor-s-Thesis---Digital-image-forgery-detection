/*
 * WindowMain.cpp
 *
 *  Created on: 17 sie 2016
 *      Author: andrzej
 */

#include "WindowMain.h"
#include "WindowMenu.h"
#include <iostream>

WindowMain::WindowMain()
{
	menu = new WindowMenu(this);
	exivData = new WindowExivData(this);
	secureImage = new WindowSecureImage(this);
	secureCheck = new WindowSecureCheck(this);
	allExivData = new WindowAllExivData(this);

	stackedWidget = new QStackedWidget();
	stackedWidget->addWidget(menu);
	stackedWidget->addWidget(exivData);
	stackedWidget->addWidget(secureImage);
	stackedWidget->addWidget(secureCheck);
	stackedWidget->addWidget(allExivData);

	pageComboBox = new QComboBox();
	pageComboBox->addItem(tr("WidokMenu"));
	pageComboBox->addItem(tr("WidokExivData"));
	pageComboBox->addItem(tr("WidokSecureImage"));
	pageComboBox->addItem(tr("WidokSecureCheck"));
	pageComboBox->addItem(tr("WidokWszystkieInformacjeExif"));

	connect(pageComboBox, SIGNAL(activated(int)), stackedWidget, SLOT(setCurrentIndex(int)));

	this->setCentralWidget(stackedWidget);

	this->setGeometry(100, 100, 1256, 924);

	showMenu();

	// ______________ UWAGA POTEM TO GDZIE INDZIEJ CZY COS | V | V | V |

	exivOperations = new ExivOperations(this);
	drawingOperations = new DrawingOperations();

	// _______________________________________________________________

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
}

bool WindowMain::checkImageFile(std::string fileName)
{
	if (exivOperations->isImageFile(fileName))
	{
		return true;
	}
	return false;
}

void WindowMain::showMenu()
{
	stackedWidget->setCurrentIndex(0);
}

void WindowMain::showExivData(QString fileName)
{
	std::vector<std::string> data;

	if (checkImageFile(fileName.toLatin1().data()))
	{
		if (!exivOperations->readFromFile(fileName.toStdString()))
		{
			// Ostrzezenie ze nie znaleziono informacji exif
			message.setText("W pliku nie znaleziono informacji exif !!!");
			message.exec();
		}
		data = exivOperations->getBasicExifData();

		drawingOperations->setImage(fileName.toStdString());

		QImage histogramGrey = drawingOperations->getHistogramGrey();
		QImage histogramRGB = drawingOperations->getHistogramRGB();

		exivData->prepareWindow(data);
		exivData->setImage(fileName);
		exivData->setHistogramGrey(histogramGrey);
		exivData->setHistogramRGB(histogramRGB);
		stackedWidget->setCurrentIndex(1);
	}
	else
	{
		message.setText("Nie ma takiego pliku !!!");
		message.exec();
	}
}

void WindowMain::showSecureImage(QString fileName)
{
	if (checkImageFile(fileName.toLatin1().data()))
	{
		exivOperations->readFromFile(fileName.toStdString()); // TODO ta funkcja zwraca true false
															  // mozna wywalic info ze zapis zakonczony powodzeniem czy cos

		secureImage->setImage(fileName);
		drawingOperations->setImage(fileName.toStdString());
		stackedWidget->setCurrentIndex(2);
	}
	else
	{
		message.setText("Nie ma takiego pliku !!!");
		message.exec();
	}
}

void WindowMain::showSecureCheck(QString fileName)
{
	std::vector<std::pair<bool, std::string> > exifRaport, imageRaport, concatRaport;
	std::vector<int> savedGreyTones;
	QImage histogramComparison;
	QImage checkedImage;

	if (checkImageFile(fileName.toLatin1().data()))
	{

		secureCheck->setImage(fileName);
		drawingOperations->setImage(fileName.toStdString());

		if (!exivOperations->readFromFile(fileName.toStdString()))
		{
			exivOperations->checkExifSecurity();
			drawingOperations->checkImageSecurity(savedGreyTones);

			exifRaport = exivOperations->getRaportExif();
			imageRaport = drawingOperations->getRaportImage();

			concatRaport = exifRaport;
			for (unsigned int x = 0; x < imageRaport.size(); x++)
			{
				concatRaport.push_back(imageRaport.at(x));
			}

			histogramComparison = drawingOperations->getHistogramComparison();
			checkedImage = drawingOperations->getCheckedImage();

			secureCheck->setRaport(concatRaport);
			secureCheck->setHistogramC(histogramComparison);
			secureCheck->setCheckedImage(checkedImage);

			stackedWidget->setCurrentIndex(3);

			message.setText("W pliku nie znaleziono informacji exif !!!");
			message.exec();
		}
		else
		{
			exivOperations->checkExifSecurity();

			savedGreyTones = exivOperations->getSavedGreyTones();

			drawingOperations->checkImageSecurity(savedGreyTones);

			exifRaport = exivOperations->getRaportExif();
			imageRaport = drawingOperations->getRaportImage();

			concatRaport = exifRaport;
			for (unsigned int x = 0; x < imageRaport.size(); x++)
			{
				concatRaport.push_back(imageRaport.at(x));
			}

			histogramComparison = drawingOperations->getHistogramComparison();
			checkedImage = drawingOperations->getCheckedImage();

			secureCheck->setRaport(concatRaport);
			secureCheck->setHistogramC(histogramComparison);
			secureCheck->setCheckedImage(checkedImage);

			stackedWidget->setCurrentIndex(3);

//			else
//			{
//				message.setText("W wybranym pliku nie wykryto zabezpieczen !!!");
//				message.exec();
//			}
		}
	}
	else
	{
		message.setText("Nie ma takiego pliku !!!");
		message.exec();
	}
}

void WindowMain::showExivDataAgain()
{
	stackedWidget->setCurrentIndex(1);
}

void WindowMain::showAllExivData()
{
	std::vector<std::string> data;

	data = exivOperations->getAllExifData();

	allExivData->prepareWindow(data);
	stackedWidget->setCurrentIndex(4);
}

void WindowMain::saveImage(QString path)
{
	// TODO Pozniej bedzie tutaj argument ( obraz i typ zabezpieczenia najpewniej )

	if (drawingOperations->saveImage(path))
	{
		exivOperations->saveExifIntoFile(path.toStdString());
		showMenu();
	}
	else
	{
		message.setText("Nie udalo sie zapisac pliku !!!");
		message.exec();
	}
	return;
}

int* WindowMain::getHistGTones() const
{
	//std::vector<int> greyHistValues = drawingOperations->getHistogramGrey();

	return drawingOperations->getHistGTones();
}
