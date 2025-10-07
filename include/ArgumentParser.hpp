#pragma once

#include <string>
#include <vector>

struct Command {
    const char param1;
    const std::string& param2;
    const std::string& description;
};

namespace ArgumentParser {
    template <typename T>
    T value;

    template <typename T>
    std::vector<T> values;

    inline std::vector<Command> commands; 
    inline std::vector<std::string> split_string;

    inline bool CheckCommand(const char param1, const std::string& param2) {
        for (size_t i = 0; i < commands.size(); ++i) {
            if (param1 == commands[i].param1 && param2 == commands[i].param2)
                return true;
        }
        return false;
    }
    inline bool CheckCommand(const std::string& param2) {
        for (size_t i = 0; i < commands.size(); ++i) {
            if (param2 == commands[i].param2)
                return true;
        }
        return false;
    } 
}