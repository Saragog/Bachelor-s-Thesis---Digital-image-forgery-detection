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
	// TODO Auto-generated constructor stub

	std::cout << "Robie Konstruktora WindowExivData !!!";

	mainWindow = main;

	palette.setColor(QPalette::Background, Qt::cyan);
	this->setAutoFillBackground(true);
	this->setPalette(palette);

    //this->setGeometry(100, 100, 500, 400);
    this->setWindowTitle("JPGAnalizer");

    label = new QLabel("Informacje EXIF i diagram ... jakistam ... nie wiem jaki :D ", this);
    label->setGeometry(20, 20, 200, 100);

    button = new QPushButton("Powrot", this);
    button->setGeometry(100,600, 200, 100);

    exivInfoScroll = new QScrollArea(this);



    infoLayout = new QGridLayout;

    infoLabel = new QLabel("Ten tekst chce wyswietlic ...");
    infoLabel->setGeometry(100,200,100,50);

    infoLabel2 = new QLabel("Tekst do wyswietlenia nr 2 ...");
    infoLabel2->setGeometry(100,300,100, 50);

    infoLayout->addWidget(infoLabel);
    infoLayout->addWidget(infoLabel2);

    infoWidget = new QWidget;
    infoWidget->setLayout(infoLayout);
    exivInfoScroll->setWidget(infoWidget);

    exivInfoScroll->viewport()->setBackgroundRole(QPalette::Dark);
    exivInfoScroll->viewport()->setAutoFillBackground(true);

    exivInfoScroll->setWidgetResizable(true);
    exivInfoScroll->setGeometry(100, 200, 100, 400);

    connect(button, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
}

WindowExivData::~WindowExivData() {
	// TODO Auto-generated destructor stub
}

void WindowExivData::prepareWindow(std::vector<std::string> data)
{

}
