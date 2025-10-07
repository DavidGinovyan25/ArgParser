#pragma once

#include <iostream>

#include <ArgumentParser.hpp>


namespace ArgumentParser {
class GetMethodsHandler {   //CHECK
protected:
    std::string Get(const std::string& param, int32_t index = 0) {
        for (size_t i = 0; i < split_string.size(); ++i) {
            if (param == split_string[i] || param == split_string[i])
                return split_string[i + index];
        }
        return "2";
    }
};

class AddMethodsHandler {   //CHECK
protected:
    void Add(const char param1, const std::string& param2, const std::string& description = "") {
        if (!CheckCommand(param1, param2))
            std::cout << "ERROR:" << std::endl;
        commands.push_back(Command{.param1 = param1, .param2 = param2, .description = description});
    }
    void Add(const std::string& param2, const std::string& description = "") {
        if (!CheckCommand(param2))
            std::cout << "ERROR:" << std::endl;
        commands.push_back(Command{.param2 = param2, .description = description});
    }
};

class ArgParser : private AddMethodsHandler, private GetMethodsHandler {
public:
    std::string name;
    ArgParser(std::string s) : name(s) {}
 
    bool Parse(std::vector<std::string> v); 
    bool Parse(int argc, char *argv[]);

    template <typename T>
    ArgParser& Default(T param);    /*default argument*/
     
    // template <typename T>
    // ArgParser& StoreValue(T value);

    // template <typename T>
    // ArgParser& StoreValues(std::vector<T> values);

    ArgParser& MultiValue(int32_t min_args_count = 0);  /*some values*/ 

    const std::string& GetStringValue(const std::string& param, int32_t index = 0);

    int32_t GetIntValue(const std::string& param, int32_t index = 0);

    bool GetFlag(const std::string& param);

    ArgParser& AddIntArgument(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddIntArgument(const std::string& param2, const std::string& description = "");

    ArgParser& AddStringArgument(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddStringArgument(const std::string& param2, const std::string& description = "");

    ArgParser& AddFlag(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddFlag(const std::string& param2, const std::string& description = "");

    ArgParser& AddHelp(const char param1, const std::string& param2, const std::string& description = "");
    ArgParser& AddHelp(const std::string& param2, const std::string& description = "");

    bool Help();
    // void HelpDescription();
    ArgParser& Positional();
};
}




