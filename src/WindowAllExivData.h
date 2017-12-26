/*
 * WindowAllExivData.h
 *
 *  Created on: 9 wrz 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Komentarz:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Ten plik opisuje klasę WindowAllExivData. Pokazuje okno przedstawiające zawartość wszystkich pól EXIF znalezionych w obrazie
 */

#ifndef WINDOWALLEXIVDATA_H_
#define WINDOWALLEXIVDATA_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QScrollArea>
#include <QGridLayout>
#include <QFont>
#include <QTextCodec>

class WindowAllExivData : public QWidget
{
private:
	void clearInfo();

public:

	QWidget* mainWindow;
	QLabel* label;
	QPushButton* buttonReturn; // przycisk powrotu
	QPalette scrollPalette;
	QScrollArea* exivInfoScroll; // tutaj umieszczam zawartość EXIF
	QLabel* infoLabel;
	QGridLayout infoLayout;
	QWidget* infoWidget;
	QFont textFont;		// czcionka tekstowa
	QTextCodec* codec;  // umożliwienie polskich znaków

	WindowAllExivData(QWidget* main); // konstruktor
	virtual ~WindowAllExivData(); // destruktor
	void prepareWindow(std::vector<std::string> data); // Funkcja przygotowująca okno
};

#endif /* WINDOWALLEXIVDATA_H_ */
