/*
 * DrawingOperations.cpp
 *
 *  Created on: 11 wrz 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Komentarz:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Ten plik zawiera implementację funkcji klasy DrawingOperations.
 */

#include "DrawingOperations.h"
#include <QImageReader>
#include <QImageWriter>
#include <QFile>
#include <QColor>

// Konstrunktor klasy DrawingOperations
DrawingOperations::DrawingOperations()
{
    QImageReader reader(QString::fromAscii("./ProgramImages/blank.jpg")); // czytanie pustego obrazka do późniejszej pracy nad nim
    histogramBase = reader.read();
    actualFileName = "";												  // aktualnie nie pracujemy nad żadnym plikiem
    codec = QTextCodec::codecForName("UTF-8");							  // kodowanie aby umożliwić znaki polskie
}

// Destrunktor klasy DrawingOperations
DrawingOperations::~DrawingOperations()
{
}

// Funkcja setImage przygotowuje obiekt klasy do pracy nad plikiem o podanej ścieżce
void DrawingOperations::setImage(std::string fileName)
{
	if (actualFileName != fileName) // sprawdzenie, czy wybrany plik to nie ten sam plik co był wybrany do tej pory
									// jeśli tak to nie ma nic do aktualizacji
									// jeśli nie to trzeba aktualizować
	{
		// czytanie obrazu
		actualFileName = fileName;
		QImageReader reader(QString::fromAscii(fileName.c_str()));
		image = reader.read();
		// czytanie wielkości obrazu
		columns = image.width();
		rows = image.height();
		rotation = ROTATION0;		// domyślna rotacja 0 stopni
		accDif = 3;					// domyslna wartosc akceptowalnego odchylenia RGB piksela od sredniej z sasiadow
		// ustawianie wartości dla czytania / zapisywania do / z znaku zabezpieczającego
		signBeginningX = columns - signLength;
		signBeginningY = rows - signHeight;
		sizeFirstPixelX = columns - 7 - 6 * 15;
		sizeFirstPixelY = rows -7 - 6 * 15;
		// przygotowanie histogramów obrazu
		prepareHistogramsData();
		drawHistograms();
	}
	return;
}

// Funkcja readPixelRGB czyta RGB piksela umieszczonego w podanym miejscu, zwraca tablicę 3 liczb całkowitych
int* DrawingOperations::readPixelRGB(int col, int row)
{
	static int result[3];
	QRgb pixel;
	QColor color;
	// czytanie piksela
	pixel = image.pixel(col, row);
	color.setRgb(pixel);
	// czytanie RGB piksela
	result[0] = color.red();
	result[1] = color.green();
	result[2] = color.blue();
	return result;
}

