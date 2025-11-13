#ifndef GC2DALGORITHM_H
#define GC2DALGORITHM_H
#include "../core/gc3dTypes.h"


namespace gc3d {


/**
* @brief fitCircle 由输入的点拟合圆，采用最小二乘法
* @param [in] points 输入的2D点，大部分时候是图像上的二维点
* @param [cir] cir 输出的拟合出的圆
* @return 圆度，越小代表拟合的圆误差越小
*/
DLLEXPORT double fitCircle(const std::vector<gc3d::GPoint> &points, gc3d::GCircle& cir );

/**
* @brief fitLine 由输入的点拟合直线，采用最小二乘法
* @param [in] points 输入的2D点，大部分时候是图像上的二维点
* @param [cir] line 输出的拟合出的直线
* @return 直线度，越小代表拟合的直线误差越小
*/
DLLEXPORT double fitLine(const std::vector<gc3d::GPoint> &points, gc3d::GLine& line );

/**
* @brief crossLines 计算两条直线的交点
* @param [in] line1 第一条直线
* @param [in] line2 第二条直线
* @param [in] point 输出的交点
* @return 若有交点，则返回true，否则返回false
*/
DLLEXPORT bool crossLines(gc3d::GLine& line1,gc3d::GLine& line2 ,gc3d::GPoint& point);


/**
* @brief getLinePoints2D  获取两个点连线之间的2D点集合
* @param [in]   p1:       输入的第一个点
* @param [in]   p2:        输入的第二个点
* @param [inout]    pts    输出的2D点的集合
* @return
*/
DLLEXPORT void getLinePoints2D(GPoint p1,GPoint p2,std::vector<GPoint>& pts);


/**
* @brief convexHullEx  从一系列点集中计算凸包
* @param [in]   points:       输入的点击
* @return 输出的凸包点集
*/
DLLEXPORT std::vector<gc3d::GPoint> convexHull(const std::vector<gc3d::GPoint>& points);

/**
* @brief minAreaRect 返回点集的最小矩形
* @param [in] inPoints: 输入的点集
* @return 点集的最小矩形
*/
DLLEXPORT gc3d::GRotationRect minAreaRect(std::vector<gc3d::GPoint>& inPoints);


/**
* @brief boundingRect 求点集的最小矩形
* @param [in] points: 要求最小矩形的点集
* @return 最小矩形
*/
DLLEXPORT gc3d::GRect boundingRect(std::vector<gc3d::GPoint>& points);

/**
* @brief findHomography 根据输入的对应两组点集找单应性矩阵
* @param [in] srcPoint: 输入的初始点集
* @param [in] dstPoint: 输入的目标点集
* @param [out] h: 输出的单应性矩阵
* @return true 成功 false  找失败，一般来说点数量不够失败
*/
DLLEXPORT bool findHomography(std::vector<gc3d::GPointf>& srcPoints,std::vector<gc3d::GPointf>& dstPoints,float h[][3]);


/**
* @brief perspectiveTransform 根据输入的对应两组点集找单应性矩阵
* @param [in] h: 输入的单应性矩阵
* @param [in] oint: 输入的目标点集
* @return 返回单应性变换后的点
*/
DLLEXPORT gc3d::GPointf perspectiveTransform(float h[][3],gc3d::GPointf& point);

}



#endif // GC2DALGORITHM_H
