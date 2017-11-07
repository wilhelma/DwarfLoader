var dwarfLoader = require("./build/Release/dwarfLoader");

var filename = process.argv[2];
var filterStr = process.argv[3];

var ast = [
             {
                "type": "AssignmentExpression",
                "operator": ":=",
                "left": {
                   "type": "Artifact",
                   "name": "rules"
                },
                "right": {
                   "type": "AtomExpression",
                   "rule": "infile",
                   "regex": ".*"
                }
             }
          ];

var dwarfReader = new dwarfLoader.dwarf(filename, filterStr, "");

console.log(dwarfReader.start(JSON.stringify(ast)));
