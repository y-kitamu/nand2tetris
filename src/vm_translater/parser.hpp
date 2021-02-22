#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "command_type.hpp"


std::vector<std::string> split(std::string str, std::string separator = " ") {
    std::vector<std::string> list{};
    auto separator_length = separator.length();
    auto offset = std::string::size_type(0);
    while (true) {
        auto pos = str.find(separator, offset);
        if (pos == std::string::npos) {
            list.emplace_back(str.substr(offset));
            break;
        }
        list.emplace_back(str.substr(offset, pos));
        offset = pos + separator_length;
    }
    return list;
}


namespace vm {

class Parser {
  public:
    Parser(std::string filename) {
        ifs = std::ifstream(filename);
        if (!ifs.is_open()) {
            std::cout << "Failed to open file : " << filename << std::endl;
        }
    }
    Parser(std::ifstream input_stream) { ifs = std::move(input_stream); }

    bool hasMoreCommands() { return ifs.peek() != EOF; }
    void advance() {
        if (hasMoreCommands()) {
            std::string line;
            std::getline(ifs, line);
            current_commands = split(line);
        }
    }

    CommandType commandType() {
        std::string cmd = current_commands[0];
        if (cmd == "push") {
            return CommandType::C_PUSH;
        }
        if (cmd == "pop") {
            return CommandType::C_POP;
        }
        if (cmd == "label") {
            return CommandType::C_LABEL;
        }
        if (cmd == "goto") {
            return CommandType::C_GOTO;
        }
        if (cmd == "if-goto") {
            return CommandType::C_IF;
        }
        if (cmd == "function") {
            return CommandType::C_FUNCTION;
        }
        if (cmd == "call") {
            return CommandType::C_CALL;
        }
        if (cmd == "return") {
            return CommandType::C_RETURN;
        }
        if (cmd == "add" || cmd == "sub" || cmd == "neg" || cmd == "eq" || cmd == "gt" || cmd == "lt" ||
            cmd == "and" || cmd == "or" || cmd == "not") {
            return CommandType::C_ARITHMETIC;
        }
        return CommandType::C_NONE;
    }

    std::string arg1() {
        if (commandType() == CommandType::C_RETURN) {
            return "";
        }
        if (commandType() == CommandType::C_ARITHMETIC) {
            return current_commands[0];
        }
        return current_commands[1];
    }

    int arg2() {
        auto ctype = commandType();
        if (ctype == CommandType::C_PUSH || ctype == CommandType::C_POP ||
            ctype == CommandType::C_FUNCTION || ctype == CommandType::C_CALL) {
            return std::stoi(current_commands[2]);
        }
        return -1;
    }

  public:
    std::vector<std::string> current_commands{};

  private:
    std::ifstream ifs;
};

}  // namespace vm


#endif
