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

		secureImage->setFile(fileName);
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
	std::vector<std::pair<bool, std::string> > exifRaport;
	std::vector<int> savedValues[2];
	QImage histogramComparison;
	QImage checkedImage;

	if (checkImageFile(fileName.toLatin1().data()))
	{

		if (!exivOperations->readFromFile(fileName.toStdString()))
		{
			// TODO zamienic to na ze obraz nie jest zabezpieczony lub usunieto informacje exif
			message.setText("W pliku nie znaleziono informacji exif !!!");
			message.exec();
		}
		else
		{
			secureCheck->setFile(fileName);
			drawingOperations->setImage(fileName.toStdString());

			if (exivOperations->checkExifSecurity())
			{
				// TODO by to zwracalo jakis raport wyniku sprawdzania exifow
				// moze tam jak sie wykonuje to raport jest tworzony i inna funkcja go zwracac bedzie

				savedValues[0] = exivOperations->getSavedCornerRGBs();
				savedValues[1] = exivOperations->getSavedGreyTones();

				drawingOperations->checkImageSecurity(savedValues);

				// TODO zrobic raporty w sprawdzaniu exif i raport w sprawdzaniu obrazu
				// tutaj to pobierac i przekazac na wyjscie
				// teraz robie przekazanie obrazka histogramu zmienionego i sprawdzonego obrazu

				exifRaport = exivOperations->getRaportExif();
				histogramComparison = drawingOperations->getHistogramComparison();
				checkedImage = drawingOperations->getCheckedImage();

				secureCheck->setExifRaport(exifRaport);
				secureCheck->setHistogramC(histogramComparison);
				secureCheck->setCheckedImage(checkedImage);

				stackedWidget->setCurrentIndex(3);
			}
			else
			{
				message.setText("W wybranym pliku nie wykryto zabezpieczen !!!");
				message.exec();
			}
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
		// TODO wyswietlenie komunikatu ze nie udalo sie zapisac pliku
	}
	return;
}

std::vector<int> WindowMain::getCorners() const
{
	std::vector<int> cornerRGBs = drawingOperations->getCornerRGBs();
	return cornerRGBs;
}

int* WindowMain::getHistGTones() const
{
	//std::vector<int> greyHistValues = drawingOperations->getHistogramGrey();

	return drawingOperations->getHistGTones();
}
