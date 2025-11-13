/*///////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018-2022, GCI Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////////////////*/
#pragma once
#ifndef GC3DPOINTSETPROC_H
#define GC3DPOINTSETPROC_H
#include <vector>
#include<array>
#include "core/gc3dTypes.h"
#include "imgproc/gc3dImage.h"
#include "imgproc/gc3dImgproc.h"
#include "feature2D/gc2dalgorithm.h"
namespace gc3d {

/**
* @brief computeInv  求旋转矩阵的逆矩阵
* @param [in] R:    配准计算得到的旋转矩阵
* @param [in] RInv:    配准计算得到的位移向量
* @note R*RInv=E
* @return
*/
DLLEXPORT  void computeInv( double R[][3],double RInv[][3]);

/**
* @brief computeRT  有序点云的配准
* @param [in] srcPoints:     输入的源无序的点云
* @param [in] dstPoints:     输入的目标无序的点云
* @param [in] R:    配准计算得到的旋转矩阵
* @param [in] T:    配准计算得到的位移向量
* @note dstPoints=R*srcPoints‘+T
* @return 配准点的最大误差
*/
DLLEXPORT  float computeRT(std::vector<GPoint3f>& srcPoints, std::vector<GPoint3f>&  dstPoints, double R[][3],double T[]);

/**
* @brief ICP  无序点云的配准
* @param [in] srcPoints:     输入的源无序的点云
* @param [in] dstPoints:     输入的目标无序的点云
* @param [out] R:    配准计算得到的旋转向量
* @param [out] T:    配准计算得到的位移向量
* @note dstPoints=R*srcPoints‘+T
* @return icp误差
*/
DLLEXPORT  float icp(std::vector<GPoint3f>& srcPoints, std::vector<GPoint3f>&  dstPoints,double R[][3],double T[]) ;

/**
* @brief ICPRotate  无序点云的配准,支持旋转角度
* @param [in] srcPoints:     输入的源无序的点云
* @param [in] dstPoints:     输入的目标无序的点云
* @param [out] R:    配准计算得到的旋转向量
* @param [out] T:    配准计算得到的位移向量
* @param [in] maxDis    用于收敛的最小距离，当两个点云的平均欧式距离小于该值，即认为已经配准，不宜设置的过小，参考值0.5
* @param [in] maxAngel:    配准计算得到的位移向量
* @note dstPoints=R*srcPoints‘+T
* @return icp误差
*/
DLLEXPORT  float icpRotate(std::vector<gc3d::GPoint3f>& srcPoints, std::vector<gc3d::GPoint3f>&  dstPoints,
                           double R[][3],double T[],const double maxDis=0.5,const double maxAngel=50,const double stepAngel=10) ;


/**
* @brief convertPointsRT  无序点云的配准
* @param [in] points:    输入的点云
* @param [in] R:    旋转向量
* @param [in] T:    位移向量
* @return 返回旋转平移后的点云
*/
DLLEXPORT std::vector<GPoint3f> convertPointsRT(std::vector<GPoint3f>& points, double R[][3], double T[]);

/**
* @brief symmetricFillHole        对称补洞操作
* @param [inout] data:          补洞的输入数据，补洞之后会在该地方传回
* @param [in] radius:           补洞的默认半径，1，3，5，...
* @param [in] threshold:        补洞的阈值，只有当输入的灰度大于一定的给定的阈值才会进行补洞，这个是针对高光部分一般将该值设置为200，低光补洞可以设置为0
* @param [in] times:            补洞迭代的次数
* @param [in] areaThreshold:    只有当洞的面积小于areaThreshold才会对该区域进行补洞
* @return
*/
DLLEXPORT  void cuGFillHole(GC3DMetaData& meta,int radius,int threshold,int times,int areaThreshold=500,unsigned char* addMask=nullptr);

/**
* @brief calDepthImg  对GC3DMetaData中的深度图进行重新计算
* @param [inout] data:          输入输出的数据
* @return
*/
DLLEXPORT  void calDepthImg(GC3DMetaData& data);

/**
* @brief calDepthImgWithRange  对GC3DMetaData中的深度图进行重新计算
* @param [inout] data:         输入输出的数据
* @param [in] zMin:            灰度分布的起始高度，低于此阈值的灰度分布为0
* @param [in] zMax:            灰度分布的终止高度，高于此阈值的灰度分布为255
* @param [in] isNeedTexture:   是否添加纹理图权重
* @param [in] textureRatio:    纹理权重的占比
* @return
*/
DLLEXPORT  void calDepthImgWithRange(GC3DMetaData& data,float zMin,float zMax,bool isNeedTexture = false ,float textureRatio = 0.01);

/**
* @brief calDepthImgGData  对GC3DGridData中的深度图进行重新计算
* @param [ino] data:         输入的数据
* @param [inout] depthImage: 输入输出的灰度分布的深度图
* @return
*/
DLLEXPORT  void calDepthImgGData(GC3DGridData& data,unsigned char* depthImage);

/**
* @brief changeDataFromBasePlane  根据给定的基准面，对GC3DMetaData中的点云数据进行校正
* @param [in] basePlaneA:       平面方程参数A
* @param [in] basePlaneB:       平面方程参数B
* @param [in] basePlaneC:       平面方程参数C
* @param [in] aveHeight:        z方向的平移距离
* @note  基准面方程: basePlaneA*x+basePlaneB*y+basePlaneC*z+1=0
* @return
*/
DLLEXPORT  void changeDataFromBasePlane(GC3DMetaData& data,double basePlaneA,double basePlaneB,double basePlaneC,double aveHeight);

/**
* @brief get3DPointsFromContours  获取2D轮廓中的3D轮廓点
* @param [inout] data:         输入输出的数据
* @param [in] contours:        输入的2D轮廓点集
* @param [inout] outputPoints: 输入输出的3D轮廓点集
* @return
*/
DLLEXPORT  void get3DPointsFromContours(GC3DMetaData& data,std::vector<GPoint>& contours,std::vector<GPoint3f>& outputPoints);

/**
* @brief gridData  数据的网格化
* @param [inout] data:          输入输出的数据
* @param [inout] deviceInfo:    设备信息
* @param [in] mode:             选择网格化的模式
* @param [inout] width:         宽度
* @param [inout] height:        高度
* @param [inout] xmin:          x的最小值
* @param [inout] xmax:          x的最大值
* @param [inout] ymin:          y的最小值
* @param [inout] ymax:          y的最大值
* @param [inout] dx:            x方向的间距
* @param [inout] dy:            y方向的间距
* @param [inout] ZMat:          对应的高度
* @param [inout] ZMask:         是否有点的mask矩阵
* @param [inout] textureData:   网格化后的纹理图
* @note  mode==0是固定分辨率，设置好width ,height 从而自动得到xmin ,xm ax ymin, ymax ,dx,dy
* @note  mode==1是固定距离,设置好dx dy 自动得到xmin ,xmax ymin, ymax ,width ，height
* @note  mode==2是固定xmin ,xmax ymin, ymax ,dx,dy自动得到width，height
* @return
*/
DLLEXPORT  bool gridData(GC3DMetaData& data,gc3d::DeviceInformation&deviceInfo, int mode, int& width, int &height,float& xmin,float& xmax,float& ymin,float& ymax, float &dx, float &dy, float*&ZMat, bool *&ZMask,unsigned char*&textureData);

/**
  @group 内部函数，不建议用户使用
  */
/**
* @brief symmetricFillHole  对称补洞操作
* @param [inout] data:      补洞的输入数据，补洞之后会在该地方传回
* @param [in] radius:       补洞的默认半径，1，3，5，...
* @param [in] threshold:    补洞的阈值，只有当输入的灰度大于一定的给定的阈值才会进行补洞，这个是针对高光部分一般将该值设置为200，低光补洞可以设置为0
* @param [in] times:        补洞迭代的次数
* @return
* @note 该函数为内部应用函数，不建议使用
*/
DLLEXPORT  void symmetricFillHole(GC3DMetaData& data,int radius,int threshold,int times);

/**
* @brief symmetricFillHole  对称补洞操作
* @param [inout] data:      补洞的输入数据，补洞之后会在该地方传回
* @param [in] radius:       补洞的默认半径，1，3，5，...
* @param [in] threshold:    补洞的阈值，只有当输入的灰度大于一定的给定的阈值才会进行补洞，这个是针对高光部分一般将该值设置为200，低光补洞可以设置为0
* @param [in] times:        补洞迭代的次数
* @param [in] mediakernelSize:  中值滤波半径
* @param [in] colseOpKerSize:   闭操作半径
* @param [in] areaThreshold:    洞的面积阈值
* @param [in] method:       1-中值滤波补洞；2-膨胀腐蚀补洞；3-中值滤波+膨胀腐蚀补洞；4-根据面积大小设置阈值补洞；5-根据面积大小设置阈值补洞+中值滤波补洞；
* 6-根据面积大小设置阈值补洞+膨胀腐蚀补洞；7-根据面积大小设置阈值补洞+中值滤波补洞+膨胀腐蚀补洞；
* @return
* @note 该函数为内部应用函数，不建议使用
*/
DLLEXPORT  void fillHoleM(GC3DMetaData& data,int radius,int threshold,int times,int mediakernelSize=3,int colseOpKerSize=3,int areaThreshold=10,int methods=1);

/**
* @brief symmetricFillHole  对称补洞操作
* @param [inout] data:      补洞的输入数据，补洞之后会在该地方传回
* @param [in] radius:       补洞的默认半径，1，3，5，...
* @param [in] threshold:    补洞的阈值，只有当输入的灰度大于一定的给定的阈值才会进行补洞，这个是针对高光部分一般将该值设置为200，低光补洞可以设置为0
* @param [in] times:        补洞迭代的次数
* @param [in] kernelSize:   补洞的核半径
* @note 该函数为内部应用函数，不建议使用
* @return
*/
DLLEXPORT  void fillHole(GC3DMetaData& data,int radius,int threshold,int times,int kernelSize=3);

/**
* @brief pointClusters  点云聚类操作
* @param [inout] data:      输入gci  Metadata数据
* @param [in] points:       输出的聚类点云
* @param [in] disThre:      距离阈值，相邻两个点小于这个值认为在一个聚类
* @param [in] searchRadiu:  相邻点搜索半径
* @note
* @return
*/
DLLEXPORT  void pointClusters(GC3DMetaData& data,std::vector<std::vector<gc3d::GPoint3f>>& points,const float disThre=0.5,const int searchRadiu=1);


}

#endif // GC3DPOINTSETPROC_H
