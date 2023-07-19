#include <iostream>
#include <opencv2/opencv.hpp>

void printmask(cv::Mat& m);
void applyFilters(cv::Mat& frame, cv::Mat& result, const cv::Mat& mask, bool absolut);

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Uso: " << argv[0] << " <caminho_para_imagem>\n";
    return -1;
  }

  cv::Mat frame = cv::imread(argv[1]);
  if (frame.empty()) {
    std::cout << "Imagem não encontrada ou não pode ser lida\n";
    return -1;
  }

  float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                   0.1111, 0.1111, 0.1111, 0.1111};
  float gauss[] = {0.0625, 0.125,  0.0625, 0.125, 0.25,
                   0.125,  0.0625, 0.125,  0.0625};
  float horizontal[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  float vertical[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
  float laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
  float laplacianOfGaussian[] = {0.0625, 0.125,  0.0625, 0.125, 0.25,
                                 0.125,  0.0625, 0.125,  0.0625, -1, -1, -1, -1, -8, -1, -1, -1, -1};

  cv::Mat mask(3, 3, CV_32F);
  cv::Mat result, resultLoG;
  int absolut = 1;  // calcs abs of the image
  char key = 0;

  cv::namedWindow("filtroespacial", cv::WINDOW_NORMAL);
  cv::namedWindow("original", cv::WINDOW_NORMAL);

  mask = cv::Mat(3, 3, CV_32F, media);

  while (key != 27) {
    applyFilters(frame, result, mask, absolut);
    cv::GaussianBlur(result, resultLoG, cv::Size(3, 3), 0);
    cv::Laplacian(resultLoG, resultLoG, CV_8U, 3, 1, 0, cv::BORDER_DEFAULT);

    cv::imshow("original", frame);
    cv::imshow("filtroespacial", result);
    cv::imshow("laplacian_of_gaussian", resultLoG);
    cv::imwrite("original.png", frame);
    cv::imwrite("filtroespacial.png", result);
    cv::imwrite("laplacian_of_gaussian.png", resultLoG);


    key = (char)cv::waitKey(10);
    switch (key) {
      case 'a':
        absolut = !absolut;
        break;
      case 'm':
        mask = cv::Mat(3, 3, CV_32F, media);
        printmask(mask);
        break;
      case 'g':
        mask = cv::Mat(3, 3, CV_32F, gauss);
        printmask(mask);
        break;
      case 'h':
        mask = cv::Mat(3, 3, CV_32F, horizontal);
        printmask(mask);
        break;
      case 'v':
        mask = cv::Mat(3, 3, CV_32F, vertical);
        printmask(mask);
        break;
      case 'l':
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        printmask(mask);
        break;
      case 'b':
        mask = cv::Mat(3, 3, CV_32F, laplacianOfGaussian);
        printmask(mask);
        break;
      default:
        break;
    }
  }

  return 0;
}

void printmask(cv::Mat& m) {
  for (int i = 0; i < m.size().height; i++) {
    for (int j = 0; j < m.size().width; j++) {
      std::cout << m.at<float>(i, j) << ",";
    }
    std::cout << "\n";
  }
}

void applyFilters(cv::Mat& frame, cv::Mat& result, const cv::Mat& mask, bool absolut) {
  cv::Mat framegray, frame32f, frameFiltered;
  cv::cvtColor(frame, framegray, cv::COLOR_BGR2GRAY);
  framegray.convertTo(frame32f, CV_32F);
  cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask, cv::Point(1, 1), 0);
  if (absolut) {
    frameFiltered = cv::abs(frameFiltered);
  }
  frameFiltered.convertTo(result, CV_8U);
}