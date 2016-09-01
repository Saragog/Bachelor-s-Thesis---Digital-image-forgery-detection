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

class WindowSecureCheck : public QWidget
{
public:

	QWidget* mainWindow;
	QLabel* label;
	QPushButton* backButton;
	QPalette palette;
	QString fileName;

	WindowSecureCheck(QWidget* main);
	virtual ~WindowSecureCheck();
	void setFile(const QString name);
};

#endif /* WINDOWSECURECHECK_H_ */
