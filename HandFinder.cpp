//
//  HandFinder.cpp
//  VisionHW3
//
//  Created by Sinan on 14/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#include "HandFinder.h"


HandFinder::HandFinder(Mat&frame):realFrame(frame),mode(5){
    clearVectors();
    findConvexHull();
}

void HandFinder::findConvexHull(){
    cvtColor(realFrame, frameGray, COLOR_RGB2GRAY);
    threshold(frameGray, frameThres, 0, 255, THRESH_BINARY+THRESH_OTSU);
    convexHullAndConfexityDefects();
}

void HandFinder::clearVectors(){
    allContours.clear();
    hierarchy.clear();
    handContours.clear();
    hullPoint.clear();
    hullInt.clear();
    convdefect.clear();
    fingers.clear();
}

void HandFinder::convexHullAndConfexityDefects(){
    
    
    
    findContours( frameThres.clone(), allContours, hierarchy,
                 CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    
    
    
    findLargestContour();
    if(handContours.size()==0)
        return;
    convexHull( Mat(handContours), hullInt, false);
    convexHull( Mat(handContours), hullPoint, true);
    boundRect = boundingRect(handContours);
    minEnclosingCircle(handContours, center, radius);
    findPalmCenter();
    if (handContours.size() >3 ){
        convexityDefects(handContours, hullInt, convdefect);
    }
    eliminateConvexes();
    eliminateFingers();
    Mat drawing = realFrame.clone();
    
    if(convdefect.size()>6){
        rectangle( drawing, boundRect.tl(), boundRect.br(), yellow, 2, 8, 0 );
        drawFingers(drawing);
        circle(drawing, palmCenter, 20, blue,10);
        circle(drawing, palmCenter, 40, red,10);
        circle(drawing, center, radius, purple, 4);
        drawContours(drawing, allContours, handId, green);
        char modeString[100];
        sprintf(modeString,"Mode:%d",mode);
        CameraImageThings::myPutText(drawing,modeString,30,40);
    }
    else{
        char modeString[100];
        sprintf(modeString,"ERROR at mode:%d",mode);
        CameraImageThings::myPutText(drawing,modeString,30,40);
    }
    
    
    frameConvexHull = drawing;
}



void HandFinder::drawFingers(Mat drawing){
    for(int i=0;i<fingers.size();++i){
        Line finger = fingers[i];
        
        line( drawing, finger.getStart(), finger.getFinish(), white, 3 );
        circle(drawing, finger.getFinish(), 12, green,3);
        char text[10];
        sprintf(text,"%d.finger",i);
        CameraImageThings::myPutText(drawing,text,finger.getFinish().x,finger.getFinish().y-10);
    }
}



void sortForSpecialPurpose(vector<Line>fingers){
    Line *tempFingers = new Line[fingers.size()];
    long fSize = fingers.size();
    for(int i=0;i<fingers.size();++i)
        tempFingers[i] = fingers[i];
    for(int i=0;i<fingers.size();++i)
        for(int j=0;j<fingers.size()-1;++j){
            if(tempFingers[j].getFinish().x < tempFingers[j+1].getFinish().x)
                myswap(tempFingers[j],tempFingers[j+1]);
        }
    fingers.clear();
    for(int i=0;i<fSize;++i)
        fingers.push_back(tempFingers[i]);
    delete[] tempFingers;
}



void HandFinder::eliminateFingers(){
    Line *tempFingers = new Line[fingers.size()];
    for(int i=0;i<fingers.size();++i)
        tempFingers[i] = fingers[i];
    for(int i=0;i<fingers.size();++i)
        for(int j=0;j<fingers.size()-1;++j){
            if(tempFingers[j].getLenght()<tempFingers[j+1].getLenght())
                myswap(tempFingers[j],tempFingers[j+1]);
        }
    fingers.clear();
    for(int i=0;i<mode;++i)
        fingers.push_back(tempFingers[i]);
    
    delete[] tempFingers;
    
    
    sortForSpecialPurpose(fingers);
}

vector<Point2f> HandFinder::getFingerPositions(){
    vector<Point2f> returnValue;
    for(int i=0;i<fingers.size();++i)
        returnValue.push_back(Point2f(fingers[i].getFinish().x,fingers[i].getFinish().y));
    return returnValue;
}


void HandFinder::eliminateConvexes(){
    float tolerance = 0;
    switch(mode){
        case 1:{tolerance =  radius/25;}break;
        case 2:{tolerance =  radius/23;}break;
        case 3:{tolerance =  radius/20;}break;
        case 4:{tolerance =  radius/17;}break;
        case 5:{tolerance =  radius/15;}break;
    }
    vector<Vec4i>::iterator iter=convdefect.begin();
    while(iter != convdefect.end()){
        Vec4i vec = (*iter);
        Point ptStart(handContours[vec[0]]);
        Point ptEnd(handContours[vec[1]]);
        Point ptFar(handContours[vec[2]]);
        
        if(getDistance(ptStart, ptFar)< tolerance
           ||getDistance(ptEnd, ptFar)< tolerance){
            iter = convdefect.erase(iter);
        }
        else{
            fingers.push_back(Line(palmCenter,ptStart));
            iter++;
        }
    }
}



void HandFinder::findPalmCenter(){
    Moments mu = moments(handContours,false);
    palmCenter = Point( mu.m10/mu.m00 , mu.m01/mu.m00 + radius/3 );
}



void HandFinder::findLargestContour(){
    int index = -1;
    double largest_area = -1;
    for(int i = 0; i < allContours.size(); i++) {
        double area = contourArea(allContours[i], false);
        if(area > largest_area){
            largest_area = area;
            index = i;
        }
    }
    handId = index;
    if(allContours.size() == 0)
        return;
    else
        this->handContours = allContours[index];
}
