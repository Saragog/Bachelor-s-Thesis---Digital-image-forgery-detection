/*
 * WindowSecureCheck.cpp
 *
 *  Created on: 24 sie 2016
 *      Author: andrzej
 */

#include "WindowSecureCheck.h"
#include <QImageReader>

WindowSecureCheck::WindowSecureCheck(QWidget* main)
{
	this->mainWindow = main;
	codec = QTextCodec::codecForName("UTF-8");
    this->setWindowTitle("JPGAnalizer - Secure Check");

    mapperAccDif = new QSignalMapper(this);

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

	adjustButton = new QPushButton(codec->toUnicode("Sprawdź dla danej\ndokładności"), this);
	adjustButton->setGeometry(20, 690, 250, 80);
	adjustButton->setFont(textFont);

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

    connect(adjustButton, SIGNAL(clicked()), this, SLOT(setMapperAccDif()));

    connect(adjustButton, SIGNAL(clicked()), mapperAccDif, SLOT(map()));

    mapperAccDif->setMapping(adjustButton, spinBox->value());

    connect(mapperAccDif, SIGNAL(mapped(int)), mainWindow, SLOT(adjustToNewAccDif(int)));

	connect(backButton, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
	connect(changeImagesButton, SIGNAL(clicked()), this, SLOT(changeImage()));
}

WindowSecureCheck::~WindowSecureCheck()
{
}

void WindowSecureCheck::setImage(QString path)
{
	pixmapImage = QPixmap(path);
	pixmapImage = pixmapImage.scaled(600, 450);
	spinBox->setValue(3);
	//changeImagesButton->setText("Pokaż wynikowy obraz");

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

		QLabel* n = new QLabel(this);

		if (raport.at(step).first) n->setPixmap(positiveResult);
		else n->setPixmap(negativeResult);

		//n->setGeometry(300, 470 + 50*step, 50, 50);
		raportLayout.addWidget(n, step, 0);

		raportLabel = new QLabel(QString::fromStdString(raport.at(step).second));
		//raportLabel->setGeometry(370, 470 + 50*step, 700, 50);

		raportLayout.addWidget(raportLabel, step, 1, 1, 13);
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
	changeImagesButton->setText(codec->toUnicode("Pokaż wynikowy obraz"));
	currentImage = true;

	return;
}

void WindowSecureCheck::setMapperAccDif()
{
	mapperAccDif->setMapping(adjustButton, spinBox->value());
	return;
}

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
