#include "LineBuilder.h"
#include "Line.h"

// Camera c'tor
LineBuilder::LineBuilder(Camera camera) {
	this->camera = camera;
	this->setPos();
}

// Parameter c'tor
LineBuilder::LineBuilder(std::vector<double> res,std::vector<double> a) {
	resolution.resize(2);
	std::copy(std::begin(res), std::end(res), std::begin(resolution));
	// degrees -> radians
	this->angle.resize(2);
	this->angle[0] = a[0] * Constants::DEGREE_TO_RADIANS_RATIO;
	this->angle[1] = a[1] * Constants::DEGREE_TO_RADIANS_RATIO;
}

// ID Constructor
LineBuilder::LineBuilder(int id) {
	this->camera = cams[id];
	this->setPos();
	this->cameraID = id;

}

void LineBuilder::setPos(const cv::Vec3f& pos) {
	this->pos = pos;
}

void LineBuilder::setPos(){
	cv::Mat_<float> cameraTransformation;
	(this->camera).getCameraExtrinsicParam(cameraTransformation);
	//cv::Rect_<int> rect2(3, 0, 1, 3);
	//cv::Mat_<float> posMat = cv::Mat::zeros(cv::Size(1, 3), CV_32F);
	//cameraTransformation(rect2).copyTo(posMat);
	this->pos = cv::Vec3f(cameraTransformation.at<float>(0, 3), cameraTransformation.at<float>(1, 3), cameraTransformation.at<float>(2, 3));
	//if (Constants::DEBUG) std::cout << this->cameraID<< std::endl << std::endl;
}

cv::Vec3d LineBuilder::getPos() const {
	return this->pos;
}


void LineBuilder::setAngle(const std::vector<double>& a) {
	this->angle = a;
}


void LineBuilder::setRes(const std::vector<double>& res) {
	this->resolution = res;
}



const LineBuilder& LineBuilder::operator=(const LineBuilder& other) {
	this->camera = other.camera;
	this->angle = other.angle;
	this->resolution = other.resolution;
	this->setPos(other.pos);
	return *this;
}


Line LineBuilder::getLine(std::vector<double> pixel) const {
	if ((this->camera) != (Camera::no_camera())) {
		cv::Mat_<float> invCameraMatrix;
		cv::Mat_<float> cameraMatrix;
		cv::Mat_<float> cameraTransformation;
		cv::Mat distortion;
		(this->camera).getCalibrationParameters(cameraMatrix, invCameraMatrix, distortion);
		(this->camera).getCameraExtrinsicParam(cameraTransformation);
		cv::Mat_<float> lineMat = invCameraMatrix * (cv::Mat_<float>(cv::Vec3f(pixel[0], pixel[1], 1)));
		cv::Vec3f line(lineMat.at<float>(0, 0), lineMat.at<float>(1, 0), lineMat.at<float>(2, 0));
		line = line / (lineMat.dot(lineMat));
		cv::Mat_<float> rot = cv::Mat::eye(cv::Size(3, 3), CV_32F);
		cv::Rect_<int> rect1(0, 0, 3, 3), rect2(3, 0, 1, 3);
		cameraTransformation(rect1).copyTo(rot);
		cv::Mat_<float> mult = rot * cv::Mat_<float>(line);
		cv::Vec3f dir = cv::Vec3f(mult.at<float>(0, 0), mult.at<float>(1, 0), mult.at<float>(2, 0));
		//if (Constants::DEBUG) std::cout<<"cameraID"<<this->cameraID << std::endl << std::endl;
		return Line(this->pos, dir);
	}else{
		
		// casting int array into double array (to be used later)
		std::vector<double> pixeld = std::vector<double>(2);
		pixeld.resize(pixel.size());
		std::copy(std::begin(pixel), std::end(pixel), std::begin(pixeld));

		//pixeld = pixeld - (resolution / 2.0); //normalizing pixel pos relative to center
		pixeld[0] = pixeld[0] - (this->resolution[0] / 2.0);
		pixeld[1] = pixeld[1] - (this->resolution[1] / 2.0);
		pixeld[1] = -pixeld[1]; // coordinates (>x, ^y, .z)

		std::vector<double> temp(2);
		temp.resize(pixel.size());

		temp[0]= (pixeld[0] * tan(this->angle[0] / 2.0)) / (this->resolution[0] / 2.0); // calculating x length relative to z (=1)
		temp[1] = (pixeld[1] * tan(this->angle[1] / 2.0)) / (this->resolution[1] / 2.0); // calculating y length relative to z (=1)

		Vec3d v = { temp[0], temp[1], 1 }; //declaring the 3d line vector based on the calculation above

		return Line(this->pos, v);
		
	}
}

std::vector<Camera> LineBuilder::cams;