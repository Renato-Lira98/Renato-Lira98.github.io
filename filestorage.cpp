#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    int side = 256;
    int periodos = 4;

    Mat image(side, side, CV_8UC1);

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            float value = 127 * sin(2 * CV_PI * periodos * j / side) + 128;
            image.at<uchar>(i, j) = static_cast<uchar>(value);
        }
    }

    string filenameYML = "senoide.yml";
    string filenamePNG = "senoide.png";

    FileStorage fs(filenameYML, FileStorage::WRITE);
    fs << "mat" << image;
    fs.release();

    imwrite(filenamePNG, image);

    FileStorage fs_read(filenameYML, FileStorage::READ);
    fs_read["mat"] >> image;
    fs_read.release();

    imshow("image", image);
    waitKey();

    return 0;
}

