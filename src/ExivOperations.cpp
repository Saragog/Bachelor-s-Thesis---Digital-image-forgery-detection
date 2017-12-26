/*
 * ExivOperations.cpp
 *
 *  Created on: 27 sie 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Komentarz:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Ten plik zawiera implementację funkcji klasy ExivOperations.
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

// Konstruktor klasy ExivOperations
ExivOperations::ExivOperations(WindowMain* con)
{
	controller = con;
	codec = QTextCodec::codecForName("UTF-8");
}

// Destruktor klasy ExivOperations
ExivOperations::~ExivOperations()
{
}

// Funkcja czytająca zawartość metadanych EXIF zawartych w pliku o podanej ścieżce
bool ExivOperations::readFromFile(std::string path)
{
	allExifData.clear();
	basicExifData.clear();

	image = Exiv2::ImageFactory::open(path);
	image->readMetadata();

    exifData = image->exifData();

    Exiv2::ExifData::const_iterator end = exifData.end();
    // przechodzimy po polach exif w pliku
    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i)
    {
        const char* tn = i->typeName();
        // zawartość tych pól jest szczególnie istotna
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
        	)basicExifData.push_back(i->key() + " " + (tn ? tn : "Unknown") + " " + i->value().toString());
        // wszystkie pola będą pokazane w oddzielnym oknie
        allExifData.push_back(i->key() + " " + (tn ? tn : "Unknown") + " " + i->value().toString());
    }
    if (exifData.empty()) return false;

    return true;
}

// Funkcja getBasicExifData zwraca zawartość istotnych pól EXIF
vector<string> ExivOperations::getBasicExifData() const
{
	return basicExifData;
}

// Funkcja getAllExifData zwraca zawartość wszystkich pól EXIF
vector<string> ExivOperations::getAllExifData() const
{
	return allExifData;
}

// Funkcja convertIntToStr przekształca liczbę całkowitą na napis
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

// Funkcja convertStrToInt przekształca napis na liczbę całkowitą
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

// Funkcja encryptData jest prostym zabezpieczeniem zawartości pola komentarza użytkownika
// Do wartości ascii dla znaków odejmuje 10
void ExivOperations::encryptData(std::string& data)
{
	unsigned int length = data.length();
	for (unsigned int x = 0; x < length; x++)
	{
		if (data[x] == ' ') continue;
		else
		{
			data[x] -= 10;
		}
	}
	return;
}

// Funkcja decryptData deszyfrowuje pole komentarza użytkownika
// dodanie 10 do wartości ascii każdego znaku
void ExivOperations::decryptData(std::string& data)
{
	unsigned int length = data.length();
	for (unsigned int x = 0; x < length; x++)
	{
		if (data[x] == ' ') continue;
		else data[x] += 10;
	}
	return;
}

// isImageFile sprawdza, czy plik o podanej ścieżce jest odpowiednim plikiem JPG
// Jeśli tak to zwraca true
// Jeśli nie to zwraca false
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

// Funcja saveExifIntoFile jest funkcją zapisującą treść exif do
// istniejącego już pliku o wybranej ścieżce
void ExivOperations::saveExifIntoFile(std::string path)
{
	prepareSecurityExifData();
	Exiv2::Image::AutoPtr savedImage = Exiv2::ImageFactory::open(path);
	savedImage->setExifData(exifData);
	savedImage->writeMetadata();
	return;
}

// Funkcja prepareSecufityExifData jest funkcją przygotowującą zawartość
// niektórych pól exif w ramach zabezpieczenia
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
        actualSize = controller->getActualSize(); // pobieranie wielkości obrazu
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
		// Nie wykryto pola XDimension
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
		// Nie wykryto pola YDimension
		securityInfoToSave += '0';
		securityInfoToSave += " ";
		isPixelYDimSet = false;
	}

    v->read(securityInfoToSave);
    // ustalanie daty
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
    // zapisywanie wartości histogramu sum RGB
    for (int tone = 0; tone < 153; tone++)
    {
    	std::cout << "Zapisuje ilosc pikseli nalezacych do tonu: " << histogramGrey[tone] << std::endl;
        securityInfoToSave += " ";
        securityInfoToSave += convertIntToStr(histogramGrey[tone]);
    }

    // zaszyfrowanie zawartości pola komentarza użytkownika
    encryptData(securityInfoToSave);
    // zmiana nazwy programu na nasz program
	exifData["Exif.Image.Software"] = "JPG Analizer";

	if (!isPixelXDimSet) exifData["Exif.Photo.PixelXDimension"] = "0";
	if (!isPixelYDimSet) exifData["Exif.Photo.PixelYDimension"] = "0";
	// nadpisanie zawartości pól exif
	exifData["Exif.Photo.UserComment"] = securityInfoToSave;
	exifData["Exif.Image.DateTime"] = dateTime;

	return;
}

// Funkcja readSecurityExifWord czyta pojedyńcze słowo z zawartości pola komentarza użytkownika
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

// Funkcja checkSoftware sprawdza, czy nazwa ostatnio wykorzystanego programu jest nazwą naszego programu
bool ExivOperations::checkSoftware()
{
	std::string programName;
	try
	{
		programName = exifData["Exif.Image.Software"].value().toString();
	}
	catch (std::exception &e)
	{
		// Nie wykryto pola nazwy wykorzystanego programu
		partialRaport.first = false;
		partialRaport.second = "Nie wykryto pola ostatnio wykorzystanego programu";
		raportExif.push_back(partialRaport);
		return false;
	}

	if (programName == "JPG Analizer")
	{	// nazwa zgodna
		partialRaport.first = true;
		partialRaport.second = "Nazwa programu zgodna";
		raportExif.push_back(partialRaport);
		return true;
	}
	else
	{	// nazwa nie jest zgodna
		partialRaport.first = false;
		partialRaport.second = "Nazwa programu wykorzystanego nie jest zgodna: " + QString::fromStdString(programName);
		raportExif.push_back(partialRaport);
	}
	return false;
}

// Funkcja checkIfProtected szuka początku zabezpieczenia pola komentarza użytkownika
bool ExivOperations::checkIfProtected(std::string word)
{
	if (word == "SECURED") // slowo SECURED jest początkiem zabezpieczenia
	{
		isProtected = true;
		partialRaport.first = true;
		partialRaport.second = codec->toUnicode("Wykryto początek zabezpieczenia exif");
		raportExif.push_back(partialRaport);
		return true;
	}
	return false;
}

// Funkcja checkPixelXDimension sprawdza zgodność zawartości pola XDimension z zawartością pola komentarza użytkownika
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
		partialRaport.second = codec->toUnicode("Zawartość XDimension zgodna z zawartością komentarza: ") + QString::fromStdString(convertIntToStr(exifXDim));
		raportExif.push_back(partialRaport);
		return true;
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = codec->toUnicode("Zawartość XDimension niezgodna: w komentarzu-") + QString::fromStdString(convertIntToStr(xdim)) +
				" w polu exif-" + QString::fromStdString(convertIntToStr(exifXDim));
		raportExif.push_back(partialRaport);
	}
	return false;
}

// Funkcja checkPixelYDimension sprawdza zgodność zawartości YDimension z zawartością pola komentarza użytkownika
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
		partialRaport.second = codec->toUnicode("Zawartość YDimension zgodna z zawartościa komentarza: ") + QString::fromStdString(convertIntToStr(exifYDim));
		raportExif.push_back(partialRaport);
		return true;
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = codec->toUnicode("Zawartość YDimension niezgodna: w komentarzu-") + QString::fromStdString(convertIntToStr(ydim)) +
				" w polu exif-" + QString::fromStdString(convertIntToStr(exifYDim));
		raportExif.push_back(partialRaport);
	}
	return false;
}

// Funkcja checkDateTime jest funkcją sprawdzającą zgodność pola daty i czasu ostatniej zmiany z zawartością pola komentarza użytkownika
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
		partialRaport.second = codec->toUnicode("Zawartość pola exif zgodna z zawartoscią komentarza: ") + QString::fromStdString(date);
		raportExif.push_back(partialRaport);
		return true;
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = codec->toUnicode("Data ostatniej zmiany nie jest zgodna z datą zapisu: w komentarzu-") +
				QString::fromStdString(word) + " w polu exif-" + QString::fromStdString(date);
		raportExif.push_back(partialRaport);
	}
	return false;
}

// Funkcja checkExifSecurity jest główną funkcją weryfikującą obraz pod względem pól metadanych EXIF
// Uruchamia funkcje sprawdzające odpowiednie pola EXIF
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
		partialRaport.second = "Wykryto pole zabezpieczające exif w wybranym obrazie";
		raportExif.push_back(partialRaport);
	}
	catch (std::exception& e)
	{
		// Nie znaleziono pola z komentarzem !!!
		partialRaport.first = false;
		partialRaport.second = "Nie wykryto pola zabezpieczającego exif w wybranym obrazie";
		raportExif.push_back(partialRaport);

	}

	decryptData(securityTagContent);	// deszyfrowanie zawartości pola komentarza użytkownika

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
		partialRaport.second = codec->toUnicode("Nie wykryto początku zabezpieczenia exif");
		raportExif.push_back(partialRaport);
	}

	if (word == "")
	{
		partialRaport.first = false;
		partialRaport.second = codec->toUnicode("Nie wiadomo jaka była wartość pola PixelXDimension przed zabezpieczeniem");
		raportExif.push_back(partialRaport);

		partialRaport.first = false;
		partialRaport.second = codec->toUnicode("Nie wiadomo jaka była wartość pola PixelYDimension przed zabezpieczeniem");
		raportExif.push_back(partialRaport);

		partialRaport.first = false;
		partialRaport.second = codec->toUnicode("Nie wiadomo jaki był czas zabezpieczenia");
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

// Funkcja getSavedGreyTones zwraca liczebność grup sum RGB wczytaną z zabezpieczenia exif
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

// Funkcja getRaportExif jest funkcją zwracającą raport stworzony w trakcie weryfikacji obrazu
std::vector<std::pair<bool, QString> > ExivOperations::getRaportExif() const
{
	return raportExif;
}
