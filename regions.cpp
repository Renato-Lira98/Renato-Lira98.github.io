// Renato Emanuel Medeiros de Lira
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char** argv) {

	Mat image;
 	image=imread(argv[1], IMREAD_GRAYSCALE);

    if (image.empty()) {
        cout << "Imagem não foi carregada" << endl;
        return 1;
    }

    Point p1, p2;

	cout << "Tamanho da imagem: " << image.rows << "x" << image.cols << endl;
	
    cout << "Digite as coordenadas do ponto P1:" << endl;
    cin >> p1.y >> p1.x;

	do{
    cout << "Digite as coordenadas do ponto P2:" << endl;
    cin >> p2.y >> p2.x;
	if (p1.y >= p2.y || p1.x >= p2.x){
		cout << "Ambas coordenadas do ponto P2 tem que ser maiores que as coordenadas do ponto P1, escreva novamente!"<<endl;
	}
	}while(p1.y >= p2.y || p1.x >= p2.x);

    for (int i = p1.x; i < p2.x; i++) {
        for (int j = p1.y; j < p2.y; j++) {
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
        }
    }

    namedWindow("janela", WINDOW_AUTOSIZE);
    imshow("janela", image);
    waitKey(0);

    imwrite("Regions.png", image);

    return 0;
}
