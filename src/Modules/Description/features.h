#ifndef FEATURES_H
#define FEATURES_H

#include <opencv2/opencv.hpp>
//#include "objectdescription.h"
#include <limits>

using namespace std;
using namespace cv;

class AbstractFeatures
{
protected:
    Mat1b _image = Mat1b();
    Mat1b _mask = Mat1b();
    string _prefix = "";
    map<string,double> _dict;
public:
    AbstractFeatures();
    AbstractFeatures(Mat1b&& image, Mat1b&& mask, string prefix = "");

    list<string> header();
    list<double> features();
    virtual void all();
    virtual ~AbstractFeatures();
};

struct ColorFeatures: public AbstractFeatures
{
    ColorFeatures(Mat1b&& image, Mat1b&& mask, string prefix = "");
    ColorFeatures& minMax();
    ColorFeatures& meanStdDev();
    virtual void all();
};

class MorphoFeatures: public AbstractFeatures
{
    vector<vector<Point>> _contours;
public:
    MorphoFeatures(Mat1b&& image, Mat1b&& mask, string prefix = "");
    MorphoFeatures& area();
    MorphoFeatures& perimeter();
    virtual void all();
};


#endif // FEATURES_H
