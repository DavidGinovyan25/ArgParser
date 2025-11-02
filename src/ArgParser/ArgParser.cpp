#include <cstring>
#include <filesystem>
#include <string>

#include <ArgParser.hpp>

namespace ArgumentParser {
ArgParser& ArgParser::MultiValue(int min_args_count) {  
    commands.back().min_args_count = min_args_count;
    return *this; 
}

ArgParser& ArgParser::Positional() {
    return *this;
}  
/*
- parse short and long flags with their values
- process flags with and without "=" 
- parse several flags with their velues
- add multivalue, positional, default processing 
- parse -ac flags
*/

bool ArgParser::Parse(std::vector<std::string> v) {
    if (!split_string.empty())
        split_string = v;
    // for (int i = 0; i < split_string.size(); ++i) {
    //     if (command_handler.IsCommand(split_string[i].c_str(), "--") || command_handler.IsCommand(split_string[i].c_str(), "-")) {
    //         int index_sign_equal = split_string[i].find('=');
    //         std::string param = split_string[i].substr(2, index_sign_equal - 1);
    //         if (!command_handler.CheckCommand(param)) return false;
    //         if (index_sign_equal != std::string::npos) {
    //             if (command_handler.IsCommand(split_string[i].c_str(), "-")) {
    //                 for (int j = split_string[i].size() - 1; 1; --j) {
    //                     split_string.insert(split_string.begin() + i + 1, "-" + split_string[i].substr(split_string[i].size() - 1));
    //                     split_string[i] = split_string[i].substr(0, split_string[i].size() - 2);
    //                 }                    
    //             }
    //         }
    //         split_string.insert(split_string.begin() + i + 1, split_string[i].substr(index_sign_equal));
    //         split_string[i] = split_string[i].substr(0, index_sign_equal - 1);
    //     }
    // }

    
    return true;
}
bool ArgParser::Parse(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
        split_string.push_back(argv[i]);
    }
    return Parse(split_string) ? true : false;
}

std::string ArgParser::GetStringValue(const std::string& param, int index) { 
    int value_index = get_handler.GetValueIndex(split_string, param, index);
    if (CheckCommand(param))
        return (value_index != get_handler.kIntMissingValue) 
            ? split_string[value_index] 
            : get_handler.kStringMissingValue; 
    return get_handler.kStringMissingValue;
}
int ArgParser::GetIntValue(const std::string& param, int index) {
    int value_index = get_handler.GetValueIndex(split_string, param, index);
    if (CheckCommand(param))
        return (value_index != get_handler.kIntMissingValue) 
            ? std::strtol(split_string[value_index].c_str(), nullptr, 10) 
            : get_handler.kIntMissingValue; 
    return get_handler.kIntMissingValue;
}
bool ArgParser::GetFlag(const std::string& param) {
    if (CheckCommand(param))
        return (get_handler.GetFlagIndex(split_string, param) != get_handler.kIntMissingValue) ? true : false;
    return false;
}

// ArgParser& ArgParser::AddIntArgument(const char param1, const std::string& param2, const std::string& description) {
//     Add(command_handler.int_commands, param1, param2, description);
//     return *this;
// }
// ArgParser& ArgParser::AddIntArgument(const std::string& param2, const std::string& description) {
//     Add(command_handler.int_commands, param2, description);
//     return *this;
// }

// ArgParser& ArgParser::AddStringArgument(const char param1, const std::string& param2, const std::string& description) {
//     Add(command_handler.string_commands, param1, param2, description);
//     return *this;
// }
// ArgParser& ArgParser::AddStringArgument(const std::string& param2, const std::string& description) {
//     Add(command_handler.string_commands, param2, description);
//     return *this;
// }

// ArgParser& ArgParser::AddFlag(const char param1, const std::string& param2, const std::string& description) {
//     Add(command_handler.flag_commands, param1, param2, description);
//     return *this;
// }
// ArgParser& ArgParser::AddFlag(const std::string& param2, const std::string& description) {
//     Add(command_handler.flag_commands, param2, description);
//     return *this;
// }

