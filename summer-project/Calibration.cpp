#include "Calibration.h"





void Calibration::multiCalibrate(int nCamera, std::string inPath, std::string outputFileName, int width, int height, int patternWidth, int patternHeight, bool showProcess) {
	cv::Mat pattern = cv::imread(Constants::CALIBRATION_DIR + "randPattern.jpg");
	cv::randpattern::RandomPatternCornerFinder finder(patternWidth, patternHeight, Constants::MIN_MATCHES_IN_CALIBRATION_IMAGES);
	finder.loadPattern(pattern);
	for (int cameraIdx = 0; cameraIdx < nCamera; cameraIdx++) {
		std::vector<cv::Mat> imgVec = getImageVector(Constants::CALIBRATION_DIR, cameraIdx);
		finder.computeObjectImagePoints(imgVec);
		std::vector<cv::Mat> objPoints = finder.getObjectPoints();
		std::vector<cv::Mat> imgPoints = finder.getImagePoints();
	}
	std::string inputFileName = inPath + "imgList.xml";
	generateImgList(inputFileName, nCamera, Constants::CALIBRATION_DIR);
	cv::multicalib::MultiCameraCalibration multiCalib(cv::multicalib::MultiCameraCalibration::PINHOLE, nCamera, inputFileName, patternWidth, patternHeight, 1 , showProcess, Constants::MIN_MATCHES_IN_CALIBRATION_IMAGES);
	multiCalib.run();

	multiCalib.writeParameters(outputFileName);

}

std::vector<Camera> Calibration::parseParamsFile(std::string inputFileName) {
	cv::FileStorage toParse(inputFileName, cv::FileStorage::READ);
	int nCamera = 0;
	toParse["nCameras"] >> nCamera;
	std::vector<Camera> vect;
	for (int i = 0; i < nCamera; i++) {
		cv::Mat_<float> cameraMatrix;//camera matrix
		cv::Mat distortionCoeff;
		cv::Mat_<float> pose;
		toParse["camera_matrix_" + std::to_string(i)] >> cameraMatrix;
		toParse["camera_distortion_" + std::to_string(i)] >> distortionCoeff;
		toParse["camera_pose_" + std::to_string(i)] >> pose;
		Camera camera(cameraMatrix, distortionCoeff, pose, i);
		vect.push_back(camera);
	}
	return vect;
}





void Calibration::generateImgList(std::string inFile, int nCamera, std::string inPath) {
	cv::FileStorage imgListFile(inFile, cv::FileStorage::WRITE);
	imgListFile << "images" << "[";
	imgListFile << std::string(Constants::CALIBRATION_DIR + "randPattern.jpg");
	for (int cameraIdx = 0; cameraIdx < nCamera; cameraIdx++) {
		int suffix;
		for (int frameIdx = 0; Utilities::prefixExist(inPath, cameraIdx, frameIdx, suffix); frameIdx++) {
			imgListFile << std::string(inPath + std::to_string(cameraIdx) + "_" + std::to_string(frameIdx) + "." + Constants::SUFFICES[suffix]);
		}
	}
	imgListFile << "]";
	imgListFile.release();
}

std::vector<Camera> Calibration::getCalibration(bool needCalibration, std::string inPath, std::string camData,int& nCamera) {
	if ((!(Utilities::fileExists(inPath + camData))) || needCalibration) {
		multiCalibrate(calcNCamera(Constants::CALIBRATION_DIR), Constants::CALIBRATION_DIR, inPath + camData, 425, 300, 28, 20);
	}
	std::vector<Camera> cameras = parseParamsFile(inPath + camData);
	nCamera = cameras.size();
	return cameras;
}


int Calibration::calcNCamera(std::string inPath) {
	int nCamera = 0;
	int k;
	for (int cameraIdx = 0; Utilities::prefixExist(inPath, cameraIdx, 0, k); cameraIdx++) {
		nCamera++;
	}
	return nCamera;
}


std::vector<cv::Mat> Calibration::getImageVector(std::string inPath, int cameraIdx) {
	//dont know how to work with those kind of files, TODO: ask someone in the lab in thusday
	std::vector<cv::Mat> imgVec;
	int k;
	for (int timeStemp = 0; Utilities::prefixExist(inPath, cameraIdx, timeStemp, k); timeStemp++) {
		cv::Mat img = Utilities::tryLoad(inPath, cameraIdx, timeStemp);
		if (img.empty()) {
			std::cout << "FUCK" << std::endl;
			continue;
		}
		cv::Mat gray;
		cv::cvtColor(img, gray,cv::COLOR_BGR2GRAY);
		imgVec.push_back(gray);
	}
	return imgVec;

}

