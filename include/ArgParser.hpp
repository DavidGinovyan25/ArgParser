#include <iostream>
#include <functional>
#include <optional>
#include <variant>

#include "ArgumentParser.hpp"

namespace ArgumentParser {
class DefaultMethodsHandler {
public:
    void DefaultValue(CommandsContainer& commands, const std::string& value); 
};

class StoreHandler {    //THINK ABOUT ARCH - REMAKE
public:
    std::optional<std::reference_wrapper<int>> int_value_ref;
    std::optional<std::reference_wrapper<std::string>> string_value_ref;
    std::optional<std::reference_wrapper<bool>> bool_value_ref;
    std::vector<int64_t> int_values;
    std::vector<std::string> string_values;
    std::vector<bool> bool_values;

    template <typename T> void StoreValueMethod(std::optional<std::reference_wrapper<T>>& ref, T& val);
    template <typename T> void StoreValuesMethod(std::vector<T>& t_values, const std::vector<T>& val);
};

class GetMethodsHandler {   //THINK ABOUT ARCH
public:  
    CommandHandler command_handler;
    static constexpr int kIndexShift = 1;
    static constexpr int kIntMissingValue = -1;
    static constexpr const char* kStringMissingValue = "[MISSING VALUE]";
    std::vector<std::string> split_string;

    int GetValueIndex(const std::string& param, int index = 0);
    int GetFlagIndex(const std::string& param);
};

class AddMethodsHandler {
public:
    void Add(CommandsContainer& commands, const char param1, const std::string& param2, const std::string& description = "");
    void Add(CommandsContainer& commands, const std::string& param2, const std::string& description = "");
};

class ArgParser : public AddMethodsHandler, private GetMethodsHandler, private DefaultMethodsHandler {
private:
    StoreHandler store_handler;
public:
    std::string name;
    ArgParser() = default;
    ArgParser(std::string s) : name(std::move(s)) {}

    bool Parse(int argc, char *argv[]); //check key on correct format
    bool Parse(std::vector<std::string> v); //check key on correct format 

    bool Help();
    void HelpDescription();

    bool GetFlag(const std::string& param);
    int GetIntValue(const std::string& param, int index = 0);
    std::string GetStringValue(const std::string& param, int index = 0);

    ArgParser& MultiValue(int min_args_count = 2);
    ArgParser& Positional();

    ArgParser& Default(int value);
    ArgParser& Default(std::string value);
    ArgParser& Default(bool value);
        
    ArgParser& StoreValue(int& val);
    ArgParser& StoreValue(std::string& val);
    ArgParser& StoreValue(bool& val);

    ArgParser& StoreValues(const std::vector<int64_t>& val);
    ArgParser& StoreValues(const std::vector<std::string>& val);
    ArgParser& StoreValues(const std::vector<bool>& val);

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
inline int GetMethodsHandler::GetFlagIndex(const std::string& param) {
    for (int i = 0; i < split_string.size(); ++i) {
        if (param == split_string[i])
            return i;
    } 
    return kIntMissingValue;
}
inline int GetMethodsHandler::GetValueIndex(const std::string& param, int index) {
    int index_flag = GetFlagIndex(param);
    return (index_flag != kIntMissingValue) ? index_flag + index + kIndexShift : index_flag;
}

inline void AddMethodsHandler::Add(
        CommandsContainer& commands, 
        const char param1, 
        const std::string& param2, 
        const std::string& description) {
    //if (!command_handler.CheckCommand(param1, param2))
    commands.push_back(Command{.param1 = param1, .param2 = param2, .description = description});    
}
inline void AddMethodsHandler::Add(
        CommandsContainer& commands, 
        const std::string& param2, 
        const std::string& description) {
    //if (!command_handler.CheckCommand(param2))
        commands.push_back(Command{.param2 = param2, .description = description});
}

inline void DefaultMethodsHandler::DefaultValue(CommandsContainer& commands, const std::string& value) {
    commands[commands.size() - 1].param2 += ("=" + value);
}

template <typename T> 
inline void StoreHandler::StoreValueMethod(std::optional<std::reference_wrapper<T>>& ref, T& val) {
    static_assert(
            std::is_same_v<T, int> || 
            std::is_same_v<T, std::string> || 
            std::is_same_v<T, bool>,
            "StoreValueMethodError: can supporte only int, std::string, bool types");
    ref = std::ref(val);
}
template <typename T> 
inline void StoreHandler::StoreValuesMethod(std::vector<T>& t_values, const std::vector<T>& val) {
    t_values.insert(t_values.end(), val.begin(), val.end());
}
}