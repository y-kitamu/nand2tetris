#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iostream>

#include "vm_translater/code_writer.hpp"
#include "vm_translater/command_type.hpp"
#include "vm_translater/parser.hpp"


namespace fs = boost::filesystem;

void run_translate(std::string filename, vm::CodeWriter& writer) {
    std::cout << "Start process " << filename << std::endl;
    vm::Parser parser(filename);
    writer.setFileName(filename);

    while (parser.hasMoreCommands()) {
        parser.advance();
        auto cmd_type = parser.commandType();
        switch (cmd_type) {
            case vm::CommandType::C_ARITHMETIC:
                writer.writeArithmetic(parser.arg1());
                break;
            case vm::CommandType::C_POP:
            case vm::CommandType::C_PUSH:
                writer.writePushPop(cmd_type, parser.arg1(), parser.arg2());
                break;
            case vm::CommandType::C_GOTO:
                writer.writeGoto(parser.arg1());
                break;
            case vm::CommandType::C_IF:
                writer.writeIf(parser.arg1());
                break;
            case vm::CommandType::C_LABEL:
                writer.writeLabel(parser.arg1());
                break;
            case vm::CommandType::C_FUNCTION:
                writer.writeFunction(parser.arg1(), parser.arg2());
                break;
            case vm::CommandType::C_RETURN:
                writer.writeReturn();
                break;
            case vm::CommandType::C_CALL:
                writer.writeCall(parser.arg1(), parser.arg2());
                break;
            default:
                break;
        }
    }
    std::cout << "Finish process " << filename << std::endl;
}


int main(int argc, char** argv) {
    std::cout << "Start translate vm language to Hack assembly." << std::endl;
    if (argc != 2) {
        std::cout << "Program need one argument (*.vm file to be translated)" << std::endl;
        std::cout << "Abort" << std::endl;
        return 0;
    }

    std::string path(argv[1]);
    std::string output_filename = fs::path(path).stem().generic_string() + ".asm";
    std::string output_directory = path;
    if (!fs::is_directory(output_directory)) {
        output_directory = fs::path(path).parent_path().generic_string();
    }
    output_filename = (fs::path(output_directory) / output_filename).generic_string();
    vm::CodeWriter writer(output_filename);
    if (fs::is_directory(fs::path(path))) {
        writer.bootstrap();
        BOOST_FOREACH (const fs::path& p,
                       std::make_pair(fs::directory_iterator(path), fs::directory_iterator())) {
            if (!fs::is_directory(p) && p.extension() == ".vm") {
                run_translate(p.generic_string(), writer);
            }
        }
    } else {
        run_translate(path, writer);
    }
    writer.close();
    std::cout << "Finish translation. Save to " << output_filename << std::endl;
}
