#include "DrawingOperations.h"

#include <QColor>
#include <string>
#include <sstream>

void DrawingOperations::drawSign()
{
	int digit;
	int actualTone;

	signBeginningX = columns - signLength;
	signBeginningY = rows - signHeight;

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
	for (int y = signBeginningY+1, tone = 0; y < rows-2; y++)
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

	drawOriginalSize(0);
	drawOriginalSize(1);
	drawEdge();

	return;
}

int* DrawingOperations::convertIntToBinary(int number)
{
	static int binary[15];
	int divider = 16384;
	int rest = number;

	for (int x = 0; x < 15; x++, divider/=2)
	{
		if (rest / divider == 1)
		{
			binary[x] = 1;
			rest -= divider;
		}
		else
		{
			binary[x] = 0;
		}
	}

	return binary;
}

void DrawingOperations::drawEdge() 	// zamalowuje 5 pikseli bedacych blisko dolnego prawego rogu na bialo
									// dzieki czemu rog obrazu jest odporny na zmiejszenie obrazu maksymalnie 2 krotne
									// po takiej zmianie nadal możliwe jest wykrycie obrotu stara metoda
{
	securedImage.setPixel(columns-4, rows-2, qRgb(255, 255, 255));
	securedImage.setPixel(columns-3, rows-2, qRgb(255, 255, 255));
	securedImage.setPixel(columns-2, rows-2, qRgb(255, 255, 255));
	securedImage.setPixel(columns-2, rows-3, qRgb(255, 255, 255));
	securedImage.setPixel(columns-2, rows-4, qRgb(255, 255, 255));

	return;
}

void DrawingOperations::drawOriginalSize(short mode)
{
	int* binary;
	if (mode == 0) binary = convertIntToBinary(columns);
	else binary = convertIntToBinary(rows);

	for (int bitNumber = 0; bitNumber < 15; bitNumber++) // 1 bit i nastepnie 1 separator
	{
		for (int step = 0; step < 3; step++)
		{
			if (mode == 0)
			{
				securedImage.setPixel(sizeFirstPixelX + bitNumber*6 + step, rows-2, qRgb(128, 128, 128));
				securedImage.setPixel(sizeFirstPixelX + bitNumber*6 + step, rows-1, qRgb(128, 128, 128));
			}
			else
			{
				securedImage.setPixel(columns-2, sizeFirstPixelY + bitNumber*6 + step, qRgb(128, 128, 128));
				securedImage.setPixel(columns-1, sizeFirstPixelY + bitNumber*6 + step, qRgb(128, 128, 128));
			}
		}
		for (int step = 0; step < 3; step++)
		{
			if (binary[bitNumber])
			{
				if (mode == 0)
				{
					securedImage.setPixel(sizeFirstPixelX + bitNumber*6 + step + 3, rows-2, qRgb(255, 255, 255));
					securedImage.setPixel(sizeFirstPixelX + bitNumber*6 + step + 3, rows-1, qRgb(255, 255, 255));
				}
				else
				{
					securedImage.setPixel(columns-2, sizeFirstPixelY + bitNumber*6 + step + 3, qRgb(255, 255, 255));
					securedImage.setPixel(columns-1, sizeFirstPixelY + bitNumber*6 + step + 3, qRgb(255, 255, 255));
				}
			}
			else
			{
				if (mode == 0)
				{
					securedImage.setPixel(sizeFirstPixelX + bitNumber*6 + step + 3, rows-2, qRgb(0, 0, 0));
					securedImage.setPixel(sizeFirstPixelX + bitNumber*6 + step + 3, rows-1, qRgb(0, 0, 0));
				}
				else
				{
					securedImage.setPixel(columns-2, sizeFirstPixelY + bitNumber*6 + step + 3, qRgb(0, 0, 0));
					securedImage.setPixel(columns-1, sizeFirstPixelY + bitNumber*6 + step + 3, qRgb(0, 0, 0));
				}
			}
		}

	}

	for (int step = 0; step < 3; step++) // pierwszy separator
	{
		if (mode == 0)
		{
			securedImage.setPixel(sizeFirstPixelX + 90 + step, rows-2, qRgb(128, 128, 128));
			securedImage.setPixel(sizeFirstPixelX + 90 + step, rows-1, qRgb(128, 128, 128));
		}
		else
		{
			securedImage.setPixel(columns-2, sizeFirstPixelY + 90 + step, qRgb(128, 128, 128));
			securedImage.setPixel(columns-1, sizeFirstPixelY + 90 + step, qRgb(128, 128, 128));
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
	int infoBeginningX, infoBeginningY;
	std::ostringstream ostring;

	int wrgPxlCntr = 0; // wrong pixel counter - ile pikseli ma niewlasciwa wartosc w znaku

	int actualRow, toneIndex;

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

	//std::cout << "wielkosc obrazu: " << columns << " " << rows << std::endl;
	//std::cout << "poczatek znaku: " << signBeginningX << " " << signBeginningY << std::endl;
	//std::cout << "poczatek informacji: " << infoBeginningX << " " << infoBeginningY << std::endl;

	while (toneIndex < 153)
	{
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
					// blad jest w znaku, piksel ma wartosc niewazna !!!
					digits[numberPart] = 0;
					wrgPxlCntr++;
				}
				else
				{
					digits[numberPart] = readDigit;
				}
			}
			// Funkcja dajaca liczbe na podstawie 7 cyfr danych
			temp = convertDigitsToInt(digits);
			readGreyTones.push_back(temp);
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

	if (!wrgPxlCntr)
	{
		partialRaport.first = true;
		partialRaport.second = "Informacje o histogramie zapisane na znaku odczytane bez problemow";
	}
	else
	{
		std::cout << "Ilosc bledow: " << wrgPxlCntr << std::endl;
		partialRaport.first = false;
		ostring << wrgPxlCntr;
		partialRaport.second = "Informacje o histogramie zapisane na znaku odczytane z problemami - ilosc bledow: " +  ostring.str();
		std::cout << partialRaport.second << std::endl;
	}

	raportImage.push_back(partialRaport);

	return readGreyTones;
}
