#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double alfa;
int center_slider = 0;
int center_slider_max = 100;

int alfa_slider = 0;
int alfa_slider_max = 100;

int top_slider = 0;
int top_slider_max = 100;

float media[] = { 1,1,1,1,1,1,1,1,1 };

Mat imagemOriginal, imagemEditada, blended;
Mat imageTop;

char TrackbarName[50];

void on_trackbar_blend(int, void*) {
    alfa = (double)alfa_slider / alfa_slider_max;
    addWeighted(imagemOriginal, alfa, imageTop, 1 - alfa, 0.0, blended);
    imshow("addweighted", blended);
}

void on_trackbar_change(int, void*) {
    imagemEditada.copyTo(imageTop);
    Size size = imagemEditada.size();
    int width = size.width;
    int height = size.height;
    int limit = top_slider * width / 100;
    int base = center_slider * width / 100;
    if (limit > 0) {
        if (base >= 0 && base <= height - limit) {
            Mat tmp = imagemOriginal(Rect(0, base, width, limit));
            tmp.copyTo(imageTop(Rect(0, base, width, limit)));
        }
        else {
            Mat tmp = imagemOriginal(Rect(0, height - limit, width, limit));
            tmp.copyTo(imageTop(Rect(0, height - limit, width, limit)));
        }
    }
    on_trackbar_blend(alfa_slider, 0);
}

int main() {
    char diretorio[1000];
    
    do {
        cout << "Digite o endereço da imagem" << endl; 
        cout << "C:\\Users\\User\\Desktop\...\\<nomedaimagem>.<extencao>" << endl;
        cin >> diretorio;

        imagemOriginal = imread(diretorio);

        if (!imagemOriginal.data) {
            cout << "Imagem nao encontrada!" << endl;
            cout << "Verifique se a escrita esta correta." << endl;
        }
    } while (!imagemOriginal.data);

    imagemEditada = imagemOriginal.clone();
    Mat aux, mask, mask1;

    mask = Mat(3, 3, CV_32F, media);
    scaleAdd(mask, 1 / 9.0, Mat::zeros(3, 3, CV_32F), mask1);
    swap(mask, mask1);
    imagemEditada.convertTo(aux, CV_32F);

    for (int i = 0; i < 10; i++) {
        filter2D(aux, aux, aux.depth(), mask, Point(1, 1), 0);
        aux = abs(aux);
        aux.convertTo(imagemEditada, CV_8UC3);
        imagemOriginal.copyTo(imageTop);
    }

    namedWindow("addweighted", 1);

    sprintf_s(TrackbarName, "Decaimento");
    createTrackbar(TrackbarName, "addweighted", &alfa_slider, alfa_slider_max, on_trackbar_blend);
    on_trackbar_blend(alfa_slider, 0);

    sprintf_s(TrackbarName, "Altura ");
    createTrackbar(TrackbarName, "addweighted", &top_slider, top_slider_max, on_trackbar_change);
    on_trackbar_change(top_slider, 0);

    sprintf_s(TrackbarName, "Posição");
    createTrackbar(TrackbarName, "addweighted", &center_slider, center_slider_max, on_trackbar_change);

    imwrite("imagemEditada.png", imagemEditada);

    waitKey(0);
    return 0;
}