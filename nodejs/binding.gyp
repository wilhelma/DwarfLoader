{
  "targets": [
    {
      "target_name": "dwarfLoader",
      "cflags": ["-std=c++11" ,'-fexceptions' ,"-fPIC", "-fno-omit-frame-pointer" ],
      "sources": [
        "module.cpp",
        "DwarfWrapper.cpp",
        "../dwarf/src/DwarfReader.cpp",
        "../architecture/src/ArchBuilder.cpp",
        "../architecture/src/NamespaceRule.cpp",
        "../architecture/src/ClassRule.cpp",
        "../dwarf/src/Context.cpp",
        "../dwarf/src/DwarfHelper.cpp",
        "../dwarf/src/Duplicate.cpp",
        "../dwarf/src/Filter.cpp",
        "../dwarf/src/Type.cpp",
        "../dwarf/include/tag/TagMember.h"
      ],
      "conditions": [
        ["OS=='linux'", {
          "defines": [
            "_HAS_EXCEPTIONS=1"
          ]
        }]
      ],
      'cflags_cc': [ '-fexceptions', '-fpermissive', '-fPIC' ],
      'include_dirs': [
        '../3rdparty/json11',
        '../3rdparty/libdwarf/libdwarf',
        '../3rdparty/libz',
        '../dwarf/include',
        '../architecture/include'
      ],
      'link_settings': {
        "libraries": [
          '-L/usr/local/lib',
          '-lelf',
          '../../3rdparty/libdwarf/libdwarf/libdwarf.a',
          '../../3rdparty/libz/libz.a',
          '../../3rdparty/json11/libjson11.a'
        ]
      }
    }
  ]
}
