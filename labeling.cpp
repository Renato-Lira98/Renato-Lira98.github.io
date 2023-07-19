#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void removeBordas(Mat& image) {
    int width = image.cols;
    int height = image.rows;

    // Remover objetos das bordas
    for (int i = 0; i < width; i++) {
        floodFill(image, Point(i, 0), 255);
        floodFill(image, Point(i, height - 1), 255);
    }

    for (int i = 0; i < height; i++) {
        floodFill(image, Point(0, i), 255);
        floodFill(image, Point(width - 1, i), 255);
    }
}

void contarObjetos(Mat& image, int& nObjects, int& nBuracos) {
    int width = image.cols;
    int height = image.rows;

    nObjects = 0;
    nBuracos = 0;

    // Alterar o background
    floodFill(image, Point(0, 0), 100);

    // Contar objetos e buracos
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 0) {
                // Contar buraco
                nBuracos++;
                floodFill(image, Point(j, i), nBuracos);
            } else if (image.at<uchar>(i, j) == 255) {
                // Contar objeto
                nObjects++;
                floodFill(image, Point(j, i), 200);
            }
        }
    }
}

int main(int argc, char** argv) {
    Mat image = imread(argv[1], IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "Imagem nao carregou corretamente\n";
        return -1;
    }

    removeBordas(image);

    int nObjects, nBuracos;
    contarObjetos(image, nObjects, nBuracos);

    std::cout << "A figura tem " << nObjects << " bolhas\n";
    std::cout << "A figura tem " << nBuracos << " buracos\n";

    imshow("image", image);
    imwrite("labeling.png", image);
    waitKey();

    return 0;
}
