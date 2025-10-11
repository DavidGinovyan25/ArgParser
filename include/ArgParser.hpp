#pragma once

#include <iostream>

#include <ArgumentParser.hpp>

/*
1 - fix ArgParser and ArgumentParser classes
2 - write StoreValue(s) and Default interface
3 - check ADD/GET methods
*/

/*
1 - write all methods
*/

namespace ArgumentParser {
class GetMethodsHandler {   //CHECK
protected:
    const int kIndexShift = 1;
    const int kIntMissingValue = -1;
    const std::string& kStringMissingValue = "skibidi";
    
    int Get(const std::string& param, int index = 0);
    int GetFlagValue(const std::string& param);
};

class AddMethodsHandler {   //CHECK
protected:
    void Add(const char param1, const std::string& param2, const std::string& description = "");
    void Add(const std::string& param2, const std::string& description = "");
};

class ArgParser : private AddMethodsHandler, private GetMethodsHandler {
public:
    std::string name;
    ArgParser() = default;
    ArgParser(std::string s) : name(std::move(s)) {}
 
    bool Parse(std::vector<std::string> v); //check key on correct format 
    bool Parse(int argc, char *argv[]); //check key on correct format

    bool Help();
    void HelpDescription();

    // template <typename T> ArgParser& Default(T param);    /*default argument*/

    template <typename T> ArgParser& StoreValue(T& val);
    template <typename T> ArgParser& StoreValues(const std::vector<T>& val);    
    
    const std::string& GetStringValue(const std::string& param, int index = 0);
    int GetIntValue(const std::string& param, int index = 0);
    bool GetFlag(const std::string& param);

    ArgParser& MultiValue(int min_args_count = 0);
    ArgParser& Positional();

    ArgParser& AddIntArgument(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddIntArgument(const std::string& param2, const std::string& description = "");

    ArgParser& AddStringArgument(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddStringArgument(const std::string& param2, const std::string& description = "");

    ArgParser& AddFlag(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddFlag(const std::string& param2, const std::string& description = "");

    ArgParser& AddHelp(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddHelp(const std::string& param2, const std::string& description = "");
};
}

namespace ArgumentParser {  
    inline int GetMethodsHandler::GetFlagValue(const std::string& param) { //CHECK
        for (int i = 0; i < split_string.size(); ++i) {
            if (param == split_string[i])
                return i;
        } 
        return kIntMissingValue;
    }

    inline int GetMethodsHandler::Get(const std::string& param, int index) { //CHECK
        int index_flag = GetFlagValue(param);
        return (index_flag != kIntMissingValue) ? index_flag + index + kIndexShift : index_flag;
    }

    inline void AddMethodsHandler::Add(const char param1, const std::string& param2, const std::string& description) {  //CHECK
        if (CheckCommand(param1, param2))
            commands.push_back(Command{.param1 = param1, .param2 = param2, .description = description});    
    }
    inline void AddMethodsHandler::Add(const std::string& param2, const std::string& description) { //CHECK
        if (CheckCommand(param2))
            commands.push_back(Command{.param2 = param2, .description = description});
    }

    // template <typename T>
    // ArgParser& ArgParser::Default(T value) {

    //     return *this;
    // }

    template <typename T> ArgParser& ArgParser::StoreValue(T& val) {    //CHECK
        value<T> = &val;
        return *this;
    }
    template <typename T> ArgParser& ArgParser::StoreValues(const std::vector<T>& val) {    //CHECK
        values<T>.insert(values<T>.end(), val.begin(), val.end());
        return *this;
    }   
}