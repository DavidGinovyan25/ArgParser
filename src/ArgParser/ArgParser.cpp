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
        for (int i = 1; i < v.size(); ++i) {
            if (IsFlag(v[i].c_str(), "-")) {
                if (CheckCommand(v[i])) {
                    //check arguments
                    int x = 1;
                    while (IsFlag(v[x++].c_str(), "-")) {
                        
                    }
                }
            }

        }
        return true;
    }
    bool ArgParser::Parse(int argc, char *argv[]) {
        for (int i = 0; i < argc; ++i) {
            split_string.push_back(argv[i]);
        }
        return Parse(split_string) ? true : false;
    }

    const std::string& ArgParser::GetStringValue(const std::string& param, int index) { 
        int value_index = GetValueIndex(param, index);
        return (value_index != kIntMissingValue) ? split_string[value_index] : kStringMissingValue; 
    }
    int ArgParser::GetIntValue(const std::string& param, int index) {
        int value_index = GetValueIndex(param, index);
        return (value_index != kIntMissingValue) 
            ? std::strtol(split_string[value_index].c_str(), nullptr, 10) 
            : kIntMissingValue; 
    }
    bool ArgParser::GetFlag(const std::string& param) {
        return (GetFlagIndex(param) != kIntMissingValue) ? true : false;
    }

    ArgParser& ArgParser::AddIntArgument(const char param1, const std::string& param2, const std::string& description) {
        Add(int_commands, param1, param2, description);
        return *this;
    }
    ArgParser& ArgParser::AddIntArgument(const std::string& param2, const std::string& description) {
        Add(int_commands, param2, description);
        return *this;
    }

    ArgParser& ArgParser::AddStringArgument(const char param1, const std::string& param2, const std::string& description) {
        Add(string_commands, param1, param2, description);
        return *this;
    }
    ArgParser& ArgParser::AddStringArgument(const std::string& param2, const std::string& description) {
        Add(string_commands, param2, description);
        return *this;
    }

    ArgParser& ArgParser::AddFlag(const char param1, const std::string& param2, const std::string& description) {
        Add(flag_commands, param1, param2, description);
        return *this;
    }
    ArgParser& ArgParser::AddFlag(const std::string& param2, const std::string& description) {
        Add(flag_commands, param2, description);
        return *this;
    }

    ArgParser& ArgParser::AddHelp(const char param1, const std::string& param2, const std::string& description) {
        Add(help_commands, param1, param2, description);
        return *this;
    }
    ArgParser& ArgParser::AddHelp(const std::string& param2, const std::string& description) {
        Add(help_commands, param2, description);
        return *this;
    }

    bool ArgParser::Help() {
        return 0 == 0 ? true : false;
    }
    void ArgParser::HelpDescription() {

    }   

    ArgParser& ArgParser::Default(int value) {
        DefaultValue(int_commands, std::to_string(value));
        return *this;
    }
    ArgParser& ArgParser::Default(std::string value) {
        DefaultValue(int_commands, value);
        return *this;
    }
    ArgParser& ArgParser::Default(bool value) {
        DefaultValue(int_commands, std::to_string(value));
        return *this;
    }

    ArgParser& ArgParser::StoreValue(int& val) {    //CHECK
        store_hanlder.int_value = &val;
        return *this;
    }
    ArgParser& ArgParser::StoreValue(std::string& val) {    //CHECK
        store_hanlder.string_value = &val;
        return *this;
    }
    ArgParser& ArgParser::StoreValue(bool& val) {    //CHECK
        store_hanlder.bool_value = &val;
        return *this;
    }

    ArgParser& ArgParser::StoreValues(const std::vector<int64_t>& val) {    //CHECK
        store_hanlder.int_values.insert(store_hanlder.int_values.end(), val.begin(), val.end());
        return *this;
    }  
    ArgParser& ArgParser::StoreValues(const std::vector<std::string>& val) {    //CHECK
        store_hanlder.string_values.insert(store_hanlder.string_values.end(), val.begin(), val.end());
        return *this;
    } 
    ArgParser& ArgParser::StoreValues(const std::vector<bool>& val) {    //CHECK
        store_hanlder.bool_values.insert(store_hanlder.bool_values.end(), val.begin(), val.end());
        return *this;
    }  
}