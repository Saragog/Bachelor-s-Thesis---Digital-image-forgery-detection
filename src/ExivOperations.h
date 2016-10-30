/*
 * ExivOperations.h
 *
 *  Created on: 27 sie 2016
 *      Author: andrzej
 */

#ifndef EXIVOPERATIONS_H_
#define EXIVOPERATIONS_H_

#include "exiv2.hpp"
#include "WindowMain.h"
#include <vector>

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

	std::vector<std::pair<bool, std::string> > raportExif;
	std::pair<bool, std::string> partialRaport;

	void prepareSecurityExifData();
	std::string convertIntToStr(int number);
	int convertStrToInt(std::string word);

public:

	ExivOperations(WindowMain* con);
	virtual ~ExivOperations();

	//bool hasExifTags(std::string path);
	bool isImageFile(std::string path);

	std::string readSecurityExifWord();
	void checkExifSecurity(); 	// narazie po prostu sprawdza potem jakis raport bedzie tworzony w stringu
								// to jest glowna funkcja ktora odpytuje te mniejsze
	bool checkSoftware();
	bool checkIfProtected(std::string word);
	bool checkPixelXDimension(int xdim);
	bool checkPixelYDimension(int ydim);
	bool checkDateTime(std::string word);

	void saveExifIntoFile(std::string path);

	bool readFromFile(std::string path);
	std::vector<std::string> getBasicExifData() const;
	std::vector<std::string> getAllExifData() const;
	std::vector<int> getSavedCornerRGBs();
	std::vector<int> getSavedGreyTones();
	std::vector<std::pair<bool, std::string> > getRaportExif() const;
};

#endif /* EXIVOPERATIONS_H_ */
