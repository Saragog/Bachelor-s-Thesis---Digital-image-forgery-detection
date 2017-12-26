/*
 * SignOperations.cpp
 *
 *  Created on: 16 lis 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Komentarz:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Ten plik zawiera implementację funkcji klasy DrawingOperations odpowiedzialnych za operacje
 *  		na znaku zabezpieczającym i zabezpieczeniach wielkości obrazu.
 */

#include "DrawingOperations.h"

#include <QColor>
#include <string>
#include <sstream>

// Funkcja drawSign odpowiada za rysowanie znaku zabezpieczającego
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
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(25, 25, 25));
						break;
					}
					case 2:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(51, 51, 51));
						break;
					}
					case 3:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(76, 76, 76));
						break;
					}
					case 4:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(102, 102, 102));
						break;
					}
					case 5:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(153, 153, 153));
						break;
					}
					case 6:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(178, 178, 178));
						break;
					}
					case 7:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(204, 204, 204));
						break;
					}
					case 8:
					{
						securedImage.setPixel(signBeginningX+1 + step * 7 + part, y, qRgb(229, 229, 229));
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

	drawOriginalSize(0); // rysowanie zabezpieczenia oryginalnej wielkości (os X)
	drawOriginalSize(1); // rysowanie zabezpieczenia oryginalnej wielkości (os Y)
	drawEdge();			 // zamalowanie krawędzi dolnej prawej na biało (255,255,255)
	return;
}

// Funkcja convertBinaryToInt zamienia ciąg wartości 0 1 na liczbę całkowitą
int DrawingOperations::convertBinaryToInt(int* binary)
{
	int result = 0;
	for (int bitN = 0; bitN < 15; bitN++) // maksymalnie 15 bitów
	{
		if (binary[bitN] == 1)
		{
			result = result * 2 + 1;
		}
		else result *= 2;
	}
	return result;
}

// Funkcja convertIntToBinary zamienia liczbę całkowitą na ciąg bitów
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

// Fukcja drawEdge zamalowuje 5 pikseli będących blisko dolnego prawego rogu na biało
// dzięki temu możliwe jest wykrycie obrotu obrazu
void DrawingOperations::drawEdge()
{
	securedImage.setPixel(columns-4, rows-2, qRgb(255, 255, 255));
	securedImage.setPixel(columns-3, rows-2, qRgb(255, 255, 255));
	securedImage.setPixel(columns-2, rows-2, qRgb(255, 255, 255));
	securedImage.setPixel(columns-2, rows-3, qRgb(255, 255, 255));
	securedImage.setPixel(columns-2, rows-4, qRgb(255, 255, 255));
	return;
}

