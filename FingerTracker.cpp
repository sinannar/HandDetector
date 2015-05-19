//
//  FingerTracker.cpp
//  VisionHw4
//
//  Created by Sinan on 11/01/15.
//  Copyright (c) 2015 Sinan. All rights reserved.
//

#include "FingerTracker.h"
#include "Constratins.h" 

FingerTracker::FingerTracker(VideoCapture v0,VideoCapture v1):calibrationSystem(v0,v1){
    this->v0 = v0;
    this->v1 = v1;
    calibrationSystem = StereoCameraCalibration(v0,v1);
    calibrationSystem.defineWindows();
    calibrationSystem.calibrateCameras();
    calibrationSystem.releaseWindows();
}

void FingerTracker::simulateMouse(){

    int newMode=1;
    int mode = 1;
    time_t startTime,endTime;
    

    
    Mat image0;
    Mat image1;
    Mat cursor = Mat::zeros(1000, 1000, CV_32F);
    int cursorX = 300;
    int cursorY = 300;
    int cursorR = 10;
    namedWindow("camera0");
    namedWindow("camera1");
    namedWindow("cursor");
    time(&startTime);
    int counter=0;
    bool drawEpipoles = false;
    bool drawDistance = false;
    bool drawDistance2 = false;
    
    handFinder0.setMode(1);
    handFinder1.setMode(1);
    
    do{
        cursor = Mat::zeros(600, 600, CV_32F);
        if(!(v0.read(image0)))
            break;
        if(!(v1.read(image1)))
            break;
        
        
        handFinder0.setFrame(image0);
        handFinder1.setFrame(image1);
        vector<Point2f> fingerOf0 = handFinder0.getFingerPositions();
        vector<Point2f> fingerOf1 = handFinder1.getFingerPositions();
        

        
        circle(cursor, Point(cursorX,cursorY), cursorR, Scalar(125,125,125),10);
        
        if(track0.size()>FRAME_LIMIT)
            track0.clear();//track0.erase(track0.begin());
        if(track1.size()>FRAME_LIMIT)
            track1.clear();//track1.erase(track1.begin());

        
        
        
        
        vector<Point3f> result;
        if(handFinder0.isFoundHand() && handFinder1.isFoundHand())
            result =  calibrationSystem.triangulatePoints2(fingerOf0, fingerOf1);
       
        Mat frame0 = handFinder0.getFrameConvexHull();
        Mat frame1 = handFinder1.getFrameConvexHull();
        if(handFinder0.isFoundHand() && handFinder1.isFoundHand()){
            if(drawEpipoles){
                frame0 = calibrationSystem.drawEpipolarLines(frame0, fingerOf0, 0);
                frame1 = calibrationSystem.drawEpipolarLines(frame1, fingerOf1, 1);
            }
            if(drawDistance){
                calibrationSystem.drawDistancesOnImages(frame0, fingerOf0, result);
                calibrationSystem.drawDistancesOnImages(frame1, fingerOf1, result);
            }
            if(drawDistance2){
                calibrationSystem.drawDistancesOnImages2(frame0, fingerOf0, result);
                calibrationSystem.drawDistancesOnImages2(frame1, fingerOf1, result);
            }
            
            track0.push_back(fingerOf0);
            track1.push_back(fingerOf1);
            
            if(track0.size()>FRAME_LIMIT){
                if(track0[0][0].x > track0[FRAME_LIMIT-1][0].x && (track0[0][0].x - track0[FRAME_LIMIT-1][0].x)>PIXEL_LIMIT){
                    cout<<"----RIGHT"<<cursorX<<endl;
                    if(cursorX>0)
                        cursorX -= CURSER_DIFF;
                }
                if(track0[0][0].y > track0[FRAME_LIMIT-1][0].y && (track0[0][0].y - track0[FRAME_LIMIT-1][0].y)>PIXEL_LIMIT){
                    cout<<"----UP"<<cursorY<<endl;
                    if(cursorY>0)
                        cursorY -=CURSER_DIFF;
                }
                if(track0[0][0].x < track0[FRAME_LIMIT-1][0].x && (track0[FRAME_LIMIT-1][0].x - track0[0][0].x)>PIXEL_LIMIT){
                    cout<<"----LEFT"<<cursorX<<endl;
                    if(cursorX<600)
                        cursorX += CURSER_DIFF;
                }
                if(track0[0][0].y < track0[FRAME_LIMIT-1][0].y && (track0[FRAME_LIMIT-1][0].y - track0[0][0].y)>PIXEL_LIMIT){
                    cout<<"----DOWN"<<cursorY<<endl;
                    if(cursorY<600)
                        cursorY +=CURSER_DIFF;
                }
            }
            else
                cout<<"---NO MOVEMENT"<<endl;
            
            
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
        
        imshow("cursor", cursor);
        imshow("camera0", frame0);
        imshow("camera1", frame1);
        
        newMode = waitKey(10);
        if(newMode != -1){
            mode = newMode-48;
            int mode0 = handFinder0.setMode(mode);
            int mode1 = handFinder1.setMode(mode);
            if(mode0 == 1 && mode1 == 1){
                cerr<<"mode set edildi:"<<mode<<endl;
                cout<<"1-SIZE"<<track0.size()<<"-"<<track0[0].size()<<endl;
                track0.clear();
                track1.clear();
                cout<<"2-SIZE"<<track0.size()<<"-"<<track0[0].size()<<endl<<endl;
            }
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

    destroyWindow("camera0");
    destroyWindow("camera1");
    destroyWindow("cursor");

    
    
}


