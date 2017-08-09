{
  "targets": [
    {
      "target_name": "dwarfLoader",
      "cflags": ["-std=c++11" ,'-fexceptions' ,"-fPIC", "-fno-omit-frame-pointer" ],
      "sources": [
        "module.cpp",
        "DwarfWrapper.cpp",
        "../dwarf/src/DwarfReader.cpp",
        "../architecture/src/AndOperatorRule.cpp",
        "../architecture/src/ArchBuilder.cpp",
        "../architecture/src/ArchRule.cpp",
        "../architecture/src/ClassRule.cpp",
        "../architecture/src/FunctionRule.cpp",
        "../architecture/src/NamespaceRule.cpp",
        "../architecture/src/OrOperatorRule.cpp",
        "../architecture/src/RegexFileRule.cpp",
        "../architecture/src/RegexNameRule.cpp",
        "../architecture/src/RegexRule.cpp",
        "../architecture/src/SetOperatorRule.cpp",
        "../architecture/src/VariableRule.cpp",
        "../dwarf/src/Context.cpp",
        "../dwarf/src/DwarfHelper.cpp",
        "../dwarf/src/Duplicate.cpp",
        "../dwarf/src/Filter.cpp",
        "../dwarf/src/Type.cpp",
        "../dwarf/include/tag/TagMember.h",
        "../ast/ast-from-json/CreateAstFromJson.cpp",
        "../ast/nodes/AndExpression.cpp",
        "../ast/nodes/AtomExpression.cpp",
        "../ast/nodes/Component.cpp",
        "../ast/nodes/DefinitionExpression.cpp",
        "../ast/nodes/Expression.cpp",
        "../ast/nodes/NotExpression.cpp",
        "../ast/nodes/OrExpression.cpp",
        "../ast/nodes/Program.cpp",
        "../ast/nodes/SetExpression.cpp",
        "../ast/visitor/ASTVisitor.cpp",
        "../ast/visitor/VisitorContext.cpp"
      ],
      "conditions": [
        ["OS=='linux'", {
          "defines": [
            "_HAS_EXCEPTIONS=1"
          ]
        }]
      ],
      'cflags_cc': [ '-fexceptions', '-fpermissive', '-fPIC' ],
      'cflags_cc!': [ '-fno-rtti' ],
      'include_dirs': [
        '../3rdparty/json11',
        '../dwarf/include',
        '../architecture/include'
      ],
      'link_settings': {
        "libraries": [
          '-L/usr/local/lib',
	  '/home/wilhelma/workspace/parceive/dc_cpp/cmake-build-release/libdwarf/libdwarf/libdwarf.a',
	  '/home/wilhelma/workspace/parceive/dc_cpp/cmake-build-release/libelf/lib/libelf.a',
          '../../3rdparty/json11/libjson11.a'
        ]
      }
    }
  ]
}
