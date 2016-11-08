/*
 * DrawingOperations.cpp
 *
 *  Created on: 11 wrz 2016
 *      Author: andrzej
 */

#include "DrawingOperations.h"
#include <QImageReader>
#include <QColor>
#include <QPainter>

#include <QImageWriter>

#include <QFile>

#include <iostream>

DrawingOperations::DrawingOperations()
{
    QImageReader reader(QString::fromAscii("./ProgramImages/blank.jpg"));
    histogramBase = reader.read();
    actualFileName = "";

    accDif = 3;
    signLength = 37;
    signHeight = 33;
    digitColorRange = 10;
}

DrawingOperations::~DrawingOperations()
{
}

void DrawingOperations::setImage(std::string fileName)
{
	if (actualFileName != fileName)
	{
		actualFileName = fileName;
		QImageReader reader(QString::fromAscii(fileName.c_str()));
		image = reader.read();

		columns = image.width();
		rows = image.height();

		prepareHistogramsData();
		drawHistograms();
	}
	return;
}

void DrawingOperations::prepareHistogramsData()
{
	QRgb pixel;
	QColor color;
	int red, green, blue, tone;

	for (int t = 0; t < 153; t++) greyTones[t] = 0; // preparing table for counting pixels with grey tones

	for (int t = 0; t < 256; t++)
	{
		redTones[t] = 0;
		greenTones[t] = 0;
		blueTones[t] = 0;
	}

	for (int pixelC = 0; pixelC < columns; pixelC++)
	{
		for (int pixelR = 0; pixelR < rows; pixelR++)
		{
			pixel = image.pixel(pixelC, pixelR);
			color.setRgb(pixel);
			red = color.red();
			green = color.green();
			blue = color.blue();

			redTones[red]++;
			greenTones[green]++;
			blueTones[blue]++;

			tone = (red + green + blue) / 5;
			if (tone == 153)
			{
				greyTones[152]++;
			}
			else greyTones[tone]++;
		}
	}
	return;
}

void DrawingOperations::drawHistograms()
{
	int greatestGreyNumber = 0;
	int greatestRGBNumber = 0;

	histogramGrey = QImage(histogramBase);
	histogramRGB = QImage(histogramBase);

	for (int x = 0; x < 256; x++)
	{
		if (redTones[x] > greatestRGBNumber)
		{
			greatestRGBNumber = redTones[x];
		}
		if (greenTones[x] > greatestRGBNumber)
		{
			greatestRGBNumber = greenTones[x];
		}
		if (blueTones[x] > greatestRGBNumber)
		{
			greatestRGBNumber = blueTones[x];
		}
	}

	for (int x = 0; x < 153; x++)
	{
		if (greyTones[x] > greatestGreyNumber)
		{
			greatestGreyNumber = greyTones[x];
		}
	}

	drawHistogramGrey(greatestGreyNumber);
	drawHistogramRGB(greatestRGBNumber);

	return;
}

void DrawingOperations::drawHistogramGrey(int greatestGreyNumber)
{
	QPainter painter(&histogramGrey);
	painter.setBrush(Qt::black);
	painter.setPen(Qt::black);

	int xPos, yPos, height;

	for (int tone = 0; tone < 153; tone++)
	{
		xPos = 50 + tone * 3;
		height = ((float)greyTones[tone] / greatestGreyNumber) * 400;
		yPos = 425 - height;
		painter.drawRect(xPos , yPos, 3, height);
	}
	return;
}

