#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <iterator>
#include <sstream>


using namespace std;
using namespace cv;

Mat rotate(Mat src, double angle)   //rotate function returning mat object with parametres imagefile and angle    
{
    Mat dst;      //Mat object for output image file
    Point2f pt(src.cols/2., src.rows/2.);          //point from where to rotate    
    Mat r = getRotationMatrix2D(pt, angle, 1.0);      //Mat object for storing after rotation
    warpAffine(src, dst, r, Size(src.cols, src.rows));  ///applie an affine transforation to image.
    return dst;         //returning Mat object for output image file
}

vector<vector<string>> readText(string filename){
   string line;
   ifstream fin( filename ); assert( fin );
   vector<vector<string>> list;
   while( getline( fin, line ) )
   {
	stringstream ss(line);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;

	vector<string> frags(begin,end);
	// copy(frags.begin(), frags.end(), ostream_iterator<string>(cout, "\n"));
	list.push_back(frags);
   }
   
   return list;
}


int main(int argc, char** argv){

	Mat imageIn = imread( argv[1], IMREAD_GRAYSCALE );
	Mat fragment;
	Rect ROI;
	Mat DispImage = Mat::zeros(Size(1920,900), CV_8UC3);
	
	vector<vector<string>> grid = readText("fragments.txt");
	for ( auto row : grid )
	{
		fragment = imread( "./frag_eroded/frag_eroded_"+row[0]+".png" );
		if(! fragment.data )
    	{
        	cout <<  "Could not open or find the image" << std::endl ;
        	return -1;
   		}
 		Rect ROI(stoi(row[1]), stoi(row[2]),(int)( fragment.rows ), (int)( fragment.cols ));
		Mat temp;
		resize(fragment,temp, Size(ROI.width, ROI.height));
		temp = rotate(temp, stod(row[3]));
		temp.copyTo(DispImage(ROI));

	}
	
	imshow( "Display window", DispImage );  
	          
	waitKey(0);       	
	
	return 0;
}
