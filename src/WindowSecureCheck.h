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

class WindowSecureCheck : public QWidget
{
public:

	QWidget* mainWindow;
	QLabel* label;

	QLabel* exifProtectionLabel;
	QLabel* exifSoftwareLabel;
	QLabel* exifXDimensionLabel;
	QLabel* exifYDimensionLabel;
	QLabel* exifDateTimeLabel;

	QLabel* histogramCLabel;
	QLabel* checkedImageLabel;

	QPushButton* backButton;
	QPalette palette;
	QString fileName;

	WindowSecureCheck(QWidget* main);
	virtual ~WindowSecureCheck();
	void setFile(const QString name);
	void setExifRaport(std::vector<std::pair<bool, std::string> > raport);
	void setHistogramC(QImage histogram);
	void setCheckedImage(QImage image);
};

#endif /* WINDOWSECURECHECK_H_ */
