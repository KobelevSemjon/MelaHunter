#ifndef CREATESEGMENTATIONMAP_H
#define CREATESEGMENTATIONMAP_H

#include <segmentationmap.h>

template<typename M>
SegmentationMap createSegmentationMap(const M& segmented_image)
{
    const M& simg = segmented_image;
    using Pixel = typename M::value_type;//класс пикселя внутри Mat_

    map<Pixel,int> match_table;
    int color_counter = 0;
    for (auto It = simg.begin(); It!=simg.end(); ++It)
    {
        if (match_table.find(*It) == match_table.end())
            match_table[*It] = --color_counter;
    }
    //каждому цвету ставится в соответствие уникальное отрицательное число
    //конвертация пикселей с сохранением уникальности цветов
    Mat1i _segmented_image = Mat1i::zeros(simg.size());
    _segmented_image.forEach([&](int& pix, const int* pos)
    {
        pix = match_table.at(simg(pos));
    });

    //заливка уникальными для области идентификаторами
    int segment_id = 0;
    map<int, AreaRectPair> _navigation_map;

    for (int y=0; y<_segmented_image.rows; ++y)
        for (int x=0; x<_segmented_image.cols; ++x)
        {
            int& pix = _segmented_image.at<int>(y,x);
            if (pix < 0) //признак незалитого пикселя
            {
                Rect bounding_rect;
                double segment_area = floodFill(_segmented_image, {x,y}, segment_id, &bounding_rect);
                _navigation_map[segment_id] = make_pair(segment_area, bounding_rect);
                ++segment_id;
            }
        }
    return SegmentationMap(move(_segmented_image), move(_navigation_map));
}
#endif // CREATESEGMENTATIONMAP_H
