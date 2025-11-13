/*///////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018-2022, GCI Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////////////////*/
#pragma once
#ifndef GC3DERRMSG_H
#define GC3DERRMSG_H
#include<string>
#include<unordered_map>
#include"gc3derror.h"
namespace gc3d {
extern std::unordered_map<unsigned int,std::string> errCode2Msg={{ GC3D_SUCCESS  ,"成功"},
                                                                 { GC3D_PRODUCTTYPE_FAIL          ,"产品型号错误"},
                                                                 { GC3D_SOFTDOG_FAIL                 ,"加密狗错误"},
                                                                 { GC3D_1C1P_ERROR_EXPOSURE_NUMBER         ,"不支持的曝光次数"},
                                                                 { GC3D_1C1P_ERROR_CUDA_COMPUTE3D          ,"cuda解码错误"},
                                                                 { GC3D_1C1P_ERROR_CUDA_COMPUTE3D_HDR      ,"cuda多曝光融合错误"},
                                                                 { GC3D_1C1P_ERROR_METADATA_NONE           ,"数据为空，不可获取"},
                                                                 { GC3D_1C1P_ERROR_DEVICEINITIAL_REPEAT    ,"设备重复初始化"},
                                                                 { GC3D_1C1P_ERROR_REGISTER                ,"配准失败"},
                                                                 { GC3D_1C1P_ERROR_GRIDDATA_INVALID        ,"规则化网格数据无效"},
                                                                 { GC3D_1C1P_ERROR_REGISTED                ,"已经注册回调"},
                                                                 { GC3D_1C2P_ERROR_EXPOSURE_NUMBER         ,"不支持的曝光次数"},
                                                                 { GC3D_1C2P_ERROR_CUDA_COMPUTE3D          ,"cuda解码错误"},
                                                                 { GC3D_1C2P_ERROR_CUDA_COMPUTE3D_HDR      ,"cuda多曝光融合错误"},
                                                                 { GC3D_1C2P_ERROR_METADATA_NONE           ,"数据为空，不可获取"},
                                                                 { GC3D_1C2P_ERROR_DEVICEINITIAL_REPEAT    ,"设备重复初始化"},
                                                                 { GC3D_1C2P_ERROR_REGISTER                ,"配准失败"},
                                                                 { GC3D_1C2P_ERROR_GRIDDATA_INVALID        ,"规则化网格数据无效"},
                                                                 { GC3D_1C2P_ERROR_REGISTED                ,"已经注册回调"},
                                                                 { GC3D_2C1P_ERROR_EXPOSURE_NUMBER         ,"不支持的曝光次数"},
                                                                 { GC3D_2C1P_ERROR_CUDA_COMPUTE3D          ,"cuda解码错误"},
                                                                 { GC3D_2C1P_ERROR_CUDA_COMPUTE3D_HDR      ,"cuda多曝光融合错误"},
                                                                 { GC3D_2C1P_ERROR_METADATA_NONE           ,"数据为空，不可获取"},
                                                                 { GC3D_2C1P_ERROR_DEVICEINITIAL_REPEAT    ,"设备重复初始化"},
                                                                 { GC3D_2C1P_ERROR_REGISTER                ,"配准失败"},
                                                                 { GC3D_2C1P_ERROR_GRIDDATA_INVALID        ,"规则化网格数据无效"},
                                                                 { GC3D_2C1P_ERROR_REGISTED                ,"已经注册回调"},
                                                                 {  GC3D_1C1P_ERROR_PROJECT_TRIGER       ,"相机触发错误"},
                                                                 {  GC3D_1C2P_ERROR_PROJECT_TRIGER       ,"相机触发错误"},
                                                                 {  GC3D_2C1P_ERROR_PROJECT_TRIGER       ,"相机触发错误"},
                                                                 { GC3D_1C1P_CAM_CONNECT_FAIL           ,"相机连接失败"},
                                                                 { GC3D_1C1P_CAM_LIB_FAIL               ,"相机库匹配失败"},
                                                                 { GC3D_1C1P_CAM_INIT_FAIL              ,"相机初始化失败"},
                                                                 { GC3D_1C1P_ERROR_CAMERA_FRAME          ,"相机帧率不够"},
                                                                 { GC3D_1C1P_ERROR_CAMERA_NUM          ,"未查找到相机"},
                                                                 { GC3D_1C2P_CAM_CONNECT_FAIL           ,"相机连接失败"},
                                                                 { GC3D_1C2P_CAM_LIB_FAIL               ,"相机库匹配失败"},
                                                                 { GC3D_1C2P_CAM_INIT_FAIL              ,"相机初始化失败"},
                                                                 { GC3D_1C2P_ERROR_CAMERA_FRAME          ,"相机帧率不够"},
                                                                 { GC3D_2C1P_CAM_CONNECT_FAIL           ,"相机连接失败"},
                                                                 { GC3D_2C1P_CAM_LIB_FAIL               ,"相机库匹配失败"},
                                                                 { GC3D_2C1P_CAM_INIT_FAIL              ,"相机初始化失败"},
                                                                 { GC3D_2C1P_ERROR_CAMERA_FRAME          ,"相机帧率不够"},
                                                                 { GC3D_ALGORITHM_FITPLANE_FAIL             ,"平面拟合失败，矩阵奇异"},
                                                                 { GC3D_ALGORITHM_FITSPHERE_FAIL            ,"球拟合失败，矩阵奇异"},
                                                                 { GC3D_ALGORITHM_FITCIRCLE_FAIL            ,"圆拟合失败，矩阵奇异"},
                                                                 { GC3D_RGS_CALIBRATE_NUM_FAIL             ,"标定数据不够"},
                                                          };
}

#endif // GC3DERRMSG_H