void DrawingOperations::drawHistogramRGB(int greatestRGBNumber)
{
	QPainter painter(&histogramRGB);
	painter.setBrush(Qt::black);
	painter.setPen(Qt::black);

	int xPos, yPos, height;

	for (int tone = 0; tone < 256; tone++)
	{
		painter.setBrush(Qt::red);
		painter.setPen(Qt::red);
		xPos = 24 + tone * 2;
		height = ((float)redTones[tone] / greatestRGBNumber) * 400;
		yPos = 425 - height;
		painter.drawRect(xPos , yPos, 2, 2);

		painter.setBrush(Qt::green);
		painter.setPen(Qt::green);
		xPos = 24 + tone * 2;
		height = ((float)greenTones[tone] / greatestRGBNumber) * 400;
		yPos = 425 - height;
		painter.drawRect(xPos , yPos, 2, 2);

		painter.setBrush(Qt::blue);
		painter.setPen(Qt::blue);
		xPos = 24 + tone * 2;
		height = ((float)blueTones[tone] / greatestRGBNumber) * 400;
		yPos = 425 - height;
		painter.drawRect(xPos , yPos, 2, 2);
	}
	return;
}

void DrawingOperations::secureImage()
{

	QRgb pixel;
	QColor color;
	int columns, rows;
	int sumRed, sumGreen, sumBlue;
	int mediumRed, mediumGreen, mediumBlue;
	int adjacentPixels;
	securedImage = image;

	columns = image.width();
	rows = image.height();

	for (int col = 0; col < columns; col++)
	{
		for (int row = col % 3; row < rows; row+=3)
		{
			if (col > columns - signLength && row > rows - signHeight) continue;

			sumRed = 0;
			sumGreen = 0;
			sumBlue = 0;
			adjacentPixels = 0;

			if (col >= 1)
			{
				pixel = image.pixel(col - 1, row);
				color.setRgb(pixel);
				adjacentPixels++;
				sumRed += color.red();
				sumGreen += color.green();
				sumBlue += color.blue();
			}
			if (col >= 2)
			{
				pixel = image.pixel(col - 2, row);
				color.setRgb(pixel);
				adjacentPixels++;
				sumRed += color.red();
				sumGreen += color.green();
				sumBlue += color.blue();
			}
			if (col < columns - 1 && ( col + 1 < columns - signLength || row < rows - signLength))
			{
				pixel = image.pixel(col + 1, row);
				color.setRgb(pixel);
				adjacentPixels++;
				sumRed += color.red();
				sumGreen += color.green();
				sumBlue += color.blue();
			}
			if (col < columns - 2 && ( col + 2 < columns - signLength || row < rows - signLength))
			{
				pixel = image.pixel(col + 2, row);
				color.setRgb(pixel);
				adjacentPixels++;
				sumRed += color.red();
				sumGreen += color.green();
				sumBlue += color.blue();
			}
			if (row >= 1)
			{
				pixel = image.pixel(col, row - 1);
				color.setRgb(pixel);
				adjacentPixels++;
				sumRed += color.red();
				sumGreen += color.green();
				sumBlue += color.blue();
			}
			if (row >= 2)
			{
				pixel = image.pixel(col, row - 2);
				color.setRgb(pixel);
				adjacentPixels++;
				sumRed += color.red();
				sumGreen += color.green();
				sumBlue += color.blue();
			}
			if (row < rows - 1 && ( row + 1 < rows - signHeight || col < columns - signLength))
			{
				pixel = image.pixel(col, row + 1);
				color.setRgb(pixel);
				adjacentPixels++;
				sumRed += color.red();
				sumGreen += color.green();
				sumBlue += color.blue();
			}
			if (row < rows - 2 && ( row + 2 < rows - signHeight || col < columns - signLength))
			{
				pixel = image.pixel(col, row + 2);
				color.setRgb(pixel);
				adjacentPixels++;
				sumRed += color.red();
				sumGreen += color.green();
				sumBlue += color.blue();
			}

			mediumRed = sumRed / adjacentPixels;
			mediumGreen = sumGreen / adjacentPixels;
			mediumBlue = sumBlue / adjacentPixels;

			securedImage.setPixel(col, row, qRgb(mediumRed, mediumGreen, mediumBlue));
			//securedImage.setPixel(col, row, qRgb(0, 0, 0));
		}
	}

	return;
}

QImage DrawingOperations::getHistogramGrey() const
{
	return histogramGrey;
}

