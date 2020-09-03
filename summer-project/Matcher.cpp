/////////////////////////////////
// file written by Noam Licht, //
// written in 01/08/2020       //
/////////////////////////////////
#include "Matcher.h"


// c'tor
Matcher::Matcher(){
	this->detector = cv::ORB::create(minHessian);
	this->matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE_HAMMING);
}
// copy c'tor
Matcher::Matcher(const Matcher & other){
	this->detector = cv::Ptr<cv::ORB>(other.detector);
	this->matcher = cv::Ptr<cv::DescriptorMatcher>(other.matcher);
}
// init image feature using the object's detector
void Matcher::initImg(Image & img) const{
	img.init(this->detector);
}


std::vector<cv::DMatch> Matcher::match2(Image& img1, Image& img2, bool drawMatch) const {
	// k nearest neighbor matching using opencv
	std::vector<std::vector<cv::DMatch>> knnMatches;
	this->matcher->knnMatch(img1.desc, img2.desc, knnMatches, 2);
	//-- Filter matches using the Lowe's ratio test
	std::vector<cv::DMatch> filteredMatches;
	for (size_t i = 0; i < knnMatches.size(); i++) {
		if (knnMatches[i].size() == 1 || knnMatches[i][0].distance < Constants::RATIO_THRESH * knnMatches[i][1].distance) {
			filteredMatches.push_back(knnMatches[i][0]);
		}
	}
	// 2nd filtering matches which have too large cost (based in geometry knowladge)
	std::vector<pKeyPoint> feature;
	std::vector<cv::DMatch> macthes = filteredMatches; filteredMatches.clear();
	std::vector<cv::KeyPoint> nkey1, nkey2;
	Cluster clus;
	for (const cv::DMatch& match : macthes) {
		// getting the pixel position of the features
		cv::Point2d p1 = img1.key[match.queryIdx].pt, p2 = img2.key[match.trainIdx].pt;
		// building line for each feture using the LineBulider class
		Line l1 = img1.lb.getLine({ p1.x,p1.y });
		Line l2 = img2.lb.getLine({ p2.x,p2.y });
		// adding the line to the cluster
		clus.add(l1);
		clus.add(l2);
		if (clus.cost() < Constants::GOOD_MATCH_COST) { // checking whether the cost of the match is good enough
			filteredMatches.push_back(match);
			nkey1.push_back(img1.key[match.queryIdx]);
			nkey2.push_back(img2.key[match.trainIdx]);
		}
		// removing the line from the cluster
		clus.remove(l1);
		clus.remove(l2);
	}
	if (drawMatch) { // draw a window with the matches (before and after the 2d filtering)
		this->draw(img1, img2, macthes);
		this->draw(img1, img2, filteredMatches);
	}
	return filteredMatches;
}


void Matcher::draw(const Image & img1, const Image & img2, const std::vector<cv::DMatch>& match, std::string title) const{
	//-- Draw matches using opencv interface
	cv::Mat imgMatches;
	drawMatches(img1.img, img1.key, img2.img, img2.key, match, imgMatches, cv::Scalar::all(-1),
		cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	cv::namedWindow(title, cv::WINDOW_NORMAL);
	cv::imshow(title, imgMatches);
	cv::waitKey();
}