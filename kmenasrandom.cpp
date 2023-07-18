#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

	int nCluster = 6, nRodadas = 1, cont = 1;
	Mat rotulos, centros, imagem; 
	char diretorio[1000];

	cout << "Digite o endereço da imagem: " << endl;
	cin >> diretorio;

	imagem = imread(diretorio, IMREAD_COLOR);
	Mat samples(imagem.rows * imagem.cols, 3, CV_32F);

	while (cont <= 10) {
		for (int y = 0; y < imagem.rows; y++) {
			for (int x = 0; x < imagem.cols; x++) {
				for (int z = 0; z < 3; z++) {
					samples.at<float>(y + x * imagem.rows, z) = imagem.at<Vec3b>(y, x)[z];
				}
			}
		}

		kmeans(samples,	nCluster, rotulos,
			TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001),
			nRodadas, KMEANS_RANDOM_CENTERS,centros);

		Mat rotulada(imagem.size(), imagem.type());
		for (int y = 0; y < imagem.rows; y++) {
			for (int x = 0; x < imagem.cols; x++) {
				int indice = rotulos.at<int>(y + x * imagem.rows, 0);
				rotulada.at<cv::Vec3b>(y, x)[0] = (uchar)centros.at<float>(indice, 0);
				rotulada.at<cv::Vec3b>(y, x)[1] = (uchar)centros.at<float>(indice, 1);
				rotulada.at<cv::Vec3b>(y, x)[2] = (uchar)centros.at<float>(indice, 2);
			}
		}

		stringstream ss;
		ss << "saida" << cont << ".jpg";
		string s = ss.str();
		cont++;
		imshow("clustered image", rotulada);
		imwrite(s, rotulada);
		waitKey(1);
		
	}

	return 0;
}