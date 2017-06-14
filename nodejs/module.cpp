#include "DwarfWrapper.h"

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
  DwarfWrapper::Init(exports);
}

NODE_MODULE(dwarfLoader, InitAll)