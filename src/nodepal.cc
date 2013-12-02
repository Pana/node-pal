#define HAVE_CONFIG_H
#include <node.h>
#include <v8.h>
#include "pal/pal.h"
#include "pal/layer.h"
#include "pal/label.h"
#include "pal/Geom.h"

using namespace v8;
using namespace std;

pal::Units getUnit(Local<v8::String> unit){
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

pal::SearchMethod getMethod(Local<v8::String> method){
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

pal::Arrangement getArrangement(Local<v8::String> arrangement){
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

char *v8string2charp(Local<v8::String> str){
    String::AsciiValue name(str);
    return *name;
}

Handle<Value> Method(const Arguments& args) {
    HandleScope scope;
    Local<Object> config = args[0]->ToObject();
    Local<Array> toDeal = Local<Array>::Cast(args[1]);

    pal::Pal *pal = new pal::Pal();
    pal::Units map_unit = getUnit(config->Get(String::New("mapUnit"))->ToString());
    pal::SearchMethod method = getMethod(config->Get(String::New("searchMethod"))->ToString());
    pal -> setSearch(method);
    pal -> setMapUnit(map_unit);
    pal -> setDpi(config->Get(String::New("dpi"))->NumberValue());
    pal -> setPointP(config->Get(String::New("pointP"))->NumberValue());
    pal -> setLineP(config->Get(String::New("pointP"))->NumberValue());
    pal -> setPolyP(config->Get(String::New("pointP"))->NumberValue());
    

    pal::Arrangement arrange;
    arrange = pal::P_FREE;

    // 添加layer
    for(int i = 0; i < toDeal -> Length(); i++){
        Local<Object> layerData = toDeal->Get(i)->ToObject();
        pal::Units layer_unit = getUnit(layerData->Get(String::New("unit"))->ToString());
        pal::Arrangement layer_arrange = getArrangement(layerData->Get(String::New("arrange"))->ToString());
        double priority = layerData->Get(String::New("priority"))->NumberValue();
        bool isObstacle = layerData->Get(String::New("obstacle"))->ToBoolean()->Value();
        char *name = v8string2charp(layerData->Get(String::New("name"))->ToString());
        pal::Layer * layer = pal -> addLayer(name, -1, -1, layer_arrange, layer_unit, priority, isObstacle, true, true);

        Local<Array> features = Local<Array>::Cast(layerData->Get(String::New("features")));
        for(int j = 0; j < features -> Length(); j++){
            Local<Object> feature = features->Get(j)->ToObject();
            Geom *geom = new Geom(v8string2charp(feature->Get(String::New("wkt"))->ToString()));
            double width = feature->Get(String::New("width"))->NumberValue();
            double height = feature->Get(String::New("height"))->NumberValue();
            char *fid = v8string2charp(feature->Get(String::New("fid"))->ToString());
            layer -> registerFeature(fid, geom, width, height);
        }
    }

    Local<Array> v8bbox = Local<Array>::Cast(config->Get(String::New("bbox")));
    double xmin = v8bbox->Get(0)->NumberValue();
    double ymin = v8bbox->Get(1)->NumberValue();
    double xmax = v8bbox->Get(2)->NumberValue();
    double ymax = v8bbox->Get(3)->NumberValue();
    double bbox[4] = {xmin, ymin, xmax, ymax};
    pal::PalStat * stats;
    std::list<pal::Label*> *labels = pal -> labeller(1, bbox, &stats, false);
    
    // 传回数据
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
    
    return scope.Close(label_arra);
}

void init(Handle<Object> exports) {
    exports->Set(String::NewSymbol("labeller"), FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(labeller, init)

