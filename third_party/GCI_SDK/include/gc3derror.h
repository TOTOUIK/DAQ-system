/*///////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018-2022, GCI Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////////////////*/
#ifndef GC3DERROR_H
#define GC3DERROR_H

//  GC3DErrorCode 错误码解析.
//
//返回的错误码是32位的int类型具体如下(前面的十六位一般用于的是GC3d的库，后面16位用于algorithm库)
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |S|DevId| partId|     Code      |S|LibId|        Err  Code      |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
//  where
//
//      S    - - indicates success/fail
//
//          0 - Success
//          1 - Fail (COERROR)
//
//      DevId - - 设备型号
//          0 - 单相机单光机
//          1 - 单相机二光机
//          2 - 2C1P
//          3 - 全部设备
//
//      PartId - - 部位
//          0 - 非实体部分
//          1 - 相机
//          2 - 光机
//      Code  - - 错误码
//      S    - - indicates success/fail
//
//          0 - Success
//          1 - Fail (COERROR)
//      LibId - - 库的名称
//          1 - algorithm库


#define GC3D_SUCCESS        0x00000000  //成功

//产品型号错误
#define GC3D_PRODUCTTYPE_FAIL           0x70010000  //产品型号错误
#define GC3D_SOFTDOG_FAIL               0x70020000  //加密狗错误
//非实体部分错误
#define GC3D_1C1P_ERROR_EXPOSURE_NUMBER      0x80010000   //不支持的曝光次数
#define GC3D_1C1P_ERROR_CUDA_COMPUTE3D       0x80020000   //cuda解码错误
#define GC3D_1C1P_ERROR_CUDA_COMPUTE3D_HDR   0x80030000   //cuda多曝光融合错误
#define GC3D_1C1P_ERROR_METADATA_NONE        0x80040000   //数据为空，不可获取
#define GC3D_1C1P_ERROR_DEVICEINITIAL_REPEAT 0x80050000   //设备重复初始化
#define GC3D_1C1P_ERROR_REGISTER             0x80060000   //配准失败
#define GC3D_1C1P_ERROR_GRIDDATA_INVALID     0x80070000   //规则化网格数据无效
#define GC3D_1C1P_ERROR_REGISTED             0x80080000   //已经注册回调

#define GC3D_1C2P_ERROR_EXPOSURE_NUMBER      0x90010000   //不支持的曝光次数
#define GC3D_1C2P_ERROR_CUDA_COMPUTE3D       0x90020000   //cuda解码错误
#define GC3D_1C2P_ERROR_CUDA_COMPUTE3D_HDR   0x90030000   //cuda多曝光融合错误
#define GC3D_1C2P_ERROR_METADATA_NONE        0x90040000   //数据为空，不可获取
#define GC3D_1C2P_ERROR_DEVICEINITIAL_REPEAT 0x90050000   //设备重复初始化
#define GC3D_1C2P_ERROR_REGISTER             0x90060000   //配准失败
#define GC3D_1C2P_ERROR_GRIDDATA_INVALID     0x90070000   //规则化网格数据无效
#define GC3D_1C2P_ERROR_REGISTED             0x90080000   //已经注册回调

#define GC3D_2C1P_ERROR_EXPOSURE_NUMBER      0xA0010000   //不支持的曝光次数
#define GC3D_2C1P_ERROR_CUDA_COMPUTE3D       0xA0020000   //cuda解码错误
#define GC3D_2C1P_ERROR_CUDA_COMPUTE3D_HDR   0xA0030000   //cuda多曝光融合错误
#define GC3D_2C1P_ERROR_METADATA_NONE        0xA0040000   //数据为空，不可获取
#define GC3D_2C1P_ERROR_DEVICEINITIAL_REPEAT 0xA0050000   //设备重复初始化
#define GC3D_2C1P_ERROR_REGISTER             0xA0060000   //配准失败
#define GC3D_2C1P_ERROR_GRIDDATA_INVALID     0xA0070000   //规则化网格数据无效
#define GC3D_2C1P_ERROR_REGISTED             0xA0080000   //已经注册回调

//光机错误
#define  GC3D_1C1P_ERROR_PROJECT_TRIGER      0x82010000 //相机触发错误
#define  GC3D_1C2P_ERROR_PROJECT_TRIGER      0x92010000 //相机触发错误
#define  GC3D_2C1P_ERROR_PROJECT_TRIGER      0xA2010000 //相机触发错误



//相机错误
#define GC3D_1C1P_CAM_CONNECT_FAIL           0x34300000  //相机连接失败
#define GC3D_1C1P_CAM_LIB_FAIL               0x34310000  //相机库匹配失败
#define GC3D_1C1P_CAM_INIT_FAIL              0x34320000  //相机初始化失败
#define GC3D_1C1P_ERROR_CAMERA_FRAME         0x34330000   //相机帧率不够
#define GC3D_1C1P_ERROR_CAMERA_NUM           0x34340000   //未查找到相机

#define GC3D_1C2P_CAM_CONNECT_FAIL           0x44300000  //相机连接失败
#define GC3D_1C2P_CAM_LIB_FAIL               0x44310000  //相机库匹配失败
#define GC3D_1C2P_CAM_INIT_FAIL              0x44320000  //相机初始化失败
#define GC3D_1C2P_ERROR_CAMERA_FRAME         0x44330000   //相机帧率不够

#define GC3D_2C1P_CAM_CONNECT_FAIL           0x54300000  //相机连接失败
#define GC3D_2C1P_CAM_LIB_FAIL               0x54310000  //相机库匹配失败
#define GC3D_2C1P_CAM_INIT_FAIL              0x54320000  //相机初始化失败
#define GC3D_2C1P_ERROR_CAMERA_FRAME         0x54330000   //相机帧率不够

//算法错误
#define GC3D_ALGORITHM_FITPLANE_FAIL              0x00009001  //平面拟合失败，矩阵奇异
#define GC3D_ALGORITHM_FITSPHERE_FAIL             0x00009002  //球拟合失败，矩阵奇异
#define GC3D_ALGORITHM_FITCIRCLE_FAIL             0x00009003  //圆拟合失败，矩阵奇异
#define GC3D_RGS_CALIBRATE_NUM_FAIL             0x00009004  //标定数据不够
#define GC3D_RGS_CALIBRATE_TYPE_FAIL             0x00009005  //机械手和标定类型设置不同


#endif // GC3DERROR_H
