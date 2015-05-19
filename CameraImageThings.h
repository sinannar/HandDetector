//
//  CameraImageThings.h
//  VisionHW3
//
//  Created by Sinan on 14/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#ifndef __VisionHW3__CameraImageThings__
#define __VisionHW3__CameraImageThings__

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Color.h"

using namespace std;
using namespace cv;


class CameraImageThings{
public:
    
    static void myPutText(cv::Mat& img, const char* text, int x=10, int y=30,Scalar color = cv::Scalar::all(255));
    static void myPutNumber(cv::Mat& img, int nmr,int x=10,int y=30);
    static void myPutFPS(cv::Mat& img,double fps,int x,int y);
    
};

#endif /* defined(__VisionHW3__CameraImageThings__) */
