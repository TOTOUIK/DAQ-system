/*///////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018-2022, GCI Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////////////////*/
#ifndef GC3D_H
#define GC3D_H
#include "gc3derror.h"
#include "gc3ddef.h"


namespace gc3d {
/**
 *  @defgroup GCI3D相机的GC3DDevice相关参数设置、存图等相关操作
 */
class  DLLEXPORT GC3DDevice
{
    public:
    GC3DDevice();
    ~GC3DDevice();

    /**
    * @brief 函数initialDevice负责初始化设备
    * @return 返回是否初始化成功错误代码
   */
    uint32_t initialDevice();
    /**
      * @brief 函数closeDevice负责关闭设备，释放内存
      * @return
    */
    void closeDevice();
    /**
      * @brief 函数detectCamera检测3D相机的数目
      * @return 返回检测到的相机个数
    */
    int detectCamera();
    /**
      * @brief 函数snapShot3D是进行一次扫描
      * @return 返回是否成功拍摄的错误代码
    */
    uint32_t snapShot3D();
    /**
      * @brief 函数 getGC3DMetaData 获取重建数据
      * @param [inout] userMetaData重建后的数据
      * @return 是否获取数据成功的返回代码
      * @note 该函数一般是函数snapShot3D之后
    */
    uint32_t getGC3DMetaData(GC3DMetaData& userMetaData);
    /**
      * @brief 函数 snapShot2D 软触发获取一张2D图
      * @param [in] exposureTime 获取2D图像的曝光时间
      * @param [in] gain 获取2D图像的增益
      * @param [in] data 获取得到的2D图像数据
      * @return
      * @note snapShot3D之后利用getGC3DMetaData也可以获取数据中也存在纹理数据，该函数的区别是软触发，可以简单看成利用2D相机拍照
    */
    void snapShot2D(int exposureTime,double gain,unsigned char* data);//软触发获取一张2D图
    /**
      * @brief 函数registerEvent是注册回调函数用来处理相机拍摄得到的数据
      * @return
    */
    void registerEvent(callBack func);
    /**
      * @brief 函数unRegisterEvent取消回调事件
      * @return
    */
    void unRegisterEvent();
    /**
      * @brief 函数 setNeedGridData 设置是否需要规则化网格数据
      * @param [in] gridStatus 是否设置需要规则化网格数据
      * @return
    */
    void setNeedGridData(const bool gridStatus);
    /**
      * @brief 函数 getNeedGridData 获取当前生成网格化数据状态
      * @return 当前是否生成网格化数据状态
    */
    bool getNeedGridData();
    /**
      * @brief 函数 setGridSearchRange 设置是否需要规则化网格数据查找范围
      * @param [in] gridRange 查找范围
      * @return
      * @note 一般不需要独立设置，相机会默认设置
    */
    void setGridSearchRange(const int gridRange);
    /**
      * @brief 函数 getGC3DGridData 获取规整的重建数据
      * @param [inout] userGridData 规整的网格化数据
      * @return
    */
    uint32_t getGC3DGridData(GC3DGridData& userGridData);
    /**
      * @brief 函数setCameraParameters采用结构体 GC3DCameraParameters 一次性设置所有参数
      * @param [in] cameraParams是需要设置参数的结构体
      * @return 返回设置参数设置成功的错误代码
    */
    uint32_t setCameraParameters(GC3DCameraParameters cameraParams);
    /**
      * @brief 函数 getCameraParameters 获取3D相机的设置参数
      * @return 返回类型为GC3DCameraParameters的3D相机测试参数
    */
    GC3DCameraParameters getCameraParameters();
    /**
      * @brief 函数 setReconThreshold 设置重建阈值
      * @param [in] minThreshold 重建的最小亮度阈值
      * @param [in] maxThreshold 重建的最大亮度阈值
      * @return
    */
    void setReconThreshold(const int minThreshold,const int maxThreshold);
    /**
      * @brief 函数 getReconThreshold 获取重建阈值
      * @param [inout] minThreshold 重建的最小亮度阈值
      * @param [inout] maxThreshold 重建的最大亮度阈值
      * @return
    */
    void getReconThreshold(int& minThreshold,int& maxThreshold);
    /**
      * @brief 函数 setDenoiseParameters 设置降噪参数
      * @param [in] fmr 去噪半径，支持1-2-3，E.G. 3—7*7， 2—5*5， 1—3*3
      * @param [in] denoiseIndex1 全局降噪参数1：ft1，fmr半径内最少有效点数,根据FMR设置
      * @param [in] denoiseIndex2 全局降噪参数2：ft2, fmr半径内所有点的Z方差，用于快速去除雾状大面积噪音点，t2不易太小
      * @param [in] denoiseIndex3 局部降噪参数：fmr半径内，平局高度值相比，超过t3的点滤除
      * @return
      * @note 还要综合考虑，相机分辨率，相机视野大小物理分辨率，目标特征。
    */
    void setDenoiseParameters(const int fmr,const float denoiseIndex1,const float denoiseIndex2,const float denoiseIndex3);
    /**
      * @brief 函数 getDenoiseParameters 获取降噪参数
      * @param [inout] fmr 去噪半径，
      * @param [inout] denoiseIndex1 全局降噪参数1
      * @param [inout] denoiseIndex2 全局降噪参数2
      * @param [inout] denoiseIndex3 局部降噪参数
      * @return
    */
    void getDenoiseParameters(int& fmr,float& denoiseIndex1,float& denoiseIndex2,float& denoiseIndex3);
    /**
      * @brief 函数 setSmoothParam 设置平滑参数
      * @param [in] smoothParam 0是无平滑，大于0则是平滑
      * @return
    */
    void setSmoothParam(const int smoothParam);
    /**
      * @brief 函数 getSmoothParam 获取平滑参数
      * @param [inout] smoothParam平滑参数
      * @return
    */
    int getSmoothParam();
    /**
      * @brief 函数 setHeightRange 设置高度范围
      * @param [in] minHeight 最小高度阈值
      * @param [in] maxHeight 最大高度阈值
      * @return
    */
    void setHeightRange(const float minHeight,const float maxHeight);

