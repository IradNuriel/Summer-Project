/////////////////////////////////
// file written by Lun Surd,   //
// written in 27/08/2020       //
/////////////////////////////////
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <string>
#include "Constants.h"


//namespace Utilities, all the useful things that are used in everywhere
namespace Utilities {

	/*imgFileName(parames),
	  get full path to an image based on our conventions of folderPath/cameraID_frameID.suffix*/
	std::string imgFileName(std::string folderPath, int cameraID, int frameID, std::string suffix);

	/*fileExist(params),
	  check if a file with certain name exist in the fastest way possible*/
	bool fileExists(std::string fileName);

	/*tryLoad(params),
	  try to laod the image folderPath/cameraID_frameID with all possible suffices*/
	cv::Mat tryLoad(const std::string folderPath, const int cameraID, const int frameID);

	/*prefixExist(params),
	  check if a file with certain prefix exist(check with all sufficies) in the fastest way possible;
	  and return the id of the existing suffix(in Constants::SUFFICES) if one founded*/
	bool prefixExist(const std::string folderPath, const int cameraID, const int frameID,int& suffix);
}

#endif