#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cstdio>
#include <opencv/cxcore.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp> //This is where actual SURF and SIFT algorithm is located
#include <vector>
#include "DroneCV.h"
bool Gdebug = true;


/*CvHaarClassifierCascade *cascade;
CvMemStorage *storage;
int key;
cv::Mat objectMat = cv::imread("/Users/Athos/Documents/Prog/Drone/DetectMyFace/test.png", CV_LOAD_IMAGE_COLOR);
*/

//Pour le drone utiliser le r et comparer avec les dim de limg pour savoir si le visage est plutot en haut adroite en heut a
//gauche en bas en haut etc...

/*int             detectFaces( IplImage *img )
{
    int         i;
    CvRect      *r;
    CvSeq       *faces = cvHaarDetectObjects(img, cascade, storage, 1.1, 3 , 0,  cvSize( 40, 40 ) );

    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ )
    {
        r = ( CvRect* )cvGetSeqElem( faces, i );
        cvRectangle( img, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ), CV_RGB( 255, 0, 0 ), 1, 8, 0 );
        //r not null means face detected
        if (r)
        {
            //saveImage
            std::cout << "Capturing image with r:" << r->height << std::endl;
            faceCut = cvCreateImage(cvSize(r->width, r->height), img->depth, img->nChannels);
            cvSetImageROI(img, *r);
            cvCopyImage(img, faceCut);
            if(!cvSaveImage("/Users/Athos/Documents/Prog/Drone/DetectMyFace/faceCut.png", faceCut))
            {
                fprintf(stderr, "failed to write image file\n");
            }
            return 1;
        }
    }
    cvShowImage( "Window-FT", img );
    return 0;
}


int    compareTwoImages(IplImage *img)
{
    int                  minHessian = 150;
    std::vector<cv::KeyPoint> keypointsO; //keypoints for object
    std::vector<cv::KeyPoint> keypointsS; //keypoints for scene
    //Descriptor matrices
    cv::Mat descriptors_object, descriptors_scene;
    //img
    cv::Mat sceneMat(img);
    //cv::Mat objectMat; places in global as a pig
    //Declering flann based matcher
    cv::FlannBasedMatcher matcher;
    //BFMatcher for SURF algorithm can be either set to NORM_L1 or NORM_L2.
    //But if you are using binary feature extractors like ORB, //instead of NORM_L* you use "hamming"
    //BFMatcher matcher(NORM_L1); an other matcher
    std::vector<std::vector<cv::DMatch>> matches;
    //Will be used for sorting good matches and wrong one
    std::vector<cv::DMatch> good_matches;
    //Detector
    cv::SurfDescriptorExtractor extractor;
    //The more the ratio is close to one the more it is precise vary from 0 to 1
    float nndrRatio = 0.70f;
    
    std::cout << "Initializing compareTwoImages(img) Mat and surf" << std::endl;
    
    //Indicate min hessian, the lower it is the more keypoint it will find and performance will decrease...
    cv::SurfFeatureDetector surf(minHessian);
    //sceneMat is the image taken by camera and object img from databases
    surf.detect(sceneMat,keypointsS);
    surf.detect(objectMat,keypointsO);
    std::cout << "Checking data" << std::endl;
    
    if (!objectMat.data || !sceneMat.data)
        std::cout << "Fail to init data" << std::endl;
    std::cout << "Data correct, now computing" << std::endl;
    
    extractor.compute(sceneMat, keypointsS, descriptors_scene);
    extractor.compute(objectMat, keypointsO, descriptors_object);
    
    std::cout << "Computed now matching" << std::endl;
    //pas trop compris encore
    matcher.knnMatch( descriptors_object, descriptors_scene, matches, 2 ); // find the 2 nearest neighbors
    good_matches.reserve(matches.size());
    std::cout << "Sorting good matches from wrong one" << std::endl;
    for (size_t i = 0; i < matches.size(); ++i)
    {
        if (matches[i].size() < 2)
            continue;
        const cv::DMatch &m1 = matches[i][0];
        const cv::DMatch &m2 = matches[i][1];
        
        if(m1.distance <= nndrRatio * m2.distance)
            good_matches.push_back(m1);
    }
    std::cout << "Number of good matches: " << good_matches.size() << std::endl;
    if (good_matches.size() > 8)
        return (1);
    else
        return (0);
    
}
*/

int main(int ac, char **av)
{
    int key;
    DroneCV     *drone = new DroneCV(Gdebug);
    cv::VideoCapture    cap(CV_CAP_ANY);
    std::vector<std::string> name;
    int i = -1;
    cv::Mat frameMat;
    
    cvNamedWindow( "Window-FT", 1 );
    name.push_back("ayrton");
    name.push_back("marion");
    drone->loadClassesFaces(name);
    drone->initFaceRecognizer();
    while( key != 'q' && i != 1)
    {
        cap >> frameMat;
       cv::Mat gray;
        cv::cvtColor(frameMat, gray, CV_BGR2GRAY);
        
        drone->analyzeFrame(gray);
       // cv::imshow("Window-FT", frameMat);
        key = cvWaitKey( 10 );
    }
    cvDestroyWindow( "Window-FT" );
    
    return (0);
}