    /**
      * @brief 函数 setErodeMetaDataSize 设置腐蚀范围
      * @param [in] eroSize 设置腐蚀大小
      * @return
    */
    void setErodeMetaDataSize(const int eroSize);

    /**
      * @brief 函数 getHeightRange 获取高度范围
      * @param [inout] minHeight 最小高度阈值
      * @param [inout] maxHeight 最大高度阈值
      * @return
    */
    void getHeightRange(float& minHeight,float& maxHeight);
    /**
      * @brief 函数 getMaxExposureTime 获取设备最大曝光时间
      * @return 设备的最大曝光时间
    */
    int getMaxExposureTime();
    /**
      * @brief 函数 getZeroPlaneHeight 获取零平面的高度
      * @return 零平面的高度
    */
    float getZeroPlaneHeight();
    /**
      * @brief 函数 getDeviceInfo 获得设备信息
      * @param [inout] devInfo 设备的信息数组
      * @return
    */
    void getDeviceInfo(DeviceInformation& devInfo);
    /**
      * @brief 函数 getScanTime当前扫描时间
      * @return 获取函数的扫描时间，单位毫秒
    */
    double getScanTime();
    /**
      * @brief 函数 getGCIVersion 返回3D相机的版本
      * @return 3D相机的版本
    */
    std::string getGCIVersion();
    /**
      * @brief 函数 isRGBDevice 是否为彩色相机
      * @return 是否为彩色相机
    */
    bool isRGBDevice();
    /**
      * @brief 函数 saveOptimizeData 保存优化数据
      * @return 是否成功获取
      * @note 内部开发函数，一般开发不需要该函数
    */
    bool saveOptimizeData();
    /**
      * @brief 函数 loadCameraConfig 加载相机配置文件
      * @param [in] fileFullPath 加载文件路径
      * @return
    */
    void loadCameraConfig(std::string fileFullPath);
    /**
      * @brief 函数 loadCameraConfig 保存相机配置文件
      * @param [in] fileFullPath 保存文件路径
      * @return
    */
    void saveCameraConfig(std::string fileFullPath);
    /**
      * @brief 函数 setProjectorIndex 函数主要是针对D系列以及Q系列的光机
      * @param [in] prjIdx 设置显示的光机一般情况是默认都亮的，可以根据情况设置相关的光机是否点亮，输入格式一般为0b0011，表示点亮两个光机
      * @return
    */
    void setProjectorIndex(uint32_t prjIdx);
    /**
      * @brief 函数 getErrMsg 根据错误码返回错误信息
      * @param [in] errCode 错误码
      * @param [out]  错误信息
      * @return
    */
    static std::string getErrMsg(uint32_t errCode);
    /**
      * @brief 函数 readGCIP 读取.gci格式文件
      * @param [inout] data 内容读取后存放到结构体GC3DMetaData
      * @param [in] filename .gci所在的文件名
      * @return
      * @note 此函数为静态函数与设备不相关
    */
    static void readGCIP(GC3DMetaData& data,std::string filename);
    /**
      * @brief 函数 saveGCIP 保存.gci格式文件
      * @param [inout] data 需要保存的结构体GC3DMetaData
      * @param [in] filename .gci需要保存的文件名
      * @return
      * @note 此函数为静态函数与设备不相关
    */
    static void saveGCIP(GC3DMetaData& data,std::string filename);
    /**
      * @brief 函数 savePoints 保存点云文件
      * @param [inout] data 需要保存的结构体GC3DMetaData
      * @param [in] filename 需要保存的文件名
      * @param [in] valid 其中valid = 0 保存数据包含无效点，valid = 1 仅有效点
      * @return
      * @note 此函数为静态函数与设备不相关
    */
    static void savePoints(GC3DMetaData& data,std::string filename,int valid =1);
    /**
      * @brief 函数 saveDepthData 保存网格化深度信息
      * @param [inout] data 需要保存的结构体GC3DGridData
      * @param [in] filename 需要保存的文件名
      * @return
      * @note 此函数为静态函数与设备不相关
    */
    static void saveDepthData(GC3DGridData& data,std::string filename);



};
};

#endif

