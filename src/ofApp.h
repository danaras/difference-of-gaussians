#pragma once

#include "ofMain.h"
#include "ofxCv.h"


class ofApp : public ofBaseApp{

	public:
    
        ofImage average;
        ofImage capture;
        ofImage grayCapture, edgeCapture, sobelCapture, blurCapture;
        ofImage grayAverage, edgeAverage, sobelAverage, blurAverage;
        ofImage GBA1, GBA2, GBC1, GBC2, DoGA, DoGC;
        ofImage captureGray, averageGray;
        ofPixels DoGAPixels, DoGCPixels;
        ofImage differenceAverageImage, differenceCaptureImage;
        cv::Mat differenceAverage;
        cv::Mat differenceCapture;
        cv::Mat totalDifference;
        int eucldist;
        ofImage cloneAverage;
        ofImage cloneCapture;
    
        ofxCv::ContourFinder contourFinderAverage, contourFinderCapture;

    
		void setup();
		void update();
		void draw();
  };
