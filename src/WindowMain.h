/*
 * WindowMain.h
 *
 *  Created on: 17 sie 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Comment:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Ten plik opisuje klasę WindowMain, która odpowiada za komunikację między obiektami programu.
 *  		Może być traktowana za funkcję kontrolującą.
 *  		Obsługuje większość zdarzeń występujących w trakcie działania programu.
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
	WindowMain(); // konstruktor
	virtual ~WindowMain(); // destruktor

	bool checkImageFile(std::string path); // sprawdzenie czy plik o podanej ścieżce jest plikiem obrazu
	int* getHistGTones() const;			   // zwraca liczebność 153 grup sum RGB
	std::pair<int, int> getActualSize() const; // zwraca wielkość obrazu

public slots:
	// funkcje obsługujące niektóre zdarzenia zachodzące w trakcie korzystania z aplikacji
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
