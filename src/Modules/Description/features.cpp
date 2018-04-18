#include "features.h"

AbstractFeatures::AbstractFeatures(){}

AbstractFeatures::AbstractFeatures(Mat1b image, Mat1b mask, string prefix):_image(image),_mask(mask),_prefix(prefix)
{
    if (image.size() != mask.size())
        throw std::invalid_argument("Features: image.size() != mask.size()");
}
AbstractFeatures::AbstractFeatures(Mat1b &&image, Mat1b &&mask, string prefix):_image(move(image)),_mask(move(mask)),_prefix(prefix)
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

ColorFeatures::ColorFeatures(Mat1b image, Mat1b mask, string prefix)
    :AbstractFeatures(image, mask, prefix){}


ColorFeatures::ColorFeatures(Mat1b &&image, Mat1b &&mask, string prefix)
    :AbstractFeatures(image, mask, prefix){}

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

ColorFeatures &ColorFeatures::absoluteRadialAsymmetry(string penalty_prefix, function<double (uchar, uchar)> inside_penalty, function<double (uchar, uchar)> outside_penalty)
{
    double ara = 0; const int cols = _image.cols; const int rows = _image.rows;

    int pixels_passed = 0;
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
        {
            Point str_pos = {x,y};
            Point mir_pos = {cols-1-x, rows-1-y};
            uchar x1 = _image(str_pos);
            uchar x2 = _image(mir_pos);
            if ((_mask(str_pos)!=0) && (_mask(mir_pos)!=0))
                ara += inside_penalty(x1,x2);
            if ((_mask(str_pos)!=0) && (_mask(mir_pos)!=0))
                ara += outside_penalty(x1,x2);

            if (pixels_passed < (rows*cols)/2) ++pixels_passed;
            else {x = cols; y = rows;} //break
        }
    _dict[penalty_prefix+"ARA"] = ara;
    return *this;
}

void ColorFeatures::all()
{
    minMax()
   .meanStdDev()
   .absoluteRadialAsymmetry()
   .absoluteRadialAsymmetry("L2_NO_", penalty::L2, penalty::NO)
   .absoluteRadialAsymmetry("L1_", penalty::L1, penalty::L1)
   .absoluteRadialAsymmetry("L2_", penalty::L2, penalty::L2);
}


MorphoFeatures::MorphoFeatures(Mat1b mask, string prefix)
{
    _image = Mat1b(), _mask = mask ,_prefix = prefix;
    findContours(mask, _contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}

MorphoFeatures::MorphoFeatures(Mat1b &&mask, string prefix)
{
    _image = Mat1b(), _mask = mask ,_prefix = prefix;
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