// Funkcja drawOriginalSize rysuje zabezpieczenie oryginalnej wielkości obrazu zależnie od trybu
// tryb 0 oznacza zabezpieczenie osi X
// tryb 1 oznacza zabezpieczenie osi Y
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
				securedImage.setPixel(sizeFirstPixelX + bitNumber*6 + step, rows-2, qRgb(128, 128, 128));// separator
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
					securedImage.setPixel(sizeFirstPixelX + bitNumber*6 + step + 3, rows-2, qRgb(255, 255, 255)); // kolor biały oznacza 1
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
					securedImage.setPixel(sizeFirstPixelX + bitNumber*6 + step + 3, rows-2, qRgb(0, 0, 0)); // kolor czarny oznacza 0
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

	for (int step = 0; step < 3; step++) // ostatni separator
	{
		if (mode == 0)
		{
			securedImage.setPixel(sizeFirstPixelX + 90 + step, rows-2, qRgb(128, 128, 128)); // separator
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

// Funkcja readingSizeStep zależnie od rotacji wykonuje krok czytania pikseli w zależności od trybu (czy ma czytać poziom czy pion)
std::pair<int,int> DrawingOperations::readingSizeStep(int mode, int actualX, int actualY)
{
	std::pair<int,int> newPosition;
	// zależnie od rotacji
	if (rotation == ROTATION0)
	{
		if (mode == 0)
		{
			newPosition.first = actualX-1;
			newPosition.second = actualY;
		}
		else
		{
			newPosition.first = actualX;
			newPosition.second = actualY-1;
		}
	}
	else if (rotation == ROTATION90)
	{
		if (mode == 0)
		{
			newPosition.first = actualX;
			newPosition.second = actualY-1;
		}
		else
		{
			newPosition.first = actualX+1;
			newPosition.second = actualY;
		}
	}
	else if (rotation == ROTATION180)
	{
		if (mode == 0)
		{
			newPosition.first = actualX+1;
			newPosition.second = actualY;
		}
		else
		{
			newPosition.first = actualX;
			newPosition.second = actualY+1;
		}
	}
	else
	{
		if (mode == 0)
		{
			newPosition.first = actualX;
			newPosition.second = actualY+1;
		}
		else
		{
			newPosition.first = actualX-1;
			newPosition.second = actualY;
		}
	}
	return newPosition;
}

// Funkcja readOriginalSize służy do czytania oryginalnej wielkości obrazu z zabezpieczenia
void DrawingOperations::readOriginalSize(int mode)
{
	int readBinaryNumber[15];
	int actualX, actualY;
	int bitIndex = 14;
	int value;
	bool bitSet;
	bool separatorFound;
	std::pair<int,int> newPosition;
	std::ostringstream ostring;
	// inicjowanie tablicy bitów
	for (int bit = 0; bit < 15; bit++) readBinaryNumber[bit] = 0;
	// zależnie od rotacji początkowa pozycja pikseli z których mamy czytać
	if (rotation == ROTATION0)
	{
		if (mode == 0)
		{
			actualX = columns-3;
			actualY = rows-1;
		}
		else
		{
			actualX = columns-1;
			actualY = rows-3;
		}
	}
	else if (rotation == ROTATION90)
	{
		if (mode == 0)
		{
			actualX = 0;
			actualY = rows-3;
		}
		else
		{
			actualX = 2;
			actualY = rows-1;
		}
	}
	else if (rotation == ROTATION180)
	{
		if (mode == 0)
		{
			actualX = 2;
			actualY = 0;
		}
		else
		{
			actualX = 0;
			actualY = 2;
		}
	}
	else
	{
		if (mode == 0)
		{
			actualX = columns-1;
			actualY = 2;
		}
		else
		{
			actualX = columns-3;
			actualY = 0;
		}
	}
	separatorFound = false;
	while (actualX >= 0 && actualX <= columns-1 && actualY >= 0 && actualY <= rows-1 && readPixelDigit(actualX,actualY)==9) // dopuki caly czas białe piksele to idziemy w lewo
	{
		newPosition = readingSizeStep(mode, actualX, actualY);
		actualX = newPosition.first;
		actualY = newPosition.second;
	}
	while (bitIndex >= 0)
	{
		bitSet = false;
		while (actualX >= 0 && actualX <= columns-1 && actualY >= 0 && actualY <= rows-1) // przechodzenie przez separator
		{
			value = readPixelDigit(actualX,actualY);
			if (value == 11) separatorFound = true;
			if (value == 9 || value == 0) break;
			newPosition = readingSizeStep(mode, actualX, actualY);
			actualX = newPosition.first;
			actualY = newPosition.second;
		}
		while (actualX >= 0 && actualX <= columns-1 && actualY >= 0 && actualY <= rows-1) // czytanie pikseli i wstawianie odpowiedniej wartości bitu
		{
			value = readPixelDigit(actualX,actualY);
			if (value == 11)
			{
				// znaleziono separator
				break;
			}
			if (!bitSet && value == 0) // czarny
			{
				readBinaryNumber[bitIndex] = 0;
				bitIndex--;
				bitSet = true;
			}
			else if (!bitSet && value == 9) // bialy
			{
				readBinaryNumber[bitIndex] = 1;
				bitIndex--;
				bitSet = true;
			}
			newPosition = readingSizeStep(mode, actualX, actualY); // krok kolejny czytania
			actualX = newPosition.first;
			actualY = newPosition.second;
		}
		if (!bitSet || !separatorFound) // Nie potrafiono zidentyfikowac wartosci bitu
		{
			partialRaport.first = false;
			if (mode == 0) partialRaport.second = codec->toUnicode("Błąd w czytaniu oryginalnej wielkości X");
			else partialRaport.second = codec->toUnicode("Błąd w czytaniu oryginalnej wielkości Y");
			raportImage.push_back(partialRaport);
			return;
		}
		if (value != 11)
		{
			separatorFound = false;
		}
	}
	// tworzenie części raportu
	value = convertBinaryToInt(readBinaryNumber);
	partialRaport.first = true;
	if (mode == 0) partialRaport.second = codec->toUnicode("Oryginalna wielkość obrazu X wczytana z powodzeniem: ") + QString::number(value);
	else partialRaport.second = codec->toUnicode("Oryginalna wielkość obrazu Y wczytana z powodzeniem: ") + QString::number(value);
	raportImage.push_back(partialRaport);

	if (mode == 0 && value == columns)
	{
		partialRaport.first = true;
		partialRaport.second = codec->toUnicode("Zabezpieczenie oryginalnej wielkości X zgodne z aktualną wielkością obrazu wynoszącą: ") + QString::number(columns);
	}
	else if (mode == 1 && value == rows)
	{
		partialRaport.first = true;
		partialRaport.second = codec->toUnicode("Zabezpieczenie oryginalnej wielkości Y zgodne z aktualna wielkością obrazu wynoszącą: ") + QString::number(rows);
	}
	else if (mode == 0)
	{
		partialRaport.first = false;
		partialRaport.second = codec->toUnicode("Zabezpieczenie oryginalnej wielkości X niezgodne z aktualną wielkoscią obrazu wynoszącą: ") + QString::number(columns);
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = codec->toUnicode("Zabezpieczenie oryginalnej wielkości Y niezgodne z aktualna wielkością obrazu wynoszącą: ") + QString::number(rows);
	}
	raportImage.push_back(partialRaport);

	return;
}

// Funkcja readPixelDigit pobiera położenie piksela i zwraca liczbę odpowiadającą pikselowi
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
	else if (red >= 25 - digitColorRange && red <= 25 + digitColorRange &&
		green >= 25 - digitColorRange && green <= 25+ digitColorRange &&
		blue >= 25 - digitColorRange && blue <= 25 + digitColorRange)
	{
		// 25 25 25 odcien szarosci - 1
		digit = 1;
	}
	else if (red >= 51 - digitColorRange && red <= 51 + digitColorRange &&
			green >= 51 - digitColorRange && green <= 51 + digitColorRange &&
			blue >= 51 - digitColorRange && blue <= 51 + digitColorRange)
	{
		// 51 51 51 odcien szarosci - 2
		digit = 2;
	}
	else if (red >= 76 - digitColorRange && red <= 76 + digitColorRange &&
			green >= 76 - digitColorRange && green <= 76 + digitColorRange &&
			blue >= 76 - digitColorRange && blue <= 76 + digitColorRange)
	{
		// 76 76 76 odcien szarosci - 3
		digit = 3;
	}
	else if (red >= 102 - digitColorRange && red <= 102 + digitColorRange &&
			green >= 102 - digitColorRange && green <= 102 + digitColorRange &&
			blue >= 102 - digitColorRange && blue <= 102 + digitColorRange)
	{
		// 102 102 102 odcien szarosci - 4
		digit = 4;
	}
	else if (red >= 153 - digitColorRange && red <= 153 + digitColorRange &&
			green >= 153 - digitColorRange && green <= 153 + digitColorRange &&
			blue >= 153 - digitColorRange && blue <= 153 + digitColorRange)
	{
		// 153 153 153 odcien szarosci - 5
		digit = 5;
	}
	else if (red >= 178 - digitColorRange && red <= 178 + digitColorRange &&
			green >= 178 - digitColorRange && green <= 178 + digitColorRange &&
			blue >= 178 - digitColorRange && blue <= 178 + digitColorRange)
	{
		// 204 204 204 odcien szarosci - 6
		digit = 6;
	}
	else if (red >= 204 - digitColorRange && red <= 204 + digitColorRange &&
			green >= 204 - digitColorRange && green <= 204 + digitColorRange &&
			blue >= 204 - digitColorRange && blue <= 204 + digitColorRange)
	{
		// 204 204 204 odcien szarosci - 7
		digit = 7;
	}
	else if (red >= 229 - digitColorRange && red <= 229 + digitColorRange &&
			green >= 229 - digitColorRange && green <= 229 + digitColorRange &&
			blue >= 229 - digitColorRange && blue <= 229 + digitColorRange)
	{
		// 229 229 229 odcien szarosci - 8
		digit = 8;
	}
	else if (red >= 255 - digitColorRange &&
			green >= 255 - digitColorRange &&
			blue >= 255 - digitColorRange)
	{
		// 255 255 255 czyli bialy - 9
		digit = 9;
	}
	else if (red >= 127 - sizeSeparatorColorRange && red <= 127 + sizeSeparatorColorRange &&
			green >= 127 - sizeSeparatorColorRange && green <= 127 + sizeSeparatorColorRange &&
			blue >= 127 - sizeSeparatorColorRange && blue <= 127 + sizeSeparatorColorRange)
	{
		// 127 127 127 czyli separator bitow zabezpieczenia wielkosci obrazu
		digit = 11;
	}
	else
	{
		// piksel wykracza barwa poza nasz zakres !!!
		digit = 10;
	}
	return digit;
}

// Funkcja convertDigitsToInt zamienia ciąg bitów na liczbę całkowitą
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

// Funkcja readFromSign czyta liczebność 153 grup sum barw RGB ze znaku zabezpieczającego
std::vector<int> DrawingOperations::readFromSign()
{
	std::vector<int> readGreyTones; // liczby pikseli w odcieniu grupy odczytane z obrazka ( z zabezpieczenia w postaci bloku )
	int readDigit;
	int digits[7];
	int temp;
	int infoBeginningX, infoBeginningY;
	std::ostringstream ostring;

	int wrgPxlCntr = 0; // wrong pixel counter - ile pikseli ma niewlasciwa wartosc w znaku

	int actualRow, toneIndex;

	toneIndex = 0;

	switch (rotation) // zależnie od rotacji inaczej rozpoczynamy czytanie
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
	// pokolei czytamy liczby dla 153 grup sum barw RGB
	while (toneIndex < 153)
	{
		for (int step = 0; step < 5 && toneIndex < 153; step++, toneIndex++)
		{
			for (int numberPart = 0; numberPart < 7; numberPart++)
			{
				switch (rotation) // zależnie od rotacji czytamy kolejne piksele
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
				// jeśli piksel nie reprezentuje liczby w zakresie 0-9 to oznacza, że natrafiliśmy na błędny piksel
				if (readDigit == 10 || readDigit == 11)
				{
					// blad jest w znaku, piksel ma wartosc niewazna
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
		// krok zależnie od rotacji
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
	// wypisanie części raportu o liczebności błędnych pikseli w znaku zabezpieczającym
	if (!wrgPxlCntr)
	{
		partialRaport.first = true;
		partialRaport.second = codec->toUnicode("Informacje o histogramie zapisane na znaku odczytane bez problemów");
	}
	else
	{
		partialRaport.first = false;
		partialRaport.second = codec->toUnicode("Informacje o histogramie zapisane na znaku odczytane z problemami - ilość błędów: ") +  QString::number(wrgPxlCntr);
	}

	raportImage.push_back(partialRaport);

	return readGreyTones;
}
