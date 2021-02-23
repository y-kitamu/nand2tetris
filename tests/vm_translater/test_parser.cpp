#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "vm_translater/parser.hpp"
#include "vm_translater/command_type.hpp"


namespace fs = boost::filesystem;

TEST(VmParser, split) {
    std::vector<std::string> res = split("test0 test1 test2");
    EXPECT_EQ(res.size(), 3);
    EXPECT_EQ(res[0], "test0");
    EXPECT_EQ(res[1], "test1");
    EXPECT_EQ(res[2], "test2");
}


TEST(VmParser, Parser) {
    fs::path cur_dir = fs::path(__FILE__).parent_path();
    fs::path file_path = cur_dir / "../../projects/07/StackArithmetic/SimpleAdd/SimpleAdd.vm";
    if (!fs::exists(file_path)) {
        std::cout << "File does not exists : " << file_path.generic_string() << std::endl;
        return;
    }

    auto parser = vm::Parser(file_path.generic_string());

    for (int i = 0; i < 6; i++) {
        parser.advance();
        EXPECT_EQ(parser.commandType(), vm::CommandType::C_NONE);
    }
    parser.advance();
    EXPECT_EQ(parser.current_commands[0], "push");
    EXPECT_EQ(parser.commandType(), vm::CommandType::C_PUSH);
    EXPECT_EQ(parser.arg1(), "constant");
    EXPECT_EQ(parser.arg2(), 7);

    parser.advance();
    EXPECT_EQ(parser.current_commands[0], "push");
    EXPECT_EQ(parser.commandType(), vm::CommandType::C_PUSH);
    EXPECT_EQ(parser.arg1(), "constant");
    EXPECT_EQ(parser.arg2(), 8);

    parser.advance();
    EXPECT_EQ(parser.current_commands[0], "add");
    EXPECT_EQ(parser.commandType(), vm::CommandType::C_ARITHMETIC);
    EXPECT_EQ(parser.arg1(), "add");

    EXPECT_FALSE(parser.hasMoreCommands());
}
