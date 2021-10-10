#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>

using namespace std;
using namespace cv;

Mat rotate(Mat src, double angle)
{
	Mat dst;
	Point2f pt(src.cols / 2., src.rows / 2.);
	Mat r = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(src, dst, r, Size(src.cols, src.rows));
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

void overlayImage(const Mat &background, const Mat &foreground,
				  Mat &output, Point2i location)
{
	background.copyTo(output);

	for (int y = max(location.y, 0); y < background.rows; ++y)
	{
		int fY = y - location.y;

		if (fY >= foreground.rows)
			break;

		for (int x = max(location.x, 0); x < background.cols; ++x)
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

int showImageFragments(string filename)
{
	// Mat imageIn = imread("Michelangelo_ThecreationofAdam_1707x775.jpg", IMREAD_GRAYSCALE );
	Mat fragment;
	Rect ROI;
	Mat DispImage = Mat::zeros(Size(1707, 775), CV_8UC4);
	DispImage.setTo(Scalar(193, 187, 182));

	vector<vector<string>> grid = readText(filename);
	for (auto row : grid)
	{
		fragment = imread("./frag_eroded/frag_eroded_" + row[0] + ".png", IMREAD_UNCHANGED);
		if (!fragment.data)
		{
			cout << "Could not open or find the image" << std::endl;
			return -1;
		}
		fragment = rotate(fragment, stod(row[3]));
		overlayImage(DispImage, fragment, DispImage, Point(stoi(row[1]) - fragment.cols / 2, stoi(row[2]) - fragment.rows / 2));
	}

	imshow("Display window", DispImage);
	resizeWindow("Display window", 1707, 775);

	waitKey(0);
	return 0;
}

bool isPlacedRight(int dx, int dy, int da, int x, int y, double a, int xb, int yb, double ab)
{
	if ((abs(x - xb) < dx) && (abs(y - yb) < dy) && (abs(a - ab) < da))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int calculSurface(string id)
{
	int pixelsSurface = 0;
	Mat fragment = imread("./frag_eroded/frag_eroded_" + id + ".png", IMREAD_UNCHANGED);
	if (!fragment.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return 0;
	}
	Mat_<uchar>::iterator itor = fragment.begin<uchar>();
	Mat_<uchar>::iterator itorEnd = fragment.end<uchar>();

	for (; itor != itorEnd; ++itor)
	{
		if ((*itor) > 0)
		{
			pixelsSurface++;
		}
	}

	// cvtColor(fragment, fragment, COLOR_BGR2HSV);

	return pixelsSurface;
}

float getScore(string filename, string reference, int dx, int dy, int da)
{
	float p = 0;
	int r = 0;
	int s = 0;
	int surfaceTotale = 0;
	int surfaceBonne = 0;
	int surfaceFausse = 0;
	vector<vector<string>> gridSolution = readText(filename);
	vector<vector<string>> gridReference = readText(reference);

	while (s < gridSolution.size() && r < gridReference.size())
	{
		surfaceTotale += calculSurface(gridSolution[s][0]);
		/*if (gridReference[r][0] < gridSolution[s][0])
		{
			r++;
		}
		else if (gridReference[r][0] > gridSolution[s][0])
		{
			s++;
			surfaceFausse += calculSurface(gridSolution[s][0]);
		}

		cout << gridReference[r][0] << " " << gridSolution[s][0] << " " << isPlacedRight(dx, dy, da, stoi(gridReference[r][1]), stoi(gridReference[r][2]), stod(gridReference[r][3]), stoi(gridSolution[s][1]), stoi(gridSolution[s][2]), stod(gridSolution[s][3])) << "\n";		
		r++;
		s++;*/
		if (gridReference[r][0] < gridSolution[s][0])
		{ // Vérifier que l'index est bon
			r++;
		}
		else if (gridReference[r][0] > gridSolution[s][0])
		{
			s++;
			surfaceFausse += calculSurface(gridSolution[s][0]);
		}
		/*else if (abs(stoi(gridReference[r][1])-stoi(gridSolution[s][1]))<=dx && abs(stoi(gridReference[r][2])-stoi(gridSolution[s][2]))<=dy && abs(stod(gridReference[r][3])-stod(gridSolution[s][3]))<=da){
			r++;
			s++;
			surfaceBonne+=surfaceFausse +=calculSurface(gridSolution[r][0]);
		}*/
		else if (isPlacedRight(dx, dy, da, stoi(gridReference[r][1]), stoi(gridReference[r][2]), stod(gridReference[r][3]), stoi(gridSolution[s][1]), stoi(gridSolution[s][2]), stod(gridSolution[s][3])))
		{
			r++;
			s++;
			surfaceBonne += calculSurface(gridSolution[r][0]);
		}
		else
		{
			s++;
			r++;
			surfaceFausse += calculSurface(gridSolution[s][0]);
		}
	}

	if (surfaceTotale != 0)
	{
		p = float(surfaceBonne - surfaceFausse) / float(surfaceTotale);
	}

	/* Imaginons on a :
			
						
			Solution.txt
						<index>		x		y			a (angle)
							1 		50 		35 		-154.1465
							2 		150 	25 		-20.14
			Fragment.txt
						1 51 35 -154.1465
						2 150 25 -20.14
			Il faut comparer chacun des éléments, sachant qu'on a un epsilon de précision (dx,dy,da)
			Pour comparer c'est simple on fait, pour le fragment 1
				abs(x_solution-x_fragment) si c'est inférieur à dx alors on est bon, si c'est supérieur alors c'est faux
				On fait pareil pour y et pour l'angle.

				Et si je comprend bien la précision p=(Surfacebonne-surfacefausse)/surfacetotal


		*/

	return p;
}

int main(int argc, char **argv)
{
	// showImageFragments("fragments.txt");
	cout << getScore("solution.txt", "fragments.txt", 1, 1, 1) * 100 << "%";

	return 0;
}
