/*
 * ExivOperations.h
 *
 *  Created on: 27 sie 2016
 *      Author: andrzej
 */

#ifndef EXIVOPERATIONS_H_
#define EXIVOPERATIONS_H_

#include "exiv2.hpp"
#include <vector>

class ExivOperations {
public:

	Exiv2::Image::AutoPtr image;
	Exiv2::ExifData exifData;
	std::vector<std::string> fileExif;

	ExivOperations();
	virtual ~ExivOperations();

	void readFromFile(std::string path);
	std::vector<std::string> getExifData();

};

#endif /* EXIVOPERATIONS_H_ */
