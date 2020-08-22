#include "Camera.h"

Camera::Camera(int numOfImages,std::string directoryPath,int chessBoardRows,int chessBoardClos) {
	this->chessBoardRows = chessBoardRows;
	this->chessBoardCols = chessBoardClos;
	this->numOfImages = numOfImages;
	this->directoryPath = directoryPath;
	this->calcAllCameraParameters();
}


void Camera::getCalibrationParameters(cv::Mat_<double>& cameraMatrixOut, std::vector<cv::Mat>& rvecsOut, std::vector<cv::Mat>& tvecsOut) {
	cameraMatrixOut = this->cameraMatrix;
	rvecsOut = this->rvecs;
	tvecsOut = this->tvecs;
}

void Camera::getCameraGlobalPoseTransformation(std::vector<cv::Mat_<float>>& transformationOur) {
	transformationOur = this->globalCameraTransformation;
}

void Camera::getCameraRelativePoseTransformation(std::vector<cv::Mat_<float>>& transformationOut) {
	transformationOut = this->relativeCameraTransformation;
}

void Camera::getCameraMeanRelativePoseTransformation(cv::Mat_<float>& transformationOut) {
	transformationOut = this->meanRelativeTransformation;
}

Camera::Camera() {
	this->chessBoardRows = 7;
	this->chessBoardCols = 4;
	this->numOfImages = 30;
	this->directoryPath = "chessboardcalibration/";
	this->calcAllCameraParameters();
}

Camera::Camera(const Camera& other) {
	this->chessBoardCols = other.chessBoardCols;
	this->chessBoardRows = other.chessBoardRows;
	this->numOfImages = other.numOfImages;
	this->directoryPath = other.directoryPath;
	this->calcAllCameraParameters();
}




//code based on python example by Rie Ruash, Reut Elboim and Yehonatan Leizerson.
void Camera::calcCameraIntrinsicParameters() {
	cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 35, 0.001);//square size of 35 mm 
	std::vector<cv::Point3f> objp = this->dynamicallyCreateObjp();
	std::vector<std::vector<cv::Point3f>> objPoints;//3d points of the object in real space
	std::vector<std::vector<cv::Point2f>> imgPoints;//2d points of the object in the image plane

	cv::Mat gray;
	for (int i = 0; i < this->numOfImages; i++) {//for any image
		std::string filePath = this->directoryPath + "(" + std::to_string(i) + ").jpeg";
		//CONVENTION: calibration images are sorted such that the i'th image is (i).jpeg
		cv::Mat img = cv::imread(filePath);//reading the i'th image
		if (img.empty()) {//if no image
			std::cout << "FUCK" << std::endl;
			continue;
		}
		cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);//gray<--grayscale version of the image
		std::vector<cv::Point2f> corners;//corners
		bool ret = cv::findChessboardCorners(gray, cv::Size(this->chessBoardRows, this->chessBoardCols), corners);//opencv magic to detect the chessboard corners
		if (ret) {//if detected correctly
			objPoints.push_back(objp);//points in real life+=constant
			cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);//refine image points of corners
			imgPoints.push_back(corners);//points in image plane+=corners
		}
	}
	cv::Mat distortionCoeff;//distortion coefficient 
	time_t before = time(NULL);
	calibrateCamera(objPoints, imgPoints, gray.size(), this->cameraMatrix, distortionCoeff, this->rvecs, this->tvecs);//opencv magic to get camera calibration
	time_t after = time(NULL);
	std::cout << "time took for calibration:" << (after - before) << std::endl;
}

//function to create an array of points from the form: (j,i,0) where 0<=i<this->chessBoardCols and 0<=j<this->chessBoardRows
std::vector<cv::Point3f> Camera::dynamicallyCreateObjp() {
	std::vector<cv::Point3f> objp;
	for (int i = 0; i < this->chessBoardCols;i++) {
		for (int j = 0; j < this->chessBoardRows; j++) {
			objp.push_back(cv::Point3f(j+0.0f, i+0.0f, 0.0));
		}
	}
	return objp;
}


//code based of python code from Mark Fonaryov
void Camera::calcGlobalCameraPoseTransformation() {
	std::vector<cv::Mat> globalCameraRotation;//glabal camera rotation
	std::vector<cv::Mat> globalCameraTranslation;//global camera translation
	//calculate global camera pose transformation
	for (int i = 0; i < this->numOfImages; i++) {//init global camera pose parameters
		globalCameraRotation.push_back(cv::Mat::zeros(cv::Size(3, 3), CV_32F));
		globalCameraTranslation.push_back(cv::Mat::zeros(cv::Size(1, 3), CV_32F));
		cv::Mat_<float> mat = cv::Mat::zeros(cv::Size(4, 4), CV_32F);
		mat.at<float>(cv::Point(3, 3)) = 1;
		this->globalCameraTransformation.push_back(mat);//this->globalCameraTransformation[i] will be a 4x4 matrix of 0s and the [3,3] index will be 1
	}
	for (int i = 0; i < this->numOfImages; i++) {
		cv::Rodrigues(this->rvecs[i], globalCameraRotation[i]);//globalCameraRotation[i] is the result of rodrigues rotation formula on this->rvecs[i]
		globalCameraTranslation[i] = this->tvecs[i];//globalCameraTranslation[i] is this->tvecs[i]
		//at the end of next two lines, this->globalCameraTranslation[i] will look like this block matrix:
		//this->globalCameraRotation[i](3x3)    this->globalCameraTranslation[i](1x3)
		//0(3x1)                                1(1x1)
		cv::Rect_<int> rect1(0, 0, 3, 3);
		cv::Rect_<int> rect2(3, 0, 1, 3);
		globalCameraRotation[i].copyTo(this->globalCameraTransformation[i](rect1));
		globalCameraTranslation[i].copyTo(this->globalCameraTransformation[i](rect2));
	}
}


