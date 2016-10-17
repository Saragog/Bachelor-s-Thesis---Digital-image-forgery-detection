/*
 * ExivOperations.cpp
 *
 *  Created on: 27 sie 2016
 *      Author: andrzej
 */

#include "ExivOperations.h"
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

ExivOperations::ExivOperations(WindowMain* con)
{
	controller = con;
	// TODO Auto-generated constructor stub

}

ExivOperations::~ExivOperations() {
	// TODO Auto-generated destructor stub
}

bool ExivOperations::readFromFile(std::string path)
{
	allExifData.clear();
	basicExifData.clear();

	image = Exiv2::ImageFactory::open(path);
	image->readMetadata();

    exifData = image->exifData();

    Exiv2::ExifData::const_iterator end = exifData.end();

    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i)
    {
        const char* tn = i->typeName();

        if (i->key() == "Exif.Image.Make" ||
        	i->key() == "Exif.Image.Model" ||
        	i->key() == "Exif.Image.Software" ||
        	i->key() == "Exif.Image.ProcessingSoftware" ||
        	i->key() == "Exif.Image.DateTime" ||
        	i->key() == "Exif.Photo.DateTimeOriginal" ||
        	i->key() == "Exif.Photo.DateTimeDigitized" ||
        	i->key() == "Exif.Photo.ExifVersion" ||
        	i->key() == "Exif.Photo.PixelXDimension" ||
        	i->key() == "Exif.Photo.PixelYDimension" ||
        	i->key() == "Exif.Photo.UserComment"
        	)
        {
            basicExifData.push_back(i->key() + " " + (tn ? tn : "Unknown") + " " + i->value().toString());
            allExifData.push_back(i->key() + " " + (tn ? tn : "Unknown") + " " + i->value().toString());
        }
        else
        {
            allExifData.push_back(i->key() + " " + (tn ? tn : "Unknown") + " " + i->value().toString());
        }
    }
    if (exifData.empty()) return false;
    return true;
}

vector<string> ExivOperations::getBasicExifData() const
{
	return basicExifData;
}

vector<string> ExivOperations::getAllExifData() const
{
	return allExifData;
}

std::string ExivOperations::convertIntToStr(int number)
{
	std::string result = "";

	while (number > 0)
	{
		result = (char)((number % 10) + '0') + result;
		number /= 10;
	}

	return result;
}

int ExivOperations::convertStrToInt(std::string word)
{
	int number = 0;
	char sign;

	for (unsigned int index = 0; index < word.size(); index++)
	{
		sign = word.at(index);
		number = 10 * number + (sign - '0');
	}
	return number;
}

/*
bool ExivOperations::hasExifTags(std::string path)
{
	image = Exiv2::ImageFactory::open(path);
	image->readMetadata();

	exifData = image->exifData();

	if (exifData.empty()) return false;

	return true;
}
*/

bool ExivOperations::isImageFile(std::string path)
{
    std::string p = path;

    if (p.substr(p.find_last_of(".") + 1) == "jpg" || p.substr(p.find_last_of(".") + 1) == "JPG")
    {
    	fstream file;
    	file.open(path.c_str());

    	if (file.good())
    	{
    		file.close();
    		return true;
    	}
    	else
    	{
    		return false;
    	}
    }
    else
    {
    	return false;
    }
}

void ExivOperations::saveExifIntoFile(std::string path)
{
	prepareSecurityExifData();

	Exiv2::Image::AutoPtr savedImage = Exiv2::ImageFactory::open(path);
	savedImage->setExifData(exifData);
	savedImage->writeMetadata();
	return;
}

