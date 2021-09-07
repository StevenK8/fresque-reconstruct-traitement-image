#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>






using namespace std;
using namespace cv;



int main(int argc, char** argv){

	Mat imageIn = imread( argv[1], IMREAD_GRAYSCALE );

	Mat img1 = imread( "./frag_eroded/frag_eroded_0.png" );

	Mat DispImage = Mat::zeros(Size(1920,1080), CV_8UC3);
	
	
	if(! img1.data )                              // Check for invalid input
    {
        	cout <<  "Could not open or find the image" << std::endl ;
        	return -1;
   	}

	Rect ROI(10, 20, (int)( 200 ), (int)( 300 ));
    Mat temp;resize(img1,temp, Size(ROI.width, ROI.height));
    temp.copyTo(DispImage(ROI));
	
	
	imshow( "Display window", DispImage );             
	waitKey(0);       	
	
	return 0;
}



void ShowManyImages(string title) {
int size;
int i;
int x, y;
int nArgs = 5;

Mat DispImage = Mat::zeros(Size(1920,1080), CV_8UC3);

va_list args;


for (i = 0; i < nArgs; i++) {

    Mat img = va_arg(args, Mat);

    if(img.empty()) {
        printf("Invalid arguments");
        return;
    }

    Rect ROI(100, 130, (int)( 200 ), (int)( 300 ));
    Mat temp; resize(img,temp, Size(ROI.width, ROI.height));
    temp.copyTo(DispImage(ROI));
}


namedWindow( title, 1 );
imshow( title, DispImage);
waitKey();


va_end(args);
}