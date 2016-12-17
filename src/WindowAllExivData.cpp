/*
 * WindowAllExivData.cpp
 *
 *  Created on: 9 wrz 2016
 *      Author: andrzej
 */

#include "WindowAllExivData.h"
#include <iostream>

WindowAllExivData::WindowAllExivData (QWidget* main)
{
	mainWindow = main;
    this->setWindowTitle("JPGAnalizer");

    label = new QLabel("Wszystkie Informacje Exif znalezione w pliku", this);
    label->setGeometry(20, 20, 400, 100);

    buttonReturn = new QPushButton("Powrot", this);
    buttonReturn->setGeometry(50,800, 200, 60);

	scrollPalette.setColor(QPalette::Background, Qt::lightGray);
    exivInfoScroll = new QScrollArea(this);
    infoWidget = new QWidget;
    infoWidget->setLayout(&infoLayout);

    exivInfoScroll->setWidgetResizable(true);
    exivInfoScroll->setGeometry(150, 100, 900, 670);

    connect(buttonReturn, SIGNAL(clicked()), mainWindow, SLOT(showExivDataAgain()));
}

WindowAllExivData::~WindowAllExivData()
{
	clearInfo();
	delete label;
	delete buttonReturn;
}

// _______________________________________________________________

void WindowAllExivData::clearInfo()
{
	QLayoutItem* temp;
	while ((temp = infoLayout.takeAt(0)) != 0)
	{
		delete temp;
	}
	return;
}

void WindowAllExivData::prepareWindow(std::vector<std::string> data)
{
	clearInfo();

    exivInfoScroll->setWidget(infoWidget);
    exivInfoScroll->viewport()->setAutoFillBackground(true);
    exivInfoScroll->viewport()->setPalette(scrollPalette);

	for (unsigned int step = 0; step < data.size(); step++)
	{
		infoLabel = new QLabel(QString::fromStdString(data.at(step)));
		infoLabel->setGeometry(100, 200 + 100*step, 100, 50);

		infoLayout.addWidget(infoLabel);
	}
}
