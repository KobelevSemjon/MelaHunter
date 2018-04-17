#include "features.h"

AbstractFeatures::AbstractFeatures(){}
AbstractFeatures::AbstractFeatures(Mat1b &&image, Mat1b &&mask, string prefix):_image(image),_mask(mask),_prefix(prefix)
{
    if (image.size() != mask.size())
        throw std::invalid_argument("Features: image.size() != mask.size()");
}
list<string> AbstractFeatures::header()
{
    list<string> header_list;
    for (auto& p: _dict)
        header_list.push_back(_prefix + p.first);
    return header_list;
}
list<double> AbstractFeatures::features()
{
    list<double> features_list;
    for (auto& p: _dict)
        features_list.push_back(p.second);
    return features_list;
}
void AbstractFeatures::all(){}

AbstractFeatures::~AbstractFeatures(){}

ColorFeatures::ColorFeatures(Mat1b &&image, Mat1b &&mask, string prefix)
    :AbstractFeatures(move(image), move(mask), prefix){}

ColorFeatures &ColorFeatures::minMax()
{
    minMaxLoc(_image, &_dict["MIN"], &_dict["MAX"],0,0,_mask);
    return *this;
}

ColorFeatures &ColorFeatures::meanStdDev()
{
    Vec3d MX,SD;
    cv::meanStdDev(_image, MX, SD,_mask);
    _dict["MX"] = MX[0]; _dict["SD"] =SD[0];
    return *this;
}

void ColorFeatures::all()
{
    minMax().meanStdDev();
}


MorphoFeatures::MorphoFeatures(Mat1b &&image, Mat1b &&mask, string prefix):AbstractFeatures(move(image), move(mask), prefix)
{
    findContours(mask, _contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}

MorphoFeatures &MorphoFeatures::area()
{
    _dict["AREA"] = countNonZero(_mask);
    return *this;
}

MorphoFeatures &MorphoFeatures::perimeter()
{
    double P = 0;
    for (vector<Point> c: _contours)
        P += arcLength(c,true);
    _dict["PERIMETER"] = P;
    return *this;
}

void MorphoFeatures::all(){area().perimeter();}
