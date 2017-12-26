/*
 * HistogramOperations.cpp
 *
 *  Created on: 16 lis 2016
 *      Autor: andrzej
 *
 *  Komentarz:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Ten plik zawiera implementację funkcji klasy DrawingOperations odpowiedzialnych za operacje na histogramach obrazu.
 */

#include "DrawingOperations.h"

#include <QPainter>

// Przyporządkowuje piksele obrazu do odpowiednich grup
void DrawingOperations::prepareHistogramsData()
{
	QRgb pixel;
	QColor color;
	int red, green, blue, tone;

	for (int t = 0; t < 153; t++) greyTones[t] = 0; // przygotowywanie tablic liczebności grup dla obu histogramów

	for (int t = 0; t < 256; t++)
	{
		redTones[t] = 0;
		greenTones[t] = 0;
		blueTones[t] = 0;
	}

	// przechodzenie po obrazie i przyporządkowywanie pikseli do odpowiednich grup
	for (int pixelC = 0; pixelC < columns; pixelC++)
	{
		for (int pixelR = 0; pixelR < rows; pixelR++)
		{
			if (isPixelSecurityPart(pixelC, pixelR)) continue; // pominięcie części zabezpieczających (znak zabezpieczający i zabezpieczenia wielkości obrazu)

			pixel = image.pixel(pixelC, pixelR);
			color.setRgb(pixel);
			red = color.red();
			green = color.green();
			blue = color.blue();

			redTones[red]++;					// odpowiednio grupy czerwieni zieleni i niebieskiego
			greenTones[green]++;
			blueTones[blue]++;

			tone = (red + green + blue) / 5;
			if (tone == 153)
			{
				greyTones[152]++;				// grupa sum barw RGB
			}
			else greyTones[tone]++;
		}
	}

	return;
}

// Funkcja drawHistograms odpowiada za wyznaczenie najbardziej liczbych grup w histogramach i uruchomieniu funkcji rysujących poszczególne histogramy
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

	drawHistogramGrey(greatestGreyNumber); // rysowanie histogramu sum RGB
	drawHistogramRGB(greatestRGBNumber);   // rysowanie histogramu RGB obrazu

	return;
}

// Funkcja drawHistogramGrey rysuje histogram sum barw RGB
void DrawingOperations::drawHistogramGrey(int greatestGreyNumber)
{
	QPainter painter(&histogramGrey);

	// PISANIE PO HISTOGRAMIE
    QFont f=painter.font();
    f.setPointSize(10);
    f.setBold(true);
    painter.setFont(f);
    // piszemy na rysunku histogramu
    painter.drawText( QPoint(7,75), QString::number(greatestGreyNumber));
    painter.drawText( QPoint(40, 400), "0");
    painter.drawText( QPoint(50, 415), "0");
    painter.drawText(QPoint(500, 415), "153");
    painter.drawText(QPoint(10, 20), "Liczebnosc grupy");
    painter.drawText(QPoint(300, 430), "Numer grupy");
	painter.setBrush(QColor(150, 150, 150));
	painter.setPen(QColor(150, 150, 150));

	int xPos, yPos, height;
	for (int tone = 0; tone < 153; tone++) // pętla rysuje prostokąty dla 153 grup sum RGB
	{
		xPos = 50 + tone * 3;
		height = ((float)greyTones[tone] / greatestGreyNumber) * 325;
		yPos = 400 - height;
		painter.drawRect(xPos , yPos, 3, height);
	}
	return;
}

