#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
	cout << "INICIANDO O PROGRAMA..." << endl;
	// INICIALIZANDO VÁRIAVEIS ...
	char diretorio[1000];
	Mat image;
	int cols, rows, bburacos = 0, bolhas=0;
	Point p; 

	// RECEBENDO IMAGEM ...
	do {
		cout << "Digite a localizacao da imagem: " << endl;
		cout << "EX.: C:\\User\\Desktop\\<nomedaimagem>.<png ou jpeg>" << endl;
		cin >> diretorio;

		image = imread(diretorio, IMREAD_GRAYSCALE);

		if (!image.data) {
			cout << "Imagem nao encontrada!" << endl;
			cout << "Verifique se a escrita esta correta." << endl;
		}
	} while (!image.data);

    imshow("janelaOriginal", image);

    cols = image.cols;
    rows = image.rows;
    p.x = 0;
    p.y = 0;

    // REMOVENDO AS BOLHAS LOCALIZADAS NAS BORDAS ...
    cout << "Removendo as bolhas localizadas nas bordas..." << endl;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (image.at<uchar>(i, j) == 255) {
                if (i == 0 || j == 0 || i == (rows - 1) || j == (cols - 1)) {
                    p.x = j;
                    p.y = i;
                    floodFill(image, p, 0);
                }
            }
        }
    }

    p.x = 0;
    p.y = 0;
    floodFill(image, p, 200);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (image.at<uchar>(i, j) == 255) {
                p.x = j;
                p.y = i;
                bolhas++;
                floodFill(image, p, 30);
            }
        }
    }

    cout << "Operacao finalizada ... " << endl; 
    imshow("JanelaSBolhas", image);
    imwrite("JanelaSBolhas.png", image);
    waitKey();

    // CONTANDO QUANTAS BOLHAS TEM BURACOS... 
    cout << "Contando quantas bolhas tem buraco..." << endl;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (image.at<uchar>(i, j) == 0) {
                if (image.at<uchar>(i - 1, j) != 200) {
                    bburacos++;
                    p.x = j;
                    p.y = i;
                    floodFill(image, p, 200);
                }
            }
        }
    }

    cout << "Operacao finalizada... " << endl;
    imshow("labeling", image);
    imwrite("labeling.png", image);

    cout << "total de bolhas com buracos: " << bburacos << endl;
    cout << "total de bolhas sem buracos: " << bolhas - bburacos << endl;
    cout << "total de bolhas: " << bolhas << endl;
    waitKey();

    return 0;
}
