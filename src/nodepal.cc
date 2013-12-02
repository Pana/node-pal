#define HAVE_CONFIG_H
#include <node.h>
#include <v8.h>
#include "pal/pal.h"
#include "pal/layer.h"
#include "pal/label.h"
#include "pal/Geom.h"

using namespace v8;
using namespace std;

/*
* 根据传入v8 string返回unit单位
*/
pal::Units getUnit(Local<String> unit){
    String::AsciiValue str(unit);
    if(strcmp(*str, "PIXEL") == 0){
        return pal::PIXEL;
    }else if(strcmp(*str, "METER") == 0){
        return pal::METER;
    }else if(strcmp(*str, "FOOT") == 0){
        return pal::FOOT;
    }else if(strcmp(*str, "DEGREE") == 0){
        return pal::DEGREE;
    }else{
        return pal::PIXEL;
    }
}

/*
* 根据传入v8 string返回search 方法
*/
pal::SearchMethod getMethod(Local<String> method){
    String::AsciiValue str(method);
    if(strcmp(*str, "CHAIN") == 0){
        return pal::CHAIN;
    }else if(strcmp(*str, "POPMUSIC_TABU_CHAIN") == 0){
        return pal::POPMUSIC_TABU_CHAIN;
    }else if(strcmp(*str, "POPMUSIC_TABU") == 0){
        return pal::POPMUSIC_TABU;
    }else if(strcmp(*str, "POPMUSIC_CHAIN") == 0){
        return pal::POPMUSIC_CHAIN;
    }else{
        return pal::POPMUSIC_TABU;    
    }
}

/*
* 根据传入v8 string返回arrangement模式
*/
pal::Arrangement getArrangement(Local<String> arrangement){
    String::AsciiValue str(arrangement);
    if(strcmp(*str, "P_POINT") == 0){
        return pal::P_POINT;
    }else if(strcmp(*str, "P_LINE") == 0){
        return pal::P_LINE;
    }else if(strcmp(*str, "P_HORIZ") == 0){
        return pal::P_HORIZ;
    }else if(strcmp(*str, "P_FREE") == 0){
        return pal::P_FREE;
    }else if(strcmp(*str, "P_LINE_AROUND") == 0){
        return pal::P_LINE_AROUND;
    }else{
        return pal::P_FREE;
    }
}

/*
* 从对象中获取属性
*/
Handle<Value> getter(Local<Object> obj, char* key){
    return obj->Get(String::New(key));
}

/*
* v8 string转换成char *
*/
char *v8string2charp(Local<v8::String> str){
    String::AsciiValue name(str);
    return *name;
}

/*
* labeller方法
*/
Handle<Value> Method(const Arguments& args) {
    HandleScope scope;
    Local<Object> config = args[0]->ToObject();
    Local<Array> toDeal = Local<Array>::Cast(args[1]);

    // 初始化pal对象
    pal::Pal *pal = new pal::Pal();
    pal::Units map_unit = getUnit(getter(config, "mapUnit")->ToString());
    pal::SearchMethod method = getMethod(getter(config, "searchMethod")->ToString());
    pal -> setSearch(method);
    pal -> setMapUnit(map_unit);
    pal -> setDpi(getter(config, "dpi")->NumberValue());
    pal -> setPointP(getter(config, "pointP")->NumberValue());
    pal -> setLineP(getter(config, "lineP")->NumberValue());
    pal -> setPolyP(getter(config, "polyP")->NumberValue());
    

    // 添加layer
    for(int i = 0; i < toDeal -> Length(); i++){
        Local<Object> layerData = toDeal->Get(i)->ToObject();
        pal::Units layer_unit = getUnit(getter(layerData, "unit")->ToString());
        pal::Arrangement layer_arrange = getArrangement(getter(layerData, "arrange")->ToString());
        double priority = getter(layerData, "priority")->NumberValue();
        bool isObstacle = getter(layerData, "obstacle")->ToBoolean()->Value();
        char *name = v8string2charp(getter(layerData, "name")->ToString());
        pal::Layer * layer = pal -> addLayer(name, -1, -1, layer_arrange, layer_unit, priority, isObstacle, true, true);

        // 添加feature
        Local<Array> features = Local<Array>::Cast(layerData->Get(String::New("features")));
        for(int j = 0; j < features -> Length(); j++){
            Local<Object> feature = features->Get(j)->ToObject();
            Geom *geom = new Geom(v8string2charp(getter(feature, "wkt")->ToString()));
            double width = getter(feature, "width")->NumberValue();
            double height = getter(feature, "height")->NumberValue();
            String::AsciiValue fid(getter(feature, "fid")->ToString());
            layer -> registerFeature(*fid, geom, width, height);
        }
    }

    // 设定bbox, 计算label
    Local<Array> v8bbox = Local<Array>::Cast(config->Get(String::New("bbox")));
    double xmin = v8bbox->Get(0)->NumberValue();
    double ymin = v8bbox->Get(1)->NumberValue();
    double xmax = v8bbox->Get(2)->NumberValue();
    double ymax = v8bbox->Get(3)->NumberValue();
    double bbox[4] = {xmin, ymin, xmax, ymax};
    pal::PalStat * stats;
    std::list<pal::Label*> *labels = pal -> labeller(1, bbox, &stats, false);
    
    // 组装返回数据数组
    Handle<Array> label_arra = Array::New(labels->size());
    int index = 0;
    while(labels->size()>0){
        Handle<v8::Object> label = v8::Object::New();
        pal::Label *l = labels -> front();
        label->Set(String::New("origX"), Number::New(l->getOrigX()));
        label->Set(String::New("origY"), Number::New(l->getOrigY()));
        label->Set(String::New("x1"), Number::New(l->getX(0)));
        label->Set(String::New("y1"), Number::New(l->getY(0)));
        label->Set(String::New("x2"), Number::New(l->getX(1)));
        label->Set(String::New("y2"), Number::New(l->getY(1)));
        label->Set(String::New("x3"), Number::New(l->getX(2)));
        label->Set(String::New("y3"), Number::New(l->getY(2)));
        label->Set(String::New("x4"), Number::New(l->getX(3)));
        label->Set(String::New("y4"), Number::New(l->getY(3)));
        label->Set(String::New("rotation"), Number::New(l->getRotation()));
        label->Set(String::New("featureId"), String::New(l->getFeatureId()));

        label_arra->Set(Number::New(index), label);
        index++;
        delete labels->front();
        labels->pop_front();
    }
    
    // 返回数据
    return scope.Close(label_arra);
}

void init(Handle<Object> exports) {
    exports->Set(String::NewSymbol("labeller"), FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(labeller, init)

