var pal = require('../index')

var toDealed = [
    {
        name: 'layer1', 
        features: [
            {wkt: 'POLYGON((1 1,50 1,50 50,1 50,1 1))', label: 'first label', width: 30, height: 10}
        ]
    }
], opts = {bbox: [0,0,100,100]}
var result = pal.labeller(toDealed, opts)


console.log(result)