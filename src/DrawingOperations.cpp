/*
 * DrawingOperations.cpp
 *
 *  Created on: 11 wrz 2016
 *      Author: Andrzej Dackiewicz
 *
 *  Comment:This file is a part of Digital forgery detection program that was
 *  		an engineering thesis of the author.
 *  		This file is an implementation of DrawingOperations class which is a part of the model.
 *  		This class is used for calculation and drawing operations.
 */

#include "DrawingOperations.h"
#include <QImageReader>
#include <QImageWriter>
#include <QFile>
#include <QColor>

#include <iostream>

// Constructor of DrawingOperations class
DrawingOperations::DrawingOperations()
{
    QImageReader reader(QString::fromAscii("./ProgramImages/blank.jpg"));
    histogramBase = reader.read();
    actualFileName = "";

    // TODO przerobic te trzeba na const bo nie sa zmieniane nigdzie
    // starting values of important variables
    signLength = 38;				// length of security sign
    signHeight = 34;				// height of security sign
    digitColorRange = 5;			// acceptable deviation of read RGB when reading grey histogram values
    sizeSeparatorColorRange = 25;	// acceptable difference of RGB when reading original size from image
}

// Destructor of DrawingOperations class
DrawingOperations::~DrawingOperations()
{
}

// Function setImage prepares object of DrawingOperations class to work on an image from a file of given path.
void DrawingOperations::setImage(std::string fileName)
{
	if (actualFileName != fileName) // checks if chosen file is not the same file that was used before
									// if yes then there is no need to prepare object
									// everything should be already set
	{
		// reading image from given file
		actualFileName = fileName;
		QImageReader reader(QString::fromAscii(fileName.c_str()));
		image = reader.read();

		// reading size of image
		columns = image.width();
		rows = image.height();

		rotation = ROTATION0;
		accDif = 3;					// domyslna wartosc akceptowalnego odchylenia RGB piksela od sredniej z sasiadow
									// starting value of acceptable difference - is used in image validation
									// acceptable difference for RGB deviation of pixel from average of its neighbours

		// setting default values for validation of security sign
		signBeginningX = columns - signLength;
		signBeginningY = rows - signHeight;
		sizeFirstPixelX = columns - 7 - 6 * 15;
		sizeFirstPixelY = rows -7 - 6 * 15;

		// preparation of histograms based on given image
		prepareHistogramsData();
		drawHistograms();
	}
	return;
}

// Function readPixelRGB reads RGB values of a pixel positioned in given column and row
int* DrawingOperations::readPixelRGB(int col, int row)
{
	static int result[3];
	QRgb pixel;
	QColor color;
	// reading pixel
	pixel = image.pixel(col, row);
	color.setRgb(pixel);
	// reading RGB of a pixel
	result[0] = color.red();
	result[1] = color.green();
	result[2] = color.blue();
	return result;
}

