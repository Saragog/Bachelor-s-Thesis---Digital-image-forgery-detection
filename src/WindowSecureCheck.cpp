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

	exifProtectionTagLabel = new QLabel(this);
	exifProtectionTagLabel->setGeometry(620, 480, 850, 40);

	exifIsProtectedLabel = new QLabel(this);
	exifIsProtectedLabel->setGeometry(620, 520, 850, 40);

	exifSoftwareLabel = new QLabel(this);
	exifSoftwareLabel->setGeometry(620, 560, 850, 40);

	exifXDimensionLabel = new QLabel(this);
	exifXDimensionLabel->setGeometry(620, 600, 850, 40);

	exifYDimensionLabel = new QLabel(this);
	exifYDimensionLabel->setGeometry(620, 640, 850, 40);

	exifDateTimeLabel = new QLabel(this);
	exifDateTimeLabel->setGeometry(620, 680, 850, 40);

	imageRotationLabel = new QLabel(this);
	imageRotationLabel->setGeometry(620, 720, 850, 40);

	imageHistogramSourceLabel = new QLabel(this);
	imageHistogramSourceLabel->setGeometry(620, 760, 850, 40);

	imageWrgPxlCntrLabel = new QLabel(this);
	imageWrgPxlCntrLabel->setGeometry(620, 800, 850, 40);

	histogramCLabel = new QLabel(this);
	histogramCLabel->setGeometry(620, 10, 650, 450);

	imageLabel = new QLabel(this);
	imageLabel->setGeometry(10, 10, 600, 450);

	backButton = new QPushButton("Powrot", this);
	backButton->setGeometry(100, 500, 100, 100);

	timer = new QTimer(this);

	palette.setColor(QPalette::Background, Qt::cyan);
	this->setAutoFillBackground(true);
	this->setPalette(palette);

	currentImage = true;

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

void WindowSecureCheck::setRaport(std::vector<std::pair<bool, std::string> > raport)
{
	exifProtectionTagLabel->setText(QString::fromStdString(raport.at(0).second));
	exifIsProtectedLabel->setText(QString::fromStdString(raport.at(1).second));
	exifSoftwareLabel->setText(QString::fromStdString(raport.at(2).second));
	exifXDimensionLabel->setText(QString::fromStdString(raport.at(3).second));
	exifYDimensionLabel->setText(QString::fromStdString(raport.at(4).second));
	exifDateTimeLabel->setText(QString::fromStdString(raport.at(5).second));

	imageRotationLabel->setText(QString::fromStdString(raport.at(6).second));
	imageHistogramSourceLabel->setText(QString::fromStdString(raport.at(7).second));
	imageWrgPxlCntrLabel->setText(QString::fromStdString(raport.at(8).second));

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
