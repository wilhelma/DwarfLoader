#ifndef DwarfWrapper_H
#define DwarfWrapper_H

#include <node.h>
#include <node_object_wrap.h>
#include <string>
#include "DwarfReader.h"
#include "ArchBuilder.h"
#include "NamespaceRule.h"
#include "FunctionRule.h"
#include "ClassRule.h"
#include "Filter.h"
#include "Duplicate.h"
#include <stdlib.h>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <v8.h>

using pcv::dwarf::DwarfReader;
using pcv::ArchBuilder;
using pcv::ArchRule;
using pcv::NamespaceRule;
using pcv::FunctionRule;
using pcv::ClassRule;
using pcv::dwarf::Filter;
using pcv::dwarf::DieDuplicate;


class DwarfWrapper : public node::ObjectWrap {
    private:
        DwarfReader* reader_;
        ArchBuilder* arch_;
        Filter* filter_;
        DieDuplicate duplicate_;
        const std::string ast_;

        explicit DwarfWrapper(const std::string &fileName,
                              const std::string& filterStr,
                              const std::string& ast);
        ~DwarfWrapper();

        static v8::Persistent<v8::Function> constructor;
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void start(const v8::FunctionCallbackInfo<v8::Value>& args);

    public:
        static void Init(v8::Local<v8::Object> exports);
};

#endif
