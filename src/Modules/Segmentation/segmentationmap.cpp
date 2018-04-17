#include "segmentationmap.h"

//! \brief Пустой конструктор
SegmentationMap::SegmentationMap(){}

//! \brief Специальный конструктор
SegmentationMap::SegmentationMap(Mat1i &&segmented_image, map<int, AreaRectPair> &&navigation_map):
    _segmented_image(segmented_image),
    _navigation_map(navigation_map){}

int SegmentationMap::id(const Point &point) const {return _segmented_image(point);}

Segment SegmentationMap::operator[](int segment_id)
{
    Rect bounding_rect = segmentRect(segment_id);
    Mat1b mask(bounding_rect.size());

    _segmented_image(bounding_rect).forEach([&](int& pix, const int* pos)
    {
        mask(pos) = (pix==segment_id)?255:0;
    });
    return Segment(bounding_rect, mask);
}

//! \brief Вырезать сегмент
Segment SegmentationMap::cut(int segment_id)
{
    if (!isExist(segment_id))
    {
        string massage = format("SegmentationMap::cut: segment_id ==%d doesn't exist", segment_id);
        throw invalid_argument(massage);
    }
    return operator [](segment_id);

}

Segment SegmentationMap::cut(const Point &point)
{
    if (!isExist(point))
    {
        string massage = format("SegmentationMap::cut: point == {%d; %d} doesn't exist", point.x, point.y);
        throw invalid_argument(massage);
    }
    return operator [](id(point));
}

bool SegmentationMap::isExist(int id) const
{return _navigation_map.find(id)!=_navigation_map.end();}

bool SegmentationMap::isExist(const Point &point) const
{
    Rect rect({0,0},_segmented_image.size());
    return point.inside(rect);
}


bool SegmentationMap::isValid() const
    {return !(_navigation_map.empty());}


bool SegmentationMap::isCompatible(const Mat &image) const
{return (image.rows == _segmented_image.rows) && (image.cols == _segmented_image.cols);}

//! \todo Добавить проверки на существование id1 и id2
void SegmentationMap::join(int id1, int id2)
{
    assert (isExist(id1) && isExist(id2));

    Rect rect1 = segmentRect(id1);
    Rect rect2 = segmentRect(id2);

    int joined_area = segmentArea(id1) + segmentArea(id2);
    Rect joined_rect = rect1 | rect2;

    for (int y = rect2.y; y<= rect2.br().y; ++y)
        for (int x = rect2.x; x<= rect2.br().x; ++x)
        {
            int& pix = _segmented_image({x,y});
            if (pix==id2) pix=id1;
        }

    _navigation_map.at(id1) = make_pair(joined_area,joined_rect);

    _navigation_map.erase(id2);
}
//! Теперь гарантирует связность
int SegmentationMap::joinToEnviroment(int id)
{
    Rect seg_rect = segmentRect(id);
    Rect img_rect = Rect({0,0},_segmented_image.size());

    map<int,set<Vec2i>> boarders;

    for(int y=seg_rect.y-1;y<=seg_rect.br().y+1;++y)
    for(int x=seg_rect.x-1;x<=seg_rect.br().x+1;++x)
        if(img_rect.contains({x,y}) && (_segmented_image({x,y})==id))
            for (int j = -1; j<= 1; ++j)
            for (int i = -1; i<= 1; ++i)
            {
                if(img_rect.contains({x+i,y+j}))
                {
                    int nid = _segmented_image({x+i,y+j});
                    if (nid!=id) boarders[nid].insert({x+i,y+j});
                }
            }

    int new_id;
    size_t max_length=0;
    for(const pair<int,set<Vec2i>>& b: boarders)
        if (b.second.size() > max_length)
        {
            new_id = b.first;
            max_length = b.second.size();
        }

    join(new_id,id);
    return new_id;
}

void SegmentationMap::combine(int area_threshold)
{
    if (area_threshold<1) return;
    list<int> combine_list;
    for (auto p: _navigation_map)
    {
        int area = p.second.first;
        if (area < area_threshold)
            combine_list.push_back(p.first);
    }
    for (int id: combine_list)
        joinToEnviroment(id);
}

int SegmentationMap::length() const {return int(_navigation_map.size());}

vector<int> SegmentationMap::identifiers() const
{
    list<int> keys;

    for (const auto& p: _navigation_map)
        keys.push_back(p.first);

    return vector<int>(keys.begin(),keys.end());
}

vector<int> SegmentationMap::neighbors(int id) const
{
    Rect seg_rect = segmentRect(id);
    Rect img_rect = Rect({0,0},_segmented_image.size());
    set<int> neigh_set{id};

    for (int y = seg_rect.y-1; y<= seg_rect.br().y+1; ++y)
    for (int x = seg_rect.x-1; x<= seg_rect.br().x+1; ++x)
    {
        if (img_rect.contains({x,y}))
        {
            int nid = _segmented_image({x,y});
            if (neigh_set.find(nid)==neigh_set.end())
                for (int j = -1; j<= 1; ++j)
                for (int i = -1; i<= 1; ++i)
                    if (img_rect.contains({x+i,y+j}) && _segmented_image({x+i,y+j}) == id)
                    {
                        neigh_set.insert(nid);
                        i=j=1;//break
                    }
        }
    }
    neigh_set.erase(id);
    return vector<int>(neigh_set.begin(),neigh_set.end());
}

double SegmentationMap::segmentArea(int id) const
{
    return _navigation_map.at(id).first;
}

double SegmentationMap::segmentArea(const Point &point) const
{
    return _navigation_map.at(id(point)).first;
}

Rect SegmentationMap::segmentRect(int id) const
{
    return _navigation_map.at(id).second;
}

Rect SegmentationMap::segmentRect(const Point &point) const
{
    return _navigation_map.at(id(point)).second;
}

Mat3b SegmentationMap::visualize() const
{
    map<int,Vec3b> colors;
    int width = _segmented_image.cols;
    int height = _segmented_image.rows;

    for(int y=0;y<height;++y)
        for(int x=0;x<width;++x)
        {
            int pix=_segmented_image({x,y});
            if (colors.find(pix)==colors.end())
                colors[pix]={uchar(rand()%256),uchar(rand()%256),uchar(rand()%256)};
        }

    Mat3b ret(_segmented_image.size());
    ret.forEach([&](Vec3b& pix, const int* pos)
        {
            int seg_id = _segmented_image(pos);
            pix = colors[seg_id];
        });
    return ret;
}

void SegmentationMap::drawBorders(Mat3b& image, int thickness) const//!без проверок
{
    map<int,Vec3b> colors;
    int width = _segmented_image.cols;
    int height = _segmented_image.rows;

    for(int y=0;y<height;++y)
        for(int x=0;x<width;++x)
        {
            int pix=_segmented_image({x,y});
            if (colors.find(pix)==colors.end())
                colors[pix]={uchar(rand()%256),uchar(rand()%256),uchar(rand()%256)};

            int& t = thickness;

            for (int ny=-t;ny<=t;++ny)
                for (int nx=-t;nx<=t;++nx)
                {
                    Point n{x+nx, y+ny};
                    if (!(0<=n.x && n.x<width && 0<n.y && n.y<height) || pix!=_segmented_image(n))
                    {
                        image({x,y}) = colors.at(pix);
                        nx=ny=t;//break;
                    }
                }
        }
}
