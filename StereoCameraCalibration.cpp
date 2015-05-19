//
//  StereoCameraCalibration.cpp
//  StereoCalibrationClass
//
//  Created by Sinan on 13/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#include "StereoCameraCalibration.h"
#include "CameraImageThings.h"

StereoCameraCalibration::StereoCameraCalibration(VideoCapture c1,VideoCapture c2){
    this->camera1 = c1;
    this->camera2 = c2;
    needToDestroyCaptures = false;
    boardSize = Size(9,6);
}

StereoCameraCalibration::StereoCameraCalibration(int c1,int c2){
    this->camera1 = VideoCapture(c1);
    this->camera2= VideoCapture(c2);
    needToDestroyCaptures = true;
    boardSize = Size(9,6);
}

void StereoCameraCalibration::defineWindows(string c1,string c2){
    this->windowsC1 = c1;
    this->windowsC2 = c2;
    namedWindow(this->windowsC1);
    namedWindow(this->windowsC2);
}

void StereoCameraCalibration::releaseWindows(){
    destroyWindow(this->windowsC1);
    destroyWindow(this->windowsC2);
    if(needToDestroyCaptures){
        this->camera1.release();
        this->camera2.release();
    }
}

StereoCameraCalibration::~StereoCameraCalibration(){
    if(needToDestroyCaptures){
        this->camera1.release();
        this->camera2.release();
    }
}

void StereoCameraCalibration::readImage(){
    camera1.read(image1);
    camera2.read(image2);
}

void StereoCameraCalibration::showImages(){
    imshow(windowsC1, image1);
    imshow(windowsC2, image2);
}
void StereoCameraCalibration::show_Images(){
    imshow(windowsC1, _image1);
    imshow(windowsC2, _image2);
}

void StereoCameraCalibration::testEpipolars(){
    vector<Point2f> points = vector<Point2f>();
    int r = image1.rows/4;
    int c = image1.cols/4;
    for(int i=1;i<=3;++i)
        for(int j=1;j<=3;++j)
            points.push_back(Point2f(r*i,c*j));
    namedWindow("w1");
    namedWindow("w2");
    do{
        readImage();
        Mat imageToShow = drawEpipolarLines(points);
        for(int i=0;i<points.size();++i){
            Point2f p = points[i];
            circle(image1, p, 3, green);
        }
        imshow("w1", image1);
        imshow("w2", imageToShow);
        

    }while(cvWaitKey(30)!=27);
    destroyWindow("w1");
    destroyWindow("w2");
}


void StereoCameraCalibration::drawDistancesOnImages(Mat&image,vector<Point2f>points,vector<Point3f>distances){
    for(int i=0;i<points.size()-1;++i){
        Point2f mid((points[i].x+points[i+1].x)/2,(points[i].y+points[i+1].y)/2);
        double distance = sqrt( ((distances[i].x-distances[i+1].x) * (distances[i].x-distances[i+1].x))+
                               ((distances[i].y-distances[i+1].y) * (distances[i].y-distances[i+1].y))+
                               ((distances[i].z-distances[i+1].z) * (distances[i].z-distances[i+1].z)) );
        
        if(!std::isinf(distance)){
            line(image,points[i],points[i+1],red);
        }
        
    }
}

void StereoCameraCalibration::drawDistancesOnImages2(Mat&image,vector<Point2f>points,vector<Point3f>distances){
    for(int i=0;i<points.size()-1;++i){
        Point2f mid((points[i].x+points[i+1].x)/2,(points[i].y+points[i+1].y)/2);
        double distance = sqrt( ((distances[i].x-distances[i+1].x) * (distances[i].x-distances[i+1].x))+
                               ((distances[i].y-distances[i+1].y) * (distances[i].y-distances[i+1].y))+
                               ((distances[i].z-distances[i+1].z) * (distances[i].z-distances[i+1].z)) );
        
        if(!std::isinf(distance)){
            cout<<i<<"-"<<i+1<<"distance:"<<distance<<endl;
        }
        
    }
}


