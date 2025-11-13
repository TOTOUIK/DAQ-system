/*///////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018-2022, GCI Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////////////////*/
#ifndef GC3DDEF_H
#define GC3DDEF_H
#include <string>
#include <functional>
#ifdef GCI_OS_WIN32
#define  DLLEXPORT __declspec(dllexport)
#else
#define  DLLEXPORT
#endif
namespace gc3d {

/**
 *  @brief 结构体GC3DMetaData是3D相机采集输出的结果
 * 利用函数snapShot3D进行一次拍摄之后都可以采用函数getGC3DMetaData获取得到GC3DMetaData结构体内的相关信息。
 * 该结构体中包含的相关信息主要有3D相机的图像分辨率imgW，imgH，获取得到的顶点信息x,y,z，法相信息nx,ny,nz,纹理
 * 图像textureData，深度图像depthImageData，条纹预览图像previewImgData，图像上每个点是否有效的信息maskflag，
 * 以及顶点的个数信息validPointsNum。
 * @note  结构体中相机采集得到的顶点信息x,y,z，法相信息nx,ny,nz,纹理图像textureData，深度图像depthImageData，
 * 条纹预览图像previewImgData，以及点是否有效的信息maskflag都是采用数组的形式存储，且一一对应的。
 * 图像坐标(i,j)，其对应的数组序号为idx=i*imgW+j利用该序号可以获取得到该坐标系的空间点信息x[idx],y[idx], z[idx];
 * 其他对应的法相纹理等对应信息可以用该序号idx获得。
  */
struct DLLEXPORT GC3DMetaData{
    int imgW, imgH;                                           //!<图像的宽imgW 和高 imgH
    float *x  = nullptr,  *y  = nullptr,  *z  = nullptr;      //!<顶点的空间坐标数组，大小为imgW*imgH,数组为行有先
    float *nx = nullptr,  *ny = nullptr,  *nz = nullptr;      //!<顶点的法相信息数组，大小为imgW*imgH,数组为行有先
    unsigned char *textureData = nullptr;                     //!<纹理图像数据数组，分辨率为imgW*imgH
    unsigned char *depthImageData = nullptr;                  //!<深度图像数据数组，分辨率为imgW*imgH
    unsigned char *previewImgData = nullptr;                  //!<预览图像数据数组，分辨率为imgW*imgH
    bool *maskflag = nullptr;                                 //!<有效点标志位数组，分辨率为imgW*imgH
    int validPointsNum = 0;                                   //!<有效点的个数
};

/**
 *  @brief 结构体GC3DGridData是输出格式化数据
 * 首先采用setNeedGridData设置需要输出GC3DGridData，利用函数snapShot3D进行一次拍摄之后都可以采用函数getGC3DGridData
 * 获取得到GC3DGridData结构体内的相关信息。 该结构体中包含的相关信息主要有3D相机的格式化后的图像分辨率width，height，X、Y轴方向
 * 固定间距dx,dy;以及每个点对应的高度信息depthImageData，以及格式化后新数据的纹理信息textureData；数据有效信息maskflag，以及
 * 有效的点个数validPointsNum。
 * @note 该数据类型一般是为了适配传统基于线激光软件的等间距数据要求，默认情况下不开启。
  */
struct DLLEXPORT GC3DGridData{
    float dx,dy;                                              //!<格式化数据的X轴以及Y轴的固定间距dx,dy
    int width,height;                                         //!<格式化数据的图像的分辨率宽width 和高 height
    float* depthImageData;                                    //!<格式化数据的对应的高度信息数组，分辨率为height*width
    unsigned char *textureData = nullptr;                     //!<格式化数据的纹理图像数据数组  ，分辨率为height*width
    bool *maskflag = nullptr;                                 //!<格式化数据的有效点标志位数组  ，分辨率为height*width
    int validPointsNum = 0;                                   //!<格式化数据的有效点的个数
};

/**
 *  @brief SetupTypes是传感器的组成型号
  */
enum SetupTypes {
    NoCam,                                                    //!<默认无相机
    Cam1Prj1,                                                 //!<传感器组成是一个相机和一个投影仪
    Cam1Prj2,                                                 //!<传感器组成是一个相机和两个投影仪
    Cam2Prj1                                                  //!<传感器组成是两个相机和一个投影仪
};

/**
 *  @brief 结构体DeviceInformation是GCI 3D相机相关设置参数
 * 利用函数getDeviceInfo可以获取得到GCI 3D相机的设备信息主要有3D相机传感器分辨率：sensorWidth、sensorHeight；产品类型：setupType；
 * 产品序列号：serialNum；产品类型：productType。
 *  @note 该函数主要可以应用于在未启动相机时候获取得到设备相关信息。
  */
struct DeviceInformation{
    int sensorWidth ,sensorHeight ;                          //!<传感器的分辨率宽sensorWidth 和高 sensorHeight
    SetupTypes setupType = NoCam;                            //!<传感器的产品类型
    std::string serialNum = "";                              //!<传感器的序列号
    std::string productType = "";                            //!<传感器的产品型号
};

/**
 *  @brief 结构体GC3DCameraParameters是GCI 3D相机的设置的参数
 * 利用函数setCameraParameters和getCameraParameters可以设置以及获取得到3D相机的相关参数。GCI 3D相机的设置参数主要有曝光次数，曝光时间，
 * Gamma使能，Gamma值，相机增益。
  */
struct GC3DCameraParameters{
    int exposureNum = 1;                                      //!<3D相机的曝光次数
    int exposureTime = 800;                                   //!<3D相机的曝光时间，单位us
    int exposureTime2 = 2000;                                 //!<3D相机的第二次曝光时间，单位us
    int exposureTime3 = 3000;                                 //!<3D相机的第三次曝光时间，单位us
    bool enableGamma = false;                                 //!<3D相机的Gamma使能，一般设置false，否则影响成像
    double gammaValue = 0.8;                                  //!<3D相机的gamma参数，一般不设置，否则影响成像
    double gain = 0.0;						                  //!<3D相机的增益设置
};



/**
 *  @brief 用于处理相机获取得到的数据的回调函数callBack
 * @param [inout] GC3DMetaData需要回调函数处理的采集数据
 * @param [inout] GC3DGridData需要回调函数处理的格式化数据
 * @param [in]   回调函数中传入的次数
  */
typedef std::function<void(GC3DMetaData&,GC3DGridData&,int)> callBack;
};
#endif // GC3DDEF_H
