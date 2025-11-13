/*///////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018-2022, GCI Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////////////////*/
#ifndef GC3DRGS_H
#define GC3DRGS_H

#include<iostream>
#include<vector>
#include"../core/gc3dCoreTypes.h"
#include "rgsdef.h"
#include "gc3dtcp.h"


namespace gc3d {
/**
    * @brief initialRgs 用于初始化RGS库，必须要使用GCI 的加密狗
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS  initialRgs();

/**
    * @brief calibrate 标定函数接口
    * @param [in] gripLocals
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS  calibrate(std::vector<gc3d::GripperLocal>& gripLocals,std::vector<std::vector<gc3d::GPoint3f>>& cameraPoints,gc3d::GRotation& R,gc3d::GTranslation& T,
                                           gc3d::CalibrateError& error,GCIROBOTTYPE robType=GCI_SIX_ROBOT,GCICALTYPE calType=GCI_EYEINHAND,GCIPOINTTYPE ptType=GCI_POINT_UNORDER);

/**
    * @brief computeRTNPoint 标定函数接口,主要用于眼在手外系统，必须是正交直角坐标系
    * @param [in] cameraPoints      输入的相机坐标系下的3D特征点
    * @param [in] gripPoints        输入的机械手末端的接触特征点的位置
    * @param [inout] R              输出的旋转矩阵
    * @param [inout] T              输出的平移向量
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS computeRTNPoint(std::vector<gc3d::GPoint3f>& cameraPoints,std::vector<gc3d::GPoint3f>& gripPoints,gc3d::GRotation& R,gc3d::GTranslation& T);


/**
    * @brief handToBasePoint 用于将机械手坐标系下的点转换到基座坐标系
    * @param [in] srcp       输入的点
    * @param [in] grip       机械手的当前坐标
    * @param [inout] dstp       输出的点
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */

extern "C" DLLEXPORT RGS_STATUS  handToBasePoint(const GPoint3f srcp,GripperLocal& grip,GPoint3f& dstp);
/**
    * @brief convertToBasePoint 用于将相机坐标系下的点转换到基座坐标系
    * @param [in] srcp      输入的点
    * @param [in] grip      机械手的当前坐标
    * @param [inout] dstp      输出的点
    * @param [in] R      输入的相机到机械手的旋转矩阵
    * @param [in] T      输入的相机到机械手的平移向量
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS  convertToBasePoint(const GPoint3f srcp,GripperLocal& grip,GPoint3f& dstp,const gc3d::GRotation R,const gc3d::GTranslation T);
/**
    * @brief convertToHandPoint 用于将相机坐标系下的点转换到法兰坐标系
    * @param [in] srcp      输入的点
    * @param [inout] dstp      输出的点
    * @param [in] R      输入的相机到机械手的旋转矩阵
    * @param [in] T      输入的相机到机械手的平移向量
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS  convertToHandPoint(const GPoint3f srcp,GPoint3f& dstp,const gc3d::GRotation R,const gc3d::GTranslation T);
/**
    * @brief localGrips 用于定位的函数，该算法采用icp的方式进行配准，对模板点集和当前点集的大小无要求
    * @param [in] srcps             输入的模板点集
    * @param [in] dstps             输入的当前点集
    * @param [in] teachGrips        需要定位的机械手坐标点数组
    * @param [inout] outGrips       输出的定位的机械手坐标点数组
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS  localGrips(std::vector<GPoint3f>& srcps,std::vector<GPoint3f>& dstps,std::vector<GripperLocal>& teachGrips,
                                             std::vector<GripperLocal>& outGrips,GCIROBOTTYPE robType=GCI_SIX_ROBOT,GCILOCTYPE locType=GCI_XYZRXRYRZ);
/**
    * @brief localGrips 用于定位的函数，该算法采用icp的方式进行配准，对模板点集和当前点集的大小无要求
    * @param [in] srcps             输入的模板点集
    * @param [in] dstps             输入的当前点集
    * @param [in] teachGrips        需要定位的机械手坐标点数组
    * @param [inout] outGrips       输出的定位的机械手坐标点数组
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS  localGripsNP(std::vector<GPoint3f>& srcps,std::vector<GPoint3f>& dstps,std::vector<GripperLocal>& teachGrips,
                                               std::vector<GripperLocal>& outGrips,GCIROBOTTYPE robType=GCI_SIX_ROBOT);

/**
    * @brief OATtoEulerAngles 将川崎机械手的坐标角度O,A,T转化为欧拉角RX,RY,RZ
    * @param [inout] grip       输入(输出)的机械手坐标
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS  OATtoEulerAngles(gc3d::GripperLocal& grip);
/**
    * @brief EulerAnglestoOAT 将欧拉角RX,RY,RZ转化为川崎机械手的坐标角度O,A,T
    * @param [inout] grip       输入(输出)的机械手坐标
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS  EulerAnglestoOAT(gc3d::GripperLocal& grip);

/**
    * @brief ABCtoEulerAngles 将川崎机械手的坐标角度A,B,C转化为欧拉角RX,RY,RZ
    * @param [inout] grip       输入(输出)的机械手坐标
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS ABCtoEulerAngles(gc3d::GripperLocal& grip);

/**
    * @brief EulerAnglestoABC 将欧拉角RX,RY,RZ转化为川崎机械手的坐标角度A,B,C
    * @param [inout] grip       输入(输出)的机械手坐标
    * @return GC3D_SUCCESS or GC3D_SOFTDOG_FAIL
   */
extern "C" DLLEXPORT RGS_STATUS EulerAnglestoABC(gc3d::GripperLocal& grip);

}
#endif