vector<Point3f> StereoCameraCalibration::triangulatePoints2(vector<Point2f>pointsImage1,vector<Point2f>pointsImage2){
    vector<Point3f> retVal;
    Mat realWorldCoordinates;
    cv::triangulatePoints(projectionMatrix[0], projectionMatrix[1], pointsImage1, pointsImage2, realWorldCoordinates);
    cout<<realWorldCoordinates.rows<<"-"<<realWorldCoordinates.cols<<endl;
    for(int i=0;i<realWorldCoordinates.cols;++i){
        retVal.push_back(Point3f(
                                 realWorldCoordinates.at<double>(i,0)/realWorldCoordinates.at<double>(i,3),
                                 realWorldCoordinates.at<double>(i,1)/realWorldCoordinates.at<double>(i,3),
                                 realWorldCoordinates.at<double>(i,2)/realWorldCoordinates.at<double>(i,3)));
    }
    return retVal;
}



void StereoCameraCalibration::findChessBoardOnce(int index){
    bool found = false;
    do{
        cvWaitKey(100)==27?(found = true):(found = false);
        readImage();
        found = findChessboardCornersAndDraw(image1,image2,boardSize);
        if(found)
            saveImages(image1, image2, index);
        
        CameraImageThings::myPutNumber(image1, index+1);
        CameraImageThings::myPutNumber(image2, index+1);
        show_Images();
    }while(!found);
}

void StereoCameraCalibration::saveImages(Mat image1,Mat image2,int pairIndex){
    cameraImagePoints[0].push_back(corners1);
    cameraImagePoints[1].push_back(corners2);
    cvtColor(image1, image1, COLOR_BGR2GRAY);
    cvtColor(image2, image2, COLOR_BGR2GRAY);
    std::ostringstream image1String, image2String;
    image1String<<"image1"<<pairIndex<<".jpg";
    image2String<<"image2"<<pairIndex<<".jpg";
    imwrite(image1String.str().c_str(), image1);
    imwrite(image2String.str().c_str(), image2);
}


