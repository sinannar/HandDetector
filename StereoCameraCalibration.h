//
//  StereoCameraCalibration.h
//  StereoCalibrationClass
//
//  Created by Sinan on 13/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#ifndef __StereoCalibrationClass__StereoCameraCalibration__
#define __StereoCalibrationClass__StereoCameraCalibration__

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "Color.h"

#define timeGap 3000000000U

using namespace cv;
using namespace std;

class StereoCameraCalibration{

public:
    StereoCameraCalibration(VideoCapture c1,VideoCapture c2);
    StereoCameraCalibration(int c1,int c2);
    
    void defineWindows(string c1="leftImage",string c2="rightImage");
    void releaseWindows();
    void readImage();
    void showImages();
    void show_Images();
    
    void calibrateCameras();
    
    void testEpipolars();
    
    Mat drawEpipolarLines(vector<Point2f> points);
    Mat drawEpipolarLines(Mat m,vector<Point2f> points,int number);
    
    ~StereoCameraCalibration();
private:
    bool findChessboardCornersAndDraw(Mat inputLeft, Mat inputRight, Size boardSize);
    void saveImages(Mat image1,Mat image2,int pairIndex);
    void findChessBoardOnce(int index);
    vector<Vec3f> getEpipolarLines(vector<Point2f> points);
    vector<Vec3f> getEpipolarLines(vector<Point2f> points,int number);
    void calculateProjectionMatrixes();
    
public:
    vector<Point3f> triangulatePoints2(vector<Point2f>pointsImage1,vector<Point2f>pointsImage2);
    void drawDistancesOnImages(Mat&image,vector<Point2f>points,vector<Point3f>distances);
    void drawDistancesOnImages2(Mat&image,vector<Point2f>points,vector<Point3f>distances);
    
private:
    bool needToDestroyCaptures;
    VideoCapture camera1;
    VideoCapture camera2;
    
    string windowsC1;
    string windowsC2;
    
    Mat image1;
    Mat image2;
    Mat copyImage1;
    Mat copyImage2;
    Mat _image1;
    Mat _image2;
    
    Size boardSize;
    
    vector<Point2f> corners1;
    vector<Point2f> corners2;
    vector<vector<Point2f> > cameraImagePoints[2];
    vector<vector<Point3f> > objectPoints;
    
    Mat cameraMatrix[2], distCoeffs[2];
    Mat RotationM, TranslationM, EssentialM, FundementalM;
    
    Mat projectionMatrix[2];
    
    int numberOfStereoPairs = 14;
    int stereoPairIndex = 0;
    int cornerImageIndex=0;
    int goIn = 1;
    double rms;
};


#endif /* defined(__StereoCalibrationClass__StereoCameraCalibration__) */
