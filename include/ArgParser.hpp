#include <iostream>
#include <functional>
#include <optional>
#include <variant>

#include "ArgumentParser.hpp"

namespace ArgumentParser {
//class DefaultMethodsHandler {
// public:
//     void DefaultValue(CommandsContainer& commands, const std::string& value); 
// };

class StoreHandler {
public:
    template<typename T>
    using op_ref = std::optional<std::reference_wrapper<T>>;

    op_ref<int> int_value_ref;
    op_ref<std::string> string_value_ref;
    op_ref<bool> bool_value_ref;

    op_ref<std::vector<int64_t>> int_values_ref;
    op_ref<std::vector<std::string>> string_values_ref;
    op_ref<std::vector<bool>> bool_values_ref;

    template <typename T> void StoreValueMethod(op_ref<T>& type_value_ref, T& val);
    template <typename T> void StoreValuesMethod(op_ref<std::vector<T>>& type_values_ref, std::vector<T>& val);
};

class GetMethodsHandler : public virtual CommandHandler  {
public:  
    //static constexpr int kIndexShift = 1;
    static constexpr int kIntMissingValue = -1;
    static constexpr const char* kStringMissingValue = "[MISSING VALUE]";
    std::optional<std::string> GetFlagValue(const std::string& flag, int index = 0);
    // int GetValueIndex(const std::vector<std::string>& split_string, const std::string& param, int index = 0);
    // int GetFlagIndex(const std::vector<std::string>& split_string, const std::string& param);
};

class AddMethodsHandler : public virtual CommandHandler {
public:
    void Add(ContainerType type, const char param1, const std::string& param2, const std::string& description = "");
    void Add(ContainerType type, const std::string& param2, const std::string& description = "");
};

class ArgParser : public AddMethodsHandler {//, private DefaultMethodsHandler {
public:
    StoreHandler store_handler;
    GetMethodsHandler get_handler;
    std::vector<std::string> split_string;
public:
    std::string name;
    ArgParser() = default;
    ArgParser(std::string s) : name(std::move(s)) {}

    bool Parse(int argc, char *argv[]);
    bool Parse(std::vector<std::string> v);

    bool Help();
    void HelpDescription();

    bool GetFlag(const std::string& param);
    std::optional<int> GetIntValue(const std::string& param, int index = 0);
    std::optional<std::string> GetStringValue(const std::string& param, int index = 0);

    ArgParser& MultiValue(int min_args_count = 0);
    ArgParser& Positional();

    template<typename T> 
    ArgParser& Default(T value);
    // ArgParser& Default(int value);
    // ArgParser& Default(std::string value);
    // ArgParser& Default(bool value);
        
    ArgParser& StoreValue(int& val);
    ArgParser& StoreValue(std::string& val);
    ArgParser& StoreValue(bool& val);

    ArgParser& StoreValues(std::vector<int64_t>& val);
    ArgParser& StoreValues(std::vector<std::string>& val);
    ArgParser& StoreValues(std::vector<bool>& val);

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
// inline int GetMethodsHandler::GetFlagIndex(const std::vector<std::string>& split_string, const std::string& param) {
//     for (int i = 0; i < split_string.size(); ++i) {
//         if (param == split_string[i])
//             return i;
//     } 
//     return kIntMissingValue;
// }
// inline int GetMethodsHandler::GetValueIndex(
//         const std::vector<std::string>& split_string, 
//         const std::string& param, 
//         int index) {
//     int index_flag = GetFlagIndex(split_string, param);
//     return (index_flag != kIntMissingValue) ? index_flag + index + kIndexShift : index_flag;
// }

inline std::optional<std::string> GetMethodsHandler::GetFlagValue(const std::string& flag, int index) {
    for (int i = 0; i < commands.size(); ++i) {
        if (flag == commands[i].param2 && index < commands[i].args[index].size())
            return commands[i].args[index];
    }
    return std::nullopt;
}

inline void AddMethodsHandler::Add(
        ContainerType type, 
        const char param1, 
        const std::string& param2, 
        const std::string& description) {
    if (!CheckCommand(param1) ||
        !CheckCommand(param2))
        commands.push_back(Command{.type = type, .param1 = param1, .param2 = param2, .description = description});    
}
inline void AddMethodsHandler::Add(
        ContainerType type, 
        const std::string& param2, 
        const std::string& description) {
    if (!CheckCommand(param2))
        commands.push_back(Command{.type = type, .param2 = param2, .description = description});
    
}

// inline void DefaultMethodsHandler::DefaultValue(CommandsContainer& commands, const std::string& value) {
//     commands.back().param2 += ("=" + value);
// }


template <typename T> 
inline void StoreHandler::StoreValueMethod(op_ref<T>& type_value_ref, T& value) {
    static_assert(
            std::is_same_v<T, int> || 
            std::is_same_v<T, std::string> || 
            std::is_same_v<T, bool>,
            "StoreValueMethodError: can supporte only int, std::string, bool types");
    type_value_ref = std::ref(value);
}
template <typename T> 
inline void StoreHandler::StoreValuesMethod(op_ref<std::vector<T>>& type_values_ref, std::vector<T>& values) {
    static_assert(
            std::is_same_v<T, int64_t> || 
            std::is_same_v<T, std::string> || 
            std::is_same_v<T, bool>,
            "StoreValueMethodError: can supporte only int, std::string, bool types");
    type_values_ref = std::ref(values);
}

template <typename T> inline ArgParser& ArgParser::Default(T value) { 
    commands.back().param2 += ("=" + std::to_string(value));
    return *this;
}
template <> inline ArgParser& ArgParser::Default<std::string>(std::string value) { 
    commands.back().param2 += ("=" + value);
    return *this;
}
}