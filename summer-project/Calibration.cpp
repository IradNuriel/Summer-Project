/////////////////////////////////
// file written by Irad Nuriel,//
// written in 27/08/2020       //
/////////////////////////////////
#include "Calibration.h"




/*multiCalibrate(params),
take number of cameras, file to write the cameras info into, physical width and hight of the pattern;
and write the final instrincs and extrincs parameters of each camera to the output file(output file should be an xml file).*/
void Calibration::multiCalibrate(int nCamera, std::string outputFileName, int patternWidth, int patternHeight, bool showProcess) {
	//reading the pattern image.
	cv::Mat pattern = cv::imread(Constants::CALIBRATION_DIR + "randPattern.jpg");

	//initializing finder
	cv::randpattern::RandomPatternCornerFinder finder(patternWidth, patternHeight, Constants::MIN_MATCHES_IN_CALIBRATION_IMAGES);
	finder.loadPattern(pattern);

	//some opencv stuff to make sure the the program will not crash inside multiCalib.run() (it will cause unexpected behavior)
	for (int cameraIdx = 0; cameraIdx < nCamera; cameraIdx++) {
		std::vector<cv::Mat> imgVec = getImageVector(Constants::CALIBRATION_DIR, cameraIdx);
		finder.computeObjectImagePoints(imgVec);
		std::vector<cv::Mat> objPoints = finder.getObjectPoints();
		std::vector<cv::Mat> imgPoints = finder.getImagePoints();
	}

	//generating the image list
	std::string inputFileName = Constants::CALIBRATION_DIR + "imgList.xml";
	generateImgList(inputFileName, nCamera);
	
	//calibrating cameras using opencv multiCalibrate
	cv::multicalib::MultiCameraCalibration multiCalib(cv::multicalib::MultiCameraCalibration::PINHOLE, nCamera, inputFileName, patternWidth, patternHeight, 1 , showProcess, Constants::MIN_MATCHES_IN_CALIBRATION_IMAGES);
	multiCalib.run();

	//writing cameras parameters into a file
	multiCalib.writeParameters(outputFileName);

}


/*parseParamsFile(params),
take a name of xml file which contains all the camera info,
and return a vector af the cameras.*/
std::vector<Camera> Calibration::parseParamsFile(std::string inputFileName) {
	//open file to parsing
	cv::FileStorage toParse(inputFileName, cv::FileStorage::READ);
	
	//number of cameras
	int nCamera = 0;
	//reading number of cameras from the file
	toParse["nCameras"] >> nCamera;

	//generate camera vectors
	std::vector<Camera> vect;

	//for each camera
	for (int i = 0; i < nCamera; i++) {
		//camera parameters:
		//camera matrix
		cv::Mat_<float> cameraMatrix;
		//distortion vector
		cv::Mat distortionCoeff;
		//pose matrix
		cv::Mat_<float> pose;

		//reading camera matrix
		toParse["camera_matrix_" + std::to_string(i)] >> cameraMatrix;
		
		//reding distortion vector
		toParse["camera_distortion_" + std::to_string(i)] >> distortionCoeff;
		
		//reading camera pose matrix(rotation&translation)
		toParse["camera_pose_" + std::to_string(i)] >> pose;

		//initializing i'th camera
		Camera camera(cameraMatrix, distortionCoeff, pose, i);

		//pushing camera to the vector
		vect.push_back(camera);
	}

	//rturning camera vector
	return vect;
}




/*generateImgList(params),
take a name of xml file which will contains list of all images and number of cameras;
and make a xml file named `inFile` that contains the name of all the calibration images.*/
void Calibration::generateImgList(std::string inFile, int nCamera) {
	//open image file to writing
	cv::FileStorage imgListFile(inFile, cv::FileStorage::WRITE);

	//writing "image" tag
	imgListFile << "images" << "[";

	//writing random pattern name
	imgListFile << std::string(Constants::CALIBRATION_DIR + "randPattern.jpg");
	//for each image writing the image file path+name to the file
	for (int cameraIdx = 0; cameraIdx < nCamera; cameraIdx++) {
		int suffix;
		for (int frameIdx = 0; Utilities::prefixExist(Constants::CALIBRATION_DIR, cameraIdx, frameIdx, suffix); frameIdx++) {
			imgListFile << std::string(Constants::CALIBRATION_DIR + std::to_string(cameraIdx) + "_" + std::to_string(frameIdx) + "." + Constants::SUFFICES[suffix]);
		}
	}
	//closing the tag
	imgListFile << "]";

	//releasing the file
	imgListFile.release();
}

/*getCalibration(params), this is the only function that should be called from main.
take a boolean indicating whether or not we need a calibration, path to the final calibration file, its name, and a reference to an int;
returns a vector of camera objects, and the number of cameras returned via the int reference*/
std::vector<Camera> Calibration::getCalibration(bool needCalibration, std::string inPath, std::string camData,int& nCamera) {
	if ((!(Utilities::fileExists(inPath + camData))) || needCalibration) {//if we want calibration to happen of there is no calibration data
		//calibrate cameras
		multiCalibrate(calcNCamera(Constants::CALIBRATION_DIR), inPath + camData, 28, 20, Constants::DEBUG);
	}
	//read cameras calibration info
	std::vector<Camera> cameras = parseParamsFile(inPath + camData);
	
	//getting number of cameras
	nCamera = cameras.size();
	
	//return camera vector.
	return cameras;
}



/*calcNCamera(params),
take a path to the calibration images folder;
and calculate and retrun the number of cameras.*/
int Calibration::calcNCamera(std::string inPath) {
	//number of cameras
	int nCamera = 0;

	//suffix
	int k;

	//for each camera that you can open its 0'th frame (frame 0 in all cameras so we use it as a reference frame) 
	for (int cameraIdx = 0; Utilities::prefixExist(inPath, cameraIdx, 0, k); cameraIdx++) {
		//there is one more cameras
		nCamera++;
	}

	//return number of cameras
	return nCamera;
}

/*getImageVector(params),
take a path to the calibration images folder and camera index;
and return a vector of images that was taken from camera number `camera index`*/
std::vector<cv::Mat> Calibration::getImageVector(std::string inPath, int cameraIdx) {
	//initialize vector of images
	std::vector<cv::Mat> imgVec;
	int k;
	for (int timeStemp = 0; Utilities::prefixExist(inPath, cameraIdx, timeStemp, k); timeStemp++) {//for any frame from this camera
		//load the frame
		cv::Mat img = Utilities::tryLoad(inPath, cameraIdx, timeStemp);
		
		//if couldn't read we fucked something up
		if (img.empty()) {
			std::cout << "FUCK" << std::endl;
			continue;
		}

		//get a grayscale version of the image
		cv::Mat gray;
		cv::cvtColor(img, gray,cv::COLOR_BGR2GRAY);

		//push the grayscale version to the image vector
		imgVec.push_back(gray);
	}

	//return the image vector
	return imgVec;
}

