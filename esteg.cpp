#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat imagemCodificada, imagemEscondida, imagemPortadora, imagemDecodificada;
    Vec3b valCod, valEsc, valPort, valDec;
    int nbits = 5;

    // Recebendo a imagem codificada;
    imagemCodificada = cv::imread(argv[1], cv::IMREAD_COLOR);

    if (imagemCodificada.empty()) {
        cout << "Imagem nao carregou corretamente" << endl;
        return -1;
    }

    // Inicializando as matrizes clonadas
    imagemEscondida = Mat(imagemCodificada.size(), imagemCodificada.type());
    imagemPortadora = Mat(imagemCodificada.size(), imagemCodificada.type());
    imagemDecodificada = Mat(imagemCodificada.size(), imagemCodificada.type());

    // Realizando a decodificação da imagem;
    for (int i = 0; i < imagemCodificada.rows; i++) {
        for (int j = 0; j < imagemCodificada.cols; j++) {
            valCod = imagemCodificada.at<Vec3b>(i, j);

            valEsc[0] = valCod[0] << nbits;
            valEsc[1] = valCod[1] << nbits;
            valEsc[2] = valCod[2] << nbits;

            imagemEscondida.at<Vec3b>(i, j) = valEsc;

            valPort[0] = valCod[0] >> nbits << nbits;
            valPort[1] = valCod[1] >> nbits << nbits;
            valPort[2] = valCod[2] >> nbits << nbits;

            imagemPortadora.at<Vec3b>(i, j) = valPort;

            valDec[0] = valCod[0] >> (8 - nbits) << (8 - nbits);
            valDec[1] = valCod[1] >> (8 - nbits) << (8 - nbits);
            valDec[2] = valCod[2] >> (8 - nbits) << (8 - nbits);

            imagemDecodificada.at<Vec3b>(i, j) = valDec;
        }
    }

    imwrite("imagemEscondida.png", imagemEscondida);
    imwrite("imagemPortadora.png", imagemPortadora);
    imwrite("imagemDecodificada.png", imagemDecodificada);

    return 0;
}
