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
public:

	QWidget* mainWindow;
	QLabel* label;
	QPushButton* button;
	QPalette palette;
	QString fileName;
	QScrollArea* exivInfoScroll;

	QGridLayout* infoLayout;
	QWidget* infoWidget;

	QLabel* infoLabel;
	QLabel* infoLabel2;

	WindowExivData(QWidget* main);
	virtual ~WindowExivData();

	void prepareWindow(std::vector<std::string> data);
};

#endif /* WindowExivData_H_ */
