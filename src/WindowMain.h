/*
 * WindowMain.h
 *
 *  Created on: 17 sie 2016
 *      Author: andrzej
 */

#ifndef WINDOWMAIN_H_
#define WINDOWMAIN_H_

#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QStackedWidget>
#include <QBoxLayout>
#include <QComboBox>
#include "WindowExivData.h"
#include "WindowSecureCheck.h"
#include "WindowSecureImage.h"

#include "ExivOperations.h"

#include <QFileDialog>
#include <QStringList>
#include <QMessageBox>

class WindowMain : QMainWindow
{
	Q_OBJECT

private:
	QWidget* menu;
	WindowExivData* exivData;
	WindowSecureImage* secureImage;
	WindowSecureCheck* secureCheck;

	QStackedWidget *stackedWidget;
	QVBoxLayout *layout;
	QComboBox *pageComboBox;
	QMessageBox message;

	// TODO Potem zrobic jakas klase ktora przechowuje to by byl podzial a nie nie wiem co ... |
	// 																						   V
	ExivOperations* exivOperations;


	bool checkImageFile(char* fileName);

public:
	WindowMain();
	virtual ~WindowMain();

public slots:
	void showMenu();
	void showExivData(QString fileName);
	void showSecureImage(QString fileName);
	void showSecureCheck(QString fileName);
};
#endif /* WINDOWMAIN_H_ */
