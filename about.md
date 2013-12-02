PAL需要开发成Node.js addon方便使用:

* 集成PAL的所有功能
* 灵活, 可配置
* 数据输入,输出方便, 支持多种格式


## 文件结构

    pal代码位于src pal中:
        外部头文件: label.h, layer.h, pal.h, palexception.h, palgeometry.h, plastat.h
        自定义Geom对象: Geom.h, Geom.cpp
        其他为pal内部代码: 
        config.h为pal在mac平台configure之后生成文件


## 问题
    node-gyp build会有warning
        Feat 重定义


## 说明

* 在c++模块内没有对参数进行检查, 检查工作由js代码完成
* 接口说明参看代码
* 添加配套使用方法




