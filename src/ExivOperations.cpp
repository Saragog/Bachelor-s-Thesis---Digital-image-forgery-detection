/*
 * ExivOperations.cpp
 *
 *  Created on: 27 sie 2016
 *      Author: andrzej
 */

#include "ExivOperations.h"

using namespace std;

ExivOperations::ExivOperations()
{
	// TODO Auto-generated constructor stub

}

ExivOperations::~ExivOperations() {
	// TODO Auto-generated destructor stub
}

void ExivOperations::readFromFile(std::string path)
{

	image = Exiv2::ImageFactory::open(path);
	image->readMetadata();

    exifData = image->exifData();

    if (exifData.empty())
    {
        std::string error("Brak danych typu EXIF w wybranym pliku !!!");
        throw Exiv2::Error(1, error);
    }
    Exiv2::ExifData::const_iterator end = exifData.end();

    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i)
    {
        const char* tn = i->typeName();

        fileExif.push_back(i->key() + " " + (tn ? tn : "Unknown") + " " + i->value().toString());
    }
}

vector<string> ExivOperations::getExifData()
{
    cout << "\n\nString jaki udalo sie wygramolic to:\n";


	for (unsigned int x = 0; x < fileExif.size(); x++)
	{
	    cout << fileExif.at(x) << endl << endl;
	}

	return fileExif;
}
