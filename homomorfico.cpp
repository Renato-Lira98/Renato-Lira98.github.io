#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

void reciveVal(int** x, int** x2) {

	*x = new int;
	*x2 = new int;
}

void setVal(int* gl, int* gh, int* c, int* d0) {

	*gl = 10;
	*gh = 20;
	*c  = 60;
	*d0 = 70;
}

//	Funcoes para direcionar ponteiros
void on_gammaL (int, void*){}
void on_gammaH (int, void*){}
void on_c      (int, void*){}
void on_d0     (int, void*){}


void deslDFT(Mat& image) {

	Mat temp, A, B, C, D;

	image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
	int cols = image.cols / 2;
	int rows = image.rows / 2;

	// trocar regiões
	A = image(Rect(0, 0, cols, rows));
	B = image(Rect(cols, 0, cols, rows));
	C = image(Rect(0, rows, cols, rows));
	D = image(Rect(cols, rows, cols, rows));

	A.copyTo(temp);
	D.copyTo(A);
	temp.copyTo(D);

	B.copyTo(temp);
	C.copyTo(B);
	temp.copyTo(C);
}

void filterHomomo(Mat temp, int* gl, int* gh, int* c,
						int* d0, int dft_M, int dft_N) {

	float gl_temp, gh_temp, c_temp, d0_temp, aux, aux2; 
	gl_temp = *gl / 10;
	gh_temp = *gh / 10;
	c_temp  = *c / 10;
	d0_temp = *d0 / 10;

	for (int i = 0; i < temp.rows; i++) {
		for (int j = 0; j < temp.cols; j++) {

			aux  = (i - dft_M / 2) * (i - dft_M / 2)
				 + (j - dft_N / 2) * (j - dft_N / 2);
			aux2 = (1.0 - (float)exp(-(c_temp * aux / (d0_temp * d0_temp))));
			temp.at<float>(i, j) = (gh_temp - gl_temp)
				* aux2 + gl_temp;
		}
	}
}

int main() {

	Mat imaginaryInput, imageComplex, multsp,
		padded, filter, mag;
	Mat image, grayimage, temp;
	Mat_<float> realInput, zeros;
	vector<Mat> planos;
	int* gl, * gh, * c, * d0;
	int dft_M, dft_N;
	char diretorio[1000];

	cout << "Digite o endereço da imagem" << endl;
	cin >> diretorio;
	image = imread(diretorio, IMREAD_GRAYSCALE);

	reciveVal(&gl, &gh);
	reciveVal(&c , &d0);
	setVal(gl, gh, c, d0);

	imshow("Original", image);

	dft_M = getOptimalDFTSize(image.rows);
	dft_N = getOptimalDFTSize(image.cols);

	copyMakeBorder(image, padded, 0, dft_M - image.rows,
					0, dft_N - image.cols, BORDER_CONSTANT,
					Scalar::all(0));

	zeros = Mat_<float>::zeros(padded.size());
	imageComplex = Mat(padded.size(), CV_32FC2, Scalar(0));
	filter = imageComplex.clone();
	temp = Mat(dft_M, dft_N, CV_32F);
	namedWindow("Filtro", 1);

	createTrackbar("c", "Filtro", c, 100, on_c);
	createTrackbar("d0 ", "Filtro", d0, 100, on_d0);
	createTrackbar("gammaH", "Filtro", gh, 100, on_gammaH);
	createTrackbar("gammaL", "Filtro", gl, 100, on_gammaL);

	while (1) {

		on_c(*c, 0);
		on_d0(*d0, 0);
		on_gammaH(*gh, 0);
		on_gammaL(*gl, 0);

		copyMakeBorder(image, padded, 0, dft_M - image.rows,
			0, dft_N - image.cols, BORDER_CONSTANT,
			Scalar::all(0));

		planos.clear();
		realInput = Mat_<float>(padded);
		planos.push_back(realInput);
		planos.push_back(zeros);

		merge(planos, imageComplex);
		dft(imageComplex, imageComplex);
		deslDFT(imageComplex);
		filterHomomo(temp, gl, gh, c, d0, dft_M, dft_N);

		Mat comps[] = { temp, temp };
		merge(comps, 2, filter);
		mulSpectrums(imageComplex, filter, imageComplex, 0);

		deslDFT(imageComplex);
		idft(imageComplex, imageComplex);
		planos.clear();
		split(imageComplex, planos);
		normalize(planos[0], planos[0], 0, 1, NORM_MINMAX);
		imshow("Filtro Homomo", planos[0]);

		if (waitKey(10) == 27) break; 

	}
	return 0;

}