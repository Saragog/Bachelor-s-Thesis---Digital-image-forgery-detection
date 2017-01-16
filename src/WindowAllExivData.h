/*
 * WindowAllExivData.h
 *
 *  Created on: 9 wrz 2016
 *      Author: andrzej
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

class WindowAllExivData : public QWidget
{
private:
	void clearInfo();

public:

	QWidget* mainWindow;
	QLabel* label;

	QPushButton* buttonReturn;

	QPalette scrollPalette;
	QScrollArea* exivInfoScroll;

	QLabel* infoLabel;
	QGridLayout infoLayout;
	QWidget* infoWidget;
	QFont textFont;

	WindowAllExivData(QWidget* main);
	virtual ~WindowAllExivData();

	//QPushButton

	void prepareWindow(std::vector<std::string> data);
};

#endif /* WINDOWALLEXIVDATA_H_ */
