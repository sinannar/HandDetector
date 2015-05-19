//
//  main.cpp
//  VisionHW3
//
//  Created by Sinan on 14/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#include <iostream>
#include "StereoCameraCalibration.h"
#include "HandFinder.h"
#include "DisparityMap.h"

int main(int argc, const char * argv[]) {
    
    VideoCapture v0(0),v1(1);
    StereoCameraCalibration system(v0,v1);
    system.defineWindows();
    system.calibrateCameras();
    system.releaseWindows();
    
    HandFinder finder0,finder1;
    
    
    int newMode=1;
    int mode = 5;
    time_t startTime,endTime;
    
    Mat image0;
    Mat image1;
    namedWindow("camera0");
    namedWindow("camera1");

    time(&startTime);
    int counter=0;
    bool drawEpipoles = false;
    bool drawDistance = false;
    bool drawDistance2 = false;
    
    do{
        if(!(v0.read(image0)))
            break;
        if(!(v1.read(image1)))
            break;
    
        
        finder0.setFrame(image0);
        finder1.setFrame(image1);
        vector<Point2f> fingerOf0 = finder0.getFingerPositions();
        vector<Point2f> fingerOf1 = finder1.getFingerPositions();
        vector<Point3f> result;
        if(finder0.isFoundHand() && finder1.isFoundHand())
            result =  system.triangulatePoints2(fingerOf0, fingerOf1);
        Mat frame0 = finder0.getFrameConvexHull();
        Mat frame1 = finder1.getFrameConvexHull();
        if(finder0.isFoundHand() && finder1.isFoundHand()){
            if(drawEpipoles){
                frame0 = system.drawEpipolarLines(frame0, fingerOf0, 0);
                frame1 = system.drawEpipolarLines(frame1, fingerOf1, 1);
            }
            if(drawDistance){
                system.drawDistancesOnImages(frame0, fingerOf0, result);
                system.drawDistancesOnImages(frame1, fingerOf1, result);
            }
            if(drawDistance2){
                system.drawDistancesOnImages2(frame0, fingerOf0, result);
                system.drawDistancesOnImages2(frame1, fingerOf1, result);
            }
        }
        
        time(&endTime);
        ++counter;
        double sec=difftime(endTime,startTime);
        double fps=counter/sec;
        CameraImageThings::myPutFPS(frame0, fps, 500, 40);
        CameraImageThings::myPutFPS(frame1, fps, 500, 40);
        
        CameraImageThings::myPutText(frame0, drawDistance?"D:IMAGE":"",30,70,red);
        CameraImageThings::myPutText(frame1, drawDistance?"D:IMAGE":"",30,70,red);
        CameraImageThings::myPutText(frame0, drawEpipoles?"E:IMAGE":"",30,100,red);
        CameraImageThings::myPutText(frame1, drawEpipoles?"E:IMAGE":"",30,100,red);
        CameraImageThings::myPutText(frame0, drawDistance2?"D:CONSOLE":"",30,130,red);
        CameraImageThings::myPutText(frame1, drawDistance2?"D:CONSOLE":"",30,130,red);
        
        imshow("camera0", frame0);
        imshow("camera1", frame1);
        //imshow("disparity", disparityMap);
        
        newMode = waitKey(1);
        if(newMode != -1){
            mode = newMode-48;
            int mode0 = finder0.setMode(mode);
            int mode1 = finder1.setMode(mode);
            if(mode0 == 1 && mode1 == 1)
                cerr<<"mode set edildi:"<<mode<<endl;
            else
                cerr<<" mode set edilemedi:"<<newMode<<endl;
            
            if(newMode == 101){
                drawEpipoles = !drawEpipoles;
            }
            if(newMode == 100){
                drawDistance = !drawDistance;
            }
            if(newMode == 102){
                drawDistance2 = !drawDistance2;
            }
            
        }
        
        
    }while( newMode!=27/*ESC*/ &&
            newMode!=48/*ZERO*/ &&
            newMode!=113/*Q*/ &&
            newMode!=96/*~*/ &&
            newMode!=9/*TAB*/ &&
            newMode!=63236/*F1*/ &&
            newMode!=63237/*F2*/);
    
    v0.release();
    v1.release();
    destroyWindow("camera0");
    destroyWindow("camera1");
    
    return 0;
}