QImage DrawingOperations::getHistogramRGB() const
{
	return histogramRGB;
}

bool DrawingOperations::saveImage(QString path) // funkcja zapisuje obraz do pliku o podanej sciezce
{
	try
	{
		QFile file(path);
		file.open(QIODevice::WriteOnly);

		secureImage(); // funkcja do zabezpieczenia obrazu

		drawSign(); // funkcja do tworzenia znaczka zabezpieczajacego przed utrata danych exif

		prepareHistogramsData(); // funkcja przygotowuje dane do tworzenia histogramow

		securedImage.save(&file, "JPG", 100); // zapis obrazu

		file.close();
	}
	catch (std::exception& e)
	{
		return false; // jesli nie udalo sie zapisac obrazu ( np za malo pamieci )
	}

	return true;
}

void DrawingOperations::drawSign()
{
	int digit;
	int actualTone;

	signBeginningX = columns - signLength;
	signBeginningY = rows - signHeight;

	std::cout << "\nRysuje znaczek !!!\n";

	// rysowanie gornej i dolnej czesci ramki:
	for (int x = signBeginningX; x < columns; x++)
	{
		securedImage.setPixel(x, signBeginningY, qRgb(255, 255, 255));
		securedImage.setPixel(x, rows-1, qRgb(255, 255, 255));
	}

	// rysowanie lewej i prawej czesci ramki:
	for (int y = signBeginningY+1; y < rows-1; y++)
	{
		securedImage.setPixel(signBeginningX , y, qRgb(255, 255, 255));
		securedImage.setPixel(columns-1 , y, qRgb(255, 255, 255));
	}

	// rysowanie zawartosci znaku zabezpieczajacego:
	for (int y = signBeginningY+1, tone = 0; y < rows-1; y++)
	{
		for (int step = 0; step < 5; step++, tone++)
		{
			if (tone < 153) actualTone = greyTones[tone];
			else actualTone = 0;

			for (int part = 0, divisor = 1000000; part < 7; part++, divisor/=10)
			{
				digit = actualTone / divisor;
				actualTone = actualTone - digit * divisor;

				switch (digit)
				{
					case 0:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(0, 0, 0));
						break;
					}
					case 1:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(28, 28, 28));
						break;
					}
					case 2:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(56, 56, 56));
						break;
					}
					case 3:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(84, 84, 84));
						break;
					}
					case 4:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(112, 112, 112));
						break;
					}
					case 5:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(140, 140, 140));
						break;
					}
					case 6:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(168, 168, 168));
						break;
					}
					case 7:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(196, 196, 196));
						break;
					}
					case 8:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(224, 224, 224));
						break;
					}
					case 9:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(255, 255, 255));
						break;
					}
				}
			}
		}
	}

	return;
}

std::vector<int> DrawingOperations::getCornerRGBs() const
{
	std::vector<int> cornerRGBs;
	QColor color;
	QRgb pixel;

	pixel = image.pixel(0,0);
	color.setRgb(pixel);
	cornerRGBs.push_back(color.red());
	cornerRGBs.push_back(color.green());
	cornerRGBs.push_back(color.blue());

	pixel = image.pixel(image.width()-1, 0);
	color.setRgb(pixel);
	cornerRGBs.push_back(color.red());
	cornerRGBs.push_back(color.green());
	cornerRGBs.push_back(color.blue());

	pixel = image.pixel(0, image.height()-1);
	color.setRgb(pixel);
	cornerRGBs.push_back(color.red());
	cornerRGBs.push_back(color.green());
	cornerRGBs.push_back(color.blue());

	pixel = image.pixel(image.width()-1, image.height()-1);
	color.setRgb(pixel);
	cornerRGBs.push_back(color.red());
	cornerRGBs.push_back(color.green());
	cornerRGBs.push_back(color.blue());

	return cornerRGBs;
}

int* DrawingOperations::getHistGTones()
{
	return greyTones;
}

