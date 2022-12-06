#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;
using namespace cv;

void AffineRotate(int i, int j, float xx, float yy, float phi, int x_c, int y_c) {
    xx = (y_c - i) * sin(phi) + (j - x_c) * cos(phi) + x_c,
        yy = (j - x_c) * sin(phi) + (i - y_c) * cos(phi) + y_c;
}
int NewSize(float lh, float lr, float ld, float rd) {
    float maximum = max(lh, max(lr, max(ld, rd)));
    float minimum = min(lh, min(lr, min(ld, rd)));
    return ceil(abs(maximum) + abs(minimum));
}
void First_step(Mat src, float phi) {

    int x_c = (src.rows / 2) - 1;
    int y_c = (src.cols / 2) - 1;

    float top_left_x = 0.0; float top_left_y = 0.0; float top_right_x = 0.0; float top_right_y = 0.0;
    float bottom_left_x = 0.0; float  bottom_left_y = 0.0; float bottom_right_x = 0.0;float bottom_right_y = 0.0;

    AffineRotate(0, 0, top_left_x, top_left_y, phi, x_c, y_c);
    AffineRotate(0, src.rows - 1, top_right_x, top_right_y, phi, x_c, y_c);
    AffineRotate(src.cols - 1, 0, bottom_left_x, bottom_left_y, phi, x_c, y_c);
    AffineRotate(src.cols - 1, src.rows - 1, bottom_right_x, bottom_right_y, phi, x_c, y_c);


    int nWidth = NewSize(top_left_x, top_right_x, bottom_left_x, bottom_right_x);
    int nHeight = NewSize(top_left_y, top_right_y, bottom_left_y, bottom_right_y);

}
void Second_step(Mat src, Mat res, int nWidth, int nHeight, float phi) {
    int Width = src.rows;
    int Height = src.cols;
    int x_c = (Width / 2) - 1;
    int y_c = (Height / 2) - 1;

    int dx = (nWidth / 2) - 1;
    int dy = (nHeight / 2) - 1;


    for (int i = 0; i < nHeight; i++) {
        for (int j = 0; j < nWidth; j++) {


            float yy = (j - dx) * sin(phi) + (i - dy) * cos(phi) + y_c;
            float xx = -(i - dy) * sin(phi) + (j - dx) * cos(phi) + x_c;

            if (yy >= 0 && xx >= 0 && xx < Width && yy < Height) {

                int y = (int)yy, x = (int)xx;
                float u = yy - y, v = xx - x;

                float b1 = (v - 1) * (v - 2) * (v + 1) * (u - 1) * (u - 2) * (u + 1) / 4,
                    b2 = -v * (v - 2) * (v + 1) * (u - 1) * (u - 2) * (u + 1) / 4,
                    b3 = -(v - 1) * (v - 2) * (v + 1) * u * (u - 2) * (u + 1) / 4,
                    b4 = v * (v - 2) * (v + 1) * u * (u - 2) * (u + 1) / 4,
                    b5 = -v * (v - 1) * (v - 2) * (u - 1) * (u - 2) * (u + 1) / 12,
                    b6 = -(v - 1) * (v - 2) * (v + 1) * u * (u - 1) * (u - 2) / 12,
                    b7 = v * (v - 1) * (v - 2) * u * (u - 2) * (u + 1) / 12,
                    b8 = v * (v - 2) * (v + 1) * u * (u - 1) * (u - 2) / 12,
                    b9 = v * (v - 1) * (v + 1) * (u - 1) * (u - 2) * (u + 1) / 12,
                    b10 = (v - 1) * (v - 2) * (v + 1) * u * (u - 1) * (u + 1) / 12,
                    b11 = v * (v - 1) * (v - 2) * u * (u - 1) * (u - 2) / 36,
                    b12 = -v * (v - 1) * (v + 1) * u * (u - 2) * (u + 1) / 12,
                    b13 = -v * (v - 2) * (v + 1) * u * (u - 1) * (u + 1) / 12,
                    b14 = -v * (v - 1) * (v + 1) * u * (u - 1) * (u - 2) / 36,
                    b15 = -v * (v - 1) * (v - 2) * u * (u - 1) * (u + 1) / 36,
                    b16 = v * (v - 1) * (v + 1) * u * (u - 1) * (u + 1) / 36;
          
                src.at<uchar>(j + i * nWidth) = (b1 * src.at<uchar>(x + y * Width) + b2 * src.at<uchar>(x + (y + 1) * Width) +
                    b3 * src.at<uchar>((x + 1) + (y + 0) * Width) + b4 * src.at<uchar>((x + 1) + (y + 1) * Width) +
                    b5 * src.at<uchar>((x + 0) + (y - 1) * Width) + b6 * src.at<uchar>((x - 1) + (y + 0) * Width) +
                    b7 * src.at<uchar>((x + 1) + (y - 1) * Width) + b8 * src.at<uchar>((x - 1) + (y + 1) * Width) +
                    b9 * src.at<uchar>((x + 0) + (y + 2) * Width) + b10 * src.at<uchar>((x + 2) + (y + 0) * Width) +
                    b11 * src.at<uchar>((x - 1) + (y - 1) * Width) + b12 * src.at<uchar>((x + 1) + (y + 2) * Width) +
                    b13 * src.at<uchar>((x + 2) + (y + 1) * Width) + b14 * src.at<uchar>((x - 1) + (y + 2) * Width) +
                    b15 * src.at<uchar>((x + 2) + (y - 1) * Width) + b16 * src.at<uchar>((x + 2) + (y + 2) * Width));
            }
        }
    }
}

int main(int argc, char* argv[])
{ 

    cv::Mat src,res;
    src = cv::imread("LABS/Peppers.png", cv::IMREAD_ANYDEPTH);           //reading image file in mat object
    float phi = 30;
    Mat res = src.clone();
    int NewWidth = 0;
    int NewHeight = 0;
    int nWidth = 0; 
    int nHeight = 0;
    First_step(src, phi);
    Second_step(src, src, nWidth, nHeight, phi);
    imshow("Input_Image", src);
    imshow("Bicubic", res);
    waitKey(0);                     //to exit press escape
    return 0;
}


