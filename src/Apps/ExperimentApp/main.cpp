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

    Mat3b moc = imread("C:\\qt_pro\\DST\\3_dataset\\dots_296.jpg");
    pyrMeanShiftFiltering(moc, moc, 10, 5, 0); //!!!!
    Mat1b plane(moc.size());
    Mat1i markers(moc.size());
    extractChannel(moc, plane, 0);// blue
    //plane.forEach([](uchar& pix, const int* pos){if(pix<5) pix=255;});
    Canny(plane,plane,5,10);

    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;
    findContours(plane, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    int compCount = 0;
    for(int idx = 0; idx >= 0; idx = hierarchy[idx][0], compCount++)
    {
       drawContours(markers, contours, idx, Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);
    }

/*
    int radius = 10;
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
    });*/

    watershed(moc, markers);
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
    Mat3b img = imread("C:\\qt_pro\\DST\\3_dataset\\dots_296.jpg");
    for (int id: sm.identifiers())
    {
        sm.cut(id).drawBorder(img);
    }    
    imshow( "image", img);

//    map<int, double> means_map;
//    for (int id = 0; id < sm.length() ;++id)
//    {
//        Segment seg = sm.cut(id);
//        means_map[id] = mean(seg.from(plane),seg.mask())[0];
//    }
/*
    DataFrame df("Names");

    for(int id = 0; id < sm.length() ;++id)
    {
        Segment seg = sm.cut(id);
        Mat3b fragment = seg.from(img);

        MorphoFeatures morphof(seg.mask());

        Mat1b channel_0(fragment.size());  extractChannel(fragment, channel_0, 0);
        Mat1b channel_1(fragment.size());  extractChannel(fragment, channel_1, 1);
        Mat1b channel_2(fragment.size());  extractChannel(fragment, channel_2, 2);

        ColorFeatures B_feat(channel_0, seg.mask(),"B_");   B_feat.all();
        ColorFeatures G_feat(channel_1, seg.mask(),"G_");   G_feat.all();
        ColorFeatures R_feat(channel_2, seg.mask(),"R_");   R_feat.all();

        cvtColor(fragment,fragment,cv::COLOR_BGR2HSV);

        extractChannel(fragment, channel_0, 0);
        extractChannel(fragment, channel_1, 1);
        extractChannel(fragment, channel_2, 2);

        ColorFeatures H_feat(channel_0, seg.mask(),"H_");   H_feat.all();
        ColorFeatures S_feat(channel_1, seg.mask(),"S_");   S_feat.all();
        ColorFeatures V_feat(channel_2, seg.mask(),"V_");   V_feat.all();


        ObjectDescription obj(to_string(id));
        obj << morphof.features()
            << sm.neighbors(id).size()
            << B_feat.features()
            << G_feat.features()
            << R_feat.features()
            << H_feat.features()
            << S_feat.features()
            << V_feat.features();

        if (id == 0)
        {
            list<string> header{"class"};
            header.splice(end(header), morphof.header());
            header.push_back("NEIGH");
            header.splice(end(header), B_feat.header());
            header.splice(end(header), G_feat.header());
            header.splice(end(header), R_feat.header());
            header.splice(end(header), H_feat.header());
            header.splice(end(header), S_feat.header());
            header.splice(end(header), V_feat.header());
            df.setHeader(header);
        }
        df.newObject(obj);
    }
    df.write("C:\\MelaHunter\\test.csv");
    cout << "END";
*/
    return a.exec();
}