void DrawingOperations::drawChange(int col, int row)
{
	QRgb pixel;
	QColor color;

	for (int c = col - 2; c < col + 3; c++)
	{
		for (int r = row - 2; r < row + 3; r++)
		{
			if (rotation == ROTATION0 && c >= signBeginningX && r >= signBeginningY) continue;
			if (rotation == ROTATION90 && c <= signBeginningX && r >= signBeginningY) continue;
			if (rotation == ROTATION180 && c <= signBeginningX && r <= signBeginningY) continue;
			if (rotation == ROTATION270 && c >= signBeginningX && r <= signBeginningY) continue;

			if (c >= 0 && c < columns && r >= 0 && r < rows && pixelsChanged[c][r])
			{
				pixel = image.pixel(c, r);
				color.setRgb(pixel);

				checkedImage.setPixel(c, r, qRgb(255, 0, 0));
				pixelsChanged[c][r] = false;
			}
		}
	}

	return;
}

void DrawingOperations::lookForSimilarPixels(int col, int row)
{
	drawChange(col, row);

	if (row - 1 >= 0 && pixelResults[col][row-1])
	{
		// sprawdzanie otoczenia piksela gornego
		checkAdjacentPixels(col, row-1);
	}
	if (col - 1 >= 0 && pixelResults[col-1][row])
	{
		// sprawdzanie otoczenia piksela lewego
		checkAdjacentPixels(col-1, row);
	}
	if (col + 1 < columns && pixelResults[col+1][row])
	{
		// sprawdzanie otoczenia piksela prawego
		checkAdjacentPixels(col+1, row);
	}
	if (row + 1 < rows && pixelResults[col][row+1])
	{
		// sprawdzanie otoczenia piksela dolnego
		checkAdjacentPixels(col, row+1);
	}

	return;
}

// _________________________________________

