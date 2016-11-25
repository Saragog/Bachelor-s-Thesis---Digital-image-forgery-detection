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
#include <QImage>
#include <QTimer>

class WindowSecureCheck : public QWidget
{
	Q_OBJECT

private:
	QWidget* mainWindow;
	QLabel* label;

	QLabel* exifProtectionTagLabel;
	QLabel* exifIsProtectedLabel;
	QLabel* exifSoftwareLabel;
	QLabel* exifXDimensionLabel;
	QLabel* exifYDimensionLabel;
	QLabel* exifDateTimeLabel;

	QLabel* imageRotationLabel;
	QLabel* imageHistogramSourceLabel;
	QLabel* imageWrgPxlCntrLabel;

	QPixmap pixmapImage;
	QPixmap pixmapCheckedImage;
	QLabel* histogramCLabel;
	QLabel* imageLabel;

	QTimer* timer;

	QPushButton* backButton;
	QPalette palette;
	std::string fileName;
	bool currentImage; 	// mowi ktory obraz aktualnie jest pokazywany
						// potrzebne do przestawiania obrazkow za pomoca timera
						// true oznacza ze pokazywany jest zwykly obraz
						// false oznacza ze pokazywany jest obraz wynikajacy ze sprawdzenia obrazu
public:

	WindowSecureCheck(QWidget* main);
	virtual ~WindowSecureCheck();
	void setImage(QString path);
	void setRaport(std::vector<std::pair<bool, std::string> > raport);
	void setHistogramC(QImage histogram);
	void setCheckedImage(QImage image);

private slots:
	void changeImage();
	void stopTimer();
};

#endif /* WINDOWSECURECHECK_H_ */
