/*
 * WindowSecureImage.h
 *
 *  Created on: 24 sie 2016
 *      Author: Andrzej Dackiewicz
 *
 *  Komentarz: Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Opisuje klasę WindowSecureCheck pokazującą wyniki działania programu.
 */

#ifndef WINDOWSECURECHECK_H_
#define WINDOWSECURECHECK_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QScrollArea>
#include <QGridLayout>
#include <QImage>
#include <QFont>
#include <QSpinBox>
#include <QSignalMapper>
#include <QTextCodec>

class WindowSecureCheck : public QWidget
{
	Q_OBJECT

private:
	QWidget* mainWindow;
	QLabel* label;

	QPixmap pixmapImage;
	QPixmap pixmapCheckedImage;
	QLabel* histogramCLabel;
	QLabel* imageLabel;

	QPixmap positiveResult, negativeResult;

	QPushButton* backButton;
	QPalette palette;
	bool currentImage; 	// mowi ktory obraz aktualnie jest pokazywany
						// potrzebne do przestawiania obrazkow za pomoca timera
						// true oznacza ze pokazywany jest zwykly obraz
						// false oznacza ze pokazywany jest obraz wynikajacy ze sprawdzenia obrazu

	QPalette raportPalette;
	QScrollArea* raportScroll;

	QLabel* accLabel;	// akceptowalna roznica
	QLabel* raportLabel;
	QGridLayout raportLayout;
	QWidget* raportWidget;
	QFont textFont;
	QSpinBox* spinBox;
	QPushButton* adjustButton;
	QPushButton* changeImagesButton;
	QSignalMapper* mapperAccDif;
	QTextCodec* codec;

	void clearRaport();	// czyszczenie raportu weryfikacji obrazu

public:

	WindowSecureCheck(QWidget* main);
	virtual ~WindowSecureCheck();
	void setImage(QString path);
	void setRaport(std::vector<std::pair<bool, QString> > raport);
	void setHistogramC(QImage histogram);
	void setCheckedImage(QImage image);

public slots:
	void setMapperAccDif();

private slots:
	void changeImage();
};

#endif /* WINDOWSECURECHECK_H_ */
