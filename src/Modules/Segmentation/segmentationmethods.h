#ifndef SEGMENTATIONMETHODS_H
#define SEGMENTATIONMETHODS_H

#include <createsegmentationmap.h>

SegmentationMap otsuSegmentation(Mat1b& image)
{
    SegmentationMap sm;
    threshold(image, image, 0 ,255, THRESH_BINARY_INV | THRESH_OTSU );
    sm = createSegmentationMap(image);
    return sm;
}
//!\todo Исправить квадратную окрестность на круг
//SegmentationMap watershedSegmentation(Mat1b& image, int radius = 1)
//{
//    SegmentationMap sm;
//    if (radius<1) throw std::invalid_argument("watershedSegmentation: radius must be >0");

//    Mat1i markers(image);
//    markers.forEach([&](uchar& pix, const int* pos) //Поиск локальных минимумумов в квадратной окрестности
//    {
//        pix = (plane(pos)!=0)?255:0;
//        int x = pos[1];
//        int y = pos[0];
//        for (int c = -radius; c <= radius; ++c)
//            for (int r = -radius; r <= radius; ++r)
//            {
//                Point neighbor{x+c, y+r};
//                if (Rect({0,0},markers.size()).contains(neighbor))//0<=neighbor.x && neighbor.x<plane.cols && 0<neighbor.y && neighbor.y<plane.rows)
//                    if (plane(neighbor)<plane(pos)) pix = 0;
//            }
//    });
//    imshow("1",markers);
//    return sm;
//}

#endif // SEGMENTATIONMETHODS_H
