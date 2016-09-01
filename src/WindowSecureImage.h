/*
 * WindowSecureImage.h
 *
 *  Created on: 26 sie 2016
 *      Author: andrzej
 */

#ifndef WindowSecureImage_H_
#define WindowSecureImage_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPalette>

class WindowSecureImage : public QWidget
{
public:

	QWidget* mainWindow;
	QLabel* label;
	QPushButton* backButton;
	QPalette palette;
	QString fileName;

	WindowSecureImage(QWidget* main);
	virtual ~WindowSecureImage();

	void setFile(QString name);
};

#endif /* WindowSecureImage_H_ */
