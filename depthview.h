#ifndef DEPTHVIEW_H
#define DEPTHVIEW_H

#include <QGraphicsView>
#include <QLabel>
#include <QObject>
#include <QPointer>
#include "gc3ddef.h"

// class DepthView : public QGraphicsView
// {
//     Q_OBJECT
// public:
//     explicit DepthView(QWidget *parent = nullptr);

//     void setMetaData(const gc3d::GC3DMetaData &meta);
//     void setXYZLabel(QLabel *label);

// protected:
//     void mouseMoveEvent(QMouseEvent *event) override;

// private:
//     gc3d::GC3DMetaData m_meta;
//     bool m_hasMeta = false;
//     QLabel *m_labelXYZ = nullptr;
// };

// #endif


class DepthView : public QObject
{
    Q_OBJECT
public:
    explicit DepthView(QObject *parent = nullptr);

    // 把已有的 QGraphicsView 传进来（UI 中的 graphicsCamera）
    void attachToView(QGraphicsView *view);

    // 设置用于查询 xyz 的元数据（CameraController 保留的 last meta）
    void setMetaData(const gc3d::GC3DMetaData *meta);

    // 设置 label 用于显示 xyz（在右下角或任意位置）
    void setLabelXYZ(QLabel *label);

signals:
    void mousePositionChanged(int imageX, int imageY);

protected:
    // event filter 用于捕获鼠标移动
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QPointer<QGraphicsView> m_view;
    const gc3d::GC3DMetaData *m_meta = nullptr;
    QPointer<QLabel> m_labelXYZ;
    void updateLabelForPos(int ix, int iy);
};

#endif // DEPTHVIEW_H
