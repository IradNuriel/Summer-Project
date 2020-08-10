#include "Matcher.h"
#include "Constants.h"

Matcher::Matcher(){
	this->detector = cv::ORB::create(minHessian);
	this->matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE_HAMMING);
}

Matcher::Matcher(const Matcher & other){
	this->detector = cv::Ptr<cv::ORB>(other.detector);
	this->matcher = cv::Ptr<cv::DescriptorMatcher>(other.matcher);
}

const void Matcher::initImage(Image & img){
	img.init(this->detector);
}

const std::vector<pKeyPoint> Matcher::match2(const cv::Mat& img1, const cv::Mat& img2, bool drawMatch) {
	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	//detect keypoints in input images.
	this->detector->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
	this->detector->detectAndCompute(img2, cv::noArray(), keypoints2, descriptors2);
	//k nearest neighbor matching.
	std::vector<std::vector<cv::DMatch>> knnMatches;
	this->matcher->knnMatch(descriptors1, descriptors2, knnMatches, 2);
	
	//-- Filter matches using the Lowe's ratio test
	std::vector<cv::DMatch> goodMatches;
	for (size_t i = 0; i < knnMatches.size(); i++) {
		if (knnMatches[i][0].distance < Constants::RATIO_THRESH * knnMatches[i][1].distance) {
			goodMatches.push_back(knnMatches[i][0]);
		}
	}
	// discarding

	std::vector<pKeyPoint> feature;
	for (const cv::DMatch& match: goodMatches) {
		feature.emplace_back(keypoints1[match.queryIdx] , keypoints2[match.trainIdx]);
	}
	if (drawMatch) {
		this->draw(img1, img2, feature);
	}
	return feature;
}
const void Matcher::match2(Image& img1, Image& img2, bool drawMatch) {
	//k nearest neighbor matching
	std::vector<std::vector<cv::DMatch>> knnMatches;
	this->matcher->knnMatch(img1.desc, img2.desc, knnMatches, 2 );
	//-- Filter matches using the Lowe's ratio test
	std::vector<cv::DMatch> goodMatches;
	for (size_t i = 0; i < knnMatches.size(); i++) {
		if (knnMatches[i][0].distance < Constants::RATIO_THRESH * knnMatches[i][1].distance) {
			goodMatches.push_back(knnMatches[i][0]);
		}
	}
	// discarding
	std::vector<pKeyPoint> feature;
	std::vector<cv::DMatch> betterMatch;
	std::vector<cv::KeyPoint> nkey1, nkey2;
	for (const cv::DMatch& match : goodMatches) {
		cv::Point2d p1 = img1.key[match.queryIdx].pt, p2 = img2.key[match.trainIdx].pt;
		Line l1 = img1.lb.getLine(img1.pos, { p1.x,p1.y });
		Line l2 = img2.lb.getLine(img2.pos, { p2.x,p2.y });
		Cluster clus;
		clus.add(l1);
		clus.add(l2);
		if (clus.cost() < Constants::GOOD_MATCH_COST) {
			betterMatch.push_back(match);
			nkey1.push_back(img1.key[match.queryIdx]);
			nkey2.push_back(img2.key[match.trainIdx]);
		}
	}
	if (drawMatch) {
		this->draw(img1, img2, goodMatches);
		this->draw(img1, img2, betterMatch);
	}
	/*img1.key = nkey1;
	img2.key = nkey2;*/
}


const void Matcher::draw(const cv::Mat& img1, const cv::Mat& img2, const std::vector<pKeyPoint>& kvec){
	//-- Draw matches
	std::vector<cv::KeyPoint> key1, key2;
	std::vector<cv::DMatch> vmtach;
	for (const pKeyPoint& p : kvec) {
		vmtach.push_back(DMatch(key1.size(), key2.size(),0));
		key1.push_back(p.first);
		key2.push_back(p.second);
	}
	cv::Mat imgMatches;
	drawMatches(img1, key1, img2, key2, vmtach, imgMatches, cv::Scalar::all(-1),
		cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	cv::namedWindow("Good Matches", cv::WINDOW_NORMAL);
	cv::imshow("Good Matches", imgMatches);
	cv::waitKey();
}

const void Matcher::draw(const Image & img1, const Image & img2, const std::vector<cv::DMatch>& match, std::string title){
	cv::Mat imgMatches;
	drawMatches(img1.img, img1.key, img2.img, img2.key, match, imgMatches, cv::Scalar::all(-1),
		cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	cv::namedWindow(title, cv::WINDOW_NORMAL);
	cv::imshow(title, imgMatches);
	cv::waitKey();
}



