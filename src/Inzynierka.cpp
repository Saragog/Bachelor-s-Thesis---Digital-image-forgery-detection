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

    WindowMain mainWindow;

    std::cout << "\n";

    a.exec();

    // ___________________________________________________________
	return 0;
}
