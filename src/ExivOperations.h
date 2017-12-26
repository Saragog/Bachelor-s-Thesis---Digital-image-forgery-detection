/*
 * ExivOperations.h
 *
 *  Created on: 27 sie 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Komentarz:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  			  został stworzony w ramach pracy inżynierskiej.
 *  			  Ten plik opisuje klasę ExivOperations
 */

#ifndef EXIVOPERATIONS_H_
#define EXIVOPERATIONS_H_

#include "exiv2.hpp"
#include "WindowMain.h"
#include <vector>
#include <QTextCodec>
#include <QString>

class WindowMain;

class ExivOperations {
private:

	WindowMain* controller;
	Exiv2::Image::AutoPtr image;
	Exiv2::ExifData exifData;

	unsigned int curIndexSTag;
	std::string securityTagContent;

	std::vector<std::string> allExifData;
	std::vector<std::string> basicExifData;

	std::vector<std::pair<bool, QString> > raportExif; // raport weryfikacji obrazu pod względem EXIF
	std::pair<bool, QString> partialRaport;			   // jeden wiersz raportu

	bool isProtected;
	QTextCodec* codec;

	void prepareSecurityExifData(); // przygotowuje zawartość wybranych pól EXIF w ramach zabezpieczenia
	std::string convertIntToStr(int number); // zamienia liczbę całkowitą na napis
	int convertStrToInt(std::string word); // zamienia napis na liczbę całkowitą
	void encryptData(std::string& data);	// szyfruje treść, która ma być wstawiona do pola komentarza użytkownika
	void decryptData(std::string& data);	// deszyfrowuje treść pola komentarza użytkownika

public:
	ExivOperations(WindowMain* con);
	virtual ~ExivOperations();
	bool isImageFile(std::string path);

	std::string readSecurityExifWord(); // czyta pojedyńcze słowo z zawartości pola komentarza użytkownika
	void checkExifSecurity(); 	// to jest glowna funkcja ktora odpytuje te mniejsze
	bool checkSoftware();
	bool checkIfProtected(std::string word);
	bool checkPixelXDimension(int xdim);
	bool checkPixelYDimension(int ydim);
	bool checkDateTime(std::string word);

	void saveExifIntoFile(std::string path);					// zapisywanie metadanych exif do pliku o podanej ścieżce

	bool readFromFile(std::string path);
	std::vector<std::string> getBasicExifData() const;			 // zwraczanie raportów
	std::vector<std::string> getAllExifData() const;
	std::vector<int> getSavedGreyTones();						  // zwraca liczebność grup sum RGB wczytanych z zabezpieczenia exif
	std::vector<std::pair<bool, QString> > getRaportExif() const; // zwraca raport exif wytworzony przy weryfikacji zawartości exif
};

#endif /* EXIVOPERATIONS_H_ */
