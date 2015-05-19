//
//  DisparityMap.h
//  VisionHW3
//
//  Created by Sinan on 14/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#ifndef __VisionHW3__DisparityMap__
#define __VisionHW3__DisparityMap__

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
#include <stdio.h>

using namespace cv;
using namespace std;

class DisparityMap{
public:
    DisparityMap();
    
    void SetImages(Mat im1,Mat im2){image1 = im1.clone();image2 = im2.clone();calculateDisparity();}
    Mat getDisparityMap(){return disparity;};
    
private:
    void calculateDisparity();
private:
    StereoSGBM sgbm;
    StereoBM sbm;
    Mat image1;
    Mat image2;
    Mat disparity;
};

#endif /* defined(__VisionHW3__DisparityMap__) */
