#include <cstring>
#include <string>
#include <vector>

struct Commands {
    char param1;
    std::string param2;
    std::string description;
};

using CommandsContainer = std::vector<Commands>;

namespace ArgumentParser {
    /*IT`S GLOBAL VARIABLE*/
    inline CommandsContainer int_commands;      
    inline CommandsContainer string_commands;
    inline CommandsContainer flag_commands;
    inline CommandsContainer help_commands;
    inline std::vector<std::string> split_string;

    bool CheckCommand(const char param1, const std::string& param2);
    bool CheckCommand(const std::string& param2);
    bool IsFlag(const char* flag, const char* string);

}

namespace ArgumentParser {
    inline bool CheckCommand(const char param1, const std::string& param2) {
        for (int i = 0; i < int_commands.size(); ++i) {
            if (param1 == int_commands[i].param1 && param2 == int_commands[i].param2)
                return true;
        }
        for (int i = 0; i < string_commands.size(); ++i) {
            if (param1 == int_commands[i].param1 && param2 == string_commands[i].param2)
                return true;
        }
        for (int i = 0; i < flag_commands.size(); ++i) {
            if (param1 == int_commands[i].param1 && param2 == flag_commands[i].param2)
                return true;
        }
        return false;
    }
    inline bool CheckCommand(const std::string& param2) {
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
    inline bool IsFlag(const char* flag, const char* string) {
        return (std::strncmp(flag, string, sizeof(string)) == 0) ? true : false;
    }         
}