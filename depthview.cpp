#include "DepthView.h"
#include <QEvent>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QDebug>

DepthView::DepthView(QObject *parent)
    : QObject(parent)
{ }

void DepthView::attachToView(QGraphicsView *view)
{
    if (m_view && m_view == view) return;
    if (m_view) m_view->removeEventFilter(this);
    m_view = view;
    if (m_view) {
        // 安装 event filter 到 viewport，保证收到鼠标移动事件
        m_view->viewport()->installEventFilter(this);
        // 开启鼠标跟踪（即使没有按键按下也能收到 MouseMove）
        m_view->viewport()->setMouseTracking(true);
        m_view->setMouseTracking(true);
    }
}

void DepthView::setMetaData(const gc3d::GC3DMetaData *meta)
{
    m_meta = meta;
}

void DepthView::setLabelXYZ(QLabel *label)
{
    m_labelXYZ = label;
    if (m_labelXYZ) {
        m_labelXYZ->setText("");
    }
}

bool DepthView::eventFilter(QObject *watched, QEvent *event)
{
    if (!m_view) return QObject::eventFilter(watched, event);

    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        // 将视口坐标转换为 scene 坐标
        QPointF scenePt = m_view->mapToScene(me->pos());
        QRectF bounds = m_view->scene() ? m_view->scene()->itemsBoundingRect() : QRectF();
        if (bounds.isNull()) {
            if (m_labelXYZ) m_labelXYZ->setText("");
            return QObject::eventFilter(watched, event);
        }

        // 将 scene 坐标映射为图像像素坐标（scene 的 bounds 对应图像实际像素）
        double sx = (scenePt.x() - bounds.left());
        double sy = (scenePt.y() - bounds.top());
        int imgX = int(std::round(sx));
        int imgY = int(std::round(sy));

        // 若 meta 有效则按 meta 分辨率比例映射
        if (m_meta && m_meta->imgW > 0 && m_meta->imgH > 0) {
            double fx = bounds.width() / double(m_meta->imgW);
            double fy = bounds.height() / double(m_meta->imgH);
            if (fx != 0 && fy != 0) {
                imgX = int((scenePt.x() - bounds.left()) / fx + 0.5);
                imgY = int((scenePt.y() - bounds.top()) / fy + 0.5);
            }
        }

        // clamp
        if (imgX >= 0 && imgY >= 0) {
            updateLabelForPos(imgX, imgY);
            emit mousePositionChanged(imgX, imgY);
        } else {
            if (m_labelXYZ) m_labelXYZ->setText("");
        }
    }
    return QObject::eventFilter(watched, event);
}

void DepthView::updateLabelForPos(int ix, int iy)
{
    if (!m_labelXYZ) return;
    if (!m_meta) {
        m_labelXYZ->setText("");
        return;
    }
    if (ix < 0 || iy < 0 || ix >= m_meta->imgW || iy >= m_meta->imgH) {
        m_labelXYZ->setText("");
        return;
    }
    int idx = iy * m_meta->imgW + ix;
    if (!m_meta->maskflag) {
        m_labelXYZ->setText("");
        return;
    }
    bool valid = m_meta->maskflag[idx];
    if (!valid) {
        m_labelXYZ->setText(QString("X,Y,Z: invalid"));
        return;
    }
    float x = 0.0f, y = 0.0f, z = 0.0f;
    if (m_meta->x) x = m_meta->x[idx];
    if (m_meta->y) y = m_meta->y[idx];
    if (m_meta->z) z = m_meta->z[idx];

    m_labelXYZ->setText(QString("X:%1  Y:%2  Z:%3").arg(x,0,'f',3).arg(y,0,'f',3).arg(z,0,'f',3));
}
