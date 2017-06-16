#include "DwarfWrapper.h"

v8::Persistent<v8::Function> DwarfWrapper::constructor;

DwarfWrapper::DwarfWrapper(const std::string &fileName){
   filter_ = new Filter("(.+)main(.+)", "(.+)boost(.+)");

   DwarfWrapper::reader_ = new DwarfReader(fileName, duplicate_, *filter_);
   arch_ = new ArchBuilder(reader_->getContext());
}

DwarfWrapper::~DwarfWrapper(){
    delete reader_;
    delete arch_;
}

void DwarfWrapper::Init(v8::Local<v8::Object> exports){
    v8::Isolate* isolate = exports->GetIsolate();
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, "dwarf"));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);

    NODE_SET_PROTOTYPE_METHOD(tpl, "start", start);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(v8::String::NewFromUtf8(isolate, "dwarf"), tpl->GetFunction());
}

void DwarfWrapper::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if (args.IsConstructCall()) {
        v8::String::Utf8Value str(args[0]);
        //std::string s(*str);
        DwarfWrapper* obj = new DwarfWrapper(*str);
       // printf("from new the filname is : %s", *str);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
        v8::Local<v8::Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
        args.GetReturnValue().Set(result);
    }
}

void DwarfWrapper::start(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    //v8::EscapableHandleScope scope(isolate);
    DwarfWrapper* obj = ObjectWrap::Unwrap<DwarfWrapper>(args.Holder());
    try{
        obj->reader_->start();
    } catch (DwarfError& e) {
        std::cerr << e.what();
        //return scope.Escape(Undefined(isolate));
    }
    std::unique_ptr<ArchRule> nRule{ new NamespaceRule() };
    std::unique_ptr<ArchRule> cRule{ new ClassRule() };
    obj->arch_->apply(nRule.get());
    obj->arch_->apply(cRule.get());
    std::stringstream buffer;
   // std::cout << *(obj->arch_) << '\n';
   buffer << *(obj->arch_);
   std::string output = buffer.str();
    //obj->Wrap(args.This());
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate,output.c_str()));
}

