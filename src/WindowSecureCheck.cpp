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
