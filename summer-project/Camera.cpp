#include "Camera.h"




void Camera::getCalibrationParameters(cv::Mat_<float>& cameraMatrixOut, cv::Mat_<float>& cameraMatrixInverseOut, cv::Mat& distortionVec) const {
	cameraMatrixOut = this->cameraMatrix;
	cameraMatrixInverseOut = this->cameraMatrix.inv();
	distortionVec = this->distortionCoeff;
}


void Camera::getCameraExtrinsicParam(cv::Mat_<float>& transformationOut) const {
	transformationOut = this->meanRelativeTransformation;
}


Camera& Camera::no_camera() {
	static Camera noCamera = Camera(-1);
	return noCamera;
}

Camera::Camera(int num) {
	if (num = -1) {
		this->cameraNum = -1;
	}
	else {
		this->cameraNum = num;
	}
}

Camera::Camera(const Camera& other) {
	this->cameraMatrix = other.cameraMatrix;
	this->cameraNum = other.cameraNum;
	this->distortionCoeff = other.distortionCoeff;
	this->meanRelativeTransformation = other.meanRelativeTransformation;
}

Camera::Camera(cv::Mat_<float> cameraMatrix, cv::Mat diffCoeff, cv::Mat_<float> transformation, int cameraNum) {
	this->cameraMatrix = cameraMatrix;
	this->distortionCoeff = diffCoeff;
	this->meanRelativeTransformation = transformation;
	this->cameraNum = cameraNum;
}

std::ostream& operator<<(std::ostream& out, const Camera& camera) {
	out << "Camera parameters[" << std::endl;
	out << "Camera Matrix: " << std::endl << cv::format(camera.cameraMatrix,cv::Formatter::FMT_NUMPY) << "," <<std::endl << std::endl;
	out << "Inverse Camera Matrix" << std::endl << cv::format(camera.cameraMatrix.inv(), cv::Formatter::FMT_NUMPY) << "," << std::endl << std::endl;
	out << "distortion vector: " << std::endl << cv::format(camera.distortionCoeff, cv::Formatter::FMT_NUMPY) << "," << std::endl << std::endl;
	out << "Mean Transformation Matrix: " << std::endl << cv::format(camera.meanRelativeTransformation,cv::Formatter::FMT_NUMPY) << "]" << std::endl;
	return out;
}
