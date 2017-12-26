/*
 * WindowMain.cpp
 *
 *  Created on: 17 sie 2016
 *      Autor: Andrzej Dackiewicz
 *
 *  Comment:Ten plik jest częścią programu wykrywającego fałszerstwa cyfrowe w obrazach, który
 *  		został stworzony w ramach pracy inżynierskiej.
 *  		Ten plik implementuje funkcje klasy WindowMain, która odpowiada za komunikację między obiektami programu.
 *  		Może być traktowana za funkcję kontrolującą.
 *  		Obsługuje większość zdarzeń występujących w trakcie działania programu.
 */

#include "WindowMain.h"
#include "WindowMenu.h"

WindowMain::WindowMain()
{
	// Tworzenie widoków
	menu = new WindowMenu(this);
	exivData = new WindowExivData(this);
	secureImage = new WindowSecureImage(this);
	secureCheck = new WindowSecureCheck(this);
	allExivData = new WindowAllExivData(this);

	// QStackedWidget ułatwia zarządzanie oknami
	stackedWidget = new QStackedWidget();
	// Każde okno ma swoje id
	stackedWidget->addWidget(menu); // id 0
	stackedWidget->addWidget(exivData); // id 1
	stackedWidget->addWidget(secureImage); // id 2
	stackedWidget->addWidget(secureCheck); // id 3
	stackedWidget->addWidget(allExivData); // id 4

	// QComboBox ułatwia podmianę okien
	pageComboBox = new QComboBox();
	pageComboBox->addItem(tr("WidokMenu"));
	pageComboBox->addItem(tr("WidokExivData"));
	pageComboBox->addItem(tr("WidokSecureImage"));
	pageComboBox->addItem(tr("WidokSecureCheck"));
	pageComboBox->addItem(tr("WidokWszystkieInformacjeExif"));

	// Podmiana okien
	connect(pageComboBox, SIGNAL(activated(int)), stackedWidget, SLOT(setCurrentIndex(int)));
	this->setCentralWidget(stackedWidget);
	this->setGeometry(100, 100, 1256, 924);
	// Na początku pokazywane jest menu główne programu
	showMenu();
	// ExivOperations i DrawingOperations to klasy, które mogą być uznane za model programu
	// Wykonują większość obliczeń w aplikacji
	exivOperations = new ExivOperations(this);
	drawingOperations = new DrawingOperations();

	// MessageBox zostanie pokazany, jeśli będą problemy z wybranym plikiem
	message.setGeometry(600, 450, 200, 200);
	message.setText("Nie ma takiego pliku !!!");
	message.setDefaultButton(QMessageBox::Ok);
	this->show();
}

// Destruktor klasy WindowMain
WindowMain::~WindowMain()
{
	delete menu;
	delete exivData;
	delete secureImage;
	delete secureCheck;
	delete allExivData;
	delete stackedWidget;
	delete pageComboBox;
	delete exivOperations;
	delete drawingOperations;
}

// Funkcja checkImageFile sprawdza, czy plik o podanej ścieżce jest plikiem obrazu
bool WindowMain::checkImageFile(std::string fileName)
{
	if (exivOperations->isImageFile(fileName))
	{
		return true;
	}
	return false;
}

// Funkcja showMenu przestawia aktualne okno na okno menu głównego
void WindowMain::showMenu()
{
	stackedWidget->setCurrentIndex(0);
	return;
}

// Funkcja showExivData przestawia aktualne okno na okno z histogramami i wybranymi metadanymi EXIF
void WindowMain::showExivData(QString fileName)
{
	// wektor na metadane
	std::vector<std::string> data;

	if (checkImageFile(fileName.toLatin1().data())) // sprawdzenie, czy plik jest plikiem obrazu
	{
		if (!exivOperations->readFromFile(fileName.toStdString())) // sprawdza, czy w pliku są metadane EXIF
		{
			// Wyświetlenie komunikatu, że nie znaleziono metadanych EXIF
			message.setText("W pliku nie znaleziono informacji exif !!!");
			message.exec();
		}
		data = exivOperations->getBasicExifData();
		drawingOperations->setImage(fileName.toStdString());

		// tworzenie histogramu sum barw RGB i histogramu RGB
		QImage histogramGrey = drawingOperations->getHistogramGrey();
		QImage histogramRGB = drawingOperations->getHistogramRGB();

		// przygotowywanie okna
		exivData->prepareWindow(data);
		exivData->setImage(fileName);
		exivData->setHistogramGrey(histogramGrey);
		exivData->setHistogramRGB(histogramRGB);
		// podmiana okien na okno z histogramami i wybranymi metadanymi
		stackedWidget->setCurrentIndex(1);
	}
	else
	{	// nie znaleziono pliku
		message.setText("Nie ma takiego pliku !!!");
		message.exec();
	}
	return;
}

