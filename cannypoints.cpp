#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;


vector<int> showCenter(vector<int> x, int step) {

    for (uint i = 0; i < x.size(); i++) {
        x[i] = x[i] * step + step / 2;
    }
    return x;
}

void funcCircle(Mat image, Mat imageArt, Vec3b color, int r, int x, int y) {

    color = image.at<Vec3b>(x, y);
    circle(imageArt, Point(y, x), r, CV_RGB(color[2], color[1], color[0]), -1, LINE_AA);
}

int main() {
    vector<int> cols, rows;
    Mat imagem, fps, points, gray, ofusc, borders;
    int x, y, fator = 60, passo = 5, jitter = 3, raio = 3;
    Vec3b cor;
    char diretorio[1000];

    cout << "Digite o endereço da imagem: " << endl;
    cin >> diretorio;

    imagem = imread(diretorio);
    Mat imagemArte(imagem.rows, imagem.cols, imagem.type());
    srand(time(0));

    if (!imagem.data) {
        cout << "nao abriu" << endl;
        exit(0);
    }

    rows.resize(imagem.rows / passo);
    cols.resize(imagem.cols / passo);
    iota(rows.begin(), rows.end(), 0);
    iota(cols.begin(), cols.end(), 0);
    rows = showCenter(rows, passo);
    cols = showCenter(cols, passo);

    random_shuffle(rows.begin(), rows.end());
    for (auto i : rows) {

        random_shuffle(cols.begin(), cols.end());
        for (auto j : cols) {

            x = i + rand() % (2 * jitter) - jitter + 1;
            y = j + rand() % (2 * jitter) - jitter + 1;
            funcCircle(imagem, imagemArte, cor, raio, x, y);
        }
    }
    imwrite("points.jpg", imagemArte);

    cvtColor(imagemArte, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, ofusc, Size(5, 5), 25, 25);
    Canny(ofusc, borders, fator, 3 * fator);
    imwrite("canny.jpg", borders);

    for (int i = 0; i < imagemArte.rows; i++) {
        for (int j = 0; j < imagemArte.cols; j++) {
            if (borders.at<uchar>(i, j) == 255) {
                funcCircle(imagemArte, imagemArte, cor, 2, i, j);
            }
        }
    }

    imwrite("arte.jpg", imagemArte);
    imshow("arte", imagemArte);
    waitKey();
    return 0;
}
