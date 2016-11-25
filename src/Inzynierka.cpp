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

    // ___________________________________________________________
    // QT onko prace nad
    // ___________________________________________________________


    QApplication a(argc, argv);
    a.setStyleSheet("QWidget {background-image: url(./ProgramImages/Background.jpg);"
    		"				  background-repeat: no-repeat;}"
    				"QPushButton {border: 3px solid #7f7f7f}"
    				// TODO zrobic jakis wyglad i zrobic plik dla tego wygladu qss
    				// i go tutaj podczepic


    );

    WindowMain mainWindow;
    a.exec();

    // ___________________________________________________________
	return 0;
}
