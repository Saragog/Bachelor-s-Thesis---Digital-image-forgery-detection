//============================================================================
// Name        : Inzynierka.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Praca Inzynierska
//				 Temat: Wykrywanie Falszerstw w obrazach cyfrowych
//============================================================================

#include "exiv2.hpp"
#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include "WindowMain.h"

using namespace std;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    //QFile file("stylesheet.qss");
    //file.open(QFile::ReadOnly);
    //QString styleSheet = QLatin1String(file.readAll());
    //a.setStyleSheet(styleSheet);

    // Uklad wygladu dla calej aplikacji

    a.setStyleSheet(
    		"QMainWindow"
    		"		{"
    		"			background-image: url(./ProgramImages/Background.jpg);"
    		"			background-repeat: no-repeat;"
    		"		}"
    		"QLabel"
    		"		{"
    		//"			background: transparent;"
    		"			background-color: rgb(230, 230, 230);"
    		"			padding: 2px;"
    		"			border: 3px solid #000000;"
    		//"			font-size: 40;"
			"		}"
    		"QScrollArea"
    		"		{"
    		"			background-color: rgba(100, 100, 100, 0);"
    		"		}"
    		"QPushButton"
    		"		{"
    		"			background-color: rgb(230, 230, 230);"
    		"			border: 3px solid #3f3f3f;"
    		"			"
    		"		}"
    		"QLineEdit"
    		"		{"
    		"			border: 3px solid #888888;"
    		"		}"
    		/*
    		"QLineEdit"
    		"		{"
    		"			border: 3px solid #7f7f7f;"
    		"		}"
    		"QScrollArea"
    		"		{"
    		//"			border: 2px solid #666666;"
    		//"			background: transparent;"
    		"		}"*/
    );

    WindowMain mainWindow;
    a.exec();

    // ___________________________________________________________
	return 0;
}
