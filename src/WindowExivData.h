/*
 * WindowExivData.h
 *
 *  Created on: 17 sie 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Komentarz: Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Plik ten zawiera opis klasy WindowExivData
 */

#ifndef WindowExivData_H_
#define WindowExivData_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QScrollArea>
#include <QGridLayout>
#include <QFont>
#include <QTextCodec>

class WindowExivData : public QWidget
{
private:
	void clearInfo();

public:

	QWidget* mainWindow;
	QLabel* label;
	// przyciski powrotu i pokazania całej zawartości metadanych EXIF
	QPushButton* buttonReturn;
	QPushButton* buttonAll;

	QPalette palette;
	QPalette scrollPalette;
	std::string fileName;
	QScrollArea* exivInfoScroll;
	QLabel* infoLabel;
	QGridLayout infoLayout;
	QWidget* infoWidget;
	QLabel* imageLabel;	// badany obraz
	QLabel* histogramGreyLabel; // histogram sum barw RGB
	QLabel* histogramRGBLabel;	// histogram barw RGB
	QFont textFont;
	QTextCodec* codec;

	WindowExivData(QWidget* main);	// konstruktor klasy
	virtual ~WindowExivData(); 		// destruktor klasy

	void prepareWindow(std::vector<std::string> data);	// funkcja przygotowująca okno do wyświetlenia
	void setImage(QString path);						// funkcja ustalająca badany obraz
	void setHistogramGrey(QImage histogram);			// funkcja aktualizująca histogram sum barw RGB
	void setHistogramRGB(QImage histogram);				// funkcja aktualizująca histogram barw RGB

};

#endif /* WindowExivData_H_ */
