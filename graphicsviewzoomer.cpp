#include "graphicsviewzoomer.h"
#include <QWheelEvent>
#include <QTransform>

GraphicsViewZoomer::GraphicsViewZoomer(QGraphicsView* targetView, QObject* parent)
    : QObject(parent)
    , m_view(targetView)
{
    if (m_view) {
        m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        m_view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
        m_view->viewport()->installEventFilter(this);
    }
}

bool GraphicsViewZoomer::eventFilter(QObject* watched, QEvent* event)
{
    if (!m_view) return QObject::eventFilter(watched, event);

    if (event->type() == QEvent::Wheel) {
        auto* wheel = static_cast<QWheelEvent*>(event);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        const QPoint pixelDelta = wheel->pixelDelta();
        const QPoint angleDelta = wheel->angleDelta();
        int deltaY = angleDelta.isNull() ? pixelDelta.y() : angleDelta.y();
#else
        int deltaY = wheel->delta();
#endif
        if (deltaY != 0) {
            applyScale(deltaY > 0);
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}

void GraphicsViewZoomer::applyScale(bool zoomIn)
{
    const QTransform t = m_view->transform();
    const double currentScale = t.m11(); // 等比缩放，取 x 轴尺度
    double nextScale = currentScale * (zoomIn ? m_step : (1.0 / m_step));
    if (nextScale < m_minScale) nextScale = m_minScale;
    if (nextScale > m_maxScale) nextScale = m_maxScale;

    // 将当前视图等比重设为目标尺度：先归一化到 1，再缩放到 nextScale
    if (currentScale != 0.0) {
        m_view->scale(1.0 / currentScale, 1.0 / currentScale);
    }
    m_view->scale(nextScale, nextScale);
}



