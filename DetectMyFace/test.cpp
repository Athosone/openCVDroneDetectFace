/*// Example showing how to read and write images
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>

int main(int argc, char** argv)
{
    IplImage * pInpImg = 0;
    
    // Load an image from file - change this based on your image name
    pInpImg = cvLoadImage("/Users/Athos/Documents/Prog/Drone/DetectMyFace/test.jpg", CV_LOAD_IMAGE_UNCHANGED);
    if(!pInpImg)
    {
        fprintf(stderr, "failed to load input image\n");
        return -1;
    }
    
    // Write the image to a file with a different name,
    // using a different image format -- .png instead of .jpg
    if( !cvSaveImage("/Users/Athos/Documents/Prog/Drone/DetectMyFace/my_image_copy.png", pInpImg) )
    {
        fprintf(stderr, "failed to write image file\n");
    }
    
    // Remember to free image memory after using it!
    cvReleaseImage(&pInpImg);
    
    return 0;
}*/
/*

cv::SurfFeatureDetector detector( this->_minHessian );
cv::Mat faceFound(this->_faceCut);
std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

detector.detect( this->_faceToRecognize, keypoints_1 );
detector.detect(faceFound , keypoints_2 );

//-- Step 2: Calculate descriptors (feature vectors)
cv::SurfDescriptorExtractor extractor;

cv::Mat descriptors_1, descriptors_2;

extractor.compute( this->_faceToRecognize, keypoints_1, descriptors_1 );
extractor.compute( this->_faceCut, keypoints_2, descriptors_2 );

//-- Step 3: Matching descriptor vectors using FLANN matcher
cv::FlannBasedMatcher matcher;
std::vector< cv::DMatch > matches;
matcher.match( descriptors_1, descriptors_2, matches );

double max_dist = 0; double min_dist = 100;

//-- Quick calculation of max and min distances between keypoints
for( int i = 0; i < descriptors_1.rows; i++ )
{ double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
            }

printf("-- Max dist : %f \n", max_dist );
printf("-- Min dist : %f \n", min_dist );

//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
//-- PS.- radiusMatch can also be used here.
std::vector< cv::DMatch > good_matches;

for( int i = 0; i < descriptors_1.rows; i++ )
{
    if( matches[i].distance < 2*min_dist )
    {
        good_matches.push_back( matches[i]);
    }
}

 */

//DETECT FACES OLD
/* int         i;
 CvRect      *r;
 CvSeq       *faces = cvHaarDetectObjects(img, this->_cascade, this->_storage, 1.1, 3 , 0,  cvSize( 40, 40 ) );
 int         res = 0;
 
 for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ )
 {
 r = ( CvRect* )cvGetSeqElem( faces, i );
 cvRectangle( img, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ), CV_RGB( 255, 0, 0 ), 1, 8, 0 );
 //r not null means face detected
 if (r)
 {
 //saveImage
 //ALWAYS SET ROI BEFORE PROCESSING else dimmension wont fit
 cvSetImageROI(img, *r);
 this->_faceCut = cvCreateImage(cvSize(r->width, r->height), img->depth, img->nChannels);
 cvCopy(img, this->_faceCut);
 res = 1;
 cvResetImageROI(img);
 }
 }
 if (this->_faceCut)
 cvShowImage( "Window-FT", this->_faceCut );*/


//MAIN OLD
/*CvCapture *capture;
IplImage *frame;

capture = cvCreateCameraCapture(CV_CAP_ANY);
cvNamedWindow( "FaceToRecognized", 2 );
cvNamedWindow( "FaceCut", 3 );
 
 while ()
 //frame = cvQueryFrame( capture );

 
 cvReleaseCapture( &capture );
 // cvReleaseHaarClassifierCascade( &cascade );
 // cvReleaseMemStorage( &storage );
 
 */

