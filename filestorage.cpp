#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

using namespace std; 
using namespace cv; 

int SIDE = 256;
int PERIODOS = 4;
int M_PI = 3.141516;


int main(int argc, char** argv) {
    cout << "INICIANDO..." << endl;

    stringstream ss_img, ss_yml;
    Mat image;

    ss_yml << "senoide-" << SIDE << ".yml";
    image = Mat::zeros(SIDE, SIDE, CV_32FC1);

    FileStorage fs(ss_yml.str(), FileStorage::WRITE);

    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            image.at<float>(i, j) = 127 * sin(2 * M_PI * PERIODOS * j / SIDE) + 128;
        }
    }

    fs << "mat" << image;
    fs.release();

    normalize(image, image, 0, 255, NORM_MINMAX);
    image.convertTo(image, CV_8U);
    ss_img << "senoide-" << SIDE << ".png";
    imwrite(ss_img.str(), image);

    cout << "Matriz da imagem png... " << endl;
    cout << image << endl;

    fs.open(ss_yml.str(), FileStorage::READ);
    fs["mat"] >> image;

    normalize(image, image, 0, 255, NORM_MINMAX);
    image.convertTo(image, CV_8U);

    imshow("image", image);
    waitKey();

    return 0;
}