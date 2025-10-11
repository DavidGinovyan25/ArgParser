#include <cstring>

#include <ArgParser.hpp>

namespace ArgumentParser {
    ArgParser& ArgParser::MultiValue(int min_args_count) {  
        return *this; 
    }

    bool ArgParser::Parse(std::vector<std::string> v) { //TAKE INTO ACCOUNT POSITIONAL, MULTIVALUE, DEFAULT 
        for (int i = 1; i < v.size(); ++i) {
            /*
            1 - check program name
            2 - parse short and long flags with their values
            3 - parse 
            */
        }
        return true;
    }
    bool ArgParser::Parse(int argc, char *argv[]) {
        for (int i = 0; i < argc; ++i) {
            split_string.push_back(argv[i]);
        }
        if (!Parse(split_string))
            return false ;
        return true;
    }

    const std::string& ArgParser::GetStringValue(const std::string& param, int index) { 
        int value_index = Get(param, index);
        return (value_index != kIntMissingValue) ? split_string[value_index] : kStringMissingValue; 
    }
    int ArgParser::GetIntValue(const std::string& param, int index) {
        int value_index = Get(param, index);
        return (value_index != kIntMissingValue) 
            ? std::strtol(split_string[value_index].c_str(), nullptr, 10) 
            : kIntMissingValue; 
    }
    bool ArgParser::GetFlag(const std::string& param) {
        return (GetFlagValue(param) != kIntMissingValue) ? true : false;
    }

    ArgParser& ArgParser::AddIntArgument(const char param1, const std::string& param2, const std::string& description) {
        Add(param1, param2, description);
        return *this;
    }
    ArgParser& ArgParser::AddIntArgument(const std::string& param2, const std::string& description) {
        Add(param2, description);
        return *this;
    }

    ArgParser& ArgParser::AddStringArgument(const char param1, const std::string& param2, const std::string& description) {
        Add(param1, param2, description);
        return *this;
    }
    ArgParser& ArgParser::AddStringArgument(const std::string& param2, const std::string& description) {
        Add(param2, description);
        return *this;
    }

    ArgParser& ArgParser::AddFlag(const char param1, const std::string& param2, const std::string& description) {
        Add(param1, param2, description);
        return *this;
    }
    ArgParser& ArgParser::AddFlag(const std::string& param2, const std::string& description) {
        Add(param2, description);
        return *this;
    }

    ArgParser& ArgParser::AddHelp(const char param1, const std::string& param2, const std::string& description) {
        Add(param1, param2, description);
        return *this;
    }
    ArgParser& ArgParser::AddHelp(const std::string& param2, const std::string& description) {
        Add(param2, description);
        return *this;
    }

    bool ArgParser::Help() {
        if (0) 
            return false;
        return true;
    }
    void ArgParser::HelpDescription() {

    }

    ArgParser& ArgParser::Positional() {
        return *this;
    }       
}