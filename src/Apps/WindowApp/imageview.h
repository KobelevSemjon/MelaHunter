#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include <QGraphicsView>
#include <QWheelEvent>

class ImageView:public QGraphicsView
{
    Q_OBJECT
public:
    ImageView(QWidget *parent): QGraphicsView(parent){}
    void wheelEvent(QWheelEvent *event)
    {
        float delta=event->delta();
        float coef=1+delta/800;
        scale(coef,coef);
        centerOn(mapToScene(event->pos()));
    }
};

#endif // IMAGEVIEW_H
