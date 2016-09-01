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
	Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open("./Zdjecia_Oryginalne/DSC03096.jpg");
	image->readMetadata();

    Exiv2::ExifData &exifData = image->exifData();

    if (exifData.empty())
    {
        std::string error("./Zdjecia_Oryginalne/DSC03096.jpg");
        error += ": No Exif data found in the file";
        throw Exiv2::Error(1, error);
    }
    Exiv2::ExifData::const_iterator end = exifData.end();

    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
        const char* tn = i->typeName();
        std::cout << std::setw(44) << std::setfill(' ') << std::left
                  << i->key() << " "
                  << "0x" << std::setw(4) << std::setfill('0') << std::right
                  << std::hex << i->tag() << " "
                  << std::setw(9) << std::setfill(' ') << std::left
                  << (tn ? tn : "Unknown") << " "
                  << std::dec << std::setw(3)
                  << std::setfill(' ') << std::right
                  << i->count() << "  "
                  << std::dec << i->value()
                  << "\n";
    }

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