void ExivOperations::prepareSecurityExifData()
{
    Exiv2::Value::AutoPtr v = Exiv2::Value::create(Exiv2::unsignedLong);

    std::string temp;
    std::string dateTime = "";
    std::string securityInfoToSave = "SECURED ";
    securityInfoToSave += exifData["Exif.Photo.PixelXDimension"].value().toString();
    securityInfoToSave += " ";
    securityInfoToSave += exifData["Exif.Photo.PixelYDimension"].value().toString();

    v->read(securityInfoToSave);

    Exiv2::ExifKey key("Exif.Photo.UserComment");
	time_t t = time(0);
	struct tm * now = localtime( & t );

	dateTime = dateTime + convertIntToStr(now->tm_year + 1900) + ":";
	temp = convertIntToStr(now->tm_mon + 1);
	if (temp.size() == 1) dateTime += "0";
	dateTime = dateTime + temp + ":";
	temp = convertIntToStr(now->tm_mday);
	if (temp.size() == 1) dateTime += "0";
	dateTime = dateTime + temp + " ";
	temp = convertIntToStr(now->tm_hour);
	if (temp.size() == 1) dateTime += "0";
	else if (temp.size() == 0) dateTime += "00";
	dateTime = dateTime + temp + ":";
	temp = convertIntToStr(now->tm_min);
	if (temp.size() == 1) dateTime += "0";
	else if (temp.size() == 0) dateTime += "00";
	dateTime = dateTime + temp + ":";
	temp = convertIntToStr(now->tm_sec);
	if (temp.size() == 1) dateTime += "0";
	else if (temp.size() == 0) dateTime += "00";
	dateTime += temp;

	securityInfoToSave += " ";
	securityInfoToSave += dateTime;

    std::vector<int> cornerPixelsRGBs = controller->getCorners();

    for (int step = 0; step < 12; step++)
    {
    	securityInfoToSave += " ";
    	securityInfoToSave += convertIntToStr(cornerPixelsRGBs.at(step));
    }


    // TODO sprawdzic czy to dziala zrobic wczytywanie tego histogramu i sprawdzanie czy jest ok
    // zobaczyc jak to wyglada w komentazu exifowym
    // Testowac dzialanie zabezpieczenia !!!
    // Jesli dziala to mozna zajac sie sprawdzaniu przy rotacjach
    // Po rotacjach bedzie mozna pomyslec o rozszerzaniu i zwezaniu na podstawie kilku algorytmow
    // Zrobic zabezpieczenie tego pola z informacja / pomyslec nad ukrywaniem tego moze cos
    // no i chyba jesli to bedzie dzialac to mozna prawie uznac za zakonczone chyba :D ( testy testy )

    int* histogramGrey = controller->getHistGTones();

    for (int tone = 0; tone < 153; tone++)
    {
        securityInfoToSave += " ";
        securityInfoToSave += convertIntToStr(histogramGrey[tone]);
    }

	exifData["Exif.Image.Software"] = "JPG Analizer";
	exifData["Exif.Photo.UserComment"] = securityInfoToSave;
	exifData["Exif.Image.DateTime"] = dateTime;
	return;
}

std::string ExivOperations::readSecurityExifWord()
{
	std::string word = "";
	char sign;

	while (curIndexSTag < securityTagContent.size())
	{
		sign = securityTagContent.at(curIndexSTag);
		if (sign == ' ')break;
		word += sign;
		curIndexSTag++;
	}
	return word;
}

bool ExivOperations::checkSoftware()
{
	std::string programName;

	try
	{
		programName = exifData["Exif.Image.Software"].value().toString();
	}
	catch (std::exception &e)
	{
		// Nie wykryto pola nazwy wykorzystanego programu !!!
		partialRaport.first = false;
		partialRaport.second = "Nie wykryto pola ostatnio wykorzystanego programu";

		raportExif.push_back(partialRaport);
		return false;
	}

	std::cout << "Program ma nazwe: " << programName << std::endl;

	if (programName == "JPG Analizer")
	{
		partialRaport.first = true;
		partialRaport.second = "Nazwa programu zgodna";
		raportExif.push_back(partialRaport);
		cout << "\nNazwa programu zgadza sie :D \n";
		return true;
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = "Nazwa programu wykorzystanego nie jest zgodna";
		raportExif.push_back(partialRaport);
		cout << "\nNazwa programu sie nie zgadza ... \n";
	}
	return false;
}

bool ExivOperations::checkIfProtected(std::string word)
{
	//std::pair<bool, std::string> partialRaport;
	if (word == "SECURED")
	{
		partialRaport.first = true;
		partialRaport.second = "Wykryto zabezpieczenie";
		raportExif.push_back(partialRaport);
		return true;
	}
	//cout << "\nNie jest secured ... najwyrazniej \n";
	return false;
}

bool ExivOperations::checkPixelXDimension(int xdim)
{
	int exifXDim;

	try
	{
		exifXDim = exifData["Exif.Photo.PixelXDimension"].value().toLong();
	}
	catch(exception &e)
	{
		// nie wykryto pola PixelXDimension
		partialRaport.first = false;
		partialRaport.second = "Nie wykryto pola PixelXDimension";
		raportExif.push_back(partialRaport);
		return false;
	}

	if (exifXDim == xdim)
	{
		partialRaport.first = true;
		partialRaport.second = "XDimension sie zgadza";
		raportExif.push_back(partialRaport);
		cout << "\nXDimension wa okeei :D \n";
		return true;
	}
	else
	{
		partialRaport.first = true;
		partialRaport.second = "XDimension nie jest zgodne";
		raportExif.push_back(partialRaport);
		cout << "\nXDimension sie nie zgadza ... \n";
	}
	return false;
}

