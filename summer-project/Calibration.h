/////////////////////////////////
// file written by Irad Nuriel,//
// written in 27/08/2020       //
/////////////////////////////////


#ifndef _CALIBRATION_
#define _CALIBRATION_

//opencv includes
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/ccalib/multicalib.hpp"

//data structures includes
#include <vector>
#include <fstream>


//our code includes
#include "Camera.h"
#include "Constants.h"
#include "Utilities.h"


//namedpace Calibration, contains all the functions which related to cameras calibration.
namespace Calibration {
	
	/*multiCalibrate(params), 
	take number of cameras, file to write the cameras info into, physical width and hight of the pattern;
	and write the final instrincs and extrincs parameters of each camera to the output file(output file should be an xml file).*/
	void multiCalibrate(int nCamera, std::string outputFileName, int patternWidth, int patternHeight, bool showProcess = false);

	/*parseParamsFile(params), 
	take a name of xml file which contains all the camera info, 
	and return a vector af the cameras.*/
	std::vector<Camera> parseParamsFile(std::string inputFileName);

	/*generateImgList(params), 
	take a name of xml file which will contains list of all images and number of cameras; 
	and make a xml file named `inFile` that contains the name of all the calibration images.*/
	void generateImgList(std::string inFile, int nCamera);

	/*getCalibration(params), this is the only function that should be called from main.
	take a boolean indicating whether or not we need a calibration, path to the final calibration file, its name, and a reference to an int; 
	returns a vector of camera objects, and the number of cameras returned via the int reference*/
	std::vector<Camera> getCalibration(bool needCalibration, std::string inPath, std::string camData,int& nCamera);
	
	/*calcNCamera(params),
	take a path to the calibration images folder;
	and calculate and retrun the number of cameras.*/
	int calcNCamera(std::string inPath);

	/*getImageVector(params),
	take a path to the calibration images folder and camera index;
	and return a vector of images that was taken from camera number `camera index`*/
	std::vector<cv::Mat> getImageVector(std::string inPath, int cameraIdx);
}








#endif // !_MULTI_CALIBRATION
