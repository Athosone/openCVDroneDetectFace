//
//  DroneCV.cpp
//  DetectMyFace
//
//  Created by Werck Ayrton on 01/08/2014.
//  Copyright (c) 2014 Athosone. All rights reserved.
//

#include "DroneCV.h"
#include "CONSTANT.h"
#include "loadData.h"
#include <dirent.h>
#include <fcntl.h>
#include <string.h>

DroneCV::DroneCV(bool Gdebug)
{
    this->_cascade = ( CvHaarClassifierCascade* )cvLoad("/Users/Athos/Documents/Prog/Drone/DetectMyFace/DetectMyFace/Base.lproj/haarcascade_frontalface_alt.xml", 0, 0, 0 );
    this->casc.load("/Users/Athos/Documents/Prog/Drone/DetectMyFace/DetectMyFace/Base.lproj/haarcascade_frontalface_alt.xml");
    this->_storage = cvCreateMemStorage( 0 );
    this->_debug = Gdebug;
    
}

void        DroneCV::initFaceRecognizer()
{
    try
    {
        read_csv(fnn_csv, this->_imagesDb, this->_labels);
        this->_imgDim[0] = this->_imagesDb[0].rows;
        this->_imgDim[1] = this->_imagesDb[0].cols;
    } catch (cv::Exception err)
    {
        this->log("Error with files" + err.msg);
    }
    
    /*(a rajouter avec enum pour les differents type de reco)*/
    this->_model = createFisherFaceRecognizer();
    this->_model->train(this->_imagesDb, this->_labels);
    this->log("Model trained");
}

void rotateMat(cv::Mat& src, double angle, cv::Mat& dst)
{
    int len = std::max(src.cols, src.rows);
    cv::Point2f pt(len/2., len/2.);
    cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);
    
    cv::warpAffine(src, dst, r, cv::Size(len, len));
}

DroneCV::~DroneCV()
{
    cvReleaseHaarClassifierCascade( &this->_cascade );
    cvReleaseMemStorage( &this->_storage );
}

int        DroneCV::loadClassesFaces(std::vector<std::string> name, std::string path)
{
    int seq = 0;
    ofstream   file(fnn_csv.c_str(), ios::out | ios::trunc);
    cv::Mat faceCut;
    struct dirent *ent;
    DIR           *dir;
    
    for (int i = 0; i < name.size(); i++)
    {
        std::string pathTo = path + name[i] + "/";
        
        dir = opendir(pathTo.c_str());
        seq = 0;
        if (dir == NULL)
        {
            this->log("Could not open dir: " + pathTo);
        }
        else
        {
            //TODO checker le dépôt d'arrivée afin de ne pas refaire tout le temps la même étape si il a deja des images on stop et pass au prochain nom
            while ((ent = readdir(dir)) != NULL)
            {
                this->log("Processing " + pathTo + ent->d_name);
                if (ent->d_name[0] == '.' || strstr(ent->d_name, "_format")!= NULL)
                    continue;
                faceCut = cv::imread(pathTo + ent->d_name, CV_LOAD_IMAGE_GRAYSCALE);
                cv::equalizeHist(faceCut, faceCut);
                std::vector<cv::Rect_<int>> faces;
                if (name[i] == "ayrton")
                    rotateMat(faceCut, 90, faceCut);
                else
                    rotateMat(faceCut, -90, faceCut);
                cv::imshow("Window FT", faceCut);
                if (!faceCut.empty())
                {
                    this->casc.detectMultiScale(faceCut, faces, 1.1, 3, 0, cv::Size(30, 30));
                    
                    for (int j = 0; j < faces.size(); j++)
                    {
                        Rect face_rect_i = faces[i];
                        //Resize
                        cv::Mat face = faceCut(face_rect_i);
                        cv::imshow("Window FT", face);
                        
                        cv::Mat face_resized;
                        cv::resize(face, face_resized, cv::Size(100, 100), 1.0, 1.0, cv::INTER_CUBIC);
                        //a encapsuler
                        std::string final = pathTo + name[i] + "_format" + "/" + name[i] + "_" + std::to_string(seq) + ".jpg";
                        this->log("Writing image to " + final);
                        cv::imwrite(final, face_resized);
                        seq++;
                        file << final + ";" + std::to_string(i) << std::endl;
                        break;
                    }
                    faceCut.release();
                }
                else
                    this->log("Could not load face");
            }
            closedir(dir);
        }
    }
    return 0;
}