// ArgParser& ArgParser::AddHelp(const char param1, const std::string& param2, const std::string& description) {
//     Add(command_handler.help_commands, param1, param2, description);
//     return *this;
// }
// ArgParser& ArgParser::AddHelp(const std::string& param2, const std::string& description) {
//     Add(command_handler.help_commands, param2, description);
//     return *this;
// }
ArgParser& ArgParser::AddIntArgument(const char param1, const std::string& param2, const std::string& description) {
    Add(ContainerType::INT_T, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddIntArgument(const std::string& param2, const std::string& description) {
    Add(ContainerType::INT_T, param2, description);
    return *this;
}

ArgParser& ArgParser::AddStringArgument(const char param1, const std::string& param2, const std::string& description) {
    Add(ContainerType::STRING_T, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddStringArgument(const std::string& param2, const std::string& description) {
    Add(ContainerType::STRING_T, param2, description);
    return *this;
}

ArgParser& ArgParser::AddFlag(const char param1, const std::string& param2, const std::string& description) {
    Add(ContainerType::FLAG_T, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddFlag(const std::string& param2, const std::string& description) {
    Add(ContainerType::FLAG_T, param2, description);
    return *this;
}

ArgParser& ArgParser::AddHelp(const char param1, const std::string& param2, const std::string& description) {
    //Add(command_handler.help_commands, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddHelp(const std::string& param2, const std::string& description) {
    //Add(command_handler.help_commands, param2, description);
    return *this;
}

// ArgParser& ArgParser::Default(int value) {
//     DefaultValue(command_handler.int_commands, std::to_string(value));
//     return *this;
// }
// ArgParser& ArgParser::Default(std::string value) {
//     DefaultValue(command_handler.string_commands, value);
//     return *this;
// }
// ArgParser& ArgParser::Default(bool value) {
//     DefaultValue(command_handler.flag_commands, std::to_string(value));
//     return *this;
// }
template <typename T>
ArgParser& ArgParser::Default(T value) {
    commands.back().param2 
        += ("=" + (std::is_same_v<T, std::string>) ? value : std::to_string(value));
    return *this;
}

ArgParser& ArgParser::StoreValue(int& val) {    
    store_handler.StoreValueMethod<>(store_handler.int_value_ref, val);
    return *this;
}
ArgParser& ArgParser::StoreValue(std::string& val) {    
    store_handler.StoreValueMethod(store_handler.string_value_ref, val);
    return *this;
}
ArgParser& ArgParser::StoreValue(bool& val) {    
    store_handler.StoreValueMethod(store_handler.bool_value_ref, val);
    return *this;
}

ArgParser& ArgParser::StoreValues(std::vector<int64_t>& val) {    
    store_handler.StoreValuesMethod(store_handler.int_values_ref, val);
    return *this;
}  
ArgParser& ArgParser::StoreValues(std::vector<std::string>& val) {    
    store_handler.StoreValuesMethod(store_handler.string_values_ref, val);
    return *this;
} 
ArgParser& ArgParser::StoreValues(std::vector<bool>& val) {    
    store_handler.StoreValuesMethod(store_handler.bool_values_ref, val);
    return *this;
}  



// bool ArgParser::Help() {
//     return (!command_handler.help_commands.empty()) ? true : false;
// }
// void ArgParser::HelpDescription() {
//     for (Command command : command_handler.help_commands) {
//         std::cout << command.param1 << " " << command.param2 << " " << command.description;
//     }
// }   
// }

bool ArgParser::Help() {
    return (!commands.empty()) ? true : false;
}
void ArgParser::HelpDescription() {
    for (Command command : commands) {
        std::cout << command.param1 << " " << command.param2 << " " << command.description;
    }
}   
}