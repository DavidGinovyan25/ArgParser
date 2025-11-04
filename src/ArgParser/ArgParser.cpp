#include <algorithm>
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

bool ArgParser::Parse(std::vector<std::string> v) {
    if (!split_string.empty())
        split_string = v;
    for (int i = 0; i < commands.size(); ++i) {
        if (commands[i].type == CommandType::INT_T || commands[i].type == CommandType::STRING_T)
            commands[i].min_args_count = 1;
    }
    for (int i = 0; i < split_string.size(); ++i) {
        if (!IsCommand(split_string[i].c_str(), "--") && !IsCommand(split_string[i].c_str(), "-")) 
            continue; 

        size_t index_equal_sign = split_string[i].find('=');
        if (index_equal_sign == std::string::npos)
            continue;
        split_string.insert(split_string.begin() + i + 1, split_string[i].substr(index_equal_sign + 1));
        split_string[i] = split_string[i].substr(0, index_equal_sign);

        if (IsCommand(split_string[i].c_str(), "--"))
            continue;
        while (split_string[i].size() > 2) {
            int index_last_command = split_string[i].size() - 1;
            split_string.insert(split_string.begin() + i + 1, split_string[i].substr(index_last_command));
            split_string[i] = split_string[i].substr(0, index_last_command);    
        }
    }
    for (int i = 0;  i < split_string.size(); ++i) {
        if (!IsCommand(split_string[i].c_str(), "--") && !IsCommand(split_string[i].c_str(), "-")) 
            continue;

        auto it = std::find_if(commands.begin(), commands.end(), [&](const Command& cmd) {
            return (split_string[i].size() > 2 && split_string[i].substr(2) == cmd.param2) || 
                (split_string[i].size() > 1 && split_string[i][1] == cmd.param1);});

        if (it == commands.end()) {
            std::cout << "unreserved command" << std::endl;
            return false;
        }
        while (i < split_string.size() - 1 && 
            !IsCommand(split_string[i + 1].c_str(), "--") && !IsCommand(split_string[i + 1].c_str(), "-")) {
            it->args.push_back(split_string[++i]);
        }
        if (it->args.size() < it->min_args_count) {
            std::cout << "min args doeesnt have huy" << std::endl;
            return false;
        }
    }
    // for (int i = 0; i < commands.size(); ++i) {
    //     std::cout << commands[i].param2 << " ";
    //     for (int j = 0; j < commands[i].args.size(); ++j) {
    //         std::cout << commands[i].args[j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    return true;
}
bool ArgParser::Parse(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        split_string.push_back(argv[i]);
    }
    return Parse(split_string) ? true : false;
}

std::optional<std::string> ArgParser::GetStringValue(const std::string& param, int index) { 
    return GetFlagValue(param, index);
}
std::optional<int> ArgParser::GetIntValue(const std::string& param, int index) {
    return (GetFlagValue(param, index) != std::nullopt) 
        ? std::optional<int>{std::strtol(kStringMissingValue, nullptr, 10)} : std::nullopt;
}
bool ArgParser::GetFlag(const std::string& param) {
    return (GetFlagValue(param, 0) != std::nullopt) ? true : false;
}

ArgParser& ArgParser::AddIntArgument(const char param1, const std::string& param2, const std::string& description) {
    Add(CommandType::INT_T, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddIntArgument(const std::string& param2, const std::string& description) {
    Add(CommandType::INT_T, param2, description);
    return *this;
}

ArgParser& ArgParser::AddStringArgument(const char param1, const std::string& param2, const std::string& description) {
    Add(CommandType::STRING_T, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddStringArgument(const std::string& param2, const std::string& description) {
    Add(CommandType::STRING_T, param2, description);
    return *this;
}

ArgParser& ArgParser::AddFlag(const char param1, const std::string& param2, const std::string& description) {
    std::string x = std::to_string(param1);
    Add(CommandType::FLAG_T, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddFlag(const std::string& param2, const std::string& description) {
    Add(CommandType::FLAG_T, param2, description);
    return *this;
}

ArgParser& ArgParser::AddHelp(const char param1, const std::string& param2, const std::string& description) {
    Add(CommandType::HELP_T, param1, param2, description);
    return *this;
}
ArgParser& ArgParser::AddHelp(const std::string& param2, const std::string& description) {
    Add(CommandType::HELP_T, param2, description);
    return *this;
}

ArgParser& ArgParser::StoreValue(int& val) {    
    store_handler.StoreValueMethod(store_handler.int_value_ref, val);
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

bool ArgParser::Help() {
    return (!commands.empty()) ? true : false;
}
void ArgParser::HelpDescription() {
    for (Command command : commands) {
        std::cout << command.param1 << " " << command.param2 << " " << command.description;
    }
}   
}