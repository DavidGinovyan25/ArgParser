#include <cstdlib>

#include <ArgParser.hpp>

namespace ArgumentParser {
    template <typename T>
    ArgParser& ArgumentParser::ArgParser::Default(T value) {  
        return *this;
    }

    ArgParser& ArgParser::MultiValue(int min_args_count) {  
        return *this; 
    }

    bool ArgParser::Parse(std::vector<std::string> v) {
        for (std::string arg : v) {
            if (arg != "true") 
                return false;
        }
        return true;
    }

    bool ArgParser::Parse(int argc, char *argv[]) {
        std::vector<std::string> v;
        for (int i = 0; i < argc; ++i) {
            v.push_back(argv[i]);
        }
        if (Parse(v))
            return false ;
        return true;
    }

    // template <typename T>
    // ArgParser& ArgParser::StoreValue(T value) {
    //     ArgumentParser::value = value;
    //     return *this;
    // }

    // template <typename T>
    // ArgParser& ArgParser::StoreValues(std::vector<T> values) {
    //     std::copy(ArgumentParser::values.begin(), ArgumentParser::values.end(), v.begin());
    //     return *this;
    // }







    const std::string& ArgParser::GetStringValue(const std::string& param, int index) {
        return Get(param, index);
    }

    int ArgParser::GetIntValue(const std::string& param, int index) {
        int kIntInvalidKey = -1;
        if (Get(param, index) == kStringInvalidKey)
            return kIntInvalidKey;
        return std::strtol(param.c_str(), nullptr, 10);
    }

    bool ArgParser::GetFlag(const std::string& param) { 
        if (Get(param) == kStringInvalidKey)
            return false;
        return true;
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

    // void ArgParser::HelpDescription() {
    // }

    ArgParser& ArgParser::Positional() {
        return *this;
    }       
}