import argparse

def readAllLines(filename):
    with open(filename, 'r') as inputFile:
        return inputFile.readlines()

def convertFileNameToVariable(fileName):
    variable = fileName.replace("/", "__")
    variable = variable.replace(".", "_")
    return variable

def prepareLine(line):
    return line.replace("\t", "    ").replace("\"", "\\\"").replace("\n", "\\n")

def main():
    parser = argparse.ArgumentParser(
        prog="convertToHpp",
        description="Convert file to hpp includeable file"
    )
    parser.add_argument('fileName')
    parser.add_argument('outputFile')
    args = parser.parse_args()

    lines = readAllLines(args.fileName)

    with open(args.outputFile, "w") as outpuFile:
        outpuFile.write("#pragma once\n")
        outpuFile.write(f"const char* {convertFileNameToVariable(args.fileName)} = {{\n")
        for line in lines:
            outpuFile.write(f"  \"{prepareLine(line)}\"\n")
        outpuFile.write(f"}};")


if __name__ == "__main__":
    main()