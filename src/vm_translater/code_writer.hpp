#ifndef CODE_WRITER_HPP
#define CODE_WRITER_HPP

#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "command_type.hpp"

namespace fs = boost::filesystem;


namespace vm {

class CodeWriter {
  public:
    CodeWriter(std::string filename) : filename(filename) {
        ofs = std::ofstream(filename);
        if (!ofs.is_open()) {
            std::cout << "Failed to open file : " << filename << std::endl;
        }
    }

    void setFileName(std::string filename) { this->filename = filename; }

    void writeArithmetic(std::string command) {
        ofs << "@SP" << std::endl;
        ofs << "AM=M-1" << std::endl;
        if (command == "neg" || command == "not") {
            if (command == "neg") {
                ofs << "M=-M" << std::endl;
            } else if (command == "not") {
                ofs << "M=!M" << std::endl;
            }
            ofs << "@SP" << std::endl;
            ofs << "M=M+1" << std::endl;
        } else {
            ofs << "D=M" << std::endl;
            ofs << "A=A-1" << std::endl;
            if (command == "eq" || command == "gt" || command == "lt") {
                ofs << "D=M-D" << std::endl;
                ofs << "M=-1" << std::endl;  // if true
                ofs << "@JUMP" << std::to_string(counter) << std::endl;
                if (command == "eq") {
                    ofs << "D;JEQ" << std::endl;
                } else if (command == "gt") {
                    ofs << "D;JGT" << std::endl;
                } else if (command == "lt") {
                    ofs << "D;JLT" << std::endl;
                }
                ofs << "@SP" << std::endl;
                ofs << "A=M-1" << std::endl;
                ofs << "M=0" << std::endl;  // if false
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
    }

    std::string getBasename() {
        std::string basename = fs::basename(filename);
        basename = basename.substr(0, basename.length() - 3);
        return basename;
    }


    void writePushPop(CommandType command_type, std::string segment, int index) {
        auto push_template = [this](std::string symbol, int index) {
            ofs << "@" + symbol << std::endl;
            ofs << "D=M" << std::endl;
            ofs << "@" + std::to_string(index) << std::endl;
            ofs << "A=D+A" << std::endl;
            ofs << "D=M" << std::endl;
        };
        auto pop_template = [this](std::string symbol, int index) {
            ofs << "@" + symbol << std::endl;
            ofs << "D=M" << std::endl;
            ofs << "@" + std::to_string(index) << std::endl;
            ofs << "D=D+A" << std::endl;
            ofs << "@SP" << std::endl;
            ofs << "A=M" << std::endl;
            ofs << "M=D" << std::endl;
            ofs << "A=A-1" << std::endl;
            ofs << "D=M" << std::endl;
            ofs << "A=A+1" << std::endl;
            ofs << "A=M" << std::endl;
            ofs << "M=D" << std::endl;
        };
        switch (command_type) {
            case CommandType::C_PUSH:
                if (segment == "constant") {
                    ofs << "@" << std::to_string(index) << std::endl;
                    ofs << "D=A" << std::endl;
                } else if (segment == "local") {
                    push_template("LCL", index);
                } else if (segment == "argument") {
                    push_template("ARG", index);
                } else if (segment == "this") {
                    push_template("THIS", index);
                } else if (segment == "that") {
                    push_template("THAT", index);
                } else if (segment == "temp") {
                    ofs << "@R" << std::to_string(5 + index) << std::endl;
                    ofs << "D=M" << std::endl;
                } else if (segment == "pointer") {
                    ofs << "@R" << std::to_string(3 + index) << std::endl;
                    ofs << "D=M" << std::endl;
                } else if (segment == "static") {
                    ofs << "@" << getBasename() << "." << std::to_string(index) << std::endl;
                    ofs << "D=M" << std::endl;
                }
                ofs << "@SP" << std::endl;
                ofs << "A=M" << std::endl;
                ofs << "M=D" << std::endl;
                ofs << "@SP" << std::endl;
                ofs << "M=M+1" << std::endl;
                break;
            case CommandType::C_POP:
                if (segment == "local") {
                    pop_template("LCL", index);
                } else if (segment == "argument") {
                    pop_template("ARG", index);
                } else if (segment == "this") {
                    pop_template("THIS", index);
                } else if (segment == "that") {
                    pop_template("THAT", index);
                } else if (segment == "temp") {
                    ofs << "@SP" << std::endl;
                    ofs << "A=M-1" << std::endl;
                    ofs << "D=M" << std::endl;
                    ofs << "@R" << std::to_string(5 + index) << std::endl;
                    ofs << "M=D" << std::endl;
                } else if (segment == "pointer") {
                    ofs << "@SP" << std::endl;
                    ofs << "A=M-1" << std::endl;
                    ofs << "D=M" << std::endl;
                    ofs << "@R" << std::to_string(3 + index) << std::endl;
                    ofs << "M=D" << std::endl;
                } else if (segment == "static") {
                    ofs << "@SP" << std::endl;
                    ofs << "A=M-1" << std::endl;
                    ofs << "D=M" << std::endl;
                    ofs << "@" << getBasename() << "." << std::to_string(index) << std::endl;
                    ofs << "M=D" << std::endl;
                }
                ofs << "@SP" << std::endl;
                ofs << "M=M-1" << std::endl;
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
