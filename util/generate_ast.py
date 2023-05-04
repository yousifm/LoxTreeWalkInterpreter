#!/bin/python3

import sys
from collections import OrderedDict

def defineAst(outputDir, basename, types):
    for type in types:
        classname = type.split(':')[0].strip()
        fields = type.split(":")[1].strip().split(',');
        fields = list(map(str.strip, fields))
        path = outputDir + '/' + classname.lower() + '.h'
        defineType(path, basename, classname, fields)

def defineType(path, basename, classname, fields):
    field_map = OrderedDict(map(swapTuple, map(str.split, fields)))
    private_field_map = OrderedDict({'_' + name: type for name, type in field_map.items()})
    initialization_list = [private_name + '(' + argument_name + ')'
                           for private_name, argument_name in zip(private_field_map.keys(), field_map.keys())]

    with open(path, 'w') as f:
        lines = [
            '#include \"' + basename.lower() + '.h\"\n\n',
            'namespace ' + basename + '{\n\n',
            'class ' + classname + ' : public ' + basename + ' {\n',
            'public:\n',
            '  ' + classname + ' (' + ', '.join(fields) + ')\n',
            '  : ' + ", ".join(initialization_list) + ' {}\n\n'
            'private:\n  ',
            '\n  '.join(['const ' + type + ' ' + name + ';' for name, type in private_field_map.items()]),
            '\n};\n\n',
            '}'
        ]
        f.writelines(lines)

def swapTuple(t):
    return (t[1], t[0])


if __name__ == '__main__':
    args = sys.argv

    if len(args) != 2:
        print("Usage: generate_ast.py <output dir>")
        exit(64);

    outputDir = args[1]
    
    defineAst(outputDir, "Expr", [
        "Binary   : Expr& left, Token op, Expr& right",
        "Grouping : Expr& expression",
        "Literal  : Token::literal_variant value",
        "Unary    : Token op, Expr& right"
    ]);