int        DroneCV::matchFacesOld()
{
    std::vector<cv::KeyPoint> keypointsO;
    std::vector<cv::KeyPoint> keypointsS;
    cv::Mat descriptors_object, descriptors_scene;
    cv::Mat foundFacesColor(this->_faceCut);
    cv::Mat foundFaces;
    cv::cvtColor(foundFacesColor, foundFaces, CV_BGR2GRAY);
    cv::FlannBasedMatcher matcher;
    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> good_matches;
    cv::SurfDescriptorExtractor extractor;
    cv::SurfFeatureDetector surf(this->_minHessian);
    
    surf.detect(foundFaces,keypointsS);
    surf.detect(this->_faceToRecognize,keypointsO);
    if (!this->_faceToRecognize.data || !foundFaces.data)
    {
        this->log("Fail to init data in DronceCV::matchFaces");
        return (0);
    }
    
    extractor.compute(foundFaces, keypointsS, descriptors_scene);
    extractor.compute(this->_faceToRecognize, keypointsO, descriptors_object);
    
    if(descriptors_object.type()!=CV_32F || descriptors_scene.type()!=CV_32F)
    {
        this->log("TYPE OBJECT " + std::to_string(descriptors_object.type()));
        this->log("TYPE SCENE " + std::to_string(descriptors_scene.type()));
        return (0);
    }
    //Bot image must be in grayscale
    try {
        matcher.knnMatch( descriptors_object, descriptors_scene, matches, 2); // find the 2 nearest neighbors
        
    } catch (cv::Exception e) {
        this->log(e.err);
    }
    cv::imshow("FaceToRecognized", this->_faceToRecognize);
    cv::imshow("FaceCut", foundFaces);
    good_matches.reserve(matches.size());
    for (size_t i = 0; i < matches.size(); ++i)
    {
        if (matches[i].size() < 2)
            continue;
        const cv::DMatch &m1 = matches[i][0];
        const cv::DMatch &m2 = matches[i][1];
        
        if(m1.distance <= this->_nndrRatio * m2.distance)
            good_matches.push_back(m1);
    }
    this->log("Number of good matches" + std::to_string(good_matches.size()));
    foundFaces.release();
    if (good_matches.size() > 8)
        return (1);
    else
        return (0);
}

int         DroneCV::detectFaces(cv::Mat gray)
{
    std::vector<cv::Rect_<int>> faces;
    //Biggest impact on perf
    this->casc.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(50, 50));
    
    for (int i = 0; i < faces.size(); i++)
    {
        Rect face_rect_i = faces[i];
        //Resize
        cv::Mat face = gray(face_rect_i);
        cv::Mat face_resized;
        
        cv::resize(face, face_resized, cv::Size(this->_imgDim[0], this->_imgDim[1]), 1.0, 1.0, cv::INTER_CUBIC);
        int prediction = this->_model->predict(face_resized);
        std::cout << std::to_string(prediction) << std::endl;
        rectangle(gray, face_rect_i, CV_RGB(0, 255,0), 1);
        // Create the text we will annotate the box with:
        string box_text = format("Prediction = %d", prediction);
        // Calculate the position for annotated text (make sure we don't
        // put illegal values in there):
        int pos_x = std::max(face_rect_i.tl().x - 10, 0);
        int pos_y = std::max(face_rect_i.tl().y - 10, 0);
        // And now put it into the image:
        putText(gray, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
        imshow("Window-FT", gray);
    }
    
    return (1);
}

void        DroneCV::analyzeFrame(cv::Mat img)//gray one
{
    this->detectFaces(img);
}

void                   DroneCV::log(const std:: string &str, logType lt)
{
    //Maybe use pointer to member function, at least for training :p
    if (_debug)
    {
        if (lt == logType::INFO)
            this->_log.setStream(new std::ofstream("INFO.log", std::ios_base::ate|std::ios_base::app), true);
        else if (lt == logType::ERROR)
            this->_log.setStream(new std::ofstream("ERROR.log", std::ios_base::ate|std::ios_base::app), true);
        else if (lt == logType::DEBUGMODE)
            this->_log.setStream(new std::ofstream("DEBUG.log", std::ios_base::ate|std::ios_base::app), true);
        else
            this->_log.setStream(&std::cout, false);
        this->_log.log(str, lt);//);
        
    }
}

