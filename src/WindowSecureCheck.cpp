/*
 * WindowSecureCheck.cpp
 *
 *  Created on: 24 sie 2016
 *      Author: andrzej
 */

#include "WindowSecureCheck.h"

WindowSecureCheck::WindowSecureCheck(QWidget* main)
{
	this->mainWindow = main;

	label = new QLabel("Window Secure Check", this);
	label->setGeometry(100, 100, 200, 100);

	histogramCLabel = new QLabel(this);
	histogramCLabel->setGeometry(620, 10, 650, 450);

	imageLabel = new QLabel(this);
	imageLabel->setGeometry(10, 10, 600, 450);

	backButton = new QPushButton("Powrot", this);
	backButton->setGeometry(100, 500, 100, 100);

	timer = new QTimer(this);

	currentImage = true;

	raportPalette.setColor(QPalette::Background, Qt::lightGray);
    raportScroll = new QScrollArea(this);
    raportWidget = new QWidget;
    raportWidget->setLayout(&raportLayout);

    raportScroll->setWidgetResizable(true);
    raportScroll->setGeometry(300, 470, 900, 450);

	connect(backButton, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
	connect(backButton, SIGNAL(clicked()), this, SLOT(stopTimer()));
	connect(timer, SIGNAL(timeout()), this, SLOT(changeImage()));
}

WindowSecureCheck::~WindowSecureCheck()
{
}

void WindowSecureCheck::setImage(QString path)
{
	fileName = path.toStdString();

	pixmapImage = QPixmap(path);
	pixmapImage = pixmapImage.scaled(600, 450);

	return;
}

void WindowSecureCheck::clearRaport()
{
	QLayoutItem* temp;
	while ((temp = raportLayout.takeAt(0)) != 0)
	{
		delete temp;
	}
	return;
}

void WindowSecureCheck::setRaport(std::vector<std::pair<bool, std::string> > raport)
{
	clearRaport();

    raportScroll->setWidget(raportWidget);
    raportScroll->viewport()->setAutoFillBackground(true);
    raportScroll->viewport()->setPalette(raportPalette);

	for (unsigned int step = 0; step < raport.size(); step++)
	{
		// TODO zrobic pojawianie sie znakow ze sie udalo lub nie obok - znaki sa gotowe do zrobienia tego
		raportLabel = new QLabel(QString::fromStdString(raport.at(step).second));
		raportLabel->setGeometry(300, 470 + 50*step, 900, 50);

		raportLayout.addWidget(raportLabel);
	}

	return;
}

void WindowSecureCheck::setHistogramC(QImage histogram)
{
	QPixmap pixmap;
	pixmap = pixmap.fromImage(histogram);
	histogramCLabel->setPixmap(pixmap);
	return;
}

void WindowSecureCheck::setCheckedImage(QImage image)
{
	pixmapCheckedImage = QPixmap::fromImage(image);
	pixmapCheckedImage = pixmapCheckedImage.scaled(600, 450);
	imageLabel->setPixmap(pixmapImage);

	currentImage = true;
	timer->start(1000);

	return;
}

void WindowSecureCheck::changeImage()
{
	if (currentImage)
	{
		currentImage = false;
		imageLabel->setPixmap(pixmapCheckedImage);
	}
	else
	{
		currentImage = true;
		imageLabel->setPixmap(pixmapImage);
	}

	return;
}

void WindowSecureCheck::stopTimer()
{
	timer->stop();
	return;
}
