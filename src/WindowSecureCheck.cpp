/*
 * WindowSecureCheck.cpp
 *
 *  Created on: 24 sie 2016
 *      Author: andrzej
 */

#include "WindowSecureCheck.h"

WindowSecureCheck::WindowSecureCheck(QWidget* main)
{
	// TODO Auto-generated constructor stub

	this->mainWindow = main;

	label = new QLabel("Window Secure Check", this);
	label->setGeometry(100, 100, 200, 100);

	exifProtectionLabel = new QLabel(this);
	exifProtectionLabel->setGeometry(620, 480, 600, 50);

	exifSoftwareLabel = new QLabel(this);
	exifSoftwareLabel->setGeometry(620, 530, 600, 50);

	exifXDimensionLabel = new QLabel(this);
	exifXDimensionLabel->setGeometry(620, 580, 600, 50);

	exifYDimensionLabel = new QLabel(this);
	exifYDimensionLabel->setGeometry(620, 630, 600, 50);

	exifDateTimeLabel = new QLabel(this);
	exifDateTimeLabel->setGeometry(620, 680, 600, 50);

	histogramCLabel = new QLabel(this);
	histogramCLabel->setGeometry(620, 10, 600, 450);

	checkedImageLabel = new QLabel(this);
	checkedImageLabel->setGeometry(10, 10, 600, 450);

	backButton = new QPushButton("Powrot", this);
	backButton->setGeometry(100, 500, 100, 100);

	palette.setColor(QPalette::Background, Qt::cyan);
	this->setAutoFillBackground(true);
	this->setPalette(palette);

	connect(backButton, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
}

WindowSecureCheck::~WindowSecureCheck()
{
	// TODO Auto-generated destructor stub
}

void WindowSecureCheck::setFile(QString name)
{
	fileName = name;

	return;
}

void WindowSecureCheck::setExifRaport(std::vector<std::pair<bool, std::string> > raport)
{
	// TODO tutaaaj !!! :D

	exifProtectionLabel->setText(QString::fromStdString(raport.at(0).second));
	if (raport.size() > 1)
	{
		exifSoftwareLabel->setText(QString::fromStdString(raport.at(1).second));
		exifXDimensionLabel->setText(QString::fromStdString(raport.at(2).second));
		exifYDimensionLabel->setText(QString::fromStdString(raport.at(3).second));
		exifDateTimeLabel->setText(QString::fromStdString(raport.at(4).second));
	}

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
	QPixmap pixmap;
	pixmap = pixmap.fromImage(image);
	pixmap = pixmap.scaled(600, 450);
	checkedImageLabel->setPixmap(pixmap);
	return;
}
