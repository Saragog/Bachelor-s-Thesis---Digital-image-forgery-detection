/*
 * DrawingOperations.cpp
 *
 *  Created on: 11 wrz 2016
 *      Author: andrzej
 */

#include "DrawingOperations.h"
#include <QImageReader>

#include <QImageWriter>
#include <QFile>
#include <QColor>

DrawingOperations::DrawingOperations()
{
    QImageReader reader(QString::fromAscii("./ProgramImages/blank.jpg"));
    histogramBase = reader.read();
    actualFileName = "";

    accDif = 3;
    signLength = 38;
    signHeight = 34;
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
			if (col >= sizeFirstPixelX && row >= rows - 2) continue;
			if (col >= columns - 2 && row >= sizeFirstPixelY) continue;
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
		}
	}

	return;
}

bool DrawingOperations::saveImage(QString path) // funkcja zapisuje obraz do pliku o podanej sciezce
{
	try
	{
		QFile file(path);
		file.open(QIODevice::WriteOnly);

		sizeFirstPixelX = columns - 7 - 6 * 15;
		sizeFirstPixelY = rows -7 - 6 * 15;

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
		rotation = ROTATION0;
	}
	else if (checkRotation90())
	{
		partialRaport.first = true;
		partialRaport.second = "Wykryto rotacje 90 stopni";
		raportImage.push_back(partialRaport);
		rotation = ROTATION90;
	}
	else if (checkRotation180())
	{
		partialRaport.first = true;
		partialRaport.second = "Wykryto rotacje 180 stopni";
		raportImage.push_back(partialRaport);
		rotation = ROTATION180;
	}
	else if (checkRotation270())
	{
		partialRaport.first = true;
		partialRaport.second = "Wykryto rotacje 270 stopni";
		raportImage.push_back(partialRaport);
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
		partialRaport.second = "Histogram stworzony na podstawie danych zapisanych na znaku";
		raportImage.push_back(partialRaport);
		checkHistogramGrey(readFromSign());
	}

	checkChanges();

	return;
}

QImage DrawingOperations::getCheckedImage() const
{
	return checkedImage;
}

std::vector<std::pair<bool, std::string> > DrawingOperations::getRaportImage() const
{
	return raportImage;
}
