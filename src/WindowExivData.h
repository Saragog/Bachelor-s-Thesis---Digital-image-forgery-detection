/*
 * WindowExivData.h
 *
 *  Created on: 17 sie 2016
 *      Author: andrzej
 */

#ifndef WindowExivData_H_
#define WindowExivData_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QScrollArea>
#include <QGridLayout>

class WindowExivData : public QWidget
{
private:
	void clearInfo();

public:

	QWidget* mainWindow;
	QLabel* label;

	QPushButton* buttonReturn;
	QPushButton* buttonAll;

	QPalette palette;
	QPalette scrollPalette;
	QString fileName;
	QScrollArea* exivInfoScroll;

	QLabel* infoLabel;
	QGridLayout infoLayout;
	QWidget* infoWidget;

	QLabel* imageLabel;
	QLabel* histogramGreyLabel;
	QLabel* histogramRGBLabel;

	WindowExivData(QWidget* main);
	virtual ~WindowExivData();

	void prepareWindow(std::vector<std::string> data);
	void setImage(QString path);
	void setHistogramGrey(QImage histogram);
	void setHistogramRGB(QImage histogram);

};

#endif /* WindowExivData_H_ */
