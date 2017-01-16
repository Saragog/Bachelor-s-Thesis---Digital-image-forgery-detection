/*
 * WindowSecureImage.h
 *
 *  Created on: 24 sie 2016
 *      Author: andrzej
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
#include <QTimer>
#include <QFont>
#include <QSpinBox>
#include <QSignalMapper>

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

	QTimer* timer;

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

	QSignalMapper* mapperAccDif;

	void clearRaport();

public:

	WindowSecureCheck(QWidget* main);
	virtual ~WindowSecureCheck();
	void setImage(QString path);
	void setRaport(std::vector<std::pair<bool, std::string> > raport);
	void setHistogramC(QImage histogram);
	void setCheckedImage(QImage image);

public slots:
	void setMapperAccDif();

private slots:
	void changeImage();
	void stopTimer();
};

#endif /* WINDOWSECURECHECK_H_ */
