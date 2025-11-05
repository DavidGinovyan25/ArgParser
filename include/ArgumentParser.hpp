#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

namespace ArgumentParser {
enum class CommandType {
    INT_T,
    STRING_T,
    FLAG_T,
    HELP_T
};

struct Command {
    CommandType type;
    char param1;
    std::string param2;
    std::string description;
    std::vector<std::string> args;
    int min_args_count = 0;
    bool miltivalue = false;
    bool positional = false;
};

using CommandsContainer = std::unordered_map<std::string, Command>;

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
    for (const auto& [key, cmd] : commands) {
        if ((s.size() > 2 && s.substr(2) == cmd.param2) || (s.size() > 1 && s[1] == cmd.param1))                             
            return true;
    }
    return false;
}
}