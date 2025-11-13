#ifndef FILESTORAGE_H
#define FILESTORAGE_H
#include <iostream>
#include <vector>
#include <string>
#include "../core/gc3dTypes.h"
#include "../rgs/gc3drgs.h"
namespace gc3d {
/**
 *  @defgroup Filestorage 目前仅支持特定数据格式的读取和写入
 */
class DLLEXPORT Filestorage
{
public:
    /**
    * @brief 默认构造函数
    * @return
   */
    Filestorage(std::string strXmlFilePath);
    /**
    * @brief setXmlFilePath  设置完整文件路径名
    * @param [in] strXmlFilePath 文件名，后缀必须为.xml
    * @return
   */
    void setXmlFilePath(std::string strXmlFilePath);
    /**
    * @brief readGPointSet  读取一维的GPoint数组,数据名称关键字错误会导致读取失败
    * @param [inout] points 读取的数组
    * @param [in] keyValue 读取的数组名关键字
    * @return
   */
    void readGPointSet(std::vector<gc3d::GPoint>& points,std::string keyValue);
    /**
    * @brief readGPointSets  读取二维的GPoint数组,数据名称关键字错误会导致读取失败
    * @param [inout] points 读取的数组
    * @param [in] keyValue 读取的数组名关键字
    * @return
   */
    void readGPointSets(std::vector<std::vector<gc3d::GPoint>>& points,std::string keyValue);
    /**
    * @brief writeGPointSet  写入一维的GPoint数组,数据名称关键字错误会导致写入失败
    * @param [inout] points 写入的数组
    * @param [in] keyValue 写入的数组名关键字
    * @return
   */
    void writeGPointSet(std::vector<gc3d::GPoint>& points,std::string keyValue);
    /**
    * @brief writeGPointSet  写入二维的GPoint数组,数据名称关键字错误会导致写入失败
    * @param [inout] points 写入的数组
    * @param [in] keyValue 写入的数组名关键字
    * @return
   */
    void writeGPointSets(std::vector<std::vector<gc3d::GPoint>>& points,std::string keyValue);
    /**
    * @brief readGPoint3fSet  读取一维的GPoint3f数组,数据名称关键字错误会导致读取失败
    * @param [inout] points 读取的数组
    * @param [in] keyValue 读取的数组名关键字
    * @return
   */
    void readGPoint3fSet(std::vector<gc3d::GPoint3f>& points,std::string keyValue);
    /**
    * @brief readGPoint3fSets  读取二维的GPoint3f数组,数据名称关键字错误会导致读取失败
    * @param [inout] points 读取的数组
    * @param [in] keyValue 读取的数组名关键字
    * @return
   */
    void readGPoint3fSets(std::vector<std::vector<gc3d::GPoint3f>>& points,std::string keyValue);

    /**
    * @brief writeGPoint3fSet  写入一维的GPoint3f数组,数据名称关键字错误会导致写入失败
    * @param [inout] points 写入的数组
    * @param [in] keyValue 写入的数组名关键字
    * @return
   */
    void writeGPoint3fSet(std::vector<gc3d::GPoint3f>& points,std::string keyValue);
    /**
    * @brief writeGPoint3fSets  写入一维的GPoint3f数组,数据名称关键字错误会导致写入失败
    * @param [inout] points 写入的数组
    * @param [in] keyValue 写入的数组名关键字
    * @return
   */
    void writeGPoint3fSets(std::vector<std::vector<gc3d::GPoint3f>>& points,std::string keyValue);
    /**
    * @brief readDoubleSet  读取一维的double数组,数据名称关键字错误会导致读取失败
    * @param [inout] data 读取的double数组
    * @param [in] keyValue 读取的数组名关键字
    * @return
   */
    void readDoubleSet(std::vector<double>& data,std::string keyValue);
    /**
    * @brief readDoubleSets  读取二维的double数组,数据名称关键字错误会导致读取失败
    * @param [inout] data 读取的double数组
    * @param [in] keyValue 读取的数组名关键字
    * @return
   */
    void readDoubleSets(std::vector<std::vector<double>>& data,std::string keyValue);
    /**
    * @brief writeDoubleSet  写入一维的double数组,数据名称关键字错误会导致写入失败
    * @param [inout] data 写入的数组
    * @param [in] keyValue 写入的数组名关键字
    * @return
   */
    void writeDoubleSet(std::vector<double>& data,std::string keyValue);
    /**
    * @brief writeDoubleSets  写入二维的double数组,数据名称关键字错误会导致写入失败
    * @param [inout] data 写入的数组
    * @param [in] keyValue 写入的数组名关键字
    * @return
   */
    void writeDoubleSets(std::vector<std::vector<double>>& data,std::string keyValue);
    /**
    * @brief readGripperSets  读取机械手坐标数组,数据名称关键字错误会导致读取失败
    * @param [inout] data 读取的机械手坐标数组
    * @param [in] keyValue 读取的数组名关键字
    * @return
   */
    void readGripperSets(std::vector<gc3d::GripperLocal>& data,std::string keyValue);
    /**
    * @brief writeGripperSets  写入机械手坐标数组,数据名称关键字错误会导致写入失败
    * @param [inout] data 写入的数组
    * @param [in] keyValue 写入的数组名关键字
    * @return
   */
    void writeGripperSets(std::vector<gc3d::GripperLocal>& data,std::string keyValue);


};
}
#endif // FILESTORAGE_H
