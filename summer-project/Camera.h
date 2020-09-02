/////////////////////////////////
// file written by Irad Nuriel,//
// written in 27/08/2020       //
/////////////////////////////////

#ifndef _CAMERA_H_
#define _CAMERA_H_

//includes of opencv stuff
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"


//data types includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <ostream>


//our file includes
#include "Constants.h"



/*class Camera, contains all the parameters of the camera(camera matrix, distortion vector, pose matrix and camera number)*/
class Camera {
public:

	//default constructor
	Camera(int num=0);

	//copy constructor
	Camera(const Camera& other);

	//real constructor
	Camera(cv::Mat_<float> cameraMatrix, cv::Mat distortionCoeff, cv::Mat_<float> transformation,int cameraNum);
	
	//operator <<, only for debuging
	friend std::ostream& operator<<(std::ostream& out, const Camera& camera);
	
	/*getCalibrationParameters(params),
	  input: references to a cv::Mat_<double>, and two std::vector<cv::Mat> references; 
	  the return value will be in the parameters you've pass in*/
	void getCalibrationParameters(cv::Mat_<float>& cameraMatrixOut, cv::Mat_<float>& cameraMarixInverse, cv::Mat& disstortionVec) const;
	
	/*getCameraExtrincParam,
	  input: std::vector<cv::Mat_<float>> reference; 
	  the return value will be in the parameters you've pass in*/
	void getCameraExtrinsicParam(cv::Mat_<float>& transformationOut) const;

	//no_camera(), a singelton! represent a no camera, so we will be able to support old LineBuilder
	static Camera& no_camera();

	//operator != , compare cameras based on if the camera numbers of both are different
	bool operator!=(const Camera& other) const;
private:
	//the number of the camera(when cameras sorted from 1 in the most left to n in the most right)
	int cameraNum;
	//camera intrinsic parameters:
	
	//camera matrix
	cv::Mat_<float> cameraMatrix;

	//distortion vector
	cv::Mat distortionCoeff;
	
	//camera pose parameter
	cv::Mat_<float> meanRelativeTransformation;
};


#endif // !_CAMERA_H_
