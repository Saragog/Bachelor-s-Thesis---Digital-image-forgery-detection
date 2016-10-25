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

// ___
#include <QImageWriter>
// ___

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

    // ______
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
			if ((col + row) % 3 != 0) continue;

			if (col > columns - 40 && row > rows - 40) continue;

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
			if (col < columns - 1 && ( col + 1 < columns - signLength || row < rows - signLength))
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
			if (row < rows - 1 && ( col + 1 < columns - signHeight || row < rows - signHeight))
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

			// _____________________

/*			int red, green, blue;

			if (col >= 1300 && col < 1400 && row >= 1100 && row < 1200)
			{
				std::cout << "\nDla piksela o pozycji: " << col << " " << row << std::endl;
				std::cout << "Srednia wynosi: " << mediumRed << " " <<mediumGreen << " " << mediumBlue << std::endl;

				if (row - 1 >= 0)
				{
					pixel = image.pixel(col, row-1);
					color.setRgb(pixel);

					red = color.red();
					green = color.green();
					blue = color.blue();
					std::cout << "Piksel na gorze: " << red << " " << green << " " << blue << std::endl;
				}

				if (col - 1 >= 0)
				{
					pixel = image.pixel(col-1, row);
					color.setRgb(pixel);

					red = color.red();
					green = color.green();
					blue = color.blue();
					std::cout << "Piksel po lewej: " << red << " " << green << " " << blue << std::endl;
				}

				if (col + 1 < columns)
				{
					pixel = image.pixel(col+1, row);
					color.setRgb(pixel);

					red = color.red();
					green = color.green();
					blue = color.blue();
					std::cout << "Piksel po prawej: " << red << " " << green << " " << blue << std::endl;
				}

				if (row + 1 < rows)
				{
					pixel = image.pixel(col, row+1);
					color.setRgb(pixel);

					red = color.red();
					green = color.green();
					blue = color.blue();
					std::cout << "Piksel na dole: " << red << " " << green << " " << blue << std::endl;
				}
			}
*/
			// _____________________

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
	// ____
	// ____

	try
	{
		QFile file(path);
		file.open(QIODevice::WriteOnly);

		secureImage();

		drawSign(); // Funkcja do tworzenia znaczka zabezpieczajacego przed utrata danych exif

		prepareHistogramsData();

		securedImage.save(&file, "JPG", 100);

		file.close();

		// ___

		QFile f2("/home/andrzej/workspace/Inzynierka/Zdjecia_Edytowane/x.jpg");
		QImageWriter writer(&f2, "JPG");
		writer.setCompression(0);
		writer.setQuality(100);
		writer.write(securedImage);

	}
	catch (std::exception& e)
	{
		return false;
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

			std::cout << "\nDla tone: " << tone + 1<< " CalyGreyTone = " << actualTone;

			for (int part = 0, divisor = 1000000; part < 7; part++, divisor/=10)
			{
				//std::cout << "\nZamalowuje 1 piksel znaczka: " << signBeginningX+1 + step * 7 + part << " " << y;

				digit = actualTone / divisor;
				actualTone = actualTone - digit * divisor;
				std::cout << std::endl << digit;
				//std::cout << std::endl << actualTone;

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
			if (c >= signBeginningX && r >= signBeginningY) continue;

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

	if (col < signBeginningX || row < signBeginningY) checkAdjacentRight(col, row, red, green, blue);
	if (col < signBeginningX || row < signBeginningY) checkAdjacentDown(col, row, red, green, blue);

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

bool DrawingOperations::checkChanges()
{
	signBeginningX = columns - signLength;
	signBeginningY = rows - signHeight;

	//std::cout << std::endl << signBeginningX << " " << signBeginningY << std::endl;

	QRgb pixel;
	QColor color;
	int sumRed, sumGreen, sumBlue;
	int mediumRed, mediumGreen, mediumBlue;
	int adjacentPixels;
	int red, green, blue;

	int biggestRed, biggestGreen, biggestBlue;

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

	for (int c = signBeginningX; c < columns; c++)	// zaznaczenie znaczka ze jest juz wykorzystane by nie sprawdzalo w srodku
	{
		for (int r = signBeginningY; r < rows; r++)
		{
			pixelResults[c][r] = false;
		}
	}

	for (int col = 0; col < columns; col++)//0; col < columns; col++)
	{
		for (int row = col % 3; row < rows; row+=3)
		{

			if ((col + row) % 3 != 0) continue;

			biggestRed = 0;
			biggestGreen = 0;
			biggestBlue = 0;

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

					if (color.red() > biggestRed) biggestRed = color.red();
					if (color.green() > biggestGreen) biggestGreen = color.green();
					if (color.blue() > biggestBlue) biggestBlue = color.blue();
				}
				if (col < columns - 1 && (col + 1 < signBeginningX || row < signBeginningY))
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
				if (row >= 1)
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
				if (row < rows - 1 && (col < signBeginningX || row + 1 < signBeginningY))
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

					// zrobic sprawdzenie drogiej szansy :D
					// polega na tym ze wartosc odczytana piksela nie moze byc duzo rozniaca sie od najwyzszej z wartosci sasiadow
					// powiedzmy ze kazda z barw do 15 wieksza moze byc ale nie wiecej


					// ______________
/*
					if (row - 1 >= 0)
					{
						pixel = image.pixel(col, row-1);
						color.setRgb(pixel);

						red = color.red();
						green = color.green();
						blue = color.blue();
						std::cout << "Piksel na gorze: " << red << " " << green << " " << blue << std::endl;
					}

					if (col - 1 >= 0)
					{
						pixel = image.pixel(col-1, row);
						color.setRgb(pixel);

						red = color.red();
						green = color.green();
						blue = color.blue();
						std::cout << "Piksel po lewej: " << red << " " << green << " " << blue << std::endl;
					}

					if (col + 1 < columns && (col + 1 < signBeginningX || row < signBeginningY))
					{
						pixel = image.pixel(col+1, row);
						color.setRgb(pixel);

						red = color.red();
						green = color.green();
						blue = color.blue();
						std::cout << "Piksel po prawej: " << red << " " << green << " " << blue << std::endl;
					}

					if (row + 1 < rows && (col < signBeginningX || row + 1 < signBeginningY))
					{
						pixel = image.pixel(col, row+1);
						color.setRgb(pixel);

						red = color.red();
						green = color.green();
						blue = color.blue();
						std::cout << "Piksel na dole: " << red << " " << green << " " << blue << std::endl;
					}
*/


					pixelResults[col][row] = false;
					lookForSimilarPixels(col, row);

					// ______________


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

	std::cout << "\nKolor rgb piksela: " << red << " " << green << " " << blue << std::endl;

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
		// 50 50 50 czyli jakis taki szary numero uno - 1
		digit = 1;
	}
	else if (red >= 56 - digitColorRange && red <= 56 + digitColorRange &&
			green >= 56 - digitColorRange && green <= 56 + digitColorRange &&
			blue >= 56 - digitColorRange && blue <= 56 + digitColorRange)
	{
		// 100 100 100 czyli jakis taki szary numero due - 2
		digit = 2;
	}
	else if (red >= 84 - digitColorRange && red <= 84 + digitColorRange &&
			green >= 84 - digitColorRange && green <= 84 + digitColorRange &&
			blue >= 84 - digitColorRange && blue <= 84 + digitColorRange)
	{
		// 50 100 150 czyli fiolecik jakis - 3
		digit = 3;
	}
	else if (red >= 112 - digitColorRange && red <= 112 + digitColorRange &&
			green >= 112 - digitColorRange && green <= 112 + digitColorRange &&
			blue >= 112 - digitColorRange && blue <= 112 + digitColorRange)
	{
		// 150 150 150 czyli odcien szarosci numero tre - 4
		digit = 4;
	}
	else if (red >= 140 - digitColorRange && red <= 140 + digitColorRange &&
			green >= 140 - digitColorRange && green <= 140 + digitColorRange &&
			blue >= 140 - digitColorRange && blue <= 140 + digitColorRange)
	{
		// 200 200 200 czyli odcien szarosci numero quattro - 5
		digit = 5;
	}
	else if (red >= 168 - digitColorRange && red <= 168 + digitColorRange &&
			green >= 168 - digitColorRange && green <= 168 + digitColorRange &&
			blue >= 168 - digitColorRange && blue <= 168 + digitColorRange)
	{
		// 255 255 0 czyli zolty - 6
		digit = 6;
	}
	else if (red >= 196 - digitColorRange && red <= 196 + digitColorRange &&
			green >= 196 - digitColorRange && green <= 196 + digitColorRange &&
			blue >= 196 - digitColorRange && blue <= 196 + digitColorRange)
	{
		// 255 0 255 czyli ciemny rozowy - 7
		digit = 7;
	}
	else if (red >= 224 - digitColorRange && red <= 224 + digitColorRange &&
			green >= 224 - digitColorRange && green <= 224 + digitColorRange &&
			blue >= 224 - digitColorRange && blue <= 224 + digitColorRange)
	{
		// 0 255 255 czyli cyan lekki niebieski - 8
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

void DrawingOperations::checkImageSecurity(std::vector<int> savedGreyTones)
{
	std::vector<int> readGreyTones; // ilosci pikseli w odcieni szarosci odczytane z obrazka ( z zabezpieczenia w postaci bloku )
	bool rotation; // bool ze udalo sie wykryc rotacje i zapisywana jest ona w jakiejs zmiennej ???
	int infoBeginningX, infoBeginningY;
	short rotationResult;
	int readDigit;
	int digits[7];
	int temp;

	// TODO tutaj pozniej bedzie sprawdzanie obrotu i wtedy bedzie wiadomo jak sprawdzac znak i zaleznosci pikselowe

	rotation = 1;	// TODO funkcja wykrywajaca jaki jest obrot
	// zaleznie od wyniku to poczatek
	signBeginningX = columns - signLength;
	signBeginningY = rows - signHeight;
	rotationResult = 0;


/*
	if (savedGreyTones.size() == 153)				// sprawdza wielkosc wektora
	{
		checkHistogramGrey(savedGreyTones);
	}
	else */if (rotation)
	{
		// TODO zrobic wykrywanie obrotu i czytanie w tym miejscu jakies funkcje !!! :D

		// zaleznie od rotation
		infoBeginningX = signBeginningX+1;
		infoBeginningY = signBeginningY+1;



		for (int actualRow = infoBeginningY, toneIndex = 0; toneIndex < 153; actualRow++)
		{
			//std::cout << "\nSzereg: " << actualRow << " toneIndex: " << toneIndex << std::endl;
			for (int step = 0; step < 5 && toneIndex < 153; step++, toneIndex++)
			{
				for (int numberPart = 0; numberPart < 7; numberPart++)
				{
					readDigit = readPixelDigit(infoBeginningX + step * 7 + numberPart, actualRow);

					if (readDigit == 10)
					{
						//std::cout << "\nBlad czytania wartosci piksela znaku - nieznany kolor !!!\n";
						// blad jest w znaku piksel ma wartosc niewazna !!!
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

		}
		checkHistogramGrey(readGreyTones);
	}
	/*else if (1)
	{

	}
	else if (1)
	{

	}
	else if (1)
	{

	}*/
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
