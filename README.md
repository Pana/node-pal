node-pal
========
[PAL](http://pal.heig-vd.ch/) Node.js binding


## PAL -- Automated Label Placement
PAL是一个成熟C++的标注库. 提供高效的meta-heuristic算法用于寻找点,线,多边形的标注摆放位置.
通过JPAL JNI wrapper同样可以在Java平台使用, 基于LGPL, GNU许可证协议

<center>
    ![](http://pal.heig-vd.ch/uploads/images/unpaled.png)   ![](http://pal.heig-vd.ch/uploads/images/paled.png)
</center>

### PAL特点

* 点, 线, 多边形标注计算
* 高效, 避免重叠
* 支持多种地图单位
* 支持多种摆放规则

### Polygon
<tr style="text-align: center;">
<td class="scenario-title" style="text-align: center;"><img title="Centroid" src="http://thepana.com/pal/uploads/images/poly-ppt.png" alt=""></td>
<td class="scenario-title" style="text-align: center;"><img title="On perimeter" src="http://thepana.com/pal/uploads/images/poly-pl.png" alt=""></td>
<td class="scenario-title" style="text-align: center;"><img title="Around perimeter" src="http://thepana.com/pal/uploads/images/poly-pla.png" alt=""></td>
<td class="scenario-title" style="text-align: center;"><img title="Horizontal" src="http://thepana.com/pal/uploads/images/poly-ph.png" alt=""></td>
<td class="scenario-title" style="text-align: center;"><img title="Free" src="http://thepana.com/pal/uploads/images/poly-pf.png" alt=""></td>
</tr>

### Line
<tr style="text-align: left;">
<td class="scenario-title" style="text-align: left;"><img title="On the line" src="http://thepana.com/pal/uploads/images/line_pla.png" alt=""></td>
<td class="scenario-title" style="text-align: center;"><img title="Around the line" src="http://thepana.com/pal/uploads/images/line_pl.png" alt=""></td>
</tr>

### Point
<tr style="text-align: center;">
<td class="scenario-title" style="text-align: center;"><img title="Around the point" src="http://thepana.com/pal/uploads/images/pt_ppt.png" alt=""></td>
</tr>

关于PAL的更多内容(下载, 文档, 效果, 使用实例)参看官方网站

## Example
```
var opts = {
    dpi: 72,   
    pointP: 1,  // 点候选位置数量
    lineP: 1,   // 线候选位置数量
    polyP: 1,   // 多边形候选位置数量
    bbox: [0, 0, 100, 100]  // label范围
}
var data = [
    {
        name: 'layer1', 
        features: [
            {
                wkt: 'POLYGON((1 1,50 1,50 50,1 50,1 1))',  // feature wkt表示
                label: 'first label',     // label内容
                width: 30,      // 宽度
                height: 10      // 高度
            }
        ]
    }
]

var pal = require('node-pal')
var labels = pal.labeller(data, opts)   // 进行label计算

// 结果
[ 
    { 
        origX: 25.5,  //原始点x坐标
        origY: 25.5,  //原始点y坐标
        x1: 25.5,     //左上角x
        y1: 25.5,     //左上角y
        x2: 55.5,     //右上角x
        y2: 25.5,     //右上角y
        x3: 55.5,     //右下角x
        y3: 35.5,     //右下角y
        x4: 25.5,     //左下角x
        y4: 35.5,     //左下角y
        rotation: 0,  //旋转角度
        featureId: 'f0' //
    } 
]
```

## Document
label position 计算接口

    pa.labeller(data, opt)


data为layer(name, features数组必选)数组, layer地理数据为features数组(feature包含: wkt, width, height, name) :

```
[
    {
        name: 'layer_name',
        obstacle: false,  // 是否为障碍物
        unit: 'PIXEL',  // 计算单位
        priority: 0.5,  // 优先级
        arrange: 'P_LINE',   // 摆放模式
        features: [{wkt: "LINESTRING(x1 y1, x2 y2, x3 y3)", width: 40, height: 10, name: '东直门内大街 '}]       
    }
]
```


配置选项opt(可选):

* `DPI` 默认72
* `pointP` 点label候选位置数量, 默认为1
* `lineP`  线候选位置数量, 默认为1
* `polyP`  多边形候选位置数量, 默认为1
* `mapUnit` 地图单位, 默认为`PIXEL`, 可选值: `'PIXEL', 'METER', 'FOOT', 'DEGREE'`
* `searchMethod` 搜索方法, 默认`POPMUSIC_TABU`, 可选值: `'CHAIN', 'POPMUSIC_TABU_CHAIN', 'POPMUSIC_TABU', 'POPMUSIC_CHAIN'`
* `bbox`  计算区域, 格式为[minx, miny, maxx, maxy], 默认[0,0,1000,1000]

## PAL缺点 

* label宽高需要实现提供
* 计算区域为矩形,不能随线段曲折, 无法计算单文字角度



# TODO

* src代码文件组织结构优化, 将pal外部头文件和源代码分开
* 代码编译warning: Feat 重定义
* 支持更多pal配置项: add_layer, labeller方法
* addon 代码写的不够优雅, 数据类型转换方法太原始
* getUnit等方法采用更好方式实现: switch, 数组索引等等
* 开发js辅助代码, 简化使用步骤, 支持geojson, wkt等多种格式数据
* 计算结果关联label



