/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
This is a revised circle fitting algorithm of the original algorithm, by Nikolai Chernov

The original algorithm fits a circle to a given (known) set of data points (in 2D).
It is an algebraic fit, based on the journal article:

A. Al-Sharadqah and N. Chernov, "Error analysis for circle fitting algorithms",
Electronic Journal of Statistics, Vol. 3, pages 886-911, (2009)

In contribution to the original algorithm, we have added functions for
circle fitting on circular objects, in live-stream 2D-images.

THE REVISED ALGORITHM HAS THE FOLLOWING IMPLEMENTATION STEPS
Image acquisition-> Data points extraction, using Canny edge detection-> Gathering of data points, using arrays
-> Circle fitting onto data points-> Printing of the fit circles arc and radius information, onto captured frames

IMPORTANT INFORMATION AND NOTES ABOUT OUR ADDED FUNCTIONS (ALGORITHM STEPS):

* Live-stream image acquisition, for imaging of a circular object

* Canny edge detection- to extract edges (data points) of a circular objects arc.
x-coordinate of data points are stored in "XiData" (allocated array)
y-coordinate of data points are stored in "YiData" (allocated array)
The number of data points extracted, is stored in "NrOfPoints" (an integer)

The three variables "XiData", "YiData" and "NrOfPoints" are sent as arguments
to the circle fitting algorithm, by Nikolai Chernov, which fits a circle to the data points.
The fit circles parameters can be accessed by the class object FitCircle, where:

FitCircle.a - gives the "X-coordinate" of the fit circles center
FitCircle.b - gives the "Y-coordinate" of the fit circles center
FitCircle.r - gives the "radius" of the fit circle
FitCircle.s - gives the "the root mean square error" (the estimate of sigma)
FitCircle.j - gives the total number of "iterations"

* Printing of the fit circles arc onto captured colored frames-> indicates visually how well the fit circle is.
Further, the fit circles radius is also printed onto the frames.
The radius is given both in number of pixels and real-word value.
For real-world radius, user has to estimate the pixel size (in mm) and store it into the variable PixelSizeInMilimeter

Original algorithm by Nikolai Chernov  (September 2012)
Revised by Sohran Eliassi (December 2016)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

//-----------------------------------------------APPLICATION STARTS HERE-------------------------------------------------

// INCLUDE HEADERFILES
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

#include "mystuff.h"
#include "data.h"
#include "circle.h"
#include "Utilities.cpp"
#include "CircleFitByHyper.cpp"
#include "PrintFitCirclesArc.h"
#include "PrintFitCirclesArc.cpp"

using namespace cv;
using namespace std;


