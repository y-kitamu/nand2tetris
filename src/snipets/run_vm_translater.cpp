#include <iostream>

#include "vm_translater/code_writer.hpp"
#include "vm_translater/command_type.hpp"
#include "vm_translater/parser.hpp"


int main(int argc, char** argv) {
    std::cout << "Start translate vm language to Hack assembly." << std::endl;
    if (argc != 2) {
        std::cout << "Program need one argument (*.vm file to be translated)" << std::endl;
        std::cout << "Abort" << std::endl;
        return 0;
    }

    std::string filename(argv[1]);
    vm::Parser parser(filename);

    std::string output_filename = filename.substr(0, filename.size() - 3) + ".asm";
    vm::CodeWriter writer(output_filename);

    while (parser.hasMoreCommands()) {
        parser.advance();
        auto cmd_type = parser.commandType();
        if (cmd_type == vm::CommandType::C_ARITHMETIC) {
            writer.writeArithmetic(parser.arg1());
        } else if (cmd_type == vm::CommandType::C_POP || cmd_type == vm::CommandType::C_PUSH) {
            writer.writePushPop(cmd_type, parser.arg1(), parser.arg2());
        }
    }

    writer.close();
    std::cout << "Finish translation. Save to " << output_filename << std::endl;
}
