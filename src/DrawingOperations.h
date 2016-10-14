/*
 * DrawingOperations.h
 *
 *  Created on: 11 wrz 2016
 *      Author: andrzej
 */

#ifndef DRAWINGOPERATIONS_H_
#define DRAWINGOPERATIONS_H_

#include <QImage>


class DrawingOperations
{
private:
	int greyTones[153]; 	// each per 5 / 255 x 3

	int redTones[256]; 		// odcienie kolorow rgb do narysowania na histogram rgb
	int greenTones[256];
	int blueTones[256];
	int accDif; 			// acceptable difference - maksymalne akceptowane odchylenie wartosci kolorow rgb piksela

	bool** blockResults;	// weryfikacja blokow obrazu ktore bloki sa falszywe
							// wykorzystywane do zamalowywania sasiednich blokow jesli
	int colIndex, rowIndex; // centralne punkty maja te same rgb
	int columns, rows;

	QImage image;
	QImage histogramBase;
	QImage histogramGrey;
	QImage histogramRGB;
	QImage histogramGComparison;
	QImage securedImage;
	QImage checkedImage;
	std::string actualFileName;

	void drawHistogramGrey(int greatestGreyNumber);
	void drawHistogramRGB(int greatestRGBNumber);
	void secureImage();
	void prepareHistograms();

	bool checkPixel(unsigned int row, unsigned int col, int r, int g, int b) const;

	void drawChange(int col, int row); 										// podswietlac bedzie na czerwono zmiane w obrazie
	void checkAdjacentSquares(int col, int row, int colInd, int rowInd); 	// funkcja rekurencyjna sprawdzajaca czy sasiednie obszary nie sa takie same
																			// jesli sa to zamalowuje je za pomoca drawchange
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
	void checkImageSecurity(std::vector<int>* savedValues);
};

#endif /* DRAWINGOPERATIONS_H_ */
