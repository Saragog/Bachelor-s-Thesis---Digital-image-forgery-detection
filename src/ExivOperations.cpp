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
}

ExivOperations::~ExivOperations()
{
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

	if (number == 0) return "0";

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
	bool isPixelXDimSet, isPixelYDimSet;
    Exiv2::Value::AutoPtr v = Exiv2::Value::create(Exiv2::unsignedLong);

    std::string temp;
    std::string dateTime = "";
    std::string securityInfoToSave = "SECURED ";

    isPixelXDimSet = true;
    isPixelYDimSet = true;

	try
	{
		securityInfoToSave += exifData["Exif.Photo.PixelXDimension"].value().toString();
		securityInfoToSave += " ";
	}
	catch (std::exception &e)
	{
		// Nie wykryto pola XDimension !!!
		securityInfoToSave += '0';
		securityInfoToSave += " ";
		isPixelXDimSet = false;
	}

	try
	{
		securityInfoToSave += exifData["Exif.Photo.PixelYDimension"].value().toString();
		securityInfoToSave += " ";
	}
	catch (std::exception &e)
	{
		// Nie wykryto pola YDimension !!!
		securityInfoToSave += '0';
		securityInfoToSave += " ";
		isPixelYDimSet = false;
	}

    v->read(securityInfoToSave);

    Exiv2::ExifKey key("Exif.Photo.UserComment");
	time_t t = time(0);
	struct tm * now = localtime( & t );

	dateTime += convertIntToStr(now->tm_year + 1900) + ":";
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

	securityInfoToSave += dateTime;

    int* histogramGrey = controller->getHistGTones();

    for (int tone = 0; tone < 153; tone++)
    {
    	std::cout << "Zapisuje ilosc pikseli nalezacych do tonu: " << histogramGrey[tone] << std::endl;
        securityInfoToSave += " ";
        securityInfoToSave += convertIntToStr(histogramGrey[tone]);
    }

	exifData["Exif.Image.Software"] = "JPG Analizer";

	if (!isPixelXDimSet) exifData["Exif.Photo.PixelXDimension"] = "0";
	if (!isPixelYDimSet) exifData["Exif.Photo.PixelYDimension"] = "0";

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
	if (word == "SECURED")
	{
		partialRaport.first = true;
		partialRaport.second = "Wykryto zabezpieczenie";
		raportExif.push_back(partialRaport);
		return true;
	}
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
		partialRaport.second = "Data ostatniej zmiany zgodna z data zapisu";
		raportExif.push_back(partialRaport);
		cout << "\nData zmiany sie zgaaadza :D \n";
		return true;
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = "Data ostatniej zmiany nie jest zgodna z data zapisu";
		raportExif.push_back(partialRaport);
		cout << "\nData ostatniej zmiany nie jest zgodna z data zapisu\n";
	}
	return false;
}

void ExivOperations::checkExifSecurity()
{
	std::string word;
	curIndexSTag = 0;
	int temp;

	raportExif.clear();

	checkSoftware();

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
		partialRaport.first = false;
		partialRaport.second = "Nie wiadomo jaka byla wartosc pola PixelXDimension przed zabezpieczeniem";
		raportExif.push_back(partialRaport);

		partialRaport.first = false;
		partialRaport.second = "Nie wiadomo jaka byla wartosc pola PixelYDimension przed zabezpieczeniem";
		raportExif.push_back(partialRaport);

		partialRaport.first = false;
		partialRaport.second = "Nie wiadomo jaki byl czas zabezpieczenia";
		raportExif.push_back(partialRaport);
	}
	else
	{
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
	return;
}

/*
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
*/

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
