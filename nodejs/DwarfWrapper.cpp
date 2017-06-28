#include "DwarfWrapper.h"

v8::Persistent<v8::Function> DwarfWrapper::constructor;

DwarfWrapper::DwarfWrapper(const std::string &fileName, const std::string& filterStr){
   filter_ = new Filter("(.+)" + filterStr + "(.+)", "(.+)boost(.+)");
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
        v8::String::Utf8Value filterStr(args[1]);

        DwarfWrapper* obj = new DwarfWrapper(*str, *filterStr);
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
    DwarfWrapper* obj = ObjectWrap::Unwrap<DwarfWrapper>(args.Holder());

    try {
        obj->reader_->start();
    } catch (DwarfError& e) {
        std::cerr << e.what();
    }

    ArchBuilder builder(obj->reader_->getContext());

    ArchRule *sRule = new NamespaceRule("preprocessor", ".*simplecpp.*");
    ArchRule *xmlRule = new NamespaceRule("output", ".*tinyxml2.*");
    ArchRule *mRule = new FunctionRule("match", ".*::(M|m)atch.*");
    ArchRule *guiRule = new ClassRule("gui", "((?!ErrorLogger).)*", ".*gui.*");
    ArchRule *libRule = new ClassRule("library", ".*Library.*");
    ArchRule *valueRule = new ClassRule("ValueFlow", ".*Value.*");
    ArchRule *settingsRule = new ClassRule("settings", ".*Settings.*");
    ArchRule *cliRule = new ClassRule("cli", ".*", ".*cli.*");
    ArchRule *checkRule = new ClassRule("checks", "Check.*");

    builder.apply(sRule);
    builder.apply(xmlRule);
    builder.apply(mRule);
    builder.apply(guiRule);
    builder.apply(libRule);
    builder.apply(valueRule);
    builder.apply(settingsRule);
    builder.apply(cliRule);
    builder.apply(checkRule);

    builder.finish();
    std::stringstream buffer;
    buffer << builder;
    std::string output = buffer.str();
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate,output.c_str()));
}

