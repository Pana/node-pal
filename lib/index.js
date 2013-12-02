var pal = require('../build/Release/labeller')
    , extend = require('extend')

module.exports = {
    labeller: labeller,

    searchMethod: ['CHAIN', 'POPMUSIC_TABU_CHAIN', 'POPMUSIC_TABU', 'POPMUSIC_CHAIN'],

    mapUnit: ['PIXEL', 'METER', 'FOOT', 'DEGREE'],

    arrange: ['P_POINT', 'P_LINE', 'P_HORIZ', 'P_FREE', 'P_LINE_AROUND']
}


/*
    labeller -- label计算方法
        opts -- pal配置项
        {
            dpi: 72,
            pointP: 1,
            lineP: 1,
            polyP: 1,
            searchMethod: '',  // 'CHAIN', 'POPMUSIC_TABU_CHAIN', 'POPMUSIC_TABU', 'POPMUSIC_CHAIN'
            mapUnit: '',  // 'PIXEL', 'METER', 'FOOT', 'DEGREE'
            bbox: []
        }

        data -- label数据(一个元素代表layer, 必须包含name和features数组否则被过滤掉, feature属性为: wkt, width, height, name)
        [{
            name: '',
            obstacle: false,
            unit: '',
            priority: 0.5,
            arrange: '', // 'P_POINT', 'P_LINE', 'P_HORIZ', 'P_FREE', 'P_LINE_AROUND'
            features: [{wkt: '', width: '', height: '', name: ''}]
        },...
        ]
*/
function labeller(data, opts){
    opts = extend({
        dpi: 72,
        pointP: 1,
        lineP: 1,
        polyP: 1,
        searchMethod: 'POPMUSIC_TABU',
        mapUnit: 'PIXEL',
        bbox: [0, 0, 1000, 1000]
    }, opts)

    var dealedData = [], layerDefaultConfig = {obstacle: false, priority: 0.5, arrange: 'P_POINT', unit:'PIXEL'}

    data.forEach(function(item, i){
        if(item.name && item.features && Array.isArray(item.features)){
            dealedData.push(extend(layerDefaultConfig, item))
        }
        // 添加对每个feature的检查
        item.features.forEach(function(feature, i){
            feature.fid = "f" + i;
        });
    })

    return pal.labeller(opts, dealedData)
}


/*
* 将geojson数据转换为labeller可以处理的格式
*/
function prepareGeojson(){

}


/*
* textWidth
*/
function textWidth(){

}