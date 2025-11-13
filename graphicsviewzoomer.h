#ifndef GRAPHICSVIEWZOOMER_H
#define GRAPHICSVIEWZOOMER_H

#include <QObject>
#include <QGraphicsView>

class GraphicsViewZoomer : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsViewZoomer(QGraphicsView* targetView, QObject* parent = nullptr);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QGraphicsView* m_view;
    double m_minScale = 0.1;
    double m_maxScale = 10.0;
    double m_step = 1.15; // 每次缩放倍率

    void applyScale(bool zoomIn);
};

#endif // GRAPHICSVIEWZOOMER_H



