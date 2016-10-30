/*
 * DrawingOperations.h
 *
 *  Created on: 11 wrz 2016
 *      Author: andrzej
 */

#ifndef DRAWINGOPERATIONS_H_
#define DRAWINGOPERATIONS_H_

#define ROTATION0 0
#define ROTATION90 1
#define ROTATION180 2
#define ROTATION270 3

#include <QImage>


class DrawingOperations
{
private:
	int greyTones[153]; 	// each per 5 / 255 x 3

	int redTones[256]; 		// odcienie kolorow rgb do narysowania na histogram rgb
	int greenTones[256];
	int blueTones[256];

	int accDif; 			// acceptable difference - maksymalne akceptowane odchylenie wartosci kolorow rgb piksela

	bool** pixelResults;	// weryfikacja pikselow obrazu ktore piksele sa falszywe
							// inna niz pixelsChanged ( mowi w ktorych pikselach wykryto falszerstwo a nie ktore maja byc zamalowane)

	bool** pixelsChanged;   // tablica wskazuje ktore piksele zostaly przemalowane ( do pokazania miejsca zmian )
							// procz samych pikseli wykrytych bedzie malowane wokol by lepiej bylo widac efekt pracy
							// istnieje po to aby 2 razy tych samych pikseli nie kolorowac
							// false wskazuje ze nie mozna zamalowac - bo juz jest zamalowany ten piksel

	int signLength; 		// dlugosc znaczka (os X)
	int signHeight;			// szerokosc / wysokosc znaczka (os Y)
	int digitColorRange;	// akceptowalne wychylenie koloru piksela w znaczku (do okreslenia wartosci zapisanej w pikselu)

	int signBeginningX, signBeginningY;		// parametry poczatku znaczka na obrazie (odpowiednio os X i os Y)

	int infoBeginningX, infoBeginningY;

	int columns, rows;						// ilosc kolumn i szeregow w obrazie

	unsigned char rotation;	// przechowuje informacje o sprawdzeniu obrotu obrazu
							// mozliwe wartosci to makrodefinicje rozpoczynajace sie od ROTATION

	QImage image;
	QImage histogramBase;
	QImage histogramGrey;
	QImage histogramRGB;
	QImage histogramGComparison;
	QImage securedImage;
	QImage checkedImage;
	std::string actualFileName;

	std::vector<std::pair<bool, std::string> > raportImage;
	std::pair<bool, std::string> partialRaport;

	void drawHistogramGrey(int greatestGreyNumber);
	void drawHistogramRGB(int greatestRGBNumber);
	void secureImage();

	void prepareHistogramsData();
	void drawHistograms();

	void drawSign();
	std::vector<int> readFromSign();
	int convertDigitsToInt(int* digits);

	bool checkRotation0();
	bool checkRotation90();
	bool checkRotation180();
	bool checkRotation270();

	void detectRotation();

	unsigned char readPixelDigit(int col, int row);

	void drawChange(int col, int row); 									// podswietlac bedzie na czerwono zmiane w obrazie
	void lookForSimilarPixels(int col, int row);						// funkcja sprawdza czy piksele wokol znalezionego niepasujacego piksela
																		// nie powtarzaja sie jesli tak to je zamaluje
	void checkAdjacentPixels(int col, int row);						 	// funkcja rekurencyjna sprawdzajaca czy sasiednie obszary nie sa takie same
																		// jesli sa to zamalowuje je za pomoca drawchange
	void checkAdjacentUp(int col, int row, int red, int green, int blue);
	void checkAdjacentLeft(int col, int row, int red, int green, int blue);
	void checkAdjacentRight(int col, int row, int red, int green, int blue);
	void checkAdjacentDown(int col, int row, int red, int green, int blue);

	bool checkChanges();
	void checkHistogramGrey (std::vector<int> greyTones);

public:

	void setImage(std::string filename);
	QImage getHistogramGrey() const;
	QImage getHistogramRGB() const;
	DrawingOperations();
	virtual ~DrawingOperations();
	bool saveImage(QString path);
	std::vector<int> getCornerRGBs() const;
	int* getHistGTones();
	QImage getHistogramComparison() const;
	QImage getCheckedImage() const;
	void checkImageSecurity(std::vector<int> savedGreyTones);
	std::vector<std::pair<bool, std::string> > getRaportImage() const;
};

#endif /* DRAWINGOPERATIONS_H_ */
