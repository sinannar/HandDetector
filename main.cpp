//
//  main.cpp
//  VisionHw02
//
//  Created by Sinan on 26/10/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#include "HandFinder.h"
#include "CameraImageThings.h"

#define CAMERA_INPUT
#define VIDEO_INPUT

int main(int argc, const char * argv[]) {
    
    //VideoCapture stream("/Users/apple/Desktop/CV/hand.m4v");
    VideoCapture stream(1);
    
    int x = stream.get(CV_CAP_PROP_FRAME_WIDTH);
    int y = stream.get(CV_CAP_PROP_FRAME_HEIGHT);

    HandFinder finder;
    Mat frame;
    Mat frameGray;
    Mat frameThres;
    Mat frameConvexHull;
    
    namedWindow("originalFrame",CV_WINDOW_AUTOSIZE);
    namedWindow("gray form",CV_WINDOW_AUTOSIZE);
    namedWindow("threshold",CV_WINDOW_AUTOSIZE);
    namedWindow("convex hull",CV_WINDOW_AUTOSIZE);
    moveWindow("originalFrame",0,0);
    moveWindow("gray form",x,0);
    moveWindow("threshold",0,y);
    moveWindow("convex hull",x,y);

    
    time_t startTime,endTime;
    int mode = 5;
    int newMode=1;
    time(&startTime);
    int counter=0;
    
    do{
        //read from stream
        if(!(stream.read(frame)))
            break;
        
        //set frame to calculation
        finder.setFrame(frame);
        
        //get processed frames
        frameGray = finder.getFrameGray();
        frameThres = finder.getFrameThres();
        frameConvexHull = finder.getFrameConvexHull();
        
        //error check
        if(frameConvexHull.rows == 0 || frameConvexHull.cols==0)
            continue;
        
        //fps calculations
        time(&endTime);
        ++counter;
        double sec=difftime(endTime,startTime);
        double fps=counter/sec;
        CameraImageThings::myPutFPS(frameConvexHull, fps, 500, 40);

        //inage shows
        imshow("originalFrame",frame);
        imshow("gray form",frameGray);
        imshow("threshold",frameThres);
        imshow("convex hull",frameConvexHull);
        
        //user input for exit or mode selection
        newMode = waitKey(1);
        if(newMode != -1){
            mode = newMode-48;
            int modeR = finder.setMode(mode);
            if(modeR == 1)
                cerr<<"mode set edildi:"<<mode<<endl;
            else
                cerr<<"mode set edilemedi:"<<newMode<<endl;
        }
        
    }while( newMode!=27/*ESC*/ &&
            newMode!=48/*ZERO*/ &&
            newMode!=96/*~*/ &&
            newMode!=9/*TAB*/ &&
            newMode!=63236/*F1*/ &&
            newMode!=63237/*F2*/);
    
    stream.release();
    return 0;
}