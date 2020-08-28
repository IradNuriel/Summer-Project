#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "Constants.h"
#include <time.h>
#include <ostream>

class Camera {
public:
	Camera(int num=0);
	Camera(const Camera& other);
	Camera(cv::Mat_<float> cameraMatrix, cv::Mat distortionCoeff, cv::Mat_<float> transformation,int cameraNum);
	friend std::ostream& operator<<(std::ostream& out, const Camera& camera);
	//input: references to a cv::Mat_<double>, and two std::vector<cv::Mat> references, the return value will be in the parameters you've pass in 
	void getCalibrationParameters(cv::Mat_<float>& cameraMatrixOut, cv::Mat_<float>& cameraMarixInverse, cv::Mat& disstortionVec) const;
	//input: std::vector<cv::Mat_<float>> reference, the return value will be in the parameters you've pass in
	void getCameraExtrinsicParam(cv::Mat_<float>& transformationOut) const;
	static Camera& no_camera();
	bool operator!=(const Camera& other) const;
private:
	//the number of the camera(when cameras sorted from 1 in the most left to n in the most right)
	int cameraNum;
	//camera intrinsic parameters
	cv::Mat_<float> cameraMatrix;//camera matrix
	cv::Mat distortionCoeff;
	//camera pose parameters
	cv::Mat_<float> meanRelativeTransformation;
};


#endif // !_CAMERA_H_
