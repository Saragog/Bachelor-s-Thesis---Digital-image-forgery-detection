#include "DrawingOperations.h"

#include <QPainter>

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
			if (isPixelSecurityPart(pixelC, pixelR)) continue;

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

	// PISANIE PO HISTOGRAMIE
    QFont f=painter.font();
    f.setPointSize(10);
    f.setBold(true);
    painter.setFont(f);

    painter.drawText( QPoint(7,75), QString::number(greatestGreyNumber));
    painter.drawText( QPoint(40, 400), "0");
    painter.drawText( QPoint(50, 415), "0");
    painter.drawText(QPoint(500, 415), "153");
    painter.drawText(QPoint(10, 20), "Liczebnosc grupy");
    painter.drawText(QPoint(300, 430), "Numer grupy");

	painter.setBrush(QColor(150, 150, 150));
	painter.setPen(QColor(150, 150, 150));

	int xPos, yPos, height;

	for (int tone = 0; tone < 153; tone++)
	{
		xPos = 50 + tone * 3;
		height = ((float)greyTones[tone] / greatestGreyNumber) * 325;
		yPos = 400 - height;
		painter.drawRect(xPos , yPos, 3, height);
	}
	return;
}

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
    painter.drawText(QPoint(545, 415), "255");
    painter.drawText(QPoint(10, 20), "Liczebnosc grupy");
    painter.drawText(QPoint(300, 430), "Numer grupy");

	int xPos, yPos, height;

	for (int tone = 0; tone < 256; tone++)
	{
		painter.setBrush(Qt::red);
		painter.setPen(Qt::red);
		xPos = 50 + tone * 2;
		height = ((float)redTones[tone] / greatestRGBNumber) * 325;
		yPos = 400 - height;
		painter.drawRect(xPos , yPos, 2, 2);

		painter.setBrush(Qt::green);
		painter.setPen(Qt::green);
		xPos = 50 + tone * 2;
		height = ((float)greenTones[tone] / greatestRGBNumber) * 325;
		yPos = 400 - height;
		painter.drawRect(xPos , yPos, 2, 2);

		painter.setBrush(Qt::blue);
		painter.setPen(Qt::blue);
		xPos = 50 + tone * 2;
		height = ((float)blueTones[tone] / greatestRGBNumber) * 325;
		yPos = 400 - height;
		painter.drawRect(xPos , yPos, 2, 2);
	}
	return;
}

void DrawingOperations::checkHistogramGrey(std::vector<int> savedGreyTones)
{
	int heightSecured, heightActual;
	int greatestNumber;
	int xPos, yPos;

	//histogramGComparison = QImage(histogramBase);

	QPainter painter(&histogramGComparison);

	// szukanie najwiekszej wartosci sposrod aktualnych i zapisanych

	// TODO !!! zmienic nazwe funkcji na rysowanie roznicy histogramow bo z tej nazwy nie wiadomo o co chodzi

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

		if (heightActual > heightSecured)
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
		else if (heightActual < heightSecured)
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
		else // obie wysokosci takie same czyli w ogole nie ma zmiany w histogramie
		{
			painter.setBrush(Qt::green);
			painter.setPen(Qt::green);
			yPos = 400 - heightSecured;

			painter.drawRect(xPos, yPos, 3, heightSecured);
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

int* DrawingOperations::getHistGTones()
{
	return greyTones;
}

QImage DrawingOperations::getHistogramComparison() const
{
	return histogramGComparison;
}
