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
	Camera();
	Camera(const Camera& other);
	Camera(cv::Mat_<float> cameraMatrix, cv::Mat distortionCoeff, cv::Mat_<float> transformation);
	Camera(int numOfImages,std::string directoryPath,int chessBoardRows=7,int chessBoardCols=4);
	friend std::ostream& operator<<(std::ostream& out, const Camera& camera);
	//input: references to a cv::Mat_<double>, and two std::vector<cv::Mat> references, the return value will be in the parameters you've pass in 
	void getCalibrationParameters(cv::Mat_<float>& cameraMatrixOut, cv::Mat_<float>& cameraMarixInverse, cv::Mat& disstortionVec);
	//input: std::vector<cv::Mat_<float>> reference, the return value will be in the parameters you've pass in
	void getCameraExtrinsicParam(cv::Mat_<float>& transformationOut);
	std::string fullFileName(unsigned int i);
	bool fileExists(std::string fileName);


private:
	void calcAllCameraParameters();
	//initialization functions
	void calcMeanRelativeCameraPoseTransformation();
	void calcRelativeCameraPoseTransformation();
	void calcGlobalCameraPoseTransformation();
	void calcCameraIntrinsicParameters();
	std::vector<cv::Point3f> dynamicallyCreateObjp();
	//size of chessboard that the calibration performed on.
	int chessBoardRows;
	int chessBoardCols;
	//num of images of the chessboard
	int numOfImages;
	//path to the images of the chessboard
	std::string directoryPath;
	//camera intrinsic parameters
	cv::Mat_<float> cameraMatrix;//camera matrix
	std::vector<cv::Mat> rvecs;//camera rotation
	std::vector<cv::Mat> tvecs;//camera translation
	cv::Mat distortionCoeff;
	//global camera pose parameters
	std::vector<cv::Mat_<float>> globalCameraTransformation;//glabal camera transformation
	//relative camera pose parameters
	std::vector<cv::Mat_<float>> relativeCameraTransformation;//relative camera transformation
	//mean of all relative camera transformation
	cv::Mat_<float> meanRelativeTransformation;
};


#endif // !_CAMERA_H_
