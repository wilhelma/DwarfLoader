var dwarfLoader = require("./build/Release/dwarfLoader");

var filename = process.argv[2];
//console.log(filename);

var dwarfReader = new dwarfLoader.dwarf(filename);

console.log(dwarfReader.start());
