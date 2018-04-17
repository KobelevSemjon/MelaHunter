#ifndef SEGMENTATION_H
#define SEGMENTATION_H
/*! \defgroup Segmentation Модуль сегментации
 * \brief Набор классов и методов, обеспечивающих индивидуальный доступ к сегментам
 * изображения, необходимого для последующего расчета числовых признаков.
 * Пример использования
 * \code
int main(int argc, char *argv[])
{
    using namespace cv;
    QCoreApplication a(argc, argv);

    int radius =5;
    Mat1b image = Mat1b::zeros(2*radius+1,2*radius+1);
    circle(image,{radius,radius},radius,255,-1);
    cout << "original image:" << endl;
    cout << image << endl;

    auto sm = createSegmentationMap(image);
    imshow("marked image", sm.visualize());

    for (int id :sm.identifiers())
    {
        Segment seg = sm.cut(id);
        cout << format("%d segment mean =",id);
        cout << mean(seg.from(image),seg.mask())[0] << endl;
    }
    return a.exec();
}\endcode
*/

#include "segment.h"
#include "segmentationmap.h"
#include "createsegmentationmap.h"
#include "segmentationmethods.h"

#endif // SEGMENTATION_H
