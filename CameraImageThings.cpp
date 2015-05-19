//
//  CameraImageThings.cpp
//  VisionHw02
//
//  Created by Sinan on 09/11/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#include "CameraImageThings.h"


void CameraImageThings::myPutText(cv::Mat& img, const char* text, int x, int y) {
    static const int fontFace = cv::FONT_HERSHEY_TRIPLEX;
    static const double fontScale = 0.8;
    static const int thickness = 1;
    cv::Point textOrg(x,y);
    cv::putText(img, text, textOrg, fontFace, fontScale, cv::Scalar::all(255), thickness,14);
}

void CameraImageThings::myPutFPS(cv::Mat& img,double fps,int x,int y){
    char modeString[100];
    sprintf(modeString,"FPS:%.2f",fps);
    CameraImageThings::myPutText(img,modeString,x,y);
}
