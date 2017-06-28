var dwarfLoader = require("./build/Release/dwarfLoader");

var filename = process.argv[2];
var filterStr = process.argv[3];

console.log(filename);
console.log(filterStr);

var dwarfReader = new dwarfLoader.dwarf(filename, filterStr);

console.log(dwarfReader.start());