// Funkcja secureImage służy do zabezpieczania wybranego obrazu
void DrawingOperations::secureImage()
{
	int* pixelRGB;		// zmienna do czytania RGB piksela
	int sumRed, sumGreen, sumBlue;	//
	int mediumRed, mediumGreen, mediumBlue;
	int adjacentPixels;
	securedImage = image;

	// rotacja obrazu w zapisywaniu musi być równa 0
	rotation = ROTATION0;
	// wyliczanie położenia początkowych pikseli w znaku zabezpieczającym
	signBeginningX = columns - signLength;
	signBeginningY = rows - signHeight;

	// pętla uzależniająca piksele od RGB ich sąsiadów
	for (int col = 0; col < columns; col++)
	{
		for (int row = col % 3; row < rows; row+=3)
		{
			if (isPixelSecurityPart(col, row)) continue; // sprawdzenie czy piksel nie jest częścią elementów zabezpieczających
			sumRed = 0;					// suma wartości barw czerwonych w sąsiadach
			sumGreen = 0;				// suma wartości barw zielonych w sąsiadach
			sumBlue = 0;				// suma wartości barw niebieskich w sąsiadach
			adjacentPixels = 0;			// licznik sąsiadów - maksymalnie 8 sąsiadów
			// odpowiednio 8 sąsiadów
			if (col >= 1) 									// pierwszy z lewej
			{
				pixelRGB = readPixelRGB(col-1, row);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1];	sumBlue += pixelRGB[2];
			}
			if (col >= 2)									// drugi z lewej
			{
				pixelRGB = readPixelRGB(col-2, row);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (col < columns - 1 && !isPixelSecurityPart(col+1, row)) // pierwszy z prawej
			{
				pixelRGB = readPixelRGB(col+1, row);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (col < columns - 2 && !isPixelSecurityPart(col+2, row)) // drugi z prawej
			{
				pixelRGB = readPixelRGB(col+2, row);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (row >= 1)									// pierwszy z góry
			{
				pixelRGB = readPixelRGB(col, row-1);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (row >= 2)									// drugi z góry
			{
				pixelRGB = readPixelRGB(col, row - 2);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (row < rows - 1 && !isPixelSecurityPart(col, row+1)) // pierwszy od dołu
			{
				pixelRGB = readPixelRGB(col, row + 1);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			if (row < rows - 2 && !isPixelSecurityPart(col, row+2))	// drugi od dołu
			{
				pixelRGB = readPixelRGB(col, row + 2);
				adjacentPixels++;
				sumRed += pixelRGB[0]; sumGreen += pixelRGB[1]; sumBlue += pixelRGB[2];
			}
			// teraz liczymy średnią RGB z sąsiadów
			mediumRed = sumRed / adjacentPixels;
			mediumGreen = sumGreen / adjacentPixels;
			mediumBlue = sumBlue / adjacentPixels;
			securedImage.setPixel(col, row, qRgb(mediumRed, mediumGreen, mediumBlue)); // ustalamy wyznaczone RGB dla piksela
		}
	}
	return;
}

// Funkcja saveImage zabezpiecza obraz i zapisuje go pod podaną ścieżką
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
		// operacja zapisywania nie udała się
		return false;
	}
	return true;
}

// Funkcja drawChange zamalowuje kwadraty 5x5 pikseli na obrazie wynikowym weryfikacji odpowiednio wokół podanego położenia piksela
void DrawingOperations::drawChange(int col, int row)
{
	QRgb pixel;
	QColor color;
	// pętla przechodząca po pikselach wokół zadanego położenia
	for (int c = col - 2; c < col + 3; c++)
	{
		for (int r = row - 2; r < row + 3; r++)
		{
			if (isPixelSecurityPart(c, r)) continue; // sprawdzenie czy piksel nie został już zamalowany
			// jeśli nie to można zamalować piksel
			if (c >= 0 && c < columns && r >= 0 && r < rows && pixelsChanged[c][r])
			{
				pixel = image.pixel(c, r);
				color.setRgb(pixel);
				// zamalowanie piksela na czerwony
				checkedImage.setPixel(c, r, qRgb(255, 0, 0));
				pixelsChanged[c][r] = false;
			}
		}
	}
	return;
}

// Funkcja lookForSimilarPixels sprawdza, czy wokół pikseli, których RGB nie pasuje do otoczenia, nie ma pikseli o tej samej barwie
// Dzięki temu obiekty o stałym kolorze, wprowadzone na obraz, powinny być całe zamalowane
void DrawingOperations::lookForSimilarPixels(int col, int row)
{
	drawChange(col, row); // zamalowanie wokół niepasującego piksela

	// sprawdzenie dla górnego piksela
	if (row - 1 >= 0 && pixelResults[col][row-1]) checkAdjacentPixels(col, row-1);

	// sprawdzenie dla lewego piksela
	if (col - 1 >= 0 && pixelResults[col-1][row]) checkAdjacentPixels(col-1, row);

	// sprawdzenie dla prawego piksela
	if (col + 1 < columns && pixelResults[col+1][row]) checkAdjacentPixels(col+1, row);

	// sprawdzenie dla dolnego piksela
	if (row + 1 < rows && pixelResults[col][row+1]) checkAdjacentPixels(col, row+1);
	return;
}

// Funkcja checkAdjacentPixels wywołuje sprawdzenie czy sąsiedzi zadanego piksela nie mają tego samego RGB
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
		case ROTATION0: // dla rotacji 0
		{
			checkAdjacentUp(col, row, red, green, blue);
			checkAdjacentLeft(col, row, red, green, blue);
			if (col < signBeginningX || row < signBeginningY) checkAdjacentRight(col, row, red, green, blue);
			if (col < signBeginningX || row < signBeginningY) checkAdjacentDown(col, row, red, green, blue);
			break;
		}
		case ROTATION90: // dla rotacji 90
		{
			checkAdjacentUp(col, row, red, green, blue);
			if (col > signBeginningX || row < signBeginningY) checkAdjacentLeft(col, row, red, green, blue);
			checkAdjacentRight(col, row, red, green, blue);
			if (col > signBeginningX || row < signBeginningY) checkAdjacentDown(col, row, red, green, blue);
			break;
		}
		case ROTATION180: // dla rotacji 180
		{
			if (col > signBeginningX || row > signBeginningY) checkAdjacentUp(col, row, red, green, blue);
			if (col > signBeginningX || row > signBeginningY) checkAdjacentLeft(col, row, red, green, blue);
			checkAdjacentRight(col, row, red, green, blue);
			checkAdjacentDown(col, row, red, green, blue);
			break;
		}
		case ROTATION270: // dla rotacji 270
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

// Funkcja checkAdjacentUp sprawdza piksele znajdujące się na górze
// Parametry to startowy piksel i jego barwa RGB (3 liczby całkowite)
void DrawingOperations::checkAdjacentUp(int col, int row, int red, int green, int blue)
{
	QRgb pixel;
	QColor checked;

	// sprawdzanie na górze
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
			// jeśli piksel ma sąsiada po lewej to idziemy w lewo i zaznaczamy wszystkie piksele tego samego koloru aż do napotkania innego koloru lub konca obrazu
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
			// jeśli piksel ma sąsiada po prawej to idziemy w prawo i zaznaczamy wszystkie piksele tego samego koloru aż do napotkania innego koloru lub konca obrazu
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
			{	// czytanie kolejnego piksela na górze
				pixel = image.pixel(actualCol, actualRow-1);
				checked.setRgb(pixel);
			}
		}
	}

	return;
}

// Funkcja checkAdjacentLeft sprawdza piksele znajdujące się po lewej
// Parametry to startowy piksel i jego barwa RGB (3 liczby całkowite)
void DrawingOperations::checkAdjacentLeft(int col, int row, int red, int green, int blue)
{
	QRgb pixel;
	QColor checked;

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
			// jeśli piksel ma sąsiada na górze to idziemy na górę i zaznaczamy wszystkie piksele tego samego koloru aż do napotkania innego koloru lub konca obrazu
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
			// jeśli piksel ma sąsiada na dole to idziemy na dół i zaznaczamy wszystkie piksele tego samego koloru aż do napotkania innego koloru lub konca obrazu
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
				// czytanie kolejnego piksela po lewej
				pixel = image.pixel(actualCol-1, actualRow);
				checked.setRgb(pixel);
			}
		}
	}

	return;
}

