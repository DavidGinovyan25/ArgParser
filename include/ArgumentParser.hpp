#include <cstring>
#include <string>
#include <vector>

namespace ArgumentParser {
enum class ContainerType {
    INT_T,
    STRING_T,
    FLAG_T
};
struct Command {
    int min_args_count = 0;
    ContainerType type;
    char param1;
    std::string param2;
    std::string description;
    std::vector<std::string> args;
};

using CommandsContainer = std::vector<Command>;

class CommandHandler {
public:
    // CommandsContainer int_commands;      
    // CommandsContainer string_commands;
    // CommandsContainer flag_commands;
    // CommandsContainer help_commands;
    CommandsContainer commands;
    bool CheckCommand(const char param1);
    bool CheckCommand(const std::string& param1);
    bool IsCommand(const char* flag, const char* string);
};
}

namespace ArgumentParser {
inline bool CommandHandler::CheckCommand(const char param1) {
    for (int i = 0; i < commands.size(); ++i) {
        if (param1 == commands[i].param1)
            return true;
    }
    // for (int i = 0; i < int_commands.size(); ++i) {
    //     if (param1 == int_commands[i].param1)
    //         return true;
    // }
    // for (int i = 0; i < string_commands.size(); ++i) {
    //     if (param1 == string_commands[i].param1)
    //         return true;
    // }
    // for (int i = 0; i < flag_commands.size(); ++i) {
    //     if (param1 == flag_commands[i].param1)
    //         return true;
    // }
    return false;
}
inline bool CommandHandler::CheckCommand(const std::string& param2) {
    for (int i = 0; i < commands.size(); ++i) {
        if (param2 == commands[i].param2)
            return true;
    }
    // for (int i = 0; i < int_commands.size(); ++i) {
    //     if (param2 == int_commands[i].param2)
    //         return true;
    // }
    // for (int i = 0; i < string_commands.size(); ++i) {
    //     if (param2 == string_commands[i].param2)
    //         return true;
    // }
    // for (int i = 0; i < flag_commands.size(); ++i) {
    //     if (param2 == flag_commands[i].param2)
    //         return true;
    // }
    return false;
}
inline bool CommandHandler::IsCommand(const char* flag, const char* string) {
    return (std::strncmp(flag, string, strlen(string)) == 0) ? true : false;
}
}