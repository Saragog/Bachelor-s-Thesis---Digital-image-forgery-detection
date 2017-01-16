/*
 * WindowExivData.cpp
 *
 *  Created on: 17 sie 2016
 *      Author: andrzej
 */

#include "WindowExivData.h"
#include <iostream>

WindowExivData::WindowExivData (QWidget* main)
{
	mainWindow = main;

	scrollPalette.setColor(QPalette::Background, Qt::lightGray);

    this->setWindowTitle("JPGAnalizer - Exif Data");

    textFont = QFont();
    textFont.setPointSize(14);
    textFont.setBold(true);

    buttonReturn = new QPushButton("Powrot", this);
    buttonReturn->setGeometry(50,850, 180, 60);
    buttonReturn->setFont(textFont);

    buttonAll = new QPushButton("Pokaz wszystkie informacje exif", this);
    buttonAll->setGeometry(250, 850, 330, 60);
    buttonAll->setFont(textFont);

    imageLabel = new QLabel(this);
    imageLabel->setGeometry(10, 10, 606, 456);

    histogramGreyLabel = new QLabel(this);
    histogramGreyLabel->setGeometry(620, 10, 606, 456);

    histogramRGBLabel = new QLabel(this);
    histogramRGBLabel->setGeometry(620, 470, 606, 456);

    connect(buttonReturn, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
    connect (buttonAll, SIGNAL(clicked()), mainWindow, SLOT(showAllExivData()));
}

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

void WindowExivData::clearInfo()
{
	QLayoutItem* temp;
	while ((temp = infoLayout.takeAt(0)) != 0)
	{
		delete temp;
	}
	return;
}

void WindowExivData::prepareWindow(std::vector<std::string> data)
{
	clearInfo();

    exivInfoScroll = new QScrollArea(this);

    infoWidget = new QWidget;
    infoWidget->setLayout(&infoLayout);
    infoWidget->setAutoFillBackground(false);
    exivInfoScroll->setWidget(infoWidget);

    exivInfoScroll->viewport()->setAutoFillBackground(true);
    exivInfoScroll->viewport()->setPalette(scrollPalette);

    exivInfoScroll->setWidgetResizable(true);
    exivInfoScroll->setGeometry(10, 470, 600, 370);

	for (unsigned int step = 0; step < data.size(); step++)
	{
		infoLabel = new QLabel(QString::fromStdString(data.at(step)));
		infoLabel->setGeometry(100, 200 + 100*step, 100, 50);
		infoLayout.addWidget(infoLabel);
	}
	return;
}

void WindowExivData::setImage(QString path)
{
	fileName = path.toStdString(); // TODO mozna dorobic pokazywanie nazwy / sciezki aktualnego pliku
	QPixmap pixmap(path);
	pixmap = pixmap.scaled(600, 450);
	imageLabel->setPixmap(pixmap);

	return;
}

void WindowExivData::setHistogramGrey(QImage image)
{
	QPixmap pixmap;
	pixmap = pixmap.fromImage(image);
	histogramGreyLabel->setPixmap(pixmap);
	return;
}

void WindowExivData::setHistogramRGB(QImage image)
{
	QPixmap pixmap;
	pixmap = pixmap.fromImage(image);
	histogramRGBLabel->setPixmap(pixmap);
	return;
}
