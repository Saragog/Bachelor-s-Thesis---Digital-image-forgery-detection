/*
 * WindowMain.cpp
 *
 *  Created on: 17 sie 2016
 *      Author: andrzej
 */

#include "WindowMain.h"
#include "WindowMenu.h"
#include <iostream>
#include <fstream>
#include <iostream>

WindowMain::WindowMain() {
	// TODO Auto-generated constructor stub

	menu = new WindowMenu(this);
	exivData = new WindowExivData(this);
	secureImage = new WindowSecureImage(this);
	secureCheck = new WindowSecureCheck(this);

	stackedWidget = new QStackedWidget();
	stackedWidget->addWidget(menu);
	stackedWidget->addWidget(exivData);
	stackedWidget->addWidget(secureImage);
	stackedWidget->addWidget(secureCheck);

	message.setGeometry(300, 300, 200, 200);
	message.setText("Nie ma takiego pliku !!!");
	message.setDefaultButton(QMessageBox::Ok);

	pageComboBox = new QComboBox();
	pageComboBox->addItem(tr("WidokMenu"));
	pageComboBox->addItem(tr("WidokExivData"));
	pageComboBox->addItem(tr("WidokSecureImage"));
	pageComboBox->addItem(tr("WidokSecureCheck"));

	connect(pageComboBox, SIGNAL(activated(int)), stackedWidget, SLOT(setCurrentIndex(int)));

	this->setCentralWidget(stackedWidget);

	this->setGeometry(400, 200, 1024, 768);

	showMenu();

	// TO DZIAAAALAAAA KURWAAAA :D :D :D :D :D :D : D: D: :D :D :D

	// ______________ UWAGA POTEM TO GDZIE INDZIEJ CZY COS | V | V | V |

	exivOperations = new ExivOperations();

	exivOperations->readFromFile("./Zdjecia_Oryginalne/DSC03096.jpg");
	exivOperations->getExifData();

	// _______________________________________________________________


	this->show();
}

WindowMain::~WindowMain() {
	// TODO Auto-generated destructor stub
	std::cout << "USUWAM MAINA !!!";
	delete menu;
	delete exivData;
}

bool WindowMain::checkImageFile(char* fileName)
{
	std::cout << "\n\nFileName = " << fileName << std::endl;

	std::fstream plik;
	plik.open(fileName);
    if( plik.good() == true )
    {
        plik.close();
        return true;
    }
    else
    {
    	message.exec();
    	return false;
    }
}

void WindowMain::showMenu()
{
	std::cout << "Odpalam Menu !!!";

	stackedWidget->setCurrentIndex(0);
}

void WindowMain::showExivData(QString fileName)
{
	std::vector<std::string> data;

	std::cout << "Odpalam ExivData !!!";

	if (checkImageFile(fileName.toLatin1().data()))
	{
		exivOperations->readFromFile("./Zdjecia_Oryginalne/DSC03096.jpg");
		data = exivOperations->getExifData();

		exivData->prepareWindow(data);
		stackedWidget->setCurrentIndex(1);
	}
}

void WindowMain::showSecureImage(QString fileName)
{
	std::cout << "Odpalam ExivResults !!!";

	if (checkImageFile(fileName.toLatin1().data()))
	{
		secureImage->setFile(fileName);

		stackedWidget->setCurrentIndex(2);
	}
}

void WindowMain::showSecureCheck(QString fileName)
{
	std::cout << "Odpalam Secure Check !!!";

	if (checkImageFile(fileName.toLatin1().data()))
	{
		secureCheck->setFile(fileName);

		stackedWidget->setCurrentIndex(3);
	}
}
