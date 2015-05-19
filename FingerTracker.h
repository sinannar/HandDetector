//
//  FingerTracker.h
//  VisionHw4
//
//  Created by Sinan on 11/01/15.
//  Copyright (c) 2015 Sinan. All rights reserved.
//

#ifndef __VisionHw4__FingerTracker__
#define __VisionHw4__FingerTracker__

#include <stdio.h>
#include "StereoCameraCalibration.h"
#include "HandFinder.h"

class FingerTracker{
public:
    FingerTracker(VideoCapture v0,VideoCapture v1);
    void simulateMouse();
private:
    VideoCapture v0;
    VideoCapture v1;
    StereoCameraCalibration calibrationSystem;
    HandFinder handFinder0,handFinder1;
    vector<vector<Point2f>> track0;
    vector<vector<Point2f>> track1;
};

#endif /* defined(__VisionHw4__FingerTracker__) */
