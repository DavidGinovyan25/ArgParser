#include <iostream>
#include <functional>
#include <optional>

#include "ArgumentParser.hpp"

namespace ArgumentParser {
class StorageHandler {
public:
    void operator()(std::reference_wrapper<int> stored_ref, const std::vector<std::string>& args) {
        if (!args.empty())
            stored_ref.get() = std::strtol(args[0].c_str(), nullptr, 10);
    }
    void operator()(std::reference_wrapper<std::string> stored_ref, const std::vector<std::string>& args) {
        if (!args.empty()) 
            stored_ref.get() = args[0];
    }
    void operator()(std::reference_wrapper<bool> stored_ref, const std::vector<std::string>& args) {
        stored_ref.get() = true;
    }
    void operator()(std::reference_wrapper<std::vector<int64_t>> stored_ref, const std::vector<std::string>& args) {
        for (const auto& arg : args) {
            stored_ref.get().push_back(std::strtoll(arg.c_str(), nullptr, 10));
        }
    }
    void operator()(std::reference_wrapper<std::vector<std::string>> stored_ref, const std::vector<std::string>& args) {
        stored_ref.get() = args;
    }
    void operator()(std::reference_wrapper<std::vector<bool>> stored_ref, const std::vector<std::string>& args) {
        for (const auto& arg : args) {
            stored_ref.get().push_back(true);
        }
    }
};    

class ArgParser : public CommandManager {
public:
    std::vector<std::string> split_string;
    StorageHandler storage_handler;
    std::string program_name;
public:
    std::string name;
    ArgParser(std::string s) : name(std::move(s)) {}

    bool Parse(int argc, char *argv[]);
    bool Parse(std::vector<std::string> v);

    bool Help();
    std::string HelpDescription();

    bool GetFlag(const std::string& param);
    std::optional<int> GetIntValue(const std::string& param, int index = 0);
    std::optional<std::string> GetStringValue(const std::string& param, int index = 0);

    ArgParser& MultiValue(int min_args_count = 0);
    ArgParser& Positional();

    template<typename T> 
    ArgParser& Default(T value);
        
    ArgParser& StoreValue(int& value);
    ArgParser& StoreValue(std::string& value);
    ArgParser& StoreValue(bool& value);
    
    ArgParser& StoreValues(std::vector<int64_t>& values);
    ArgParser& StoreValues(std::vector<std::string>& values);
    ArgParser& StoreValues(std::vector<bool>& values);

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
template <typename T> inline ArgParser& ArgParser::Default(T value) { 
    current_cmd->args.push_back(std::to_string(value));
    commands_list.back().args.push_back(std::to_string(value));
    return *this;
}
template<> inline ArgParser& ArgParser::Default<const char*>(const char* value) { 
    std::string val = value;
    current_cmd->args.push_back(value);
    commands_list.back().args.push_back(value);
    return *this;
}
}