#ifndef RGSDEF_H
#define RGSDEF_H
#include<vector>

#define RGS_SUCCESS                        0x00009003  //成功
#define RGS_CALIBRATE_NUM_FAIL             0x00009004  //标定数据不够
#define RGS_CALIBRATE_TYPE_FAIL            0x00009005  //机械手和标定类型设置不同
#define RGS_SOFTDOG_FAIL                   0x00009006  //加密狗错误
#define RGS_ICP_ERROR                      0x00009007  //成功
#define RGS_PI      3.141592653

#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif
#define RGS_STATUS uint32_t

namespace gc3d {


//-------机械手类型--------------//
/*brief 不同的机械手类型其坐标表示方法不一样，有的是XYZ位移平台
 * 有的是六轴机械手，六轴机械手不同厂家表示方法也不同，相应的其标
 * 定和定位的方法也不同，在执行本程序之前需要初始化其机械手类型，
 * 根据其坐标表示方法不同有以下几种类型，初始化的时候需设置
*/
enum GCIROBOTTYPE{
    GCI_XYZ_ROBOT=0,                //XYZ位移平台
    GCI_SIX_ROBOT,                  //六轴机械手(x,y,z,Rx,Ry,Rz)
    GCI_SIX_KUKA,                   //库卡六轴机械手(x,y,z,A,B,C)
    GCI_SIX_KAWASAKI                //川崎六轴机械手(x,y,z,O,A,T)
};

//-------场景类型--------------//
/*brief 相机安装的方式有两种，眼在手上和眼在手外，不同的方式选在不同
 * 类型
*/
enum GCICALTYPE{
    GCI_EYEINHAND=0,        //眼在手上
    GCI_EYETOHAND           //眼在手外
};



//-------定位类型--------------//
/*brief 相机安装的方式有两种，眼在手上和眼在手外，不同的方式选在不同
 * 类型
*/
enum GCILOCTYPE{
    GCI_XYZRXRYRZ=0,        //机械手每个值都需要定位
    GCI_XYZRZ,              //机械手只需要定位X,Y,Z,RZ,其中RX和RY不会变化
    GCI_XYZ                //机械手只需要定位X,Y,Z,不会有角度的变化
};


//-------标定点云类型--------------//
/*brief 标定时点云的类型可以支持两种，有序点云表示在标定时每次点云数量
 * 是一样的，并且是一一对应的；无序点云表示每次的点云数量都可能会不同，
 * 但都是一个模型；不用的点云类型在计算RT采用的算法不同
*/
enum GCIPOINTTYPE{
    GCI_POINT_ORDER=0,      //有序点云
    GCI_POINT_UNORDER       //无序点云
};
//六轴机械手参数结构体
struct GripperLocal{
    double x=0;
    double y=0;
    double z=0;
    double rx=0;
    double ry=0;
    double rz=0;
};
//旋转矩阵3X3
struct GRotation{
    double r11=0;
    double r12=0;
    double r13=0;
    double r21=0;
    double r22=0;
    double r23=0;
    double r31=0;
    double r32=0;
    double r33=0;
};
//平移向量
struct GTranslation{
    double tx=0;
    double ty=0;
    double tz=0;
};

//标定误差结构体
struct CalibrateError{
    std::vector<GripperLocal> gripError;
    GripperLocal errorMax,errorMin,errorSTD;
};
}


#endif // RGSDEF_H
