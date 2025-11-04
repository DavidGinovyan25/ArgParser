#include <cstring>
#include <string>
#include <vector>

namespace ArgumentParser {
enum class ContainerType {
    INT_T,
    STRING_T,
    FLAG_T,
    HELP_T
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
    CommandsContainer commands;
    bool IsCommand(const char* flag, const char* string);
    bool IsReservedCommand(const std::string& s);
};
}

namespace ArgumentParser {
inline bool CommandHandler::IsCommand(const char* flag, const char* string) {
    return (std::strncmp(flag, string, strlen(string)) == 0) ? true : false;
}
inline bool CommandHandler::IsReservedCommand(const std::string& s) {
    for (Command cmd : commands) {
        if ((s.size() > 2 && s.substr(2) == cmd.param2) || (s.size() > 1 && s[1] == cmd.param1))                             
            return true;
    }
    return false;
}
}