// Funkcja drawHistogramRGB rysuje histogram barw RGB
void DrawingOperations::drawHistogramRGB(int greatestRGBNumber)
{
	QPainter painter(&histogramRGB);

	// PISANIE PO HISTOGRAMIE
    QFont f=painter.font();
    f.setPointSize(10);
    f.setBold(true);
    painter.setFont(f);

    painter.drawText( QPoint(7,75), QString::number(greatestRGBNumber));
    painter.drawText( QPoint(40, 400), "0");
    painter.drawText( QPoint(50, 415), "0");
    painter.drawText(QPoint(545, 415), "256");
    painter.drawText(QPoint(10, 20), "Liczebnosc grupy");
    painter.drawText(QPoint(300, 430), "Numer grupy");

	int xPos, yPos, height;

	// dla każdego indeksu grup rysujemy 3 punkty (2x2 piksele)
	for (int tone = 0; tone < 256; tone++)
	{
		painter.setBrush(Qt::red);									// rysowanie wartości dla barwy czerwieni
		painter.setPen(Qt::red);
		xPos = 50 + tone * 2;
		height = ((float)redTones[tone] / greatestRGBNumber) * 325;
		yPos = 400 - height;
		painter.drawRect(xPos , yPos, 2, 2);

		painter.setBrush(Qt::green);								// rysowanie wartości dla barwy zieleni
		painter.setPen(Qt::green);
		xPos = 50 + tone * 2;
		height = ((float)greenTones[tone] / greatestRGBNumber) * 325;
		yPos = 400 - height;
		painter.drawRect(xPos , yPos, 2, 2);

		painter.setBrush(Qt::blue);									// rysowanie wartości dla barwy niebieskiego
		painter.setPen(Qt::blue);
		xPos = 50 + tone * 2;
		height = ((float)blueTones[tone] / greatestRGBNumber) * 325;
		yPos = 400 - height;
		painter.drawRect(xPos , yPos, 2, 2);
	}
	return;
}

// Funkcja checkHistogramGrey rysuje histogram porównawczy wartości historycznych odczytanych z zabezpieczenia i aktualnego stanu obrazu
void DrawingOperations::checkHistogramGrey(std::vector<int> savedGreyTones)
{
	int heightSecured, heightActual;
	int greatestNumber;
	int xPos, yPos;

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

	// PISANIE PO HISTOGRAMIE
    QFont f=painter.font();
    f.setPointSize(10);
    f.setBold(true);
    painter.setFont(f);

    painter.drawText( QPoint(7,75), QString::number(greatestNumber));
    painter.drawText( QPoint(40, 400), "0");
    painter.drawText( QPoint(50, 415), "0");
    painter.drawText(QPoint(500, 415), "153");
    painter.drawText(QPoint(10, 20), "Liczebnosc grupy");
    painter.drawText(QPoint(300, 430), "Numer grupy");

	// wyliczanie polozenia, kolejnosci rysowania i wysokosci prostokatow
	for (int tone = 0; tone < 153; tone++)
	{
		xPos = 50 + tone * 3;

		painter.setBrush(Qt::green);
		painter.setPen(Qt::green);

		heightSecured = ((float)savedGreyTones[tone] / greatestNumber ) * 325;
		heightActual = ((float)greyTones[tone] / greatestNumber ) * 325;

		if (heightActual > heightSecured) // jeśli wartość dla aktualnego stanu obrazu większa od wielkości zapisanej
		{
			painter.setBrush(Qt::red);
			painter.setPen(Qt::red);
			yPos = 400 - heightActual;

			painter.drawRect(xPos, yPos, 3, heightActual);

			painter.setBrush(Qt::yellow);
			painter.setPen(Qt::yellow);
			yPos = 400 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);
		}
		else if (heightActual < heightSecured) // jeśli wartość dla aktualnego stanu obrazu mniejsza od wielkości zapisanej
		{
			painter.setBrush(Qt::yellow);
			painter.setPen(Qt::yellow);
			yPos = 400 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);

			painter.setBrush(Qt::red);
			painter.setPen(Qt::red);
			yPos = 400 - heightActual;

			painter.drawRect(xPos, yPos, 3, heightActual);
		}
		else // obie wysokosci takie same
		{
			painter.setBrush(Qt::green);
			painter.setPen(Qt::green);
			yPos = 400 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);
		}
	}
	return;
}

// Funkcja getHistogramGrey zwraca narysowany histogram sum barw RGB
QImage DrawingOperations::getHistogramGrey() const
{
	return histogramGrey;
}

// Funkcja getHistogramRGB zwraca narysowany histogram barw RGB
QImage DrawingOperations::getHistogramRGB() const
{
	return histogramRGB;
}

// Funkcja getHistGTones zwraca liczebność poszczególnych grup histogramu sum barw RGB
int* DrawingOperations::getHistGTones()
{
	return greyTones;
}

// Funkcja getHistogramComparison zwraca narysowany histogram porównawczy
QImage DrawingOperations::getHistogramComparison() const
{
	return histogramGComparison;
}
