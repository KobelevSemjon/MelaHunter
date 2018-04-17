#ifndef SEGMENT_H
#define SEGMENT_H
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Segment
{
    Point _top_left;
    Mat1b _mask;

public:
    Segment();
    Segment(Point top_left, const Mat1b& mask);
    Segment(Point top_left, Mat1b&& mask);
    Segment(Rect rect, const Mat1b& mask); //!\todo ввести дефолтное значение маски
    Segment(Rect rect, Mat1b&& mask);

    template <typename M>
    M from(const M& image){return image(Rect(_top_left, _mask.size()));}
    const Mat1b& mask() const;
    vector<vector<Point>> contours() const;
    void drawBorder(Mat3b& image, const Vec3b& color = {255,255,255});
};


#endif // SEGMENT_H
