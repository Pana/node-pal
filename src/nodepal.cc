#define HAVE_CONFIG_H
#include <node.h>
#include <v8.h>
#include "pal/pal.h"
#include "pal/layer.h"
#include "pal/label.h"

using namespace v8;

Handle<Value> Method(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New("world"));
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("labeller"), FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(labeller, init)





// 传递js对象到addon里边
// 传递js数组到addon中
// 从addon中传递js对象数组出来