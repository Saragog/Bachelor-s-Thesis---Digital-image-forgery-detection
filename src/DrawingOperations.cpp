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
    accDif = 20;//20;//35;//10;
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
	// TODO zabezpieczyc jakos ten obraz a nie ... :D
	// TERAZ TO TRZEBA ZROBIC !!!

	QRgb pixel;
	QColor color;
	int columns, rows;
	int sumRed, sumGreen, sumBlue;
	int mediumRed, mediumGreen, mediumBlue;
	int adjacentPixels;
	securedImage = image;

	columns = image.width();
	rows = image.height();

	for (int col = 1; col < columns; col+=3)
	{
		for (int row = 1; row < rows; row+=3)
		{
			//if (((row - col) % 3) == 0)
			//{
				sumRed = 0;
				sumGreen = 0;
				sumBlue = 0;
				adjacentPixels = 0;

				//if (col >= 1)
				//{
					pixel = image.pixel(col - 1, row);
					color.setRgb(pixel);
					adjacentPixels++;
					sumRed += color.red();
					sumGreen += color.green();
					sumBlue += color.blue();
				//}
				//if (col < columns - 1)
				//{
					pixel = image.pixel(col + 1, row);
					color.setRgb(pixel);
					adjacentPixels++;
					sumRed += color.red();
					sumGreen += color.green();
					sumBlue += color.blue();
				//}
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

				if (col == 1 && row == 1)
				{
					std::cout << "\nZapisany czerwony = " << mediumRed <<
							 " Zapisany zielony = " << mediumGreen <<
							 " Zapisany niebieski = " << mediumBlue << std::endl;
				}

				securedImage.setPixel(col, row, qRgb(mediumRed, mediumGreen, mediumBlue));
			//}
		}
	}

	/*

	for (int row = 0; row < image.height(); row++)
	{
		for (int col = 0; col < image.width(); col++)
		{
			if ((row + col) % 3 == 0)
			{

			}
		}
	}

	*/

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
	//	std::cout << "\nPiksel " << col << " " << row << " pasuje :D \n";
		return true;
	}
	//std::cout << "\nPiksel " << col << " " << row << " nie pasuje ... \n";
	return false;
}

void DrawingOperations::drawChange(int col, int row)
{
	//QColor color;
	//QRgb pixel;

	//std::cout << "\nPiksel do zamalowania to: " << row << " " << col << std::endl;

	int columns, rows;
	columns = image.width();
	rows = image.height();

	//std::cout << "\nColumns: " << columns << " Rows: " << rows << std::endl;

	for (int actualCol = col - 1; actualCol <= col + 1; actualCol++)
	{
		for (int actualRow = row - 1; actualRow <= row + 1; actualRow++)
		{

			//std::cout << "\nactualCol = " << actualCol << " actualRow = " << actualRow << std::endl;
			if (actualCol >= 0 && actualCol < columns && actualRow >= 0 && actualRow < rows)
			{
				//std::cout << "\nMaluje !!!\n";
				//pixel = image.pixel(actualCol, actualRow);
				//color.setRgb(pixel);
				checkedImage.setPixel(actualCol, actualRow, qRgb(255, 0, 0));
			}
			if (actualRow + 1 == rows) break;
		}
		if (actualCol + 1 == columns) break;
	}

	return;
}

