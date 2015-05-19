//
//  CameraImageThings.h
//  VisionHw02
//
//  Created by Sinan on 09/11/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#ifndef __VisionHw02__CameraImageThings__
#define __VisionHw02__CameraImageThings__

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


class CameraImageThings{
public:
    
    static void myPutText(cv::Mat& img, const char* text, int x=10, int y=30);
    static void myPutFPS(cv::Mat& img,double fps,int x,int y);

};

#endif /* defined(__VisionHw02__CameraImageThings__) */
