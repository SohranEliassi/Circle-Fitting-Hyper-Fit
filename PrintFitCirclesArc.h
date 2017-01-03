// Create a class function for computing a fit circle´s arc y-coordinate values
// Print the fit circle´s arc onto an original (colored) frame

#pragma once
#ifndef PRINTFITCIRCLESARC_H
#define PRINTFITCIRCLESARC_H
#endif;

#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class PrintFitCirclesArc
{
public:
	void PrintArc(double &centerX, double &centerY, double &Radius, Mat &frame);
};

