//
//  HandFinder.h
//  VisionHW3
//
//  Created by Sinan on 14/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#ifndef __VisionHW3__HandFinder__
#define __VisionHW3__HandFinder__


#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "CameraImageThings.h"
#include "Color.h"
using namespace std;
using namespace cv;



//unnamed namespace
namespace{
    double getDistance(Point start,Point end){
        return sqrt((start.x-end.x)*(start.x-end.x) + (start.y-end.y)*(start.y-end.y));
    }
    
    template<class T>
    void myswap(T& a,T& b){
        T temp = a;
        a = b;
        b = temp;
    }
}

//Line class decleration and implementation
class Line{
private:
    Point start;
    Point finish;
public:
    Line(){}
    Line(Point s,Point f):start(s),finish(f){}
    void setStart(Point s){start = s;}
    Point getStart(){return start;}
    void setFinish(Point f){finish = f;}
    Point getFinish(){return finish;}
    double getLenght(){return getDistance(start,finish);}
};

//HandFinder class decleration
class HandFinder{
public:
    //Construtcots
    HandFinder():mode(5){}
    HandFinder(Mat&frame);
    
public:
    //public method to access
    void setFrame(Mat&frame){realFrame = frame;clearVectors();findConvexHull();}
    Mat getFrameConvexHull(){return frameConvexHull;}
    Mat getFrameGray(){return frameGray;}
    Mat getFrameThres(){return frameThres;}
    vector<Point2f> getFingerPositions();
    bool isFoundHand(){return convdefect.size()>6?true:false;}
private:
    //private method for calculations
    void findConvexHull();
    void clearVectors();
    void convexHullAndConfexityDefects();
    void drawFingers(Mat drawing);
    void eliminateFingers();
    void drawThings(Mat drawing, vector<Vec4i> convDefect, vector<Point> contours,Point2f center);
    void eliminateConvexes();
    void findPalmCenter();
    void findLargestContour();
    
public:
    //program mode
    int getMode(){return mode;}
    int setMode(int _mode){
        if(_mode>=1 && _mode<=5){
            mode = _mode;
            return 1;
        }
        else
            return 0;
    }
    
    //class variables
private:
    int mode;
    int handId;
    
    Mat realFrame;
    Mat frameGray;
    Mat frameThres;
    Mat frameConvexHull;
    
    vector<vector<Point>>allContours;
    vector<Vec4i> hierarchy;
    
    vector<Point>handContours;
    
    vector<Point> hullPoint;
    vector<int> hullInt;
    vector<Vec4i>convdefect;
    
    vector<Line> fingers;
    
    Rect boundRect;
    Point2f center;
    float radius;
    Point palmCenter;
    
};

#endif /* defined(__VisionHW3__HandFinder__) */
