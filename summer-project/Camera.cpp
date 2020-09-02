/////////////////////////////////
// file written by Irad Nuriel,//
// written in 27/08/2020       //
/////////////////////////////////
#include "Camera.h"



//default constructor
Camera::Camera(int num) {
	if (num = -1) {
		this->cameraNum = -1;
	}
	else {
		this->cameraNum = num;
	}
}

//copy constructor
Camera::Camera(const Camera& other) {
	this->cameraMatrix = other.cameraMatrix;
	this->cameraNum = other.cameraNum;
	this->distortionCoeff = other.distortionCoeff;
	this->meanRelativeTransformation = other.meanRelativeTransformation;
}


//real constructor
Camera::Camera(cv::Mat_<float> cameraMatrix, cv::Mat diffCoeff, cv::Mat_<float> transformation, int cameraNum) {
	this->cameraMatrix = cameraMatrix;
	this->distortionCoeff = diffCoeff;
	this->meanRelativeTransformation = transformation;
	this->cameraNum = cameraNum;
}


//operator <<, only for debuging
std::ostream& operator<<(std::ostream& out, const Camera& camera) {
	out << "Camera parameters[" << std::endl;
	out << "Camera Matrix: " << std::endl << cv::format(camera.cameraMatrix,cv::Formatter::FMT_NUMPY) << "," <<std::endl << std::endl;
	out << "Inverse Camera Matrix" << std::endl << cv::format(camera.cameraMatrix.inv(), cv::Formatter::FMT_NUMPY) << "," << std::endl << std::endl;
	out << "distortion vector: " << std::endl << cv::format(camera.distortionCoeff, cv::Formatter::FMT_NUMPY) << "," << std::endl << std::endl;
	out << "Mean Transformation Matrix: " << std::endl << cv::format(camera.meanRelativeTransformation,cv::Formatter::FMT_NUMPY) << "]" << std::endl;
	return out;
}

/*getCalibrationParameters(params),
  input: references to a cv::Mat_<double>, and two std::vector<cv::Mat> references;
  the return value will be in the parameters you've pass in*/
void Camera::getCalibrationParameters(cv::Mat_<float>& cameraMatrixOut, cv::Mat_<float>& cameraMatrixInverseOut, cv::Mat& distortionVec) const {
	cameraMatrixOut = this->cameraMatrix;
	cameraMatrixInverseOut = this->cameraMatrix.inv();
	distortionVec = this->distortionCoeff;
}

/*getCameraExtrincParam,
  input: std::vector<cv::Mat_<float>> reference;
  the return value will be in the parameters you've pass in*/
void Camera::getCameraExtrinsicParam(cv::Mat_<float>& transformationOut) const {
	transformationOut = this->meanRelativeTransformation;
}



//no_camera(), a singelton! represent a no camera, so we will be able to support old LineBuilder
Camera& Camera::no_camera() {
	static Camera noCamera = Camera(-1);//camera number of no camera is -1; it's a singelton
	return noCamera;
}


//operator != , compare cameras based on if the camera numbers of both are different
bool Camera::operator!=(const Camera & other) const {
	return this->cameraNum != other.cameraNum;
}


