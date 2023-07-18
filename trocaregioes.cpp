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
  
  int x = image.rows;
  int y = image.cols;
  
  Mat temp = image.clone();
  
  Rect region1(0, 0, y/2, x/2);
  Rect region2(y/2, 0, y/2, x/2);
  Rect region3(0, x/2, y/2, x/2);
  Rect region4(y/2, x/2, y/2, x/2);
  
  Mat roi1 = temp(region1);
  Mat roi2 = temp(region2);
  Mat roi3 = temp(region3);
  Mat roi4 = temp(region4);

  roi3.copyTo(image(region2));  // R3 -> R2
  roi4.copyTo(image(region1));  // R4 -> R1
  roi1.copyTo(image(region4));  // R1 -> R4
  roi2.copyTo(image(region3));  // R2 -> R3
  
  imshow("janela", image);
  waitKey(0);
  
  imwrite("trocaregioes.png", image);

  return 0;
}
