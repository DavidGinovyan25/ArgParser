#include <cstring>
#include <string>
#include <vector>

namespace ArgumentParser {
struct Command {
    char param1;
    std::string param2;
    std::string description;
};

using CommandsContainer = std::vector<Command>;

class CommandHandler {
public:
    CommandsContainer int_commands;      
    CommandsContainer string_commands;
    CommandsContainer flag_commands;
    CommandsContainer help_commands;

    bool CheckCommand(const char param1, const std::string& param2);
    bool CheckCommand(const std::string& param2);
    bool IsCommand(const char* flag, const char* string);

};
}
namespace ArgumentParser {
inline bool CommandHandler::CheckCommand(const char param1, const std::string& param2) {
    for (int i = 0; i < int_commands.size(); ++i) {
        if (param1 == int_commands[i].param1 || param2 == int_commands[i].param2)
            return true;
    }
    for (int i = 0; i < string_commands.size(); ++i) {
        if (param1 == int_commands[i].param1 || param2 == string_commands[i].param2)
            return true;
    }
    for (int i = 0; i < flag_commands.size(); ++i) {
        if (param1 == int_commands[i].param1 || param2 == flag_commands[i].param2)
            return true;
    }
    return false;
}
inline bool CommandHandler::CheckCommand(const std::string& param2) {
    for (int i = 0; i < int_commands.size(); ++i) {
        if (param2 == int_commands[i].param2)
            return true;
    }
    for (int i = 0; i < string_commands.size(); ++i) {
        if (param2 == string_commands[i].param2)
            return true;
    }
    for (int i = 0; i < flag_commands.size(); ++i) {
        if (param2 == flag_commands[i].param2)
            return true;
    }        
    return false;
}
inline bool CommandHandler::IsCommand(const char* flag, const char* string) {
    return (std::strncmp(flag, string, sizeof(string)) == 0) ? true : false;
}         
}