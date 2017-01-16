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
#include <QImageReader>
#include <QPixmap>
#include <QImage>
#include <QMessageBox>
#include <QFont>

#include "WindowMain.h"

class WindowMenu : public QWidget
{
	Q_OBJECT

private:

	WindowMain* mainWindow;
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
	QSignalMapper* mapperTextEdited;
	QImage defaultImage;
	QLabel* imageLabel;
	QFont textFont;
	void setDefaultImage();

public:
	WindowMenu(WindowMain* main);
	virtual ~WindowMenu();

public slots:
	void chooseFile();
	void setMapperExivInfo();
	void setMapperSecureImage();
	void setMapperSecureCheck();

	void setMapperTextEdited();

private slots:
	void validateImage(QString path);

};

#endif /* WINDOWMENU_H_ */
