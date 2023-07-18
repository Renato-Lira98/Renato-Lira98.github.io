#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat imagemCodificada, imagemPortadora, imagemEscondida;
	Vec3b valCod, valPort, valEsc;
	int nbits = 3;
	char diretorio[1000];


	// Recebendo a imagem;
	do {
		cout << "Digite a localizacao da imagem codificada: " << endl;
		cout << "EX.: C:\\User\\Desktop\\<nomedaimagem>.<png ou jpeg>" << endl;
		cin >> diretorio;

		imagemCodificada = imread(diretorio, IMREAD_COLOR);

		if (!imagemCodificada.data) {
			cout << "Imagem nao encontrada!" << endl;
			cout << "Verifique o endereco digitado." << endl;
		}
	} while (!imagemCodificada.data);

	// Clonando;
	imagemEscondida = imagemCodificada.clone();
	imagemPortadora = imagemCodificada.clone();

	// Realiazando a decodificação da imagem;
	for (int i = 0; i < imagemCodificada.rows; i++) {
		for (int j = 0; j < imagemCodificada.cols; j++) {
			valCod = imagemCodificada.at<Vec3b>(i, j);

			valEsc[0] = valCod[0] << (8 - nbits);
			valEsc[1] = valCod[1] << (8 - nbits);
			valEsc[2] = valCod[2] << (8 - nbits);

			imagemEscondida.at<Vec3b>(i, j) = valEsc;

			valPort[0] = valCod[0] >> nbits << nbits;
			valPort[1] = valCod[1] >> nbits << nbits;
			valPort[2] = valCod[2] >> nbits << nbits;

			imagemPortadora.at<Vec3b>(i, j) = valPort;
		}
	}	

	imwrite("imagemEscondida.png", imagemEscondida);
	imwrite("imagemPortadora.png", imagemPortadora);

	return 0;
}