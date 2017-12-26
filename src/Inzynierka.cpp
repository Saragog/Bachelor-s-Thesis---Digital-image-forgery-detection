//============================================================================
// Name        : Inzynierka.cpp
// Author      : Andrzej Dackiewicz
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Praca Inzynierska
//				 Temat: Wykrywanie Falszerstw w obrazach cyfrowych
//============================================================================

#include "exiv2.hpp"
#include <iostream>
#include <QApplication>
//#include <QMainWindow>
#include "WindowMain.h"

using namespace std;

// Tutaj rozpoczyna się działanie aplikacji
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Uklad wygladu dla calej aplikacji
    a.setStyleSheet(
    		"QMainWindow"
    		"		{"
    		"			background-image: url(./ProgramImages/Background.jpg);"
    		"			background-repeat: no-repeat;"
    		"		}"
    		"QLabel"
    		"		{"
    		"			background-color: rgb(210, 210, 210);"
    		"			border: 3px solid #000000;"
			"		}"
    		"QScrollArea"
    		"		{"
    		"			background-color: rgb(100, 100, 100, 0);"
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
    );
    WindowMain mainWindow;
    a.exec(); // uruchomienie aplikacji
	return 0;
}
