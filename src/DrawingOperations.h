/*
 * DrawingOperations.h
 *
 *  Created on: 11 wrz 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Komentarz:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Ten plik opisuje klasę DrawingOperations
 */

#ifndef DRAWINGOPERATIONS_H_
#define DRAWINGOPERATIONS_H_

#define ROTATION0 0
#define ROTATION90 1
#define ROTATION180 2
#define ROTATION270 3

#include <QImage>
#include <QTextCodec>
#include <QString>

class DrawingOperations
{
private:
	int greyTones[153]; 	// tablica wartości grup sumy RGB
	int redTones[256]; 		// odcienie kolorow rgb do narysowania na histogram barw RGB
	int greenTones[256];
	int blueTones[256];

	int accDif; 			// acceptable difference - maksymalne akceptowane odchylenie wartosci kolorow rgb piksela

	bool** pixelResults;	// weryfikacja pikselow obrazu ktore piksele sa falszywe
							// inna niz pixelsChanged ( mowi w ktorych pikselach wykryto falszerstwo a nie ktore maja byc zamalowane)

	bool** pixelsChanged;   // tablica wskazuje ktore piksele zostaly przemalowane ( do pokazania miejsca zmian )
							// procz samych pikseli wykrytych bedzie malowane wokol by lepiej bylo widac efekt pracy
							// istnieje po to aby 2 razy tych samych pikseli nie kolorowac
							// false wskazuje ze nie mozna zamalowac - bo juz jest zamalowany ten piksel

	static const int signLength = 38; 			// dlugosc znaku zabezpieczającego (os X)
	static const int signHeight = 34;			// szerokosc / wysokosc znaku zabezpieczającego (os Y)
	int sizeFirstPixelX;						// piksel poczatkowy zabezpieczenia dlugosci (X)
	int sizeFirstPixelY;						// piksel poczatkowy zabezpieczenia wysokosci (Y)
	static const int digitColorRange = 5;		// akceptowalne wychylenie koloru piksela w znaku (do okreslenia wartosci zapisanej w pikselu)
	static const int sizeSeparatorColorRange = 25; // akceptowalne odchylenie koloru piksela separujacego bity wielkosci oryginalnej obrazu

	int signBeginningX, signBeginningY;		// parametry poczatku znaku na obrazie (odpowiednio os X i os Y)

	int columns, rows;						// ilosc kolumn i szeregow w obrazie

	unsigned char rotation;	// przechowuje informacje o sprawdzeniu obrotu obrazu
							// mozliwe wartosci to makrodefinicje rozpoczynajace sie od ROTATION

	QImage image;			// badany obraz
	QImage histogramBase;	// rysunek podstawy do rysowania histogramów
	QImage histogramGrey;	// histogram sumy RGB
	QImage histogramRGB;	// histogram barw RGB
	QImage histogramGComparison;	// histogram porównawczy wartości sum RGB
	QImage securedImage;	// obraz będący wynikiem zabezpieczania
	QImage checkedImage;	// obraz wynikowy weryfikacji
	std::string actualFileName;

	std::vector<std::pair<bool, QString> > raportImage;	// raport generowany podczas weryfikacji obrazu
	std::pair<bool, QString> partialRaport;
	QTextCodec* codec;

	void drawHistogramGrey(int greatestGreyNumber);
	void drawHistogramRGB(int greatestRGBNumber);
	int* readPixelRGB(int col, int row);
	void secureImage();

	void prepareHistogramsData();
	void drawHistograms();

	int convertBinaryToInt(int* binary);
	int* convertIntToBinary(int number);
	void drawSign();
	void drawOriginalSize(short mode); // mode 0 oznacza rysowanie XDimension | mode 1 oznacza rysowanie YDimension
	void drawEdge();
	std::vector<int> readFromSign();
	std::pair<int,int> readingSizeStep(int mode, int actualX, int actualY);
	void readOriginalSize(int mode); // tryb 0 dla czytania oryginalnej wielkosci X tryb 1 dla oryginalnego Y
	int convertDigitsToInt(int* digits);

	bool checkRotation0();
	bool checkRotation90();
	bool checkRotation180();
	bool checkRotation270();

	void detectRotation();

	unsigned char readPixelDigit(int col, int row);
	unsigned char readSavedXDim();
	unsigned char readSavedYDim();

	void drawChange(int col, int row); 									// podswietlac bedzie na czerwono zmiane w obrazie
	void lookForSimilarPixels(int col, int row);						// funkcja sprawdza czy piksele wokol znalezionego niepasujacego piksela
																		// nie powtarzaja sie jesli tak to je zamaluje
	void checkAdjacentPixels(int col, int row);
	void checkAdjacentUp(int col, int row, int red, int green, int blue);
	void checkAdjacentLeft(int col, int row, int red, int green, int blue);
	void checkAdjacentRight(int col, int row, int red, int green, int blue);
	void checkAdjacentDown(int col, int row, int red, int green, int blue);

	bool isPixelSecurityPart(int col, int row);
	bool checkChanges();
	void checkHistogramGrey (std::vector<int> greyTones);

public:

	void setImage(std::string filename);
	QImage getHistogramGrey() const;
	QImage getHistogramRGB() const;
	DrawingOperations();
	virtual ~DrawingOperations();
	bool saveImage(QString path);
	int* getHistGTones();
	QImage getHistogramComparison() const;
	QImage getCheckedImage() const;
	void checkImageSecurity(std::vector<int> savedGreyTones);
	std::vector<std::pair<bool, QString> > getRaportImage() const;
	std::pair<int, int> getActualSize() const;
	bool isLargeEnough() const;
	QImage checkForNewAccDif(int newAccDif);
};

#endif /* DRAWINGOPERATIONS_H_ */
