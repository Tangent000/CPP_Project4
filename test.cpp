#include <iostream>
// #include <opencv4/opencv2/core/mat.hpp>
#include "cnn.hpp"
using namespace std;
int * data1 = new int[12]{1,2,3,4,5,6,7,8,9,10,11,12};
float * data2 = new float[8]{1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8};
int main()
{
    Mat * mat1 = new Mat_int(3,4,1,data1);
    Mat * mat2 = new Mat_float(2,2,2,data2);
    mat1->printMat();
    mat2->printMat();
    return 0;
}