// Funkcja showSecureImage podmienia okno menu głównego na okno zebezpieczania obrazu
void WindowMain::showSecureImage(QString fileName)
{
	if (checkImageFile(fileName.toLatin1().data())) // sprawdzenie, czy plik jest plikiem obrazu
	{
		// ustawienie modelu aplikacji
		exivOperations->readFromFile(fileName.toStdString());
		secureImage->setImage(fileName);
		drawingOperations->setImage(fileName.toStdString());

		// sprawdzenie, czy obraz jest wystarczająco duży by go zabezpieczyć ( minimum 97x97 pikseli)
		if (!drawingOperations->isLargeEnough())
		{
			// informacja, że obraz jest za mały
			message.setText("Ten obraz jest za maly aby stworzyc na nim zabezpieczenia !!!");
			message.exec();
		}
		else
		{
			// przestawienie okna na okno zabezpieczania obrazu
			stackedWidget->setCurrentIndex(2);
		}
	}
	else
	{
		// nie znaleziono pliku
		message.setText("Nie ma takiego pliku !!!");
		message.exec();
	}
	return;
}

// Funkcja showSecureCheck podmienia okno menu głównego na okno weryfikacji obrazu
void WindowMain::showSecureCheck(QString fileName)
{
	std::vector<std::pair<bool, QString> > exifRaport, imageRaport, concatRaport;
	std::vector<int> savedGreyTones;
	QImage histogramComparison;
	QImage checkedImage;

	if (checkImageFile(fileName.toLatin1().data())) 	// sprawdzenie, czy plik jest plikiem obrazu
	{
		// ustawienie modelu aplikacji
		secureCheck->setImage(fileName);
		drawingOperations->setImage(fileName.toStdString());

		if (!drawingOperations->isLargeEnough()) // sprawdzenie, czy obraz jest wystarczająco duży, aby wykonać na nim sprawdzenie
		{
			// obraz jest za mały
			message.setText("Ten obraz jest za maly aby sprawdzic dla niego zabezpieczenia !!!");
			message.exec();
			return;
		}
		else if (exivOperations->readFromFile(fileName.toStdString())) // sprawdzenie, czy plik zawiera metadane EXIF
		{
			exivOperations->checkExifSecurity();
			savedGreyTones = exivOperations->getSavedGreyTones();
		}
		else
		{
			exivOperations->checkExifSecurity();
			// wiadomość, że w pliku nie znaleziono metadanych EXIF
			message.setText("W pliku nie znaleziono informacji exif !!!");
			message.exec();
		}

		drawingOperations->checkImageSecurity(savedGreyTones);

		// przygotowanie raportu sprawdzania
		exifRaport = exivOperations->getRaportExif();
		imageRaport = drawingOperations->getRaportImage();

		concatRaport = exifRaport;
		for (unsigned int x = 0; x < imageRaport.size(); x++)
		{
			concatRaport.push_back(imageRaport.at(x));
		}

		// otrzymywanie histogramu porównującego histogram historyczny i histogram oparty na aktualnym stanie obrazu
		histogramComparison = drawingOperations->getHistogramComparison();
		checkedImage = drawingOperations->getCheckedImage();

		// przygotowanie okna
		secureCheck->setRaport(concatRaport);
		secureCheck->setHistogramC(histogramComparison);
		secureCheck->setCheckedImage(checkedImage);

		// podmiana okna menu głównego z oknem weryfikacji
		stackedWidget->setCurrentIndex(3);
	}
	else
	{
		// plik nie został znaleziony
		message.setText("Nie ma takiego pliku !!!");
		message.exec();
	}
	return;
}

// Funkcja showExivDataAgain podmienia okno ze wszystkimi metadanymi EXIF na okno z histogramami i wybranymi metadanymi
void WindowMain::showExivDataAgain()
{
	stackedWidget->setCurrentIndex(1);
	return;
}

// Funkcja showAllExivData podmienia okno z histogramami i wybranymi metadanymi na okno ze wszystkimi metadanymi znalezionymi w pliku obrazu
void WindowMain::showAllExivData()
{
	std::vector<std::string> data;

	// przygotowanie okna
	data = exivOperations->getAllExifData();
	allExivData->prepareWindow(data);
	// zamiana okien
	stackedWidget->setCurrentIndex(4);
}

// Funkcja saveImage obsługuje zdarzenie wciśnięcia przycisku zapisu w oknie zabezpieczania obrazu
void WindowMain::saveImage(QString path)
{
	if (drawingOperations->saveImage(path)) // zabezpiecza obraz i zapisuje w odpowiedniej lokalizacji
	{
		// pisze zawartość metadanych do pliku
		exivOperations->saveExifIntoFile(path.toStdString());
		showMenu(); // zamienia aktualne okno na okno menu głównego
	}
	else
	{
		// wystąpił błąd podczas zapisywania obrazu
		message.setText("Nie udalo sie zapisac pliku !!!");
		message.exec();
	}
	return;
}

// Funkcja adjustToNewAccDif obsługuje ponowienie badań na obrazie z wybranym parametrem dokładności weryfikacji
// Dodatkowo aktualizuje okno weryfikacji obrazu
void WindowMain::adjustToNewAccDif(int newAccDif)
{
	QImage checkedImage;
	// otrzymuje obraz wynikowy
	checkedImage = drawingOperations->checkForNewAccDif(newAccDif);
	// aktualizacja obrazu wynikowego
	secureCheck->setCheckedImage(checkedImage);
	return;
}

// Funkcja getHistGTones zwraca liczebność wszystkich 153 grup sum RGB
int* WindowMain::getHistGTones() const
{
	return drawingOperations->getHistGTones();
}

// Funkcja getActualSize zwraca aktualną wielkość obrazu
std::pair<int, int> WindowMain::getActualSize() const
{
	return drawingOperations->getActualSize();
}
