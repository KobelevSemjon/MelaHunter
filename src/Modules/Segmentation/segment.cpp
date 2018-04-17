#include "segment.h"

Segment::Segment(){}
Segment::Segment(Point top_left, const Mat1b &mask):_top_left(top_left),_mask(mask){}
Segment::Segment(Point top_left, Mat1b &&mask):_top_left(top_left){swap(_mask, mask);}
Segment::Segment(Rect rect, const Mat1b &mask):_top_left(rect.tl()),_mask(mask){}
Segment::Segment(Rect rect, Mat1b &&mask):_top_left(rect.tl()){swap(_mask, mask);}

const Mat1b &Segment::mask() const {return _mask;}

vector<vector<Point>> Segment::contours() const
{
    vector<vector<Point>> contours;
    cv::findContours(_mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    return contours;
}

void Segment::drawBorder(Mat3b &image, const Vec3b &color)
{
    for (const vector<Point>& contour: contours())
        for (const Point& p: contour)
            image(p+_top_left) = color;
}
