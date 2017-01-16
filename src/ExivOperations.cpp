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
#include <string>
#include <sstream>

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
    unsigned int actualX, actualY;
    std::string temp;
    std::string dateTime = "";
    std::string securityInfoToSave = "SECURED ";
    std::pair<int, int> actualSize;
    isPixelXDimSet = true;
    isPixelYDimSet = true;

    try
    {
    	// Pobieranie aktualnej wielkosci obrazu

        actualSize = controller->getActualSize();
        actualX = actualSize.first;
        actualY = actualSize.second;

        std::cout << "actualX: " << actualX << std::endl;
        std::cout << "actualY: " << actualY << std::endl;

    	exifData["Exif.Photo.PixelXDimension"] = actualX;
    	exifData["Exif.Photo.PixelYDimension"] = actualY;
    }
    catch (std::exception &e)
    {
    	// Blad w trakcie zapisywania aktualnej wielkosci obrazu
    }

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
    	//std::cout << "Zapisuje ilosc pikseli nalezacych do tonu: " << histogramGrey[tone] << std::endl;
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
		return true;
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = "Nazwa programu wykorzystanego nie jest zgodna: " + programName;
		raportExif.push_back(partialRaport);
	}
	return false;
}

bool ExivOperations::checkIfProtected(std::string word)
{
	if (word == "SECURED")
	{
		isProtected = true;
		partialRaport.first = true;
		partialRaport.second = "Wykryto poczatek zabezpieczenia exif";
		raportExif.push_back(partialRaport);
		return true;
	}
	return false;
}

bool ExivOperations::checkPixelXDimension(int xdim)
{
	int exifXDim;
	std::string s;
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
		partialRaport.second = "Zawartosc XDimension zgodna z zawartoscia komentarza: " + convertIntToStr(exifXDim);
		raportExif.push_back(partialRaport);
		return true;
	}
	else
	{
		partialRaport.first = false;
		s = "Zawartosc XDimension niezgodna: w komentarzu-" + convertIntToStr(xdim) + " w polu exif-" + convertIntToStr(exifXDim);
		partialRaport.second = s;
		raportExif.push_back(partialRaport);
	}
	return false;
}

bool ExivOperations::checkPixelYDimension(int ydim)
{
	int exifYDim;
	std::string s;

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
		partialRaport.second = "Zawartosc YDimension zgodna z zawartoscia komentarza: " + convertIntToStr(exifYDim);
		raportExif.push_back(partialRaport);
		return true;
	}
	else
	{
		partialRaport.first = false;
		s = "Zawartosc YDimension niezgodna: w komentarzu-" + convertIntToStr(ydim) + " w polu exif-" + convertIntToStr(exifYDim);
		partialRaport.second = s;
		raportExif.push_back(partialRaport);
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
		partialRaport.second = "Zawartosc pola exif zgodna z zawartoscia komentarza: " + date;
		raportExif.push_back(partialRaport);
		return true;
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = "Data ostatniej zmiany nie jest zgodna z data zapisu: w komentarzu-" + word + " w polu exif-" + date;
		raportExif.push_back(partialRaport);
	}
	return false;
}

void ExivOperations::checkExifSecurity()
{
	std::string word;
	curIndexSTag = 0;
	int temp;
	isProtected = false;

	raportExif.clear();

	checkSoftware();

	try
	{
		// Znaleziono pole komentarza zabezpieczajacego
		securityTagContent = exifData["Exif.Photo.UserComment"].value().toString();
		partialRaport.first = true;
		partialRaport.second = "Wykryto pole zabezpieczajace exif w wybranym obrazie";
		raportExif.push_back(partialRaport);
	}
	catch (std::exception& e)
	{
		// Nie znaleziono pola z komentarzem !!!
		partialRaport.first = false;
		partialRaport.second = "Nie wykryto pola zabezpieczajacego exif w wybranym obrazie";
		raportExif.push_back(partialRaport);

	}

	word = readSecurityExifWord();
	curIndexSTag += 1;

	while (word != "" && !checkIfProtected(word))
	{
		word = readSecurityExifWord();
		curIndexSTag += 1;
	}

	if (!isProtected)
	{
		// nie znaleziono SECURED w polu komentarza zabezpieczajacego
		partialRaport.first = false;
		partialRaport.second = "Nie wykryto poczatku zabezpieczenia exif";
		raportExif.push_back(partialRaport);
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
