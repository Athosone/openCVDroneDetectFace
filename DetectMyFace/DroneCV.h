//
//  DroneCV.h
//  DetectMyFace
//
//  Created by Werck Ayrton on 01/08/2014.
//  Copyright (c) 2014 Athosone. All rights reserved.
//

#ifndef __DetectMyFace__DroneCV__
#define __DetectMyFace__DroneCV__

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cstdio>
#include <opencv/cxcore.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <vector>
#include <fstream>
#include "MyLog.h"

class DroneCV
{
private:
    bool                   _debug = true;
    
    /*CV*/
    IplImage                *_faceCut;
    int                     _minHessian = 150;
    float                   _nndrRatio = 0.7f;
    int                      _imgDim[2];
    const cv::Mat                 _faceToRecognize = cv::imread("/Users/Athos/Documents/Prog/Drone/DetectMyFace/test.png",CV_LOAD_IMAGE_GRAYSCALE);
    std::vector<cv::Mat>    _imagesDb;
    std::vector<int>        _labels;
    
    CvHaarClassifierCascade *_cascade;
    cv::CascadeClassifier   casc;
    cv::Ptr<cv::FaceRecognizer>     _model;

    CvMemStorage            *_storage;
    
    MyLog                  _log;
    
public:
    DroneCV(bool Gdebug = true);
    ~DroneCV();
    
    int                    loadClassesFaces(std::vector<std::string> name, std::string path = "/Users/Athos/Documents/Prog/Drone/DetectMyFace/");
    int                    matchFacesOld();
    void                   analyzeFrame(cv::Mat);
    int                    detectFaces(cv::Mat);
    void                   initFaceRecognizer();
    
    //  void                   logInfo(const std::string&) const;
    //  void                   logError(const std::string&) const;
    //  void                   logDebug(const std::string&) const;
    void                   log(const std:: string&, logType lt = NOTHING);
};

#endif /* defined(__DetectMyFace__DroneCV__) */
