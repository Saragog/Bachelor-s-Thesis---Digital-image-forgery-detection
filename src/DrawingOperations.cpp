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

#include <QFile>

#include <iostream>

DrawingOperations::DrawingOperations()
{
    QImageReader reader(QString::fromAscii("./ProgramImages/blank.jpg"));
    histogramBase = reader.read();
    actualFileName = "";
    accDif = 10;//35;//10;
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

		prepareHistograms();
	}
	return;
}

void DrawingOperations::prepareHistograms()
{
	QRgb pixel;
	QColor color;
	int red, green, blue, tone;
	int greatestGreyNumber = 0;
	int greatestRGBNumber = 0;

	for (int t = 0; t < 153; t++) greyTones[t] = 0; // preparing table for counting pixels with grey tones

	for (int t = 0; t < 256; t++)
	{
		redTones[t] = 0;
		greenTones[t] = 0;
		blueTones[t] = 0;
	}

	histogramGrey = QImage(histogramBase);
	histogramRGB = QImage(histogramBase);

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
			greyTones[tone]++;
		}
	}

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
		for (int row = 0; row < rows; row++)
		{
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
			if (col < columns - 1)
			{
				pixel = image.pixel(col + 1, row);
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
			if (row < rows - 1)
			{
				pixel = image.pixel(col, row + 1);
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

bool DrawingOperations::saveImage(QString path)
{
	try
	{
		QFile file(path);
		file.open(QIODevice::WriteOnly);

		secureImage();
		void prepareHistograms();
		securedImage.save(&file, "JPG");

		file.close();
	}
	catch (std::exception& e)
	{
		return false;
	}

	return true;
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

bool DrawingOperations::checkPixel(unsigned int col, unsigned int row, int r, int g, int b) const
{
	QColor color;
	QRgb pixel;
	pixel = image.pixel(col, row);
	color.setRgb(pixel);

	if (r >= color.red() - accDif && r <= color.red() + accDif &&
		g >= color.green() - accDif && g <= color.green() + accDif &&
		b >= color.blue() - accDif && b <= color.blue() + accDif
	)
	{
		std::cout << "\nPiksel " << col << " " << row << " pasuje :D \n";
		return true;
	}
	std::cout << "\nPiksel " << col << " " << row << " nie pasuje ... \n";
	return false;
}

void DrawingOperations::drawChange(int col, int row)
{
	QRgb pixel;
	QColor color;

	for (int c = col - 2; c < col + 3; c++)
	{
		for (int r = row - 2; r < row + 3; r++)
		{
			if (c >= 0 && c < columns && r >= 0 && r < rows && pixelsChanged[c][r])
			{
				pixel = image.pixel(c, r);
				color.setRgb(pixel);
				// TODO wyliczanie koloru na jaki trzeba zamalowac ten piksel tutaj !!!

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
	QRgb pixel;
	QColor colorActual;
	int red, green, blue;

	pixel = image.pixel(col, row);
	colorActual.setRgb(pixel);

	red = colorActual.red();
	green = colorActual.green();
	blue = colorActual.blue();

	pixelResults[col][row] = false;

	checkAdjacentUp(col, row, red, green, blue);
	checkAdjacentLeft(col, row, red, green, blue);
	checkAdjacentRight(col, row, red, green, blue);
	checkAdjacentDown(col, row, red, green, blue);

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
				pixelResults[actualCol][actualRow-1] &&
				red == checked.red() &&
				green == checked.green() &&
				blue == checked.blue(); actualRow--
				)
		{
			// sprawdza na lewo i prawo
			pixelResults[actualCol][actualRow-1] = false;
			drawChange(actualCol, actualRow-1);

			pixel = image.pixel(actualCol-1, actualRow);
			checked.setRgb(pixel);

			for (int innerRow = actualRow, innerCol = actualCol-1; innerCol-1 >= 0 &&
					pixelResults[col-1][row] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerCol--
					)
			{
				pixelResults[innerCol][innerRow] = false;
				drawChange(innerCol, innerRow);

				if (innerCol-1 >= 0)
				{
					pixel = image.pixel(innerCol-1, actualRow);
					checked.setRgb(pixel);
				}
			}

			pixel = image.pixel(actualCol+1, actualRow);
			checked.setRgb(pixel);

			for (int innerRow = actualRow, innerCol = actualCol+1; innerCol < columns &&
					pixelResults[col+1][row] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerCol++
					)
			{
				pixelResults[innerCol][innerRow] = false;
				drawChange(innerCol, innerRow);

				if (innerCol+1 < columns)
				{
					pixel = image.pixel(innerCol+1, actualRow);
					checked.setRgb(pixel);
				}
			}

			pixel = image.pixel(actualCol, actualRow-1);
			checked.setRgb(pixel);
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
			// sprawdza na gore i dol
			pixelResults[actualCol-1][actualRow] = false;
			drawChange(actualCol-1, actualRow);

			pixel = image.pixel(actualCol, actualRow-1);
			checked.setRgb(pixel);

			for (int innerRow = actualRow-1, innerCol = actualCol; innerRow-1 >= 0 &&
					pixelResults[col][row-1] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerRow--
					)
			{
				pixelResults[innerCol][innerRow] = false;
				drawChange(innerCol, innerRow);

				if (innerRow-1 >= 0)
				{
					pixel = image.pixel(innerCol, innerRow-1);
					checked.setRgb(pixel);
				}
			}

			pixel = image.pixel(actualCol, actualRow+1);
			checked.setRgb(pixel);

			for (int innerRow = actualRow+1, innerCol = actualCol; innerRow < rows &&
					pixelResults[col][row+1] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerRow++
					)
			{
				pixelResults[innerCol][innerRow] = false;
				drawChange(innerCol, innerRow);

				if (innerRow+1 < rows)
				{
					pixel = image.pixel(innerCol, innerRow+1);
					checked.setRgb(pixel);
				}
			}

			pixel = image.pixel(actualCol-1, actualRow);
			checked.setRgb(pixel);
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

		for (int actualRow = row, actualCol = col+1; actualCol >= 0 &&
				pixelResults[actualCol][actualRow] &&
				red == checked.red() &&
				green == checked.green() &&
				blue == checked.blue(); actualCol++
				)
		{
			// sprawdza na gore i dol
			pixelResults[actualCol+1][actualRow] = false;
			drawChange(actualCol+1, actualRow);

			pixel = image.pixel(actualCol, actualRow-1);
			checked.setRgb(pixel);

			for (int innerRow = actualRow-1, innerCol = actualCol; innerRow-1 >= 0 &&
					pixelResults[col][row-1] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerRow--
					)
			{
				pixelResults[innerCol][innerRow] = false;
				drawChange(innerCol, innerRow);

				if (innerRow-1 >= 0)
				{
					pixel = image.pixel(innerCol, innerRow-1);
					checked.setRgb(pixel);
				}
			}

			pixel = image.pixel(actualCol, actualRow+1);
			checked.setRgb(pixel);

			for (int innerRow = actualRow+1, innerCol = actualCol; innerRow+1 < rows &&
					pixelResults[col][row+1] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerRow++
					)
			{
				pixelResults[innerCol][innerRow] = false;
				drawChange(innerCol, innerRow);

				if (innerRow+1 < rows)
				{
					pixel = image.pixel(innerCol, innerRow+1);
					checked.setRgb(pixel);
				}
			}

			pixel = image.pixel(actualCol+1, actualRow);
			checked.setRgb(pixel);
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
				pixelResults[actualCol][actualRow+1] &&
				red == checked.red() &&
				green == checked.green() &&
				blue == checked.blue(); actualRow++
				)
		{
			// sprawdza na lewo i prawo
			pixelResults[actualCol][actualRow+1] = false;
			drawChange(actualCol, actualRow+1);

			pixel = image.pixel(actualCol-1, actualRow);
			checked.setRgb(pixel);

			for (int innerRow = actualRow, innerCol = actualCol-1; innerCol-1 >= 0 &&
					pixelResults[col-1][row] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerCol--
					)
			{
				pixelResults[innerCol][innerRow] = false;
				drawChange(innerCol, innerRow);

				if (innerCol-1 >= 0)
				{
					pixel = image.pixel(innerCol-1, actualRow);
					checked.setRgb(pixel);
				}
			}

			pixel = image.pixel(actualCol+1, actualRow);
			checked.setRgb(pixel);

			for (int innerRow = actualRow, innerCol = actualCol+1; innerCol+1 < columns &&
					pixelResults[col+1][row] &&
					red == checked.red() &&
					green == checked.green() &&
					blue == checked.blue(); innerCol++
					)
			{
				pixelResults[innerCol][innerRow] = false;
				drawChange(innerCol, innerRow);

				if (innerCol+1 < columns)
				{
					pixel = image.pixel(innerCol+1, actualRow);
					checked.setRgb(pixel);
				}
			}

			pixel = image.pixel(actualCol, actualRow+1);
			checked.setRgb(pixel);
		}
	}

	return;
}

// _____________________________________________

/*
void DrawingOperations::checkAdjacentPixels(int col, int row)
{
	QRgb pixel;
	QColor colorActual, checked;

	pixel = image.pixel(col, row);
	colorActual.setRgb(pixel);

	pixelResults[col][row] = false;

	std::cout << " Actual Red = " << colorActual.red() << " Actual Green = " << colorActual.green() << " Actual Blue = " << colorActual.blue() << std::endl;

	pixel = image.pixel(col, row - 1);
	checked.setRgb(pixel);
	std::cout << " Gora Red = " << checked.red() << " Gora Green = " << checked.green() << " Gora Blue = " << checked.blue() << std::endl;

	pixel = image.pixel(col - 1, row);
	checked.setRgb(pixel);
	std::cout << " Lewo Red = " << checked.red() << " Lewo Green = " << checked.green() << " Lewo Blue = " << checked.blue() << std::endl;

	pixel = image.pixel(col + 1, row);
	checked.setRgb(pixel);
	std::cout << " Prawo Red = " << checked.red() << " Prawo Green = " << checked.green() << " Prawo Blue = " << checked.blue() << std::endl;

	pixel = image.pixel(col, row + 1);
	checked.setRgb(pixel);
	std::cout << " Dol Red = " << checked.red() << " Dol Green = " << checked.green() << " Dol Blue = " << checked.blue() << std::endl;


	std::cout << "Kolumna = " << col << " Szereg = " << row << std::endl;
	if (pixelResults[col][row-1]) std::cout << "Mozna isc w gore\n";
	if (pixelResults[col-1][row]) std::cout << "Mozna isc w lewo\n";
	if (pixelResults[col+1][row]) std::cout << "Mozna isc w prawo\n";
	if (pixelResults[col][row+1]) std::cout << "Mozna isc w dol\n";
	std::cout << "\n\n";

	// gora
	if (row - 1 >= 0 && pixelResults[col][row-1])
	{
		pixel = image.pixel(col, row - 1);
		checked.setRgb(pixel);

		if (colorActual.red() == checked.red() &&
			colorActual.green() == checked.green() &&
			colorActual.blue() == checked.blue())
		{
		//	std::cout << "Ide w gore\n";
			pixelResults[col][row-1] = false;
			drawChange(col, row-1);
			checkAdjacentPixels(col, row - 1);
		}
	}
	// lewo
	if (col - 1 >= 0 && pixelResults[col-1][row])
	{
		pixel = image.pixel(col - 1, row);
		checked.setRgb(pixel);

		if (colorActual.red() == checked.red() &&
			colorActual.green() == checked.green() &&
			colorActual.blue() == checked.blue())
		{
		//	std::cout << "Ide w lewo\n";
			pixelResults[col-1][row] = false;
			drawChange(col-1, row);
			checkAdjacentPixels(col - 1, row);
		}
	}
	// prawo
	if (col + 1 < columns && pixelResults[col+1][row])
	{
		pixel = image.pixel(col + 1, row);
		checked.setRgb(pixel);

		if (colorActual.red() == checked.red() &&
			colorActual.green() == checked.green() &&
			colorActual.blue() == checked.blue())
		{
		//	std::cout << "Ide w prawo\n";
			pixelResults[col+1][row] = false;
			drawChange(col+1, row);
			checkAdjacentPixels(col + 1, row);
		}
	}
	// dol
	if (row + 1 < rows && pixelResults[col][row+1])
	{

		pixel = image.pixel(col, row + 1);
		checked.setRgb(pixel);

		if (colorActual.red() == checked.red() &&
			colorActual.green() == checked.green() &&
			colorActual.blue() == checked.blue())
		{
		//	std::cout << "Ide w dol\n";
			pixelResults[col][row+1] = false;
			drawChange(col, row+1);
			checkAdjacentPixels(col, row + 1);
		}
	}
	return;
}
*/

bool DrawingOperations::checkChanges()
{
	QRgb pixel;
	QColor color;
	int sumRed, sumGreen, sumBlue;
	int mediumRed, mediumGreen, mediumBlue;
	int adjacentPixels;
	int red, green, blue;

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
/*
	// _______________________________________
	//QRgb pixel;
	QColor colorActual, checked;
	pixel = image.pixel(col, row);
	colorActual.setRgb(pixel);

	std::cout << " Actual Red = " << colorActual.red() << " Actual Green = " << colorActual.green() << " Actual Blue = " << colorActual.blue() << std::endl;

	pixel = image.pixel(col, row - 1);
	checked.setRgb(pixel);
	std::cout << " Gora Red = " << checked.red() << " Gora Green = " << checked.green() << " Gora Blue = " << checked.blue() << std::endl;

	pixel = image.pixel(col - 1, row);
	checked.setRgb(pixel);
	std::cout << " Lewo Red = " << checked.red() << " Lewo Green = " << checked.green() << " Lewo Blue = " << checked.blue() << std::endl;

	pixel = image.pixel(col + 1, row);
	checked.setRgb(pixel);
	std::cout << " Prawo Red = " << checked.red() << " Prawo Green = " << checked.green() << " Prawo Blue = " << checked.blue() << std::endl;

	pixel = image.pixel(col, row + 1);
	checked.setRgb(pixel);
	std::cout << " Dol Red = " << checked.red() << " Dol Green = " << checked.green() << " Dol Blue = " << checked.blue() << std::endl;

	// __________________________________________
*/
	for (int col = 0; col < columns; col++)//columns; col++) //580 581//columns; col++)
	{
		for (int row = 0; row < rows; row++)//rows ; row++)
		{
				if (!pixelResults[col][row])
				{
					continue;
				}

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
				if (col < columns - 1)
				{
					pixel = image.pixel(col + 1, row);
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
				if (row < rows - 1)
				{
					pixel = image.pixel(col, row + 1);
					color.setRgb(pixel);
					adjacentPixels++;
					sumRed += color.red();
					sumGreen += color.green();
					sumBlue += color.blue();
				}

				mediumRed = sumRed / adjacentPixels;
				mediumGreen = sumGreen / adjacentPixels;
				mediumBlue = sumBlue / adjacentPixels;

				pixel = image.pixel(col, row);
				color.setRgb(pixel);

				red = color.red();
				green = color.green();
				blue = color.blue();

				if (red >= mediumRed - accDif && red <= mediumRed + accDif &&
					green >= mediumGreen - accDif && green <= mediumGreen + accDif &&
					blue >= mediumBlue - accDif && blue <= mediumBlue + accDif
					)
				{
				//	std::cout << "\nPiksel jest ok !!!\n";
				}
				else
				{
				//	std::cout << "\nPiksel sie nie zgadza !!!\n";
				//	std::cout << "Piksel: " << col << " " << row << " wartosci: " << red << " " << green << " " << blue <<
				//				" srednie wartosci to: " << mediumRed << " " << mediumGreen << " " << mediumBlue << std::endl;

					pixelResults[col][row] = false;
					lookForSimilarPixels(col, row);
				}
		}
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

	histogramGComparison = QImage(histogramBase);

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

			painter.setBrush(Qt::green);
			painter.setPen(Qt::green);
			yPos = 425 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);
		}
		else if (heightActual < heightSecured)
		{
			painter.setBrush(Qt::green);
			painter.setPen(Qt::green);
			yPos = 425 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);

			painter.setBrush(Qt::red);
			painter.setPen(Qt::red);
			yPos = 425 - heightActual;

			painter.drawRect(xPos, yPos, 3, heightActual);
		}
		else // obie wysokosci takie same czyli w ogole nie ma zmiany w histogramie
		{
			painter.setBrush(Qt::yellow);
			painter.setPen(Qt::yellow);
			yPos = 425 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);
		}
	}
	return;
}

void DrawingOperations::checkImageSecurity(std::vector<int> sCRGBs, std::vector<int> savedGreyTones)
{
	std::vector<int> rgbs;

	if (sCRGBs.size() == 12 && savedGreyTones.size() == 153)				// sprawdza wielkosci wektorow
	{
		checkPixel(0, 0, sCRGBs[0], sCRGBs[1], sCRGBs[2]);
		checkPixel(image.width() - 1, 0, sCRGBs[3], sCRGBs[4], sCRGBs[5]);
		checkPixel(0, image.height() - 1, sCRGBs[6], sCRGBs[7], sCRGBs[8]);
		checkPixel(image.width() - 1, image.height() - 1, sCRGBs[9], sCRGBs[10], sCRGBs[11]);

		checkHistogramGrey(savedGreyTones);
	}
	else
	{
		histogramGComparison = QImage(histogramBase);
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
