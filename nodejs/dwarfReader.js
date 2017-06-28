var dwarfLoader = require("./build/Release/dwarfLoader");

var filename = process.argv[2];
var filterStr = process.argv[3];

var ast = [
             {
                "type": "DefinitionExpression",
                "operator": ":=",
                "left": {
                   "type": "Component",
                   "name": "Checks"
                },
                "right": {
                   "type": "AtomExpression",
                   "rule": "class",
                   "regex": "^Check.*"
                }
             }
          ];

var dwarfReader = new dwarfLoader.dwarf(filename, filterStr, JSON.stringify(ast));

console.log(dwarfReader.start());
