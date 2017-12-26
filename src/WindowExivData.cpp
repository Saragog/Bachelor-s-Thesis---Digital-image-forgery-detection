/*
 * WindowExivData.cpp
 *
 *  Created on: 17 sie 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Komentarz: Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Implementuje klasę WindowExivData, której celem jest przedstawienie histogramu sum RGB, histogramu RGB i pokazuje wybrane pola EXIF.
 */

#include "WindowExivData.h"
#include <iostream>

// konstruktor klasy WindowExivData
WindowExivData::WindowExivData (QWidget* main)
{
	mainWindow = main;
	codec = QTextCodec::codecForName("UTF-8");
	scrollPalette.setColor(QPalette::Background, Qt::lightGray);
    this->setWindowTitle("JPGAnalizer - Exif Data");
    textFont = QFont();
    textFont.setPointSize(14);
    textFont.setBold(true);

    // przycisk powrotu do menu głównego
    buttonReturn = new QPushButton(codec->toUnicode("Powrót"), this);
    buttonReturn->setGeometry(50,850, 180, 60);
    buttonReturn->setFont(textFont);
    // przycisk do okna pokazującego wszystkie metadane exif
    buttonAll = new QPushButton(codec->toUnicode("Pokaż wszystkie informacje exif"), this);
    buttonAll->setGeometry(250, 850, 330, 60);
    buttonAll->setFont(textFont);
    // miejsce na badany obraz
    imageLabel = new QLabel(this);
    imageLabel->setGeometry(10, 10, 606, 456);
    // histogram sum barw RGB
    histogramGreyLabel = new QLabel(this);
    histogramGreyLabel->setGeometry(620, 10, 606, 456);
    // hisgoram barw RGB
    histogramRGBLabel = new QLabel(this);
    histogramRGBLabel->setGeometry(620, 470, 606, 456);
    // obsługa zdarzeń naciśnięć na przyciski
    connect(buttonReturn, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
    connect (buttonAll, SIGNAL(clicked()), mainWindow, SLOT(showAllExivData()));
}

// Destruktor klasy WindowExivData
WindowExivData::~WindowExivData()
{
	clearInfo();
	delete label;
	delete buttonReturn;
	delete buttonAll;
	delete imageLabel;
	delete histogramGreyLabel;
	delete histogramRGBLabel;
	delete exivInfoScroll;
}

// Funkcja clearInfo czyści miejsce, gdzie pokazywana jest zawartość wybranych metadanych EXIF
void WindowExivData::clearInfo()
{
	QLayoutItem* temp;
	while ((temp = infoLayout.takeAt(0)) != 0)
	{
		delete temp;
	}
	return;
}

// Funkcja prepareWindow przygotowuje okno do wyświetlenia.
// Na wejście otrzymuje wektor napisów, jakie powinien pokazać
void WindowExivData::prepareWindow(std::vector<std::string> data)
{
	clearInfo(); // czyszczenie z aktualnej zawartości
    exivInfoScroll = new QScrollArea(this);
    infoWidget = new QWidget;
    infoWidget->setLayout(&infoLayout);
    infoWidget->setAutoFillBackground(false);
    exivInfoScroll->setWidget(infoWidget);
    exivInfoScroll->viewport()->setAutoFillBackground(true);
    exivInfoScroll->viewport()->setPalette(scrollPalette);
    exivInfoScroll->setWidgetResizable(true);
    exivInfoScroll->setGeometry(10, 470, 600, 370);
    // pokolei dodawana jest nowa zawartość
	for (unsigned int step = 0; step < data.size(); step++)
	{
		infoLabel = new QLabel(QString::fromStdString(data.at(step)));
		infoLabel->setGeometry(100, 200 + 100*step, 100, 50);
		infoLayout.addWidget(infoLabel);
	}
	return;
}

// Funkcja setImage ustala pokazywany obraz na ten z pliku o określonej ścieżce
void WindowExivData::setImage(QString path)
{
	fileName = path.toStdString();
	QPixmap pixmap(path);
	pixmap = pixmap.scaled(600, 450);
	imageLabel->setPixmap(pixmap);

	return;
}

// Funkcja setHistogramGrey aktualizuje histogram sum barw RGB
void WindowExivData::setHistogramGrey(QImage image)
{
	QPixmap pixmap;
	pixmap = pixmap.fromImage(image);
	histogramGreyLabel->setPixmap(pixmap);
	return;
}

// Funkcja setHistogramRGB aktualizuje histogram barw RGB
void WindowExivData::setHistogramRGB(QImage image)
{
	QPixmap pixmap;
	pixmap = pixmap.fromImage(image);
	histogramRGBLabel->setPixmap(pixmap);
	return;
}