void DrawingOperations::checkAdjacentSquares(int col, int row, int colInd, int rowInd)
{
	QRgb pixel;
	QColor colorActual, checked;
	drawChange(col, row);
	pixel = image.pixel(col, row);
	colorActual.setRgb(pixel);

	//std::cout << "\nrowInd - 1 = " << rowInd - 1 << " blockRes = " << blockResults[colInd][rowInd-1] <<
	//		" ";

	// gora
	if (rowInd - 1 >= 0 && blockResults[colInd][rowInd-1])
	{
		//std::cout << "\ngora\n";
		if (row - 3 >= 0)
		{
			pixel = image.pixel(col, row - 3);
			checked.setRgb(pixel);

			if (colorActual.red() == checked.red() &&
				colorActual.green() == checked.green() &&
				colorActual.blue() == checked.blue())
			{
				blockResults[colInd][rowInd] = false;
				checkAdjacentSquares(col, row - 3, colInd, rowInd - 1);
			}
		}
	}
	// lewo
	if (colInd - 1 >= 0 && blockResults[colInd-1][rowInd])
	{
	//	std::cout << "\nlewo\n";

		if (col - 3 >= 0)
		{
			pixel = image.pixel(col - 3, row);
			checked.setRgb(pixel);

			if (colorActual.red() == checked.red() &&
				colorActual.green() == checked.green() &&
				colorActual.blue() == checked.blue())
			{
				blockResults[colInd][rowInd] = false;
				checkAdjacentSquares(col - 3, row, colInd - 1, rowInd);
			}
		}
	}
	// prawo
	if (colInd + 1 < colIndex && blockResults[colInd+1][rowInd])
	{
	//	std::cout << "\nprawo\n";
		if (col + 3 < columns)
		{
			pixel = image.pixel(col + 3, row);
			checked.setRgb(pixel);

			if (colorActual.red() == checked.red() &&
				colorActual.green() == checked.green() &&
				colorActual.blue() == checked.blue())
			{
				blockResults[colInd][rowInd] = false;
				checkAdjacentSquares(col + 3, row, colInd + 1, rowInd);
			}
		}
	}
	// dol
	if (rowInd + 1 < rowIndex && blockResults[colInd][rowInd+1])
	{
	//	std::cout << "\ndol\n";
		if (row + 3 < rows)
		{
			pixel = image.pixel(col, row + 3);
			checked.setRgb(pixel);

			if (colorActual.red() == checked.red() &&
				colorActual.green() == checked.green() &&
				colorActual.blue() == checked.blue())
			{
				blockResults[colInd][rowInd] = false;
				checkAdjacentSquares(col, row + 3, colInd, rowInd + 1);
			}
		}
	}
	return;
}

bool DrawingOperations::checkChanges()
{
	QRgb pixel;
	QColor color;
	int sumRed, sumGreen, sumBlue;
	int mediumRed, mediumGreen, mediumBlue;
	int adjacentPixels;
	int red, green, blue;

	checkedImage = QImage(image);

	if (columns % 3 != 0) colIndex = columns / 3 + 1;
	else colIndex = columns / 3;

	blockResults = new bool*[colIndex];

	if (rows % 3 != 0) rowIndex = rows / 3 + 1;
	else rowIndex = rows / 3;

	std::cout << "Columns = " << columns << " Rows = " << rows << " colIndex = " << colIndex << " rowIndex = " << rowIndex << std::endl;

	for (int col = 0; col < colIndex; col++)
	{
		blockResults[col] = new bool[rowIndex];

		for (int row = 0; row < rowIndex; row++)
		{
			blockResults[col][row] = true;
		}
	}

	for (int col = 1, colInd = 0; col < columns; col+=3, colInd++)
	{
		for (int row = 1, rowInd = 0; row < rows ; row+=3, rowInd++)//rows; row++)
		{
				if (!blockResults[colInd][rowInd])
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

				/*if (red == 176 && green == 190 && blue == 55)
				{
					std::cout << "\nNatrafilem na ten kolor:";
					std::cout << "\nmediumRed = " << mediumRed <<
								 "\nmediumGreen = " << mediumGreen <<
								 "\nmediumBlue = " << mediumBlue << std::endl;

					std::cout << "\nPo lewej: ";
					pixel = image.pixel(col - 1, row);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					std::cout << "\nPo prawej: ";
					pixel = image.pixel(col + 1, row);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					std::cout << "\nZ gory: ";
					pixel = image.pixel(col, row - 1);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					std::cout << "\nZ dolu: ";
					pixel = image.pixel(col, row + 1);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					std::cout << "\nZ dolu: ";
					pixel = image.pixel(col, row + 1);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					return false;
				}*/

				if (red >= mediumRed - accDif && red <= mediumRed + accDif &&
					green >= mediumGreen - accDif && green <= mediumGreen + accDif &&
					blue >= mediumBlue - accDif && blue <= mediumBlue + accDif
					)
				{
					//std::cout << "\nPiksel jest ok !!!\n";
				}
				else
				{
					//std::cout << "\nPiksel sie nie zgadza !!!\n";
					//std::cout << "\nPiksel: " << row << " " << col << " wartosci: " << red << " " << green << " " << blue <<
					//			" srednie wartosci to: " << mediumRed << " " << mediumGreen << " " << mediumBlue << std::endl;

					blockResults[colInd][rowInd] = false;
					checkAdjacentSquares(col, row, colInd, rowInd);
				}

		}
	}

	for (int col = 0; col < colIndex; col++)
	{
		delete blockResults[col];
	}
	delete blockResults;

	return false;
}








