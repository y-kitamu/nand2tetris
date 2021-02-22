#ifndef COMMAND_TYPE_HPP
#define COMMAND_TYPE_HPP

namespace vm {

enum class CommandType {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL,
    C_NONE,
};

}  // namespace vm

#endif
