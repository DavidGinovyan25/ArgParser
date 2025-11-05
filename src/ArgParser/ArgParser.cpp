#include <algorithm>
#include <cstring>
#include <filesystem>
#include <string>

#include <ArgParser.hpp>

namespace ArgumentParser {

ArgParser& ArgParser::Positional() {
    current_cmd->positional = true;
    return *this;
}  
    // for (std::string s: split_string) {
    //     std::cout << s << " ";
    // }

    // for (const auto& [key, cmd]: commands) {
    //     std::cout << cmd.param2;
    //     for (const auto& arg: cmd.args) {
    //         std::cout << " " << arg;
    //     }
    //     std::cout << std::endl;
    // }


bool ArgParser::Parse(std::vector<std::string> v) {
    if (!split_string.empty())
        split_string = v;

    auto is_not_command = [&](int i){
        return !IsCommand(split_string[i].c_str(), "--") && !IsCommand(split_string[i].c_str(), "-");};

    auto tokenize = [&](int start, int finish, int i) {
        split_string.insert(split_string.begin() + i + 1, split_string[i].substr(start));
        split_string[i] = split_string[i].substr(0, finish);};

    for (int i = 0; i < split_string.size(); ++i) {
        if (is_not_command(i)) 
            continue;
        size_t index_equal_sign = split_string[i].find('=');
        if (index_equal_sign != std::string::npos) {
            tokenize(index_equal_sign + 1, index_equal_sign, i);
            continue;
        }
        if (IsCommand(split_string[i].c_str(), "--"))
            continue;
        while (split_string[i].size() > 2) {
            tokenize(split_string[i].size() - 1, split_string[i].size() - 1, i);   
        }
    }
    for (int i = 0;  i < split_string.size(); ++i) {//если некоманды будут идти первыми//если у команд нет аргументов
        if (is_not_command(i)) 
            continue;

        auto it_long = commands.find(split_string[i].substr(2));
        auto it_short = std::find_if(commands.begin(), commands.end(), 
            [&](const auto& pair) {return pair.second.param1 == split_string[i][1];});
        auto it = (it_short != commands.end()) ? it_short : (it_long != commands.end()) ? it_long : commands.end();
        if (it == commands.end()) {
            std::cout << "unreserved command " << split_string[i] << std::endl;
            return false;
        }
        if (it->second.min_args_count) {
            while (i < split_string.size() - 1 && is_not_command(i + 1)) {
                it->second.args.push_back(split_string[++i]);
            }
            continue;
        }
        if (i < split_string.size() - 1 && is_not_command(i + 1))
            it->second.args.push_back(split_string[++i]);

        auto it_p = std::find_if(commands.begin(), commands.end(), 
            [&](const auto& pair) {return pair.second.positional;});
        if (it_p == commands.end()) {
            std::cout << "extra arguments" << std::endl;
            return false;
        }
        while (i < split_string.size() - 1 && is_not_command(i + 1)) {
            it_p->second.args.push_back(split_string[++i]);
        }
        if (it->second.args.size() < it->second.min_args_count) {
            std::cout << "min args doeesnt have huy" << std::endl;
            return false;
        }
    }
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

ArgParser& ArgParser::MultiValue(int min_args_count) {  
    current_cmd->min_args_count = min_args_count;
    return *this; 
}

bool ArgParser::Help() {
    return (!commands.empty()) ? true : false;
}
std::string ArgParser::HelpDescription() {
    std::string description(3, '\n');
    std::string dashs(50, '=');
    description += (dashs + '\n');
    for (const auto& [key, cmd]: commands) {
        if (cmd.type == CommandType::HELP_T)
            description += ("-" + std::string(1, cmd.param1) + " || --" + cmd.param2 + " || " + cmd.description + "\n");
    }
    description += (dashs + '\n');
    return description;
}   
}