void Camera::calcRelativeCameraPoseTransformation() {
	std::vector<cv::Mat_<float>> relativeCameraRotation;//relative camera rotation
	std::vector<cv::Mat_<float>> relativeCameraTranslation;//relative camera translation
	//calculate relative camera pose transformation
	for (int i = 0; i < this->numOfImages; i++) {//init relative camera pose parameters
		relativeCameraRotation.push_back(cv::Mat::zeros(cv::Size(3, 3), CV_32F));
		relativeCameraTranslation.push_back(cv::Mat::zeros(cv::Size(1, 3), CV_32F));
		cv::Mat_<float> mat = cv::Mat::zeros(cv::Size(4, 4), CV_32F);
		mat.at<float>(cv::Point(3, 3)) = 1;
		this->relativeCameraTransformation.push_back(mat);
	}
	//after the two next rows, this->relativeCameraTransformation[0] will be the identity matrix of 4x4.
	relativeCameraRotation[0] = cv::Mat::eye(cv::Size(3, 3), CV_32F);
	relativeCameraRotation[0].copyTo(this->relativeCameraTransformation[0](cv::Rect_<int>(0, 0, 3, 3)));
	for (int i = 1; i < this->numOfImages; i++) {
		this->relativeCameraTransformation[i] = (this->globalCameraTransformation[i - 1] * (this->globalCameraTransformation[i].inv()));
		//relativeCmeraRotation[i]<-top left (3x3) corner of relativeCameraTransformation[i]
		cv::Rect_<int> rect1(0, 0, 3, 3);
		this->relativeCameraTransformation[i](rect1).copyTo(relativeCameraRotation[i](rect1));
		//relativeCmeraTranslation[i]<-top right (1x3) corner of relativeCameraTransformation[i]
		cv::Rect_<int> rect2(3, 0, 1, 3);
		cv::Rect_<int> rect3(0, 0, 1, 3);
		this->relativeCameraTransformation[i](rect2).copyTo(relativeCameraTranslation[i](rect3));
	}
}
void Camera::calcMeanRelativeCameraPoseTransformation(){
	//calculating the mean of all relative camera transformations.
	this->meanRelativeTransformation = cv::Mat::zeros(cv::Size(4, 4), CV_32F);
	for (int i = 1; i < this->numOfImages; i++) {
		this->meanRelativeTransformation += this->relativeCameraTransformation[i];
	}
	this->meanRelativeTransformation = this->meanRelativeTransformation / this->numOfImages;
}

void Camera::calcAllCameraParameters() {//calculate camera parameters
	this->calcCameraIntrinsicParameters();
	this->calcGlobalCameraPoseTransformation();
	this->calcRelativeCameraPoseTransformation();
	this->calcMeanRelativeCameraPoseTransformation();
}


std::ostream& operator<<(std::ostream& out, const Camera& camera) {
	out << "Camera parameters[" << std::endl;
	out << "Camera Matrix: " << camera.cameraMatrix << "," <<std::endl;
	out << "Rotation Matrix Vector:[" << std::endl;
	for (cv::Mat rot : camera.rvecs) {
		out << rot << ((rot.data==(camera.rvecs[camera.rvecs.size()-1].data))? "],":"," )<< std::endl;
	}
	out << "Translation Matrix Vector[" << std::endl;
	for (cv::Mat trans : camera.tvecs) {
		out << trans << ((trans.data == (camera.tvecs[camera.tvecs.size() - 1].data)) ? "]," : ",") << std::endl;
	}
	out << "Global Transformation Matrix Vector[" << std::endl;
	for (cv::Mat trans : camera.globalCameraTransformation) {
		out << trans << ((trans.data == (camera.globalCameraTransformation[camera.globalCameraTransformation.size() - 1].data)) ? "]," : ",") << std::endl;
	}
	out << "Relative Transformation Matrix Vector[:" << std::endl;
	for (cv::Mat trans : camera.relativeCameraTransformation) {
		out << trans << ((trans.data == (camera.relativeCameraTransformation[camera.relativeCameraTransformation.size() - 1].data)) ? "]," : ",") << std::endl;
	}
	out << "Mean Relative Transformation Matrix: " << camera.meanRelativeTransformation << std::endl;
	return out;
}

