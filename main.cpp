//
//  main.cpp
//  VisionHW4
//
//  Created by Sinan on 14/12/14.
//  Copyright (c) 2014 Sinan. All rights reserved.
//


#include <iostream>
#include "StereoCameraCalibration.h"
#include "HandFinder.h"
#include "DisparityMap.h"
#include "FingerTracker.h"
int main(int argc, const char * argv[]) {
    VideoCapture v0(0),v1(1);
    FingerTracker tracker = FingerTracker(v0,v1);
    tracker.simulateMouse();
    v0.release();
    v1.release();
    return 0;
}

