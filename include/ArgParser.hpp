#include <iostream>
#include <functional>
#include <optional>

#include "ArgumentParser.hpp"

namespace ArgumentParser {
class StoreHandler {
public:
    template<typename T>
    using opt_ref_wrap = std::optional<std::reference_wrapper<T>>;

    opt_ref_wrap<int> int_value_ref;
    opt_ref_wrap<std::string> string_value_ref;
    opt_ref_wrap<bool> bool_value_ref;

    opt_ref_wrap<std::vector<int64_t>> int_values_ref;
    opt_ref_wrap<std::vector<std::string>> string_values_ref;
    opt_ref_wrap<std::vector<bool>> bool_values_ref;

    template <typename T> void StoreValueMethod(opt_ref_wrap<T>& type_value_ref, T& val);
    template <typename T> void StoreValuesMethod(opt_ref_wrap<std::vector<T>>& type_values_ref, std::vector<T>& val);
};

class GetMethodsHandler : public virtual CommandHandler  {
public:  
    static constexpr int kIntMissingValue = -1;
    static constexpr const char* kStringMissingValue = "[MISSING VALUE]";
    std::optional<std::string> GetFlagValue(const std::string& flag, int index = 0);
};

class AddMethodsHandler : public virtual CommandHandler {
public:
    void Add(CommandType type, const char param1, const std::string& param2, const std::string& description = "");
    void Add(CommandType type, const std::string& param2, const std::string& description = "");
};

class ArgParser : public AddMethodsHandler, public GetMethodsHandler  {
public:
    StoreHandler store_handler;
    std::vector<std::string> split_string;
public:
    std::string name;
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
inline std::optional<std::string> GetMethodsHandler::GetFlagValue(const std::string& flag, int index) {
    for (int i = 0; i < commands.size(); ++i) {
        if (flag == commands[i].param2 && index < commands[i].args.size())
            return commands[i].args[index];
    }
    return std::nullopt;
}

inline void AddMethodsHandler::Add(
        CommandType type, 
        const char param1, 
        const std::string& param2, 
        const std::string& description) {
    if (!IsReservedCommand(std::to_string(param1)) && !IsReservedCommand(param2)) {
        commands.push_back(Command{.type = type, .param1 = param1, .param2 = param2, .description = description});
    }
}
inline void AddMethodsHandler::Add(
        CommandType type, 
        const std::string& param2, 
        const std::string& description) {
    if (!IsReservedCommand(param2)) {
        commands.push_back(Command{.type = type, .param2 = param2, .description = description});
    }
}

template <typename T> 
inline void StoreHandler::StoreValueMethod(opt_ref_wrap<T>& type_value_ref, T& value) {
    static_assert(
            std::is_same_v<T, int> || 
            std::is_same_v<T, std::string> || 
            std::is_same_v<T, bool>,
            "StoreValueMethodError: can supporte only int, std::string, bool types");
    type_value_ref = std::ref(value);
}
template <typename T> 
inline void StoreHandler::StoreValuesMethod(opt_ref_wrap<std::vector<T>>& type_values_ref, std::vector<T>& values) {
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
template<> inline ArgParser& ArgParser::Default<const char*>(const char* value) { 
    std::string val = value;
    commands.back().param2 += ("=" + val);
    return *this;
}
}