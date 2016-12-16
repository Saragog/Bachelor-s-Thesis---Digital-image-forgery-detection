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
#include <QFont>
#include "WindowExivData.h"
#include "WindowSecureCheck.h"
#include "WindowSecureImage.h"
#include "WindowAllExivData.h"

#include "ExivOperations.h"
#include "DrawingOperations.h"

#include <QFileDialog>
#include <QStringList>
#include <QMessageBox>

class ExivOperations;

class WindowMain : public QMainWindow
{
	Q_OBJECT

private:
	QWidget* menu;
	WindowExivData* exivData;
	WindowSecureImage* secureImage;
	WindowSecureCheck* secureCheck;
	WindowAllExivData* allExivData;

	QStackedWidget *stackedWidget;
	QVBoxLayout *layout;
	QComboBox *pageComboBox;
	QLabel image;
	QFont textFont;

	QMessageBox message;

	// TODO Potem zrobic jakas klase ktora przechowuje to by byl podzial a nie nie wiem co ... |
	// 																						   V

	ExivOperations* exivOperations;
	DrawingOperations* drawingOperations;

public:
	WindowMain();
	virtual ~WindowMain();

	bool checkImageFile(std::string path);
	int* getHistGTones() const;
	QFont* getTextFont();

public slots:
	void showMenu();
	void showExivData(QString fileName);
	void showSecureImage(QString fileName);
	void showSecureCheck(QString fileName);
	void showAllExivData();
	void showExivDataAgain();

	void saveImage(QString path); // potem zrobic by przekazywany byl obraz
};
#endif /* WINDOWMAIN_H_ */
