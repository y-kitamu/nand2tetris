#ifndef CODE_WRITER_HPP
#define CODE_WRITER_HPP

#include <fstream>
#include <iostream>
#include <string>

#include "command_type.hpp"


namespace vm {

class CodeWriter {
  public:
    CodeWriter(std::string filename) {
        ofs = std::ofstream(filename);
        if (!ofs.is_open()) {
            std::cout << "Failed to open file : " << filename << std::endl;
        }
    }

    void setFileName(std::string filename) { this->filename = filename; }

    void writeArithmetic(std::string command) {
        ofs << "@SP" << std::endl;
        ofs << "M=M-1" << std::endl;
        if (command == "neg" || command == "not") {
            ofs << "A=M" << std::endl;
            if (command == "neg") {
                ofs << "M=-M" << std::endl;
            } else if (command == "not") {
                ofs << "M=!M" << std::endl;
            }
        } else {
            ofs << "D=M" << std::endl;
            ofs << "M=M-1" << std::endl;
            ofs << "A=M" << std::endl;
            if (command == "eq" || command == "gt" || command == "lt") {
                ofs << "D=D-M" << std::endl;
                ofs << "M=0" << std::endl;
                ofs << "@JUMP" << std::to_string(counter) << std::endl;
                if (command == "eq") {
                    ofs << "D;JEQ" << std::endl;
                } else if (command == "gt") {
                    ofs << "D;JGT" << std::endl;
                } else if (command == "lt") {
                    ofs << "D;JLT" << std::endl;
                }
                ofs << "@SP" << std::endl;
                ofs << "A=M" << std::endl;
                ofs << "M=-1" << std::endl;
                ofs << "(JUMP" << std::to_string(counter++) << ")" << std::endl;
            } else {
                if (command == "add") {
                    ofs << "M=D+M" << std::endl;
                } else if (command == "sub") {
                    ofs << "M=M-D" << std::endl;
                } else if (command == "and") {
                    ofs << "M=D&M" << std::endl;
                } else if (command == "or") {
                    ofs << "M=D|M" << std::endl;
                }
            }
        }
        ofs << "@SP" << std::endl;
        ofs << "M=M+1" << std::endl;
    }
    void writePushPop(CommandType command_type, std::string segment, int index) {
        switch (command_type) {
            case CommandType::C_PUSH:
                ofs << "@SP" << std::endl;
                ofs << "A=M" << std::endl;
                if (segment == "constant") {
                    ofs << "M=" << std::to_string(index) << std::endl;
                }
                ofs << "@SP" << std::endl;
                ofs << "M=M+1" << std::endl;
                break;
            case CommandType::C_POP:
                break;
            default:
                std::cout << "function `writePushPop` is called with invalid command." << std::endl;
        }
    }
    void close() { ofs.close(); }

  private:
    int counter = 0;
    std::ofstream ofs;
    std::string filename;
};

}  // namespace vm

#endif
