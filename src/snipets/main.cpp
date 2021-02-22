#include <iostream>
#include <algorithm>

#include "assembler/code.hpp"
#include "assembler/parser.hpp"
#include "assembler/symboltable.hpp"


int main(int argc, char ** argv) {
    if (argc != 2) {
        return 0;
    }

    std::string input_filename = std::string(argv[1]);
    Code code = Code();
    Parser parser(input_filename);
    SymbolTable table = SymbolTable();

    auto dot_pos = input_filename.find_last_of('.');
    std::string output_filename = input_filename.substr(0, dot_pos) +  ".hack";
    std::cout << "input filename  : "  << input_filename << std::endl;
    std::cout << "output filename : "  << output_filename << std::endl;

    std::ofstream ofs(output_filename);
    if (!ofs.is_open()) {
        std::cout << "Failed to open output file" << std::endl;
    }

    int address = 0;
    while (parser.hasMoreCommands()) {
        parser.advance();
        CommandType command_type = parser.commandType();
        if (command_type == CommandType::L_COMMAND) {
            table.addEntry(parser.symbol(), address);
        } else {
            address++;
        }
    }

    parser.reset();

    int ram_address = 16;
    while (parser.hasMoreCommands()) {
        parser.advance();
        CommandType command_type = parser.commandType();
        if (command_type == CommandType::C_COMMAND) {
            std::string dest = code.dest(parser.dest()).to_string();
            std::string comp = code.comp(parser.comp()).to_string();
            std::string jump = code.jump(parser.jump()).to_string();
            ofs << "111" << comp << dest << jump << std::endl;
        } else if (command_type == CommandType::A_COMMAND) {
            std::string symbol = parser.symbol();
            int address;
            if (std::all_of(symbol.begin(), symbol.end(), isdigit)) {
                address = std::stoi(parser.symbol());
            } else {
                if (!table.contains(symbol)) {
                    table.addEntry(symbol, ram_address);
                    ram_address++;
                }
                address = table.getAddress(symbol);
            }
            ofs << '0' << std::bitset<15>(address) << std::endl;
        }
    }
    std::cout << "Finish Parse input file" << std::endl;
}
