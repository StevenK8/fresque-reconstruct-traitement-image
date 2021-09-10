#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <iterator>
#include <sstream>

using namespace std;
using namespace cv;

Mat rotate(Mat src, double angle)
{
	Mat dst;
	Point2f pt(src.cols / 2., src.rows / 2.);
	Mat r = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(src, dst, r, Size(src.cols, src.rows), INTER_LINEAR, BORDER_CONSTANT, Scalar(205, 203, 206));
	return dst;
}

vector<vector<string>> readText(string filename)
{
	string line;
	ifstream fin(filename);
	assert(fin);
	vector<vector<string>> list;
	while (getline(fin, line))
	{
		stringstream ss(line);
		istream_iterator<string> begin(ss);
		istream_iterator<string> end;

		vector<string> frags(begin, end);
		list.push_back(frags);
	}

	return list;
}

void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
				  cv::Mat &output, cv::Point2i location)
{
	background.copyTo(output);

	for (int y = std::max(location.y, 0); y < background.rows; ++y)
	{
		int fY = y - location.y;

		if (fY >= foreground.rows)
			break;

		for (int x = std::max(location.x, 0); x < background.cols; ++x)
		{
			int fX = x - location.x;

			if (fX >= foreground.cols)
				break;

			double opacity = ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3]) / 255.;

			for (int c = 0; opacity > 0 && c < output.channels(); ++c)
			{
				unsigned char foregroundPx =
					foreground.data[fY * foreground.step + fX * foreground.channels() + c];
				unsigned char backgroundPx =
					background.data[y * background.step + x * background.channels() + c];
				output.data[y * output.step + output.channels() * x + c] =
					backgroundPx * (1. - opacity) + foregroundPx * opacity;
			}
		}
	}
}

int main(int argc, char **argv)
{

	// Mat imageIn = imread( argv[1], IMREAD_GRAYSCALE );
	Mat fragment;
	Rect ROI;
	Mat DispImage = Mat::zeros(Size(1707, 775), CV_8UC4);
	DispImage.setTo(Scalar(192, 194, 191));
	// Mat background(DispImage.rows,DispImage.cols, CV_8UC3, Scalar(205, 203, 206));
	// Rect ROI2(0,0,(int)( DispImage.rows ), (int)( DispImage.cols ));
	// background.copyTo(DispImage);

	vector<vector<string>> grid = readText("fragments.txt");
	for (auto row : grid)
	{
		fragment = imread("./frag_eroded/frag_eroded_" + row[0] + ".png", CV_8UC4);
		if (!fragment.data)
		{
			cout << "Could not open or find the image" << std::endl;
			return -1;
		}
		// Rect ROI(stoi(row[1]), stoi(row[2]),(int)( fragment.rows ), (int)( fragment.cols ));
		// Mat temp;
		// resize(fragment,temp, Size(ROI.width, ROI.height));
		fragment = rotate(fragment, stod(row[3]));
		overlayImage(DispImage, fragment, DispImage, cv::Point(stoi(row[1]), stoi(row[2])));
		// temp.copyTo(DispImage(ROI));
	}

	imshow("Display window", DispImage);
	resizeWindow("Display window", 1707, 775);

	waitKey(0);

	return 0;
}
