#ifndef FEATURES_H
#define FEATURES_H

#include <opencv2/opencv.hpp>
//#include "objectdescription.h"
#include <functional>

using namespace std;
using namespace cv;

//!\todo header -> static constexpr возвращающий все названия признаков.
class AbstractFeatures
{
protected:
    Mat1b _image = Mat1b();
    Mat1b _mask = Mat1b();
    string _prefix = "";
    map<string,double> _dict;
public:
    AbstractFeatures();
    AbstractFeatures(Mat1b image, Mat1b mask, string prefix = "");
    AbstractFeatures(Mat1b&& image, Mat1b&& mask, string prefix = "");

    list<string> header();
    list<double> features();
    virtual void all();
    virtual ~AbstractFeatures();
};

namespace penalty {
static const function<double (uchar, uchar)> NO = [](uchar, uchar){return 0.;};
static const function<double (uchar, uchar)> L1 = [](uchar x1, uchar x2){return double((x1<x2)?(x2-x1):(x1-x2));};
static const function<double (uchar, uchar)> L2 = [](uchar x1, uchar x2){return pow(L1(x1,x2),2);};
}

struct ColorFeatures: public AbstractFeatures
{
    ColorFeatures(Mat1b image, Mat1b mask, string prefix = "");
    ColorFeatures(Mat1b&& image, Mat1b&& mask, string prefix = "");
    ColorFeatures& minMax();
    ColorFeatures& meanStdDev();
    ColorFeatures& absoluteRadialAsymmetry(string penalty_prefix = "L1_NO_", function<double(uchar,uchar)> inside_penalty = penalty::L1, function<double(uchar,uchar)> outside_penalty = penalty::NO);

    virtual void all();
};

class MorphoFeatures: public AbstractFeatures
{
    vector<vector<Point>> _contours;
public:
    MorphoFeatures(Mat1b mask, string prefix = "");
    MorphoFeatures(Mat1b&& mask, string prefix = "");
    MorphoFeatures& area();
    MorphoFeatures& perimeter();
    virtual void all();
};


#endif // FEATURES_H
