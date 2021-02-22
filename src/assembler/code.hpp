#ifndef __ASSEMBLER_CODE_HPP__
#define __ASSEMBLER_CODE_HPP__

#include <bitset>


class Code {
  public:
    std::bitset<3> dest(std::string mnemonic) {
        std::bitset<3> dest;
        if (mnemonic == "") {
            dest = std::bitset<3>("000");
        } else if (mnemonic == "M") {
            dest = std::bitset<3>("001");
        } else if (mnemonic == "D") {
            dest = std::bitset<3>("010");
        } else if (mnemonic == "MD") {
            dest = std::bitset<3>("011");
        } else if (mnemonic == "A") {
            dest = std::bitset<3>("100");
        } else if (mnemonic == "AM") {
            dest = std::bitset<3>("101");
        } else if (mnemonic == "AD") {
            dest = std::bitset<3>("110");
        } else {
            dest = std::bitset<3>("111");
        }
        return dest;
    }

    std::bitset<3> jump(std::string mnemonic) {
        std::bitset<3> jump;
        if (mnemonic == "") {
            jump = std::bitset<3>("000");
        } else if (mnemonic == "JGT") {
            jump = std::bitset<3>("001");
        } else if (mnemonic == "JEQ") {
            jump = std::bitset<3>("010");
        } else if (mnemonic == "JGE") {
            jump = std::bitset<3>("011");
        } else if (mnemonic == "JLT") {
            jump = std::bitset<3>("100");
        } else if (mnemonic == "JNE") {
            jump = std::bitset<3>("101");
        } else if (mnemonic == "JLE") {
            jump = std::bitset<3>("110");
        } else {
            jump = std::bitset<3>("111");
        }
        return jump;
    }

    std::bitset<7> comp(std::string mnemonic) {
        std::bitset<7> comp;
        if (mnemonic == "0") {
            comp = std::bitset<7>("0101010");
        } else if (mnemonic == "1") {
            comp = std::bitset<7>("0111111");
        } else if (mnemonic == "-1") {
            comp = std::bitset<7>("0111010");
        } else if (mnemonic == "D") {
            comp = std::bitset<7>("0001100");
        } else if (mnemonic == "A") {
            comp = std::bitset<7>("0110000");
        } else if (mnemonic == "!D") {
            comp = std::bitset<7>("0001101");
        } else if (mnemonic == "!A") {
            comp = std::bitset<7>("0110001");
        } else if (mnemonic == "-D") {
            comp = std::bitset<7>("0001111");
        } else if (mnemonic == "-A") {
            comp = std::bitset<7>("0110011");
        } else if (mnemonic == "D+1") {
            comp = std::bitset<7>("0011111");
        } else if (mnemonic == "A+1") {
            comp = std::bitset<7>("0110111");
        } else if (mnemonic == "D-1") {
            comp = std::bitset<7>("0001110");
        } else if (mnemonic == "A-1") {
            comp = std::bitset<7>("0110010");
        } else if (mnemonic == "D+A") {
            comp = std::bitset<7>("0000010");
        } else if (mnemonic == "D-A") {
            comp = std::bitset<7>("0010011");
        } else if (mnemonic == "A-D") {
            comp = std::bitset<7>("0000111");
        } else if (mnemonic == "D&A") {
            comp = std::bitset<7>("0000000");
        } else if (mnemonic == "D|A") {
            comp = std::bitset<7>("0010101");
        } else if (mnemonic == "M") {
            comp = std::bitset<7>("1110000");
        } else if (mnemonic == "!M") {
            comp = std::bitset<7>("1110001");
        } else if (mnemonic == "-M") {
            comp = std::bitset<7>("1110011");
        } else if (mnemonic == "M+1") {
            comp = std::bitset<7>("1110111");
        } else if (mnemonic == "M-1") {
            comp = std::bitset<7>("1110010");
        } else if (mnemonic == "D+M") {
            comp = std::bitset<7>("1000010");
        } else if (mnemonic == "D-M") {
            comp = std::bitset<7>("1010011");
        } else if (mnemonic == "M-D") {
            comp = std::bitset<7>("1000111");
        } else if (mnemonic == "D&M") {
            comp = std::bitset<7>("1000000");
        } else if (mnemonic == "D|M") {
            comp = std::bitset<7>("1010101");
        }
        return comp;
    }
};

#endif // __ASSEMBLER_PARSER_HPP__
