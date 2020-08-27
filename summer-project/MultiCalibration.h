#ifndef _MULTI_CALIBRATION
#define _MULTI_CALIBRATION

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/ccalib/multicalib.hpp"

#include "Camera.h"
#include "Constants.h"
#include "Utilities.h"
#include <vector>
#include <fstream>




std::vector<Camera> multiCalibrate(int nCamera,std::string inPath,std::string outputFileName,int width,int height,int patternWidth,int patternHeight,bool showProcess=false) {
	cv::Mat pattern = cv::imread(Constants::CALIBRATION_DIR+"/randPattern.jpg");
	cv::randpattern::RandomPatternCornerFinder finder(width, height, Constants::MIN_HESSIAN);
	finder.loadPattern(pattern);
	std::vector<cv::Mat_<float>> cameraMatrixs;
	std::vector<std::vector<cv::Mat>> rvecss;//camera rotation
	std::vector<std::vector<cv::Mat>> tvecss;//camera translation
	std::vector<cv::Mat> distortionCoeffs;
	std::vector<cv::Mat_<float>> meanTranss;
	for (int cameraIdx = 0; cameraIdx < nCamera; cameraIdx++) {
		std::vector<cv::Mat> imgVec = getImageVector(Constants::CALIBRATION_DIR, cameraIdx);
		finder.computeObjectImagePoints(imgVec);
		std::vector<cv::Mat> objPoints = finder.getObjectPoints();
		std::vector<cv::Mat> imgPoints = finder.getImagePoints();
		cv::Mat_<float> cameraMatrix;//camera matrix
		std::vector<cv::Mat> rvecs;//camera rotation
		std::vector<cv::Mat> tvecs;//camera translation
		cv::Mat distortionCoeff;
		cv::calibrateCamera(objPoints, imgPoints, imgVec[0].size(), cameraMatrix, distortionCoeff, rvecs, tvecs);//opencv magic to get camera calibration
		cameraMatrixs.push_back(cameraMatrix);
		rvecss.push_back(rvecs);
		tvecss.push_back(tvecs);
		distortionCoeffs.push_back(distortionCoeff);
	}
	std::string inputFileName = inPath + "/imgList.xml";
	generateImgList(inputFileName,nCamera,Constants::CALIBRATION_DIR);
	cv::multicalib::MultiCameraCalibration multiCalib(cv::multicalib::MultiCameraCalibration::PINHOLE, nCamera, inputFileName, patternWidth, patternHeight, showProcess, Constants::MIN_HESSIAN);
	multiCalib.run();

	multiCalib.writeParameters(outputFileName);
	
}

std::vector<Camera> parseParamsFile(std::string inputFileName) {
	cv::FileStorage toParse(inputFileName, cv::FileStorage::READ);
	int nCamera=0;
	toParse["nCamera"] >> nCamera;
	std::vector<Camera> vect;
	for (int i = 0; i < nCamera; i++) {
		cv::Mat_<float> cameraMatrix;//camera matrix
		cv::Mat distortionCoeff;
		cv::Mat pose;
		toParse["camera_matrix_" + std::to_string(i)] >> cameraMatrix;
		toParse["camera_distortion_" + std::to_string(i)] >> distortionCoeff;
		toParse["camera_pose_" + std::to_string(i)] >> pose;
		Camera camera(cameraMatrix, distortionCoeff, pose, i);
		vect.push_back(camera);
	}
}





void generateImgList(std::string inFile,int nCamera,std::string inPath) {
	std::ofstream imgListFile;
	imgListFile.open(inFile);
	imgListFile.flush();
	imgListFile << "images" << "[";
	for (int cameraIdx = 0; cameraIdx < nCamera; cameraIdx++) {
		int suffix;
		for (int frameIdx = 0; Utilities::prefixExist(inPath ,cameraIdx,frameIdx,suffix); frameIdx++) {
			imgListFile << std::string(inPath + std::to_string(cameraIdx) + "_" + std::to_string(frameIdx) + Constants::SUFFICES[suffix]);
		}
	}
	imgListFile << "]";
	imgListFile.close();
}

std::vector<Camera> getCalibration(bool needCalibration, std::string inPath, std::string camData) {
	if ((!fileExists(inPath+camData)) || needCalibration) {
		multiCalibrate(calcNCamera(Constants::CALIBRATION_DIR),Constants::CALIBRATION_DIR,inPath+camData,425,300,28,20);
	}
	std::vector<Camera> cameras = parseParamsFile(inPath + camData);
	return cameras;
}


int calcNCamera(std::string inPath) {
	int nCamera=0;
	int k;
	for (int cameraIdx = 0; Utilities::prefixExist(inPath  ,cameraIdx ,0,k); cameraIdx++) {
		nCamera++;
	}
	return nCamera;
}


std::vector<cv::Mat> getImageVector(std::string inPath,int cameraIdx) {
	//dont know how to work with those kind of files, TODO: ask someone in the lab in thusday
	std::vector<cv::Mat> imgVec;
	int k;
	for (int timeStemp = 0; Utilities::prefixExist(inPath ,cameraIdx, timeStemp,k); timeStemp++) {
		cv::Mat img = Utilities::tryLoad(inPath,cameraIdx,timeStemp);
		if (img.empty()) {
			std::cout << "FUCK" << std::endl;
			continue;
		}
		imgVec.push_back(img);
	}
	return imgVec;

}




// this should be super fast
bool fileExists(std::string fileName) {
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}







#endif // !_MULTI_CALIBRATION
