#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void printmask(Mat &m) {
	for (int i = 0; i < m.size().height; i++) {
		for (int j = 0; j < m.size().width; j++) {
			cout << m.at<float>(i, j) << ",";
		}
		cout << "\n";
	}
}

int main() {
	float media[] = { 0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
				   0.1111, 0.1111, 0.1111, 0.1111 };
	float gauss[] = { 0.0625, 0.125,  0.0625, 0.125, 0.25,
					 0.125,  0.0625, 0.125,  0.0625 };
	float horizontal[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	float vertical[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
	float laplacian[] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };
	float boost[] = { 0, -1, 0, -1, 5.2, -1, 0, -1, 0 };
	float laplacianOfGaussian[] = { 0,0,1,0,0,
									0,1,2,1,0,
									1,2,-16,2,1,
									0,1,2,1,0,
									0,0,1,0,0 };
	char diretorio[1000], key;
	Mat imagem, framegray, frame32f, frameFiltered,
		mask(3, 3, CV_32F), result;
	int absolut;

	do {
		cout << "Digite a localizacao da imagem: " << endl;
		cout << "EX.: C:\\User\\Desktop\\<nomedaimagem>.<png ou jpeg>" << endl;
		cin >> diretorio;

		imagem = imread(diretorio);

		if (!imagem.data) {
			cout << "Imagem nao encontrada!" << endl;
			cout << "Verifique se a escrita esta correta." << endl;
		}
	} while (!imagem.data);

	cvtColor(imagem, framegray, COLOR_BGR2GRAY);
	imshow("original", framegray);

	framegray.convertTo(frame32f, CV_32F);

	mask = Mat(3, 3, CV_32F, media);
	absolut = 1;

	for (;;) {
		filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1, 1), 0);

		if (absolut) {
			Mat frameAbs;
			absdiff(frameFiltered, Scalar(0), frameAbs);
			frameFiltered = frameAbs;
		}

		frameFiltered.convertTo(result, CV_8U);
		imshow("filtroespacial", result);

		key = (char)waitKey(0);
		if (key == 27) break;
		switch (key) {
		case 'a':
			absolut = !absolut;
			break;
		case 'm':
			mask = Mat(3, 3, CV_32F, media);
			printmask(mask);
			break;
		case 'g':
			mask = cv::Mat(3, 3, CV_32F, gauss);
			printmask(mask);
			break;
		case 'l':
			mask = cv::Mat(3, 3, CV_32F, laplacian);
			printmask(mask);
			imwrite("laplacian.png", result);
			break;
		case 'o':
			mask = cv::Mat(5, 5, CV_32F, laplacianOfGaussian);
			printmask(mask);
			imwrite("laplacianofgaussian.png", result);
			break;
		default:
			break;
		}
	}

	return 0;
}