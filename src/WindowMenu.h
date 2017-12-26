/*
 * MainWindow.h
 *
 *  Created on: 17 sie 2016
 *      Author: Andrzej Dackiewicz
 *  Komentarz: Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Opisuje klasę WindowMenu
*/

#ifndef WINDOWMENU_H_
#define WINDOWMENU_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QLineEdit>
#include <QSignalMapper>
#include <QImageReader>
#include <QPixmap>
#include <QImage>
#include <QMessageBox>
#include <QFont>
#include <QTextCodec>

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
	QSignalMapper* mapperExivInfo;		// signal mapper pozwala na obsługę zdarzenia z parametrami
	QSignalMapper* mapperSecureImage;
	QSignalMapper* mapperSecureCheck;
	QSignalMapper* mapperTextEdited;
	QImage defaultImage;				// domyślny obraz pokazywany, jeśli adres pod wskazaną ścieżką nie wskazuje na plik obrazu
	QLabel* imageLabel;
	QFont textFont;
	QTextCodec* codec;
	void setDefaultImage();

public:
	WindowMenu(WindowMain* main); // konstruktor
	virtual ~WindowMenu(); 		  // destruktor

public slots:					  // funkcje wywoływane przy zdarzeniach
	void chooseFile();
	void setMapperExivInfo();
	void setMapperSecureImage();
	void setMapperSecureCheck();

	void setMapperTextEdited();

private slots:
	void validateImage(QString path); // aktualizacja obrazu

};

#endif /* WINDOWMENU_H_ */
