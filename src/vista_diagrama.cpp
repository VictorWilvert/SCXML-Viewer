
#include "../include/vista_diagrama.h"

void VistaDiagrama::wheelEvent(QWheelEvent *event)
{
    // calculos tipicos segundo Qt
    const int degrees = event->delta() / 8;
    int steps = degrees / 15;
    qreal scaleFactor = 0.05; //How fast we zoom
    const qreal minFactor = 0.01;
    const qreal maxFactor = 3.0;
    if(steps > 0)
    {
        h11 = (h11 >= maxFactor) ? h11 : (h11 + scaleFactor);
        h22 = (h22 >= maxFactor) ? h22 : (h22 + scaleFactor);
    }
    else
    {
        h11 = (h11 <= minFactor) ? minFactor : (h11 - scaleFactor);
        h22 = (h22 <= minFactor) ? minFactor : (h22 - scaleFactor);
    }
    //  fazendo o zoom
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setTransform(QTransform(h11,0, 0, h22,  0, 0));
}
