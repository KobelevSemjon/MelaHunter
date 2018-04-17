#ifndef SEGMENTATIONMAP_H
#define SEGMENTATIONMAP_H

#include "opencv2/opencv.hpp"
#include "segment.h"

using namespace std;
using namespace cv;
using AreaRectPair = pair<double,Rect>;

namespace std
{
/*! \brief Специализация шаблона less для cv::Vec<_Tp1, cn>.
 * \details Позволяет сравнивать 2 вектора по рекурентному правилу:
 *  ЕСЛИ lhs[i] != rhs[i] ТО ВЕРНУТЬ lhs[i]<rhs[i]
 *  ИНАЧЕ ПОВТОРИТЬ ДЛЯ i+1
 * Позволяет использовать cv::Vec<_Tp1, cn> как ключ в словарях, деревьях и т.д.
 */
template <typename _Tp1, int cn >
struct less<cv::Vec<_Tp1, cn>>
{
    bool operator()(const cv::Vec<_Tp1, cn>& lhs, const cv::Vec<_Tp1, cn>& rhs ) const
    {
        int lsize = sizeof(lhs)/sizeof(lhs[0]);
        for (int i=0; i<lsize; ++i)
            if (lhs[i]!=rhs[i])
                return lhs[i]<rhs[i];
        return false;
    }
};
}

/*! \brief Класс карты сегментации.
 * \details Обеспечивает вырезание сегментов изображения по границам одноцветных областей переданного
 * сегмнтированного изображения (segemented_image).
 */
class SegmentationMap
{
    Mat1i _segmented_image;
    map<int, AreaRectPair> _navigation_map;

public:
    SegmentationMap();
    SegmentationMap(Mat1i&& segmented_image, map<int, AreaRectPair>&& navigation_map);

    int id(const Point& point) const;

    Segment operator[](int segment_id);//!без проверок
    Segment cut(int segment_id);
    Segment cut(const Point& point);

    bool isExist(int id) const;
    bool isExist(const Point& point) const;
    bool isValid() const;
    bool isCompatible(const Mat& image) const;

    void join(int id1, int id2);
    int joinToEnviroment(int id);
    void combine(int area_threshold);

    int length() const;
    vector<int> identifiers() const;
    vector<int> neighbors(int id) const;

    double segmentArea(int id) const;
    double segmentArea(const Point& point) const;
    Rect segmentRect(int id) const;
    Rect segmentRect(const Point& point) const;

    Mat3b visualize()const;
    void drawBorders(Mat3b& image, int thickness = 1) const;
};


#endif // SEGMENTATIONMAP_H