// Funkcja checkAdjacentRight sprawdza piksele znajdujące się po prawej
// Parametry to startowy piksel i jego barwa RGB (3 liczby całkowite)
void DrawingOperations::checkAdjacentRight(int col, int row, int red, int green, int blue)
{
	QRgb pixel;
	QColor checked;

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
			// jeśli piksel ma sąsiada na górze to idziemy na górę i zaznaczamy wszystkie piksele tego samego koloru aż do napotkania innego koloru lub konca obrazu
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
			// jeśli piksel ma sąsiada na dole to idziemy na dół i zaznaczamy wszystkie piksele tego samego koloru aż do napotkania innego koloru lub konca obrazu
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

// Funkcja checkAdjacentDown sprawdza piksele znajdujące się po na dole
// Parametry to startowy piksel i jego barwa RGB (3 liczby całkowite)
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
			// jeśli piksel ma sąsiada na po lewej to idziemy w lewo i zaznaczamy wszystkie piksele tego samego koloru aż do napotkania innego koloru lub konca obrazu
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
			// jeśli piksel ma sąsiada po prawej to idziemy w prawo i zaznaczamy wszystkie piksele tego samego koloru aż do napotkania innego koloru lub konca obrazu
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

// Funkcja isPixelSecurityPart zależnie od rotacji sprawdza czy piksel o zadanym położeniu nie jest częścią zabezpieczeń
// zwraca true jeśli tak
// false jeśli nie
bool DrawingOperations::isPixelSecurityPart(int col, int row)
{
	// zależnie od rotacji
	if (rotation == ROTATION0 && ((col >= signBeginningX && row >= signBeginningY) || // dla rotacji 0 stopni
			(col >= sizeFirstPixelX && row >= rows-2) ||
			(col >= columns-2 && row >= sizeFirstPixelY))) return true;

	else if (rotation == ROTATION90 && ((col <= signBeginningX && row >= signBeginningY) || // dla rotacji 90 stopni
			(col <= sizeFirstPixelY && row >= rows-2) ||
			(col <= 1 && row >= sizeFirstPixelX))) return true;

	else if (rotation == ROTATION180 && ((col <= signBeginningX && row <= signBeginningY) || // dla rotacji 180 stopni
			(col <= sizeFirstPixelX && row <= 1) ||
			(col <= 1 && row <= sizeFirstPixelY))) return true;

	else if (rotation == ROTATION270 && ((col >= signBeginningX && row <= signBeginningY) || // dla rotacji 270 stopni
			(col >= sizeFirstPixelY && row <= 1) ||
			(col >= columns-2 && row <= sizeFirstPixelX))) return true;

	return false;
}

// Funkcja checkChanges sprawdza zależności między pikselami - wykonywane w ramach weryfikacji obrazu
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

	for (col = sizeFirstPixelX; col < signBeginningX; col++)			// zaznaczenie zabezpieczenia rozmiaru X by nie bylo sprawdzane
	{
		pixelResults[col][rows-2] = false;
		pixelResults[col][rows-1] = false;
	}
	for (row = sizeFirstPixelY; row < signBeginningY; row++)			// zaznaczenie zabezpieczenia rozmiaru X by nie bylo sprawdzane
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

					if (!(red >= mediumRed - accDif && red <= mediumRed + accDif &&  //sprawdzanie poprawnosci z mozliwoscia odchylenia rowna accDif
						green >= mediumGreen - accDif && green <= mediumGreen + accDif &&
						blue >= mediumBlue - accDif && blue <= mediumBlue + accDif
						))
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

// Funkcja checkRotation0 sprawdza 3 piksele narożne w prawym dolnym rogu obrazu
// Jeśli są białe to znaczy, że tam jest znak zabezpieczający - czyli nie było rotacji
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

// Funkcja checkRotation90 sprawdza 3 piksele narożne w lewym dolnym rogu obrazu
// Jeśli są białe to znaczy, że tam jest znak zabezpieczający - czyli była rotacja obrazu 90 stopni
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

// Funkcja checkRotation180 sprawdza 3 piksele narożne w lewym górnym rogu obrazu
// Jeśli są białe to znaczy, że tam jest znak zabezpieczający - czyli była rotacja obrazu 180 stopni
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

// Funkcja checkRotation270 sprawdza 3 piksele narożne w prawym górnym rogu obrazu
// Jeśli są białe to znaczy, że tam jest znak zabezpieczający - czyli była rotacja obrazu 270 stopni
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

// Funkcja detectRotation sprawdza rotację obrazu
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
		partialRaport.second = codec->toUnicode("Wykryto rotację 90 stopni");
		raportImage.push_back(partialRaport);

		sizeFirstPixelX = rows - 7 - 6 * 15;
		sizeFirstPixelY = 6 + 6 * 15;

		rotation = ROTATION90;
	}
	else if (checkRotation180())
	{
		partialRaport.first = true;
		partialRaport.second = codec->toUnicode("Wykryto rotację 180 stopni");
		raportImage.push_back(partialRaport);

		sizeFirstPixelX = 6 + 6 * 15;
		sizeFirstPixelY = 6 + 6 * 15;

		rotation = ROTATION180;
	}
	else if (checkRotation270())
	{
		partialRaport.first = true;
		partialRaport.second = codec->toUnicode("Wykryto rotację 270 stopni");
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
		partialRaport.second = codec->toUnicode("Nie byłem w stanie określić rotacji obrazu");
		raportImage.push_back(partialRaport);
		rotation = ROTATION0;
	}
	return;
}

// Funkcja checkImageSecurity jest główną funkcją sprawdzającą
// Uruchamia sprawdzanie obrazu pod względem rotacji, czytania oryginalnej wielkości, zależności między pikselami
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
		partialRaport.second = codec->toUnicode("Nie było potrzeby sprawdzania poprawności znaku zabezpieczającego");
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
std::vector<std::pair<bool, QString> > DrawingOperations::getRaportImage() const
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
