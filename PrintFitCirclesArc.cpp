#include "PrintFitCirclesArc.h"

// A function for computing a fit circle큦 arc y-coordinate values
// Print the fit circle큦 arc onto an original (colored) frame
void PrintFitCirclesArc::PrintArc(double &centerX, double &centerY, double &Radius, Mat &frame)
{
	int *yData = new int[frame.cols + 1];
	for (int i = 0; i <= frame.cols; i++)
	{
		// Calculate & store the fit circle큦 arc y- coordinate values
		yData[i] = (centerY - double(sqrt(((Radius)*(Radius)) - ((centerX - i)*(centerX - i))))) + 0.5;

		//Print the fit circle큦 arc onto the original (colored) frame
		if (((i >= 0) && (i <= frame.cols)) && (((yData[i]) >= 0) && ((yData[i]) <= frame.rows)))
		{
			//Use GREEN COLOR for the fit circle큦 arc
			frame.at<Vec3b>((yData[i]), i)[0] = 0;
			frame.at<Vec3b>((yData[i]), i)[1] = 255;
			frame.at<Vec3b>((yData[i]), i)[2] = 0;
		}
	}
	delete yData;
}