//--------------------------------------------------PROGRAM STARTS HERE--------------------------------------------------
int main()
{
	// create a file-stream object
	ofstream myfile1, myfile2;

	// Access camera and set frame resolution and frame rate
	VideoCapture cap(0); // open the camera, using id 1
	if (!cap.isOpened())  // check if camera is opened, if not- Exit program!
		return -1;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	cap.set(CV_CAP_PROP_FPS, 1); // NOTE: might not have effect on some cameras!

	//Used for resizing (downscaling) of the output frame to be displayed
	//Downscale to atleast screen size when output frame size (camera resolution) is larger than screen size
	Size size(1280, 720);

	//Matrix, for storing the resized output frame
	Mat dst;//dst image

	// Matrix, for storing the incoming frames from the camera
	Mat frame;

	// Matrix, for storing the binary frame (i.e. the resulting frame after applying canny edge detector)
	Mat edges;

	// The location to store the binary frame 
	string EdgeLocation = "C:/Users/Sohran/Downloads/Edges.jpg";

	// The location to store the output frame 
	string OutputFrameLocation = "C:/Users/Sohran/Downloads/OutputFrame.jpg";

	//Variables for defining the area of the binary frame to search for edges (data points)
	unsigned short int xArea1 = 0;
	unsigned short int xArea2 = 1279;
	unsigned short int yArea1 = 0;
	unsigned short int yArea2 = 719;

	// flag for detecting the first data points xi- coordinate value
	bool check = true;

	// variables for storing the first & last data points xi- coordinate value
	unsigned short int FirstDataPoint, LastDataPoint = 0;

	// varable for storing the number of data points
	unsigned short int NrOfPoints = 0;

	//Pointers to point to allocated memory, holding the data points (xi,yi) coordinate values
	reals *XiData, *YiData = NULL;

	// Create an object of the circle fitting class
	// This object will have member variables holding the fit circles parameters
	Circle FitCircle;

	// size of an pixel, in milimeter-scale
	// used for converting the fit circles radius to real-world value in meter-scale
	const double PixelSizeInMilimeter = 0.09316770186;

	// variable used for storing the fit circles radius in meter-scale
	double RadiusMeter = 0;

	// The file name of the files that will hold radius and time information
	myfile1.open("Radius.txt");
	myfile2.open("Radius&Time.txt");

	// create an object of the class PrintFitCirclesArc
	PrintFitCirclesArc PrintFitCirclesArcOnFrame;

	//Nr of decimal digits to output when cout is called
	cout.precision(4);

	//---------------------------------SIMULTANEOUS CIRCLE FITTING PROCESS STARTS HERE----------------------------------
	while (true)
	{
		clock_t tStart = clock();					 // Start time - used for calculating the algorithm execution time

		cap >> frame;								 // Capture a frame from camera
		//cv::flip(frame, frame, 0);				 // Un-comment if input frame needs to flipped
		cvtColor(frame, edges, CV_BGR2GRAY);		 // convert frame to grayscale
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);// Blurr frame to reduce noise
		Canny(edges, edges, 100, 150, 3);			 // Perform canny edge detection
		imwrite(EdgeLocation, edges);				 // Save resulting (binary) frame to hard drive

		//Search for edges (data points) within the defined area of the binary frame
		//Store first & last edge큦 (data point큦) x[i]- coordinate value
		for (int x = xArea1; x <= xArea2; x++)
		{
			for (int y = yArea1; y <= yArea2; y++)
			{
				if (edges.at<uchar>(y, x) == 255)    // check if pixel is an edge (data point)
				{
					if (check == true)
					{
						FirstDataPoint = x;			 // Store first data point큦 x[i]- coordinate value
						check = false;
					}
					LastDataPoint = x;				 // Store last data point큦 x[i]- coordinate value
				}
			}
		}
		check = true;								 // set check back to true, necessary for next processed frame

		NrOfPoints = LastDataPoint - FirstDataPoint + 1; //Store the number of data points

		//Allocate memory for arrays that will store data points coordinate values
		//Allocated memory size depends on the number of data points (NrOfPoints)
		XiData = new reals[NrOfPoints];
		YiData = new reals[NrOfPoints];

		//Search again for edges (data points) within defined area of the binary frame
		//This time, store all edges (data points) coordinate values into the allocated arrays
		for (int x = xArea1; x <= xArea2; x++)
		{
			for (int y = yArea1; y <= yArea2; y++)
			{
				if (edges.at<uchar>(y, x) == 255) // check if pixel is an edge (data point)
				{
					XiData[x - FirstDataPoint] = x; // Array storing x[i]- coordinate values
					YiData[x - FirstDataPoint] = y; // Array storing y[i]- coordinate values
				}
			}
		}

		//Pass arrays (data points) and NrOfPoints as argument to the class object "data1" (runs a constructor)
		//Fit a circle to the data points
		//The class object "FitCircle" have member variables holding the fit circles parameters
		Data data1(NrOfPoints, XiData, YiData);
		FitCircle = CircleFitByHyper(data1);

		//Delete allocated arrays (data points) - not needed anymore -> more memory efficient
		delete XiData;
		delete YiData;

		//Convert the fit circle큦 radius from number of pixels to meter-scale
		RadiusMeter = ((PixelSizeInMilimeter * FitCircle.r) / 1000);

		// Print the fit circle큦 arc onto the original (colored) frame
		PrintFitCirclesArcOnFrame.PrintArc(FitCircle.a, FitCircle.b, FitCircle.r, frame);

		//Print the radius information (number of pixels and meter-scale) onto the original (colored) frame
		ostringstream strs1;
		ostringstream strs3;
		ostringstream strs5;
		strs1 << "Radius px = " << FitCircle.r;
		strs3 << "Radius meter = " << RadiusMeter;
		string str1 = strs1.str();
		string str3 = strs3.str();
		putText(frame, str1, cvPoint(10, 20),
			FONT_HERSHEY_COMPLEX_SMALL, 1.3, cvScalar(0, 0, 255), 1, CV_AA);
		putText(frame, str3, cvPoint(10, 50),
			FONT_HERSHEY_COMPLEX_SMALL, 1.3, cvScalar(0, 0, 255), 1, CV_AA);

		// Save output frame to hard drive
		imwrite(OutputFrameLocation, frame);

		// Resize output frame and display it
		resize(frame, dst, size);//resize image
		imshow("object", dst);
		waitKey(1);

		// Output the algorithm execution time
		cout << "Algorithm speed: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
	}
}
