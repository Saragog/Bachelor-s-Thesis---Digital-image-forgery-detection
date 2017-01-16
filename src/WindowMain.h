/*
 * WindowMain.h
 *
 *  Created on: 17 sie 2016
 *      Author: Andrzej Dackiewicz
 *
 *  Comment:This file is a part of Digital forgery detection program that was
 *  		an engineering thesis of the author.
 *  		This file is a description of the main window of the program.
 *  		The WindowMain class can be considered the controller of this program.
 *  		It handles most of the events that take place during program run.
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
	QMessageBox message;
	ExivOperations* exivOperations;
	DrawingOperations* drawingOperations;

public:
	WindowMain();
	virtual ~WindowMain();

	bool checkImageFile(std::string path);
	int* getHistGTones() const;
	std::pair<int, int> getActualSize() const;

public slots:
	void showMenu();
	void showExivData(QString fileName);
	void showSecureImage(QString fileName);
	void showSecureCheck(QString fileName);
	void showAllExivData();
	void showExivDataAgain();

	void adjustToNewAccDif(int newAccDif);
	void saveImage(QString path);
};
#endif /* WINDOWMAIN_H_ */