void DrawingOperations::checkAdjacentPixels(int col, int row)
{
	// TODO Przetestowac dzialanie tego :D

	QRgb pixel;
	QColor colorActual;
	int red, green, blue;

	pixel = image.pixel(col, row);
	colorActual.setRgb(pixel);

	red = colorActual.red();
	green = colorActual.green();
	blue = colorActual.blue();

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

bool DrawingOperations::checkChanges()
{
	//std::cout << "\nRows: " << rows;
	//std::cout << "Columns: " << columns << std::endl;

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

	// ____________________________ ZAZNACZANIE PRZESTRZENI ZNACZKA by nie bylo sprawdzane

	for (col = signBeginningX; col < columns && col >= 0;) // zaznaczenie znaczka ze jest juz wykorzystane by nie sprawdzalo w srodku
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
					if (col >= 1)
					{
						// TODO tutaj ify dla wchodzenia do znaczka na lewo

						if ((rotation == ROTATION90 && (col - 1 <= signBeginningX && row >= signBeginningY)) ||
							 (rotation == ROTATION180 && (col - 1 <= signBeginningX && row <= signBeginningY)))
						{
							continue;
						}

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
					if (col >= 2)
					{
						// TODO tutaj ify dla wchodzenia do znaczka na lewo

						if ((rotation == ROTATION90 && (col - 2 <= signBeginningX && row >= signBeginningY)) ||
							 (rotation == ROTATION180 && (col - 2 <= signBeginningX && row <= signBeginningY)))
						{
							continue;
						}

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
					if (col < columns - 1)
					{
						if ((rotation == ROTATION0 && (col + 1 >= signBeginningX && row >= signBeginningY)) ||
							  (rotation == ROTATION270 && (col + 1 >= signBeginningX && row <= signBeginningY)))
						{
							continue;
						}

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
					if (col < columns - 2)
					{
						if ((rotation == ROTATION0 && (col + 2 >= signBeginningX && row >= signBeginningY)) ||
							  (rotation == ROTATION270 && (col + 2 >= signBeginningX && row <= signBeginningY)))
						{
							continue;
						}

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
					if (row >= 1)
					{
						if ((rotation == ROTATION180 && (col <= signBeginningX && row -1 <= signBeginningY)) ||
								(rotation == ROTATION270 && (col >= signBeginningX && row - 1 <= signBeginningY)))
						{
							continue;
						}

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
					if (row >= 2)
					{
						if ((rotation == ROTATION180 && (col <= signBeginningX && row -2 <= signBeginningY)) ||
								(rotation == ROTATION270 && (col >= signBeginningX && row - 2 <= signBeginningY)))
						{
							continue;
						}

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
					if (row < rows - 1)
					{
						if ((rotation == ROTATION0 && (col >= signBeginningX && row + 1 >= signBeginningY)) ||
								(rotation == ROTATION90 && (col <= signBeginningX && row + 1 >= signBeginningY)))
						{
							continue;
						}

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
					if (row < rows - 2)
					{
						if ((rotation == ROTATION0 && (col >= signBeginningX && row + 2 >= signBeginningY)) ||
								(rotation == ROTATION90 && (col <= signBeginningX && row + 2 >= signBeginningY)))
						{
							continue;
						}

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
						// TODO w takim razie tutaj powinno byc zliczanie pikseli dla ktorych sie nie udalo sprawdzenie

						//std::cout << "\nPiksel sie nie zgadza !!!\n";
						//std::cout << "Piksel: " << col << " " << row << " wartosci: " << red << " " << green << " " << blue <<
						//			" srednie wartosci to: " << mediumRed << " " << mediumGreen << " " << mediumBlue << std::endl;

						// ______________
	//
	//					if (row - 1 >= 0)
	//					{
	//						pixel = image.pixel(col, row-1);
	//						color.setRgb(pixel);
	//
	//						red = color.red();
	//						green = color.green();
	//						blue = color.blue();
	//						std::cout << "Piksel na gorze: " << red << " " << green << " " << blue << std::endl;
	//					}

	//					if (col - 1 >= 0)
	//					{
	//						pixel = image.pixel(col-1, row);
	//						color.setRgb(pixel);

	//						red = color.red();
	//						green = color.green();
	//						blue = color.blue();
	//						std::cout << "Piksel po lewej: " << red << " " << green << " " << blue << std::endl;
	//					}

	//					if (col + 1 < columns && (col + 1 < signBeginningX || row < signBeginningY))
	//					{
	//						pixel = image.pixel(col+1, row);
	//						color.setRgb(pixel);

	//						red = color.red();
	//						green = color.green();
	//						blue = color.blue();
	//						std::cout << "Piksel po prawej: " << red << " " << green << " " << blue << std::endl;
	//					}

	//					if (row + 1 < rows && (col < signBeginningX || row + 1 < signBeginningY))
	//					{
	//						pixel = image.pixel(col, row+1);
	//						color.setRgb(pixel);

	//						red = color.red();
	//						green = color.green();
	//						blue = color.blue();
	//						std::cout << "Piksel na dole: " << red << " " << green << " " << blue << std::endl;
	//					}


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

void DrawingOperations::checkHistogramGrey(std::vector<int> savedGreyTones)
{
	int heightSecured, heightActual;
	int greatestNumber;
	int xPos, yPos;

	//histogramGComparison = QImage(histogramBase);

	QPainter painter(&histogramGComparison);

	// szukanie najwiekszej wartosci sposrod aktualnych i zapisanych

	for (int tone = 0; tone < 153; tone++)
	{
		if (greatestNumber < savedGreyTones[tone])
		{
			greatestNumber = savedGreyTones[tone];
		}
		if (greatestNumber < greyTones[tone])
		{
			greatestNumber = greyTones[tone];
		}
	}

	// wyliczanie polozenia, kolejnosci rysowania i wysokosci prostokatow

	for (int tone = 0; tone < 153; tone++)
	{
		xPos = 50 + tone * 3;

		painter.setBrush(Qt::green);
		painter.setPen(Qt::green);

		heightSecured = ((float)savedGreyTones[tone] / greatestNumber ) * 400;
		heightActual = ((float)greyTones[tone] / greatestNumber ) * 400;

		if (heightActual > heightSecured)
		{
			painter.setBrush(Qt::red);
			painter.setPen(Qt::red);
			yPos = 425 - heightActual;

			painter.drawRect(xPos, yPos, 3, heightActual);

			painter.setBrush(Qt::yellow);
			painter.setPen(Qt::yellow);
			yPos = 425 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);
		}
		else if (heightActual < heightSecured)
		{
			painter.setBrush(Qt::yellow);
			painter.setPen(Qt::yellow);
			yPos = 425 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);

			painter.setBrush(Qt::red);
			painter.setPen(Qt::red);
			yPos = 425 - heightActual;

			painter.drawRect(xPos, yPos, 3, heightActual);
		}
		else // obie wysokosci takie same czyli w ogole nie ma zmiany w histogramie
		{
			painter.setBrush(Qt::green);
			painter.setPen(Qt::green);
			yPos = 425 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);
		}
	}
	return;
}

unsigned char DrawingOperations::readPixelDigit(int col, int row)
{
	QRgb pixel;
	QColor color;

	int red, green, blue;
	unsigned char digit;

	pixel = image.pixel(col, row);
	color.setRgb(pixel);

	red = color.red();
	green = color.green();
	blue = color.blue();

	//std::cout << "\nKolor rgb piksela: " << red << " " << green << " " << blue << std::endl;

	if (red <= 0 + digitColorRange &&
		green <= 0 + digitColorRange &&
		blue <= 0 + digitColorRange)
	{
		// 0 0 0 czyli czarny - 0
		digit = 0;
	}
	else if (red >= 28 - digitColorRange && red <= 28 + digitColorRange &&
		green >= 28 - digitColorRange && green <= 28 + digitColorRange &&
		blue >= 28 - digitColorRange && blue <= 28 + digitColorRange)
	{
		// 28 28 28 odcien szarosci - 1
		digit = 1;
	}
	else if (red >= 56 - digitColorRange && red <= 56 + digitColorRange &&
			green >= 56 - digitColorRange && green <= 56 + digitColorRange &&
			blue >= 56 - digitColorRange && blue <= 56 + digitColorRange)
	{
		// 56 56 56 odcien szarosci - 2
		digit = 2;
	}
	else if (red >= 84 - digitColorRange && red <= 84 + digitColorRange &&
			green >= 84 - digitColorRange && green <= 84 + digitColorRange &&
			blue >= 84 - digitColorRange && blue <= 84 + digitColorRange)
	{
		// 84 84 84 odcien szarosci - 3
		digit = 3;
	}
	else if (red >= 112 - digitColorRange && red <= 112 + digitColorRange &&
			green >= 112 - digitColorRange && green <= 112 + digitColorRange &&
			blue >= 112 - digitColorRange && blue <= 112 + digitColorRange)
	{
		// 112 112 112 odcien szarosci - 4
		digit = 4;
	}
	else if (red >= 140 - digitColorRange && red <= 140 + digitColorRange &&
			green >= 140 - digitColorRange && green <= 140 + digitColorRange &&
			blue >= 140 - digitColorRange && blue <= 140 + digitColorRange)
	{
		// 140 140 140 odcien szarosci - 5
		digit = 5;
	}
	else if (red >= 168 - digitColorRange && red <= 168 + digitColorRange &&
			green >= 168 - digitColorRange && green <= 168 + digitColorRange &&
			blue >= 168 - digitColorRange && blue <= 168 + digitColorRange)
	{
		// 168 168 168 odcien szarosci - 6
		digit = 6;
	}
	else if (red >= 196 - digitColorRange && red <= 196 + digitColorRange &&
			green >= 196 - digitColorRange && green <= 196 + digitColorRange &&
			blue >= 196 - digitColorRange && blue <= 196 + digitColorRange)
	{
		// 196 196 196 odcien szarosci - 7
		digit = 7;
	}
	else if (red >= 224 - digitColorRange && red <= 224 + digitColorRange &&
			green >= 224 - digitColorRange && green <= 224 + digitColorRange &&
			blue >= 224 - digitColorRange && blue <= 224 + digitColorRange)
	{
		// 224 224 224 odcien szarosci - 8
		digit = 8;
	}
	else if (red >= 255 - digitColorRange &&
			green >= 255 - digitColorRange &&
			blue >= 255 - digitColorRange)
	{
		// 255 255 255 czyli bialy - 9
		digit = 9;
	}
	else
	{
		// piksel wykracza barwa poza nasz zakres !!!
		digit = 10;
	}
	return digit;
}

int DrawingOperations::convertDigitsToInt(int* digits)
{
	int number = 0;

	for (int step = 0; step < 7; step++)
	{
		number *= 10;
		number += digits[step];
	}

	return number;
}

std::vector<int> DrawingOperations::readFromSign()
{
	std::vector<int> readGreyTones; // ilosci pikseli w odcieni szarosci odczytane z obrazka ( z zabezpieczenia w postaci bloku )
	int readDigit;
	int digits[7];
	int temp;

	int wrgPxlCntr = 0; // wrong pixel counter - ile pikseli ma niewlasciwa wartosc w znaczku

	int actualRow, toneIndex;

										// TODO everything ... zrobic ify i while zamiast forow czy cos takiego i powinno byc ok
										// rozrysowac sobie najlepiej ;D
										// poprawic to z kosciolkiem tam jest w chodzeniu zdaje sie w lewo tam cos nie tak jest
										// zrobic by jakis raporcik sie robil i w nim np jaki obrot ( ew ze sie nie udalo )
										// do tego zliczanie miejsc gdzie sie wykrylo falszerstwo i to bedzie pokazywane

	toneIndex = 0;

	switch (rotation)
	{
		case ROTATION0:
		{
			infoBeginningX = signBeginningX+1;
			infoBeginningY = signBeginningY+1;
			actualRow = infoBeginningY;
			break;
		}
		case ROTATION90:
		{
			infoBeginningX = signBeginningX-1;
			infoBeginningY = signBeginningY+1;
			actualRow = infoBeginningX;
			break;
		}
		case ROTATION180:
		{
			infoBeginningX = signBeginningX-1;
			infoBeginningY = signBeginningY-1;
			actualRow = infoBeginningY;
			break;
		}
		case ROTATION270:
		{
			infoBeginningX = signBeginningX+1;
			infoBeginningY = signBeginningY-1;
			actualRow = infoBeginningX;
			break;
		}
	}

	while (toneIndex < 153)
	{
		//std::cout << "\nSzereg: " << actualRow << " toneIndex: " << toneIndex << std::endl;
		for (int step = 0; step < 5 && toneIndex < 153; step++, toneIndex++)
		{
			for (int numberPart = 0; numberPart < 7; numberPart++)
			{
				switch (rotation)
				{
					case ROTATION0:
					{
						readDigit = readPixelDigit(infoBeginningX + step * 7 + numberPart, actualRow);
						break;
					}
					case ROTATION90:
					{
						readDigit = readPixelDigit(actualRow, infoBeginningY + step * 7 + numberPart);
						break;
					}
					case ROTATION180:
					{
						readDigit = readPixelDigit(infoBeginningX - step * 7 - numberPart, actualRow);
						break;
					}
					case ROTATION270:
					{
						readDigit = readPixelDigit(actualRow, infoBeginningY - step * 7 - numberPart);
						break;
					}
				}

				if (readDigit == 10)
				{
					//std::cout << "\nBlad czytania wartosci piksela znaku - nieznany kolor !!!\n";
					// blad jest w znaku piksel ma wartosc niewazna !!!
					digits[numberPart] = 0;
					wrgPxlCntr++;
				}
				else
				{
					//std::cout << "\nWczytalem piksel dajacy wartosc: " << readDigit << std::endl;
					digits[numberPart] = readDigit;
				}
			}
			// Funkcja dajaca liczbe na podstawie 7 cyfr danych
			temp = convertDigitsToInt(digits);
			readGreyTones.push_back(temp);

			std::cout << "\n Wektor teraz ma wielkosc " << readGreyTones.size() << " dodalem: " << temp;
		}

		switch (rotation)
		{
			case ROTATION0:
			{
				actualRow++;
				break;
			}
			case ROTATION90:
			{
				actualRow--;
				break;
			}
			case ROTATION180:
			{
				actualRow--;
				break;
			}
			case ROTATION270:
			{
				actualRow++;
				break;
			}
		}
	}

	if (wrgPxlCntr)
	{
		partialRaport.first = true;
		partialRaport.second = "Informacje o histogramie zapisane na znaczku odczytane prawidlowo";
		raportImage.push_back(partialRaport);
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = "Informacje o histogramie zapisane na znaczku odczytane z bledami - ilosc bledow: " + wrgPxlCntr;
		raportImage.push_back(partialRaport);
	}

	return readGreyTones;
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
		std::cout << "\nObrot o 0 stopni !!!\n";
		partialRaport.first = true;
		partialRaport.second = "Wykryto brak rotacji";
		raportImage.push_back(partialRaport);
		rotation = ROTATION0;
	}
	else if (checkRotation90())
	{
		std::cout << "\nObrot o 90 stopni !!!\n";
		partialRaport.first = true;
		partialRaport.second = "Wykryto rotacje 90 stopni";
		raportImage.push_back(partialRaport);
		rotation = ROTATION90;
	}
	else if (checkRotation180())
	{
		std::cout << "\nObrot o 180 stopni !!!\n";
		partialRaport.first = true;
		partialRaport.second = "Wykryto rotacje 180 stopni";
		raportImage.push_back(partialRaport);
		rotation = ROTATION180;
	}
	else if (checkRotation270())
	{
		std::cout << "\nObrot o 270 stopni !!!\n";
		partialRaport.first = true;
		partialRaport.second = "Wykryto rotacje 270 stopni";
		raportImage.push_back(partialRaport);
		rotation = ROTATION270;
	}
	else
	{
		// TODO zastanowic sie tutaj jak bedzie gdy nie wykryjemy obrotu.
		// Moze potraktowac to tak jakby obrotu nie bylo i najwyzej wszystko bedzie zamazane
		std::cout << "\nNie potrafilem znalezc obrotu !!!\n";
		partialRaport.first = false;
		partialRaport.second = "Nie bylem w stanie okreslic rotacji";
		raportImage.push_back(partialRaport);
		rotation = ROTATION0;
	}

	return;
}

void DrawingOperations::checkImageSecurity(std::vector<int> savedGreyTones)
{
	histogramGComparison = QImage(histogramBase);

	detectRotation();

	if (savedGreyTones.size() == 153)				// sprawdza wielkosc wektora
	{
		partialRaport.first = true;
		partialRaport.second = "Histogram stworzony na podstawie danych zapisanych w polu exif";
		raportImage.push_back(partialRaport);

		partialRaport.first = false;
		partialRaport.second = "Nie bylo potrzeby sprawdzania poprawnosci znaczka";
		raportImage.push_back(partialRaport);
		checkHistogramGrey(savedGreyTones);
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = "Histogram stworzony na podstawie danych zapisanych na znaczku";
		raportImage.push_back(partialRaport);
		checkHistogramGrey(readFromSign());
	}

	checkChanges();

	return;
}

QImage DrawingOperations::getHistogramComparison() const
{
	return histogramGComparison;
}

QImage DrawingOperations::getCheckedImage() const
{
	return checkedImage;
}

std::vector<std::pair<bool, std::string> > DrawingOperations::getRaportImage() const
{
	return raportImage;
}