bool ExivOperations::checkPixelYDimension(int ydim)
{
	int exifYDim;

	try
	{
		exifYDim = exifData["Exif.Photo.PixelYDimension"].value().toLong();
	}
	catch(exception &e)
	{
		// nie wykryto pola PixelYDimension
		partialRaport.first = false;
		partialRaport.second = "Nie wykryto pola PixelYDimension";
		raportExif.push_back(partialRaport);
		return false;
	}

	if (exifYDim == ydim)
	{
		partialRaport.first = true;
		partialRaport.second = "YDimension sie zgadza";
		raportExif.push_back(partialRaport);
		cout << "\nYDimension wa okeei :D \n";
		return true;
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = "YDimension nie jest zgodne";
		raportExif.push_back(partialRaport);
		cout << "\nYDimension sie nie zgadza ... \n";
	}
	return false;
}

bool ExivOperations::checkDateTime(std::string word)
{
	std::string date;

	try
	{
		date = exifData["Exif.Image.DateTime"].value().toString();
	}
	catch(exception &e)
	{
		partialRaport.first = false;
		partialRaport.second = "Nie wykryto pola DateTime";
		raportExif.push_back(partialRaport);
		return false;
	}

	if (date == word)
	{
		partialRaport.first = true;
		partialRaport.second = "Data zmiany sie zgadza";
		raportExif.push_back(partialRaport);
		cout << "\nData zmiany sie zgaaadza :D \n";
		return true;
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = "Data zmiany sie nie zgadza";
		raportExif.push_back(partialRaport);
		cout << "\nData zmiany ostatniej sie nie zgadza ... \n";
	}
	return false;
}

bool ExivOperations::checkExifSecurity()
{
	std::string word;
	curIndexSTag = 0;
	int temp;

	raportExif.clear();

	try
	{
		securityTagContent = exifData["Exif.Photo.UserComment"].value().toString();
	}
	catch (std::exception& e)
	{
		cout << "\n\nZLAPALEEEM !!!\n\n";
		// Nie znaleziono pola z komentarzem !!!
		partialRaport.first = false;
		partialRaport.second = "Nie wykryto pola zabezpieczajacego w wybranym obrazie";

		raportExif.push_back(partialRaport);
		return false;
	}

	word = readSecurityExifWord();
	curIndexSTag += 1;

	while (word != "" && !checkIfProtected(word))
	{
		word = readSecurityExifWord();
		curIndexSTag += 1;
	}

	if (word == "")
	{
		// TODO
		// Nie wykryto zabezpieczenia exif !!!

		partialRaport.first = false;
		partialRaport.second = "Nie wykryto zabezpieczen";

		raportExif.push_back(partialRaport);
		return false;
	}
	else
	{
		checkSoftware();
		word = readSecurityExifWord();
		curIndexSTag += 1;
		temp = convertStrToInt(word);
		checkPixelXDimension(temp);
		word = readSecurityExifWord();
		curIndexSTag += 1;
		temp = convertStrToInt(word);
		checkPixelYDimension(temp);
		word = readSecurityExifWord();
		word = word + ' ';
		curIndexSTag += 1;
		word = word + readSecurityExifWord();
		curIndexSTag += 1;
		checkDateTime(word);
	}

	return true;
}

std::vector<int> ExivOperations::getSavedCornerRGBs()
{
	std::vector<int> rgbs;
	for (int step = 0; step < 12; step++)
	{
		rgbs.push_back(convertStrToInt(readSecurityExifWord()));
		curIndexSTag += 1;
	}
	return rgbs;
}

std::vector<int> ExivOperations::getSavedGreyTones()
{
	std::vector<int> savedGreyTones;
	for (int x = 0; x < 153; x++)
	{
		savedGreyTones.push_back(convertStrToInt(readSecurityExifWord()));
		curIndexSTag += 1;
	}
	return savedGreyTones;
}

std::vector<std::pair<bool, std::string> > ExivOperations::getRaportExif() const
{
	return raportExif;
}
