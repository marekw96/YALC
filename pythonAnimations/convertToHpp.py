import argparse
import os

def readAllLines(filename):
    with open(filename, 'r') as inputFile:
        return inputFile.readlines()

def convertFileNameToVariable(fileName):
    base_name = os.path.basename(fileName)
    variable = base_name.replace("/", "__")
    variable = variable.replace(".", "_")
    return variable

def prepareLine(line):
    return line.replace("\\", "\\\\").replace("\t", "    ").replace("\"", "\\\"").replace("\n", "\\n")


def main():
    parser = argparse.ArgumentParser(
        prog="convertToHpp",
        description="Convert file to hpp includeable file"
    )
    parser.add_argument('fileName')
    parser.add_argument('outputFile')
    parser.add_argument('--var-name', dest='varName', default=None, help='Variable name override')
    args = parser.parse_args()

    lines = readAllLines(args.fileName)
    var_name = args.varName if args.varName else convertFileNameToVariable(args.fileName)

    with open(args.outputFile, "w") as outputFile:
        outputFile.write("#pragma once\n")
        outputFile.write(f"static const char* {var_name} = {{\n")
        for line in lines:
            outputFile.write(f"  \"{prepareLine(line)}\"\n")
        outputFile.write("};\n")


if __name__ == "__main__":
    main()