bool StereoCameraCalibration::findChessboardCornersAndDraw(Mat img1, Mat img2, Size boardSize){
    _image1 = img1;
    _image2 = img2;
    bool found1 = false, found2 = false;
    found1 = findChessboardCorners(img1, boardSize, corners1, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
    found2 = findChessboardCorners(img2, boardSize, corners2, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
    drawChessboardCorners(_image1, boardSize, corners1, found1);
    drawChessboardCorners(_image2, boardSize, corners2, found2);
    if(found1 && found2)
        return true;
    else
        return false;
}

void StereoCameraCalibration::calibrateCameras(){
    for(int i=0;i<14;++i)
    {
        findChessBoardOnce(i);
        cvWaitKey();
        //cout<<i<<endl;
    }
    objectPoints.resize(numberOfStereoPairs);
    for (int i=0; i<numberOfStereoPairs; i++) {
        for (int j=0; j<boardSize.height; j++) {
            for (int k=0; k<boardSize.width; k++) {
                objectPoints[i].push_back(Point3f(float(j),float(k),0.0));
            }
        }
    }
    cameraMatrix[0] = Mat::eye(3, 3, CV_64F);
    cameraMatrix[1] = Mat::eye(3, 3, CV_64F);
    
    rms = stereoCalibrate(objectPoints, cameraImagePoints[0], cameraImagePoints[1],
                          cameraMatrix[0], distCoeffs[0],
                          cameraMatrix[1], distCoeffs[1],
                          image1.size(), RotationM, TranslationM, EssentialM, FundementalM,
                          TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5),
                          CALIB_FIX_ASPECT_RATIO +
                          CALIB_ZERO_TANGENT_DIST +
                          CALIB_SAME_FOCAL_LENGTH +
                          CALIB_RATIONAL_MODEL +
                          CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5 );
    
    cout<<"RMS Error: "<<endl<<rms<<endl<<endl;;
    cout<<"Camera Matrix 0"<<endl<<cameraMatrix[0]<<endl<<endl<<endl;
    cout<<"Camera Matrix 1"<<endl<<cameraMatrix[1]<<endl<<endl<<endl;
    cout<<"Dist Coef 0"<<endl<<distCoeffs[0]<<endl<<endl<<endl;
    cout<<"Dist Coef 1"<<endl<<distCoeffs[1]<<endl<<endl<<endl;
    cout<<"Essential Matrix"<<endl<<EssentialM<<endl<<endl<<endl;
    cout<<"Translation Matrix"<<endl<<TranslationM<<endl<<endl<<endl;
    cout<<"Rotation Matrix"<<endl<<RotationM<<endl<<endl<<endl;
    cout<<"Fundamental Matrix"<<endl<<FundementalM<<endl<<endl<<endl;
    calculateProjectionMatrixes();
    cout<<endl<<endl<<"calculateProjectionMatrixes"<<endl;
    cout<<"Projection Matrix 0"<<endl<<projectionMatrix[0]<<endl<<endl<<endl;
    cout<<"Projection Matrix 1"<<endl<<projectionMatrix[1]<<endl<<endl<<endl;

    
}

void StereoCameraCalibration::calculateProjectionMatrixes(){
//    Mat rt = Mat::zeros(3, 4, cameraMatrix[0].type());
//    for(int i=0;i<3;++i){
//        for(int j=0;j<3;++j){
//            rt.at<double>(i,j) = RotationM.at<double>(i,j);
//        }
//        rt.at<double>(i,3) = TranslationM.at<double>(i,0);
//    }
//    
//    this->projectionMatrix[0] =  cameraMatrix[0] * rt;
//    this->projectionMatrix[1] =  cameraMatrix[1] * rt;
    Rect validPixROI1, validPixROI2;
    Mat R1,R2,P1,P2,Q;
    stereoRectify(cameraMatrix[0].clone(), distCoeffs[0].clone(),
                  cameraMatrix[1].clone(), distCoeffs[1].clone(),
                  image1.size(), RotationM.clone(), TranslationM.clone(),
                  R1,R2,P1,P2,Q,CALIB_ZERO_DISPARITY, 1, image1.size(), &validPixROI1, &validPixROI1);
    projectionMatrix[0] = P1.clone();
    projectionMatrix[1] = P2.clone();
}

vector<Vec3f> StereoCameraCalibration::getEpipolarLines(vector<Point2f> points){
    vector<Vec3f> lines = vector<Vec3f>(points.size());
    if(points.size()>0){
        computeCorrespondEpilines(points, 1, FundementalM, lines);
    }
    return lines;
}

vector<Vec3f> StereoCameraCalibration::getEpipolarLines(vector<Point2f> points,int number){
    vector<Vec3f> lines = vector<Vec3f>(points.size());
    if(points.size()>0){
        computeCorrespondEpilines(points, number+1, FundementalM, lines);
    }
    return lines;
}



Mat StereoCameraCalibration::drawEpipolarLines(vector<Point2f> points){
    vector<Vec3f> lines = getEpipolarLines(points);
    Mat retVal = image2.clone();
    
    for (vector<cv::Vec3f>::const_iterator it = lines.begin();
         it != lines.end(); ++it) {
        
        // draw the epipolar line between first and last column
        cv::line(retVal, cv::Point(0, -(*it)[2] / (*it)[1]),
                 cv::Point(retVal.cols, -((*it)[2] + (*it)[0] * retVal.cols) / (*it)[1]),
                 green);
    }
    
    return retVal;
}

Mat StereoCameraCalibration::drawEpipolarLines(Mat m,vector<Point2f> points,int number){
    vector<Vec3f> lines = getEpipolarLines(points,number);
    Mat retVal = m.clone();
    
    for (vector<cv::Vec3f>::const_iterator it = lines.begin();
         it != lines.end(); ++it) {
        
        // draw the epipolar line between first and last column
        cv::line(retVal, cv::Point(0, -(*it)[2] / (*it)[1]),
                 cv::Point(retVal.cols, -((*it)[2] + (*it)[0] * retVal.cols) / (*it)[1]),
                 green);
    }
    
    return retVal;
}


