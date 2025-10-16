#include <iostream>

#include "ArgumentParser.hpp"

/*
1 - MAYBE ADD CONTEINERS FOR EACH TYPE
2 - WRITE MULTIPLE AND POSITIONAL FUNCION
3 - WRITE PARSE FUNCTION 
*/

/*
1 - write all methods
*/

namespace ArgumentParser {
class DefaultMethods {
public:
    void DefaultValue(CommandsContainer& commands, const std::string& value); 
};

class StoreHandler {
public:
    int* int_value = nullptr;
    std::string* string_value = nullptr;
    bool* bool_value = nullptr;
    std::vector<int> int_values;
    std::vector<std::string> string_values;
    std::vector<bool> bool_values;
};

class GetMethodsHandler {   //CHECK
protected:
    const int kIndexShift = 1;
    const int kIntMissingValue = -1;
    const std::string& kStringMissingValue = "skibidi";

    int GetValueIndex(const std::string& param, int index = 0);
    int GetFlagIndex(const std::string& param);
};

class AddMethodsHandler {   //CHECK
protected:
    void Add(CommandsContainer& commands, const char param1, const std::string& param2, const std::string& description = "");
    void Add(CommandsContainer& commands, const std::string& param2, const std::string& description = "");
};

class ArgParser : private AddMethodsHandler, private GetMethodsHandler, private DefaultMethods, private StoreHandler {
public:
    std::string name;
    StoreHandler store_hanlder;
    ArgParser() = default;
    ArgParser(std::string s) : name(std::move(s)) {}
 
    bool Parse(int argc, char *argv[]); //check key on correct format
    bool Parse(std::vector<std::string> v); //check key on correct format 

    bool Help();
    void HelpDescription();

    ArgParser& Default(int value);
    ArgParser& Default(std::string value);
    ArgParser& Default(bool value);
       
    ArgParser& StoreValue(int& val);
    ArgParser& StoreValue(std::string& val);
    ArgParser& StoreValue(bool& val);

    ArgParser& StoreValues(const std::vector<int64_t>& val);
    ArgParser& StoreValues(const std::vector<std::string>& val);
    ArgParser& StoreValues(const std::vector<bool>& val);

    bool GetFlag(const std::string& param);
    int GetIntValue(const std::string& param, int index = 0);
    const std::string& GetStringValue(const std::string& param, int index = 0);

    ArgParser& MultiValue(int min_args_count = 2);
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
    inline int GetMethodsHandler::GetFlagIndex(const std::string& param) { //CHECK
        for (int i = 0; i < split_string.size(); ++i) {
            if (param == split_string[i])
                return i;
        } 
        return kIntMissingValue;
    }
    inline int GetMethodsHandler::GetValueIndex(const std::string& param, int index) { //CHECK
        int index_flag = GetFlagIndex(param);
        return (index_flag != kIntMissingValue) ? index_flag + index + kIndexShift : index_flag;
    }

    inline void AddMethodsHandler::Add(
            CommandsContainer& commands, 
            const char param1, 
            const std::string& param2, 
            const std::string& description) {  //CHECK
        if (CheckCommand(param1, param2))
            commands.push_back(Commands{.param1 = param1, .param2 = param2, .description = description});    
    }
    inline void AddMethodsHandler::Add(
            CommandsContainer& commands, 
            const std::string& param2, 
            const std::string& description) { //CHECK
        if (CheckCommand(param2))
            commands.push_back(Commands{.param2 = param2, .description = description});
    }

    inline void DefaultMethods::DefaultValue(CommandsContainer& commands, const std::string& value) {
        commands[commands.size() - 1].param2 += ("=" + value);
    }
}