// Function secureImage
void DrawingOperations::secureImage()
{
	int* pixelRGB;
	int columns, rows;
	int sumRed, sumGreen, sumBlue;
	int mediumRed, mediumGreen, mediumBlue;
	int adjacentPixels;
	securedImage = image;

	// while securing image rotation has to be set to 0
	rotation = ROTATION0;
	// setting size of image and beginning of security sign
	columns = image.width();
	rows = image.height();
	signBeginningX = columns - signLength;
	signBeginningY = rows - signHeight;

	// main loop that makes pixels dependant on their neighbours
	// columns
	for (int col = 0; col < columns; col++)
	{
		// rows | every third pixel is to be changed
		for (int row = col % 3; row < rows; row+=3)
		{
			if (isPixelSecurityPart(col, row)) continue; // checks if actual pixel is a part of security sign or
														 // original size security
			sumRed = 0;
			sumGreen = 0;
			sumBlue = 0;
			adjacentPixels = 0;

			// checking if actual pixel has neighbours
			// adding RGBs of neighbours and counting how many neighbours there are
			if (col >= 1) 									// first to the left
			{
				pixelRGB = readPixelRGB(col-1, row);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1];	sumBlue += pixelRGB[2];
			}
			if (col >= 2)									// second to the left
			{
				pixelRGB = readPixelRGB(col-2, row);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (col < columns - 1 && !isPixelSecurityPart(col+1, row)) // first to the right
			{
				pixelRGB = readPixelRGB(col+1, row);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (col < columns - 2 && !isPixelSecurityPart(col+2, row)) // second to the right
			{
				pixelRGB = readPixelRGB(col+2, row);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (row >= 1)									// first up
			{
				pixelRGB = readPixelRGB(col, row-1);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (row >= 2)									// second up
			{
				pixelRGB = readPixelRGB(col, row - 2);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (row < rows - 1 && !isPixelSecurityPart(col, row+1)) // first down
			{
				pixelRGB = readPixelRGB(col, row + 1);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (row < rows - 2 && !isPixelSecurityPart(col, row+2))	// second down
			{
				pixelRGB = readPixelRGB(col, row + 2);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			// now we can get new RGB of pixel
			mediumRed = sumRed / adjacentPixels;
			mediumGreen = sumGreen / adjacentPixels;
			mediumBlue = sumBlue / adjacentPixels;
			// setting pixel RGB to computed values
			securedImage.setPixel(col, row, qRgb(mediumRed, mediumGreen, mediumBlue));
		}
	}
	return;
}

// Function saveImage is used to prepare security image and save it under given path
// If saving is successfull then returns true else false
bool DrawingOperations::saveImage(QString path)
{
	try
	{
		QFile file(path);
		file.open(QIODevice::WriteOnly);

		sizeFirstPixelX = columns - 7 - 6 * 15;
		sizeFirstPixelY = rows -7 - 6 * 15;

		secureImage(); // funkcja do zabezpieczenia obrazu

		drawSign(); // funkcja do tworzenia znaku zabezpieczajacego przed utrata danych exif

		prepareHistogramsData(); // funkcja przygotowuje dane do tworzenia histogramow

		securedImage.save(&file, "JPG", 100); // zapis obrazu

		file.close();
	}
	catch (std::exception& e)
	{
		// program encountered a problem saving image
		return false;
	}
	return true;
}

// Function drawChange is used to draw 5x5 pixels squares in image that is a result of validation
// Draws square around pixel of given column and row
void DrawingOperations::drawChange(int col, int row)
{
	QRgb pixel;
	QColor color;
	for (int c = col - 2; c < col + 3; c++)
	{
		for (int r = row - 2; r < row + 3; r++)
		{
			if (isPixelSecurityPart(c, r)) continue; // checks if pixel was already painted
			// if not then it can be painted
			if (c >= 0 && c < columns && r >= 0 && r < rows && pixelsChanged[c][r])
			{
				pixel = image.pixel(c, r);
				color.setRgb(pixel);
				// setting pixel RGB to red
				checkedImage.setPixel(c, r, qRgb(255, 0, 0));
				pixelsChanged[c][r] = false;
			}
		}
	}
	return;
}

// If a change is detected then lookForSimilarPixels function is used to check if neighbouring pixels have neighbours that have the same RGB as them
// Thanks to this function it is possible to find added objects of the same colour and mark them
void DrawingOperations::lookForSimilarPixels(int col, int row)
{
	drawChange(col, row); // marking square around pixel considered as changed

	// checking around upper pixel
	if (row - 1 >= 0 && pixelResults[col][row-1]) checkAdjacentPixels(col, row-1);

	// checking around left pixel
	if (col - 1 >= 0 && pixelResults[col-1][row]) checkAdjacentPixels(col-1, row);

	// checking around right pixel
	if (col + 1 < columns && pixelResults[col+1][row]) checkAdjacentPixels(col+1, row);

	// checking around lower pixel
	if (row + 1 < rows && pixelResults[col][row+1]) checkAdjacentPixels(col, row+1);
	return;
}

// Depending on detected rotation function checkAdjacentPixels uses TODO ...
void DrawingOperations::checkAdjacentPixels(int col, int row)
{
	QRgb pixel = image.pixel(col, row);
	QColor colorActual(pixel);

	int red = colorActual.red();
	int green = colorActual.green();
	int blue = colorActual.blue();

	pixelResults[col][row] = false;

	switch (rotation)
	{
		case ROTATION0:
		{
			checkAdjacentUp(col, row, red, green, blue);
			checkAdjacentLeft(col, row, red, green, blue);
			if (col < signBeginningX || row < signBeginningY) checkAdjacentRight(col, row, red, green, blue);
			if (col < signBeginningX || row < signBeginningY) checkAdjacentDown(col, row, red, green, blue);
			break;
		}
		case ROTATION90:
		{
			checkAdjacentUp(col, row, red, green, blue);
			if (col > signBeginningX || row < signBeginningY) checkAdjacentLeft(col, row, red, green, blue);
			checkAdjacentRight(col, row, red, green, blue);
			if (col > signBeginningX || row < signBeginningY) checkAdjacentDown(col, row, red, green, blue);
			break;
		}
		case ROTATION180:
		{
			if (col > signBeginningX || row > signBeginningY) checkAdjacentUp(col, row, red, green, blue);
			if (col > signBeginningX || row > signBeginningY) checkAdjacentLeft(col, row, red, green, blue);
			checkAdjacentRight(col, row, red, green, blue);
			checkAdjacentDown(col, row, red, green, blue);
			break;
		}
		case ROTATION270:
		{
			if (col < signBeginningX || row > signBeginningY) checkAdjacentUp(col, row, red, green, blue);
			checkAdjacentLeft(col, row, red, green, blue);
			if (col < signBeginningX || row > signBeginningY) checkAdjacentRight(col, row, red, green, blue);
			checkAdjacentDown(col, row, red, green, blue);
			break;
		}
	}

	return;
}

void DrawingOperations::checkAdjacentUp(int col, int row, int red, int green, int blue)
{
	QRgb pixel;
	QColor checked;

	// leci w gore

	if (row - 1 >= 0)
	{
		pixel = image.pixel(col, row-1);
		checked.setRgb(pixel);

		for (int actualRow = row-1, actualCol = col; actualRow >= 0 &&
				pixelResults[actualCol][actualRow] &&
				red == checked.red() &&
				green == checked.green() &&
				blue == checked.blue(); actualRow--
				)
		{
			// sprawdza na lewo i prawo
			pixelResults[actualCol][actualRow] = false;
			drawChange(actualCol, actualRow);

			if (actualCol-1 >= 0)
			{
				pixel = image.pixel(actualCol-1, actualRow);
				checked.setRgb(pixel);
			}

			for (int innerCol = actualCol-1; innerCol >= 0 &&
					pixelResults[innerCol][actualRow] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerCol--
					)
			{
				pixelResults[innerCol][actualRow] = false;
				drawChange(innerCol, actualRow);

				if (innerCol-1 >= 0)
				{
					pixel = image.pixel(innerCol-1, actualRow);
					checked.setRgb(pixel);
				}
			}

			if (actualCol+1 < columns)
			{
				pixel = image.pixel(actualCol+1, actualRow);
				checked.setRgb(pixel);
			}

			for (int innerCol = actualCol+1; innerCol < columns &&
					pixelResults[innerCol][actualRow] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerCol++
					)
			{
				pixelResults[innerCol][actualRow] = false;
				drawChange(innerCol, actualRow);

				if (innerCol+1 < columns)
				{
					pixel = image.pixel(innerCol+1, actualRow);
					checked.setRgb(pixel);
				}
			}

			if (actualRow-1 >= 0)
			{
				pixel = image.pixel(actualCol, actualRow-1);
				checked.setRgb(pixel);
			}
		}
	}

	return;
}

void DrawingOperations::checkAdjacentLeft(int col, int row, int red, int green, int blue)
{
	QRgb pixel;
	QColor checked;

	// leci w lewo

	if (col-1 >= 0)
	{
		pixel = image.pixel(col-1, row);
		checked.setRgb(pixel);

		for (int actualRow = row, actualCol = col-1; actualCol >= 0 &&
				pixelResults[actualCol][actualRow] &&
				red == checked.red() &&
				green == checked.green() &&
				blue == checked.blue(); actualCol--
				)
		{
			pixelResults[actualCol][actualRow] = false;
			drawChange(actualCol, actualRow);

			// sprawdza na gore i dol
			if (actualRow-1 >= 0)
			{
				pixel = image.pixel(actualCol, actualRow-1);
				checked.setRgb(pixel);
			}

			for (int innerRow = actualRow-1; innerRow >= 0 &&
					pixelResults[actualCol][innerRow] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerRow--
					)
			{
				pixelResults[actualCol][innerRow] = false;
				drawChange(actualCol, innerRow);

				if (innerRow-1 >= 0)
				{
					pixel = image.pixel(actualCol, innerRow-1);
					checked.setRgb(pixel);
				}
			}

			if (actualRow+1 < rows)
			{
				pixel = image.pixel(actualCol, actualRow+1);
				checked.setRgb(pixel);
			}

			for (int innerRow = actualRow+1; innerRow < rows &&
					pixelResults[actualCol][innerRow] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerRow++
					)
			{
				pixelResults[actualCol][innerRow] = false;
				drawChange(actualCol, innerRow);

				if (innerRow+1 < rows)
				{
					pixel = image.pixel(actualCol, innerRow+1);
					checked.setRgb(pixel);
				}
			}

			if (actualCol-1 >= 0)
			{
				pixel = image.pixel(actualCol-1, actualRow);
				checked.setRgb(pixel);
			}
		}
	}

	return;
}

void DrawingOperations::checkAdjacentRight(int col, int row, int red, int green, int blue)
{
	QRgb pixel;
	QColor checked;

	// leci w prawo

	if (col+1 < columns)
	{
		pixel = image.pixel(col+1, row);
		checked.setRgb(pixel);

		for (int actualRow = row, actualCol = col+1; actualCol < columns &&
				pixelResults[actualCol][actualRow] &&
				red == checked.red() &&
				green == checked.green() &&
				blue == checked.blue(); actualCol++
				)
		{
			// sprawdza na gore i dol
			pixelResults[actualCol][actualRow] = false;
			drawChange(actualCol, actualRow);

			if (actualRow-1 >= 0)
			{
				pixel = image.pixel(actualCol, actualRow-1);
				checked.setRgb(pixel);
			}

			for (int innerRow = actualRow-1; innerRow >= 0 &&
					pixelResults[actualCol][innerRow] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerRow--
					)
			{
				pixelResults[actualCol][innerRow] = false;
				drawChange(actualCol, innerRow);

				if (innerRow-1 >= 0)
				{
					pixel = image.pixel(actualCol, innerRow-1);
					checked.setRgb(pixel);
				}
			}

			if (actualRow+1 < rows)
			{
				pixel = image.pixel(actualCol, actualRow+1);
				checked.setRgb(pixel);
			}

			for (int innerRow = actualRow+1; innerRow < rows &&
					pixelResults[actualCol][innerRow] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerRow++
					)
			{
				pixelResults[actualCol][innerRow] = false;
				drawChange(actualCol, innerRow);

				if (innerRow+1 < rows)
				{
					pixel = image.pixel(actualCol, innerRow+1);
					checked.setRgb(pixel);
				}
			}

			if (actualCol+1 < columns)
			{
				pixel = image.pixel(actualCol+1, actualRow);
				checked.setRgb(pixel);
			}
		}
	}

	return;
}

void DrawingOperations::checkAdjacentDown(int col, int row, int red, int green, int blue)
{
	QRgb pixel;
	QColor checked;

	// leci w dol

	if (row+1 < rows)
	{
		pixel = image.pixel(col, row+1);
		checked.setRgb(pixel);

		for (int actualRow = row+1, actualCol = col; actualRow < rows &&
				pixelResults[actualCol][actualRow] &&
				red == checked.red() &&
				green == checked.green() &&
				blue == checked.blue(); actualRow++
				)
		{
			// sprawdza na lewo i prawo
			pixelResults[actualCol][actualRow] = false;
			drawChange(actualCol, actualRow);

			if (actualCol-1 >= 0)
			{
				pixel = image.pixel(actualCol-1, actualRow);
				checked.setRgb(pixel);
			}

			for (int innerCol = actualCol-1; innerCol >= 0 &&
					pixelResults[innerCol][actualRow] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerCol--
					)
			{
				pixelResults[innerCol][actualRow] = false;
				drawChange(innerCol, actualRow);

				if (innerCol-1 >= 0)
				{
					pixel = image.pixel(innerCol-1, actualRow);
					checked.setRgb(pixel);
				}
			}

			if (actualCol+1 < columns)
			{
				pixel = image.pixel(actualCol+1, actualRow);
				checked.setRgb(pixel);
			}

			for (int innerCol = actualCol+1; innerCol < columns &&
					pixelResults[innerCol][actualRow] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerCol++
					)
			{

				pixelResults[innerCol][actualRow] = false;
				drawChange(innerCol, actualRow);

				if (innerCol+1 < columns)
				{
					pixel = image.pixel(innerCol+1, actualRow);
					checked.setRgb(pixel);
				}
			}

			if (actualRow+1 < rows)
			{
				pixel = image.pixel(actualCol, actualRow+1);
				checked.setRgb(pixel);
			}
		}
	}

	return;
}

// Function isPixelSecurityPart depending on rotation says if given pixel belongs to security sign or
// original size security
// returns true if it is and false otherwise
bool DrawingOperations::isPixelSecurityPart(int col, int row)
{
	// depending on rotation
	if (rotation == ROTATION0 && ((col >= signBeginningX && row >= signBeginningY) || // rotation 0 degrees
			(col >= sizeFirstPixelX && row >= rows-2) ||
			(col >= columns-2 && row >= sizeFirstPixelY))) return true;

	else if (rotation == ROTATION90 && ((col <= signBeginningX && row >= signBeginningY) || // rotation 90 degrees
			(col <= sizeFirstPixelY && row >= rows-2) ||
			(col <= 1 && row >= sizeFirstPixelX))) return true;

	else if (rotation == ROTATION180 && ((col <= signBeginningX && row <= signBeginningY) || // rotation 180 degrees
			(col <= sizeFirstPixelX && row <= 1) ||
			(col <= 1 && row <= sizeFirstPixelY))) return true;

	else if (rotation == ROTATION270 && ((col >= signBeginningX && row <= signBeginningY) || // rotation 270 degrees
			(col >= sizeFirstPixelY && row <= 1) ||
			(col >= columns-2 && row <= sizeFirstPixelX))) return true;

	return false;
}

// TODO napisac duzo komentarzy podzielic to cos na wiecej funkcji bo to slabo wyglada
bool DrawingOperations::checkChanges()
{
	QRgb pixel;
	QColor color;
	int sumRed, sumGreen, sumBlue;
	int mediumRed, mediumGreen, mediumBlue;
	int adjacentPixels;
	int red, green, blue;

	int biggestRed, biggestGreen, biggestBlue;
	int col, row;

	// ________________________________ INICJOWANIE TABLIC I OBRAZU DO POKAZANIA ZMIAN

	checkedImage = QImage(image);

	pixelResults = new bool*[columns];

	for (int col = 0; col < columns; col++)
	{
		pixelResults[col] = new bool[rows];

		for (int row = 0; row < rows; row++)
		{
			pixelResults[col][row] = true;
		}
	}

	pixelsChanged = new bool*[columns];

	for (int col = 0; col < columns; col++)
	{
		pixelsChanged[col] = new bool[rows];

		for (int row = 0; row < rows; row++)
		{
			pixelsChanged[col][row] = true;
		}
	}

	// ____________________________ ZAZNACZANIE PRZESTRZENI ZNAKU by nie bylo sprawdzane zaleznie od obrotu

	for (col = signBeginningX; col < columns && col >= 0;) // zaznaczenie znaku ze jest juz wykorzystane by nie sprawdzalo w srodku
	{
		for (row = signBeginningY; row < rows && row >= 0;)
		{
			pixelResults[col][row] = false;
			if (rotation == ROTATION0 || rotation == ROTATION90) row++;
			else row--;
		}
		if (rotation == ROTATION0 || rotation == ROTATION270) col++;
		else col--;
	}

	for (col = sizeFirstPixelX; col < signBeginningX; col++)						// zaznaczenie zabezpieczenia rozmiaru X by nie bylo sprawdzane
	{
		pixelResults[col][rows-2] = false;
		pixelResults[col][rows-1] = false;
	}
	for (row = sizeFirstPixelY; row < signBeginningY; row++)						// zaznaczenie zabezpieczenia rozmiaru X by nie bylo sprawdzane
	{
		pixelResults[columns-2][row] = false;
		pixelResults[columns-1][row] = false;
	}

	// ____________________________ USTALANIE PARAMETROW POCZATKOWYCH PETLI SPRAWDZAJACEJ

	if (rotation == ROTATION0 || rotation == ROTATION270) col = 0;
	else col = columns-1;

	while (1)
	{
		if (rotation == ROTATION0) row = (col % 3) - 3;						// ustawiane parametry tak by przy wejsciu w petle od razu byly przestawiane
		if (rotation == ROTATION90) row = ((columns - col - 1) % 3) - 3;	// zrobilem tak aby uniknac powtarzania kodu i by moc wykorzystac operacje continue
		if (rotation == ROTATION180) row = (rows - 1 - (columns - 1 - col) % 3) + 3;
		if (rotation == ROTATION270) row = (rows - 1 - col % 3) + 3;

		while (1)
		{
			if (rotation == ROTATION0 || rotation == ROTATION90) row += 3;
			if (rotation == ROTATION180 || rotation == ROTATION270) row -=3;

			if ((rotation == ROTATION0 || rotation == ROTATION90) && row >= rows) break;
			if ((rotation == ROTATION180 || rotation == ROTATION270) && row <= -1) break;

			biggestRed = 0;
			biggestGreen = 0;
			biggestBlue = 0;

				if (pixelResults[col][row])
				{
					sumRed = 0;
					sumGreen = 0;
					sumBlue = 0;
					adjacentPixels = 0;

					// sprawdzanie na lewo o 1
					if (col >= 1 && !isPixelSecurityPart(col-1, row))
					{
						pixel = image.pixel(col - 1, row);
						color.setRgb(pixel);
						adjacentPixels++;
						sumRed += color.red();
						sumGreen += color.green();
						sumBlue += color.blue();

						if (color.red() > biggestRed) biggestRed = color.red();
						if (color.green() > biggestGreen) biggestGreen = color.green();
						if (color.blue() > biggestBlue) biggestBlue = color.blue();
					}
					// sprawdzanie na lewo o 2
					if (col >= 2 && !isPixelSecurityPart(col-2, row))
					{
						pixel = image.pixel(col - 2, row);
						color.setRgb(pixel);
						adjacentPixels++;
						sumRed += color.red();
						sumGreen += color.green();
						sumBlue += color.blue();

						if (color.red() > biggestRed) biggestRed = color.red();
						if (color.green() > biggestGreen) biggestGreen = color.green();
						if (color.blue() > biggestBlue) biggestBlue = color.blue();
					}
					// sprawdzanie na prawo o 1
					if (col < columns - 1 && !isPixelSecurityPart(col+1, row))
					{
						pixel = image.pixel(col + 1, row);
						color.setRgb(pixel);
						adjacentPixels++;
						sumRed += color.red();
						sumGreen += color.green();
						sumBlue += color.blue();

						if (color.red() > biggestRed) biggestRed = color.red();
						if (color.green() > biggestGreen) biggestGreen = color.green();
						if (color.blue() > biggestBlue) biggestBlue = color.blue();
					}
					// sprawdzanie na prawo o 2
					if (col < columns - 2 && !isPixelSecurityPart(col+2, row))
					{
						pixel = image.pixel(col + 2, row);
						color.setRgb(pixel);
						adjacentPixels++;
						sumRed += color.red();
						sumGreen += color.green();
						sumBlue += color.blue();

						if (color.red() > biggestRed) biggestRed = color.red();
						if (color.green() > biggestGreen) biggestGreen = color.green();
						if (color.blue() > biggestBlue) biggestBlue = color.blue();
					}
					// sprawdzanie na gorze o 1
					if (row >= 1 && !isPixelSecurityPart(col, row-1))
					{
						pixel = image.pixel(col, row - 1);
						color.setRgb(pixel);
						adjacentPixels++;
						sumRed += color.red();
						sumGreen += color.green();
						sumBlue += color.blue();

						if (color.red() > biggestRed) biggestRed = color.red();
						if (color.green() > biggestGreen) biggestGreen = color.green();
						if (color.blue() > biggestBlue) biggestBlue = color.blue();
					}
					// sprawdzanie na gorze o 2
					if (row >= 2 && !isPixelSecurityPart(col, row-2))
					{
						pixel = image.pixel(col, row - 2);
						color.setRgb(pixel);
						adjacentPixels++;
						sumRed += color.red();
						sumGreen += color.green();
						sumBlue += color.blue();

						if (color.red() > biggestRed) biggestRed = color.red();
						if (color.green() > biggestGreen) biggestGreen = color.green();
						if (color.blue() > biggestBlue) biggestBlue = color.blue();
					}
					// sprawdzanie na dole o 1
					if (row < rows - 1 && !isPixelSecurityPart(col, row+1))
					{
						pixel = image.pixel(col, row + 1);
						color.setRgb(pixel);
						adjacentPixels++;
						sumRed += color.red();
						sumGreen += color.green();
						sumBlue += color.blue();

						if (color.red() > biggestRed) biggestRed = color.red();
						if (color.green() > biggestGreen) biggestGreen = color.green();
						if (color.blue() > biggestBlue) biggestBlue = color.blue();
					}
					// sprawdzanie na dole o 2
					if (row < rows - 2 && !isPixelSecurityPart(col, row+2))
					{
						pixel = image.pixel(col, row + 2);
						color.setRgb(pixel);
						adjacentPixels++;
						sumRed += color.red();
						sumGreen += color.green();
						sumBlue += color.blue();

						if (color.red() > biggestRed) biggestRed = color.red();
						if (color.green() > biggestGreen) biggestGreen = color.green();
						if (color.blue() > biggestBlue) biggestBlue = color.blue();
					}

					mediumRed = sumRed / adjacentPixels;						// wyliczanie srednich wartosci rgb sasiadow
					mediumGreen = sumGreen / adjacentPixels;
					mediumBlue = sumBlue / adjacentPixels;

					pixel = image.pixel(col, row);
					color.setRgb(pixel);

					red = color.red();
					green = color.green();
					blue = color.blue();

					if (red >= mediumRed - accDif && red <= mediumRed + accDif &&  //sprawdzanie poprawnosci z mozliwoscia odchylenia rowna accDif
						green >= mediumGreen - accDif && green <= mediumGreen + accDif &&
						blue >= mediumBlue - accDif && blue <= mediumBlue + accDif
						)
					{
						// TODO nie wiem moze tutaj jakies zliczanie bedzie ile pikseli jest ok a ile nie czy cos ???
						//	std::cout << "\nPiksel jest ok !!!\n";
					}
					else
					{
						pixelResults[col][row] = false;
						lookForSimilarPixels(col, row);
					}
				}
		}

		if (rotation == ROTATION0 || rotation == ROTATION270) col++;
		else col--;

		if ((rotation == ROTATION0 || rotation == ROTATION270) && col == columns) break;
		if ((rotation == ROTATION90 || rotation == ROTATION180) && col == -1) break;
	}


	for (int col = 0; col < columns; col++)
	{
		delete pixelResults[col];
		delete pixelsChanged[col];
	}
	delete pixelResults;
	delete pixelsChanged;

	return false;
}

bool DrawingOperations::checkRotation0()
{
	signBeginningX = columns - signLength;
	signBeginningY = rows - signHeight;

	if (readPixelDigit(columns-1, rows-1) == 9 &&
		readPixelDigit(columns-2, rows-1) == 9 &&
		readPixelDigit(columns-1, rows-2) == 9)
	{
		return true;
	}

	return false;
}

bool DrawingOperations::checkRotation90()
{
	signBeginningX = signHeight - 1;
	signBeginningY = rows - signLength;

	if (readPixelDigit(0, rows-2) == 9 &&
		readPixelDigit(0, rows-1) == 9 &&
		readPixelDigit(1, rows-1) == 9)
	{
		return true;
	}

	return false;
}

bool DrawingOperations::checkRotation180()
{
	signBeginningX = signLength - 1;
	signBeginningY = signHeight - 1;

	if (readPixelDigit(0, 1) == 9 &&
		readPixelDigit(0, 0) == 9 &&
		readPixelDigit(0, 1) == 9)
	{
		return true;
	}

	return false;
}

bool DrawingOperations::checkRotation270()
{
	signBeginningX = columns - signHeight;
	signBeginningY = signLength - 1;

	if (readPixelDigit(columns-2, 0) == 9 &&
		readPixelDigit(columns-1, 0) == 9 &&
		readPixelDigit(columns-1, 1) == 9)
	{
		return true;
	}

	return false;
}

void DrawingOperations::detectRotation()
{
	if (checkRotation0())
	{
		partialRaport.first = true;
		partialRaport.second = "Wykryto brak rotacji";
		raportImage.push_back(partialRaport);

		sizeFirstPixelX = columns - 7 - 6 * 15;
		sizeFirstPixelY = rows -7 - 6 * 15;

		rotation = ROTATION0;
	}
	else if (checkRotation90())
	{
		partialRaport.first = true;
		partialRaport.second = "Wykryto rotacje 90 stopni";
		raportImage.push_back(partialRaport);

		sizeFirstPixelX = rows - 7 - 6 * 15;
		sizeFirstPixelY = 6 + 6 * 15;

		rotation = ROTATION90;
	}
	else if (checkRotation180())
	{
		partialRaport.first = true;
		partialRaport.second = "Wykryto rotacje 180 stopni";
		raportImage.push_back(partialRaport);

		sizeFirstPixelX = 6 + 6 * 15;
		sizeFirstPixelY = 6 + 6 * 15;

		rotation = ROTATION180;
	}
	else if (checkRotation270())
	{
		partialRaport.first = true;
		partialRaport.second = "Wykryto rotacje 270 stopni";
		raportImage.push_back(partialRaport);

		sizeFirstPixelX = 6 + 6 * 15;
		sizeFirstPixelY = columns - 7 - 6 * 15;

		rotation = ROTATION270;
	}
	else
	{
		// Sytuacja gdy nie zdolalismy wykryc obrotu obrazu -> obraz sprawdzany tak jak obrot 0 stopni
		signBeginningX = columns - signLength;
		signBeginningY = rows - signHeight;

		partialRaport.first = false;
		partialRaport.second = "Nie bylem w stanie okreslic rotacji obrazu";
		raportImage.push_back(partialRaport);
		rotation = ROTATION0;
	}

	return;
}

void DrawingOperations::checkImageSecurity(std::vector<int> savedGreyTones)
{
	histogramGComparison = QImage(histogramBase);

	raportImage.clear();

	detectRotation();

	readOriginalSize(0);							// sprawdzenie zapisanej oryginalnej wielkosci obrazu 0 - X / 1 - Y
	readOriginalSize(1);

	if (savedGreyTones.size() == 153)				// sprawdza wielkosc wektora
	{
		partialRaport.first = true;
		partialRaport.second = "Histogram stworzony na podstawie danych zapisanych w polu exif";
		raportImage.push_back(partialRaport);

		partialRaport.first = true;
		partialRaport.second = "Nie bylo potrzeby sprawdzania poprawnosci znaku zabezpieczajacego";
		raportImage.push_back(partialRaport);
		checkHistogramGrey(savedGreyTones);
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = "Histogram stworzony na podstawie danych zapisanych na znaku";
		raportImage.push_back(partialRaport);
		checkHistogramGrey(readFromSign());
	}

	checkChanges();

	return;
}

// Funkcja getCheckedImage zwraca obraz z zaznaczonymi na czerwono miejscami, które mogly ulec edycji
QImage DrawingOperations::getCheckedImage() const
{
	return checkedImage;
}

// Funkcja getRaportImage zwraca raport wynikowy tworzony w trakcie badan na obrazie
std::vector<std::pair<bool, std::string> > DrawingOperations::getRaportImage() const
{
	return raportImage;
}

// Funkcja getActualSize zwraca rozmiar obrazu
std::pair<int, int> DrawingOperations::getActualSize() const
{
	std::pair<int, int> size;
	size.first = columns;
	size.second = rows;
	return size;
}

// Funkcja isLargeEnough mowi czy obraz jest wystarczająco duży do wykonania na nim zabezpieczenia / sprawdzenia
bool DrawingOperations::isLargeEnough() const
{
	if (columns >= 97 && rows >= 97) return true;
	return false;
}

// Funkcja checkForNewAccDif przeprowadza badania na obrazie dla nowej wartosci mozliwego odchylenia
QImage DrawingOperations::checkForNewAccDif(int newAccDif)
{
	accDif = newAccDif;
	checkChanges();
	return checkedImage;
}
