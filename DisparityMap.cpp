//
//  DisparityMap.cpp
//  VisionHW3
//
//  Created by Sinan on 14/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//

#include "DisparityMap.h"

DisparityMap::DisparityMap(){
    sbm.state->SADWindowSize = 9;
    sbm.state->numberOfDisparities = 112;
    sbm.state->preFilterSize = 5;
    sbm.state->preFilterCap = 61;
    sbm.state->minDisparity = -39;
    sbm.state->textureThreshold = 507;
    sbm.state->uniquenessRatio = 0;
    sbm.state->speckleWindowSize = 0;
    sbm.state->speckleRange = 8;
    sbm.state->disp12MaxDiff = 1;
    
    sgbm.SADWindowSize = 5;
    sgbm.numberOfDisparities = 192;
    sgbm.preFilterCap = 4;
    sgbm.minDisparity = -64;
    sgbm.uniquenessRatio = 1;
    sgbm.speckleWindowSize = 150;
    sgbm.speckleRange = 2;
    sgbm.disp12MaxDiff = 10;
    sgbm.fullDP = false;
    sgbm.P1 = 600;
    sgbm.P2 = 2400;
}


void DisparityMap::calculateDisparity(){
    Mat g1;
    Mat g2;
    Mat disp8;
    cvtColor(image1, g1, CV_BGR2GRAY);
    cvtColor(image2, g2, CV_BGR2GRAY);
    sbm(g1, g2, disparity);
    normalize(disparity, disp8, 0, 255, CV_MINMAX, CV_8U);
    sgbm(g1, g2, disparity);
    normalize(disparity, disp8, 0, 255, CV_MINMAX, CV_8U);
}