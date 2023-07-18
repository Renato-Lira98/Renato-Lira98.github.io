#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

    Mat imagemOriginal, imagemCinza, imagemEqualizada;
    char diretorio[1000];

    // Carregar a imagem
    cout << "Digite a localizacao da imagem: " << endl;
    cout << "EX.: C:\\User\\Desktop\\<nomedaimagem>.<png ou jpeg>" << endl;
    cin >> diretorio;

    imagemOriginal = imread(diretorio);

    if (imagemOriginal.empty()) {
        cout << "Não foi possível carregar a imagem" << endl;
        return -1;
    }

    // Converter para escala de cinza
    cvtColor(imagemOriginal, imagemCinza, COLOR_BGR2GRAY);

    // Equalizar o histograma
    equalizeHist(imagemCinza, imagemEqualizada);

    imshow("Imagem em Escala de Cinza", imagemCinza);
    imwrite("ImagememEscaladeCinza.png", imagemCinza);
    imshow("Imagem Equalizada", imagemEqualizada);
    imwrite("ImagemEqualizada.png", imagemEqualizada);


    waitKey(0);

    return 0;
}