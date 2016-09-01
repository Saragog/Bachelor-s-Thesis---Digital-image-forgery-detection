/*
 * W.cpp
 *
 *  Created on: 26 sie 2016
 *      Author: andrzej
 */

#include "WindowSecureImage.h"

WindowSecureImage::WindowSecureImage(QWidget* main)
{
	// TODO Auto-generated constructor stub

	mainWindow = main;

	label = new QLabel("Secure Image", this);
	label->setGeometry(100, 100, 200, 100);

	backButton = new QPushButton("Powrot", this);
	backButton->setGeometry(100, 400, 100, 100);

	palette.setColor(QPalette::Background, Qt::cyan);
	this->setAutoFillBackground(true);
	this->setPalette(palette);

	connect(backButton, SIGNAL(clicked()), mainWindow, SLOT(showMenu()));
}

WindowSecureImage::~WindowSecureImage()
{
	// TODO Auto-generated destructor stub
}

void WindowSecureImage::setFile(QString name)
{
	fileName = name;

	return;
}
