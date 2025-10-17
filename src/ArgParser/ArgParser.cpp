#include <cstring>
#include <filesystem>

#include <ArgParser.hpp>

namespace ArgumentParser {
ArgParser& ArgParser::MultiValue(int min_args_count) {  //realize with Parse  
    return *this; 
}

ArgParser& ArgParser::Positional() {    //realize with Parse
    return *this;
}  
/*
- parse short and long flags with their values
- process flags with and without "=" 
- parse several flags with their velues
- add multivalue, positional, default processing 
- parse -ac flags
*/

bool ArgParser::Parse(std::vector<std::string> v) { //TAKE INTO ACCOUNT POSITIONAL, MULTIVALUE 
    if (!split_string.empty()) {
        for (int i = 0; i < v.size(); ++i) {
            split_string.push_back(v[i]);
        }            
    }
    // for (int i = 1; i < v.size(); ++i) {
    //     if (IsFlag(v[i].c_str(), "-")) {
    //         if (CheckCommand(v[i])) {
    //             //check arguments
    //             int x = 1;
    //             while (IsFlag(v[x++].c_str(), "-")) {
                    
    //             }
    //         }
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
    int value_index = GetValueIndex(param, index);
    if (command_handler.CheckCommand(param))
        return (value_index != kIntMissingValue) ? split_string[value_index] : kStringMissingValue; 
    return kStringMissingValue;
}
int ArgParser::GetIntValue(const std::string& param, int index) {
    int value_index = GetValueIndex(param, index);
    if (command_handler.CheckCommand(param))
        return (value_index != kIntMissingValue) 
            ? std::strtol(split_string[value_index].c_str(), nullptr, 10) 
            : kIntMissingValue; 
    return kIntMissingValue;
}
bool ArgParser::GetFlag(const std::string& param) {
    if (command_handler.CheckCommand(param))
        return (GetFlagIndex(param) != kIntMissingValue) ? true : false;
    return false;
}

ArgParser& ArgParser::AddIntArgument(const char param1, const std::string& param2, const std::string& description) {
    Add(command_handler.int_commands, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddIntArgument(const std::string& param2, const std::string& description) {
    Add(command_handler.int_commands, param2, description);
    return *this;
}

ArgParser& ArgParser::AddStringArgument(const char param1, const std::string& param2, const std::string& description) {
    Add(command_handler.string_commands, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddStringArgument(const std::string& param2, const std::string& description) {
    Add(command_handler.string_commands, param2, description);
    return *this;
}

ArgParser& ArgParser::AddFlag(const char param1, const std::string& param2, const std::string& description) {
    Add(command_handler.flag_commands, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddFlag(const std::string& param2, const std::string& description) {
    Add(command_handler.flag_commands, param2, description);
    return *this;
}

ArgParser& ArgParser::AddHelp(const char param1, const std::string& param2, const std::string& description) {
    Add(command_handler.help_commands, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddHelp(const std::string& param2, const std::string& description) {
    Add(command_handler.help_commands, param2, description);
    return *this;
}

ArgParser& ArgParser::Default(int value) {
    DefaultValue(command_handler.int_commands, std::to_string(value));
    return *this;
}
ArgParser& ArgParser::Default(std::string value) {
    DefaultValue(command_handler.string_commands, value);
    return *this;
}
ArgParser& ArgParser::Default(bool value) {
    DefaultValue(command_handler.flag_commands, std::to_string(value));
    return *this;
}

ArgParser& ArgParser::StoreValue(int& val) {    //CHECK
    store_handler.StoreValueMethod(store_handler.int_value_ref, val);
    return *this;
}
ArgParser& ArgParser::StoreValue(std::string& val) {    //CHECK
    store_handler.StoreValueMethod(store_handler.string_value_ref, val);
    return *this;
}
ArgParser& ArgParser::StoreValue(bool& val) {    //CHECK
    store_handler.StoreValueMethod(store_handler.bool_value_ref, val);
    return *this;
}

ArgParser& ArgParser::StoreValues(const std::vector<int64_t>& val) {    //CHECK
    store_handler.StoreValuesMethod(store_handler.int_values, val);
    return *this;
}  
ArgParser& ArgParser::StoreValues(const std::vector<std::string>& val) {    //CHECK
    store_handler.StoreValuesMethod(store_handler.string_values, val);
    return *this;
} 
ArgParser& ArgParser::StoreValues(const std::vector<bool>& val) {    //CHECK
    store_handler.StoreValuesMethod(store_handler.bool_values, val);
    return *this;
}  

bool ArgParser::Help() {
    return (!command_handler.help_commands.empty()) ? true : false;
}
void ArgParser::HelpDescription() {
    for (Command command : command_handler.help_commands) {
        std::cout << command.param1 << " " << command.param2 << " " << command.description;
    }
}   
}