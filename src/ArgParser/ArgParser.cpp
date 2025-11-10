#include <algorithm>
#include <cstring>
#include <filesystem>
#include <string>
#include <map>
#include <ArgParser.hpp>

namespace ArgumentParser {
auto print = [](const std::string& text) {std::cout << text << std::endl;};

bool ArgParser::Parse(std::vector<std::string> v) {
    auto error_handling = [&](bool error_condition, const char* error_type) {
        if (!error_condition) return false; std::cout << error_type << std::endl; return true;};

    auto is_not_command = [&](int i) {
        return !IsCommand(split_string[i].c_str(), "--") && !IsCommand(split_string[i].c_str(), "-");};

    auto tokenize = [&](int start, int finish, int i) {
        split_string.insert(split_string.begin() + i + 1, split_string[i].substr(start));
        split_string[i] = split_string[i].substr(0, finish);};

    if (commands_list.empty())
        return error_handling(true, "Empty commands_list");    

    if (!split_string.empty())
        split_string = v;
    
    if (split_string.empty())
        return error_handling(true, "Empty split_string");
        
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

    auto is_arg = [&](int& i){return i < split_string.size() - 1 && is_not_command(i + 1);};
    auto add_arg = [&](int& i, auto& it){if(is_arg(i))it->second.args.push_back(split_string[++i]);};
    auto add_args = [&](int& i, auto& it){while(is_arg(i)) {add_arg(i, it);}};
    auto vec_pos_com = commands_list.begin();

    for (int i = 0;  i < split_string.size(); ++i) {//если некоманды будут идти первыми//если у команд нет аргументов
        if (is_not_command(i)) {
            vec_pos_com = std::find_if(vec_pos_com, commands_list.end(), 
                [&](const Command& cmd){return cmd.positional;});

            if (vec_pos_com == commands_list.end())
                return error_handling(true, "extra arguments");

            auto map_pos_com = commands_dict.find(vec_pos_com->param2);
            ++vec_pos_com;
            add_args(i, map_pos_com);
            map_pos_com->second.args.insert(map_pos_com->second.args.begin(), split_string[0]);
            continue;
        }
        auto it_long_command = commands_dict.find(split_string[i].substr(2));
        auto it_short_command = std::find_if(commands_dict.begin(), commands_dict.end(), 
            [&](const auto& pair){return pair.second.param1 == split_string[i][1];});

        auto it_map = (it_short_command != commands_dict.end())
            ? it_short_command 
            : (it_long_command != commands_dict.end()) ? it_long_command : commands_dict.end();

        if (it_map == commands_dict.end()) 
            return error_handling(true, "unreserved command");
                
        if (it_map->second.multivalue) {
            add_args(i, it_map);
            if (it_map->second.args.size() < it_map->second.min_args_count)
                return error_handling(true, "min args doesnt have");
            continue;
        }
        add_arg(i, it_map);
        if (is_arg(i)) {
            vec_pos_com = std::find_if(vec_pos_com, commands_list.end(), 
                [&](const Command& cmd){return cmd.positional;});

            if (vec_pos_com == commands_list.end())
                return error_handling(true, "extra arguments");

            auto map_pos_com = commands_dict.find(vec_pos_com->param2);
            ++vec_pos_com;
            add_args(i, map_pos_com);
            if (map_pos_com->second.args.size() < map_pos_com->second.min_args_count)
                return error_handling(true, "min args doesnt have");
        }
    }

    for (const auto& [key, cmd]: commands_dict) {
        std::cout << cmd.param2;
        for (const auto& arg: cmd.args) {
            std::cout << " " << arg;
        }
        std::cout << std::endl;
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
    return GetFlagValue(param, index).has_value()
        ? std::optional<int>{std::strtol(kStringMissingValue, nullptr, 10)} : std::nullopt;
}
bool ArgParser::GetFlag(const std::string& param) {
    return GetFlagValue(param, 0).has_value();
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