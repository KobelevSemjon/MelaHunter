#include <QCoreApplication>
#include <QDebug>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "segmentation.h"
#include "description.h"

int main(int argc, char *argv[])
{
    using namespace std;
    using namespace cv;

    QCoreApplication a(argc, argv);

    Mat3b img = imread("C:\\qt_pro\\DST\\3_dataset\\dots_296.jpg");
    Mat1b plane(img.size());
    extractChannel(img, plane, 0);// blue
    Mat1i markers(img.size());

    int radius = 5;
    //Локальные минимумы
    markers.forEach([&](int& pix, const int* pos)
    {
        pix = (plane(pos)!=0)?255:0;
        int x = pos[1];
        int y = pos[0];
        for (int c = -radius; c <= radius; ++c)
            for (int r = -radius; r <= radius; ++r)
            {
                Point neighbor{x+c, y+r};
                if (Rect({0,0},markers.size()).contains(neighbor))//0<=neighbor.x && neighbor.x<plane.cols && 0<neighbor.y && neighbor.y<plane.rows)
                    if (plane(neighbor)<plane(pos)) pix = 0;
            }
    });
    int mark_counter = 1;
    markers.forEach([&](int& pix, const int* pos)
    {
        if (pix!=0)
        {
            Point seed = {pos[1], pos[0]};
            floodFill(markers,seed,0);
            pix = mark_counter++;
        }
    });

    watershed(img, markers);
    markers.forEach([&](int& pix, const int* pos)
    {
        if (pix==-1)
        {
            Point p = {pos[1], pos[0]};
            Rect img_rect = Rect({0,0},markers.size());
            uchar winner_value = 255;

            for (int c = -1; c <= 1; ++c)
            for (int r = -1; r <= 1; ++r)
            {
                Point neigh{p.x+c, p.y+r};
                if (neigh.inside(img_rect))
                    if (markers(neigh)>-1 && winner_value>plane(neigh))
                    {
                        winner_value = plane(neigh);
                        pix = markers(neigh);
                    }
            }
        }
    });

    auto sm = createSegmentationMap(markers); //Составление карты сегментации
//    map<int, double> means_map;
//    for (int id = 0; id < sm.length() ;++id)
//    {
//        Segment seg = sm.cut(id);
//        means_map[id] = mean(seg.from(plane),seg.mask())[0];
//    }

    DataFrame df("Names");

    for(int id = 0; id < sm.length() ;++id)
    {
        Segment seg = sm.cut(id);
        ColorFeatures colorf(seg.from(plane),Mat1b(seg.mask()),"B_");
        MorphoFeatures morphof(seg.from(plane),Mat1b(seg.mask()));
        colorf.all(); morphof.all();


        ObjectDescription obj(to_string(id));
        obj << colorf.features()
            << morphof.features()
            << sm.neighbors(id).size();
        if (id == 0)
        {
            list<string> header{"class"};
            header.splice(end(header), colorf.header());
            header.splice(end(header), morphof.header());
            header.push_back("NEIGH");
            df.setHeader(header);
        }
        df.newObject(obj);
    }
    df.write("C:\\MelaHunter\\test.csv");

    return a.exec();
}
