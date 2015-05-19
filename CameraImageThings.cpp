//
//  CameraImageThings.cpp
//  VisionHW3
//
//  Created by Sinan on 14/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#include "CameraImageThings.h"
#include <cstdio>
#include <cstring>
using namespace std;

void CameraImageThings::myPutText(cv::Mat& img, const char* text, int x, int y,Scalar color) {
    static const int fontFace = cv::FONT_HERSHEY_TRIPLEX;
    static const double fontScale = 0.6;
    static const int thickness = 1;
    cv::Point textOrg(x,y);
    cv::putText(img, text, textOrg, fontFace, fontScale, color, thickness,14);
}

void CameraImageThings::myPutNumber(cv::Mat& img, int nmr,int x,int y){
    char modeString[100];
    sprintf(modeString,"%d",nmr);
    CameraImageThings::myPutText(img,modeString,x,y);
}




void CameraImageThings::myPutFPS(cv::Mat& img,double fps,int x,int y){
    char modeString[100];
    sprintf(modeString,"FPS:%.2f",fps);
    CameraImageThings::myPutText(img,modeString,x,y);
}
