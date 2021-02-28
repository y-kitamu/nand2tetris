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

    void bootstrap() {
        ofs << "@256" << std::endl;
        ofs << "D=A" << std::endl;
        ofs << "@SP" << std::endl;
        ofs << "M=D" << std::endl;
        writeCall("Sys.init", 0);
    }

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
        std::string basename = fs::path(filename).stem().generic_string();
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

    void writeLabel(std::string label) { ofs << "(" << label << ")" << std::endl; }

    void writeGoto(std::string label) {
        ofs << "@" << label << std::endl;
        ofs << "0;JMP" << std::endl;
    }

    void writeIf(std::string label) {
        ofs << "@SP" << std::endl;
        ofs << "M=M-1" << std::endl;
        ofs << "A=M" << std::endl;
        ofs << "D=M" << std::endl;
        ofs << "@" << label << std::endl;
        ofs << "D;JNE" << std::endl;
    }

    void writeCall(std::string functionName, int numArgs) {
        auto push_label = [this](std::string label, bool is_a = false) {
            this->ofs << "@" << label << std::endl;
            if (is_a) {
                this->ofs << "D=A" << std::endl;
            } else {
                this->ofs << "D=M" << std::endl;
            }
            this->ofs << "@SP" << std::endl;
            this->ofs << "A=M" << std::endl;
            this->ofs << "M=D" << std::endl;
            this->ofs << "@SP" << std::endl;
            this->ofs << "M=M+1" << std::endl;
        };

        auto set_pointer = [this](std::string label, int offset) {
            this->ofs << "@" << std::to_string(offset) << std::endl;
            this->ofs << "D=A" << std::endl;
            this->ofs << "@SP" << std::endl;
            this->ofs << "D=M-D" << std::endl;
            this->ofs << "@" << label << std::endl;
            this->ofs << "M=D" << std::endl;
        };

        std::string return_addr_label = functionName + "$return" + std::to_string(counter++);
        push_label(return_addr_label, true);
        push_label("LCL");
        push_label("ARG");
        push_label("THIS");
        push_label("THAT");

        set_pointer("ARG", numArgs + 5);
        set_pointer("LCL", 0);

        ofs << "@" << functionName << std::endl;
        ofs << "0;JMP" << std::endl;
        writeLabel(return_addr_label);
    }

    void writeReturn() {
        // get return value to R13
        ofs << "@SP" << std::endl;
        ofs << "A=M-1" << std::endl;
        ofs << "D=M" << std::endl;
        ofs << "@R13" << std::endl;
        ofs << "M=D" << std::endl;

        // get return address to R14
        ofs << "@LCL" << std::endl;
        ofs << "D=M" << std::endl;
        ofs << "@5" << std::endl;
        ofs << "A=D-A" << std::endl;
        ofs << "D=M" << std::endl;
        ofs << "@R14" << std::endl;
        ofs << "M=D" << std::endl;

        // set SP to correct position
        ofs << "@ARG" << std::endl;
        ofs << "D=M+1" << std::endl;
        ofs << "@SP" << std::endl;
        ofs << "M=D" << std::endl;

        auto set_pointer = [this](std::string label, std::string base_label, int offset) {
            this->ofs << "@" << base_label << std::endl;
            this->ofs << "D=M" << std::endl;
            this->ofs << "@" << std::to_string(std::abs(offset)) << std::endl;
            this->ofs << "A=D-A" << std::endl;
            this->ofs << "D=M" << std::endl;
            this->ofs << "@" << label << std::endl;
            this->ofs << "M=D" << std::endl;
        };

        set_pointer("THAT", "LCL", 1);
        set_pointer("THIS", "LCL", 2);
        set_pointer("ARG", "LCL", 3);
        set_pointer("LCL", "LCL", 4);

        // set return value to top of stack
        ofs << "@R13" << std::endl;
        ofs << "D=M" << std::endl;
        ofs << "@SP" << std::endl;
        ofs << "A=M-1" << std::endl;
        ofs << "M=D" << std::endl;

        // set return address to A
        ofs << "@R14" << std::endl;
        ofs << "A=M" << std::endl;

        // jump to return address
        ofs << "0;JMP" << std::endl;
    }

    void writeFunction(std::string functionName, int numLocals) {
        writeLabel(functionName);
        for (int i = 0; i < numLocals; i++) {
            writePushPop(CommandType::C_PUSH, "local", i);
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
