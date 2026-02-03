#include <algorithm>
#include <cstring>
#include <string>
#include <map>
#include <ArgParser.hpp>

namespace ArgumentParser {
std::optional<std::string> ArgParser::GetStringValue(const std::string& param, int index) { 
    return GetFlagValue(param, index);
}
std::optional<int> ArgParser::GetIntValue(const std::string& param, int index) {
    auto int_value = GetFlagValue(param, index);
    return int_value.has_value()
        ? std::optional<int>{std::strtol(int_value->c_str(), nullptr, 10)} 
        : std::nullopt;
}
bool ArgParser::GetFlag(const std::string& param) {
    return commands_dict.find(param) != commands_dict.end();
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

ArgParser& ArgParser::MultiValue(int min_args_count) {  
    current_cmd->min_args_count = min_args_count;
    current_cmd->multivalue = true;
    commands_list.back().min_args_count = min_args_count;
    commands_list.back().multivalue = true;
    return *this; 
}

ArgParser& ArgParser::Positional() {
    current_cmd->positional = true;
    commands_list.back().positional = true;
    return *this;
}  

bool ArgParser::Help() {
    return (!commands_dict.empty()) ? true : false;
}
std::string ArgParser::HelpDescription() {
    std::string description(3, '\n');
    std::string dashs(50, '=');
    description += (dashs + '\n');
    for (const auto& [key, cmd]: commands_dict) {
        if (cmd.type == CommandType::HELP_T)
            description += ("-" + std::string(1, cmd.param1) + " || --" + cmd.param2 + " || " + cmd.description + "\n");
    }
    description += (dashs + '\n');
    return description;
}   
}