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
	codec = QTextCodec::codecForName("UTF-8");
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

	exivInfoScroll = new QScrollArea(this);

    infoWidget = new QWidget;
    infoWidget->setLayout(&infoLayout);
    infoWidget->setAutoFillBackground(false);
    exivInfoScroll->setWidget(infoWidget);

    exivInfoScroll->setWidgetResizable(true);
    exivInfoScroll->setGeometry(150, 100, 900, 670);

    exivInfoScroll->viewport()->setAutoFillBackground(true);
    exivInfoScroll->viewport()->setPalette(scrollPalette);

	for (unsigned int step = 0; step < data.size(); step++)
	{
		infoLabel = new QLabel(QString::fromStdString(data.at(step)));
		infoLabel->setGeometry(100, 200 + 100*step, 100, 50);
		infoLayout.addWidget(infoLabel);
	}
	return;
}