/*
bool DrawingOperations::checkChanges()
{
	QRgb pixel;
	QColor color;
	int sumRed, sumGreen, sumBlue;
	int mediumRed, mediumGreen, mediumBlue;
	int adjacentPixels;
	int red, green, blue;

	checkedImage = QImage(image);

	if (columns % 3 != 0) colIndex = columns / 3 + 1;
	else colIndex = columns / 3;

	blockResults = new bool*[colIndex];

	if (rows % 3 != 0) rowIndex = rows / 3 + 1;
	else rowIndex = rows / 3;

	std::cout << "Columns = " << columns << " Rows = " << rows << " colIndex = " << colIndex << " rowIndex = " << rowIndex << std::endl;

	for (int col = 0; col < colIndex; col++)
	{
		blockResults[col] = new bool[rowIndex];

		for (int row = 0; row < rowIndex; row++)
		{
			blockResults[col][row] = true;
		}
	}

	for (int col = 1, colInd = 0; col < columns; col+=3, colInd++)
	{
		for (int row = 1, rowInd = 0; row < rows ; row+=3, rowInd++)//rows; row++)
		{
				if (!blockResults[colInd][rowInd])
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

				if (red == 176 && green == 190 && blue == 55)
				{
					std::cout << "\nNatrafilem na ten kolor:";
					std::cout << "\nmediumRed = " << mediumRed <<
								 "\nmediumGreen = " << mediumGreen <<
								 "\nmediumBlue = " << mediumBlue << std::endl;

					std::cout << "\nPo lewej: ";
					pixel = image.pixel(col - 1, row);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					std::cout << "\nPo prawej: ";
					pixel = image.pixel(col + 1, row);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					std::cout << "\nZ gory: ";
					pixel = image.pixel(col, row - 1);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					std::cout << "\nZ dolu: ";
					pixel = image.pixel(col, row + 1);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					std::cout << "\nZ dolu: ";
					pixel = image.pixel(col, row + 1);
					color.setRgb(pixel);
					std::cout << "red = " << color.red();
					std::cout << " green = " << color.green();
					std::cout << " blue = " << color.blue();

					return false;
				}

				if (red >= mediumRed - accDif && red <= mediumRed + accDif &&
					green >= mediumGreen - accDif && green <= mediumGreen + accDif &&
					blue >= mediumBlue - accDif && blue <= mediumBlue + accDif
					)
				{
					//std::cout << "\nPiksel jest ok !!!\n";
				}
				else
				{
					//std::cout << "\nPiksel sie nie zgadza !!!\n";
					//std::cout << "\nPiksel: " << row << " " << col << " wartosci: " << red << " " << green << " " << blue <<
					//			" srednie wartosci to: " << mediumRed << " " << mediumGreen << " " << mediumBlue << std::endl;

					blockResults[colInd][rowInd] = false;
					checkAdjacentSquares(col, row, colInd, rowInd);
				}

		}
	}

	for (int col = 0; col < colIndex; col++)
	{
		delete blockResults[col];
	}
	delete blockResults;

	return false;
}
*/
void DrawingOperations::checkHistogramGrey(std::vector<int> savedGreyTones)
{
	int heightSecured, heightActual;
	int greatestSecured, greatestActual;
	int xPos, yPos;

	histogramGComparison = QImage(histogramBase);

	QPainter painter(&histogramGComparison);

	greatestSecured = savedGreyTones[0];
	greatestActual = greyTones[0];

	// TODO zrobic by tworzyl sie histogram porownujacy obie wartosci

	for (int tone = 1; tone < 153; tone++)
	{
		if (greatestSecured < savedGreyTones[tone])
		{
			greatestSecured = savedGreyTones[tone];
		}
		if (greatestActual < greyTones[tone])
		{
			greatestActual = greyTones[tone];
		}
	}

	for (int tone = 0; tone < 153; tone++)
	{
		xPos = 50 + tone * 3;

		painter.setBrush(Qt::green);
		painter.setPen(Qt::green);

		heightSecured = ((float)savedGreyTones[tone] / greatestSecured ) * 400;
		heightActual = ((float)greyTones[tone] / greatestActual ) * 400;

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

void DrawingOperations::checkImageSecurity(std::vector<int>* savedValues)
{
	std::vector<int> rgbs;
	rgbs = savedValues[0];

	checkPixel(0, 0, rgbs[0], rgbs[1], rgbs[2]);
	checkPixel(image.width() - 1, 0, rgbs[3], rgbs[4], rgbs[5]);
	checkPixel(0, image.height() - 1, rgbs[6], rgbs[7], rgbs[8]);
	checkPixel(image.width() - 1, image.height() - 1, rgbs[9], rgbs[10], rgbs[11]);

	checkHistogramGrey(savedValues[1]);

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
