#include "ofApp.h"
using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    contourFinderAverage.setMinAreaRadius(50);
    contourFinderAverage.setMaxAreaRadius(2000);
    contourFinderCapture.setMinAreaRadius(50);
    contourFinderCapture.setMaxAreaRadius(2000);
    average.load("images/eye2.jpg");
    capture.load("images/eye.jpg");
    average.resize(250, 150);
    capture.resize(250, 150);
}

//--------------------------------------------------------------
void ofApp::update(){
    contourFinderAverage.setThreshold(10);
    contourFinderCapture.setThreshold(10);
    

    normalize(average);
    normalize(capture);
    average.update();
    capture.update();
    
    convertColor(capture, captureGray, CV_RGB2GRAY);

    convertColor(average, averageGray, CV_RGB2GRAY);
    
    averageGray.update();
    captureGray.update();
    
    normalize(averageGray);
    normalize(captureGray);
    averageGray.update();
    captureGray.update();
    

    GaussianBlur(averageGray, GBA1, 10);
    GaussianBlur(averageGray, GBA2, 5);
    GaussianBlur(captureGray, GBC1, 10);
    GaussianBlur(captureGray, GBC2, 5);
    GBA1.update();
    GBA2.update();
    GBC1.update();
    GBC2.update();
    normalize(GBA1);
    normalize(GBA2);
    normalize(GBC1);
    normalize(GBC2);
    GBA1.update();
    GBA2.update();
    GBC1.update();
    GBC2.update();
    
    cv::Mat GBA1GrayMat = toCv(GBA1);
    cv::Mat GBA2GrayMat = toCv(GBA2);
    cv::Mat GBC1GrayMat = toCv(GBC1);
    cv::Mat GBC2GrayMat = toCv(GBC2);
    

    imitate(differenceAverageImage, GBA1);
    imitate(differenceCaptureImage, GBC1);
    
    differenceAverage = toCv(differenceAverageImage);
    differenceCapture = toCv(differenceCaptureImage);
    cv::absdiff(GBA1GrayMat, GBA2GrayMat, differenceAverage);
    cv::absdiff(GBC1GrayMat, GBC2GrayMat, differenceCapture);

    differenceAverageImage.update();
    differenceCaptureImage.update();
    
    normalize(differenceAverageImage);
    normalize(differenceCaptureImage);
    blur(differenceAverageImage, 5);
    blur(differenceCaptureImage, 5);



    
    differenceAverageImage.update();
    differenceCaptureImage.update();
    cloneAverage.clone(differenceAverageImage);
    cloneCapture.clone(differenceCaptureImage);
    
    //find contours of the images and crop the images to the bounding boxes of the contours
    //contourFinderAverage.setThreshold(mouseX);
    //contourFinderCapture.setThreshold(mouseX);
    contourFinderAverage.findContours(differenceAverageImage);
    contourFinderCapture.findContours(differenceCaptureImage);
    
    differenceAverageImage.crop(contourFinderAverage.getBoundingRect(0).x, contourFinderAverage.getBoundingRect(0).y, contourFinderAverage.getBoundingRect(0).width, contourFinderAverage.getBoundingRect(0).height);
    differenceCaptureImage.crop(contourFinderCapture.getBoundingRect(0).x, contourFinderCapture.getBoundingRect(0).y, contourFinderCapture.getBoundingRect(0).width, contourFinderCapture.getBoundingRect(0).height);
    differenceAverageImage.resize(250, 150);
    differenceCaptureImage.resize(250, 150);

    
//
    
    //compare two images by calculating an euclidian distance among pixels of the two images
    auto& compareAveragePixels =differenceAverageImage.getPixels();
    auto& compareCapturePixels =differenceCaptureImage.getPixels();
    int pixelNumber =0;
    vector<int> squaredDifference;
    squaredDifference.reserve(150 * 250);
    for(int y = 0; y < 150; y++){
        for(int x = 0; x < 250; x++){
            signed char difference = compareAveragePixels[pixelNumber] - compareCapturePixels[pixelNumber];
            squaredDifference.push_back(difference * difference);
            pixelNumber += 1;
        }
    }
    eucldist = 0;
    for(int i = 0; i<squaredDifference.size(); i++){
        eucldist += squaredDifference[i];
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    average.draw(0, 0, 250, 150);
    //drawMat(differenceAverage, 0, 600, 500, 300);
    GBA1.draw(250, 0, 250, 150);
    GBA2.draw(500, 0, 250, 150);
    cloneAverage.draw(750, 0, 250, 150);
    differenceAverageImage.draw(1000, 0, 250, 150);
    
    capture.draw(0, 150, 250, 150);
    //drawMat(differenceCapture, 500, 600, 500, 300);
    GBC1.draw(250, 150, 250, 150);
    GBC2.draw(500, 150, 250, 150);
    cloneCapture.draw(750, 150, 250, 150);
    differenceCaptureImage.draw(1000, 150, 250, 150);
    
    ofDrawBitmapString("Difference: " + ofToString(eucldist), 10, 280);
    //ofDrawBitmapString("Threshold: " + ofToString(mouseX), 10, 250);
    
    ofPushMatrix();
    ofTranslate(750, 0);
    contourFinderAverage.draw();
    ofPopMatrix();

    
    ofPushMatrix();
    ofTranslate(750, 150);
    contourFinderCapture.draw();
    ofPopMatrix();
    

    
}

