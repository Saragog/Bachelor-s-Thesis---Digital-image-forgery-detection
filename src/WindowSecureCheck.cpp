/*
 * WindowSecureCheck.cpp
 *
 *  Created on: 24 sie 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Komentarz: Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Implementuje funkcje klasy WindowSecureCheck pokazującej wyniki działania programu.
 */

#include "WindowSecureCheck.h"
#include <QImageReader>

// Konstruktor klasy WindowSecureCheck
WindowSecureCheck::WindowSecureCheck(QWidget* main)
{
	this->mainWindow = main;
	codec = QTextCodec::codecForName("UTF-8");
    this->setWindowTitle("JPGAnalizer - Secure Check");
    mapperAccDif = new QSignalMapper(this);
    // domyślny wygląd okna weryfikacji obrazu
	histogramCLabel = new QLabel(this);
	histogramCLabel->setGeometry(620, 10, 600, 450);
	imageLabel = new QLabel(this);
	imageLabel->setGeometry(10, 10, 606, 456);
	textFont = QFont();
	textFont.setPointSize(14);
	textFont.setBold(true);
	changeImagesButton = new QPushButton(this);
	changeImagesButton->setGeometry(20, 480, 250, 80);
	changeImagesButton->setFont(textFont);
	accLabel = new QLabel(codec->toUnicode("Dokładność sprawdzenia:"), this);
	accLabel->setGeometry(20, 580, 250, 50);
	accLabel->setFont(textFont);
    spinBox = new QSpinBox(this);
    spinBox->setMinimum(0);
    spinBox->setMaximum(20);
    spinBox->setGeometry(20, 630, 250, 40);
    // przycisk weryfikacji dla podanej wartości dokładności
	adjustButton = new QPushButton(codec->toUnicode("Sprawdź dla danej\ndokładności"), this);
	adjustButton->setGeometry(20, 690, 250, 80);
	adjustButton->setFont(textFont);
	// przycisk powrotu
	backButton = new QPushButton(codec->toUnicode("Powrót"), this);
	backButton->setGeometry(20, 790, 250, 80);
	backButton->setFont(textFont);
	positiveResult = QPixmap("./ProgramImages/tick.png");
	negativeResult = QPixmap("./ProgramImages/x.png");
	currentImage = true;
	raportPalette.setColor(QPalette::Background, Qt::lightGray);
    raportScroll = new QScrollArea(this);
    raportWidget = new QWidget;
    raportWidget->setLayout(&raportLayout);
    raportScroll->setWidgetResizable(true);
    raportScroll->setGeometry(300, 470, 900, 450);

    // obsługa zdarzeń
    connect(adjustButton, SIGNAL(clicked()), this, SLOT(setMapperAccDif()));
    connect(adjustButton, SIGNAL(clicked()), mapperAccDif, SLOT(map()));
    mapperAccDif->setMapping(adjustButton, spinBox->value());
    connect(mapperAccDif, SIGNAL(mapped(int)), mainWindow, SLOT(adjustToNewAccDif(int)));
	connect(backButton, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
	connect(changeImagesButton, SIGNAL(clicked()), this, SLOT(changeImage()));
}

// Destruktor klasy WindowSecureCheck
WindowSecureCheck::~WindowSecureCheck()
{
}

// Funkcja setImage ustala badany obraz
void WindowSecureCheck::setImage(QString path)
{
	pixmapImage = QPixmap(path);
	pixmapImage = pixmapImage.scaled(600, 450);
	spinBox->setValue(3);
	return;
}

// Funkcja clearRaport czyści raport weryfikacji obrazu
void WindowSecureCheck::clearRaport()
{
	QLayoutItem* temp;
	while ((temp = raportLayout.takeAt(0)) != 0)
	{
		delete temp;
	}
	return;
}

// Funkcja setRaport służy do ustalania pokazywanego raportu wyników weryfikacji obrazu
void WindowSecureCheck::setRaport(std::vector<std::pair<bool, QString> > raport)
{
	clearRaport(); // czyszczenie aktualnego raportu
    raportScroll->setWidget(raportWidget);
    raportScroll->viewport()->setAutoFillBackground(true);
    raportScroll->viewport()->setPalette(raportPalette);

	for (unsigned int step = 0; step < raport.size(); step++) // wypełnienie nowymi danymi
	{
		QLabel* n = new QLabel(this);
		if (raport.at(step).first) n->setPixmap(positiveResult);
		else n->setPixmap(negativeResult);
		raportLayout.addWidget(n, step, 0);
		raportLabel = new QLabel(raport.at(step).second);
		raportLayout.addWidget(raportLabel, step, 1, 1, 13);
	}
	return;
}

// Funkcja setHistogramC służy do aktualizacji histogramu porównawczego
void WindowSecureCheck::setHistogramC(QImage histogram)
{
	QPixmap pixmap;
	pixmap = pixmap.fromImage(histogram);
	histogramCLabel->setPixmap(pixmap);
	return;
}

// Funkcja setCheckedImage służy do aktualizacji badanego obrazu
// powoduje przestawienie widoku na badany obraz
void WindowSecureCheck::setCheckedImage(QImage image)
{
	pixmapCheckedImage = QPixmap::fromImage(image);
	pixmapCheckedImage = pixmapCheckedImage.scaled(600, 450);
	imageLabel->setPixmap(pixmapImage);
	changeImagesButton->setText(codec->toUnicode("Pokaż wynikowy obraz"));
	currentImage = true;

	return;
}

// Funkcja setMapperAccDif pozwala na wykorzystanie wartości wpisanej w spinBox do wykonania nowych badań na obrazie
void WindowSecureCheck::setMapperAccDif()
{
	mapperAccDif->setMapping(adjustButton, spinBox->value());
	return;
}

// Funkcja changeImage powoduje przestawienie obrazu z badanego na wynikowy i odwrotnie
void WindowSecureCheck::changeImage()
{
	if (currentImage)
	{
		currentImage = false;
		imageLabel->setPixmap(pixmapCheckedImage);
		changeImagesButton->setText(codec->toUnicode("Pokaż badany obraz"));
	}
	else
	{
		currentImage = true;
		imageLabel->setPixmap(pixmapImage);
		changeImagesButton->setText(codec->toUnicode("Pokaż wynikowy obraz"));
	}
	return;
}
