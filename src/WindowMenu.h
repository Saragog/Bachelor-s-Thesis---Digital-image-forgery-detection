/*
 * MainWindow.h
 *
 *  Created on: 17 sie 2016
 *      Author: andrzej
 */

#ifndef WINDOWMENU_H_
#define WINDOWMENU_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QLineEdit>
#include <QFileDialog>
#include <QSignalMapper>

class WindowMenu : public QWidget
{
	Q_OBJECT

public:

	QWidget* mainWindow;
	QLabel* label;
	QLineEdit* editableLine;
	QPushButton* buttonExivInfo;
	QPushButton* buttonSecureImage;
	QPushButton* buttonSecureCheck;
	QPushButton* buttonChooseImage;
	QPalette palette;
	QSignalMapper* mapperExivInfo;
	QSignalMapper* mapperSecureImage;
	QSignalMapper* mapperSecureCheck;

	WindowMenu(QWidget* main);
	virtual ~WindowMenu();

public slots:
	void chooseFile();
	void setMapperExivInfo();
	void setMapperSecureImage();
	void setMapperSecureCheck();
};

#endif /* WINDOWMENU_H_ */
