/*
 * WindowAllExivData.cpp
 *
 *  Created on: 9 wrz 2016
 *      Autor: Andrzej Dackiewicz
 *
 *      Komentarz:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Ten plik zawiera implementację klasy WindowAllExivData. Pokazuje okno przedstawiające zawartość wszystkich pól EXIF znalezionych w obrazie
 */

#include "WindowAllExivData.h"

// Konstruktor klasy WindowAllExivData
WindowAllExivData::WindowAllExivData (QWidget* main)
{
	mainWindow = main;
	codec = QTextCodec::codecForName("UTF-8");
	// inicjalizacja widoku okna
    this->setWindowTitle("JPGAnalizer - All Exif Data");
    label = new QLabel("Wszystkie informacje Exif znalezione w pliku", this);
    textFont = label->font();
    textFont.setPointSize(14);
    textFont.setBold(true);
    label->setGeometry(400, 40, 450, 50);
    label->setAlignment(Qt::AlignCenter);
    label->setFont(textFont);
    buttonReturn = new QPushButton(codec->toUnicode("Powrót"), this);
    buttonReturn->setGeometry(500, 780, 250, 120);
    buttonReturn->setFont(textFont);
	scrollPalette.setColor(QPalette::Background, Qt::lightGray);
	// reakcja na zdarzenie kliknięcia przycisku powrotu
    connect(buttonReturn, SIGNAL(clicked()), mainWindow, SLOT(showExivDataAgain()));
}

// destruktor
WindowAllExivData::~WindowAllExivData()
{
	// czyszczenie wszystkich obiektów zawartych w treści scrollarea
	clearInfo();
	delete label;
	delete buttonReturn;
}

// Funkcja clearInfo usuwa obiekty w treści scrollarea
void WindowAllExivData::clearInfo()
{
	QLayoutItem* temp;
	while ((temp = infoLayout.takeAt(0)) != 0)
	{
		delete temp;
	}
	return;
}

// Funkcja prepareWindow przygotowuje okno do pokazania użytkownikowi
// Otrzymuje na wejście wektor treści zawartości pól metadanych EXIF
void WindowAllExivData::prepareWindow(std::vector<std::string> data)
{
	clearInfo();	// czyszczenie aktualnej zawartości scrollarea
	exivInfoScroll = new QScrollArea(this);
    infoWidget = new QWidget;
    infoWidget->setLayout(&infoLayout);
    infoWidget->setAutoFillBackground(false);
    exivInfoScroll->setWidget(infoWidget);
    exivInfoScroll->setWidgetResizable(true);
    exivInfoScroll->setGeometry(150, 100, 900, 670);
    exivInfoScroll->viewport()->setAutoFillBackground(true);
    exivInfoScroll->viewport()->setPalette(scrollPalette);
    // wstawianie obiektów QLabel z odpowiednią treścią do przestrzeni, gdzie mają te informacje zostać pokazane
	for (unsigned int step = 0; step < data.size(); step++)
	{
		infoLabel = new QLabel(QString::fromStdString(data.at(step)));
		infoLabel->setGeometry(100, 200 + 100*step, 100, 50);
		infoLayout.addWidget(infoLabel);
	}
	return;
}
