#ifndef __ASSEMBLER_PARSER_HPP__
#define __ASSEMBLER_PARSER_HPP__

#include <string>
#include <fstream>

#include <boost/algorithm/string.hpp>

enum CommandType {
    A_COMMAND,
    C_COMMAND,
    L_COMMAND,
};

class Parser {
  public:
    Parser(std::string filename) {
        ifs = std::ifstream(filename);
    }

    bool hasMoreCommands() {
        while (!ifs.eof()) {
            std::getline(ifs, current_commands);
            auto pos = current_commands.find_first_of("//");
            current_commands = current_commands.substr(0, pos);
            boost::trim(current_commands);
            if (current_commands.length() > 0) {
                return true;
            }
        }
        return false;
    }

    void advance() {
    }

    CommandType commandType() {
        if (current_commands[0] == '@') {
            return CommandType::A_COMMAND;
        } else if (current_commands[0] == '(') {
            return CommandType::L_COMMAND;
        }
        return CommandType::C_COMMAND;
    }

    std::string symbol() {
        std::size_t found = current_commands.find_last_not_of(')');
        return current_commands.substr(1, found);
    }

    std::string dest() {
        auto pos = current_commands.find('=');
        if (pos == std::string::npos) {
            return "";
        }
        return current_commands.substr(0, pos);
    }

    std::string comp() {
        auto start_pos = current_commands.find('=');
        auto end_pos = current_commands.find(';');
        if (start_pos == std::string::npos) {
            start_pos = 0;
        } else {
            start_pos++;
        }
        return current_commands.substr(start_pos, end_pos);
    }

    std::string jump() {
        auto pos = current_commands.find(';');
        if (pos == std::string::npos) {
            return "";
        }
        return current_commands.substr(pos + 1);
    }

    void reset() {
        ifs.clear();
        ifs.seekg(0, std::ios_base::beg);
    }

  private:
    std::ifstream ifs;
    std::string current_commands;
};

#endif // __ASSEMBLER_PARSER_HPP__
